#ifndef _DSI_HOST_V01_DRV_MPI_H_
#define _DSI_HOST_V01_DRV_MPI_H_

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

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
int32_t dsi_gen_wr_cmd(host_core_t hostId, uint8_t* params, uint16_t param_length);
uint32_t dsi_gen_rd_cmd(host_core_t hostId, uint8_t* params, uint16_t param_length, uint8_t bytes_to_read, uint8_t* read_buffer);
int32_t dsi_dcs_wr_cmd(host_core_t hostId, uint8_t* params, uint16_t param_length);
uint32_t dsi_dcs_rd_cmd(host_core_t hostId, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t* read_buffer);
int32_t dsi_dcs_rd_cmd_4_identify(host_core_t hostId, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t *read_buffer);
int32_t dsi_host_v0_init(dsi_host_init_in_t *inParam, dsi_host_init_out_t *outParam);
int32_t dsi_host_v0_deinit(dsi_host_deinit_in_t *inParam, dsi_host_deinit_out_t *outParam);
int32_t dsi_host_hs_mode(host_core_t hostId);
int32_t dsi_host_lp_mode(host_core_t hostId);
int32_t dsi_host_power_on(dsi_host_power_on_in_t *inParam, dsi_host_power_on_out_t *outParam);
int32_t dsi_host_power_off(dsi_host_power_off_in_t *inParam, dsi_host_power_off_out_t *outParam);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

