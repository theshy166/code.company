#include <stdlib.h>
#include <malloc.h>
#include "sstring.h"
#include "glb_reg.h"
#include "system.h"
#include "uart_drv.h"
#include "spic.h"
#include "spi_nor_flash.h"
#include "secure.h"
#include "dma.h"
#include "pkcs5.h"
#include "rsa.h"
#include "bignum.h"
#include "lzma.h"
#include "efuse_drv.h"
#include "aes.h"
#include "spic_reg.h"
#include "mc_hw_rsa.h"
#include "hash.h"
#include "sha.h"
#include "sched_internal.h"

static struct sched_node sfc_node;
static struct sched_node aes_node;
static struct sched_node lzma_node;
static struct sched_node hash_node;
static struct sched_node mem_node;
#define AES_ROUNDUP(x, y)		((x + ((y)-1)) & ~((y)-1))


static struct sched_buffer *sched_buffer_get(struct sched_node *node, BUF_STATE_E state)
{
	struct sched_buffer *idle_buf = NULL;
	if (node) {
		if (state == node->pp_buf[0].state) {
			idle_buf = &node->pp_buf[0];
		} else {
			if (state == node->pp_buf[1].state) {
				idle_buf = &node->pp_buf[1];
			}
		}
	}
	return idle_buf;
}

static struct sched_buffer *sched_buffer_ready(struct sched_node *cur, struct sched_node *prev)
{
	int idx;
	struct sched_buffer *buf = NULL;
	if ((prev) && (cur)) {
		for (idx = 0; idx < 2; idx++) {
			if (BUF_STS_READY == prev->pp_buf[idx].state) {
				if (cur->runtime.seq == prev->pp_buf[idx].seq) {
					buf = &prev->pp_buf[idx];
					break;
				}
			}
		}
	}
	return buf;
}

static void pp_buffer_init(struct sched_buffer *pp, void *base)
{
	pp->buffer = base;
	pp->seq = 0;
	pp->state = BUF_STS_IDLE;
}

static unsigned int _sched_work_mode(u32 sfc, u32 aes, u32 lzma, u32 hash, u32 mem)
{
	unsigned int work_mode = 0;
	if (mem) {
		work_mode |= (1 << 4);
	}
	if (sfc) {
		work_mode |= (1 << 3);
	}
	if (aes) {
		work_mode |= (1 << 2);
	}
	if (lzma) {
		work_mode |= (1 << 1);
	}
	if (hash) {
		work_mode |= (1 << 0);
	}
	return work_mode;
}

 int cpu_sched_mode_init(struct boot_header *header, u32 sfc, u32 aes, u32 lzma, u32 hash, u32 mem)
{
	int ret = 0;
	unsigned int work_mode = _sched_work_mode(sfc, aes, lzma, hash, mem);
	switch (work_mode) {
		case 0x5:
			/* eMMC: iram->aes->iram->hash. */
			uart_puts("here case 5\r\n");
			AES_NODE_INIT(header, CONTI_MEM_MODE, CONTI_MEM_MODE, header->decode_length);
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;

		case 0x3:
			/* eMMC: iram->lzma->iram->hash. */
			LZMA_NODE_INIT(header, CONTI_MEM_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;

		case 0x6:
			/* eMMC: iram->aes->lzma->iram. */
			AES_NODE_INIT(header, CONTI_MEM_MODE, PP_BUF_MODE, header->decode_length);
			LZMA_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			break;

		case 0x7:
			/* eMMC: iram->aes->lzma->iram->hash. */
			AES_NODE_INIT(header, CONTI_MEM_MODE, PP_BUF_MODE, header->decode_length);
			LZMA_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;

		case 0xa:
			/* sfc->lzma->iram. */
			SFC_NODE_INIT(header, PP_BUF_MODE, header->lzma_zip_len);
			LZMA_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			break;

		case 0xb:
			/* support: sfc->lzma->iram->hash. */
			SFC_NODE_INIT(header, PP_BUF_MODE, header->lzma_zip_len);
			LZMA_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;

		case 0xd:
			/* sfc->aes->iram->hash. */
			SFC_NODE_INIT(header, PP_BUF_MODE, header->decode_length);
			AES_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->decode_length);
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;

		case 0xe:
			/* sfc->aes->lzma->iram. */
			SFC_NODE_INIT(header, PP_BUF_MODE, header->lzma_zip_len);
			AES_NODE_INIT(header, PP_BUF_MODE, PP_BUF_MODE, header->decode_length);
			LZMA_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			break;

		case 0xf:
			/* support: sfc->aes->lzma->iram->hash. */
			SFC_NODE_INIT(header, PP_BUF_MODE, header->decode_length);
			AES_NODE_INIT(header, PP_BUF_MODE, PP_BUF_MODE, header->decode_length);
			LZMA_NODE_INIT(header, PP_BUF_MODE, CONTI_MEM_MODE, header->lzma_zip_len);
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;


		case 0x1:
			HASH_NODE_INIT(header, CONTI_MEM_MODE, header->length);
			break;
		case 0x15:
			AES_NODE_INIT(header, CONTI_MEM_MODE, CONTI_MEM_MODE, header->decode_length);
			VMEM_NODE_INIT(header, CONTI_MEM_MODE, header->decode_length);
			VMEM_HASH_NODE_INIT(header, CONTI_MEM_MODE, header->decode_length);
			break;
		case 0x0:
		case 0x2:
		case 0x4:
		case 0x8:
		case 0x9:
		case 0xc:
		default:
			ret = -1;
			break;
	}

	return ret;
}
 
