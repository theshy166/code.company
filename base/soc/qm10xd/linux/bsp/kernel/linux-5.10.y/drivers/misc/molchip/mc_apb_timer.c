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


#define AP_PERI_SOFT_RST0		(0xB4)
#define TIMER0_RTC_SOFT_RST		(BIT(19))
#define TIMER0_XTL_SOFT_RST		(BIT(18))

#define TIMER0_CLK_FREQ			(32768)
#define TIMER0_MS_CNT			(TIMER0_CLK_FREQ / 1000)
#define TIMER1_CLK_FREQ			(32768)
#define TIMER1_MS_CNT			(TIMER1_CLK_FREQ / 1000)
#define TIMER2_CLK_FREQ			(24000000)
#define TIMER2_US_CNT			(TIMER2_CLK_FREQ / 1000000)

#define TMR0_LOAD_LO			(0x0000)
#define TMR0_LOAD_HI			(0x0004)
#define TMR0_VALUE_LO			(0x0008)
#define TMR0_VALUE_HI			(0x000c)
#define TMR0_CTL			(0x0010)
#define TMR0_INT			(0x0014)
#define TMR0_VALUE_SHDW_LO		(0x0018)
#define TMR0_VALUE_SHDW_HI		(0x001c)
#define TMR1_LOAD_LO			(0x0020)
#define TMR1_LOAD_HI			(0x0024)
#define TMR1_VALUE_LO			(0x0028)
#define TMR1_VALUE_HI			(0x002c)
#define TMR1_CTL			(0x0030)
#define TMR1_INT			(0x0034)
#define TMR1_VALUE_SHDW_LO		(0x0038)
#define TMR1_VALUE_SHDW_HI		(0x003c)
#define TMR2_LOAD_LO			(0x0040)
#define TMR2_LOAD_HI			(0x0044)
#define TMR2_VALUE_LO			(0x0048)
#define TMR2_VALUE_HI			(0x004c)
#define TMR2_CTL			(0x0050)
#define TMR2_INT			(0x0054)
#define TMR2_VALUE_SHDW_LO		(0x0058)
#define TMR2_VALUE_SHDW_HI		(0x005c)

/*1: 64 bit,  0:32 bit*/
#define TMRn_CTRL_TMRn_WIDTH_SEL	(BIT(16))
/*1: Enabled,  0: Disabled*/
#define TMRn_CTRL_TMRn_RUN		(BIT(1))
/*1:Period mode  0: one-time mode*/
#define TMRn_CTRL_TMRn_MODE		(BIT(0))

/*irq interrupt clear, 1: clear, write only*/
#define TMRn_INT_TMRn_IRQ_CLR		(BIT(3))
/*irq interrupt masked status, read only*/
#define TMRn_INT_TMRn_IRQ_MASK		(BIT(2))
/*irq interrupt raw, read only*/
#define TMRn_INT_TMRn_IRQ_RAW		(BIT(1))
/*irq interrupt enabled, r/w*/
#define TMRn_INT_TMRn_IRQ_EN		(BIT(0))


struct molchip_apb_timer {
	spinlock_t lock;
	void __iomem *apb_timer_base_va;
	struct regmap *cpu_sys_ahb_regmap;
};

static struct molchip_apb_timer mol_apb_timer;

static unsigned int mol_apb_timer_read(void)
{
	unsigned int ret;
	unsigned long flags;

	spin_lock_irqsave(&mol_apb_timer.lock, flags);
	ret = __raw_readl(mol_apb_timer.apb_timer_base_va + TMR2_VALUE_SHDW_LO);
	spin_unlock_irqrestore(&mol_apb_timer.lock, flags);

	return ret;
}

unsigned int apb_timer_get_value_us(void)
{
	return mol_apb_timer_read() / TIMER2_US_CNT;
}
EXPORT_SYMBOL(apb_timer_get_value_us);

unsigned int apb_timer_get_value_ms(void)
{
	return apb_timer_get_value_us() / 1000;
}
EXPORT_SYMBOL(apb_timer_get_value_ms);

