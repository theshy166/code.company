/*
 * (C) Copyright 2024-2025
 * Jerry Cao <jerry.cao@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _LT00_SPL_H_
#define _LT00_SPL_H_


#define IMG_INVALID			0xFFFFFFFF

#define SPL_DOWN_ADDR			0x40500000
#define SPL_LOAD_ADDR			0x27000000
#define SPL_NAND_LOCATION_OFFS		(0 << 11)
#define SPL_NOR_LOCATION_OFFS        	(0 << 10)
#define SPL_EMMC_LOCATION_OFFS        0
#define SPL_SDIO1_LOCATION_OFFS		(8 << 10)
#define SPL_MAX_SIZE			0x80000
#define HEADER_LEN			0x400

#define UBOOT_DOWN_ADDR                 0x40500000
#define UBOOT_LOAD_ADDR			0x41600000
#define UBOOT_NAND_LOCATION_OFFS          (128 << 11)
#define UBOOT_NOR_LOCATION_OFFS           (32 << 10)
#define UBOOT_EMMC_LOCATION_OFFS          (5120 << 10)
#define UBOOT_SDIO1_LOCATION_OFFS         (1024 << 10)
#define UBOOT_MAX_SIZE			0x100000

#define KERNEL_DOWN_ADDR                   0x40500000
#define KERNEL_LOAD_ADDR		   0x40008000
#define KERNEL_NAND_LOCATION_OFFS          (8192 << 11)
#define KERNEL_NOR_LOCATION_OFFS           (1024 << 10)
#define KERNEL_EMMC_LOCATION_OFFS           (6144 << 10)
#define KERNEL_SDIO1_LOCATION_OFFS          (2048 << 10)
#define KERNEL_MAX_SIZE			0xC00000


#define DTB_DOWN_ADDR			0x40448000
#define DTB_LOAD_ADDR                   0x41000000
#define DTB_NOR_LOCATION_OFFS		(9216 << 10)
#define DTB_MAX_SIZE 			0x1000

#define DISK_DOWN_ADDR			0x40450000
#define DISK_LOAD_ADDR			0x41008000
#define DISK_NOR_LOCATION_OFFS		(4448 << 10)
#define DISK_MAX_SIZE			0x300000

#define DDR_NOR_LOCATION_OFFS		(32 << 10)

#define PRE_ROLLING_DOWN_ADDR                 0x41000000
#define PRE_ROLLING_LOAD_ADDR				  0x30010000
#define PRE_ROLLING_NAND_LOCATION_OFFS        (256 << 11)
#define PRE_ROLLING_NOR_LOCATION_OFFS         (64 << 10)
#define PRE_ROLLING_EMMC_LOCATION_OFFS        (64 << 10)
#define PRE_ROLLING_SDIO1_LOCATION_OFFS       (64 << 10)
#define PRE_ROLLING_MAX_SIZE	              0x30000

#define RTT_DOWN_ADDR                 0x40500000
#define RTT_LOAD_ADDR				  0x50300000
#define RTT_NAND_LOCATION_OFFS        (320 << 11)
#define RTT_NOR_LOCATION_OFFS         (256 << 10)
#define RTT_EMMC_LOCATION_OFFS        (256 << 10)
#define RTT_SDIO1_LOCATION_OFFS       (256 << 10)
#define RTT_MAX_SIZE	              0x100000

#define NNMODEL_DOWN_ADDR                 0x61000000
#define NNMODEL_LOAD_ADDR				  0x60300000
#define NNMODEL_NAND_LOCATION_OFFS        (6144 << 11)
#define NNMODEL_NOR_LOCATION_OFFS         (9220 << 10)
#define NNMODEL_EMMC_LOCATION_OFFS        (11268 << 10)
#define NNMODEL_SDIO1_LOCATION_OFFS       (14336 << 10)
#define NNMODEL_MAX_SIZE	              0x1900000

#define RTTAP_DOWN_ADDR                 0x41000000
#define RTTAP_LOAD_ADDR				  0x40000800
#define RTTAP_NAND_LOCATION_OFFS        (1152 << 11)
#define RTTAP_NOR_LOCATION_OFFS         (1152 << 10)
#define RTTAP_EMMC_LOCATION_OFFS        (6144 << 10)
#define RTTAP_SDIO1_LOCATION_OFFS       (0)
#define RTTAP_MAX_SIZE	              0x200000


#define VDSP_DOWN_ADDR                 0x41000000
#define VDSP_LOAD_ADDR				  0x40000800
#define VDSP_NAND_LOCATION_OFFS        (6144 << 11)
#define VDSP_NOR_LOCATION_OFFS         (9220 << 10)
#define VDSP_EMMC_LOCATION_OFFS        (11268 << 10)
#define VDSP_SDIO1_LOCATION_OFFS       (2048 << 10)
#define VDSP_MAX_SIZE	              0x2000000

#define SYSTEM_DOWN_ADDR                 0x46000000
#define SYSTEM_LOAD_ADDR				  0x30010000
#define SYSTEM_NAND_LOCATION_OFFS        (10752 << 11) // 1500000
#define SYSTEM_NOR_LOCATION_OFFS         (64 << 10)
#define SYSTEM_EMMC_LOCATION_OFFS        (64 << 10)
#define SYSTEM_SDIO1_LOCATION_OFFS       (64 << 10)
#define SYSTEM_MAX_SIZE	              0x1800000

#define ROOTFS_DOWN_ADDR                0x46000000
#define ROOTFS_LOAD_ADDR				  0x30010000
#define ROOTFS_NAND_LOCATION_OFFS        (23040 << 11) // 2D00000
#define ROOTFS_NOR_LOCATION_OFFS         (5120 << 10)
#define ROOTFS_EMMC_LOCATION_OFFS        (64 << 10)
#define ROOTFS_SDIO1_LOCATION_OFFS       (64 << 10)
#define ROOTFS_MAX_SIZE	              0x600000

#define DATA_DOWN_ADDR                 0x46000000
#define DATA_LOAD_ADDR				  0x30010000
#define DATA_NAND_LOCATION_OFFS        (26112 << 11) // 3300000
#define DATA_NOR_LOCATION_OFFS         (15360 << 10)
#define DATA_EMMC_LOCATION_OFFS        (64 << 10)
#define DATA_SDIO1_LOCATION_OFFS       (64 << 10)
#define DATA_MAX_SIZE	              0x4000000

#define SPL_IMG_ID                      1
#define Uboot_IMG_ID                    2
#define Kernel_IMG_ID                   3

#define	PRE_ROLLING_IMG_ID	4
#define RTT_IMG_ID			5
#define NNMODEL_ID			6
#define RTTAP_ID			7
#define VDSP_ID			8
#define Dtb_IMG_ID			9
#define Disk_IMG_ID			10
#define SYSTEM_IMG_ID	       4
#define ROOTFS_IMG_ID	       5
#define DATA_IMG_ID	              6


#define IMG_MAX_SIZE			0x4000

#define MAGIC_NUMBER	0x92000000
#define COLD			0x57616B65

/* dummy register, and its usage. */
#define REG_RESERVED12_BASE		(0x10000530)
#define REG_RESERVED13_BASE		(0x10000534)
#define REG_RESERVED14_BASE		(0x10000538)
#define REG_RESERVED15_BASE		(0x1000053C)
#define BOOT_FLAG_AP2CP_BASE		(REG_RESERVED15_BASE)
#define BOOT_FLAG_CP2AP_BASE		(REG_RESERVED14_BASE)
#define MP_FLAG_AP2CP_BASE		(REG_RESERVED13_BASE)
#define MP_FLAG_CP2AP_BASE		(REG_RESERVED12_BASE)

