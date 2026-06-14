
#ifndef _MOL_LCM_SPI_PARAM_H_
#define _MOL_LCM_SPI_PARAM_H_

#include "data_type.h"
#include "mol_lcm_serial_bus.h"

typedef struct {
    uint32_t pin_base_addr;
    uint32_t spi_base_addr;
    serial_bus_info *info;
}lcm_spi_init_in_t;

typedef struct {
    uint32_t reserved;
}lcm_spi_init_out_t;

typedef struct {
    uint32_t reserved;
}lcm_spi_deinit_in_t;

typedef struct {
    uint32_t reserved;
}lcm_spi_deinit_out_t;

#endif
