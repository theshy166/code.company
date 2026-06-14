#include "mol_lcm_local.h"
#include "dsi_com_param.h"
#include "mol_lcm.h"

#define LCM_QFH024004_DEBUG 0

#define LCM_QFH024004_WIDTH 1200
#define LCM_QFH024004_HEIGHT 1920
#define READ_TIMEOUT 500


const lcm_cfg_t s_lcm_mipi_qfh24004_info;

dsi_setting_tab_t s_lcm_mipi_qfh24004_init_setting[] = {
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
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB0, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC3, 0x4F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC4, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC5, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC6, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC7, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC8, 0x4D}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC9, 0x52}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCA, 0x51}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCD, 0x5D}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCE, 0x5B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCF, 0x4B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD0, 0x49}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD1, 0x47}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD2, 0x45}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD3, 0x41}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD7, 0x50}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD8, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD9, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDA, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDB, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDC, 0x4E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDD, 0x52}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDE, 0x51}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE1, 0x5E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE2, 0x5C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE3, 0x4C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE4, 0x4A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE5, 0x48}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE6, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE7, 0x42}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB0, 0x03}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xBE, 0x03}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCC, 0x44}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC8, 0x07}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC9, 0x05}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCA, 0x42}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCD, 0x3E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCF, 0x60}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD2, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD3, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD4, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD5, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD6, 0x03}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD7, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD9, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDB, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE4, 0xF0}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE5, 0x0A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB0, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC2, 0x08}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC4, 0x10}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB0, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC0, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC1, 0x0A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC2, 0x20}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC3, 0x24}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC4, 0x23}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC5, 0x29}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC6, 0x23}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC7, 0x1C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC8, 0x19}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xC9, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCA, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCB, 0x18}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCC, 0x1A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCD, 0x1E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCE, 0x20}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCF, 0x23}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD0, 0x07}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD1, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD2, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD3, 0x0A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD4, 0x13}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD5, 0x1C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD6, 0x1A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD7, 0x13}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD8, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xD9, 0x1C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDA, 0x19}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDB, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDC, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDD, 0x18}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDE, 0x1A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xDF, 0x1E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x20}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE1, 0x23}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE2, 0x07}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB0, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB2, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xB0, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCD, 0xDC}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xCC, 0x00}},
	{DSI_DI_END_OF_TABLE, 0x00, {0}}

};

dsi_setting_tab_t lcm_sleep_out_qfh24004_setting[] = {
    {DSI_DI_GENERIC_WRITE_0_PARAM, 0x01, {0x11}},
    {DSI_DI_DELAY,               120,       },
    // display on
    {DSI_DI_GENERIC_WRITE_0_PARAM, 0x01, {0x29}},
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};

static int32_t _lcm_mipi_qfh24004_PowerOff(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&s_lcm_mipi_qfh24004_info);

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif


    return rtn;
}

static int32_t _lcm_mipi_qfh24004_PowerOn(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&s_lcm_mipi_qfh24004_info);

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_qfh24004_identify(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;
    return rtn;
}

static int32_t _lcm_mipi_qfh24004_init(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, s_lcm_mipi_qfh24004_init_setting, ARRAY_SIZE(s_lcm_mipi_qfh24004_init_setting));
    rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, lcm_sleep_out_qfh24004_setting, ARRAY_SIZE(lcm_sleep_out_qfh24004_setting));

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_qfh24004_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    //rtn = lcm_back_light_gpio_set(0);

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_qfh24004_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    //rtn = lcm_back_light_gpio_set(1);

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_mipi_qfh24004_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcm_mipi_qfh24004_PowerOff(param, ops);

    #if LCM_QFH024004_DEBUG
    ops->debug_fun(LCM_QFH024004_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun _s_lcm_mipi_qfh24004_ctrl =
{
    .poweron = _lcm_mipi_qfh24004_PowerOn,
    .poweroff = _lcm_mipi_qfh24004_PowerOff,
    .identify = _lcm_mipi_qfh24004_identify,
    .init = _lcm_mipi_qfh24004_init,
    .suspend = _lcm_mipi_qfh24004_suspend,
    .resume = _lcm_mipi_qfh24004_resume,
    .deinit = _lcm_mipi_qfh24004_deinit,
};

#if 1
static lcm_display_sync_info_t _s_lcm_mipi_qfh24004_dp_info =
{
    .hor_sync_active = 24,
    .hor_back_porch = 80,
    .hor_active_pixel = 1200,
    .hor_front_porch = 60,

    .ver_sync_active = 2,
    .ver_back_porch = 10,
    .ver_active_line = 1920,
    .ver_front_porch = 14,
};
#endif
#if 0
static lcm_display_sync_info_t _s_lcm_mipi_qfh24004_dp_info =
{
    .hor_sync_active = 10,
    .hor_back_porch = 40,
    .hor_active_pixel = 1200,
    .hor_front_porch = 80,

    .ver_sync_active = 4,
    .ver_back_porch = 16,
    .ver_active_line = 1920,
    .ver_front_porch = 20,
};
#endif

static dsi_timing_t _s_lcm_mipi_qfh24004_timing =
{
    /*clk lane timing cfg*/
    .clane.mode = 1,
    .clane.lp11 = 0,
    .clane.lp01 = 0,
    .clane.zero = 25,
    .clane.prepare = 5,
    .clane.pre = 4,
    .clane.post = 13,
    .clane.trail = 7,
    .clane.inittime = 11,
    .clane.exit = 11,
    /*data lane timing cfg*/
    .dlane.mode = 1,
    .dlane.lp11 = 0,
    .dlane.lp01 = 11,
    .dlane.zero = 9,
    .dlane.prepare = 5,
    .dlane.trail = 7,
    .dlane.inittime = 11,
    .dlane.exit = 11,
    /*data lane0 read timing cfg*/
    .read.mode = 1,
    .read.tago = 47,
    .read.tasure = 17,
    .read.taget = 59,
    /*clk cfg*/
    .clk = {
        .vou.mode = 1,
        .vou.sel = 0x3,
        .vou.div = 0,

        .mif.mode = 1,
        .mif.pll_doubler = 0,
        .mif.pll_div_s = 1,
        .mif.pll_n = 0,
        .mif.pll_kint = 728802,
        .mif.pll_nint = 53,
        .mif.pixelclk_div = 12,
        .mif.pll_pdiv = 1,
        .mif.dhd_div = 0,
    }

};

static lcm_mipi_info _s_lcm_mipi_qfh24004_mipi_info =
{
    .work_mode = DSI_VIDEO_MODE,
    .lane_num = DSI_FOUR_LANE,
    .format = DSI_RGB888,
    .packet_size = 256,
    .timing = (void *)&_s_lcm_mipi_qfh24004_timing,
};

const lcm_cfg_t s_lcm_mipi_qfh24004_info =
{
    .verder_name = VENDER_NAME,
    .name = "2081101QFH024004_53L_A",
    .width = LCM_QFH024004_WIDTH,
    .height = LCM_QFH024004_HEIGHT,
    .type = LCM_TYPE_MIPI, /*mcu, rgb, mipi*/
    .fps = 40,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &_s_lcm_mipi_qfh24004_dp_info,
    .infor = {
    .mipi = &_s_lcm_mipi_qfh24004_mipi_info,
    },
    .fun = &_s_lcm_mipi_qfh24004_ctrl,
};

