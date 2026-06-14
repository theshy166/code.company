// SPDX-License-Identifier: GPL-2.0
/*
 * PCIe controller Common driver for Molchip SoCs
 *
 * Copyright (C) 2023 Molchip Semiconductor.
 *
 * Author: jerry cao <jerry.cao@molchip.com>
 */

#include "pcie-molchip.h"
#include <linux/regmap.h>
#include <linux/clk.h>
#include <asm/cacheflush.h>

u32 int_status_0;
u32 int_status_1;
u32 int_status_2;

u8 internal_ref_clk;
u8 check_once;

u32 mc_readl_hsif(struct mc_common *com, u32 offset)
{
	u32 val;

	regmap_read(com->hsif_regmap, offset, &val);
	return val;
}

void mc_writel_hsif(struct mc_common *com, u32 offset, u32 val)
{
	regmap_write(com->hsif_regmap, offset, val);
}

u32 mc_readl_glb(struct mc_common *com, u32 offset)
{
	u32 val;

	regmap_read(com->glb_regmap, offset, &val);
	return val;
}

void mc_writel_glb(struct mc_common *com, u32 offset, u32 val)
{
	regmap_write(com->glb_regmap, offset, val);
}

u32 mc_readl_ahb_misc(struct mc_common *com, u32 offset)
{
	u32 val = readl(com->ahb_misc_base + offset);
	return val;
}

void mc_writel_ahb_misc(struct mc_common *com, u32 offset, u32 val)
{
	writel(val, com->ahb_misc_base + offset);
}

u32 mc_readl_phy_cfg(struct mc_common *com, u32 offset)
{
	u32 val = readl(com->phy_cfg_base + offset);
	return val;
}

void mc_writel_phy_cfg(struct mc_common *com, u32 offset, u32 val)
{
	writel(val, com->phy_cfg_base + offset);
}

u8 mc_readb_phy_cfg(struct mc_common *com, u32 offset)
{
	u8 val = readb(com->phy_cfg_base + offset);
	return val;
}

void mc_writeb_phy_cfg(struct mc_common *com, u32 offset, u8 val)
{
	writeb(val, com->phy_cfg_base + offset);
}

u32 mc_readl_ahb_core(struct mc_common *com, u32 offset)
{
	u32 val = readl(com->ahb_core_base + offset);
	return val;
}

void mc_writel_ahb_core(struct mc_common *com, u32 offset, u32 val)
{
	writel(val, com->ahb_core_base + offset);
}

void set_MPS(struct dw_pcie *pci, unsigned int mps)
{
	unsigned int val = 0;
	u8 pcie_base = 0;

	pcie_base = dw_pcie_find_capability(pci, PCI_CAP_ID_EXP);

	val = dw_pcie_readl_dbi(pci,
		pcie_base + DEVICE_CONTROL_DEVICE_STATUS);
	val &= ~(PCIE_CAP_MAX_PAYLOAD_SIZE_CS_MASK << PCIE_CAP_MAX_PAYLOAD_SIZE_CS_SHIFT);
	val |= mps << PCIE_CAP_MAX_PAYLOAD_SIZE_CS_SHIFT;
	dw_pcie_writel_dbi(pci,
		pcie_base + DEVICE_CONTROL_DEVICE_STATUS, val);
}

void set_RCB(struct dw_pcie *pci, unsigned int rcb)
{
	unsigned int val = 0;
	u8 pcie_base = 0;

	pcie_base = dw_pcie_find_capability(pci, PCI_CAP_ID_EXP);

	val = dw_pcie_readl_dbi(pci,
		pcie_base + LINK_CONTROL_LINK_STATUS_REG);
	val &= ~(PCIE_CAP_RCB_MASK << PCIE_CAP_RCB_SHIFT);
	val |= rcb << PCIE_CAP_RCB_SHIFT;
	dw_pcie_writel_dbi(pci,
		pcie_base + LINK_CONTROL_LINK_STATUS_REG, val);
}

void set_trgt_map(struct dw_pcie *pci)
{
	u32 val = 0;

	val = dw_pcie_readl_dbi(pci, TRGT_MAP_CTRL_OFF);
	val |= (BAR0_TRGT_MAP_CTRL | BAR1_TRGT_MAP_CTRL | BAR2_TRGT_MAP_CTRL |
		BAR3_TRGT_MAP_CTRL | BAR4_TRGT_MAP_CTRL | BAR5_TRGT_MAP_CTRL);
	dw_pcie_writel_dbi(pci, TRGT_MAP_CTRL_OFF, val);
}

u32 get_ltssm_state(struct mc_common *com, struct dw_pcie *pci)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, SII_POWER_MAN1);
	val &= ((SMLH_LTSSM_STATE_MASK << SMLH_LTSSM_STATE_SHIFT) |
		PM_CURNT_STATE_MASK);
	val = (val >> SMLH_LTSSM_STATE_SHIFT);
	switch (val) {
	case 0x00:
		dev_info(pci->dev, "ltssm state:s_detect_quiet\r\n");
		break;

	case 0x01:
		dev_info(pci->dev, "ltssm state:s_detect_act\r\n");
		break;

	case 0x02:
		dev_info(pci->dev, "ltssm state:s_poll_active\r\n");
		break;

	case 0x03:
		dev_info(pci->dev, "ltssm state:s_poll_compliance\r\n");
		break;

	case 0x04:
		dev_info(pci->dev, "ltssm state:s_poll_config\r\n");
		break;

	case 0x05:
		dev_info(pci->dev, "ltssm state:s_pre_detect_quiet\r\n");
		break;

	case 0x06:
		dev_info(pci->dev, "ltssm state:s_detect_wait\r\n");
		break;

	case 0x07:
		dev_info(pci->dev, "ltssm state:s_cfg_linnkwd_start\r\n");
		break;

	case 0x08:
		dev_info(pci->dev, "ltssm state:s_cfg_linkwd_acept\r\n");
		break;

	case 0x09:
		dev_info(pci->dev, "ltssm state:s_cfg_lanenum_wai\r\n");
		break;

	case 0x0A:
		dev_info(pci->dev, "ltssm state:s_cfg_lanenum_acept\r\n");
		break;

	case 0x0B:
		dev_info(pci->dev, "ltssm state:s_cfg_complete\r\n");
		break;

	case 0x0C:
		dev_info(pci->dev, "ltssm state:s_cfg_idle\r\n");
		break;

	case 0x0D:
		dev_info(pci->dev, "ltssm state:s_rcvry_lock\r\n");
		break;

	case 0x0E:
		dev_info(pci->dev, "ltssm state:s_recvry_speed\r\n");
		break;

	case 0x0F:
		dev_info(pci->dev, "ltssm state:s_rcvry_rcvrccfg\r\n");
		break;

	case 0x10:
		dev_info(pci->dev, "ltssm state:s_rcvry_idle\r\n");
		break;

	case 0x11:
		dev_info(pci->dev, "ltssm state:s_l0\r\n");
		break;

	case 0x12:
		dev_info(pci->dev, "ltssm state:s_l0s\r\n");
		break;

	case 0x13:
		dev_info(pci->dev, "ltssm state:s_l123_send_eidle\r\n");
		break;

	case 0x14:
		dev_info(pci->dev, "ltssm state:s_l1_idle\r\n");
		break;

	case 0x15:
		dev_info(pci->dev, "ltssm state:s_l2_idle\r\n");
		break;

	case 0x16:
		dev_info(pci->dev, "ltssm state:s_l2_wake\r\n");
		break;

	case 0x17:
		dev_info(pci->dev, "ltssm state:s_disabled_entry\r\n");
		break;

	case 0x18:
		dev_info(pci->dev, "ltssm state:s_disabled_idle\r\n");
		break;

	case 0x19:
		dev_info(pci->dev, "ltssm state:s_disabled\r\n");
		break;

	case 0x1A:
		dev_info(pci->dev, "ltssm state:s_lpbk_entry\r\n");
		break;

	case 0x1B:
		dev_info(pci->dev, "ltssm state:s_lpbk_active\r\n");
		break;

	case 0x1C:
		dev_info(pci->dev, "ltssm state:s_lpbk_exit\r\n");
		break;

	case 0x1D:
		dev_info(pci->dev, "ltssm state:s_lpbk_exit_timeout\r\n");
		break;

	case 0x1E:
		dev_info(pci->dev, "ltssm state:s_hot-reset_entry\r\n");
		break;

	case 0x1F:
		dev_info(pci->dev, "ltssm state:s_hot_reset\r\n");
		break;

	case 0x20:
		dev_info(pci->dev, "ltssm state:s_rcvry_eq0\r\n");
		break;

	case 0x21:
		dev_info(pci->dev, "ltssm state:s_rcvry_eq1\r\n");
		break;

	case 0x22:
		dev_info(pci->dev, "ltssm state:s_rcvry_eq2\r\n");
		break;

	case 0x23:
		dev_info(pci->dev, "ltssm state:s_rcvry_eq3\r\n");
		break;

	default:
		dev_info(pci->dev, "ltssm state:s_l0\r\n");
		break;
	}
	return val;
}


