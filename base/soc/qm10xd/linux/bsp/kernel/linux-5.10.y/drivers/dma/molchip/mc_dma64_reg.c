#include "mc_dma64_reg.h"
#include "mc_dma_lib.h"

void dma64_ch_enable(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	/*ch_enable*/
	value = ch_regs->full_channel_regs.full_chn_cfg;
	value |= CH_ENABLE;
	ch_regs->full_channel_regs.full_chn_cfg = value;
}

void dma64_ch_disable(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	/*ch_enable*/
	value = ch_regs->full_channel_regs.full_chn_cfg;
	value &= ~CH_ENABLE;
	ch_regs->full_channel_regs.full_chn_cfg = value;
}

void dma64_priority_set(struct mc_dma_chan *mcc){

	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	/*ch priority config in dummy cfg*/
	value = ch_regs->full_channel_regs.full_chn_cfg;
	value &= ~(CH_PRIORITY_MASK<<CH_PRIORITY_SHIFT);
	value |= mcc->chan.dma_config.priority<<CH_PRIORITY_SHIFT;
	ch_regs->full_channel_regs.full_chn_cfg = value;	
}

void dma64_blk_len_set(struct mc_dma_chan *mcc,u32 len){
	struct mc_dma_ch_regs   *ch_regs;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	ch_regs->full_channel_regs.full_chn_blk_len &= ~BLK_LEN_MASK;
	ch_regs->full_channel_regs.full_chn_blk_len |= len;
}

void dma64_trsc_len_set(struct mc_dma_chan *mcc,u32 len){
	struct mc_dma_ch_regs   *ch_regs;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	ch_regs->full_channel_regs.full_chn_trsc_len &= ~TRSC_LEN_MASK;
	ch_regs->full_channel_regs.full_chn_trsc_len |= len;
}

void dma64_frag_len_set(struct mc_dma_chan *mcc,u32 len){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_frag_len;
	value &= ~FRAG_LEN_MASK;
	value |= len;
	ch_regs->full_channel_regs.full_chn_frag_len = value;
}

void dma64_llist_en(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	/*config valid llist en configed in dma_init*/
	value = ch_regs->full_channel_regs.full_chn_cfg;
	value |= LLIST_EN;
	ch_regs->full_channel_regs.full_chn_cfg = value;
}

void dma64_interrupt_en(struct mc_dma_chan *mcc,u32 val){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	/*config interrupt enable*/
	value = ch_regs->full_channel_regs.full_chn_int;
	value |= val;
	ch_regs->full_channel_regs.full_chn_int = value;
}

void dma64_llist_ptr_set(struct mc_dma_chan *mcc,struct mc_desc*desc){
	u32 v_h;
	u32 v_l;
	struct mc_dma_ch_regs   *ch_regs;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	v_l = (u32)(desc->txd.phys & 0xFFFFFFFF);
	v_h = (u32)((desc->txd.phys >> 32) & 0xF);
	ch_regs->full_channel_regs.full_chn_llist_ptr = v_l;
	ch_regs->full_channel_regs.full_chn_src_blk_step &= ~(0xF << 28);
	ch_regs->full_channel_regs.full_chn_src_blk_step |= v_h;
}

void dma64_intrc_disable_all(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	/* Enable interrupts */
	value = ch_regs->full_channel_regs.full_chn_int;
	value &= ~CH_ALLINTRC_EN_MASK;
	ch_regs->full_channel_regs.full_chn_int = value;
}

void dma64_send_software_request(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs   *ch_regs;
	u32			value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_req;
	value |=0x1;
	ch_regs->full_channel_regs.full_chn_req = value;
}

void dma64_clk_gate_en(struct mc_dma *mc){
	void __iomem* comreg_io = mc->common_regs; 
	struct mc_dma_common_reg* common_regs;
	common_regs = (struct mc_dma_common_reg*)comreg_io; 	
	/*init dma pause  DMA_AUTOCLKGATE_EN*/	
	common_regs->dma_pause |= DMA_AUTOCLKGATE_EN;
}

void dma64_frag_wait_set(struct mc_dma *mc,u32 val){
	void __iomem* comreg_io = mc->common_regs; 
	struct mc_dma_common_reg* common_regs;
	common_regs = (struct mc_dma_common_reg*)comreg_io; 	
	/*init dma frag wait*/
	common_regs->dma_frag_wait &= ~0xffff;
	common_regs->dma_frag_wait |= val; 
}

void dma64_ch_pause(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_pause;
	value |= CH_PAUSE_EN;
	ch_regs->full_channel_regs.full_chn_pause= value;
}

void dma64_ch_resume(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_pause;
	value &= ~CH_PAUSE_EN;
	ch_regs->full_channel_regs.full_chn_pause= value;
}

u32 dma64_read_reg_ch_pause(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_pause;
	return value;
}

u32 dma64_read_reg_cfg(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_cfg;
	return value;
}

u32 dma64_read_reg_dest(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_dest_addr;
	return value;
}

u32 dma64_read_reg_src(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_src_addr;
	return value;
}
void dma64_write_reg_dest(struct mc_dma_chan *mcc,u32 val){
	struct mc_dma_ch_regs	*ch_regs;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	ch_regs->full_channel_regs.full_chn_dest_addr = val;
}

u32 dma64_read_trsc_len(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs	*ch_regs;
	u32 value;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	value = ch_regs->full_channel_regs.full_chn_trsc_len;
	return value;
}

u32 dma64_get_int_mask_status_reg(struct mc_dma_common_reg * com_regs){
	return com_regs->dma_int_mask_status;
}

