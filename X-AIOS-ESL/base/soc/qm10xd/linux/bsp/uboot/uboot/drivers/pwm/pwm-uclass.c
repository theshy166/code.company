/*
 * Copyright (c) 2016 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <pwm.h>
#ifdef CONFIG_PWM_MOLCHIP
#include <dm/uclass.h>
#include <dm/device-internal.h>
#endif

int pwm_set_invert(struct udevice *dev, uint channel, bool polarity)
{
	struct pwm_ops *ops = pwm_get_ops(dev);

	if (!ops->set_invert)
		return -ENOSYS;

	return ops->set_invert(dev, channel, polarity);
}

int pwm_set_config(struct udevice *dev, uint channel, uint period_ns,
		   uint duty_ns)
{
	struct pwm_ops *ops = pwm_get_ops(dev);

	if (!ops->set_config)
		return -ENOSYS;

	return ops->set_config(dev, channel, period_ns, duty_ns);
}

int pwm_set_enable(struct udevice *dev, uint channel, bool enable)
{
	struct pwm_ops *ops = pwm_get_ops(dev);

	if (!ops->set_enable)
		return -ENOSYS;

	return ops->set_enable(dev, channel, enable);
}

#ifdef CONFIG_PWM_MOLCHIP
int pwm_open(struct udevice *dev, bool enable)
{
	struct pwm_ops *ops = pwm_get_ops(dev);

	if (!ops->udev_open)
		return -ENOSYS;

	return ops->udev_open(dev, enable);
}

struct udevice *pwm_get_dev_by_name(const char *devname)
{
	struct udevice *it;
	struct uclass *uc;

	uclass_get(UCLASS_PWM, &uc);
	uclass_foreach_dev(it, uc) {
		if (device_probe(it))
			continue;
		/* Check for the name or the sequence number to match */
		if (strcmp(it->driver->name, devname) == 0)
			return it;
	}

	return NULL;
}
#endif

UCLASS_DRIVER(pwm) = {
	.id		= UCLASS_PWM,
	.name		= "pwm",
};
