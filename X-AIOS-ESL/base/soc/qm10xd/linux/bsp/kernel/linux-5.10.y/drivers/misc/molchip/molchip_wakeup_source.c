#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/uaccess.h>  // for copy_from_user
#include <linux/mfd/syscon.h>
#include <linux/slab.h>
#include <linux/molchip.h>
#include <linux/pm_wakeirq.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/mini_clock/clock.h>
#include <dt-bindings/net/molchip-ephy.h>

#define MOL_WAKE_SOURCE_GPIO "gpio"
#define MOL_WAKE_SOURCE_SADC "sadc"
#define MOL_WAKE_SOURCE_ETHERNET "ethernet"
#define MOL_WAKE_SOURCE_SYSTIMER "systimer"
#define MOL_WAKE_SOURCE_EVENT "event"

#define MOL_WAKE_SOURCE_DEVICE_SYSTIMER "9700000.systimer"
#define MOL_WAKE_SOURCE_DEVICE_ETHERNET "b400000.ethernet"
#define MOL_WAKE_SOURCE_DEVICE_SARADC "aa00000.saradc"

#define SOC_GPIO_WAKEUP_SOURCE_MAX		16
#define SOC_GPIO_GROUNP_PINS		8

#define MOL_DEEP_SLEEP_SEL_OFF	(0x0)
#define MOL_PLL_DSLP_SEL	(BIT(0))

#define MOL_GEN_PERI_APB_CLK_CTRL	(0xb0)
#define MOL_INT_CTRL0_ENABLE	(0x9f00008)
#define MOL_INT_CTRL1_ENABLE	(0xa000008)

#define MOL_SYSTIMER_INT_CTRL1_BIT	(BIT(3))
#define MOL_ETHERNET_INT_CTRL1_BIT	(BIT(16))
#define MOL_SADC_INT_CTRL1_BIT	(BIT(13))
#define MOL_GPIO6_INT_CTRL1_BIT	(BIT(11))
#define MOL_GPIO7_INT_CTRL1_BIT	(BIT(14))

#define MOL_GPIO0_INT_CTRL0_BIT	(BIT(24))
#define MOL_GPIO1_INT_CTRL0_BIT	(BIT(25))
#define MOL_GPIO2_INT_CTRL0_BIT	(BIT(26))
#define MOL_GPIO3_INT_CTRL0_BIT	(BIT(27))
#define MOL_GPIO4_INT_CTRL0_BIT	(BIT(28))
#define MOL_GPIO5_INT_CTRL0_BIT	(BIT(29))

struct mol_pm_wake_priv {
	void * __iomem int_ctrl0;
	void * __iomem int_ctrl1;
	struct regmap * cpu_sys_regmap;
	int pll_dslp_sel_on;

	int gpio_wakeup_source_num;
	int gpio_wakeup_source[SOC_GPIO_WAKEUP_SOURCE_MAX];

	unsigned int gpio_wake_en;
	unsigned int syst_wake_en;
	unsigned int ethernet_wake_en;
	unsigned int sadc_wake_en;

	unsigned int int_ctrl0_state;
	unsigned int int_ctrl1_state;

	int net_phy_type;

};

static struct mol_pm_wake_priv *pm_wakeup_priv;
extern void __iomem *mol_pmu_reg_base;

static DEFINE_SPINLOCK(wakeup_sc_lock);

static struct proc_dir_entry *wakeup_source_dir;


/*for gpio wake*/
bool get_mol_wakeup_source(const char *name, void ** data)
{
	*data = &pm_wakeup_priv->gpio_wakeup_source_num;
	return true;
}
EXPORT_SYMBOL(get_mol_wakeup_source);

bool gpio_is_wakeup_sys(unsigned int gpio_no)
{
	int i;
	for (i = 0; i < pm_wakeup_priv->gpio_wakeup_source_num; i++) {
		if (gpio_no == pm_wakeup_priv->gpio_wakeup_source[i])
			return true;
	}
	return false;
}
EXPORT_SYMBOL(gpio_is_wakeup_sys);

