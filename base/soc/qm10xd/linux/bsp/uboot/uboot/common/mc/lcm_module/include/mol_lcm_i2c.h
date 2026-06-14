/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _MOL_LCM_I2C_H_
#define _MOL_LCM_I2C_H_

#include "data_type.h"
#include "mol_lcm_serial_bus.h"

typedef void lcm_i2c_fun_t(void);

typedef struct {
	int32_t (*read)(serial_bus_info *info, uint32_t cmd, uint32_t *data);
	int32_t (*write)(serial_bus_info *info, uint32_t cmd, uint32_t data);
	int32_t (*write_burst)(serial_bus_info *info, uint32_t cmd, uint8_t* buf, uint32_t num);
	int32_t (*read_burst)(serial_bus_info *info, uint32_t cmd, uint8_t* buf, uint32_t num);
}lcm_i2c_fun;

int32_t lcm_i2c_init(lcm_i2c_fun_t *inParam);
int32_t lcm_i2c_deInit(lcm_i2c_fun_t *inParam);

#endif
