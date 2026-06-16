#include "reg.h"
#include "vou_clk_v01_reg_def.h"
#include "vou_clk_v01_drv.h"
#include "os.h"
#include "debug.h"

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
#define VOU_CLK_PARAM_DEBUG 0
#define VOU_CLK_FUN_DEBUG 0

//#define _CLK_STANDARD_PERATION_ 1

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef enum {
    VOU_DHD0_ID = 0,
    VOU_DHD1_ID,
    VOU_DEV_ID_MAX,
}vou_dev_id;

typedef enum {
    LCM_CLK_OFF = 0,
    LCM_CLK_ON,
    LCM_CLK_STATUS_MAX,
}lcm_clk_status_e;

typedef struct {
    lcm_clk_status_e status;
    int32_t on_cnt;
    int32_t off_cnt;
    int32_t cur_cnt;
}lcm_clk_status_t;

typedef struct {
    lcm_clk_status_t clk_status;

}lcm_clk_context_t;

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
lcm_clk_context_t s_lcm_clk_context = {0x00};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
static lcm_clk_context_t *_get_lcm_clk_context(void)
{
    return &s_lcm_clk_context;
}

int32_t vou_clk_calc(vou_clk_calc_in_t *inParam, vou_clk_calc_out_t *outParam)
{
    int32_t rtn = 0;
    uint32_t clk_src_tab[4] = {24000000, 120000000, 150000000, 200000000};
    uint32_t clk_src = 0x00;
    uint32_t total_freq = 0x00;
    uint32_t total_freq_doubler = 0x00;
    uint32_t freq_diff = 0x00;
    uint32_t min_freq_diff = 0xffffffff;

    uint32_t div_freq = 0x00;
    uint32_t div_idx = 1;
    uint32_t clk_sel_idx = 0x00;

    uint32_t clk_sel = 0x00;
    uint32_t div = 0x00;

    #if VOU_CLK_PARAM_DEBUG
    printf("\n");
    #endif

    if (1 == inParam->clkInfo.mode) {
        #if VOU_CLK_PARAM_DEBUG
        printf("-----------------vou pll manual calc-------------------------\n");
        #endif
        clk_sel = inParam->clkInfo.cfg.sel;
        div = inParam->clkInfo.cfg.div + 1;
    } else {

        #if VOU_CLK_PARAM_DEBUG
        printf("-----------------vou pll auto calc---------------------------\n");
        #endif

        total_freq = inParam->total_pixel * inParam->tx_frq_pixel;
        total_freq_doubler = total_freq << 1;

        for (clk_sel_idx = 0; clk_sel_idx < 4; clk_sel_idx++) {

            clk_src = clk_src_tab[clk_sel_idx];

            for(div_idx = 1; div_idx < 257; div_idx++ ) {

                div_freq = clk_src / div_idx;

                if (div_freq < total_freq_doubler) {  // vou proc clk must more than total proc
                    continue;
                }

                freq_diff = total_freq_doubler > div_freq ? total_freq_doubler - div_freq : div_freq - total_freq_doubler;

                if (freq_diff < min_freq_diff) {
                    min_freq_diff = freq_diff;
                    clk_sel = clk_sel_idx;
                    div = div_idx;
                }
            }
        }
    }

    outParam->cfg.sel = clk_sel;
    outParam->cfg.div = div - 1;
    outParam->if_clk = clk_src_tab[clk_sel]/div;

    #if VOU_CLK_PARAM_DEBUG
    printf("total_freq: %d, total_pixel: %d, tx_frq_pixel: %d, total_freq_doubler: %d\n", total_freq, inParam->total_pixel, inParam->tx_frq_pixel, total_freq_doubler);
    printf("vou sel:%d, div:%d, if_clk:%d\n", outParam->cfg.sel, outParam->cfg.div, outParam->if_clk);
    printf("src_clk:%d, div:%d, pixel:%d\n", clk_src_tab[clk_sel], div, clk_src_tab[clk_sel]/div);
    printf("-----------------vou pll calc end----------------------------\n\n");
    #endif

    return rtn;
}

