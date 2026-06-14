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
#include "udisk_flash.h"
#include "mach/molchip_gpio.h"

#define ALIGN_UP(x, a)              ((x+a-1)&(~(a-1)))

#define LCM_W		800
#define LCM_H		1280
#define LOGO_W		800 //dhd0 only support 1024x600
#define LOGO_H		1280
#define JPG_ADDR    0x43A00000
#define Y_ADDR_1    0x43B00000
#define UV_ADDR_1   0x43C00000
#define Y_ADDR_2	0x43D00000	//(LOGO_YADDR0) //(0x43D00000) //MMZ END ADDR 0x44000000
#define UV_ADDR_2	0x43E00000	//(LOGO_CADDR0) //(0x43D00000 + (ALIGN_UP(JPG_W, 16) * ALIGN_UP(JPG_H, 16))) //JPEG DEC MUST ALIGN 16
#define LOGO_NAME_1 "logo_0.jpg"
#define LOGO_NAME_2 "logo_1.jpg"
#define LOGO_UPDATE "logo_update.jpg"
#define LOGO_DEFAULT "default.jpg"
#define LOGO_LANDS_1 "logo_lands_0.jpg"
#define LOGO_LANDS_2 "logo_lands_1.jpg"

typedef enum {
	VOU_DEV_IDX_DHD = 0,
	VOU_DEV_IDX_DHD1,
	VOU_DEV_IDX_DSD,
	VOU_DEV_IDX_NUM,
} vou_dev_idx_e;

extern unsigned int JPEG_WaitIRQ(void);
extern unsigned int JPEG_WaitIRQ_Timeout(void);
extern void fast_osd(int type, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph);
extern void fast_osd_by_id(int id, int type, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph);
extern void hw_jpeg_dec(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr);

static int show_jpeg_from_part(int id, const char *name, unsigned int y_addr, unsigned int uv_addr)
{
	int jpg_size = 0;
	unsigned int logo_width = LOGO_W;
	unsigned int logo_height = LOGO_H;

	jpg_size = mc_read_resource_file((void*)JPG_ADDR, name, 0, 0);
	if (jpg_size > 0) {
		hw_jpeg_dec(JPG_ADDR, jpg_size, y_addr, uv_addr);
		if (!JPEG_WaitIRQ_Timeout()) {
			fast_osd_by_id(id, 1, (LCM_W - logo_width) / 2, (LCM_H - logo_height) / 2, logo_width, logo_height, y_addr, uv_addr, logo_width, logo_width, logo_height);
			printf("show %s done !\n", name);
			return 0;
		}
	}

	printf("show %s failed !\n", name);
	return 1;
}

static int show_jpeg_welcome(int id, unsigned int y_addr, unsigned int uv_addr)
{
	extern const __attribute__((aligned(64))) unsigned int logo_jpg_welcome_w320h160_map[];
	int jpg_size = 11752;	//_binary_logo_jpg_end - _binary_logo_jpg_start;
	uint32_t jpg_addr = (unsigned int)&logo_jpg_welcome_w320h160_map;
	unsigned int logo_width = 320;
	unsigned int logo_height = 160;

	hw_jpeg_dec(jpg_addr, jpg_size, y_addr, uv_addr);
    if (!JPEG_WaitIRQ_Timeout()) {
		fast_osd_by_id(id, 1, (LCM_W - logo_width) / 2, (LCM_H - logo_height) / 2, logo_width, logo_height, y_addr, uv_addr, logo_width, logo_width, logo_height);
		printf("show welcome done !\n");
		return 0;
	}

	printf("show welcome failed !\n");
	return 1;
}

