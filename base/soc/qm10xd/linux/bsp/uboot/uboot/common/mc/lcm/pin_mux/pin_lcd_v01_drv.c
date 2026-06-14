#include "../reg/reg.h"
#include "../reg/reg_pool.h"
#include "pin_lcd_v01_drv.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct {
    uint32_t id;
    uint32_t reg_offset;
    uint32_t val;
    uint32_t default_val;
}pin_fun_cfg;

typedef struct {
    description_t description;
    pin_fun_cfg *pin_table;
}pin_fun_list_t;

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
pin_fun_cfg s_gpio_pin_table[] = {
    {0 , 0x0004, 0x1, 0x00}, //GPIO0_0
    {1 , 0x0008, 0x1, 0x00}, //GPIO0_1
    {2 , 0x000C, 0x1, 0x00}, //GPIO0_2
    {3 , 0x0010, 0x1, 0x00}, //GPIO0_3
    {4 , 0x0014, 0x1, 0x00}, //GPIO0_4
    {5 , 0x0018, 0x1, 0x00}, //GPIO0_5
    {6 , 0x001C, 0x1, 0x00}, //GPIO0_6
    {7 , 0x0020, 0x1, 0x00}, //GPIO0_7
    {8 , 0x0024, 0x1, 0x00}, //GPIO1_0
    {9 , 0x0028, 0x1, 0x00}, //GPIO1_1
    {10, 0x002C, 0x1, 0x00}, //GPIO1_2
    {11, 0x0030, 0x1, 0x00}, //GPIO1_3
    {12, 0x0034, 0x1, 0x00}, //GPIO1_4
    {13, 0x0038, 0x1, 0x00}, //GPIO1_5
    {14, 0x003C, 0x1, 0x00}, //GPIO1_6
    {15, 0x0048, 0x1, 0x00}, //GPIO1_7
    {16, 0x0054, 0x1, 0x00}, //GPIO2_0
    {17, 0x0058, 0x1, 0x00}, //GPIO2_1
    {18, 0x006C, 0x1, 0x00}, //GPIO2_2
    {19, 0x0070, 0x1, 0x00}, //GPIO2_3
    {20, 0x0078, 0x1, 0x00}, //GPIO2_4
    {21, 0x007C, 0x1, 0x00}, //GPIO2_5
    {22, 0x0080, 0x1, 0x00}, //GPIO2_6
    {23, 0x0084, 0x1, 0x00}, //GPIO2_7
    {24, 0x0088, 0x1, 0x00}, //GPIO3_0
    {25, 0x008C, 0x1, 0x00}, //GPIO3_1
    {26, 0x0090, 0x1, 0x00}, //GPIO3_2
    {27, 0x0094, 0x1, 0x00}, //GPIO3_3
    {28, 0x0098, 0x1, 0x00}, //GPIO3_4
    {29, 0x00C4, 0x1, 0x00}, //GPIO3_5
    {30, 0x00C8, 0x1, 0x00}, //GPIO3_6
    {31, 0x00CC, 0x1, 0x00}, //GPIO3_7
    {32, 0x00D0, 0x1, 0x00}, //GPIO4_0
    {33, 0x00D4, 0x1, 0x00}, //GPIO4_1
    {34, 0x00D8, 0x1, 0x00}, //GPIO4_2
    {35, 0x00DC, 0x1, 0x00}, //GPIO4_3
    {36, 0x00E0, 0x1, 0x00}, //GPIO4_4
    {37, 0x00E4, 0x1, 0x00}, //GPIO4_5
    {38, 0x00E8, 0x1, 0x00}, //GPIO4_6
    {39, 0x00EC, 0x1, 0x00}, //GPIO4_7
    {40, 0x00F0, 0x1, 0x00}, //GPIO5_0
    {41, 0x00F4, 0x1, 0x00}, //GPIO5_1
    {42, 0x00F8, 0x1, 0x00}, //GPIO5_2
    {43, 0x00FC, 0x1, 0x00}, //GPIO5_3
    {44, 0x0100, 0x1, 0x00}, //GPIO5_4
    {45, 0x0104, 0x1, 0x00}, //GPIO5_5
    {46, 0x0108, 0x1, 0x00}, //GPIO5_6
    {47, 0x009C, 0x1, 0x00}, //GPIO5_7
    {48, 0x00A0, 0x1, 0x00}, //GPIO6_0
    {49, 0x00A4, 0x1, 0x00}, //GPIO6_1
    {50, 0x00A8, 0x1, 0x00}, //GPIO6_2
    {51, 0x00AC, 0x1, 0x00}, //GPIO6_3
    {52, 0x00B0, 0x1, 0x00}, //GPIO6_4
    {53, 0x00B4, 0x1, 0x00}, //GPIO6_5
    {54, 0x00B8, 0x1, 0x00}, //GPIO6_6
    {55, 0x00BC, 0x1, 0x00}, //GPIO6_7
    {56, 0x00C0, 0x1, 0x00}, //GPIO7_0
    {57, 0x0130, 0x1, 0x00}, //GPIO7_1
    {58, 0x0134, 0x1, 0x00}, //GPIO7_2
    {59, 0x010C, 0x1, 0x00}, //GPIO7_3
    {60, 0x0110, 0x1, 0x00}, //GPIO7_4
    {61, 0x0114, 0x1, 0x00}, //GPIO7_5
    {62, 0x0118, 0x1, 0x00}, //GPIO7_6
    {63, 0x0124, 0x1, 0x00}, //GPIO7_7
    {64, 0x0128, 0x1, 0x00}, //GPIO8_0
    {65, 0x012C, 0x1, 0x00}, //GPIO8_1
    {66, 0x0138, 0x1, 0x00}, //GPIO8_2
    {67, 0x0164, 0x1, 0x00}, //GPIO8_3
    {68, 0x0168, 0x1, 0x00}, //GPIO8_4
    {69, 0x016C, 0x1, 0x00}, //GPIO8_5
    {70, 0x0170, 0x1, 0x00}, //GPIO8_6
    {71, 0x0174, 0x1, 0x00}, //GPIO8_7
    {72, 0x0178, 0x1, 0x00}, //GPIO9_0
    {73, 0x017C, 0x0, 0x00}, //GPIO9_1
    {74, 0x0180, 0x0, 0x00}, //GPIO9_2
    {75, 0x0184, 0x0, 0x00}, //GPIO9_3
    {76, 0x0188, 0x0, 0x00}, //GPIO9_4
    {77, 0x018C, 0x0, 0x00}, //GPIO9_5
    {78, 0x0190, 0x0, 0x00}, //GPIO9_6
    {79, 0x0194, 0x0, 0x00}, //GPIO9_7
    {80, 0x0198, 0x0, 0x00}, //GPIO10_0
    {81, 0x019C, 0x0, 0x00}, //GPIO10_1
    {82, 0x01A0, 0x0, 0x00}, //GPIO10_2
    {83, 0x01A4, 0x0, 0x00}, //GPIO10_3
    {84, 0x01A8, 0x0, 0x00}, //GPIO10_4
    {85, 0x01AC, 0x0, 0x00}, //GPIO10_5
    {86, 0x011C, 0x1, 0x00}, //GPIO10_6
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_rgb_24_pin_table[] = {
    {0x00, 0x64, 0x00, 0x00},   //LCD_DE
    {0x00, 0x68, 0x00, 0x00},   //LCD_HSYNC
    {0x00, 0x6c, 0x00, 0x00},   //LCD_VSYNC
    {0x00, 0x70, 0x00, 0x00},   //LCD_CLK
    {0x00, 0x74, 0x00, 0x00},   //LCD_D0
    {0x00, 0x78, 0x00, 0x00},   //LCD_D1
    {0x00, 0x7c, 0x00, 0x00},   //LCD_D2
    {0x00, 0x80, 0x00, 0x00},   //LCD_D4
    {0x00, 0x84, 0x00, 0x00},   //LCD_D3
    {0x00, 0x88, 0x00, 0x00},   //LCD_D5
    {0x00, 0x8c, 0x00, 0x00},   //LCD_D7
    {0x00, 0x90, 0x00, 0x00},   //LCD_D6
    {0x00, 0x94, 0x00, 0x00},   //LCD_D9
    {0x00, 0x98, 0x00, 0x00},   //LCD_D8
    {0x00, 0x9c, 0x00, 0x00},   //LCD_D10
    {0x00, 0xa0, 0x00, 0x00},   //LCD_D11
    {0x00, 0xa4, 0x00, 0x00},   //LCD_D12
    {0x00, 0xa8, 0x00, 0x00},   //LCD_D13
    {0x00, 0xac, 0x00, 0x00},   //LCD_D14
    {0x00, 0xb0, 0x00, 0x00},   //LCD_D15
    {0x00, 0xb4, 0x00, 0x00},   //LCD_D16
    {0x00, 0xb8, 0x00, 0x00},   //LCD_D17
    {0x00, 0xbc, 0x02, 0x00},   //LCD_D21
    {0x00, 0xc0, 0x02, 0x00},   //LCD_D20
    {0x00, 0xc4, 0x02, 0x00},   //LCD_D23
    {0x00, 0xc8, 0x02, 0x00},   //LCD_D22
    {0x00, 0xcc, 0x02, 0x00},   //LCD_D19
    {0x00, 0xd0, 0x02, 0x00},   //LCD_D18
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_rgb_18_pin_table[] = {
    {0x00, 0x64, 0x00, 0x00},   //LCD_DE
    {0x00, 0x68, 0x00, 0x00},   //LCD_HSYNC
    {0x00, 0x6c, 0x00, 0x00},   //LCD_VSYNC
    {0x00, 0x70, 0x00, 0x00},   //LCD_CLK
    {0x00, 0x74, 0x00, 0x00},   //LCD_D0
    {0x00, 0x78, 0x00, 0x00},   //LCD_D1
    {0x00, 0x7c, 0x00, 0x00},   //LCD_D2
    {0x00, 0x80, 0x00, 0x00},   //LCD_D4
    {0x00, 0x84, 0x00, 0x00},   //LCD_D3
    {0x00, 0x88, 0x00, 0x00},   //LCD_D5
    {0x00, 0x8c, 0x00, 0x00},   //LCD_D7
    {0x00, 0x90, 0x00, 0x00},   //LCD_D6
    {0x00, 0x94, 0x00, 0x00},   //LCD_D9
    {0x00, 0x98, 0x00, 0x00},   //LCD_D8
    {0x00, 0x9c, 0x00, 0x00},   //LCD_D10
    {0x00, 0xa0, 0x00, 0x00},   //LCD_D11
    {0x00, 0xa4, 0x00, 0x00},   //LCD_D12
    {0x00, 0xa8, 0x00, 0x00},   //LCD_D13
    {0x00, 0xac, 0x00, 0x00},   //LCD_D14
    {0x00, 0xb0, 0x00, 0x00},   //LCD_D15
    {0x00, 0xb4, 0x00, 0x00},   //LCD_D16
    {0x00, 0xb8, 0x00, 0x00},   //LCD_D17
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_rgb_16_pin_table[] = {
    {0x00, 0x64, 0x00, 0x00},   //LCD_DE
    {0x00, 0x68, 0x00, 0x00},   //LCD_HSYNC
    {0x00, 0x6c, 0x00, 0x00},   //LCD_VSYNC
    {0x00, 0x70, 0x00, 0x00},   //LCD_CLK
    {0x00, 0x74, 0x00, 0x00},   //LCD_D0
    {0x00, 0x78, 0x00, 0x00},   //LCD_D1
    {0x00, 0x7c, 0x00, 0x00},   //LCD_D2
    {0x00, 0x80, 0x00, 0x00},   //LCD_D4
    {0x00, 0x84, 0x00, 0x00},   //LCD_D3
    {0x00, 0x88, 0x00, 0x00},   //LCD_D5
    {0x00, 0x8c, 0x00, 0x00},   //LCD_D7
    {0x00, 0x90, 0x00, 0x00},   //LCD_D6
    {0x00, 0x94, 0x00, 0x00},   //LCD_D9
    {0x00, 0x98, 0x00, 0x00},   //LCD_D8
    {0x00, 0x9c, 0x00, 0x00},   //LCD_D10
    {0x00, 0xa0, 0x00, 0x00},   //LCD_D11
    {0x00, 0xa4, 0x00, 0x00},   //LCD_D12
    {0x00, 0xa8, 0x00, 0x00},   //LCD_D13
    {0x00, 0xac, 0x00, 0x00},   //LCD_D14
    {0x00, 0xb0, 0x00, 0x00},   //LCD_D15
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_rgb_6_pin_table[] = {
    {0x00, 0x64, 0x00, 0x00},   //LCD_DE
    {0x00, 0x68, 0x00, 0x00},   //LCD_HSYNC
    {0x00, 0x6c, 0x00, 0x00},   //LCD_VSYNC
    {0x00, 0x70, 0x00, 0x00},   //LCD_CLK
    {0x00, 0x74, 0x00, 0x00},   //LCD_D0
    {0x00, 0x78, 0x00, 0x00},   //LCD_D1
    {0x00, 0x7c, 0x00, 0x00},   //LCD_D2
    {0x00, 0x80, 0x00, 0x00},   //LCD_D4
    {0x00, 0x84, 0x00, 0x00},   //LCD_D3
    {0x00, 0x88, 0x00, 0x00},   //LCD_D5
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_mcu_18_pin_table[] = {
    {0x00, 0x64, 0x02, 0x00},    //LCD_TE
    {0x00, 0x68, 0x02, 0x00},    //LCD_CSN
    {0x00, 0x6c, 0x02, 0x00},    //LCD_RS
    {0x00, 0x70, 0x02, 0x00},    //LCD_WRN
    {0x00, 0x74, 0x00, 0x00},    //LCD_D0
    {0x00, 0x78, 0x00, 0x00},    //LCD_D1
    {0x00, 0x7c, 0x00, 0x00},    //LCD_D2
    {0x00, 0x80, 0x00, 0x00},    //LCD_D4
    {0x00, 0x84, 0x00, 0x00},    //LCD_D3
    {0x00, 0x88, 0x00, 0x00},    //LCD_D5
    {0x00, 0x8c, 0x00, 0x00},    //LCD_D7
    {0x00, 0x90, 0x00, 0x00},    //LCD_D6
    {0x00, 0x94, 0x00, 0x00},    //LCD_D9
    {0x00, 0x98, 0x00, 0x00},    //LCD_D8
    {0x00, 0x9c, 0x00, 0x00},    //LCD_D10
    {0x00, 0xa0, 0x00, 0x00},    //LCD_D11
    {0x00, 0xa4, 0x00, 0x00},    //LCD_D12
    {0x00, 0xa8, 0x00, 0x00},    //LCD_D13
    {0x00, 0xac, 0x00, 0x00},    //LCD_D14
    {0x00, 0xb0, 0x00, 0x00},    //LCD_D15
    {0x00, 0xb4, 0x00, 0x00},    //LCD_D16
    {0x00, 0xb8, 0x00, 0x00},    //LCD_D17
    {0x00, 0xcc, 0x03, 0x00},    //LCD_RDN
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_mipi_1c4d_pin_table[] = {
    {0x00, 0xbc, 0x00, 0x00},    //DSI_DP3
    {0x00, 0xc0, 0x00, 0x00},    //DSI_DN3
    {0x00, 0xc4, 0x00, 0x00},    //DSI_DP2
    {0x00, 0xc8, 0x00, 0x00},    //DSI_DN2
    {0x00, 0xcc, 0x00, 0x00},    //DSI_CKP1
    {0x00, 0xd0, 0x00, 0x00},    //DSI_CKN1
    {0x00, 0xd4, 0x00, 0x00},    //DSI_CKN0
    {0x00, 0xd8, 0x00, 0x00},    //DSI_CKP0
    {0x00, 0xdc, 0x00, 0x00},    //DSI_DN1
    {0x00, 0xe0, 0x00, 0x00},    //DSI_DP1
    {0x00, 0xe4, 0x00, 0x00},    //DSI_DN0
    {0x00, 0xe8, 0x00, 0x00},    //DSI_DP0
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_mipi0_1c2d_pin_table[] = {
    {0x00, 0xd4, 0x00, 0x00},    //DSI_CKN0
    {0x00, 0xd8, 0x00, 0x00},    //DSI_CKP0
    {0x00, 0xdc, 0x00, 0x00},    //DSI_DN1
    {0x00, 0xe0, 0x00, 0x00},    //DSI_DP1
    {0x00, 0xe4, 0x00, 0x00},    //DSI_DN0
    {0x00, 0xe8, 0x00, 0x00},    //DSI_DP0
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg s_mipi1_1c2d_pin_table[] = {
    {0x00, 0xbc, 0x00, 0x00},    //DSI_DP3
    {0x00, 0xc0, 0x00, 0x00},    //DSI_DN3
    {0x00, 0xc4, 0x00, 0x00},    //DSI_DP2
    {0x00, 0xc8, 0x00, 0x00},    //DSI_DN2
    {0x00, 0xcc, 0x00, 0x00},    //DSI_CKP1
    {0x00, 0xd0, 0x00, 0x00},    //DSI_CKN1
    {MODULE_INVALID, MODULE_INVALID, MODULE_INVALID, MODULE_INVALID}
};

pin_fun_cfg *s_lcm_pin_table[] = {
    s_rgb_24_pin_table,       //lcm_pin_rgb_24
    s_rgb_18_pin_table,       //lcm_pin_rgb_18
    s_rgb_16_pin_table,       //lcm_pin_rgb_16
    s_rgb_6_pin_table,       //lcm_pin_rgb_6
    s_mcu_18_pin_table,       //lcm_pin_mcu_18
    s_mipi_1c4d_pin_table,    //lcm_pin_mipi_1c4d
    s_mipi0_1c2d_pin_table,   //lcm_pin_mipi0_1c2d
    s_mipi1_1c2d_pin_table,   //lcm_pin_mipi1_1c2d
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
static pin_fun_cfg *_get_lcm_pin_ptr(lcm_pin_type_e lcm_pin_type)
{
    int32_t rtn = SUCCESS;
    pin_fun_cfg *pin_cfg_ptr = NULL;

    CHECK_ENUM(lcm_pin_type, lcm_pin_type_max, NULL, "lcm_pin_type invali ~_~!\n");

    pin_cfg_ptr = s_lcm_pin_table[lcm_pin_type];
    CHECK_PTR(pin_cfg_ptr, NULL, "pin_cfg_ptr is NULL ~_~!\n");

    EXIT:
    if (rtn != SUCCESS) {
        //
    }

    return pin_cfg_ptr;
}

static pin_fun_cfg *_get_gpio_cfg_ptr(uint32_t gpio_id)
{
    int32_t rtn = SUCCESS;
    pin_fun_cfg *pin_cfg_ptr = s_gpio_pin_table;
    pin_fun_cfg *gpio_pin_cfg_ptr = NULL;

    CHECK_PTR(pin_cfg_ptr, NULL, "pin_cfg_ptr is NULL ~_~!\n");

    while(0xffffffff != pin_cfg_ptr->id) {
        if (gpio_id == pin_cfg_ptr->id) {
            gpio_pin_cfg_ptr = pin_cfg_ptr;
            break;
        }
        pin_cfg_ptr++;
    };

    EXIT:
    if (rtn != SUCCESS) {
        //
    }

    return gpio_pin_cfg_ptr;
}

static int32_t _lcm_bus_pin_default_get(lcm_pin_type_e lcm_pin_type, ADDR base_addr)
{
    int32_t rtn = SUCCESS;
    pin_fun_cfg *lcm_pin_ptr = _get_lcm_pin_ptr(lcm_pin_type);

    CHECK_PTR(lcm_pin_ptr, NULL, "lcm_pin_ptr is NULL ~_~!\n");

    do {
        if ((MODULE_INVALID == lcm_pin_ptr->id)
            && (MODULE_INVALID == lcm_pin_ptr->reg_offset)) {
            break;
        }
        lcm_pin_ptr->default_val = regRead32(base_addr + lcm_pin_ptr->reg_offset);
    }while(lcm_pin_ptr++);

    EXIT:

    return rtn;
}

static int32_t _lcm_gpio_pin_default_get(ADDR base_addr, uint32_t gpio_id)
{
    int32_t rtn = 0;
    pin_fun_cfg *gpio_pin_cfg = NULL;

    gpio_pin_cfg = _get_gpio_cfg_ptr( gpio_id);
    CHECK_PTR(gpio_pin_cfg, NULL, "gpio_pin_cfg null ~_~!\n");

    gpio_pin_cfg->val = regRead32(base_addr + gpio_pin_cfg->reg_offset);

    EXIT:

    return rtn;
}


int32_t lcm_bus_pin_cfg(ADDR base_addr, lcm_pin_type_e lcm_pin_type)
{
    int32_t rtn = SUCCESS;
    pin_fun_cfg *lcm_pin_ptr = _get_lcm_pin_ptr(lcm_pin_type);

    CHECK_PTR(lcm_pin_ptr, NULL, "lcm_pin_ptr is NULL ~_~!\n");

    rtn = _lcm_bus_pin_default_get(lcm_pin_type, base_addr);

    do {
        if ((MODULE_INVALID == lcm_pin_ptr->id)
            && (MODULE_INVALID == lcm_pin_ptr->reg_offset)) {
            break;
        }
        regWrite32(base_addr + lcm_pin_ptr->reg_offset, lcm_pin_ptr->val);
    }while(lcm_pin_ptr++);

    EXIT:

    return rtn;
}

int32_t lcm_bus_pin_default_cfg(ADDR base_addr, lcm_pin_type_e lcm_pin_type)
{
    int32_t rtn = SUCCESS;
    pin_fun_cfg *lcm_pin_ptr = _get_lcm_pin_ptr(lcm_pin_type);

    CHECK_PTR(lcm_pin_ptr, NULL, "lcm_pin_ptr is NULL ~_~!\n");

    do {
        if ((MODULE_INVALID == lcm_pin_ptr->id)
            && (MODULE_INVALID == lcm_pin_ptr->reg_offset)) {
            break;
        }
        regWrite32(base_addr + lcm_pin_ptr->reg_offset, lcm_pin_ptr->default_val);
    }while(lcm_pin_ptr++);

    EXIT:

    return rtn;
}

int32_t lcm_gpio_pin_cfg(ADDR base_addr, uint32_t gpio_id)
{
    int32_t rtn = 0;
    pin_fun_cfg *gpio_pin_cfg = NULL;

    rtn = _lcm_gpio_pin_default_get(base_addr, gpio_id);
    EXIT_IF_FAIL(rtn, NULL, "_lcm_gpio_pin_default_get ~_~!\n");

    gpio_pin_cfg = _get_gpio_cfg_ptr( gpio_id);
    CHECK_PTR(gpio_pin_cfg, NULL, "gpio_pin_cfg null ~_~!\n");

    regWrite32(base_addr + gpio_pin_cfg->reg_offset, gpio_pin_cfg->val);

    EXIT:

    return rtn;
}

int32_t lcm_gpio_default_cfg(ADDR base_addr, uint32_t gpio_id)
{
    int32_t rtn = 0;
    pin_fun_cfg *gpio_pin_cfg = NULL;

    gpio_pin_cfg = _get_gpio_cfg_ptr( gpio_id);
    CHECK_PTR(gpio_pin_cfg, NULL, "gpio_pin_cfg null ~_~!\n");

    regWrite32(base_addr + gpio_pin_cfg->reg_offset, gpio_pin_cfg->default_val);

    EXIT:

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

