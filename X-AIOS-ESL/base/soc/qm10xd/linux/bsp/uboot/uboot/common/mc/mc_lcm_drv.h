#ifndef __MC_LCM_DRV_H__
#define __MC_LCM_DRV_H__
#include "mc_vou_osal.h"

typedef enum {
    LCM_TYPE_MCU = 0,
    LCM_TYPE_RGB,
    LCM_TYPE_MIPI,
    LCM_TYPE_SPI,
    LCM_TYPE_NONE,
    LCM_TYPE_MAX
}lcm_type_e;

void lcm_func_init(vou_dev_idx_e devId);
void dsi_init(void);
void rgb_init(void);
#endif