void display_logo_id(int id, const char *logo_name) {
	char name[20] = {0};
	uint32_t y_addr = 0;
	uint32_t uv_addr = 0;

	if (id == VOU_DEV_IDX_DHD) {
		y_addr = Y_ADDR_1;
		uv_addr = UV_ADDR_1;
		strcpy(name, LOGO_NAME_1);
	} else if (id == VOU_DEV_IDX_DHD1) {
		y_addr = Y_ADDR_2;
		uv_addr = UV_ADDR_2;
		strcpy(name, LOGO_NAME_2);
	}
#ifdef CONFIG_UBOOT_XKPHOTO
	isFlashMode();

	// landscape mode
	if (env_get_ulong("landscape", 10, 0) == 1) {
		strcpy(name, LOGO_LANDS_1);
	}
#else
	if (!isFlashMode()) {
		strcpy(name, LOGO_UPDATE);
	}
#endif

	if (logo_name) {
		strcpy(name, logo_name);
	}

#ifdef CONFIG_QUA_LOGO_PART
	// display jpeg from logo partition
	if (show_jpeg_from_part(id, name, y_addr, uv_addr) == 0) {
		return;
	}
	// if logo_0.jpg or logo_1.jpg data is corrupted, display default.jpg
	if (show_jpeg_from_part(id, LOGO_DEFAULT, y_addr, uv_addr) == 0) {
		return;
	}
#endif
	show_jpeg_welcome(id, y_addr, uv_addr);
}

void display_logo_id_addr(int id, const char * jpg_addr, const char * jpg_size) {
	uint32_t y_addr = 0;
	uint32_t uv_addr = 0;

	if (id == VOU_DEV_IDX_DHD) {
		y_addr = Y_ADDR_1;
		uv_addr = UV_ADDR_1;
	} else if (id == VOU_DEV_IDX_DHD1) {
		y_addr = Y_ADDR_2;
		uv_addr = UV_ADDR_2;
	}

	hw_jpeg_dec(simple_strtoul(jpg_addr, NULL, 16), simple_strtoul(jpg_size, NULL, 16), y_addr, uv_addr);
    if (!JPEG_WaitIRQ_Timeout()) {
		fast_osd_by_id(id, 1, (LCM_W - LOGO_W) / 2, (LCM_H - LOGO_H) / 2, LOGO_W, LOGO_H, y_addr, uv_addr, LOGO_W, LOGO_W, LOGO_H);
		printf("show jpg logo done !\n");
		return;
	}

	printf("show jpg logo failed !\n");
}

void qua_boot_logo_jd9365d(void) {
	display_logo_id(VOU_DEV_IDX_DHD, NULL);
	#if defined(CONFIG_QUA_LCM_2ND_SCR)
	display_logo_id(VOU_DEV_IDX_DHD1, NULL);
	#endif
}

int gpio_direction_output(unsigned char pin, gpio_output_mode mode);
int gpio_direction_input(unsigned char pin);
int gpio_get_value(unsigned char pin);
int gpio_set_value(unsigned char pin, gpio_output_mode mode);
int gpio_request(unsigned gpio, const char *label);

extern void qua_writel(uint32_t value, uint32_t addr);
extern uint32_t qua_readl(uint32_t addr);

#ifdef CONFIG_UBOOT_XKPHOTO
#define REG_PIN_MUX_BASE        0x10200000
#define REG_LCD_DE_PIN                0x64
#define REG_LCD_HS_PIN                0x68
#define REG_LCD_VS_PIN                0x6C
#define REG_LCD_D0_PIN                0x74
#define REG_LCD_D1_PIN                0x78
#define REG_LCD_BL_PIN                0xb0

#define REG_PIN_DRV_BASE        0x0B100000
#define REG_LCD_DE_DRV                0x28
#define REG_LCD_HS_DRV                0x2C
#define REG_LCD_VS_DRV                0x30
#define REG_LCD_D0_DRV                0x38
#define REG_LCD_D1_DRV                0x3C

