/*
 * (C) Copyright 2011 Linaro
 * Ryan Harkin, <ryan.harkin@linaro.org>
 *
 * Configuration for Versatile Express. Parts were derived from other ARM
 *   configurations.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MOLCHIP_H
#define __MOLCHIP_H

#define V2M_BASE		0x80000000
#define CONFIG_SYS_TEXT_BASE	0x81600000
#define CONFIG_SYS_LOAD_ADDR		(V2M_BASE + 0x01600000)

/* additions for new relocation code */
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - GENERATED_GBL_DATA_SIZE)

#define LINUX_BOOT_PARAM_ADDR		(V2M_BASE + 0x2000)

#define FLASH_MAX_SECTOR_SIZE		0x0008000	/* 32 KB sectors */
#define CONFIG_ENV_SIZE			FLASH_MAX_SECTOR_SIZE
/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			(V2M_BASE)	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE		0x10000000	/* 256 MB */


/*v2m_serial0: uart@090000*/
#define V2M_UART0		       0x13030000	

#define CONFIG_FH_UART
#define CONFIG_CONS_INDEX		0

/* timer */
#define V2M_TIMER01		0x13110000 /*0x2c000600*/
#define CONFIG_SYS_TIMER_RATE		1000000
#define CONFIG_SYS_TIMER_COUNTER	(V2M_TIMER01 + 0x4)
#define CONFIG_SYS_TIMER_COUNTS_DOWN

#if 1	/* todo-:only for build */
 #define BOOTENV 
#else
/*
#include <config_distro_bootcmd.h>
*/
#endif
/*
#include <config_distro_defaults.h>
*/
/*
 * List of all commands and options that when defined enables support for
 * features required by distros to support boards in a standardised and
 * consistent manner.
 */

#define CONFIG_BOOTP_BOOTPATH
/*
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
*/
#define CONFIG_BOOTP_PXE
/*
#define CONFIG_BOOTP_SUBNETMASK

#define CONFIG_CMDLINE_EDITING
*/
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_LONGHELP
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_ENV_VARS_UBOOT_CONFIG

/* 181207,zimage boot ok */
#define MY_STR_BOOTZIMAGE \
	"help;" \
	"bootz 0x80008000 - 0x81000000; "
/* fail */
#define MY_STR_BOOTIMAGE \
	"help;" \
	"bootm 0x80008000 - 0x81000000; "
	
#define CONFIG_BOOTCOMMAND		MY_STR_BOOTZIMAGE

#endif /* __MOLCHIP_H */
