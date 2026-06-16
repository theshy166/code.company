#ifndef _CTRL_LCD_V01_DRV_H_
#define _CTRL_LCD_V01_DRV_H_

#include "data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
int32_t lcm_dvdd_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_avdd_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_iovdd_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_vcom_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_vgh_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_vgl_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_reset_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_standby_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_back_light_gpio_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_back_light_pwm_set(uint32_t lcm_id, uint32_t val);
int32_t lcm_power_pwm_set(uint32_t lcm_id, uint32_t val);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

