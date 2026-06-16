/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Molchip clock driver
 *
 * Copyright (C) 2024 Molchip Electronics Co., Ltd.
 *		https://www.molchip.com
 *
 * Author: Jerry Cao <jerry.cao@molchip.com>
 */

#include "clock.h"

struct mc_clock_mini mc_fixed_clock_array[] = {
	/** FIX_FAC **/
	{CLK_32K,	"clk_32k",		FIX_FAC,	0,	0,	0,	32768,		0,	0,	0},
	{CLK_XTL_24,	"clk_xtl_24",		FIX_FAC,	0,	0,	0,	24000000,	0,	0,	0},
	{CLK_XTL_24_1,	"clk_xtl_24_1",		FIX_FAC,	0,	0,	0,	24100000,	0,	0,	0},
	{CLK_XTL_1,	"clk_xtl_1",		FIX_FAC,	0,	0,	0,	1000000,	0,	0,	0},
	{CLK_PLL_960,	"clk_pll_960",		FIX_FAC,	0,	0,	0,	960000000,	0,	0,	0},
	{CLK_PLL_800,	"clk_pll_800",		FIX_FAC,	0,	0,	0,	800000000,	0,	0,	0},
	{CLK_PLL_600,	"clk_pll_600",		FIX_FAC,	0,	0,	0,	600000000,	0,	0,	0},
	{CLK_PLL_480,	"clk_pll_480",		FIX_FAC,	0,	0,	0,	480000000,	0,	0,	0},
	{CLK_PLL_400,	"clk_pll_400",		FIX_FAC,	0,	0,	0,	400000000,	0,	0,	0},
	{CLK_PLL_300,	"clk_pll_300",		FIX_FAC,	0,	0,	0,	300000000,	0,	0,	0},
	{CLK_PLL_267,	"clk_pll_267",		FIX_FAC,	0,	0,	0,	267000000,	0,	0,	0},
	{CLK_PLL_200,	"clk_pll_200",		FIX_FAC,	0,	0,	0,	200000000,	0,	0,	0},
	{CLK_PLL_150,	"clk_pll_150",		FIX_FAC,	0,	0,	0,	150000000,	0,	0,	0},
	{CLK_PLL_133_5,	"clk_pll_133_5",	FIX_FAC,	0,	0,	0,	133500000,	0,	0,	0},
	{CLK_PLL_120,	"clk_pll_120",		FIX_FAC,	0,	0,	0,	120000000,	0,	0,	0},
	{CLK_PLL_100,	"clk_pll_100",		FIX_FAC,	0,	0,	0,	100000000,	0,	0,	0},
	{CLK_PLL_60,	"clk_pll_60",		FIX_FAC,	0,	0,	0,	60000000,	0,	0,	0},
	{CLK_PLL_50_1,	"clk_pll_50_1",		FIX_FAC,	0,	0,	0,	50100000,	0,	0,	0},
	{CLK_PLL_50,	"clk_pll_50",		FIX_FAC,	0,	0,	0,	50000000,	0,	0,	0},
	{CLK_PLL_40,	"clk_pll_40",		FIX_FAC,	0,	0,	0,	40000000,	0,	0,	0},
	{CLK_PLL_20,	"clk_pll_20",		FIX_FAC,	0,	0,	0,	20000000,	0,	0,	0},
};


/** TOP SYS **/
static const struct clk_addr_info top_gate_clks[] = {
	{0x10000200,	2,	1},	/* CKG_GLB_APB_DMC_EN */
	{0x10000200,	8,	1},	/* CKG_PMU_EN */
	{0x10000200,	9,	1},	/* CKG_DEB_EN */
	{0x10000568,	0,	1},	/* CKG_APB_JTAG_EN */
	{0x10000120,	16,	1},	/* CKG_AUX0_EN */
};

static const struct clk_addr_info top_mux_clks[] = {
	{0x10300220,	0,	2},	/* CKG_MUX_GLB_APB */
	{0x10300224,	0,	2},	/* CKG_MUX_PMU */
	{0x10300228,	0,	2},	/* CKG_MUX_DEB */
};

static const struct clk_addr_info top_mux_clks_divider[] = {
	{0x10300220,	8,	3},	/* CKG_MUX_GLB_APB_DIV **/
	{0x10300224,	8,	6},	/* CKG_MUX_PMU_DIV **/
	{0x10300228,	8,	6},	/* CKG_MUX_DEB_DIV **/
};

static const char *const ckg_mux_glb_apb_p[] =	{"clk_xtl_24",	"clk_pll_100",	"clk_pll_120",	"clk_pll_150"};
static const char *const ckg_mux_pmu_p[] =	{"clk_32k",	"clk_xtl_24",	"clk_pll_100"};
static const char *const ckg_mux_deb_p[] =	{"clk_32k",	"clk_xtl_24",	"clk_pll_100"};

struct mc_clock_mini mc_sys_top_clock[] = {
	/** top gate **/
	{CKG_GLB_APB_DMC_EN,	"ckg_glb_apb_dmc_en",	GATE_P,	0,	&top_gate_clks[0],	0,	0,	0,	0,	0},
	{CKG_PMU_EN,		"ckg_pmu_en",		GATE_P,	0,	&top_gate_clks[1],	0,	0,	0,	0,	0},
	{CKG_DEB_EN,		"ckg_deb_en",		GATE_P,	0,	&top_gate_clks[2],	0,	0,	0,	0,	0},
	{CKG_APB_JTAG_EN,	"ckg_apb_jtag_en",	GATE_P,	0,	&top_gate_clks[3],	0,	0,	0,	0,	0},
	{CKG_AUX0_EN,		"ckg_aux0_en",		GATE_P,	0,	&top_gate_clks[4],	0,	0,	0,	0,	0},

	/** top mux **/
	{CKG_MUX_GLB_APB,	"ckg_glb_apb",		MUX_P,	0,	&top_mux_clks[0],	&top_mux_clks_divider[0],	0,	ckg_mux_glb_apb_p,	ARRAY_SIZE(ckg_mux_glb_apb_p),	0},
	{CKG_MUX_PMU,		"ckg_pmu",		MUX_P,	0,	&top_mux_clks[1],	&top_mux_clks_divider[1],	0,	ckg_mux_pmu_p,		ARRAY_SIZE(ckg_mux_pmu_p),	0},
	{CKG_MUX_DEB,		"ckg_deb",		MUX_P,	0,	&top_mux_clks[2],	&top_mux_clks_divider[2],	0,	ckg_mux_deb_p,		ARRAY_SIZE(ckg_mux_deb_p),	0},
};

