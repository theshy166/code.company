// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/mini_clock/clock.h>
#include <linux/io.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/interrupt.h>
#include <linux/pm_wakeirq.h>
#include <linux/syscore_ops.h>
#define CKG_SYST_CFG			(0x238)
#define CLK_SRC_32K			(0x0)
#define CLK_SRC_24M			(0x2)

#define AP_PERI_SOFT_RST0		(0xB4)
#define SYST_SOFT_RST			(BIT(16))

#define USE_SYSTIMER_HW_MS

#define SYS_TIMER_ALM			(0x0)
#define SYS_TIMER_CNT			(0x4)
#define APB_WR_INT			(0x8)
#define SYS_TIMER_INT_CLR		(BIT(3))
#define SYS_TIMER_INT_MASK		(BIT(2))
#define SYS_TIMER_INT_RAW		(BIT(1))
#define SYS_TIMER_INT_EN		(BIT(0))
#define SYS_TIMER_CNT_SHDW		(0xC)
#define SYS_TIMER_VALUE_MIN		(0x10)
#define SYS_TIMER_VALUE_MAX		(0x14)

#define TIMER_VALUE_MIN_MS		(2000)
#define TIMER_VALUE_MIN_US		(2667)
#define TIMER_VALUE_MAX			(32768)

struct molchip_syst {
	spinlock_t lock;
	struct device *dev;
	void __iomem *syst_base_va;
	struct regmap *cpu_sys_ahb_regmap;
	struct regmap *cpu_sys_clk_rf_regmap;
};

static struct molchip_syst mol_syst;

static unsigned int mol_syst_read(void)
{
	unsigned int ret;
	unsigned long flags;

	spin_lock_irqsave(&mol_syst.lock, flags);
	ret = __raw_readl(mol_syst.syst_base_va + SYS_TIMER_CNT_SHDW);
	spin_unlock_irqrestore(&mol_syst.lock, flags);

	return ret;
}


#ifndef USE_SYSTIMER_HW_MS
unsigned int syst_get_value_us(void)
{
	return mol_syst_read();
}
EXPORT_SYMBOL(syst_get_value_us);
#endif

unsigned int syst_get_value_ms(void)
{
#ifdef USE_SYSTIMER_HW_MS
	return mol_syst_read();
#else
	return mol_syst_read() / 1000;
#endif
}
EXPORT_SYMBOL(syst_get_value_ms);

void syst_delayms(unsigned int ms)
{
	unsigned int v;
	unsigned int v1, v2;

#ifdef USE_SYSTIMER_HW_MS
	if (ms > 0xFFFFFFFF) {
#else
	if (ms > (0xFFFFFFFF / 1000)) {
#endif
		pr_err("delay parm exceed max \r\n");
		return;
	}


	v1 = mol_syst_read();
	do {
		v2 = mol_syst_read();
		if (v2 >= v1)
			v = v2 - v1;
		else
			v = (0xFFFFFFFFU - v1) + v2;
	#ifdef USE_SYSTIMER_HW_MS
	} while (v < ms);
	#else
	} while (v < 1000 * ms);
	#endif
}
EXPORT_SYMBOL(syst_delayms);

