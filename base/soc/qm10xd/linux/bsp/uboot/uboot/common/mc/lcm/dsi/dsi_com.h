#ifndef _DSI_COM_H_
#define _DSI_COM_H_

#include "../../lcm_module/include/data_type.h"
#include "../../lcm_module/include/dsi_com_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

dsi_phy_work_mode_e get_phy_work_mode(phy_core_e phyId, dsi_phy_lane_num_e laneNum);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
