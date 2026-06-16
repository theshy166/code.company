
#ifndef _MOL_LCM_SPI_H_
#define _MOL_LCM_SPI_H_

#include "mol_lcm_serial_bus.h"


typedef struct {
    lcm_serial_bus_spi_type type;
    uint32_t baudRate;
    union{
        spi_bus_timing_t *spi;
        spi_bus_timing_t *ssp;
        spi_bus_timing_t *nsm;
    }timing;
    void *clk;
}lcm_spi_info;

typedef struct {
    int32_t (*write_cmd)(void *handle, uint32_t cmd);
    int32_t (*write_data)(void *handle, uint32_t data);
    int32_t (*write_burst_data)(void *handle, uint32_t *data, uint32_t num);
    int32_t (*read_data)(void *handle, uint32_t *data);
    int32_t (*read_burst_data)(void *handle, uint32_t *data, uint32_t num);
    int32_t (*io_ctrl)(void *inParam, void *outParam);
    int32_t (*push_tab)(void *inParam, void *outParam);
}lcm_spi_fun;

#endif
