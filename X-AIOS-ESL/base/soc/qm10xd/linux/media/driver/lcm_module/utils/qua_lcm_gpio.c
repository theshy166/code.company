#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/platform_device.h>
// #include <linux/clk.h>
// #include <linux/of.h>
// #include <linux/version.h>
// #include <linux/fb.h>
// #include <linux/pinctrl/consumer.h>
// #include <linux/interrupt.h>
// #include <linux/of.h>
// #include <linux/of_gpio.h>
// #include <linux/spi/spi.h>
// #include <linux/i2c.h>
// #include <linux/regmap.h>
#include <linux/gpio.h>
#include <linux/gpio/driver.h>
#include <linux/gpio/consumer.h>

unsigned char qua_lcm_readb(unsigned addr)
{
    return (*((volatile unsigned char *)(addr)));
}

void qua_lcm_writeb(unsigned char v, unsigned addr)
{
    (*((volatile unsigned char *)(addr))) = (v);
}

unsigned int qua_lcm_readl(unsigned addr)
{
    return (*((volatile unsigned int *)(addr)));
}

void qua_lcm_writel(unsigned int v, unsigned addr)
{
    (*((volatile unsigned int *)(addr))) = (v);
}

int qua_lcm_gpio_request(unsigned gpio, const char *label)
{
    return gpio_request(gpio, label);
}

void qua_lcm_gpio_free(unsigned gpio)
{
    gpio_free(gpio);
}

int qua_lcm_gpio_direction_output(unsigned gpio, int value)
{
    return gpio_direction_output(gpio, value);
}

int qua_lcm_gpio_direction_input(unsigned gpio)
{
    return gpio_direction_input(gpio);
}

int qua_lcm_gpio_get_value(unsigned gpio)
{
    return gpio_get_value(gpio);
}

void qua_lcm_gpio_set_value(unsigned gpio, int value)
{
    gpio_set_value(gpio, value);
}
