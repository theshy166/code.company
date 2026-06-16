#include "dsi_pixel_info.h"
#include "dsi_phy_v01_drv_mpi_param.h"
#include "dsi_timing_calc_param.h"

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
#define TIMING_DEBUG_LOG 0
/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
/*
Total pixel = H-total * V-total *fps
Bitclk = Total pixel * bitsD/ lane_num
*/

typedef struct {
    uint32_t lane_clk;
}dsi_phy_ui_calc_in_t;

typedef struct {
    uint32_t UI;
}dsi_phy_ui_calc_out_t;

static int32_t _dsi_phy_ui_calc(dsi_phy_ui_calc_in_t *inParam, dsi_phy_ui_calc_out_t *outParam)
{
    #define NS 1000000000
    int32_t rtn = 0x00;
    uint32_t bit_clk = 0x00;
    uint32_t UI_8 = 0x00;
    uint32_t UI = 0x00;

    bit_clk = inParam->lane_clk << 1;

    /*bit clk expand 1000X, UI time expand 1000X for timing(prepare/zero/trail) min/max calc*/
    bit_clk = bit_clk/1000;
    UI = NS / bit_clk;
    UI_8 = UI * 8;

    outParam->UI = UI;

    if (UI_8) {

    }
    #if TIMING_DEBUG_LOG
    printf("UI time:%d.%dns, %d.%dns\n", (outParam->UI)/1000, (outParam->UI)%1000, UI_8/1000, UI_8%1000);
    #endif

    return rtn;
}

static int32_t _ui_time_to_reg_carry(uint32_t time, uint32_t UI, uint32_t *reg)
{
    uint32_t ui_time = 0x00;

    /*
    time is time(ns), the value is time(ns) * 1000;
    UI is time(ns), the value is half bit clk cycle;
    reg is register value, the register value mean 8*UI

    reg mean time is (8 * UI) * (reg + 1) ns
    */

    ui_time = ((time + ((8 * UI) - 1)) / (8 * UI));

    if (0 < ui_time) {
        ui_time -= 1;
    }

    *reg = ui_time;

    return 0;
}

static int32_t _ui_time_to_reg_truncation(uint32_t time, uint32_t UI, uint32_t *reg)
{
    uint32_t ui_time = 0x00;

    /*
    time is time(ns), the value is time(ns) * 1000;
    UI is time(ns), the value is half bit clk cycle;
    reg is register value, the register value mean 8*UI

    reg mean time is (8 * UI) * (reg + 1) ns
    */

    ui_time = (time / (8 * UI));

    if (0 < ui_time) {
        ui_time -= 1;
    }

    *reg = ui_time;

    return 0;
}

static int32_t _reg_to_ui_time(uint32_t reg, uint32_t UI, uint32_t *time)
{
    /*
    time is time(ns), the value is time(ns) * 1000;
    UI is time(ns), the value is half bit clk cycle;
    reg is register value, the register value mean 8*UI

    reg mean time is (8 * UI) * (reg + 1) ns
    */

    *time = (reg + 1) * (8 * UI);

    return 0;
}

static int32_t _dsi_phy_clk_lp11_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_CLK_LP11_BASE 50000 //50ns
    #define MC_CLK_LP11_OFFSET 100000 // 100ns

    int32_t rtn = 0x00;
    uint32_t clk_lp11_time = 0x00;
    uint32_t clk_lp11_reg = 0x00;

    /*
    MIN_CLK_LP11_BASE is time(ns), the value is time(ns) * 1000;

    clk_lp11_reg is register value, the register value mean 8*UI

    lp01 mean ltpx
    */

    clk_lp11_time = MIN_CLK_LP11_BASE + MC_CLK_LP11_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_lp11_time, UI, &clk_lp11_reg);

    rtn = _reg_to_ui_time(clk_lp11_reg, UI, &clk_lp11_time);

    dsi_timing->clane.lp11 = clk_lp11_reg;

    if (rtn) {

    }

    #if TIMING_DEBUG_LOG
    printf("clk lp_11       :%d.%dns, reg:0x%x\n", clk_lp11_time/1000, clk_lp11_time%1000, dsi_timing->clane.lp11);
    #endif

    return 0;
}