void syst_set_alarm(unsigned int ms)
{
	int ret;

#ifdef USE_SYSTIMER_HW_MS
	if (ms > 0xFFFFFFFF) {
#else
	if (ms > (0xFFFFFFFF / 1000)) {
#endif
		pr_err("delay parm exceed max \r\n");
		goto exit1;
	}

	__raw_writel(0, mol_syst.syst_base_va + APB_WR_INT);

	ret = regmap_update_bits(mol_syst.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		SYST_SOFT_RST, SYST_SOFT_RST);
	if (ret)
		goto exit1;

	ret = regmap_update_bits(mol_syst.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		SYST_SOFT_RST, ~SYST_SOFT_RST);
	if (ret)
		goto exit1;

#ifdef USE_SYSTIMER_HW_MS
	__raw_writel(ms, mol_syst.syst_base_va + SYS_TIMER_ALM);
#else
	__raw_writel(ms * 1000, mol_syst.syst_base_va + SYS_TIMER_ALM);
#endif

	__raw_writel(SYS_TIMER_INT_EN, mol_syst.syst_base_va + APB_WR_INT);

exit1:
	return;
}
EXPORT_SYMBOL(syst_set_alarm);

static int molchip_syst_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t molchip_syst_read(struct file *file, char __user *userbuf, size_t count, loff_t *ppos)
{
	unsigned int val = 0;

	val = syst_get_value_ms();
	if (copy_to_user(userbuf, &val, 4))
		return -EFAULT;

	return 0;
}

static const struct file_operations molchip_syst_fops = {
	.owner = THIS_MODULE,
	.open = molchip_syst_open,
	.read = molchip_syst_read,
};

static irqreturn_t molchip_syst_isr(int irq, void *dev_id)
{
	struct molchip_syst *info = (struct molchip_syst *)dev_id;
	u32 int_sts = readl_relaxed(info->syst_base_va + APB_WR_INT);

	if (int_sts & SYS_TIMER_INT_RAW) {
		int_sts |= SYS_TIMER_INT_CLR;
		int_sts &= ~(SYS_TIMER_INT_EN);
		writel_relaxed(int_sts, info->syst_base_va + APB_WR_INT);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static ssize_t syst_debug_store(struct device *dev,
       struct device_attribute *attr, const char *buf, size_t size)
{

	unsigned int ms;

	if (!buf || size > PAGE_SIZE)
		return -EINVAL;

	ms = simple_strtoul(buf, NULL, 10);
	syst_set_alarm(ms);

	return size;
}
static ssize_t syst_debug_show(struct device *dev,
       struct device_attribute *attr, char *buf)
{
	return 0;
}

static DEVICE_ATTR(alarm_event, 0644, syst_debug_show,syst_debug_store);

static int molchip_syst_probe(struct platform_device *pdev)
{
	struct miscdevice *my_misc_device;
	int ret, irq;
	struct resource *res;
	bool wakeup;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "not provide mem resource\n");
		return -ENODEV;
	}

	mol_syst.dev = &pdev->dev;

	mol_syst.syst_base_va = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(mol_syst.syst_base_va))
		return PTR_ERR(mol_syst.syst_base_va);

	mol_syst.cpu_sys_ahb_regmap = syscon_regmap_lookup_by_phandle(
		pdev->dev.of_node, "mol,syscon");
	if (IS_ERR(mol_syst.cpu_sys_ahb_regmap))
		return PTR_ERR(mol_syst.cpu_sys_ahb_regmap);

	mol_syst.cpu_sys_clk_rf_regmap = syscon_regmap_lookup_by_phandle(
		pdev->dev.of_node, "molchip,cpu-clk-rf-syscon");
	if (IS_ERR(mol_syst.cpu_sys_clk_rf_regmap))
		return PTR_ERR(mol_syst.cpu_sys_clk_rf_regmap);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		ret = irq;
		dev_err(&pdev->dev, "no irq resource?\n");
		goto err;
	}

	ret = devm_request_irq(&pdev->dev, irq, molchip_syst_isr,
		IRQF_SHARED | IRQF_EARLY_RESUME, dev_name(&pdev->dev), &mol_syst);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed requesting irq %d\n", irq);
		goto err;
	}

	my_misc_device = devm_kzalloc(&pdev->dev, sizeof(struct miscdevice),
		GFP_KERNEL);
	if (!my_misc_device)
		return -ENOMEM;

	my_misc_device->minor = MISC_DYNAMIC_MINOR;
	my_misc_device->name = "mol_syst";
	my_misc_device->fops = &molchip_syst_fops;

	dev_set_drvdata(&pdev->dev, my_misc_device);


	ret = misc_register(my_misc_device);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register misc device\n");
		goto err;
	}

	ret = clk_mini_disable(CKG_SYST_EN);
	if (ret)
		goto err;

	ret = clk_mini_disable(CKG_APB_SYST_EN);
	if (ret)
		goto err;

