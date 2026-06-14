#include "mol_lcm_local.h"
#include "dsi_com_param.h"
#include "mol_lcm.h"

#define LCM_EK79007_DEBUG 0

#define LCM_EK79007_WIDTH 1024
#define LCM_EK79007_HEIGHT 600
#define READ_TIMEOUT 500

const lcm_cfg_t s_lcm_mipi_ek79007_info;

dsi_setting_tab_t s_lcm_mipi_ek79007_init_setting[] = {
    /*
    Note :

    Data ID will depends on the following rule.

    count of parameters > 1  => Data ID = 0x39
    count of parameters = 1 => Data ID = 0x15
    count of parameters = 0 => Data ID = 0x05

    Structure Format :

    {DCS command, count of parameters, {parameter list}}
    {REGFLAG_DELAY, milliseconds of time, {}},

    ...

    Setting ending by predefined flag

    {REGFLAG_END_OF_TABLE, 0x00, {}}
    */
#if 0 //mtk
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0xB2, 0x10}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x80, 0x58}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x81, 0x47}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x82, 0xD4}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x83, 0x88}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x84, 0xA9}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x85, 0xC3}},
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x86, 0x82}},

    // sleep out
    //{DSI_DI_DELAY,               6,       },
    {DSI_DI_DCS_WRITE_1_PARAM, 0x02, {0x11, 00}},
    {DSI_DI_DELAY,               6,       },
    // display on
    //{DSI_DI_DCS_WRITE_1_PARAM, 0x01, {0x29, 00}},
    //{DSI_DI_DELAY,               20,       },

#else //HK
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x80, 0x47}},
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x81, 0x40}},
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x82, 0x04}},
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x83, 0x77}},
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x84, 0x0f}},
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x85, 0x70}},
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x86, 0x70}},
#endif
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};
#if 0
static mipi_lcm_setting_tab lcm_sleep_in_setting[] = {
    // sleep in
    {DSI_DI_DELAY,               20,       },
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x10}},
    {DSI_DI_DELAY,                20,       },
    // Display off
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x28}},
    {DSI_DI_DELAY,               20,       },
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};
#endif
dsi_setting_tab_t lcm_sleep_out_setting[] = {
    // sleep out
    {DSI_DI_DELAY,               20,       },
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x11}},
    {DSI_DI_DELAY,               20,       },
    // display on
    {DSI_DI_DCS_WRITE_0_PARAM, 0x01, {0x29}},
    {DSI_DI_DELAY,               20,       },
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};
#if 0
static mipi_lcm_setting_tab lcm_test_setting[] = {
    // sleep out
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x81, 0x40}},
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};

static mipi_lcm_setting_tab lcm_sw_reset_setting[] = {
    // sw reset
    {DSI_DI_DCS_WRITE_1_PARAM, 2, {0x01, 0x11}},
    {DSI_DI_DELAY,             20,            },
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};
#endif

static int32_t _lcm_mipi_ek79007_PowerOff(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&s_lcm_mipi_ek79007_info);

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif


    return rtn;
}

