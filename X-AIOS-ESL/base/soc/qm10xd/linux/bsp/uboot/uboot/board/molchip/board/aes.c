#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "aes.h"
#include "secure.h"

void aes_enable(void)
{
	u32 val;

	val = __raw_readl(AP_MTX_CLK_CTRL0);
	val |= CKG_AHB_AES_EN;
	__raw_writel(val, AP_MTX_CLK_CTRL0);

	val = __raw_readl(CPU_SYS_RST1);
	val |= AES_SOFT_RST;
	__raw_writel(val, CPU_SYS_RST1);

	mc_udelay(1);
		//for(i=0;i<0x1000;i++);
	
	val &= ~(AES_SOFT_RST);
	__raw_writel(val, CPU_SYS_RST1);
}

void aes_enable_1st_iv(bool en)
{
	u32 val;

	val = aes_read(ENCRYPT_CONTROL);
	if (en)
		val |= CONTROL_FIRST;
	else
		val &= ~CONTROL_FIRST;
	aes_write(ENCRYPT_CONTROL, val);
}

void aes_threshold(void)
{
	u32 val;

	val = aes_read(FIFO_THOLD);
	val &= ~IF_THOLD_MASK;
	val &= ~OF_THOLD_MASK;
	val |= IF_THRESHOLD(0x3f);
	val |= OF_THRESHOLD(0x3f);
	aes_write(FIFO_THOLD, val);
}


void aes_clr_intr(void)
{
	u32 val;

	val = CLR_STOP_INTR | CLR_ERR_INTR | CLR_DONE_INTR;
	aes_write(INTR_CLR, val);
}

void aes_en_intr(void)
{
	u32 val;

	val = STOP_INTR_EN | ERR_INTR_EN | DONE_INTR_EN;
	aes_write(INTR_ENABLE, val);
}

u32 aes_intr_src(void)
{
	u32 val;

	val = aes_read(INTR_SRC);
	return val;
}

void aes_decrypt(bool decrypt)
{
	u32 val;

	val = aes_read(ENCRYPT_CONTROL);
	if (decrypt)
		val |= CONTROL_DECRYPT;
	else
		val &= ~CONTROL_DECRYPT;
	aes_write(ENCRYPT_CONTROL, val);
}

void aes_write_iv(u32 *iv, u32 len)
{
	int i;

	for (i = 0; i < len; i++)
		aes_write(AES_REG_IV(i), iv[i]);
}

void aes_config_mode(u32 mode, u32 emode)
{
	u32 val;

	val = aes_read(ENCRYPT_CONTROL);
	val &= ~CONTROL_EMODE_MASK;
	val &= ~CONTROL_MODE_MASK;
	val |= EMODE(emode);
	val |= METHOD(mode);
	aes_write(ENCRYPT_CONTROL, val);
}


void aes_start(unsigned int len)
{
	unsigned int val;

	val = aes_read(EXT_DMA_CFG1);
	val &= ~DMA_RXDATA_BYTE_LEN;
	val &= ~DMA_TXDATA_BYTE_LEN;
	val |= 0x400040;
	aes_write(EXT_DMA_CFG1, val);

	//enable ext dma
	val = aes_read(EXT_DMA_CFG);
	val |= BIT(0);
	aes_write(EXT_DMA_CFG, val);

	//enable internel  dma
	val = aes_read(DMA_CTRL);
	val |= BIT(0);
	aes_write(DMA_CTRL, val);
}
