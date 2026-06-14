/*
 * Cryptographic API.
 *
 * Driver for AES acceleration.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
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
#include <linux/pm_runtime.h>
#include <linux/dma/mc_dma64.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/crypto.h>
#include <crypto/scatterwalk.h>
#include <crypto/algapi.h>
#include <crypto/aes.h>
#include <crypto/aes.h>
#include <crypto/des.h>
#include <crypto/internal/skcipher.h>
#include <crypto/cryptodev.h>
#include "mc-aes.h"
#include <linux/mini_clock/clock.h>


static struct mc_aes_drv mc_aes = {
	.dev_list = LIST_HEAD_INIT(mc_aes.dev_list),
	.lock = __SPIN_LOCK_UNLOCKED(mc_aes.lock),
};

BLOCKING_NOTIFIER_HEAD(aes_chain);
EXPORT_SYMBOL_GPL(aes_chain);
/* Shared functions */
#define KEY_IN_EFUSE "KISE"
#define KEY_IN_EFUSE_LEN strlen(KEY_IN_EFUSE) - 1


#define USE_CIPHER_KEY "CIPHER_KEY"
#define USE_CIPHER_KEY_LEN strlen(USE_CIPHER_KEY)
#define CIPHER_KEY "CKIS"
#define CIPHER_KEY_LEN strlen(CIPHER_KEY)
unsigned int cipher_key_buf[4] __attribute__((aligned(64)));

#define USE_EXT_DMA
static inline u32 mc_aes_read(struct mc_aes_dev *dd, u32 offset)
{
	u32 value = readl_relaxed(dd->io_base + offset);
	return value;
}

static inline void mc_aes_write(struct mc_aes_dev *dd,
					u32 offset, u32 value)
{
	writel_relaxed(value, dd->io_base + offset);
}

static void mc_aes_write_le_n(struct mc_aes_dev *dd, u32 offset,
			      const u32 *value, int count)
{
	for (; count--; value++, offset += 4)
		mc_aes_write(dd, offset, cpu_to_be32(*value));
}

static inline void mc_aes_write_le_block(struct mc_aes_dev *dd, u32 offset,
					 const u32 *value)
{
	mc_aes_write_le_n(dd, offset, value, SIZE_IN_WORDS(AES_BLOCK_SIZE));
}

static inline size_t mc_aes_padlen(size_t len, size_t block_size)
{
	len &= block_size - 1;
	return len ? block_size - len : 0;
}

static struct mc_aes_dev *mc_aes_find_dev(struct mc_aes_base_ctx *ctx)
{
	struct mc_aes_dev *aes_dd = NULL;
	struct mc_aes_dev *tmp;

	spin_lock_bh(&mc_aes.lock);
	if (!ctx->dd) {
		list_for_each_entry(tmp, &mc_aes.dev_list, list) {
			aes_dd = tmp;
			break;
		}
		ctx->dd = aes_dd;
	} else {
		aes_dd = ctx->dd;
	}

	spin_unlock_bh(&mc_aes.lock);

	return aes_dd;
}

static int mc_aes_hw_init(struct mc_aes_dev *dd)
{
	int reg;

	//aes en
	regmap_update_bits(dd->cpu_sysy_base, dd->aes_clk_ctrl_reg,
			(1 << dd->aes_clk_ctrl_bit), (1 << dd->aes_clk_ctrl_bit));

	regmap_update_bits(dd->cpu_sysy_base, dd->aes_reset_reg,
			(3 << dd->aes_sw_reset_bit), (3 << dd->aes_sw_reset_bit));

	regmap_update_bits(dd->cpu_sysy_base, dd->aes_reset_reg,
			(3 << dd->aes_sw_reset_bit), ~(3 << dd->aes_sw_reset_bit));

	reg = MC_AES_DMA_THRESHOLD << AES_OUTFIFO_THOLD_OFFSET;
	reg |= MC_AES_DMA_THRESHOLD;
	mc_aes_write(dd, AES_FIFO_THRESHOLD, reg);

	mc_aes_write(dd, AES_INTR_ENABLE, AES_INTR_ENABLE_DONE);

	return 0;
}

static inline void mc_aes_set_mode(struct mc_aes_dev *dd,
				      const struct mc_aes_reqctx *rctx)
{

	int reg;
	reg = mc_aes_read(dd,AES_ENCRYPT_CTRL);
	reg &= ~(AES_CTRL_DECRYPT | AES_CTRL_METHOD_MASK | AES_CTRL_EMODE_MASK);
	reg |= rctx->mode;
	reg |= AES_CTRL_FIRST;
	mc_aes_write(dd,AES_ENCRYPT_CTRL,reg);
}