int pcie_link_up(struct mc_common *com, struct dw_pcie *pci)
{
	u32 val = 0;
	u8 parm = 0;

	val = mc_readl_ahb_core(com, SII_LINK_RST_STATUS);
	if (val & SMLH_LINK_UP) {
		val = mc_readl_ahb_core(com, SII_DEBUG_INTERFACE0);
		if (val & RDLH_LINK_UP)
			return 1;
		else
			return 0;
	} else {
		if (get_ltssm_state(com, pci) == 0) {
			if (com->port_num == 1) {
				parm = mc_readb_phy_cfg(com,
					0x19);
				parm |= BIT(5);
				mc_writeb_phy_cfg(com, 0x19,
					parm);
			}
			return 2;
		} else {
			return 0;
		}
	}
}

u32 get_strap_status(struct mc_common *com)
{
	return mc_readl_glb(com, STRAP_PIN_STATUS);
}

u32 get_eco_reg1(struct mc_common *com)
{
	return mc_readl_hsif(com, ECO_REG1);
}

void adjust_pcie_pll_cfg(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_glb(com, PLL2_CFG3);
	val &= ~(PCIE_RG_CLKBUF_LDO_VOUT_MASK << PCIE_RG_CLKBUF_LDO_VOUT_SHIFT);
	val |= (PCIE_RG_CLKBUF_LDO_VOUT_2_P_117 <<
			PCIE_RG_CLKBUF_LDO_VOUT_SHIFT);
	val &= ~(PCIE_RG_CLKBUF_LDO_VREF_MASK << PCIE_RG_CLKBUF_LDO_VREF_SHIFT);
	val |= (PCIE_RG_CLKBUF_LDO_VREF_0_P_47 <<
			PCIE_RG_CLKBUF_LDO_VREF_SHIFT);
	mc_writel_glb(com, PLL2_CFG3, val);
}

void pcie_ref_clk_enable(struct mc_common *com)
{
	u32 val = 0;
	u32 enable_bit = 0;

	adjust_pcie_pll_cfg(com);

	if (com->port_num == 0)
		enable_bit = CLK_REF_PCIE1_EN;
	else
		enable_bit = CLK_REF_SATA_EN;

	/*remove write protect*/
	val = 0xFEDC0123;
	mc_writel_glb(com, PLL_WR_PROTECT, val);

	val = mc_readl_glb(com, PLL_CLK_OUTPUT_EN);
	if (internal_ref_clk) {
		/*High, internal clk, enable PLL*/
		val |= enable_bit;
	} else {
		/*default Low, extral clk, disable PLL*/
		val &= (u32)(~enable_bit);
	}
	mc_writel_glb(com, PLL_CLK_OUTPUT_EN, val);

	/*restore write protect*/
	val = 0;
	mc_writel_glb(com, PLL_WR_PROTECT, val);
}

int phy_cfg_enable(struct mc_common *com, struct platform_device *pdev)
{
	int ret;

	com->phycfg_en = devm_clk_get(&pdev->dev, "phycfg_en");
	if (IS_ERR(com->phycfg_en)) {
		ret = PTR_ERR(com->phycfg_en);
		dev_err(&pdev->dev, "phycfg base enable failed (%d)\n", ret);
		return ret;
	}

	ret = clk_prepare_enable(com->phycfg_en);
	if (ret) {
		if (!IS_ERR(com->phycfg_en))
			clk_disable_unprepare(com->phycfg_en);
	}

	return ret;
}

int aux_axi_core_misc_en(struct mc_common *com, struct platform_device *pdev)
{
	int ret;

	com->aux_ref_en = devm_clk_get(&pdev->dev, "aux_ref_en");
	if (IS_ERR(com->aux_ref_en)) {
		ret = PTR_ERR(com->aux_ref_en);
		dev_err(&pdev->dev, "aux ref enable failed (%d)\n", ret);
		goto clk_disable;
	}

	ret = clk_prepare_enable(com->aux_ref_en);
	if (ret)
		goto clk_disable;

	com->axi_en = devm_clk_get(&pdev->dev, "axi_en");
	if (IS_ERR(com->axi_en)) {
		ret = PTR_ERR(com->axi_en);
		dev_err(&pdev->dev, "axi enable failed (%d)\n", ret);
		goto clk_disable;
	}

	ret = clk_prepare_enable(com->axi_en);
	if (ret)
		goto clk_disable;

	com->ahb_core_en = devm_clk_get(&pdev->dev, "ahb_core_en");
	if (IS_ERR(com->ahb_core_en)) {
		ret = PTR_ERR(com->ahb_core_en);
		dev_err(&pdev->dev, "ahb core enable failed (%d)\n", ret);
		goto clk_disable;
	}

	ret = clk_prepare_enable(com->ahb_core_en);
	if (ret)
		goto clk_disable;

	com->ahb_misc_en = devm_clk_get(&pdev->dev, "ahb_misc_en");
	if (IS_ERR(com->ahb_misc_en)) {
		ret = PTR_ERR(com->ahb_misc_en);
		dev_err(&pdev->dev, "ahb misc enable failed (%d)\n", ret);
		goto clk_disable;
	}

	ret = clk_prepare_enable(com->ahb_misc_en);
	if (ret)
		goto clk_disable;

	return 0;

clk_disable:
	if (!IS_ERR(com->aux_ref_en))
		clk_disable_unprepare(com->aux_ref_en);
	if (!IS_ERR(com->axi_en))
		clk_disable_unprepare(com->axi_en);
	if (!IS_ERR(com->ahb_core_en))
		clk_disable_unprepare(com->ahb_core_en);
	if (!IS_ERR(com->ahb_misc_en))
		clk_disable_unprepare(com->ahb_misc_en);

	return ret;
}

void select_ref_clk_source(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_ahb_misc(com, SUBSYS_CFG0);
	if (com->port_num == 0) {
		if (internal_ref_clk) {
			/*High, internal single-end clk*/
			val &= (u32)(~REFCLK100M_SEL);
			val &= (u32)(~REG_REFCLK_SEL);
		} else {
			/*default Low, extral clk*/
			val &= (u32)(~REFCLK100M_SEL);
			val |= (u32)(REG_REFCLK_SEL);
		}
	}
	mc_writel_ahb_misc(com, SUBSYS_CFG0, val);
}

void x2_phy_adjust_ldo_vref_sel(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_phy_cfg(com, PHY_LANE01_LDO_VREF_SEL);
	val &= (u32)(~(LDO_VREF_SEL_MASK << LDO_VREF_SEL_SHIFT));
	mc_writel_phy_cfg(com, PHY_LANE01_LDO_VREF_SEL, val);
}