int fast_boot_init(struct boot_header *header, u32 sfc, u32 aes, u32 lzma, u32 hash, u32 mem)
{
	int ret = 0;
	int size = 0;
	int idx, cnt = 0;
	struct sched_node *nodes[5];
	struct sched_node *prev_node = NULL, *next_node = NULL;
	unsigned int buf_size = header->granularity;

	ret = cpu_sched_mode_init(header, sfc, aes, lzma, hash, mem);
	if (ret) {
		goto exit_ret;
	}
	if (sfc) {
		void *sfc_buffer = NULL;
		sfc_node.state = MOD_STS_IDLE;
		sfc_node.runtime.seq = 0;

		/* Pingpong buffer init. */
		
		#ifdef CONFIG_SPL_BUILD
			sfc_buffer = smalloc(buf_size * 2);
		#endif
		
		if (sfc_buffer) {
			void *p_buf = (void *)(((unsigned int)sfc_buffer) + buf_size);
			pp_buffer_init(&(sfc_node.pp_buf[0]), sfc_buffer);
			pp_buffer_init(&(sfc_node.pp_buf[1]), p_buf);
		}

		nodes[cnt++] = &sfc_node;
	} else {
		sfc_node.state = MOD_STS_INVALID;
	}

	if(mem) {
		mem_node.state = MOD_STS_IDLE;
		mem_node.runtime.seq = 0;
		pp_buffer_init(&(mem_node.pp_buf[0]), NULL);
		pp_buffer_init(&(mem_node.pp_buf[1]), NULL);
		
		nodes[cnt++] = &mem_node;

	} else {
		mem_node.state = MOD_STS_INVALID;
	}

	if (aes) {	

		void *aes_buffer = NULL;
		aes_init();
		aes_decrypt(true);
		size = AES_ROUNDUP(header->decode_length, 16);

		*(volatile unsigned int*)0x0c600050 = size;
		aes_node.state = MOD_STS_IDLE;
		aes_node.runtime.seq = 0;

		/* Pingpong buffer init. */
		if (PP_BUF_MODE == aes_node.out_mode) {
		
			#ifdef CONFIG_SPL_BUILD
			  aes_buffer = smalloc(buf_size * 2);
			#endif
		
			if (aes_buffer) {
				void *p_buf = (void *)(((unsigned int)aes_buffer) + buf_size);
				pp_buffer_init(&(aes_node.pp_buf[0]), aes_buffer);
				pp_buffer_init(&(aes_node.pp_buf[1]), p_buf);
			}
		}

		nodes[cnt++] = &aes_node;
	} else {
		aes_node.state = MOD_STS_INVALID;
	}

	if (lzma) {
		
		lzma_sch_init(header);

		lzma_node.state = MOD_STS_IDLE;
		lzma_node.runtime.seq = 0;
		/* TODO: remove in future. */
		lzma_node.dst_base = header->lzma_dst;

		/* Pingpong buffer init. */
		pp_buffer_init(&(lzma_node.pp_buf[0]), NULL);
		pp_buffer_init(&(lzma_node.pp_buf[1]), NULL);

		nodes[cnt++] = &lzma_node;
	} else {
		lzma_node.state = MOD_STS_INVALID;
	}

	if (hash) {

		hash_enable((header->sel >> 29) & 0x3);

		hash_sched_init(20, header->length);
	
		hash_node.state = MOD_STS_IDLE;
		hash_node.runtime.seq = 0;

		/* Pingpong buffer init. */
		pp_buffer_init(&(hash_node.pp_buf[0]), NULL);
		pp_buffer_init(&(hash_node.pp_buf[1]), NULL);

		nodes[cnt++] = &hash_node;
	} else {
		hash_node.state = MOD_STS_INVALID;
	}

	prev_node = nodes[0];
	for (idx = 1; idx < cnt; idx++) {
		next_node = nodes[idx];
		next_node->prev = prev_node;
		prev_node->next = next_node;
		prev_node = next_node;
	}
	next_node->next = NULL;

exit_ret:
	return ret;
}


