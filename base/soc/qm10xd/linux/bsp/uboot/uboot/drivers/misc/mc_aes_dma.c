#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <misc.h>

#include "mc_aes.h"
#include "mc_aes_dma.h"

void aes_init_chn_reg(u32 chn)
{
	u32 i;
	u32 reg;
	reg = FULL_CH_PAUSE(chn);
	for(i = 0; i < 0x10; i ++) {
		if(reg == FULL_CH_INT(chn)){
			reg += 4;
			continue;
		}
		aes_dma_write(0,reg);
		reg += 4;
	}
}
void aes_dma_int_clear(u32 chn)
{
	u32 v;
	v = aes_dma_read(FULL_CH_INT(chn));
	v |= CH_CFG_ERR_INT_CLR | CH_LLIST_INT_CLR | CH_TRSC_INT_CLR |
			CH_BLK_INT_CLR | CH_FRAGMENT_INT_CLR;
	aes_dma_write(v,FULL_CH_INT(chn));
}

void aes_enable_channel(u32 chn)
{
	aes_dma_write(CH_EN,FULL_CH_CFG(chn));
}
static void aes_dma_frag_wait(u32 wait)
{
	u32 v;
	v = aes_dma_read(DMA_FRAG_WAIT);
	v &= ~(DMA_FRAG_WAIT_MASK << DMA_FRAG_WAIT_SHIFT);
	wait &= DMA_FRAG_WAIT_MASK;
	v |= (wait << DMA_FRAG_WAIT_SHIFT);
	aes_dma_write(v,DMA_FRAG_WAIT);
}
static void aes_dma_chn_priority(u32 chn,u32 pri)
{
	u32 v;
	v = aes_dma_read(FULL_CH_CFG(chn));
	v &= ~(CH_PRIORITY_MASK << CH_PRIORITY_SHIFT);
	pri &= CH_PRIORITY_MASK;
	v |= (pri << CH_PRIORITY_SHIFT);
	aes_dma_write(v,FULL_CH_CFG(chn));
}
void aes_dma_chn_src(u32 chn,u32 src)
{
	aes_dma_write(src,FULL_CH_SRC_ADDR(chn));
}
void aes_dma_chn_dst(u32 chn,u32 dst)
{
	aes_dma_write(dst,FULL_CH_DST_ADDR(chn));
}

void aes_request_cid(u32 chn, unsigned int req_id)
{
	unsigned int chnnl;

	chnnl = chn + 1;
	aes_dma_write(chnnl, REQ1_CID(req_id));
}

static void aes_dma_req_mode(u32 chn,u32 mode)
{
	u32 v;
	v = aes_dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(REQ_MODE_MASK << REQ_MODE_SHIFT);
	mode &= REQ_MODE_MASK;
	v |= (mode << REQ_MODE_SHIFT);
	aes_dma_write(v,FULL_CH_FRAG_LEN(chn));
}
static void aes_dma_switch_mode(u32 chn,u32 mode)
{
	u32 v;
	v = aes_dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(SWT_MODE_MASK << SWT_MODE_SHIFT);
	mode &= SWT_MODE_MASK;
	v |= (mode << SWT_MODE_SHIFT);
	aes_dma_write(v,FULL_CH_FRAG_LEN(chn));
}

static void aes_dma_ahb_size(u32 chn,u32 size)
{
	u32 v;
	v = aes_dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(DST_SIZE_MASK << DST_SIZE_SHIFT);
	v &= ~(SRC_SIZE_MASK << SRC_SIZE_SHIFT);
	size &= DST_SIZE_MASK;
	v |= (size << DST_SIZE_SHIFT) | (size << SRC_SIZE_SHIFT);
	aes_dma_write(v,FULL_CH_FRAG_LEN(chn));
}
static void aes_dma_trsf_step(u32 chn,u32 step)
{
	u32 v;
	v = aes_dma_read(FULL_CH_TRSF_STEP(chn));
	v &= ~(DST_TRSF_STEP_MASK << DST_TRSF_STEP_SHIFT);
	v &= ~(SRC_TRSF_STEP_MASK << SRC_TRSF_STEP_SHIFT);
	step &= SRC_TRSF_STEP_MASK;
	v |= (step << SRC_TRSF_STEP_SHIFT) | (step << DST_TRSF_STEP_SHIFT);
	aes_dma_write(v,FULL_CH_TRSF_STEP(chn));
}

