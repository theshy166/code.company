#ifndef _MC_DMA_LIB_H
#define _MC_DMA_LIB_H


#include <linux/interrupt.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include "../dmaengine.h"
#include "molchip.h"
#include <linux/dma/mc_dma64.h>

#define VALIDATION_MAGIC_NUMBER				0x20150113
#define NOT_VALIDATION_MAGIC_NUMBER			0x43243243
#define FREE_MAGIC_NUMBER				0x65734568
#define NOT_FREE_MAGIC_NUMBER				0x79879854

#define MC_DMA64_MAX_NR_CHANNELS			32
#define MC_CHAN_REGS_OFFSET				0x40

#define CHN_TRSC_LEN_LLIST_NODE_INT_EN			(0x1<<31)
#define CHN_TRSC_LEN_LLIST_NODE_INT_FRAG		(0x1<<28)
#define CHN_TRSC_LEN_LLIST_NODE_INT_BLK			(0x1<<29)
#define CHN_TRSC_LEN_LLIST_NODE_INT_TRSC		(0x1<<30)

#define CH_FRAGMENT_INT_EN				(0x1<<0)
#define CH_BLK_INT_EN					(0x1<<1)
#define CH_TRSC_INT_EN					(0x1<<2)
#define CH_LLIST_INT_EN					(0x1<<3)
#define CH_CFG_ERR_INT_EN				(0x1<<4)

#define CH_ALLINTRC_EN_MASK				CH_FRAGMENT_INT_EN|\
							CH_BLK_INT_EN|\
							CH_TRSC_INT_EN|\
							CH_LLIST_INT_EN|\
							CH_CFG_ERR_INT_EN

#define	CHN_FRAGMENT_INT_MASK				(0x1<<16)
#define	CHN_TRANS_INT_MASK				(0x1<<18)
#define CHN_LLIST_INT_MASK				(0x1<<19)

#define CHN_FRAGMENT_INT_CLR				(0x1<<24)
#define CHN_BLK_INT_CLR					(0x1<<25)
#define CHN_TRANSACTION_CLR				(0x1<<26)
#define CHN_LINKLIST_CLR				(0x1<<27)
#define CHN_CFG_ERR_INT_CLR				(0x1<<28)

#define CH_ALLINTRC_CLR_MASK				CHN_FRAGMENT_INT_CLR|\
							CHN_BLK_INT_CLR|\
							CHN_TRANSACTION_CLR|\
							CHN_LINKLIST_CLR|\
							CHN_CFG_ERR_INT_CLR

#define REQ_MODE_SHIFT					24

#define SRC_AHB_SIZE_SHIFT				30
#define DEST_AHB_SIZE_SHIFT				28

#define DMA_AUTOCLKGATE_EN				0x3f0000
#define DMA_FRAG_WAIT					0x0

#define LLIST_END					1
#define NOT_LLIST_END					0
#define LLIST_END_SHIFT					19
#define ADDR_FIX_EN_SHIFT				20
#define SRC_FIX					    	0
#define DST_FIX						1
#define ADDR_FIX_SEL_SHIFT				21
#define ADDR_WRAP_EN_SHIFT				22

#define DST_TRSF_STEP_SHIFT				16

#define BLK_LEN_DUMMY					10
#define TRSC_LEN_DUMMY					10
#define FRAG_LEN_DUMMY					10

#define LLIST_EN					(1<<4)
#define CH_PRIORITY_SHIFT				12
#define CH_PRIORITY_MASK				3
#define CH_ENABLE					1

#define CH_PAUSE_EN					1
#define CH_PAUSE_STATE					(1<<2)

#define BLK_LEN_MASK					0x1ffff;
#define FRAG_LEN_MASK					0x1ffff;
#define TRSC_LEN_MASK					0xfffffff;

#define REQ_MODE_MASK					0x3
#define SRC_DST_AHB_SIZE_MASK				0xf

#define TRSF_STEP_MASK					0xffffffff
#define WORD_STEP					0x4
#define REQ_DMA_SET					1

#define LEN_BOUND					0x200
#define FRAGMENT_S					0x4
#define FRAGMENT_B					0x100

#define	MCC_DMA_START					0
#define MCC_DMA_COMPLETE				1


enum mc_dma_req_mode{
	FRAGMENT,
	BLOCK,
	TRANSACTION,
	LINKLIST
};

enum mc_dma_ahb_size{
	BYTE,
	HALFWORD,
	WORD,
	DWORD
};


enum mc_dmac_flags{
	MC_DMA_IS_CYCLIC = 0,
	MC_DMA_IS_SOFT_LLP = 1,
	MC_DMA_IS_PAUSED = 2,
	MC_DMA_IS_INITALIZED = 3,
};

enum{
	DMA_INTRC_IDLE,
	DMA_INTRC_BUSY,	
};

struct mc_dma_common_reg{
	volatile u32 dma_pause;
	volatile u32 dma_frag_wait;
	volatile u32 req_pend0_en;
	volatile u32 req_pend1_en;
	volatile u32 dma_int_raw_status;
	volatile u32 dma_int_mask_status;
	volatile u32 dma_req_status;
	volatile u32 dma_en_status;
	volatile u32 dma_debug_status;
	volatile u32 dma_arb_sel_status;
	volatile u32 chn_start_chn_cfg_1grp;
	volatile u32 chn_start_chn_cfg_2grp;
	volatile u32 dma_chn_arprot;
	volatile u32 dma_chn_awprot;
	volatile u32 dma_chn_prot_flag;
	volatile u32 dma_glb_prot;
	volatile u32 dma_pend0_prot;
	volatile u32 dma_pend1_prot;
	volatile u32 dma_reqid0_prot;
	volatile u32 dma_reqid1_prot;
	volatile u32 sync_sec_n_normal;
	volatile u32 cnt_chn_sel;
	volatile u32 total_trsf_byte_cnt1;
	volatile u32 total_trsf_byte_cnt2;
};