#define GPIO_NUM_INDEX(x,y)        (8*x + y)
#define GPIO_NUM_LCD_DE                GPIO_NUM_INDEX(3,3)
#define GPIO_NUM_LCD_HS                GPIO_NUM_INDEX(3,4)
#define GPIO_NUM_LCD_VS                GPIO_NUM_INDEX(3,5)
#define GPIO_NUM_LCD_D0                GPIO_NUM_INDEX(3,7)
#define GPIO_NUM_LCD_D1                GPIO_NUM_INDEX(4,0)
#define GPIO_NUM_LCD_BL                GPIO_NUM_INDEX(5,6)
#else
#define REG_PIN_MUX_BASE        0x10200000
#define REG_LCD_DE_PIN                0x64
#define REG_LCD_HS_PIN                0x68
#define REG_LCD_VS_PIN                0x6C
#define REG_LCD_D0_PIN                0x74
#define REG_LCD_D1_PIN                0x78
#define REG_LCD_BL_PIN                0xec

#define REG_PIN_DRV_BASE        0x0B100000
#define REG_LCD_DE_DRV                0x28
#define REG_LCD_HS_DRV                0x2C
#define REG_LCD_VS_DRV                0x30
#define REG_LCD_D0_DRV                0x38
#define REG_LCD_D1_DRV                0x3C

#define GPIO_NUM_INDEX(x,y)        (8*x + y)
#define GPIO_NUM_LCD_DE                GPIO_NUM_INDEX(3,3)
#define GPIO_NUM_LCD_HS                GPIO_NUM_INDEX(3,4)
#define GPIO_NUM_LCD_VS                GPIO_NUM_INDEX(3,5)
#define GPIO_NUM_LCD_D0                GPIO_NUM_INDEX(3,7)
#define GPIO_NUM_LCD_D1                GPIO_NUM_INDEX(4,0)
#define GPIO_NUM_LCD_BL                GPIO_NUM_INDEX(0,5)
#endif

#define PIN_MUX_GPIO_ENABLE		(1)
static void __qua_lcm_mipi_jd9365d_hrst(unsigned int vci_pin_reg_oft, unsigned int rst_pin_reg_oft,
	unsigned int vci_pin_gpio_num, unsigned int rst_pin_gpio_num)
{
	unsigned int pmux_reg = (unsigned int)REG_PIN_MUX_BASE;
	printf("%s e\n", __func__);
	/* power on rst timing gpio */
	qua_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + rst_pin_reg_oft);
	gpio_request(rst_pin_gpio_num, "rst_gpio");
	gpio_direction_output(rst_pin_gpio_num, 0);
	gpio_set_value(rst_pin_gpio_num, 0);

	/* power on vci timing gpio, 0: on, 1: off */
	qua_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + vci_pin_reg_oft);
	gpio_request(vci_pin_gpio_num, "vci_gpio");
	gpio_direction_output(vci_pin_gpio_num, 1);
	gpio_set_value(vci_pin_gpio_num, 1);
	mdelay(500);
	gpio_set_value(vci_pin_gpio_num, 0);
	mdelay(10);

	/* power on rst timing */
	gpio_set_value(rst_pin_gpio_num, 1);
	mdelay(10);
	gpio_set_value(rst_pin_gpio_num, 0);
	mdelay(20);
	gpio_set_value(rst_pin_gpio_num, 1);
	mdelay(10);

	/* power backlight gpio enable, 0x102000ec */
	qua_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + REG_LCD_BL_PIN);
	gpio_request(GPIO_NUM_LCD_BL, "bkl_gpio");
	gpio_direction_output(GPIO_NUM_LCD_BL, 1);
	gpio_set_value(GPIO_NUM_LCD_BL, 1);

	// gpio_free(rst_pin_gpio_num);
	// gpio_free(vci_pin_gpio_num);
	printf("%s x\n", __func__);
}