void refclkpn_pin_direction(struct mc_common *com, u32 type)
{
	u32 val = 0;

	val = mc_readl_phy_cfg(com, PLL_01_REF_CONTROL);
	if (internal_ref_clk) {
		if (type == ROOT_PORT_OF_PCIE_RC) {
		/*High, internal clk, output 100MHz, REFCLKP/N driver enable*/
			val |= PCIE_REFCLK_TX_EN;
			val &= (u32)(~PCIE_DEVICE_EN);
			val |= PCIE_EN;
			val &= ~(REFCLK_DRV_SEL_MASK << REFCLK_DRV_SEL_SHIFT);
			val |= (REFCLK_DRV_SEL_VAL << REFCLK_DRV_SEL_SHIFT);
			val |= REFCLK_CKOUT_SEL;
		} else {
			/*High, internal clk, REFCLKP/N power down*/
			val &= (u32)(~PCIE_REFCLK_TX_EN);
			val &= (u32)(~PCIE_DEVICE_EN);
			val &= (u32)(~PCIE_EN);
		}
	} else {
		/*default Low, extal clk, REFCLKP/N receive enable*/
		val &= (u32)(~PCIE_REFCLK_TX_EN);
		val |= (u32)(PCIE_DEVICE_EN);
		val |= (u32)(PCIE_EN);
	}
	mc_writel_phy_cfg(com, PLL_01_REF_CONTROL, val);
}

void port0_rst_and_clk_enable(struct mc_common *com,
				struct platform_device *pdev, u32 type)
{
	u32 val = 0;

	/* pipe rst, port rst*/
	val = mc_readl_hsif(com, PCIE_CTRL_SOFT);
	val |= (X2_DEBUG_SEL_POR_N | X2_DEBUG_SEL_PIPE_RSTN_I
		| X2_PIPE_SOFT_RST | X2_POR_N_SOFT_RST);
	mc_writel_hsif(com, PCIE_CTRL_SOFT, val);

	/*apb_phycfg rst*/
	val = mc_readl_hsif(com, PCIE_PHY_CLK_RST_CTRL);
	val |= PHY_X2_SOFT_RST;
	mc_writel_hsif(com, PCIE_PHY_CLK_RST_CTRL, val);

	/*ahb misc rst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val |= PCIE2_MISC_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	/*ahb core rst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val |= PCIE2_CORE_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	/*axi_dbi rst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val |= PCIE2_AXI_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	phy_cfg_enable(com, pdev);

	udelay(1000);

	aux_axi_core_misc_en(com, pdev);

	udelay(1000);

	/*ahb misc unrst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val &= ~PCIE2_MISC_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*ahb core unrst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val &= ~PCIE2_CORE_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*axi_dbi unrst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val &= ~PCIE2_AXI_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*apb_phycfg unrst*/
	val = mc_readl_hsif(com, PCIE_PHY_CLK_RST_CTRL);
	val &= ~PHY_X2_SOFT_RST;
	mc_writel_hsif(com, PCIE_PHY_CLK_RST_CTRL, val);

	udelay(1000);

	/*select clock source*/
	select_ref_clk_source(com);
	udelay(1000);

	/*REFCLKP/N direction*/
	refclkpn_pin_direction(com, type);
	udelay(1000);

	x2_phy_adjust_ldo_vref_sel(com);
	#ifdef CONFIG_PCIE_MOLCHIP_RC_SOC_REF_CLK_SSC_ENABLE
	x2_phy_ssc_enable(com);
	#endif

	/*port unrst*/
	val = mc_readl_hsif(com, PCIE_CTRL_SOFT);
	val &= ~X2_POR_N_SOFT_RST;
	mc_writel_hsif(com, PCIE_CTRL_SOFT, val);

	udelay(1000);//at least 105us

	/*pipe unrst*/
	val = mc_readl_hsif(com, PCIE_CTRL_SOFT);
	val &= ~X2_PIPE_SOFT_RST;
	mc_writel_hsif(com, PCIE_CTRL_SOFT, val);

	udelay(1000);
}

void port1_rst_and_clk_enable(struct mc_common *com,
			struct platform_device *pdev, u32 type)
{
	u32 val = 0;

	mc_writel_hsif(com, PCIE_GEN2_X1_COMPHY_CTRL, 0);

	/*ahb misc rst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val |= PCIE_GEN2_X1_MISC_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*ahb misc clk enable*/
	val = mc_readl_hsif(com, PCIE_X2_SYS_CLK_RST_CTRL);
	val |= CKG_AHB_PCIE_GEN2_X1_MISC_EN;
	mc_writel_hsif(com, PCIE_X2_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*ahb misc unrst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val &= ~PCIE_GEN2_X1_MISC_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*phy rst*/
	val = mc_readl_ahb_misc(com, PIPE_MISC_1);
	val |= (POR_N_SOFT_RST | DEBUG_SEL_POR_N);
	mc_writel_ahb_misc(com, PIPE_MISC_1, val);

	/*clk sel*/
	val = mc_readl_ahb_misc(com, PIPE_MISC_1);
	val &= ~(CLK_SEL_CFG_MASK << CLK_SEL_CFG_SHIFT);
	val |= (CLK_SEL_100MHZ << CLK_SEL_CFG_SHIFT);//FPGA:CLK_SEL_25MHZ,
	mc_writel_ahb_misc(com, PIPE_MISC_1, val);

	/*clk ref src & CLKREFP/N direction*/
	val = mc_readl_ahb_misc(com, PIPE_MISC_1);
	val &= ~(CLKREF_SRC_CFG_MASK << CLKREF_SRC_CFG_SHIFT);
	if (internal_ref_clk) {
		if (type == ROOT_PORT_OF_PCIE_RC) {
		/*Use PLL_CKREF_INNER as reference clock source*/
		/*Use CKREFP/N as output clock to provide a differential*/
		/*100M reference clock in PCIE mode while CLKREQ=1.*/
			val |= (0x2 << CLKREF_SRC_CFG_SHIFT);
			val |= CLKREQ_CFG;
		} else {
			/*Use PLL_CKREF_INNER as reference clock source*/
			/*CKREFP/N is not active, the PAD can be floating*/
			val |= (0x0 << CLKREF_SRC_CFG_SHIFT);
		}
	} else {
		/*Use CKREFP/N as input reference clock source*/
		/*PLL_CKREF_INNER is not active, the logic can be High or Low*/
		val |= (0x1 << CLKREF_SRC_CFG_SHIFT);
	}
	mc_writel_ahb_misc(com, PIPE_MISC_1, val);

	/*apb_phycfg rst*/
	val = mc_readl_hsif(com, COMPHY_CLK_RST);
	val |= CMPHY2_CFG_SOFT_RST;
	mc_writel_hsif(com, COMPHY_CLK_RST, val);

	/*ahb core rst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val |= PCIE_GEN2_X1_CORE_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	/*axi_dbi rst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val |= PCIE_GEN2_X1_AXI_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	phy_cfg_enable(com, pdev);

	udelay(1000);

	aux_axi_core_misc_en(com, pdev);

	udelay(1000);

	/*ahb core unrst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val &= ~PCIE_GEN2_X1_CORE_AHB_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*axi_dbi unrst*/
	val = mc_readl_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL);
	val &= ~PCIE_GEN2_X1_AXI_SOFT_RST;
	mc_writel_hsif(com, PCIE_X4_SYS_CLK_RST_CTRL, val);

	udelay(1000);

	/*apb_phycfg unrst*/
	val = mc_readl_hsif(com, COMPHY_CLK_RST);
	val &= ~CMPHY2_CFG_SOFT_RST;
	mc_writel_hsif(com, COMPHY_CLK_RST, val);

	udelay(1000);

	/*phy unrst*/
	val = mc_readl_ahb_misc(com, PIPE_MISC_1);
	val |= DEBUG_SEL_POR_N;
	val &= ~POR_N_SOFT_RST;
	mc_writel_ahb_misc(com, PIPE_MISC_1, val);

	udelay(1000);
}

