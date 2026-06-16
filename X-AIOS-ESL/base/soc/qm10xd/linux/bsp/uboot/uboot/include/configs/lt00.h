/*
 * (C) Copyright 2024 Molchip
 * Jerry Cao, <jerry.cao@molchip.com>
 *
 * Configuration for Molchip LT00 chip. Parts were derived from other ARM
 *   configurations.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MOLCHIP_LT00_H
#define __MOLCHIP_LT00_H

#define V2M_BASE		0x40000000
#define CONFIG_SYS_TEXT_BASE	0x41600000
#define CONFIG_SYS_LOAD_ADDR		(V2M_BASE + 0x01600000)

/* additions for new relocation code */
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - GENERATED_GBL_DATA_SIZE)

#define LINUX_BOOT_PARAM_ADDR		(V2M_BASE + 0x2000)

/* env in flash instead of CFG_ENV_IS_NOWHERE */
#ifdef CONFIG_CMD_NAND
#define CONFIG_ENV_OFFSET		0x20000
#else
#define CONFIG_ENV_OFFSET		0x90000
#endif
#define CONFIG_ENV_SECT_SIZE		0x10000

#define FLASH_MAX_SECTOR_SIZE		0x0008000	/* 32 KB sectors */
#define CONFIG_ENV_SIZE			FLASH_MAX_SECTOR_SIZE
/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 1024 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			(V2M_BASE)	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE		0x10000000	/* 256 MB */


/*v2m_serial0: uart@090000*/
#define V2M_UART0		       0x18400000

#define CONFIG_FH_UART
#define CONFIG_CONS_INDEX		0

/* timer */
#define V2M_TIMER01		0x09700000
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

#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS

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
#define SF_READ_CONTINUE_MODE/**/
/* 181207,zimage boot ok */
#define MY_STR_BOOTZIMAGE \
	"help;"
/* fail */
#ifdef CONFIG_CMD_NAND
#ifdef LINUX_KERNEL_SIZE_HEX
#define MY_STR_BOOTIMAGE \
                        "nand read 0x40007fc0 boot " __stringify(LINUX_KERNEL_SIZE_HEX) ";" \
                        "bootz 0x40007fc0 - -;"
#else
#define MY_STR_BOOTIMAGE \
			"nand read 0x40007fc0 boot 0x400000;" \
			"bootz 0x40007fc0 - -;"
#endif
#else
#ifndef CONFIG_MOLCHIP_FASTBOOT
#define MY_STR_BOOTIMAGE \
	"sf probe;" \
	"sf read 0x40008000 0x100000 0x700000;" \
	"bootz 0x40008000 - -;"
#elif defined SF_READ_CONTINUE_MODE
#define MY_STR_BOOTIMAGE \
		"sf probe;" \
		"sfrm cont_nw;" \
		"sf read 0x40008000 0x100000 0x3c0000;" \
		"sfrd 0x40008000 0x3c0000;" \
		"bootz 0x40008000 - -;"
#else
#define MY_STR_BOOTIMAGE \
	"sf probe;" \
	"sf read 0x40008000 0x110000 0x400000;" \
	"bootz 0x40008000 - -;"
#endif
#endif
#define CONFIG_BOOTCOMMAND		MY_STR_BOOTIMAGE


#ifdef CONFIG_UBOOT_XKPHOTO
/*boot to recovery mode */
#define MY_STR_RECOVERYZIMAGE \
        "usb stop;usb start;fatload usb 0:1 0x40008000 recovery.img;" \
        "bootz 0x40008000 - -; "

#else
/*recovery kernel start addr: 0x00400000*/
/*
 #: name                size            offset          mask_flags
 0: boot-spl            0x00020000      0x00000000      0
 1: boot-env            0x00020000      0x00020000      0
 2: boot-uboot          0x000c0000      0x00040000      0
 3: logo                0x00100000      0x00100000      0
 4: padding             0x00100000      0x00200000      0
 5: misc                0x00100000      0x00300000      0
 6: recovery            0x00c00000      0x00400000      0
 7: boot                0x00500000      0x01000000      0
 8: system              0x01800000      0x01500000      0
 9: rootfs              0x00600000      0x02d00000      0
10: data                0x04d00000      0x03300000      0
*/
/*boot to recovery mode */
#ifdef RECOVERY_KERNEL_SIZE_HEX
#define MY_STR_RECOVERYZIMAGE \
	"nand read 0x40008000 recovery " __stringify(RECOVERY_KERNEL_SIZE_HEX) ";" \
	"bootz 0x40008000 - -; "
#else
#define MY_STR_RECOVERYZIMAGE \
	"nand read 0x40008000 recovery 0x00300000;" \
	"bootz 0x40008000 - -; "
#endif
#endif

#define CONFIG_SPL_TEXT_BASE		0x27000400
#define CONFIG_SPL_STACK		0x2700B000
#define CORE1_STACK_TOP			0x40002000
#define CORE1_STACK_SIZE		0x2000

#define CONFIG_SPL_PAD_TO  0x8000

#define	CONFIG_USB_MAX_CONTROLLER_COUNT	2
#define PHYS_64MB_SIZE			0x4000000

#define MMU_DCACHE_EN

#if 1
#ifdef MMU_DCACHE_EN
#define CONFIG_SYS_NONCACHED_MEMORY     (1 << 20)
#endif
#endif

#endif /* __MOLCHIP_LT00_H */
