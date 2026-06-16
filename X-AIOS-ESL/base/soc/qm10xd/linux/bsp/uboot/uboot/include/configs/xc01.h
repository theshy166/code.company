/*
 * (C) Copyright 2011 Linaro
 * Ryan Harkin, <ryan.harkin@linaro.org>
 *
 * Configuration for Versatile Express. Parts were derived from other ARM
 *   configurations.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MOLCHIP_XC01_H
#define __MOLCHIP_XC01_H

#define V2M_BASE		0x40000000
#define CONFIG_SYS_TEXT_BASE	0x41600000
#define CONFIG_SYS_LOAD_ADDR		(V2M_BASE + 0x01600000)

/* additions for new relocation code */
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - GENERATED_GBL_DATA_SIZE)

#define LINUX_BOOT_PARAM_ADDR		(V2M_BASE + 0x2000)

/* env in flash instead of CFG_ENV_IS_NOWHERE */
#ifdef CONFIG_CMD_NAND
#define CONFIG_ENV_OFFSET		0x1120000
#else
#define CONFIG_ENV_OFFSET		0x110000
#endif
#define CONFIG_ENV_SECT_SIZE		0x10000

#define FLASH_MAX_SECTOR_SIZE		0x0008000	/* 32 KB sectors */
#define CONFIG_ENV_SIZE			FLASH_MAX_SECTOR_SIZE
/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			(V2M_BASE)	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE		0x10000000	/* 256 MB */


/*v2m_serial0: uart@090000*/
#define V2M_UART0		       0x18400000

#define CONFIG_FH_UART
#define CONFIG_CONS_INDEX		0

/* timer */
#define V2M_TIMER01		0x18A00000
#define CONFIG_SYS_TIMER_RATE		1000
#define CONFIG_SYS_TIMER_COUNTER	(V2M_TIMER01 + 0x4)

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
#ifdef CONFIG_MC_SPI_NFC_NAND
	#define CONFIG_SYS_MAX_NAND_DEVICE	CONFIG_SPI_NAND_MAX_CHIP_NUM
	#define CONFIG_SYS_NAND_MAX_CHIPS	CONFIG_SPI_NAND_MAX_CHIP_NUM
#endif
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
	"help;"
/* fail */
#define MY_STR_BOOTIMAGE \
	"help;" \
	"mmc read 0x40007800 0x3000 0x8000;" \
	"bootx 0x40008000 - 0x41000000; "

#define CONFIG_BOOTCOMMAND		MY_STR_BOOTIMAGE

#define CONFIG_SPL_TEXT_BASE		0x30000800
#define CONFIG_SPL_STACK		0x30050000
#define CORE1_STACK_TOP			0x40002000
#define CORE1_STACK_SIZE		0x2000

#define CONFIG_SPL_PAD_TO  0x8000

#define	CONFIG_USB_MAX_CONTROLLER_COUNT	2
#define PHYS_64MB_SIZE			0x4000000

#endif /* __MOLCHIP_H */
