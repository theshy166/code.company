/*
 * Copyright (c) 2019 Jason Qin
 *
 * PWM driver for Molchip SoCs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 */

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/mini_clock/clock.h>
#define MC_PWM_NUM		4
#define MC_PWM_MAX_PERIOD		0x1FFFFFF

#define MC_PWM_STOP_LEVEL		0


#define MC_REG_GLOBAL_CTRL0		(0x0)
#define BIT_CFG_UPDATE(_a)	(1 << (_a))

#define MC_REG_GLOBAL_CTRL1		(0x4)
#define BIT_SHADOW_EN(_a)		(1 << (_a))
#define BIT_STOP_NOW(_a)		(0x10000 << (_a))

#define MC_REG_GLOBAL_CTRL2		(0x8)
#define BIT_OUTPUT_EN(_a)		(1 << (_a))

#define MC_REG_GLOBAL_INT_EN		(0x10)
#define BIT_ALL_INT_EN(_a)	(1 << (_a))
#define BIT_ONCE_INT_EN(_a)	(0x100 << (_a))

#define REG_GLOBAL_INT_STATUS		(0x14)

#define MC_REG_PWM_CTRL(_a)		((_a + 1) * 0x80)
#define BIT_PWM_CTRL_KEEP		(1 << 0)
#define BIT_PWM_CTRL_STOP_LEVEL_MASK		(0x3 << 1)
#define BIT_PWM_CTRL_STOP_LEVEL		(1)
#define STOP_LEVEL_LOW		(0)
#define STOP_LEVEL_HIGH		(3)
#define STOP_LEVEL_KEEP2		(2)
#define STOP_LEVEL_KEEP1		(1)

#define BIT_C_PWM_DELAY_EN		(1 << 3)

#define MC_REG_PWM_PERIOD(_a)		((_a + 1) * 0x80 + 0x4)
#define MC_REG_PWM_DUTY(_a)		((_a + 1) * 0x80 + 0x8)
#define MC_REG_PWM_PHASE(_a)		((_a + 1) * 0x80 + 0xc)
#define MC_REG_PWM_DELAY(_a)		((_a + 1) * 0x80 + 0x10)
#define MC_REG_PWM_NUM(_a)		((_a + 1) * 0x80 + 0x14)
#define MC_REG_PWM_DONE_CNT(_a)		((_a + 1) * 0x80 + 0x20)
#define MC_REG_PWM_TOTAL_CNT(_a)		((_a + 1) * 0x80 + 0x24)
#define MC_REG_PWM_STATUS2(_a)		((_a + 1) * 0x80 + 0x28)
#define BIT_PWM_PHASE_GREAT_PERIOD	(1 << 0)
#define BIT_PWM_DUTY_GREAT_PERIOD	(1 << 1)
#define BIT_PWM_NUM_EQUAL_ZERO	(1 << 2)
#define BIT_PWM_FINISH_ABUNORMALLY	(1 << 3)
#define BIT_PWM_CHANNEL_BUSY	(1 << 4)

/**
 * struct molchip_pwm_chip - private data of PWM chip
 * @chip:		generic PWM chip
 * @variant:		local copy of hardware variant data
 * @inverter_mask:	inverter status for all channels - one bit per channel
 * @base:		base address of mapped PWM registers
 * @base_clk:		base clock used to drive the timers
 */

static char mbuf[] = "Usage:\nWrite Command:\n\techo value(date_type: uint64 value) > debug\n\tvalue description:\n\tBit[3:0]: channel_num\n\tBit[7:4]: channel_en\n\tBit[15:8]: duty_set\n\tBit[31:16]: reserved\n\tBit[63:32]: period_ns_set";

struct molchip_pwm_chip {
	struct pwm_chip chip;
	u32 inverter_mask;
	struct platform_device *pdev;
	void __iomem *base;
	struct clk *apb_clk_mux;
	u32 default_apb_clk_mux;
	struct clk *pwm_clk;
	struct clk *pwm_apb_clk;

	spinlock_t lock;
	struct regmap *regmap;
};

struct molchip_pwm_drv_data {
	int	(*clk_init) (struct molchip_pwm_chip *chip);
};


static inline
struct molchip_pwm_chip *to_molchip_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct molchip_pwm_chip, chip);
}

static int pwm_molchip_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct molchip_pwm_chip *our_chip = to_molchip_pwm_chip(chip);
	u32 reg;

	spin_lock(&our_chip->lock);
	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL2);
	reg |= BIT_OUTPUT_EN(pwm->hwpwm);
	writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL2);
	spin_unlock(&our_chip->lock);
	return 0;
}

