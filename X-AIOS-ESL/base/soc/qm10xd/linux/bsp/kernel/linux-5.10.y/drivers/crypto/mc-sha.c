// SPDX-License-Identifier: GPL-2.0
/*
 * Cryptographic API.
 *
 * Support for MC SHA1/SHA256 HW acceleration.
 *
 * Copyright (c) 2023 MOLCHIP
 * Author: Gilbert.Shi  <gilbert.shi@molchip.com>
 *
 * Some ideas are from omap-sham.c drivers.
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/hw_random.h>
#include <linux/platform_device.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/dma/mc_dma64.h>
#include <linux/device.h>
#include <linux/dmaengine.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/crypto.h>
#include <crypto/scatterwalk.h>
#include <crypto/algapi.h>
#include <crypto/sha.h>
#include <crypto/hash.h>
#include <crypto/internal/hash.h>
#include "mc-sha-regs.h"
#include <linux/xc-clock-simple/clock.h>


#define MC_SHA_PRIORITY	400

/* SHA flags */
#define SHA_FLAGS_BUSY			BIT(0)
#define	SHA_FLAGS_FINAL			BIT(1)
#define SHA_FLAGS_DMA_ACTIVE	BIT(2)
#define SHA_FLAGS_OUTPUT_READY	BIT(3)
#define SHA_FLAGS_INIT			BIT(4)
#define SHA_FLAGS_DMA_READY		BIT(6)

/* bits[11:8] are reserved. */

#define SHA_FLAGS_FINUP		BIT(16)
#define SHA_FLAGS_SG		BIT(17)
#define SHA_FLAGS_ERROR		BIT(23)
#define SHA_FLAGS_PAD		BIT(24)

#define SHA_OP_INIT	0
#define SHA_OP_UPDATE	1
#define SHA_OP_FINAL	2
#define SHA_OP_DIGEST	3
#define SHA_BUFFER_LEN		(PAGE_SIZE)
#define MC_SHA_DMA_THRESHOLD		0

struct mc_sha_caps {
	bool	has_dma;
	bool	has_dualbuff;
	bool	has_sha_384_512;
};

struct mc_sha_dev;

/*
 * .statesize = sizeof(struct mc_sha_reqctx) must be <= PAGE_SIZE / 8 as
 * tested by the ahash_prepare_alg() function.
 */
struct mc_sha_reqctx {
	struct mc_sha_dev	*dd;
	unsigned long	flags;
	unsigned long	op;

	u8	digest[SHA512_DIGEST_SIZE] __aligned(sizeof(u32));
	u64	digcnt[2];
	size_t	bufcnt;
	size_t	buflen;
	dma_addr_t	dma_addr;

	/* walk state */
	struct scatterlist	*sg;
	unsigned int	offset;	/* offset in current sg */
	unsigned int	total;	/* total request */

	size_t block_size;
	size_t hash_size;

	u8 *buffer;
};

typedef int (*mc_sha_fn_t)(struct mc_sha_dev *);

struct mc_sha_ctx {
	struct mc_sha_dev	*dd;
	mc_sha_fn_t		start;
	unsigned long		flags;
	u8 buf[SHA_BUFFER_LEN] __aligned(sizeof(u32));
};

#define MC_SHA_QUEUE_LENGTH	50

struct mc_sha_dma {
	struct dma_chan			*chan;
	struct dma_slave_config dma_conf;
	struct scatterlist	*sg;
	int			nents;
	unsigned int		last_sg_length;
};

struct mc_sha_dev {
	struct list_head	list;
	unsigned long		phys_base;
	struct device		*dev;
	int					max_freq;
	int					irq;
	void __iomem		*io_base;

	spinlock_t		lock;
	struct tasklet_struct	done_task;
	struct tasklet_struct	queue_task;

	unsigned long		flags;
	struct crypto_queue	queue;
	struct ahash_request	*req;
	bool			is_async;
	bool			force_complete;
	mc_sha_fn_t		resume;
	mc_sha_fn_t		cpu_transfer_complete;

	struct mc_sha_dma	dma_lch_in;
	int rx_dma_id;
	int flag_len;

	struct mc_sha_caps	caps;
	struct completion	dma_completion;

	struct scatterlist	tmp;

};

struct mc_sha_drv {
	struct list_head	dev_list;
	spinlock_t		lock;
};

 struct mc_sha_drv mc_sha = {
	.dev_list = LIST_HEAD_INIT(mc_sha.dev_list),
	.lock = __SPIN_LOCK_UNLOCKED(mc_sha.lock),
};

static inline u32 mc_sha_read(struct mc_sha_dev *dd, u32 offset)
{
	u32 value = readl_relaxed(dd->io_base + offset);

	return value;
}

static inline void mc_sha_write(struct mc_sha_dev *dd,
					u32 offset, u32 value)
{
	writel_relaxed(value, dd->io_base + offset);
}

static inline int mc_sha_complete(struct mc_sha_dev *dd, int err)
{
	struct ahash_request *req = dd->req;

	dd->flags &= ~(SHA_FLAGS_BUSY | SHA_FLAGS_FINAL |
		       SHA_FLAGS_DMA_READY | SHA_FLAGS_OUTPUT_READY);
	
	if ((dd->is_async || dd->force_complete) && req->base.complete)
			req->base.complete(&req->base, err);

	/* handle new request */
	tasklet_schedule(&dd->queue_task);

	return err;
}

