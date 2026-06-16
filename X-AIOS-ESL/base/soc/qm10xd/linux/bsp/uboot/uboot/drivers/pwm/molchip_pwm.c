/*
 * Copyright 2020 Molchip Inc.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <pwm.h>
#include <asm/io.h>
#include <fdt_support.h>
#include <dm/device-internal.h>

#define MAX_PWM_CH	4
#define CLOCK_SOURCE_FRE_ADD 	0x09200244	/* 0:24M, 1:50M, 2:100M */
#define ENABLE1_ADDR		0x0C0000AC
#define ENABLE2_ADDR		0x0C0000B0
#define ENABLE1_BIT		5
#define ENABLE2_BIT		7

#define PWM_CONFIG_EN		0x000
#define PWM_CH_EN		0x008
#define PWM_INT_EN		0x010

#define PWM_CH_INVAIL		0xffffffff

DECLARE_GLOBAL_DATA_PTR;

struct molchip_pwmx_reg {
	unsigned int ch_base;
	unsigned int period;
	unsigned int duty;
	unsigned int phase;
	unsigned int delay;
	unsigned int num;
};
struct molchip_pwm_priv {
	struct molchip_pwmx_reg *regs;
	unsigned int enable1_addr;
	unsigned int enable2_addr;
	unsigned int enable1_bit;
	unsigned int enable2_bit;
	unsigned int clock_source_addr;
	unsigned int pwm_base;
	unsigned int (*pin_func)[3];
};

static unsigned int pin_func_arr[MAX_PWM_CH][3] = {
/*    flag     addr   func_num */
	{0,0x102000AC,0x2},	/* pwm channel 0 G2_PAD18 */
	{0,0x10200078,0x2},	/* pwm channel 1 G2_PAD5 */
	//{0,0x10200060,0x3},	/* pwm channel 2 */
	//{0,0x1020005c,0x3},	/* pwm channel 3 */

};

static int molchip_pwm_open(struct udevice *dev, bool enable)
{
	struct molchip_pwm_priv *priv = dev_get_priv(dev);
	unsigned int val = 0;

	if (enable == true) {
		/* enable pwm */
		val = readl(priv->enable1_addr);
		val = val | (1 << priv->enable1_bit);
		writel(val, priv->enable1_addr);
		if (val != readl(priv->enable1_addr)) {
			printf("error: pwm open config1 fail!\r\n");
			return -EPERM;
		}

		val = readl(priv->enable2_addr);
		val = val | (1 << priv->enable2_bit);
		writel(val, priv->enable2_addr);
		if (val != readl(priv->enable2_addr)) {
			printf("error: pwm open config2 fail!\r\n");
			return -EPERM;
		}

		/* disable all pwm interrupt */
		writel(0x0, priv->pwm_base + PWM_INT_EN);
	} else {
		/* disable pwm */
		val = readl(priv->enable1_addr);
		val = val & (~(1 << priv->enable1_bit));
		writel(val, priv->enable1_addr);
		if (val != readl(priv->enable1_addr)) {
			printf("error: pwm open config1 fail!\r\n");
			return -EPERM;
		}

		val = readl(priv->enable2_addr);
		val = val & (~(1 << priv->enable2_bit));
		writel(val, priv->enable2_addr);
		if (val != readl(priv->enable2_addr)) {
			printf("error: pwm open config2 fail!\r\n");
			return -EPERM;
		}
	}

	return 0;
}

static int molchip_pwm_set_config(struct udevice *dev, uint channel,
				uint period_ns, uint duty_ns)
{
	struct molchip_pwm_priv *priv = dev_get_priv(dev);
	unsigned int clk_fre=0,val=0,min_ns=0;
	unsigned long long max_ns=0;

	/* determine whether the channel is valid */
	if (**(priv->pin_func + channel) == 0) {
		printf("error: channel invaild\r\n");
		return -EINVAL;
	}

	/* determine whether the pin function is valid*/
	val = readl(*(*(priv->pin_func + channel)+1));
	if (*(*(priv->pin_func + channel)+2) != val) {
		printf("error: PWM Pin function invaild\r\n");
		return -EINVAL;
	}

	val= readl(priv->clock_source_addr);
	switch (val) {
	case 0:
		clk_fre = 24; 	/*24MHz*/
		break;
	case 1:
		clk_fre = 50; 	/*50MHz*/
		break;
	case 2:
		clk_fre = 100; 	/*100MHz*/
		break;
	default:
		printf("error: PWM clock invaild\r\n");
		return -EINVAL;
	}

	max_ns= (0x2000000ULL-1) * 1000;
	max_ns = (max_ns/clk_fre);
	min_ns = (1000/clk_fre);
	if ((period_ns > max_ns) || (period_ns < min_ns)) {
		printf("error: PWM Improper clock frequency\r\n");
		return -EINVAL;
	}

	if ((duty_ns > max_ns) || (duty_ns < min_ns)) {
		printf("error: PWM Duty invaild\r\n");
		return -EINVAL;
	}

	/* disable the channel configure */
	val = readl(priv->pwm_base + PWM_CONFIG_EN);
	val = val & (~(1 << channel));
	writel(val, priv->pwm_base + PWM_CONFIG_EN);

	priv->regs = (void *)(priv->pwm_base + (channel + 1) * 0x80);

	/**
	 * When enabled, output immediately and infinitely,
	 * and the pin keeps low level after stop.
	 */
	 priv->regs->ch_base = 0x1;

	 /* config period */
	val = (period_ns * clk_fre)/1000;
	if (!val) {
		printf("error: PWM The period is in the critical state\r\n");
		return -EINVAL;
	}
	priv->regs->period = val;

	/* config duty */
	if (duty_ns > period_ns) {
		printf("error: PWM duty greater than period\r\n");
		return -EINVAL;
	}
	val = (duty_ns * clk_fre)/1000;
	priv->regs->duty = val;

	/* enable the channel configure */
	val = readl(priv->pwm_base + PWM_CONFIG_EN);
	val = val | (1 << channel);
	writel(val, priv->pwm_base + PWM_CONFIG_EN);

	return 0;
}

