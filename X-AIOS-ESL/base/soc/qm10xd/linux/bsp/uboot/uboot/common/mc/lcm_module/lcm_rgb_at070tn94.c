#include "include/mol_lcm_rgb.h"
#include "include/mol_lcm_local.h"
#include "include/rgb_com_param.h"
#include "include/mol_lcm_spi.h"

#define LCM_AT070TN94_DEBUG 0

#define LCM_AT070TN94_WIDTH  800
#define LCM_AT070TN94_HEIGHT 480

lcm_cfg_t s_lcm_rgb_at070tn94_info;

static int32_t _lcm_rgb_at070tn94_power_off(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    rtn = ops->bus_deinit((void *)param, (void *)&s_lcm_rgb_at070tn94_info);
    EXIT_IF_FAIL(rtn, NULL, "ops->bus_deinit ~_~!\n");

    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_rgb_at070tn94_power_on(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    ops->bus_init((void *)param, (void *)&s_lcm_rgb_at070tn94_info);

    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_rgb_at070tn94_identify(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;
    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif
    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_rgb_at070tn94_init(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_rgb_at070tn94_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    rtn = _lcm_rgb_at070tn94_power_off(param, ops);

    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_rgb_at070tn94_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_rgb_at070tn94_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_AT070TN94_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun s_lcm_rgb_at070tn94_ctrl = {
    .poweron = _lcm_rgb_at070tn94_power_on,
    .poweroff = _lcm_rgb_at070tn94_power_off,
    .identify = _lcm_rgb_at070tn94_identify,
    .init = _lcm_rgb_at070tn94_init,
    .suspend = _lcm_rgb_at070tn94_suspend,
    .resume = _lcm_rgb_at070tn94_resume,
    .deinit = _lcm_rgb_at070tn94_deinit,
};

static lcm_display_sync_info_t s_lcm_rgb_at070tn94_dp_info =
{
    .hor_sync_active = 1,
    .hor_back_porch = 45,
    .hor_active_pixel = LCM_AT070TN94_WIDTH,
    .hor_front_porch = 210,

    .ver_sync_active = 1,
    .ver_back_porch = 22,
    .ver_active_line = LCM_AT070TN94_HEIGHT,
    .ver_front_porch = 22,
};

static rgb_clk_t s_lcm_rgb_at070tn94_clk =
{
    .vou.mode = 1,
    .vou.sel = 0x3,
    .vou.div = 0,

    .mif.mode = 1,
    .mif.pll_doubler = 0,
    .mif.pll_div_s = 1,
    .mif.pll_n = 0,
    .mif.pll_kint = 0x2d0e56,
    .mif.pll_nint = 0x2c,
    .mif.pixelclk_div = 0,
    .mif.pll_pdiv = 5,
    .mif.dhd_div = 1,
};

static lcm_rgb_info s_lcm_rgb_at070tn94_rgb_info =
{
    .ctrl_bus_mode = SERIAL_BUS_MAX,
    .video_bus_width = 0,
    .h_sync_pol = 0,
    .v_sync_pol = 0,
    .de_pol = 0,
    .format = LCM_RGB_888,
    .if_type = RGB_IF_24BITS,
    .order = RGB_ORDER_RGB,
    .dither = 0,
    .bitswap = 0,
    .serial = NULL,
    .clk = (void *)&s_lcm_rgb_at070tn94_clk,
};

lcm_cfg_t s_lcm_rgb_at070tn94_info =
{
    .verder_name = VENDER_NAME,
    .name = "rgb24_AT070TN94_800x480",
    .width = LCM_AT070TN94_WIDTH,
    .height = LCM_AT070TN94_HEIGHT,
    .type = LCM_TYPE_RGB, /*mcu, rgb, mipi*/
    .fps = 60,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &s_lcm_rgb_at070tn94_dp_info,
    .infor = {
        .rgb = &s_lcm_rgb_at070tn94_rgb_info,
    },
    .fun = &s_lcm_rgb_at070tn94_ctrl,
};
