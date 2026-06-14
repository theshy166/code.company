/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Molchip PCIe controller driver
 *
 * Copyright (C) 2023 Molchip Electronics Co., Ltd.
 *		https://www.molchip.com
 *
 * Author: Jerry Cao <jerry.cao@molchip.com>
 */

#ifndef _PCIE_MOLCHIP_H
#define _PCIE_MOLCHIP_H

#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include "pcie-designware.h"
#include "../../pci.h"

#define SHADOW_OFFSET			(1 << 19)

#define PCIE_DMA_OFFSET			(4 << 19)

#define DMA_CTRL_OFF			0x8
#define NUM_DMA_RD_CHAN_SHIFT		16
#define NUM_DMA_RD_CHAN_MASK		0xF
#define NUM_DMA_WR_CHAN_SHIFT		0
#define NUM_DMA_WR_CHAN_MASK		0xF

#define DMA_WRITE_ENGINE_EN_OFF		0x0c
#define DMA_WRITE_ENGINE		BIT(0)

#define DMA_WRITE_DOORBELL_OFF		0x10

#define DMA_WRITE_CHANNEL_ARB_WEIGHT_LOW_OFF	0x18

#define DMA_READ_ENGINE_EN_OFF		0x2c
#define DMA_READ_ENGINE			BIT(0)

#define DMA_READ_DOORBELL_OFF		0x30

#define DMA_READ_CHANNEL_ARB_WEIGHT_LOW_OFF	0x38

#define DMA_WRITE_INT_STATUS_OFF	0x4c
#define WR_ABORT_INT_STATUS_SHIFT	16
#define WR_DONE_INT_MASK_SHIFT		0

#define DMA_WRITE_INT_MASK_OFF		0x54
#define WR_ABORT_INT_MASK_SHIFT		16
#define WR_DONE_INT_MASK_SHIFT		0

#define DMA_WRITE_INT_CLEAR_OFF		0x58
#define WR_ABORT_INT_CLEAR_SHIFT	16
#define WR_DONE_INT_CLEAR_SHIFT		0

#define DMA_WRITE_ERR_STATUS_OFF	0x5c
#define LINKLIST_ELEMENT_FETCH_ERR_DETECT_SHIFT	16
#define APP_READ_ERROR_DETECT_SHIFT	0

#define DMA_WRITE_DONE_IMWR_LOW_OFF	0x60
#define DMA_WRITE_DONE_IMWR_HIGH_OFF	0x64
#define DMA_WRITE_ABORT_IMWR_LOW_OFF	0x68
#define DMA_WRITE_ABORT_IMWR_HIGH_OFF	0x6c
#define DMA_WRITE_CH01_IMWR_DATA_OFF	0x70
#define DMA_WRITE_CH23_IMWR_DATA_OFF	0x74
#define DMA_WRITE_CH45_IMWR_DATA_OFF	0x78
#define DMA_WRITE_CH67_IMWR_DATA_OFF	0x7c

#define DMA_WRITE_LINKED_LIST_ERR_EN_OFF	0x90
#define WR_CHANNEL_LLLAIE_SHIFT		16
#define WR_CHANNEL_LLRAIE_SHIFT		0

#define DMA_READ_INT_STATUS_OFF		0xa0
#define RD_ABORT_INT_STATUS_SHIFT	16
#define RD_DONE_INT_STATUS_SHIFT	0

#define DMA_READ_INT_MASK_OFF		0xa8
#define RD_ABORT_INT_MASK_SHIFT		16
#define RD_DONE_INT_MASK_SHIFT		0

#define DMA_READ_INT_CLEAR_OFF		0xac
#define RD_ABORT_INT_CLEAR_SHIFT	16
#define RD_DONE_INT_CLEAR_SHIFT		0

#define DMA_READ_ERR_STATUS_LOW_OFF	0xb4
#define LINK_LIST_ELEMENT_FETCH_ERR_DETECT_SHIFT	16
#define APP_WR_ERR_DETECT_SHIFT		0

#define DMA_READ_ERR_STATUS_HIGH_OFF	0xb8
#define DATA_POISIONING_SHIFT		24
#define CPL_TIMEOUT_SHIFT		16
#define CPL_ABORT_SHIFT			8
#define UNSUPPORTED_REQ_SHIFT		0

