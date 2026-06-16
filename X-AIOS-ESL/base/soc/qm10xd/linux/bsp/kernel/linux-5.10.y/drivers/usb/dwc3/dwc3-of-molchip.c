// SPDX-License-Identifier: GPL-2.0
/*
 * dwc3-of-molchip.c - OF glue layer for molchip integrations
 *
 * Copyright (c) 2022 molchip - https://www.molchip.com
 *
 * Author: Junbao.Zhang <junbao.zhang@molchip.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>

struct dwc3_of_molchip {
	struct device		*dev;
	struct clk_bulk_data	*clks;
	int			num_clocks;
	struct reset_control	*resets;
	bool			need_reset;
};

static int dwc3_of_molchip_probe(struct platform_device *pdev)
{
	struct dwc3_of_molchip	*molchip;
	struct device		*dev = &pdev->dev;
	struct device_node	*np = dev->of_node;

	int			ret;

	molchip = devm_kzalloc(dev, sizeof(*molchip), GFP_KERNEL);
	if (!molchip)
		return -ENOMEM;

	platform_set_drvdata(pdev, molchip);
	molchip->dev = dev;


	ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(35));
    if (ret) {
    	dev_err(&(pdev->dev), "dma_set_mask_and_coherent failed\n");
        return ret;
    }

	/*
	 * Some controllers need to toggle the usb3-otg reset before trying to
	 * initialize the PHY, otherwise the PHY times out.
	 */
	if (of_device_is_compatible(np, "rockchip,rk3399-dwc3"))
		molchip->need_reset = true;

	molchip->resets = of_reset_control_array_get(np, false, true,
						    true);
	if (IS_ERR(molchip->resets)) {
		ret = PTR_ERR(molchip->resets);
		dev_err(dev, "failed to get device resets, err=%d\n", ret);
		return ret;
	}

	ret = reset_control_deassert(molchip->resets);
	if (ret)
		goto err_resetc_put;

	ret = clk_bulk_get_all(molchip->dev, &molchip->clks);
	if (ret < 0)
		goto err_resetc_assert;

	molchip->num_clocks = ret;
	ret = clk_bulk_prepare_enable(molchip->num_clocks, molchip->clks);
	if (ret)
		goto err_resetc_assert;

	ret = of_platform_populate(np, NULL, NULL, dev);
	if (ret)
		goto err_clk_put;

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);

	return 0;

err_clk_put:
	clk_bulk_disable_unprepare(molchip->num_clocks, molchip->clks);
	clk_bulk_put_all(molchip->num_clocks, molchip->clks);

err_resetc_assert:
	reset_control_assert(molchip->resets);

err_resetc_put:
	reset_control_put(molchip->resets);
	return ret;
}

static void __dwc3_of_molchip_teardown(struct dwc3_of_molchip *molchip)
{
	of_platform_depopulate(molchip->dev);

	clk_bulk_disable_unprepare(molchip->num_clocks, molchip->clks);
	clk_bulk_put_all(molchip->num_clocks, molchip->clks);
	molchip->num_clocks = 0;

	reset_control_assert(molchip->resets);

	reset_control_put(molchip->resets);

	pm_runtime_disable(molchip->dev);
	pm_runtime_put_noidle(molchip->dev);
	pm_runtime_set_suspended(molchip->dev);
}

static int dwc3_of_molchip_remove(struct platform_device *pdev)
{
	struct dwc3_of_molchip	*molchip = platform_get_drvdata(pdev);

	__dwc3_of_molchip_teardown(molchip);

	return 0;
}

static void dwc3_of_molchip_shutdown(struct platform_device *pdev)
{
	struct dwc3_of_molchip	*molchip = platform_get_drvdata(pdev);

	__dwc3_of_molchip_teardown(molchip);
}

static int __maybe_unused dwc3_of_molchip_runtime_suspend(struct device *dev)
{
	struct dwc3_of_molchip	*molchip = dev_get_drvdata(dev);

	clk_bulk_disable(molchip->num_clocks, molchip->clks);

	return 0;
}

static int __maybe_unused dwc3_of_molchip_runtime_resume(struct device *dev)
{
	struct dwc3_of_molchip	*molchip = dev_get_drvdata(dev);

	return clk_bulk_enable(molchip->num_clocks, molchip->clks);
}

static int __maybe_unused dwc3_of_molchip_suspend(struct device *dev)
{
	struct dwc3_of_molchip *molchip = dev_get_drvdata(dev);

	if (molchip->need_reset)
		reset_control_assert(molchip->resets);

	return 0;
}

static int __maybe_unused dwc3_of_molchip_resume(struct device *dev)
{
	struct dwc3_of_molchip *molchip = dev_get_drvdata(dev);

	if (molchip->need_reset)
		reset_control_deassert(molchip->resets);

	return 0;
}

static const struct dev_pm_ops dwc3_of_molchip_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwc3_of_molchip_suspend, dwc3_of_molchip_resume)
	SET_RUNTIME_PM_OPS(dwc3_of_molchip_runtime_suspend,
			dwc3_of_molchip_runtime_resume, NULL)
};

static const struct of_device_id of_dwc3_molchip_match[] = {
	{ .compatible = "molchip,dwc3" },
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_dwc3_molchip_match);

static struct platform_driver dwc3_of_molchip_driver = {
	.probe		= dwc3_of_molchip_probe,
	.remove		= dwc3_of_molchip_remove,
	.shutdown	= dwc3_of_molchip_shutdown,
	.driver		= {
		.name	= "dwc3-of-molchip",
		.of_match_table = of_dwc3_molchip_match,
		.pm	= &dwc3_of_molchip_dev_pm_ops,
	},
};

module_platform_driver(dwc3_of_molchip_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DesignWare USB3 OF molchip Glue Layer");