static int molchip_pwm_set_enable(struct udevice *dev, uint channel,
				 bool enable)
{
	struct molchip_pwm_priv *priv = dev_get_priv(dev);
	unsigned int val = 0;

	if ( enable == true) {
		val = readl(priv->pwm_base + PWM_CH_EN);
		val = val | (1 << channel);
		writel(val, priv->pwm_base + PWM_CH_EN);
		if (val != readl(priv->pwm_base + PWM_CH_EN))
			return -EPERM;
	} else {
		val = readl(priv->pwm_base + PWM_CH_EN);
		val = val & (~(1 << channel));
		writel(val, priv->pwm_base + PWM_CH_EN);
		if (val != readl(priv->pwm_base + PWM_CH_EN))
			return -EPERM;
	}

	return 0;
}

static int molchip_pwm_probe(struct udevice *dev)
{
	struct molchip_pwm_priv *priv = dev_get_priv(dev);
	int pwm_node = fdt_path_offset(gd->fdt_blob, "/pwm");
	unsigned int pwm_ch=0;
	int i=0;

	priv->pwm_base = 0;
	priv->pwm_base = (unsigned int)devfdt_get_addr(dev);
	if (priv->pwm_base == 0) {
		printf("pwm base address not found!\r\n");
		return -EINVAL;
	}

	priv->enable1_addr = ENABLE1_ADDR;
	priv->enable2_addr = ENABLE2_ADDR;
	priv->enable1_bit = ENABLE1_BIT;
	priv->enable2_bit = ENABLE2_BIT;
	priv->clock_source_addr = CLOCK_SOURCE_FRE_ADD;
	priv->pin_func = pin_func_arr;

	pwm_ch = fdtdec_get_uint(gd->fdt_blob, pwm_node, "molchip,pwm-channel", 0);
	for (i=0; i<MAX_PWM_CH; i++)
	{
		if (((pwm_ch >> i) & 0x1) == 0x1) {
			**(priv->pin_func + i) = 1;
			writel(*(*(priv->pin_func + i) + 2), *(*(priv->pin_func + i) + 1));
		}
	}

	return 0;
}

static const struct pwm_ops molchip_pwm_ops = {
	.udev_open	= molchip_pwm_open,
	.set_config	= molchip_pwm_set_config,
	.set_enable	= molchip_pwm_set_enable,
};

static const struct udevice_id molchip_pwm_match[] = {
	{ .compatible = "molchip,mc-pwm" },
	{ }
};

U_BOOT_DRIVER(molchip_pwm) = {
	.name	= "molchip_pwm",
	.id	= UCLASS_PWM,
	.of_match = molchip_pwm_match,
	.ops	= &molchip_pwm_ops,
	.probe	= molchip_pwm_probe,
	.priv_auto_alloc_size	= sizeof(struct molchip_pwm_priv),
};

static int pwm_test(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	struct udevice *dev;
	uint period_ns, duty_ns,chn;

	if (argc != 4) {
		printf("Usage: test_pwm <chn> <period_ns> <duty_ns>\n");
		return -1;
	}

	chn = simple_strtoul(argv[1], NULL, 10);
	period_ns = simple_strtoul(argv[2], NULL, 10);
	duty_ns = simple_strtoul(argv[3], NULL, 10);

	if (period_ns == 0 || duty_ns == 0 || duty_ns > period_ns) {
		printf("Invalid arguments: period_ns must be > 0 and duty_ns must be <= period_ns\n");
		return -1;
	}

	dev = pwm_get_dev_by_name("molchip_pwm");

	if (!dev) {
		printf("molchip_pwm device not found\n");
		return -1;
	}

	pwm_open(dev, true);
	pwm_set_enable(dev, chn, 0);

	if (pwm_set_config(dev, chn, period_ns, duty_ns)) {
		printf("Failed to configure PWM\n");
		return -1;
	}

	if (pwm_set_enable(dev, chn, true)) {
		printf("Failed to enable PWM\n");
		return -1;
	}

	printf("PWM configured: period_ns = %u, duty_ns = %u\n", period_ns, duty_ns);
	return 0;
}

U_BOOT_CMD(
	test_pwm, 4, 1, pwm_test,
	"test the pwm",
	""
);
