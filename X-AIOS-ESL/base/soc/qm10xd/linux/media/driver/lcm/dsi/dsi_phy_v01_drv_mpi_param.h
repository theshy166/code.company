#ifndef _DSI_PHY_V01_DRV_MPI_PARAM_H_
#define _DSI_PHY_V01_DRV_MPI_PARAM_H_

#include "data_type.h"
#include "dsi_phy_v01_drv_param.h"
#include "dsi_phy_v01_drv_mpi_param.h"
#include "dsi_com_param.h"


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
    ADDR phy_addr;
    ADDR vou_addr;
    phy_core_e phyId;
    uint32_t fps;
    dsi_format_e format;
    dsi_phy_lane_num_e laneNum;
    lcm_display_sync_info_t *dp_sync_info;
    dsi_timing_t *timing;
}dsi_phy_init_in_t;

typedef struct {
    dsi_phy_work_mode_e *phyMode;
    uint32_t *host_sync_div;
    uint32_t *phy_txreadyesc;
}dsi_phy_init_out_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_update_outparam_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_stream_on_inparam_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_stream_on_outparam_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_stream_off_inparam_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_stream_off_outparam_t;

typedef struct {
    phy_core_e phyId;
}dsi_phy_deinit_in_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_deinit_out_t;

typedef struct {
    phy_core_e phyId;
    uint32_t rst_base_addr;
    uint32_t clk_base_addr;
    uint32_t phy_base_addr;
}dsi_phy_power_on_in_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_power_on_out_t;

typedef struct {
    phy_core_e phyId;
    uint32_t phy_base_addr;
    uint32_t rst_base_addr;
}dsi_phy_power_off_in_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_power_off_out_t;

typedef struct {
    uint32_t phyId;
    uint32_t phy_addr;
    uint32_t vou_addr;
    uint32_t pll_doubler; // src clk doubler en
    uint32_t pll_div_s; // feadback div sel 0:integer 1:fractional
    uint32_t pll_n; // f_pfd_div integer [24-14] for integer mode
    uint32_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
    uint32_t pll_nint; // f_pfd_div integer [8-0] for fractional mode
    uint32_t pll_pdiv;     // post div [25-23]
    uint32_t pixelclk_div; // fvco/N [29-26]
    uint32_t dhd_div;
    uint32_t lane_clk;
}dsi_phy_pll_on_in_t;

typedef struct {
    uint32_t reserved;
}dsi_phy_pll_on_out_t;


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