static int32_t _dsi_phy_clk_lp01_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_CLK_LP01_BASE 50000 //50ns
    #define MC_CLK_LP01_OFFSET 100000 // 100ns

    int32_t rtn = 0x00;
    uint32_t clk_lp01_time = 0x00;
    uint32_t clk_lp01_reg = 0x00;

    /*
    MIN_CLK_LP01_BASE is time(ns), the value is time(ns) * 1000;

    clk_lp01_reg is register value, the register value mean 8*UI

    lp01 mean ltpx
    */

    clk_lp01_time = MIN_CLK_LP01_BASE + MC_CLK_LP01_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_lp01_time, UI, &clk_lp01_reg);

    rtn = _reg_to_ui_time(clk_lp01_reg, UI, &clk_lp01_time);

    dsi_timing->clane.lp01 = clk_lp01_reg;

    #if TIMING_DEBUG_LOG
    printf("clk lp_01       :%d.%dns, reg:0x%x\n", clk_lp01_time/1000, clk_lp01_time%1000, dsi_timing->clane.lp01);
    #endif

    return rtn;
}

static int32_t _dsi_phy_clk_prepare_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_CLK_PREPARE_BASE 38000
    #define MAX_CLK_PREPARE_BASE 95000

    int32_t rtn = 0x00;
    uint32_t min_clk_prepare = 0x00;
    uint32_t max_clk_prepare = 0x00;
    uint32_t clk_prepare_time = 0x00;
    uint32_t clk_prepare_reg = 0x00;

    /*
    MIN_CLK_PREPARE_BASE and MAX_CLK_PREPARE_BASE is time(ns), the value is time(ns) * 1000;

    min_clk_prepare and max_clk_prepare is register value, the register value mean 8*UI
    */

    rtn = _ui_time_to_reg_carry(MIN_CLK_PREPARE_BASE, UI, &min_clk_prepare);
    rtn = _ui_time_to_reg_truncation(MAX_CLK_PREPARE_BASE, UI, &max_clk_prepare);

    clk_prepare_reg = min_clk_prepare + ((max_clk_prepare - min_clk_prepare + 1) >> 1);

    rtn = _reg_to_ui_time(clk_prepare_reg, UI, &clk_prepare_time);

    dsi_timing->clane.prepare = clk_prepare_reg;

    #if TIMING_DEBUG_LOG
    printf("clk hs_prepare  :%d.%dns, reg:0x%x\n",
                                                clk_prepare_time/1000, clk_prepare_time%1000,
                                                dsi_timing->clane.prepare);
    #endif

    return rtn;
}

static int32_t _dsi_phy_clk_zeros_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define CLK_PREPARE_ZERO_BASE 300000 //300ns
#define MC_PREPARE_ZERO_OFFSET 100000 //100ns

    int32_t rtn = 0x00;
    uint32_t clk_prepare_time = 0x00;
    uint32_t clk_zero_time = 0x00;
    uint32_t clk_zero_reg = 0x00;
    uint32_t min_clk_zero_time = 0x00;

    rtn = _reg_to_ui_time(dsi_timing->clane.prepare, UI, &clk_prepare_time);

    min_clk_zero_time = CLK_PREPARE_ZERO_BASE - clk_prepare_time;

    clk_zero_time = min_clk_zero_time + MC_PREPARE_ZERO_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_zero_time, UI, &clk_zero_reg);

    rtn = _reg_to_ui_time(clk_zero_reg, UI, &clk_zero_time);

    dsi_timing->clane.zero = clk_zero_reg;

    if (rtn) {

    }
    #if TIMING_DEBUG_LOG
    printf("clk hs_zero     :%d.%dns, reg:0x%x\n",
                                                clk_zero_time/1000, clk_zero_time%1000,
                                                dsi_timing->clane.zero);
    #endif

    return 0;
}

static int32_t _dsi_phy_clk_pre_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_CLK_PRE_BASE 5000 //5ns

