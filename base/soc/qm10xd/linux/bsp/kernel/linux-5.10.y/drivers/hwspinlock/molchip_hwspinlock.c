// MOLCHIP-License-Identifier: GPL-2.0
/*
 * Molchip hardware spinlock driver
 * Copyright (C) 2023 Molchip
 */

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/hwspinlock.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "hwspinlock_internal.h"

/* hwspinlock registers definition */
#define HWSPINLOCK_TOKEN(_X_)		(0x1000 + 0x1000 * (_X_))

/* unlocked value */
#define HWSPINLOCK_NOTTAKEN		0x10C5A8A8

/* hwspinlock number */
#define MOLCHIP_HWLOCKS_NUM		32

#define HWSPINLOCK_FPGA

struct molchip_hwspinlock_dev {
	void __iomem *base;
	struct clk *clk;
	struct hwspinlock_device bank;
};

/* try to lock the hardware spinlock */
static int molchip_hwspinlock_trylock(struct hwspinlock *lock)
{
	struct molchip_hwspinlock_dev *molchip_hwlock =
		dev_get_drvdata(lock->bank->dev);
	void __iomem *addr = lock->priv;
	int lock_id;

	if (!readl(addr))
		return 1;

	lock_id = hwlock_to_id(lock);
	/* get the hardware spinlock id*/
	dev_warn(molchip_hwlock->bank.dev,
		 "hwspinlock [%d] lock failed !\n",
		 lock_id);
	return 0;
}

/* unlock the hardware spinlock */
static void molchip_hwspinlock_unlock(struct hwspinlock *lock)
{
	void __iomem *lock_addr = lock->priv;

	writel(HWSPINLOCK_NOTTAKEN, lock_addr);
}

/* The specs recommended below number as the retry delay time */
static void molchip_hwspinlock_relax(struct hwspinlock *lock)
{
	ndelay(10);
}

static const struct hwspinlock_ops molchip_hwspinlock_ops = {
	.trylock = molchip_hwspinlock_trylock,
	.unlock = molchip_hwspinlock_unlock,
	.relax = molchip_hwspinlock_relax,
};

static void molchip_hwspinlock_disable(void *data)
{
#ifndef HWSPINLOCK_FPGA
	struct molchip_hwspinlock_dev *molchip_hwlock = data;

	clk_disable_unprepare(molchip_hwlock->clk);
#endif
}

static int molchip_hwspinlock_probe(struct platform_device *pdev)
{
	struct molchip_hwspinlock_dev *molchip_hwlock;
	struct hwspinlock *lock;
	int i, ret;

	if (!pdev->dev.of_node)
		return -ENODEV;

	molchip_hwlock = devm_kzalloc(&pdev->dev,
				   sizeof(struct molchip_hwspinlock_dev) +
				   MOLCHIP_HWLOCKS_NUM * sizeof(*lock),
				   GFP_KERNEL);
	if (!molchip_hwlock)
		return -ENOMEM;

	molchip_hwlock->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(molchip_hwlock->base))
		return PTR_ERR(molchip_hwlock->base);
#ifndef HWSPINLOCK_FPGA
	molchip_hwlock->clk = devm_clk_get(&pdev->dev, "hslock_clk");
	if (IS_ERR(molchip_hwlock->clk)) {
		dev_err(&pdev->dev, "get hwspinlock clock failed!\n");
		return PTR_ERR(molchip_hwlock->clk);
	}

	ret = clk_prepare_enable(molchip_hwlock->clk);
	if (ret)
		return ret;
#endif
	ret = devm_add_action_or_reset(&pdev->dev, molchip_hwspinlock_disable,
				       molchip_hwlock);
	if (ret) {
		dev_err(&pdev->dev,
			"Failed to add hwspinlock disable action\n");
		return ret;
	}

	for (i = 0; i < MOLCHIP_HWLOCKS_NUM; i++) {
		lock = &molchip_hwlock->bank.lock[i];
		lock->priv = molchip_hwlock->base + HWSPINLOCK_TOKEN(i);
		writel(HWSPINLOCK_NOTTAKEN, molchip_hwlock->base + HWSPINLOCK_TOKEN(i));
	}

	platform_set_drvdata(pdev, molchip_hwlock);

	return devm_hwspin_lock_register(&pdev->dev, &molchip_hwlock->bank,
					 &molchip_hwspinlock_ops, 0,
					 MOLCHIP_HWLOCKS_NUM);
}

static const struct of_device_id molchip_hwspinlock_of_match[] = {
	{ .compatible = "molchip,hwspinlock", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, molchip_hwspinlock_of_match);

static struct platform_driver molchip_hwspinlock_driver = {
	.probe = molchip_hwspinlock_probe,
	.driver = {
		.name = "molchip_hwspinlock",
		.of_match_table = of_match_ptr(molchip_hwspinlock_of_match),
	},
};

static int __init molchip_hwspinlock_init(void)
{
	return platform_driver_register(&molchip_hwspinlock_driver);
}
postcore_initcall(molchip_hwspinlock_init);

static void __exit molchip_hwspinlock_exit(void)
{
	platform_driver_unregister(&molchip_hwspinlock_driver);
}
module_exit(molchip_hwspinlock_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hardware spinlock driver for MOLCHIP");
MODULE_AUTHOR("Siyuan Song <siyuan.song@molchip.com>");

