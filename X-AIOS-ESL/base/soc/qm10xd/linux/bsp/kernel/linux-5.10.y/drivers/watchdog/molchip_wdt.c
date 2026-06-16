/*
 * Molchip WDT driver - Used by molchip
 *
 * Copyright (C) 2019 - 2023 Molchip, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/clk.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/watchdog.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/mini_clock/clock.h>

#define MC_WDT_DEFAULT_TIMEOUT	10
/* Supports 1 - 516 sec */
#define MC_WDT_MIN_TIMEOUT	1
#define MC_WDT_MAX_TIMEOUT	516

/* Counter maximum value */
#define MC_WDT_COUNTER_MAX 0xFFFFFFFF
#define WDT_CLK_FREQ		32768

static int wdt_timeout = MC_WDT_DEFAULT_TIMEOUT;
static int nowayout = WATCHDOG_NOWAYOUT;
static volatile int molchip_soc_restart;
static struct task_struct  *tmp_task = NULL;

module_param(wdt_timeout, int, 0);
MODULE_PARM_DESC(wdt_timeout,
		 "Watchdog time in seconds. (default="
		 __MODULE_STRING(CDNS_WDT_DEFAULT_TIMEOUT) ")");

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout,
		 "Watchdog cannot be stopped once started (default="
		 __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

/**
 * struct mc_wdt - Watchdog device structure
 * @regs: baseaddress of device
 * @rst: reset flag
 * @clk: struct clk * of a clock source
 * @prescaler: for saving prescaler value
 * @ctrl_clksel: counter clock prescaler selection
 * @io_lock: spinlock for IO register access
 * @mc_wdt_device: watchdog device structure
 *
 * Structure containing parameters specific to cadence watchdog.
 */
struct mc_wdt {
	void __iomem		*regs;
	bool			rst;
	u32			ap_cp_flag;
	spinlock_t		io_lock;
	struct watchdog_device	mc_wdt_device;
};

/*************************Register Map**************************************/

/* Register Offsets for the WDT */
#define MC_WDT_LOAD_OFFSET      0x0000
#define MC_WDT_CTRL_OFFSET      0x0004
#define MC_WDT_CLEAR_OFFSET     0x0008
#define MC_WDT_INTRAW_OFFSET    0x000C
#define MC_WDT_INT_OFFSET       0x0010
#define MC_WDT_VAL_OFFSET       0x0014
#define MC_WDT_LOCK_OFFSET      0x0018
#define MC_WDT_READ_OFFSET      0x001C
#define MC_WDT_IRQVAL_OFFSET    0x0020

/*
 * WDT_LOAD Register
 */
#define WDT_LOAD_CRV_MASK       0xFFFF
#define BITS_WDT_LOAD_MSK(x)    ((x) & MC_WDT_COUNTER_MAX)

/*
 * WDT_CTRL Register
 */
#define BIT_WDT_IRQ_EN          (0x1 << 0)
#define BIT_WDT_RUN             (0x1 << 1)
#define BIT_WDT_NEW             (0x1 << 2)
#define BIT_WDT_RST_EN          (0x1 << 3)

/*
 * WDT_LOCK Register
 */
#define BIT_WDT_UNLOCK          (0x1ACCE551)

/*
 * Zero Mode Register - This register controls how the time out is indicated
 * and also contains the access code to allow writes to the register (0xABC).
 */
#define CDNS_WDT_ZMR_WDEN_MASK	0x00000001 /* Enable the WDT */
#define CDNS_WDT_ZMR_RSTEN_MASK	0x00000002 /* Enable the reset output */
#define CDNS_WDT_ZMR_IRQEN_MASK	0x00000004 /* Enable IRQ output */
#define CDNS_WDT_ZMR_RSTLEN_16	0x00000030 /* Reset pulse of 16 pclk cycles */
#define CDNS_WDT_ZMR_ZKEY_VAL	0x00ABC000 /* Access key, 0xABC << 12 */
/*
 * Counter Control register - This register controls how fast the timer runs
 * and the reset value and also contains the access code to allow writes to
 * the register.
 */
#define CDNS_WDT_CCR_CRV_MASK	0x00003FFC /* Counter reset value */

