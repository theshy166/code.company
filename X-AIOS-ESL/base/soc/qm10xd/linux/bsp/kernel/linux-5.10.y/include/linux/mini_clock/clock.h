/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Molchip clock driver
 *
 * Copyright (C) 2024 Molchip Electronics Co., Ltd.
 *		https://www.molchip.com
 *
 * Author: Jerry Cao <jerry.cao@molchip.com>
 */

#ifndef _MC_CLK_H_
#define _MC_CLK_H_

#define SYS_TOP_CLK_ID		0x100
#define SYS_CPU_CLK_ID		0x200
#define SYS_DMC_CLK_ID		0x400


#define INDEX_IN_ARRAY(x, y)	((x) ^ (y))

/** fixed factor **/
#define CLK_32K			0
#define CLK_XTL_24		1
#define CLK_XTL_24_1		2
#define CLK_XTL_1		3
#define CLK_PLL_960		4
#define CLK_PLL_800		5
#define CLK_PLL_600		6
#define CLK_PLL_480		7
#define CLK_PLL_400		8
#define CLK_PLL_300		9
#define CLK_PLL_267		10
#define CLK_PLL_200		11
#define CLK_PLL_150		12
#define CLK_PLL_133_5		13
#define CLK_PLL_120		14
#define CLK_PLL_100		15
#define CLK_PLL_60		16
#define CLK_PLL_50_1		17
#define CLK_PLL_50		18
#define CLK_PLL_40		19
#define CLK_PLL_20		20



/******************* TOP SYS ************************************************/
/* gate */
#define CKG_GLB_APB_DMC_EN	(SYS_TOP_CLK_ID + 0)
#define CKG_PMU_EN		(SYS_TOP_CLK_ID + 1)
#define CKG_DEB_EN		(SYS_TOP_CLK_ID + 2)
#define CKG_APB_JTAG_EN		(SYS_TOP_CLK_ID + 3)
#define CKG_AUX0_EN		(SYS_TOP_CLK_ID + 4)


/* mux */
#define CKG_MUX_GLB_APB		(SYS_TOP_CLK_ID + 5)
#define CKG_MUX_PMU		(SYS_TOP_CLK_ID + 6)
#define CKG_MUX_DEB		(SYS_TOP_CLK_ID + 7)