/* cpu sys gates */
static const struct clk_addr_info cpu_gate_clks[] = {
	{0xc0000a0,	3,	1},	/* VDEC_EN */
	{0xc0000a0,	4,	1},	/* JPG_EN */
	{0xc000000,	0,	1},	/* CKG_CA7_CORE_EN */
	{0xc0001c0,	29,	1},	/* CKG_CA7_DBG_FUNC_EN */
	{0xc0000ac,	3,	1},	/* CKG_I2C0_EN */
	{0xc0000ac,	4,	1},	/* CKG_I2C1_EN */
	{0xc0000ac,	9,	1},	/* CKG_UART0_EN */
	{0xc0000ac,	10,	1},	/* CKG_UART1_EN */
	{0xc0000ac,	11,	1},	/* CKG_UART2_EN */
	{0xc0000ac,	23,	1},	/* CKG_UART3_EN */
	{0xc0000b0,	23,	1},	/* CKG_TS_24M_EN */
	{0xc0000ac,	12,	1},	/* CKG_GPIO_DEBOUCE_EN */
	{0xc0000ac,	6,	1},	/* CKG_SPI0_EN */
	{0xc0000ac,	8,	1},	/* CKG_SPI1_EN */
	{0xc0000ac,	7,	1},	/* CKG_SPI_SLV_EN */
	{0xc0000ac,	5,	1},	/* CKG_PWM_EN */
	{0xc000000,	1,	1},	/* CKG_SYST_EN */
	{0xc000000,	4,	1},	/* CKG_XTL_TMR_EN */
	{0xc000000,	3,	1},	/* CKG_RTC_TMR_EN */
	{0xc0000a8,	0,	1},	/* CKG_SADCW_EN */
	{0xc0000b0,	26,	1},	/* CKG_I2S0_SLOW_EN */
	{0xc0000b8,	3,	1},	/* CKG_I2S0_FAST_EN */
	{0xc0000b8,	4,	1},	/* CKG_I2S0_FRAC_DIV_EN */
	{0xc0000e0,	0,	1},	/* CKG_EFUSE_EN */
	{0xc00001c,	1,	1},	/* CKG_DMA0_EN */
	{0xc0000dc,	28,	1},	/* CKG_AES_EN */
	{0xc0000dc,	23,	1},	/* CKG_AHB_HSLOCK_EN */
	{0xc0000ac,	14,	1},	/* CKG_AHB_IROM_EN */
	{0xc0000b0,	5,	1},	/* CKG_APB_I2C0_EN */
	{0xc0000b0,	6,	1},	/* CKG_APB_I2C1_EN */
	{0xc0000b0,	12,	1},	/* CKG_APB_UART0_EN */
	{0xc0000b0,	13,	1},	/* CKG_APB_UART1_EN */
	{0xc0000b0,	1,	1},	/* CKG_APB_UART2_EN */
	{0xc0000b0,	17,	1},	/* CKG_APB_UART3_EN */
	{0xc0000b0,	11,	1},	/* CKG_APB_GPIO_EN */
	{0xc0000b0,	8,	1},	/* CKG_APB_SPI0_EN */
	{0xc0000b0,	19,	1},	/* CKG_APB_SPI1_EN */
	{0xc0000b0,	16,	1},	/* CKG_APB_SPI_SLV_EN */
	{0xc0000b0,	7,	1},	/* CKG_APB_PWM_EN */
	{0xc0000a8,	2,	1},	/* CKG_APB_SADC_EN */
	{0xc0000b0,	4,	1},	/* CKG_APB_WDT0_EN */
	{0xc0000b0,	3,	1},	/* CKG_APB_INT_CTRL_EN */
	{0xc0000b0,	2,	1},	/* CKG_APB_TIMER_EN */
	{0xc0000b0,	10,	1},	/* CKG_APB_SYST_EN */
	{0xc0000b0,	22,	1},	/* CKG_APB_I2S0_EN */
	{0xc0000dc,	9,	1},	/* CKG_APB_DAP_EN */
	{0xc0001fc,	3,	1},	/* CKG_APB_EPHY_PHYCFG_EN */
	{0xc0001d4,	1,	1},	/* CKG_APB_GMAC0_CSR_EN */
	{0xc0000b0,	0,	1},	/* CKG_APB_EFUSE_EN */
	{0xc0000ac,	1,	1},	/* CKG_APB_USB2_EN */
	{0xc0000ac,	21,	1},	/* CKG_APB_PTS_EN */
	{0xc0000b0,	9,	1},	/* CKG_WDT_EN */
	{0xc0000ac,	0,	1},	/* CKG_AHB_USB2_EN */
	{0xc0001e0,	9,	1},	/* CKG_GMAC0_TX_EN */
	{0xc0001e0,	5,	1},	/* CKG_GMAC0_RX_EN */
	{0xc0001e0,	1,	1},	/* CKG_GMAC0_RMII_EN */
	{0xc0001fc,	2,	1},	/* CKG_EPHY0_REF_EN */
	{0xc0000ac,	19,	1},	/* CKG_VDEC_EN */
	{0xc0000ac,	20,	1},	/* CKG_AXI_EMAC_EN */
	{0xc0000dc,	1,	1},	/* CKG_AHB_SDIO0_EN */
	{0xc0000dc,	3,	1},	/* CKG_SDIO0_EN */
	{0xc0000dc,	2,	1},	/* CKG_SDIO0_DLLREF_EN */
	{0xc0001c0,	0,	1},	/* CKG_PTS_EN */
	{0xc000010,	4,	1},	/* CKG_CS_DBG_EN */
};