bool external_phy_wakeup_sys(void)
{
	if (pm_wakeup_priv->ethernet_wake_en &&
			pm_wakeup_priv->net_phy_type == EXTERNAL_PHY)
		return true;
	return false;
}
EXPORT_SYMBOL(external_phy_wakeup_sys);

static struct wakeup_source * get_wakeup_source_by_name(const char * name)
{
	struct wakeup_source * ws;

	ws = wakeup_sources_walk_start();
	if (!ws)
		return NULL;

	do {
		if (strcmp(ws->name, name) == 0) {
			break;
		}

		ws = wakeup_sources_walk_next(ws);
	}while(ws != NULL);

	return ws;
}
/*int_ctrl set*/

static void mol_int_ctrlx_set(void)
{
	unsigned long flags;
	unsigned int reg;

	clk_mini_enable(CKG_APB_INT_CTRL_EN);

	spin_lock_irqsave(&wakeup_sc_lock, flags);

	reg = (MOL_GPIO0_INT_CTRL0_BIT | MOL_GPIO1_INT_CTRL0_BIT | MOL_GPIO2_INT_CTRL0_BIT | 
			MOL_GPIO3_INT_CTRL0_BIT | MOL_GPIO4_INT_CTRL0_BIT | MOL_GPIO5_INT_CTRL0_BIT);

	writel(reg, pm_wakeup_priv->int_ctrl0 +0x40);
	writel(reg, pm_wakeup_priv->int_ctrl0 +0x8);

	reg = (MOL_SYSTIMER_INT_CTRL1_BIT | MOL_ETHERNET_INT_CTRL1_BIT | MOL_SADC_INT_CTRL1_BIT |
			MOL_GPIO6_INT_CTRL1_BIT | MOL_GPIO7_INT_CTRL1_BIT);

	writel(reg, pm_wakeup_priv->int_ctrl1 +0x40);
	writel(reg, pm_wakeup_priv->int_ctrl1 +0x8);

	spin_unlock_irqrestore(&wakeup_sc_lock, flags);

	clk_mini_disable(CKG_APB_INT_CTRL_EN);

}

void mol_int_ctrlx_get_state(void)
{
	clk_mini_enable(CKG_APB_INT_CTRL_EN);

	pm_wakeup_priv->int_ctrl0_state = readl(pm_wakeup_priv->int_ctrl0 + 0x4);
	pm_wakeup_priv->int_ctrl1_state = readl(pm_wakeup_priv->int_ctrl1 + 0x4);

	clk_mini_disable(CKG_APB_INT_CTRL_EN);

}
EXPORT_SYMBOL(mol_int_ctrlx_get_state);

/*for ethernet sleep/wakeup*/
static void mol_pll_deepslp_sel(int sel)
{
	unsigned long flags;
	unsigned int reg;

	spin_lock_irqsave(&wakeup_sc_lock, flags);
	reg = readl(mol_pmu_reg_base + MOL_DEEP_SLEEP_SEL_OFF);
	reg &= ~MOL_PLL_DSLP_SEL;
	if (sel)
		reg |= MOL_PLL_DSLP_SEL;
	writel(reg, mol_pmu_reg_base + MOL_DEEP_SLEEP_SEL_OFF);
	spin_unlock_irqrestore(&wakeup_sc_lock, flags);
}

static void device_can_wakeup_enable(struct device *dev, int enable)
{
	if (!dev)
		return;

	if (enable)
		dev->power.can_wakeup = 1;
	else
		dev->power.can_wakeup = 0;
}

static void ethernet_set_wakeup_source(struct wakeup_source *ws)
{
	if (!ws)
		return;
	if (strcmp(ws->name, MOL_WAKE_SOURCE_DEVICE_ETHERNET) == 0) {
		device_can_wakeup_enable(ws->dev->parent, true);
		pm_wakeup_priv->pll_dslp_sel_on = 1;
	}
}

static void saradc_set_wakeup_source(struct wakeup_source *ws)
{
	if (!ws)
		return;
	if (strcmp(ws->name, MOL_WAKE_SOURCE_DEVICE_SARADC) == 0) {
		device_can_wakeup_enable(ws->dev->parent, true);
	}
}

