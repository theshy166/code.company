/*
 * MOLCHIP USB PHY Support
 *
 * (C) Copyright 2019
 * MOLCHIP, <www.molchip.com>
 *
 * Author: Jason Qin <jason.qin@molchip.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <asm/io.h>
#include <usb.h>
#include <asm/arch/hardware.h>

struct molchip_usb2_phy {
	void *base;
	void *ahb;
	volatile u32 *apb;
	u32 c_phy_id;
	u32 c_soc_id;
};

enum phy_mode {
	PHY_MODE_INVALID,
	PHY_MODE_USB_HOST,
	PHY_MODE_USB_DEVICE,
	PHY_MODE_USB_OTG,
};

#define REG_APB_ECO_CHIP_ID 	0x10
#define EPHY_ECO_CHIP_ID 0x38b55000


#define REG_AP_USB0_BIST_CTRL 0x48
#define REG_AP_USB1_BIST_CTRL 0x54
#define BIT_REG_USB_BIST_EN	BIT(0)


#define REG_AHB_PERI_CLK 	0x00
#define BIT_CKG_AHB_USB2_1_EN	BIT(7)
#define BIT_CKG_AHB_USB2_0_EN	BIT(6)

#define REG_AHB_PERI_SOF_RESET 	0x4
#define BIT_USB2_0_UTMI_SOFT_RESET	BIT(4)
#define BIT_USB2_1_UTMI_SOFT_RESET	BIT(5)
#define BIT_USB2_0_AHB_SOFT_RESET	BIT(6)
#define BIT_USB2_1_AHB_SOFT_RESET	BIT(7)
#define BIT_USB2_0_SOFT_RESET	BIT(2)
#define BIT_USB2_1_SOFT_RESET	BIT(3)

/* Phy config compatibility reg define */
#define REG_PHY_CFG_COMP	0x44
#define BIT_REG_PHY_CFG_7_5_MSK	GENMASK(7, 5)
#define BIT_REG_PHY_CFG_7_5(n)	(((n) << 5) & BIT_REG_PHY_CFG_7_5_MSK)
#define BIT_REG_PHY_CFG_4_2_MSK	GENMASK(4, 2)
#define BIT_REG_PHY_CFG_4_2(n)	(((n) << 2) & BIT_REG_PHY_CFG_4_2_MSK)


/* Phy config reg define */
#define REG_PHY_CFG_CTRL	0x48
#define BIT_REG_PHY_CFG_14_12_MSK	GENMASK(14, 12)
#define BIT_REG_PHY_CFG_14_12(n)	(((n) << 12) & BIT_REG_PHY_CFG_14_12_MSK)

#define REG_PHY_CFG_CTRL0	0x60
#define REG_PHY_CFG_CTRL1	0x6c
#define BIT_REG_PHY_CFG_21_19_MSK	GENMASK(21, 19)
#define BIT_REG_PHY_CFG_21_19(n)	(((n) << 19) & BIT_REG_PHY_CFG_21_19_MSK)
#define BIT_REG_PHY_CFG_17  BIT(17)
#define BIT_REG_PHY_CFG_14	BIT(14)


#define REG_PHY_DIG_CTRL0	0x64
#define REG_PHY_DIG_CTRL1	0x70
#define BIT_REG_PHY_CLK_DIG_31  BIT(31)
#define BIT_REG_PHY_DIG_CTRL_29_27_MSK	GENMASK(29, 27)
#define BIT_REG_PHY_DIG_CTRL_29_27(n)	(((n) << 27) & BIT_REG_PHY_DIG_CTRL_29_27_MSK)
#define BIT_REG_PHY_DIG_CTRL_23_18_MSK  GENMASK(23, 18)
#define BIT_REG_PHY_DIG_CTRL_23_18(n)  (((n) << 18) & BIT_REG_PHY_DIG_CTRL_23_18_MSK)
#define BIT_REG_PHY_DIG_CTRL_MODE	BIT(13)
#define BIT_REG_PHY_CLK_RECOVERY_INVERT	BIT(6)

#define BIT_REG_PHY_DIG_CLR_RX_ERR	BIT(24)



