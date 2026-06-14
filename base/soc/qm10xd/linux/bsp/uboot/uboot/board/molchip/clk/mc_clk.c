#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>

void molchip_spic_clk_enable(void)
{
	setbits_le32(REG_SFC_CTRL0, CKG_APB_SFC_EN);
	setbits_le32(REG_SFC_CTRL0, CKG_SFC_EN);
}

#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_PDL_BUILD) || defined(CONFIG_SDL_BUILD)

void molchip_dma_clk_enable(void)
{
	setbits_le32(REG_SFC_CTRL0,CKG_DMA0_EN);
}

void molchip_systimer_clk_enable(void)
{
	setbits_le32(REG_CPLL_CTRL0,CKG_SYST_EN);
	setbits_le32(REG_CEN_PERI_APB_CLK_CTRL,CKG_APB_SYST_EN);
}

void molchip_uart_clk_enable(unsigned int uport)
{
	if (REG_UART0_BASE == uport){
		setbits_le32(REG_CEN_PERI_CLK_CTRL,CKG_UART0_EN);
		setbits_le32(REG_CEN_PERI_APB_CLK_CTRL,CKG_APB_UART0_EN);
	} else if (REG_UART1_BASE == uport) {
		setbits_le32(REG_CEN_PERI_CLK_CTRL,CKG_UART1_EN);
		setbits_le32(REG_CEN_PERI_APB_CLK_CTRL,CKG_APB_UART1_EN);
	}
}
#endif


unsigned int mc_get_cpu_clk(void)
{
	unsigned int val, soc_freq;

	val = readl(REG_CKG_CA7_CORE_CFG);
	val &= (CKG_CA7_CORE_SEL_MASK << CKG_CA7_CORE_SEL_OFFSET);

	switch (val) {
	case 0:
		soc_freq = 24000000;
		break;
	case 1:
		soc_freq = 480000000;
		break;
	case 2:
		soc_freq = 600000000;
		break;
	case 3:
		soc_freq = 800000000;
		break;
	case 4:
	default:
		soc_freq = 960000000;
		break;
	}

	return soc_freq;
}
