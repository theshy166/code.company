
#include "mol_lcm_local.h"
#include "mol_lcm.h"

#define LCM_MCU16_ST7796S_DEBUG 0

#define DISPLAY_NONE 0
#define DISPLAY_SINGLE 1
#define DISPLAY_LOOP 2

#define DISPLAY_DEBUG DISPLAY_SINGLE

#define LCM_MCU16_ST7796S_WIDTH 320
#define LCM_MCU16_ST7796S_HEIGHT 480

const lcm_cfg_t s_lcm_mcu16_st7796s_info;

lcm_setting_tab_t s_lcm_mcu16_st7796s_setting[] = {
    {LCM_SETTING_CMD, 0x01},    //reset
    {LCM_DELAY, 5},

    {LCM_SETTING_CMD, 0xF0},
    {LCM_SETTING_DATA, 0xC3},

    {LCM_SETTING_CMD, 0xF0},
    {LCM_SETTING_DATA, 0x96},

    {LCM_SETTING_CMD, 0x36},
    {LCM_SETTING_DATA, 0x40},

    {LCM_SETTING_CMD, 0x3A},
    {LCM_SETTING_DATA, 0x05},   //16bits/pixel RGB565

    {LCM_SETTING_CMD, 0xB4},    //1-dot Inversion
    {LCM_SETTING_DATA, 0x01},

    {LCM_SETTING_CMD, 0xB1},
    {LCM_SETTING_DATA, 0x80},   //FRS[D7-D4], DIVA[D1-D0] 81 for 15Hz

    {LCM_SETTING_DATA, 0x10},   //RTNA[4:0]

    {LCM_SETTING_CMD, 0xB5},
    {LCM_SETTING_DATA, 0x1F},   //VFP FF for 15Hz

    {LCM_SETTING_DATA, 0x50},   //VBP FF for 15Hz
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, 0x20},   //HBP

    {LCM_SETTING_CMD, 0xB6},
    {LCM_SETTING_DATA, 0x8A},
    {LCM_SETTING_DATA, 0x07},
    {LCM_SETTING_DATA, 0x3B},   //480 Gates

    {LCM_SETTING_CMD, 0xC0},
    {LCM_SETTING_DATA, 0x80},
    {LCM_SETTING_DATA, 0x64},   //VGH=15V VGL=-10V

    {LCM_SETTING_CMD, 0xC1},
    {LCM_SETTING_DATA, 0x13},   //VOP=4.5V

    {LCM_SETTING_CMD, 0xC2},
    {LCM_SETTING_DATA, 0xA7},

    {LCM_SETTING_CMD, 0xC5},
    {LCM_SETTING_DATA, 0x09},

    {LCM_SETTING_CMD, 0xE8},
    {LCM_SETTING_DATA, 0x40},
    {LCM_SETTING_DATA, 0x8a},
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, 0x29},
    {LCM_SETTING_DATA, 0x19},
    {LCM_SETTING_DATA, 0xA5},
    {LCM_SETTING_DATA, 0x33},

    {LCM_SETTING_CMD, 0xE0},
    {LCM_SETTING_DATA, 0xF0},
    {LCM_SETTING_DATA, 0x06},
    {LCM_SETTING_DATA, 0x0B},
    {LCM_SETTING_DATA, 0x07},
    {LCM_SETTING_DATA, 0x06},
    {LCM_SETTING_DATA, 0x05},
    {LCM_SETTING_DATA, 0x2E},
    {LCM_SETTING_DATA, 0x33},
    {LCM_SETTING_DATA, 0x47},
    {LCM_SETTING_DATA, 0x3A},
    {LCM_SETTING_DATA, 0x17},
    {LCM_SETTING_DATA, 0x16},
    {LCM_SETTING_DATA, 0x2E},
    {LCM_SETTING_DATA, 0x31},

    {LCM_SETTING_CMD, 0xE1},
    {LCM_SETTING_DATA, 0xF0},
    {LCM_SETTING_DATA, 0x09},
    {LCM_SETTING_DATA, 0x0D},
    {LCM_SETTING_DATA, 0x09},
    {LCM_SETTING_DATA, 0x08},
    {LCM_SETTING_DATA, 0x23},
    {LCM_SETTING_DATA, 0x2E},
    {LCM_SETTING_DATA, 0x33},
    {LCM_SETTING_DATA, 0x46},
    {LCM_SETTING_DATA, 0x38},
    {LCM_SETTING_DATA, 0x13},
    {LCM_SETTING_DATA, 0x13},
    {LCM_SETTING_DATA, 0x2C},
    {LCM_SETTING_DATA, 0x32},

    {LCM_SETTING_CMD, 0xF0},
    {LCM_SETTING_DATA, 0x3C},

    {LCM_SETTING_CMD, 0xF0},
    {LCM_SETTING_DATA, 0x69},

    {LCM_SETTING_CMD, 0x2A},     //320
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, ((LCM_MCU16_ST7796S_WIDTH - 1)>>8)},
    {LCM_SETTING_DATA, ((LCM_MCU16_ST7796S_WIDTH - 1)&0xff)},

    {LCM_SETTING_CMD, 0x2B},     //480
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, ((LCM_MCU16_ST7796S_HEIGHT - 1)>>8)},
    {LCM_SETTING_DATA, ((LCM_MCU16_ST7796S_HEIGHT - 1)&0xff)},

    {LCM_SETTING_CMD, 0x35},
    {LCM_SETTING_DATA, 0x00},

    {LCM_SETTING_CMD, 0x11},
    {LCM_DELAY,120},             //ms
    {LCM_SETTING_CMD, 0x29},
    {LCM_DELAY,50},
    {LCM_SETTING_CMD, 0x21},
    {LCM_DELAY,50},
    {LCM_SETTING_CMD, 0X2C},     //Write Gram
    {LCM_END_OF_TABLE, LCM_END_OF_TABLE},     //list end flag
};

