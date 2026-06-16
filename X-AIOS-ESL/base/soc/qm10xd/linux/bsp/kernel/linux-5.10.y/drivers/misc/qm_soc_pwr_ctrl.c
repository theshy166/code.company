#include "linux/dev_printk.h"
#include "linux/gpio/consumer.h"
#include "linux/of.h"
#include "linux/property.h"
#include "linux/reboot.h"
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/syscore_ops.h>
#include <linux/platform_data/qm_soc_pwr_ctrl.h>


static unsigned long g_action = 0;

static int reboot_notifier(struct notifier_block *nb,
							 unsigned long action, void *data)
{
	switch (action) {
	case SYS_RESTART:
		g_action = SYS_RESTART;
		//pr_info("system is restarting\n");
		break;
	case SYS_POWER_OFF:
		g_action = SYS_POWER_OFF;
		//pr_info("system is powering off\n");
		break;
	case SYS_HALT:
		g_action = SYS_POWER_OFF;
		//pr_info("system is halting\n");
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block reboot_nb = {
	.notifier_call = reboot_notifier,
};

struct soc_power_ctrl {
	int pwr_gpio; // GPIO pin used to indicate status or control power
	unsigned power_on_level; // level indicating power on (0:low, 1:high)
	struct syscore_ops syscore_ops;
};

static void soc_power_ctrl_syscore_shutdown(void);

static struct soc_power_ctrl g_soc_pwr = {
	.pwr_gpio = -EINVAL,
	.power_on_level = 0,
	.syscore_ops = {
		.shutdown = soc_power_ctrl_syscore_shutdown,
	},
};

static void soc_power_ctrl_syscore_shutdown(void)
{
	if (g_action == SYS_RESTART)
		return;

	if (gpio_is_valid(g_soc_pwr.pwr_gpio)) {
		pr_info("going to shutdown\n");
		gpio_set_value(g_soc_pwr.pwr_gpio, !g_soc_pwr.power_on_level);
	}
}

static int soc_power_ctrl_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const struct soc_power_ctrl_platdata *pdata = dev_get_platdata(dev);
	int ret;

	if (pdata) {
		if (!gpio_is_valid(pdata->pwr_gpio)) {
			dev_err(dev, "pwr gpio %d is invalid\n", pdata->pwr_gpio);
			return -EINVAL;
		}
		g_soc_pwr.pwr_gpio = pdata->pwr_gpio;
		g_soc_pwr.power_on_level = pdata->power_on_level;

	} else {
		g_soc_pwr.pwr_gpio = of_get_named_gpio(dev->of_node, "pwr-gpio", 0);
		if (g_soc_pwr.pwr_gpio < 0) {
			dev_err(dev, "can not get pwr gpio\n");
			return -EINVAL;
		}
		dev_info(dev, "pwr_gpio=%d\n", g_soc_pwr.pwr_gpio);
		if (device_property_read_u32(dev, "power-on-level", &g_soc_pwr.power_on_level)) {
			dev_err(dev, "can not get power level property\n");
			return -EINVAL;
		}
		dev_info(dev, "power-on-level=%d\n", g_soc_pwr.power_on_level);
	}

	ret = devm_gpio_request_one(dev, g_soc_pwr.pwr_gpio,
				g_soc_pwr.power_on_level ? GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW,
				"soc_power_indicator");
	if (ret) {
		if (ret == -EPROBE_DEFER) {
			dev_dbg(dev, "GPIO controller not ready, deferring probe\n");
			return -EPROBE_DEFER;
		}
		dev_err(dev, "Failed to request status-gpio\n");
		return ret;
	}

	register_syscore_ops(&g_soc_pwr.syscore_ops);
	register_reboot_notifier(&reboot_nb);

	dev_info(dev, "probe success\n");
	return 0;
}

static int soc_power_ctrl_remove(struct platform_device *pdev)
{
	unregister_reboot_notifier(&reboot_nb);
	pr_info("%s\n", __FUNCTION__);
	return 0;
}

static const struct of_device_id soc_power_ctrl_of_match[] = {
	{ .compatible = "qm,soc-power-ctrl" },
	{ }
};
MODULE_DEVICE_TABLE(of, soc_power_ctrl_of_match);

static struct platform_driver soc_power_ctrl_driver = {
	.probe = soc_power_ctrl_probe,
	.remove = soc_power_ctrl_remove,
	.driver = {
		.name = "qm-soc-power-ctrl",
		.of_match_table = soc_power_ctrl_of_match,
	},
};

module_platform_driver(soc_power_ctrl_driver);

MODULE_AUTHOR("QuaMing");
MODULE_DESCRIPTION("SOC power indicator driver");
MODULE_LICENSE("GPL");