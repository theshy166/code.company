/*
 * mc_platform.c - platform 'pseudo' bus for legacy devices
 *
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 *
 * This file is released under the GPLv2
 *
 * Please see Documentation/driver-model/platform.txt for more
 * information.
 */

#include <linux/string.h>
#include <linux/mc_platform_device.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
//#include <linux/bootmem.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/pm_domain.h>
#include <linux/idr.h>
#include <linux/acpi.h>
#include <linux/clk/clk-conf.h>
#include <linux/limits.h>
#include <linux/property.h>

#include "base.h"
#include "power/power.h"

int __mc_platform_driver_register(struct platform_driver *drv,
				struct module *owner)
{
	return __platform_driver_register(drv, owner);
}
EXPORT_SYMBOL(__mc_platform_driver_register);

void mc_platform_driver_unregister(struct platform_driver *drv)
{
	platform_driver_unregister(drv);
}
EXPORT_SYMBOL(mc_platform_driver_unregister);

struct resource *mc_platform_get_resource(struct platform_device *dev,
				       unsigned int type, unsigned int num)
{
	return platform_get_resource(dev, type, num);
}
EXPORT_SYMBOL(mc_platform_get_resource);

int mc_platform_get_irq(struct platform_device *dev, unsigned int num)
{
	return platform_get_irq(dev, num);
}
EXPORT_SYMBOL(mc_platform_get_irq);
