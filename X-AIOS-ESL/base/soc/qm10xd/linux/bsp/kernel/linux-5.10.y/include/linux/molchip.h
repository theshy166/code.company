/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Copyright (C) 2019 MOLCHIP Limited
 */

#ifndef _LINUX_MOLCHIP_H
#define _LINUX_MOLCHIP_H

#include <linux/device.h>
#include <linux/regmap.h>



/* Config bridge API */


/* Config regmap API */


/* Platform control */

void molchip_flags_set(u32 data);
void molchip_flags_holding_pen(void);

/*for gpio wake*/
struct mol_gpio_wake {
	int gpio_wakeup_source_num;
	int gpio_wakeup_source[64];
};

#ifdef CONFIG_MOLCHIP_WAKEUP_SOURCE

void mol_int_ctrlx_get_state(void);
bool gpio_is_wakeup_sys(unsigned int gpio_no);
bool external_phy_wakeup_sys(void);
bool get_mol_wakeup_source(const char *name, void **data);

#else

static inline void mol_int_ctrlx_get_state(void) {
	return;
}

static inline bool gpio_is_wakeup_sys(unsigned int gpio_no)
{
	return false;
}

static inline bool external_phy_wakeup_sys(void) {
	return false;
}

static inline bool get_mol_wakeup_source(const char *name, void **data) {
	return false;
}

#endif //CONFIG_MOLCHIP_WAKEUP_SOURCE

#endif