#define AP_CP_FLAG_CLK_INIT	(1 << 0)
#define AP_CP_FLAG_FEED_DOG	(1 << 1)
#define AP_CP_FLAG_INT_FUNC	(1 << 2)
#define AP_CP_FLAG_FULL		(AP_CP_FLAG_CLK_INIT | \
				AP_CP_FLAG_FEED_DOG | AP_CP_FLAG_INT_FUNC)

//extern void __iomem *mc_reboot_base;

/* Write access to Registers */
static void mc_wdt_writereg(struct mc_wdt *wdt, u32 offset, u32 val, u32 msk)
{
        u32 reg_val = readl_relaxed(wdt->regs + offset);
        reg_val &= ~msk;
        reg_val |= val;
	writel_relaxed(BIT_WDT_UNLOCK, wdt->regs + MC_WDT_LOCK_OFFSET);
        writel_relaxed(reg_val, wdt->regs + offset);
	writel_relaxed(0, wdt->regs + MC_WDT_LOCK_OFFSET);
}


/**
 * mc_wdt_stop - Stop the watchdog.
 *
 * @wdd: watchdog device
 *
 * Read the contents of the ZMR register, clear the WDEN bit
 * in the register and set the access key for successful write.
 *
 * Return: always 0
 */
static int mc_wdt_stop(struct watchdog_device *wdd)
{
	struct mc_wdt *wdt = watchdog_get_drvdata(wdd);
	unsigned long flags;

	if (wdt->ap_cp_flag & (AP_CP_FLAG_FEED_DOG | AP_CP_FLAG_INT_FUNC)) {
		spin_lock_irqsave(&wdt->io_lock, flags);
		mc_wdt_writereg(wdt, MC_WDT_CTRL_OFFSET, 0, BIT_WDT_RUN);
		spin_unlock_irqrestore(&wdt->io_lock, flags);
	}

	return 0;
}

/**
 * mc_wdt_reload - Reload the watchdog timer (i.e. pat the watchdog).
 *
 * @wdd: watchdog device
 *
 * Write the restart key value (0x00001999) to the restart register.
 *
 * Return: always 0
 */
static int mc_wdt_reload(struct watchdog_device *wdd)
{
	struct mc_wdt *wdt = watchdog_get_drvdata(wdd);
	unsigned long flags;

	if (wdt->ap_cp_flag & AP_CP_FLAG_FEED_DOG) {
		spin_lock_irqsave(&wdt->io_lock, flags);
		mc_wdt_writereg(wdt, MC_WDT_LOAD_OFFSET, wdd->timeout * WDT_CLK_FREQ, MC_WDT_COUNTER_MAX);
		spin_unlock_irqrestore(&wdt->io_lock, flags);
	}

	return 0;
}

/**
 * mc_wdt_start - Enable and start the watchdog.
 *
 * @wdd: watchdog device
 *
 * The counter value is calculated according to the formula:
 *		calculated count = (timeout * clock) / prescaler + 1.
 * The calculated count is divided by 0x1000 to obtain the field value
 * to write to counter control register.
 * Clears the contents of prescaler and counter reset value. Sets the
 * prescaler to 4096 and the calculated count and access key
 * to write to CCR Register.
 * Sets the WDT (WDEN bit) and either the Reset signal(RSTEN bit)
 * or Interrupt signal(IRQEN) with a specified cycles and the access
 * key to write to ZMR Register.
 *
 * Return: always 0
 */
static int mc_wdt_start(struct watchdog_device *wdd)
{
	struct mc_wdt *wdt = watchdog_get_drvdata(wdd);
	unsigned int data = BIT_WDT_RUN|BIT_WDT_NEW;
	unsigned int count;
	unsigned long flags;
	//unsigned long clock_f = clk_get_rate(wdt->clk);
	unsigned long clock_f = WDT_CLK_FREQ;

	if (wdt->ap_cp_flag & (AP_CP_FLAG_FEED_DOG)) {
		/*
		 * Counter value divisor to obtain the value of
		 * counter reset to be written to control register.
		 */
		count = wdd->timeout * clock_f;

		spin_lock_irqsave(&wdt->io_lock, flags);
		mc_wdt_writereg(wdt, MC_WDT_CTRL_OFFSET, 0, 0xF);
		mc_wdt_writereg(wdt, MC_WDT_LOAD_OFFSET, count, MC_WDT_COUNTER_MAX);

		/* Reset on timeout if specified in device tree. */
		if (wdt->rst) {
			data |= BIT_WDT_RST_EN;
			data &= ~BIT_WDT_IRQ_EN;
		} else {
			data &= ~BIT_WDT_RST_EN;
			data |= BIT_WDT_IRQ_EN;
		}
		mc_wdt_writereg(wdt, MC_WDT_CLEAR_OFFSET,1,1);
		mc_wdt_writereg(wdt, MC_WDT_CTRL_OFFSET, data, 0xF);
		mc_wdt_writereg(wdt, MC_WDT_IRQVAL_OFFSET, 0, MC_WDT_COUNTER_MAX);
		set_bit(WDOG_HW_RUNNING, &wdd->status);
		spin_unlock_irqrestore(&wdt->io_lock, flags);
	}

	return 0;
}

