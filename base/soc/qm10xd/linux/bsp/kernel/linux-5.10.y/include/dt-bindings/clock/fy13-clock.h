/*
 * Copyright (c) 2016-2017 Linaro Ltd.
 * Copyright (c) 2016-2017 Molchip Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __DTS_FY13_CLOCK_H
#define __DTS_FY13_CLOCK_H

/* fixed rate clocks */
#define CLK_UPLL			0
#define CLK_MPLL			1
#define CLK_MPLL1			2
#define CLK_MPLL2			3
#define CLK_EPLL			4
#define CLK_EPLL1			5
#define CLK_VEUPLL			6
#define	CLK_AIPLL			7

/* clk in crgctrl */
#define CLK_MPLL_120		8
#define CLK_MPLL_250		9
#define CLK_UPLL_320		10
#define CLK_UPLL_128		11
#define CLK_UPLL_91_4		12
#define CLK_UPLL_64			13
#define CLK_EPLL_200		14
#define CLK_EPLL_100		15
#define CLK_EPLL_50			16
#define CLK_EPLL_25			17
#define CLK_EPLL_20			18
#define CLK_MPLL_273		19
#define CLK_MPLL_300		20
#define	CLK_AIPLL_500		21
#define	CLK_UPLL_160		22


/*apapb_gate*/
#define CKG_UART0			23
#define CKG_UART1			24
#define CKG_UART2			25
#define CKG_UART3			26
#define CKG_IR				27
#define CKG_I2C0			28
#define CKG_I2C1			29
#define CKG_I2S0			30
#define CKG_I2S1			31
#define CKG_I2S2			32
#define CKG_I2S3			33
#define CKG_I2S4			34
#define CKG_RTC_TMR0		35
#define CKG_RTC_TMR1		36
#define CKG_RTC_TMR2		37
#define CKG_XTL_TMR0		38
#define CKG_XTL_TMR1		39
#define CKG_XTL_TMR2		40
#define CKG_AP_SYST			41
#define CKG_IOE 			42
#define CKG_WDG0            43
#define CKG_WDG1            44
#define CKG_SPI				45
#define CKG_EFUSE			46
#define CKG_PWM				47
#define CKG_SRC				48
#define CKG_RSA				49
#define CKG_HASH			50
#define CKG_BSM_TMR			51
#define CKG_APB_UART0		52
#define CKG_APB_UART1		53
#define CKG_APB_UART2		54
#define CKG_APB_UART3		55
#define CKG_APB_IR			56
#define CKG_APB_I2C0		57
#define CKG_APB_I2C1		58
#define CKG_APB_I2S0		59
#define CKG_APB_I2S1		60
#define CKG_APB_I2S2		61
#define CKG_APB_I2S3		62
#define CKG_APB_I2S4		63
#define CKG_APB_TMR0        64
#define CKG_APB_TMR1        65
#define CKG_APB_TMR2		66
#define CKG_APB_SYST  		67
#define CKG_APB_RTCT		68
#define CKG_APB_IOE			69
#define CKG_APB_WDG0		70
#define CKG_APB_WDG1		71
#define CKG_APB_GPIO		72
#define CKG_APB_SPI			73
#define CKG_APB_EFUSE0		74
#define CKG_IOE_PUB	        75
#define CKG_IOE_CH0         76
#define CKG_IOE_CH1 		77
#define CKG_APB_PWM  		78
#define CKG_APB_SRC			79
#define CKG_APB_RSA			80
#define CKG_APB_HASH		81
#define CKG_I2S0_FAST_EN 	82
#define CKG_I2S1_FAST_EN 	83
#define CKG_I2S2_FAST_EN 	84
#define CKG_I2S3_FAST_EN 	85
#define CKG_I2S4_FAST_EN 	86
#define	CKG_I2S0_FRAC_DIV_EN 	87
#define	CKG_I2S1_FRAC_DIV_EN 	88
#define	CKG_I2S2_FRAC_DIV_EN 	89
#define	CKG_I2S3_FRAC_DIV_EN 	90
#define	CKG_I2S4_FRAC_DIV_EN	91
#define CKG_APB_DEWARP_EN	92
#define CKG_DEWARP_EN		93
#define CKG_DEWARP_MAPC_EN  94
#define CKG_APB_KCF_EN		95
#define CKG_KCF_EN			96
#define CKG_APB_UART4_EN 	97
#define	CKG_UART4_EN		98
#define CKG_APB_I2S5T_EN		99
#define CKG_I2S5_FRAC_DIV_EN    100
#define CKG_I2S5_SLOW_EN		101
#define CKG_I2S5_FAST_EN		102
#define CKG_APB_I2S6_EN         103
#define CKG_I2S6_FRAC_DIV_EN	104
#define CKG_I2S6_SLOW_EN		105
#define CKG_I2S6_FAST_EN		106
#define CKG_APB_I2C2_EN			107
#define	CKG_I2C2_EN				108
#define	CKG_VIRTUALVO_TMR_EN	109
#define CKG_APB_I2C3_EN			110
#define	CKG_I2C3_EN				111


