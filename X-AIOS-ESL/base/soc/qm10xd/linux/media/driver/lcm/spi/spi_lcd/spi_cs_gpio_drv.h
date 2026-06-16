#ifndef _SPI_CS_GPIO_DRV_H_
#define _SPI_CS_GPIO_DRV_H_

#include "data_type.h"

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
int32_t spi_rest_gpio_init(uint32_t base_addr);
int32_t spi_cs_rs_init(uint32_t base_addr);
int32_t spi_lcd_reset(uint32_t base_addr, uint32_t val);
int32_t spi_cs_gpio(uint32_t base_addr, uint32_t val);
int32_t spi_rs_gpio(uint32_t base_addr, uint32_t val);
void spi_lcd_pinmux(void);
void spi_lcd_gpio_request(int request);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

