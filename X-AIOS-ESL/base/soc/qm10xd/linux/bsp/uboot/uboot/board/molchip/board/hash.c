#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "hash.h"
#include "dma.h"

void hash_enable(unsigned int clk_sel)
{
	unsigned int val;

	val = __raw_readl(CEN_PERI_CLK_CTRL);
	val &= ~CKG_HASH_EN;
	__raw_writel(val, CEN_PERI_CLK_CTRL);

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val &= ~CKG_APB_HASH_EN;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);

mc_udelay(1);
		//for(i=0;i<0x1000;i++);
	val = __raw_readl(AP_PERI_SOFT_RST0);
	val |= HASH_SOFT_RST;
	__raw_writel(val, AP_PERI_SOFT_RST0);

	val = __raw_readl(CKG_HASH_CFG);
	val &= (~0x3);
	__raw_writel(val, CKG_HASH_CFG);

	val |= 2;
	__raw_writel(val, CKG_HASH_CFG);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	val = __raw_readl(CEN_PERI_CLK_CTRL);
	val |= CKG_HASH_EN;
	__raw_writel(val, CEN_PERI_CLK_CTRL);

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val |= CKG_APB_HASH_EN;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);

	val = __raw_readl(AP_PERI_SOFT_RST0);
	val &= ~HASH_SOFT_RST;
	__raw_writel(val, AP_PERI_SOFT_RST0);
}

void hash_mode(unsigned int mode)
{
	unsigned int val;

	val = hash_readl(HASH_CFG);
	val &= ~HASH_MODE_MASK;
	val |= mode & HASH_MODE_MASK;
	hash_writel(val, HASH_CFG);
}

void hash_msg_len(unsigned int len)
{
	hash_writel(len, HASH_MSG_LEN_CFG0);
}

void hash_start(void)
{
	unsigned int val;

	val = hash_readl(HASH_CFG);
	val &= ~HASH_START;
	hash_writel(val, HASH_CFG);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	val |= HASH_START;
	hash_writel(val, HASH_CFG);
}
#if 0
void read_result(unsigned int *result, unsigned int mode)
{
	unsigned int len, i;

	if (mode == HASH_SHA1) {
		len = ((160 >> 3) >> 2);
		for (i = 0; i < len; i++)
			result[i] = swab32(hash_readl(HASH_H0 + i*4));
	} else if (mode == HASH_SHA256) {
		len = ((256 >> 3) >> 2);
		for (i = 0; i < len; i++)
			result[i] = swab32(hash_readl(HASH_H0 + i*4));

	} else if (mode == HASH_SHA512) {
		len = ((512 >> 3) >> 2);
		for (i = 0; i < len; i += 2) {
			result[i] = swab32(hash_readl(HASH_H0 + (i + 1)*4));
			result[i + 1] = swab32(hash_readl(HASH_H0 + i*4));
		}
	} else if (mode == HASH_SHA384) {
		len = ((384 >> 3) >> 2);
		for (i = 0; i < len; i += 2) {
			result[i] = swab32(hash_readl(HASH_H0 + (i + 1)*4));
			result[i + 1] = swab32(hash_readl(HASH_H0 + i*4));
		}
	}
}
#else
void (*read_result)(unsigned int *, unsigned int )
= (void (*)(unsigned int *,unsigned int))(0x632c);
#endif
bool hash_done(unsigned int time,unsigned int chn){

	unsigned int val;
	u32 v;
	u32 t1,t2;
	u32 status = INT_STATUS;

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = hash_readl(HASH_INT);
		if((v & status) == status) {
		
			val= hash_readl(HASH_INT);
			val |= HASH_INT_CLR;
			hash_writel(val, HASH_INT);
		
			val= hash_readl(HASH_INT);
			val &= (~HASH_INT_CLR);
			hash_writel(val, HASH_INT);
			dma_int_clear(27);
			goto hash_complete;
	}
	} while((t2 - t1) < time);
		
	return false;
	
hash_complete:
	return true;
}

bool hash_handle_done(u32 chn)
{
	u32 v,status;
#if 0
	u32 t1,t2,val;
	u32 status = INT_STATUS;

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = hash_readl(HASH_INT);
		if((v & status) == status) {
		
			val= hash_readl(HASH_INT);
			val |= HASH_INT_CLR;
			hash_writel(val, HASH_INT);
		
			val= hash_readl(HASH_INT);
			val &= (~HASH_INT_CLR);
			hash_writel(val, HASH_INT);
		 break;
		}
	} while((t2 - t1) < 5000);
#endif
	 status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
		 CH_FRAGMENT_INT_MASK_STAT;

	v = dma_read(FULL_CH_INT(chn));
	if ((v & status) == status) {
		dma_int_clear(chn);
		return true;
	}
	return false;
}

void hash_sched_init(u32 chn, u32 len)
{

	hash_dma_init(chn);
	hash_mode(HASH_SHA256);
	hash_msg_len(len);
	len = (((len + 3) >> 2) << 2);
	enable_channel(chn);
	hash_writel(0x80*4, MEM_INFO_CFG2);
}

volatile  unsigned int hash_start_cnt =0;
void hash_handle_start(u32 src, u32 dst, u32 len)
{
	dma_chn_src(20, (src));
	dma_chn_dst(20, REG_HASH_BASE + HASH_FIFO);
	hash_dma_trsc_len(20, len);
	enable_channel(20);
	if (hash_start_cnt ==0)
	{
		hash_start();
	}
	hash_start_cnt++;
}

int hash_cfg(unsigned int chn, unsigned int src, unsigned int len)
{
	
	hash_dma_init(chn);
	hash_mode(HASH_SHA256);
	hash_msg_len(len);
	len = (((len + 3) >> 2) << 2);
	enable_channel(chn);
	dma_chn_src(chn, (src));
	dma_chn_dst(chn, REG_HASH_BASE + HASH_FIFO);
	hash_dma_trsc_len(chn, len);
	hash_writel(0x80*4, MEM_INFO_CFG2);
	return true;
}