static int32_t _lcm_mipi_ek79007_PowerOn(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&s_lcm_mipi_ek79007_info);

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_ek79007_identify(void * param, lcm_ops *ops)
{
#define MIPI_EK79007_ID_ADDR (0x81)
#define MIPI_EK79007_ID (0x88)

    int32_t rtn = SUCCESS;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;
    uint8_t read_buf = 0;
    uint8_t read_cnt = 4;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "inParam ptr is NULL ~_~!\n");
    CHECK_PTR(ops, NULL, "ops ptr is NULL ~_~!\n");

    goto EXIT;

    while(read_cnt--) {
        ops->ctrl_fun.mipi.dcs_read(lcmHandle->lcm_id, DSI_DI_DCS_READ_0_PARAM, MIPI_EK79007_ID_ADDR, 1, &read_buf);

        if(MIPI_EK79007_ID == read_buf) {
            goto EXIT;
        }

        rtn = FAIL_ID_NO_MATCH;

        ops->delayMs(2);
    }

    printf("ek79007_4lane id : 0x%x:0x%x", MIPI_EK79007_ID, read_buf);

    EXIT:

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_ek79007_init(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, s_lcm_mipi_ek79007_init_setting, ARRAY_SIZE(s_lcm_mipi_ek79007_init_setting));
    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, lcm_sleep_out_setting, ARRAY_SIZE(lcm_sleep_out_setting));

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_ek79007_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    //rtn = lcm_back_light_gpio_set(0);

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_ek79007_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    //rtn = lcm_back_light_gpio_set(1);

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_ek79007_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcm_mipi_ek79007_PowerOff(param, ops);

    #if LCM_EK79007_DEBUG
    ops->debug_fun(LCM_EK79007_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun _s_lcm_mipi_ek79007_ctrl =
{
    .poweron = _lcm_mipi_ek79007_PowerOn,
    .poweroff = _lcm_mipi_ek79007_PowerOff,
    .identify = _lcm_mipi_ek79007_identify,
    .init = _lcm_mipi_ek79007_init,
    .suspend = _lcm_mipi_ek79007_suspend,
    .resume = _lcm_mipi_ek79007_resume,
    .deinit = _lcm_mipi_ek79007_deinit,
};

static lcm_display_sync_info_t _s_lcm_mipi_ek79007_dp_info =
{
    .hor_sync_active = 40,
    .hor_back_porch = 120,
    .hor_active_pixel = LCM_EK79007_WIDTH,
    .hor_front_porch = 160,

    .ver_sync_active = 5,
    .ver_back_porch = 18,
    .ver_active_line = LCM_EK79007_HEIGHT,
    .ver_front_porch = 12,
};

static dsi_timing_t _s_lcm_mipi_ek79007_timing =
{
    /*clk lane timing cfg*/
    .clane.mode = 1,
    .clane.lp11 = 0,
    .clane.lp01 = 0,
    .clane.zero = 0x22,
    .clane.prepare = 0x1,
    .clane.pre = 0x05,
    .clane.post = 0x0b,
    .clane.trail = 0x04,
    .clane.inittime = 0x00,
    .clane.exit = 0x06,
    /*data lane timing cfg*/
    .dlane.mode = 1,
    .dlane.lp11 = 0,
    .dlane.lp01 = 6,
    .dlane.zero = 0x0f,
    .dlane.prepare = 0x01,
    .dlane.trail = 0x05,
    .dlane.inittime = 0x00,
    .dlane.exit = 0x06,
    /*data lane0 read timing cfg*/
    .read.mode = 1,
    .read.tago = 0x1b,
    .read.tasure = 0x0b,
    .read.taget = 0x22,
    /*clk cfg*/
    .clk = {
        .vou.mode = 1,
        .vou.sel = 0x3,
        .vou.div = 0,

        .mif.mode = 1,
        .mif.pll_doubler = 0,
        .mif.pll_div_s = 1,
        .mif.pll_n = 0,
        .mif.pll_kint = 5637144,
        .mif.pll_nint = 42,
        .mif.pixelclk_div = 12,
        .mif.pll_pdiv = 2,
        .mif.dhd_div = 1,
    }
};

static lcm_mipi_info _s_lcm_mipi_ek79007_mipi_info =
{
    .work_mode = DSI_VIDEO_MODE,
    .lane_num = DSI_FOUR_LANE,
    .format = DSI_RGB888,
    .packet_size = 256,
    .timing = (void *)&_s_lcm_mipi_ek79007_timing,
};

const lcm_cfg_t s_lcm_mipi_ek79007_info =
{
    .verder_name = VENDER_NAME,
    .name = "ek79007_dsi_wsvga_vdo",
    .width = LCM_EK79007_WIDTH,
    .height = LCM_EK79007_HEIGHT,
    .type = LCM_TYPE_MIPI, /*mcu, rgb, mipi*/
    .fps = 50,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &_s_lcm_mipi_ek79007_dp_info,
    .infor = {
    .mipi = &_s_lcm_mipi_ek79007_mipi_info,
    },
    .fun = &_s_lcm_mipi_ek79007_ctrl,
};