#define MC_CLK_PRE_OFFSET 10000 // 10ns
#define MC_CLK_PRE_UI_NUM 24

    int32_t rtn = 0x00;
    uint32_t clk_pre_time = 0x00;
    uint32_t clk_pre_reg = 0x00;

    /*
    MIN_CLK_PRE_BASE is time(ns), the value is time(ns) * 1000;

    clk_pre_reg is register value, the register value mean 8*UI
    */

    clk_pre_time = MIN_CLK_PRE_BASE + (MC_CLK_PRE_UI_NUM * UI) + MC_CLK_PRE_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_pre_time, UI, &clk_pre_reg);

    rtn = _reg_to_ui_time(clk_pre_reg, UI, &clk_pre_time);

    dsi_timing->clane.pre = clk_pre_reg;

    #if TIMING_DEBUG_LOG
    printf("clk pre         :%d.%dns, reg:0x%x\n", clk_pre_time/1000, clk_pre_time%1000, dsi_timing->clane.pre);
    #endif

    return rtn;
}

static int32_t _dsi_phy_clk_post_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_CLK_POST_BASE 60000 //60ns
#define MIN_CLK_POST_UI_NUM 52
#define MC_CLK_POST_OFFSET 30000 // 30ns

    int32_t rtn = 0x00;
    uint32_t clk_post_time = 0x00;
    uint32_t clk_post_reg = 0x00;

    /*
    MIN_CLK_POST_BASE is time(ns), the value is time(ns) * 1000;

    clk_post_reg is register value, the register value mean 8*UI
    */

    clk_post_time = MIN_CLK_POST_BASE + (MIN_CLK_POST_UI_NUM * UI) + MC_CLK_POST_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_post_time, UI, &clk_post_reg);

    rtn = _reg_to_ui_time(clk_post_reg, UI, &clk_post_time);

    dsi_timing->clane.post = clk_post_reg;

    #if TIMING_DEBUG_LOG
    printf("clk post        :%d.%dns, reg:0x%x\n", clk_post_time/1000, clk_post_time%1000, dsi_timing->clane.post);
    #endif

    return rtn;
}

static int32_t _dsi_phy_clk_trail_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_CLK_TRAIL_BASE 60000 //60ns
    #define MC_CLK_TRAIL_OFFSET 30000 //60ns
    int32_t rtn = 0x00;
    uint32_t clk_trail_time = 0x00;
    uint32_t clk_trail_reg = 0x00;

    /*
    MIN_CLK_TRAIL_BASE is time(ns), the value is time(ns) * 1000;

    clk_trail_reg is register value, the register value mean 8*UI
    */

    clk_trail_time = MIN_CLK_TRAIL_BASE + MC_CLK_TRAIL_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_trail_time, UI, &clk_trail_reg);

    rtn = _reg_to_ui_time(clk_trail_reg, UI, &clk_trail_time);

    dsi_timing->clane.trail = clk_trail_reg;

    #if TIMING_DEBUG_LOG
    printf("clk trail       :%d.%dns, reg:0x%x\n", clk_trail_time/1000, clk_trail_time%1000, dsi_timing->clane.trail);
    #endif

    return rtn;
}

static int32_t _dsi_phy_clk_inittime_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_CLK_INIT_TIME_BASE 100000 //100ns
#define MC_CLK_INIT_TIME_OFFSET 50000 // 50ns

    int32_t rtn = 0x00;
    uint32_t clk_init_time = 0x00;
    uint32_t clk_init_time_reg = 0x00;

    /*
    MIN_CLK_INIT_TIME_BASE is time(ns), the value is time(ns) * 1000;

    clk_init_time_reg is register value, the register value mean 8*UI
    */

    clk_init_time = MIN_CLK_INIT_TIME_BASE + MC_CLK_INIT_TIME_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_init_time, UI, &clk_init_time_reg);

    rtn = _reg_to_ui_time(clk_init_time_reg, UI, &clk_init_time);

    dsi_timing->clane.inittime = clk_init_time_reg;

    #if TIMING_DEBUG_LOG
    printf("clk init time   :%d.%dns, reg:0x%x\n", clk_init_time/1000, clk_init_time%1000, dsi_timing->clane.inittime);
    #endif

    return rtn;
}

