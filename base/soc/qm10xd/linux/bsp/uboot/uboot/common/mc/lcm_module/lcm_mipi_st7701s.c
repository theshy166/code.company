#include "include/mol_lcm_local.h"
#include "include/dsi_com_param.h"
#include "include/mol_lcm.h"

#define LCM_ST7701S_DEBUG 0

#define LCM_ST7701S_WIDTH 480
#define LCM_ST7701S_HEIGHT 480

const lcm_cfg_t s_lcm_mipi_st7701s_info;

static dsi_setting_tab_t s_lcm_mipi_st7701s_init_code[] = {
    /* Command2, BK3 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x13}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xEF,0x08}},
    /* Command2, BK0 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x10}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xC0,0x3B,0x00}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xC1,0x0D,0x0C}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xC2,0x21,0x08}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xCC,0x18}},
    {DSI_DI_DCS_WRITE_LONG,    0x13, {0x11,0x00, 0xB0,0x00,0x11,0x18,0x0E,0x11,0x06,0x07,0x08,0x07,0x22,0x04,0x12,0x0F,0xAA,0x31,0x18}},
    {DSI_DI_DCS_WRITE_LONG,    0x13, {0x11,0x00, 0xB1,0x00,0x11,0x19,0x0E,0x12,0x07,0x08,0x08,0x08,0x22,0x04,0x11,0x11,0xA9,0x32,0x18}},

    /* Command2, BK1 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x11}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB0,0x60}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB1,0x30}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB2,0x8A}},                       /* VGH:  15.00 v */
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB3,0x80}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB5,0x4B}},                       /* VGL:  -9.55 v */
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB7,0x85}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB8,0x21}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xC0,0x07}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xC1,0x78}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xC2,0x78}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xD0,0x88}},
    {DSI_DI_DCS_WRITE_LONG,    0x06, {0x04,0x00, 0xE0,0x00,0x1B,0x02}},
    /* --- */
    {DSI_DI_DCS_WRITE_LONG,    0x0E, {0x0C,0x00, 0xE1,0x08,0xA0,0x00,0x00,0x07,0xA0,0x00,0x00,0x00,0x44,0x44}},
    /* +++ */
    {DSI_DI_DCS_WRITE_LONG,    0x0F, {0x0D,0x00, 0xE2,0x11,0x11,0x44,0x44,0xED,0xA0,0x00,0x00,0xEC,0xA0,0x00,0x00}},
    {DSI_DI_DCS_WRITE_LONG,    0x07, {0x05,0x00, 0xE3,0x00,0x00,0x11,0x11}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xE4,0x44,0x44}},
    {DSI_DI_DCS_WRITE_LONG,    0x13, {0x11,0x00, 0xE5,0x0A,0xE9,0xD8,0xA0,0x0C,0xEB,0xD8,0xA0,0x0E,0xED,0xD8,0xA0,0x10,0xEF,0xD8,0xA0}},
    {DSI_DI_DCS_WRITE_LONG,    0x07, {0x05,0x00, 0xE6,0x00,0x00,0x11,0x11}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xE7,0x44,0x44}},
    {DSI_DI_DCS_WRITE_LONG,    0x13, {0x11,0x00, 0xE8,0x09,0xE8,0xD8,0xA0,0x0B,0xEA,0xD8,0xA0,0x0D,0xEC,0xD8,0xA0,0x0F,0xEE,0xD8,0xA0}},
    {DSI_DI_DCS_WRITE_LONG,    0x0A, {0x08,0x00, 0xEB,0x02,0x00,0xE4,0xE4,0x88,0x00,0x40}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xEC,0x3C,0x00}},
    {DSI_DI_DCS_WRITE_LONG,    0x13, {0x11,0x00, 0xED,0xAB,0x89,0x76,0x54,0x02,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x20,0x45,0x67,0x98,0xBA}},
    {DSI_DI_DCS_WRITE_LONG,    0x09, {0x07,0x00, 0xEF,0x08,0x08,0x08,0x45,0x3F,0x54}},
    /* Command2, BK3 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x13}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xE8,0x00,0x0E}},
    /* Command2, BK0 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x00}},
    // sleep out
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x11}},
    {DSI_DI_DELAY,              120, {0x00}},
    /* Command2, BK3 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x13}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xE8,0x00,0x0C}},
    {DSI_DI_DELAY,               10, {0x00}},
    {DSI_DI_DCS_WRITE_LONG,    0x05, {0x03,0x00, 0xE8,0x00,0x00}},
    /* Command2, BK0 */
    {DSI_DI_DCS_WRITE_LONG,    0x08, {0x06,0x00, 0xFF,0x77,0x01,0x00,0x00,0x00}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x3A,0x50}},
    {DSI_DI_END_OF_TABLE,      0x00, {0}}
};