static void __seq_update(struct sched_node_runtime *rt_ctx, struct sched_buffer *buffer)
{
	if (rt_ctx) {
		if (buffer) {
			buffer->seq = rt_ctx->seq;
		}
		rt_ctx->seq += 1;
	}
}

static int sfc_state_switch(struct boot_header *header)
{
	int ret = 0;
	int single_done = 0;
	struct sched_buffer *idle_buf = NULL;
	unsigned int remain_size, single_transfer_size = header->granularity;
	struct sched_node_runtime *rt_ctx = &sfc_node.runtime;

	if (MOD_STS_BUSY == sfc_node.state) {
		/* check whether module is done. */
		single_done = sfc_transfer_done();
		if (single_done) {
			sfc_node.state = MOD_STS_IDLE;
			sfc_node.finished_size += rt_ctx->cur_handle_size;
			sfc_node.src_base += rt_ctx->cur_handle_size;
			if (rt_ctx->cur_buf) {
				rt_ctx->cur_buf->state = BUF_STS_READY;
			}
		}
	}

	if (MOD_STS_IDLE == sfc_node.state) {
		/* check whether finish */
		if (sfc_node.finished_size < sfc_node.total_size) {
			/* check whether self buffer idle */
			idle_buf = sched_buffer_get(&sfc_node, BUF_STS_IDLE);
			if (idle_buf) {
				/* config & start new action. */
				remain_size = sfc_node.total_size - sfc_node.finished_size;
				if (single_transfer_size > remain_size) {
					single_transfer_size = remain_size;
				}
				sfc_transfer_start(sfc_node.src_base, idle_buf->buffer, single_transfer_size);
				rt_ctx->cur_handle_size = single_transfer_size;
				idle_buf->state = BUF_STS_BUSY;
				sfc_node.state = MOD_STS_BUSY;
				rt_ctx->cur_buf = idle_buf;
				__seq_update(rt_ctx, idle_buf);
			}
		} else {
			sfc_node.state = MOD_STS_DONE;
		}
	} else {
		/* nothing to do. */
	}

	return ret;
}
extern volatile  unsigned int aes_start_cnt;

bool vmem_transfer_done(struct boot_header *header)
{
	u32 ret = 0;
	u32 last_base = mem_node.src_base + mem_node.runtime.cur_handle_size;
	u32 *buf_ptr = (u32 *)(last_base + header->granularity);
	if ((true == sdhci_data_tran_done(0))|| (*buf_ptr != ENTRY_BAD_ADDRESS)) {
		ret = 1;
	}
	return ret;
}
static int vmem_state_switch(struct boot_header *header)
{
	int ret = 0;
	u32 last_base = 0;
	u32 *buf_ptr = NULL;
	unsigned int remain_size, single_transfer_size = header->granularity;

	if (mem_node.finished_size < mem_node.total_size) {
		remain_size = mem_node.total_size - mem_node.finished_size;
		if (single_transfer_size > remain_size) {
			single_transfer_size = remain_size;
		}
		last_base = mem_node.src_base + mem_node.finished_size;
		buf_ptr = (u32 *)(last_base + single_transfer_size);
		sdhci_data_tran_done(0);
		if ((*buf_ptr != ENTRY_BAD_ADDRESS)) {
			mem_node.finished_size += single_transfer_size;
			mem_node.actual_out_size += single_transfer_size;
		}
		
	} else {
		mem_node.state = MOD_STS_DONE;
	}


	return ret;
}

