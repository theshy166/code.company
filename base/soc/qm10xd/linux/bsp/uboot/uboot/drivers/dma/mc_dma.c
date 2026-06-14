#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/dma.h>
#include "../../board/molchip/board/system.h"
#include <malloc.h>
#include <memalign.h>

void molchip_dma_clk_enable(void)
{

#if defined(CONFIG_TARGET_FY01)
		setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AXI_DMA_EN \
			| CKG_AXI_DMA_FORCE_EN | CKG_AXI_DMA_AUTO_GATE_SEL);
#elif defined(CONFIG_TARGET_FY00)
		setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_DMA_EN);
#elif defined(CONFIG_TARGET_FY10D)
		setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_DMA_EN);
#elif defined(CONFIG_TARGET_FY10DS)
		setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_DMA_EN);
#elif defined(CONFIG_TARGET_XC01)
		setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_DMA0_EN);
#endif

}

void dma_reset(void)
{
	u32 v,i;
	v = __raw_readl(REG_DMA0_REG);
	v |= DMA_SOFT_RST;
	__raw_writel(v, REG_DMA0_REG);
	for(i = 0; i< 0x100;i++);
	v &= ~DMA_SOFT_RST;
	__raw_writel(v, REG_DMA0_REG);
}

void init_chn_reg(u32 chn)
{
	u32 i;
	u32 reg;
	reg = FULL_CH_PAUSE(chn);
	for(i = 0; i < 0x10; i ++) {
		if(reg == FULL_CH_INT(chn)){
			reg += 4;
			continue;
		}
		dma_write(0,reg);
		reg += 4;
	}
}

void enable_channel(u32 chn)
{
	u32 val;

	val = dma_read(FULL_CH_CFG(chn));
	val |= CH_EN;
	dma_write(val, FULL_CH_CFG(chn));

}
static void dma_frag_wait(u32 wait)
{
	u32 v;
	v = dma_read(DMA_FRAG_WAIT);
	v &= ~(DMA_FRAG_WAIT_MASK << DMA_FRAG_WAIT_SHIFT);
	wait &= DMA_FRAG_WAIT_MASK;
	v |= (wait << DMA_FRAG_WAIT_SHIFT);
	dma_write(v,DMA_FRAG_WAIT);
}
static void dma_chn_priority(u32 chn,u32 pri)
{
	u32 v;
	v = dma_read(FULL_CH_CFG(chn));
	v &= ~(CH_PRIORITY_MASK << CH_PRIORITY_SHIFT);
	pri &= CH_PRIORITY_MASK;
	v |= (pri << CH_PRIORITY_SHIFT);
	dma_write(v,FULL_CH_CFG(chn));
}
void dma_chn_src(u32 chn,u32 src)
{
	dma_write(src,FULL_CH_SRC_ADDR(chn));
}
void dma_chn_dst(u32 chn,u32 dst)
{
	dma_write(dst,FULL_CH_DST_ADDR(chn));
}
void dma_trsc_len(u32 chn,u32 len)
{
	u32 v;
	v = dma_read(FULL_CH_TRSC_LEN(chn));
	v &= ~(TRSC_LEN_MASK << TRSC_LEN_SHIFT);
	len &= TRSC_LEN_MASK;
	v |= (len << TRSC_LEN_SHIFT);
	dma_write(v,FULL_CH_TRSC_LEN(chn));
	v = dma_read(FULL_CH_BLK_LEN(chn));
	v &= ~(BLK_LEN_MASK << BLK_LEN_SHIFT);
	len &= BLK_LEN_MASK;
	v |= (len << BLK_LEN_SHIFT);
	dma_write(v,FULL_CH_BLK_LEN(chn));
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(FRAG_LEN_MASK << FRAG_LEN_SHIFT);
	len &= FRAG_LEN_MASK;
	v |= (len << FRAG_LEN_SHIFT);
	dma_write(v,FULL_CH_FRAG_LEN(chn));
}
static void dma_req_mode(u32 chn,u32 mode)
{
	u32 v;
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(REQ_MODE_MASK << REQ_MODE_SHIFT);
	mode &= REQ_MODE_MASK;
	v |= (mode << REQ_MODE_SHIFT);
	dma_write(v,FULL_CH_FRAG_LEN(chn));
}
static void dma_switch_mode(u32 chn,u32 mode)
{
	u32 v;
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(SWT_MODE_MASK << SWT_MODE_SHIFT);
	mode &= SWT_MODE_MASK;
	v |= (mode << SWT_MODE_SHIFT);
	dma_write(v,FULL_CH_FRAG_LEN(chn));
}