static int32_t _dsi_phy_clk_exit_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_CLK_EXIT_BASE 100000 //100ns
#define MC_CLK_EXIT_OFFSET 50000 // 50ns

    int32_t rtn = 0x00;
    uint32_t clk_exit_time = 0x00;
    uint32_t clk_exit_reg = 0x00;

    /*
    MIN_CLK_EXIT_BASE is time(ns), the value is time(ns) * 1000;

    clk_exit_reg is register value, the register value mean 8*UI
    */

    clk_exit_time = MIN_CLK_EXIT_BASE + MC_CLK_EXIT_OFFSET;

    rtn = _ui_time_to_reg_carry(clk_exit_time, UI, &clk_exit_reg);

    rtn = _reg_to_ui_time(clk_exit_reg, UI, &clk_exit_time);

    dsi_timing->clane.exit = clk_exit_reg;

    #if TIMING_DEBUG_LOG
    printf("clk exit        :%d.%dns, reg:0x%x\n", clk_exit_time/1000, clk_exit_time%1000, dsi_timing->clane.exit);
    #endif

    return rtn;
}

static int32_t _dsi_phy_data_lp11_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_DATA_LP11_BASE 50000 //50ns
    #define MC_DATA_LP11_OFFSET 100000 // 100ns

    int32_t rtn = 0x00;
    uint32_t data_lp11_time = 0x00;
    uint32_t data_lp11_reg = 0x00;

    /*
    MIN_DATA_LP01_BASE is time(ns), the value is time(ns) * 1000;

    min_clk_prepare and max_clk_prepare is register value, the register value mean 8*UI

    lp01 mean ltpx
    */

    data_lp11_time = MIN_DATA_LP11_BASE + MC_DATA_LP11_OFFSET;

    rtn = _ui_time_to_reg_carry(data_lp11_time, UI, &data_lp11_reg);

    rtn = _reg_to_ui_time(data_lp11_reg, UI, &data_lp11_time);

    dsi_timing->dlane.lp11 = data_lp11_reg;

    if (rtn) {

    }

    #if TIMING_DEBUG_LOG
    printf("data lp_11      :%d.%dns, reg:0x%x\n", data_lp11_time/1000, data_lp11_time%1000, dsi_timing->dlane.lp11);
    #endif

    return 0;
}

static int32_t _dsi_phy_data_lp01_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_DATA_LP01_BASE 50000 //50ns
    #define MC_DATA_LP01_OFFSET 100000 // 100ns

    int32_t rtn = 0x00;
    uint32_t data_lp01_time = 0x00;
    uint32_t data_lp01_reg = 0x00;

    /*
    MIN_DATA_LP01_BASE is time(ns), the value is time(ns) * 1000;

    min_clk_prepare and max_clk_prepare is register value, the register value mean 8*UI

    lp01 mean ltpx
    */

    data_lp01_time = MIN_DATA_LP01_BASE + MC_DATA_LP01_OFFSET;

    rtn = _ui_time_to_reg_carry(data_lp01_time, UI, &data_lp01_reg);

    rtn = _reg_to_ui_time(data_lp01_reg, UI, &data_lp01_time);

    dsi_timing->dlane.lp01 = data_lp01_reg;

    if (rtn) {

    }

    #if TIMING_DEBUG_LOG
    printf("data lp_01      :%d.%dns, reg:0x%x\n", data_lp01_time/1000, data_lp01_time%1000, dsi_timing->dlane.lp01);
    #endif

    return 0;
}

