/*
 *  linux/arch/arm/plat-molchip/include/plat/platsmp.h
 *
 *  Copyright (C) 201o MOLCHIP Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

extern void molchip_secondary_startup(void);
extern void molchip_secondary_init(unsigned int cpu);
extern int  molchip_boot_secondary(unsigned int cpu, struct task_struct *idle);