static void mc_aes_set_iv_as_last_ciphertext_block(struct mc_aes_dev *dd)
{
	unsigned int last_iv[4];
	struct skcipher_request *req = skcipher_request_cast(dd->areq);
	struct mc_aes_reqctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	unsigned int ivsize = crypto_skcipher_ivsize(skcipher);
	 if (rctx->mode & AES_FLAGS_ENCRYPT) {
		last_iv[0] =cpu_to_be32(mc_aes_read(dd,LAST_IV(0)));
		last_iv[1] =cpu_to_be32(mc_aes_read(dd,LAST_IV(1)));
		last_iv[2] =cpu_to_be32(mc_aes_read(dd,LAST_IV(2)));
		last_iv[3] =cpu_to_be32(mc_aes_read(dd,LAST_IV(3)));
		memcpy(req->iv, last_iv, ivsize);
	} else {
		last_iv[0] =cpu_to_be32(mc_aes_read(dd,LAST_IV(0)));
		last_iv[1] =cpu_to_be32(mc_aes_read(dd,LAST_IV(1)));
		last_iv[2] =cpu_to_be32(mc_aes_read(dd,LAST_IV(2)));
		last_iv[3] =cpu_to_be32(mc_aes_read(dd,LAST_IV(3)));
		memcpy(req->iv, last_iv, ivsize);
	}
}
static inline int mc_aes_complete(struct mc_aes_dev *dd, int err)
{
	struct skcipher_request *req = skcipher_request_cast(dd->areq);
	struct mc_aes_reqctx *rctx = skcipher_request_ctx(req);

	dd->flags &= ~AES_FLAGS_BUSY;
	if (!err && !dd->ctx->is_aead &&
		(rctx->mode & AES_CTRL_EMODE_MASK) != AES_EMODE_ECB) {
		if ((rctx->mode & AES_CTRL_EMODE_MASK) != AES_EMODE_CTR)
			mc_aes_set_iv_as_last_ciphertext_block(dd);
	}

	dd->areq->complete(dd->areq, err);
	pm_runtime_put(dd->dev);

	tasklet_schedule(&dd->queue_task);

	return err;
}

static void mc_aes_write_ctrl_key(struct mc_aes_dev *dd,
				     const u32 *iv, const u32 *key, int keylen)
{
	struct skcipher_request *req = skcipher_request_cast(dd->areq);
	struct mc_aes_reqctx *rctx = skcipher_request_ctx(req);
	struct aes_key_param param;

	if(memcmp(key, KEY_IN_EFUSE, KEY_IN_EFUSE_LEN) == 0) {
		printk("USR Efuse KEY\r\n");
		dd->efuse_key = true;
		param.length = keylen;
		param.entry = key[3];
		printk("param.length  is 0x%08x param.entry is 0x%08x\r\n",param.length,param.entry );
		blocking_notifier_call_chain(&aes_chain, AES_MSG_KEY_REQUEST, &param);
	} else {

		blocking_notifier_call_chain(&aes_chain, AES_MSG_KEY_RELEASE, NULL);

		if (memcmp(key, CIPHER_KEY, CIPHER_KEY_LEN) == 0) {
			dd->cipher_key_flags = true;
			mc_aes_write_le_n(dd, AES_KEY(0), &key[1], SIZE_IN_WORDS(keylen));
		} else if (memcmp(key, USE_CIPHER_KEY, USE_CIPHER_KEY_LEN) == 0) {
			cipher_key_buf[0] = cpu_to_be32(cipher_key_buf[0]);
			cipher_key_buf[1] = cpu_to_be32(cipher_key_buf[1]);
			cipher_key_buf[2] = cpu_to_be32(cipher_key_buf[2]);
			cipher_key_buf[3] = cpu_to_be32(cipher_key_buf[3]);
			mc_aes_write_le_n(dd, AES_KEY(0), &cipher_key_buf[0], SIZE_IN_WORDS(keylen));

		} else {
			mc_aes_write_le_n(dd, AES_KEY(0), key, SIZE_IN_WORDS(keylen));
		}

	}

	if (iv)
	{
		switch(rctx->mode & AES_CTRL_METHOD_MASK) {
		case AES_METHOD_DES:
		case AES_METHOD_TDES:
			mc_aes_write_le_n(dd, AES_IV(0), iv, 2);
			break;
		default:
			mc_aes_write_le_block(dd, AES_IV(0), iv);
			break;
		}
	}
}

static inline void mc_aes_write_ctrl(struct mc_aes_dev *dd,
					const u32 *iv)

{
	mc_aes_write_ctrl_key(dd, iv,
				 dd->ctx->key, dd->ctx->keylen);
}


static bool mc_aes_check_aligned(struct mc_aes_dev *dd,
				    struct scatterlist *sg,
				    size_t len,
				    struct mc_aes_dma *dma)
{
	int nents;

	#ifndef USE_EXT_DMA
		if (!IS_ALIGNED(len, dd->ctx->block_size)){
			return false;
		}
	#endif

	for (nents = 0; sg; sg = sg_next(sg), ++nents) {
		//app should 16byte aligned can use exit dma
		if (!IS_ALIGNED(sg->offset, sizeof(u32))){
			return false;
		}

		if (len <= sg->length) {

			#ifndef USE_EXT_DMA
				if (!IS_ALIGNED(len, dd->ctx->block_size))
						return false;
			#endif

			dma->nents = nents+1;
			dma->remainder = sg->length - len;
			sg->length = len;

			#ifndef USE_EXT_DMA
				if(dma->nents > 1)
					return false;
			#endif

			return true;
		}

		#ifndef USE_EXT_DMA
			if (!IS_ALIGNED(sg->length, dd->ctx->block_size))
				return false;
		#endif

		len -= sg->length;
	}

