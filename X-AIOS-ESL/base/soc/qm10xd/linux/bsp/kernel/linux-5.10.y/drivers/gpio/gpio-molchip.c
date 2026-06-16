/*
 * Molchip GPIO driver
 *
 * Copyright (C) 2019 Molchip Technology Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio/driver.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/acpi.h>
#include <linux/platform_device.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/syscore_ops.h>
#include <linux/pm_wakeirq.h>
#include <linux/molchip.h>

/* Molchip gpio register offsets */
#define GPIO_DATA	0x00	/*gpio data 1:high 0:low*/
#define GPIO_DMSK	0x04	/*gpio masked*/
#define GPIO_DIR	0x08	/*gpio direction 1:input 0:output*/
#define GPIO_IS 	0x0C	/*gpio bits interrupt sense*/
#define GPIO_IBE	0x10	/*gpio bits interrupt both edges 1:both edge 0:single edge*/
#define GPIO_IEV	0x14	/*gpio bits interrupt event*/
#define GPIO_IE  	0x18	/*gpio bits interrupt enable*/
#define GPIO_RIS	0x1C	/*gpio bits raws interrupt status*/
#define GPIO_MIS	0x20	/*gpio bits masked interrupt status*/
#define GPIO_IC 	0x24	/*gpio bits interrupt clear*/
#define GPIO_INEN	0x28	/*gpio input enable*/
#define GPIO_DB_EN	0x30	/*gpio debounce enable*/
#define GPIO_DB_INV	0x34	/*gpio debounce inverse*/
#define GPIO_DB_BOTH	0x38	/*gpio debounce both edge enable*/

#define GPIO_CLK_FB 1

#define GPIO_GROUP_NUM			8
#define HAVE_RESUME_PIN			1
#define INVALID_RESUME_PIN		(-1)
#define GPIO_GROUP_IS_VALID		1


#ifdef GPIO_CLK_FB
#include <linux/mini_clock/clock.h>
#else
#include <linux/clk.h>
#endif

static int gpio_nr;
struct molchip_gpio {
	struct gpio_chip	gc;
	struct irq_chip		irq_chip;
	void __iomem		*base;
	spinlock_t		lock;
	unsigned int		irq;
	unsigned int		id;

};

struct mc_platform_data {
	/* number of the first GPIO */
	unsigned	gpio_base;

	/* number of the first IRQ.
	 * If the IRQ functionality in not desired this must be set to 0.
	 */
	unsigned	irq_base;

	u16		directions;	/* startup directions, 1: out, 0: in */
	u16		values;		/* startup values */
};

struct molchip_gpio_SuspendResume {
	void __iomem		*base;
	int			group_flag;
	int			resume_pin_num;
	unsigned int		gpio_data;
	unsigned int		gpio_dmsk;
	unsigned int		gpio_dir;
	unsigned int		gpio_is;
	unsigned int		gpio_ibe;
	unsigned int		gpio_iev;
	unsigned int		gpio_ie;
	unsigned int		gpio_inen;
	unsigned int		gpio_db_en;
	unsigned int		gpio_db_inv;
	unsigned int		gpio_db_both;
};
struct molchip_gpio_SuspendResume mc_gpio_SuspendResume[GPIO_GROUP_NUM];

static void mc_set_mask(struct molchip_gpio *gc, unsigned offset)
{
	u16 mask;

	mask = readl(gc->base + GPIO_DMSK);
	mask |= BIT(offset);
	writel(mask, gc->base + GPIO_DMSK);
}

static int mc_get_direction(struct gpio_chip *gc, unsigned offset)
{
	struct molchip_gpio *chip = gpiochip_get_data(gc);
#if 0
	if ( !(readl(chip->base + GPIO_DMSK) & BIT(offset)) )
		return -1;
#endif
	return !(readl(chip->base + GPIO_DIR) & BIT(offset));
}

static int mc_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	unsigned long flags;
	u16 gpiodir;
	u16 inen;

	spin_lock_irqsave(&chip->lock, flags);

	gpiodir = readl(chip->base + GPIO_DIR);
	gpiodir &= ~(BIT(offset));
	writel(gpiodir, chip->base + GPIO_DIR);

	/*enable gpio input function */

	inen = readl(chip->base + GPIO_INEN);
	inen |= BIT(offset);
	writel(inen, chip->base + GPIO_INEN);

	mc_set_mask(chip, offset);

	spin_unlock_irqrestore(&chip->lock, flags);

	return 0;
}