int pcie_ref_clk_ready_check(struct mc_common *com)
{
	u32 val = 0;
	int ret = 0;

	if (com->port_num == 0) {
		val = mc_readl_phy_cfg(com, PHY_LANE01_PLL_LOCK);
		if (val & REF_PLL_LOCK)
			ret = 0;
		else
			ret = -1;
	}

	if (com->port_num == 1) {
		val = mc_readl_hsif(com, COMBPHY2_DBGSEL);
		val &= ~(COMPHY2_TEST_SEL_MASK << COMPHY2_TEST_SEL_SHIFT);
		val |= TEST_SEL_PLLLOCKED << COMPHY2_TEST_SEL_SHIFT;
		mc_writel_hsif(com, COMBPHY2_DBGSEL, val);

		val = mc_readl_hsif(com, COMBPHY2_INFO1);
		if (val & COMPHY2_STATUS_INFO_TESTTO)
			ret = 0;
		else
			ret = -1;
	}

	return ret;
}

void disable_ltssm(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val &= ~APP_LTSSM_ENABLE;
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);
}

void set_type(struct mc_common *com, u32 type)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val &= ~(DEVICE_TYPE_MASK << DEVICE_TYPE_SHIFT);
	val |= (type << DEVICE_TYPE_SHIFT);
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);
}

void enable_ltssm(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val |= APP_LTSSM_ENABLE;
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);
}

int get_speed(struct mc_common *com, struct dw_pcie *pci)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, PIPE_INFO0);
	if ((com->port_num == 0) || (com->port_num == 1))
		val = (val >> 2)&0x7;

	if (val == 0)
		dev_info(pci->dev, "speed: GEN1\r\n");
	if (val == 1)
		dev_info(pci->dev, "speed: GEN2\r\n");
	if (val == 2)
		dev_info(pci->dev, "speed: GEN3\r\n");

	return (val + 1);
}

int pcie_dma_init(struct mc_common *com, struct dw_pcie *pci)
{
	u32 val = 0;

	val = dw_pcie_readl_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF);
	val &= ~(0xFF << 16);
	val &= ~(0xFF);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF, val);

	val = dw_pcie_readl_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF);
	val &= ~(0xFF << 16);
	val &= ~(0xFF);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF, val);

	val = mc_readl_ahb_core(com, INT_MASK2);
	val &= ~(0xFF);
	mc_writel_ahb_core(com, INT_MASK2, val);


	return 0;
}

int pcie_error_init(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, INT_MASK0);
	val &= ~(0x3FF);
	mc_writel_ahb_core(com, INT_MASK0, val);

	val = mc_readl_ahb_core(com, INT_MASK1);
	val &= ~(0x1FFFFF);
	mc_writel_ahb_core(com, INT_MASK1, val);

	return 0;
}

void pcie_error_handler(struct mc_common *com, struct dw_pcie *pci)
{
	u32 status_0 = 0;
	u32 status_1 = 0;
	u32 i = 0;

	status_0 = mc_readl_ahb_core(com, INT_STATUS0);

	status_1 = mc_readl_ahb_core(com, INT_STATUS1);

	for (i = 0; i <= 13; i++) {
		if ((status_0 & (1 << i)) != 0)
			int_status_0 |= (1 << i);
	}

	for (i = 0; i <= 20; i++) {
		if ((status_1 & (1 << i)) != 0)
			int_status_1 |= (1 << i);
	}

}


void pcie_dma_handler(struct mc_common *com, struct dw_pcie *pci)
{
	u32 val = 0;
	u32 i = 0;

	val = mc_readl_ahb_core(com, INT_STATUS2);
	for (i = 0; i <= 7; i++) {
		if ((val & (1 << i)) != 0)
			int_status_2 |= (1 << i);
	}

	val = dw_pcie_readl_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_STATUS_OFF);
	#if defined(CONFIG_PCIE_MOLCHIP_EP_DMA_TEST) || \
	defined(CONFIG_PCIE_MOLCHIP_RC_DMA_TEST)
	dev_info(pci->dev, "int_status_2=0x%x, wrint_int=0x%x\r\n",
					int_status_2, val);
	#endif
	if (val >> 16)
		dev_info(pci->dev, "DMA write abort\r\n");

	for (i = 16; i <= 23; i++) {
		if ((val & (1 << i)) != 0)
			dw_pcie_writel_dbi(pci,
			PCIE_DMA_OFFSET + DMA_WRITE_INT_CLEAR_OFF, (1 << i));
	}
	for (i = 0; i <= 7; i++) {
		if ((val & (1 << i)) != 0)
			dw_pcie_writel_dbi(pci,
			PCIE_DMA_OFFSET + DMA_WRITE_INT_CLEAR_OFF, (1 << i));
	}

	val = dw_pcie_readl_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_STATUS_OFF);
	#if defined(CONFIG_PCIE_MOLCHIP_EP_DMA_TEST) || \
	defined(CONFIG_PCIE_MOLCHIP_RC_DMA_TEST)
	dev_info(pci->dev, "int_status_2=0x%x, rdint_int=0x%x\r\n",
					int_status_2, val);
	#endif
	if (val >> 16)
		dev_info(pci->dev, "DMA read abort\r\n");
	for (i = 16; i <= 23; i++) {
		if ((val & (1 << i)) != 0)
			dw_pcie_writel_dbi(pci,
			PCIE_DMA_OFFSET + DMA_READ_INT_CLEAR_OFF, (1 << i));
	}
	for (i = 0; i <= 7; i++) {
		if ((val & (1 << i)) != 0)
			dw_pcie_writel_dbi(pci,
			PCIE_DMA_OFFSET + DMA_READ_INT_CLEAR_OFF, (1 << i));
	}

}

void pcie_linkdown_handler(struct mc_common *com, struct dw_pcie *pci)
{
	unsigned int val;

	get_ltssm_state(com, pci);
	val = mc_readl_ahb_core(com, INT_PCIE_LINK_DOWN_STATUS);
	if (val & INT_LINK_DOWN_STATUS)
		dev_info(pci->dev, "link status:0x%x-->link down\r\n", val);

}


void pcie_compliance_test(struct mc_common *com, struct dw_pcie *pci, int speed, int preset)
{
	u32 val = 0;
	u8 pcie_base = 0;
	int i;

	//1. disable LTSSM_ENABLE
	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val &= ~APP_LTSSM_ENABLE;
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);

	//2. enable ENTER_COMPLIANCE, set target speed, set preset
	pcie_base = dw_pcie_find_capability(pci, PCI_CAP_ID_EXP);

	val = dw_pcie_readl_dbi(pci, pcie_base + LINK_CONTROL2_LINK_STATUS2_REG);
	val &= ~(TARGET_LINK_SPEED_MASK << TARGET_LINK_SPEED_SHIFT);
	val |= speed << TARGET_LINK_SPEED_SHIFT;
	val &= ~(COMPLIANCE_PRESET_MASK << COMPLIANCE_PRESET_SHIFT);
	val |= preset << COMPLIANCE_PRESET_SHIFT;
	val |= ENTER_COMPLIANCE;

	dw_pcie_writel_dbi(pci,
		pcie_base + LINK_CONTROL2_LINK_STATUS2_REG, val);

	//3.enable LTSSM_ENABLE
	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val |= APP_LTSSM_ENABLE;
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);

	//4.check ltssm status
	for (i = 0; i < 100; i++) {
		val = get_ltssm_state(com, pci);
		if (val == 0x03)
			break;
	}

	//5.check link speed
	get_speed(com, pci);
}