static dsi_setting_tab_t s_lcm_mipi_st7701s_work_code[] = {
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x36,0x00}},           /* 0x00: RGB  0x08: BGR */
    // display on
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x29}},
    {DSI_DI_DELAY,               70, {0x00}},
    {DSI_DI_END_OF_TABLE,      0x00, {0}}
};

static dsi_setting_tab_t s_lcm_mipi_st7701s_reset_code[] = {
    // sw reset
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x01}},
    {DSI_DI_DELAY,                5, {0x00}},
    {DSI_DI_END_OF_TABLE,      0x00, {0}}
};

static int32_t _lcm_mipi_st7701s_PowerOn(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    //lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_ST7701S_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #endif

    ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&s_lcm_mipi_st7701s_info);

    return rtn;
}

static int32_t _lcm_mipi_st7701s_PowerOff(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_ST7701S_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #endif

    ops->power.reset(lcmHandle->lcm_id, 0);
    ops->delayMs(10);

    ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&s_lcm_mipi_st7701s_info);

    return rtn;
}

static int32_t _lcm_mipi_st7701s_identify(void * param, lcm_ops *ops)
{
#define MIPI_ST7701S_480_ID0_ADDR (0xA1)
#define MIPI_ST7701S_480_ID1_ADDR (0xA8)
#define MIPI_ST7701S_480_ID0 (0x88)
#define MIPI_ST7701S_480_ID1 (0x02)
    int32_t rtn = SUCCESS;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;
    uint8_t read_data[2] = {0};

    #if LCM_ST7701S_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "inParam ptr is NULL ~_~!\n");
    CHECK_PTR(ops, NULL, "ops ptr is NULL ~_~!\n");

    goto EXIT;

    read_data[0] = 0xff;
    read_data[1] = 0xff;

    rtn = ops->ctrl_fun.mipi.dcs_read(lcmHandle->lcm_id, DSI_DI_DCS_READ_0_PARAM, MIPI_ST7701S_480_ID0_ADDR, 1, &read_data[0]);
    rtn = ops->ctrl_fun.mipi.dcs_read(lcmHandle->lcm_id, DSI_DI_DCS_READ_0_PARAM, MIPI_ST7701S_480_ID1_ADDR, 1, &read_data[1]);

    if ((MIPI_ST7701S_480_ID0 != read_data[0])
        || (MIPI_ST7701S_480_ID1 != read_data[1])) {
        rtn = FAIL_ID_NO_MATCH;
    }

    printf("st7701s_2lane id : 0x%x:0x%x, 0x%x:0x%x", MIPI_ST7701S_480_ID0, MIPI_ST7701S_480_ID1, read_data[0],read_data[1]);

    EXIT:

    #if LCM_ST7701S_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_st7701s_init(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_ST7701S_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #endif

    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, s_lcm_mipi_st7701s_reset_code, ARRAY_SIZE(s_lcm_mipi_st7701s_reset_code));
    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, s_lcm_mipi_st7701s_init_code, ARRAY_SIZE(s_lcm_mipi_st7701s_init_code));
    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, s_lcm_mipi_st7701s_work_code, ARRAY_SIZE(s_lcm_mipi_st7701s_work_code));

    //rtn = ops->power.back_light_gpio(lcmHandle->lcm_id, 1);

    #if LCM_ST7701S_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_st7701s_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_ST7701S_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #endif

     rtn = ops->power.back_light_gpio(lcmHandle->lcm_id, 0);

    #if LCM_ST7701S_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_st7701s_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_ST7701S_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #endif

     rtn = ops->power.back_light_gpio(lcmHandle->lcm_id, 1);

    #if LCM_ST7701S_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_st7701s_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_ST7701S_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #endif

    rtn = ops->power.back_light_gpio(lcmHandle->lcm_id, 0);
    rtn = _lcm_mipi_st7701s_PowerOff(param, ops);

    #if LCM_ST7701S_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun s_lcm_mipi_st7701s_ctrl =
{
    .poweron = _lcm_mipi_st7701s_PowerOn,
    .poweroff = _lcm_mipi_st7701s_PowerOff,
    .identify = _lcm_mipi_st7701s_identify,
    .init = _lcm_mipi_st7701s_init,
    .suspend = _lcm_mipi_st7701s_suspend,
    .resume = _lcm_mipi_st7701s_resume,
    .deinit = _lcm_mipi_st7701s_deinit,
};

