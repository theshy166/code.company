
#ifndef _MOL_LCM_SERIAL_BUS_H_
#define _MOL_LCM_SERIAL_BUS_H_

#include "data_type.h"

typedef enum{
    SERIAL_BUS_I2C = 0,
    SERIAL_BUS_SPI,
    SERIAL_BUS_MAX
}lcm_serial_bus_type;

typedef enum{
    SPI_SPI = 0,
    SPI_SSP,
    SPI_NSM,
    SPI_TYPE_MAX
}lcm_serial_bus_spi_type;

typedef enum{
    SPI_MODE0 = 0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
    SPI_MODE_MAX
}lcm_serial_bus_spi_mode;

typedef struct{
    uint32_t mode; /*lcm_serial_bus_spi_mode*/
    uint32_t c_setup;
    uint32_t c_hold;
    uint32_t d_setup;
    uint32_t d_hold;

    uint32_t rd8_start_dummy_cycle;
    uint32_t rd_start_dummy_cycle;
    uint32_t rd_end_dummy_cycle;
    uint32_t delay;
}spi_bus_timing_t;

typedef struct{
    uint32_t slave_addr;
    uint32_t regLen;
    uint32_t dataLen;
}i2c_bus_timing_t;

typedef struct{
    uint32_t baudRate; /* sw max 140k because usleep and cup instruction execution */
    /* 140k 110k 90k 75k 65k 58k 52k */
    union{
        spi_bus_timing_t spi;
        i2c_bus_timing_t iic;
    }timing;
}serial_bus_info;

#endif