#define DMA_READ_LINKED_LIST_ERR_EN_OFF	0xc4
#define RD_CHANNEL_LLLAIE_SHIFT		16
#define RD_CHANNEL_LLRAIE_SHIFT		0

#define DMA_READ_DONE_IMWR_LOW_OFF	0xcc
#define DMA_READ_DONE_IMWR_HIGH_OFF	0xd0
#define DMA_READ_ABORT_IMWR_LOW_OFF	0xd4
#define DMA_READ_ABORT_IMWR_HIGH_OFF	0xd8
#define DMA_READ_CH01_IMWR_DATA_OFF	0xdc
#define DMA_READ_CH23_IMWR_DATA_OFF	0xe0
#define DMA_READ_CH45_IMWR_DATA_OFF	0xe4
#define DMA_READ_CH67_IMWR_DATA_OFF	0xe8

#define DMA_CH_CONTROL1_OFF_WRCH	0x200
#define LLE				BIT(9)
#define CCS				BIT(8)

#define CS_SHIFT			5
#define CS_RUN				0x1
#define CS_HALT				0x2
#define CS_STOP				0x3

#define RIE				BIT(4)
#define LIE				BIT(3)
#define LLP				BIT(2)
#define TCB				BIT(1)
#define CB				BIT(0)

#define DMA_CH_CONTROL2_OFF_WRCH	0x204
#define DMA_TRANSFER_SIZE_OFF_WRCH	0x208
#define DMA_SAR_LOW_OFF_WRCH		0x20c
#define DMA_SAR_HIGH_OFF_WRCH		0x210
#define DMA_DAR_LOW_OFF_WRCH		0x214
#define DMA_DAR_HIGH_OFF_WRCH		0x218
#define DMA_LLP_LOW_OFF_WRCH		0x21c
#define DMA_LLP_HIGH_OFF_WRCH		0x220

#define DMA_CH_CONTROL1_OFF_RDCH	0x300
#define DMA_CH_CONTROL2_OFF_RDCH	0x304
#define DMA_TRANSFER_SIZE_OFF_RDCH	0x308
#define DMA_SAR_LOW_OFF_RDCH		0x30c
#define DMA_SAR_HIGH_OFF_RDCH		0x310
#define DMA_DAR_LOW_OFF_RDCH		0x314
#define DMA_DAR_HIGH_OFF_RDCH		0x318
#define DMA_LLP_LOW_OFF_RDCH		0x31c
#define DMA_LLP_HIGH_OFF_RDCH		0x320

#define LL_DMA_DATA_ELEMENT_DWORD_LENGTH	0x6
#define LL_DMA_LINK_ELEMENT_DWORD_LENGTH	0x4

#define GIC_ITS_ADDR			(0x03000040 + 0x1000000)

/*PF_PCIE_CAP*/
#define DEVICE_CONTROL_DEVICE_STATUS		0x8
#define PCIE_CAP_MAX_READ_REQ_SIZE_SHIFT	12
#define PCIE_CAP_MAX_READ_REQ_SIZE_MASK		0x7
#define PCIE_CAP_MAX_PAYLOAD_SIZE_CS_SHIFT	5
#define PCIE_CAP_MAX_PAYLOAD_SIZE_CS_MASK	0x7
#define MAX_128_BYTE_SIZE			0x0
#define MAX_256_BYTE_SIZE			0x1
#define MAX_512_BYTE_SIZE			0x2

#define LINK_CONTROL_LINK_STATUS_REG		0x10
#define PCIE_CAP_RCB_SHIFT			3
#define PCIE_CAP_RCB_MASK			1
#define RCB_64B					0
#define RCB_128B				1

#define LINK_CONTROL2_LINK_STATUS2_REG		0x30
#define COMPLIANCE_PRESET_SHIFT			12
#define COMPLIANCE_PRESET_MASK			0xF
#define ENTER_COMPLIANCE			BIT(4)
#define TARGET_LINK_SPEED_SHIFT			0
#define TARGET_LINK_SPEED_MASK			0xF

/* Synopsys-specific PCIe configuration registers */
#define PCIE_PORT_LINK_CONTROL		0x710
#define LOOPBACK_ENABLE			BIT(2)

