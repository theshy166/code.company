#ifndef _PIN_LCD_V01_DRV_H_
#define _PIN_LCD_V01_DRV_H_

#include "../../lcm_module/include/data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef enum {
    lcm_pin_rgb_24 = 0x00,
    lcm_pin_rgb_18,
    lcm_pin_rgb_16,
    lcm_pin_rgb_6,
    lcm_pin_mcu_18,
    lcm_pin_mipi_1c4d,
    lcm_pin_mipi0_1c2d,
    lcm_pin_mipi1_1c2d,
    lcm_pin_type_max,
}lcm_pin_type_e;

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
int32_t lcm_bus_pin_cfg(ADDR base_addr, lcm_pin_type_e lcm_pin_type);
int32_t lcm_bus_pin_default_cfg(ADDR base_addr, lcm_pin_type_e lcm_pin_type);
int32_t lcm_gpio_pin_cfg(ADDR base_addr, uint32_t gpio_id);
int32_t lcm_gpio_default_cfg(ADDR base_addr, uint32_t gpio_id);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