	return false;
}

static inline void mc_aes_restore_sg(const struct mc_aes_dma *dma)
{
	struct scatterlist *sg = dma->sg;
	int nents = dma->nents;

	if (!dma->remainder)
		return;

	while (--nents > 0 && sg)
		sg = sg_next(sg);

	if (!sg)
		return;

	sg->length += dma->remainder;
}

static int mc_aes_dma_init(struct mc_aes_dev *dd)
{
	int ret = 0;
	dma_cap_mask_t		mask_rx,mask_tx;
	dma_cap_zero(mask_rx);
	dma_cap_set(DMA_SLAVE, mask_rx);
	dma_cap_zero(mask_tx);
	dma_cap_set(DMA_SLAVE, mask_tx);

	if (!dd->src.chan) {

		dd->src.chan = dma_request_channel(mask_rx,NULL,NULL);
		if (!dd->src.chan) {
			return ENODEV;
		}
	}

	if (!dd->dst.chan) {
		dd->dst.chan = dma_request_channel(mask_tx,NULL,NULL);
		if (!dd->src.chan) {
			return ENODEV;
		}
	}

	return ret;
}

static int mc_aes_map(struct mc_aes_dev *dd,
			 struct scatterlist *src,
			 struct scatterlist *dst,
			 size_t len)
{
	bool src_aligned, dst_aligned;
	size_t padlen;

	dd->total = len;
	dd->src.sg = src;
	dd->dst.sg = dst;
	dd->real_dst = dst;
	src_aligned = mc_aes_check_aligned(dd, src, len, &dd->src);

	if (src == dst)
		dst_aligned = src_aligned;

	else
		dst_aligned = mc_aes_check_aligned(dd, dst, len, &dd->dst);

	// aes as slave src_aligned and dst_aligned is true
	if (!src_aligned || !dst_aligned) {
		padlen = mc_aes_padlen(len, dd->ctx->block_size);

		if (dd->buflen < len + padlen)
			return -ENOMEM;

		if (!src_aligned) {
			sg_copy_to_buffer(src, sg_nents(src), dd->buf, len);
			dd->src.sg = &dd->aligned_sg;
			dd->src.nents = 1;
			dd->src.remainder = 0;
		}

		if (!dst_aligned) {
			dd->dst.sg = &dd->aligned_sg;
			dd->dst.nents = 1;
			dd->dst.remainder = 0;
		}

		sg_init_one(&dd->aligned_sg, dd->buf, len + padlen);
	}


	if (dd->src.sg == dd->dst.sg) {
		dd->src.sg_len = dma_map_sg(dd->dev, dd->src.sg, dd->src.nents,
					    DMA_BIDIRECTIONAL);
		dd->dst.sg_len = dd->src.sg_len;
		if (!dd->src.sg_len)
			return -EFAULT;
	} else {
		dd->src.sg_len = dma_map_sg(dd->dev, dd->src.sg, dd->src.nents,
					    DMA_TO_DEVICE);
		if (!dd->src.sg_len)
			return -EFAULT;


		if (dd->cipher_key_flags == true) {

			dd->cipher_key_buf_phys = dma_map_single(dd->dev, cipher_key_buf, sizeof(cipher_key_buf), DMA_FROM_DEVICE);
			if (dma_mapping_error(dd->dev, dd->cipher_key_buf_phys)) {
				printk("DMA mapping error for cipher_key_buf\n");
				return -EFAULT;
			}

			sg_dma_address(dd->dst.sg) = dd->cipher_key_buf_phys;
			sg_dma_len(dd->dst.sg) = sizeof(cipher_key_buf);
			dd->dst.sg_len = 1;

		} else{

		dd->dst.sg_len = dma_map_sg(dd->dev, dd->dst.sg, dd->dst.nents,
					    DMA_FROM_DEVICE);
		if (!dd->dst.sg_len) {
			dma_unmap_sg(dd->dev, dd->src.sg, dd->src.nents,
				     DMA_TO_DEVICE);
			return -EFAULT;
		}
	}
	}

	return 0;
}

static void mc_aes_unmap(struct mc_aes_dev *dd)
{
	if (dd->src.sg == dd->dst.sg) {
		dma_unmap_sg(dd->dev, dd->src.sg, dd->src.nents,
			     DMA_BIDIRECTIONAL);

		if (dd->src.sg != &dd->aligned_sg)
			mc_aes_restore_sg(&dd->src);
	} else {
		dma_unmap_sg(dd->dev, dd->dst.sg, dd->dst.nents,
			     DMA_FROM_DEVICE);

		if (dd->dst.sg != &dd->aligned_sg)
			mc_aes_restore_sg(&dd->dst);

		dma_unmap_sg(dd->dev, dd->src.sg, dd->src.nents,
			     DMA_TO_DEVICE);

		if (dd->src.sg != &dd->aligned_sg)
			mc_aes_restore_sg(&dd->src);
	}

	if (dd->dst.sg == &dd->aligned_sg)
		sg_copy_from_buffer(dd->real_dst, sg_nents(dd->real_dst),
				    dd->buf, dd->total);
}

 static void mc_aes_dma_callback(void *data)
{
	struct mc_aes_dev *dd = data;
	mc_aes_unmap(dd);
}

 static int mc_aes_dma_transfer_start(struct mc_aes_dev *dd,
		enum dma_transfer_direction dir)
{
	 struct dma_async_tx_descriptor *desc;
	 struct dma_slave_config config;
	 dma_async_tx_callback callback;
	 struct mc_aes_dma *dma;
	 int err;

