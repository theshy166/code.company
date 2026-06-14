// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#define MINI_CLOCK_API
#ifdef MINI_CLOCK_API
#include <linux/mini_clock/clock.h>
#else
#include <linux/clk.h>
#endif
#include <linux/io.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>


#define PTS_VALUE_LO		(0xC8)
#define PTS_VALUE_HI		(0xCC)

#define PTS_DEFAULT_FREQ	(1000000)

/* write any value can trigger pts get snapshoot, here we use 0*/
#define PTS_TRIGGER_VALUE	(0)

struct molchip_pts {
	spinlock_t lock;
	void __iomem *pts_base_va;
	struct regmap *pts_val_regmap;
	#ifndef MINI_CLOCK_API
	struct clk *clk;
	struct clk *apb_clk;
	#endif
};

static struct molchip_pts mol_pts;

unsigned long long mol_pts_read(void)
{
	unsigned long long val64 = 0;
	unsigned int low_val, hi_val;

	spin_lock(&mol_pts.lock);

	writel(PTS_TRIGGER_VALUE, mol_pts.pts_base_va);
	regmap_read(mol_pts.pts_val_regmap, PTS_VALUE_HI, &hi_val);
	regmap_read(mol_pts.pts_val_regmap, PTS_VALUE_LO, &low_val);

	val64 = hi_val;
	val64 = val64 << 32;
	val64 |= low_val;

	spin_unlock(&mol_pts.lock);
	return val64;
}
EXPORT_SYMBOL(mol_pts_read);

unsigned long long pts_get_value_us(void)
{
	return mol_pts_read();
}
EXPORT_SYMBOL(pts_get_value_us);

unsigned long long pts_get_value_ms(void)
{
	return div_u64(mol_pts_read(), 1000);
}
EXPORT_SYMBOL(pts_get_value_ms);

static int molchip_pts_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t molchip_pts_read(struct file *file, char __user *userbuf, size_t count, loff_t *ppos)
{
	unsigned long long val64 = 0;

	val64 = mol_pts_read();
	if (copy_to_user(userbuf, &val64, 8))
		return -EFAULT;

	return 0;
}

static const struct file_operations molchip_pts_fops = {
	.owner = THIS_MODULE,
	.open = molchip_pts_open,
	.read = molchip_pts_read,
};

static int molchip_pts_probe(struct platform_device *pdev)
{
	struct miscdevice *my_misc_device;
	int ret;
	struct resource *res;

#ifdef MINI_CLOCK_API

	ret = clk_mini_enable(CKG_PTS_EN);
	if (ret)
		return -1;
	ret = clk_mini_enable(CKG_APB_PTS_EN);
	if (ret)
		return -1;

	if (!of_property_read_u32(pdev->dev.of_node, "clock-freq", &ret))
		ret = clk_mini_set_rate(CKG_MUX_PTS, ret);
	else
		ret = clk_mini_set_rate(CKG_MUX_PTS, PTS_DEFAULT_FREQ);
	if (ret) {
		dev_err(&pdev->dev, "failed to set ckg_mux_pts\n");
		return ret;
	}

#else
	struct clk *clk;

	mol_pts.clk = devm_clk_get(&pdev->dev, "ckg_pts_en");
	if (IS_ERR(mol_pts.clk)) {
		dev_err(&pdev->dev, "ckg_pts_en clock not found. %p\n",
			&pdev->dev);
		ret = PTR_ERR(mol_pts.clk);
		return ret;
	}

	ret = clk_prepare_enable(mol_pts.clk);
	if (ret) {
		dev_err(&pdev->dev, "Unable to enable ckg_pts_en clock.\n");
		return ret;
	}

	mol_pts.apb_clk = devm_clk_get(&pdev->dev, "ckg_apb_pts_en");
	if (IS_ERR(mol_pts.apb_clk)) {
		dev_err(&pdev->dev, "ckg_apb_pts_en clock not found.\n");
		ret = PTR_ERR(mol_pts.apb_clk);
		return ret;
	}

	ret = clk_prepare_enable(mol_pts.apb_clk);
	if (ret) {
		dev_err(&pdev->dev, "Unable to enable ckg_apb_pts_en clock.\n");
		return ret;
	}

	clk = devm_clk_get(&pdev->dev, "ckg_mux_pts");
	if (!IS_ERR_OR_NULL(clk)) {
		if (!of_property_read_u32(pdev->dev.of_node, "clock-freq", &ret))
			ret = clk_set_rate(clk, ret);
		else
			ret = clk_set_rate(clk, PTS_DEFAULT_FREQ);
		if (ret) {
			dev_err(&pdev->dev, "failed to set ckg_mux_pts\n");
			return ret;
		}
	} else {
		dev_err(&pdev->dev, "pts_mux_pts not found. %p\n", &pdev->dev);
	}
#endif

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "not provide mem resource\n");
		return -ENODEV;
	}
	mol_pts.pts_base_va = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(mol_pts.pts_base_va))
		return PTR_ERR(mol_pts.pts_base_va);

	mol_pts.pts_val_regmap = syscon_regmap_lookup_by_phandle(
		pdev->dev.of_node, "molchip,syscon-pts-value");
	if (IS_ERR(mol_pts.pts_val_regmap))
		return PTR_ERR(mol_pts.pts_val_regmap);

	my_misc_device = devm_kzalloc(&pdev->dev, sizeof(struct miscdevice),
		GFP_KERNEL);
	if (!my_misc_device)
		return -ENOMEM;

	my_misc_device->minor = MISC_DYNAMIC_MINOR;
	my_misc_device->name = "mol_pts";
	my_misc_device->fops = &molchip_pts_fops;

	dev_set_drvdata(&pdev->dev, my_misc_device);

	ret = misc_register(my_misc_device);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register misc device\n");
		return ret;
	}

	return 0;
}

static int molchip_pts_remove(struct platform_device *pdev)
{
	struct miscdevice *my_misc_device = dev_get_drvdata(&pdev->dev);

	misc_deregister(my_misc_device);
	return 0;
}

static const struct of_device_id molchip_pts_of_match[] = {
	{.compatible = "molchip,pts"},
	{},
};

MODULE_DEVICE_TABLE(of, molchip_pts_of_match);

static struct platform_driver molchip_pts_driver = {
	.driver = {
		.name = "molchip-pts-device",
		.of_match_table = molchip_pts_of_match,
	},
	.probe = molchip_pts_probe,
	.remove = molchip_pts_remove,
};

module_platform_driver(molchip_pts_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siyuan Song");
MODULE_DESCRIPTION("Molchip Pts Driver");