static size_t mc_sha_append_sg(struct mc_sha_reqctx *ctx)
{
	size_t count;

	while ((ctx->bufcnt < ctx->buflen) && ctx->total) {
		count = min(ctx->sg->length - ctx->offset, ctx->total);
		count = min(count, ctx->buflen - ctx->bufcnt);

		if (count <= 0) {
			/*
			* Check if count <= 0 because the buffer is full or
			* because the sg length is 0. In the latest case,
			* check if there is another sg in the list, a 0 length
			* sg doesn't necessarily mean the end of the sg list.
			*/
			if ((ctx->sg->length == 0) && !sg_is_last(ctx->sg)) {
				ctx->sg = sg_next(ctx->sg);
				continue;
			} else {
				break;
			}
		}

		scatterwalk_map_and_copy(ctx->buffer + ctx->bufcnt, ctx->sg,
			ctx->offset, count, 0);

		ctx->bufcnt += count;
		ctx->offset += count;
		ctx->total -= count;

		if (ctx->offset == ctx->sg->length) {
			ctx->sg = sg_next(ctx->sg);
			if (ctx->sg)
				ctx->offset = 0;
			else
				ctx->total = 0;
		}
	}

	return 0;
}

/*
 * The purpose of this padding is to ensure that the padded message is a
 * multiple of 512 bits (SHA1/SHA256) or 1024 bits (SHA384/SHA512).
 * The bit "1" is appended at the end of the message followed by
 * "padlen-1" zero bits. Then a 64 bits block (SHA1/SHA256) or
 * 128 bits block (SHA384/SHA512) equals to the message length in bits
 * is appended.
 *
 * For SHA1/SHA256, padlen is calculated as followed:
 *  - if message length < 56 bytes then padlen = 56 - message length
 *  - else padlen = 64 + 56 - message length
 *
 * For SHA384/SHA512, padlen is calculated as followed:
 *  - if message length < 112 bytes then padlen = 112 - message length
 *  - else padlen = 128 + 112 - message length
 */
static void mc_sha_fill_padding(struct mc_sha_reqctx *ctx, int length)
{
	unsigned int index, padlen;
	__be64 bits[2];
	u64 size[2];

	size[0] = ctx->digcnt[0];
	size[1] = ctx->digcnt[1];

	size[0] += ctx->bufcnt;
	if (size[0] < ctx->bufcnt)
		size[1]++;

	size[0] += length;
	if (size[0]  < length)
		size[1]++;

	bits[1] = cpu_to_be64(size[0] << 3);
	bits[0] = cpu_to_be64(size[1] << 3 | size[0] >> 61);

	switch (ctx->flags & SHA_FLAGS_ALGO_MASK) {
	case SHA_FLAGS_SHA384:
	case SHA_FLAGS_SHA512:
		index = ctx->bufcnt & 0x7f;
		padlen = (index < 112) ? (112 - index) : ((128+112) - index);
		*(ctx->buffer + ctx->bufcnt) = 0x80;
		memset(ctx->buffer + ctx->bufcnt + 1, 0, padlen-1);
		memcpy(ctx->buffer + ctx->bufcnt + padlen, bits, 16);
		ctx->bufcnt += padlen + 16;
		ctx->flags |= SHA_FLAGS_PAD;
		break;

	default:
		index = ctx->bufcnt & 0x3f;
		padlen = (index < 56) ? (56 - index) : ((64+56) - index);
		*(ctx->buffer + ctx->bufcnt) = 0x80;
		memset(ctx->buffer + ctx->bufcnt + 1, 0, padlen-1);
		memcpy(ctx->buffer + ctx->bufcnt + padlen, &bits[1], 8);
		ctx->bufcnt += padlen + 8;
		ctx->flags |= SHA_FLAGS_PAD;
		break;
	}
}

static struct mc_sha_dev *mc_sha_find_dev(struct mc_sha_ctx *tctx)
{
	struct mc_sha_dev *dd = NULL;
	struct mc_sha_dev *tmp;

	spin_lock_bh(&mc_sha.lock);
	if (!tctx->dd) {
		list_for_each_entry(tmp, &mc_sha.dev_list, list) {
			dd = tmp;
			break;
		}
		tctx->dd = dd;
	} else {
		dd = tctx->dd;
	}

	spin_unlock_bh(&mc_sha.lock);

	return dd;
}

