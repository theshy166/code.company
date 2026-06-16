/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __ASM_ARM_ARCH_HARDWARE_H__
#define __ASM_ARM_ARCH_HARDWARE_H__

#if defined(CONFIG_TARGET_TS01)
# include <mach/molchip_ts01.h>
#elif defined(CONFIG_TARGET_FY01)
# include <mach/molchip_fy01.h>
#elif defined(CONFIG_TARGET_FY02)
# include <mach/molchip_fy02.h>
#elif defined(CONFIG_TARGET_FY02B)
# include <mach/molchip_fy02b.h>
#elif defined(CONFIG_TARGET_FY02P)
# include <mach/molchip_fy02p.h>
#elif defined(CONFIG_TARGET_FY10P)
# include <mach/molchip_fy10p.h>
#elif defined(CONFIG_TARGET_FY00)
# include <mach/molchip_fy00.h>
#elif defined(CONFIG_TARGET_FY10D)
# include <mach/molchip_fy10d.h>
#elif defined(CONFIG_TARGET_FY10DS)
# include <mach/molchip_fy10d.h>
#elif defined(CONFIG_TARGET_FY12)
# include <mach/molchip_fy12.h>
#elif defined(CONFIG_TARGET_XC01)
# include <mach/molchip_xc01.h>
#elif defined(CONFIG_TARGET_LT00)
# include <mach/molchip_lt00.h>
#elif defined(CONFIG_TARGET_MC3302)
# include <mach/molchip_mc3302.h>
#else
# error "Unsupported Molchip processor"
#endif

#endif /* __ASM_ARM_ARCH_HARDWARE_H__ */