static void dma_ahb_size(u32 chn,u32 size)
{
	u32 v;
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(DST_SIZE_MASK << DST_SIZE_SHIFT);
	v &= ~(SRC_SIZE_MASK << SRC_SIZE_SHIFT);
	size &= DST_SIZE_MASK;
	v |= (size << DST_SIZE_SHIFT) | (size << SRC_SIZE_SHIFT);
	dma_write(v,FULL_CH_FRAG_LEN(chn));
}
static void dma_trsf_step(u32 chn,u32 step)
{
	u32 v;
	v = dma_read(FULL_CH_TRSF_STEP(chn));
	v &= ~(DST_TRSF_STEP_MASK << DST_TRSF_STEP_SHIFT);
	v &= ~(SRC_TRSF_STEP_MASK << SRC_TRSF_STEP_SHIFT);
	step &= SRC_TRSF_STEP_MASK;
	v |= (step << SRC_TRSF_STEP_SHIFT) | (step << DST_TRSF_STEP_SHIFT);
	dma_write(v,FULL_CH_TRSF_STEP(chn));
}
void dma_set_trsf_step(u32 chn,u32 step){
	u32 v;
	v = dma_read(FULL_CH_TRSF_STEP(chn));
	v &= ~(DST_TRSF_STEP_MASK << DST_TRSF_STEP_SHIFT);
	v &= ~(SRC_TRSF_STEP_MASK << SRC_TRSF_STEP_SHIFT);
	step &= SRC_TRSF_STEP_MASK;
	v |= (step << SRC_TRSF_STEP_SHIFT) | (step << DST_TRSF_STEP_SHIFT);
	dma_write(v,FULL_CH_TRSF_STEP(chn));
}
static void dma_wrap_enable(u32 chn,bool en)
{
	u32 v;
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(ADDR_WRAP_EN);
	if(en)
		v |= ADDR_WRAP_EN;
	dma_write(v,FULL_CH_FRAG_LEN(chn));
}
static void dma_fix_enable(u32 chn,bool en)
{
	u32 v;
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(ADDR_FIX_EN);
	if(en)
		v |= ADDR_FIX_EN;
	dma_write(v,FULL_CH_FRAG_LEN(chn));
}
void dma_int_enable(u32 chn)
{
	u32 v;
	v = dma_read(FULL_CH_INT(chn));
	v |= CH_CFG_ERR_INT_EN | CH_LLIST_INT_EN | CH_TRSC_INT_EN |
			CH_BLK_INT_EN | CH_FRAGMENT_INT_EN;
	dma_write(v,FULL_CH_INT(chn));
}

void dma_int_disable(u32 chn)
{
	u32 v;
	v = dma_read(FULL_CH_INT(chn));
	v &= ~(CH_CFG_ERR_INT_EN | CH_LLIST_INT_EN | CH_TRSC_INT_EN |
			CH_BLK_INT_EN | CH_FRAGMENT_INT_EN);
	dma_write(v,FULL_CH_INT(chn));
}

void dma_request(u32 chn)
{
	u32 v;
	v = dma_read(FULL_CH_REQ(chn));
	v |= CH_REQ;
	dma_write(v,FULL_CH_REQ(chn));
}