	 memset(&config, 0, sizeof(config));
	 config.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	 config.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	 config.src_maxburst = AES_REQ_BYTE_NUM_PERCENT;
	 config.dst_maxburst = AES_REQ_BYTE_NUM_PERCENT;

	 switch (dir) {
	 case DMA_MEM_TO_DEV:
		 dma = &dd->src;
		 callback = NULL;
		 config.slave_id = dd->rx_dma_id;
		 config.direction = DMA_MEM_TO_DEV;
		 config.src_addr =  sg_dma_address(dd->src.sg);
		 config.dst_addr = dd->phys_base +AES_RX_FIFO;
	     err = dmaengine_slave_config(dd->src.chan, &config);
	    if (err){
			printk("dmaengine_slave_config fail \r\n");
	     }

	    desc = dmaengine_prep_slave_sg( dd->src.chan, dma->sg, dma->sg_len, dir,
						DMA_PREP_INTERRUPT | DMA_CTRL_ACK);

		desc->callback = NULL;
		desc->callback_param = dd;
		dmaengine_submit(desc);
		dma_async_issue_pending(dd->src.chan);
		break;

	 case DMA_DEV_TO_MEM:
		 dma = &dd->dst;
		 callback =mc_aes_dma_callback;
		 config.slave_id = dd->tx_dma_id;
		 config.direction = DMA_DEV_TO_MEM;
		 config.src_addr = dd->phys_base + AES_TX_FIFO;
		 config.dst_addr =  sg_dma_address(dd->dst.sg);
		 err = dmaengine_slave_config(dd->dst.chan, &config);
		 if (err){
			printk("dmaengine_slave_config fail \r\n");
		 }

		 desc = dmaengine_prep_slave_sg(dd->dst.chan, dma->sg, dma->sg_len, dir,
						DMA_PREP_INTERRUPT | DMA_CTRL_ACK);

		 desc->callback =  mc_aes_dma_callback;
		 desc->callback_param = dd;
		 dmaengine_submit(desc);
		 dma_async_issue_pending(dd->dst.chan);
		 break;

	 default:
		 return -EINVAL;
	 }

	return 0;
}

static int mc_aes_dma_start(struct mc_aes_dev *dd,
			       struct scatterlist *src,
			       struct scatterlist *dst,
			       size_t len,
			       mc_aes_fn_t resume)
{
	int err;

	err = mc_aes_map(dd, src, dst, len);
	if (err)
		goto exit;

	dd->resume = resume;

	/* Set output DMA transfer first */
	mc_aes_write(dd, AES_DMA_TRAS_SIZE, len);

	err = mc_aes_dma_transfer_start(dd, DMA_MEM_TO_DEV);
	if (err)
			printk("AES DMA RX CONFIG FAIL\r\n");

	err = mc_aes_dma_transfer_start(dd, DMA_DEV_TO_MEM);
	if (err)
		printk("AES DMA TX CONFIG FAIL\r\n");

	//aes fifo config
	mc_aes_write(dd, AES_FIFO_THRESHOLD, (0xF << AES_INFIFO_THOLD_OFFSET)| (0xF << AES_OUTFIFO_THOLD_OFFSET));

	//aes dma req num percent
	mc_aes_write(dd, AES_EXI_DMA_REQ, (AES_REQ_BYTE_NUM_PERCENT)|(AES_REQ_BYTE_NUM_PERCENT << 16));

	/* Then set input DMA transfer */
	mc_aes_write(dd, AES_EXI_DMA_EN, AES_DMA_EN);
	mc_aes_write(dd, AES_DMA_CTRL, AES_DMA_EN);
	return -EINPROGRESS;

exit:
	return mc_aes_complete(dd, err);
}

static int mc_aes_handle_queue(struct mc_aes_dev *dd,
				  struct crypto_async_request *new_areq)
{
	struct crypto_async_request *areq, *backlog;
	struct mc_aes_base_ctx *ctx;
	unsigned long flags;
	int err, ret = 0;

	spin_lock_irqsave(&dd->lock, flags);
	if (new_areq)
		ret = crypto_enqueue_request(&dd->queue, new_areq);
	if (dd->flags & AES_FLAGS_BUSY) {
		spin_unlock_irqrestore(&dd->lock, flags);
		return ret;
	}
	backlog = crypto_get_backlog(&dd->queue);
	areq = crypto_dequeue_request(&dd->queue);
	if (areq)
		dd->flags |= AES_FLAGS_BUSY;
	spin_unlock_irqrestore(&dd->lock, flags);

	if (!areq)
		return ret;

	if (backlog)
		backlog->complete(backlog, -EINPROGRESS);

	ctx = crypto_tfm_ctx(areq->tfm);

	dd->areq = areq;
	dd->ctx = ctx;

	err = ctx->start(dd);
	return  err;
}


