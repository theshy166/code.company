
#ifndef _MOL_LCM_MCU_H_
#define _MOL_LCM_MCU_H_

#include "data_type.h"
#include "mol_lcm_local_def.h"
#include "lcm_com_param.h"

typedef enum {
    MCU_IF_18BITS = 0,
    MCU_IF_16BITS,
    MCU_IF_9BITS,
    MCU_IF_8BITS,
    MCU_IF_MAX
}lcm_mcu_if_type_e;

typedef enum {
    MCU_LSB_FIRST = 0,
    MCU_HSB_FIRST,
    MCU_TRANS_MAX,
}lcm_mcu_trans_e;

typedef enum {
    LCM_SETTING_CMD = 0,
    LCM_SETTING_DATA,
    LCM_SETTING_MAX,
}lcm_setting_type_e;

typedef struct  {
    uint32_t type;
    uint32_t val;
}lcm_setting_tab_t;

typedef struct {
    uint32_t csType;
    uint32_t rdSetup;
    uint32_t rdHold;
    uint32_t rdDestory;
    uint32_t wrSetup;
    uint32_t wrHold;
    uint32_t wrDestory;
}lcm_parallel_bus_timing;

typedef struct {
    uint32_t eb;
    lcm_mode_e mode;
    lcm_edge_e edge;
}lcm_te_t;

typedef struct {
    vou_clk_t vou;
    mif_clk_t mif;
}lcm_mcu_clk_t;

typedef struct {
    lcm_display_format_e format;
    lcm_mcu_if_type_e if_type;
    lcm_mcu_trans_e trans;
    lcm_rgb_order_e order;
    uint32_t dither;
    uint32_t bitswap;
    uint32_t rd_eb;
    lcm_te_t te;
    lcm_parallel_bus_timing *timing;
    lcm_mcu_clk_t *clk;
}lcm_mcu_info;

typedef struct {
    int32_t (*write_cmd)(void *handle, uint32_t cmd);
    int32_t (*write_data)(void *handle, uint32_t data);
    int32_t (*write_burst_data)(void *handle, uint32_t *data, uint32_t num);
    int32_t (*read_data)(void *handle, uint32_t *data, uint32_t num);
    int32_t (*read_burst_data)(void *handle, uint32_t *data, uint32_t num);
    int32_t (*push_tab)(void *handle, void *setting, uint32_t num);
}lcm_mcu_fun;

#endif