#define TRGT_MAP_CTRL_OFF		0x81c
#define BAR0_TRGT_MAP_CTRL		BIT(0)
#define BAR1_TRGT_MAP_CTRL		BIT(1)
#define BAR2_TRGT_MAP_CTRL		BIT(2)
#define BAR3_TRGT_MAP_CTRL		BIT(3)
#define BAR4_TRGT_MAP_CTRL		BIT(4)
#define BAR5_TRGT_MAP_CTRL		BIT(5)

#define GEN3_RELATED_OFF		0x890
#define GEN3_EQUALIZATION_DISABLE_BIT	BIT(16)

#define PIPE_LOOPBACK_CONTROL_OFF	0x8b8
#define PIPE_LOOPBACK_BIT		BIT(31)

#define MISC_CONTROL_1_OFF		0x8BC
#define DBI_RO_WR_EN			BIT(0)

#define MSIX_ADDRESS_MATCH_LOW_OFF	0x940
#define MSIX_ADDRESS_MATCH_HIGH_OFF	0x944

/*HSIF SYS AHB REG*/
#define PCIE_X4_SYS_CLK_RST_CTRL	0x08
#define PCIE2_MISC_AHB_SOFT_RST		BIT(20)
#define PCIE_GEN2_X1_MISC_AHB_SOFT_RST	BIT(19)
#define PCIE2_CORE_AHB_SOFT_RST		BIT(18)
#define PCIE_GEN2_X1_CORE_AHB_SOFT_RST	BIT(17)
#define PCIE2_AXI_SOFT_RST		BIT(15)
#define PCIE_GEN2_X1_AXI_SOFT_RST	BIT(14)

#define PCIE_X2_SYS_CLK_RST_CTRL	0x0C
#define CKG_AHB_PCIE_GEN2_X1_MISC_EN	BIT(14)
#define CKG_AHB_PCIE_GEN2_X1_CORE_EN	BIT(13)
#define CKG_AXI_PCIE_GEN2_X1_EN		BIT(12)
#define CKG_PCIE_GEN2_X1_AUX_REF_EN	BIT(11)
#define CKG_AHB_PCIE_X2_MISC_EN		BIT(6)
#define CKG_AHB_PCIE2_CORE_EN		BIT(5)
#define CKG_AXI_PCIE2_EN		BIT(4)
#define CKG_PCIE2_AUX_REF_EN		BIT(3)

#define PCIE_PHY_CLK_RST_CTRL		0x10
#define CKG_APB_PCIEX2_PHYCFG_EN	BIT(2)
#define PHY_X2_SOFT_RST			BIT(1)

#define COMPHY_CLK_RST			0x44
#define CKG_APB_CMPHY2_CFG_EN		BIT(7)
#define CMPHY2_CFG_SOFT_RST		BIT(2)

#define COMBPHY2_INFO1			0x438
#define COMPHY2_STATUS_INFO_TESTTO	BIT(27)

#define COMBPHY2_DBGSEL			0x43C
#define COMPHY2_TEST_SEL_SHIFT		24
#define COMPHY2_TEST_SEL_MASK		0xF
#define TEST_SEL_PLLLOCKED		0x2

#define ECO_REG1			0x804
#define PORT_0_RC_AVALIABLE		BIT(0)	//0--not avaliable  1--avaliable
#define PORT_1_RC_AVALIABLE		BIT(1)	//0--not avaliable  1--avaliable
#define PORT_0_EP_ROM_USED		BIT(2)	//0--not used as ep  1--used
#define PORT_1_EP_ROM_USED		BIT(3)	//0--not used as ep  1--used
#define CONTROLLER_0_DISABLED		BIT(4)	//0---enabled	1--disabled
#define CONTROLLER_1_DISABLED		BIT(5)	//0---enabled	1--disabled

#define PCIE_CTRL_SOFT			0x90C
#define X2_DEBUG_SEL_POR_N		BIT(14)
#define X2_DEBUG_SEL_PIPE_RSTN_I	BIT(13)
#define X2_PIPE_SOFT_RST		BIT(1)
#define X2_POR_N_SOFT_RST		BIT(0)