/*apahb_gate*/
#define CKG_DMA0				0
#define CKG_DMA1				1
#define CKG_BGM					2
#define CKG_VPPU0				3
#define CKG_VPPU1				4
#define CKG_AHB_AES0			5
#define CKG_VPPU2_EN			6
#define CKG_HSLOCK				7
#define CKG_AHB_BGM				8
#define CKG_AHB_VPPU0			9
#define CKG_AHB_VPPU1			10
#define CKG_AHB_VPPU2			11
#define CKG_AP_APB_ARCH			12


/*ap_sys clk*/
#define CLK_MUX_AP_APB	    0
#define CLK_MUX_SPI 		1
#define CLK_MUX_EFUSE		2
#define CLK_MUX_SYST        3
#define CLK_MUX_XTL_TMR0	4
#define CLK_MUX_RTC_TMR0	5
#define CLK_MUX_XTL_TMR1	6
#define CLK_MUX_RTC_TMR1	7
#define CLK_MUX_XTL_TMR2	8
#define CLK_MUX_RTC_TMR2	9
#define CLK_MUX_UART0		10
#define CLK_MUX_UART1		11
#define CLK_MUX_UART2		12
#define CLK_MUX_UART3		13
#define CLK_MUX_IR			14
#define CLK_MUX_I2C0		15
#define CLK_MUX_I2C1		16
#define CLK_MUX_PWM			17
#define CLK_MUX_HASH		18
#define CLK_MUX_SRC			19
#define	CLK_MUX_RSA			20
#define CLK_MUX_AP_SYS		21
#define CLK_MUX_RTC_24M		22
#define CLK_DEWARP			23
#define CLK_KCF				24
#define CLK_IOE				25
#define CLK_BGM				26
#define CLK_IVE				27
#define CLK_MUX_UART4		28
#define CLK_MUX_I2C2		29
#define CLK_MUX_I2C3		30
#define CLK_DEWARP_MAPC		31

/*topclk*/
#define CKG_GLB_VEU_AP      0
#define CLK_MUX_PTS			1
#define CLK_MUX_EPHY		2
#define CLK_MUX_GLB_APB		3
#define CLK_MUX_VEU			4
#define CLK_MUX_VOU			5
#define CLK_MUX_VDU0		6
#define CLK_GLB_VDU1_CFG 	7
#define CLK_GLB_AI_CFG		8
#define CLK_GLB_DMC_CFG		9

