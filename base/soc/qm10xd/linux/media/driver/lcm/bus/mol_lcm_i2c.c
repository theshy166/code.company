#include "data_type.h"
#include "mol_lcm_i2c.h"

int32_t lcm_i2c_init(lcm_i2c_fun_t *inParam){
	int32_t rtn = 0;

	return rtn;
}

int32_t lcm_i2c_deInit(lcm_i2c_fun_t *inParam){
	int32_t rtn = 0;

	return rtn;
}

int32_t i2c_read(serial_bus_info *info,uint32_t cmd,uint32_t *data)
{
	int32_t rtn = 0;

	return rtn;
}

int32_t i2c_read_burst(serial_bus_info *info,uint32_t cmd,uint8_t *buf,uint32_t num)
{
	int32_t rtn = 0;

	return rtn;
}

int32_t i2c_write_burst(serial_bus_info *info,uint32_t cmd,uint8_t *buf,uint32_t num)
{
	int32_t rtn = 0;

	return rtn;
}

int32_t i2c_write(serial_bus_info *info,uint32_t cmd,uint32_t data)
{
	int32_t rtn = 0;

	return rtn;

}

lcm_i2c_fun lcm_i2c_rw ={
	.read = i2c_read,
	.write = i2c_write,
	.write_burst = i2c_write_burst,
	.read_burst = i2c_read_burst,
};