static int mc_direction_output(struct gpio_chip *gc, unsigned offset,
		int value)
{
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	unsigned long flags;
	u16 gpiodir, data;

	spin_lock_irqsave(&chip->lock, flags);

	gpiodir = readl(chip->base + GPIO_DIR);
	gpiodir |= BIT(offset);
	writel(gpiodir, chip->base + GPIO_DIR);

	mc_set_mask(chip, offset);
	data = readl(chip->base + GPIO_DATA);
	if (value != 0)
		data |= BIT(offset);
	else
		data &= (~BIT(offset));

	writel(data, chip->base + GPIO_DATA);

	spin_unlock_irqrestore(&chip->lock, flags);

	return 0;
}

static int mc_get_value(struct gpio_chip *gc, unsigned offset)
{
	struct molchip_gpio *chip = gpiochip_get_data(gc);

	return !!(readl(chip->base + GPIO_DATA) & BIT(offset));
}

static void mc_set_value(struct gpio_chip *gc, unsigned offset, int value)
{
	u16 data;
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	unsigned long flags;

	spin_lock_irqsave(&chip->lock, flags);
	mc_set_mask(chip, offset);
	data = readl(chip->base + GPIO_DATA);
	if (value != 0)
		data |= BIT(offset);
	else
		data &= (~BIT(offset));

	writel(data, chip->base + GPIO_DATA);
	spin_unlock_irqrestore(&chip->lock, flags);
}

static int mc_irq_type(struct irq_data *d, unsigned trigger)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	int offset = irqd_to_hwirq(d);
	unsigned long flags;
	u16 gpiois, gpioibe, gpioiev;
	u16 bit = BIT(offset);

	if (offset < 0 || offset >= gpio_nr)
		return -EINVAL;

	if ((trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) &&
	    (trigger & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING)))
	{
		dev_err(gc->parent,
			"trying to configure line %d for both level and edge "
			"detection, choose one!\n",
			offset);
		return -EINVAL;
	}


	spin_lock_irqsave(&chip->lock, flags);

	gpioiev = readl(chip->base + GPIO_IEV);
	gpiois = readl(chip->base + GPIO_IS);
	gpioibe = readl(chip->base + GPIO_IBE);

	if (trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) {
		bool polarity = trigger & IRQ_TYPE_LEVEL_HIGH;
		/* Disable edge detection */
		gpioibe &= ~bit;
		/* Enable level detection */
		gpiois |= bit;
		/* Select polarity */
		if (polarity)
			gpioiev |= bit;
		else
			gpioiev &= ~bit;
		irq_set_handler_locked(d, handle_level_irq);
		dev_dbg(gc->parent, "line %d: IRQ on %s level\n",
			offset,
			polarity ? "HIGH" : "LOW");
	} else if ((trigger & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH) {
		/* Disable level detection */
		gpiois &= ~bit;
		/* Select both edges, setting this makes GPIOEV be ignored */
		gpioibe |= bit;
		irq_set_handler_locked(d, handle_edge_irq);
		dev_dbg(gc->parent, "line %d: IRQ on both edges\n", offset);
	} else if ((trigger & IRQ_TYPE_EDGE_RISING) ||
		   (trigger & IRQ_TYPE_EDGE_FALLING)) {
		bool rising = trigger & IRQ_TYPE_EDGE_RISING;

		/* Disable level detection */
		gpiois &= ~bit;
		/* Clear detection on both edges */
		gpioibe &= ~bit;
		/* Select edge */
		if (rising)
			gpioiev |= bit;
		else
			gpioiev &= ~bit;
		irq_set_handler_locked(d, handle_edge_irq);
		dev_dbg(gc->parent, "line %d: IRQ on %s edge\n",
			offset,
			rising ? "RISING" : "FALLING");
	} else {
		/* No trigger: disable everything */
		gpiois &= ~bit;
		gpioibe &= ~bit;
		gpioiev &= ~bit;
		irq_set_handler_locked(d, handle_bad_irq);
		dev_warn(gc->parent, "no trigger selected for line %d\n",
			 offset);
	}

	writel(gpiois, chip->base + GPIO_IS);
	writel(gpioibe, chip->base + GPIO_IBE);
	writel(gpioiev, chip->base + GPIO_IEV);

	spin_unlock_irqrestore(&chip->lock, flags);

	return 0;
}