static int32_t _dsi_phy_data_prepare_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_DATA_PREPARE_BASE 40000
    #define MIN_DATA_PREPARE_UI_NUM 4
    #define MAX_DATA_PREPARE_BASE 85000
    #define MAX_DATA_PREPARE_UI_NUM 6

    int32_t rtn = 0x00;
    uint32_t min_data_prepare_time = 0x00;
    uint32_t max_data_prepare_time = 0x00;
    uint32_t min_data_prepare_reg = 0x00;
    uint32_t max_data_prepare_reg = 0x00;
    uint32_t data_prepare_time = 0x00;
    uint32_t data_prepare_reg = 0x00;

    min_data_prepare_time = MIN_DATA_PREPARE_BASE + (UI * MIN_DATA_PREPARE_UI_NUM);
    max_data_prepare_time = MAX_DATA_PREPARE_BASE + (UI * MAX_DATA_PREPARE_UI_NUM);

    rtn = _ui_time_to_reg_carry(min_data_prepare_time, UI, &min_data_prepare_reg);
    rtn = _ui_time_to_reg_truncation(max_data_prepare_time, UI, &max_data_prepare_reg);

    data_prepare_reg= min_data_prepare_reg + ((max_data_prepare_reg - min_data_prepare_reg + 1) >> 1);

    rtn = _reg_to_ui_time(data_prepare_reg, UI, &data_prepare_time);

    dsi_timing->dlane.prepare = data_prepare_reg;

    #if TIMING_DEBUG_LOG
    printf("data hs_prepare :%d.%dns, reg:0x%x\n",
                                                data_prepare_time/1000, data_prepare_time%1000,
                                                dsi_timing->dlane.prepare);
    #endif

    return rtn;
}

static int32_t _dsi_phy_data_zero_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define DATA_PREPARE_ZERO_BASE 145000 //ns
#define DATA_PREPARE_ZERO_UI_NUM 10

    int32_t rtn = 0x00;
    uint32_t data_prepare_time = 0x00;
    uint32_t data_zero_time = 0x00;
    uint32_t data_zero_reg = 0x00;
    uint32_t min_data_zero_time = 0x00;

    rtn = _reg_to_ui_time(dsi_timing->dlane.prepare, UI, &data_prepare_time);

    min_data_zero_time = DATA_PREPARE_ZERO_BASE - data_prepare_time;

    data_zero_time = min_data_zero_time + (DATA_PREPARE_ZERO_BASE >> 2) + (DATA_PREPARE_ZERO_UI_NUM * UI);

    rtn = _ui_time_to_reg_carry(data_zero_time, UI, &data_zero_reg);

    rtn = _reg_to_ui_time(data_zero_reg, UI, &data_zero_time);

    dsi_timing->dlane.zero = data_zero_reg;

    if (rtn) {

    }

    #if TIMING_DEBUG_LOG
    printf("data hs_zero    :%d.%dns, reg:0x%x\n",
                                                data_zero_time/1000, data_zero_time%1000,
                                                dsi_timing->dlane.zero);
    #endif

    return 0;
}

static int32_t _dsi_phy_data_trail_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
    #define MIN_DATA_TRAIL_BASE 60000 //60ns
    #define DATA_TRAIL_UI_NUM0 8
    #define DATA_TRAIL_UI_NUM1 4
    #define MC_DATA_TRAIL_OFFSET 30000 // 30ns

    int32_t rtn = 0x00;
    uint32_t data_trail_time = 0x00;
    uint32_t data_trail_reg = 0x00;

    /*
    MIN_DATA_TRAIL_BASE is time(ns), the value is time(ns) * 1000;

    data_trail_reg is register value, the register value mean 8*UI
    */

    if ((DATA_TRAIL_UI_NUM0 * UI) < (MIN_DATA_TRAIL_BASE + (DATA_TRAIL_UI_NUM1 * UI))) {
        data_trail_time = MIN_DATA_TRAIL_BASE + (DATA_TRAIL_UI_NUM1 * UI);

    } else {
        data_trail_time = DATA_TRAIL_UI_NUM0 * UI;
    }
    data_trail_time += MC_DATA_TRAIL_OFFSET;

    rtn = _ui_time_to_reg_carry(data_trail_time, UI, &data_trail_reg);

    rtn = _reg_to_ui_time(data_trail_reg, UI, &data_trail_time);

    dsi_timing->dlane.trail = data_trail_reg;

    #if TIMING_DEBUG_LOG
    printf("data trail      :%d.%dns, reg:0x%x\n", data_trail_time/1000, data_trail_time%1000, dsi_timing->dlane.trail);
    #endif

    return rtn;
}

