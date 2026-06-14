#ifndef _MC_DMA64_REG_H
#define _MC_DMA64_REG_H

#include <linux/dma/mc_dma64.h>
#include "mc_dma_lib.h"

void dma64_ch_enable(struct mc_dma_chan *mcc);
void dma64_ch_disable(struct mc_dma_chan *mcc);
void dma64_priority_set(struct mc_dma_chan *mcc);
void dma64_blk_len_set(struct mc_dma_chan *mcc,u32 len);
void dma64_trsc_len_set(struct mc_dma_chan *mcc,u32 len);
void dma64_frag_len_set(struct mc_dma_chan *mcc, u32 len);
void dma64_llist_en(struct mc_dma_chan *mcc);
void dma64_interrupt_en(struct mc_dma_chan *mcc,u32 val);
void dma64_llist_ptr_set(struct mc_dma_chan *mcc,struct mc_desc*desc);
u32 dma64_get_int_mask_status_reg(struct mc_dma_common_reg * com_regs);
u32 dma64_get_full_chn_int(struct mc_dma_ch_regs* ch_regs);
void dma64_clear_int(struct mc_dma_ch_regs *ch_regs,u32 val);

void dma64_intrc_disable_all(struct mc_dma_chan *mcc);
void dma64_clk_gate_en(struct mc_dma *mc);
void dma64_frag_wait_set(struct mc_dma *mc,u32 val);
void dma64_ch_pause(struct mc_dma_chan *mcc);
void dma64_ch_resume(struct mc_dma_chan *mcc);
void dma64_send_software_request(struct mc_dma_chan *mcc);

u32 dma64_read_reg_cfg(struct mc_dma_chan *mcc);
u32 dma64_read_reg_ch_pause(struct mc_dma_chan *mcc);
u32 dma64_read_reg_dest(struct mc_dma_chan *mcc);
u32 dma64_read_reg_src(struct mc_dma_chan *mcc);
u32 dma64_read_trsc_len(struct mc_dma_chan *mcc);
u32 dma64_get_trsc_len(struct mc_desc *desc);
void dma64_write_trsc_len(struct mc_dma_chan *mcc,u32 val);
void dma64_write_reg_dest(struct mc_dma_chan *mcc,u32 val);
void dma64_set_src(struct mc_desc *desc,unsigned long val);
void dma64_set_dst(struct mc_desc *desc,unsigned long val);
void dma64_set_trsc_len(struct mc_desc *desc,u32 len);
void dma64_set_blk_len(struct mc_desc *desc,u32 len);
void dma64_set_frag_len(struct mc_desc *desc,u32 len);
void dma64_set_req_mode(struct mc_desc *desc,u32 val);
void dma64_set_ahbsize(struct mc_desc *desc,u32 src_size,u32 dest_size);
void dma64_fix_disable(struct mc_desc *desc);
void dma64_fix_enable(struct mc_desc *desc,u32 val);
void dma64_wrap_disable(struct mc_desc *desc);
void dma64_set_trsf_step(struct mc_desc *desc,u32 src_step,u32 dest_step);
void dma64_set_step(struct mc_desc *desc,u32 frag_step,u32 src_blk_step,u32 dest_blk_step);
void dma64_set_llist_end(struct mc_desc *desc,u32 val);
void dma64_set_llist_node_intrc_en_disable(struct mc_desc *desc);
void dma64_set_llist_ptr(struct mc_desc *desc,unsigned long val);
void dma64_set_llist_node_intrc_en_enable(struct mc_desc *desc,u32 val);
#endif