static void mc_irq_handler(struct irq_desc *desc)
{
	unsigned long pending;
	int offset;
	struct gpio_chip *gc = irq_desc_get_handler_data(desc);
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	struct irq_chip *irqchip = irq_desc_get_chip(desc);

	chained_irq_enter(irqchip, desc);

	pending = readl(chip->base + GPIO_MIS);
	if (pending) {
		for_each_set_bit(offset, &pending, gpio_nr)
			generic_handle_irq(irq_find_mapping(gc->irq.domain,
							    offset));
	}

	chained_irq_exit(irqchip, desc);
}

static void mc_irq_mask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	u16 mask = BIT(irqd_to_hwirq(d) % gpio_nr);
	u16 gpioie;

	spin_lock(&chip->lock);

	gpioie = readl(chip->base + GPIO_IE) & ~mask;
	writel(gpioie, chip->base + GPIO_IE);

	spin_unlock(&chip->lock);
}

static void mc_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	u16 mask = BIT(irqd_to_hwirq(d) % gpio_nr);
	u16 gpioie;

	spin_lock(&chip->lock);

	gpioie = readl(chip->base + GPIO_IE) | mask;
	writel(gpioie, chip->base + GPIO_IE);

	spin_unlock(&chip->lock);
}

/**
 * molchip_irq_ack() - ACK an edge IRQ
 * @d: IRQ data for this IRQ
 *
 * This gets called from the edge IRQ handler to ACK the edge IRQ
 * in the GPIOIC (interrupt-clear) register. For level IRQs this is
 * not needed: these go away when the level signal goes away.
 */
static void mc_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct molchip_gpio *chip = gpiochip_get_data(gc);
	u16 mask = BIT(irqd_to_hwirq(d) % gpio_nr);

	spin_lock(&chip->lock);

	writel(mask, chip->base + GPIO_IC);

	spin_unlock(&chip->lock);
}

static int mc_irq_set_wake(struct irq_data *d, unsigned int state)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct molchip_gpio *chip = gpiochip_get_data(gc);

	return irq_set_irq_wake(chip->irq, state);
}

static void mc_gpio_save_regs(int gpio_group_num)
{
	if ( gpio_group_num >= GPIO_GROUP_NUM ) {
		printk("%s line=%d you input gpio_group_num(%d) is err\n",__FUNCTION__,__LINE__,gpio_group_num);
		return;
	}

	mc_gpio_SuspendResume[gpio_group_num].gpio_data = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DATA);
	mc_gpio_SuspendResume[gpio_group_num].gpio_dmsk = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DMSK);
	mc_gpio_SuspendResume[gpio_group_num].gpio_dir = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DIR);
	mc_gpio_SuspendResume[gpio_group_num].gpio_is = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IS);
	mc_gpio_SuspendResume[gpio_group_num].gpio_ibe = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IBE);
	mc_gpio_SuspendResume[gpio_group_num].gpio_iev = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IEV);
	mc_gpio_SuspendResume[gpio_group_num].gpio_ie = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IE);
	mc_gpio_SuspendResume[gpio_group_num].gpio_inen = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_INEN);
	mc_gpio_SuspendResume[gpio_group_num].gpio_db_en = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DB_EN);
	mc_gpio_SuspendResume[gpio_group_num].gpio_db_inv = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DB_INV);
	mc_gpio_SuspendResume[gpio_group_num].gpio_db_both = readl(mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DB_BOTH);
}

static void mc_gpio_cfg_HighResistanceState(int gpio_group_num)
{
	if ( gpio_group_num >= GPIO_GROUP_NUM ) {
		printk("%s line=%d you input gpio_group_num(%d) is err\n",__FUNCTION__,__LINE__,gpio_group_num);
		return;
	}

	if (mc_gpio_SuspendResume[gpio_group_num].resume_pin_num != INVALID_RESUME_PIN) {
		writel(0x0 | (0x1 << mc_gpio_SuspendResume[gpio_group_num].resume_pin_num), mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IE);
		writel(0x0 | (0x1 << mc_gpio_SuspendResume[gpio_group_num].resume_pin_num), mc_gpio_SuspendResume[gpio_group_num].base + GPIO_INEN);
	} else {
		writel(0x0, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IE);
		writel(0x0, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_INEN);
	}
	writel(0x0, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DIR);  //cfg input
}