/**
 * mc_wdt_settimeout - Set a new timeout value for the watchdog device.
 *
 * @wdd: watchdog device
 * @new_time: new timeout value that needs to be set
 * Return: 0 on success
 *
 * Update the watchdog_device timeout with new value which is used when
 * mc_wdt_start is called.
 */
static int mc_wdt_settimeout(struct watchdog_device *wdd,
			       unsigned int new_time)
{
	wdd->timeout = new_time;

	return mc_wdt_start(wdd);
}

/**
 * mc_wdt_get_timeleft - get  the number of seconds before reboot .
 *
 * @wdd: watchdog device
 * Return: 0 on success
 *
 */
static unsigned int mc_wdt_get_timeleft(struct watchdog_device *wdd, unsigned int *timeleft)
{
	struct mc_wdt *wdt = watchdog_get_drvdata(wdd);
	unsigned long clock_f = WDT_CLK_FREQ;
	*timeleft =  readl_relaxed(wdt->regs + MC_WDT_VAL_OFFSET)/clock_f;

	return 0;

 }

static long mc_wdt_ioctl(struct watchdog_device *wdd,
			unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_value;
	unsigned int val;

	if (cmd == WDIOC_KEEPALIVE) {
		mc_wdt_reload(wdd);
	} else if (cmd == WDIOC_SETTIMEOUT){
		get_user(new_value, p);
		mc_wdt_settimeout(wdd, new_value);
	} else if (cmd == SIGUSR1){
		tmp_task = get_current();
	} else if (cmd == WDIOC_GETTIMELEFT){
		mc_wdt_get_timeleft(wdd, &val);
		put_user(val, p);
	}

	return 0;
}


/**
 * mc_wdt_irq_handler - Notifies of watchdog timeout.
 *
 * @irq: interrupt number
 * @dev_id: pointer to a platform device structure
 * Return: IRQ_HANDLED
 *
 * The handler is invoked when the watchdog times out and a
 * reset on timeout has not been enabled.
 */
static irqreturn_t mc_wdt_irq_handler(int irq, void *dev_id)
{
	struct platform_device *pdev = dev_id;
	struct mc_wdt *wdt = platform_get_drvdata(pdev);
	struct kernel_siginfo info;

	dev_info(&pdev->dev,
		"Watchdog timed out.  maybe reboot system in 2s\n");

	mc_wdt_stop(&wdt->mc_wdt_device);
	/* Clear watchdog timeout irq interrupt */
	mc_wdt_writereg(wdt,MC_WDT_CLEAR_OFFSET,1,1);

	/* reboot system in 2s */
	if (wdt->mc_wdt_device.id == 1) {
		mc_wdt_stop(&wdt->mc_wdt_device);
		memset(&info, 0, sizeof(struct kernel_siginfo));
		info.si_signo = SIGUSR1;
		info.si_code = SI_QUEUE;
		info.si_int = 1;
		if (tmp_task != NULL) {
			if (send_sig_info(SIGUSR1, &info, tmp_task) < 0) {
				printk("Send signal to User Fail!\r\n");
			}
		}

	} else {
		wdt->rst = 1;
		mc_wdt_settimeout(&wdt->mc_wdt_device,2);
	}

	return IRQ_HANDLED;
}

/*
 * Info structure used to indicate the features supported by the device
 * to the upper layers. This is defined in watchdog.h header file.
 */