static int aes_state_switch(struct boot_header *header)
{
	int ret = 0;
	bool single_done = 0;
	unsigned int remain_size, single_transfer_size = header->granularity;
	struct sched_buffer *idle_buf = NULL;
	struct sched_buffer *prev_ready_buf = NULL;
	struct sched_node_runtime *rt_ctx = &aes_node.runtime;

	if (MOD_STS_BUSY == aes_node.state) {
		/* check whether module is done. */
		//single_done = aes_handle_done();
		 single_done = aes_dma_wait_done(2,3);
		if (single_done) {
			aes_node.state = MOD_STS_IDLE;
			aes_node.finished_size += rt_ctx->cur_handle_size;
			aes_node.actual_out_size += rt_ctx->cur_handle_size;
			if (rt_ctx->cur_buf) {
				rt_ctx->cur_buf->state = BUF_STS_READY;
			}
			if (rt_ctx->pre_buf) {
				rt_ctx->pre_buf->state = BUF_STS_IDLE;
			}
		}
	}

	if (MOD_STS_IDLE == aes_node.state) {
		/* check whether finish */
		if (aes_node.finished_size < aes_node.total_size) {
			remain_size = aes_node.total_size - aes_node.finished_size;
			if (remain_size < single_transfer_size) {
				single_transfer_size = remain_size;
			}

			if (PP_BUF_MODE == aes_node.out_mode) {
				/* check whether self buffer idle */
				idle_buf = sched_buffer_get(&aes_node, BUF_STS_IDLE);
				if (idle_buf) {
					if (PP_BUF_MODE == aes_node.in_mode) {
						/* check whether pre module buffer idle */
						prev_ready_buf = sched_buffer_ready(&aes_node, aes_node.prev);
						if (prev_ready_buf) {
							/* config & start new action. */
							//aes_handle_start(prev_ready_buf->buffer, idle_buf->buffer, single_transfer_size);

							aes_handle_start((unsigned int)prev_ready_buf->buffer, (unsigned int)idle_buf->buffer,single_transfer_size);
							rt_ctx->cur_handle_size = single_transfer_size;
							rt_ctx->pre_buf = prev_ready_buf;
							rt_ctx->cur_buf = idle_buf;
							__seq_update(rt_ctx, idle_buf);
							idle_buf->state = BUF_STS_BUSY;
							aes_node.state = MOD_STS_BUSY;
						}
					} else {
						/* check whether pre-node data ready. */
						if (aes_node.prev) {
							/* TODO: what happened? */
						} else {
							/* first level: read data from iram. */
							unsigned int data_in_base = aes_node.src_base + aes_node.finished_size;
							aes_handle_start(data_in_base, (unsigned int)idle_buf->buffer,single_transfer_size);
							//aes_handle_start((void *)data_in_base, idle_buf->buffer, single_transfer_size);
							rt_ctx->cur_handle_size = single_transfer_size;
							rt_ctx->cur_buf = idle_buf;
							idle_buf->state = BUF_STS_BUSY;
							aes_node.state = MOD_STS_BUSY;
						}
					}
				}

			} else if (CONTI_MEM_MODE == aes_node.out_mode) {
				unsigned int data_out_base = aes_node.dst_base + aes_node.finished_size;
				if (PP_BUF_MODE == aes_node.in_mode) {
					/* check whether pre module buffer idle */
					prev_ready_buf =  sched_buffer_ready(&aes_node, aes_node.prev);
					if (prev_ready_buf) {
						/* config & start new action. */
						//aes_handle_start(prev_ready_buf->buffer, (void *)data_out_base, single_transfer_size);

						aes_handle_start((unsigned int)prev_ready_buf->buffer, data_out_base,single_transfer_size);
						rt_ctx->cur_handle_size = single_transfer_size;
						rt_ctx->pre_buf = prev_ready_buf;
						rt_ctx->cur_buf = idle_buf;
						__seq_update(rt_ctx, idle_buf);
						idle_buf->state = BUF_STS_BUSY;
						aes_node.state = MOD_STS_BUSY;
					}
				} else {
					unsigned int ready_size = 0;
					unsigned int data_in_base = aes_node.src_base + aes_node.finished_size;
					//aes_handle_start((void *)data_in_base, (void *)data_out_base, single_transfer_size);
					if ((aes_node.prev) && (aes_node.prev->actual_out_size > aes_node.finished_size)) {
			 			ready_size = aes_node.prev->actual_out_size - aes_node.finished_size;
					}
					if (ready_size >= single_transfer_size) {
						aes_handle_start(data_in_base, data_out_base,single_transfer_size);
						rt_ctx->cur_handle_size = single_transfer_size;
						//rt_ctx->cur_buf = idle_buf;
						//idle_buf->state = BUF_STS_BUSY;
						aes_node.state = MOD_STS_BUSY;
					}
				}
			}

		} else {
			aes_node.state = MOD_STS_DONE;
			aes_start_cnt =0;
		}
	}

	return ret;
}
extern volatile  unsigned int lzma_sched;
extern volatile  unsigned int cur_cnt, last_cnt;

