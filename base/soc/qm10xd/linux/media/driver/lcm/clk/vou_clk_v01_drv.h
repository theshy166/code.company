#ifndef _VIO_APB_DRV_H_
#define _VIO_APB_DRV_H_

#include "data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef enum {
    VOU_OUT_HDMI = 0,
    VOU_OUT_DSI,
    VOU_OUT_AVE,
    VOU_OUT_BT656,
    VOU_OUT_LCM,
    VOU_OUT_MODE_MAX,
}vou_out_mode_e;

typedef struct {
    uint32_t sel; //vo clk sel
    uint32_t div; // vo clk div
}vou_clk_cfg_t;

typedef struct {
    uint32_t mode; // 0:auto calc 1:manual calc
    vou_clk_cfg_t cfg;
}vou_clk_info_t;

typedef struct {
    vou_clk_info_t clkInfo;
    uint32_t total_pixel;
    uint32_t tx_frq_pixel;
}vou_clk_calc_in_t;

typedef struct {
    uint32_t if_clk;
    vou_clk_cfg_t cfg;
}vou_clk_calc_out_t;


typedef struct {
    ADDR clk_base_addr;
    ADDR rst_base_addr;
    uint32_t clk_sel;
    uint32_t clk_div;
}lcm_clk_on_in_t;

typedef struct {
    uint32_t reserved;
}lcm_clk_on_out_t;

typedef struct {
    ADDR clk_base_addr;
    ADDR rst_base_addr;
}lcm_clk_off_in_t;

typedef struct {
    uint32_t reserved;
}lcm_clk_off_out_t;

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
int32_t vou_clk_calc(vou_clk_calc_in_t *inParam, vou_clk_calc_out_t *outParam);
int32_t lcm_clk_off(lcm_clk_off_in_t *inParam, lcm_clk_off_out_t *outParam);
int32_t lcm_clk_on(lcm_clk_on_in_t *inParam, lcm_clk_on_out_t *outParam);
int32_t lcm_clk_status_clean(void);
int32_t vou_dhd_clk_on(ADDR base_addr, uint32_t sel, uint32_t div);
int32_t vou_dhd_rst(ADDR base_addr, uint32_t dhd_id);
int32_t vou_dhd_clk_off(ADDR base_addr, uint32_t dhd_id);
int32_t vou_lcd_src_sel(ADDR base_addr, uint32_t sel);
int32_t vou_dhd_src_sel(ADDR base_addr, uint32_t dhd_id, uint32_t sel);
int32_t vou_clk_on(ADDR base_addr, uint32_t sel, uint32_t div);
int32_t vou_reset(ADDR base_addr);
int32_t vou_clk_on_reset(ADDR clk_base_addr, uint32_t sel, uint32_t div, ADDR rst_base_addr);
int32_t vou_clk_off(ADDR base_addr);
int32_t dsi_host_clk_on(ADDR base_addr);
int32_t dsi_host_reset(ADDR base_addr);
int32_t dsi_host_clk_on_reset(ADDR clk_base_addr, ADDR rst_base_addr);
int32_t dsi_host_clk_off(ADDR base_addr);
int32_t dsi_phy_clk_on(ADDR base_addr);
int32_t dsi_phy_reset(ADDR base_addr);
int32_t dsi_phy_clk_on_reset(ADDR clk_base_addr, ADDR rst_base_addr);
int32_t dsi_phy_clk_off(ADDR base_addr);
int32_t vou_dhd_out_mode(ADDR base_addr, uint32_t dhd_id, vou_out_mode_e mode);
int32_t vou_dhd_force_update(ADDR base_addr, uint32_t dhd_id, uint32_t force_update);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

