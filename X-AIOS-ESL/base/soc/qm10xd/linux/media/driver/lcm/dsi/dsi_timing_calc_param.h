#ifndef _DSI_TIMING_CALC_PARAM_H_
#define _DSI_TIMING_CALC_PARAM_H_

#include "data_type.h"
#include "dsi_com_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
typedef struct {
    lcm_display_sync_info_t *dp_sync;
    uint32_t fps;
    uint32_t format;
    uint32_t laneNum;
    uint32_t lane_clk;
    dsi_timing_t *timing;
}dsi_phy_timing_in_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_timing_out_t;


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
