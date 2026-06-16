#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>

void molchip_spic_clk_enable(void)
{
	writel(2, CLK_SPIC_SSI_CFG);
	setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_SFC_EN);
	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AHB_SFC_EN);
}

#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_PDL_BUILD) || defined(CONFIG_SDL_BUILD)

void molchip_dma_clk_enable(void)
{
	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_DMA0_EN);
	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_DMA_EN);
}

void molchip_systimer_clk_enable(void)
{
	setbits_le32(REG_AP_PERI_CLK_CTRL,CKG_SYST_EN);

	setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_SYST_EN); 
}

void molchip_uart_clk_enable(unsigned int uport)
{
	if(REG_UART0_BASE == uport)
	{	
		setbits_le32(REG_AP_PERI_CLK_CTRL,CKG_UART0_EN);

		setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_UART0_EN);
	}
	else if(REG_UART1_BASE == uport)
	{
		setbits_le32(REG_AP_PERI_CLK_CTRL,CKG_UART1_EN);

		setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_UART1_EN);

	}

}
#endif
#define CPU_CLK_SEL 0x02E00008
#define CKG_DIG_CTL 0x18000048
int mc_get_cpu_clk(void)
{
	unsigned int pll_sel = *(volatile unsigned int*)CPU_CLK_SEL & 0x3;
	unsigned int dig_ctl = (*(volatile unsigned int*)CKG_DIG_CTL >> 24) & 0xFF;

	switch (pll_sel) {
		case 0:
			return 24000000;
		case 1:
			return 640000000;
		case 2:
			return 12*(dig_ctl + 32)*1000000;
		case 3:
			return 1500000000;
		default:
			return 24000000;
	}
}