static void mc_gpio_restore_regs(int gpio_group_num)
{
	if ( gpio_group_num >= GPIO_GROUP_NUM ) {
		printk("%s line=%d you input gpio_group_num(%d) is err\n",__FUNCTION__,__LINE__,gpio_group_num);
		return;
	}

	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_dmsk, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DMSK);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_dir, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DIR);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_is, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IS);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_ibe, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IBE);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_iev, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IEV);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_inen, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_INEN);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_db_en, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DB_EN);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_db_inv, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DB_INV);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_db_both, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DB_BOTH);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_data, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_DATA);
	writel(0xFF, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IC);
	writel(mc_gpio_SuspendResume[gpio_group_num].gpio_ie, mc_gpio_SuspendResume[gpio_group_num].base + GPIO_IE);
}

static void get_gpio_wake_pins(void)
{
	struct mol_gpio_wake * wake;
	int i;

	if (!get_mol_wakeup_source(NULL, (void *)&wake))
		return;

	for (i = 0; i < wake->gpio_wakeup_source_num ; i++) {
		int group, offset;
		u32 val;

		group = wake->gpio_wakeup_source[i] / gpio_nr;
		offset = wake->gpio_wakeup_source[i] % gpio_nr;

		val = readl(mc_gpio_SuspendResume[group].base + GPIO_INEN);
		writel(val | 0x1 << offset, mc_gpio_SuspendResume[group].base + GPIO_INEN);  //ensable input en
		writel(val | 0x1 << offset, mc_gpio_SuspendResume[group].base + GPIO_IE);  //enable interrupt

	}
}

static int molchip_gpio_syscore_suspend(void)
{
	int i = 0;

	for (i=0; i<GPIO_GROUP_NUM; i++) {
		if (mc_gpio_SuspendResume[i].group_flag == GPIO_GROUP_IS_VALID) {
			mc_gpio_save_regs(i);
			mc_gpio_cfg_HighResistanceState(i);
		}
	}

	get_gpio_wake_pins();

	clk_mini_disable(CKG_APB_GPIO_EN);

	return 0;
}

static void molchip_gpio_syscore_resume(void)
{
	int i = 0;

	clk_mini_enable(CKG_APB_GPIO_EN);

	for (i=0; i<GPIO_GROUP_NUM; i++) {
		if (mc_gpio_SuspendResume[i].group_flag == GPIO_GROUP_IS_VALID) {
			mc_gpio_restore_regs(i);
		}
	}
}

static int molchip_gpio_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mc_platform_data *pdata = dev_get_platdata(dev);
	struct molchip_gpio *chip;
	struct gpio_irq_chip *girq;
	struct resource *res;
	int ret, id, irq_base;
	u32 group_number, resume_pin[2];
	bool wakeup;

	ret = device_property_read_u32(&pdev->dev, "group-number", &group_number);
	if (ret)
		return ret;
	gpio_nr = group_number;
	chip = devm_kzalloc(dev, sizeof(*chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	if (pdata) {
		chip->gc.base = pdata->gpio_base;
		irq_base = pdata->irq_base;
		if (irq_base <= 0) {
			dev_err(&pdev->dev, "invalid IRQ base in pdata\n");
			return -ENODEV;
		}
	} else {
		chip->gc.base = -1;
		irq_base = 0;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	chip->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(chip->base))
		return PTR_ERR(chip->base);

	id = of_alias_get_id(dev->of_node, "gpio");

	if (id < 0) {
		dev_err(&pdev->dev, "failed to get gpio id\n");
		return -ENODEV;
	}

	//gpio suspend resume
	memset(&mc_gpio_SuspendResume[id], 0x0, sizeof(struct molchip_gpio_SuspendResume));
	mc_gpio_SuspendResume[id].resume_pin_num = INVALID_RESUME_PIN;
	
	ret = of_property_read_u32_array(dev->of_node, "gpio-resume-pin", resume_pin, 2);
	if (0 != ret) {
		dev_err(&pdev->dev, "read dts gpio-resume-pin node err\n");
	}

	if (HAVE_RESUME_PIN == resume_pin[0]) {
		mc_gpio_SuspendResume[id].resume_pin_num = resume_pin[1];
	}
	mc_gpio_SuspendResume[id].base = chip->base;
	mc_gpio_SuspendResume[id].group_flag = GPIO_GROUP_IS_VALID;

	//need clock done
#ifdef GPIO_CLK_FB
	if(!clk_mini_is_enabled(CKG_APB_GPIO_EN)) {
		ret = clk_mini_enable(CKG_APB_GPIO_EN);
		if(ret)
			clk_mini_disable(CKG_APB_GPIO_EN);
	}

#else
	struct clk *clk;
	clk = devm_clk_get(&pdev->dev, "gpio_clk");
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "gpio_clk clock not found.\n");
		return PTR_ERR(clk);
	}

	ret = clk_prepare_enable(clk);
	if (ret) {
		dev_err(&pdev->dev, "Unable to enable gpio_clk clock.\n");
		return ret;
	}
