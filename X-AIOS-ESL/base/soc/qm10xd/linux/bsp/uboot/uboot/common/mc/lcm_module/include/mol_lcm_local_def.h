
#ifndef _MOU_LCM_LOCAL_DEFINE_H_
#define _MOU_LCM_LOCAL_DEFINE_H_

#define VENDER_NAME "MC_LCM"

#define LCM_DELAY 0xFFFFFFFE
#define LCM_END_OF_TABLE 0xFFFFFFFF

typedef enum {
    LCM_TYPE_MCU = 0,
    LCM_TYPE_RGB,
    LCM_TYPE_MIPI,
    LCM_TYPE_SPI,
    LCM_TYPE_NONE,
    LCM_TYPE_MAX
}lcm_type_e;

typedef enum{
    LCD_MISMATCH = 0,
    LCD_MATCH
}id_ret_t;

typedef enum {
    LCM_BUS_8080 = 0,
    LCM_BUS_RGB,
    LCM_BUS_SPI,
    LCM_BUS_IIC,
    LCM_BUS_MAX
}lcm_bus;

typedef enum{
    LCM_POLARITY_POS = 0,
    LCM_POLARITY_NEG,
    LCM_POLARITY_MAX
}lcm_polarity;

typedef enum{
    LCM_POS_EDGE = 0,
    LCM_NEG_EDGE,
    LCM_EDGE_MAX
}lcm_edge_e;

typedef enum{
    LCM_HW_MODE = 0,
    LCM_SW_MODE,
    LCM_MODE_MAX
}lcm_mode_e;

typedef enum {
    LCM_DIRECT_NORMAL = 0,
    LCM_DIRECT_ROT_90,
    LCM_DIRECT_ROT_180,
    LCM_DIRECT_ROT_270,
    LCM_DIRECT_MIR_H,
    LCM_DIRECT_MIR_V,
    LCM_DIRECT_MIR_HV,
    LCM_DIRECT_MAX
}lcm_direction;

typedef enum {
    LCM_RGB_888 = 0,
    LCM_RGB_666,
    LCM_RGB_565,
    LCM_RGB_GRAY6,
}lcm_display_format_e;

typedef enum {
    RGB_ORDER_RGB = 0,
    RGB_ORDER_RBG,
    RGB_ORDER_GBR,
    RGB_ORDER_GRB,
    RGB_ORDER_BRG,
    RGB_ORDER_BGR,
    RGB_ORDER_MAX
}lcm_rgb_order_e;
#endif
