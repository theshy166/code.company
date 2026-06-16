/*
 * Copyright (c) 2019 Molchip  Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <dt-bindings/clock/xc-clock.h>
#include <linux/clk-provider.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include "clk.h"

static const struct mc_fixed_rate_clock  fixed_rate_clks[] = {
	{	XTL,			"xtl",			"crystal_24M",	0,		24000000,	},
	{	RTC,			"rtc",			"crystal_32k",	0,		32768,		},
	{	PLL_DIV2,		"pll_div2",		"PLL",			0,		1000000000,	},
	{	PLL_DIV3,		"pll_div3",		"PLL",			0,		667000000,	},
	{	PLL_DIV4,		"pll_div4",		"PLL",			0,		500000000,	},
	{	PLL_DIV5,		"pll_div5",		"PLL",			0,		400000000,	},
	{	PLL_DIV6,		"pll_div6",		"PLL",			0,		334000000,	},
	{	PLL_DIV7,		"pll_div7",		"PLL",			0,		286000000,	},
	{	PLL_1000_1,		"pll_1000_1",	"PLL",			0,		1000100000,	},
	{	PLL_990,		"pll_990",		"SPLL",			0,		990000000,	},
	{	PLL_667_1,		"pll_667_1",	"PLL",			0,		667100000,	},
	{	PLL_667_2,		"pll_667_2",	"PLL",			0,		667200000,	},
	{	PLL_667_3,		"pll_667_3",	"PLL",			0,		667300000,	},
	{	PLL_667_4,		"pll_667_4",	"PLL",			0,		667400000,	},
	{	PLL_500_1,		"pll_500_1",	"PLL",			0,		500100000,	},
	{	PLL_400_1,		"pll_400_1",	"PLL",			0,		400100000,	},
	{	PLL_286_1,		"pll_286_1",	"PLL",			0,		286100000,	},
	{	PLL_200_2,		"pll_200_2",	"PLL",			0,		200200000,	},
	{	SPLL_DIV2,		"spll_div2",	"SPLL",			0,		743000000,	},
	{	SPLL_DIV3,		"spll_div3",	"SPLL",			0,		495000000,	},
	{	SPLL_DIV4,		"spll_div4",	"SPLL",			0,		372000000,	},
	{	SPLL_DIV5,		"spll_div5",	"SPLL",			0,		297000000,	},
	{	SPLL_DIV6,		"spll_div6",	"SPLL",			0,		248000000,	},
	{	SPLL_DIV7,		"spll_div7",	"SPLL",			0,		213000000,	},
	{	SPLL_425_1,		"spll_425_1",	"SPLL",			0,		425100000,	},
	{	SPLL_594_1,		"spll_594_1",	"SPLL",			0,		594100000,	},
	{	SPLL_743_1,		"spll_743_1",	"SPLL",			0,		743100000,	},
	{	DSI_PHY,		"dsi_phy",		"DSI_PHY",		0,		297000000,	},
	{	CSI_PHY,		"csi_phy",		"CSI_PHY",		0,		187500000,	},
	{	CLK_RC,			"clk_rc",		"RC",			0,		96000000,	},
	{	RC_DIV4,		"rc_div4",		"RC",			0,		24100000,	},
	{	EPHY_REF,		"ephy_ref",		"EPLL",			0,		200100000,	},
};

/* fixed factor */
static const struct mc_fixed_factor_clock  fixed_factor_clks[] = {
	{	CLK_XTL_24,			"clk_xtl_24",		"xtl",			1,	1,	0,	},
	{	CLK_XTL_1,			"clk_xtl_1",		"xtl",			1,	24,	0,	},
	{	CLK_32K,			"clk_32k",			"rtc",			1,	1,	0,	},
	{	CLK_PLL_500,		"clk_pll_500",		"pll_div2",		1,	2,	0,	},
	{	CLK_PLL_250,		"clk_pll_250",		"pll_div2",		1,	4,	0,	},
	{	CLK_PLL_125,		"clk_pll_125",		"pll_div2",		1,	8,	0,	},
	{	CLK_PLL_333_5,		"clk_pll_333_5",	"pll_div3",		1,	2,	0,	},
	{	CLK_PLL_66_7,		"clk_pll_66_7",		"pll_div3",		1,	10,	0,	},
	{	CLK_PLL_400,		"clk_pll_400",		"pll_div5",		1,	1,	0,	},
	{	CLK_PLL_200,		"clk_pll_200",		"pll_div5",		1,	2,	0,	},
	{	CLK_PLL_100,		"clk_pll_100",		"pll_div5",		1,	4,	0,	},
	{	CLK_PLL_80,			"clk_pll_80",		"pll_div5",		1,	5,	0,	},
	{	CLK_PLL_50,			"clk_pll_50",		"pll_div5",		1,	8,	0,	},
	{	CLK_PLL_25,			"clk_pll_25",		"pll_div5",		1,	16,	0,	},
	{	CLK_PLL_20,			"clk_pll_20",		"pll_div5",		1,	20,	0,	},
	{	CLK_PLL_286,		"clk_pll_286",		"pll_div7",		1,	1 ,	0,	},
	{	CLK_PLL_500_05,		"clk_pll_500_05",	"pll_1000_1",	1,	2 ,	0,	},
	{	CLK_PLL_250_025,	"clk_pll_250_025",	"pll_1000_1",	1,	4 ,	0,	},
	{	CLK_PLL_990,		"clk_pll_990",		"pll_990",		1,	1 ,	0,	},
	{	CLK_PLL_667_1,		"clk_pll_667_1",	"pll_667_1",	1,	1 ,	0,	},
	{	CLK_PLL_333_6,		"clk_pll_333_6",	"pll_667_2",	1,	2 ,	0,	},
	{	CLK_PLL_333_65,		"clk_pll_333_65",	"pll_667_3",	1,	2 ,	0,	},
	{	CLK_PLL_667_4,		"clk_pll_667_4",	"pll_667_4",	1,	1 ,	0,	},
	{	CLK_PLL_500_1,		"clk_pll_500_1",	"pll_500_1",	1,	1 ,	0,	},
	{	CLK_PLL_400_1,		"clk_pll_400_1",	"pll_400_1",	1,	1 ,	0,	},
	{	CLK_PLL_200_2,		"clk_pll_200_2",	"pll_200_2",	1,	1 ,	0,	},
	{	CLK_PLL_286_1,		"clk_pll_286_1",	"pll_286_1",	1,	1 ,	0,	},
	{	CLK_SPLL_45,		"clk_spll_45",		"spll_div3",	1,	11,	0,	},
	{	CLK_SPLL_297,		"clk_spll_297",		"spll_div5",	1,	1,	0,	},
	{	CLK_SPLL_148_5,		"clk_spll_148_5",	"spll_div5",	1,	2,	0,	},
	{	CLK_SPLL_74_25,		"clk_spll_74_25",	"spll_div5",	1,	4,	0,	},
	{	CLK_SPLL_27,		"clk_spll_27",		"spll_div5",	1,	11,	0,	},
	{	CLK_SPLL_425_1,		"clk_spll_425_1",	"spll_425_1",	1,	1,	0,	},
	{	CLK_SPLL_594_1,		"clk_spll_594_1",	"spll_594_1",	1,	1,	0,	},
	{	CLK_SPLL_743_1,		"clk_spll_743_1",	"spll_743_1",	1,	1,	0,	},
	{	CLK_RC_200_1,		"clk_rc_200_1",		"clk_rc",		1,	1,	0,	},
	{	CLK_RC_24_1,		"clk_rc_24_1",		"rc_div4",		1,	1,	0,	},

};