static lcm_display_sync_info_t s_lcm_mipi_st7701s_dp_info =
{
    .hor_sync_active = 20,
    .hor_back_porch = 180,
    .hor_active_pixel = LCM_ST7701S_WIDTH,
    .hor_front_porch = 180,

    .ver_sync_active = 8,
    .ver_back_porch = 30,
    .ver_active_line = LCM_ST7701S_HEIGHT,
    .ver_front_porch = 30,
};


static dsi_timing_t s_lcm_mipi_st7701s_timing =
{
    /*clk lane timing cfg*/
    .clane.mode = 1,
    .clane.lp11 = 0,
    .clane.lp01 = 0,
    .clane.zero = 13,
    .clane.prepare = 2,
    .clane.pre = 3,
    .clane.post = 10,
    .clane.trail = 3,
    .clane.inittime = 6,
    .clane.exit = 6,
    /*data lane timing cfg*/
    .dlane.mode = 1,
    .dlane.lp11 = 0,
    .dlane.lp01 = 6,    //dlane lpx
    .dlane.prepare = 3,
    .dlane.zero = 4,
    .dlane.trail = 4,
    .dlane.inittime = 6,
    .dlane.exit = 6,
    /*data lane0 read timing cfg*/
    .read.mode = 1,
    .read.tago = 27,
    .read.tasure = 10,
    .read.taget = 34,
    /*clk cfg*/
    .clk = {
        .vou.mode = 1,
        .vou.sel = 0x3,
        .vou.div = 0,

        .mif.mode = 1,
        .mif.pll_doubler = 0,
        .mif.pll_div_s = 1,
        .mif.pll_n = 0,
        .mif.pll_kint = 0x46dc5d,
        .mif.pll_nint = 0x38,
        .mif.pixelclk_div = 0,
        .mif.pll_pdiv = 2,
        .mif.dhd_div = 2,
    }
};

static lcm_mipi_info s_lcm_mipi_st7701s_mipi_info =
{
    .work_mode = DSI_VIDEO_MODE,
    .lane_num = DSI_TWO_LANE,
    .format = DSI_RGB888,
    .packet_size = 256,
    .timing = (void *)&s_lcm_mipi_st7701s_timing,
};

const lcm_cfg_t s_lcm_mipi_st7701s_info =
{
    .verder_name = VENDER_NAME,
    .name = "st7701s_2lane_480_480",
    .width = LCM_ST7701S_WIDTH,
    .height = LCM_ST7701S_HEIGHT,
    .type = LCM_TYPE_MIPI, /*mcu, rgb, mipi*/
    .fps = 60,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &s_lcm_mipi_st7701s_dp_info,
    .infor = {
    .mipi = &s_lcm_mipi_st7701s_mipi_info,
    },
    .fun = &s_lcm_mipi_st7701s_ctrl,
};