static int mc_sha_init(struct ahash_request *req)
{
	unsigned int reg_val=0;
	struct crypto_ahash *tfm = crypto_ahash_reqtfm(req);
	struct mc_sha_ctx *tctx = crypto_ahash_ctx(tfm);
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	struct mc_sha_dev *dd = mc_sha_find_dev(tctx);
	ctx->dd = dd;
	ctx->flags = 0;
	switch (crypto_ahash_digestsize(tfm)) {
	case SHA1_DIGEST_SIZE:
		ctx->flags |= SHA_FLAGS_SHA1;
		ctx->block_size = SHA1_BLOCK_SIZE;
		dd->flag_len =0x40;
		break;
	case SHA256_DIGEST_SIZE:
		ctx->flags |= SHA_FLAGS_SHA256;
		ctx->block_size = SHA256_BLOCK_SIZE;
		dd->flag_len =0x40;
		break;
	case SHA384_DIGEST_SIZE:
		ctx->flags |= SHA_FLAGS_SHA384;
		ctx->block_size = SHA384_BLOCK_SIZE;
		dd->flag_len =0x80;
		break;
	case SHA512_DIGEST_SIZE:
		ctx->flags |= SHA_FLAGS_SHA512;
		ctx->block_size = SHA512_BLOCK_SIZE;
		dd->flag_len =0x80;
		break;
	default:
		return -EINVAL;
		break;
	}

	reg_val = mc_sha_read(dd,HASH_CFG);
	reg_val = (reg_val & (~SHA_MR_ALGO_MASK));
	reg_val |=ctx->flags;
	mc_sha_write(dd, HASH_CFG,reg_val);
	reg_val = mc_sha_read(dd,HASH_CFG);

	ctx->bufcnt = 0;
	ctx->digcnt[0] = 0;
	ctx->digcnt[1] = 0;
	ctx->buffer = tctx->buf;
	ctx->buflen = SHA_BUFFER_LEN;
	
	//mc_sha_write(dd, HASH_MSG_LEN_CFG0, req->nbytes);
	return 0;
}

static inline int mc_sha_wait_for_data_ready(struct mc_sha_dev *dd,
						mc_sha_fn_t resume)
{
	u32 isr = mc_sha_read(dd, HASH_INT);
	if (unlikely(isr & INIT_STATUS))
		return resume(dd);
	dd->resume = resume;
	return -EINPROGRESS;
}

static int mc_hash_dma_wait(struct  mc_sha_dev *dd, unsigned int len)
 {
	 unsigned long long ms;
 
	 ms = 2 * len * MSEC_PER_SEC * BITS_PER_BYTE;
	 ms += ms + 200;
 
	 if (ms > UINT_MAX)
		 ms = UINT_MAX;
 
	 ms = wait_for_completion_timeout(&dd->dma_completion,
					  msecs_to_jiffies(ms));
 
	 if (ms == 0)
		 return -ETIMEDOUT;
	
	 dmaengine_terminate_sync(dd->dma_lch_in.chan);
	 dma_release_channel(dd->dma_lch_in.chan);
	 dd->dma_lch_in.chan = NULL;
	 return 0;
 }
 