void apb_timer_delayus(unsigned int us)
{
	unsigned int delta, cur, past = __raw_readl(
		mol_apb_timer.apb_timer_base_va + TMR2_VALUE_SHDW_LO);

	do {
		cur = __raw_readl(mol_apb_timer.apb_timer_base_va +
			TMR2_VALUE_SHDW_LO);
		if (cur <= past)
			delta = (past - cur) / TIMER2_US_CNT;
		else
			delta = (0xFFFFFFFFU - cur + past) / TIMER2_US_CNT;
	} while (delta < us);
}
EXPORT_SYMBOL(apb_timer_delayus);

void apb_timer_set_alarm(unsigned int us)
{
	int ret;

	if (us > 0xFFFFFFFF) {
		pr_err("delay parm exceed max \r\n");
		goto exit1;
	}

	__raw_writel(0, mol_apb_timer.apb_timer_base_va + TMR2_INT);

	__raw_writel(0, mol_apb_timer.apb_timer_base_va + TMR2_CTL);

	ret = regmap_update_bits(mol_apb_timer.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		TIMER0_XTL_SOFT_RST, TIMER0_XTL_SOFT_RST);
	if (ret)
		goto exit1;

	ret = regmap_update_bits(mol_apb_timer.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		TIMER0_XTL_SOFT_RST, ~TIMER0_XTL_SOFT_RST);
	if (ret)
		goto exit1;

	__raw_writel(us, mol_apb_timer.apb_timer_base_va + TMR2_LOAD_LO);

	__raw_writel(TMRn_INT_TMRn_IRQ_EN,
		mol_apb_timer.apb_timer_base_va + TMR2_INT);

	__raw_writel(TMRn_CTRL_TMRn_RUN | TMRn_CTRL_TMRn_MODE,
		mol_apb_timer.apb_timer_base_va + TMR2_CTL);

exit1:
	return;
}
EXPORT_SYMBOL(apb_timer_set_alarm);


static int molchip_apb_timer_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t molchip_apb_timer_read(struct file *file, char __user *userbuf, size_t count, loff_t *ppos)
{
	unsigned int val = 0;

	val = apb_timer_get_value_us();
	if (copy_to_user(userbuf, &val, 4))
		return -EFAULT;

	return 0;
}

static const struct file_operations molchip_apb_timer_fops = {
	.owner = THIS_MODULE,
	.open = molchip_apb_timer_open,
	.read = molchip_apb_timer_read,
};

