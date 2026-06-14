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

#include "phy-molchip-usb.h"
#include <linux/xc-clock-simple/clock.h>


#define USB_GBL_BASE_ADDR1	0x34c0c000
#define USB_GBL_BASE_ADDR2	0x34d0c000


static int DRV_usb_Suspend(struct device *dev)
{
	int rtn = 0;
#if 0	
	struct molchip_usb_phy* usb_phy = (struct molchip_usb_phy* )dev_get_drvdata(dev);
	if(usb_phy->usb_ref_clk){
		clk_disable_unprepare(usb_phy->usb_ref_clk);
		usb_phy->usb_ref_clk = NULL;
	}

	if(usb_phy->usb_suspend_clk){
		clk_disable_unprepare(usb_phy->usb_suspend_clk);
		usb_phy->usb_suspend_clk = NULL;
	}

	if(usb_phy->usb_axi_clk){
		clk_disable_unprepare(usb_phy->usb_axi_clk);
		usb_phy->usb_axi_clk = NULL;
	}

	if(usb_phy->usb_cfg_clk){
		clk_disable_unprepare(usb_phy->usb_cfg_clk);
		usb_phy->usb_cfg_clk = NULL;
	}
#else
	clk_mini_disable(CKG_USB32_REF_EN);
	clk_mini_disable(CKG_USB3_SUSPEND_EN);
	clk_mini_disable(CKG_AXI_USB3_EN);
	clk_mini_disable(CKG_APB_USB3_CFG_EN);

#endif

	printk("############## %s################\n",__func__);

	return rtn;
}

