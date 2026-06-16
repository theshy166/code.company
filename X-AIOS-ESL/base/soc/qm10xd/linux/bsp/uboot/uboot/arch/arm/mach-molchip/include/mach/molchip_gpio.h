#ifndef _MC_GPIO_H_
#define _MC_GPIO_H_


#define REG_BASE_GPIO0							0xb000000
#define REG_BASE_GPIO1							0xb010000
#define REG_BASE_GPIO2							0xb020000
#define REG_BASE_GPIO3							0xb030000
#define REG_BASE_GPIO4							0xb040000
#define REG_BASE_GPIO5							0xb050000
#define REG_BASE_GPIO6							0xb060000
#define REG_BASE_GPIO7							0xb070000

#define GPIO_DATA	0x00	/*gpio data 1:high 0:low*/
#define GPIO_DMSK	0x04	/*gpio masked*/
#define GPIO_DIR	0x08	/*gpio direction 1:input 0:output*/
#define GPIO_IS 	0x0C	/*gpio bits interrupt sense*/
#define GPIO_IBE	0x10	/*gpio bits interrupt both edges 1:both edge 0:single edge*/
#define GPIO_IEV	0x14	/*gpio bits interrupt event*/
#define GPIO_IE  	0x18	/*gpio bits interrupt enable*/
#define GPIO_RIS	0x1C	/*gpio bits raws interrupt status*/
#define GPIO_MIS	0x20	/*gpio bits masked interrupt status*/
#define GPIO_IC 	0x24	/*gpio bits interrupt clear*/
#define GPIO_INEN	0x28	/*gpio input enable*/

#define GPIO_GROUP_NUM 8
#define GPIO_EVERY_GROUP_PIN_NUM 8
#define GPIO_MAX_NUM 64


#define gpio_readl(reg)				__raw_readl(reg)
#define gpio_writel(value, reg)		__raw_writel((value), reg)

#define GPIO_CLK_EN 0xC0000B0 

typedef enum
{
	gpio_dir_input,
	gpio_dir_output,
}gpio_direction;

typedef enum
{
	gpio_mode_low,
	gpio_mode_high,
}gpio_output_mode;

/*
 *pin = group_num*8 + pin_num
 *mode:gpio_mode_low -low level 	gpio_mode_high -high level
 *return value: 0 - true  	-1 - false
*/
int gpio_direction_output(u8 pin,gpio_output_mode mode);

/*
 *pin = group_num*8 + pin_num
 *return value: 0 - true  	-1 - false
*/
int gpio_direction_input(u8 pin);

/*
 *pin = group_num*8 + pin_num
 *return value: 0 - true    	-1 - false
*/
int gpio_get_value(u8 pin);


/*
 *pin = group_num*8 + pin_num
 *return value: 0 - true    	-1 - false
*/
int gpio_set_value(u8 pin,gpio_output_mode mode);


/*
 *gpio clock init and Determine the number of GPIO
*/
int gpio_request(unsigned gpio, const char *label);



#endif

