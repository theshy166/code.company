/*
 * Platform driver for molchip  DMA64 Controller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/of.h>
#include <linux/of_dma.h>
#include <linux/of_platform.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include "mc_app_dma64.h"
#include "utility.h"

#define DRV_NAME	"dma64"

struct mc_dma64_drv_data {
	int	(*clk_init) (struct mc_dma64_chip *chip);
};

static struct dma_chan* mc_dma_of_xlate(struct of_phandle_args *dma_spec,
					struct of_dma *ofdma){
	return NULL;
}

static struct mc_dma64_data *
mc_dma64_parse_dt(struct platform_device *pdev){

	struct device_node *np = pdev->dev.of_node;
	struct mc_dma64_data *pdata;
	u32 tmp;
	u32 nr_channels;

	if (!np) {
		dev_err(&pdev->dev, "Missing DT data\n");
		return NULL;
	}
	if (of_property_read_u32(np, "dma-channels", &nr_channels))
		return NULL;
	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return NULL;
	pdata->nr_channels = nr_channels;
	if (!of_property_read_u32(np, "chan_allocation_order", &tmp))
		pdata->chan_allocation_order = (unsigned char)tmp;

	return pdata;
}

static int mc_dma64_clk_init_fy00_haps(struct mc_dma64_chip *chip)
{
	struct device *dev = chip->dev;
	int ret = 0;

	chip->dma_eb = devm_clk_get(dev,"dma_eb");
	if(IS_ERR(chip->dma_eb)) {
		dev_err(dev, "dma_eb clock not found.\n");
		return PTR_ERR(chip->dma_eb);
	}
	ret = clk_prepare_enable(chip->dma_eb);
	if(ret)
		dev_err(dev,"Unable to enable  dma_eb\n");

	return ret;
}

static int mc_dma64_clk_init_fy00(struct mc_dma64_chip  *chip)
{
	struct device *dev = chip->dev;
	int ret = 0;

	chip->dma_eb = devm_clk_get(dev,"dma_eb");
	if(IS_ERR(chip->dma_eb)) {
		dev_err(dev, "dma_eb clock not found.\n");
		return PTR_ERR(chip->dma_eb);
	}
	ret = clk_prepare_enable(chip->dma_eb);
	if(ret)
		dev_err(dev,"Unable to enable  dma_eb\n");

	return ret;
}

static int mc_dma64_clk_init_fy01(struct mc_dma64_chip *chip)
{
	struct device *dev = chip->dev;
	int ret = 0;

	chip->dma_eb = devm_clk_get(dev,"dma_eb");
	if(IS_ERR(chip->dma_eb)) {
		dev_err(dev, "dma_eb clock not found.\n");
		return PTR_ERR(chip->dma_eb);
	}
	ret = clk_prepare_enable(chip->dma_eb);
	if(ret) {
		dev_err(dev,"Unable to enable  dma_eb\n");
		return ret;
	}

	chip->dma_force = devm_clk_get(dev,"dma_force");
	if(IS_ERR(chip->dma_force)) {
		dev_err(dev, "dma_force clock not found.\n");
		return PTR_ERR(chip->dma_force);
	}
	ret = clk_prepare_enable(chip->dma_force);
	if(ret){
		dev_err(dev,"Unable to enable dma_force\n");
		clk_disable_unprepare(chip->dma_eb);
		return ret;
	};

	chip->dma_auto = devm_clk_get(dev,"dma_auto");
	if(IS_ERR(chip->dma_auto)) {
		dev_err(dev, "dma_auto clock not found.\n");
		return PTR_ERR(chip->dma_auto);
	}
	ret = clk_prepare_enable(chip->dma_auto);
	if(ret){
		dev_err(dev,"Unable to enable dma_auto\n");
		clk_disable_unprepare(chip->dma_eb);
		clk_disable_unprepare(chip->dma_force);
	}

	return ret;
}

static int mc_dma64_clk_init_fy01_haps(struct mc_dma64_chip *chip)
{
	/* Todo: */
	return 0;
}

static const struct mc_dma64_drv_data fy01_haps_data = {
	.clk_init = mc_dma64_clk_init_fy01_haps,
};

static const struct mc_dma64_drv_data fy01_data = {
	.clk_init = mc_dma64_clk_init_fy01,
};

