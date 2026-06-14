#include "reg.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#define GPIO_BASE_ADDR   (0x1AE00000)
#define GPIO_BASE_OFFSET (0x00010000)
#define GPIO(x) (x)

#define GPIO0_0_RST (0)


#define GPIO6_0_CS (0)
#define GPIO6_3_RS (3)

#define GPIO_DATA (0x00)
#define GPIO_MASK (0x04)
#define GPIO_DIR  (0x08)

#define GPIO_NUM_RST (8*8+7)
#define GPIO_NUM_RS (6*8+3)
#define GPIO_NUM_CSN (6*8+0)


uint32_t s_cs_rs_val = 0x00;
/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
int32_t spi_cs_rs_val(uint32_t cs_rs, uint32_t val)
{

    if (0 == val) {
        s_cs_rs_val &= ~(1 << cs_rs);
    } else if (1 == val) {
        s_cs_rs_val |= (1 << cs_rs);
    }

    return s_cs_rs_val;
}

void spi_lcd_pinmux(void){
	//regWrite32(0x28200198, 0x1);//rst gpio8_7
	//regWrite32(0x28200124, 0x0);//rs gpio6_3
	//regWrite32(0x28200118, 0x0);//csn gpio6_0
}

void spi_lcd_gpio_request(int request){
	if(request){
		//gpio_request(GPIO_NUM_RST, 0);//rst gpio8_7
		//gpio_request(GPIO_NUM_RS, 0);//rs gpio6_3
		//gpio_request(GPIO_NUM_CSN, 0);//csn gpio6_0
	}else{
		//gpio_free(GPIO_NUM_RST);//rst gpio8_7
		//gpio_free(GPIO_NUM_RS);//rs gpio6_3
		//gpio_free(GPIO_NUM_CSN);//csn gpio6_0
	}
}

int32_t spi_rest_gpio_init(uint64_t base_addr)
{
    int32_t rtn = SUCCESS;

    //printk("@@@%s %d\n", __func__, __LINE__);
    //gpio_direction_output(GPIO_NUM_RST, 1);
    //gpio_set_value(GPIO_NUM_RST , 1);
    return rtn;
}

int32_t spi_cs_rs_init(uint64_t base_addr)
{
    int32_t rtn = SUCCESS;

    //printk("@@@%s %d\n", __func__, __LINE__);
    //gpio_direction_output(GPIO_NUM_RS, 1);
    //gpio_direction_output(GPIO_NUM_CSN, 1);
    //gpio_set_value(GPIO_NUM_RS , 1);
    //gpio_set_value(GPIO_NUM_CSN , 1);
    return rtn;
}

int32_t spi_lcd_reset(uint64_t base_addr, uint32_t val)
{
    int32_t rtn = SUCCESS;

    //printk("@@@%s %d value:%u\n", __func__, __LINE__, val);
    //gpio_direction_output(GPIO_NUM_RST, val);
    //gpio_set_value(GPIO_NUM_RST , val);

    return rtn;
}

int32_t spi_cs_gpio(uint64_t base_addr, uint32_t val)
{
    int32_t rtn = SUCCESS;

    //printk("@@@%s %d value:%u\n", __func__, __LINE__, val);
    //gpio_direction_output(GPIO_NUM_CSN, val);
    //gpio_set_value(GPIO_NUM_CSN , val);

    return rtn;
}

int32_t spi_rs_gpio(uint64_t base_addr, uint32_t val)
{
    int32_t rtn = SUCCESS;

    //printk("@@@%s %d value:%u\n", __func__, __LINE__, val);
    //gpio_direction_output(GPIO_NUM_RS, val);
    //gpio_set_value(GPIO_NUM_RS , val);

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
