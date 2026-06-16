#ifndef _DSI_PIXEL_INFO_PARAM_H_
#define _DSI_PIXEL_INFO_PARAM_H_

#include "../../lcm_module/include/data_type.h"
#include "dsi_phy_v01_drv_mpi_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
typedef struct {
    lcm_display_sync_info_t *dp_sync;
    uint32_t fps;
}dsi_get_pixel_clk_in_t;

typedef struct {
    uint32_t pixel_clk;
}dsi_get_pixel_clk_out_t;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
