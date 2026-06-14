/*
#
# Copyright (c) Quaming Intelligent Technology Co., Ltd.
# (C) Copyright 2002-2006 All Rights Reserved.
# 
# SPDX-License-Identifier:	GPL-2.0+
#
*/

#include <common.h>
#include <asm/io.h>
#include "resource_img.h"

#define ALIGN_UP(x, a)              ((x+a-1)&(~(a-1)))

#define LCM_W		1024
#define LCM_H		600
#define LOGO_W		320
#define LOGO_H		160
#define JPG_ADDR    0x43B00000
#define Y_ADDR		0x43C00000	//(LOGO_YADDR0) //(0x43D00000) //MMZ END ADDR 0x44000000
#define UV_ADDR		0x43D00000	//(LOGO_CADDR0) //(0x43D00000 + (ALIGN_UP(JPG_W, 16) * ALIGN_UP(JPG_H, 16))) //JPEG DEC MUST ALIGN 16

extern unsigned int JPEG_WaitIRQ(void);
extern void fast_osd(int type, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph);
extern void hw_jpeg_dec(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr);

void qua_boot_logo_ek79001h(void) {
	unsigned int jpg_addr = 0;
	unsigned int logo_width = 0;
	unsigned int logo_height = 0;
	int jpg_size = 0;

	extern const __attribute__((aligned(64))) unsigned int logo_jpg_welcome_w320h160_map[];
	jpg_size = 11752;	//_binary_logo_jpg_end - _binary_logo_jpg_start;
	jpg_addr = (unsigned int)&logo_jpg_welcome_w320h160_map;
	logo_width = LOGO_W;
	logo_height = LOGO_H;
	hw_jpeg_dec(jpg_addr, jpg_size, Y_ADDR, UV_ADDR);
    JPEG_WaitIRQ();
    fast_osd(1, (LCM_W - logo_width) / 2, (LCM_H - logo_height) / 2, logo_width, logo_height, Y_ADDR, UV_ADDR, logo_width, logo_width, logo_height);
}

extern void qua_writel(uint32_t value, uint32_t addr);
extern void qua_gpio_direction_output(unsigned int gpio_num);
extern void qua_gpio_set_value(unsigned int gpio_num, unsigned int val);

void qua_gpio_set_lcm_ek79001h(void) {

	#define REG_TOP_PIN_BASE    0x102000
	#define REG_GPIO7_1_PIN     (0xE4)
	#define REG_GPIO0_5_PIN     (0xEC)

	#define MC_GPIO7_1	(7*8+1)
	#define MC_GPIO0_5	(0*8+5)

	qua_writel(1, REG_TOP_PIN_BASE + REG_GPIO7_1_PIN);
	qua_gpio_direction_output(MC_GPIO7_1);
	qua_gpio_set_value(MC_GPIO7_1, 1);

	qua_writel(1, REG_TOP_PIN_BASE + REG_GPIO0_5_PIN);
	qua_gpio_direction_output(MC_GPIO0_5);
	qua_gpio_set_value(MC_GPIO0_5, 1);
}