static int32_t _dsi_phy_data_inittime_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_DATA_INIT_TIME_BASE 100000 //100ns
#define MC_DATA_INIT_TIME_OFFSET 50000 // 50ns

    int32_t rtn = 0x00;
    uint32_t data_init_time = 0x00;
    uint32_t data_init_time_reg = 0x00;

    /*
    MIN_DATA_INIT_TIME_BASE is time(ns), the value is time(ns) * 1000;

    data_init_time_reg is register value, the register value mean 8*UI
    */

    data_init_time = MIN_DATA_INIT_TIME_BASE + MC_DATA_INIT_TIME_OFFSET;

    rtn = _ui_time_to_reg_carry(data_init_time, UI, &data_init_time_reg);

    rtn = _reg_to_ui_time(data_init_time_reg, UI, &data_init_time);

    dsi_timing->dlane.inittime = data_init_time_reg;

    #if TIMING_DEBUG_LOG
    printf("data init time  :%d.%dns, reg:0x%x\n", data_init_time/1000, data_init_time%1000, dsi_timing->dlane.inittime);
    #endif

    return rtn;
}

static int32_t _dsi_phy_data_exit_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_DATA_EXIT_BASE 100000 //100ns
#define MC_DATA_EXIT_OFFSET 50000 // 50ns

    int32_t rtn = 0x00;
    uint32_t data_exit_time = 0x00;
    uint32_t data_exit_reg = 0x00;

    /*
    MIN_DATA_EXIT_BASE is time(ns), the value is time(ns) * 1000;

    data_exit_reg is register value, the register value mean 8*UI
    */

    data_exit_time = MIN_DATA_EXIT_BASE + MC_DATA_EXIT_OFFSET;

    rtn = _ui_time_to_reg_carry(data_exit_time, UI, &data_exit_reg);

    rtn = _reg_to_ui_time(data_exit_reg, UI, &data_exit_time);

    dsi_timing->dlane.exit = data_exit_reg;

    #if TIMING_DEBUG_LOG
    printf("data exit       :%d.%dns, reg:0x%x\n", data_exit_time/1000, data_exit_time%1000, dsi_timing->dlane.exit);
    #endif

    return rtn;
}

static int32_t _dsi_phy_read_tago_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define READ_TA_GO_TLPX_NUM 4

    int32_t rtn = 0x00;
    uint32_t data_lp01_time = 0x00;
    uint32_t read_tago_time = 0x00;
    uint32_t read_tago_reg = 0x00;

    /*
    read_tago_reg is register value, the register value mean 8*UI
    */

    rtn = _reg_to_ui_time(dsi_timing->dlane.lp01, UI, &data_lp01_time);

    read_tago_time = READ_TA_GO_TLPX_NUM * data_lp01_time;

    rtn = _ui_time_to_reg_carry(read_tago_time, UI, &read_tago_reg);

    rtn = _reg_to_ui_time(read_tago_reg, UI, &read_tago_time);

    dsi_timing->read.tago = read_tago_reg;

    #if TIMING_DEBUG_LOG
    printf("read ta go      :%d.%dns, reg:0x%x\n", read_tago_time/1000, read_tago_time%1000, dsi_timing->read.tago);
    #endif

    return rtn;
}

static int32_t _dsi_phy_read_tasure_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define MIN_READ_TA_SURE_TLPX_NUM 1
#define MAX_READ_TA_SURE_TLPX_NUM 2

    int32_t rtn = 0x00;
    uint32_t data_lp01_time = 0x00;

    uint32_t max_read_tasure_time = 0x00;
    uint32_t min_read_tasure_time = 0x00;
    uint32_t read_tasure_time = 0x00;
    uint32_t read_tasure_reg = 0x00;

    /*
    read_tasure_reg is register value, the register value mean 8*UI
    */

    rtn = _reg_to_ui_time(dsi_timing->dlane.lp01, UI, &data_lp01_time);

    min_read_tasure_time = MIN_READ_TA_SURE_TLPX_NUM * data_lp01_time;
    max_read_tasure_time = MAX_READ_TA_SURE_TLPX_NUM * data_lp01_time;
    read_tasure_time = min_read_tasure_time + ((max_read_tasure_time - min_read_tasure_time) >> 1);

    rtn = _ui_time_to_reg_carry(read_tasure_time, UI, &read_tasure_reg);

    rtn = _reg_to_ui_time(read_tasure_reg, UI, &read_tasure_time);

    dsi_timing->read.tasure = read_tasure_reg;

    #if TIMING_DEBUG_LOG
    printf("read ta sure    :%d.%dns, reg:0x%x\n", read_tasure_time/1000, read_tasure_time%1000, dsi_timing->read.tasure);
    #endif

    return rtn;
}