/*hsif_sys_ahb_clk*/
#define CLK_EMMC_EN 				0
#define CLK_EMMC_BASE_EN			1
#define CLK_EMMC_CQETM_EN			2
#define CLK_EMMC_TM_EN				3
#define CLK_AXI_EMMC_EN				4
#define CLK_EMMC_PHYCFG_EN			5
#define CLK_EMMC_2X_EN				6
#define CLK_AXI_USB20_EN			7
#define CLK_AXI_USB21_EN			8
#define CLK_USB20_REF_EN			9
#define CLK_USB21_REF_EN			10
#define CLK_AHB_PCIE0_CORE_EN		11
#define CLK_AHB_PCIE1_CORE_EN		12
#define CLK_AHB_PCIE_X4_MISC_EN		13
#define CLK_PCIE2_AUX_REF_EN		14
#define CLK_AXI_PCIE2_EN			15
#define CLK_AHB_PCIE2_CORE_EN		16
#define	CLK_AHB_PCIEX2_MISC_EN		17
#define CLK_PCIE1_AUX_REF_EN		18
#define	CLK_AXI_PCIE1_EN			19
#define	CLK_AHB_PCIE1_X1_CORE_EN	20
#define	CLK_AHB_PCIEX1_MISC_EN		21
#define	CLK_APB_PCIEX2_PHYCFG_EN    22
#define CLK_SATA0_PMALIVE_EN		23
#define CLK_SATA0_RXOOB_EN			24
#define CLK_AXI_SATA0_EN			25
#define CLK_SATA1_PMALIVE_EN		26
#define CLK_SATA1_RXOOB_EN			27
#define CLK_AXI_SATA1_EN			28
#define CLK_SATA2_PMALIVE_EN		29
#define CLK_SATA2_RXOOB_EN			30
#define CLK_AXI_SATA2_EN			31
#define	CLK_APB_CMPHY0_EN			32
#define CLK_APB_CMPHY1_EN			33
#define CLK_APB_CMPHY2_EN			34
#define	CKG_APB_SFC					35
#define	CKG_SFC						36
#define	CLK_AXI_RAIDE_EN			37
#define	CLK_AFIFOW_EN				38

#define CKG_EMMC_SEL 				39
#define CKG_EMMC_BASE_SEL			40
#define  CKG_EMMC_CQETM_SEL 		41
#define  CKG_EMMC_TM_SEL 			42
#define  CKG_USB20_REF_SEL     		43
#define  CKG_USB21_REF_SEL      	44
#define  CKG_PCIE0_AUX_REF_SEL     	45
#define  CKG_PCIE1_AUX_REF_SEL     	46
#define  CKG_PCIE2_AUX_REF_SEL     	47
#define	CKG_PCIE2_GEN2_X1_AUX_REF_SEL	48
#define  CKG_SATA0_PMALIAVE_SEL     49
#define  CKG_SATA0_RXOOB_SEL        50
#define  CKG_SATA1_PMALIAVE_SEL     51
#define  CKG_SATA1_RXOOB_SEL        52
#define  CKG_SATA2_PMALIAVE_SEL    	53
#define  CKG_SATA2_RXOOB_SEL       	54
#define  CKG_SATA3_PMALIAVE_SEL     55
#define  CKG_SATA3_RXOOB_SEL        56
#define  CKG_SATA4_PMALIAVE_SEL     57
#define  CKG_SATA4_RXOOB_SEL        58
#define  CLK_MUX_SFC				59




/*vdu0_sys_clk*/
#define CKG_VDU0_VPPU860_EN		0
#define CKG_VDU0_VDEC660_EN		1
#define CKG_VDU0_APB_PERFMON_EN	2
#define CKG_VDU0_GLB_VDU0_EN	3
#define CKG_VDU0_VPU_EN			4
#define CKG_VDU0_G2D860_EN		5
#define CKG_VDU0_JPEG_EN		6
#define CKG_VDU0_VDEC660_SEL	7
#define CKG_VDU0_VPPU860_SEL	8
#define CKG_VDU0_VPU_SEL		9
#define CKG_VDU0_G2D860_SEL		10
#define CKG_VDU0_JPG_SEL		11

/*vdu1_sys_clk*/
#define CKG_VDU1_VPPU860_EN		0
#define CKG_VDU1_VDEC660_EN		1
#define CKG_VDU1_APB_PERFMON_EN	2
#define CKG_VDU1_GLB_VDU1_EN	3
#define CKG_VDU1_VPU_EN			4
#define CKG_VDU1_G2D860_EN		5
#define CKG_VDU1_JPEG_EN		6
#define CKG_VDU1_VDEC660_SEL	7
#define CKG_VDU1_VPPU860_SEL	8
#define CKG_VDU1_VPU_SEL		9
#define CKG_VDU1_G2D860_SEL		10
#define CKG_VDU1_JPG_SEL		11



