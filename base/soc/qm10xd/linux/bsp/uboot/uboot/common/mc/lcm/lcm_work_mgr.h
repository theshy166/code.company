#ifndef _MC_LCM_WORK_MGR_H_
#define _MC_LCM_WORK_MGR_H_

#include "../lcm_module/include/data_type.h"
#include "lcm_work_mgr_param.h"

char *get_lcm_work_mode_str(uint32_t lcmWorkMode);
lcm_work_mode_e get_lcm_work_mode(lcm_cfg_t *inParam);
int32_t set_lcm_work_mode(uint32_t lcmId, lcm_work_mode_e lcmWorkMode);
int32_t check_lcm_work_mode(uint32_t lcmId, lcm_work_mode_e lcmWorkMode);

#endif
