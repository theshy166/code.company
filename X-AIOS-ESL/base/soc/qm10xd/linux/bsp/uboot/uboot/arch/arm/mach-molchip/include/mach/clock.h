/*
 * (C) Copyright 2019
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _CLOCK_H
#define _CLOCK_H

#include <linux/types.h>

#if defined(CONFIG_MACH_FY01)
#include <asm/arch/clock_fy01.h>
#endif

#ifndef __ASSEMBLY__
int clock_init(void);
#endif

#endif /* _SUNXI_CLOCK_H */
