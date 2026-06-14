/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _MOLCHIP_SPL_H_
#define _MOLCHIP_SPL_H_

#if defined(CONFIG_TARGET_TS01)
#include <mach/ts01_spl.h>
#elif defined(CONFIG_TARGET_FY01)
#include <mach/fy01_spl.h>
#elif defined(CONFIG_TARGET_FY02)
#include <mach/fy02_spl.h>
#elif defined(CONFIG_TARGET_FY02B)
#include <mach/fy02b_spl.h>
#elif defined(CONFIG_TARGET_FY02P)
#include <mach/fy02p_spl.h>
#elif defined(CONFIG_TARGET_FY10P)
#include <mach/fy10p_spl.h>
#elif defined(CONFIG_TARGET_FY00)
#include <mach/fy00_spl.h>
#elif defined(CONFIG_TARGET_FY10D)
#include <mach/fy00_spl.h>
#elif defined(CONFIG_TARGET_FY10DS)
#include <mach/fy00_spl.h>
#elif defined(CONFIG_TARGET_FY12)
#include <mach/fy12_spl.h>
#elif defined(CONFIG_TARGET_XC01)
#include <mach/xc01_spl.h>
#elif defined(CONFIG_TARGET_LT00)
#include <mach/lt00_spl.h>
#elif defined(CONFIG_TARGET_MC3302)
#include <mach/mc3302_spl.h>
#else
# error "Unsupported Molchip processor"
#endif



enum{
	BOOT_DEVICE_RAM,
	BOOT_DEVICE_EMMC,
	BOOT_DEVICE_NAND,
	BOOT_DEVICE_RAWNAND,
	BOOT_DEVICE_ONENAND,
	BOOT_DEVICE_NOR,
	BOOT_DEVICE_BOOTROM,
	BOOT_DEVICE_SDIO0,
	BOOT_DEVICE_SDIO1,
	BOOT_DEVICE_USB,
	BOOT_DEVICE_NONE

};

typedef enum boot_type{
	pre_rolling,
	rtt,
	uboot,
	kernel,
	nnmodel
}boot_type_t;

struct boot_image_info {
	/*
	image name.
	SPL:0-spl 1-uboot 2-kernel 3-pre_rolling 4-rtt 5-nnmodel
	PDL:0-pre-rolling 1-rtt 2-uboot 3-kernel 4-nnmodel
	*/
	unsigned char name;
	unsigned char type; /* 0:data; 1:image 2 n_image */
	unsigned char boot_scheme; /* 0: normal, 1: hw cascade, 2: cpu sched. */
	unsigned char flag; /*bit0 - header*/
	unsigned int rtt0_boot_addr;
	unsigned int rtt1_boot_addr;
	unsigned int nvm_base;
	unsigned int mem_base;
	unsigned int size;
};

struct boot_image {
	unsigned int magic_number;
	unsigned int cnt;
	unsigned int nvm_class; /* 0: nor, 1: nand, 2: emmc. */
	struct boot_image_info data[5];
};

enum load_mode {
	LOAD_DATA = 0,
	LOAD_IMAGE,
	LOAD_nIMAGE,
};

struct spl_image_info {
	const char *name;
	u8 os;
	ulong load_addr;
	ulong entry_point;
	u32 size;
	u32 flags;
	void *arg;
};

int spl_parse_image_header(struct spl_image_info *spl_image,const struct image_header *header);
void spl_bootm(void);
int spl_nand_load_image(struct boot_image_info * info);
int spl_nand_ctroller_reset(void);

#endif
