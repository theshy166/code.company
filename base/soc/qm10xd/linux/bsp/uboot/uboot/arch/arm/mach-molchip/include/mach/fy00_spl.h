/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _FY00_SPL_H_
#define _FY00_SPL_H_


#define IMG_INVALID			0xFFFFFFFF

#define SPL_DOWN_ADDR			0x41000000
#define SPL_LOAD_ADDR			0x00100000
#define SPL_NAND_LOCATION_OFFS		(0 << 11)
#define SPL_NOR_LOCATION_OFFS        	(0 << 10)
#define SPL_EMMC_LOCATION_OFFS        0

#define SPL_MAX_SIZE			0x50000

#define UBOOT_DOWN_ADDR                 0x41000000
#define UBOOT_LOAD_ADDR			0x41600000
#define UBOOT_NAND_LOCATION_OFFS          (128 << 11)
#define UBOOT_NOR_LOCATION_OFFS           (32 << 10)
#define UBOOT_EMMC_LOCATION_OFFS           (32 << 10)

#define UBOOT_MAX_SIZE			0x60000

#define KERNEL_DOWN_ADDR                   0x40048000
#define KERNEL_LOAD_ADDR		   0x40008000
#define KERNEL_NAND_LOCATION_OFFS          (48 << 10)
#define KERNEL_NOR_LOCATION_OFFS           (320 << 10)
#define KERNEL_EMMC_LOCATION_OFFS           (320 << 10)

#define KERNEL_MAX_SIZE			0x400000


#define DTB_DOWN_ADDR			0x40448000
#define DTB_LOAD_ADDR                   0x41000000
#define DTB_NOR_LOCATION_OFFS		(4416 << 10)
#define DTB_MAX_SIZE 			0x1000

#define DISK_DOWN_ADDR			0x40450000
#define DISK_LOAD_ADDR			0x41008000
#define DISK_NOR_LOCATION_OFFS		(4448 << 10)
#define DISK_MAX_SIZE			0x300000

#define DDR_NOR_LOCATION_OFFS		(32 << 10)


#define SPL_IMG_ID                      1
#define Uboot_IMG_ID                    2
#define Kernel_IMG_ID                   3
#define Dtb_IMG_ID			4
#define Disk_IMG_ID			5

#define IMG_MAX_SIZE			0x4000


#endif


