
#include "mol_lcm_rgb.h"
#include "mol_lcm_local.h"
#include "rgb_com_param.h"
#include "mol_lcm_spi.h"

#define LCM_DEBUG   1

#define LCM_WIDTH  800
#define LCM_HEIGHT 480

#define REG_PIN_MUX_BASE	0x10200000
#define REG_LCM_PWR_GPIO6_2 0xBC
#define REG_LCM_BKL_GPIO6_4	0xC4
#define REG_LCM_DIP_GPIO6_3	0xC8

#define GPIO_NUM_INDEX(x,y)	(8*x + y)
#define GPIO_NUM_LCM_PWR		GPIO_NUM_INDEX(6,2)
#define GPIO_NUM_LCM_BKL		GPIO_NUM_INDEX(6,4)
#define GPIO_NUM_LCM_DIP		GPIO_NUM_INDEX(6,3)

#define PIN_MUX_GPIO_ENABLE		(1)

extern unsigned int qua_lcm_readl(unsigned addr);
extern void qua_lcm_writel(unsigned int v, unsigned addr);

extern int  qua_lcm_gpio_request(unsigned gpio, const char *label);
extern void qua_lcm_gpio_free(unsigned gpio);
extern int  qua_lcm_gpio_get_value(unsigned gpio);
extern int  qua_lcm_gpio_direction_input(unsigned gpio);
extern void qua_lcm_gpio_set_value(unsigned gpio, int value);
extern int  qua_lcm_gpio_direction_output(unsigned gpio, int value);

const lcm_cfg_t qua_lcm_rgb_st7265_tcxd050iblma_info;
static int _lcm_pwr_on_flag = 0;
static void *_vpmuxbase = NULL;

static int32_t qua_lcm_rgb_st7265_power_off(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    qua_lcm_gpio_direction_output(GPIO_NUM_LCM_PWR, 0);
    qua_lcm_gpio_direction_output(GPIO_NUM_LCM_BKL, 0);

    qua_lcm_gpio_set_value(GPIO_NUM_LCM_PWR, 0);
    qua_lcm_gpio_set_value(GPIO_NUM_LCM_BKL, 0);

    rtn = ops->bus_deinit((void *)param, (void *)&qua_lcm_rgb_st7265_tcxd050iblma_info);
    EXIT_IF_FAIL(rtn, NULL, "ops->bus_deinit ~_~!\n");

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif
    fhosal_iounmap(_vpmuxbase);
    return rtn;
}

static int32_t qua_lcm_rgb_st7265_power_on(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;
    unsigned int pmux_reg = 0;

    _vpmuxbase = fhosal_ioremap_nocache(REG_PIN_MUX_BASE, 0x100);
	pmux_reg = (unsigned int)_vpmuxbase;

	/* power on rst timing gpio */
	qua_lcm_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + REG_LCM_PWR_GPIO6_2);
    qua_lcm_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + REG_LCM_BKL_GPIO6_4);
    qua_lcm_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + REG_LCM_DIP_GPIO6_3);

    qua_lcm_gpio_request(GPIO_NUM_LCM_PWR, "lcm_pwr");
    qua_lcm_gpio_request(GPIO_NUM_LCM_BKL, "lcm_bkl");
    qua_lcm_gpio_request(GPIO_NUM_LCM_DIP, "lcm_disp");

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    qua_lcm_gpio_direction_output(GPIO_NUM_LCM_PWR, 0);
    qua_lcm_gpio_direction_output(GPIO_NUM_LCM_DIP, 0);
    qua_lcm_gpio_direction_output(GPIO_NUM_LCM_BKL, 0);

    qua_lcm_gpio_set_value(GPIO_NUM_LCM_PWR, 1);
    qua_lcm_gpio_set_value(GPIO_NUM_LCM_DIP, 0);
    qua_lcm_gpio_set_value(GPIO_NUM_LCM_BKL, 1);

    ops->bus_init((void *)param, (void *)&qua_lcm_rgb_st7265_tcxd050iblma_info);
	_lcm_pwr_on_flag=1;

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif
    return rtn;
}


static int32_t qua_lcm_rgb_st7265_identify(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif
    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_st7265_init(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_st7265_deinit(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

    rtn = qua_lcm_rgb_st7265_power_off(param, ops);

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_st7265_suspend(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t qua_lcm_rgb_st7265_resume(void * param, lcm_ops *ops)
{
    int32_t rtn = 0;

    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(param, NULL, "lcm_handle_t is null ~_~!\n");
    CHECK_PTR(ops, NULL, "lcm_ops is null ~_~!\n");

EXIT:
    #if LCM_DEBUG
    ops->debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

lcm_module_fun qua_lcm_rgb_st7265_ctrl = {
    .poweron = qua_lcm_rgb_st7265_power_on,
    .poweroff = qua_lcm_rgb_st7265_power_off,
    .identify = qua_lcm_rgb_st7265_identify,
    .init = qua_lcm_rgb_st7265_init,
    .suspend = qua_lcm_rgb_st7265_suspend,
    .resume = qua_lcm_rgb_st7265_resume,
    .deinit = qua_lcm_rgb_st7265_deinit,
};

static lcm_display_sync_info_t qua_lcm_rgb_st7265_dp_info =
{
    .hor_sync_active = 4,
    .hor_back_porch = 16,
    .hor_active_pixel = LCM_WIDTH,
    .hor_front_porch = 16,

    .ver_sync_active = 4,
    .ver_back_porch = 16,
    .ver_active_line = LCM_HEIGHT,
    .ver_front_porch = 16,
};

static rgb_clk_t qua_lcm_rgb_st7265_clk =
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

static lcm_rgb_info qua_lcm_rgb_st7265_rgb_info =
{
    .ctrl_bus_mode = SERIAL_BUS_MAX,
    .video_bus_width = 0,
    .h_sync_pol = 0,
    .v_sync_pol = 0,
    .de_pol = 0,
    .format = LCM_RGB_565,
    .if_type = RGB_IF_16BITS,
    .order = RGB_ORDER_BGR,
    .dither = 0,
    .bitswap = 0,
    .serial = NULL,
    .clk = (void *)&qua_lcm_rgb_st7265_clk,
};

const lcm_cfg_t qua_lcm_rgb_st7265_tcxd050iblma_info =
{
    .verder_name = VENDER_NAME,
    .name = "qua_rgb_800_480_60_st7265_tcxd050iblma",
    .width = LCM_WIDTH,
    .height = LCM_HEIGHT,
    .type = LCM_TYPE_RGB, /*mcu, rgb, mipi*/
    .fps = 60,
    .direction = LCM_DIRECT_NORMAL,
    //.pixel_clk = 836*516*60,
    .dp_sync = &qua_lcm_rgb_st7265_dp_info,
    .infor = {
        .rgb = &qua_lcm_rgb_st7265_rgb_info,
    },
    .fun = &qua_lcm_rgb_st7265_ctrl,
};