int qua_lcm_mipi_jd9365d_hrst(int lcm_id)
{
	printf("%s e\n", __func__);
	printf("%s lcm_id=%d\n", __func__, lcm_id);
	if(lcm_id == 0)
		__qua_lcm_mipi_jd9365d_hrst(REG_LCD_D0_PIN, REG_LCD_DE_PIN, GPIO_NUM_LCD_D0, GPIO_NUM_LCD_DE);

	if(lcm_id == 1)
		__qua_lcm_mipi_jd9365d_hrst(REG_LCD_D1_PIN, REG_LCD_HS_PIN, GPIO_NUM_LCD_D1, GPIO_NUM_LCD_HS);
	printf("%s x\n", __func__);
	return 0;
}

static void __qua_lcm_mipi_jd9365d_rst(unsigned int rst_pin_reg_oft, unsigned int rst_pin_gpio_num)
{
	unsigned int pmux_reg = (unsigned int)REG_PIN_MUX_BASE;
	printf("%s e\n", __func__);
	/* power on rst timing gpio */
	qua_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + rst_pin_reg_oft);
	gpio_request(rst_pin_gpio_num, "rst_gpio");
	gpio_direction_output(rst_pin_gpio_num, 1);
	gpio_set_value(rst_pin_gpio_num, 0);
	mdelay(10);
	/* power on rst timing */
	gpio_set_value(rst_pin_gpio_num, 1);
	mdelay(10);
	gpio_set_value(rst_pin_gpio_num, 0);
	mdelay(20);
	gpio_set_value(rst_pin_gpio_num, 1);
	mdelay(10);

	// gpio_free(rst_pin_gpio_num);
	// gpio_free(vci_pin_gpio_num);
	printf("%s x\n", __func__);
}

int qua_lcm_mipi_jd9365d_rst(int lcm_id)
{
	printf("%s e\n", __func__);
	printf("%s lcm_id=%d\n", __func__, lcm_id);
	if(lcm_id == 0)
		__qua_lcm_mipi_jd9365d_rst(REG_LCD_DE_PIN, GPIO_NUM_LCD_DE);

	if(lcm_id == 1)
		__qua_lcm_mipi_jd9365d_rst(REG_LCD_HS_PIN, GPIO_NUM_LCD_HS);
	printf("%s x\n", __func__);
	return 0;
}