#define PCIE_GEN2_X1_COMPHY_CTRL	0x914
#define COMPHY2_FUNC_SEL		BIT(0)

/*AHB Core Registers*/
#define INT_MASK0			0x0
#define INT_STATUS0			0xC

#define INT_MASK1			0x4
#define INT_STATUS1			0x10

#define INT_MASK2			0x8
#define INT_STATUS2			0x14

#define INT_PCIE_LINK_DOWN_STATUS	0x34
#define INT_LINK_DOWN_STATUS		BIT(0)
#define INT_LINK_UP_STATUS		BIT(1)

#define SII_LINK_RST_STATUS		0x80
#define SMLH_LINK_UP			BIT(2)

#define SII_GENERAL_CORE_CTRL		0x84
#define DEVICE_TYPE_SHIFT		3
#define DEVICE_TYPE_MASK		0xF
#define PCIE_EP				0x0
#define ROOT_PORT_OF_PCIE_RC		0x4
#define APP_LTSSM_ENABLE		BIT(7)

#define MSI_INTERFACE_0			0x120
#define MSI_INTERFACE_2			0x128

#define SII_INT0			0x180

#define SII_DEBUG_INTERFACE0		0x340
#define RDLH_LINK_UP			BIT(22)

#define SII_POWER_MAN1			0x404
#define SMLH_LTSSM_STATE_MASK		(0x3F)
#define SMLH_LTSSM_STATE_SHIFT		(3)
#define PM_CURNT_STATE_MASK		(0x7)

#define PIPE_CFG			0x608
#define PIPE_LANE_3_TIE_OFF		BIT(22)
#define PIPE_LANE_2_TIE_OFF		BIT(21)

#define PIPE_INFO0			0x60c

#ifdef CONFIG_PCIE_MOLCHIP_RC_DE_EMPHASIS
#define PCIE_PHY_X2_DEBUG_CTRL1		0x704
#define PIPE_TXDEEMPH_I_L0_DEBUG_SHIFT	1
#define PIPE_TXDEEMPH_I_L0_DEBUG_MASK	0x3FFFF
#define DEBUG_SEL_PIPE_TXDEEMPH_I_L0	BIT(0)

#define PCIE_PHY_X2_DEBUG_CTRL2		0x708
#define PIPE_TXDEEMPH_I_L1_DEBUG_SHIFT	1
#define PIPE_TXDEEMPH_I_L1_DEBUG_MASK	0x3FFFF
#define DEBUG_SEL_PIPE_TXDEEMPH_I_L1	BIT(0)
#endif

/*CEN_GLB_APB_REG registers*/
#define STRAP_PIN_STATUS		0x7C
#define RESET_CHECK			BIT(3)

#define PLL_CLK_OUTPUT_EN		0xE4
#define CLK_REF_SATA_EN			BIT(1)
#define CLK_REF_PCIE1_EN		BIT(0)
#define PLL_WR_PROTECT			0x100

#define PLL2_CFG3			0xFC
#define PCIE_RG_CLKBUF_LDO_VOUT_SHIFT	11
#define PCIE_RG_CLKBUF_LDO_VOUT_MASK	0x3
#define PCIE_RG_CLKBUF_LDO_VOUT_2_P_117	0x3
#define PCIE_RG_CLKBUF_LDO_VREF_SHIFT	8
#define PCIE_RG_CLKBUF_LDO_VREF_MASK	0x7
#define PCIE_RG_CLKBUF_LDO_VREF_0_P_47	0x3

#define PCIE_MSI_ADDR_LO		0x820
#define PCIE_MSI_ADDR_HI		0x824
#define PCIE_MSI_INTR0_ENABLE		0x828
#define PCIE_MSI_INTR0_MASK		0x82C
#define PCIE_MSI_INTR0_STATUS		0x830

/*AHB_MISC registers*/
#define SUBSYS_CFG0			0x0
#define REFCLK100M_SEL			BIT(9)
#define REG_REFCLK_SEL			BIT(8)