static void systimer_set_wakeup_source(struct wakeup_source *ws)
{
	if (!ws)
		return;

	if (strcmp(ws->name, MOL_WAKE_SOURCE_DEVICE_SYSTIMER) == 0) {
		device_can_wakeup_enable(ws->dev->parent, true);
	}

}

static int gpio_read(struct seq_file *m, void *v) {
	seq_printf(m, "Gpio Wakeup Source\n");
	return 0;
}

static int sadc_read(struct seq_file *m, void *v) {
	seq_printf(m, "%d\n", pm_wakeup_priv->sadc_wake_en);
	return 0;
}

static int ethernet_read(struct seq_file *m, void *v) {
	seq_printf(m, "%d\n", pm_wakeup_priv->ethernet_wake_en);
	return 0;
}

static int systimer_read(struct seq_file *m, void *v) {
	seq_printf(m, "%d\n", pm_wakeup_priv->syst_wake_en);
	return 0;
}

static int event_read(struct seq_file *m, void *v) {
	if (pm_wakeup_priv->int_ctrl1_state & MOL_SYSTIMER_INT_CTRL1_BIT)
		seq_printf(m, "systimer\n");
	else if (pm_wakeup_priv->int_ctrl1_state & MOL_ETHERNET_INT_CTRL1_BIT)
		seq_printf(m, "ethernet\n");
	else if (pm_wakeup_priv->int_ctrl1_state & MOL_SADC_INT_CTRL1_BIT)
		seq_printf(m, "sadc\n");
	else if (pm_wakeup_priv->int_ctrl1_state & (MOL_GPIO6_INT_CTRL1_BIT | MOL_GPIO7_INT_CTRL1_BIT))
		seq_printf(m, "gpio6-7\n");

	if (pm_wakeup_priv->int_ctrl0_state & (MOL_GPIO0_INT_CTRL0_BIT | MOL_GPIO1_INT_CTRL0_BIT
			| MOL_GPIO3_INT_CTRL0_BIT | MOL_GPIO4_INT_CTRL0_BIT | MOL_GPIO5_INT_CTRL0_BIT))
		seq_printf(m, "gpio0-5\n");
	return 0;
}

static ssize_t gpio_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	return count;
}

static int __data_form_user(struct file *file, const char __user *buf, size_t count,
		loff_t *ppos, char * buffer, int size)
{
	if (count > size - 1) {
		return -EINVAL;
	}
	if (copy_from_user(buffer, buf, count)) {
		return -EFAULT;
	}
	buffer[count] = '\0';

	return count;
}
static ssize_t sadc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	char kbuf[16];
	unsigned long val, flags;
	struct wakeup_source *ws;
	count = __data_form_user(file, buf, count, ppos, kbuf, 16);
	if (count < 0)
		return count;
	val = simple_strtoul(kbuf, NULL, 16);

	ws = get_wakeup_source_by_name(MOL_WAKE_SOURCE_DEVICE_SARADC);
	if (!ws)
		goto sadc_w_out;

	spin_lock_irqsave(&wakeup_sc_lock, flags);

	if (val) {
		device_can_wakeup_enable(ws->dev->parent, true);
		pm_wakeup_priv->sadc_wake_en = 1;
	} else {
		device_can_wakeup_enable(ws->dev->parent, false);
		pm_wakeup_priv->sadc_wake_en = 0;
	}

	spin_unlock_irqrestore(&wakeup_sc_lock, flags);

sadc_w_out:
	return count;
}

