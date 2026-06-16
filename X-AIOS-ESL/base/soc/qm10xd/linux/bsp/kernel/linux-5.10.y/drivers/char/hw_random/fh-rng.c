/*
 * Copyright (C) 2019 Fullhan Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/hw_random.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/random.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/xc-clock-simple/clock.h>

//#define RNG_SEED	0x0
//#define RNG_CTRL	0x4
//  #define RNG_SEED_SEL	BIT(2)
//  #define RNG_RING_EN	BIT(1)
//  #define RNG_EN	BIT(0)
//#define RNG_RAN_NUM	0x10
//#define RNG_PHY_SEED	0x14

#define     TRNG_OUTPUT_0(n)			(0x0000 + 0x80*n)	//n:0~3
#define     TRNG_OUTPUT_1(n)			(0x0004 + 0x80*n)
#define     TRNG_OUTPUT_2(n)			(0x0008 + 0x80*n)
#define     TRNG_OUTPUT_3(n)			(0x000C + 0x80*n)
#define     TRNG_STATUS(n)				(0x0010 + 0x80*n)
#define     TRNG_INTACK(n)				(0x0010 + 0x80*n)

#define     TRNG_CONTROL(n)				(0x0014 + 0x80*n)
#define     TRNG_CONFIG(n)				(0x0018 + 0x80*n)
#define     TRNG_ALARMCNT(n)			(0x001c + 0x80*n)
#define     TRNG_FROENABLE(n)			(0x0020 + 0x80*n)
#define     TRNG_FRODETUNE(n)			(0x0024 + 0x80*n)
#define     TRNG_ALRMMASK(n)			(0x0028 + 0x80*n)
#define     TRNG_ALARMSTOP(n)			(0x002c + 0x80*n)

#define     RO_DATA_TYPE				(0x0200)
#define     RO_DATA_AV_SEL				(0x0204)
#define     RO_AES_CURR_DATA0			(0x0208)
#define     RO_AES_CURR_DATA1			(0x020c)
#define     RO_AES_CURR_DATA2			(0x0210)
#define     RO_AES_CURR_DATA3			(0x0214)
#define     RO_AES_BUFF_STATUS			(0x0218)
#define     RO_AES_EN					(0x0220)
#define     RO0_CURR_DATA				(0x0224)
#define     RO1_CURR_DATA				(0x0228)
#define     RO2_CURR_DATA				(0x022c)
#define     RO3_CURR_DATA				(0x0230)

#define     RO_AES_BUFF(M)				(0x1000 + 0x4*M)	//M:0~1023

#define to_fh_trng(p)	container_of(p, struct fh_trng, rng)

//static int seed_sel;
//module_param(seed_sel, int, S_IRUGO);
//MODULE_PARM_DESC(seed_sel, "Auto reload seed. 0, use LFSR(default); 1, use ring oscillator.");

struct fh_trng {
	void __iomem *base;
	struct hwrng rng;
};

static int fh_rng_init(struct hwrng *rng)
{
	struct fh_trng *trng = to_fh_trng(rng);

	writel(0x30801, trng->base + TRNG_CONFIG(0));
	writel(0x8200ff, trng->base + TRNG_ALARMCNT(0));
	writel(0xff00ff, trng->base + TRNG_FROENABLE(0));
	writel(0x6607, trng->base + TRNG_CONTROL(0));

	writel(0x30801, trng->base + TRNG_CONFIG(1));
	writel(0x8200ff, trng->base + TRNG_ALARMCNT(1));
	writel(0xff00ff, trng->base + TRNG_FROENABLE(1));
	writel(0x6607, trng->base + TRNG_CONTROL(1));

	writel(0x30801, trng->base + TRNG_CONFIG(2));
	writel(0x8200ff, trng->base + TRNG_ALARMCNT(2));
	writel(0xff00ff, trng->base + TRNG_FROENABLE(2));
	writel(0x6607, trng->base + TRNG_CONTROL(2));

	writel(0x30801, trng->base + TRNG_CONFIG(3));
	writel(0x8200ff, trng->base + TRNG_ALARMCNT(3));
	writel(0xff00ff, trng->base + TRNG_FROENABLE(3));
	writel(0x6607, trng->base + TRNG_CONTROL(3));

	writel(1, trng->base + RO_AES_EN);

	return 0;
}

static void fh_rng_cleanup(struct hwrng *rng)
{
	//struct fh_trng *trng = to_fh_trng(rng);

	//do someyhing to close hardware trng
}

static int fh_rng_read(struct hwrng *rng, void *buf, size_t max, bool wait)
{
	struct fh_trng *trng = to_fh_trng(rng);
	uint32_t i=0, cnt=0;
	uint8_t *dst = buf;
	uint32_t src;

	while(1)
	{
		if(0xff == readl(trng->base + RO_AES_BUFF_STATUS))
			break;

		if (++i > 1000)
			return 0;
	}

	i = 0;
	while(i < max)
	{
		if (cnt >= 1024)
		{
			printk("warning: max(%d) already exceed the maximum TRNG buff size(4096) \r\n", max);
			break;
		}
		if (i+4 <= max)
		{
			src = readl(trng->base + RO_AES_BUFF(cnt));
			memcpy(dst+i, (uint8_t *)&src, 4);
			i += 4;
		}
		else
		{
			src = readl(trng->base + RO_AES_BUFF(cnt));
			memcpy(dst+i, (uint8_t *)&src, max-i);
			i = max;
		}
		cnt++;
	}

	//memcpy(buf, (uint8_t *)(trng->base + RO_AES_BUFF(0)), max);
	writel(1, trng->base + RO_AES_BUFF_STATUS);

	return i;
}

static int fh_trng_probe(struct platform_device *pdev)
{

	struct fh_trng *rng;
	int ret;
	struct resource *res;

#if 1
/*
	struct clk *trng_clk = devm_clk_get(&pdev->dev, "trng_clk");;
	if (!IS_ERR(trng_clk)) {
		ret = clk_prepare_enable(trng_clk);
		if (ret) {
			dev_err(&pdev->dev, "failed to enable trng clock\n");
			return ret;
		}
	} else
		dev_warn(&pdev->dev, "Can't find trng_clk!\n");
*/
	clk_mini_enable(CKG_AHB_RNG_EN);
