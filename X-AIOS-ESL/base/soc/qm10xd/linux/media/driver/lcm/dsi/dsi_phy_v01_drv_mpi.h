#ifndef _DSI_PHY_V01_DRV_MPI_H_
#define _DSI_PHY_V01_DRV_MPI_H_

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

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
int32_t dsi_phy_v0_init(dsi_phy_init_in_t *inParam, dsi_phy_init_out_t *outParam);
int32_t dsi_phy_v0_deinit(dsi_phy_deinit_in_t *inParam, dsi_phy_deinit_out_t *outParam);
int32_t dsi_phy_power_on(dsi_phy_power_on_in_t *inParam, dsi_phy_power_on_out_t *outParam);
int32_t dsi_phy_power_off(dsi_phy_power_off_in_t *inParam, dsi_phy_power_off_out_t *outParam);
int32_t dsi_phy_pll_v1_on(dsi_phy_pll_on_in_t *inParam, dsi_phy_pll_on_out_t *outParam);
int32_t dsi_phy_param_show(fhosal_proc_entry_t *sqf, uint32_t phyId);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