/*vdu2_sys_clk*/
#define CKG_VDU2_VPPU860_2_EN	0
#define CKG_VDU2_VDEC660_2_EN  	1
#define CKG_VDU2_VDEC660_3_EN   2
#define CKG_VDU2_VPPU860_3_EN	3
#define CKG_VDU2_APB_PERFMON_EN 4
#define CKG_VDU2_ROT_EN			5
#define CKG_VDU2_BGMCV_EN		6
#define CKG_VDU2_BUS_EN			7
#define CKG_VDU2_VPPU860_2_SEL	8
#define CKG_VDU2_VPPU860_3_SEL	9
#define CKG_VDU2_VDEC660_2_SEL	10
#define CKG_VDU2_VDEC660_3_SEL	11
#define CKG_VDU2_BGMCV_SEL		12
#define CKG_VDU2_ROT_SEL		13
#define CKG_VDU2_BUS_SEL		14

/*vdu3_sys_clk*/
#define CKG_VDU3_VPPU860_EN		0
#define CKG_VDU3_VDEC660_0_EN  	1
#define CKG_VDU3_VDEC660_1_EN   2
#define CKG_VDU3_G2D860_EN      3
#define CKG_VDU3_SYS_SEL		4



/*vpu_sys_apb_clk*/
#define CKG_VPU 				0
#define CKG_AHB_VPU				1
#define CKG_AXI_VDMA			2
#define CKG_APB_VDMA			3
#define CKG_VPPU				4
#define CKG_BM_TIMER			5
#define CKG_APB_BM_VDMA			6
#define CKG_AXI_BM_VDMA			7
#define CKG_APB_BM_VPPU			8
#define CKG_AXI_BM_VPPU			9
#define CKG_APB_BM_VPU			10
#define CKG_AXI_BM_VPU			11
#define CLK_MUX_VPPU_SEL		12