static int mc_sha_dma_chn_req(struct mc_sha_dev *dd)
 {
	 dma_cap_mask_t  mask_rx;
	 
	 dma_cap_zero(mask_rx);
	 dma_cap_set(DMA_SLAVE, mask_rx);
 
	 if (!dd->dma_lch_in.chan) {
		 dd->dma_lch_in.chan = dma_request_channel(mask_rx,NULL,NULL);
		 if (!dd->dma_lch_in.chan) {
			 dev_err(dd->dev,"failed to get hash rx dma channel\n");
			 return PTR_ERR(dd->dma_lch_in.chan);;
		 }
	 }
 
	 return 0;
 }

 static int mc_sha_xmit_dma(struct mc_sha_dev *dd, dma_addr_t dma_addr1,
		size_t length1,  dma_addr_t dma_addr2, size_t length2, int final)
{
	
	 struct dma_async_tx_descriptor *desc;
	 int err,reg_val,ret;
	 struct scatterlist sg[2];
	 
	 struct mc_sha_reqctx *ctx = ahash_request_ctx(dd->req);
	 
	 err = mc_sha_dma_chn_req(dd);
	 if(err != 0)
	 	 return PTR_ERR(dd->dma_lch_in.chan); 
	 
	 struct dma_slave_config config = {
		 .direction = DMA_MEM_TO_DEV,
		 .src_addr =  dma_addr1,
		 .dst_addr = dd->phys_base + HASH_FIFO,
		 .dst_maxburst = dd->flag_len,
		 .src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES,
		 .src_maxburst = dd->flag_len,
		 .dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES,
		 .device_fc = false,
		 .slave_id = dd->rx_dma_id,
	 };

	err = dmaengine_slave_config(dd->dma_lch_in.chan, &config);
	if (err)
	 	 printk("dmaengine_slave_config fail \r\n");

	if (length2) {
		sg_init_table(sg, 2);
		sg_dma_address(&sg[0]) = dma_addr1;
		sg_dma_len(&sg[0]) = length1;
		sg_dma_address(&sg[1]) = dma_addr2;
		sg_dma_len(&sg[1]) = length2;
		desc = dmaengine_prep_slave_sg(dd->dma_lch_in.chan, sg, 2,
								DMA_MEM_TO_DEV, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	} else {
		sg_init_table(sg, 1);
		sg_dma_address(&sg[0]) = dma_addr1;
		sg_dma_len(&sg[0]) = length1;
		desc = dmaengine_prep_slave_sg(dd->dma_lch_in.chan, sg, 1,
								DMA_MEM_TO_DEV, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	}

	desc->callback = NULL;
	desc->callback_param = NULL;

	/* should be non-zero before next lines to disable clocks later */
	ctx->digcnt[0] += length1;
	if (ctx->digcnt[0] < length1)
		ctx->digcnt[1]++;

	if (final)
		dd->flags |= SHA_FLAGS_FINAL; /* catch last interrupt */

	dd->flags |=  SHA_FLAGS_DMA_ACTIVE;

	/* Start DMA transfer */
	dmaengine_submit(desc);
	dma_async_issue_pending(dd->dma_lch_in.chan);

	//start sha
	reg_val = mc_sha_read(dd,HASH_CFG);
	reg_val |= HASH_START;
	mc_sha_write(dd, HASH_CFG, reg_val);
	
	ret = mc_hash_dma_wait(dd,dd->req->nbytes);
	if(ret != 0 )
		printk("DMA wait fail \r\n");

	return -EINPROGRESS;
}

static int mc_sha_xmit_start(struct mc_sha_dev *dd, dma_addr_t dma_addr1,
		size_t length1, dma_addr_t dma_addr2, size_t length2,int final)
{
		return mc_sha_xmit_dma(dd, dma_addr1, length1, dma_addr2,  length2, final);
}

static int mc_sha_xmit_dma_map(struct mc_sha_dev *dd,
					struct mc_sha_reqctx *ctx,
					size_t length, int final)
{
	ctx->dma_addr = dma_map_single(dd->dev, ctx->buffer,
				ctx->buflen + ctx->block_size, DMA_TO_DEVICE);
	if (dma_mapping_error(dd->dev, ctx->dma_addr)) {
		return mc_sha_complete(dd, -EINVAL);
	}

	ctx->flags &= ~SHA_FLAGS_SG;

	/* next call does not fail... so no unmap in the case of error */
	return mc_sha_xmit_start(dd, ctx->dma_addr, length, 0,0,final);
}

static int mc_sha_update_dma_slow(struct mc_sha_dev *dd)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(dd->req);
	unsigned int final;
	size_t count;

	mc_sha_append_sg(ctx);

	final = (ctx->flags & SHA_FLAGS_FINUP) && !ctx->total;

	if (final)
		mc_sha_fill_padding(ctx, 0);

	if (final || (ctx->bufcnt == ctx->buflen)) {
		count = ctx->bufcnt;
		ctx->bufcnt = 0;
		return mc_sha_xmit_dma_map(dd, ctx, count, final);
	}

	return 0;
}

static int mc_sha_update_dma_start(struct mc_sha_dev *dd)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(dd->req);
	unsigned int length, final, tail;
	struct scatterlist *sg;
	unsigned int count;

	if (!ctx->total)
		return 0;

	if (ctx->bufcnt || ctx->offset)
		return mc_sha_update_dma_slow(dd);

	sg = ctx->sg;

	if (!IS_ALIGNED(sg->offset, sizeof(u32)))
		return mc_sha_update_dma_slow(dd);

	if (!sg_is_last(sg) && !IS_ALIGNED(sg->length, ctx->block_size))
		/* size is not ctx->block_size aligned */
		return mc_sha_update_dma_slow(dd);

	length = min(ctx->total, sg->length);

	if (sg_is_last(sg)) {
		if (!(ctx->flags & SHA_FLAGS_FINUP)) {
			/* not last sg must be ctx->block_size aligned */
			tail = length & (ctx->block_size - 1);
			length -= tail;
		}
	}

	ctx->total -= length;
	ctx->offset = length; /* offset where to start slow */

	final = (ctx->flags & SHA_FLAGS_FINUP) && !ctx->total;

	/* Add padding */
	if (final) {
		
		tail = length & (ctx->block_size - 1);
		length -= tail;
		ctx->total += tail;
		ctx->offset = length; /* offset where to start slow */

		sg = ctx->sg;
		mc_sha_append_sg(ctx);
		mc_sha_fill_padding(ctx, length);

		ctx->dma_addr = dma_map_single(dd->dev, ctx->buffer,
			ctx->buflen + ctx->block_size, DMA_TO_DEVICE);
		if (dma_mapping_error(dd->dev, ctx->dma_addr)) {
	
			dev_err(dd->dev, "dma %zu bytes error\n",
				ctx->buflen + ctx->block_size);
			return mc_sha_complete(dd, -EINVAL);
		}

		if (length == 0) {
			ctx->flags &= ~SHA_FLAGS_SG;
			count = ctx->bufcnt;
			ctx->bufcnt = 0;
			return mc_sha_xmit_start(dd, ctx->dma_addr, count,0,0, final);
		} else {
			ctx->sg = sg;
			if (!dma_map_sg(dd->dev, ctx->sg, 1,
				DMA_TO_DEVICE)) {
					dev_err(dd->dev, "dma_map_sg  error\n");
					return mc_sha_complete(dd, -EINVAL);
			}

			ctx->flags |= SHA_FLAGS_SG;

			count = ctx->bufcnt;
			ctx->bufcnt = 0;
			return mc_sha_xmit_start(dd, sg_dma_address(ctx->sg),
					length, ctx->dma_addr, count, final);
		}
	}

	if (!dma_map_sg(dd->dev, ctx->sg, 1, DMA_TO_DEVICE)) {
		dev_err(dd->dev, "dma_map_sg  error\n");
		return mc_sha_complete(dd, -EINVAL);
	}

	ctx->flags |= SHA_FLAGS_SG;

	/* next call does not fail... so no unmap in the case of error */
	return mc_sha_xmit_start(dd, sg_dma_address(ctx->sg), length,0,0, final);
}

