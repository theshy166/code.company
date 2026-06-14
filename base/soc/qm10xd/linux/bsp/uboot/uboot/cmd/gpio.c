/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <div64.h>
#include <dm.h>
#include <malloc.h>
#include <mapmem.h>
#include <spi.h>
#include <spi_flash.h>
#include <jffs2/jffs2.h>
#include <linux/mtd/mtd.h>

#include <asm/io.h>
#include <dm/device-internal.h>
#include <mach/molchip_gpio.h>


static int get_option(char * const argv[],char *opt)
{
	if(strcmp(argv[1], opt) == 0){
		return 1;
	}else if(strcmp(argv[3], opt) == 0){
		return 2;
	}else{
		return -1;
	}
}

static int get_option_value(char * const argv[],int gpio_num_flag)
{
	char *endp;
	if(gpio_num_flag == 1){
		return simple_strtoul(argv[2], &endp, 0);
	}else{
		return simple_strtoul(argv[4], &endp, 0);
	}
}

static int do_gpio_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int gpio_in_num = -1;
	int gpio_out_num = -1;
	int gpio_in_num_flag = -1;
	int gpio_out_num_flag = -1;
	int value;
	int value_0 = 0;
	int value_1 = 1;

	gpio_in_num_flag = get_option(argv,"gpio_in_num");
	gpio_out_num_flag = get_option(argv,"gpio_out_num");

	if(gpio_in_num_flag == -1 || gpio_out_num_flag == -1)
	{
		printf("input option args error\n");
		return 0;
	}

	gpio_in_num = get_option_value(argv,gpio_in_num_flag);
	gpio_out_num = get_option_value(argv,gpio_out_num_flag);
	printf("you input args:gpio_in_num=%d gpio_out_num=%d\n",gpio_in_num,gpio_out_num);

	if(gpio_in_num < 0 || gpio_in_num > 63 || gpio_out_num < 0 || gpio_out_num > 63){
		printf("input option args value error\n");
		return 0;
	}

	gpio_request(gpio_out_num, "gpio_out");
	gpio_request(gpio_in_num, "gpio_in");

	gpio_direction_output(gpio_out_num,value_0);
	gpio_direction_input(gpio_in_num);

	gpio_set_value(gpio_out_num,value_0);
	value = gpio_get_value(gpio_in_num);
	if(value_0 == value){
		printf("success:gpio%d_setvalue=%d eq gpio%d_getvalue=%d\n",gpio_out_num,value_0,gpio_in_num,value);
	}else{
		printf("error:gpio%d_setvalue=%d neq gpio%d_getvalue=%d\n",gpio_out_num,value_0,gpio_in_num,value);
	}

	mdelay(1000);

	gpio_set_value(gpio_out_num,value_1);
	value = gpio_get_value(gpio_in_num);
	if(value_1 == value){
		printf("success:gpio%d_setvalue=%d eq gpio%d_getvalue=%d\n",gpio_out_num,value_1,gpio_in_num,value);
	}else{
		printf("error:gpio%d_setvalue=%d neq gpio%d_getvalue=%d\n",gpio_out_num,value_1,gpio_in_num,value);
	}

	return 0;
}

U_BOOT_CMD(
	gpio,	5,	1,	do_gpio_test,
	"uboot gpio test",
	"gpio_test gpio_in_num n1 gpio_out_num n2\n"
	"n1:\n"
	"	[0:63]\n"
	"n2:\n"
	"	[0:63]\n"
);