void pcie_loopback_test(struct mc_common *com, struct dw_pcie *pci, int speed)
{
	u32 val = 0;
	u8 pcie_base = 0;
	int i;

	//1. disable LTSSM_ENABLE
	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val &= ~APP_LTSSM_ENABLE;
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);

	//2. disable ENTER_COMPLIANCE, set target speed, set default preset
	pcie_base = dw_pcie_find_capability(pci, PCI_CAP_ID_EXP);

	val = dw_pcie_readl_dbi(pci, pcie_base + LINK_CONTROL2_LINK_STATUS2_REG);
	val &= ~ENTER_COMPLIANCE;
	val &= ~(TARGET_LINK_SPEED_MASK << TARGET_LINK_SPEED_SHIFT);
	val |= speed << TARGET_LINK_SPEED_SHIFT;
	val &= ~(COMPLIANCE_PRESET_MASK << COMPLIANCE_PRESET_SHIFT);

	dw_pcie_writel_dbi(pci,
		pcie_base + LINK_CONTROL2_LINK_STATUS2_REG, val);

	val = dw_pcie_readl_dbi(pci, GEN3_RELATED_OFF);
	val |= GEN3_EQUALIZATION_DISABLE_BIT;
	dw_pcie_writel_dbi(pci, GEN3_RELATED_OFF, val);

	val = dw_pcie_readl_dbi(pci, PIPE_LOOPBACK_CONTROL_OFF);
	val |= PIPE_LOOPBACK_BIT;
	dw_pcie_writel_dbi(pci, PIPE_LOOPBACK_CONTROL_OFF, val);

	//3. set loopback enable
	val = dw_pcie_readl_dbi(pci, PCIE_PORT_LINK_CONTROL);
	val |= LOOPBACK_ENABLE;
	dw_pcie_writel_dbi(pci, PCIE_PORT_LINK_CONTROL, val);

	//4.enable LTSSM_ENABLE
	val = mc_readl_ahb_core(com, SII_GENERAL_CORE_CTRL);
	val |= APP_LTSSM_ENABLE;
	mc_writel_ahb_core(com, SII_GENERAL_CORE_CTRL, val);

	//5.check ltssm status
	for (i = 0; i < 100; i++) {
		val = get_ltssm_state(com, pci);
		if (val == 0x1B)
			break;
	}

	//6.check link speed
	get_speed(com, pci);
}

void get_lane_eye_diagram(struct mc_common *com, struct dw_pcie *pci, int speed,
	u32 en_scope_xeye, u32 scope_en, u32 xeye_w, u32 yeye_h)
{
	u32 val = 0;
	u32 x_l, x_r, y_u, y_d;
	u32 eye_w, eye_h;

	/* 1. en scope xeye_right */
	val = mc_readl_phy_cfg(com, en_scope_xeye);
	val |= (EN_SCOPE_XEYE_MASK << EN_SCOPE_XEYE_SHIFT);
	mc_writel_phy_cfg(com, en_scope_xeye, val);

	/* 2.scope en =0, to clear last time EYE value */
	val = mc_readl_phy_cfg(com, scope_en);
	val &= ~SCOPE_EN;
	mc_writel_phy_cfg(com, scope_en, val);

	/* 3.scope en =1 , begin sweep EYE scope */
	val = mc_readl_phy_cfg(com, scope_en);
	val |= SCOPE_EN;
	mc_writel_phy_cfg(com, scope_en, val);

	/* 4.wait a long time for scope end */
	mdelay(200);

	/* 5.read  eye data */
	val = mc_readl_phy_cfg(com, xeye_w);
	x_l = (val >> XEYE_LEFT_SHIFT) & XEYE_LEFT_MASK;
	x_r = (val >> XEYE_RIGHT_SHIFT) & XEYE_RIGHT_MASK;

	val = mc_readl_phy_cfg(com, yeye_h);
	y_u = (val >> YEYE_UP_SHIFT) & YEYE_UP_MASK;
	y_d = (val >> YEYE_DOWN_SHIFT) & YEYE_DOWN_MASK;

	eye_w = (x_r - x_l);
	eye_w *= 200;
	eye_w /= 64;
	eye_h = (y_d - y_u) * 2 * 2;
	if (speed == 1) {
		if ((eye_w >= 40) && (eye_h >= 175))
			dev_info(pci->dev, "lane width: %d, height %d, pass\r\n",
			eye_w, eye_h);
		else
			dev_info(pci->dev, "lane width: %d, height %d, fail\r\n",
			eye_w, eye_h);
	} else if (speed == 2) {
		if ((eye_w >= 32) && (eye_h >= 100))
			dev_info(pci->dev, "lane width: %d, height %d, pass\r\n",
			eye_w, eye_h);
		else
			dev_info(pci->dev, "lane width: %d, height %d, fail\r\n",
			eye_w, eye_h);
	} else if (speed == 3) {
		if ((eye_w >= 30) && (eye_h >= 25))
			dev_info(pci->dev, "lane width: %d, height %d, pass\r\n",
			eye_w, eye_h);
		else
			dev_info(pci->dev, "lane width: %d, height %d, fail\r\n",
			eye_w, eye_h);
	}

}

void dma_write_lie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
			u32 dar_l, u32 dar_h, u32 size)
{
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
						DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_CH_CONTROL1_OFF_WRCH, LIE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_CH_CONTROL2_OFF_WRCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_TRANSFER_SIZE_OFF_WRCH, size);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_SAR_LOW_OFF_WRCH, sar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_SAR_HIGH_OFF_WRCH, sar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_DAR_LOW_OFF_WRCH, dar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_DAR_HIGH_OFF_WRCH, dar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_LLP_LOW_OFF_WRCH, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_LLP_HIGH_OFF_WRCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DOORBELL_OFF,
						ch_num);
}

void ll_dma_write_lie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
			u32 dar_l, u32 dar_h, u32 entry_size, u32 total_size)
{
	u32 count = ((total_size / entry_size) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH)
			+ LL_DMA_LINK_ELEMENT_DWORD_LENGTH;
	u32 data_element_count = (total_size / entry_size);
	u32 *link_list_tt = NULL;
	u32 i = 0;
	u64 addr = 0;
	u32 ll_low = 0;
	u32 ll_hi = 0;

	link_list_tt = (u32 *)__get_dma_pages(GFP_KERNEL,
				get_order(sizeof(u32)*count));
	if (link_list_tt == NULL) {
		dev_info(pci->dev, "malloc %d fail\r\n", count);
		return;
	}

	addr = virt_to_phys(link_list_tt);
	ll_low = lower_32_bits(addr);
	ll_hi = upper_32_bits(addr);

	for (i = 0; i < data_element_count; i++) {
		link_list_tt[i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = CB;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 1]
						= entry_size;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 2]
						= sar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 3]
						= sar_h;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 4]
						= dar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 5]
						= dar_h;
	}

	link_list_tt[(data_element_count - 1) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);
	link_list_tt[(data_element_count / 2) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);

	link_list_tt[count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH] = (LLP | TCB);
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 1] = 0;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 2] = ll_low;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 3] = ll_hi;

	__flush_dcache_area(link_list_tt, sizeof(u32)*count);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
						DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF,
				(1 << (ch_num + WR_CHANNEL_LLLAIE_SHIFT)));

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_WRCH, (CCS | LLE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_WRCH, ll_low);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_WRCH, ll_hi);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DOORBELL_OFF,
				ch_num);
}

void dma_write_rie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
				u32 dar_l, u32 dar_h, u32 size)
{
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
					DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF,
				(1 << (ch_num + WR_ABORT_INT_MASK_SHIFT)) |
				(1 << (ch_num + WR_DONE_INT_MASK_SHIFT)));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DONE_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DONE_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ABORT_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ABORT_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH01_IMWR_DATA_OFF,
				0x00010000);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH23_IMWR_DATA_OFF,
				0x00030002);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH45_IMWR_DATA_OFF,
				0x00050004);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH67_IMWR_DATA_OFF,
				0x00070006);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_WRCH, (RIE | LIE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL2_OFF_WRCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_TRANSFER_SIZE_OFF_WRCH, size);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_LOW_OFF_WRCH, sar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_HIGH_OFF_WRCH, sar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_LOW_OFF_WRCH, dar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_HIGH_OFF_WRCH, dar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_WRCH, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_WRCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DOORBELL_OFF, ch_num);
}

