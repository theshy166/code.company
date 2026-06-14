#ifndef _DSI_MPI_PARAM_H_
#define _DSI_MPI_PARAM_H_

#include "../../lcm_module/include/data_type.h"
#include "../../lcm_module/include/dsi_com_param.h"
#include "dsi_host_v01_drv_mpi_param.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

typedef struct {
    host_core_t hostId;

    ADDR rst_base_addr;
    ADDR clk_base_addr;
    ADDR vou_base_addr;
    ADDR host_base_addr;
    ADDR phy_base_addr;
    ADDR pin_base_addr;

    uint32_t fps;
    dsi_format_e format;
    dsi_phy_lane_num_e laneNum;
    dsi_timing_t timing;
    lcm_display_sync_info_t dp_sync_info;
}dsi_init_in_t;

typedef struct {
    uint32_t reserved;
}dsi_init_out_t;

typedef struct {
    host_core_t hostId;
    uint32_t reserved;
}dsi_deinit_in_t;

typedef struct {
    uint32_t reserved;
}dsi_deinit_out_t;

typedef struct {
    host_core_t hostId;
}dsi_hs_mode_in_t;

typedef struct {
    uint32_t reserved;
}dsi_hs_mode_out_t;

typedef struct {
    host_core_t hostId;
}dsi_lp_mode_in_t;

typedef struct {
    uint32_t reserved;
}dsi_lp_mode_out_t;


/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

