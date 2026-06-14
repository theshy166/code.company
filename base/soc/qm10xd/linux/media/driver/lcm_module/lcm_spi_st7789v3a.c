#include "mol_lcm_local.h"
#include "rgb_com_param.h"
#include "mol_lcm_spi.h"

#define LCM_ST7789V3A_DEBUG 0

#define LCM_ST7789V3A_WIDTH  240
#define LCM_ST7789V3A_HEIGHT 320

lcm_cfg_t lcm_spi_st7789v3a_info;

lcm_setting_tab_t s_lcm_spi_st7789v3a_info[] =
{
    {LCM_SETTING_CMD,  0xb6},
    {LCM_SETTING_DATA, 0x04},
    {LCM_SETTING_DATA, 0x01},
    {LCM_SETTING_DATA, 0x9f},
    {LCM_SETTING_DATA, 0x00},
    {LCM_SETTING_DATA, 0x02},
    {LCM_DELAY,        20 },
    {LCM_END_OF_TABLE, 0x00}
};

static int32_t _lcm_spi_st7789v3a_power_on(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    ops->power.reset(0, 0);
    ops->power.back_light_gpio(0, 1);
    ops->power.dvdd(0, 1);

    //msleep(20);
    ops->power.reset(0, 1);
    //msleep(20);
    ops->power.reset(0, 0);

    //msleep(20);
    ops->power.reset(0, 1);
    //msleep(20);

    ops->bus_init((void *)param, (void *)&lcm_spi_st7789v3a_info);

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif


    return rtn;
}


static int32_t _lcm_spi_st7789v3a_power_off(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    CHECK_PTR(param, NULL, "param is null ~_~!\n");
    CHECK_PTR(ops, NULL, "ops is null ~_~!\n");

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    ops->power.back_light_gpio(0, 0);
    ops->power.dvdd(0, 0);

    ops->bus_deinit((void *)param, (void *)&lcm_spi_st7789v3a_info);

    EXIT:

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_spi_st7789v3a_identify(void * param, lcm_ops *ops)
{
#define LCM_ST7789V3A_ID0 0x30
#define LCM_ST7789V3A_ID1 0x30
#define LCM_ST7789V3A_ID2 0x01
    int32_t rtn = 0;
    //serial_bus_info *info = lcm_spi_st7789v3a_info.infor.rgb->serial;
    uint8_t data[4] = {0x00};

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    //ops->ctrl_fun.spi.read(info, 0x04, data, 0x03);

    if ((LCM_ST7789V3A_ID0 == data[0])
        && (LCM_ST7789V3A_ID1 == data[1])
        && (LCM_ST7789V3A_ID2 == data[2])) {
        rtn = SUCCESS;
    } else {
        rtn = FAIL_INVALID;
        printf("%s id :0x%x, 0x%x, 0x%x ~_~!\n",__FUNCTION__,data[0], data[1], data[2]);
    }

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_spi_st7789v3a_init(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    serial_bus_info *info = lcm_spi_st7789v3a_info.infor.rgb->serial;
    //uint8_t data[4] = {0x00};

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    ops->ctrl_fun.spi.write_cmd(info, 0xfd);
    ops->ctrl_fun.spi.write_data(info, 0x06);
    ops->ctrl_fun.spi.write_data(info, 0x08);

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_spi_st7789v3a_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcm_spi_st7789v3a_power_off(param, ops);

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_spi_st7789v3a_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    ops->power.back_light_gpio(0, 0);

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _lcm_spi_st7789v3a_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    ops->power.back_light_gpio(0, 1);

    #if LCM_ST7789V3A_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun _lcm_spi_st7789v3a_ctrl = {
    .poweron = _lcm_spi_st7789v3a_power_on,
    .poweroff = _lcm_spi_st7789v3a_power_off,
    .identify = _lcm_spi_st7789v3a_identify,
    .init = _lcm_spi_st7789v3a_init,
    .suspend = _lcm_spi_st7789v3a_suspend,
    .resume = _lcm_spi_st7789v3a_resume,
    .deinit = _lcm_spi_st7789v3a_deinit,
};

static lcm_display_sync_info_t _lcm_spi_st7789v3a_dp_info =
{
    .hor_sync_active = 10,
    .hor_back_porch = 6,
    .hor_active_pixel = LCM_ST7789V3A_WIDTH,
    .hor_front_porch = 10,

    .ver_sync_active = 9,
    .ver_back_porch = 1,
    .ver_active_line = LCM_ST7789V3A_HEIGHT,
    .ver_front_porch = 2,
};

static spi_bus_timing_t _lcm_spi_st7789v3a_timming =
{
    .mode = SPI_MODE0, /*lcm_serial_bus_spi_mode*/
    .c_setup = 0,
    .c_hold = 0,
    .d_setup = 0,
    .d_hold = 0,
    .rd8_start_dummy_cycle = 0,
    .rd_start_dummy_cycle = 0,
    .rd_end_dummy_cycle = 0,
    .delay = 0,
};

static rgb_clk_t _lcm_spi_st7789v3a_clk =
{
    /*pll cfg*/
    .reserved = 0,
};

static lcm_spi_info _lcm_spi_st7789v3a_info =
{
    .type = SPI_SPI,
    .baudRate = 6000000,
    .timing = {
        .spi = &_lcm_spi_st7789v3a_timming,
    },
    .clk = &_lcm_spi_st7789v3a_clk,
};

lcm_cfg_t lcm_spi_st7789v3a_info =
{
    .verder_name = VENDER_NAME,
    .name = "spi_st7789v3a_240x320_30fps",
    .width = LCM_ST7789V3A_WIDTH,
    .height = LCM_ST7789V3A_HEIGHT,
    .type = LCM_TYPE_SPI, /*mcu, rgb, spi*/
    .fps = 60,
    .direction = LCM_DIRECT_NORMAL,
    .dp_sync = &_lcm_spi_st7789v3a_dp_info,
    .infor = {
        .spi = &_lcm_spi_st7789v3a_info,
    },
    .fun = &_lcm_spi_st7789v3a_ctrl,
};