/* AES async block ciphers */

static int mc_aes_transfer_complete(struct mc_aes_dev *dd)
{
	return mc_aes_complete(dd, 0);
}

static int mc_aes_start(struct mc_aes_dev *dd)
{
	int err = 0;
	struct skcipher_request *req = skcipher_request_cast(dd->areq);
	struct mc_aes_reqctx *rctx = skcipher_request_ctx(req);
	mc_aes_hw_init(dd);
	err = pm_runtime_resume_and_get(dd->dev);
	if (err < 0) {
		dev_err(dd->dev, "failed to get sync: %d\n", err);
		return err;
	}

	mc_aes_set_mode(dd, rctx);
	mc_aes_write_ctrl(dd, (void *)req->iv);

	return mc_aes_dma_start(dd, req->src, req->dst, req->cryptlen,
					   mc_aes_transfer_complete);
}

static int mc_aes_crypt(struct skcipher_request *req, unsigned long mode)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct mc_aes_base_ctx *ctx = crypto_skcipher_ctx(skcipher);
	struct mc_aes_reqctx *rctx;
	struct mc_aes_dev *dd;

	switch (mode & AES_CTRL_METHOD_MASK) {
	case AES_METHOD_DES:
	case AES_METHOD_TDES:
		ctx->block_size = DES_BLOCK_SIZE;
		break;
	default:
		if((mode & AES_CTRL_EMODE_MASK) == AES_EMODE_CFB)
			ctx->block_size = DES_BLOCK_SIZE;
		else
			ctx->block_size = AES_BLOCK_SIZE;
		break;
	}

	dd = mc_aes_find_dev(ctx);
	if (!dd)
		return -ENODEV;

	rctx = skcipher_request_ctx(req);
	rctx->mode = mode;

	return mc_aes_handle_queue(dd, &req->base);
}

static int mc_aes_setkey(struct crypto_skcipher *tfm, const u8 *key,
			   unsigned int keylen)
{
	struct mc_aes_base_ctx *ctx = crypto_skcipher_ctx(tfm);
	memcpy(ctx->key, key, keylen);
	if (memcmp(key, CIPHER_KEY, CIPHER_KEY_LEN) == 0) {
		keylen -= CIPHER_KEY_LEN;
	}

	if (keylen != AES_KEYSIZE_128 &&
	    keylen != AES_KEYSIZE_192 &&
	    keylen != AES_KEYSIZE_256 &&
	    keylen != DES_KEY_SIZE) {
		return -EINVAL;
	}

	memcpy(ctx->key, key, keylen);
	ctx->keylen = keylen;

	return 0;
}

static inline int mc_aes_emothod(struct skcipher_request *req)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct mc_aes_base_ctx *ctx = crypto_skcipher_ctx(skcipher);

	switch(ctx->keylen) {

	case AES_KEYSIZE_256:
		return AES_METHOD_AES_K256;

	case AES_KEYSIZE_192:
		return AES_METHOD_AES_K192;

	default:case AES_KEYSIZE_128:
		return AES_METHOD_AES_K128;

	}
}

static int mc_aes_ecb_encrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_ECB | emothod);
}

static int mc_aes_ecb_decrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_ECB | emothod | AES_CTRL_DECRYPT);
}

static int mc_aes_cbc_encrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_CBC | emothod);
}

static int mc_aes_cbc_decrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_CBC | emothod| AES_CTRL_DECRYPT);
}

static int mc_aes_ofb_encrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_OFB | emothod );
}

static int mc_aes_ofb_decrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_OFB | emothod | AES_CTRL_DECRYPT);
}

static int mc_aes_cfb_encrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_CFB | emothod);
}

static int mc_aes_cfb_decrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_CFB | emothod | AES_CTRL_DECRYPT);
}

static int mc_aes_ctr_encrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_CTR | emothod);
}

static int mc_aes_ctr_decrypt(struct skcipher_request *req)
{
	int emothod;
	emothod = mc_aes_emothod(req);
	return mc_aes_crypt(req, AES_EMODE_CTR | emothod | AES_CTRL_DECRYPT);
}

static int mc_des_ecb_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_ECB | AES_METHOD_DES);
}

static int mc_des_ecb_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_ECB | AES_METHOD_DES | AES_CTRL_DECRYPT);
}

static int mc_des_cbc_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CBC | AES_METHOD_DES);
}

static int mc_des_cbc_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CBC | AES_METHOD_DES | AES_CTRL_DECRYPT);
}

static int mc_des_cfb_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CFB | AES_METHOD_DES);
}

static int mc_des_cfb_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CFB | AES_METHOD_DES | AES_CTRL_DECRYPT);
}

static int mc_des_ofb_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_OFB | AES_METHOD_DES);
}

static int mc_des_ofb_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_OFB | AES_METHOD_DES | AES_CTRL_DECRYPT);
}

static int mc_tdes_ecb_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_ECB | AES_METHOD_TDES);
}

static int mc_tdes_ecb_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_ECB | AES_METHOD_TDES | AES_CTRL_DECRYPT);
}

static int mc_tdes_cbc_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CBC | AES_METHOD_TDES);
}

static int mc_tdes_cbc_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CBC | AES_METHOD_TDES | AES_CTRL_DECRYPT);
}

