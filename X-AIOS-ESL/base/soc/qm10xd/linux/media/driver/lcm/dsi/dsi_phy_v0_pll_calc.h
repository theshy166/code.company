#ifndef _DSI_PHY_V01_PLL_CALC_H_
#define _DSI_PHY_V01_PLL_CALC_H_

#include "data_type.h"
#include "dsi_phy_v0_pll_calc_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

int32_t dsi_phy_pll_cfg_get(dsi_phy_pll_calc_in_t *inParam, dsi_phy_pll_calc_out_t *outParam);
int32_t rgb_pll_cfg_get(dsi_phy_pll_calc_in_t *inParam, dsi_phy_pll_calc_out_t *outParam);
int32_t mcu_pll_cfg_get(dsi_phy_pll_calc_in_t *inParam, dsi_phy_pll_calc_out_t *outParam);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