#ifdef USE_SYSTIMER_HW_MS
	ret = regmap_write(mol_syst.cpu_sys_clk_rf_regmap, CKG_SYST_CFG,
		CLK_SRC_32K);
#else
	ret = regmap_write(mol_syst.cpu_sys_clk_rf_regmap, CKG_SYST_CFG,
		CLK_SRC_24M);
#endif
	if (ret)
		goto err;

	ret = clk_mini_enable(CKG_APB_SYST_EN);
	if (ret)
		goto err;

	ret = regmap_update_bits(mol_syst.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		SYST_SOFT_RST, SYST_SOFT_RST);
	if (ret)
		goto exit;

	ret = regmap_update_bits(mol_syst.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		SYST_SOFT_RST, ~SYST_SOFT_RST);
	if (ret)
		goto exit;

#ifdef USE_SYSTIMER_HW_MS
	__raw_writel(TIMER_VALUE_MIN_MS,
		mol_syst.syst_base_va + SYS_TIMER_VALUE_MIN);
#else
	__raw_writel(TIMER_VALUE_MIN_US,
		mol_syst.syst_base_va + SYS_TIMER_VALUE_MIN);
#endif

	__raw_writel(TIMER_VALUE_MAX, mol_syst.syst_base_va +
		SYS_TIMER_VALUE_MAX);

	ret = clk_mini_enable(CKG_SYST_EN);
	if (ret)
		goto exit;

	wakeup = device_property_read_bool(&pdev->dev, "wakeup-source");
	if (wakeup) {
			ret = device_init_wakeup(&pdev->dev, true);
			if (ret)
					goto exit;
			ret = dev_pm_set_wake_irq(&pdev->dev, irq);
			if (ret)
					goto exit;
	}

	device_create_file(&pdev->dev, &dev_attr_alarm_event);

	return 0;
exit:
	clk_mini_disable(CKG_APB_SYST_EN);
err:
	return ret;
}

static int molchip_syst_remove(struct platform_device *pdev)
{
	struct miscdevice *my_misc_device = dev_get_drvdata(&pdev->dev);

	misc_deregister(my_misc_device);
	clk_mini_disable(CKG_SYST_EN);
	clk_mini_disable(CKG_APB_SYST_EN);

	return 0;
}

static const struct of_device_id molchip_syst_of_match[] = {
	{.compatible = "mol,systimer"},
	{},
};

MODULE_DEVICE_TABLE(of, molchip_syst_of_match);

static struct platform_driver molchip_syst_driver = {
	.driver = {
		.name = "molchip-syst-device",
		.of_match_table = molchip_syst_of_match,
	},
	.probe = molchip_syst_probe,
	.remove = molchip_syst_remove,
};

static int mol_syst_suspend(void)
{
	clk_mini_disable(CKG_APB_SYST_EN);
	if (!device_may_wakeup(mol_syst.dev)) {
		clk_mini_disable(CKG_SYST_EN);
	}
	return 0;
}

static void  mol_syst_resume(void)
{
	clk_mini_enable(CKG_APB_SYST_EN);
	if (!device_may_wakeup(mol_syst.dev)) {
		clk_mini_enable(CKG_SYST_EN);
	}
}

static struct syscore_ops mol_syst_pm_ops = {
	.resume = mol_syst_resume,
	.suspend = mol_syst_suspend,
};

static int __init mc_syst_init(void)
{
	int ret;

	register_syscore_ops(&mol_syst_pm_ops);

	ret = platform_driver_register(&molchip_syst_driver);
	if (ret) {
		pr_err("Failed to register efuse driver\n");
		return ret;
	}

	return 0;
}

static void __exit mc_syst_exit(void)
{
	unregister_syscore_ops(&mol_syst_pm_ops);

	return platform_driver_unregister(&molchip_syst_driver);
}

subsys_initcall(mc_syst_init);
module_exit(mc_syst_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jerry Cao");
MODULE_DESCRIPTION("Molchip systimer Driver");