static void dma_clk_enable(void)
{
	u32 v;
	v = dma_read(REG_DMA_PAUSE);
	v |= (GLB_REG_CLK_EN | CHNL_REG_CLK_EN |
			REQ_CID_CLK_EN | INT_REG_CLK_EN |
			AXI_MST_CLK_EN | AUDIO_CNT_CLK_EN);
	dma_write(v,REG_DMA_PAUSE);
}
void dma_int_clear(u32 chn)
{
	u32 v;
	v = dma_read(FULL_CH_INT(chn));
	v |= CH_CFG_ERR_INT_CLR | CH_LLIST_INT_CLR | CH_TRSC_INT_CLR |
			CH_BLK_INT_CLR | CH_FRAGMENT_INT_CLR;
	dma_write(v,FULL_CH_INT(chn));
}
void dma_wait_complete(u32 chn)
{
	u32 v;
	u32 t1,t2;
	u32 status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
			 CH_FRAGMENT_INT_MASK_STAT;
	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = dma_read(FULL_CH_INT(chn));
		if((v & status) == status) {
			dma_int_clear(chn);
			break;
		}
	}while(t2 - t1 < DMA_WAIT_READY_TIME_OUT);
}

u32 dma_llist_wait_complete(u32 chn, u32 timeout)
{
	u32 v;
	u32 t1,t2;
	u32 status = CH_LLIST_INT_MASK_STAT;
	u32 ret = false;
	
	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = dma_read(FULL_CH_INT(chn));
		if((v & status) == status) {
			dma_int_clear(chn);
			ret = true;
			break;
		}
	}while(t2 - t1 < timeout);

	return ret;
}

void spic_request_cid(u32 chn, u32 id )
{
	chn = chn + 1;
	dma_write(chn,REQ1_CID(id));
	
}

void dma_init(u32 chn ,u32 id)
{
	//dma_reset();
//	init_chn_reg(chn);
	dma_clk_enable();
	dma_frag_wait(0);
	enable_channel(chn);
	dma_int_enable(chn);
	dma_chn_priority(chn,PRIORITY_LOWEST);
	dma_req_mode(chn,REQ_TRANSACTION);
	dma_switch_mode(chn,SWT_ABCD_ABCD);
	dma_ahb_size(chn,AHB_SIZE_WORD);
	dma_trsf_step(chn,4);
	dma_fix_enable(chn,false);
	dma_wrap_enable(chn,false);
	spic_request_cid(chn,id);
}
////////UART RX DMA///////////////
void dma_set_trans_len(u32 chn,u32 val){
	dma_write(val,FULL_CH_TRSC_LEN(chn));
}

void uart_int_enable(u32 chn)
{
	u32 v;
	v = dma_read(FULL_CH_INT(chn));
	v |= CH_FRAGMENT_INT_EN;
	dma_write(v,FULL_CH_INT(chn));
}

u32 dma_get_trans_len(u32 chn){
	u32 len;
	len = dma_read(FULL_CH_TRSC_LEN(chn));
	len &= 0xfffffff;
	return len;
}

void dma_set_src(u32 chn,u32 val){
	dma_write(val,FULL_CH_SRC_ADDR(chn));
}
void dma_set_dest(u32 chn,u32 val){
	dma_write(val,FULL_CH_DST_ADDR(chn));
}
void dma_frag_len_reg_set(u32 chn,u32 val){
	dma_write(val,FULL_CH_FRAG_LEN(chn));
}
void dma_blk_len_reg_set(u32 chn,u32 val){
	dma_write(val,FULL_CH_BLK_LEN(chn));

}
void uart_request_cid(u32 chn,u32 uart_addr)
{
	chn = chn + 1;
	if(uart_addr == REG_UART0_BASE){
		dma_write(chn,REQ1_CID(UART0_RX_RID));
	}else if(uart_addr == REG_UART1_BASE){
		dma_write(chn,REQ1_CID(UART1_RX_RID));
	}else if(uart_addr == REG_UART2_BASE){
		dma_write(chn,REQ1_CID(UART2_RX_RID));
	}

}

void llist_enable(u32 chn)
{
	u32 val;

	val = dma_read(FULL_CH_CFG(chn));
	val |= LLIST_EN;
	dma_write(val, FULL_CH_CFG(chn));
}

