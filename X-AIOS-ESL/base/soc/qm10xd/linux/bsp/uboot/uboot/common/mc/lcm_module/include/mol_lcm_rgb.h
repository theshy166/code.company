#ifndef _MOL_LCM_RGB_H_
#define _MOL_LCM_RGB_H_

#include "data_type.h"
#include "mol_lcm_local_def.h"
#include "mol_lcm_serial_bus.h"
#include "lcm_com_param.h"

typedef enum {
    RGB_IF_18BITS = 0,
    RGB_IF_16BITS,
    RGB_IF_6BITS,
    RGB_IF_24BITS,
    RGB_IF_MAX
}lcm_rgb_if_type_e;

typedef struct {
    vou_clk_t vou;
    mif_clk_t mif;
}rgb_clk_t;

typedef struct {
    lcm_serial_bus_type ctrl_bus_mode; /*spi, i2c, none*/
    uint16_t video_bus_width;
    uint16_t h_sync_pol;
    uint16_t v_sync_pol;
    uint16_t de_pol;

    lcm_display_format_e format;
    lcm_rgb_if_type_e if_type;
    lcm_rgb_order_e order;
    uint32_t dither;
    uint32_t bitswap;

    serial_bus_info *serial;
    rgb_clk_t *clk;
}lcm_rgb_info;

#endif
