#ifndef _MOL_GPIO_SPI_DRV_H_
#define _MOL_GPIO_SPI_DRV_H_

#include "data_type.h"
#include "mol_lcm_spi_param.h"

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
int32_t gpio_spi_init(lcm_spi_init_in_t *inParam);
int32_t gpio_spi_deinit(lcm_spi_deinit_in_t *inParam);

int32_t gpio_spi_write_cmd(serial_bus_info *info, int32_t cmd);
int32_t gpio_spi_write_data(serial_bus_info *info, int32_t data);
int32_t gpio_spi_read(serial_bus_info *info, int32_t cmd, uint8_t *data, int32_t read_len);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