static void mc_sha_update_dma_stop(struct mc_sha_dev *dd)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(dd->req);
	if (ctx->flags & SHA_FLAGS_SG) {
		dma_unmap_sg(dd->dev, ctx->sg, 1, DMA_TO_DEVICE);
		if (ctx->sg->length == ctx->offset) {
			
			ctx->sg = sg_next(ctx->sg);
			if (ctx->sg)
				ctx->offset = 0;
		}
		if (ctx->flags & SHA_FLAGS_PAD) {
			dma_unmap_single(dd->dev, ctx->dma_addr,
				ctx->buflen + ctx->block_size, DMA_TO_DEVICE);
		}
	} else {
		dma_unmap_single(dd->dev, ctx->dma_addr, ctx->buflen +
						ctx->block_size, DMA_TO_DEVICE);
	}
}

static int mc_sha_update_req(struct mc_sha_dev *dd)
{
	struct ahash_request *req = dd->req;
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	int err;

	err = mc_sha_update_dma_start(dd);

	/* wait for dma completion before can take more data */
	dev_dbg(dd->dev, "update: err: %d, digcnt: 0x%llx 0%llx\n",
			err, ctx->digcnt[1], ctx->digcnt[0]);

	return err;
}

static int mc_sha_final_req(struct mc_sha_dev *dd)
{
	struct ahash_request *req = dd->req;
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	int err = 0;
	int count;

	if (ctx->bufcnt >= MC_SHA_DMA_THRESHOLD) {
		mc_sha_fill_padding(ctx, 0);
		count = ctx->bufcnt;
		ctx->bufcnt = 0;
		err = mc_sha_xmit_dma_map(dd, ctx, count, 1);
	}

	return err;
}

static void mc_sha_copy_hash(struct ahash_request *req)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	u32 *hash = (u32 *)ctx->digest;
	unsigned int i, hashsize;

	switch (ctx->flags & SHA_FLAGS_ALGO_MASK) {
	case SHA_FLAGS_SHA1:
		hashsize = SHA1_DIGEST_SIZE;
		break;

	case SHA_FLAGS_SHA256:
		hashsize = SHA256_DIGEST_SIZE;
		break;

	case SHA_FLAGS_SHA384:
	case SHA_FLAGS_SHA512:
		hashsize = SHA512_DIGEST_SIZE;
		break;

	default:
		/* Should not happen... */
		return;
	}

	for (i = 0; i < hashsize / sizeof(u32); ++i)
		hash[i] = cpu_to_be32(mc_sha_read(ctx->dd, HASH(i)));
}

static void mc_sha_copy_ready_hash(struct ahash_request *req)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);

	if (!req->result)
		return;

	switch (ctx->flags & SHA_FLAGS_ALGO_MASK) {
	default:
	case SHA_FLAGS_SHA1:
		memcpy(req->result, ctx->digest, SHA1_DIGEST_SIZE);
		break;
	case SHA_FLAGS_SHA256:
		memcpy(req->result, ctx->digest, SHA256_DIGEST_SIZE);
		break;

	case SHA_FLAGS_SHA384:
		memcpy(req->result, ctx->digest, SHA384_DIGEST_SIZE);
		break;

	case SHA_FLAGS_SHA512:
		memcpy(req->result, ctx->digest, SHA512_DIGEST_SIZE);
		break;
	}
}

static int mc_sha_finish(struct ahash_request *req)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	struct mc_sha_dev *dd = ctx->dd;

	if (ctx->digcnt[0] || ctx->digcnt[1])
		mc_sha_copy_ready_hash(req);

	dev_dbg(dd->dev, "digcnt: 0x%llx 0x%llx, bufcnt: %zd\n", ctx->digcnt[1],
		ctx->digcnt[0], ctx->bufcnt);

	return 0;
}

static void mc_sha_finish_req(struct ahash_request *req, int err)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	struct mc_sha_dev *dd = ctx->dd;

	if (!err) {
		mc_sha_copy_hash(req);
		if (SHA_FLAGS_FINAL & dd->flags)
			err = mc_sha_finish(req);
	} else {
		ctx->flags |= SHA_FLAGS_ERROR;
	}

	/* atomic operation is not needed here */
	(void)mc_sha_complete(dd, err);
}

static int mc_sha_hw_init(struct mc_sha_dev *dd)
{

	if (!(SHA_FLAGS_INIT & dd->flags)) {
		dd->flags |= SHA_FLAGS_INIT;
	}

	//mc_sha_write(dd, HASH_MSG_LEN_CFG0, dd->req->nbytes);
	mc_sha_write(dd, MEM_INFO_CFG2, dd->flag_len);
	
	return 0;
}

static int mc_sha_handle_queue(struct mc_sha_dev *dd,
				  struct ahash_request *req)
{
	struct crypto_async_request *async_req, *backlog;
	struct mc_sha_ctx *ctx;
	unsigned long flags;
	bool start_async;
	int err = 0, ret = 0;

	spin_lock_irqsave(&dd->lock, flags);
	if (req)
		ret = ahash_enqueue_request(&dd->queue, req);

	if (SHA_FLAGS_BUSY & dd->flags) {
		spin_unlock_irqrestore(&dd->lock, flags);
		return ret;
	}

	backlog = crypto_get_backlog(&dd->queue);
	async_req = crypto_dequeue_request(&dd->queue);

	if (async_req)
		dd->flags |= SHA_FLAGS_BUSY;
	
	spin_unlock_irqrestore(&dd->lock, flags);