void set_llist_ptr(u32 chn, u32 ptr)
{
	dma_write(ptr, FULL_CH_LLIST_PTR(chn));
}

void dma_link_list(dma_link_list_dev *dma_dev, u32 loop, NodeCfg *node)
{
	int i;
	unsigned long start,end;

	//Fill node content
	for (i = 0; i < dma_dev->node_cnt; i++) {
		node[i].SRC_ADDR = dma_dev->src_addr_arr[i];
		node[i].DST_ADDR = dma_dev->dst_addr_arr[i];
		//clear ahb size
		node[i].FRAG_LEN &= ~(DST_SIZE_MASK<<DST_SIZE_SHIFT);
		node[i].FRAG_LEN &= ~(SRC_SIZE_MASK<<SRC_SIZE_SHIFT);

		//src and dst ahb size
		node[i].FRAG_LEN |= dma_dev->ahb_size;

		//clear and cfg word mode
		node[i].FRAG_LEN &= ~(REQ_MODE_MASK<<SWT_MODE_SHIFT);
		node[i].FRAG_LEN |= dma_dev->word_mode;

		//link list mode
		node[i].FRAG_LEN &= ~(REQ_MODE_MASK<<REQ_MODE_SHIFT);
		node[i].FRAG_LEN |= (0x2<<REQ_MODE_SHIFT);

		//disbabled addr_wrap_en
		node[i].FRAG_LEN &= ~(ADDR_WRAP_SEL);
		node[i].FRAG_LEN &= ~(ADDR_WRAP_EN);

		if (dma_dev->direction == DMA_MEM_TO_DEV) {
			node[i].FRAG_LEN |= (ADDR_FIX_SEL);
			node[i].FRAG_LEN |= (ADDR_FIX_EN);
		} else if (dma_dev->direction == DMA_DEV_TO_MEM) {
			node[i].FRAG_LEN &= ~(ADDR_FIX_SEL);
			node[i].FRAG_LEN |= (ADDR_FIX_EN);
		} else if (dma_dev->direction == DMA_MEM_TO_MEM) {
			node[i].FRAG_LEN &= ~(ADDR_FIX_SEL);
			node[i].FRAG_LEN &= ~(ADDR_FIX_EN);
		}

		if (i < (dma_dev->node_cnt - 1)) {
			node[i].FRAG_LEN &= ~(LLIST_END);
			node[i].LLIST_PTR = (u32)(&node[i + 1]);
		}
		if (i == (dma_dev->node_cnt - 1)) {
			if (loop) {
				node[i].FRAG_LEN &= ~(LLIST_END);
				node[i].LLIST_PTR = (u32)(&node[0]);
			}
			if (!loop)
				node[i].FRAG_LEN |= (LLIST_END);
		}
		//set frag len
		node[i].FRAG_LEN |= dma_dev->trsc_len_arr[i];
		//set blk len
		node[i].BLK_LEN |= dma_dev->trsc_len_arr[i];
		//set trsc len
		node[i].TRSC_LEN &= ~(LLIST_NODE_INT_EN);
		node[i].TRSC_LEN &= ~(LLIST_NODE_INT_TYPE_MASK <<
			LLIST_NODE_INT_TYPE_SHIFT);
		node[i].TRSC_LEN |= dma_dev->trsc_len_arr[i];

		if (dma_dev->ahb_size == SRCDST_AHB_SIZE_BYTE)
			node[i].TRSF_STEP = TRANS_STEP_BYTE;
		else if (dma_dev->ahb_size == SRCDST_AHB_SIZE_HALFWORD)
			node[i].TRSF_STEP = TRANS_STEP_HALFWORD;
		else if (dma_dev->ahb_size == SRCDST_AHB_SIZE_WORD)
			node[i].TRSF_STEP = TRANS_STEP_WORD;
		else if (dma_dev->ahb_size == SRCDST_AHB_SIZE_DWORD)
			node[i].TRSF_STEP = TRANS_STEP_DWORD;

		node[i].WRAP_PTR &= ~(WRAP_PTR_MASK);
		node[i].WRAP_TO &= ~(WRAP_TO_MASK);

		node[i].FRAG_STEP = 0x0;
		node[i].SRC_BLK_STEP = 0x0;
		node[i].DST_BLK_STEP = 0x0;

	}

	start = (unsigned long)node & ~(ARCH_DMA_MINALIGN - 1);
	end = ALIGN(start + ((sizeof(struct _NodeCfg)*dma_dev->node_cnt)), ARCH_DMA_MINALIGN);
	flush_cache((unsigned long)start, (end - start));

	init_chn_reg(dma_dev->ch_id);

	//invalid node to start the dma link list config
	//req cid

	spic_request_cid(dma_dev->ch_id,SPIC_RX_RID);

	llist_enable(dma_dev->ch_id);
	dma_int_enable(dma_dev->ch_id);
	dma_req_mode(dma_dev->ch_id, REQ_LINK_LIST);
	set_llist_ptr(dma_dev->ch_id, (u32)(&node[0]));
	enable_channel(dma_dev->ch_id);
}

