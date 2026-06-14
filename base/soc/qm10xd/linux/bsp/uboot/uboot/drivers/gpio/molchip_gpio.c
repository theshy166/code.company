/*
 * Copyright (C) 2012 Stefan Roese <sr@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Driver for SPEAr600 GPIO controller
 */


#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <mach/spic.h>
#include <mach/molchip_gpio.h>

static u32 gpio_base_tab[GPIO_GROUP_NUM] = {
	REG_BASE_GPIO0,
	REG_BASE_GPIO1,
	REG_BASE_GPIO2,
	REG_BASE_GPIO3,
	REG_BASE_GPIO4,
	REG_BASE_GPIO5,
	REG_BASE_GPIO6,
	REG_BASE_GPIO7,
};

static int gpio_set_mask(u8 pin)
{
	u32 gpio_base,gpio_addr,val;
	u8 offset;
	gpio_base = gpio_base_tab[pin / GPIO_EVERY_GROUP_PIN_NUM];
	
	offset = pin % GPIO_EVERY_GROUP_PIN_NUM;
	gpio_addr = gpio_base + GPIO_DMSK;
	val = gpio_readl(gpio_addr);
	val |= BIT(offset);
	gpio_writel(val,gpio_addr);
	return 1;
}

static int gpio_set_direction(u8 pin,gpio_direction direction)
{
	u32 gpio_base,gpio_addr,val;
	u8 offset;
	
	gpio_base = gpio_base_tab[pin / GPIO_EVERY_GROUP_PIN_NUM];

	gpio_set_mask(pin);
	
	gpio_addr = gpio_base + GPIO_DIR;
	offset = pin % GPIO_EVERY_GROUP_PIN_NUM;
	val = gpio_readl(gpio_addr);
	if(direction == gpio_dir_output) {
		val |= BIT(offset);
		gpio_writel(val,gpio_addr);
	}else if(direction == gpio_dir_input){
		val &= ~BIT(offset);
		gpio_writel(val,gpio_addr);
	}else{
		return 0;
	}

	return 1;
}

static int gpio_input_enable(u8 pin)
{
	u32 gpio_base,gpio_addr,val;
	u8 offset;
	
	gpio_base = gpio_base_tab[pin / GPIO_EVERY_GROUP_PIN_NUM];

	offset = pin % GPIO_EVERY_GROUP_PIN_NUM;
	gpio_addr = gpio_base + GPIO_INEN;
	
	val = gpio_readl(gpio_addr);
	val |= BIT(offset);
	gpio_writel(val,gpio_addr);

	return 1;
}

int gpio_set_value(u8 pin,gpio_output_mode mode)
{
	u32 gpio_base,gpio_addr,val;
	u8 offset;
	
	gpio_base = gpio_base_tab[pin / GPIO_EVERY_GROUP_PIN_NUM];
	
	gpio_addr = gpio_base + GPIO_DATA;
	offset = pin % GPIO_EVERY_GROUP_PIN_NUM;
	val = gpio_readl(gpio_addr);
	if(mode == gpio_mode_high) {
		val |= BIT(offset);
		gpio_writel(val,gpio_addr);
	}else if(mode == gpio_mode_low){
		val &= ~BIT(offset);
		gpio_writel(val,gpio_addr);
	}else{
		return -1;
	}

	return 0;
}

int gpio_direction_output(u8 pin,gpio_output_mode mode)
{
	u8 ret;
	ret = gpio_set_direction(pin,gpio_dir_output);
	if(ret == 0){
		return -1;
	}
	ret = gpio_set_value(pin,mode);
	return 0;
}

int gpio_direction_input(u8 pin)
{
	u8 ret;
	ret= gpio_set_direction(pin,gpio_dir_input);
	if(ret == 0){
		return -1;
	}
	ret = gpio_input_enable(pin);
	return 0;
}

int gpio_get_value(u8 pin)
{
	u32 gpio_base,gpio_addr,val;
	u8 offset;
	
	gpio_base = gpio_base_tab[pin / GPIO_EVERY_GROUP_PIN_NUM];

	gpio_addr = gpio_base + GPIO_DATA;
	val = gpio_readl(gpio_addr);
	offset = pin % GPIO_EVERY_GROUP_PIN_NUM;
	val &= BIT(offset);

	return  (!!val);
}

int gpio_request(unsigned gpio, const char *label)
{
	u32 ret;
	if(gpio >= GPIO_MAX_NUM){
		printf("gpio need less than 64!");
		return -1;
	}
	ret = gpio_readl(GPIO_CLK_EN);
	ret |= BIT(11);
	gpio_writel(ret,GPIO_CLK_EN);

	return 0;
}