	if (!async_req)
		return ret;

	if (backlog)
		backlog->complete(backlog, -EINPROGRESS);


	ctx = crypto_tfm_ctx(async_req->tfm);

	dd->req = ahash_request_cast(async_req);
	start_async = (dd->req != req);

	dd->is_async = start_async;
	dd->force_complete = false;

	/* WARNING: ctx->start() MAY change dd->is_async. */
	err = ctx->start(dd);
	
	return (start_async) ? ret : err;
}

static int mc_sha_done(struct mc_sha_dev *dd);

static int mc_sha_start(struct mc_sha_dev *dd)
{
	struct ahash_request *req = dd->req;
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	int err;

	err = mc_sha_hw_init(dd);
	if (err)
		return mc_sha_complete(dd, err);

	/*
	 * mc_sha_update_req() and mc_sha_final_req() can return either:
	 *  -EINPROGRESS: the hardware is busy and the SHA driver will resume
	 *                its job later in the done_task.
	 *                This is the main path.
	 *
	 * 0: the SHA driver can continue its job then release the hardware
	 *    later, if needed, with mc_sha_finish_req().
	 *    This is the alternate path.
	 *
	 * < 0: an error has occurred so mc_sha_complete(dd, err) has already
	 *      been called, hence the hardware has been released.
	 *      The SHA driver must stop its job without calling
	 *      mc_sha_complete(), otherwise mc_sha_complete() would be
	 *      called a second time.
	 *
	 * Please note that currently, mc_sha_final_req() never returns 0.
	 */

	dd->resume = mc_sha_done;
	if (ctx->op == SHA_OP_UPDATE) {
		err = mc_sha_update_req(dd);
		if (!err && (ctx->flags & SHA_FLAGS_FINUP))
			/* no final() after finup() */
			err = mc_sha_final_req(dd);
	} else if (ctx->op == SHA_OP_FINAL) {
		err = mc_sha_final_req(dd);
	}

	if (!err)
		/* done_task will not finish it, so do it here */
		mc_sha_finish_req(req, err);

	return err;
}

static int mc_sha_enqueue(struct ahash_request *req, unsigned int op)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	struct mc_sha_ctx *tctx = crypto_tfm_ctx(req->base.tfm);
	struct mc_sha_dev *dd = tctx->dd;

	ctx->op = op;

	return mc_sha_handle_queue(dd, req);
}

static int mc_sha_update(struct ahash_request *req)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);

	struct mc_sha_dev *dd = ctx->dd;

	mc_sha_write(dd, HASH_MSG_LEN_CFG0, req->nbytes);

	if (!req->nbytes)
		return 0;
	
	ctx->total = req->nbytes;
	ctx->sg = req->src;
	ctx->offset = 0;
	

	if ((ctx->bufcnt + ctx->total < ctx->buflen) &&
	    !(ctx->flags & SHA_FLAGS_FINUP))
		return mc_sha_append_sg(ctx);

	return mc_sha_enqueue(req, SHA_OP_UPDATE);
}

static int mc_sha_final(struct ahash_request *req)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);

	ctx->flags |= SHA_FLAGS_FINUP;

	if (ctx->flags & SHA_FLAGS_ERROR)
		return 0; /* uncompleted hash is not needed */

	if (ctx->flags & SHA_FLAGS_PAD)
		/* copy ready hash (+ finalize hmac) */
		return mc_sha_finish(req);

	return mc_sha_enqueue(req, SHA_OP_FINAL);
}

static int mc_sha_finup(struct ahash_request *req)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	int err1, err2;

	ctx->flags |= SHA_FLAGS_FINUP;

	err1 = mc_sha_update(req);

	if (err1 == -EINPROGRESS ||
	    (err1 == -EBUSY && (ahash_request_flags(req) &
				CRYPTO_TFM_REQ_MAY_BACKLOG))){
						return err1;
					}

	/*
	 * final() has to be always called to cleanup resources
	 * even if udpate() failed, except EINPROGRESS
	 */
	err2 = mc_sha_final(req);

	return err1 ?: err2;
}

static int mc_sha_digest(struct ahash_request *req)
{
	return mc_sha_init(req) ?: mc_sha_finup(req);
}

static int mc_sha_export(struct ahash_request *req, void *out)
{
	const struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	memcpy(out, ctx, sizeof(*ctx));
	return 0;
}

static int mc_sha_import(struct ahash_request *req, const void *in)
{
	struct mc_sha_reqctx *ctx = ahash_request_ctx(req);
	memcpy(ctx, in, sizeof(*ctx));
	return 0;
}

static int mc_sha_cra_init(struct crypto_tfm *tfm)
{
	struct mc_sha_ctx *ctx = crypto_tfm_ctx(tfm);

	crypto_ahash_set_reqsize(__crypto_ahash_cast(tfm),
				 sizeof(struct mc_sha_reqctx));
	ctx->start = mc_sha_start;

	return 0;
}

static void mc_sha_alg_init(struct ahash_alg *alg)
{
	alg->halg.base.cra_priority = MC_SHA_PRIORITY;
	alg->halg.base.cra_flags = CRYPTO_ALG_ASYNC;
	alg->halg.base.cra_ctxsize = sizeof(struct mc_sha_ctx);
	alg->halg.base.cra_module = THIS_MODULE;
	alg->halg.base.cra_init = mc_sha_cra_init;

	alg->halg.statesize = sizeof(struct mc_sha_reqctx);

	alg->init = mc_sha_init;
	alg->update = mc_sha_update;
	alg->final = mc_sha_final;
	alg->finup = mc_sha_finup;
	alg->digest = mc_sha_digest;
	alg->export = mc_sha_export;
	alg->import = mc_sha_import;
}

