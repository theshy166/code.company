#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "aes.h"
#include "secure.h"
#include "dma.h"
#include "lzma.h"


void lzma_enable(struct boot_header *header)
{
	unsigned int reg_val;

	reg_val = reg_readl(IOE_CFG);
	reg_val &= ~CKG_LZMA_CORE_EN;
	reg_val &= ~CKG_IOE_PUB_EN;
	reg_writel(reg_val, IOE_CFG);

	reg_val = reg_readl(IOE_CFG);
	reg_val |= IOE_LZMA_CORE_SOFT_RST;
	reg_val |=  CKG_IOE_PUB_SOFT_RST;
	reg_writel(reg_val, IOE_CFG);
	
	mc_udelay(1);
		
	//for(i=0;i<0x1000;i++);
	//lzma clk sel
	reg_val = reg_readl(IOE_CFG);
	reg_val |= (((header->lzma_ctrl >> 16) & 0x7F) << 8);
	reg_writel(reg_val, IOE_CFG);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	reg_val = reg_readl(IOE_CFG);
	reg_val |= CKG_LZMA_CORE_EN;
	reg_val |= CKG_IOE_PUB_EN;
	reg_writel(reg_val, IOE_CFG);

	reg_val = reg_readl(IOE_CFG);
	reg_val &= ~IOE_LZMA_CORE_SOFT_RST;
	reg_val &= ~CKG_IOE_PUB_SOFT_RST;
	reg_writel(reg_val, IOE_CFG);
}

void lzma_cfg(unsigned int lzma_src_addr, unsigned int lzma_dst_addr,
	struct boot_header *header)
{
	unsigned int reg_val;

	lzma_enable(header);

	if (header->cascade != 1)
	{
		if (0 == header->cascade) {
			reg_val = reg_readl(LZMA_DMA_CTRL);
			reg_val &= ~ BIT(0);
			reg_val |= 0x30;
			reg_writel(reg_val, LZMA_DMA_CTRL);
		} else {
			reg_val = reg_readl(LZMA_DMA_CTRL);
			reg_val &= ~ BIT(0);
			reg_val |= 0x10;
			reg_writel(reg_val, LZMA_DMA_CTRL);
		}
	}


	if ((header->lzma_ctrl & BIT(23)) != 0)
	{
		reg_writel(header->lzma_decode_reg, DECODE_CTRL0);
		reg_writel(header->lzma_dma_ctrl_cfg, LZMA_DMA_CTRL);
	}
	reg_val = reg_readl(DECODE_CTRL0);
	reg_val |= (1 << 8);
	reg_writel(reg_val, DECODE_CTRL0);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);
	reg_val = reg_readl(DECODE_CTRL0);
	reg_val &= ~(1 << 8);
	reg_writel(reg_val, DECODE_CTRL0);

	reg_writel(lzma_src_addr >> 3, SRC_START_ADDR);
	reg_writel((lzma_src_addr >> 3) + 0x40 - 1, SRC_END_ADDR);
	reg_writel(lzma_dst_addr >> 3, DES_BASE_ADDR);
	reg_writel(header->lzma_zip_len, SRC_FILE_LENGTH);
	reg_writel(0xFFFFFFFF, INT_DONE_MASK);
}

void lzma_start(void)
{
	unsigned int val;

	val = reg_readl(DECODE_CTRL0);
	val |= 1;
	reg_writel(val, DECODE_CTRL0);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	val = reg_readl(DECODE_CTRL0);
	val &= (~1);
	reg_writel(val, DECODE_CTRL0);
}

volatile  unsigned int lzma_sched = 0;
void lzma_handle_start(u32 src, u32 dst, u32 len)
{
	unsigned int reg_val;

//src cfg
	reg_writel(src >> 3, SRC_START_ADDR);
	
//start
	if(lzma_sched == 0)
	{
		lzma_start();
	}else{
		reg_val = reg_readl(DECODE_CTRL0);
		reg_val |= BIT(4);
		reg_writel(reg_val, DECODE_CTRL0);
		
		mc_udelay(1);
		//for(i=0;i<0x1000;i++);
		reg_val = reg_readl(DECODE_CTRL0);
		reg_val &= (~ BIT(4));
		reg_writel(reg_val, DECODE_CTRL0);
	}
	lzma_sched ++;
}


bool lzma_handle_done(unsigned int *size)
{

	static unsigned int cur_cnt, last_cnt = 0;
	unsigned int status = reg_readl(INT_DONE_STATUS);

	if ((status & 0x1) == 0x1) {
		last_cnt=0;
		cur_cnt=0;
		*size = reg_readl(LZMA_DECODE_CNT);
		return true;
	}

	if((status & 0x10) == 0x10) {
		reg_writel(0x10, INT_DONE_CLEAR);
		if (0 == last_cnt) {
			last_cnt = reg_readl(LZMA_DECODE_CNT);
			if (last_cnt >= 0x800) {
				*size = 0x800;
			} else {
				*size = 0;
			}
		} else {
			cur_cnt = reg_readl(LZMA_DECODE_CNT);
			if (cur_cnt - last_cnt >= 0x800) {
				*size = last_cnt + 0x800;
			} else {
				*size = last_cnt;
			}
			last_cnt = cur_cnt;
		}
		return true;
	}

	return false;
}
void lzma_sch_init(struct boot_header *header)
{
	unsigned int reg_val;
	lzma_enable(header);	
	
	reg_val = reg_readl(DECODE_CTRL0);
	reg_val |= (1 << 8);
	reg_writel(reg_val, DECODE_CTRL0);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	reg_val = reg_readl(DECODE_CTRL0);
	reg_val &= ~(1 << 8);
	reg_writel(reg_val, DECODE_CTRL0);
	reg_writel(header->lzma_zip_len, SRC_FILE_LENGTH);
	reg_writel(0xFFFFFFFF, INT_DONE_MASK);
	
	
	reg_val = reg_readl(LZMA_DMA_CTRL);
	reg_val &= (~ BIT(16));
	reg_val |=BIT(16);
	reg_val &= (~(0xFF << 20));
	reg_val |= ((header->granularity / 1024 ) << 20);// 4KB
	reg_val &=  (~ BIT(0));
	reg_val |= (0x3 <<4);
	reg_writel(reg_val, LZMA_DMA_CTRL);
	
	
	reg_val = reg_readl(LZMA_DMA_CTRL);
	reg_val |=BIT(17);
	reg_writel(reg_val, LZMA_DMA_CTRL);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);
	reg_val &=  (~ BIT(17));
	reg_writel(reg_val, LZMA_DMA_CTRL);

	reg_writel(header->lzma_dst >> 3, DES_BASE_ADDR);

}



bool lzma_done(unsigned int timer)
{
	unsigned int val, v, t1, t2;
	unsigned int status = INT_DONE;

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = reg_readl(INT_DONE_STATUS);
		if ((v & status) == status) {
			val = reg_readl(INT_DONE_CLEAR);
			val |= LZMA_DONE_CLEAR;
			reg_writel(val, INT_DONE_CLEAR);

			val = reg_readl(INT_DONE_CLEAR);
			val &= ~LZMA_DONE_CLEAR;
			reg_writel(val, INT_DONE_CLEAR);

			goto lzma_complete;
		}
	} while (t2 - t1 < timer);

	return false;
lzma_complete:
	return true;
}