static int mc_tdes_cfb_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CFB | AES_METHOD_TDES);
}

static int mc_tdes_cfb_decrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_CFB | AES_METHOD_TDES | AES_CTRL_DECRYPT);
}

static int mc_tdes_ofb_encrypt(struct skcipher_request *req)
{

	return mc_aes_crypt(req, AES_EMODE_OFB | AES_METHOD_TDES);
}

static int mc_tdes_ofb_decrypt(struct skcipher_request *req)
{
	return mc_aes_crypt(req, AES_EMODE_OFB | AES_METHOD_TDES | AES_CTRL_DECRYPT);
}

static int mc_aes_init(struct crypto_skcipher *tfm)
{
	struct mc_aes_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_skcipher_set_reqsize(tfm, sizeof(struct mc_aes_reqctx));
	ctx->base.start = mc_aes_start;

	return 0;
}

static struct skcipher_alg aes_algs[] = {
{
	.base.cra_name		= "ecb(aes)",
	.base.cra_driver_name	= "mc-ecb-aes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= AES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= AES_MIN_KEY_SIZE,
	.max_keysize	= AES_MAX_KEY_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_aes_ecb_encrypt,
	.decrypt	= mc_aes_ecb_decrypt,
},
{
	.base.cra_name		= "cbc(aes)",
	.base.cra_driver_name	= "mc-cbc-aes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= AES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= AES_MIN_KEY_SIZE,
	.max_keysize	= AES_MAX_KEY_SIZE,
	.ivsize		= AES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_aes_cbc_encrypt,
	.decrypt	= mc_aes_cbc_decrypt,
},
{
	.base.cra_name		= "ofb(aes)",
	.base.cra_driver_name	= "mc-ofb-aes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= AES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= AES_MIN_KEY_SIZE,
	.max_keysize	= AES_MAX_KEY_SIZE,
	.ivsize		= AES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_aes_ofb_encrypt,
	.decrypt	= mc_aes_ofb_decrypt,
},
{
	.base.cra_name		= "cfb8(aes)",
	.base.cra_driver_name	= "mc-cfb8-aes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= CFB8_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= AES_MIN_KEY_SIZE,
	.max_keysize	= AES_MAX_KEY_SIZE,
	.ivsize		= AES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_aes_cfb_encrypt,
	.decrypt	= mc_aes_cfb_decrypt,
},
{
	.base.cra_name		= "ctr(aes)",
	.base.cra_driver_name	= "mc-ctr-aes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= 1,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= AES_MIN_KEY_SIZE,
	.max_keysize	= AES_MAX_KEY_SIZE,
	.ivsize		= AES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_aes_ctr_encrypt,
	.decrypt	= mc_aes_ctr_decrypt,
},
{
	.base.cra_name		= "ecb(des)",
	.base.cra_driver_name	= "mc-ecb-des",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= DES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= DES_KEY_SIZE,
	.max_keysize	= DES_KEY_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_des_ecb_encrypt,
	.decrypt	= mc_des_ecb_decrypt,
},
{
	.base.cra_name		= "cbc(des)",
	.base.cra_driver_name	= "mc-cbc-des",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= DES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= DES_KEY_SIZE,
	.max_keysize	= DES_KEY_SIZE,
	.ivsize		= DES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_des_cbc_encrypt,
	.decrypt	= mc_des_cbc_decrypt,
},
{
	.base.cra_name		= "cfb8(des)",
	.base.cra_driver_name	= "mc-cfb8-des",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= CFB8_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= DES_KEY_SIZE,
	.max_keysize	= DES_KEY_SIZE,
	.ivsize		= DES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_des_cfb_encrypt,
	.decrypt	= mc_des_cfb_decrypt,
},
{
	.base.cra_name		= "ofb(des)",
	.base.cra_driver_name	= "mc-ofb-des",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= DES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= DES_KEY_SIZE,
	.max_keysize	= DES_KEY_SIZE,
	.ivsize		= DES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_des_ofb_encrypt,
	.decrypt	= mc_des_ofb_decrypt,
},
{
	.base.cra_name		= "ecb(des3_ede)",
	.base.cra_driver_name	= "mc-ecb-tdes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= DES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= 3 * DES_KEY_SIZE,
	.max_keysize	= 3 * DES_KEY_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_tdes_ecb_encrypt,
	.decrypt	= mc_tdes_ecb_decrypt,
},
{
	.base.cra_name		= "cbc(des3_ede)",
	.base.cra_driver_name	= "mc-cbc-tdes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= DES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= 3 * DES_KEY_SIZE,
	.max_keysize	= 3 * DES_KEY_SIZE,
	.ivsize		= DES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_tdes_cbc_encrypt,
	.decrypt	= mc_tdes_cbc_decrypt,
},
{
	.base.cra_name		= "cfb8(des3_ede)",
	.base.cra_driver_name	= "mc-cfb8-tdes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= CFB8_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= 3 * DES_KEY_SIZE,
	.max_keysize	= 3 * DES_KEY_SIZE,
	.ivsize		= DES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_tdes_cfb_encrypt,
	.decrypt	= mc_tdes_cfb_decrypt,
},
{
	.base.cra_name		= "ofb(des3_ede)",
	.base.cra_driver_name	= "mc-ofb-tdes",
	.base.cra_priority	= 400,
	.base.cra_blocksize		= DES_BLOCK_SIZE,
	.base.cra_ctxsize		= sizeof(struct mc_aes_ctx),
	.init		= mc_aes_init,
	.min_keysize	= 3 * DES_KEY_SIZE,
	.max_keysize	= 3 * DES_KEY_SIZE,
	.ivsize		= DES_BLOCK_SIZE,
	.setkey		= mc_aes_setkey,
	.encrypt	= mc_tdes_ofb_encrypt,
	.decrypt	= mc_tdes_ofb_decrypt,
},
};

