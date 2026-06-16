#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mfd/syscon.h>
#include <linux/hwspinlock.h>

#define LOCK_OFFSET		0x1000
#define SPINLOCK_REL		0x55AA10C5	/* release */
#define SPINLOCK_NUM_MAX	32

struct hwspinlock_data {
	void __iomem *base;
	struct regmap *regmap;
	unsigned int spinlock_flag;
	unsigned int sw_reg;
	unsigned int sw_bit;
};

static struct hwspinlock_data hwsl_dat;

int hwspinlock_init(struct hwspinlock_data *lock)
{
	struct hwspinlock_data *hwsl=lock;
	int i;
	/* open hwspinclock switch */
	regmap_update_bits(hwsl->regmap, hwsl->sw_reg,
			(1 << hwsl->sw_bit), (1 << hwsl->sw_bit));
	/* unlock all hwspinclock*/
	hwsl->spinlock_flag=0x0;
	for(i = 1; i <= SPINLOCK_NUM_MAX; i++)
	{
		writel(SPINLOCK_REL,hwsl->base + LOCK_OFFSET * i);
	}

	return 0;
}

unsigned int hwspinlock_lock(void)
{
	struct hwspinlock_data *hwsl = &hwsl_dat;
	unsigned int i,num=0;

	for(i = 0; i < SPINLOCK_NUM_MAX; i++)
	{
		if(hwsl->spinlock_flag & (1 << i))
			continue;

		hwsl->spinlock_flag |= (1 << i);
		num = i + 1;
		readl(hwsl->base + LOCK_OFFSET * num);

		return num;
	}

	return num;
}

int hwspinlock_unlock(unsigned int num)
{
	struct hwspinlock_data *hwsl = &hwsl_dat;
	if((!(hwsl->spinlock_flag & (1 << (num -1)))) || (num > 32))
		return -EINVAL;
	hwsl->spinlock_flag &= (~(1 << (num -1)));
	writel(SPINLOCK_REL,hwsl->base + LOCK_OFFSET * num);

	return 0;
}

void __iomem *get_hwspinlock_base(void)
{
	return hwsl_dat.base;
}

static int hwspinlock_probe(struct platform_device *pdev)
{
	struct hwspinlock_data *hwsl = &hwsl_dat;
	struct device *dev = &pdev->dev;
	struct resource *res;
	int ret;

	if (!hwsl)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	hwsl->base = ioremap(res->start, resource_size(res));
	if (!hwsl->base)
		return -ENOMEM;

	if (of_find_property(dev->of_node, "mc,syscon", NULL)) {
		hwsl->regmap = syscon_regmap_lookup_by_phandle(dev->of_node, "mc,syscon");
		if (IS_ERR_OR_NULL(hwsl->regmap)) {
			dev_err(dev, "%s: failed to get syscon regmap\n", __func__);
			return -ENXIO;
		}

		ret = of_property_read_u32_index(dev->of_node, "sw_con", 0, &hwsl->sw_reg);
		if (ret < 0) {
			dev_err(dev, "the offset address of switch control not found!\n");
			return ret;
		}

		ret = of_property_read_u32_index(dev->of_node, "sw_con", 1, &hwsl->sw_bit);
		if (ret < 0) {
			dev_err(dev, "the bit of switch control not found!\n");
			return ret;
		}

	} else {
		dev_err(dev, "mc,syscon property not found!\n");
		return -ENXIO;
	}

	platform_set_drvdata(pdev, hwsl);

	hwspinlock_init(hwsl);

	return 0;
}

static const struct of_device_id hwspinlock_of_match[] = {
	{ .compatible = "molchip,spinlock", },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, hwspinlock_of_match);

static struct platform_driver hwspinlock_driver = {
		.driver = {
		.name   = "hwspinlock",
		.of_match_table = of_match_ptr(hwspinlock_of_match),
	},
	.probe  = hwspinlock_probe,
};

module_platform_driver(hwspinlock_driver);

MODULE_DESCRIPTION("Hardware spinlock driver for MOLCHIP");
MODULE_AUTHOR("www.molchip.com");
MODULE_LICENSE("GPL");

