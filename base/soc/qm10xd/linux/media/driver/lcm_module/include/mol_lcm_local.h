
#ifndef _MOU_LCM_LOCAL_H_
#define _MOU_LCM_LOCAL_H_

#include "mol_lcm_local_def.h"
#include "mol_lcm_i2c.h"
#include "mol_lcm_spi.h"
#include "mol_lcm_sfc.h"
#include "mol_lcm_mcu.h"
#include "mol_lcm_rgb.h"
#include "mol_lcm_mipi.h"
#include "mol_lcm_spi_gpio.h"
#include "lcm_com_param.h"

#define LCM_READ_TIMEOUT_MS 100

typedef struct {
    int32_t (*dvdd)(uint32_t lcm_id, uint32_t val);
    int32_t (*avdd)(uint32_t lcm_id, uint32_t val);
    int32_t (*iovdd)(uint32_t lcm_id, uint32_t val);
    int32_t (*vcom)(uint32_t lcm_id, uint32_t val);
    int32_t (*vgh)(uint32_t lcm_id, uint32_t val);
    int32_t (*vgl)(uint32_t lcm_id, uint32_t val);
    int32_t (*reset)(uint32_t lcm_id, uint32_t val);
    int32_t (*standby)(uint32_t lcm_id, uint32_t val);
    int32_t (*back_light_gpio)(uint32_t lcm_id, uint32_t val);
    int32_t (*back_light_pwm)(uint32_t lcm_id, uint32_t val);
    int32_t (*power_pwm)(uint32_t lcm_id, uint32_t val);
}lcm_power_fun;

typedef struct {
    union {
        lcm_mcu_fun mcu;
        lcm_mipi_dsi_fun mipi;
        lcm_i2c_fun i2c;
        lcm_spi_fun spi;
        lcm_spi_ctrl_fun spi_ctrl;
    } ctrl_fun;
    lcm_power_fun power;
    lcm_sfc_fun sfc;
    int32_t (*bus_init)(void *param, void *lcm_cfg);
    int32_t (*bus_deinit)(void *param, void *lcm_cfg);
    void (*delayMs)(uint32_t ms);
    int32_t (*debug_fun)(uint32_t eb, const char *fun, uint32_t line, char *status);
}lcm_ops;

typedef struct {
    int32_t (*poweron)(void * param, lcm_ops *ops); //param -> lcm_handle_t
    int32_t (*poweroff)(void * param,lcm_ops *ops); //param -> lcm_handle_t
    int32_t (*identify)(void * param, lcm_ops *ops); //param -> lcm_handle_t
    int32_t (*init)(void * param, lcm_ops *ops); //param -> lcm_handle_t
    int32_t (*suspend)(void * param, lcm_ops *ops); //param -> lcm_handle_t
    int32_t (*resume)(void * param, lcm_ops *ops); //param -> lcm_handle_t
    int32_t (*deinit)(void * param, lcm_ops *ops); //param -> lcm_handle_t
}lcm_module_fun;

typedef struct {
    char verder_name[256]; /*platform vender name*/
    char name[256]; /*lcm name*/
    uint32_t width; /*lcm resolution width*/
    uint32_t height; /*lcm resolution height*/
    lcm_type_e type; /*mcu, rgb, mipi*/
    uint32_t fps; /*lcm reflash frame rate*/
    lcm_direction direction;
    lcm_display_sync_info_t *dp_sync;
    union {
        lcm_mcu_info *mcu;
        lcm_rgb_info *rgb;
        lcm_mipi_info *mipi;
        lcm_spi_info *spi;
    }infor;
    lcm_module_fun *fun;
}lcm_cfg_t;

#endif