static int32_t _dsi_phy_read_taget_calc(dsi_timing_t *dsi_timing, uint32_t UI)
{
#define READ_TA_GET_TLPX_NUM 5

    int32_t rtn = 0x00;
    uint32_t data_lp01_time = 0x00;
    uint32_t read_taget_time = 0x00;
    uint32_t read_taget_reg = 0x00;

    /*
    read_taget_reg is register value, the register value mean 8*UI
    */

    rtn = _reg_to_ui_time(dsi_timing->dlane.lp01, UI, &data_lp01_time);

    read_taget_time = READ_TA_GET_TLPX_NUM * data_lp01_time;

    rtn = _ui_time_to_reg_carry(read_taget_time, UI, &read_taget_reg);

    rtn = _reg_to_ui_time(read_taget_reg, UI, &read_taget_time);

    dsi_timing->read.taget = read_taget_reg;

    #if TIMING_DEBUG_LOG
    printf("read ta get     :%d.%dns, reg:0x%x\n", read_taget_time/1000, read_taget_time%1000, dsi_timing->read.taget);
    #endif

    return rtn;
}

int32_t dsi_phy_timing_calc(dsi_phy_timing_in_t *inParam, dsi_phy_timing_out_t *outParam)
{
    int32_t rtn = 0x00;
    dsi_timing_t *timing_ptr = inParam->timing;
    uint32_t UI = 0x00;
    dsi_phy_ui_calc_in_t ui_calc_in = {0x00};
    dsi_phy_ui_calc_out_t ui_calc_out = {0x00};

    #if TIMING_DEBUG_LOG
    printf("\n");
    printf("-----------------mipi timing calc --------------------\n");
    #endif

    if (0x00 == timing_ptr->clane.mode) {
        ui_calc_in.lane_clk = inParam->lane_clk;
        rtn = _dsi_phy_ui_calc(&ui_calc_in, &ui_calc_out);
        UI = ui_calc_out.UI;
    }

    #if TIMING_DEBUG_LOG
    printf("timing_ptr->clane.mode:%d\n", timing_ptr->clane.mode);
    #endif

    if (0x00 == timing_ptr->clane.mode) {
        /*auto calc mipi clk timing*/
        rtn = _dsi_phy_clk_lp11_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_lp01_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_prepare_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_zeros_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_pre_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_post_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_trail_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_inittime_calc(timing_ptr, UI);
        rtn = _dsi_phy_clk_exit_calc(timing_ptr, UI);
    }

    #if TIMING_DEBUG_LOG
    printf("timing_ptr->dlane.mode:%d\n", timing_ptr->dlane.mode);
    #endif

    if (0x00 == timing_ptr->dlane.mode) {
        /*auto calc mipi data timing*/
        rtn = _dsi_phy_data_lp11_calc(timing_ptr, UI);
        rtn = _dsi_phy_data_lp01_calc(timing_ptr, UI);
        rtn = _dsi_phy_data_prepare_calc(timing_ptr, UI);
        rtn = _dsi_phy_data_zero_calc(timing_ptr, UI);
        rtn = _dsi_phy_data_trail_calc(timing_ptr, UI);
        rtn = _dsi_phy_data_inittime_calc(timing_ptr, UI);
        rtn = _dsi_phy_data_exit_calc(timing_ptr, UI);
    }

    #if TIMING_DEBUG_LOG
    printf("timing_ptr->read.mode:%d\n", timing_ptr->read.mode);
    #endif

    if (0x00 == timing_ptr->read.mode) {
        /*auto calc mipi read timing*/
        rtn = _dsi_phy_read_tago_calc(timing_ptr, UI);
        rtn = _dsi_phy_read_tasure_calc(timing_ptr, UI);
        rtn = _dsi_phy_read_taget_calc(timing_ptr, UI);
    }

    #if TIMING_DEBUG_LOG
    printf("-----------------mipi timing calc end--------------------\n");
    printf("\n");
    #endif

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
