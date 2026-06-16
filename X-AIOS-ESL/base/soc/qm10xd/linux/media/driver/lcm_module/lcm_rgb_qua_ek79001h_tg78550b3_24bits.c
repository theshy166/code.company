#include "mol_lcm_rgb.h"
#include "mol_lcm_local.h"
#include "rgb_com_param.h"
#include "mol_lcm_spi.h"

#define LCM_EK79001H_DEBUG 0

#define LCM_EK79001H_WIDTH  1024
#define LCM_EK79001H_HEIGHT 600

lcm_cfg_t qua_lcm_rgb_ek79001h_tg78550b3_24bits_info;

static int32_t qua_lcm_rgb_ek79001h_power_off(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    rtn = ops->bus_deinit((void *)param, (void *)&qua_lcm_rgb_ek79001h_tg78550b3_24bits_info);
    EXIT_IF_FAIL(rtn, NULL, "ops->bus_deinit ~_~!\n");

    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_ek79001h_power_on(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    ops->bus_init((void *)param, (void *)&qua_lcm_rgb_ek79001h_tg78550b3_24bits_info);

    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_ek79001h_identify(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;
    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif
    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_ek79001h_init(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_ek79001h_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    rtn = qua_lcm_rgb_ek79001h_power_off(param, ops);

    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_ek79001h_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_ek79001h_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = SUCCESS;

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");


    EXIT:

    #if LCM_EK79001H_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static lcm_module_fun s_qua_lcm_rgb_ek79001h_ctrl = {
    .poweron = qua_lcm_rgb_ek79001h_power_on,
    .poweroff = qua_lcm_rgb_ek79001h_power_off,
    .identify = qua_lcm_rgb_ek79001h_identify,
    .init = qua_lcm_rgb_ek79001h_init,
    .suspend = qua_lcm_rgb_ek79001h_suspend,
    .resume = qua_lcm_rgb_ek79001h_resume,
    .deinit = qua_lcm_rgb_ek79001h_deinit,
};

static lcm_display_sync_info_t s_qua_lcm_rgb_ek79001h_dp_info =
{
    .hor_sync_active = 1,
    .hor_back_porch = 170,
    .hor_active_pixel = LCM_EK79001H_WIDTH,
    .hor_front_porch = 149,

    .ver_sync_active = 1,
    .ver_back_porch = 20,
    .ver_active_line = LCM_EK79001H_HEIGHT,
    .ver_front_porch = 14,
};

static rgb_clk_t s_qua_lcm_rgb_ek79001h_clk =
{
    .vou.mode = 1,
    .vou.sel = 0x3,
    .vou.div = 0,

    .mif.mode = 0,
    .mif.pll_doubler = 0,
    .mif.pll_div_s = 1,
    .mif.pll_n = 0,
    .mif.pll_kint = 0x2aaaaa,
    .mif.pll_nint = 0x55,
    .mif.pixelclk_div = 6,
    .mif.pll_pdiv = 3,
    .mif.dhd_div = 7,
};

static lcm_rgb_info s_qua_lcm_rgb_ek79001h_rgb_info =
{
    .ctrl_bus_mode = SERIAL_BUS_MAX,
    .video_bus_width = 0,
    .h_sync_pol = 0,
    .v_sync_pol = 0,
    .de_pol = 0,
    .format = LCM_RGB_888,
    .if_type = RGB_IF_24BITS,
    .order = RGB_ORDER_BGR,
    .dither = 0,
    .bitswap = 0,
    .serial = NULL,
    .clk = (void *)&s_qua_lcm_rgb_ek79001h_clk,
};

lcm_cfg_t qua_lcm_rgb_ek79001h_tg78550b3_24bits_info =
{
    .verder_name = VENDER_NAME,
    .name = "qua_ek79001h_rgb_1024_600_60_tg78550b3_24bits",
    .width = LCM_EK79001H_WIDTH,
    .height = LCM_EK79001H_HEIGHT,
    .type = LCM_TYPE_RGB, /*mcu, rgb, mipi*/
    .fps = 60,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &s_qua_lcm_rgb_ek79001h_dp_info,
    .infor = {
        .rgb = &s_qua_lcm_rgb_ek79001h_rgb_info,
    },
    .fun = &s_qua_lcm_rgb_ek79001h_ctrl,
};