/* cpu sys mux */
static const struct clk_addr_info cpu_mux_clks[] = {
	{0xc0000c4,	5,	1},	/* CKG_MUX_I2S0_PAD */
	{0xc0000c4,	2,	3},	/* CKG_MUX_I2S0_SLOW */
	{0xc0000c4,	0,	2},	/* CKG_MUX_I2S0_FAST */
	{0xc0000b8,	2,	1},	/* CKG_MUX_I2S0_SOURCE */
	{0xc0001d4,	2,	2},	/* CKG_MUX_GMAC0_RMII_PAD */
	{0xc0001e0,	26,	1},	/* CKG_MUX_EPHY0_TX_INV */
	{0xc0001e0,	24,	1},	/* CKG_MUX_EPHY0_RX_INV */
	{0x9200220,	0,	3},	/* CKG_MUX_CA7_CORE */
	{0x920022c,	0,	2},	/* CKG_MUX_CS_DBG */
	{0x9200238,	0,	2},	/* CKG_MUX_SYST */
	{0x9200244,	0,	2},	/* CKG_MUX_PWM */
	{0x9200248,	16,	1},	/* CKG_MUX_EFUSE_PAD */
	{0x920024c,	0,	2},	/* CKG_MUX_SADCW */
	{0x9200250,	0,	2},	/* CKG_MUX_UART0 */
	{0x9200254,	0,	2},	/* CKG_MUX_UART1 */
	{0x9200258,	0,	2},	/* CKG_MUX_UART2 */
	{0x920025c,	0,	2},	/* CKG_MUX_UART3 */
	{0x9200260,	0,	2},	/* CKG_MUX_I2C0 */
	{0x9200264,	0,	2},	/* CKG_MUX_I2C1 */
	{0x9200268,	0,	2},	/* CKG_MUX_SPI0 */
	{0x920026c,	0,	2},	/* CKG_MUX_SPI1 */
	{0x9200270,	0,	2},	/* CKG_MUX_SPI_SLV */
	{0x9200274,	0,	2},	/* CKG_MUX_GPIO_DEBOUCE */
	{0x9200278,	0,	2},	/* CKG_MUX_VDU_BUS */
	{0x920027c,	0,	2},	/* CKG_MUX_SDIO0 */
	{0x92002b8,	0,	2},	/* CKG_MUX_PTS */
	{0,		0,	0},	/* CKG_CA7_BUS_DIV */
	{0,		0,	0},	/* CKG_CA7_DBG_DIV */
	{0,		0,	0},	/* CKG_TS_24M_DIV */
	{0,		0,	0},	/* CKG_EPHY_REF_DIV */
};

static const struct clk_addr_info cpu_mux_clks_divider[] = {
	{0,		0,	0},	/* CKG_MUX_I2S0_PAD */
	{0,		0,	0},	/* CKG_MUX_I2S0_SLOW */
	{0,		0,	0},	/* CKG_MUX_I2S0_FAST */
	{0,		0,	0},	/* CKG_MUX_I2S0_SOURCE */
	{0,		0,	0},	/* CKG_MUX_GMAC0_RMII_PAD */
	{0,		0,	0},	/* CKG_MUX_EPHY0_TX_INV */
	{0,		0,	0},	/* CKG_MUX_EPHY0_RX_INV */
	{0,		0,	0},	/* CKG_MUX_CA7_CORE */
	{0,		0,	0},	/* CKG_MUX_CS_DBG */
	{0x9200238,	8,	8},	/* CKG_SYST_DIV */
	{0,		0,	0},	/* CKG_MUX_PWM */
	{0,		0,	0},	/* CKG_MUX_EFUSE_PAD */
	{0x920024c,	8,	6},	/* CKG_SADCW_DIV */
	{0x9200250,	8,	3},	/* CKG_UART0_DIV */
	{0x9200254,	8,	3},	/* CKG_UART1_DIV */
	{0x9200258,	8,	3},	/* CKG_UART2_DIV */
	{0x920025c,	8,	3},	/* CKG_UART3_DIV */
	{0x9200260,	8,	3},	/* CKG_I2C0_DIV */
	{0x9200264,	8,	3},	/* CKG_I2C1_DIV */
	{0,		0,	0},	/* CKG_MUX_SPI0 */
	{0,		0,	0},	/* CKG_MUX_SPI1 */
	{0,		0,	0},	/* CKG_MUX_SPI_SLV */
	{0x9200274,	8,	8},	/* CKG_GPIO_DEBOUCE_DIV */
	{0x9200278,	8,	3},	/* CKG_VDU_BUS_DIV */
	{0,		0,	0},	/* CKG_MUX_SDIO0 */
	{0x92002b8,	8,	6},	/* CKG_PTS_DIV */
	{0x9200224,	8,	3},	/* CKG_CA7_BUS_DIV */
	{0x9200228,	8,	3},	/* CKG_CA7_DBG_DIV */
	{0x92002b4,	8,	4},	/* CKG_TS_24M_DIV */
	{0x92002bc,	8,	2},	/* CKG_EPHY_REF_DIV */
};

static const char *const ckg_mux_i2s0_pad_p[] =		{"clk_xtl_24"};
static const char *const ckg_mux_i2s0_slow_p[] =	{"clk_xtl_24",	"clk_pll_100",	"clk_pll_120",	"clk_pll_150",	"clk_pll_200"};
static const char *const ckg_mux_i2s0_fast_p[] =	{"clk_xtl_24",	"clk_pll_480",	"clk_pll_600"};
static const char *const ckg_mux_i2s0_source_p[] =	{"clk_xtl_24"};
static const char *const ckg_mux_gmac0_rmii_pad_p[] =	{"clk_xtl_24"};
static const char *const ckg_mux_ephy0_tx_inv_p[] =	{"clk_xtl_24"};
static const char *const ckg_mux_ephy0_rx_inv_p[] =	{"clk_xtl_24"};
static const char *const ckg_mux_ca7_core_p[] =		{"clk_xtl_24",	"clk_pll_480",	"clk_pll_600",	"clk_pll_800",	"clk_pll_960"};
static const char *const ckg_mux_cs_dbg_p[] =		{"clk_xtl_24",	"clk_pll_60",	"clk_pll_100"};
static const char *const ckg_mux_syst_p[] =		{"clk_32k",	"clk_xtl_1",	"clk_xtl_24"};
static const char *const ckg_mux_pwm_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100"};
static const char *const ckg_mux_efuse_pad_p[] =	{"clk_xtl_24"};
static const char *const ckg_mux_sadcw_p[] =		{"clk_xtl_24",	"clk_pll_40",	"clk_pll_60",	"clk_pll_100"};
static const char *const ckg_mux_uart0_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_uart1_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_uart2_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_uart3_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_i2c0_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_i2c1_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_spi0_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_spi1_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_spi_slv_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100",	"clk_pll_120"};
static const char *const ckg_mux_gpio_debouce_p[] =	{"clk_32k",	"clk_pll_20",	"clk_xtl_24",	"clk_pll_50"};
static const char *const ckg_mux_vdu_bus_p[] =		{"clk_xtl_24",	"clk_pll_120",	"clk_pll_150",	"clk_pll_200"};
static const char *const ckg_mux_sdio0_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100"};
static const char *const ckg_mux_pts_p[] =		{"clk_32k",	"clk_xtl_24",	"clk_pll_50"};
static const char *const ckg_mux_ca7_bus_p[] =		{"clk_xtl_24"};
static const char *const ckg_mux_ca7_dbg_p[] =		{"clk_xtl_24"};
static const char *const ckg_mux_ts_24m_p[] =		{"clk_xtl_24"};
static const char *const ckg_mux_ephy_ref_p[] =		{"clk_xtl_24"};