static irqreturn_t molchip_apb_timer_isr(int irq, void *dev_id)
{
	struct molchip_apb_timer *info = (struct molchip_apb_timer *)dev_id;
	u32 int_sts = readl_relaxed(info->apb_timer_base_va + TMR2_INT);

	if (int_sts & TMRn_INT_TMRn_IRQ_RAW) {
		int_sts |= TMRn_INT_TMRn_IRQ_CLR;
		writel_relaxed(int_sts, info->apb_timer_base_va + TMR2_INT);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static int molchip_apb_timer_probe(struct platform_device *pdev)
{
	struct miscdevice *my_misc_device;
	int ret, irq;
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "not provide mem resource\n");
		return -ENODEV;
	}

	mol_apb_timer.apb_timer_base_va = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(mol_apb_timer.apb_timer_base_va))
		return PTR_ERR(mol_apb_timer.apb_timer_base_va);

	mol_apb_timer.cpu_sys_ahb_regmap = syscon_regmap_lookup_by_phandle(
		pdev->dev.of_node, "mol,syscon");
	if (IS_ERR(mol_apb_timer.cpu_sys_ahb_regmap))
		return PTR_ERR(mol_apb_timer.cpu_sys_ahb_regmap);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		ret = irq;
		dev_err(&pdev->dev, "no irq resource?\n");
		goto err0;
	}

	ret = devm_request_irq(&pdev->dev, irq, molchip_apb_timer_isr,
		IRQF_SHARED, dev_name(&pdev->dev), &mol_apb_timer);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed requesting irq %d\n", irq);
		goto err0;
	}

	my_misc_device = devm_kzalloc(&pdev->dev, sizeof(struct miscdevice),
		GFP_KERNEL);
	if (!my_misc_device)
		return -ENOMEM;

	my_misc_device->minor = MISC_DYNAMIC_MINOR;
	my_misc_device->name = "mol_apb_timer";
	my_misc_device->fops = &molchip_apb_timer_fops;

	dev_set_drvdata(&pdev->dev, my_misc_device);


	ret = misc_register(my_misc_device);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register misc device\n");
		goto err0;
	}

	ret = clk_mini_enable(CKG_APB_TIMER_EN);
	if (ret)
		goto err0;

	ret = clk_mini_enable(CKG_XTL_TMR_EN);
	if (ret)
		goto err1;

	ret = clk_mini_enable(CKG_RTC_TMR_EN);
	if (ret)
		goto err2;

	ret = regmap_update_bits(mol_apb_timer.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		TIMER0_RTC_SOFT_RST, TIMER0_RTC_SOFT_RST);
	if (ret)
		goto err3;

	ret = regmap_update_bits(mol_apb_timer.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		TIMER0_RTC_SOFT_RST, ~TIMER0_RTC_SOFT_RST);
	if (ret)
		goto err3;


	ret = regmap_update_bits(mol_apb_timer.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		TIMER0_XTL_SOFT_RST, TIMER0_XTL_SOFT_RST);
	if (ret)
		goto err3;

	ret = regmap_update_bits(mol_apb_timer.cpu_sys_ahb_regmap, AP_PERI_SOFT_RST0,
		TIMER0_XTL_SOFT_RST, ~TIMER0_XTL_SOFT_RST);
	if (ret)
		goto err3;

	__raw_writel(0xFFFFFFFF,
		mol_apb_timer.apb_timer_base_va + TMR2_LOAD_LO);

	__raw_writel(TMRn_INT_TMRn_IRQ_EN,
		mol_apb_timer.apb_timer_base_va + TMR2_INT);

	__raw_writel(TMRn_CTRL_TMRn_RUN | TMRn_CTRL_TMRn_MODE,
		mol_apb_timer.apb_timer_base_va + TMR2_CTL);

	return 0;
err3:
	clk_mini_disable(CKG_RTC_TMR_EN);
err2:
	clk_mini_disable(CKG_XTL_TMR_EN);
err1:
	clk_mini_disable(CKG_APB_TIMER_EN);
err0:
	return ret;
}

static int molchip_apb_timer_remove(struct platform_device *pdev)
{
	struct miscdevice *my_misc_device = dev_get_drvdata(&pdev->dev);

	misc_deregister(my_misc_device);
	clk_mini_disable(CKG_RTC_TMR_EN);
	clk_mini_disable(CKG_XTL_TMR_EN);
	clk_mini_disable(CKG_APB_TIMER_EN);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int molchip_apb_timer_suspend(struct device *dev)
{
	clk_mini_disable(CKG_RTC_TMR_EN);
	clk_mini_disable(CKG_XTL_TMR_EN);
	clk_mini_disable(CKG_APB_TIMER_EN);
	pr_debug("apb_timer_suspend done\r\n");

	return 0;
}

static int molchip_apb_timer_resume(struct device *dev)
{
	clk_mini_enable(CKG_APB_TIMER_EN);
	clk_mini_enable(CKG_XTL_TMR_EN);
	clk_mini_enable(CKG_RTC_TMR_EN);

	return 0;
}
#endif

static const struct of_device_id molchip_apb_timer_of_match[] = {
	{.compatible = "mol,apb_timer"},
	{},
};

MODULE_DEVICE_TABLE(of, molchip_apb_timer_of_match);

static SIMPLE_DEV_PM_OPS(molchip_apb_timer_pm_ops,
			 molchip_apb_timer_suspend, molchip_apb_timer_resume);

static struct platform_driver molchip_apb_timer_driver = {
	.driver = {
		.name = "molchip-apb-timer-device",
		.of_match_table = molchip_apb_timer_of_match,
		.pm	= &molchip_apb_timer_pm_ops,
	},
	.probe = molchip_apb_timer_probe,
	.remove = molchip_apb_timer_remove,
};

module_platform_driver(molchip_apb_timer_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jerry Cao");
MODULE_DESCRIPTION("Molchip apb timer Driver");