static const struct mc_gate_clock  top_gate_clks[] = {
	{CKG_PTS_EN					, "ckg_pts_en"				 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 19, 0, 0},
	{CKG_GLB_AON_CPU_EN			, "ckg_glb_aon_cpu_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 24, 0, 0},
	{CKG_GLB_AON_ISP_EN			, "ckg_glb_aon_isp_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 25, 0, 0},
	{CKG_GLB_AON_VEU_EN			, "ckg_glb_aon_veu_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 26, 0, 0},
	{CKG_ARC600_0_EN			, "ckg_arc600_0_en"			 	, "arc600_0_ctrl"		, CLK_IGNORE_UNUSED, 0x0030, 1,  0, 0},
	{CKG_ARC600_1_EN			, "ckg_arc600_1_en"			 	, "arc600_1_ctrl"		, CLK_IGNORE_UNUSED, 0x0050, 1,  0, 0},
	{CKG_DMA1_EN				, "ckg_dma1_en"				 	, "ckg_peri_gate_ctrl" 	, CLK_IGNORE_UNUSED, 0x0204, 18, 0, 0},
	{CKG_GLB_APB_DMC_EN			, "ckg_glb_apb_dmc_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 17, 0, 0},
	{CKG_GLB_APB_VOU_EN			, "ckg_glb_apb_vou_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 16, 0, 0},
	{CKG_GLB_APB_NNP_EN			, "ckg_glb_apb_nnp_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 15, 0, 0},
	{CKG_GLB_APB_VEU_EN			, "ckg_glb_apb_veu_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 14, 0, 0},
	{CKG_GLB_APB_ISP_EN			, "ckg_glb_apb_isp_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 13, 0, 0},
	{CKG_APB_WDT0_EN			, "ckg_apb_wdt0_en"			 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 11, 0, 0},
	{CKG_APB_WDT1_EN			, "ckg_apb_wdt1_en"			 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 10, 0, 0},
	{CKG_APB_SYST_EN			, "ckg_apb_syst_en"			 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 9 , 0, 0},
	{CKG_SYST_EN				, "ckg_syst_en"				 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 13, 0, 0},
	{CKG_APB_UART3_EN			, "ckg_apb_uart3_en"			, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 8 , 0, 0},
	{CKG_UART3_EN				, "ckg_uart3_en"				, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 5 , 0, 0},
	{CKG_APB_UART2_EN			, "ckg_apb_uart2_en"			, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 7 , 0, 0},
	{CKG_UART2_EN				, "ckg_uart2_en"				, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 6 , 0, 0},
	{CKG_APB_RTCT_EN			, "ckg_apb_rtct_en"			 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 6 , 0, 0},
	{CKG_APB_GPIO_EN			, "ckg_apb_gpio_en"			 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 5 , 0, 0},
	{CKG_APB_TIMER0_EN			, "ckg_apb_timer0_en"			, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 4 , 0, 0},
	{CKG_APB_TIMER1_EN			, "ckg_apb_timer1_en"			, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 3 , 0, 0},
	{CKG_APB_TIMER2_EN			, "ckg_apb_timer2_en"			, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 2 , 0, 0},
	{CKG_APB_I2C2_EN			, "ckg_apb_i2c2_en"			 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 1 , 0, 0},
	{CKG_I2C2_EN				, "ckg_i2c2_en"				 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 4 , 0, 0},
	{CKG_APB_INT_CTRL_EN		, "ckg_apb_int_ctrl_en"		 	, "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 0 , 0, 0},
	{CKG_WDT_EN					, "ckg_wdt_en"				 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 14, 0, 0},
	{CKG_XTL_TMR0_EN			, "ckg_xtl_tmr0_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 12, 0, 0},
	{CKG_RTC_TMR0_EN			, "ckg_rtc_tmr0_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 11, 0, 0},
	{CKG_XTL_TMR1_EN			, "ckg_xtl_tmr1_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 10, 0, 0},
	{CKG_RTC_TMR1_EN			, "ckg_rtc_tmr1_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 9 , 0, 0},
	{CKG_XTL_TMR2_EN			, "ckg_xtl_tmr2_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 8 , 0, 0},
	{CKG_RTC_TMR2_EN			, "ckg_rtc_tmr2_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 7 , 0, 0},
	{CKG_GPIO_DEBOUCE_EN		, "ckg_gpio_debouce_en"		 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 22, 0, 0},
	{CKG_CAM_IF_EN				, "ckg_cam_if_en"				, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 3 , 0, 0},
	{CKG_SENSOR0_EN				, "ckg_sensor0_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 2 , 0, 0},
	{CKG_SENSOR1_EN				, "ckg_sensor1_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 1 , 0, 0},
	{CKG_SENSOR2_EN				, "ckg_sensor2_en"			 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 0 , 0, 0},
	{CKG_PMU_EN					, "ckg_pmu_en"				 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 15, 0, 0},
	{CKG_DEB_EN					, "ckg_deb_en"				 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 16, 0, 0},
	{CKG_DVPIN_DLLREF_EN		, "ckg_dvpin_dllref_en"		 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 17, 0, 0},
	{CKG_BUSMON_TIMER_EN		, "ckg_busmon_timer_en"		 	, "ckg_peri_gate_ctrl"  , CLK_IGNORE_UNUSED, 0x0204, 21, 0, 0},
	{CKG_AUX0_EN				, "ckg_aux0_en"				 	, "clk_aux_test_cfg"	, CLK_IGNORE_UNUSED, 0x0120, 16, 0, 0},
	{CKG_APB_C2C_CA7_TO_ARC0_EN	, "ckg_apb_c2c_ca7_to_arc0_en"  , "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 23, 0, 0},
	{CKG_APB_C2C_ARC0_TO_CA7_EN	, "ckg_apb_c2c_arc0_to_ca7_en"  , "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 22, 0, 0},
	{CKG_APB_C2C_CA7_TO_ARC1_EN	, "ckg_apb_c2c_ca7_to_arc1_en"  , "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 21, 0, 0},
	{CKG_APB_C2C_ARC1_TO_CA7_EN	, "ckg_apb_c2c_arc1_to_ca7_en"  , "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 20, 0, 0},
	{CKG_APB_C2C_ARC0_TO_ARC1_EN, "ckg_apb_c2c_arc0_to_arc1_en" , "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 19, 0, 0},
	{CKG_APB_C2C_ARC1_TO_ARC0_EN, "ckg_apb_c2c_arc1_to_arc0_en" , "ckg_apb_gate_ctrl"	, CLK_IGNORE_UNUSED, 0x0200, 18, 0, 0},
	{CKG_APB_NNP_EN	, "ckg_apb_nnp_en"	, "share_ram_clk_ctrll", CLK_IGNORE_UNUSED, 0x092c, 13, 0, 0},
	{CKG_NNP_EN		, "ckg_nnp_en"		, "share_ram_clk_ctrll", CLK_IGNORE_UNUSED, 0x092c, 14, 0, 0},
	{CKG_VDSP_EN	 , "ckg_vdsp_en"	, "share_ram_clk_ctrll", CLK_IGNORE_UNUSED, 0x092c, 12, 0, 0},
};

static const struct mc_gate_clock  cpu_gate_clks[] = {
	{CKG_CA7_CORE_EN		  , "ckg_ca7_core_en"		   , "cpll_ctrl0"			  , CLK_IGNORE_UNUSED  , 0x0000  , 0   , 0  , 0},
	{CKG_CPU_DBG_EN 		  , "ckg_cpu_dbg_en"		   , "cpll_ctrl0"			  , CLK_IGNORE_UNUSED  , 0x0000  , 2   , 0  , 0},
	{DAP_EN 				  , "dap_en"				   , "cpu_clk_gate_ctrl"	  , CLK_IGNORE_UNUSED  , 0x0010  , 4   , 0  , 0},
	{CKG_DMA0_EN			  , "ckg_dma0_en"			   , "dma_ctrl" 			  , CLK_IGNORE_UNUSED  , 0x001c  , 1   , 0  , 0},
	{CKG_AES_EN 			  , "ckg_aes_en"			   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 28  , 0  , 0},
	{CKG_AHB_RNG_EN 		  , "ckg_ahb_rng_en"		   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 24  , 0  , 0},
	{CKG_AHB_HSLOCK_EN		  , "ckg_ahb_hslock_en" 	   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 23  , 0  , 0},
	{CKG_AHB_IROM_EN		  , "ckg_ahb_irom_en"		   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 14  , 0  , 0},
	{CKG_APB_RSA_EN 		  , "ckg_apb_rsa_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 3   , 0  , 0},
	{CKG_RSA_EN 			  , "ckg_rsa_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 1   , 0  , 0},
	{CKG_APB_HASH_EN		  , "ckg_apb_hash_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 4   , 0  , 0},
	{CKG_HASH_EN			  , "ckg_hash_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 2   , 0  , 0},
	{CKG_APB_SRC_EN 		  , "ckg_apb_src_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 1   , 0  , 0},
	{CKG_SRC_EN 			  , "ckg_src_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 0   , 0  , 0},
	{CKG_APB_I2C0_EN		  , "ckg_apb_i2c0_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 5   , 0  , 0},
	{CKG_I2C0_EN			  , "ckg_i2c0_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 3   , 0  , 0},
	{CKG_APB_I2C1_EN		  , "ckg_apb_i2c1_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 6   , 0  , 0},
	{CKG_I2C1_EN			  , "ckg_i2c1_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 4   , 0	, 0},
	{CKG_APB_I2C3_EN		  , "ckg_apb_i2c3_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 15  , 0	, 0},
	{CKG_I2C3_EN			  , "ckg_i2c3_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 13  , 0	, 0},
	{CKG_APB_UART0_EN		  , "ckg_apb_uart0_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 12  , 0	, 0},
	{CKG_UART0_EN			  , "ckg_uart0_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 9   , 0	, 0},
	{CKG_APB_UART1_EN		  , "ckg_apb_uart1_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 13  , 0	, 0},
	{CKG_UART1_EN			  , "ckg_uart1_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 10  , 0	, 0},
	{CKG_APB_DMIC0_EN		  , "ckg_apb_dmic0_en"		   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 19  , 0	, 0},
	{CKG_DMIC0_EN			  , "ckg_dmic0_en"			   , "dmic0_ctrl0"			  , CLK_IGNORE_UNUSED  , 0x018c  , 24  , 0	, 0},
	{CKG_APB_DMIC1_EN		  , "ckg_apb_dmic1_en"		   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 18  , 0	, 0},
	{CKG_DMIC1_EN			  , "ckg_dmic1_en"			   , "dmic1_ctrl0"			  , CLK_IGNORE_UNUSED  , 0x0194  , 24  , 0	, 0},
	{CKG_APB_GPIO_CLK_EN	  , "ckg_apb_gpio_clk_en"	   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 11  , 0	, 0},
	{CKG_APB_I2S0_EN		  , "ckg_apb_i2s0_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 10  , 0	, 0},
	{CKG_APB_I2S1_EN		  , "ckg_apb_i2s1_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 14  , 0	, 0},
	{CKG_APB_SPI0_EN		  , "ckg_apb_spi0_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 8   , 0	, 0},
	{CKG_SPI0_EN			  , "ckg_spi0_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 6   , 0	, 0},
	{CKG_APB_SPI1_EN		  , "ckg_apb_spi1_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 9   , 0	, 0},
	{CKG_SPI1_EN			  , "ckg_spi1_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 7   , 0	, 0},
	{CKG_APB_PWM_EN 		  , "ckg_apb_pwm_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 7   , 0	, 0},
	{CKG_PWM_EN 			  , "ckg_pwm_en"			   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 5   , 0	, 0},
	{CKG_APB_MOTORCTL0_EN	  , "ckg_apb_motorctl0_en"	   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 16  , 0	, 0},
	{CKG_APB_MOTORCTL1_EN	  , "ckg_apb_motorctl1_en"	   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 17  , 0	, 0},
	{CKG_APB_EFUSE_EN		  , "ckg_apb_efuse_en"		   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 0   , 0	, 0},
	{CKG_EFUSE_EN			  , "ckg_efuse_en"			   , "ap_mtx_clk_ctrl1" 	  , CLK_IGNORE_UNUSED  , 0x00e0  , 0   , 0	, 0},
	{CKG_APB_DAP_EN 		  , "ckg_apb_dap_en"		   , "ap_mtx_clk_ctrl0" 	  , CLK_IGNORE_UNUSED  , 0x00dc  , 9   , 0	, 0},
	{CKG_GPIO_DEBOUCE_CLK_EN  , "ckg_gpio_debouce_clk_en"  , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 12  , 0	, 0},
	{CKG_MOTORCTL0_STM_EN	  , "ckg_motorctl0_stm_en"	   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 16  , 0	, 0},
	{CKG_MOTORCTL1_STM_EN	  , "ckg_motorctl1_stm_en"	   , "cen_peri_apb_clk_ctrl"  , CLK_IGNORE_UNUSED  , 0x00b0  , 19  , 0	, 0},
	{CKG_I2S0_SLOW_EN		  , "ckg_i2s0_slow_en"		   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 8   , 0	, 0},
	{CKG_I2S0_FAST_EN		  , "ckg_i2s0_fast_en"		   , "iis0_clk_ctrl"		  , CLK_IGNORE_UNUSED  , 0x00b8  , 3   , 0	, 0},
	{CKG_I2S0_FRAC_DIV_EN	  , "ckg_i2s0_frac_div_en"	   , "iis0_clk_ctrl"		  , CLK_IGNORE_UNUSED  , 0x00b8  , 4   , 0	, 0},
	{CKG_I2S1_SLOW_EN		  , "ckg_i2s1_slow_en"		   , "cen_peri_clk_ctrl"	  , CLK_IGNORE_UNUSED  , 0x00ac  , 11  , 0	, 0},
	{CKG_I2S1_FAST_EN		  , "ckg_i2s1_fast_en"		   , "iis1_clk_ctrl"		  , CLK_IGNORE_UNUSED  , 0x00e4  , 3   , 0	, 0},
	{CKG_I2S1_FRAC_DIV_EN	  , "ckg_i2s1_frac_div_en"	   , "iis1_clk_ctrl"		  , CLK_IGNORE_UNUSED  , 0x00e4  , 4   , 0	, 0},
};    
	
static const struct mc_gate_clock  isp_gate_clks[] = {                                        
	{CKG_CSI_CFG_EN			 , "ckg_csi_cfg_en"			 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 28, 0, 0},
	{CKG_EPHY_REF_EN		 , "ckg_ephy_ref_en"		 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0004, 27, 0, 0},
	{CKG_IR_EN				 , "ckg_ir_en"				 , "ckg_cfg2", CLK_IGNORE_UNUSED, 0x0008, 26, 0, 0},
	{CKG_AHB_VPU_EN			 , "ckg_ahb_vpu_en"			 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 10, 0, 0},
	{CKG_VPU_EN				 , "ckg_vpu_en"				 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 7 , 0, 0},
	{CKG_VICAP_EN			 , "ckg_vicap_en"			 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 6	, 0, 0},
	{CKG_AHB_2DLUT_EN		 , "ckg_ahb_2dlut_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 28, 0, 0},
	{CKG_2DLUT_EN			 , "ckg_2dlut_en"			 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 29, 0, 0},
	{CKG_AHB_ISP_EN			 , "ckg_ahb_isp_en"			 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 11, 0, 0},
	{CKG_ISP_EN				 , "ckg_isp_en"				 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 9	, 0, 0},
	{CKG_APB_USB3_CFG_EN	 , "ckg_apb_usb3_cfg_en"	 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 0	, 0, 0},
	{CKG_APB_DPHYRX_PCS_EN	 , "ckg_apb_dphyrx_pcs_en"	 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 16, 0, 0},
	{CKG_APB_GMAC1_CSR_EN	 , "ckg_apb_gmac1_csr_en"	 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 18, 0, 0},
	{CKG_AXI_USB3_EN		 , "ckg_axi_usb3_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 1	, 0, 0},
	{CKG_AXI_GMAC1_EN		 , "ckg_axi_gmac1_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 19, 0, 0},
	{CKG_CSI_4L_APB_EN		 , "ckg_csi_4l_apb_en"		 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 5	, 0, 0},
	{CKG_CSI_2L_APB_EN		 , "ckg_csi_2l_apb_en"		 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 4	, 0, 0},
	{CKG_MIPI0_ADAPTER_EN	 , "ckg_mipi0_adapter_en"	 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 3	, 0, 0},
	{CKG_MIPI0_ADAPTER_APB_EN, "ckg_mipi0_adapter_apb_en", "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 2	, 0, 0},
	{CKG_MIPI1_ADAPTER_EN	 , "ckg_mipi1_adapter_en"	 , "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 1	, 0, 0},
	{CKG_MIPI1_ADAPTER_APB_EN, "ckg_mipi1_adapter_apb_en", "ckg_cfg0", CLK_IGNORE_UNUSED, 0x0000, 0	, 0, 0},
	{CKG_USB3_SUSPEND_EN	 , "ckg_usb3_suspend_en"	 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 7	, 0, 0},
	{CKG_USB32_REF_EN		 , "ckg_usb32_ref_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 11, 0, 0},
	{CKG_GMAC1_TX_EN		 , "ckg_gmac1_tx_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 20, 0, 0},
	{CKG_GMAC1_RX_EN		 , "ckg_gmac1_rx_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 21, 0, 0},
	{CKG_GMAC1_RMII_EN		 , "ckg_gmac1_rmii_en"		 , "ckg_cfg1", CLK_IGNORE_UNUSED, 0x0004, 22, 0, 0},
};

static const struct mc_gate_clock nnp_gate_clks[] = {
	{CKG_VPPU400_EN, "ckg_vppu400_en", "ckg_ctrl", CLK_IGNORE_UNUSED, 0x0024, 10, 0, 0},
};

static const struct mc_gate_clock veu_gate_clks[] = { 
	{CKG_VENC460_EN			, "ckg_venc460_en"			, "venc460_cfg"				,CLK_IGNORE_UNUSED, 0x0020, 7 , 0, 0},
	{CKG_LZMA_CORE_EN		, "ckg_lzma_core_en"		, "ioe_cfg"					,CLK_IGNORE_UNUSED, 0x0014, 17, 0, 0},
	{CKG_EMMC_2X_EN			, "ckg_emmc_2x_en"			, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 24, 0, 0},
	{CKG_EMMC_EN			, "ckg_emmc_en"				, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 23, 0, 0},
	{CKG_AHB_EMMC_EN		, "ckg_ahb_emmc_en"			, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 26, 0, 0},
	{CKG_EMMC_BASE_EN		, "ckg_emmc_base_en"		, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 22, 0, 0},
	{CKG_EMMC_CQETM_EN		, "ckg_emmc_cqetm_en"		, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 21, 0, 0},
	{CKG_EMMC_TM_EN			, "ckg_emmc_tm_en"			, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 20, 0, 0},
	{CKG_SDIO0_EN			, "ckg_sdio0_en"			, "sdio0_ctrl2"				,CLK_IGNORE_UNUSED, 0x018c, 5 , 0, 0},
	{CKG_AHB_SDIO0_EN		, "ckg_ahb_sdio0_en"		, "sdio0_ctrl2"				,CLK_IGNORE_UNUSED, 0x018c, 0 , 0, 0},
	{CKG_SDIO0_DLLREF_EN	, "ckg_sdio0_dllref_en"		, "sdio0_ctrl2"				,CLK_IGNORE_UNUSED, 0x018c, 4 , 0, 0},
	{CKG_SDIO1_EN			, "ckg_sdio1_en"			, "sdio1_ctrl2"				,CLK_IGNORE_UNUSED, 0x0190, 5 , 0, 0},
	{CKG_AHB_SDIO1_EN		, "ckg_ahb_sdio1_en"		, "sdio1_ctrl2"				,CLK_IGNORE_UNUSED, 0x0190, 0 , 0, 0},
	{CKG_SDIO1_DLLREF_EN	, "ckg_sdio1_dllref_en"		, "sdio1_ctrl2"				,CLK_IGNORE_UNUSED, 0x0190, 4 , 0, 0},
	{CKG_SFC_EN				, "ckg_sfc_en"				, "sfc_ctrl0"				,CLK_IGNORE_UNUSED, 0x01a4, 0 , 0, 0},
	{CKG_APB_SFC_EN			, "ckg_apb_sfc_en"			, "sfc_ctrl0"				,CLK_IGNORE_UNUSED, 0x01a4, 1 , 0, 0},
	{CKG_SADCW_EN			, "ckg_sadcw_en"			, "sadc_ctrl"				,CLK_IGNORE_UNUSED, 0x0188, 0 , 0, 0},
	{CKG_EPHY0_REF_EN		, "ckg_ephy0_ref_en"		, "ephy_ctrl0"				,CLK_IGNORE_UNUSED, 0x01a0, 2 , 0, 0},
	{CKG_APB_EPHY_PHYCFG_EN	, "ckg_apb_ephy_phycfg_en"	, "ephy_ctrl0"				,CLK_IGNORE_UNUSED, 0x01a0, 3 , 0, 0},
	{CKG_APB_EMMC_PHYCFG_EN	, "ckg_apb_emmc_phycfg_en"	, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 27, 0, 0},
	{CKG_APB_AUDIO_CODEC_EN	, "ckg_apb_audio_codec_en"	, "audiocodec_ctrl"			,CLK_IGNORE_UNUSED, 0x01e4, 4 , 0, 0},
	{CKG_APB_PTS_EN			, "ckg_apb_pts_en"			, "pts_cfg"					,CLK_IGNORE_UNUSED, 0x0024, 0 , 0, 0},
	{CKG_APB_SADC_EN		, "ckg_apb_sadc_en"			, "sadc_ctrl"				,CLK_IGNORE_UNUSED, 0x0188, 2 , 0, 0},
	{CKG_APB_GMAC0_CSR_EN	, "ckg_apb_gmac0_csr_en"	, "gmac0_cfg"				,CLK_IGNORE_UNUSED, 0x0028, 1 , 0, 0},
	{CKG_AHB_JPG_EN			, "ckg_ahb_jpg_en"			, "jpg_cfg"					,CLK_IGNORE_UNUSED, 0x0018, 8 , 0, 0},
	{CKG_JPG_EN				, "ckg_jpg_en"				, "jpg_cfg"					,CLK_IGNORE_UNUSED, 0x0018, 7 , 0, 0},
	{CKG_AHB_ISP460_EN		, "ckg_ahb_isp460_en"		, "isp460_cfg"				,CLK_IGNORE_UNUSED, 0x001c, 8 , 0, 0},
	{CKG_ISP460_EN			, "ckg_isp460_en"			, "isp460_cfg"				,CLK_IGNORE_UNUSED, 0x001c, 7 , 0, 0},
	{CKG_APB_ENC_BUF_CTRL_EN, "ckg_apb_enc_buf_ctrl_en"	, "veu_sys_prerolling_ctrl"	,CLK_IGNORE_UNUSED, 0x01bc, 4 , 0, 0},
	{CKG_AXI_EMMC_EN		, "ckg_axi_emmc_en"			, "emmc_ctrl0"				,CLK_IGNORE_UNUSED, 0x0194, 25, 0, 0},
	{CKG_AXI_GMAC0_EN		, "ckg_axi_gmac0_en"		, "gmac0_cfg"				,CLK_IGNORE_UNUSED, 0x0028, 0 , 0, 0},
	{CKG_IOE_PUB_EN			, "ckg_ioe_pub_en"			, "ioe_cfg"					,CLK_IGNORE_UNUSED, 0x0014, 18, 0, 0},
	{CKG_IOE_CH0_EN			, "ckg_ioe_ch0_en"			, "ioe_cfg"					,CLK_IGNORE_UNUSED, 0x0014, 16, 0, 0},
	{CKG_GMAC0_TX_EN		, "ckg_gmac0_tx_en"			, "gmac_ctrl_1"				,CLK_IGNORE_UNUSED, 0x0104, 9 , 0, 0},
	{CKG_GMAC0_RX_EN		, "ckg_gmac0_rx_en"			, "gmac_ctrl_1"				,CLK_IGNORE_UNUSED, 0x0104, 5 , 0, 0},
	{CKG_GMAC0_RMII_EN		, "ckg_gmac0_rmii_en"		, "gmac_ctrl_1"				,CLK_IGNORE_UNUSED, 0x0104, 1 , 0, 0},
};

static const struct mc_gate_clock vou_gate_clks[] = {
	{CKG_APB_DPHY_CFG_EN , "ckg_apb_dphy_cfg_en", "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 18, 0, 0},
	{CKG_APB_DSI_CFG_EN	 , "ckg_apb_dsi_cfg_en"	, "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 17, 0, 0},
	{CKG_AHB_VGS_EN		 , "ckg_ahb_vgs_en"		, "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 22, 0, 0},
	{CKG_VGS_EN			 , "ckg_vgs_en"			, "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 20, 0, 0},
	{CKG_G2D_EN			 , "ckg_g2d_en"			, "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 13, 0, 0},
	{CKG_VDEC460_EN		 , "ckg_vdec460_en"		, "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 15, 0, 0},
	{CKG_VOU400_EN		 , "ckg_vou400_en"		, "ck_ctrl_0", CLK_IGNORE_UNUSED, 0x0000, 14, 0, 0},
};

static const char *const ckg_mux_pts_p[]= {"clk_xtl_24"			, "clk_pll_25"		, "clk_pll_50"};
static const char *const ckg_mux_syst_p[]= {"clk_32k"   		, "clk_xtl_1"		, "clk_xtl_24"};
static const char *const ckg_mux_uart3_p[]= {"clk_xtl_24"		, "clk_rc_200_1"	, "clk_pll_100"	,	 "clk_pll_125",		 "clk_spll_148_5"};
static const char *const ckg_mux_uart2_p[]= {"clk_xtl_24"		, "clk_rc_200_1"	, "clk_pll_100"	,	 "clk_pll_125",		 "clk_spll_148_5"};
static const char *const ckg_mux_i2c2_p[]= {"clk_xtl_24"		, "clk_rc_200_1"	, "clk_pll_125"	,	 "clk_spll_148_5"};
static const char *const ckg_mux_gpio_debouc_p[]= {"clk_32k"   	,	"clk_xtl_1"		, "clk_xtl_24"	,	 "clk_rc_24_1"	,	"clk_pll_50"};
static const char *const ckg_mux_cam_if_p[]= {"clk_xtl_24"		, "clk_spll_74_25"	, "clk_pll_100"	,	 "clk_rc_200_1"};
static const char *const ckg_mux_sensor0_p[]= {"clk_xtl_24"		, "clk_spll_27"		, "clk_spll_45"	,	 "clk_rc_24_1"	,	 "clk_spll_74_25"};
static const char *const ckg_mux_sensor1_p[]= {"clk_xtl_24"		, "clk_spll_27"		, "clk_spll_45"	,	 "clk_rc_24_1"	,	 "clk_spll_74_25"};
static const char *const ckg_mux_sensor2_p[]= {"clk_xtl_24"		, "clk_spll_27"		, "clk_spll_45"	,	 "clk_rc_24_1"	,	 "clk_spll_74_25"};
static const char *const ckg_mux_glb_apb_cfg_p[]= {"clk_xtl_24"	, "clk_pll_100"		, "clk_rc_200_1",	 "clk_pll_200_2"};
static const char *const ckg_mux_glb_aon_cfg_p[]= {"clk_xtl_24"	, "clk_spll_74_25"	, "clk_rc_200_1",	 "clk_pll_286"	,	 "clk_pll_400"};
static const char *const ckg_mux_vou_bus_p[]= {"clk_xtl_24"     , "clk_rc_200_1"    ,"clk_pll_250_025",  "clk_pll_333_65"};


static const struct mc_composite  top_composite_clks[] = {
	MUX_DIV_FLAGS( CKG_MUX_PTS			, "ckg_pts",		 ckg_mux_pts_p		, 0x0220, 0, 2 , 8, 6, CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CKG_MUX_SYST			, "ckg_syst",		 ckg_mux_syst_p		, 0x0228, 0, 2 , 8, 8, CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CKG_MUX_UART3		, "ckg_uart3",		 ckg_mux_uart3_p		, 0x0248, 0, 3 , 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_UART2		, "ckg_uart2",		 ckg_mux_uart2_p		, 0x0244, 0, 3 , 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_I2C2			, "ckg_i2c2",		 ckg_mux_i2c2_p		, 0x024c, 0, 2 , 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_GPIO_DEBOUC	, "ckg_gpio_debouc", ckg_mux_gpio_debouc_p, 0x0250, 0, 3 , 8, 8, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_CAM_IF		, "ckg_cam_if",		 ckg_mux_cam_if_p		, 0x025c, 0, 2 , 8, 4, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_SENSOR0		, "ckg_sensor0",	 ckg_mux_sensor0_p	, 0x0260, 0, 3 , 8, 4, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_SENSOR1		, "ckg_sensor1",	 ckg_mux_sensor1_p	, 0x0264, 0, 3 , 8, 4, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_SENSOR2		, "ckg_sensor2",	 ckg_mux_sensor2_p	, 0x0268, 0, 3 , 8, 4, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_GLB_APB_CFG	, "ckg_glb_apb",	 ckg_mux_glb_apb_cfg_p, 0x0224, 0, 2 , 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_GLB_AON_CFG	, "ckg_glb_aon",	 ckg_mux_glb_aon_cfg_p, 0x0258, 0, 3 , 8, 4, CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CKG_MUX_VOU_BUS		, "ckg_vou_bus",	 ckg_mux_vou_bus_p	, 0x092c, 8, 2 , 20,3, CLK_SET_RATE_PARENT ),
};

static const char *const ckg_mux_pmu_p[] = {"clk_32k", "clk_xtl_1", "clk_xtl_24" "clk_rc_24_1", "clk_pll_100"};
static const char *const ckg_mux_deb_p[] = {"clk_32k", "clk_xtl_1", "clk_xtl_24" "clk_rc_24_1", "clk_pll_100"};
static const char *const ckg_mux_dvpin_dllref_p[] = {"clk_xtl_24", "clk_pll_125"};

static const struct mc_mux_clock  top_mux_clks[] = {
	{CKG_MUX_PMU_CFG			, "ckg_pmu", ckg_mux_pmu_p			, ARRAY_SIZE(ckg_mux_pmu_p			), CLK_SET_RATE_PARENT, 0x026c, 0, 3, 0, 0},
	{CKG_MUX_DEB_CFG			, "ckg_deb", ckg_mux_deb_p			, ARRAY_SIZE(ckg_mux_deb_p			), CLK_SET_RATE_PARENT, 0x0270, 0, 3, 0, 0},
	{CKG_MUX_DVPIN_DLLREF_CFG	, "ckg_dvpin_dllref", ckg_mux_dvpin_dllref_p	, ARRAY_SIZE(ckg_mux_dvpin_dllref_p	), CLK_SET_RATE_PARENT, 0x0274, 0, 1, 0, 0},
};

static const char *const ckg_mux_dmic0_p[] 			= {"clk_xtl_24", "clk_pll_333_5", "clk_pll_400_1", "clk_pll_500_1"};
static const char *const ckg_mux_dmic1_p[] 			= {"clk_xtl_24", "clk_pll_333_5", "clk_pll_400_1", "clk_pll_500_1"};
static const char *const ckg_mux_motorctl0_db_p[]	= {"clk_32k",    "clk_xtl_1",     "clk_xtl_24"};
static const char *const ckg_mux_motorctl1_db_p[]   = {"clk_32k",    "clk_xtl_1",     "clk_xtl_24"};
static const char *const ckg_mux_i2s0_slow_pad_p[]	= {"clk_xtl_24"};
static const char *const ckg_mux_i2s0_slow_p[]		= {"clk_xtl_24", "clk_pll_50",    "clk_pll_100",   "clk_spll_148_5", "clk_pll_200"};
static const char *const ckg_mux_i2s0_fast_p[]		= {"clk_xtl_24", "clk_pll_400_1", "clk_pll_500_1"};
static const char *const ckg_mux_i2s0_source_p[]	= {"clk_xtl_24"};
static const char *const ckg_mux_i2s1_slow_pad_p[]	= {"clk_xtl_24"};
static const char *const ckg_mux_i2s1_slow_p[]		= {"clk_xtl_24", "clk_pll_50",    "clk_pll_100",   "clk_spll_148_5", "clk_pll_200"};
static const char *const ckg_mux_i2s1_fast_p[]		= {"clk_xtl_24", "clk_pll_50",    "clk_pll_400_1", "clk_pll_500_1"};
static const char *const ckg_mux_i2s1_source_p[]	= {"clk_xtl_24"};//quanok

static const struct mc_mux_clock  cpu_mux_clks[] = {
	{CKG_MUX_DMIC0			, "ckg_dmic0",			 ckg_mux_dmic0_p			, ARRAY_SIZE(ckg_mux_dmic0_p		), CLK_SET_RATE_PARENT, 0x018c, 22, 2, 0, 0},
	{CKG_MUX_DMIC1			, "ckg_dmic1",			 ckg_mux_dmic1_p			, ARRAY_SIZE(ckg_mux_dmic1_p		), CLK_SET_RATE_PARENT, 0x0194, 22, 2, 0, 0},
    {CKG_MUX_MOTORCTL0_DB	, "ckg_motorctl0_db",	 ckg_mux_motorctl0_db_p	, ARRAY_SIZE(ckg_mux_motorctl0_db_p	), CLK_SET_RATE_PARENT, 0x00b0, 17, 2, 0, 0},
	{CKG_MUX_MOTORCTL1_DB	, "ckg_motorctl1_db",	 ckg_mux_motorctl1_db_p	, ARRAY_SIZE(ckg_mux_motorctl1_db_p	), CLK_SET_RATE_PARENT, 0x00b0, 20, 2, 0, 0},
    {CKG_MUX_I2S0_SLOW_PAD	, "ckg_i2s0_slow_pad",	 ckg_mux_i2s0_slow_pad_p	, ARRAY_SIZE(ckg_mux_i2s0_slow_pad_p), CLK_SET_RATE_PARENT, 0x00c4, 5 , 1, 0, 0},
    {CKG_MUX_I2S0_SLOW		, "ckg_i2s0_slow",		 ckg_mux_i2s0_slow_p		, ARRAY_SIZE(ckg_mux_i2s0_slow_p	), CLK_SET_RATE_PARENT, 0x00c4, 2 , 3, 0, 0},
    {CKG_MUX_I2S0_FAST		, "ckg_i2s0_fast",		 ckg_mux_i2s0_fast_p		, ARRAY_SIZE(ckg_mux_i2s0_fast_p	), CLK_SET_RATE_PARENT, 0x00c4, 0 , 2, 0, 0},
    {CKG_MUX_I2S0_SOURCE	, "ckg_i2s0_source",	 ckg_mux_i2s0_source_p	, ARRAY_SIZE(ckg_mux_i2s0_source_p	), CLK_SET_RATE_PARENT, 0x00b8, 2 , 1, 0, 0},
    {CKG_MUX_I2S1_SLOW_PAD	, "ckg_i2s1_slow_pad",	 ckg_mux_i2s1_slow_pad_p	, ARRAY_SIZE(ckg_mux_i2s1_slow_pad_p), CLK_SET_RATE_PARENT, 0x00f0, 5 , 1, 0, 0},
    {CKG_MUX_I2S1_SLOW		, "ckg_i2s1_slow",		 ckg_mux_i2s1_slow_p		, ARRAY_SIZE(ckg_mux_i2s1_slow_p	), CLK_SET_RATE_PARENT, 0x00f0, 2 , 3, 0, 0},
    {CKG_MUX_I2S1_FAST		, "ckg_i2s1_fast",		 ckg_mux_i2s1_fast_p		, ARRAY_SIZE(ckg_mux_i2s1_fast_p	), CLK_SET_RATE_PARENT, 0x00f0, 0 , 2, 0, 0},
    {CKG_MUX_I2S1_SOURCE	, "ckg_i2s1_source",	 ckg_mux_i2s1_source_p	, ARRAY_SIZE(ckg_mux_i2s1_source_p	), CLK_SET_RATE_PARENT, 0x00e4, 2 , 1, 0, 0},
};

static const char *const ckg_mux_ca7_core_p[]	= {"clk_xtl_24",  "clk_pll_667_1",  "clk_spll_743_1",  "clk_pll_990"};
static const char *const ckg_mux_rsa_p[]		= {"clk_xtl_24",  "clk_pll_100",    "clk_pll_200",     "clk_pll_333_5", "clk_pll_400_1"};
static const char *const ckg_mux_hash_p[]		= {"clk_xtl_24",  "clk_pll_50",     "clk_pll_333_5"};
static const char *const ckg_mux_src_p[]		= {"clk_xtl_24",  "clk_pll_50",     "clk_pll_100"};
static const char *const ckg_mux_spi0_p[]		= {"clk_xtl_24",  "clk_pll_50",     "clk_pll_100"};
static const char *const ckg_mux_spi1_p[]		= {"clk_xtl_24",  "clk_pll_50",     "clk_pll_100"};
static const char *const ckg_mux_pwm_p[]		= {"clk_xtl_24",  "clk_pll_50",     "clk_pll_100"};
static const char *const ckg_mux_cs_dbg_p[]		= {"clk_xtl_24",  "clk_pll_100",    "clk_pll_200"};
static const char *const ckg_mux_efuse_pad_p[]	= {"clk_xtl_24"};
static const char *const ckg_mux_efuse_p[]		= {"clk_xtl_24", "clk_pll_50"};//quanok

static const struct mc_mux_clock  cpucomposite_mux_clks[] = {
	{CKG_MUX_CA7_CORE	, "ckg_ca7_core",	 ckg_mux_ca7_core_p	, ARRAY_SIZE(ckg_mux_ca7_core_p	), CLK_SET_RATE_PARENT, 0x0220, 0, 2, 0, 0},
	{CKG_MUX_RSA		, "ckg_rsa",		 ckg_mux_rsa_p		, ARRAY_SIZE(ckg_mux_rsa_p		), CLK_SET_RATE_PARENT, 0x0238, 0, 3, 0, 0},
    {CKG_MUX_HASH		, "ckg_hash",		 ckg_mux_hash_p		, ARRAY_SIZE(ckg_mux_hash_p		), CLK_SET_RATE_PARENT, 0x023c, 0, 2, 0, 0},
	{CKG_MUX_SRC		, "ckg_src",		 ckg_mux_src_p		, ARRAY_SIZE(ckg_mux_src_p		), CLK_SET_RATE_PARENT, 0x0240, 0, 2, 0, 0},
    {CKG_MUX_SPI0		, "ckg_spi0",		 ckg_mux_spi0_p		, ARRAY_SIZE(ckg_mux_spi0_p		), CLK_SET_RATE_PARENT, 0x025c, 0, 2, 0, 0},
    {CKG_MUX_SPI1		, "ckg_spi1",		 ckg_mux_spi1_p		, ARRAY_SIZE(ckg_mux_spi1_p		), CLK_SET_RATE_PARENT, 0x0260, 0, 2, 0, 0},
    {CKG_MUX_PWM		, "ckg_pwm",		 ckg_mux_pwm_p		, ARRAY_SIZE(ckg_mux_pwm_p		), CLK_SET_RATE_PARENT, 0x0264, 0, 2, 0, 0},
    {CKG_MUX_CS_DBG		, "ckg_cs_dbg",		 ckg_mux_cs_dbg_p		, ARRAY_SIZE(ckg_mux_cs_dbg_p	), CLK_SET_RATE_PARENT, 0x022c, 0, 2, 0, 0},
    {CKG_MUX_EFUSE_PAD	, "ckg_efuse_pad",	 ckg_mux_efuse_pad_p	, ARRAY_SIZE(ckg_mux_efuse_pad_p), CLK_SET_RATE_PARENT, 0x0268,16, 1, 0, 0},
    {CKG_MUX_EFUSE		, "ckg_efuse",		 ckg_mux_efuse_p		, ARRAY_SIZE(ckg_mux_efuse_p	), CLK_SET_RATE_PARENT, 0x0268, 0, 1, 0, 0},
};

static const char *const ckg_mux_i2c0_p[]			= {"clk_xtl_24", "clk_pll_100", "clk_pll_125", "clk_spll_148_5"};
static const char *const ckg_mux_i2c1_p[]			= {"clk_xtl_24", "clk_pll_100", "clk_pll_125", "clk_spll_148_5"};	
static const char *const ckg_mux_i2c3_p[]			= {"clk_xtl_24", "clk_pll_100", "clk_pll_125", "clk_spll_148_5"};
static const char *const ckg_mux_uart0_p[]			= {"clk_xtl_24", "clk_pll_50",  "clk_pll_100", "clk_pll_125",    "clk_spll_148_5"};
static const char *const ckg_mux_uart1_p[]			= {"clk_xtl_24", "clk_pll_50",  "clk_pll_100", "clk_pll_125",    "clk_spll_148_5"};
static const char *const ckg_mux_gpio_debouce_p[]	= {"clk_32k",    "clk_pll_20",  "clk_xtl_24",  "clk_pll_50"};
static const char *const ckg_mux_motorctl0_stm_p[]	= {"clk_xtl_24", "clk_pll_50",  "clk_pll_100"};
static const char *const ckg_mux_motorctl1_stm_p[]	= {"clk_xtl_24", "clk_pll_50",  "clk_pll_100"};//quanok

static const struct mc_composite  cpucomposite_clks[] = {
	MUX_DIV_FLAGS( CKG_MUX_I2C0			, "ckg_i2c0",			 ckg_mux_i2c0_p			, 0x0244, 0, 2, 8, 3, CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CKG_MUX_I2C1			, "ckg_i2c1",			 ckg_mux_i2c1_p			, 0x0248, 0, 2, 8, 3, CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CKG_MUX_I2C3			, "ckg_i2c3",			 ckg_mux_i2c3_p			, 0x024c, 0, 2, 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_UART0		, "ckg_uart0",			 ckg_mux_uart0_p		, 0x0250, 0, 3, 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_UART1		, "ckg_uart1",			 ckg_mux_uart1_p		, 0x0254, 0, 3, 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_GPIO_DEBOUCE	, "ckg_gpio_debouce",	 ckg_mux_gpio_debouce_p	, 0x0258, 0, 2, 8, 8, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_MOTORCTL0_STM, "ckg_motorctl0_stm",	 ckg_mux_motorctl0_stm_p, 0x026c, 0, 2, 8, 3, CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_MOTORCTL1_STM, "ckg_motorctl1_stm",	 ckg_mux_motorctl1_stm_p, 0x0270, 0, 2, 8, 3, CLK_SET_RATE_PARENT ),
};

static const char *const ckg_mux_ir_p[]		= {"clk_xtl_24", "clk_pll_80"};
static const char *const ckg_mux_isp_bus_p[]= {"clk_xtl_24", "clk_pll_200", "pll_286_1",      "clk_pll_333_6"};
static const char *const ckg_mux_vpu_p[]	= {"clk_xtl_24", "clk_pll_250", "clk_pll_333_5",  "clk_pll_400"};
static const char *const ckg_mux_vicap_p[]	= {"clk_xtl_24", "clk_pll_400", "clk_pll_500_05", "clk_spll_594_1"};//quanok

//?CKG_CFG0	0x2c100004	ckg_ephy_ref_div	[26:25]	2'b0	clk_ephy_refclkdivition
static const struct mc_composite  isp_composite_clks[] = {
	MUX_DIV_FLAGS( CKG_MUX_IR		, "ckg_ir",		 ckg_mux_ir_p		, 0x008, 27, 1 , 28, 3 , CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CKG_MUX_ISP_BUS	, "ckg_isp_bus", ckg_mux_isp_bus_p	, 0x000, 24, 2 , 20, 4 , CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_VPU		, "ckg_vpu",	 ckg_mux_vpu_p		, 0x008, 10, 2 , 12, 4 , CLK_SET_RATE_PARENT ),
    MUX_DIV_FLAGS( CKG_MUX_VICAP	, "ckg_vicap",	 ckg_mux_vicap_p	, 0x000, 17, 2 , 13, 4 , CLK_SET_RATE_PARENT ),
};

static const char *const ckg_mux_csi_cfg_p[] 		= {"clk_xtl_24"	, "clk_pll_50", "clk_pll_100"	, "clk_pll_200"};
static const char *const ckg_mux_usb3_suspend_p[] 	= {"clk_32k"	, "clk_pll_20", "clk_xtl_24"	, "clk_pll_50", "clk_pll_100"};
static const char *const ckg_mux_usb32_ref_p[] 		= {"clk_32k"	, "clk_pll_20", "clk_xtl_24"	, "clk_pll_50", "clk_pll_100"};
//static const char *const ckg_mux_gmac1_rmii_pad_p[] = {"clk_32k", "clk_xtl_1", "clk_xtl_24", "clk_pll_100"}; //?

static const struct mc_mux_clock isp_mux_clks[] = {
	{CKG_MUX_CSI_CFG		, "ckg_csi_cfg",		 ckg_mux_csi_cfg_p		, ARRAY_SIZE(ckg_mux_csi_cfg_p			), CLK_SET_RATE_PARENT, 0x0000, 29, 2, 0, 0},
	{CKG_MUX_USB3_SUSPEND	, "ckg_usb3_suspend",	 ckg_mux_usb3_suspend_p	, ARRAY_SIZE(ckg_mux_usb3_suspend_p		), CLK_SET_RATE_PARENT, 0x0004, 4 , 3, 0, 0},
	{CKG_MUX_USB32_REF		, "ckg_usb32_ref",		 ckg_mux_usb32_ref_p		, ARRAY_SIZE(ckg_mux_usb32_ref_p		), CLK_SET_RATE_PARENT, 0x0004, 8 , 3, 0, 0},
	//{CKG_MUX_GMAC1_RMII_PAD	, "ckg_gmac1_rmii_pad	", ckg_mux_gmac1_rmii_pad_p	, ARRAY_SIZE(ckg_mux_gmac1_rmii_pad_p	), CLK_IGNORE_UNUSED, 0x0004, 23, 2, 0, 0},
};

//?	
//0x1c000010	ckg_veu_sys_div				[3:0]	4'h0	veusysclksrcdivision
//0x1c000194	ckg_emmc_div				[17:11]	7'h1	
//0x1c0001a0	ckg_ephy0_ref_div			[1:0]	2'b0	
//0x1c000028	ckg_gmac0_rmii_pad_sel		[3:2]	2'b10	gmac0rmiiclksourcesel
//0x1c000194	ckg_emmc_base_sel			[19:18]	2'h2	
//0x1c000198	ckg_emmc_base_div			[19:16]	4'b1
static const char *const ckg_mux_venc460_p[]		= {"clk_xtl_24",  "clk_rc_200_1 ", "clk_pll_500_05", "clk_pll_667_1"};
static const char *const ckg_mux_venc460_slow_p[] 	= {"clk_xtl_24",  "clk_rc_200_1",  "clk_pll_400",    "clk_pll_500_05", "clk_spll_594_1"};
static const char *const ckg_mux_lzma_core_p[]		= {"clk_xtl_24",  "clk_rc_200_1",  "clk_pll_400",    "clk_pll_500",    "clk_pll_667_4"};	
static const char *const ckg_mux_emmc_2x_p[]		= {"clk_xtl_24",  "clk_pll_200",   "clk_spll_297",   "clk_pll_400"};
static const char *const ckg_mux_emmc_1x_p[]		= {"clk_xtl_24",  "clk_pll_200"};
static const char *const ckg_mux_emmc_cqetm_p[]		= {"clk_pll_20",  "clk_xtl_24"};
static const char *const ckg_mux_emmc_tm_p[]		= {"clk_pll_20",  "clk_xtl_24"};
static const char *const ckg_mux_sfc_p[]			= {"clk_xtl_24",  "clk_pll_286",   "clk_pll_333_5",  "clk_pll_400",    "clk_pll_500"};	
static const char *const ckg_mux_sadcw_p[]			= {"clk_xtl_24",  "clk_spll_45",   "clk_pll_100"};
static const char *const ckg_mux_sdio0_p[]			= {"clk_xtl_24",  "clk_pll_50"};
static const char *const ckg_mux_sdio1_p[]			= {"clk_xtl_24",  "clk_pll_50"};

static const struct mc_mux_clock veu_mux_clks[] = {
	{CKG_MUX_SDIO0, "ckg_sdio0",	ckg_mux_sdio0_p, ARRAY_SIZE(ckg_mux_sdio0_p), CLK_SET_RATE_PARENT, 0x018c, 6, 1, 0, 0},
	{CKG_MUX_SDIO1, "ckg_sdio1",	ckg_mux_sdio1_p, ARRAY_SIZE(ckg_mux_sdio1_p), CLK_SET_RATE_PARENT, 0x0190, 6, 1, 0, 0},
};

static const struct mc_composite  veu_composite_clks[] = {
		MUX_DIV_FLAGS( CKG_MUX_VENC460		, "ckg_venc460",		 ckg_mux_venc460_p		, 0x0020, 4, 3, 0, 4 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_VENC460_SLOW , "ckg_venc460_slow",	 ckg_mux_venc460_slow_p 	, 0x0020, 14,3, 10,4 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_LZMA_CORE	, "ckg_lzma_core",		 ckg_mux_lzma_core_p		, 0x0014, 12,3, 8, 4 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_EMMC_2X		, "ckg_emmc_2x",		 ckg_mux_emmc_2x_p		, 0x0194, 2, 2, 4, 7 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_EMMC_1X		, "ckg_emmc_1x",		 ckg_mux_emmc_1x_p		, 0x0194, 2, 2, 11, 7 , CLK_SET_RATE_PARENT ),

		MUX_DIV_FLAGS( CKG_MUX_EMMC_CQETM	, "ckg_emmc_cqetm",		 ckg_mux_emmc_cqetm_p		, 0x0198, 0, 2, 2, 5 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_EMMC_TM		, "ckg_emmc_tm",		 ckg_mux_emmc_tm_p		, 0x0198, 7, 2, 9, 5 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_SFC			, "ckg_sfc",			 ckg_mux_sfc_p			, 0x01a4, 4, 3, 7, 4 , CLK_SET_RATE_PARENT ),
		MUX_DIV_FLAGS( CKG_MUX_SADCW		, "ckg_sadcw",			 ckg_mux_sadcw_p			, 0x0188, 4, 2, 8, 6 , CLK_SET_RATE_PARENT ),
};

static const char *const ckg_mux_vgs_p[]			= {"clk_xtl_24", "clk_rc_200_1","clk_pll_250","clk_pll_333_5","clk_pll_400"};
static const struct mc_composite  vou_composite_clks[] = {
	MUX_DIV_FLAGS( CKG_MUX_VGS		, "ckg_vgs", ckg_mux_vgs_p, 0x006c, 4, 3, 0, 4 , CLK_SET_RATE_PARENT ),
};

static const char *const ckg_mux_nnp_p[]	  = {"clk_xtl_24", "clk_rc_200_1", "clk_pll_500", "clk_pll_667"};
static const char *const ckg_mux_ai_bus_p[]	  = {"clk_xtl_24", "clk_rc_200_1", "clk_pll_500", "clk_pll_667"};
static const char *const ckg_mux_vdsp_core_p[]= {"clk_xtl_24", "clk_rc_200_1", "clk_pll_400", "clk_pll_500_05"};
static const char *const ckg_mux_veu_sys_p[]= {"clk_xtl_24", "clk_rc_200_1", "clk_pll_400", "clk_pll_500_05"};

static const struct mc_mux_clock  top_gate_extra_clks[] = {
	{CKG_MUX_NNP_P		, "ckg_nnp",		 ckg_mux_nnp_p	  , ARRAY_SIZE(ckg_mux_nnp_p	),  CLK_SET_RATE_PARENT, 0x092c, 16, 2, 0, 0},
	{CKG_MUX_AI_BUS_P	, "ckg_ai_bus", 	ckg_mux_ai_bus_p	  , ARRAY_SIZE(ckg_mux_ai_bus_p	),  CLK_SET_RATE_PARENT, 0x092c, 18, 2, 0, 0},
	{CKG_MUX_VDSP_CORE_P, "ckg_vdsp_core",	 ckg_mux_vdsp_core_p, ARRAY_SIZE(ckg_mux_vdsp_core_p),CLK_SET_RATE_PARENT, 0x092c, 4 , 2, 0, 0},
	{CKG_MUX_VEU_SYS_P, "ckg_veu_sys",	 ckg_mux_veu_sys_p, ARRAY_SIZE(ckg_mux_veu_sys_p),CLK_SET_RATE_PARENT, 0x092c, 0 , 3, 0, 0},
};

static struct mc_clock_data *clk_plldiv_data;
static void plldiv_init(struct device *dev)
{

	int nr = ARRAY_SIZE(fixed_rate_clks) +
			ARRAY_SIZE(fixed_factor_clks);
	int i;

	clk_plldiv_data = mc_clk_init(dev, nr);
	if (!clk_plldiv_data) {
		dev_err(dev, "%s: clk_plldiv_data is null\n",__func__);
		return;
	}

	for (i = 0; i < nr; i++)
		clk_plldiv_data->clk_data.clks[i] = ERR_PTR(-EPROBE_DEFER);

	mc_clk_register_fixed_rate(fixed_rate_clks,
					 ARRAY_SIZE(fixed_rate_clks),
					 clk_plldiv_data);

	mc_clk_register_fixed_factor(fixed_factor_clks,
				 ARRAY_SIZE(fixed_factor_clks),
				 clk_plldiv_data);
}

static void topsys_gate_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(top_gate_clks)
			+ ARRAY_SIZE(top_gate_extra_clks);;

	if (!clk_plldiv_data) {
		dev_err(dev, "%s:clk_plldiv_data is null",__func__);
		return;
	}

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(top_gate_clks,
					ARRAY_SIZE(top_gate_clks),clk_data);
	mc_clk_register_mux(top_gate_extra_clks,
					ARRAY_SIZE(top_gate_extra_clks), clk_data);
}

static void topsys_composite_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(top_composite_clks) 
			+ ARRAY_SIZE(top_mux_clks);

	if (!clk_plldiv_data) {
		dev_err(dev, "%s:clk_plldiv_data is null",__func__);
		return;
	}

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_composites(top_composite_clks,
					ARRAY_SIZE(top_composite_clks),clk_data);		
	mc_clk_register_mux(top_mux_clks,
					ARRAY_SIZE(top_mux_clks), clk_data);
}

static void cpusys_gate_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(cpu_gate_clks)
			+ ARRAY_SIZE(cpu_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(cpu_gate_clks,
					ARRAY_SIZE(cpu_gate_clks),clk_data);
	mc_clk_register_mux(cpu_mux_clks,
					ARRAY_SIZE(cpu_mux_clks), clk_data);
}

static void cpusys_composite_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(cpucomposite_mux_clks)
			+ ARRAY_SIZE(cpucomposite_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}

	mc_clk_register_mux(cpucomposite_mux_clks,
					ARRAY_SIZE(cpucomposite_mux_clks), clk_data);
	mc_clk_register_composites(cpucomposite_clks,
					ARRAY_SIZE(cpucomposite_clks),clk_data);
}

static void ispsys_composite_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(isp_gate_clks) +
			ARRAY_SIZE(isp_mux_clks) +
            ARRAY_SIZE(isp_composite_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	
	mc_clk_register_gate(isp_gate_clks,
					ARRAY_SIZE(isp_gate_clks),clk_data);

	mc_clk_register_mux(isp_mux_clks,
				 ARRAY_SIZE(isp_mux_clks),clk_data);

    mc_clk_register_composites(isp_composite_clks,
                    ARRAY_SIZE(isp_composite_clks),
                    clk_data);
}

static void nnpsys_composite_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(nnp_gate_clks);
            //ARRAY_SIZE(nnp_composite_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}

	mc_clk_register_gate(nnp_gate_clks,
					ARRAY_SIZE(nnp_gate_clks),clk_data);

    //mc_clk_register_composites(nnp_composite_clks,
    //                ARRAY_SIZE(nnp_composite_clks),
    //                clk_data);
}

static void veusys_composite_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(veu_gate_clks) +
			ARRAY_SIZE(veu_mux_clks) + 
            ARRAY_SIZE(veu_composite_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}

	mc_clk_register_gate(veu_gate_clks,
					ARRAY_SIZE(veu_gate_clks),clk_data);

	mc_clk_register_mux(veu_mux_clks,
				 ARRAY_SIZE(veu_mux_clks),clk_data);

    mc_clk_register_composites(veu_composite_clks,
                    ARRAY_SIZE(veu_composite_clks),
                    clk_data);
}

static void vousys_composite_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(vou_gate_clks) +
            ARRAY_SIZE(vou_composite_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}

	mc_clk_register_gate(vou_gate_clks,
					ARRAY_SIZE(vou_gate_clks),clk_data);

    mc_clk_register_composites(vou_composite_clks,
                    ARRAY_SIZE(vou_composite_clks),
                    clk_data);
}

static const struct of_device_id clk_match_table[] = {

//plldiv-srcclk,default romcode set up depend on efuse
//reg_addr_base:0x2f300000
	{ .compatible = "plldiv-srcclk",
	  .data =  plldiv_init },

//topsys-gate-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x10000000
	{ .compatible = "topsys-gate-clk",
	  .data =  topsys_gate_init },

//topsys-compostite-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x10300000
	{ .compatible = "topsys-compostite-clk",
	  .data =  topsys_composite_init },

//cpusys-gate-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0xc000000
	{ .compatible = "cpusys-gate-clk",
	  .data =  cpusys_gate_init },

//cpusys-composite-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x9200000
	{ .compatible = "cpusys-composite-clk",
	  .data =  cpusys_composite_init },

//ispsys-composite-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x2c100000,except that ckg_dvpin_dllref_en
	{ .compatible = "ispsys-composite-clk",
	  .data =  ispsys_composite_init },

//nnpsys-composite-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x28100000,except that share_ram_clk_ctrll
	{ .compatible = "nnpsys-composite-clk",
	  .data =  nnpsys_composite_init },

//veusys-composite-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x1c000000,except that ckg_veu_sys_sel
	{ .compatible = "veusys-composite-clk",
	  .data =  veusys_composite_init },

//vousys-composite-clk,defaulr driver owner set up depend on ccf
//reg_addr_base:0x20100000,except that share_ram_clk_ctrll
	{ .compatible = "vousys-composite-clk",
	  .data =  vousys_composite_init },

	{ }
};

static int  clk_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	void (*init_func)(struct device *dev);

	init_func = of_device_get_match_data(dev);
	if (!init_func)
		return -ENODEV;

	init_func(dev);
	return 0;
}

static struct platform_driver clk_driver = {
	.probe		 = clk_probe,
	.driver		 = {
	.name  = "xc-clk",
	.of_match_table = clk_match_table,
	},
};

static int __init clk_init(void)
{
	return platform_driver_register(& clk_driver);
}
core_initcall( clk_init);

