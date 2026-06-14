#ifndef _MOL_LCM_SPI_API_H_
#define _MOL_LCM_SPI_API_H_

#include "data_type.h"
#include "spi_v01_drv_mpi_param.h"

#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
void *lcmSpiApiInit(spi_init_in_t *inParam, spi_init_out_t *outParam);
int32_t lcmSpiApiDeInit(spi_deinit_in_t *inParam, spi_deinit_out_t *outParam);
int32_t lcmSpiApiWriteCmd(void *handle, uint32_t cmd);
int32_t lcmSpiApiWriteData(void *handle, uint32_t data);
int32_t lcmSpiApiWriteBurstData(void *handle, uint32_t *data, uint32_t num);
int32_t lcmSpiApiReadData(void *handle, uint32_t *data);
int32_t lcmSpiApiReadBurstData(void *handle, uint32_t *data, uint32_t num);
int32_t lcmSpiApiIoCtrl(void *inParam, void *outParam);
int32_t lcmSpiApiPushTable(void *inParam, void *outParam);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

