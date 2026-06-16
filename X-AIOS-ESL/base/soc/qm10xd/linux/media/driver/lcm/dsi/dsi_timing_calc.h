#ifndef _DSI_TIMING_CALC_H_
#define _DSI_TIMING_CALC_H_

#include "dsi_timing_calc_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
int32_t dsi_phy_timing_calc(dsi_phy_timing_in_t *inParam, dsi_phy_timing_out_t *outParam);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
