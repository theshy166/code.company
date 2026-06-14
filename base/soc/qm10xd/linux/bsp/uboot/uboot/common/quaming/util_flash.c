/*
 * (C) Copyright 2024 Quaming
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm-generic/gpio.h>
#include "util_flash.h"


struct gpio_config {
	uint32_t ngpio;
	bool     active_low;
} Config = {
	.ngpio = 42, // GPIO5_2
	.active_low = 0,
};

static int flashing_fw_requested_by_gpio(void)
{
	int val;
       gpio_request(Config.ngpio, NULL);
	val = gpio_direction_input(Config.ngpio);
	if (val)
		return 0;

	val =gpio_get_value(Config.ngpio);

	return val < 0 ? 0 : (val == Config.active_low);
}

/*
 * Returns True if a firmware update is requested
 */
int flashing_fw_requested(void)
{
	return flashing_fw_requested_by_gpio();
}


// GPIO4_2 配置结构
static struct gpio_config BoardIDConfig = {
    .ngpio = 34,        // GPIO4_2
    .active_low = 0,
};

#define GPIO_CFG_REG 0x0b100048 // GPIO4_2 的寄存器地址
#define GPIO4_2_PULL_DOWN 0x00000024 // GPIO4_2 的下拉配置值50K
#define GPIO4_2_PULL_UP 0x00000023 // GPIO4_2 的上拉配置值20K
static inline void gpio_cfg_writel(u32 val, int reg)
{
	writel(val, reg);
}
// 硬件版本检测函数
const char *get_board_id(void)
{
    int val;
	gpio_cfg_writel(GPIO4_2_PULL_DOWN, GPIO_CFG_REG); // 设置GPIO4_2的下拉配置
    // 申请GPIO资源
    if (gpio_request(BoardIDConfig.ngpio, "board_id")) {
        printf("Error requesting GPIO%d\n", BoardIDConfig.ngpio);
        return "UNKNOWN";
    }

    // 设置为输入模式
    if (gpio_direction_input(BoardIDConfig.ngpio)) {
        printf("Error setting GPIO%d direction\n", BoardIDConfig.ngpio);
        //gpio_free(BoardIDConfig.ngpio);
        return "UNKNOWN";
    }

    // 读取电平值
    val = gpio_get_value(BoardIDConfig.ngpio);
    //gpio_free(BoardIDConfig.ngpio); // 释放GPIO资源
    printf("GPIO%d value: %d\n", BoardIDConfig.ngpio, val);
    // 根据电平判断硬件版本
    return (val == BoardIDConfig.active_low) ? "ESL10D" : "ESL20D";
}