static void pwm_molchip_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct molchip_pwm_chip *our_chip = to_molchip_pwm_chip(chip);
	u32 reg;

	spin_lock(&our_chip->lock);
	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL2);
	reg &= ~BIT_OUTPUT_EN(pwm->hwpwm);
	writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL2);
	spin_unlock(&our_chip->lock);
}

static void __pwm_molchip_config(struct molchip_pwm_chip *our_chip, u32 ch, u32 dc, u32 pv)
{
	u32 reg = 0;

	dev_dbg(our_chip->chip.dev, "config ch[%d], dc %d, pv %d\n", ch, dc, pv);
	spin_lock(&our_chip->lock);

	reg = BIT_PWM_CTRL_KEEP;
	reg |= (MC_PWM_STOP_LEVEL << BIT_PWM_CTRL_STOP_LEVEL) &
	BIT_PWM_CTRL_STOP_LEVEL_MASK;
	writel(reg, our_chip->base + MC_REG_PWM_CTRL(ch));

	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL0);
	if( !(reg & BIT_CFG_UPDATE(ch)) ) {
		reg |= BIT_CFG_UPDATE(ch);
		writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL0);
	}

	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL2);
	if( !(reg & BIT_OUTPUT_EN(ch)) ) {
		reg |= BIT_OUTPUT_EN(ch);
		writel(100, our_chip->base + MC_REG_PWM_PERIOD(ch));
		writel(99, our_chip->base + MC_REG_PWM_DUTY(ch));
		writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL2);
	}

	writel(pv, our_chip->base + MC_REG_PWM_PERIOD(ch));
	writel(dc, our_chip->base + MC_REG_PWM_DUTY(ch));
	spin_unlock(&our_chip->lock);
}

static void _pwm_molchip_config(struct molchip_pwm_chip *our_chip, u32 ch, u32 dc, u32 pv)
{
	u32 reg = 0;

	dev_dbg(our_chip->chip.dev, "config ch[%d], dc %d, pv %d\n", ch, dc, pv);
	spin_lock(&our_chip->lock);
	//disable channel config
	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL0);
	reg &= ~BIT_CFG_UPDATE(ch);
	writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL0);
	//pulse will stop pulse
	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL1);
	reg |= BIT_STOP_NOW(ch);
	writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL1);

	reg = BIT_PWM_CTRL_KEEP;
	reg |= (MC_PWM_STOP_LEVEL << BIT_PWM_CTRL_STOP_LEVEL) &
		BIT_PWM_CTRL_STOP_LEVEL_MASK;
	writel(reg, our_chip->base + MC_REG_PWM_CTRL(ch));
	writel(pv, our_chip->base + MC_REG_PWM_PERIOD(ch));
	writel(dc, our_chip->base + MC_REG_PWM_DUTY(ch));
	//stop old pulse
	//reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL2);
	//reg &= ~BIT_OUTPUT_EN(ch);
	//writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL2);
	//update channel config
	reg = readl(our_chip->base + MC_REG_GLOBAL_CTRL0);
	reg |= BIT_CFG_UPDATE(ch);
	writel(reg, our_chip->base + MC_REG_GLOBAL_CTRL0);
	spin_unlock(&our_chip->lock);
}

static int pwm_molchip_config(struct pwm_chip *chip, struct pwm_device *pwm,
			      int duty_ns, int period_ns)
{
	struct molchip_pwm_chip *our_chip = to_molchip_pwm_chip(chip);
	u64 val, div, clk_rate;
	u32 pv, dc;

	if (period_ns > NSEC_PER_SEC || period_ns < 0) {
		printk("period_ns %d\r\n",period_ns);
		return -EINVAL;
	}

	/*if (our_chip->apb_clk_mux)
		clk_rate = clk_get_rate(our_chip->apb_clk_mux);
	else
		clk_rate = our_chip->default_apb_clk_mux;*/
	//clk_rate = 100000000;
	clk_rate = clk_mini_get_rate(CKG_MUX_PWM);
	dev_dbg(chip->dev, "get pwm clk mux %lld \n", clk_rate);
	div = 1000000000;
	val = clk_rate * period_ns;
	pv = div64_u64(val, div);
	val = clk_rate * duty_ns;
	dc = div64_u64(val, div);
	if( dc > pv ||  pv > MC_PWM_MAX_PERIOD ) {
		dev_err(chip->dev, "invalid pwm param \r\n");
		return -EINVAL;
	}
	if(dc == pv)
		__pwm_molchip_config(our_chip, pwm->hwpwm, dc, pv);
	else
		_pwm_molchip_config(our_chip, pwm->hwpwm, dc, pv);
	return 0;
}