static struct ahash_alg sha_1_256_algs[] = {
{
	.halg.base.cra_name		= "sha1",
	.halg.base.cra_driver_name	= "mc-sha1",
	.halg.base.cra_blocksize	= SHA1_BLOCK_SIZE,

	.halg.digestsize = SHA1_DIGEST_SIZE,
},
{
	.halg.base.cra_name		= "sha256",
	.halg.base.cra_driver_name	= "mc-sha256",
	.halg.base.cra_blocksize	= SHA256_BLOCK_SIZE,

	.halg.digestsize = SHA256_DIGEST_SIZE,
},
};

static struct ahash_alg sha_384_512_algs[] = {
{
	.halg.base.cra_name		= "sha384",
	.halg.base.cra_driver_name	= "mc-sha384",
	.halg.base.cra_blocksize	= SHA384_BLOCK_SIZE,
	.halg.base.cra_alignmask	= 0x3,

	.halg.digestsize = SHA384_DIGEST_SIZE,
},
{
	.halg.base.cra_name		= "sha512",
	.halg.base.cra_driver_name	= "mc-sha512",
	.halg.base.cra_blocksize	= SHA512_BLOCK_SIZE,
	.halg.base.cra_alignmask	= 0x3,

	.halg.digestsize = SHA512_DIGEST_SIZE,
},
};

static void mc_sha_queue_task(unsigned long data)
{
	struct mc_sha_dev *dd = (struct mc_sha_dev *)data;
	mc_sha_handle_queue(dd, NULL);
}

static int mc_sha_done(struct mc_sha_dev *dd)
{
	int err = 0;

 	if (SHA_FLAGS_DMA_READY & dd->flags) {
		if (SHA_FLAGS_DMA_ACTIVE & dd->flags) {
			dd->flags &= ~SHA_FLAGS_DMA_ACTIVE;
			mc_sha_update_dma_stop(dd);
		}
		if (SHA_FLAGS_OUTPUT_READY & dd->flags) {
			/* hash or semi-hash ready */
			dd->flags &= ~(SHA_FLAGS_DMA_READY |
						SHA_FLAGS_OUTPUT_READY);
			err = mc_sha_update_dma_start(dd);
			
			if (err != -EINPROGRESS)
				goto finish;
		}
	}
	return err;

finish:
	/* finish curent request */
	mc_sha_finish_req(dd->req, err);

	return err;
}

static void mc_sha_done_task(unsigned long data)
{
	struct mc_sha_dev *dd = (struct mc_sha_dev *)data;
	dd->is_async = true;
	(void)dd->resume(dd);
}

static irqreturn_t mc_sha_irq(int irq, void *dev_id)
{
	u32 reg;
	
	struct mc_sha_dev *sha_dd = dev_id;

	complete(&sha_dd->dma_completion);

	reg = mc_sha_read(sha_dd, HASH_INT);
	if (reg & INIT_STATUS) {
		reg = (reg | HASH_INT);
		mc_sha_write(sha_dd, HASH_INT, reg);
	
		reg =mc_sha_read(sha_dd, HASH_CFG);
		reg &= (~(HASH_START));
		mc_sha_write(sha_dd, HASH_CFG, reg);
		if (SHA_FLAGS_BUSY & sha_dd->flags) {
			sha_dd->flags |= SHA_FLAGS_OUTPUT_READY;
				sha_dd->flags |= SHA_FLAGS_DMA_READY;
			tasklet_schedule(&sha_dd->done_task);
		} else {
			dev_warn(sha_dd->dev, "SHA interrupt when no active requests.\n");
		}
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static void mc_sha_unregister_algs(struct mc_sha_dev *dd)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(sha_1_256_algs); i++)
		crypto_unregister_ahash(&sha_1_256_algs[i]);

	if (dd->caps.has_sha_384_512) {
		for (i = 0; i < ARRAY_SIZE(sha_384_512_algs); i++)
			crypto_unregister_ahash(&sha_384_512_algs[i]);
	}
}

static int mc_sha_register_algs(struct mc_sha_dev *dd)
{
	int err, i, j;

	for (i = 0; i < ARRAY_SIZE(sha_1_256_algs); i++) {
		mc_sha_alg_init(&sha_1_256_algs[i]);

		err = crypto_register_ahash(&sha_1_256_algs[i]);
		if (err){
			goto err_sha_1_256_algs;
		}
	
	}

	for (i = 0; i < ARRAY_SIZE(sha_384_512_algs); i++) {
		mc_sha_alg_init(&sha_384_512_algs[i]);

		err = crypto_register_ahash(&sha_384_512_algs[i]);
		if (err)
			goto err_sha_384_512_algs;
	}

	return 0;

err_sha_384_512_algs:
	for (j = 0; j < i; j++)
		crypto_unregister_ahash(&sha_384_512_algs[j]);
err_sha_1_256_algs:
	for (j = 0; j < i; j++)
		crypto_unregister_ahash(&sha_1_256_algs[j]);

	return err;
}