/******************* CPU SYS ************************************************/
/* gate */
#define VDEC_EN			(SYS_CPU_CLK_ID + 0)
#define JPG_EN			(SYS_CPU_CLK_ID + 1)
#define CKG_CA7_CORE_EN		(SYS_CPU_CLK_ID + 2)
#define CKG_CA7_DBG_FUNC_EN	(SYS_CPU_CLK_ID + 3)
#define CKG_I2C0_EN		(SYS_CPU_CLK_ID + 4)
#define CKG_I2C1_EN		(SYS_CPU_CLK_ID + 5)
#define CKG_UART0_EN		(SYS_CPU_CLK_ID + 6)
#define CKG_UART1_EN		(SYS_CPU_CLK_ID + 7)
#define CKG_UART2_EN		(SYS_CPU_CLK_ID + 8)
#define CKG_UART3_EN		(SYS_CPU_CLK_ID + 9)
#define CKG_TS_24M_EN		(SYS_CPU_CLK_ID + 10)
#define CKG_GPIO_DEBOUCE_EN	(SYS_CPU_CLK_ID + 11)
#define CKG_SPI0_EN		(SYS_CPU_CLK_ID + 12)
#define CKG_SPI1_EN		(SYS_CPU_CLK_ID + 13)
#define CKG_SPI_SLV_EN		(SYS_CPU_CLK_ID + 14)
#define CKG_PWM_EN		(SYS_CPU_CLK_ID + 15)
#define CKG_SYST_EN		(SYS_CPU_CLK_ID + 16)
#define CKG_XTL_TMR_EN		(SYS_CPU_CLK_ID + 17)
#define CKG_RTC_TMR_EN		(SYS_CPU_CLK_ID + 18)
#define CKG_SADCW_EN		(SYS_CPU_CLK_ID + 19)
#define CKG_I2S0_SLOW_EN	(SYS_CPU_CLK_ID + 20)
#define CKG_I2S0_FAST_EN	(SYS_CPU_CLK_ID + 21)
#define CKG_I2S0_FRAC_DIV_EN	(SYS_CPU_CLK_ID + 22)
#define CKG_EFUSE_EN		(SYS_CPU_CLK_ID + 23)
#define CKG_DMA0_EN		(SYS_CPU_CLK_ID + 24)
#define CKG_AES_EN		(SYS_CPU_CLK_ID + 25)
#define CKG_AHB_HSLOCK_EN	(SYS_CPU_CLK_ID + 26)
#define CKG_AHB_IROM_EN		(SYS_CPU_CLK_ID + 27)
#define CKG_APB_I2C0_EN		(SYS_CPU_CLK_ID + 28)
#define CKG_APB_I2C1_EN		(SYS_CPU_CLK_ID + 29)
#define CKG_APB_UART0_EN	(SYS_CPU_CLK_ID + 30)
#define CKG_APB_UART1_EN	(SYS_CPU_CLK_ID + 31)
#define CKG_APB_UART2_EN	(SYS_CPU_CLK_ID + 32)
#define CKG_APB_UART3_EN	(SYS_CPU_CLK_ID + 33)
#define CKG_APB_GPIO_EN		(SYS_CPU_CLK_ID + 34)
#define CKG_APB_SPI0_EN		(SYS_CPU_CLK_ID + 35)
#define CKG_APB_SPI1_EN		(SYS_CPU_CLK_ID + 36)
#define CKG_APB_SPI_SLV_EN	(SYS_CPU_CLK_ID + 37)
#define CKG_APB_PWM_EN		(SYS_CPU_CLK_ID + 38)
#define CKG_APB_SADC_EN		(SYS_CPU_CLK_ID + 39)
#define CKG_APB_WDT0_EN		(SYS_CPU_CLK_ID + 40)
#define CKG_APB_INT_CTRL_EN	(SYS_CPU_CLK_ID + 41)
#define CKG_APB_TIMER_EN	(SYS_CPU_CLK_ID + 42)
#define CKG_APB_SYST_EN		(SYS_CPU_CLK_ID + 43)
#define CKG_APB_I2S0_EN		(SYS_CPU_CLK_ID + 44)
#define CKG_APB_DAP_EN		(SYS_CPU_CLK_ID + 45)
#define CKG_APB_EPHY_PHYCFG_EN	(SYS_CPU_CLK_ID + 46)
#define CKG_APB_GMAC0_CSR_EN	(SYS_CPU_CLK_ID + 47)
#define CKG_APB_EFUSE_EN	(SYS_CPU_CLK_ID + 48)
#define CKG_APB_USB2_EN		(SYS_CPU_CLK_ID + 49)
#define CKG_APB_PTS_EN		(SYS_CPU_CLK_ID + 50)
#define CKG_WDT_EN		(SYS_CPU_CLK_ID + 51)
#define CKG_AHB_USB2_EN		(SYS_CPU_CLK_ID + 52)
#define CKG_GMAC0_TX_EN		(SYS_CPU_CLK_ID + 53)
#define CKG_GMAC0_RX_EN		(SYS_CPU_CLK_ID + 54)
#define CKG_GMAC0_RMII_EN	(SYS_CPU_CLK_ID + 55)
#define CKG_EPHY0_REF_EN	(SYS_CPU_CLK_ID + 56)
#define CKG_VDEC_EN		(SYS_CPU_CLK_ID + 57)
#define CKG_AXI_EMAC_EN		(SYS_CPU_CLK_ID + 58)
#define CKG_AHB_SDIO0_EN	(SYS_CPU_CLK_ID + 59)
#define CKG_SDIO0_EN		(SYS_CPU_CLK_ID + 60)
#define CKG_SDIO0_DLLREF_EN	(SYS_CPU_CLK_ID + 61)
#define CKG_PTS_EN		(SYS_CPU_CLK_ID + 62)
#define CKG_CS_DBG_EN		(SYS_CPU_CLK_ID + 63)
/* mux */
#define CKG_MUX_I2S0_PAD	(SYS_CPU_CLK_ID + 64)
#define CKG_MUX_I2S0_SLOW	(SYS_CPU_CLK_ID + 65)
#define CKG_MUX_I2S0_FAST	(SYS_CPU_CLK_ID + 66)
#define CKG_MUX_I2S0_SOURCE	(SYS_CPU_CLK_ID + 67)
#define CKG_MUX_GMAC0_RMII_PAD	(SYS_CPU_CLK_ID + 68)
#define CKG_MUX_EPHY0_TX_INV	(SYS_CPU_CLK_ID + 69)
#define CKG_MUX_EPHY0_RX_INV	(SYS_CPU_CLK_ID + 70)
#define CKG_MUX_CA7_CORE	(SYS_CPU_CLK_ID + 71)
#define CKG_MUX_CS_DBG		(SYS_CPU_CLK_ID + 72)
#define CKG_MUX_SYST		(SYS_CPU_CLK_ID + 73)
#define CKG_MUX_PWM		(SYS_CPU_CLK_ID + 74)
#define CKG_MUX_EFUSE_PAD	(SYS_CPU_CLK_ID + 75)
#define CKG_MUX_SADCW		(SYS_CPU_CLK_ID + 76)
#define CKG_MUX_UART0		(SYS_CPU_CLK_ID + 77)
#define CKG_MUX_UART1		(SYS_CPU_CLK_ID + 78)
#define CKG_MUX_UART2		(SYS_CPU_CLK_ID + 79)
#define CKG_MUX_UART3		(SYS_CPU_CLK_ID + 80)
#define CKG_MUX_I2C0		(SYS_CPU_CLK_ID + 81)
#define CKG_MUX_I2C1		(SYS_CPU_CLK_ID + 82)
#define CKG_MUX_SPI0		(SYS_CPU_CLK_ID + 83)
#define CKG_MUX_SPI1		(SYS_CPU_CLK_ID + 84)
#define CKG_MUX_SPI_SLV		(SYS_CPU_CLK_ID + 85)
#define CKG_MUX_GPIO_DEBOUCE	(SYS_CPU_CLK_ID + 86)
#define CKG_MUX_VDU_BUS		(SYS_CPU_CLK_ID + 87)
#define CKG_MUX_SDIO0		(SYS_CPU_CLK_ID + 88)
#define CKG_MUX_PTS		(SYS_CPU_CLK_ID + 89)
#define CKG_CA7_BUS_DIV		(SYS_CPU_CLK_ID + 90)
#define CKG_CA7_DBG_DIV		(SYS_CPU_CLK_ID + 91)
#define CKG_TS_24M_DIV		(SYS_CPU_CLK_ID + 92)
#define CKG_EPHY_REF_DIV	(SYS_CPU_CLK_ID + 93)



