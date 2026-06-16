
#ifndef _MOL_LCM_H_
#define _MOL_LCM_H_

#include "mol_lcm_local.h"

#ifndef __KERNEL__
#ifndef __iomem
#define __iomem
#endif
#endif

#define INVALID_INDEX  0xFFFFFF
#define LCM_MAX_ID 2

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(v) (sizeof(v)/sizeof((v)[0]))
#endif

typedef enum{
    LCM_OK = 0,
    LCM_ID_ERR ,
    LCM_INDEX_ERR
}lcm_rnt;

typedef enum{
    LCM_ID_MAIN = 0,
    LCM_ID_SUB,
    LCM_ID_MAX
}lcm_id_mode;

typedef enum{
    LCM_ID_NORMAL_OPS = 0,
    LCM_ID_ASSING_OPS,
    LCM_ID_DISABLE_OPS,
    LCM_ID_OPS_MAX
}lcm_id_cfg_e;

typedef enum {
    LCM_REGBASE_TOPRST = 0, // rst
    LCM_REGBASE_TOPCLK,     // clk
    LCM_REGBASE_VOU_SEL,     // clk
    LCM_REGBASE_VOU,
    LCM_REGBASE_DSI_DPHY,
    LCM_REGBASE_DSI_HOST_4LANE,
    LCM_REGBASE_DSI_HOST_2LANE,
    LCM_REGBASE_GPIO,
    LCM_REGBASE_SPI,
    LCM_REGBASE_I2C,
    LCM_REGBASE_PIN_MUX,
    LCM_REGBASE_NUM,
} lcm_reg_base_idx;

typedef struct {
    void __iomem *base[LCM_REGBASE_NUM];
    uint32_t phyAddr[LCM_REGBASE_NUM];
} lcm_reg_base_t;

typedef struct {
    lcm_id_mode lcm_id;
    uint32_t pin_ctrl;
    int32_t (*lcm_power)(uint32_t val);
    int32_t (*lcm_reset)(uint32_t val);
    int32_t (*mipi)(uint32_t eb);
    int32_t (*mcu)(uint32_t eb);
    int32_t (*spi)(uint32_t eb);
    int32_t (*iic)(uint32_t eb);
}mol_lcm_probe_in;

typedef struct {
    lcm_id_mode lcm_id;
}mol_lcm_get_info_in;

typedef struct {
    lcm_id_mode lcm_id;
    lcm_ops ops;
    void *handle;
}mol_lcm_init_in;

typedef struct {
    lcm_id_mode lcm_id;
    lcm_ops ops;
}mol_lcm_deinit_in;

typedef struct {
    lcm_id_mode lcm_id;
    uint32_t light;
}mol_lcm_back_light_in;

typedef struct {
    lcm_id_mode lcm_id;
}mol_lcm_suspend_in;

typedef struct {
    lcm_id_mode lcm_id;
}mol_lcm_resume_in;

typedef struct {
    uint8_t idx;
    uint32_t width;
    uint32_t height;
    uint32_t fps;
} lcm_init_param_t;

typedef struct{
    lcm_id_mode lcm_id;
    lcm_id_cfg_e lcm_id_cfg;
    uint32_t status;
    lcm_ops ops;
    lcm_init_param_t init_param;
    lcm_reg_base_t *reg;
    mol_lcm_probe_in lcm_probe;
    mol_lcm_get_info_in lcm_get_info;
    mol_lcm_init_in lcm_init;
    mol_lcm_deinit_in lcm_deinit;
    mol_lcm_back_light_in lcm_back_light;
    mol_lcm_suspend_in lcm_suspend;
    mol_lcm_resume_in lcm_resume;
    lcm_cfg_t *lcmCfg;
    void *handle;
    serial_bus_info info;
    uint32_t prob_info[512];
}lcm_handle_t;

int32_t mol_lcm_probe(lcm_handle_t *handle);
int32_t mol_lcm_init(lcm_handle_t *handle);
int32_t mol_lcm_deinit(lcm_handle_t *handle);
int32_t mol_lcm_suspend(lcm_handle_t *handle);
int32_t mol_lcm_resume(lcm_handle_t *handle);

#endif
