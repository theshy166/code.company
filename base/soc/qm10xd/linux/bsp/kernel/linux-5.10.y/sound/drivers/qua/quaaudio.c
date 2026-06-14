/*
 * linux/sound/drivers/qua/quaaudio.c
 *
 * simple ioctl for qua audio device control
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/extcon.h>
#include <linux/extcon-provider.h>
#include "quaaudio.h"
#include <linux/delay.h>

#define QUA_AUDIO_NAME "quaaudio"
#define MAX_AUDIO_DEV 2

static struct extcon_dev *extcon_virtual_jack;
struct qua_audio_ops g_qua_ops[MAX_AUDIO_DEV];
static struct class *qua_audio_class;
static int qua_audio_major;
static int rst_gpio;
static bool reset_active_low;
static int dev_num = 0;

struct gpio_extcon_data {
    struct extcon_dev *sdev;
    unsigned gpio;
    struct delayed_work work;
};

int open_dev_default(void) {
    printk("open dev\n");
    if (gpio_is_valid(rst_gpio)) {
        gpio_set_value(rst_gpio, reset_active_low ? QUA_GPIO_LOW_LEVEL : QUA_GPIO_HIGH_LEVEL);
    }
    return 0;
}

int close_dev_default(void) {
    printk("close dev\n");
    if (gpio_is_valid(rst_gpio)) {
        gpio_set_value(rst_gpio, reset_active_low ? QUA_GPIO_HIGH_LEVEL : QUA_GPIO_LOW_LEVEL);
    }
    return 0;
}

void qua_audio_ops_set(struct qua_audio_ops *ops) {
    if (dev_num < MAX_AUDIO_DEV) {
        if (ops && ops->open_dev)
            g_qua_ops[dev_num].open_dev = ops->open_dev;

        if (ops && ops->close_dev)
            g_qua_ops[dev_num].close_dev = ops->close_dev;

        g_qua_ops[dev_num].data = ops->data;
    }
    dev_num++;
}
EXPORT_SYMBOL_GPL(qua_audio_ops_set);
static int qua_audio_suspend(struct platform_device *pdev, pm_message_t state) {
    int dev_index;
    if (dev_num > 0) {
        for (dev_index = 0; dev_index < dev_num; dev_index++) {
            if (g_qua_ops[dev_index].close_dev)
                g_qua_ops[dev_index].close_dev(g_qua_ops[dev_index].data);
        }
    } else {
        close_dev_default();
    }

    return 0;
}

static int qua_audio_resume(struct platform_device *pdev) {
    int dev_index;
    if (dev_num > 0) {
        for (dev_index = 0; dev_index < dev_num; dev_index++) {
            if (g_qua_ops[dev_index].open_dev)
                g_qua_ops[dev_index].open_dev(g_qua_ops[dev_index].data);
        }
    } else {
        open_dev_default();
    }
    return 0;
}

static int qua_audio_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int qua_audio_release(struct inode *inode, struct file *file)
{
    return 0;
}

static long qua_audio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int dev_index;
    switch (cmd) {
        case QUA_AUDIO_IOCTL_DEV_OPEN:
            if (dev_num > 0) {
                for (dev_index = 0; dev_index < dev_num; dev_index++) {
                    if (g_qua_ops[dev_index].open_dev)
                        g_qua_ops[dev_index].open_dev(g_qua_ops[dev_index].data);
                }
            } else {
                open_dev_default();
            }
            break;

        case QUA_AUDIO_IOCTL_DEV_CLOSE:
            if (dev_num > 0) {
                for (dev_index = 0; dev_index < dev_num; dev_index++) {
                    if (g_qua_ops[dev_index].close_dev)
                        g_qua_ops[dev_index].close_dev(g_qua_ops[dev_index].data);
                }
            } else {
                close_dev_default();
            }
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static void gpio_work(struct work_struct *work)
{
    struct gpio_extcon_data *extcon_data;
    int val;

    extcon_data = container_of(work, struct gpio_extcon_data, work.work);
    val = gpio_get_value(extcon_data->gpio);
    extcon_set_state_sync(extcon_data->sdev, EXTCON_JACK_MICROPHONE, val);
    pr_info("extcon val is %d\n", val);
}

static irqreturn_t headset_irq(int irq, void *priv) {
    struct gpio_extcon_data *extcon_data = (struct gpio_extcon_data *)priv;
    queue_delayed_work(system_power_efficient_wq, &extcon_data->work,
        msecs_to_jiffies(150));
    return IRQ_HANDLED;
}

static const struct file_operations qua_audio_fops = {
    .owner = THIS_MODULE,
    .open = qua_audio_open,
    .release = qua_audio_release,
    .unlocked_ioctl = qua_audio_ioctl,
};

static const unsigned int jack_cable[] = {
    EXTCON_JACK_MICROPHONE,
    EXTCON_JACK_HEADPHONE,
    EXTCON_JACK_LINE_IN,
    EXTCON_NONE,
};

static int register_extcon_device_for_jack(struct platform_device *pdev)
{
    int ret = -1;
    struct device *dev = &pdev->dev;
    extcon_virtual_jack = devm_extcon_dev_allocate(dev, jack_cable);
    if (IS_ERR_OR_NULL(extcon_virtual_jack)) {
        pr_err("extcon_dev_allocate fail\n");
        return ret;
    }
    ret = devm_extcon_dev_register(dev, extcon_virtual_jack);
    if (ret) {
        pr_err("extcon_dev_register extcon jack err!\n");
        return ret;
    }
    device_rename(&extcon_virtual_jack->dev, "audio");
    return 0;
}

static void unregister_extcon_device_for_jack(void)
{
    if (extcon_virtual_jack)
        extcon_dev_unregister(extcon_virtual_jack);
}

static int qua_audio_probe(struct platform_device *pdev) {
    int ret = -1;
    struct device *dev = &pdev->dev;
    struct device *qua_audio_device;
    enum of_gpio_flags gpio_flags;
    struct gpio_extcon_data *extcon_data = NULL;
    int gpio_headset_det = 0;

    qua_audio_major = register_chrdev(0, QUA_AUDIO_NAME, &qua_audio_fops);
    if (qua_audio_major < 0) {
        pr_err("Failed to register char device\n");
        goto err;
    }

    qua_audio_class = class_create(THIS_MODULE, QUA_AUDIO_NAME);
    if (IS_ERR(qua_audio_class)) {
        pr_err("device class file already in use\n");
        goto err_class;
    }

    qua_audio_device = device_create(qua_audio_class, NULL, MKDEV(qua_audio_major, 0),
        NULL, "%s", QUA_AUDIO_NAME);

    if (IS_ERR(qua_audio_device)) {
        pr_err("failed to create device\n");
        goto err_device;
    }

    if (IS_ENABLED(CONFIG_OF) && dev->of_node) {
        enum of_gpio_flags flags;
        rst_gpio = -1;
        ret = of_get_named_gpio_flags(dev->of_node, "reset-gpio", 0, &flags);
        if (ret < 0) {
            pr_err("no reset-gpio\n");
        } else {
            rst_gpio = ret;
            reset_active_low = flags & OF_GPIO_ACTIVE_LOW;

            gpio_request(rst_gpio, "qua_audio_reset");
            gpio_direction_output(rst_gpio, reset_active_low ? QUA_GPIO_HIGH_LEVEL : QUA_GPIO_LOW_LEVEL);
        }

        gpio_headset_det = of_get_named_gpio_flags(dev->of_node, "headset-det-gpios", 0, &gpio_flags);
        if (gpio_headset_det < 0) {
            pr_err("no headset-det-gpios\n");
        } else {
            extcon_data = kzalloc(sizeof(struct gpio_extcon_data), GFP_KERNEL);
            if (!extcon_data)
                goto err_extcon;

            register_extcon_device_for_jack(pdev);
            extcon_data->gpio = gpio_headset_det;
            extcon_data->sdev = extcon_virtual_jack;

            devm_gpio_request(dev, gpio_headset_det, "headset-det");
            INIT_DELAYED_WORK(&extcon_data->work, gpio_work);
            gpio_direction_input(gpio_headset_det);
            ret = devm_request_irq(dev, gpio_to_irq(gpio_headset_det), headset_irq,
                IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_SHARED , "headset plug", extcon_data);
            if (ret < 0) {
                pr_err("irq request fail\n");
                goto err_extcon;
            }
            schedule_delayed_work(&extcon_data->work, msecs_to_jiffies(150));
        }
    }
    return 0;
err_extcon:
    if (extcon_data)
        kfree(extcon_data);
err_device:
    class_destroy(qua_audio_class);
err_class:
    unregister_chrdev(qua_audio_major, QUA_AUDIO_NAME);
err:
    return ret;
}

static int qua_audio_remove(struct platform_device *pdev)
{
    device_destroy(qua_audio_class, MKDEV(qua_audio_major, 0));
    class_destroy(qua_audio_class);
    unregister_chrdev(qua_audio_major, QUA_AUDIO_NAME);
    unregister_extcon_device_for_jack();
    return 0;
}

static const struct of_device_id qua_audio_match[] = {
    {
        .compatible = "qua,audio",
    },
    {},
};

MODULE_DEVICE_TABLE(of, qua_audio_match);

 static struct platform_driver qua_audio_driver = {
    .probe = qua_audio_probe,
    .remove = qua_audio_remove,
    .suspend = qua_audio_suspend,
    .resume = qua_audio_resume,
    .driver = {
         .name = QUA_AUDIO_NAME,
         .of_match_table = qua_audio_match,
    },
};

static __init int qua_audio_init(void) {
    return platform_driver_register(&qua_audio_driver);
}

static __exit void qua_audio_exit(void)
{
    platform_driver_unregister(&qua_audio_driver);
}

module_init(qua_audio_init);
module_exit(qua_audio_exit);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Qua audio driver");
MODULE_ALIAS("qua audio module");