static ssize_t ethernet_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	char kbuf[16];
	unsigned long val, flags;
	struct wakeup_source *ws;

	count = __data_form_user(file, buf, count, ppos, kbuf, 16);
	if (count < 0)
		return count;
	val = simple_strtoul(kbuf, NULL, 16);

	ws = get_wakeup_source_by_name(MOL_WAKE_SOURCE_DEVICE_ETHERNET);
	if (!ws)
		goto ethernet_w_out;

	spin_lock_irqsave(&wakeup_sc_lock, flags);

	if (val) {
		device_can_wakeup_enable(ws->dev->parent, true);
		pm_wakeup_priv->ethernet_wake_en = 1;
		pm_wakeup_priv->pll_dslp_sel_on = 1;
	} else {
		device_can_wakeup_enable(ws->dev->parent, false);
		pm_wakeup_priv->ethernet_wake_en = 0;
		pm_wakeup_priv->pll_dslp_sel_on = 0;
	}

	spin_unlock_irqrestore(&wakeup_sc_lock, flags);

ethernet_w_out:
	return count;

}

static ssize_t systimer_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	char kbuf[16];
	unsigned long val, flags;
	struct wakeup_source *ws;
	count = __data_form_user(file, buf, count, ppos, kbuf, 16);
	if (count < 0)
		return count;
	val = simple_strtoul(kbuf, NULL, 16);

	ws = get_wakeup_source_by_name(MOL_WAKE_SOURCE_DEVICE_SYSTIMER);
	if (!ws)
		goto syst_w_out;

	spin_lock_irqsave(&wakeup_sc_lock, flags);

	if (val) {
		device_can_wakeup_enable(ws->dev->parent, true);
		pm_wakeup_priv->syst_wake_en = 1;
	} else {
		device_can_wakeup_enable(ws->dev->parent, false);
		pm_wakeup_priv->syst_wake_en = 0;
	}

	spin_unlock_irqrestore(&wakeup_sc_lock, flags);

syst_w_out:
	return count;

}

static int gpio_open(struct inode *inode, struct file *file) {
	return single_open(file, gpio_read, NULL);
}

static int sadc_open(struct inode *inode, struct file *file) {
	return single_open(file, sadc_read, NULL);
}

static int ethernet_open(struct inode *inode, struct file *file) {
	return single_open(file, ethernet_read, NULL);
}

static int systimer_open(struct inode *inode, struct file *file) {
	return single_open(file, systimer_read, NULL);
}

static int event_open(struct inode *inode, struct file *file) {
	return single_open(file, event_read, NULL);
}