static struct watchdog_info mc_wdt_info = {
	.identity	= "mc_wdt watchdog",
	.options	= WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING |
			  WDIOF_MAGICCLOSE,
};

/* Watchdog Core Ops */
static const struct watchdog_ops mc_wdt_ops = {
	.owner = THIS_MODULE,
	.start = mc_wdt_start,
	.stop = mc_wdt_stop,
	.ping = mc_wdt_reload,
	.set_timeout = mc_wdt_settimeout,
	.ioctl = mc_wdt_ioctl,
};

/************************Platform Operations*****************************/
/**
 * mc_wdt_probe - Probe call for the device.
 *
 * @pdev: handle to the platform device structure.
 * Return: 0 on success, negative error otherwise.
 *
 * It does all the memory allocation and registration for the device.
 */
static int mc_wdt_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret, irq;
	struct mc_wdt *wdt;
	struct watchdog_device *mc_wdt_device;
	bool no_need_init_clock;

	wdt = devm_kzalloc(&pdev->dev, sizeof(*wdt), GFP_KERNEL);
	if (!wdt)
		return -ENOMEM;

	mc_wdt_device = &wdt->mc_wdt_device;
	mc_wdt_device->info = &mc_wdt_info;
	mc_wdt_device->ops = &mc_wdt_ops;
	mc_wdt_device->timeout = MC_WDT_DEFAULT_TIMEOUT;
	mc_wdt_device->min_timeout = MC_WDT_MIN_TIMEOUT;
	mc_wdt_device->max_timeout = MC_WDT_MAX_TIMEOUT;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	wdt->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(wdt->regs))
		return PTR_ERR(wdt->regs);

	ret = of_property_read_u32(pdev->dev.of_node, "ap-cp-flag", &wdt->ap_cp_flag);
	if (ret)
		wdt->ap_cp_flag = AP_CP_FLAG_FULL;
	no_need_init_clock = of_property_read_bool(pdev->dev.of_node,"bypass-init-clock");

	if (wdt->ap_cp_flag & AP_CP_FLAG_CLK_INIT) {
		if(no_need_init_clock == 0)
		{
			//wdt_apb_clk
			ret = clk_mini_enable(CKG_APB_WDT0_EN);
			if(ret){
				dev_err(&pdev->dev, "failed to enable  wdt_apb_clk\n");
				goto exit;
			}

			//wdt_clk
			ret = clk_mini_enable(CKG_WDT_EN);
			if (ret){
				dev_err(&pdev->dev, "failed to enable  clk_wdt\n");
				goto exit;
			}
			//if(of_property_read_bool(pdev->dev.of_node, "molchip,sw-glb-rst"))
			//	mc_reboot_base = 0;
			//else
			//	mc_reboot_base = wdt->regs;
		}
	}

	if (wdt->ap_cp_flag & AP_CP_FLAG_INT_FUNC) {
		/* Register the interrupt */
		wdt->rst = of_property_read_bool(pdev->dev.of_node, "reset-on-timeout");
		irq = platform_get_irq(pdev, 0);
		if (!wdt->rst && irq >= 0) {
			ret = devm_request_irq(&pdev->dev, irq, mc_wdt_irq_handler, 0,
					       pdev->name, pdev);
			if (ret) {
				dev_err(&pdev->dev,
					"cannot register interrupt handler err=%d\n",
					ret);
				goto exit;
			}
		}
	}

	/* Initialize the members of mc_wdt structure */
	mc_wdt_device->parent = &pdev->dev;

	ret = watchdog_init_timeout(mc_wdt_device, wdt_timeout, &pdev->dev);
	if (ret) {
		dev_err(&pdev->dev, "unable to set timeout value\n");
		goto exit;
	}

	watchdog_set_nowayout(mc_wdt_device, nowayout);
	watchdog_stop_on_reboot(mc_wdt_device);
	watchdog_set_drvdata(mc_wdt_device, wdt);

	spin_lock_init(&wdt->io_lock);

	ret = watchdog_register_device(mc_wdt_device);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register wdt device\n");
		goto err_clk_disable;
	}
	platform_set_drvdata(pdev, wdt);

	if(of_property_read_bool(pdev->dev.of_node, "no-way-timeout")) {
		set_bit(WDOG_NO_WAY_OUT,&wdt->mc_wdt_device.status);
	} else {
		clear_bit(WDOG_NO_WAY_OUT,&wdt->mc_wdt_device.status);
	}

	if(of_property_read_bool(pdev->dev.of_node, "molchip-soc-restart")) {
		molchip_soc_restart = 1;
	} else {
		molchip_soc_restart = 0;
	}

	dev_dbg(&pdev->dev, "Molchip Watchdog Timer at %p with timeout %ds%s\n",
		 wdt->regs, mc_wdt_device->timeout,
		 nowayout ? ", nowayout" : "");

	return 0;