void ll_dma_write_rie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
			u32 dar_l, u32 dar_h, u32 entry_size, u32 total_size)
{
	u32 count = ((total_size / entry_size) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH) +
			LL_DMA_LINK_ELEMENT_DWORD_LENGTH;
	u32 data_element_count = (total_size / entry_size);
	u32 *link_list_tt = NULL;
	u32 i = 0;
	u64 addr = 0;
	u32 ll_low = 0;
	u32 ll_hi = 0;

	link_list_tt = (u32 *)__get_dma_pages(GFP_KERNEL,
				get_order(sizeof(u32)*count));
	if (link_list_tt == NULL) {
		dev_info(pci->dev, "malloc %d fail\r\n", count);
		return;
	}

	addr = virt_to_phys(link_list_tt);
	ll_low = lower_32_bits(addr);
	ll_hi = upper_32_bits(addr);

	for (i = 0; i < data_element_count; i++) {
		link_list_tt[i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = CB;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 1]
					= entry_size;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 2]
					= sar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 3]
					= sar_h;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 4]
					= dar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 5]
					= dar_h;
	}

	link_list_tt[(data_element_count - 1) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (RIE | LIE | CB);
	link_list_tt[(data_element_count / 2) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (RIE | LIE | CB);

	link_list_tt[count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH] = (LLP | TCB);
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 1] = 0;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 2] = ll_low;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 3] = ll_hi;

	__flush_dcache_area(link_list_tt, sizeof(u32)*count);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
				DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF,
				(1 << (ch_num + WR_ABORT_INT_MASK_SHIFT))
				| (1 << (ch_num + WR_DONE_INT_MASK_SHIFT)));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DONE_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DONE_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ABORT_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ABORT_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH01_IMWR_DATA_OFF,
				0x00010000);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH23_IMWR_DATA_OFF,
				0x00030002);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH45_IMWR_DATA_OFF,
				0x00050004);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_CH67_IMWR_DATA_OFF,
				0x00070006);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF,
				(1 << (ch_num + WR_CHANNEL_LLLAIE_SHIFT))
				| (1 << (ch_num + WR_CHANNEL_LLRAIE_SHIFT)));

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_WRCH, (CCS | LLE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_WRCH, ll_low);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_WRCH, ll_hi);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DOORBELL_OFF, ch_num);
}

void dma_read_lie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
					u32 dar_l, u32 dar_h, u32 size)
{
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
				DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, LIE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL2_OFF_RDCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_TRANSFER_SIZE_OFF_RDCH, size);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_LOW_OFF_RDCH, sar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_HIGH_OFF_RDCH, sar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_LOW_OFF_RDCH, dar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_HIGH_OFF_RDCH, dar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DOORBELL_OFF, ch_num);
}

void ll_dma_read_lie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
			u32 dar_l, u32 dar_h, u32 entry_size, u32 total_size)
{
	u32 count = ((total_size / entry_size) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH) +
			LL_DMA_LINK_ELEMENT_DWORD_LENGTH;
	u32 data_element_count = (total_size / entry_size);
	u32 *link_list_tt = NULL;
	u32 i = 0;
	u64 addr = 0;
	u32 ll_low = 0;
	u32 ll_hi = 0;

	link_list_tt = (u32 *)__get_dma_pages(GFP_KERNEL,
				get_order(sizeof(u32)*count));
	if (link_list_tt == NULL) {
		dev_info(pci->dev, "malloc %d fail\r\n", count);
		return;
	}

	addr = virt_to_phys(link_list_tt);
	ll_low = lower_32_bits(addr);
	ll_hi = upper_32_bits(addr);

	for (i = 0; i < data_element_count; i++) {
		link_list_tt[i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = CB;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 1]
					= entry_size;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 2]
					= sar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 3]
					= sar_h;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 4]
					= dar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 5]
					= dar_h;
	}

	link_list_tt[(data_element_count - 1) *
				LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);
	link_list_tt[(data_element_count / 2) *
				LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);

	link_list_tt[count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH] = (LLP | TCB);
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 1] = 0;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 2] = ll_low;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 3] = ll_hi;

	__flush_dcache_area(link_list_tt, sizeof(u32)*count);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
						DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				(1 << (ch_num + RD_CHANNEL_LLLAIE_SHIFT)));

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, (CCS | LLE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, ll_low);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, ll_hi);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DOORBELL_OFF, ch_num);
}

void dma_read_rie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
				u32 dar_l, u32 dar_h, u32 size)
{
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
						DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF,
			(1 << (ch_num + RD_ABORT_INT_MASK_SHIFT)) |
			(1 << (ch_num + RD_DONE_INT_MASK_SHIFT)));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DONE_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DONE_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ABORT_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ABORT_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH01_IMWR_DATA_OFF,
				0x00010000);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH23_IMWR_DATA_OFF,
				0x00030002);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH45_IMWR_DATA_OFF,
				0x00050004);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH67_IMWR_DATA_OFF,
				0x00070006);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, (RIE | LIE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL2_OFF_RDCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_TRANSFER_SIZE_OFF_RDCH, size);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_LOW_OFF_RDCH, sar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_HIGH_OFF_RDCH, sar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_LOW_OFF_RDCH, dar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_HIGH_OFF_RDCH, dar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DOORBELL_OFF, ch_num);
}

void ll_dma_read_rie(struct dw_pcie *pci, u32 ch_num, u32 sar_l, u32 sar_h,
			u32 dar_l, u32 dar_h, u32 entry_size, u32 total_size)
{
	u32 count = ((total_size / entry_size) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH) +
			LL_DMA_LINK_ELEMENT_DWORD_LENGTH;
	u32 data_element_count = (total_size / entry_size);
	u32 *link_list_tt = NULL;
	u32 i = 0;
	u64 addr = 0;
	u32 ll_low = 0;
	u32 ll_hi = 0;

	link_list_tt = (u32 *)__get_dma_pages(GFP_KERNEL,
				get_order(sizeof(u32)*count));
	if (link_list_tt == NULL) {
		dev_info(pci->dev, "malloc %d fail\r\n", count);
		return;
	}

	addr = virt_to_phys(link_list_tt);
	ll_low = lower_32_bits(addr);
	ll_hi = upper_32_bits(addr);

	for (i = 0; i < data_element_count; i++) {
		link_list_tt[i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = CB;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 1]
				= entry_size;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 2]
				= sar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 3]
				= sar_h;
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 4]
				= dar_l + (entry_size * i);
		link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 5]
				= dar_h;
	}

	link_list_tt[(data_element_count - 1) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (RIE | LIE | CB);
	link_list_tt[(data_element_count / 2) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (RIE | LIE | CB);

	link_list_tt[count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH] = (LLP | TCB);
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 1] = 0;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 2] = ll_low;
	link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 3] = ll_hi;

	__flush_dcache_area(link_list_tt, sizeof(u32)*count);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
				DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF,
				(1 << (ch_num + RD_ABORT_INT_MASK_SHIFT)) |
				(1 << (ch_num + RD_DONE_INT_MASK_SHIFT)));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DONE_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DONE_IMWR_HIGH_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ABORT_IMWR_LOW_OFF,
				GIC_ITS_ADDR);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ABORT_IMWR_HIGH_OFF,
				0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH01_IMWR_DATA_OFF,
				0x00010000);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH23_IMWR_DATA_OFF,
				0x00030002);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH45_IMWR_DATA_OFF,
				0x00050004);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_CH67_IMWR_DATA_OFF,
				0x00070006);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				(1 << (ch_num + RD_CHANNEL_LLLAIE_SHIFT)) |
				(1 << (ch_num + RD_CHANNEL_LLRAIE_SHIFT)));

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, (CCS | LLE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, ll_low);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, ll_hi);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DOORBELL_OFF, ch_num);
}