static int lzma_state_switch(struct boot_header *header)
{
	int ret = 0;
	int single_done = 0;
	unsigned int remain_size, single_transfer_size = header->granularity;
	struct sched_buffer *prev_ready_buf = NULL;
	struct sched_node_runtime *rt_ctx = &lzma_node.runtime;

	if (MOD_STS_BUSY == lzma_node.state) {
		/* check whether module is done. */
		single_done = lzma_handle_done(&lzma_node.actual_out_size);
		if (single_done) {
			lzma_node.state = MOD_STS_IDLE;
			lzma_node.finished_size += rt_ctx->cur_handle_size;
			if (rt_ctx->pre_buf) {
				rt_ctx->pre_buf->state = BUF_STS_IDLE;
			}
		}
	}

	if (MOD_STS_IDLE == lzma_node.state) {
		unsigned int data_out_base = lzma_node.dst_base + lzma_node.finished_size;

		/* check whether finish */
		if (lzma_node.finished_size < lzma_node.total_size) {
			remain_size = lzma_node.total_size - lzma_node.finished_size;
			if (remain_size < single_transfer_size) {
				single_transfer_size = remain_size;
			}

			if (PP_BUF_MODE == lzma_node.in_mode) {
				/* check whether pre module buffer idle */
				prev_ready_buf =  sched_buffer_ready(&lzma_node, lzma_node.prev);
				if (prev_ready_buf) {
					/* config & start new action. */
					lzma_handle_start((unsigned int)prev_ready_buf->buffer,(unsigned int)data_out_base, single_transfer_size);
					rt_ctx->cur_handle_size = single_transfer_size;
					rt_ctx->pre_buf = prev_ready_buf;
					//idle_buf->state = BUF_STS_BUSY;
					__seq_update(rt_ctx, NULL);
					lzma_node.state = MOD_STS_BUSY;
				}
			} else {
				unsigned int data_in_base = lzma_node.src_base + lzma_node.finished_size;
				lzma_handle_start((unsigned int)data_in_base, (unsigned int)data_out_base, single_transfer_size);
				rt_ctx->cur_handle_size = single_transfer_size;
				lzma_node.state = MOD_STS_BUSY;
			}
		} else {
			lzma_node.state = MOD_STS_DONE;
			lzma_sched =0;
	
		}
	}

	return ret;
}