static int molchip_usb3_phy_init(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int molchip_usb3_phy_exit(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int molchip_usb3_phy_power_on(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int molchip_usb3_phy_power_off(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static int molchip_usb3_phy_set_mode(struct phy *phy, enum phy_mode mode,int submode)
{
	printk("%s\n",__func__);

	return 0;
}

static int molchip_usb3_phy_reset(struct phy *phy)
{
	printk("%s\n",__func__);

	return 0;
}

static const struct phy_ops ops = {
	.init = molchip_usb3_phy_init,
	.exit = molchip_usb3_phy_exit,
	.power_on = molchip_usb3_phy_power_on,
	.power_off = molchip_usb3_phy_power_off,
	.set_mode = molchip_usb3_phy_set_mode,
	.owner = THIS_MODULE,
	.reset = molchip_usb3_phy_reset,
};

static int molchip_usb3_phy_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct molchip_usb_phy *usb_phy;
	struct phy_provider *phy_provider;
	struct resource *r_mem = NULL;
	u8 val;
	void __iomem *u3_baseaddr;
	u32 reg_val;
	int ret = -1;

	usb_phy = devm_kzalloc(&pdev->dev, sizeof(*usb_phy), GFP_KERNEL);
	if (!usb_phy)
		return -ENOMEM;

	usb_phy->dev = dev;

#if 0	
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem) {
		dev_err(dev, "no IO 0 resource defined.\n");
		return -ENXIO;
	}

	usb_phy->baseaddr = devm_ioremap_resource(dev, r_mem);
	if (IS_ERR(usb_phy->baseaddr)) {
		dev_err(dev, "failed to map baseaddress 0.\n");
		return PTR_ERR(usb_phy->baseaddr);
	}
#endif

	
	usb_phy->cen_apb = syscon_regmap_lookup_by_phandle(dev->of_node,
						    "molchip,syscon-clk-gate");
	if (IS_ERR(usb_phy->cen_apb))
		return PTR_ERR(usb_phy->cen_apb);

	usb_phy->sys_apb = syscon_regmap_lookup_by_phandle(dev->of_node,
						    "molchip,syscon-usb3-cfg");
	if (IS_ERR(usb_phy->sys_apb))
		return PTR_ERR(usb_phy->sys_apb);
#if 0
	regmap_update_bits(usb_phy->cen_apb, 0x04, (0x1 << 0), (0x1 << 0)); 
	regmap_update_bits(usb_phy->cen_apb, 0x04, (0x1 << 1), (0x1 << 1)); 
	regmap_update_bits(usb_phy->cen_apb, 0x04, (0x1 << 7), (0x1 << 7)); 
	regmap_update_bits(usb_phy->cen_apb, 0x04, (0x1 << 11), (0x1 << 11));
#else
/*	usb_phy->usb_cfg_clk = devm_clk_get(dev,"ckg_apb_usb3_cfg_en");
	if(IS_ERR(usb_phy->usb_cfg_clk))
		return PTR_ERR(usb_phy->usb_cfg_clk);

	ret = clk_prepare_enable(usb_phy->usb_cfg_clk);
	if(ret)
		return ret;

	usb_phy->usb_axi_clk = devm_clk_get(dev,"ckg_axi_usb3_en");
	if(IS_ERR(usb_phy->usb_axi_clk))
		return PTR_ERR(usb_phy->usb_axi_clk);

	ret = clk_prepare_enable(usb_phy->usb_axi_clk);
	if(ret)
		return ret;

	
	usb_phy->usb_suspend_clk_sel = devm_clk_get(dev,"ckg_usb3_suspend_sel");
	if(IS_ERR(usb_phy->usb_suspend_clk_sel))
		return PTR_ERR(usb_phy->usb_suspend_clk_sel);

	ret = clk_set_rate(usb_phy->usb_suspend_clk_sel,100000000); //100M
	if(ret)
		return ret;

	usb_phy->usb_suspend_clk = devm_clk_get(dev,"ckg_usb3_suspend_en");
	if(IS_ERR(usb_phy->usb_suspend_clk))
		return PTR_ERR(usb_phy->usb_suspend_clk);

	ret = clk_prepare_enable(usb_phy->usb_suspend_clk);
	if(ret)
		return ret;

	usb_phy->usb_ref_clk_sel = devm_clk_get(dev,"ckg_usb32_ref_sel");
	if(IS_ERR(usb_phy->usb_ref_clk_sel))
		return PTR_ERR(usb_phy->usb_ref_clk_sel);

	ret = clk_set_rate(usb_phy->usb_ref_clk_sel,100000000); //100M
	if(ret)
		return ret;

	usb_phy->usb_ref_clk = devm_clk_get(dev,"ckg_usb32_ref_en");
	if(IS_ERR(usb_phy->usb_ref_clk))
		return PTR_ERR(usb_phy->usb_ref_clk);

	ret = clk_prepare_enable(usb_phy->usb_ref_clk);
	if(ret)
		return ret;
*/
	clk_mini_enable(CKG_APB_USB3_CFG_EN);
	clk_mini_enable(CKG_AXI_USB3_EN);
	clk_mini_set_rate(CKG_MUX_USB3_SUSPEND,100000000);
	clk_mini_enable(CKG_USB3_SUSPEND_EN);
	clk_mini_set_rate(CKG_MUX_USB32_REF,100000000);
	clk_mini_enable(CKG_USB32_REF_EN);


#endif

	regmap_update_bits(usb_phy->sys_apb, 0x50, (0x1 << 3), (0x1 << 3)); 
	regmap_update_bits(usb_phy->sys_apb, 0x50, (0x7f << 6), (0x7f << 6)); 
	regmap_update_bits(usb_phy->sys_apb, 0x50, (0x3f << 13), (0x1c << 13)); 	
	regmap_update_bits(usb_phy->sys_apb, 0x50, (0x3f << 19), (0x1c << 19)); 

	regmap_update_bits(usb_phy->sys_apb, 0x4c, (0x1f << 11), (0x9 << 11)); 
	regmap_update_bits(usb_phy->sys_apb, 0x4c, (0x7 << 16), (0x5 << 16)); 

	regmap_update_bits(usb_phy->sys_apb, 0x54, (0x7 << 12), (0x6 << 12)); 	
	regmap_update_bits(usb_phy->sys_apb, 0x54, (0x1 << 16), (0x0 << 16)); 	
	regmap_update_bits(usb_phy->sys_apb, 0x54, (0x1 << 30), (0x1 << 30)); 

	regmap_update_bits(usb_phy->sys_apb, 0x48, (0x3 << 24), (0x3 << 24)); 	
	regmap_update_bits(usb_phy->sys_apb, 0x48, (0x3 << 22), (0x3 << 22)); 	
	regmap_update_bits(usb_phy->sys_apb, 0x48, (0x3 << 17), (0x3 << 17)); 
	regmap_update_bits(usb_phy->sys_apb, 0x48, (0xf << 13), (0x8 << 13)); 

	usb_phy->phy = devm_phy_create(dev, NULL, &ops);
	if (IS_ERR(usb_phy->phy))
		return PTR_ERR(usb_phy->phy);

	phy_set_drvdata(usb_phy->phy, usb_phy);
	platform_set_drvdata(pdev, usb_phy);

	phy_provider = devm_of_phy_provider_register(dev, of_phy_simple_xlate);

	usb_phy->c_phy_id = MOL_PHY_ID_NUM;

	return PTR_ERR_OR_ZERO(phy_provider);
}

static const struct of_device_id molchip_usb3_id_table[] = {
	{ .compatible = "molchip,usb3-phy", },
	{},
};

MODULE_DEVICE_TABLE(of, molchip_usb3_id_table);

static const struct dev_pm_ops usb_pm_ops = {
	.suspend = DRV_usb_Suspend,
};

static struct platform_driver molchip_usb3_phy_driver = {
	.probe = molchip_usb3_phy_probe,
	.driver = {
		   .name = "molchip_usb3_phy",
		   	.pm = &usb_pm_ops,
		   .of_match_table = molchip_usb3_id_table,
		   },
};

module_platform_driver(molchip_usb3_phy_driver);

MODULE_LICENSE("GPL v2");
