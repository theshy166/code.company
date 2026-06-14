/*
 * Molchip USB 2.0 PHY Driver
 *
 * Copyright (C) 2019 Jason Qin <jason.qin@molchip.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/mini_clock/clock.h>

#include "phy-molchip-usb.h"

#define USB_VBUS_CFG_REG_OFF		0x1c
#define USB_UTMIOTG_IDDIG_REG_OFF	0x0
#define USB20_PHY_CFG_OFF			0x3c
#define USB20_PHY_EYEAMP_OFF		0x20


#define USB_VBUS_CFG_REG_BIT_OFF		2
#define USB_UTMIOTG_IDDIG_REG_BIT_OFF	5
#define USB20_PHY_CFG_REG_BIT_OFF		5
#define USB20_PHY_EYEAMP_BIT_OFF		12



#define USB_VBUS_CFG_REG_MASK			0x3
#define USB_UTMIOTG_IDDIG_REG_MASK		0x3
#define USB20_PHY_CFG_REG_MASK			0x3
#define USB20_PHY_EYEAMP_MASK			0x3


#define USB_VBUS_CFG_REG_VAL			0x3
#define USB_UTMIOTG_IDDIG_REG_VAL		0x3
#define USB20_PHY_EYEAMP_VAL			0x3

#define USB20_PHY_CFG_SUSPEND_VAL		0x2
#define USB20_PHY_CFG_RESUME_VAL		0x1


static int DRV_usb_Suspend(struct device *dev)
{
	int rtn = 0;
	struct molchip_usb_phy* usb_phy = (struct molchip_usb_phy* )dev_get_drvdata(dev);

	regmap_update_bits(usb_phy->usb2_apb_cfg, USB_VBUS_CFG_REG_OFF, (USB_VBUS_CFG_REG_MASK << USB_VBUS_CFG_REG_BIT_OFF),
						0<<USB_VBUS_CFG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB_UTMIOTG_IDDIG_REG_OFF, (USB_UTMIOTG_IDDIG_REG_MASK << USB_UTMIOTG_IDDIG_REG_BIT_OFF),
						0<<USB_UTMIOTG_IDDIG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB20_PHY_CFG_OFF, (USB20_PHY_CFG_REG_MASK << USB20_PHY_CFG_REG_BIT_OFF),
						USB20_PHY_CFG_SUSPEND_VAL<<USB20_PHY_CFG_REG_BIT_OFF);
	clk_mini_disable(CKG_APB_USB2_EN);
	clk_mini_disable(CKG_AHB_USB2_EN);

	printk("############## %s################\n",__func__);

	return rtn;
}

static int DRV_usb_Resume(struct device *dev)
{
	int rtn = 0;
	struct molchip_usb_phy* usb_phy = (struct molchip_usb_phy* )dev_get_drvdata(dev);

	clk_mini_enable(CKG_APB_USB2_EN);
	clk_mini_enable(CKG_AHB_USB2_EN);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB_VBUS_CFG_REG_OFF, (USB_VBUS_CFG_REG_MASK << USB_VBUS_CFG_REG_BIT_OFF),
						USB_VBUS_CFG_REG_VAL<<USB_VBUS_CFG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB_UTMIOTG_IDDIG_REG_OFF, (USB_UTMIOTG_IDDIG_REG_MASK << USB_UTMIOTG_IDDIG_REG_BIT_OFF),
						USB_UTMIOTG_IDDIG_REG_VAL<<USB_UTMIOTG_IDDIG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB20_PHY_CFG_OFF, (USB20_PHY_CFG_REG_MASK << USB20_PHY_CFG_REG_BIT_OFF),
						USB20_PHY_CFG_RESUME_VAL<<USB20_PHY_CFG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB20_PHY_EYEAMP_OFF, (USB20_PHY_EYEAMP_MASK << USB20_PHY_EYEAMP_BIT_OFF), 
						USB20_PHY_EYEAMP_VAL<< USB20_PHY_EYEAMP_BIT_OFF);

	printk("############## %s################\n",__func__);

	return rtn;
}

static int snps_usb2_phy_init(struct phy *phy)
{

	printk("%s\n",__func__);
	return 0;
}

static int snps_usb2_phy_exit(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int snps_usb2_phy_power_on(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int snps_usb2_phy_power_off(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int snps_usb2_phy_set_mode(struct phy *phy, enum phy_mode mode,int submode)
{
	printk("%s\n",__func__);

	return 0;
}

static int snps_usb2_phy_reset(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static const struct phy_ops ops = {
	.init = snps_usb2_phy_init,
	.exit = snps_usb2_phy_exit,
	.power_on = snps_usb2_phy_power_on,
	.power_off = snps_usb2_phy_power_off,
	.set_mode = snps_usb2_phy_set_mode,
	.owner = THIS_MODULE,
	.reset = snps_usb2_phy_reset,
};

static int snps_usb2_phy_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct molchip_usb_phy *usb_phy;
	struct phy_provider *phy_provider;

	usb_phy = devm_kzalloc(&pdev->dev, sizeof(*usb_phy), GFP_KERNEL);
	if (!usb_phy)
		return -ENOMEM;

	usb_phy->dev = dev;

	printk("## %s\n",__func__);
	spin_lock_init(&usb_phy->lock);
	usb_phy->usb2_apb_cfg = syscon_regmap_lookup_by_phandle(dev->of_node,
						    "molchip,usb2-apb-cfg");
	if (IS_ERR(usb_phy->usb2_apb_cfg))
		return PTR_ERR(usb_phy->usb2_apb_cfg);

	usb_phy->phy = devm_phy_create(dev, NULL, &ops);
	if (IS_ERR(usb_phy->phy))
		return PTR_ERR(usb_phy->phy);

	phy_set_drvdata(usb_phy->phy, usb_phy);
	platform_set_drvdata(pdev, usb_phy);

	phy_provider = devm_of_phy_provider_register(dev, of_phy_simple_xlate);

	usb_phy->c_phy_id = MOL_PHY_ID_NUM;

	clk_mini_enable(CKG_APB_USB2_EN);
	clk_mini_enable(CKG_AHB_USB2_EN);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB_VBUS_CFG_REG_OFF, (USB_VBUS_CFG_REG_MASK << USB_VBUS_CFG_REG_BIT_OFF), USB_VBUS_CFG_REG_VAL<< USB_VBUS_CFG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB_UTMIOTG_IDDIG_REG_OFF, (USB_UTMIOTG_IDDIG_REG_MASK << USB_UTMIOTG_IDDIG_REG_BIT_OFF), USB_UTMIOTG_IDDIG_REG_VAL<< USB_UTMIOTG_IDDIG_REG_BIT_OFF);
	regmap_update_bits(usb_phy->usb2_apb_cfg, USB20_PHY_EYEAMP_OFF, (USB20_PHY_EYEAMP_MASK << USB20_PHY_EYEAMP_BIT_OFF), USB20_PHY_EYEAMP_VAL<< USB20_PHY_EYEAMP_BIT_OFF);

	return PTR_ERR_OR_ZERO(phy_provider);

}

static const struct of_device_id snps_usb2_id_table[] = {
	{ .compatible = "snps,usb2-phy", },
	{},
};

MODULE_DEVICE_TABLE(of, snps_usb2_id_table);

static const struct dev_pm_ops usb_pm_ops = {
	.suspend = DRV_usb_Suspend,
	.resume = DRV_usb_Resume,
};

static struct platform_driver snps_usb2_phy_driver = {
	.probe = snps_usb2_phy_probe,
	.driver = {
		   .name = "snps_usb2_phy",
		   	.pm = &usb_pm_ops,
		   .of_match_table = snps_usb2_id_table,
		   },
};

module_platform_driver(snps_usb2_phy_driver);

MODULE_LICENSE("GPL v2");