static int32_t _lcm_mcu16_st7796s_PowerOff(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    rtn = ops->bus_deinit((void *)param, (void *)&s_lcm_mcu16_st7796s_info);
    EXIT_IF_FAIL(rtn, NULL, "ops->bus_deinit ~_~!\n");

    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mcu16_st7796s_PowerOn(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    ops->bus_init((void *)param, (void *)&s_lcm_mcu16_st7796s_info);

    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mcu16_st7796s_identify(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t lcm_mcu16_st7796s_clear(const lcm_cfg_t *lcm_cfg, void * handle, uint32_t color)
{
    int32_t rtn = 0;
    mol_lcm_init_in *lcm_init = handle;
    lcm_mcu_fun *mcu = &lcm_init->ops.ctrl_fun.mcu;
    void *_handle = lcm_init->handle;
    uint32_t i = 0;

    for (i = 0; i < LCM_MCU16_ST7796S_WIDTH * LCM_MCU16_ST7796S_HEIGHT; i++){
        rtn = mcu->write_data(_handle, color);
        EXIT_IF_FAIL(rtn, NULL, "mcu->write_data\n");
    }

    EXIT:

    return rtn;
}

static int32_t _lcm_mcu16_st7796s_init(void * param,lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t * handle = (lcm_handle_t *)param;
    mol_lcm_init_in *lcm_init = &handle->lcm_init;
    lcm_mcu_fun *mcu = &ops->ctrl_fun.mcu;
    void *_handle = lcm_init->handle;
    uint32_t i = 0;

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    //rtn = _lcm_mcu16_st7796s_PowerOn(param, ops);

    rtn = mcu->push_tab((void *)_handle, (void *)&s_lcm_mcu16_st7796s_setting, sizeof(s_lcm_mcu16_st7796s_setting)/sizeof(lcm_setting_tab_t));
    EXIT_IF_FAIL(rtn, NULL, "mcu->push_tab\n");

    if (DISPLAY_LOOP == DISPLAY_DEBUG) {
        while (1) {
            if (0 == (i%3)) {
                lcm_mcu16_st7796s_clear((void *)&s_lcm_mcu16_st7796s_info, lcm_init, 0xf800);
            }
            if (1 == (i%3)){
                lcm_mcu16_st7796s_clear((void *)&s_lcm_mcu16_st7796s_info, lcm_init, 0x07e0);
            }
            if (2 == (i%3)) {
                lcm_mcu16_st7796s_clear((void *)&s_lcm_mcu16_st7796s_info, lcm_init, 0x001f);
            }
            i++;
        }
    } else if (DISPLAY_SINGLE == DISPLAY_DEBUG) {
        lcm_mcu16_st7796s_clear((void *)&s_lcm_mcu16_st7796s_info, lcm_init, 0xf800); // blue
        lcm_mcu16_st7796s_clear((void *)&s_lcm_mcu16_st7796s_info, lcm_init, 0x07e0); // green
        lcm_mcu16_st7796s_clear((void *)&s_lcm_mcu16_st7796s_info, lcm_init, 0x001f); // read
    }

    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mcu16_st7796s_suspend(void * param,lcm_ops *ops)
{
    int32_t rtn = 0;

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mcu16_st7796s_resume(void * param,lcm_ops *ops)
{
    int32_t rtn = 0;

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mcu16_st7796s_deinit(void * param,lcm_ops *ops)
{
    int32_t rtn = 0;

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcm_mcu16_st7796s_PowerOff(param, ops);
    EXIT_IF_FAIL(rtn, NULL, "_lcm_mcu16_st7796s_PowerOff ~_~!\n");

    EXIT:

    #if LCM_MCU16_ST7796S_DEBUG
    ops->debug_fun(LCM_MCU16_ST7796S_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun _s_lcm_mcu16_st7796s_ctrl =
{
    .poweron = _lcm_mcu16_st7796s_PowerOn,
    .poweroff = _lcm_mcu16_st7796s_PowerOff,
    .identify = _lcm_mcu16_st7796s_identify,
    .init = _lcm_mcu16_st7796s_init,
    .suspend = _lcm_mcu16_st7796s_suspend,
    .resume = _lcm_mcu16_st7796s_resume,
    .deinit = _lcm_mcu16_st7796s_deinit,
};

static lcm_display_sync_info_t _s_lcm_mcu16_st7796s_dp_info =
{
    .hor_sync_active = 30,
    .hor_back_porch = 30,
    .hor_active_pixel = LCM_MCU16_ST7796S_WIDTH,
    .hor_front_porch = 38,

    .ver_sync_active = 20,//4,
    .ver_back_porch = 4,
    .ver_active_line = LCM_MCU16_ST7796S_HEIGHT,
    .ver_front_porch = 8,
};

lcm_parallel_bus_timing _s_lcm_mcu16_st7796s_bus_timing =
{
    .csType = 0,
    .rdSetup = 1,
    .rdHold = 1,
    .rdDestory = 1,
    .wrSetup = 1,
    .wrHold = 1,
    .wrDestory = 1,
};

lcm_mcu_clk_t _s_lcm_mcu16_st7796s_clk =
{
    .vou.mode = 1,
    .vou.sel = 0x3,
    .vou.div = 0,

    .mif.mode = 1,
    .mif.pll_doubler = 0,
    .mif.pll_div_s = 1,
    .mif.pll_n = 0,
    .mif.pll_kint = 0x10624,
    .mif.pll_nint = 0x2b,
    .mif.pixelclk_div = 14,
    .mif.pll_pdiv = 0,
    .mif.dhd_div = 1,
};

static lcm_mcu_info _s_lcm_mcu16_st7796s_info =
{
    .format = LCM_RGB_565,
    .if_type = MCU_IF_16BITS,
    .trans = MCU_HSB_FIRST,
    .order = RGB_ORDER_BGR,
    .dither = 0,
    .bitswap = 0,
    .rd_eb = 0,
    .te = {
        .eb = 0,
        .mode = 0,
        .edge = 0,
    },
    .timing = &_s_lcm_mcu16_st7796s_bus_timing,
    .clk = &_s_lcm_mcu16_st7796s_clk,
};

const lcm_cfg_t s_lcm_mcu16_st7796s_info =
{
    .verder_name = VENDER_NAME,
    .name = "st7796s_mcu16_320_480_8bits",
    .width = LCM_MCU16_ST7796S_WIDTH,
    .height = LCM_MCU16_ST7796S_HEIGHT,
    .type = LCM_TYPE_MCU, /*mcu, rgb, mipi*/
    .fps = 60,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &_s_lcm_mcu16_st7796s_dp_info,
    .infor = {
        .mcu = &_s_lcm_mcu16_st7796s_info,
    },
    .fun = &_s_lcm_mcu16_st7796s_ctrl,
};