void dma_write_read_sim_lie(struct dw_pcie *pci, u32 ch_num,
		u32 w_sar_l, u32 w_sar_h, u32 w_dar_l, u32 w_dar_h,
		u32 r_sar_l, u32 r_sar_h, u32 r_dar_l, u32 r_dar_h,
		u32 size)
{
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
						DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_CH_CONTROL1_OFF_WRCH, LIE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_CH_CONTROL2_OFF_WRCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_TRANSFER_SIZE_OFF_WRCH, size);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_SAR_LOW_OFF_WRCH, w_sar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_SAR_HIGH_OFF_WRCH, w_sar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_DAR_LOW_OFF_WRCH, w_dar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_DAR_HIGH_OFF_WRCH, w_dar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_LLP_LOW_OFF_WRCH, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200)
				+ DMA_LLP_HIGH_OFF_WRCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
					DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, LIE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL2_OFF_RDCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_TRANSFER_SIZE_OFF_RDCH, size);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_LOW_OFF_RDCH, r_sar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_SAR_HIGH_OFF_RDCH, r_sar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_LOW_OFF_RDCH, r_dar_l);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_DAR_HIGH_OFF_RDCH, r_dar_h);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, 0);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DOORBELL_OFF, ch_num);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DOORBELL_OFF,
						ch_num);
}

void ll_dma_write_read_sim_lie(struct dw_pcie *pci, u32 ch_num,
	u32 w_sar_l, u32 w_sar_h, u32 w_dar_l, u32 w_dar_h,
	u32 r_sar_l, u32 r_sar_h, u32 r_dar_l, u32 r_dar_h,
	u32 entry_size, u32 total_size)
{
	u32 count = ((total_size / entry_size) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH)
			+ LL_DMA_LINK_ELEMENT_DWORD_LENGTH;
	u32 data_element_count = (total_size / entry_size);
	u32 *w_link_list_tt = NULL;
	u32 *r_link_list_tt = NULL;
	u32 i = 0;
	u64 addr = 0;
	u32 w_ll_low = 0;
	u32 w_ll_hi = 0;
	u32 r_ll_low = 0;
	u32 r_ll_hi = 0;

	w_link_list_tt = (u32 *)__get_dma_pages(GFP_KERNEL,
				get_order(sizeof(u32)*count));
	if (w_link_list_tt == NULL) {
		dev_info(pci->dev, "malloc %d fail\r\n", count);
		return;
	}

	addr = virt_to_phys(w_link_list_tt);
	w_ll_low = lower_32_bits(addr);
	w_ll_hi = upper_32_bits(addr);

	r_link_list_tt = (u32 *)__get_dma_pages(GFP_KERNEL,
				get_order(sizeof(u32)*count));
	if (r_link_list_tt == NULL) {
		dev_info(pci->dev, "malloc %d fail\r\n", count);
		return;
	}

	addr = virt_to_phys(r_link_list_tt);
	r_ll_low = lower_32_bits(addr);
	r_ll_hi = upper_32_bits(addr);

	for (i = 0; i < data_element_count; i++) {
		w_link_list_tt[i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = CB;
		w_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 1]
						= entry_size;
		w_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 2]
						= w_sar_l + (entry_size * i);
		w_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 3]
						= w_sar_h;
		w_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 4]
						= w_dar_l + (entry_size * i);
		w_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 5]
						= w_dar_h;
	}

	w_link_list_tt[(data_element_count - 1) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);
	w_link_list_tt[(data_element_count / 2) *
			LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);

	w_link_list_tt[count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH] = (LLP | TCB);
	w_link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 1] = 0;
	w_link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 2] =
							w_ll_low;
	w_link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 3] =
							w_ll_hi;

	__flush_dcache_area(w_link_list_tt, sizeof(u32)*count);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
						DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF,
				(1 << (ch_num + WR_CHANNEL_LLLAIE_SHIFT)));

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_WRCH, (CCS | LLE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_WRCH, w_ll_low);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_WRCH, w_ll_hi);


	for (i = 0; i < data_element_count; i++) {
		r_link_list_tt[i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = CB;
		r_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 1]
					= entry_size;
		r_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 2]
					= r_sar_l + (entry_size * i);
		r_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 3]
					= r_sar_h;
		r_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 4]
					= r_dar_l + (entry_size * i);
		r_link_list_tt[(i * LL_DMA_DATA_ELEMENT_DWORD_LENGTH) + 5]
					= r_dar_h;
	}

	r_link_list_tt[(data_element_count - 1) *
				LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);
	r_link_list_tt[(data_element_count / 2) *
				LL_DMA_DATA_ELEMENT_DWORD_LENGTH] = (LIE | CB);

	r_link_list_tt[count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH] = (LLP | TCB);
	r_link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 1] = 0;
	r_link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 2] =
								r_ll_low;
	r_link_list_tt[(count - LL_DMA_LINK_ELEMENT_DWORD_LENGTH) + 3] =
								r_ll_hi;

	__flush_dcache_area(r_link_list_tt, sizeof(u32)*count);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
						DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				(1 << (ch_num + RD_CHANNEL_LLLAIE_SHIFT)));

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, (CCS | LLE));
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, r_ll_low);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (ch_num * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, r_ll_hi);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_DOORBELL_OFF, ch_num);

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_DOORBELL_OFF,
				ch_num);
}

void dma_read_multi_arb_lie(struct dw_pcie *pci,
	u32 sar_l, u32 sar_h, u32 dar_l, u32 dar_h, u32 size)
{
	u32 rd_ch_num = 0;
	u32 ch_size = 0;
	u32 i = 0;
	u32 val = 0;

	val = dw_pcie_readl_dbi(pci, PCIE_DMA_OFFSET + DMA_CTRL_OFF);

	rd_ch_num = (val >> NUM_DMA_RD_CHAN_SHIFT) & NUM_DMA_RD_CHAN_MASK;

	ch_size = size / rd_ch_num;

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_ENGINE_EN_OFF,
				DMA_READ_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_READ_LINKED_LIST_ERR_EN_OFF,
				0);

	for (i = 0; i < rd_ch_num; i++) {
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_CH_CONTROL1_OFF_RDCH, LIE);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_CH_CONTROL2_OFF_RDCH, 0);

		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_TRANSFER_SIZE_OFF_RDCH, ch_size);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_SAR_LOW_OFF_RDCH, sar_l + (i * ch_size));
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_SAR_HIGH_OFF_RDCH, sar_h);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_DAR_LOW_OFF_RDCH, dar_l + (i * ch_size));
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_DAR_HIGH_OFF_RDCH, dar_h);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_LLP_LOW_OFF_RDCH, 0);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_LLP_HIGH_OFF_RDCH, 0);
	}

	/* 0x21084---4 4 4 4*/
	/* 0x31462---6 5 3 2*/
	/* 0x41042---8 4 2 2 */
	/* 0x84210---16 16 16 16*/
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
		DMA_READ_CHANNEL_ARB_WEIGHT_LOW_OFF, 0x84210);

	dev_info(pci->dev, "beging dma_read_test\r\n");

	for (i = 0; i < rd_ch_num; i++) {
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
			DMA_READ_DOORBELL_OFF, i);
	}
}

void dma_write_multi_arb_lie(struct dw_pcie *pci,
	u32 sar_l, u32 sar_h, u32 dar_l, u32 dar_h, u32 size)
{
	u32 wr_ch_num = 0;
	u32 ch_size = 0;
	u32 i = 0;
	u32 val = 0;

	val = dw_pcie_readl_dbi(pci, PCIE_DMA_OFFSET + DMA_CTRL_OFF);

	wr_ch_num = (val >> NUM_DMA_WR_CHAN_SHIFT) & NUM_DMA_WR_CHAN_MASK;

	ch_size = size / wr_ch_num;

	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_ENGINE_EN_OFF,
				DMA_WRITE_ENGINE);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + DMA_WRITE_INT_MASK_OFF, 0);
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
				DMA_WRITE_LINKED_LIST_ERR_EN_OFF, 0);

	for (i = 0; i < wr_ch_num; i++) {
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_CH_CONTROL1_OFF_WRCH, LIE);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_CH_CONTROL2_OFF_WRCH, 0);

		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_TRANSFER_SIZE_OFF_WRCH, ch_size);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_SAR_LOW_OFF_WRCH, sar_l + (i * ch_size));
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_SAR_HIGH_OFF_WRCH, sar_h);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_DAR_LOW_OFF_WRCH, dar_l + (i * ch_size));
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_DAR_HIGH_OFF_WRCH, dar_h);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_LLP_LOW_OFF_WRCH, 0);
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET + (i * 0x200) +
				DMA_LLP_HIGH_OFF_WRCH, 0);
	}

	/* 0x21084---4 4 4 4*/
	/* 0x31462---6 5 3 2*/
	/* 0x41042---8 4 2 2 */
	/* 0x84210---16 16 16 16*/
	dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
		DMA_WRITE_CHANNEL_ARB_WEIGHT_LOW_OFF, 0x84210);

	dev_info(pci->dev, "beging dma_write_test\r\n");

	for (i = 0; i < wr_ch_num; i++) {
		dw_pcie_writel_dbi(pci, PCIE_DMA_OFFSET +
			DMA_WRITE_DOORBELL_OFF, i);
	}
}

