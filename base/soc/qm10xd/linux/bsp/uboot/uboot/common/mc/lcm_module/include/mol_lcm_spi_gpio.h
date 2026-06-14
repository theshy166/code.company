
#ifndef _MOL_LCM_SPI_GPIO_H_
#define _MOL_LCM_SPI_GPIO_H_

#include "data_type.h"
#include "mol_lcm_spi_param.h"

typedef void lcm_spi_fun_t(void);
typedef struct {
	int32_t (*bus_init)(void *param, void *lcm_cfg);
	int32_t (*bus_deinit)(void *param, void *lcm_cfg);

    int32_t (*write_cmd)(serial_bus_info *info, uint32_t cmd);
    int32_t (*write_data)(serial_bus_info *info, uint32_t data);
    int32_t (*read)(serial_bus_info *info, uint32_t cmd, uint8_t *data, uint32_t rd_num);

    int32_t (*write_burst_cmd)(serial_bus_info *info, uint8_t *cmd, uint32_t num);
    int32_t (*write_burst_data)(serial_bus_info *info, uint8_t *data, uint32_t num);
    int32_t (*read_burst)(serial_bus_info *info, uint32_t cmd, uint8_t* buf, uint32_t rd_num);
}lcm_spi_ctrl_fun;

int32_t write_cmd(serial_bus_info *info, uint32_t cmd);
int32_t write_data(serial_bus_info *info, uint32_t data);
int32_t read_data(serial_bus_info *info, uint32_t cmd, uint8_t *data, uint32_t rd_num);
int32_t lcm_spi_init(lcm_spi_init_in_t *inParam, lcm_spi_init_out_t *outParam);
int32_t lcm_spi_deInit(lcm_spi_deinit_in_t *inParam, lcm_spi_deinit_out_t *outParam);

#endif