struct mc_clock_mini mc_sys_cpu_clock[] = {
	{VDEC_EN,			"vdec_en",			GATE_P,	0,	&cpu_gate_clks[0],	0,	0,	0,	0,	0},
	{JPG_EN,			"jpg_en",			GATE_P,	0,	&cpu_gate_clks[1],	0,	0,	0,	0,	0},
	{CKG_CA7_CORE_EN,		"ckg_ca7_core_en",		GATE_P,	0,	&cpu_gate_clks[2],	0,	0,	0,	0,	0},
	{CKG_CA7_DBG_FUNC_EN,		"ckg_ca7_dbg_func_en",		GATE_P,	0,	&cpu_gate_clks[3],	0,	0,	0,	0,	0},
	{CKG_I2C0_EN,			"ckg_i2c0_en",			GATE_P,	0,	&cpu_gate_clks[4],	0,	0,	0,	0,	0},
	{CKG_I2C1_EN,			"ckg_i2c1_en",			GATE_P,	0,	&cpu_gate_clks[5],	0,	0,	0,	0,	0},
	{CKG_UART0_EN,			"ckg_uart0_en",			GATE_P,	0,	&cpu_gate_clks[6],	0,	0,	0,	0,	0},
	{CKG_UART1_EN,			"ckg_uart1_en",			GATE_P,	0,	&cpu_gate_clks[7],	0,	0,	0,	0,	0},
	{CKG_UART2_EN,			"ckg_uart2_en",			GATE_P,	0,	&cpu_gate_clks[8],	0,	0,	0,	0,	0},
	{CKG_UART3_EN,			"ckg_uart3_en",			GATE_P,	0,	&cpu_gate_clks[9],	0,	0,	0,	0,	0},
	{CKG_TS_24M_EN,			"ckg_ts_24m_en",		GATE_P,	0,	&cpu_gate_clks[10],	0,	0,	0,	0,	0},
	{CKG_GPIO_DEBOUCE_EN,		"ckg_gpio_debouce_en",		GATE_P,	0,	&cpu_gate_clks[11],	0,	0,	0,	0,	0},
	{CKG_SPI0_EN,			"ckg_spi0_en",			GATE_P,	0,	&cpu_gate_clks[12],	0,	0,	0,	0,	0},
	{CKG_SPI1_EN,			"ckg_spi1_en",			GATE_P,	0,	&cpu_gate_clks[13],	0,	0,	0,	0,	0},
	{CKG_SPI_SLV_EN,		"ckg_spi_slv_en",		GATE_P,	0,	&cpu_gate_clks[14],	0,	0,	0,	0,	0},
	{CKG_PWM_EN,			"ckg_pwm_en",			GATE_P,	0,	&cpu_gate_clks[15],	0,	0,	0,	0,	0},
	{CKG_SYST_EN,			"ckg_syst_en",			GATE_P,	0,	&cpu_gate_clks[16],	0,	0,	0,	0,	0},
	{CKG_XTL_TMR_EN,		"ckg_xtl_tmr_en",		GATE_P,	0,	&cpu_gate_clks[17],	0,	0,	0,	0,	0},
	{CKG_RTC_TMR_EN,		"ckg_rtc_tmr_en",		GATE_P,	0,	&cpu_gate_clks[18],	0,	0,	0,	0,	0},
	{CKG_SADCW_EN,			"ckg_sadcw_en",			GATE_P,	0,	&cpu_gate_clks[19],	0,	0,	0,	0,	0},
	{CKG_I2S0_SLOW_EN,		"ckg_i2s0_slow_en",		GATE_P,	0,	&cpu_gate_clks[20],	0,	0,	0,	0,	0},
	{CKG_I2S0_FAST_EN,		"ckg_i2s0_fast_en",		GATE_P,	0,	&cpu_gate_clks[21],	0,	0,	0,	0,	0},
	{CKG_I2S0_FRAC_DIV_EN,		"ckg_i2s0_frac_div_en",		GATE_P,	0,	&cpu_gate_clks[22],	0,	0,	0,	0,	0},
	{CKG_EFUSE_EN,			"ckg_efuse_en",			GATE_P,	0,	&cpu_gate_clks[23],	0,	0,	0,	0,	0},
	{CKG_DMA0_EN,			"ckg_dma0_en",			GATE_P,	0,	&cpu_gate_clks[24],	0,	0,	0,	0,	0},
	{CKG_AES_EN,			"ckg_aes_en",			GATE_P,	0,	&cpu_gate_clks[25],	0,	0,	0,	0,	0},
	{CKG_AHB_HSLOCK_EN,		"ckg_ahb_hslock_en",		GATE_P,	0,	&cpu_gate_clks[26],	0,	0,	0,	0,	0},
	{CKG_AHB_IROM_EN,		"ckg_ahb_irom_en",		GATE_P,	0,	&cpu_gate_clks[27],	0,	0,	0,	0,	0},
	{CKG_APB_I2C0_EN,		"ckg_apb_i2c0_en",		GATE_P,	0,	&cpu_gate_clks[28],	0,	0,	0,	0,	0},
	{CKG_APB_I2C1_EN,		"ckg_apb_i2c1_en",		GATE_P,	0,	&cpu_gate_clks[29],	0,	0,	0,	0,	0},
	{CKG_APB_UART0_EN,		"ckg_apb_uart0_en",		GATE_P,	0,	&cpu_gate_clks[30],	0,	0,	0,	0,	0},
	{CKG_APB_UART1_EN,		"ckg_apb_uart1_en",		GATE_P,	0,	&cpu_gate_clks[31],	0,	0,	0,	0,	0},
	{CKG_APB_UART2_EN,		"ckg_apb_uart2_en",		GATE_P,	0,	&cpu_gate_clks[32],	0,	0,	0,	0,	0},
	{CKG_APB_UART3_EN,		"ckg_apb_uart3_en",		GATE_P,	0,	&cpu_gate_clks[33],	0,	0,	0,	0,	0},
	{CKG_APB_GPIO_EN,		"ckg_apb_gpio_en",		GATE_P,	0,	&cpu_gate_clks[34],	0,	0,	0,	0,	0},
	{CKG_APB_SPI0_EN,		"ckg_apb_spi0_en",		GATE_P,	0,	&cpu_gate_clks[35],	0,	0,	0,	0,	0},
	{CKG_APB_SPI1_EN,		"ckg_apb_spi1_en",		GATE_P,	0,	&cpu_gate_clks[36],	0,	0,	0,	0,	0},
	{CKG_APB_SPI_SLV_EN,		"ckg_apb_spi_slv_en",		GATE_P,	0,	&cpu_gate_clks[37],	0,	0,	0,	0,	0},
	{CKG_APB_PWM_EN,		"ckg_apb_pwm_en",		GATE_P,	0,	&cpu_gate_clks[38],	0,	0,	0,	0,	0},
	{CKG_APB_SADC_EN,		"ckg_apb_sadc_en",		GATE_P,	0,	&cpu_gate_clks[39],	0,	0,	0,	0,	0},
	{CKG_APB_WDT0_EN,		"ckg_apb_wdt0_en",		GATE_P,	0,	&cpu_gate_clks[40],	0,	0,	0,	0,	0},
	{CKG_APB_INT_CTRL_EN,		"ckg_apb_int_ctrl_en",		GATE_P,	0,	&cpu_gate_clks[41],	0,	0,	0,	0,	0},
	{CKG_APB_TIMER_EN,		"ckg_apb_timer_en",		GATE_P,	0,	&cpu_gate_clks[42],	0,	0,	0,	0,	0},
	{CKG_APB_SYST_EN,		"ckg_apb_syst_en",		GATE_P,	0,	&cpu_gate_clks[43],	0,	0,	0,	0,	0},
	{CKG_APB_I2S0_EN,		"ckg_apb_i2s0_en",		GATE_P,	0,	&cpu_gate_clks[44],	0,	0,	0,	0,	0},
	{CKG_APB_DAP_EN,		"ckg_apb_dap_en",		GATE_P,	0,	&cpu_gate_clks[45],	0,	0,	0,	0,	0},
	{CKG_APB_EPHY_PHYCFG_EN,	"ckg_apb_ephy_phycfg_en",	GATE_P,	0,	&cpu_gate_clks[46],	0,	0,	0,	0,	0},
	{CKG_APB_GMAC0_CSR_EN,		"ckg_apb_gmac0_csr_en",		GATE_P,	0,	&cpu_gate_clks[47],	0,	0,	0,	0,	0},
	{CKG_APB_EFUSE_EN,		"ckg_apb_efuse_en",		GATE_P,	0,	&cpu_gate_clks[48],	0,	0,	0,	0,	0},
	{CKG_APB_USB2_EN,		"ckg_apb_usb2_en",		GATE_P,	0,	&cpu_gate_clks[49],	0,	0,	0,	0,	0},
	{CKG_APB_PTS_EN,		"ckg_apb_pts_en",		GATE_P,	0,	&cpu_gate_clks[50],	0,	0,	0,	0,	0},
	{CKG_WDT_EN,			"ckg_wdt_en",			GATE_P,	0,	&cpu_gate_clks[51],	0,	0,	0,	0,	0},
	{CKG_AHB_USB2_EN,		"ckg_ahb_usb2_en",		GATE_P,	0,	&cpu_gate_clks[52],	0,	0,	0,	0,	0},
	{CKG_GMAC0_TX_EN,		"ckg_gmac0_tx_en",		GATE_P,	0,	&cpu_gate_clks[53],	0,	0,	0,	0,	0},
	{CKG_GMAC0_RX_EN,		"ckg_gmac0_rx_en",		GATE_P,	0,	&cpu_gate_clks[54],	0,	0,	0,	0,	0},
	{CKG_GMAC0_RMII_EN,		"ckg_gmac0_rmii_en",		GATE_P,	0,	&cpu_gate_clks[55],	0,	0,	0,	0,	0},
	{CKG_EPHY0_REF_EN,		"ckg_ephy0_ref_en",		GATE_P,	0,	&cpu_gate_clks[56],	0,	0,	0,	0,	0},
	{CKG_VDEC_EN,			"ckg_vdec_en",			GATE_P,	0,	&cpu_gate_clks[57],	0,	0,	0,	0,	0},
	{CKG_AXI_EMAC_EN,		"ckg_axi_emac_en",		GATE_P,	0,	&cpu_gate_clks[58],	0,	0,	0,	0,	0},
	{CKG_AHB_SDIO0_EN,		"ckg_ahb_sdio0_en",		GATE_P,	0,	&cpu_gate_clks[59],	0,	0,	0,	0,	0},
	{CKG_SDIO0_EN,			"ckg_sdio0_en",			GATE_P,	0,	&cpu_gate_clks[60],	0,	0,	0,	0,	0},
	{CKG_SDIO0_DLLREF_EN,		"ckg_sdio0_dllref_en",		GATE_P,	0,	&cpu_gate_clks[61],	0,	0,	0,	0,	0},
	{CKG_PTS_EN,			"ckg_pts_en",			GATE_P,	0,	&cpu_gate_clks[62],	0,	0,	0,	0,	0},
	{CKG_CS_DBG_EN,			"ckg_cs_dbg_en",		GATE_P,	0,	&cpu_gate_clks[63],	0,	0,	0,	0,	0},
	{CKG_MUX_I2S0_PAD,		"ckg_i2s0_pad",			MUX_P,	0,	&cpu_mux_clks[0],	0,				0,	ckg_mux_i2s0_pad_p,		ARRAY_SIZE(ckg_mux_i2s0_pad_p),		0},
	{CKG_MUX_I2S0_SLOW,		"ckg_i2s0_slow",		MUX_P,	0,	&cpu_mux_clks[1],	0,				0,	ckg_mux_i2s0_slow_p,		ARRAY_SIZE(ckg_mux_i2s0_slow_p),	0},
	{CKG_MUX_I2S0_FAST,		"ckg_i2s0_fast",		MUX_P,	0,	&cpu_mux_clks[2],	0,				0,	ckg_mux_i2s0_fast_p,		ARRAY_SIZE(ckg_mux_i2s0_fast_p),	0},
	{CKG_MUX_I2S0_SOURCE,		"ckg_i2s0_source",		MUX_P,	0,	&cpu_mux_clks[3],	0,				0,	ckg_mux_i2s0_source_p,		ARRAY_SIZE(ckg_mux_i2s0_source_p),	0},
	{CKG_MUX_GMAC0_RMII_PAD,	"ckg_gmac0_rmii_pad",		MUX_P,	0,	&cpu_mux_clks[4],	0,				0,	ckg_mux_gmac0_rmii_pad_p,	ARRAY_SIZE(ckg_mux_gmac0_rmii_pad_p),	0},
	{CKG_MUX_EPHY0_TX_INV,		"ckg_ephy0_tx_inv",		MUX_P,	0,	&cpu_mux_clks[5],	0,				0,	ckg_mux_ephy0_tx_inv_p,		ARRAY_SIZE(ckg_mux_ephy0_tx_inv_p),	0},
	{CKG_MUX_EPHY0_RX_INV,		"ckg_ephy0_rx_inv",		MUX_P,	0,	&cpu_mux_clks[6],	0,				0,	ckg_mux_ephy0_rx_inv_p,		ARRAY_SIZE(ckg_mux_ephy0_rx_inv_p),	0},
	{CKG_MUX_CA7_CORE,		"ckg_ca7_core",			MUX_P,	0,	&cpu_mux_clks[7],	0,				0,	ckg_mux_ca7_core_p,		ARRAY_SIZE(ckg_mux_ca7_core_p),		0},
	{CKG_MUX_CS_DBG,		"ckg_cs_dbg",			MUX_P,	0,	&cpu_mux_clks[8],	0,				0,	ckg_mux_cs_dbg_p,		ARRAY_SIZE(ckg_mux_cs_dbg_p),		0},
	{CKG_MUX_SYST,			"ckg_syst",			MUX_P,	0,	&cpu_mux_clks[9],	&cpu_mux_clks_divider[9],	0,	ckg_mux_syst_p,			ARRAY_SIZE(ckg_mux_syst_p),		0},
	{CKG_MUX_PWM,			"ckg_pwm",			MUX_P,	0,	&cpu_mux_clks[10],	0,				0,	ckg_mux_pwm_p,			ARRAY_SIZE(ckg_mux_pwm_p),		0},
	{CKG_MUX_EFUSE_PAD,		"ckg_efuse_pad",		MUX_P,	0,	&cpu_mux_clks[11],	0,				0,	ckg_mux_efuse_pad_p,		ARRAY_SIZE(ckg_mux_efuse_pad_p),	0},
	{CKG_MUX_SADCW,			"ckg_sadcw",			MUX_P,	0,	&cpu_mux_clks[12],	&cpu_mux_clks_divider[12],	0,	ckg_mux_sadcw_p,		ARRAY_SIZE(ckg_mux_sadcw_p),		0},
	{CKG_MUX_UART0,			"ckg_uart0",			MUX_P,	0,	&cpu_mux_clks[13],	&cpu_mux_clks_divider[13],	0,	ckg_mux_uart0_p,		ARRAY_SIZE(ckg_mux_uart0_p),		0},
	{CKG_MUX_UART1,			"ckg_uart1",			MUX_P,	0,	&cpu_mux_clks[14],	&cpu_mux_clks_divider[14],	0,	ckg_mux_uart1_p,		ARRAY_SIZE(ckg_mux_uart1_p),		0},
	{CKG_MUX_UART2,			"ckg_uart2",			MUX_P,	0,	&cpu_mux_clks[15],	&cpu_mux_clks_divider[15],	0,	ckg_mux_uart2_p,		ARRAY_SIZE(ckg_mux_uart2_p),		0},
	{CKG_MUX_UART3,			"ckg_uart3",			MUX_P,	0,	&cpu_mux_clks[16],	&cpu_mux_clks_divider[16],	0,	ckg_mux_uart3_p,		ARRAY_SIZE(ckg_mux_uart3_p),		0},
	{CKG_MUX_I2C0,			"ckg_i2c0",			MUX_P,	0,	&cpu_mux_clks[17],	&cpu_mux_clks_divider[17],	0,	ckg_mux_i2c0_p,			ARRAY_SIZE(ckg_mux_i2c0_p),		0},
	{CKG_MUX_I2C1,			"ckg_i2c1",			MUX_P,	0,	&cpu_mux_clks[18],	&cpu_mux_clks_divider[18],	0,	ckg_mux_i2c1_p,			ARRAY_SIZE(ckg_mux_i2c1_p),		0},
	{CKG_MUX_SPI0,			"ckg_spi0",			MUX_P,	0,	&cpu_mux_clks[19],	0,				0,	ckg_mux_spi0_p,			ARRAY_SIZE(ckg_mux_spi0_p),		0},
	{CKG_MUX_SPI1,			"ckg_spi1",			MUX_P,	0,	&cpu_mux_clks[20],	0,				0,	ckg_mux_spi1_p,			ARRAY_SIZE(ckg_mux_spi1_p),		0},
	{CKG_MUX_SPI_SLV,		"ckg_spi_slv",			MUX_P,	0,	&cpu_mux_clks[21],	0,				0,	ckg_mux_spi_slv_p,		ARRAY_SIZE(ckg_mux_spi_slv_p),		0},
	{CKG_MUX_GPIO_DEBOUCE,		"ckg_gpio_debouce",		MUX_P,	0,	&cpu_mux_clks[22],	&cpu_mux_clks_divider[22],	0,	ckg_mux_gpio_debouce_p,		ARRAY_SIZE(ckg_mux_gpio_debouce_p),	0},
	{CKG_MUX_VDU_BUS,		"ckg_vdu_bus",			MUX_P,	0,	&cpu_mux_clks[23],	&cpu_mux_clks_divider[23],	0,	ckg_mux_vdu_bus_p,		ARRAY_SIZE(ckg_mux_vdu_bus_p),		0},
	{CKG_MUX_SDIO0,			"ckg_sdio0",			MUX_P,	0,	&cpu_mux_clks[24],	0,				0,	ckg_mux_sdio0_p,		ARRAY_SIZE(ckg_mux_sdio0_p),		0},
	{CKG_MUX_PTS,			"ckg_pts",			MUX_P,	0,	&cpu_mux_clks[25],	&cpu_mux_clks_divider[25],	0,	ckg_mux_pts_p,			ARRAY_SIZE(ckg_mux_pts_p),		0},
	{CKG_CA7_BUS_DIV,		"ckg_ca7_bus",			MUX_P,	0,	0,			&cpu_mux_clks_divider[26],	0,	ckg_mux_ca7_bus_p,		ARRAY_SIZE(ckg_mux_ca7_bus_p),		0},
	{CKG_CA7_DBG_DIV,		"ckg_ca7_dbg",			MUX_P,	0,	0,			&cpu_mux_clks_divider[27],	0,	ckg_mux_ca7_dbg_p,		ARRAY_SIZE(ckg_mux_ca7_dbg_p),		0},
	{CKG_TS_24M_DIV,		"ckg_ts_24m",			MUX_P,	0,	0,			&cpu_mux_clks_divider[28],	0,	ckg_mux_ts_24m_p,		ARRAY_SIZE(ckg_mux_ts_24m_p),		0},
	{CKG_EPHY_REF_DIV,		"ckg_ephy_ref",			MUX_P,	0,	0,			&cpu_mux_clks_divider[29],	0,	ckg_mux_ephy_ref_p,		ARRAY_SIZE(ckg_mux_ephy_ref_p),		0},
};