#define PIPE_MISC_1			0x8
#define POR_N_SOFT_RST			BIT(15)
#define DEBUG_SEL_POR_N			BIT(14)
#define CLKREQ_CFG			BIT(12)
#define CLKREF_SRC_CFG_SHIFT		4
#define CLKREF_SRC_CFG_MASK		0x3
#define CLK_SEL_CFG_SHIFT		2
#define CLK_SEL_CFG_MASK		0x3
#define CLK_SEL_25MHZ			1
#define CLK_SEL_100MHZ			2

/*PHY_CFG registers*/
#define PLL_01_REF_CONTROL		0x01004
#define PCIE_REFCLK_TX_EN		BIT(0)
#define PCIE_EN				BIT(2)
#define PCIE_DEVICE_EN			BIT(3)
#define REFCLK_CKOUT_SEL		BIT(4)
#define REFCLK_DRV_SEL_SHIFT		14
#define REFCLK_DRV_SEL_MASK		0xff
/*REFCLKL_DRV_SEL£¬ adjust RS value*/
/*default 11110000: 50ohm, 00000000:83.3ohm, 11111111: 35.7ohm*/
/*  total 8 steps 0: increase 1: decrease*/
#define REFCLK_DRV_SEL_VAL		0xff

#define PHY_LANE01_LDO_VREF_SEL		0x01030
#define LDO_VREF_SEL_MASK		0x7
#define LDO_VREF_SEL_SHIFT		20

#ifdef CONFIG_PCIE_MOLCHIP_RC_SOC_REF_CLK_SSC_ENABLE
#define PHY_LANE01_DACPD_DSMPD		0x01018
#define DACPD				BIT(5)
#define DSMPD				BIT(4)

#define PHY_LANE01_DIVVAL		0x01020
#define DIVVAL_SHIFT			0
#define DIVVAL_MASK			0x1ffffff
#define DIVVAL_VAL			0x19ce739

#define PHY_LANE01_SPREAD		0x01034
#define SPREAD_SHIFT			16
#define SPREAD_MASK			0x7f
#define SPREAD_3900PPM			0xa

#define PHY_LANE01_DISABLE_SSCG		0x0101c
#define DOWNSPREAD			BIT(27)
#define DISABLE_SSCG			BIT(28)

#define LANE01_PLLRI_PCIE_100M_SEL_G3	0x0109c
#define PCIE_100M_SEL_G3_SHIFT		14
#define PCIE_100M_SEL_G3_MASK		0x3
#define PCIE_100M_SEL_G3_VAL		0x3
#endif
#define LANE0_EN_SCOPE_XEYE		0x00810
#define LANE1_EN_SCOPE_XEYE		0x04810

#define EN_SCOPE_XEYE_SHIFT		0
#define EN_SCOPE_XEYE_MASK		0xff

#define LANE0_SCOPE_EN			0x00634
#define LANE1_SCOPE_EN			0x04634
#define SCOPE_EN			BIT(12)

#define LANE0_XEYE_WIDTH		0x007b8
#define LANE1_XEYE_WIDTH		0x047b8
#define XEYE_RIGHT_SHIFT		16
#define XEYE_RIGHT_MASK			0xff
#define XEYE_LEFT_SHIFT			0
#define XEYE_LEFT_MASK			0xff

#define LANE0_YEYE_HEIGHT		0x007bc
#define LANE1_YEYE_HEIGHT		0x047bc
#define YEYE_DOWN_SHIFT			24
#define YEYE_DOWN_MASK			0xff
#define YEYE_UP_SHIFT			8
#define YEYE_UP_MASK			0xff

#define PHY_LANE01_PLL_LOCK		0x013c0
/*1: lock;  0:not lock*/
#define REF_PLL_LOCK			BIT(0)

struct mc_common {
	struct regmap		*hsif_regmap;	/* DT molchip,syscon-hsif-ahb */
	struct regmap		*glb_regmap;	/* DT molchip,syscon-cen-glb */
	void __iomem		*ahb_core_base;	/* DT ahb_core */
	void __iomem		*ahb_misc_base;	/* DT ahb_misc */
	void __iomem		*phy_cfg_base;	/* DT phy_cfg */
	struct clk		*aux_ref_en;
	struct clk		*axi_en;
	struct clk		*ahb_core_en;
	struct clk		*ahb_misc_en;
	struct clk		*phycfg_en;
	int			port_num;
	int			error_irq;
	int			dma_irq;
	int			linkdown_irq;
	char			err_irqname[24];
	char			dma_irqname[24];
	char			linkdown_irqname[24];
};