u32 dma64_get_full_chn_int(struct mc_dma_ch_regs* ch_regs){
	return ch_regs->full_channel_regs.full_chn_int;
}

void dma64_clear_int(struct mc_dma_ch_regs *ch_regs,u32 val){
	ch_regs->full_channel_regs.full_chn_int = val;
}
void dma64_write_trsc_len(struct mc_dma_chan *mcc,u32 val){
	struct mc_dma_ch_regs	*ch_regs;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	ch_regs->full_channel_regs.full_chn_trsc_len |= val;
}

void dma64_set_src(struct mc_desc *desc,unsigned long val){
	u32 v_h;
	u32 v_l;
	v_l = (u32)(val & 0xFFFFFFFF);
	v_h = (u32)((val >> 32) & 0xF);
	desc->lli.SRC_ADDR = v_l;
	desc->lli.WRAP_PTR &= ~(0xF << 28);
	desc->lli.WRAP_PTR |= v_h;
}

void dma64_set_dst(struct mc_desc *desc,unsigned long val){
	u32 v_h;
	u32 v_l;
	v_l = (u32)(val & 0xFFFFFFFF);
	v_h = (u32)((val >> 32) & 0xF);
	desc->lli.DEST_ADDR = v_l;
	desc->lli.WRAP_TO &= ~(0xF << 28);
	desc->lli.WRAP_TO |= v_h;
}

void dma64_set_trsc_len(struct mc_desc *desc,u32 len){
	desc->lli.TRANS_LEN &= ~TRSC_LEN_MASK;
	desc->lli.TRANS_LEN |=len;
}

u32 dma64_get_trsc_len(struct mc_desc *desc){
	u32 value;
	value =	desc->lli.TRANS_LEN;
	value &= TRSC_LEN_MASK;
	return value;
}

void dma64_set_blk_len(struct mc_desc *desc,u32 len){
	desc->lli.BLK_LEN &= ~BLK_LEN_MASK;
	desc->lli.BLK_LEN |= len;
}

void dma64_set_frag_len(struct mc_desc *desc,u32 len){
	desc->lli.FRAG_LEN &= ~FRAG_LEN_MASK;
	desc->lli.FRAG_LEN |= len;
}

void dma64_set_req_mode(struct mc_desc *desc,u32 val){
	desc->lli.FRAG_LEN &= ~(REQ_MODE_MASK<<REQ_MODE_SHIFT);
	desc->lli.FRAG_LEN |= val<<REQ_MODE_SHIFT;
}

void dma64_set_ahbsize(struct mc_desc *desc,u32 src_size,u32 dest_size){
	desc->lli.FRAG_LEN &=~(SRC_DST_AHB_SIZE_MASK<<DEST_AHB_SIZE_SHIFT);
	desc->lli.FRAG_LEN |= src_size<< SRC_AHB_SIZE_SHIFT|dest_size<<DEST_AHB_SIZE_SHIFT;
}

void dma64_fix_disable(struct mc_desc *desc){
	desc->lli.FRAG_LEN &= ~(1<<ADDR_FIX_EN_SHIFT);
}

void dma64_fix_enable(struct mc_desc *desc,u32 val){
	desc->lli.FRAG_LEN |= 1<<ADDR_FIX_EN_SHIFT;
	if(val == DST_FIX)
		desc->lli.FRAG_LEN |= 1<<ADDR_FIX_SEL_SHIFT;
	else if(val == SRC_FIX)
		desc->lli.FRAG_LEN &= ~(1<<ADDR_FIX_SEL_SHIFT);

}

void dma64_wrap_disable(struct mc_desc *desc){
	desc->lli.FRAG_LEN &= ~(1<<ADDR_WRAP_EN_SHIFT);
}

void dma64_set_trsf_step(struct mc_desc *desc,u32 src_step,u32 dest_step){
	desc->lli.TRSF_STEP &= ~TRSF_STEP_MASK;
	desc->lli.TRSF_STEP |= dest_step<<DST_TRSF_STEP_SHIFT|src_step;
}

void dma64_set_step(struct mc_desc *desc,u32 frag_step,u32 src_blk_step,u32 dest_blk_step){
	desc->lli.FRAG_STEP = frag_step;
	desc->lli.SRC_BLK_STEP = src_blk_step;
	desc->lli.DEST_BLK_STEP = dest_blk_step;
}

void dma64_set_llist_end(struct mc_desc *desc,u32 val){
	if(val == LLIST_END)
		desc->lli.FRAG_LEN |= 1<<LLIST_END_SHIFT;
	else
		desc->lli.FRAG_LEN &= ~(1<<LLIST_END_SHIFT);
}

void dma64_set_llist_node_intrc_en_disable(struct mc_desc *desc){
	desc->lli.TRANS_LEN &= ~CHN_TRSC_LEN_LLIST_NODE_INT_EN;
}

void dma64_set_llist_ptr(struct mc_desc *desc,unsigned long val){
	u32 v_h;
	u32 v_l;
	v_l = (u32)(val & 0xFFFFFFFF);
	v_h = (u32)((val >> 32) & 0xF);
	desc->lli.LLIST_PTR = v_l;
	desc->lli.SRC_BLK_STEP &= ~(0xF << 28);
	desc->lli.SRC_BLK_STEP |= v_h;
}

void dma64_set_llist_node_intrc_en_enable(struct mc_desc *desc,u32 val){
	desc->lli.TRANS_LEN |=CHN_TRSC_LEN_LLIST_NODE_INT_EN;
	desc->lli.TRANS_LEN |= val;
}