/*vio_sys_apb_clk*/
#define CKG_CVBS_PIXEL 			0
#define CKG_CVBS_VDAC			1
#define VOU_CKG_BUSMON_TIMER	2
#define CKG_AXI_USB3_EN			3
#define CKG_AXI_VOU_EN			4
#define CKG_USB_SUSPEND_EN		5
#define CKG_USB3_REF_EN			6
#define CKG_APB_HDMI0_EN		7
#define CKG_APB_HDMI1_EN		8
#define CKG_VOU_MTX_EN			9
#define CKG_VOU_EN					10
#define CLK_AXI_TRACE_EN			11
#define CLK_USB3_EN  				12
#define CLK_USB3_PIPE3_TRACE_EN     13
#define CKG_HDMI0_I2S_EN			14
#define CKG_HDMI1_I2S_EN			15
#define TIMER2_EN					16
#define	TIMER1_EN  					17
#define CKG_APB_HDMI0_PHY_EN		18
#define	CKG_APB_HDMI1_PHY_EN		19
#define CKG_APB_USB3_CMPHY_EN       20
#define	CKG_HDMI0_TCLK_TRACE_EN		21
#define CKG_HDMI1_TCLK_TRACE_EN		22
#define CKG_HDMI0_PIXEL_VOU_DIV_EN  23
#define CKG_VGA0_VDAC_RAMP_EN		24
#define CKG_HDMI0_TCLK_DIV_EN     	 25
#define CKG_IIPI_HDMI0_PIXEL_DIV_EN  26
#define CKG_IVIDP_HDMI0_PIXEL_DIV_EN 27
#define CKG_HDMI0_TCLK_EN			 28
#define CKG_HDMI1_PIXEL_VOU_DIV_EN	 29
#define CKG_HDMI1_TCLK_DIV_EN		 30
#define CKG_IIPI_HDMI1_PIXEL_DIV_EN  31
#define CKG_IVIDP_HDMI1_PIXEL_DIV_EN 32
#define CKG_HDMI1_TCLK_EN			 33
#define CKG_CVBS_VDAC_VDAC_RAMP_EN   	34
#define	CKG_DBG_RG_CVBS_VDAC_RG_RBIAS_EN  35
#define	CKG_DBG_DA_CVBS_CABLE_S_EN		36
#define	CKG_DBG_DA_CVBS_CABLE_D_EN		37
#define	CKG_DBG_RG_CVBS_VDAC_OVP_EN		38
#define CKG_VDAC_RAMP_GEN_OUT_EN	    39	
#define	CKG_DBG_RG_VGA_VDAC_BG_RBIAS_EN	40
#define	CKG_VGA_VDAC_RAMP_GEN_OUT_EN	41
#define	CKG_VPLL_108M_VOU_FORCE_EN		42
#define	CKG_RW_USB3_PME_EN				43
#define CKG_HDMI0_IAUD_DIV_EN		44
#define CKG_HDMI0_IAUD_EN			45
#define CKG_HDMI0_IREF_EN			46
#define CKG_HDMI1_IAUD_DIV_EN		47
#define CKG_HDMI1_IAUD_EN			48
#define CKG_HDMI1_IREF_EN			49
#define CKG_USB3_1_SUSPEND_EN		50
#define	CKG_USB3_1_REF_EN           51
#define CKG_AXI_VIU0_EN			    52
#define CKG_AXI_VIU1_EN				53
#define CKG_AXI_VIU2_EN				54
#define CKG_AXI_VIU3_EN				55
#define CKG_VIU0_DLL_REF_EN			56
#define CKG_VIU1_DLL_REF_EN			57
#define CKG_VIU2_DLL_REF_EN			58
#define CKG_VIU3_DLL_REF_EN			59
#define CKG_VI_ADC_REF0_EN			60
#define CKG_VI_ADC_REF1_EN			61
#define CKG_VI_ADC_REF2_EN			62
#define CKG_VI_ADC_REF3_EN			63
#define CKG_DLLREF_GMAC2_EN			64
#define CKG_DLLREF_GMAC3_EN			65
#define CKG_GMAC2_TX_EN				66
#define CKG_GMAC3_TX_EN				67
#define CKG_GMAC2_RX_EN				68
#define CKG_GMAC3_RX_EN				69
#define CKG_GMAC2_RMII_EN			70
#define CKG_GMAC3_RMII_EN			71
#define CKG_GMAC2_CSR_EN			72
#define CKG_GMAC3_CSR_EN			73
#define CKG_AXI_GMAC2_EN			74
#define CKG_AXI_GMAC3_EN			75
#define CKG_APB_USB3_1_CMPHY_EN		76
#define CKG_APB_VIU0_EN				77
#define CKG_APB_VIU1_EN				78
#define CKG_APB_VIU2_EN				79
#define CKG_APB_VIU3_EN				80
#define CKG_APB_PERFMON_VOU_MTX_EN	81	
#define CKG_APB_PERFMON_USB3_MTX_EN	82	
#define CKG_USB3_1_EN				83
#define CKG_USB3_1_PIPE3_TRACE_EN	84	
#define CKG_VICASC_DLL_REF_EN		85
#define CKG_AXI_GMAC_EN				86
#define CKG_VIU0_EN					87
#define CKG_VIU1_EN					88
#define CKG_VIU2_EN					89
#define CKG_VIU3_EN					90
#define CKG_RGMII2_DATA_SWAP_EN		91
#define CKG_RGMII3_DATA_SWAP_EN		92
#define CKG_GMAC2_RMII_OUT_INV_EN	93	
#define CKG_GMAC2_MDCK_OUT_INV_EN	94	
#define CKG_GMAC2_TX_IN_INV_EN		95
#define CKG_GMAC3_RMII_OUT_INV_EN	96	
#define CKG_GMAC3_MDCK_OUT_INV_EN	97	
#define CKG_GMAC3_TX_IN_INV_EN		98
#define CKG_VIU0_LITE_EN			99
#define CKG_VIU1_LITE_EN			100
#define	CKG_VIU2_LITE_EN			101
#define CKG_VIU3_LITE_EN			102
#define CKG_AXI_VIU0_FORCE_EN		103
#define CKG_AXI_VIU1_FORCE_EN		104
#define CKG_AXI_VIU2_FORCE_EN		105
#define CKG_AXI_VIU3_FORCE_EN		106

