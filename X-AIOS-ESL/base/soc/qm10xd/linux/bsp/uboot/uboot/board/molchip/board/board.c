/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/spl.h>
#include <mach/systimer.h>
#include "system.h"

#if defined(CONFIG_TARGET_FY02) || defined(CONFIG_TARGET_FY02B) || defined(CONFIG_TARGET_FY10P) || defined(CONFIG_TARGET_FY02P)
void (*boot)(uint address)=(void (*)(uint))(0x265);
#else
#ifdef CONFIG_TARGET_FY12
void (*boot)(uint address)=(void (*)(uint))(0x100250);
#define FY12A_CHIP_ID0 0x2800009C
#define FY12A_CHIP_ID1 0x280000A0
#define FY12A_CHIP_ID0_DATA 0xA0
#define FY12A_CHIP_ID1_DATA 0x46593132
#else
void (*boot)(uint address)=(void (*)(uint))(0x279);
#endif
#endif
#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_SDL_BUILD)
u8 nvm_boot_device(void)
{
	unsigned int val;
	val = get_strap_status();
	val >>= 1;
	if (val == 0) return BOOT_DEVICE_NOR;
	else if (val == 1) return BOOT_DEVICE_NAND;
	else if (val == 2) return BOOT_DEVICE_USB;
	else if (val == 3) return BOOT_DEVICE_SDIO1;
	else return BOOT_DEVICE_NONE;
}
#endif

#ifndef CONFIG_ARM64
void hang(void)
{
	while(1);
}
#endif

void jump_to(uint addr)
{
//    asm("bx %0"::"r"(addr));
    (*boot)(addr);
}

/* Must be called before ddr init */
void ana_bias_reset(void)
{
	unsigned long time_d;

	/* Bias config reset */
#ifdef CONFIG_TARGET_FY12
	if((readl(FY12A_CHIP_ID0) == FY12A_CHIP_ID0_DATA) && (readl(FY12A_CHIP_ID1) == FY12A_CHIP_ID1_DATA)){

		time_d = get_sys_ticks();
		while((get_sys_ticks() - time_d) < 100);
		time_d = get_sys_ticks();
		while((get_sys_ticks() - time_d) < 10);
	}else{
#endif
	//reg = REG_ANALOG_BASE + 0x48;
	//writel(readl(reg) | (1 << 16), reg);
	time_d = get_sys_ticks();
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	while((get_sys_ticks() - time_d) < 100);
	#else
	while((get_sys_ticks() - time_d) < 1);
	#endif
	//writel(readl(reg) & (~(1 << 16)), reg);
#if defined(CONFIG_TARGET_FY02) || defined(CONFIG_TARGET_FY02B) || defined(CONFIG_TARGET_FY10P) || defined(CONFIG_TARGET_FY02P)
	/* Reset Ephy CLK */
	reg = REG_ANALOG_BASE + 0x90;
	writel(readl(reg) & (~(1 << 11)), reg);
	time_d = get_sys_ticks();
	while((get_sys_ticks() - time_d) < 100);
	writel(readl(reg) | (1 << 11), reg);
	time_d = get_sys_ticks();
#else
	time_d = get_sys_ticks();
#endif
	while((get_sys_ticks() - time_d) < 10);
#ifdef CONFIG_TARGET_FY12
}
#endif
}

void nosec_jump_to(uint addr)
{
	asm("bx %0"::"r"(addr));
}

void nosec_boot(uint address)
{
	struct boot_header *header;
	header = (struct boot_header *)address;
	nosec_jump_to(header->addr);
	while(1);
}


