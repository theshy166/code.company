#ifndef _DSI_MPI_H_
#define _DSI_MPI_H_

#include "dsi_mpi_param.h"

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
int32_t dsi_generic_write(host_core_t hostId, uint8_t *param, uint16_t param_length);
int32_t dsi_generic_read(host_core_t hostId, uint8_t *param, uint16_t param_length, uint8_t bytes_to_read, uint8_t *read_buffer);
int32_t dsi_dcs_write(host_core_t hostId, uint8_t *param, uint16_t param_length);
int32_t dsi_dcs_read(host_core_t hostId, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t *read_buffer);
int32_t dsi_push_table(host_core_t hostId, dsi_setting_tab_t *table, uint32_t count);
int32_t dsi_init(dsi_init_in_t *inParam, dsi_init_out_t *outParam);
int32_t dsi_deinit(dsi_deinit_in_t *inParam, dsi_deinit_out_t *outParam);
int32_t dsi_lp_mode(dsi_lp_mode_in_t *inParam, dsi_lp_mode_out_t *outParam);
int32_t dsi_hs_mode(dsi_hs_mode_in_t *inParam, dsi_hs_mode_out_t *outParam);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

