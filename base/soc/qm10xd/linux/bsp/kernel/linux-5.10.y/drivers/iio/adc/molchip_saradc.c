/*
 * Molchip Successive Approximation Register (SAR) A/D Converter
 * Copyright (C) 2019 MOLCHIP, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/reset.h>
#include <linux/regulator/consumer.h>
#include <linux/iio/iio.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/mini_clock/clock.h>
#include <linux/pm_wakeirq.h>


#define SARADC_CTRL0			0x0000
#define SARADC_CTRL1			0x0004
#define SARADC_CTRL2			0x0008
#define SARADC_CONFIG0			0x0010
#define SARADC_CONFIG1			0x0014
#define SARADC_CONFIG2			0x0018
#define SARADC_CONFIG3			0x001C
#define SARADC_CONFIG4			0x0020
#define SARADC_CONFIG5			0x0024
#define SARADC_CONFIG6			0x0028
#define SARADC_CONFIG7			0x002C
#define SARADC_INT_EN			0x0040
#define SARADC_INT_STA			0x0044
#define SARADC_DOUT0			0x0050
#define SARADC_DOUT1			0x0054
#define SARADC_DOUT2			0x0058
#define SARADC_DOUT3			0x005C
#define SARADC_BUTTON_DOUT0		0x0060
#define SARADC_BUTTON_DOUT1		0x0064
#define SARADC_BUTTON_DOUT2		0x0068
#define SARADC_BUTTON_DOUT3		0x006C

/* SARADC_CTRL0 */
#define SARADC_ENABLE			(0x1 << 0)

/* SARADC_CTRL1 */
#define SARADC_UPDATE			(0x1 << 0)

#define SARADC_TIMEOUT			msecs_to_jiffies(100)

/* saradc keypad channel number */
#define CHAN_KEYUP_IRQ(x)			(1 << ((x)+16))
#define CHAN_KEYDOWN_IRQ(x)			(1 << ((x)+8))


struct molchip_saradc_data {
	int				num_bits;
	const struct iio_chan_spec	*channels;
	int				num_channels;
	unsigned long			clk_rate;
	u32				verf;
};

struct molchip_saradc {
	void __iomem		*regs;
	struct completion	completion;
	const struct molchip_saradc_data *data;
	u16			last_val;
	const struct iio_chan_spec *last_chan;
	struct clk *adc_clk;
	struct clk *adc_apb_clk;
	struct clk *adc_clk_mux;
};

static void __iomem *mc_saradc_base = NULL;

struct saradc_chn_info_t {
	u32 ofs;
	u32 pos;
};

static struct saradc_chn_info_t chn_info[] = {
	{SARADC_DOUT0, 0x00},
	{SARADC_DOUT0, 0x10},
	{SARADC_DOUT1, 0x00},
	{SARADC_DOUT1, 0x10},
};

void mc_set_saradc_addr(void __iomem *base)
{
	mc_saradc_base = base;
}

void __iomem *mc_get_saradc_addr(void)
{
	return mc_saradc_base;
}

static void molchip_saradc_power_down(struct molchip_saradc *info)
{
	/* Clear irq */

	writel_relaxed(0, info->regs + SARADC_INT_STA);

	/* disable sar-adc module */
	writel_relaxed(0, info->regs + SARADC_CTRL0);
}

static void molchip_saradc_disable_convert(struct molchip_saradc *info)
{
	u32 reg_val = 0;

	/* disable sar-adc interrupt */
	reg_val = readl_relaxed(info->regs + SARADC_INT_EN);
	reg_val &= ~(0x1 << info->last_chan->channel);
	writel_relaxed(reg_val, info->regs + SARADC_INT_EN);

	/* disable average sample mode */
	reg_val = readl_relaxed(info->regs + SARADC_CONFIG2);
	reg_val &= ~(0x1 << info->last_chan->channel);
	writel_relaxed(reg_val, info->regs + SARADC_CONFIG2);
}

static int molchip_saradc_conversion(struct molchip_saradc *info,
				struct iio_chan_spec const *chan)
{
	u32 reg_val = 0;

	reinit_completion(&info->completion);

	info->last_chan = chan;

	/* enable single sample mode */
	writel_relaxed((0x1 << chan->channel), info->regs + SARADC_CONFIG0);

	/* enable average sample mode */
	reg_val = readl_relaxed(info->regs + SARADC_CONFIG2);
	reg_val |= (0x1 << chan->channel);
	writel_relaxed(reg_val, info->regs + SARADC_CONFIG2);

	/* enable sar-adc interrupt */
	reg_val = readl_relaxed(info->regs + SARADC_INT_EN);
	reg_val |= (0x1 << chan->channel);
	writel_relaxed(reg_val, info->regs + SARADC_INT_EN);

	/* enable sar-adc module */
	writel_relaxed(1, info->regs + SARADC_CTRL0);

	/* Start trigger sar-adc conversion */
	writel_relaxed(1, info->regs + SARADC_CTRL1);

	if (!wait_for_completion_timeout(&info->completion, SARADC_TIMEOUT))
		return -ETIMEDOUT;

	return 0;
}

	/*chn 0*/