err_clk_disable:
	clk_mini_disable(CKG_APB_WDT0_EN);
	clk_mini_disable(CKG_WDT_EN);

exit:
	return ret;
}

int molchip_restart_control(void)
{
	return  molchip_soc_restart;
}

/**
 * mc_wdt_remove - Probe call for the device.
 *
 * @pdev: handle to the platform device structure.
 * Return: 0 on success, otherwise negative error.
 *
 * Unregister the device after releasing the resources.
 */
static int mc_wdt_remove(struct platform_device *pdev)
{
	struct mc_wdt *wdt = platform_get_drvdata(pdev);

	mc_wdt_stop(&wdt->mc_wdt_device);
	watchdog_unregister_device(&wdt->mc_wdt_device);
	clk_mini_disable(CKG_WDT_EN);
	clk_mini_disable(CKG_APB_WDT0_EN);

	return 0;
}

/**
 * mc_wdt_shutdown - Stop the device.
 *
 * @pdev: handle to the platform structure.
 *
 */
static void mc_wdt_shutdown(struct platform_device *pdev)
{
	struct mc_wdt *wdt = platform_get_drvdata(pdev);

	mc_wdt_stop(&wdt->mc_wdt_device);
	clk_mini_disable(CKG_WDT_EN);
	clk_mini_disable(CKG_APB_WDT0_EN);
}

/**
 * mc_wdt_suspend - Stop the device.
 *
 * @dev: handle to the device structure.
 * Return: 0 always.
 */
static int __maybe_unused mc_wdt_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mc_wdt *wdt = platform_get_drvdata(pdev);

	if (watchdog_active(&wdt->mc_wdt_device)) {
		mc_wdt_stop(&wdt->mc_wdt_device);
		clk_mini_disable(CKG_WDT_EN);
		clk_mini_disable(CKG_APB_WDT0_EN);
	}

	return 0;
}

/**
 * mc_wdt_resume - Resume the device.
 *
 * @dev: handle to the device structure.
 * Return: 0 on success, errno otherwise.
 */
static int __maybe_unused mc_wdt_resume(struct device *dev)
{
	int ret;
	struct platform_device *pdev = to_platform_device(dev);
	struct mc_wdt *wdt = platform_get_drvdata(pdev);

	if (watchdog_active(&wdt->mc_wdt_device)) {
		ret = clk_mini_enable(CKG_APB_WDT0_EN);
		if (ret) {
			dev_err(&pdev->dev, "unable to enable apb clock\r\n");
			goto fail;
		}

		ret = clk_mini_enable(CKG_WDT_EN);
		if (ret) {
			dev_err(dev, "unable to enable clock\n");
			goto fail;
		}
		mc_wdt_start(&wdt->mc_wdt_device);
	}

	return 0;
fail:
	return ret;
}

static SIMPLE_DEV_PM_OPS(mc_wdt_pm_ops, mc_wdt_suspend, mc_wdt_resume);

static struct of_device_id mc_wdt_of_match[] = {
	{ .compatible = "mc,wdt", },
	{ /* end of table */ }
};
MODULE_DEVICE_TABLE(of, mc_wdt_of_match);

/* Driver Structure */
static struct platform_driver mc_wdt_driver = {
	.probe		= mc_wdt_probe,
	.remove		= mc_wdt_remove,
	.shutdown	= mc_wdt_shutdown,
	.driver		= {
		.name	= "mc-wdt",
		.of_match_table = mc_wdt_of_match,
		.pm	= &mc_wdt_pm_ops,
	},
};

module_platform_driver(mc_wdt_driver);

MODULE_AUTHOR("Molchip, Inc.");
MODULE_DESCRIPTION("Watchdog driver for Molchip WDT");
MODULE_LICENSE("GPL");
