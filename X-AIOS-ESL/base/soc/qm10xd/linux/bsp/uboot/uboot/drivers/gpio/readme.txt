uboot下gpio驱动使用流程
1.dts中添加要使用的gpio信息，以在sdio中使用gpio为例：
	sdmmc0: dwmmc@1d400000 {
		compatible = "mc,dw-sdmmc";
		reg = <0x1d400000 0x400>;
		cd-gpios = <&gpio1 4 GPIO_ACTIVE_HIGH>;
              		status = "okay";
               		bus-width = <4>;
	};

2.测试代码如下：
#include <asm-generic/gpio.h>

struct gpio_desc _mytest_desc;
//struct gpio_desc _mytest_desc1;

static int mc_gpio_test(struct udevice *dev)
{
	int ret;
	
	printf("---------mc_gpio_test_bind \n");

	ret = gpio_request_by_name(dev, "cd-gpios", 0, &_mytest_desc, GPIOD_IS_OUT);

	if (ret)
		printf("===========================Get gpio_request_by_name fail %d \n",ret);

	printf("gpio_request_by_name Done %d \n",ret);

    if (dm_gpio_is_valid(&_mytest_desc))// 判断对应gpio_desc是否可用
    {   
        printf("===========================Get led1-gpio\n");
        while(1) 
        {   
            dm_gpio_set_value(&_mytest_desc, 0);  // 调用dm_gpio_set_value将led1_gpio设置输出低电平
            mdelay(1000);
            dm_gpio_set_value(&_mytest_desc, 1);  // 调用dm_gpio_set_value将led1_gpio设置输出高电平
            mdelay(1000);
        }
    }
    else
    {
        printf("===========================Can't get led1-gpio\n");
    }
	
	printf("mc_gpio_test_bind end\n");

	return 0;
}