static void aes_dma_wrap_enable(u32 chn,bool en)
{
	u32 v;
	v = aes_dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(ADDR_WRAP_EN);
	if(en)
		v |= ADDR_WRAP_EN;
	aes_dma_write(v,FULL_CH_FRAG_LEN(chn));
}

void aes_dma_int_enable(u32 chn)
{
	u32 v;
	v = aes_dma_read(FULL_CH_INT(chn));
	v |= CH_CFG_ERR_INT_EN | CH_LLIST_INT_EN | CH_TRSC_INT_EN |
			CH_BLK_INT_EN | CH_FRAGMENT_INT_EN;
	aes_dma_write(v,FULL_CH_INT(chn));
}


void aes_dma_fix_enable(u32 chn, bool en, bool sel)
{
	u32 v;

	v = aes_dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(ADDR_FIX_EN|ADDR_FIX_SEL);
	if (en)
		v |= ADDR_FIX_EN;
	if (sel)
		v |= ADDR_FIX_SEL;
	aes_dma_write(v, FULL_CH_FRAG_LEN(chn));
}

void aes_dma_trsc_len(u32 chn, u32 len)
{
	u32 v;

	v = aes_dma_read(FULL_CH_TRSC_LEN(chn));
	v &= ~(TRSC_LEN_MASK << TRSC_LEN_SHFIT);
	len &= TRSC_LEN_MASK;
	v |= (len << TRSC_LEN_SHFIT);
	aes_dma_write(v, FULL_CH_TRSC_LEN(chn));

	v = aes_dma_read(FULL_CH_BLK_LEN(chn));
	v &= ~(BLK_LEN_MASK << BLK_LEN_SHFIT);
	v |= ((0x10 ) << BLK_LEN_SHFIT);
	aes_dma_write(v, FULL_CH_BLK_LEN(chn));

	v = aes_dma_read(FULL_CH_FRAG_LEN(chn));
	v &= ~(FRAG_LEN_MASK << FRAG_LEN_SHIFT);
	v |= ((0x10) << FRAG_LEN_SHIFT);
	aes_dma_write(v, FULL_CH_FRAG_LEN(chn));
}
void aes_dma_trac_init(u32 chn, u32 req_id, bool en,bool sel)
{

	aes_init_chn_reg(chn);
	aes_dma_frag_wait(0);
	aes_dma_int_enable(chn);
	aes_dma_chn_priority(chn, PRIORITY_LOWEST);
	aes_dma_req_mode(chn, REQ_FRAGEMENT);
	aes_dma_switch_mode(chn, SWT_ABCD_DCBA);
	aes_dma_ahb_size(chn, AHB_SIZE_WORD);
	aes_dma_trsf_step(chn, 4);
	aes_dma_fix_enable(chn, en, sel);
	aes_dma_wrap_enable(chn, false);
	aes_request_cid(chn,req_id);
}

void aes_dma_trac(unsigned int src, unsigned int dest,
	unsigned int chn, unsigned int req_id, unsigned int len, bool fix)
{
	aes_dma_trac_init(chn, req_id, true,fix);
	aes_dma_chn_src(chn, src);
	aes_dma_chn_dst(chn, dest);
	aes_dma_trsc_len(chn,len);
}

bool aes_dma_wait_done(u32 rx_chn,u32 tx_chn)
{
	u32 v1,v2;

	u32 status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
		 CH_FRAGMENT_INT_MASK_STAT;

	v1 = aes_dma_read(FULL_CH_INT(rx_chn));
	v2 = aes_dma_read(FULL_CH_INT(tx_chn));
	if (((v1 & status) == status) && ((v2 & status) == status)) {
		aes_dma_int_clear(rx_chn);
		aes_dma_int_clear(tx_chn);
		return true;
	}
	return false;
}