extern volatile  unsigned int hash_start_cnt;

 int hash_state_switch(struct boot_header *header)
{
	//volatile unsigned int debug = 0;
	int ret = 0;
	int single_done = 0;
	unsigned int remain_size, single_transfer_size = header->granularity;
	struct sched_node_runtime *rt_ctx = &hash_node.runtime;

	if (MOD_STS_BUSY == hash_node.state) {
		/* check whether module is done. */
		single_done = hash_handle_done(20);
		if (single_done) {
			hash_node.finished_size += rt_ctx->cur_handle_size;
			hash_node.state = MOD_STS_IDLE;
		}

	}

	if (MOD_STS_IDLE == hash_node.state) {
		unsigned int ready_size = 0;
		unsigned int data_in_base = hash_node.src_base + hash_node.finished_size;

		if ((hash_node.prev) && (hash_node.prev->actual_out_size > hash_node.finished_size)) {
			 ready_size = hash_node.prev->actual_out_size - hash_node.finished_size;
			
		}

		/* check whether finish */
		if (hash_node.finished_size < hash_node.total_size) {
			remain_size = hash_node.total_size - hash_node.finished_size;
			if (remain_size < single_transfer_size) {
				
				single_transfer_size = remain_size;
			}
			if (ready_size >= single_transfer_size) {
					
				if (HASH_GLITCH_EXIST == header->rev0) {
					hash_node.state = MOD_STS_BUSY;
					sha256_ctx_continue((const u8 *)data_in_base, single_transfer_size);
					rt_ctx->cur_handle_size = single_transfer_size;
					hash_node.state = MOD_STS_IDLE;
					hash_node.finished_size += rt_ctx->cur_handle_size;
				} else {
					/* config & start new action. */

					hash_handle_start(data_in_base,data_in_base, single_transfer_size);
					rt_ctx->cur_handle_size = single_transfer_size;
					hash_node.state = MOD_STS_BUSY;
				}
			}
		} else {
			hash_node.state = MOD_STS_DONE;
			//hash_node.prev = NULL;
			hash_start_cnt =0;
		}
	}

	return ret;
}

#if 1
static int sched_nor_flash(void)
{
	int ret = 0;
	unsigned int boot_mode = system_boot_mode();
	switch (boot_mode) {
		case NOR_FLASH_BOOT:
		default:
			ret = 1;
			break;
		case NAND_FLASH_BOOT:
		case EMMC_BOOT:
		case SDIO0_BOOT:
		case SDIO1_BOOT:
		case UART0_DOWNLOAD:
		case USB_DOWNLOAD:
			break;
	}
	return ret;
}
#endif

#if 1
static int sched_emmc_vmem(void)
{
	int ret = 0;
	unsigned int boot_mode = system_boot_mode();
	switch (boot_mode) {
		case EMMC_BOOT:
			ret = 1;
			break;
		case NOR_FLASH_BOOT:
		case NAND_FLASH_BOOT:
		case SDIO0_BOOT:
		case SDIO1_BOOT:
		case UART0_DOWNLOAD:
		case USB_DOWNLOAD:
			break;
	}
	return ret;
}
#endif
static int fast_boot_done(void)
{
	int ret = 0;
	do {
		if ((MOD_STS_INVALID != sfc_node.state) && (MOD_STS_DONE != sfc_node.state)) {
			break;
		}
		if ((MOD_STS_INVALID != aes_node.state) && (MOD_STS_DONE != aes_node.state)) {
			break;
		}
		if ((MOD_STS_INVALID != lzma_node.state) && (MOD_STS_DONE != lzma_node.state)) {
			break;
		}
		if ((MOD_STS_INVALID != hash_node.state) && (MOD_STS_DONE != hash_node.state)) {
			break;
		}
		if ((MOD_STS_INVALID != mem_node.state) && (MOD_STS_DONE != mem_node.state)) {
			break;
		}
		ret = 1;
	} while (0);

	return ret;
}

int sched_fast_boot(struct boot_header *header)
{
	int ret = 0;
	int done = 0;
	int sfc, aes, lzma, hash, vmem;

	lzma = header->lzma;
	sfc = sched_nor_flash();
	aes = aes_sel();
	hash = rsa_sel();

	vmem = sched_emmc_vmem();
	if (vmem) {
		if ((0 == aes) || (0 == hash)) {
				vmem = 0;
			}
	}
	ret = fast_boot_init(header, sfc, aes, lzma, hash,vmem);
	if (ret) {
		goto exit_0;
	}

	while (1) {
		if (sfc) {
			ret = sfc_state_switch(header);
		}
		if (aes) {
			ret = aes_state_switch(header);
		}
		if (lzma) {
			ret = lzma_state_switch(header);
		}
		if (hash) {
			ret = hash_state_switch(header);
		}
		if (vmem) {
			ret = vmem_state_switch(header);
		}

		/* check whether the whole process is done. */
		done = fast_boot_done();
		if (done) {
			break;
		}
	}

exit_0:
	return ret;
}
