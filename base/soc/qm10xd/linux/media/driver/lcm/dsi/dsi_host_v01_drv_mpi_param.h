#ifndef _DSI_HOST_V01_DRV_MPI_PARAM_H_
#define _DSI_HOST_V01_DRV_MPI_PARAM_H_

#include "data_type.h"
#include "dsi_phy_v01_drv_mpi_param.h"

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
    HOST0 = 0, //1C4D
    HOST1,     //1C2D
    HOST_CORE_MAX
}host_core_t;

typedef struct {
    host_core_t hostId;
    ADDR base_addr;
    dsi_format_e format;
    uint32_t laneNum;
    uint32_t *host_sync_div;
    lcm_display_sync_info_t *dp_sync_info;
    uint32_t *phy_txreadyesc;
}dsi_host_init_in_t;

typedef struct {
    uint32_t reserved;
}dsi_host_init_out_t;

typedef struct {
    uint32_t reserved;
}dsi_host_update_out_t;

typedef struct {
    host_core_t hostId;
    uint32_t reserved;
}dsi_host_stream_on_in_t;

typedef struct {
    uint32_t reserved;
}dsi_host_stream_on_out_t;

typedef struct {
    host_core_t hostId;
    uint32_t reserved;
}dsi_host_stream_off_in_t;

typedef struct {
    uint32_t reserved;
}dsi_host_stream_off_out_t;

typedef struct {
    host_core_t hostId;
    uint32_t reserved;
}dsi_host_deinit_in_t;

typedef struct {
    uint32_t reserved;
}dsi_host_deinit_out_t;

typedef struct {
    host_core_t hostId;
    uint32_t clk_addr;
    uint32_t rst_addr;
    uint32_t host_addr;
    uint32_t phy_addr;
}dsi_host_power_on_in_t;

typedef struct {
    uint32_t reserved;
}dsi_host_power_on_out_t;

typedef struct {
    host_core_t hostId;
    uint32_t phy_base_addr;
    uint32_t rst_base_addr;
}dsi_host_power_off_in_t;

typedef struct {
    uint32_t reserved;
}dsi_host_power_off_out_t;

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

