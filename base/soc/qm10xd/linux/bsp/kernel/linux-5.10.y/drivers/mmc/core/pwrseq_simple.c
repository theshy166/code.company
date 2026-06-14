// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2014 Linaro Ltd
 *
 * Author: Ulf Hansson <ulf.hansson@linaro.org>
 *
 *  Simple MMC power sequence management
 */
#include <linux/clk.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/property.h>
#include <linux/mmc/host.h>

#include <linux/gpio.h>

#include "pwrseq.h"

struct mmc_pwrseq_simple {
	struct mmc_pwrseq pwrseq;
	bool clk_enabled;
	u32 post_power_on_delay_ms;
	u32 power_off_delay_us;
	u32 power_on_gpio;
	u32 power_on_gpio_value;
	struct clk *ext_clk;
	struct gpio_descs *reset_gpios;
};

#define to_pwrseq_simple(p) container_of(p, struct mmc_pwrseq_simple, pwrseq)

static void mmc_pwrseq_simple_set_gpios_value(struct mmc_pwrseq_simple *pwrseq,
					      int value)
{
	struct gpio_descs *reset_gpios = pwrseq->reset_gpios;

	if (!IS_ERR(reset_gpios)) {
		unsigned long *values;
		int nvalues = reset_gpios->ndescs;

		values = bitmap_alloc(nvalues, GFP_KERNEL);
		if (!values)
			return;

		if (value)
			bitmap_fill(values, nvalues);
		else
			bitmap_zero(values, nvalues);

		gpiod_set_array_value_cansleep(nvalues, reset_gpios->desc,
					       reset_gpios->info, values);

		kfree(values);
	}
}

static void mmc_pwrseq_simple_pre_power_on(struct mmc_host *host)
{
	struct mmc_pwrseq_simple *pwrseq = to_pwrseq_simple(host->pwrseq);

	if (!IS_ERR(pwrseq->ext_clk) && !pwrseq->clk_enabled) {
		clk_prepare_enable(pwrseq->ext_clk);
		pwrseq->clk_enabled = true;
	}

	mmc_pwrseq_simple_set_gpios_value(pwrseq, 1);
}

static void mmc_pwrseq_simple_post_power_on(struct mmc_host *host)
{
	struct mmc_pwrseq_simple *pwrseq = to_pwrseq_simple(host->pwrseq);

	mmc_pwrseq_simple_set_gpios_value(pwrseq, 0);

	if (pwrseq->post_power_on_delay_ms)
		msleep(pwrseq->post_power_on_delay_ms);
}

static void mmc_pwrseq_simple_power_off(struct mmc_host *host)
{
	struct mmc_pwrseq_simple *pwrseq = to_pwrseq_simple(host->pwrseq);

	mmc_pwrseq_simple_set_gpios_value(pwrseq, 1);

	if (pwrseq->power_off_delay_us)
		usleep_range(pwrseq->power_off_delay_us,
			2 * pwrseq->power_off_delay_us);

	if (!IS_ERR(pwrseq->ext_clk) && pwrseq->clk_enabled) {
		clk_disable_unprepare(pwrseq->ext_clk);
		pwrseq->clk_enabled = false;
	}
}

static const struct mmc_pwrseq_ops mmc_pwrseq_simple_ops = {
	.pre_power_on = mmc_pwrseq_simple_pre_power_on,
	.post_power_on = mmc_pwrseq_simple_post_power_on,
	.power_off = mmc_pwrseq_simple_power_off,
};

static const struct of_device_id mmc_pwrseq_simple_of_match[] = {
	{ .compatible = "mmc-pwrseq-simple",},
	{/* sentinel */},
};
MODULE_DEVICE_TABLE(of, mmc_pwrseq_simple_of_match);

int mmc_init_wifi_sdio(u32 gpio, u32 value ){


	//volatile unsigned int *reg;

	//GPIO 5-1  enable
	//reg =  ioremap(0x1020009c,4);
	//*reg = 0x00000001;
	//msleep(1);
	//pr_err("base 0x%x  value 0x%x\n",reg,( unsigned int)*reg);
	//pr_err("SDIO_REG1: base %p value 0x%x\n", (void *)reg, *reg);
	//iounmap(reg);
   // pr_err("power on gpio %d  value %d\n",gpio,value);
	//gpio_request(9, NULL);
	gpio_request((unsigned int)gpio, NULL);
	msleep(10);
	gpio_direction_output((unsigned int)gpio , (int )value);
	msleep(100);
	//pr_err("mmc_init_wifi_sdio is called\n");
	return 0;
}

static int mmc_pwrseq_simple_probe(struct platform_device *pdev)
{
	struct mmc_pwrseq_simple *pwrseq;
	struct device *dev = &pdev->dev;

	pwrseq = devm_kzalloc(dev, sizeof(*pwrseq), GFP_KERNEL);
	if (!pwrseq)
		return -ENOMEM;

	pwrseq->ext_clk = devm_clk_get(dev, "ext_clock");
	if (IS_ERR(pwrseq->ext_clk) && PTR_ERR(pwrseq->ext_clk) != -ENOENT)
		return PTR_ERR(pwrseq->ext_clk);
	//pr_err("pwrseq->reset_gpios  is called\n");
	pwrseq->reset_gpios = devm_gpiod_get_array(dev, "reset",
							GPIOD_OUT_HIGH);
	//pr_err("pwrseq->reset_gpios %d  \n",pwrseq->reset_gpios);

	if (IS_ERR(pwrseq->reset_gpios) &&
	    PTR_ERR(pwrseq->reset_gpios) != -ENOENT &&
	    PTR_ERR(pwrseq->reset_gpios) != -ENOSYS) {
		return PTR_ERR(pwrseq->reset_gpios);
	}

	device_property_read_u32(dev, "power-on-enable",
				 &pwrseq->power_on_gpio);
	device_property_read_u32(dev, "power-on-enable-value",
				 &pwrseq->power_on_gpio_value);

	mmc_init_wifi_sdio(pwrseq->power_on_gpio, pwrseq->power_on_gpio_value);

	device_property_read_u32(dev, "post-power-on-delay-ms",
				 &pwrseq->post_power_on_delay_ms);
	device_property_read_u32(dev, "power-off-delay-us",
				 &pwrseq->power_off_delay_us);

	pwrseq->pwrseq.dev = dev;
	pwrseq->pwrseq.ops = &mmc_pwrseq_simple_ops;
	pwrseq->pwrseq.owner = THIS_MODULE;
	platform_set_drvdata(pdev, pwrseq);

	return mmc_pwrseq_register(&pwrseq->pwrseq);
}

static int mmc_pwrseq_simple_remove(struct platform_device *pdev)
{
	struct mmc_pwrseq_simple *pwrseq = platform_get_drvdata(pdev);

	mmc_pwrseq_unregister(&pwrseq->pwrseq);

	return 0;
}

static struct platform_driver mmc_pwrseq_simple_driver = {
	.probe = mmc_pwrseq_simple_probe,
	.remove = mmc_pwrseq_simple_remove,
	.driver = {
		.name = "pwrseq_simple",
		.of_match_table = mmc_pwrseq_simple_of_match,
	},
};

module_platform_driver(mmc_pwrseq_simple_driver);
MODULE_LICENSE("GPL v2");