#endif

	spin_lock_init(&chip->lock);

	chip->gc.free = gpiochip_generic_free;
	chip->gc.get_direction = mc_get_direction;
	chip->gc.direction_input = mc_direction_input;
	chip->gc.direction_output = mc_direction_output;
	chip->gc.get = mc_get_value;
	chip->gc.set = mc_set_value;
	chip->gc.base = gpio_nr * id;
	chip->gc.ngpio = gpio_nr;
	chip->gc.label = dev_name(dev);
	chip->gc.parent = dev;
	chip->gc.owner = THIS_MODULE;

	/*
	 * irq_chip support
	 */
	writel(0, chip->base + GPIO_IE); /* disable irqs */
	writel(0, chip->base + GPIO_IS); /* set gpio trigger mode to edge */
	writel(0xFFFF, chip->base + GPIO_IC); /* clear gpio interrupts */

	chip->irq = platform_get_irq(pdev, 0);
	chip->irq_chip.name = "molchip";
	chip->irq_chip.irq_ack	= mc_irq_ack;
	chip->irq_chip.irq_mask = mc_irq_mask;
	chip->irq_chip.irq_unmask = mc_irq_unmask;
	chip->irq_chip.irq_set_type = mc_irq_type;
	chip->irq_chip.irq_set_wake = mc_irq_set_wake;

	girq = &chip->gc.irq;
	girq->chip = &chip->irq_chip;
	girq->parent_handler = mc_irq_handler;
	girq->num_parents = 1;
	girq->parents = devm_kcalloc(dev, 1, sizeof(*girq->parents),
				     GFP_KERNEL);
	if (!girq->parents)
		return -ENOMEM;

	girq->parents[0] = chip->irq;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_bad_irq;

	ret = devm_gpiochip_add_data(dev, &chip->gc, chip);
	if (ret)
		return ret;

	dev_set_drvdata(&pdev->dev, chip);

	wakeup = device_property_read_bool(&pdev->dev, "wakeup-source");
	if (wakeup) {
		ret = device_init_wakeup(&pdev->dev, true);
		if (ret)
			return -ENOMEM;
		ret = dev_pm_set_wake_irq(&pdev->dev, chip->irq);
		if (ret)
			return -ENOMEM;
	}

	return 0;
}

static const struct of_device_id mc_gpio_dt_ids[] = {
	{ .compatible = "molchip,gpio" },
	{},
};
MODULE_DEVICE_TABLE(of, mc_gpio_dt_ids);

static struct platform_driver molchip_gpio_driver = {
	.probe      = molchip_gpio_probe,
	.driver     = {
		.name   ="molchip-gpio",
		.of_match_table = of_match_ptr(mc_gpio_dt_ids),
	}
};

static struct syscore_ops molchip_gpio_syscore_ops = {
	.suspend = molchip_gpio_syscore_suspend,
	.resume = molchip_gpio_syscore_resume,
};

static int __init molchip_gpio_init(void)
{
	register_syscore_ops(&molchip_gpio_syscore_ops);

	return platform_driver_register(&molchip_gpio_driver);
}
postcore_initcall(molchip_gpio_init);

static void __exit molchip_gpio_exit(void)
{
	unregister_syscore_ops(&molchip_gpio_syscore_ops);
	platform_driver_unregister(&molchip_gpio_driver);
}
module_exit(molchip_gpio_exit);

MODULE_DESCRIPTION("Molchip GPIO driver");
MODULE_LICENSE("GPL v2");