void nor_dma_node_fill(u32 ch_id, u32 cnt, u32 *src, u32* dst, u32* len)
{
    unsigned long start,end;
    dma_link_list_dev *dma_dev = (dma_link_list_dev *)memalign(ARCH_DMA_MINALIGN, sizeof(struct _dma_link_list_dev));
    NodeCfg *node_sfc_out = (NodeCfg *)memalign(ARCH_DMA_MINALIGN, ((sizeof(struct _NodeCfg)*cnt)));
    memset(node_sfc_out,0,(sizeof(struct _NodeCfg)*cnt));

    dma_dev->ch_id = ch_id;
    dma_dev->dma_req_cid = SPIC_TX_RID;
    dma_dev->ahb_size = SRCDST_AHB_SIZE_WORD;
    dma_dev->word_mode = WORD_MODE_ABCD;
    dma_dev->direction = DMA_MEM_TO_MEM;
    dma_dev->node_cnt = cnt;
    dma_dev->src_addr_arr = src;
    dma_dev->dst_addr_arr = dst;
    dma_dev->trsc_len_arr = len;
    dma_link_list(dma_dev, 0, node_sfc_out);

    start = (unsigned long)node_sfc_out & ~(ARCH_DMA_MINALIGN - 1);
    end = ALIGN(start + ((sizeof(struct _NodeCfg)*cnt)), ARCH_DMA_MINALIGN);
    flush_cache((unsigned long)start, (end - start));
}

void nor_dma_node_fill_new(u32 ch_id, u32 cnt, u32 *src, u32* dst, u32* len, NodeCfg *node_out)
{
    dma_link_list_dev *dma_dev = (dma_link_list_dev *)memalign(ARCH_DMA_MINALIGN, sizeof(struct _dma_link_list_dev));//map_physmem(addr, 0, MAP_NOCACHE);
    //NodeCfg *node_sfc_out = (NodeCfg *)memalign(ARCH_DMA_MINALIGN, ((sizeof(struct _NodeCfg)*cnt)));
    memset(node_out,0,(sizeof(struct _NodeCfg)*cnt));

    dma_dev->ch_id = ch_id;
    dma_dev->dma_req_cid = SPIC_TX_RID;
    dma_dev->ahb_size = SRCDST_AHB_SIZE_WORD;
    dma_dev->word_mode = WORD_MODE_ABCD;
    dma_dev->direction = DMA_MEM_TO_MEM;
    dma_dev->node_cnt = cnt;
    dma_dev->src_addr_arr = src;
    dma_dev->dst_addr_arr = dst;
    dma_dev->trsc_len_arr = len;

    dma_link_list(dma_dev, 0, node_out);
    
    free(dma_dev);
}

u32 get_dma_dst_addr(unsigned int chn)
{
	u32 reg_val = 0;
	reg_val = *(volatile unsigned *)(0xc701014 + 0x40 * chn);
	return reg_val;
}