void  dma_test(struct dw_pcie *pci, int num, struct dma_parm *parm)
{
	switch (num) {
	case 1:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x80, 0x1000000);
		break;

	case 2:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x100, 0x2000000);
		break;

	case 3:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x200, 0x4000000);
		break;

	case 4:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x400, 0x8000000);
		break;

	case 5:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x800, 0x10000000);
		break;

	case 6:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x1000, 0x10000000);
		break;

	case 7:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x2000, 0x10000000);
		break;

	case 8:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x4000, 0x10000000);
		break;

	case 9:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x100000, 0x10000000);
		break;

	case 10:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x200000, 0x10000000);
		break;

	case 11:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x400000, 0x10000000);
		break;

	case 12:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x800000, 0x10000000);
		break;

	case 13:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x1000000, 0x10000000);
		break;

	case 14:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x2000000, 0x10000000);
		break;

	case 15:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x4000000, 0x10000000);
		break;

	case 16:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x8000000, 0x10000000);
		break;

	case 17:
		ll_dma_read_lie(pci, 0, lower_32_bits(parm->rd_src),
			upper_32_bits(parm->rd_src), lower_32_bits(parm->rd_dst),
			upper_32_bits(parm->rd_dst), 0x10000000, 0x10000000);
		break;

	case 18:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x80, 0x1000000);
		break;

	case 19:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x100, 0x2000000);
		break;

	case 20:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x200, 0x4000000);
		break;

	case 21:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x400, 0x8000000);
		break;

	case 22:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x800, 0x10000000);
		break;

	case 23:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x1000, 0x10000000);
		break;

	case 24:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x2000, 0x10000000);
		break;

	case 25:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x4000, 0x10000000);
		break;

	case 26:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x100000, 0x10000000);
		break;

	case 27:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x200000, 0x10000000);
		break;

	case 28:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x400000, 0x10000000);
		break;

	case 29:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x800000, 0x10000000);
		break;

	case 30:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x1000000, 0x10000000);
		break;

	case 31:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x2000000, 0x10000000);
		break;

	case 32:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x4000000, 0x10000000);
		break;

	case 33:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x8000000, 0x10000000);
		break;

	case 34:
		ll_dma_write_lie(pci, 0, lower_32_bits(parm->wr_src),
			upper_32_bits(parm->wr_src), lower_32_bits(parm->wr_dst),
			upper_32_bits(parm->wr_dst), 0x10000000, 0x10000000);
		break;

	case 35:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x80, 0x1000000);
		break;

	case 36:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x100, 0x2000000);
		break;

	case 37:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x200, 0x4000000);
		break;

	case 38:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x400, 0x8000000);
		break;

	case 39:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x800, 0x10000000);
		break;

	case 40:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x1000, 0x10000000);
		break;

	case 41:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x2000, 0x10000000);
		break;

	case 42:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x4000, 0x10000000);
		break;

	case 43:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x100000, 0x10000000);
		break;

	case 44:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x200000, 0x10000000);
		break;

	case 45:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x400000, 0x10000000);
		break;

	case 46:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x800000, 0x10000000);
		break;

	case 47:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x1000000, 0x10000000);
		break;

	case 48:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x2000000, 0x10000000);
		break;

	case 49:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x4000000, 0x10000000);
		break;

	case 50:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x8000000, 0x10000000);
		break;

	case 51:
	default:
		ll_dma_write_read_sim_lie(pci, 0, lower_32_bits(parm->wr_sim_src),
			upper_32_bits(parm->wr_sim_src), lower_32_bits(parm->wr_sim_dst),
			upper_32_bits(parm->wr_sim_dst), lower_32_bits(parm->rd_sim_src),
			upper_32_bits(parm->rd_sim_src), lower_32_bits(parm->rd_sim_dst),
			upper_32_bits(parm->rd_sim_dst), 0x10000000, 0x10000000);
		break;
	}
}

#ifdef CONFIG_PCIE_MOLCHIP_RC_DE_EMPHASIS
/*level: 0:-6.0db; 1: -3.5db; 2: no de-emphasis*/
void adjust_x2_phy_de_emphasis(struct mc_common *com, u8 level)
{
	u32 val = 0;

	val = mc_readl_ahb_core(com, PCIE_PHY_X2_DEBUG_CTRL1);
	val &= ~(PIPE_TXDEEMPH_I_L0_DEBUG_MASK <<
			PIPE_TXDEEMPH_I_L0_DEBUG_SHIFT);
	val |= (level << PIPE_TXDEEMPH_I_L0_DEBUG_SHIFT);
	val |= DEBUG_SEL_PIPE_TXDEEMPH_I_L0;
	mc_writel_ahb_core(com, PCIE_PHY_X2_DEBUG_CTRL1, val);

	val = mc_readl_ahb_core(com, PCIE_PHY_X2_DEBUG_CTRL2);
	val &= ~(PIPE_TXDEEMPH_I_L1_DEBUG_MASK <<
			PIPE_TXDEEMPH_I_L1_DEBUG_SHIFT);
	val |= (level << PIPE_TXDEEMPH_I_L1_DEBUG_SHIFT);
	val |= DEBUG_SEL_PIPE_TXDEEMPH_I_L1;
	mc_writel_ahb_core(com, PCIE_PHY_X2_DEBUG_CTRL2, val);
}
#endif

#ifdef CONFIG_PCIE_MOLCHIP_RC_SOC_REF_CLK_SSC_ENABLE
void x2_phy_ssc_enable(struct mc_common *com)
{
	u32 val = 0;

	val = mc_readl_phy_cfg(com, PHY_LANE01_DACPD_DSMPD);
	val &= (u32)(~(DACPD));
	val &= (u32)(~(DSMPD));
	mc_writel_phy_cfg(com, PHY_LANE01_DACPD_DSMPD, val);

	val = mc_readl_phy_cfg(com, PHY_LANE01_DIVVAL);
	val &= (u32)(~(DIVVAL_MASK << DIVVAL_SHIFT));
	val |= (DIVVAL_VAL << DIVVAL_SHIFT);
	mc_writel_phy_cfg(com, PHY_LANE01_DIVVAL, val);

	val = mc_readl_phy_cfg(com, PHY_LANE01_SPREAD);
	val &= (u32)(~(SPREAD_MASK << SPREAD_SHIFT));
	val |= (SPREAD_3900PPM << SPREAD_SHIFT);
	mc_writel_phy_cfg(com, PHY_LANE01_SPREAD, val);

	val = mc_readl_phy_cfg(com, PHY_LANE01_DISABLE_SSCG);
	val &= (u32)(~(DISABLE_SSCG));
	val |= DOWNSPREAD;
	mc_writel_phy_cfg(com, PHY_LANE01_DISABLE_SSCG, val);

	val = mc_readl_phy_cfg(com, LANE01_PLLRI_PCIE_100M_SEL_G3);
	val &= (u32)(~(PCIE_100M_SEL_G3_MASK << PCIE_100M_SEL_G3_SHIFT));
	val |= (PCIE_100M_SEL_G3_VAL << PCIE_100M_SEL_G3_SHIFT);
	mc_writel_phy_cfg(com, LANE01_PLLRI_PCIE_100M_SEL_G3, val);
}
#endif

