#ifndef _MC_LCM_WORK_MGR_PARAM_H_
#define _MC_LCM_WORK_MGR_PARAM_H_

#include "data_type.h"

typedef enum {
    LCM_WORK_NONE = 0,
    LCM_WORK_1C4D,
    LCM_WORK_1C3D,
    LCM_WORK_1C2D,
    LCM_WORK_1C1D,
    LCM_WORK_RGB_6,
    LCM_WORK_RGB_16,
    LCM_WORK_RGB_18,
    LCM_WORK_RGB_24,
    LCM_WORK_MCU_8,
    LCM_WORK_MCU_9,
    LCM_WORK_MCU_16,
    LCM_WORK_MCU_18,
    LCM_WORK_MCU_8_NO_RD,
    LCM_WORK_MCU_9_NO_RD,
    LCM_WORK_MCU_16_NO_RD,
    LCM_WORK_MCU_18_NO_RD,
    LCM_WORK_MODE_MAX
}lcm_work_mode_e;



#endif
