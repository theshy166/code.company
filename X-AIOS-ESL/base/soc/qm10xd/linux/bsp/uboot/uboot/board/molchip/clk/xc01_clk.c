#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>

struct clk_src_cfg clk_src_cfgs[2] = {
	{
		.base_addr = 0x2F300024,
		.clk_src_regs = {
			{ 0x2F300024, 0, 0, 0x1},
			{ 0x2F300028, 8, 0, 0x53},
			{ 0x2F30002c, 22, 0, 0x2AAAAA}
		}
	},
	{
		.base_addr = 0x2F300050,
		.clk_src_regs = {
			{ 0x2F300050, 0, 0, 0x1 },
			{ 0x2F300054, 8, 0, 0x7B },
			{ 0x2F300058, 22, 0, 0x600000 }
		}
	}
};

static int regr_bit(unsigned int addr, unsigned int end_bitpos, unsigned int start_bitpos)
{
	unsigned int bit_num = end_bitpos - start_bitpos + 1;
	unsigned int bit_mask = (1 << bit_num) - 1;
	unsigned int bit_data = (*(volatile unsigned int*)(addr) >> start_bitpos) & bit_mask;

	return bit_data;
}

int check_clk_source(unsigned int source)
{
	int pll_cfg, pll_reg, reg_val;
	struct clk_src_reg reg_info;

	for (pll_cfg = 0; pll_cfg < 2; pll_cfg++) {
		if (clk_src_cfgs[pll_cfg].base_addr == source) {
			for (pll_reg = 0; pll_reg < 3; pll_reg++) {
				reg_info =  clk_src_cfgs[pll_cfg].clk_src_regs[pll_reg];
				reg_val = regr_bit(reg_info.pll_addr, reg_info.pll_end_bit, reg_info.pll_start_bit);
				if (reg_val != reg_info.pll_val) {
					printf("error cpu clk source,please check 0x%08x",reg_info.pll_addr);
					return 1;
				}
			}
		}
	}
	return 0;
}

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
	unsigned int pll_sel,ret;

	if(regr_bit(REG_CORE_BASE,1,0)==0)
		return 24000000;
	else {
		if(regr_bit(REG_LPSEL_BASE,1,1)==1) {
			ret = check_clk_source(REG_PLL_BASE);
			if(ret) {
				return -1;
			} else {
				pll_sel = regr_bit(REG_CORE_BASE,1,0);
				switch (pll_sel) {
					case 1:
						return 400000000;
					case 2:
						return 500000000;
					case 3:
						return 500000000;
					default:
						printf("error cpu sel,please check 0x%08x",REG_CORE_BASE);
						return -1;
				}
			}
		} else if (regr_bit(REG_LPSEL_BASE,1,1)==0) {
			ret = check_clk_source(REG_SPLL_BASE);
			if(ret) {
				return -1;
			} else {
				pll_sel = regr_bit(REG_CORE_BASE,1,0);
				switch (pll_sel) {
					case 1:
						ret = check_clk_source(REG_PLL_BASE);
						if(ret) {
							return -1;
						}
						return 667000000;
					case 2:
						return 743000000;
					case 3:
						return 990000000;
					default:
						printf("error cpu sel,please check 0x%08x", REG_CORE_BASE);
						return -1;
				}
			}
		}
	}
	return -1;
}