static const struct clk_addr_info dmc_gate_clks[] = {
	{0x24000300,	13,	1},	/* CKG_DMC_2X_EN */
	{0x24000300,	20,	1},	/* CKG_BUSMON_TIMER_EN */
	{0x24000308,	25,	1},	/* CKG_AHB_SDIO1_EN */
	{0x24000308,	24,	1},	/* CKG_VOU_EN */
	{0x24000308,	23,	1},	/* CKG_VPPU_EN */
	{0x24000308,	21,	1},	/* CKG_SDIO1_EN */
	{0x24000308,	20,	1},	/* CKG_SDIO1_DLLREF_EN */
	{0x24000304,	10,	1},	/* CKG_PSRAM_PCLK_EN */
	{0x24000304,	7,	1},	/* CKG_PSRAM_P0_EN */
	{0x24000304,	6,	1},	/* CKG_PSRAM_P1_EN */
	{0x24000304,	5,	1},	/* CKG_PSRAM_P2_EN */
	{0x24000304,	4,	1},	/* CKG_PSRAM_P3_EN */
	{0x24000318,	20,	1},	/* CKG_CAN_EN */
	{0x2400031c,	23,	1},	/* CKG_PTS_EN */
	{0x24000300,	26,	1},	/* CKG_APB_DDR_PHY_EN */
	{0x24000300,	29,	1},	/* CKG_APB_DDRPHY_DSP_EN */
	{0x24000300,	21,	1},	/* CKG_APB_DMC_EN */
	{0x24000308,	19,	1},	/* CKG_APB_DPHY_CFG_EN */
	{0x24000308,	18,	1},	/* CKG_APB_DSI_CFG_EN */
	{0x24000308,	17,	1},	/* CKG_APB_ACW_EN */
	{0x24000318,	19,	1},	/* CKG_APB_CAN_EN */
	{0x2400031c,	22,	1},	/* CKG_APB_CAN_PTS_EN */
	{0x24000300,	8,	1},	/* CKG_DDR_BIST2_EN */
	{0x24000300,	9,	1},	/* CKG_DDR_BIST1_EN */
	{0x24000300,	2,	1},	/* CKG_PERF_MON0_EN */
	{0x24000300,	3,	1},	/* CKG_PERF_MON1_EN */
	{0x24000300,	4,	1},	/* CKG_PERF_MON2_EN */
	{0x24000300,	27,	1},	/* CKG_DDR_PHY_EN */
	{0x24000300,	0,	1},	/* CKG_DMC_EN */
	{0x24000e38,	0,	1},	/* CKG_SFC_EN */
	{0x24000e38,	2,	1},	/* CKG_APB_SFC_EN */
};