#else
	uint32_t cpu_sys_ahb, addr, val;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 6, 0)
	cpu_sys_ahb = (uint32_t)ioremap_nocache(0x0C000000, 0x100);
#else
	cpu_sys_ahb = (uint32_t)ioremap_wc(0x0C000000, 0x100);
#endif
	addr = cpu_sys_ahb + 0xB0;
	val = readl((const volatile void *)addr);
	writel(val | (1 << 2), (volatile void *)addr);	//trng apb clk,RO
	addr = cpu_sys_ahb + 0xDC;
	val = readl((const volatile void *)addr);
	writel(val | (1 << 24), (volatile void *)addr);	//rng ahb clk enable

	//ahb rng soft reset, 0xB4 bit0
	/*
	addr = cpu_sys_ahb + 0xB4;
	writel(0x1, (volatile void *)addr);
	writel(0x0, (volatile void *)addr);
	*/

	iounmap((void *)cpu_sys_ahb);
#endif

	rng = devm_kzalloc(&pdev->dev, sizeof(*rng), GFP_KERNEL);
	if (!rng)
		return -ENOMEM;

	platform_set_drvdata(pdev, rng);

#if 1
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	rng->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(rng->base))
		return PTR_ERR(rng->base);
#else
	rng->base = (uint32_t)ioremap_nocache(0x0C400000, 0x2000);
#endif

	rng->rng.name = pdev->name;
	rng->rng.init = fh_rng_init;
	rng->rng.cleanup = fh_rng_cleanup;
	rng->rng.read = fh_rng_read;

	ret = devm_hwrng_register(&pdev->dev, &rng->rng);
	if (ret) {
		dev_err(&pdev->dev, "failed to register hwrng\n");
		return ret;
	}

	return 0;
}

static int fh_trng_remove(struct platform_device *pdev)
{
	struct fh_trng *trng = platform_get_drvdata(pdev);

	clk_mini_disable(CKG_AHB_RNG_EN);

	devm_hwrng_unregister(&pdev->dev, &trng->rng);

	return 0;
}

static const struct of_device_id fh_rng_dt_ids[] = {
	{ .compatible = "fh,fh-rng" },
	{ }
};
MODULE_DEVICE_TABLE(of, hisi_rng_dt_ids);

static struct platform_driver fh_rng_driver = {
	.probe		= fh_trng_probe,
	.remove		= fh_trng_remove,
	.driver		= {
		.name	= "fh-rng",
		.of_match_table = of_match_ptr(fh_rng_dt_ids),
	},
};

module_platform_driver(fh_rng_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Circle Xue <xuejy344@fullhan>");
MODULE_DESCRIPTION("Fullhan random number generator driver");