int32_t vou_clk_on(ADDR base_addr, uint32_t sel, uint32_t div)
{
    int32_t rtn = 0;
    #ifndef _CLK_STANDARD_PERATION_
    union dmc_sys_vou_clk_tag vou_clk_en = {{0x00}};
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    #ifndef _CLK_STANDARD_PERATION_
    vou_clk_en.dwValue = regRead32(base_addr + VOU_CLK_EN);
    vou_clk_en.mBits.ckg_vou_bus_force_en = 1;
    vou_clk_en.mBits.ckg_vou_bus_auto_gate_sel = 0;
    vou_clk_en.mBits.ckg_vou_force_en = 1;
    vou_clk_en.mBits.ckg_vou_auto_gate_sel = 0;
    vou_clk_en.mBits.ckg_vou_en = 1;
    vou_clk_en.mBits.ckg_vou_bus_div = div;
    vou_clk_en.mBits.ckg_vou_bus_sel = sel;
    regWrite32(base_addr + VOU_CLK_EN, vou_clk_en.dwValue);
    #else
    clk_mini_set_rate(CKG_MUX_VOU_BUS, 200000000);
    clk_mini_enable(CKG_VOU_EN);
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif


    return rtn;
}

int32_t vou_reset(ADDR base_addr)
{
    int32_t rtn = 0;
    union dmc_sys_vou_rst_tag vou_rst = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    vou_rst.dwValue = regRead32(base_addr + VOU_RST);

    vou_rst.mBits.vou_pub_soft_rst = 1;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    vou_rst.mBits.vou_pub_soft_rst = 0;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_clk_on_reset(ADDR clk_base_addr, uint32_t sel, uint32_t div, ADDR rst_base_addr)
{
    int32_t rtn = 0;

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn =vou_clk_on(clk_base_addr, sel, div);
    rtn += vou_reset(rst_base_addr);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_clk_off(ADDR base_addr)
{
    int32_t rtn = 0;

    #ifndef _CLK_STANDARD_PERATION_
    union dmc_sys_vou_rst_tag vou_rst = {{0x00}};
    union dmc_sys_vou_clk_tag vou_clk_en = {{0x00}};
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    #ifndef _CLK_STANDARD_PERATION_
    vou_rst.dwValue = regRead32(base_addr + VOU_RST);
    vou_clk_en.dwValue = regRead32(base_addr + VOU_CLK_EN);

    vou_rst.mBits.vou_pub_soft_rst = 1;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    vou_clk_en.mBits.ckg_vou_en = 0;
    regWrite32(base_addr + VOU_CLK_EN, vou_clk_en.dwValue);
    #else
    clk_mini_disable(CKG_VOU_EN);
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_host_clk_on(ADDR base_addr)
{
    int32_t rtn = 0;
    #ifndef _CLK_STANDARD_PERATION_
    union dmc_sys_vou_clk_tag vou_clk_en = {{0x00}};
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    #ifndef _CLK_STANDARD_PERATION_
    vou_clk_en.dwValue = regRead32(base_addr + VOU_CLK_EN);

    vou_clk_en.mBits.ckg_apb_dsi_cfg_en = 1;
    regWrite32(base_addr + VOU_CLK_EN, vou_clk_en.dwValue);
    #else
    clk_mini_enable(CKG_APB_DSI_CFG_EN);
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_host_reset(ADDR base_addr)
{
    int32_t rtn = 0;
    union dmc_sys_vou_rst_tag vou_rst = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    vou_rst.dwValue = regRead32(base_addr + VOU_RST);

    vou_rst.mBits.dsi_soft_rst = 1;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    vou_rst.mBits.dsi_soft_rst = 0;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_host_clk_on_reset(ADDR clk_base_addr, ADDR rst_base_addr)
{
    int32_t rtn = 0;

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn =dsi_host_clk_on(clk_base_addr);
    rtn += dsi_host_reset(rst_base_addr);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_host_clk_off(ADDR base_addr)
{
    int32_t rtn = 0;

    #ifndef _CLK_STANDARD_PERATION_
    union dmc_sys_vou_clk_tag vou_clk_en = {{0x00}};
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    #ifndef _CLK_STANDARD_PERATION_
    vou_clk_en.dwValue = regRead32(base_addr + VOU_CLK_EN);

    vou_clk_en.mBits.ckg_apb_dsi_cfg_en = 0;
    regWrite32(base_addr + VOU_CLK_EN, vou_clk_en.dwValue);
    #else
    clk_mini_disable(CKG_APB_DSI_CFG_EN);
    #endif
    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_phy_clk_on(ADDR base_addr)
{
    int32_t rtn = 0;
    #ifndef _CLK_STANDARD_PERATION_
    union dmc_sys_vou_clk_tag vou_clk_en = {{0x00}};
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    #ifndef _CLK_STANDARD_PERATION_
    vou_clk_en.dwValue = regRead32(base_addr + VOU_CLK_EN);

    vou_clk_en.mBits.ckg_apb_dphy_cfg_en = 1;
    regWrite32(base_addr + VOU_CLK_EN, vou_clk_en.dwValue);
    #else
    clk_mini_enable(CKG_APB_DPHY_CFG_EN);
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_phy_reset(ADDR base_addr)
{
    int32_t rtn = 0;
    union dmc_sys_vou_rst_tag vou_rst = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    vou_rst.dwValue = regRead32(base_addr + VOU_RST);

    vou_rst.mBits.dsi_phy_soft_rst = 1;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    vou_rst.mBits.dsi_phy_soft_rst = 0;
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);
    regWrite32(base_addr + VOU_RST, vou_rst.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_phy_clk_on_reset(ADDR clk_base_addr, ADDR rst_base_addr)
{
    int32_t rtn = 0;

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn =dsi_phy_clk_on(clk_base_addr);
    rtn += dsi_phy_reset(rst_base_addr);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_phy_clk_off(ADDR base_addr)
{
    int32_t rtn = 0;
    #ifndef _CLK_STANDARD_PERATION_
    union dmc_sys_vou_clk_tag vou_clk_en = {{0x00}};
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    #ifndef _CLK_STANDARD_PERATION_
    vou_clk_en.dwValue = regRead32(base_addr + VOU_CLK_EN);

    vou_clk_en.mBits.ckg_apb_dphy_cfg_en = 0;
    regWrite32(base_addr + VOU_CLK_EN, vou_clk_en.dwValue);
    #else
    clk_mini_disable(CKG_APB_DPHY_CFG_EN);
    #endif

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t lcm_clk_off(lcm_clk_off_in_t *inParam, lcm_clk_off_out_t *outParam)
{
    int32_t rtn = 0;
    lcm_clk_context_t *lcm_clk_context = _get_lcm_clk_context();

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    lcm_clk_context->clk_status.on_cnt--;
    lcm_clk_context->clk_status.cur_cnt--;
    lcm_clk_context->clk_status.off_cnt++;

    if (0 == lcm_clk_context->clk_status.cur_cnt) {

        rtn = vou_reset(inParam->rst_base_addr);
        rtn = vou_clk_off(inParam->clk_base_addr);

        rtn = dsi_host_reset(inParam->rst_base_addr);
        rtn = dsi_host_clk_off(inParam->clk_base_addr);

        rtn = dsi_phy_reset(inParam->rst_base_addr);
        rtn = dsi_phy_clk_off(inParam->clk_base_addr);

        lcm_clk_context->clk_status.status = LCM_CLK_OFF;
        lcm_clk_context->clk_status.on_cnt = 0x00;
    }

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t lcm_clk_on(lcm_clk_on_in_t *inParam, lcm_clk_on_out_t *outParam)
{
    int32_t rtn = 0;
    lcm_clk_context_t *lcm_clk_context = _get_lcm_clk_context();

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    if (0 == lcm_clk_context->clk_status.cur_cnt) {

        rtn = vou_clk_on(inParam->clk_base_addr, inParam->clk_sel, inParam->clk_div);
        rtn = vou_reset(inParam->rst_base_addr);

        rtn = dsi_host_clk_on(inParam->clk_base_addr);
        rtn = dsi_host_reset(inParam->rst_base_addr);

        rtn = dsi_phy_clk_on(inParam->clk_base_addr);
        rtn = dsi_phy_reset(inParam->rst_base_addr);

        lcm_clk_context->clk_status.status = LCM_CLK_ON;
        lcm_clk_context->clk_status.off_cnt = CLEAN_ZERO;
    }

    lcm_clk_context->clk_status.on_cnt++;
    lcm_clk_context->clk_status.cur_cnt++;

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t lcm_clk_status_clean(void)
{
    int32_t rtn = 0;
    lcm_clk_context_t *lcm_clk_context = _get_lcm_clk_context();

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    os_memset((void *)lcm_clk_context, 0x00, sizeof(lcm_clk_context_t));

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_dhd_clk_on(ADDR base_addr, uint32_t dhd_id, uint32_t div)
{
    int32_t rtn = 0;
    union vou_dhd_chn_clk_tag clk = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    clk.dwValue = regRead32(base_addr + VOU_DHD_CLK);

    if (VOU_DHD0_ID == dhd_id) {
        clk.mBits.dhd0_en = 1;
        clk.mBits.dhd0_div = div;
    } else if (VOU_DHD1_ID == dhd_id) {
        clk.mBits.dhd1_en = 1;
        clk.mBits.dhd1_div = div;
    }

    regWrite32(base_addr + VOU_DHD_CLK, clk.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_dhd_rst(ADDR base_addr, uint32_t dhd_id)
{
    int32_t rtn = 0;
    union dmc_sys_vou_rst_tag rst = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rst.dwValue = regRead32(base_addr + VOU_RST);

    if (VOU_DHD0_ID == dhd_id) {
        rst.mBits.vou_dhd0_soft_rst = 1;
        rst.mBits.vou_mif0_soft_rst = 1;
    } else if (VOU_DHD1_ID == dhd_id) {
        rst.mBits.vou_dhd1_soft_rst = 1;
        rst.mBits.vou_mif1_soft_rst = 1;
    }
    regWrite32(base_addr + VOU_RST, rst.dwValue);
    regWrite32(base_addr + VOU_RST, rst.dwValue);

    if (VOU_DHD0_ID == dhd_id) {
        rst.mBits.vou_dhd0_soft_rst = 0;
        rst.mBits.vou_mif0_soft_rst = 0;
    } else if (VOU_DHD1_ID == dhd_id) {
        rst.mBits.vou_dhd1_soft_rst = 0;
        rst.mBits.vou_mif1_soft_rst = 0;
    }
    regWrite32(base_addr + VOU_RST, rst.dwValue);
    regWrite32(base_addr + VOU_RST, rst.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}


int32_t vou_dhd_clk_off(ADDR base_addr, uint32_t dhd_id)
{
    int32_t rtn = 0;
    union vou_dhd_chn_clk_tag clk = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    clk.dwValue = regRead32(base_addr + VOU_DHD_CLK);

    if (VOU_DHD0_ID == dhd_id) {
        clk.mBits.dhd0_en = 0;
        clk.mBits.dhd0_div = 0;
    } else if (VOU_DHD1_ID == dhd_id) {
        clk.mBits.dhd1_en = 0;
        clk.mBits.dhd1_div = 0;
    }

    regWrite32(base_addr + VOU_DHD_CLK, clk.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_dhd_out_mode(ADDR base_addr, uint32_t dhd_id, vou_out_mode_e mode)
{
    int32_t rtn = 0;
    union vou_display_format_tag out_mode = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    out_mode.dwValue = regRead32(base_addr + VOU_DISPLAY_FORMAT);

    if (VOU_DHD0_ID == dhd_id) {
        out_mode.mBits.dhd0_diaplay_format = mode;
    } else if (VOU_DHD1_ID == dhd_id) {
        out_mode.mBits.dhd1_diaplay_format = mode;
    }

    regWrite32(base_addr + VOU_DISPLAY_FORMAT, out_mode.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_dhd_force_update(ADDR base_addr, uint32_t dhd_id, uint32_t force_update)
{
    int32_t rtn = 0;
    union vou_chn_work_ctrl_tag work_ctrl = {{0x00}};
    ADDR addr = 0x00;

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    addr = base_addr + (VOU_DHD_OFFSET * (1 << dhd_id)) + VOU_DHD_CHN_WORK_CTRL;
    work_ctrl.dwValue = regRead32(addr);

    work_ctrl.mBits.force_update = 1;

    regWrite32(addr, work_ctrl.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}


int32_t vou_lcd_src_sel(ADDR base_addr, uint32_t sel)
{
    int32_t rtn = 0;
    union vou_ctrl0_tag vou_src_ctrl = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif


    //printf("------vou_lcd_src_sel------sel:%d\n", sel);
    vou_src_ctrl.dwValue = regRead32(base_addr + VOU_CTRL_0);

    vou_src_ctrl.mBits.lcd_src_sel = sel;

    regWrite32(base_addr + VOU_CTRL_0, vou_src_ctrl.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t vou_dhd_src_sel(ADDR base_addr, uint32_t dhd_id, uint32_t sel)
{
    int32_t rtn = 0;
    union vou_ctrl0_tag vou_src_ctrl = {{0x00}};

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    //printf("------vou_lcd_src_sel------dhd_id:%d, sel:%d\n", dhd_id, sel);

    vou_src_ctrl.dwValue = regRead32(base_addr + VOU_CTRL_0);

    if (0 == dhd_id) {
        vou_src_ctrl.mBits.dhd0_src_sel = sel;
        vou_src_ctrl.mBits.dhd1_src_sel = 0;
    } else if (1 == dhd_id) {
        vou_src_ctrl.mBits.dhd0_src_sel = 0;
        vou_src_ctrl.mBits.dhd1_src_sel = sel;
    }

    regWrite32(base_addr + VOU_CTRL_0, vou_src_ctrl.dwValue);

    #if VOU_CLK_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