#define CKG_AXI_USB3_SEL			107
#define CKG_AXI_VOU_SEL				108
#define CKG_AXI_USB3_SUSPEND_SEL	109
#define CKG_AXI_USB3_REF_SEL		110
#define CKG_HDMI0_IAUD_SEL			111
#define CKG_HDMI0_IREF_SEL			112
#define CKG_HDMI1_IAUD_SEL			113
#define CKG_HDMI1_IREF_SEL			114
#define CKG_USB3_1_SUSPEND_SEL		115
#define CKG_USB3_1_REF_SEL			116
#define CKG_AXI_VIU0_SEL			117
#define CKG_AXI_VIU1_SEL			118
#define CKG_AXI_VIU2_SEL			119
#define CKG_AXI_VIU3_SEL			120
#define CKG_VI_ADC_REF0_SEL			121
#define CKG_VI_ADC_REF1_SEL			122
#define CKG_VI_ADC_REF2_SEL			123
#define CKG_VI_ADC_REF3_SEL			124
#define CKG_AXI_GMAC_SEL2	  		125




/*veu_sys_ahb_clk*/
#define CKG_AHB_VEU 			0
#define CKG_APB_PERFMON_VEU		1
#define CKG_AXI_PERFMON_VEU		2
#define CKG_BUSMON_TIMER		3
#define CKG_VEU_EN				4
#define CKG_TIMER1				5
#define CKG_DLLREF_GMAC0_EN		6
#define CKG_DLLREF_GMAC1_EN		7
#define CKG_AXI_GMAC1_EN		8
#define CKG_GMAC0_CSR_EN		9
#define CKG_GMAC0_RMII_EN		10
#define CKG_GMAC0_RX_EN			11
#define CKG_GMAC0_TX_EN			12
#define CKG_AXI_GMAC0_EN		13
#define CKG_GMAC1_CSR_EN		14
#define CKG_GMAC1_RMII_EN		15
#define CKG_GMAC1_RX_EN			16
#define CKG_GMAC1_TX_EN			17
#define CKG_VEU1_ARCH_EN		18
#define CKG_VEU0_ARCH_EN		19
#define CKG_VEU_SYS_MTX_EN		20
#define CKG_VEU1_EN				21
#define CKG_VEU0_EN				22
#define CKG_APB_PERFMON_GMAC1_MTX_EN 23
#define CKG_APB_PERFMON_VEU_MTX_EN   24
#define CKG_AHB_VEU1			25
#define CKG_AHB_VEU0			26
#define CKG_VEU_AXI_GMAC_EN		27

#define CKG_VEU_SYS_SEL  		28
#define CKG_AHB_VEU_SYS_SEL		29
#define CKG_VEU1_SEL			30
#define CKG_VEU0_SEL			31
#define CKG_AXI_GMAC_SEL  		32




/*ai_sys_apb_clk*/
#define CKG_AIPLL1000M_WAIT_FORCE_EN	0
#define CKG_DIV1000M_FORCE_EN			1
#define CKG_AIPLL1000M_AI_FORCE_EN		2
#define CKG_AIPLL500M_AI_FORCE_EN		3
#define CKG_HKDLA_EN					4
#define CKG_CNN2X_EN					5
#define CKG_AXI_HKDLA_EN				6
#define CKG_AXI_CNN_EN					7
#define CKG_NNP_EN						8
#define CKG_APB_NNP_EN					9
#define CKG_APB_HKDLA_EN				10
#define CKG_APB_CNN_EN					11

#define CKG_HKDLA_SEL					12
#define CKG_CNN2X_SEL					13
#define CKG_AXI_HKDLA_SEL				14
#define CKG_AXI_CNN_SEL					15
#define CKG_NNP_SEL						16



/*top sys*/
#define CKG_EPHY_REF_EN			0
#define CKG_GLB_APB_HSIF		1
#define CKG_GLB_APB_VOU			2
#define CKG_GLB_APB_VEU			3
#define CKG_GLB_APB_AP			4
#define CKG_GLB_APB_DMC			5
#define CKG_GLB_APB_CPU			6
#define CKG_GLB_APB_VDU0		7
#define CKG_GLB_APB_VDU1		8
#define CKG_GLB_APB_VDU2		9
#define CKG_APB_JTAG			10
#define CKG_APB_ANLG			11
#define CKG_APB_PIN				12
#define CKG_APB_INT_CTL			13
#define RG_BIAS_EN				14
#define RG_BG_EN				15


#endif	/* __DTS_CLOCK_H */
