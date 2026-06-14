/*
 * mc_platform_device.h - generic, centralized driver model
 *
 * Copyright (c) 2001-2003 Patrick Mochel <mochel@osdl.org>
 *
 * This file is released under the GPLv2
 *
 * See Documentation/driver-model/ for more information.
 */

#ifndef _MC_PLATFORM_DEVICE_H_
#define _MC_PLATFORM_DEVICE_H_

#include <linux/platform_device.h>

extern struct resource *mc_platform_get_resource(struct platform_device *dev,
				       unsigned int type, unsigned int num);

extern int mc_platform_get_irq(struct platform_device *dev, unsigned int num);

#define mc_platform_driver_register(drv) \
	__mc_platform_driver_register(drv, THIS_MODULE)

extern int __mc_platform_driver_register(struct platform_driver *drv,
				struct module *owner);
extern void mc_platform_driver_unregister(struct platform_driver *drv);

#endif /* _MC_PLATFORM_DEVICE_H_ */