struct dma_parm {
	u64 rd_src;
	u64 rd_dst;
	u64 wr_src;
	u64 wr_dst;
	u64 rd_sim_src;
	u64 rd_sim_dst;
	u64 wr_sim_src;
	u64 wr_sim_dst;
};

extern u8 internal_ref_clk;
extern u8 check_once;

u32 mc_readl_hsif(struct mc_common *com, u32 offset);
void mc_writel_hsif(struct mc_common *com, u32 offset, u32 val);
u32 mc_readl_glb(struct mc_common *com, u32 offset);
void mc_writel_glb(struct mc_common *com, u32 offset, u32 val);
u32 mc_readl_ahb_misc(struct mc_common *com, u32 offset);
void mc_writel_ahb_misc(struct mc_common *com, u32 offset, u32 val);
u32 mc_readl_phy_cfg(struct mc_common *com, u32 offset);
void mc_writel_phy_cfg(struct mc_common *com, u32 offset, u32 val);
u8 mc_readb_phy_cfg(struct mc_common *com, u32 offset);
void mc_writeb_phy_cfg(struct mc_common *com, u32 offset, u8 val);
u32 mc_readl_ahb_core(struct mc_common *com, u32 offset);
void mc_writel_ahb_core(struct mc_common *com, u32 offset, u32 val);
u32 get_ltssm_state(struct mc_common *com, struct dw_pcie *pci);
int pcie_link_up(struct mc_common *com, struct dw_pcie *pci);
u32 get_strap_status(struct mc_common *com);
u32 get_eco_reg1(struct mc_common *com);
void adjust_pcie_pll_cfg(struct mc_common *com);
void pcie_ref_clk_enable(struct mc_common *com);
int phy_cfg_enable(struct mc_common *com, struct platform_device *pdev);
int aux_axi_core_misc_en(struct mc_common *com, struct platform_device *pdev);
void select_ref_clk_source(struct mc_common *com);
void x2_phy_adjust_ldo_vref_sel(struct mc_common *com);
#ifdef CONFIG_PCIE_MOLCHIP_RC_DE_EMPHASIS
/*level: 0:-6.0db; 1: -3.5db; 2: no de-emphasis*/
void adjust_x2_phy_de_emphasis(struct mc_common *com, u8 level);
#endif
#ifdef CONFIG_PCIE_MOLCHIP_RC_SOC_REF_CLK_SSC_ENABLE
void x2_phy_ssc_enable(struct mc_common *com);
#endif
void refclkpn_pin_direction(struct mc_common *com, u32 type);
void port0_rst_and_clk_enable(struct mc_common *com,
				struct platform_device *pdev, u32 type);
void port1_rst_and_clk_enable(struct mc_common *com,
			struct platform_device *pdev, u32 type);
int pcie_ref_clk_ready_check(struct mc_common *com);
void disable_ltssm(struct mc_common *com);
void set_type(struct mc_common *com, u32 type);
void enable_ltssm(struct mc_common *com);
int get_speed(struct mc_common *com, struct dw_pcie *pci);
int pcie_dma_init(struct mc_common *com, struct dw_pcie *pci);
int pcie_error_init(struct mc_common *com);
void pcie_error_handler(struct mc_common *com, struct dw_pcie *pci);
void pcie_dma_handler(struct mc_common *com, struct dw_pcie *pci);
void pcie_linkdown_handler(struct mc_common *com, struct dw_pcie *pci);
void pcie_compliance_test(struct mc_common *com, struct dw_pcie *pci, int speed, int preset);
void pcie_loopback_test(struct mc_common *com, struct dw_pcie *pci, int speed);
void get_lane_eye_diagram(struct mc_common *com, struct dw_pcie *pci, int speed,
	u32 en_scope_xeye, u32 scope_en, u32 xeye_w, u32 yeye_h);
void set_MPS(struct dw_pcie *pci, unsigned int mps);
void set_RCB(struct dw_pcie *pci, unsigned int rcb);
void set_trgt_map(struct dw_pcie *pci);
void  dma_test(struct dw_pcie *pci, int num, struct dma_parm *parm);

#endif