static const struct proc_ops gpio_proc_ops = {
	.proc_open = gpio_open,
	.proc_read = seq_read,
	.proc_write = gpio_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static const struct proc_ops sadc_proc_ops = {
	.proc_open = sadc_open,
	.proc_read = seq_read,
	.proc_write = sadc_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static const struct proc_ops ethernet_proc_ops = {
	.proc_open = ethernet_open,
	.proc_read = seq_read,
	.proc_write = ethernet_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static const struct proc_ops systimer_proc_ops = {
	.proc_open = systimer_open,
	.proc_read = seq_read,
	.proc_write = systimer_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static const struct proc_ops event_proc_ops = {
	.proc_open = event_open,
	.proc_read = seq_read,
	.proc_release = single_release,
};

static void gpio_keys_wakeup_enable(void)
{
	int i;

	for (i = 0; i < pm_wakeup_priv->gpio_wakeup_source_num; i++) {
		int irq = gpio_to_irq(pm_wakeup_priv->gpio_wakeup_source[i]);
		struct irq_desc * desc = irq_to_desc(irq);
		if (irq <= 0)
			continue;
		irq_desc_get_chip(desc)->flags |= IRQCHIP_ENABLE_WAKEUP_ON_SUSPEND;
		enable_irq_wake(irq);
		irq_set_irq_type(irq, IRQF_TRIGGER_HIGH);
	}
}

static void gpio_keys_wakeup_disable(void)
{
	int i;
	for (i = 0; i < pm_wakeup_priv->gpio_wakeup_source_num; i++) {
		int irq = gpio_to_irq(pm_wakeup_priv->gpio_wakeup_source[i]);
		if (irq <= 0)
			continue;
		irq_set_irq_type(irq, IRQ_TYPE_EDGE_BOTH);
		disable_irq_wake(irq);
	}
}

static irqreturn_t handle_gpio_wake_isr(int irq, void *dev_id)
{
	disable_irq_nosync(irq);
	return IRQ_HANDLED;
}

static int molchip_pm_wake_probe(struct platform_device *pdev)
{
	struct device *dev;
	int ret;
	struct wakeup_source *ws;

	if (!pdev)
		return -ENODEV;

	dev = &pdev->dev;

	pm_wakeup_priv = kzalloc(sizeof(struct mol_pm_wake_priv), GFP_KERNEL);
	if (!pm_wakeup_priv) {
		pr_err("%s kmalloc erro\n", __func__);
		ret = -ENOMEM;
		goto err_out;
	}

	pm_wakeup_priv->int_ctrl0 = devm_ioremap_resource(dev,
			      platform_get_resource(pdev, IORESOURCE_MEM, 0));
	if (IS_ERR(pm_wakeup_priv->int_ctrl0)) {
		dev_err(dev, "no int_ctrl0 space\n");
		ret = PTR_ERR(pm_wakeup_priv->int_ctrl0);
		goto err_out;
	}

	pm_wakeup_priv->int_ctrl1 = devm_ioremap_resource(dev,
			      platform_get_resource(pdev, IORESOURCE_MEM, 1));
	if (IS_ERR(pm_wakeup_priv->int_ctrl1)) {
		dev_err(dev, "no int_ctrl1 space\n");
		ret = PTR_ERR(pm_wakeup_priv->int_ctrl1);
		goto err_out;
	}

	mol_int_ctrlx_set();
/*
* get gpio wakeup source from dtb
*/

	pm_wakeup_priv->gpio_wakeup_source_num =
			device_property_read_u32_array(dev, "gpio-wakeup-en", NULL, 0);

	if ((pm_wakeup_priv->gpio_wakeup_source_num <= 0) ||
			(pm_wakeup_priv->gpio_wakeup_source_num > SOC_GPIO_WAKEUP_SOURCE_MAX) ) {
			
		pm_wakeup_priv->gpio_wake_en = false;
	} else {
		int idx, val;
		ret = device_property_read_u32_array(dev, "gpio-wakeup-en",
				pm_wakeup_priv->gpio_wakeup_source, pm_wakeup_priv->gpio_wakeup_source_num);
		if (ret < 0) {
			dev_err(dev, "Fail to get gpio-wakeup-source en\n");
			goto err_out;
		}
		
		pm_wakeup_priv->gpio_wake_en = true;

		for (idx = 0; idx < pm_wakeup_priv->gpio_wakeup_source_num; idx++) {

			val = pm_wakeup_priv->gpio_wakeup_source[idx];
			ret = gpio_request(val, "gpio-wake");
			if (ret) {
				pr_err("gpio %d requset failed!\n", val);
				continue;
			}
			ret = gpio_direction_input(val);
			if (ret) {
				pr_err("gpio %d direction input failed!\n", val);
				continue;
			}

			ret = request_irq(gpio_to_irq(val), handle_gpio_wake_isr, IRQF_TRIGGER_HIGH |
							IRQF_EARLY_RESUME, "gpio-wake", NULL);
			if (ret) {
					pr_err("gpio %d requset irq failed!\n", val);
					goto err_out;
			}
		}
	}

//get ethernet wakeup source
	ret = device_property_read_u32(dev, "ethernet-wakeup-en", &pm_wakeup_priv->net_phy_type);
	if (ret == 0)
		pm_wakeup_priv->ethernet_wake_en = true;

//get systimer wakeup source
	pm_wakeup_priv->syst_wake_en = device_property_read_bool(dev, "systimer-wakeup-en");
//get saradc wakeup source
	pm_wakeup_priv->sadc_wake_en = device_property_read_bool(dev, "saradc-wakeup-en");

/*
*get wakeup-source form wakeup-source list
*/

	ws = wakeup_sources_walk_start();
	do {
		device_can_wakeup_enable(ws->dev->parent, false);

		if (pm_wakeup_priv->syst_wake_en) {
			systimer_set_wakeup_source(ws);
		}

		if (pm_wakeup_priv->ethernet_wake_en) {
			ethernet_set_wakeup_source(ws);
		}

		if (pm_wakeup_priv->sadc_wake_en) {
			saradc_set_wakeup_source(ws);
		}

		ws = wakeup_sources_walk_next(ws);
	}while(ws != NULL);

/*
touch proc file
*/
	wakeup_source_dir = proc_mkdir("wakeup-source", NULL);
	if (!wakeup_source_dir) {
		pr_err("Failed to create /proc/wakeup-source directory\n");
		ret = -ENOMEM;
		goto err_out;
	}

	proc_create_data(MOL_WAKE_SOURCE_GPIO, 0644, wakeup_source_dir, &gpio_proc_ops, NULL);
	proc_create_data(MOL_WAKE_SOURCE_SADC, 0644, wakeup_source_dir, &sadc_proc_ops, NULL);
	proc_create_data(MOL_WAKE_SOURCE_ETHERNET, 0644, wakeup_source_dir, &ethernet_proc_ops, NULL);
	proc_create_data(MOL_WAKE_SOURCE_SYSTIMER, 0644, wakeup_source_dir, &systimer_proc_ops, NULL);
	proc_create_data(MOL_WAKE_SOURCE_EVENT, 0644, wakeup_source_dir, &event_proc_ops, NULL);

	return 0;


err_out:
	devres_release_group(dev, NULL);
	return ret;
}

static int molchip_pm_wake_remove(struct platform_device *pdev)
{
	iounmap(pm_wakeup_priv->int_ctrl0);
	iounmap(pm_wakeup_priv->int_ctrl1);
	kfree(pm_wakeup_priv);
	remove_proc_entry(MOL_WAKE_SOURCE_GPIO, wakeup_source_dir);
	remove_proc_entry(MOL_WAKE_SOURCE_SADC, wakeup_source_dir);
	remove_proc_entry(MOL_WAKE_SOURCE_ETHERNET, wakeup_source_dir);
	remove_proc_entry(MOL_WAKE_SOURCE_SYSTIMER, wakeup_source_dir);
	remove_proc_entry(MOL_WAKE_SOURCE_EVENT, wakeup_source_dir);
	remove_proc_entry("wakeup-source", NULL);
	return 0;
}

static const struct of_device_id molchip_wakeup_source_of_match[] = {
	{.compatible = "mol,wakeup-source"},
	{},
};
MODULE_DEVICE_TABLE(of, molchip_wakeup_source_of_match);

#ifdef CONFIG_PM_SLEEP
static int mol_pm_wakeup_suspend(struct device *dev)
{
	if (pm_wakeup_priv->pll_dslp_sel_on) {
		mol_pll_deepslp_sel(0);
	} else {
		mol_pll_deepslp_sel(1);
	}

	if (pm_wakeup_priv->sadc_wake_en) {
		clk_mini_set_rate(CKG_MUX_GLB_APB, 24000000);
	}

	gpio_keys_wakeup_enable();

	return 0;
}

static int mol_pm_wakeup_resume(struct device *dev)
{
	gpio_keys_wakeup_disable();
	if (pm_wakeup_priv->sadc_wake_en) {
		clk_mini_set_rate(CKG_MUX_GLB_APB, 150000000);
	}
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(mol_pm_wakeup_ops, mol_pm_wakeup_suspend, mol_pm_wakeup_resume);


static struct platform_driver molchip_wakeup_source_driver = {
	.driver = {
		.name = "molchip-pm-device",
		.of_match_table = molchip_wakeup_source_of_match,
		.pm = &mol_pm_wakeup_ops,
	},
	.probe = molchip_pm_wake_probe,
	.remove = molchip_pm_wake_remove,
};

int mol_pm_wake_driver_init(void){
	platform_driver_register(&molchip_wakeup_source_driver);
	return 0;
}

void mol_pm_wake_driver_exit(void){
	platform_driver_unregister(&molchip_wakeup_source_driver);
	return;
}


late_initcall(mol_pm_wake_driver_init);
module_exit(mol_pm_wake_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example module to get platform device by device tree node");