/*************************** DMC SYS *************************************/
#define CKG_DMC_2X_EN		(SYS_DMC_CLK_ID + 0)
#define CKG_BUSMON_TIMER_EN	(SYS_DMC_CLK_ID + 1)
#define CKG_AHB_SDIO1_EN	(SYS_DMC_CLK_ID + 2)
#define CKG_VOU_EN		(SYS_DMC_CLK_ID + 3)
#define CKG_VPPU_EN		(SYS_DMC_CLK_ID + 4)
#define CKG_SDIO1_EN		(SYS_DMC_CLK_ID + 5)
#define CKG_SDIO1_DLLREF_EN	(SYS_DMC_CLK_ID + 6)
#define CKG_PSRAM_PCLK_EN	(SYS_DMC_CLK_ID + 7)
#define CKG_PSRAM_P0_EN		(SYS_DMC_CLK_ID + 8)
#define CKG_PSRAM_P1_EN		(SYS_DMC_CLK_ID + 9)
#define CKG_PSRAM_P2_EN		(SYS_DMC_CLK_ID + 10)
#define CKG_PSRAM_P3_EN		(SYS_DMC_CLK_ID + 11)
#define CKG_CAN_EN		(SYS_DMC_CLK_ID + 12)
#define CKG_CAN_PTS_EN		(SYS_DMC_CLK_ID + 13)
#define CKG_APB_DDR_PHY_EN	(SYS_DMC_CLK_ID + 14)
#define CKG_APB_DDRPHY_DSP_EN	(SYS_DMC_CLK_ID + 15)
#define CKG_APB_DMC_EN		(SYS_DMC_CLK_ID + 16)
#define CKG_APB_DPHY_CFG_EN	(SYS_DMC_CLK_ID + 17)
#define CKG_APB_DSI_CFG_EN	(SYS_DMC_CLK_ID + 18)
#define CKG_APB_ACW_EN		(SYS_DMC_CLK_ID + 19)
#define CKG_APB_CAN_EN		(SYS_DMC_CLK_ID + 20)
#define CKG_APB_CAN_PTS_EN	(SYS_DMC_CLK_ID + 21)
#define CKG_DDR_BIST2_EN	(SYS_DMC_CLK_ID + 22)
#define CKG_DDR_BIST1_EN	(SYS_DMC_CLK_ID + 23)
#define CKG_PERF_MON0_EN	(SYS_DMC_CLK_ID + 24)
#define CKG_PERF_MON1_EN	(SYS_DMC_CLK_ID + 25)
#define CKG_PERF_MON2_EN	(SYS_DMC_CLK_ID + 26)
#define CKG_DDR_PHY_EN		(SYS_DMC_CLK_ID + 27)
#define CKG_DMC_EN		(SYS_DMC_CLK_ID + 28)
#define CKG_SFC_EN		(SYS_DMC_CLK_ID + 29)
#define CKG_APB_SFC_EN		(SYS_DMC_CLK_ID + 30)


