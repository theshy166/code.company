#include "../../lcm_module/include/mol_lcm_sfc.h"
#include "../../lcm_module/include/mol_lcm.h"

uint32_t s_lcm_register_id[LCM_MAX_ID] = {
0x00
};

uint32_t *_get_register_id_ptr(void)
{
    return s_lcm_register_id;
}


int32_t sfc_read(serial_bus_info *info,uint32_t cmd,uint32_t *data)
{
    int32_t rtn = 0;
    uint32_t *id_ptr = _get_register_id_ptr();

    if (NULL != id_ptr) {
        *data = id_ptr[cmd];
    } else {
        rtn = 1;
    }

    return rtn;
}

int32_t sfc_write(serial_bus_info *info,uint32_t cmd,uint32_t data)
{
    int32_t rtn = 0;
    uint32_t *id_ptr = _get_register_id_ptr();

    if (NULL != id_ptr) {
        id_ptr[cmd] = data;
    } else {
        rtn = 1;
    }

    return rtn;
}


int32_t sfc_earse(serial_bus_info *info,uint32_t cmd,uint32_t addr)
{

	return 0;
}

lcm_sfc_fun lcm_sfc_rw ={
	.read = sfc_read,
	.write = sfc_write,
	.earse = sfc_earse,
};

int32_t lcm_sfc_init(void      *inparam,lcm_sfc_fun_t *inParam)
{
    lcm_handle_t *handle = (lcm_handle_t*)inparam;
    mol_lcm_init_in *lcm_init;
    lcm_ops *ops;

    lcm_init = &handle->lcm_init;
    ops = &lcm_init->ops;
    ops->sfc.read = lcm_sfc_rw.read;
    ops->sfc.write = lcm_sfc_rw.write;
    ops->sfc.earse = lcm_sfc_rw.earse;

    return 0;
}

int32_t lcm_sfc_deInit(void     *inparam,lcm_sfc_fun_t *inParam)
{
	lcm_handle_t *handle = (lcm_handle_t*)inparam;
	mol_lcm_init_in *lcm_init;
	lcm_ops *ops;

	lcm_init = &handle->lcm_init;
	ops = &lcm_init->ops;
	ops->sfc.read = NULL;
	ops->sfc.write = NULL;
	ops->sfc.earse = NULL;
	return 0;
}