static const struct mc_dma64_drv_data fy00_haps_data = {
	.clk_init = mc_dma64_clk_init_fy00_haps,
};

static const struct mc_dma64_drv_data fy00_data = {
	.clk_init = mc_dma64_clk_init_fy00,
};

static int molchip_probe(struct platform_device *pdev){

	struct mc_dma64_chip 	*chip;
	struct resource 	*mem;
	struct device *dev = &pdev->dev;
	const struct mc_dma64_data *pdata;
	int err;
	const struct mc_dma64_drv_data *params_data = of_device_get_match_data(dev);

	chip = devm_kzalloc(dev,sizeof(*chip),GFP_KERNEL);
	if(!chip)
		return -ENOMEM;

	chip->dev = dev;
	chip->irq = platform_get_irq(pdev,0);
	if(chip->irq < 0)
		return chip->irq;

	mem = platform_get_resource(pdev,IORESOURCE_MEM,0);
	chip->common_regs = devm_ioremap_resource(dev,mem);

	if(IS_ERR(chip->common_regs))
		return PTR_ERR(chip->common_regs);

	mem = platform_get_resource(pdev,IORESOURCE_MEM,1);
	chip->chan_regs = devm_ioremap_resource(dev,mem);
	if(IS_ERR(chip->chan_regs))
		return PTR_ERR(chip->chan_regs);

	mem = platform_get_resource(pdev,IORESOURCE_MEM,2);
	chip->req_cid_regs = devm_ioremap_resource(dev,mem);
	if(IS_ERR(chip->req_cid_regs))
		return PTR_ERR(chip->req_cid_regs);

	if ((params_data) && (params_data->clk_init)) {
		err = params_data->clk_init(chip);
		if (err)
			return err;
	}

	err = dma_coerce_mask_and_coherent(&pdev->dev,DMA_BIT_MASK(32));
	if(err)
		return err;

	pdata = dev_get_platdata(dev);
	if(!pdata)
		pdata = mc_dma64_parse_dt(pdev);

	chip->pdata = pdata;
	pm_runtime_enable(&pdev->dev);
	err = mc_dma64_probe(chip);
	if(err)
		goto err_mc_dma_probe;

	platform_set_drvdata(pdev,chip);

	if(pdev->dev.of_node){
		err = of_dma_controller_register(pdev->dev.of_node,mc_dma_of_xlate,chip->mc);
		if(err)
			dev_err(&pdev->dev,"could not register of_dma_controller\n");
	}

	return 0;

err_mc_dma_probe:
	return err;

}

static int molchip_remove(struct platform_device *pdev)
{
	struct mc_dma64_chip *chip = platform_get_drvdata(pdev);

	if(pdev->dev.of_node)
		of_dma_controller_free(pdev->dev.of_node);

 	mc_dma_remove(chip);
	pm_runtime_disable(&pdev->dev);
	clk_disable_unprepare(chip->clk);
	return 0;
}

static void molchip_shutdown(struct platform_device *pdev)
{
	struct mc_dma64_chip *chip = platform_get_drvdata(pdev);
	pm_runtime_get_sync(chip->dev);
	/*TODO dma64 disable*/
//	mc_dma_disable(chip);
	pm_runtime_put_sync_suspend(chip->dev);
	clk_disable_unprepare(chip->clk);
}

static const struct of_device_id mc_dma_of_id_table[] ={
	{.compatible = "molchip,fy01-haps-dma64", .data = &fy01_haps_data},
	{.compatible = "molchip,fy01-dma64", .data = &fy01_data},
	{.compatible = "molchip,fy00-haps-dma64", .data = &fy00_haps_data},
	{.compatible = "molchip,fy00-dma64", .data = &fy01_data},
	{}
};

static struct platform_driver molchip_driver = {
	.probe		= molchip_probe,
	.remove		= molchip_remove,
	.shutdown	= molchip_shutdown,
	.driver		={
		.name	=DRV_NAME,
		.of_match_table = of_match_ptr(mc_dma_of_id_table),

	},

};

static int __init molchip_init(void)
{
	return platform_driver_register(&molchip_driver);
}
subsys_initcall(molchip_init);

static void __exit molchip_exit(void)
{
	platform_driver_unregister(&molchip_driver);
}
module_exit(molchip_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("molchip DMA64 controller");
MODULE_ALIAS("platform:" DRV_NAME);