#define CKG_MUX_DMC_2X		(SYS_DMC_CLK_ID + 31)
#define CKG_MUX_VOU_BUS		(SYS_DMC_CLK_ID + 32)
#define CKG_MUX_SDIO1		(SYS_DMC_CLK_ID + 33)
#define CKG_MUX_PSRAM_PCLK	(SYS_DMC_CLK_ID + 34)
#define CKG_MUX_CAN		(SYS_DMC_CLK_ID + 35)
#define CKG_MUX_CAN_PTS		(SYS_DMC_CLK_ID + 36)
#define CKG_MUX_SFC		(SYS_DMC_CLK_ID + 37)

enum clk_preoperty {
	FIX_RATE,
	FIX_FAC,
	GATE_P,
	MUX_P,
};

struct clk_addr_info {
	unsigned int address;
	unsigned char bit_offset;
	unsigned char bit_width;
};

struct mc_clock_mini {
	const unsigned int id;
	const char *dev_id;
	enum clk_preoperty preoperty;
	struct mc_clock_mini *parent;
	struct clk_addr_info const *addr_info;
	struct clk_addr_info const *divider_info;
	unsigned int current_rate;
	const char * const *parent_names;
	unsigned char num_parents;
	unsigned char enabled_count;
};

int clk_mini_enable(int clk_id);
int clk_mini_disable(int clk_id);
int clk_mini_set_rate(int clk_id, unsigned long rate);
unsigned long clk_mini_get_rate(int clk_id);
bool clk_mini_is_enabled(int clk_id);
#endif