static const struct clk_addr_info dmc_mux_clks[] = {
	{0x24000300,	14,	3},	/* CKG_MUX_DMC_2X */
	{0x24000308,	30,	2},	/* CKG_MUX_VOU_BUS */
	{0x24000308,	9,	2},	/* CKG_MUX_SDIO1 */
	{0x24000304,	8,	2},	/* CKG_MUX_PSRAM_PCLK */
	{0x24000318,	30,	2},	/* CKG_MUX_CAN */
	{0x2400031c,	31,	1},	/* CKG_MUX_CAN_PTS */
	{0x24000e38,	4,	3},	/* CKG_MUX_SFC */
};

static const struct clk_addr_info dmc_mux_clks_divider[] = {
	{0x24000300,	10,	3},	/* CKG_DMC_1X_DIV */
	{0x24000308,	27,	3},	/* CKG_VOU_BUS_DIV */
	{0x0,		0,	0},	/* CKG_MUX_SDIO1 */
	{0x0,		0,	0},	/* CKG_MUX_PSRAM_PCLK */
	{0x24000318,	21,	9},	/* CKG_CAN_DIV */
	{0x2400031c,	24,	7},	/* CKG_CAN_PTS_DIV */
	{0x24000e38,	11,	3},	/* CKG_SFC_DIV */
};

