
#ifndef _MOL_LCM_SFC_H_
#define _MOL_LCM_SFC_H_

#include "data_type.h"
#include "mol_lcm_serial_bus.h"

typedef void lcm_sfc_fun_t(void);
typedef struct {
	int32_t (*read)(serial_bus_info *info, uint32_t cmd, uint32_t *data);
	int32_t (*write)(serial_bus_info *info, uint32_t cmd, uint32_t data);
	int32_t (*earse)(serial_bus_info *info, uint32_t cmd, uint32_t addr);
}lcm_sfc_fun;

int32_t sfc_write(serial_bus_info *info,uint32_t cmd,uint32_t data);
int32_t lcm_sfc_init(void *inparam,lcm_sfc_fun_t *inParam);
int32_t lcm_sfc_deInit(void *inparam,lcm_sfc_fun_t *inParam);

#endif