static void mc_sha_get_cap(struct mc_sha_dev *dd)
{
	dd->caps.has_dma = 1;
	dd->caps.has_dualbuff = 1;
	dd->caps.has_sha_384_512 = 1;
}

#if defined(CONFIG_OF)
 const struct of_device_id mc_sha_dt_ids[] = {
	{ .compatible = "mc,mc-sha" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, mc_sha_dt_ids);
#endif

static int mc_sha_probe(struct platform_device *pdev)
{
	struct mc_sha_dev *sha_dd;
	struct device *dev = &pdev->dev;
	struct resource *sha_res;
	int err,ret;

	sha_dd = devm_kzalloc(&pdev->dev, sizeof(*sha_dd), GFP_KERNEL);
	if (!sha_dd)
		return -ENOMEM;

	sha_dd->dev = dev;

	platform_set_drvdata(pdev, sha_dd);

	INIT_LIST_HEAD(&sha_dd->list);
	spin_lock_init(&sha_dd->lock);

	tasklet_init(&sha_dd->done_task, mc_sha_done_task,
					(unsigned long)sha_dd);
	tasklet_init(&sha_dd->queue_task, mc_sha_queue_task,
					(unsigned long)sha_dd);

	crypto_init_queue(&sha_dd->queue, MC_SHA_QUEUE_LENGTH);

	/* Get the base address */
	sha_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!sha_res) {
		dev_err(dev, "no MEM resource info\n");
		err = -ENODEV;
		goto err_tasklet_kill;
	}
	sha_dd->phys_base = sha_res->start;

	/* Get the IRQ */
	sha_dd->irq = platform_get_irq(pdev,  0);
	if (sha_dd->irq < 0) {
		err = sha_dd->irq;
		goto err_tasklet_kill;
	}

	err = devm_request_irq(&pdev->dev, sha_dd->irq, mc_sha_irq,
			       IRQF_SHARED, "mc-sha", sha_dd);
	if (err) {
		dev_err(dev, "unable to request sha irq.\n");
		goto err_tasklet_kill;
	}

	ret = of_property_read_u32(pdev->dev.of_node, "dma-rx-id", &sha_dd->rx_dma_id);
	if (ret) {
		dev_err(dev, "get hash dma id fail\r\n");
		return ret;
	}

	ret = of_property_read_u32(pdev->dev.of_node, "hash-max-frequency", &sha_dd->max_freq);
	if (ret) {
		dev_err(dev, "Execution environment unspecified\n");
		return ret;
	}

	ret = clk_mini_set_rate(CKG_MUX_HASH,sha_dd->max_freq);
	
	if (ret) {
			dev_err(dev, "failed to set clk mux\r\n");
			return ret;
	}
	
	sha_dd->io_base = devm_ioremap_resource(&pdev->dev, sha_res);
	if (IS_ERR(sha_dd->io_base)) {
		dev_err(dev, "can't ioremap\n");
		err = PTR_ERR(sha_dd->io_base);
		goto err_tasklet_kill;
	}
	err = clk_mini_enable(CKG_HASH_EN);
	if (err)
		goto err_tasklet_kill;
	
	err = clk_mini_enable(CKG_APB_HASH_EN);
	if (err)
		goto err_iclk_unprepare;

	init_completion(&sha_dd->dma_completion);

	mc_sha_get_cap(sha_dd);

	spin_lock(&mc_sha.lock);
	list_add_tail(&sha_dd->list, &mc_sha.dev_list);
	spin_unlock(&mc_sha.lock);
	dma_set_mask_and_coherent(dev, 0xFFFFFFFF);

	err = mc_sha_register_algs(sha_dd);
	if (err)
		goto err_algs;

	dev_info(dev, "molchip SHA1/SHA256%s%s\n",
			sha_dd->caps.has_sha_384_512 ? "/SHA384/SHA512" : "");

	return 0;

err_algs:
	spin_lock(&mc_sha.lock);
	list_del(&sha_dd->list);
	spin_unlock(&mc_sha.lock);
err_iclk_unprepare:
	clk_mini_disable(CKG_HASH_EN);
err_tasklet_kill:
	tasklet_kill(&sha_dd->queue_task);
	tasklet_kill(&sha_dd->done_task);

	return err;
}

static int mc_sha_remove(struct platform_device *pdev)
{
	struct mc_sha_dev *sha_dd;

	sha_dd = platform_get_drvdata(pdev);
	if (!sha_dd)
		return -ENODEV;
	spin_lock(&mc_sha.lock);
	list_del(&sha_dd->list);
	spin_unlock(&mc_sha.lock);

	mc_sha_unregister_algs(sha_dd);

	tasklet_kill(&sha_dd->queue_task);
	tasklet_kill(&sha_dd->done_task);

	clk_mini_disable(CKG_HASH_EN);
	return 0;
}

static struct platform_driver mc_sha_driver = {
	.probe		= mc_sha_probe,
	.remove		= mc_sha_remove,
	.driver		= {
		.name	= "mc_sha",
		.of_match_table	= of_match_ptr(mc_sha_dt_ids),
	},
};

module_platform_driver(mc_sha_driver);

MODULE_DESCRIPTION("MC SHA (1/256/384/512) hw acceleration support.");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Nicolas Royer - Eukréa Electromatique");