#define REG_PHY_PARA_CTRL0	0x5C
#define REG_PHY_PARA_CTRL1	0x68
#define BIT_REG_PHY_PARA_17_16_MSK	GENMASK(17, 16)
#define BIT_REG_PHY_PARA_17_16(n)	(((n) << 16) & BIT_REG_PHY_PARA_17_16_MSK)
#define BIT_REG_PHY_PARA_6_4_MSK	GENMASK(6, 4)
#define BIT_REG_PHY_PARA_6_4(n)	(((n) << 4) & BIT_REG_PHY_PARA_6_4_MSK)
#define BIT_REG_PHY_PARA_2_0_MSK	GENMASK(2, 0)
#define BIT_REG_PHY_PARA_2_0(n)	(((n) << 0) & BIT_REG_PHY_PARA_2_0_MSK)
#define BIT_REG_PHY_LS_SLOW	BIT(11)
#define BIT_REG_PHY_LS_SLOW_EN	BIT(10)

#define REG_PHY_CFG	0x74
#define BIT_REG_PHY_CFG_R45	BIT(8)

#define REG_PHY_STS0	0xe0
#define REG_PHY_STS1	0xe4
#define BIT_REG_PHY_STS_RX_ERR	GENMASK(15, 10)


#define PHY_ID(mode)	((mode >> 8) & 0xff)
#define PHY_MOD(mode)	(mode & 0xff)
#define PHY_MOD_NULL	(0xff)

struct molchip_usb2_phy mphy = {
	.base = (void *)REG_ANALOG_BASE,
	.ahb = (void *)REG_AP_AHB_BASE,
	.apb = (volatile u32 *)REG_AP_APB_BASE,
	.c_phy_id = 0,
};

struct molchip_usb2_phy mphy1 = {
	.base = (void *)REG_ANALOG_BASE,
	.ahb = (void *)REG_AP_AHB_BASE,
	.apb = (volatile u32 *)REG_AP_APB_BASE,
	.c_phy_id = 1,
};

static void molchip_usb2_phy_reset(struct molchip_usb2_phy *usb2)
{
	u32 val;

	/* utmi rest */
	val = readl(usb2->ahb + REG_AHB_PERI_SOF_RESET);
	val |= ((usb2->c_phy_id == 0) ? BIT_USB2_0_UTMI_SOFT_RESET : BIT_USB2_1_UTMI_SOFT_RESET);
	writel(val, usb2->ahb + REG_AHB_PERI_SOF_RESET);
	mdelay(1);
	val = readl(usb2->ahb + REG_AHB_PERI_SOF_RESET);
	val &= ~((usb2->c_phy_id == 0) ? BIT_USB2_0_UTMI_SOFT_RESET : BIT_USB2_1_UTMI_SOFT_RESET);
	writel(val, usb2->ahb + REG_AHB_PERI_SOF_RESET);

	/* ahb rest */
	val = readl(usb2->ahb + REG_AHB_PERI_SOF_RESET);
	val |= ((usb2->c_phy_id == 0) ? BIT_USB2_0_AHB_SOFT_RESET : BIT_USB2_1_AHB_SOFT_RESET);
	writel(val, usb2->ahb + REG_AHB_PERI_SOF_RESET);
	mdelay(1);
	val = readl(usb2->ahb + REG_AHB_PERI_SOF_RESET);
	val &= ~((usb2->c_phy_id == 0) ? BIT_USB2_0_AHB_SOFT_RESET : BIT_USB2_1_AHB_SOFT_RESET);
	writel(val, usb2->ahb + REG_AHB_PERI_SOF_RESET);

	/* soft rest */
	val = readl(usb2->ahb + REG_AHB_PERI_SOF_RESET);
	val |= ((usb2->c_phy_id == 0) ? BIT_USB2_0_SOFT_RESET : BIT_USB2_1_SOFT_RESET);
	writel(val, usb2->ahb + REG_AHB_PERI_SOF_RESET);
	mdelay(1);
	val = readl(usb2->ahb + REG_AHB_PERI_SOF_RESET);
	val &= ~((usb2->c_phy_id == 0) ? BIT_USB2_0_SOFT_RESET : BIT_USB2_1_SOFT_RESET);
	writel(val, usb2->ahb + REG_AHB_PERI_SOF_RESET);



}

