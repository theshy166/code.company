#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/dma.h>

void dma_enable(bool en)
{
	u32 v;
	v = __raw_readl(REG_DMA_CTRL);
	if (en)
		v |= CKG_DMA0_EN;
	else
		v &= ~(CKG_DMA0_EN);
	__raw_writel(v, REG_DMA_CTRL);
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
	dma_write(CH_EN,FULL_CH_CFG(chn));
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
void hash_dma_trsc_len(u32 chn, u32 len)
{
	u32 v;
	v = dma_read(FULL_CH_TRSC_LEN(chn));
	v &= ~(TRSC_LEN_MASK << TRSC_LEN_SHIFT);
	len &= TRSC_LEN_MASK;
	v |= (len << TRSC_LEN_SHIFT);
	dma_write(v, FULL_CH_TRSC_LEN(chn));
	v = dma_read(FULL_CH_BLK_LEN(chn));
	v &= ~(BLK_LEN_MASK << BLK_LEN_SHIFT);
	v |= ((0x80 *4 ) << BLK_LEN_SHIFT);
	dma_write(v, FULL_CH_BLK_LEN(chn));
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(FRAG_LEN_MASK << FRAG_LEN_SHIFT);
	v |= ((0x80*4) << FRAG_LEN_SHIFT);
	dma_write(v, FULL_CH_FRAG_LEN(chn));
}
void aes_dma_trsc_len(u32 chn, u32 len)
{
	u32 v;
	v = dma_read(FULL_CH_TRSC_LEN(chn));
	v &= ~(TRSC_LEN_MASK << TRSC_LEN_SHIFT);
	len &= TRSC_LEN_MASK;
	v |= (len << TRSC_LEN_SHIFT);
	dma_write(v, FULL_CH_TRSC_LEN(chn));
	v = dma_read(FULL_CH_BLK_LEN(chn));
	v &= ~(BLK_LEN_MASK << BLK_LEN_SHIFT);
	v |= ((0x40 ) << BLK_LEN_SHIFT);
	dma_write(v, FULL_CH_BLK_LEN(chn));
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(FRAG_LEN_MASK << FRAG_LEN_SHIFT);
	v |= ((0x40) << FRAG_LEN_SHIFT);
	dma_write(v, FULL_CH_FRAG_LEN(chn));
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
		v |= ADDR_FIX_EN|ADDR_FIX_SEL;
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

bool soft_dma_wait_complete(u32 chn)
{
	u32 v;
	u32 status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
		 CH_FRAGMENT_INT_MASK_STAT;
	v = dma_read(FULL_CH_INT(chn));
	if ((v & status) == status) {
		dma_int_clear(chn);
		return true;
	}
	return false;
}
bool aes_dma_wait_done(u32 rx_chn,u32 tx_chn)
{
	u32 v1,v2;
	u32 status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
		 CH_FRAGMENT_INT_MASK_STAT;
	v1 = dma_read(FULL_CH_INT(rx_chn));
	v2 = dma_read(FULL_CH_INT(tx_chn));
	if (((v1 & status) == status) && ((v2 & status) == status)) {
		dma_int_clear(rx_chn);
		dma_int_clear(tx_chn);
		return true;
	}
	return false;
}
void hash_dma_wait_complete(u32 chn, u32 timeout)
{
	u32 v;
	u32 t1, t2;
	u32 status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
		 CH_FRAGMENT_INT_MASK_STAT;
	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = dma_read(FULL_CH_INT(chn));
		if ((v & status) == status) {
			dma_int_clear(chn);
			break;
		}
	} while (t2 - t1 < timeout);
}
void spic_request_cid(u32 chn, u32 tx_rid)
{
	chn = chn + 1;
	dma_write(chn,REQ1_CID(tx_rid));

}
void aes_request_cid(u32 chn, unsigned int req_id)
{
	unsigned int chnnl;
	chnnl = chn + 1;
	dma_write(chnnl, REQ1_CID(req_id));
}
void hash_request_cid(u32 chn)
{
	chn = chn + 1;
	dma_write(chn, REQ1_CID(HASH_RX_RID));
}
void aes_dma_fix_enable(u32 chn, bool en, bool sel)
{
	u32 v;
	v = dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(ADDR_FIX_EN|ADDR_FIX_SEL);
	if (en)
		v |= ADDR_FIX_EN;
	if (sel)
		v |= ADDR_FIX_SEL;
	dma_write(v, FULL_CH_FRAG_LEN(chn));
}

void dma_init(u32 chn, u32 id)
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
void hash_dma_init(u32 chn)
{
	init_chn_reg(chn);
	dma_frag_wait(0);
	enable_channel(chn);
	dma_int_enable(chn);
	dma_chn_priority(chn, PRIORITY_LOWEST);
	dma_req_mode(chn, 0);
	dma_switch_mode(chn, SWT_ABCD_DCBA);
	dma_ahb_size(chn, AHB_SIZE_WORD);
	dma_trsf_step(chn, 4);
	dma_fix_enable(chn, true);
	dma_wrap_enable(chn, false);
	hash_request_cid(chn);
}

void aes_dma_trac_init(u32 chn, u32 req_id, bool en,bool sel)
{

	init_chn_reg(chn);
	dma_frag_wait(0);
	dma_int_enable(chn);
	dma_chn_priority(chn, PRIORITY_LOWEST);
	dma_req_mode(chn, 0);
	dma_switch_mode(chn, SWT_ABCD_DCBA);
	dma_ahb_size(chn, AHB_SIZE_WORD);
	dma_trsf_step(chn, 4);
	aes_dma_fix_enable(chn, en, sel);
	dma_wrap_enable(chn, false);
	aes_request_cid(chn,req_id);
}
void aes_dma_trac(unsigned int src, unsigned int dest,
	unsigned int chn, unsigned int req_id, unsigned int len, bool fix)
{
	aes_dma_trac_init(chn, req_id, true,fix);
	dma_chn_src(chn, src);
	dma_chn_dst(chn, dest);
	aes_dma_trsc_len(chn,len);
}