static void molchip_keytouch_init(struct molchip_saradc *info)
{
	u32 reg_val;

	info->last_chan = info->data->channels;
	writel_relaxed(0x0, info->regs + SARADC_CTRL0);
	/* set active bit for sample */
	writel_relaxed(0xFFF, info->regs + SARADC_CONFIG7);
	/* set channel continue mode enable*/
	reg_val = readl_relaxed(info->regs + SARADC_CONFIG1);
	reg_val |= (0x1 << 0);
	writel_relaxed(reg_val, info->regs + SARADC_CONFIG1);
	/* set channel average mode enable*/
	reg_val = readl_relaxed(info->regs + SARADC_CONFIG2);
	reg_val |= (0x1 << 0);
	writel_relaxed(reg_val, info->regs + SARADC_CONFIG2);
	/* set channel button detect mode */
	reg_val = readl_relaxed(info->regs + SARADC_CONFIG3);
	reg_val |= (0x1 << 0);
	writel_relaxed(reg_val, info->regs + SARADC_CONFIG3);
	/* set scan time */
	writel_relaxed(0x100, info->regs + SARADC_CONFIG4);
	/* set glitch time */
	writel_relaxed(0xFFFF<<4, info->regs + SARADC_CONFIG5);

	writel_relaxed(0xff, info->regs + SARADC_CONFIG6);

	writel_relaxed(0x1, info->regs + SARADC_CTRL0);

	reg_val = readl_relaxed(info->regs + SARADC_INT_EN);
	reg_val |= CHAN_KEYDOWN_IRQ(0);
	writel_relaxed(reg_val, info->regs + SARADC_INT_EN);

}

static int molchip_saradc_read_raw(struct iio_dev *indio_dev,
				struct iio_chan_spec const *chan,
				int *val, int *val2, long mask)
{
	struct molchip_saradc *info = iio_priv(indio_dev);
	int ret;

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
		mutex_lock(&indio_dev->mlock);

		ret = molchip_saradc_conversion(info, chan);
		if (ret) {
			molchip_saradc_power_down(info);
			mutex_unlock(&indio_dev->mlock);
			return ret;
		}

		*val = info->last_val;

		molchip_saradc_disable_convert(info);

		mutex_unlock(&indio_dev->mlock);
		return IIO_VAL_INT;

	case IIO_CHAN_INFO_SCALE:
		*val = info->data->verf;
		*val2 = info->data->num_bits;
		return IIO_VAL_FRACTIONAL_LOG2;

	default:
		return -EINVAL;
	}
}

static irqreturn_t molchip_saradc_isr(int irq, void *dev_id)
{
	struct molchip_saradc *info = (struct molchip_saradc *)dev_id;
	u32 int_sts = readl_relaxed(info->regs + SARADC_INT_STA);

	if (int_sts & (0x1 << info->last_chan->channel)) {
		/* Read value */
		info->last_val = readl_relaxed(info->regs +
			chn_info[info->last_chan->channel].ofs) >>
			chn_info[info->last_chan->channel].pos;
		info->last_val &= GENMASK(info->data->num_bits - 1, 0);
		/* Clear irq & power down adc */
		writel_relaxed(~(0x1<<info->last_chan->channel),
			info->regs + SARADC_INT_STA);
		complete(&info->completion);
		return IRQ_HANDLED;
	}

	if (int_sts & (CHAN_KEYUP_IRQ(0) | CHAN_KEYDOWN_IRQ(0))) {
		writel_relaxed(~int_sts, info->regs + SARADC_INT_STA);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static const struct iio_info molchip_saradc_iio_info = {
	.read_raw = molchip_saradc_read_raw,
};

#define ADC_CHANNEL(_index, _id) {				\
	.type = IIO_VOLTAGE,					\
	.indexed = 1,						\
	.channel = _index,					\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),		\
	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),	\
	.datasheet_name = _id,					\
}

static const struct iio_chan_spec molchip_saradc_iio_channels[] = {
	ADC_CHANNEL(0, "adc0"),
	ADC_CHANNEL(1, "adc1"),
};

static const struct molchip_saradc_data saradc_data = {
	.num_bits = 12,
	.channels = molchip_saradc_iio_channels,
	.num_channels = ARRAY_SIZE(molchip_saradc_iio_channels),
	.clk_rate = 12000000,
	.verf     = 1800,
};

static const struct of_device_id molchip_saradc_match[] = {
	{
		.compatible = "molchip,saradc",
		.data = &saradc_data,
	},
	{},
};
MODULE_DEVICE_TABLE(of, molchip_saradc_match);
static int molchip_saradc_probe(struct platform_device *pdev)
{
	struct molchip_saradc *info = NULL;
	struct device *dev = &pdev->dev;
	struct device_node *np = pdev->dev.of_node;
	struct iio_dev *indio_dev = NULL;
	struct resource	*mem;
	const struct of_device_id *match;
	int ret;
	int irq;

	if (!np)
		return -ENODEV;

	indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*info));
	if (!indio_dev) {
		dev_err(&pdev->dev, "failed allocating iio device\n");
		return -ENOMEM;
	}
	info = iio_priv(indio_dev);

	match = of_match_device(molchip_saradc_match, &pdev->dev);
	info->data = match->data;

	if (mc_get_saradc_addr() == NULL) {
		mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		info->regs = devm_ioremap_resource(&pdev->dev, mem);
		if (IS_ERR(info->regs))
			return PTR_ERR(info->regs);
		mc_set_saradc_addr(info->regs);
	} else {
		info->regs = mc_get_saradc_addr();
	}