struct mc_dma_std_channel_regs{
	volatile u32 std_chn_pause;
	volatile u32 std_chn_req;
	volatile u32 std_chn_cfg;
	volatile u32 std_chn_int;
	volatile u32 std_chn_src_addr;
	volatile u32 std_chn_dest_addr;
	volatile u32 std_chn_frag_len;
	volatile u32 std_chn_blk_len;
};

struct mc_dma_full_channel_regs{
	volatile u32 full_chn_pause;
	volatile u32 full_chn_req;
	volatile u32 full_chn_cfg;
	volatile u32 full_chn_int;
	volatile u32 full_chn_src_addr;
	volatile u32 full_chn_dest_addr;
	volatile u32 full_chn_frag_len;
	volatile u32 full_chn_blk_len;
	volatile u32 full_chn_trsc_len;
	volatile u32 full_chn_trsf_step;
	volatile u32 full_chn_wrap_ptr;
	volatile u32 full_chn_wrap_to;
	volatile u32 full_chn_llist_ptr;
	volatile u32 full_chn_frag_step;
	volatile u32 full_chn_src_blk_step;
	volatile u32 full_chn_dest_blk_step;
};

struct mc_dma_ch_regs{
	union{
		struct mc_dma_std_channel_regs	std_channel_regs;
		struct mc_dma_full_channel_regs full_channel_regs;
	};
};

struct mc_dma_req_cid_regs{
	volatile u32 req0_cid;
	volatile u32 req1_cid;
	volatile u32 req2_cid;
	volatile u32 req3_cid;
	volatile u32 req4_cid;
	volatile u32 req5_cid;
	volatile u32 req6_cid;
	volatile u32 req7_cid;
	volatile u32 req8_cid;
	volatile u32 req9_cid;
	volatile u32 req10_cid;
	volatile u32 req11_cid;
	volatile u32 req12_cid;
	volatile u32 req13_cid;
	volatile u32 req14_cid;
	volatile u32 req15_cid;
	volatile u32 req16_cid;
	volatile u32 req17_cid;
	volatile u32 req18_cid;
	volatile u32 req19_cid;
	volatile u32 req20_cid;
	volatile u32 req21_cid;
	volatile u32 req22_cid;
	volatile u32 req23_cid;
	volatile u32 req24_cid;
	volatile u32 req25_cid;
	volatile u32 req26_cid;
	volatile u32 req27_cid;
	volatile u32 req28_cid;
	volatile u32 req29_cid;
	volatile u32 req30_cid;
	volatile u32 req31_cid;
};
struct device *chan2dev(struct dma_chan *chan);
struct  mc_dma *to_mc_dma_from_work(struct work_struct *work);

void mc_dma_off(struct mc_dma *mc);
void mc_dma_on(struct mc_dma *mc);
void channel_disable(struct mc_dma_chan *mcc);
void clear_interrupt_all_chan(struct mc_dma *mc);
void mcc_scan_descriptors(struct mc_dma *mc,struct mc_dma_chan *mcc);

int mc_alloc_chan_resources(struct dma_chan *chan);
void mc_free_chan_resources(struct dma_chan *chan);
struct dma_async_tx_descriptor*
		mc_prep_dma_memcpy(struct dma_chan* chan,dma_addr_t dest,dma_addr_t src,
				size_t len,unsigned long flags);
struct dma_async_tx_descriptor* 
		mc_prep_slave_sg(struct dma_chan *chan,struct scatterlist *sgl,
			unsigned int sg_len,enum dma_transfer_direction direction,
			unsigned long flags,void *context);
struct mc_cyclic_desc* 
		mc_dma_cyclic_prep(struct dma_chan *chan,dma_addr_t buf_addr,size_t buf_len,
				size_t period_len,enum dma_transfer_direction direction,unsigned long flags);
struct dma_async_tx_descriptor* 
		mc_prep_dma_sg(struct dma_chan *chan,
		struct scatterlist *dst_sg,unsigned int dst_nents,
		struct scatterlist *src_sg,unsigned int src_nents,
		unsigned long flags);
int mc_config(struct dma_chan *chan,struct dma_slave_config *sconfig);
int mc_pause(struct dma_chan* chan);
int mc_resume(struct dma_chan *chan);
int mcc_terminate_all(struct dma_chan *chan);
enum dma_status mcc_tx_status(struct dma_chan *chan,dma_cookie_t cookie,struct dma_tx_state *txstate);
void mc_dma_start(struct dma_chan *chan);
void mc_dma_cyclic_free(struct dma_chan *chan);
void mc_dma_intrc_disable(struct dma_chan *chan);
void mc_dma_chan_enable(struct dma_chan *chan);
void mc_dma_chan_req(struct dma_chan *chan);
u32 mc_dma_read_dest_addr(struct dma_chan *chan);
u32 mc_dma_read_trsc_len(struct dma_chan *chan);
void mc_dma_write_dest_addr(struct dma_chan *chan,u32 val);
void mc_dma_write_trsc_len(struct dma_chan *chan,u32 val);
u32 mc_dma_read_cfg(struct dma_chan *chan);
void mcc_descriptor_reuse_complete(struct mc_dma_chan *mcc,struct mc_desc *desc,
		bool callback_required);
void mcc_start_direct(struct dma_chan *chan);
void mc_cyclic_desc_set(struct dma_chan *chan);
#endif
