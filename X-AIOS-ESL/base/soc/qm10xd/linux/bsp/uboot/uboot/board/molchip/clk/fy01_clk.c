#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>


void molchip_spic_clk_enable(void)
{
    writel(2, CLK_SPIC_SSI_CFG);

	setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_SPIC_EN);

	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AHB_SPIC_EN | CKG_SPIC_SSI_EN);

}

void molchip_dma_clk_enable(void)
{

	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AXI_DMA_EN \
		| CKG_AXI_DMA_FORCE_EN | CKG_AXI_DMA_AUTO_GATE_SEL);
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