#if 0
	info->adc_clk_mux = devm_clk_get(&pdev->dev, "adc_mux");
	if (IS_ERR(info->adc_clk_mux)) {
		dev_err(&pdev->dev, "failed to get adc_clk_mux clk\n");
		return PTR_ERR(info->adc_clk_mux);
	}

	ret = clk_set_rate(info->adc_clk_mux, 24000000);
	if (ret) {
		dev_err(&pdev->dev, "failed to set adc_clk_mux\n");
		return ret;
	}

	info->adc_clk = devm_clk_get(&pdev->dev, "adc_ckg");
	if (IS_ERR(info->adc_clk)) {
			dev_err(&pdev->dev, "failed to get adc_ckg clk\n");
			return PTR_ERR(info->adc_clk);
	}

	ret = clk_prepare_enable(info->adc_clk);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to enable adc_ckg clock\n");
		clk_put(info->adc_clk);
		return ret;
	}

	info->adc_apb_clk = devm_clk_get(&pdev->dev, "adc_apb_ckg");
	if (IS_ERR(info->adc_apb_clk)) {
			dev_err(&pdev->dev, "failed to get adc_apb_clk clk\n");
			return PTR_ERR(info->adc_apb_clk);
	}

	ret = clk_prepare_enable(info->adc_apb_clk);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to enable adc_apb_clk clock\n");
		clk_put(info->adc_apb_clk);
		return ret;
	}

#endif
	clk_mini_set_rate(CKG_MUX_SADCW, 12000000);
	clk_mini_enable(CKG_SADCW_EN);
	clk_mini_enable(CKG_APB_SADC_EN);

	init_completion(&info->completion);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		ret = irq;
		dev_err(&pdev->dev, "no irq resource?\n");
		goto err;
	}

	ret = devm_request_irq(&pdev->dev, irq, molchip_saradc_isr,
			       IRQF_SHARED | IRQF_EARLY_RESUME, dev_name(&pdev->dev), info);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed requesting irq %d\n", irq);
		goto err;
	}

	if (device_property_read_bool(&pdev->dev, "wakeup-source")) {
		ret = device_init_wakeup(&pdev->dev, true);
		if (ret)
			goto err;
		ret = dev_pm_set_wake_irq(&pdev->dev, irq);
		if (ret)
			goto err;
	}

	platform_set_drvdata(pdev, indio_dev);
	dev_set_drvdata(dev, info);

	indio_dev->name = dev_name(&pdev->dev);
	indio_dev->dev.parent = &pdev->dev;
	indio_dev->dev.of_node = pdev->dev.of_node;
	indio_dev->info = &molchip_saradc_iio_info;
	indio_dev->modes = INDIO_DIRECT_MODE;

	indio_dev->channels = info->data->channels;
	indio_dev->num_channels = info->data->num_channels;

	ret = iio_device_register(indio_dev);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed iio_device_register\n");
		goto err;
	}
		
	return 0;
err:
	clk_mini_disable(CKG_SADCW_EN);
	clk_mini_disable(CKG_APB_SADC_EN);
	return ret;
}

static int molchip_saradc_remove(struct platform_device *pdev)
{
	struct iio_dev *indio_dev = platform_get_drvdata(pdev);

	iio_device_unregister(indio_dev);
	clk_mini_disable(CKG_SADCW_EN);
	clk_mini_disable(CKG_APB_SADC_EN);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int molchip_saradc_suspend(struct device *dev)
{
	struct molchip_saradc * info = dev_get_drvdata(dev);

	if (device_can_wakeup(dev)) {
		molchip_keytouch_init(info);
	} else {
		clk_mini_disable(CKG_SADCW_EN);
		clk_mini_disable(CKG_APB_SADC_EN);
	}
	printk("saradc_suspend done\r\n");
	return 0;
}

static int molchip_saradc_resume(struct device *dev)
{
	if (!device_can_wakeup(dev)) {
		clk_mini_set_rate(CKG_MUX_SADCW, 12000000);
		clk_mini_enable(CKG_SADCW_EN);
		clk_mini_enable(CKG_APB_SADC_EN);
	}
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(molchip_saradc_pm_ops,
			 molchip_saradc_suspend, molchip_saradc_resume);

static struct platform_driver molchip_saradc_driver = {
	.probe		= molchip_saradc_probe,
	.remove		= molchip_saradc_remove,
	.driver		= {
		.name	= "molchip-saradc",
		.of_match_table = molchip_saradc_match,
		.pm	= &molchip_saradc_pm_ops,
	},
};

module_platform_driver(molchip_saradc_driver);

MODULE_DESCRIPTION("Molchip SARADC driver");
MODULE_LICENSE("GPL v2");