/* Probe functions */
static int mc_aes_buff_init(struct mc_aes_dev *dd)
{
	dd->buf = (void *)__get_free_pages(GFP_KERNEL, MC_AES_BUFFER_ORDER);
	dd->buflen = MC_AES_BUFFER_SIZE;
	dd->buflen &= ~(AES_BLOCK_SIZE - 1);

	if (!dd->buf) {
		dev_err(dd->dev, "unable to alloc pages.\n");
		return -ENOMEM;
	}

	return 0;
}

static void mc_aes_buff_cleanup(struct mc_aes_dev *dd)
{
	free_page((unsigned long)dd->buf);
}

static void mc_aes_queue_task(unsigned long data)
{
	struct mc_aes_dev *dd = (struct mc_aes_dev *)data;

	mc_aes_handle_queue(dd, NULL);
}

static void mc_aes_done_task(unsigned long data)
{

	struct mc_aes_dev *dd = (struct mc_aes_dev *)data;

	if (dd->cipher_key_flags == true) {

		dma_unmap_single(dd->dev, dd->cipher_key_buf_phys, sizeof(cipher_key_buf), DMA_FROM_DEVICE);
		dd->cipher_key_flags = false;

	}else{

		mc_aes_unmap(dd);
	}

	(void)dd->resume(dd);
}

static irqreturn_t mc_aes_irq(int irq, void *dev_id)
{
	u32 reg = 0;
	struct mc_aes_dev *aes_dd = dev_id;

	reg = mc_aes_read(aes_dd, AES_INTR_SRC);
	mc_aes_write(aes_dd, AES_EXI_DMA_EN, 0);
	if (reg & mc_aes_read(aes_dd, AES_INTR_MASKED)) {
		mc_aes_write(aes_dd, AES_INTR_CLEAR, reg);
		if (AES_FLAGS_BUSY & aes_dd->flags)
			tasklet_schedule(&aes_dd->done_task);
		else
			dev_warn(aes_dd->dev, "AES interrupt when no active requests.\n");
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static void mc_aes_unregister_algs(struct mc_aes_dev *dd)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(aes_algs); i++)
		crypto_unregister_skcipher(&aes_algs[i]);
}

static int mc_aes_register_algs(struct mc_aes_dev *dd)
{
	int err, i, j;

	for (i = 0; i < ARRAY_SIZE(aes_algs); i++) {
		err = crypto_register_skcipher(&aes_algs[i]);
		if (err)
			goto err_aes_algs;
	}

	return 0;


err_aes_algs:
	for (j = 0; j < i; j++)
		crypto_unregister_skcipher(&aes_algs[j]);

	return err;
}