static void molchip_usb2_phy_cfg(struct molchip_usb2_phy *usb2, enum phy_mode mode)
{
	u32 reg, val;

	writel(readl(usb2->base + REG_PHY_DIG_CTRL0) | BIT_REG_PHY_CFG_14, 
		usb2->base + REG_PHY_DIG_CTRL0);
	writel(readl(usb2->base + REG_PHY_DIG_CTRL1) | BIT_REG_PHY_CFG_14, 
		usb2->base + REG_PHY_DIG_CTRL1);

	val = readl(usb2->base + REG_PHY_CFG_CTRL);
	val &= ~(BIT_REG_PHY_CFG_14_12_MSK);
	val |= BIT_REG_PHY_CFG_14_12(6);
	writel(val, usb2->base + REG_PHY_CFG_CTRL);


	val = readl(usb2->base + REG_PHY_DIG_CTRL0);
	val &= ~(BIT_REG_PHY_DIG_CTRL_29_27_MSK);
	val |= BIT_REG_PHY_DIG_CTRL_29_27(2);
	writel(val, usb2->base + REG_PHY_DIG_CTRL0);
	
	val = readl(usb2->base + REG_PHY_DIG_CTRL1);
	val &= ~(BIT_REG_PHY_DIG_CTRL_29_27_MSK);
	val |= BIT_REG_PHY_DIG_CTRL_29_27(2);
	writel(val, usb2->base + REG_PHY_DIG_CTRL1);

	val = readl(usb2->base + REG_PHY_PARA_CTRL0);
	val &= ~(BIT_REG_PHY_PARA_6_4_MSK | BIT_REG_PHY_PARA_17_16_MSK | BIT_REG_PHY_PARA_2_0_MSK);
	val |= BIT_REG_PHY_PARA_6_4(2) | BIT_REG_PHY_PARA_17_16(0) | BIT_REG_PHY_PARA_2_0(1);
	writel(val, usb2->base + REG_PHY_PARA_CTRL0);


	val = readl(usb2->base + REG_PHY_PARA_CTRL0);
	val &= ~(BIT_REG_PHY_PARA_6_4_MSK | BIT_REG_PHY_PARA_17_16_MSK | BIT_REG_PHY_PARA_2_0_MSK);
	val |= BIT_REG_PHY_PARA_6_4(2) | BIT_REG_PHY_PARA_17_16(0) | BIT_REG_PHY_PARA_2_0(1);
	writel(val, usb2->base + REG_PHY_PARA_CTRL0);

	writel(readl(usb2->base + REG_PHY_DIG_CTRL0) & ~(BIT_REG_PHY_CLK_RECOVERY_INVERT), 
		usb2->base + REG_PHY_DIG_CTRL0);

	writel(readl(usb2->base + REG_PHY_DIG_CTRL1) & ~(BIT_REG_PHY_CLK_RECOVERY_INVERT), 
		usb2->base + REG_PHY_DIG_CTRL1);

	writel(readl(usb2->base + REG_PHY_CFG_CTRL0) | BIT_REG_PHY_CFG_17, 
		usb2->base + REG_PHY_CFG_CTRL0);

	writel(readl(usb2->base + REG_PHY_CFG_CTRL1) | BIT_REG_PHY_CFG_17, 
		usb2->base + REG_PHY_CFG_CTRL1);

	val = readl(usb2->base + REG_PHY_DIG_CTRL0);
	val &= ~(BIT_REG_PHY_DIG_CTRL_23_18_MSK);
	val |= BIT_REG_PHY_DIG_CTRL_23_18(0xa);
	writel(val, usb2->base + REG_PHY_DIG_CTRL0);

	val = readl(usb2->base + REG_PHY_DIG_CTRL1);
	val &= ~(BIT_REG_PHY_DIG_CTRL_23_18_MSK);
	val |= BIT_REG_PHY_DIG_CTRL_23_18(0xa);
	writel(val, usb2->base + REG_PHY_DIG_CTRL1);

	val = readl(usb2->base + REG_PHY_DIG_CTRL0);
	val |= BIT_REG_PHY_CLK_DIG_31;
	writel(val, usb2->base + REG_PHY_DIG_CTRL0);

	val = readl(usb2->base + REG_PHY_DIG_CTRL1);
	val |= BIT_REG_PHY_CLK_DIG_31;
	writel(val, usb2->base + REG_PHY_DIG_CTRL1);

	val = readl(usb2->base + REG_PHY_CFG_COMP);
	val &= ~(BIT_REG_PHY_CFG_7_5_MSK);
	val &= ~(BIT_REG_PHY_CFG_4_2_MSK);
	val |= BIT_REG_PHY_CFG_7_5(1);
	val |= BIT_REG_PHY_CFG_4_2(1);
	writel(val, usb2->base + REG_PHY_CFG_COMP);

	val = readl(usb2->base + REG_PHY_CFG_CTRL0);
	val &= ~(BIT_REG_PHY_CFG_21_19_MSK);
	val |= BIT_REG_PHY_CFG_21_19(3);
	writel(val, usb2->base + REG_PHY_CFG_CTRL0);

	val = readl(usb2->base + REG_PHY_CFG_CTRL1);
	val &= ~(BIT_REG_PHY_CFG_21_19_MSK);
	val |= BIT_REG_PHY_CFG_21_19(3);
	writel(val, usb2->base + REG_PHY_CFG_CTRL1);

	val = readl(usb2->base + REG_PHY_CFG);
	val &= ~(BIT_REG_PHY_CFG_R45);
	writel(val, usb2->base + REG_PHY_CFG);

	mdelay(1);

	val = readl(usb2->base + REG_PHY_CFG);
	val |= (BIT_REG_PHY_CFG_R45);
	writel(val, usb2->base + REG_PHY_CFG);

	writel(readl(usb2->base + REG_PHY_PARA_CTRL0) | BIT_REG_PHY_LS_SLOW, 
		usb2->base + REG_PHY_PARA_CTRL0);

	writel(readl(usb2->base + REG_PHY_PARA_CTRL1) | BIT_REG_PHY_LS_SLOW, 
		usb2->base + REG_PHY_PARA_CTRL1);

	writel(readl(usb2->base + REG_PHY_PARA_CTRL0) & ~(BIT_REG_PHY_LS_SLOW_EN),
		usb2->base + REG_PHY_PARA_CTRL0);

	writel(readl(usb2->base + REG_PHY_PARA_CTRL1) & ~(BIT_REG_PHY_LS_SLOW_EN),
		usb2->base + REG_PHY_PARA_CTRL1);

	reg = (usb2->c_phy_id == 0) ? REG_PHY_DIG_CTRL0 : REG_PHY_DIG_CTRL1;
	val = readl(usb2->base + reg);
	switch (mode) {
	case PHY_MODE_USB_HOST:	/* Force VBUS valid, ID = 0 */
		val |= BIT_REG_PHY_DIG_CTRL_MODE;
		break;
	case PHY_MODE_USB_DEVICE:	/* Force VBUS valid, ID = 1 */
		val &= ~BIT_REG_PHY_DIG_CTRL_MODE;
		break;
	default:
		break;
	}
	writel(val, usb2->base + reg);

}