/* linux2prg:
 * 1) before enter low power, set boot flag: BOOT_FLAG_LOWPOWER_START
 * 2) before boot pre-rolling, check boot flag: BOOT_FLAG_NORMAL_BOOT
 * prg2linux:
 * 1) before wake up the whole system, set boot flag: BOOT_FLAG_WAKE_UP
 * */
typedef enum {
        BOOT_FLAG_NORMAL_BOOT = 0x0,
        BOOT_FLAG_LOWPOWER_START = 0x29800000,
        BOOT_FLAG_WAKE_UP = 0x29800001,
        BOOT_FLAG_PRG_STOP = 0x29810002,
} BOOT_FLAG_E;

/* the interact flag in multi-system: spl, linux, pre-rolling, rt-thread.
 * 1) FLAG_MAIN_MEM_READY, after initializing DDR, set this flag in spl.
 * 2) FLAG_OS_READY, after load rt-thread, set this flag in spl.
 * 3) FLAG_SHMEM_RELEASED, after iram->DDR, set this flag in pre-rolling.
 * */
typedef enum {
        FLAG_MAIN_MEM_READY = 0x29810000,
        FLAG_OS_READY = 0x29810001,
        FLAG_SHMEM_RELEASED = 0x29810002,
        FLAG_RUNTIME_SPACE_RELEASED = 0x29810003
} MS_INTERACT_FLAG_E;


#endif