#if defined(CONFIG_OF)
static const struct of_device_id mc_aes_dt_ids[] = {
	{ .compatible = "mc,aes" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mc_aes_dt_ids);

#endif
/* Probe functions */
static int mc_aes_probe(struct platform_device *pdev)
{
	struct mc_aes_dev *aes_dd;
	struct device *dev = &pdev->dev;
	struct resource *aes_res;
	int err,ret;

	struct device_node *node =dev->of_node;
	aes_dd = devm_kzalloc(&pdev->dev, sizeof(*aes_dd), GFP_KERNEL);
	if (aes_dd == NULL) {
		err = -ENOMEM;
		goto aes_dd_err;
	}
	aes_dd->cpu_sysy_base = syscon_regmap_lookup_by_phandle(node,
						       "molchip,syscon-cpu-reg");
	if (WARN_ON(!aes_dd->cpu_sysy_base))
			return -ENOMEM;
	ret = of_property_read_u32(node, "aes_sw_reset_reg",
			     &aes_dd->aes_reset_reg);
	if (WARN_ON(ret))
		return -ENOMEM;
	ret = of_property_read_u32(node, "aes_sw_reset_bit",
			     &aes_dd->aes_sw_reset_bit);
	if (WARN_ON(ret))
		return -ENOMEM;

	ret = of_property_read_u32(node, "aes_sw_clk_ctrl_reg",
			     &aes_dd->aes_clk_ctrl_reg);
	if (WARN_ON(ret))
		return -ENOMEM;

	ret = of_property_read_u32(node, "aes_sw_clk_ctrl_bit",
			     &aes_dd->aes_clk_ctrl_bit);
	if (WARN_ON(ret))
		return -ENOMEM;

	aes_dd->dev = dev;

	platform_set_drvdata(pdev, aes_dd);

	INIT_LIST_HEAD(&aes_dd->list);
	spin_lock_init(&aes_dd->lock);

	tasklet_init(&aes_dd->done_task, mc_aes_done_task,
					(unsigned long)aes_dd);
	tasklet_init(&aes_dd->queue_task, mc_aes_queue_task,
					(unsigned long)aes_dd);

	crypto_init_queue(&aes_dd->queue, MC_AES_QUEUE_LENGTH);

	/* Get the base address */
	aes_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!aes_res) {
		dev_err(dev, "no MEM resource info\n");
		err = -ENODEV;
		goto res_err;
	}
	aes_dd->phys_base = aes_res->start;

	/* Get the IRQ */
	aes_dd->irq = platform_get_irq(pdev,  0);
	if (aes_dd->irq < 0) {
		dev_err(dev, "no IRQ resource info\n");
		err = aes_dd->irq;
		goto res_err;
	}

	err = devm_request_irq(&pdev->dev, aes_dd->irq, mc_aes_irq,
			       IRQF_SHARED, "mc-aes", aes_dd);
	if (err) {
		dev_err(dev, "unable to request aes irq.\n");
		goto res_err;
	}

	/* Initializing the clock */
	err = clk_mini_enable(CKG_AES_EN);
	if(err)
		goto res_err;

	aes_dd->io_base = devm_ioremap_resource(&pdev->dev, aes_res);
	if (IS_ERR(aes_dd->io_base)) {
		dev_err(dev, "can't ioremap\n");
		err = PTR_ERR(aes_dd->io_base);
		goto res_err;
	}
	pm_runtime_enable(dev);
	err = pm_runtime_resume_and_get(dev);
	if (err < 0) {
		dev_err(dev, "%s: failed to get_sync(%d)\n",
			__func__, err);
		goto err_pm_disable;
	}

	aes_dd->efuse_key = false;
	mc_aes_hw_init(aes_dd);

	err = of_property_read_u32(dev->of_node, "molchip,dma-rx-id",
			     &aes_dd->rx_dma_id);
	if (err)
		dev_err(dev,"aes get dma rx channel id failed\n");

	err = of_property_read_u32(dev->of_node, "molchip,dma-tx-id",
			     &aes_dd->tx_dma_id);
	if (err)
		dev_err(dev,"aes get dma tx channel id failed\n");

	err = mc_aes_buff_init(aes_dd);
	if (err)
		goto err_aes_buff;

	#ifdef USE_EXT_DMA
		err = mc_aes_dma_init(aes_dd);
		if (err){
			goto err_aes_buff;
		}
	#endif

	spin_lock(&mc_aes.lock);
	list_add_tail(&aes_dd->list, &mc_aes.dev_list);
	spin_unlock(&mc_aes.lock);
	dma_set_mask_and_coherent(dev, 0xFFFFFFFFFFFFFFFF);

	err = mc_aes_register_algs(aes_dd);
	if (err)
		goto err_algs;

	dev_info(dev, "mc crypto supports aes des tdes\n");

	return 0;

err_algs:
	spin_lock(&mc_aes.lock);
	list_del(&aes_dd->list);
	spin_unlock(&mc_aes.lock);
	mc_aes_buff_cleanup(aes_dd);
err_aes_buff:
	clk_mini_disable(CKG_AES_EN);
err_pm_disable:
		pm_runtime_disable(dev);
res_err:
	tasklet_kill(&aes_dd->done_task);
	tasklet_kill(&aes_dd->queue_task);
aes_dd_err:
	if (err != -EPROBE_DEFER)
		dev_err(dev, "initialization failed.\n");

	return err;
}

static int mc_aes_remove(struct platform_device *pdev)
{
	struct mc_aes_dev *aes_dd;

	aes_dd = platform_get_drvdata(pdev);
	if (!aes_dd)
		return -ENODEV;
	spin_lock(&mc_aes.lock);
	list_del(&aes_dd->list);
	spin_unlock(&mc_aes.lock);

	mc_aes_unregister_algs(aes_dd);

	tasklet_kill(&aes_dd->done_task);
	tasklet_kill(&aes_dd->queue_task);

	mc_aes_buff_cleanup(aes_dd);

	clk_unprepare(aes_dd->iclk);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int mc_aes_suspend(struct device *dev)
{
	printk("mc_aes_suspend done\r\n");
	clk_mini_disable(CKG_AES_EN);
	pm_runtime_put_sync(dev);
	return 0;
}
static int mc_aes_resume(struct device *dev)
{
	int err ;
	printk("mc_aes_resume done\r\n");
	err = clk_mini_enable(CKG_AES_EN);
	if(err)
		printk(" clk get err \r\n");
	pm_runtime_get_sync(dev);
	return 0;
}
#endif
static SIMPLE_DEV_PM_OPS(molchip_aes_pm_ops,
			 mc_aes_suspend, mc_aes_resume);
static struct platform_driver mc_aes_driver = {
	.probe		= mc_aes_probe,
	.remove		= mc_aes_remove,
	.driver		= {
		.name	= "mc_aes",
		.of_match_table = of_match_ptr(mc_aes_dt_ids),
		.pm	= &molchip_aes_pm_ops,
	},
};

module_platform_driver(mc_aes_driver);

