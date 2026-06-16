#ifndef _SPI_V01_DRV_MPI_H_
#define _SPI_V01_DRV_MPI_H_

#include "spi_v01_drv_mpi_param.h"

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
int32_t spi_ioCtrl(spi_io_ctrl_in_t *inParam, spi_io_ctrl_out_t *outParam);
int32_t spi_v0_init(spi_init_in_t *inParam, spi_init_out_t *outParam);
int32_t spi_v0_deinit(spi_deinit_in_t *inParam, spi_deinit_out_t *outParam);
int32_t spi_single_write_cmd(void *handle, uint8_t cmd);
int32_t spi_single_write_data(void *handle, uint8_t data);
int32_t spi_burst_write_data(void *handle, ADDR *data, uint32_t len);
int32_t spi_single_read_data(void *handle, ADDR *data);
int32_t spi_burst_read_data(void *handle, ADDR *data, uint32_t len);
int32_t spi_push_table(void *handle, void *setting, uint32_t num);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