static const struct pwm_ops pwm_molchip_ops = {
	.enable		= pwm_molchip_enable,
	.disable	= pwm_molchip_disable,
	.config		= pwm_molchip_config,
	.owner		= THIS_MODULE,
};

static ssize_t debug_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	return sprintf(buf, "%s\n",mbuf);
}

static ssize_t debug_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct molchip_pwm_chip *mol_chip = dev_get_drvdata(dev);
	int ret;
	u64 data;
	u32 duty_ns, period_ns;
	struct pwm_device pwm;

	ret = kstrtoull(buf, 0, &data);
	if (ret)
		return count;
	pwm.hwpwm = data & 0xf;
	if(data & 0xf0) {
		period_ns = data >> 32;
		if (((u32)(((data & 0xffffffff) >> 8) & 0xff) / 100) == 1)
			duty_ns = period_ns;
		else
			duty_ns = div64_u64((((u64)((u32)(((data & 0xffffffff) >> 8) & 0xff) % 100) * period_ns)), 100);
		dev_warn(dev, "set pwm[%d] period_ns(%d) duty_ns(%d) \n", pwm.hwpwm, period_ns, duty_ns);
		pwm_molchip_config(&mol_chip->chip, &pwm, duty_ns, period_ns);
		pwm_molchip_enable(&mol_chip->chip, &pwm);
	} else {
		pwm_molchip_disable(&mol_chip->chip, &pwm);
	}

	return count;
}
static DEVICE_ATTR(debug, S_IWUSR|S_IRUSR, debug_show,debug_store);

static int molchip_pwm_apclk_init_fy01_haps(struct molchip_pwm_chip *chip)
{
	struct device_node *np = chip->pdev->dev.of_node;

	chip->apb_clk_mux = NULL;

	return of_property_read_u32(np, "molchip,haps-apclk",
			     &chip->default_apb_clk_mux);

}

static int molchip_pwm_apclk_init_fy01(struct molchip_pwm_chip *chip)
{
	int ret = 0;
	struct device	*dev = &chip->pdev->dev;

	chip->apb_clk_mux = devm_clk_get(dev, "ckg_mux_pwm");
	if (IS_ERR(chip->apb_clk_mux)) {
		dev_err(dev, "failed to get ckg_mux_pwm clk\n");
		return PTR_ERR(chip->apb_clk_mux);
	}

	return ret;
}

static int molchip_pwm_apclk_init_haps(struct molchip_pwm_chip *chip)
{
	struct device_node *np = chip->pdev->dev.of_node;

	chip->apb_clk_mux = NULL;

	return of_property_read_u32(np, "molchip,haps-apclk",
			     &chip->default_apb_clk_mux);
}

static int molchip_pwm_apclk_init(struct molchip_pwm_chip *chip)
{
#if 0
	struct device	*dev = &chip->pdev->dev;

	chip->apb_clk_mux = devm_clk_get(dev, "ckg_mux_pwm");
	if (IS_ERR(chip->apb_clk_mux)) {
		dev_err(dev, "failed to get ckg_mux_pwm clk\n");
		return PTR_ERR(chip->apb_clk_mux);
	}
#endif
	clk_mini_set_rate(CKG_MUX_PWM, 100000000);
	return 0;
}

static const struct molchip_pwm_drv_data fy01_haps_data = {
	.clk_init = molchip_pwm_apclk_init_fy01_haps,
};

static const struct molchip_pwm_drv_data fy01_data = {
	.clk_init = molchip_pwm_apclk_init_fy01,
};

static const struct molchip_pwm_drv_data haps_data = {
	.clk_init = molchip_pwm_apclk_init_haps,
};

static const struct molchip_pwm_drv_data pwm_data = {
	.clk_init = molchip_pwm_apclk_init,
};


static const struct of_device_id molchip_pwm_matches[] = {
	{ .compatible = "molchip,fy01-pwm", .data = &fy01_data },
	{ .compatible = "molchip,fy01-haps-pwm", .data = &fy01_haps_data },
	{ .compatible = "molchip,pwm", .data = &pwm_data },
	{ .compatible = "molchip,haps-pwm", .data = &haps_data },
	{},
};
MODULE_DEVICE_TABLE(of, molchip_pwm_matches);