static const char *const ckg_mux_dmc_2x_p[] =		{"clk_xtl_24",	"clk_pll_300",	"clk_pll_480",	"clk_pll_600"};
static const char *const ckg_mux_vou_bus_p[] =		{"clk_xtl_24",	"clk_pll_120 ",	"clk_pll_150",	"clk_pll_200"};
static const char *const ckg_mux_sdio1_p[] =		{"clk_xtl_24",	"clk_pll_50",	"clk_pll_100"};
static const char *const ckg_mux_psram_pclk_p[] =	{"clk_xtl_24",	"clk_pll_480",	"clk_pll_600",	"clk_pll_800"};
static const char *const ckg_mux_can_p[] =		{"clk_xtl_24",	"clk_pll_300",	"clk_pll_480"};
static const char *const ckg_mux_can_pts_p[] =		{"clk_pll_50",	"clk_pll_100"};
static const char *const ckg_mux_sfc_p[] =		{"clk_xtl_24",	"clk_pll_267",	"clk_pll_300",	"clk_pll_400",	"clk_pll_480"};

struct mc_clock_mini mc_sys_dmc_clock[] = {
	{CKG_DMC_2X_EN,		"ckg_dmc_2x_en",		GATE_P,	0,	&dmc_gate_clks[0],	0,	0,	0,	0,	0},
	{CKG_BUSMON_TIMER_EN,	"ckg_busmon_timer_en",		GATE_P,	0,	&dmc_gate_clks[1],	0,	0,	0,	0,	0},
	{CKG_AHB_SDIO1_EN,	"ckg_ahb_sdio1_en",		GATE_P,	0,	&dmc_gate_clks[2],	0,	0,	0,	0,	0},
	{CKG_VOU_EN,		"ckg_vou_en",			GATE_P,	0,	&dmc_gate_clks[3],	0,	0,	0,	0,	0},
	{CKG_VPPU_EN,		"ckg_vppu_en",			GATE_P,	0,	&dmc_gate_clks[4],	0,	0,	0,	0,	0},
	{CKG_SDIO1_EN,		"ckg_sdio1_en",			GATE_P,	0,	&dmc_gate_clks[5],	0,	0,	0,	0,	0},
	{CKG_SDIO1_DLLREF_EN,	"ckg_sdio1_dllref_en",		GATE_P,	0,	&dmc_gate_clks[6],	0,	0,	0,	0,	0},
	{CKG_PSRAM_PCLK_EN,	"ckg_psram_pclk_en",		GATE_P,	0,	&dmc_gate_clks[7],	0,	0,	0,	0,	0},
	{CKG_PSRAM_P0_EN,	"ckg_psram_p0_en",		GATE_P,	0,	&dmc_gate_clks[8],	0,	0,	0,	0,	0},
	{CKG_PSRAM_P1_EN,	"ckg_psram_p1_en",		GATE_P,	0,	&dmc_gate_clks[9],	0,	0,	0,	0,	0},
	{CKG_PSRAM_P2_EN,	"ckg_psram_p2_en",		GATE_P,	0,	&dmc_gate_clks[10],	0,	0,	0,	0,	0},
	{CKG_PSRAM_P3_EN,	"ckg_psram_p3_en",		GATE_P,	0,	&dmc_gate_clks[11],	0,	0,	0,	0,	0},
	{CKG_CAN_EN,		"ckg_can_en",			GATE_P,	0,	&dmc_gate_clks[12],	0,	0,	0,	0,	0},
	{CKG_CAN_PTS_EN,	"ckg_can_pts_en",		GATE_P,	0,	&dmc_gate_clks[13],	0,	0,	0,	0,	0},
	{CKG_APB_DDR_PHY_EN,	"ckg_apb_ddr_phy_en",		GATE_P,	0,	&dmc_gate_clks[14],	0,	0,	0,	0,	0},
	{CKG_APB_DDRPHY_DSP_EN,	"ckg_apb_ddrphy_dsp_en",	GATE_P,	0,	&dmc_gate_clks[15],	0,	0,	0,	0,	0},
	{CKG_APB_DMC_EN,	"ckg_apb_dmc_en",		GATE_P,	0,	&dmc_gate_clks[16],	0,	0,	0,	0,	0},
	{CKG_APB_DPHY_CFG_EN,	"ckg_apb_dphy_cfg_en",		GATE_P,	0,	&dmc_gate_clks[17],	0,	0,	0,	0,	0},
	{CKG_APB_DSI_CFG_EN,	"ckg_apb_dsi_cfg_en",		GATE_P,	0,	&dmc_gate_clks[18],	0,	0,	0,	0,	0},
	{CKG_APB_ACW_EN,	"ckg_apb_acw_en",		GATE_P,	0,	&dmc_gate_clks[19],	0,	0,	0,	0,	0},
	{CKG_APB_CAN_EN,	"ckg_apb_can_en",		GATE_P,	0,	&dmc_gate_clks[20],	0,	0,	0,	0,	0},
	{CKG_APB_CAN_PTS_EN,	"ckg_apb_can_pts_en",		GATE_P,	0,	&dmc_gate_clks[21],	0,	0,	0,	0,	0},
	{CKG_DDR_BIST2_EN,	"ckg_ddr_bist2_en",		GATE_P,	0,	&dmc_gate_clks[22],	0,	0,	0,	0,	0},
	{CKG_DDR_BIST1_EN,	"ckg_ddr_bist1_en",		GATE_P,	0,	&dmc_gate_clks[23],	0,	0,	0,	0,	0},
	{CKG_PERF_MON0_EN,	"ckg_perf_mon0_en",		GATE_P,	0,	&dmc_gate_clks[24],	0,	0,	0,	0,	0},
	{CKG_PERF_MON1_EN,	"ckg_perf_mon1_en",		GATE_P,	0,	&dmc_gate_clks[25],	0,	0,	0,	0,	0},
	{CKG_PERF_MON2_EN,	"ckg_perf_mon2_en",		GATE_P,	0,	&dmc_gate_clks[26],	0,	0,	0,	0,	0},
	{CKG_DDR_PHY_EN,	"ckg_ddr_phy_en",		GATE_P,	0,	&dmc_gate_clks[27],	0,	0,	0,	0,	0},
	{CKG_DMC_EN,		"ckg_dmc_en",			GATE_P,	0,	&dmc_gate_clks[28],	0,	0,	0,	0,	0},
	{CKG_SFC_EN,		"ckg_sfc_en",			GATE_P,	0,	&dmc_gate_clks[29],	0,	0,	0,	0,	0},
	{CKG_APB_SFC_EN,	"ckg_apb_sfc_en",		GATE_P,	0,	&dmc_gate_clks[30],	0,	0,	0,	0,	0},
	{CKG_MUX_DMC_2X,	"ckg_dmc_2x",			MUX_P,	0,	&dmc_mux_clks[0],	&dmc_mux_clks_divider[0],	0,	ckg_mux_dmc_2x_p,	ARRAY_SIZE(ckg_mux_dmc_2x_p),		0},
	{CKG_MUX_VOU_BUS,	"ckg_vou_bus",			MUX_P,	0,	&dmc_mux_clks[1],	&dmc_mux_clks_divider[1],	0,	ckg_mux_vou_bus_p,	ARRAY_SIZE(ckg_mux_vou_bus_p),		0},
	{CKG_MUX_SDIO1,		"ckg_sdio1",			MUX_P,	0,	&dmc_mux_clks[2],	0,				0,	ckg_mux_sdio1_p,	ARRAY_SIZE(ckg_mux_sdio1_p),		0},
	{CKG_MUX_PSRAM_PCLK,	"ckg_psram_pclk",		MUX_P,	0,	&dmc_mux_clks[3],	0,				0,	ckg_mux_psram_pclk_p,	ARRAY_SIZE(ckg_mux_psram_pclk_p),	0},
	{CKG_MUX_CAN,		"ckg_can",			MUX_P,	0,	&dmc_mux_clks[4],	&dmc_mux_clks_divider[4],	0,	ckg_mux_can_p,		ARRAY_SIZE(ckg_mux_can_p),		0},
	{CKG_MUX_CAN_PTS,	"ckg_can_pts",			MUX_P,	0,	&dmc_mux_clks[5],	&dmc_mux_clks_divider[5],	0,	ckg_mux_can_pts_p,	ARRAY_SIZE(ckg_mux_can_pts_p),		0},
	{CKG_MUX_SFC,		"ckg_sfc",			MUX_P,	0,	&dmc_mux_clks[6],	&dmc_mux_clks_divider[6],	0,	ckg_mux_sfc_p,		ARRAY_SIZE(ckg_mux_sfc_p),		0},
};
