#ifndef _DSI_PHY_V01_PLL_CALC_PARAM_H_
#define _DSI_PHY_V01_PLL_CALC_PARAM_H_

#include "data_type.h"
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
    uint32_t laneNum;
    uint32_t format;
    mif_clk_t *clk;
} dsi_phy_pll_calc_in_t;

typedef struct {
    uint32_t pll_doubler; // src clk doubler en
    uint32_t pll_div_s; // feadback div sel 0:integer 1:fractional
    uint32_t pll_n; // f_pfd_div integer [24-14] for integer mode
    uint32_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
    uint32_t pll_nint; // f_pfd_div integer [8-0] for fractional mode
    uint32_t pll_pdiv;     // post div [25-23]  2^n
    uint32_t pixelclk_div; // fvco/N [29-26]   16 1--15
    uint32_t dhd_div;
    uint32_t lane_clk;
    uint32_t *host_sync_div; // byte uint
} dsi_phy_pll_calc_out_t;

typedef struct {
    uint32_t base_addr;
    uint32_t pll_ctrl_param0;
}dsi_phy_pll_parser_in_t;

typedef struct {
    uint32_t lane_clk;
}dsi_phy_pll_parser_out_t;



typedef struct {
    lcm_display_sync_info_t *dp_sync;
    uint32_t fps;
    uint32_t format;
    uint32_t if_type;
    mif_clk_t *clk;
}rgb_pll_cfg_in_t;

typedef struct {
    uint32_t pll_doubler; // src clk doubler en
    uint32_t pll_div_s; // feadback div sel 0:integer 1:fractional
    uint32_t pll_n; // f_pfd_div integer [24-14] for integer mode
    uint32_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
    uint32_t pll_nint; // f_pfd_div integer [8-0] for fractional mode
    uint32_t pixelclk_div; // fvco/N [29-26]
    uint32_t pll_pdiv;     // post div [25-23]
    uint32_t dhd_div;
    uint32_t lane_clk;
}rgb_pll_cfg_out_t;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