static int pwm_molchip_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct molchip_pwm_chip *chip;
	struct resource *res;
	const struct molchip_pwm_drv_data *params_data = of_device_get_match_data(dev);
	int ret;

	chip = devm_kzalloc(&pdev->dev, sizeof(*chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;
	chip->chip.dev = &pdev->dev;
	chip->chip.ops = &pwm_molchip_ops;
	chip->chip.base = -1;
	chip->chip.npwm = MC_PWM_NUM;
	chip->inverter_mask = BIT(MC_PWM_NUM) - 1;
	chip->chip.of_xlate = NULL; //of_pwm_xlate_with_flags;
	chip->chip.of_pwm_n_cells = 2;
	chip->pdev = pdev;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	chip->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(chip->base))
		return PTR_ERR(chip->base);
#if 0
	chip->pwm_clk = devm_clk_get(&pdev->dev, "ckg_pwm");
	if (IS_ERR(chip->pwm_clk)) {
		dev_err(dev, "failed to get ckg_pwm clk\n");
		return PTR_ERR(chip->pwm_clk);
	}

	ret = clk_prepare_enable(chip->pwm_clk);
	if (ret < 0) {
		dev_err(dev, "failed to enable ckg_pwm clock\n");
		clk_put(chip->pwm_clk);
		return ret;
	}

	chip->pwm_apb_clk = devm_clk_get(&pdev->dev, "ckg_apb_pwm");
	if (IS_ERR(chip->pwm_apb_clk)) {
		dev_err(dev, "failed to get ckg_apb_pwm clk\n");
		return PTR_ERR(chip->pwm_apb_clk);
	}

	ret = clk_prepare_enable(chip->pwm_apb_clk);
	if (ret < 0) {
		dev_err(dev, "failed to enable ckg_apb_pwm clock\n");
		clk_put(chip->pwm_apb_clk);
		return ret;
	}
#endif
	clk_mini_enable(CKG_PWM_EN);
	clk_mini_enable(CKG_APB_PWM_EN);
	if ((params_data) && (params_data->clk_init)) {
		ret = params_data->clk_init(chip);
		if (ret) {
			goto err;
		}
	}

	platform_set_drvdata(pdev, chip);
	dev_set_drvdata(dev, chip);
	ret = pwmchip_add(&chip->chip);
	if (ret < 0) {
		dev_err(dev, "failed to register PWM chip\n");
		goto err;
	}

	ret = device_create_file(&pdev->dev, &dev_attr_debug);
	if (ret) {
		dev_err(dev, "failed to register attributes, ret=%d\n", ret);
		goto err;
	}

	return 0;
err:
	devm_kfree(&pdev->dev, chip);
	clk_mini_disable(CKG_PWM_EN);
	clk_mini_disable(CKG_APB_PWM_EN);
	return ret;
}

static int pwm_molchip_remove(struct platform_device *pdev)
{
	struct molchip_pwm_chip *chip = platform_get_drvdata(pdev);
	int ret;

	ret = pwmchip_remove(&chip->chip);
	if (ret < 0)
		return ret;

	devm_kfree(&pdev->dev, chip);
	clk_mini_disable(CKG_PWM_EN);
	clk_mini_disable(CKG_APB_PWM_EN);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int pwm_molchip_suspend(struct device *dev)
{
	clk_mini_disable(CKG_PWM_EN);
	clk_mini_disable(CKG_APB_PWM_EN);
	printk("pwm_suspend done\r\n");
	return 0;
}

static int pwm_molchip_resume(struct device *dev)
{
	clk_mini_set_rate(CKG_MUX_PWM, 100000000);
	clk_mini_enable(CKG_PWM_EN);
	clk_mini_enable(CKG_APB_PWM_EN);
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(pwm_molchip_pm_ops, pwm_molchip_suspend,
			 pwm_molchip_resume);

static struct platform_driver pwm_molchip_driver = {
	.driver		= {
		.name	= "molchip-pwm",
		.pm	= &pwm_molchip_pm_ops,
		.of_match_table = of_match_ptr(molchip_pwm_matches),
	},
	.probe		= pwm_molchip_probe,
	.remove		= pwm_molchip_remove,
};
module_platform_driver(pwm_molchip_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Qin <jason.qin@molchip.com>");
MODULE_ALIAS("platform:molchip-pwm");