static void molchip_usb2_phy_en(struct molchip_usb2_phy *usb2)
{
	u32 val;

	val = readl(usb2->ahb + REG_AHB_PERI_CLK);
	val |= ((usb2->c_phy_id == 0) ? BIT_CKG_AHB_USB2_0_EN : BIT_CKG_AHB_USB2_1_EN);
	writel(val, usb2->ahb + REG_AHB_PERI_CLK);

	usb2->c_soc_id = readl(usb2->apb + REG_APB_ECO_CHIP_ID);
}

static void molchip_usb2_bist_cfg(struct molchip_usb2_phy *usb2)
{
	u32 val,reg;

	reg = ((usb2->c_phy_id == 0) ? REG_AP_USB0_BIST_CTRL : REG_AP_USB1_BIST_CTRL);
	val = readl(usb2->ahb + reg);
	val &= ~(BIT_REG_USB_BIST_EN);
	writel(val, usb2->ahb + reg);
}

void molchip_usb2_enable_phy(int index)
{
	if(0 == index){
		molchip_usb2_bist_cfg(&mphy);
		molchip_usb2_phy_cfg(&mphy, PHY_MODE_USB_HOST);
		molchip_usb2_phy_en(&mphy);
		molchip_usb2_phy_reset(&mphy);
	}else{
		molchip_usb2_bist_cfg(&mphy1);
		molchip_usb2_phy_cfg(&mphy1, PHY_MODE_USB_HOST);
		molchip_usb2_phy_en(&mphy1);
		molchip_usb2_phy_reset(&mphy1);
	}
}