int qua_lcm_mipi_jd9365d_bkl(void)
{
	unsigned int pmux_reg = (unsigned int)REG_PIN_MUX_BASE;
	// printf("%s e\n", __func__);
	/* power backlight gpio enable, 0x102000b0 */
	qua_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + REG_LCD_BL_PIN);
	gpio_request(GPIO_NUM_LCD_BL, "bkl_gpio");
	gpio_direction_output(GPIO_NUM_LCD_BL, 1);
	gpio_set_value(GPIO_NUM_LCD_BL, 1);

	// printf("%s x\n", __func__);
	return 0;
}
#if 0
int qua_lcm_mipi_jd9365d_hrst(int lcm_id)
{
	// int rtn = 0, val = 0, tmp = 0, _v = 0, i = 0, count = 3;
	// volatile unsigned int *pdrv_reg = (volatile unsigned int *)_vpdrvbase;
	volatile unsigned int *pmux_reg = (volatile unsigned int *)REG_PIN_MUX_BASE;

	if(lcm_id == 0)
	{
		/* power on vci timing gpio3_7 */
		qua_writel(1, (unsigned int)pmux_reg + REG_LCD_D0_PIN);
		qua_gpio_request(GPIO_NUM_LCD_D0, 0);
		qua_gpio_direction_output(GPIO_NUM_LCD_D0, 1);
		qua_gpio_set_value(GPIO_NUM_LCD_D0, 1);
		qua_gpio_set_value(GPIO_NUM_LCD_D0, 0);
		mdelay(5);

		/* power on rst timing gpio3_3 */
		qua_writel(1, (unsigned int)pmux_reg + REG_LCD_DE_PIN);
		// tmp = qua_readl((unsigned int)pdrv_reg + REG_LCD_DE_DRV);
		// printf("%s LCD_DE drv = 0x%x\n", __func__, tmp);
		// _v = ((tmp & 0xf0) | 0x04);
		// qua_writel(_v, (unsigned int)pdrv_reg + REG_LCD_DE_DRV);

		qua_gpio_request(GPIO_NUM_LCD_DE, 0);
		qua_gpio_direction_output(GPIO_NUM_LCD_DE, 1);
		qua_gpio_set_value(GPIO_NUM_LCD_DE, 1);
		qua_gpio_set_value(GPIO_NUM_LCD_DE, 0);
		mdelay(5);
		qua_gpio_set_value(GPIO_NUM_LCD_DE, 1);
		mdelay(5);
		printf("%s gpio3_3 \n", __func__);

		// printf("%s gpio3_3 val = 0x%x\n", __func__, val);
		// qua_writel(tmp, (unsigned int)pdrv_reg + REG_LCD_DE_DRV);
		qua_gpio_free(GPIO_NUM_LCD_DE);
		qua_gpio_free(GPIO_NUM_LCD_D0);
	}

	if(lcm_id == 1)
	{
		/* power on vci timing gpio4_0 */
		qua_writel(1, (unsigned int)pmux_reg + REG_LCD_D1_PIN);
		qua_gpio_request(GPIO_NUM_LCD_D1, 0);
		qua_gpio_direction_output(GPIO_NUM_LCD_D1, 1);
		qua_gpio_set_value(GPIO_NUM_LCD_D1, 1);
		qua_gpio_set_value(GPIO_NUM_LCD_D1, 0);
		mdelay(5);

		/* power on rst timing gpio3_4 */
		qua_writel(1, (unsigned int)pmux_reg + REG_LCD_HS_PIN);
		// tmp = qua_readl((unsigned int)pdrv_reg + REG_LCD_VS_DRV);
		// printf("%s LCD_VS drv = 0x%x\n", __func__, tmp);
		// _v = ((tmp & 0xf0) | 0x04);
		// qua_writel(_v, (unsigned int)pdrv_reg + REG_LCD_VS_DRV);

		qua_gpio_request(GPIO_NUM_LCD_HS, 0);
		qua_gpio_direction_output(GPIO_NUM_LCD_HS, 1);

		qua_gpio_set_value(GPIO_NUM_LCD_HS, 1);
		// do{
			qua_gpio_set_value(GPIO_NUM_LCD_HS, 0);
			mdelay(5);
			qua_gpio_set_value(GPIO_NUM_LCD_HS, 1);
			mdelay(5);
			printf("%s gpio3_4 \n", __func__);
			// while(1);

		// }while(1);
		// printf("%s gpio3_4 val = 0x%x\n", __func__, val);

		// qua_writel(tmp, (unsigned int)pdrv_reg + REG_LCD_VS_DRV);
		qua_gpio_free(GPIO_NUM_LCD_HS);
		qua_gpio_free(GPIO_NUM_LCD_D1);
	}

	return 0;
}
#endif

static int show_qua_boot_logo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2 && argc != 3)
		goto usage;

	// 传入logo分区中logo图片的名称
	if (argc == 2) {
		display_logo_id(VOU_DEV_IDX_DHD, argv[1]);
		#if defined(CONFIG_QUA_LCM_2ND_SCR)
		display_logo_id(VOU_DEV_IDX_DHD1, argv[1]);
		#endif
	}

	// logo图片已读取到ddr中，传入ddr的地址和logo图片大小
	if (argc == 3) {
		display_logo_id_addr(VOU_DEV_IDX_DHD, argv[1], argv[2]);
		#if defined(CONFIG_QUA_LCM_2ND_SCR)
		display_logo_id_addr(VOU_DEV_IDX_DHD1, argv[1], argv[2]);
		#endif
	}

	return 0;

usage:
	printf("Usage: qua_logo %s\n", cmdtp->usage);
	return 1;
}

U_BOOT_CMD(
        qua_logo, 3, 0, show_qua_boot_logo,
        "show qua boot logo",
        "show qua boot logo"
);
