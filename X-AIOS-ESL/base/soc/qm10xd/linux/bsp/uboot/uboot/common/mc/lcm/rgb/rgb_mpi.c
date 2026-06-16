#include "../pin_mux/pin_lcd_v01_drv.h"
#include "../../lcm_module/include/mol_lcm_rgb.h"
#include "../../lcm_module/include/rgb_com_param.h"
#include "rgb_mpi_param.h"
#include "../clk/vou_clk_v01_drv.h"
#include "../debug.h"
#include "../dsi/dsi_phy_v0_pll_calc_param.h"
#include "../dsi/dsi_phy_v0_pll_calc.h"
#include "../dsi/dsi_host_v01_drv_param.h"
#include "../dsi/dsi_phy_v01_drv_mpi.h"
#include "../reg/reg.h"

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
#define RGB_FUN_DEBUG 0
#define RGB_DEBUG 0

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct {
    rgb_dsi_state_e status;
    uint32_t dhd_id;

    ADDR rst_base_addr;
    ADDR clk_base_addr;
    ADDR mif_base_addr;
    ADDR vou_base_addr;
    ADDR vou_sel_addr;
    ADDR pin_base_addr;

    lcm_pin_type_e lcm_pin_type;
    rgb_init_in_t initIn;
    rgb_init_out_t initOut;
    rgb_deinit_in_t deInitIn;
    rgb_deinit_out_t deInitOut;

    vou_clk_calc_in_t vouClkIn;
    vou_clk_calc_out_t vouClkOut;

    dsi_phy_pll_calc_in_t mifClkIn;
    dsi_phy_pll_calc_out_t mifClkOut;

    lcm_clk_on_in_t lcmClkOnIn;
    lcm_clk_on_out_t lcmClkOnOut;
    lcm_clk_off_in_t lcmClkOffIn;
    lcm_clk_off_out_t lcmClkOffOut;

    dsi_phy_power_on_in_t phyPowerOnIn;
    dsi_phy_power_on_out_t phyPowerOnOut;

    dsi_phy_pll_on_in_t phyPllCfgIn;
    dsi_phy_pll_on_out_t phyPllCfgOut;

    uint32_t host_sync_div;

}rgb_mpi_context_t;

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
rgb_mpi_context_t s_rgb_mpi_context = {0x00};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
rgb_mpi_context_t *rgb_mipi_context_get(void)
{
    return &s_rgb_mpi_context;
}

static int32_t _rgb_lane_num_format_get(lcm_rgb_if_type_e if_type, lcm_display_format_e rgb_format, uint32_t *lane_num, uint32_t *dsi_dpi_format)
{
    int32_t rtn = SUCCESS;

    switch(if_type) {
        case RGB_IF_18BITS:
            *lane_num = 17;
            *dsi_dpi_format = DSI_DPI_RGB666;
        break;
        case RGB_IF_16BITS:
            *lane_num = 15;
            *dsi_dpi_format = DSI_DPI_RGB565;
        break;
        case RGB_IF_6BITS:
        {
            switch(rgb_format) {
                case LCM_RGB_888:
                    *lane_num = 5;
                    *dsi_dpi_format = DSI_DPI_RGB888;
                break;
                case LCM_RGB_666:
                case LCM_RGB_565:
                    *lane_num = 5;
                    *dsi_dpi_format = DSI_DPI_RGB666;
                break;
                case LCM_RGB_GRAY6:
                    *lane_num = 17;
                    *dsi_dpi_format = DSI_DPI_RGB666;
                break;
                default:
                break;
            }
        }
        break;
        case RGB_IF_24BITS:
            *lane_num = 23;
            *dsi_dpi_format = DSI_DPI_RGB888;
        break;
        default:
        break;
    }

    return rtn;
}

static lcm_pin_type_e _rgb_pin_type_get(lcm_rgb_if_type_e if_type)
{
    lcm_pin_type_e lcm_pin_type = lcm_pin_rgb_24;

    switch(if_type) {
        case RGB_IF_18BITS:
            lcm_pin_type = lcm_pin_rgb_18;
        break;
        case RGB_IF_16BITS:
            lcm_pin_type = lcm_pin_rgb_16;
        break;
        case RGB_IF_6BITS:
            lcm_pin_type = lcm_pin_rgb_6;
        break;
        case RGB_IF_24BITS:
            lcm_pin_type = lcm_pin_rgb_24;
        break;
        default:
        break;
    }

    return lcm_pin_type;
}


static int32_t _rgb_init_param_cfg(rgb_init_in_t *inParam)
{
    int32_t rtn = SUCCESS;
    rgb_mpi_context_t *rgbContext = rgb_mipi_context_get();
    uint32_t frame_total_w = 0x00;
    uint32_t frame_total_h = 0x00;

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "rgb_init_in_t is null ~_~\n");

    memcpy((void *)&rgbContext->initIn, (void *)inParam, sizeof(rgb_init_in_t));

    rgbContext->dhd_id = rgbContext->initIn.lcd_id;
    rgbContext->rst_base_addr = rgbContext->initIn.rst_base_addr;
    rgbContext->clk_base_addr = rgbContext->initIn.clk_base_addr;
    rgbContext->vou_sel_addr = rgbContext->initIn.vou_sel_addr;
    rgbContext->mif_base_addr = rgbContext->initIn.mif_base_addr;
    rgbContext->vou_base_addr = rgbContext->initIn.vou_base_addr;
    rgbContext->pin_base_addr = rgbContext->initIn.pin_base_addr;
    rgbContext->phyPllCfgIn.phy_addr = rgbContext->initIn.mif_base_addr;

    rgbContext->lcm_pin_type = _rgb_pin_type_get(rgbContext->initIn.if_type);

    rgbContext->vouClkIn.clkInfo.mode = 1/*rgbContext->initIn.clk.vou.mode*/;
    rgbContext->vouClkIn.clkInfo.cfg.sel = 0x3/*rgbContext->initIn.clk.vou.sel*/;
    rgbContext->vouClkIn.clkInfo.cfg.div = 0/*rgbContext->initIn.clk.vou.div*/;

    frame_total_w = rgbContext->initIn.dp_sync_info.hor_sync_active
                    + rgbContext->initIn.dp_sync_info.hor_back_porch
                    + rgbContext->initIn.dp_sync_info.hor_active_pixel
                    + rgbContext->initIn.dp_sync_info.hor_front_porch ;

    frame_total_h = rgbContext->initIn.dp_sync_info.ver_sync_active
                    + rgbContext->initIn.dp_sync_info.ver_back_porch
                    + rgbContext->initIn.dp_sync_info.ver_active_line
                    + rgbContext->initIn.dp_sync_info.ver_front_porch;

    rgbContext->vouClkIn.total_pixel = frame_total_w * frame_total_h * rgbContext->initIn.fps;

    if (RGB_IF_6BITS == rgbContext->initIn.if_type){
        rgbContext->vouClkIn.tx_frq_pixel = 3;
    } else {
        rgbContext->vouClkIn.tx_frq_pixel = 1;
    }
    rtn = vou_clk_calc(&rgbContext->vouClkIn, &rgbContext->vouClkOut);
    EXIT_IF_FAIL(rtn, NULL, "vou_clk_calc ~_~!\n");

    rgbContext->lcmClkOnIn.clk_base_addr = rgbContext->clk_base_addr;
    rgbContext->lcmClkOnIn.rst_base_addr = rgbContext->rst_base_addr;
    rgbContext->lcmClkOnIn.clk_sel = rgbContext->vouClkOut.cfg.sel;
    rgbContext->lcmClkOnIn.clk_div = rgbContext->vouClkOut.cfg.div;

    rgbContext->lcmClkOffIn.clk_base_addr = rgbContext->clk_base_addr;
    rgbContext->lcmClkOffIn.rst_base_addr = rgbContext->rst_base_addr;

    rgbContext->mifClkIn.dp_sync = &rgbContext->initIn.dp_sync_info;
    rgbContext->mifClkIn.fps = rgbContext->initIn.fps;
    rgbContext->mifClkIn.clk = &rgbContext->initIn.clk.mif;
    rtn = _rgb_lane_num_format_get(rgbContext->initIn.if_type, rgbContext->initIn.format, &rgbContext->mifClkIn.laneNum, &rgbContext->mifClkIn.format);
    rgbContext->mifClkOut.host_sync_div = &rgbContext->host_sync_div;
    rtn = dsi_phy_pll_cfg_get(&rgbContext->mifClkIn, &rgbContext->mifClkOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_pll_cfg_get ~_~!\n");


    rgbContext->phyPllCfgIn.phyId = rgbContext->initIn.lcd_id;
    rgbContext->phyPllCfgIn.phy_addr = rgbContext->initIn.mif_base_addr;
    rgbContext->phyPllCfgIn.vou_addr = rgbContext->initIn.vou_base_addr;
    rgbContext->phyPllCfgIn.pll_doubler = rgbContext->mifClkOut.pll_doubler;
    rgbContext->phyPllCfgIn.pll_div_s = rgbContext->mifClkOut.pll_div_s;
    rgbContext->phyPllCfgIn.pll_n = rgbContext->mifClkOut.pll_n;
    rgbContext->phyPllCfgIn.pll_kint = rgbContext->mifClkOut.pll_kint;
    rgbContext->phyPllCfgIn.pll_nint = rgbContext->mifClkOut.pll_nint;
    rgbContext->phyPllCfgIn.pll_pdiv = rgbContext->mifClkOut.pll_pdiv;
    rgbContext->phyPllCfgIn.pixelclk_div = rgbContext->mifClkOut.pixelclk_div;
    rgbContext->phyPllCfgIn.dhd_div = rgbContext->mifClkOut.dhd_div;
    rgbContext->phyPllCfgIn.lane_clk = rgbContext->mifClkOut.lane_clk;

    #if RGB_DEBUG
    printf("rgbContext->lcd_id          : %d\n", rgbContext->initIn.lcd_id);
    printf("rgbContext->frame size      : %d, %d\n", frame_total_w, frame_total_h);

    printf("rgbContext->total_pixel     : %d\n", rgbContext->vouClkIn.total_pixel);
    printf("rgbContext->tx_frq_pixel    : %d\n", rgbContext->vouClkIn.tx_frq_pixel);

    printf("rgbContext->rst_base_addr   : 0x%x\n", rgbContext->initIn.rst_base_addr  );
    printf("rgbContext->clk_base_addr   : 0x%x\n", rgbContext->initIn.clk_base_addr  );
    printf("rgbContext->vou_sel_addr    : 0x%x\n", rgbContext->initIn.vou_sel_addr   );
    printf("rgbContext->mif_base_addr   : 0x%x\n", rgbContext->initIn.mif_base_addr   );
    printf("rgbContext->vou_base_addr   : 0x%x\n", rgbContext->initIn.vou_base_addr   );
    printf("rgbContext->pin_base_addr   : 0x%x\n", rgbContext->initIn.pin_base_addr   );
    printf("rgbContext->fps             : %d\n", rgbContext->initIn.fps             );
    printf("rgbContext->format          : %d\n", rgbContext->initIn.format          );
    printf("rgbContext->if_type         : %d\n", rgbContext->initIn.if_type          );

    printf("rgbContext->lcm_pin_type    : %d\n", rgbContext->lcm_pin_type          );
    printf("rgbContext->mif_laneNum     : %d\n", rgbContext->mifClkIn.laneNum          );
    printf("rgbContext->mif_format      : %d\n", rgbContext->mifClkIn.format          );

    printf("rgbContext->hor_sync_active : %d\n", rgbContext->initIn.dp_sync_info.hor_sync_active );
    printf("rgbContext->hor_back_porch  : %d\n", rgbContext->initIn.dp_sync_info.hor_back_porch  );
    printf("rgbContext->hor_active_pixel: %d\n", rgbContext->initIn.dp_sync_info.hor_active_pixel);
    printf("rgbContext->hor_front_porch : %d\n", rgbContext->initIn.dp_sync_info.hor_front_porch );
    printf("rgbContext->ver_sync_active : %d\n", rgbContext->initIn.dp_sync_info.ver_sync_active );
    printf("rgbContext->ver_back_porch  : %d\n", rgbContext->initIn.dp_sync_info.ver_back_porch  );
    printf("rgbContext->ver_active_line : %d\n", rgbContext->initIn.dp_sync_info.ver_active_line );
    printf("rgbContext->ver_front_porch : %d\n", rgbContext->initIn.dp_sync_info.ver_front_porch );

    printf("rgbContext->pll_sel          : %d\n", rgbContext->vouClkOut.cfg.sel);
    printf("rgbContext->pll_div          : %d\n", rgbContext->vouClkOut.cfg.div);

    printf("rgbContext->mif_pll_doubler  : %d\n", rgbContext->phyPllCfgIn.pll_doubler);
    printf("rgbContext->mif_pll_div_s    : %d\n", rgbContext->phyPllCfgIn.pll_div_s);
    printf("rgbContext->mif_pll_n        : %d\n", rgbContext->phyPllCfgIn.pll_n);
    printf("rgbContext->mif_pll_kint     : %d\n", rgbContext->phyPllCfgIn.pll_kint);
    printf("rgbContext->mif_pll_nint     : %d\n", rgbContext->phyPllCfgIn.pll_nint);
    printf("rgbContext->mif_pll_pdiv     : %d\n", rgbContext->phyPllCfgIn.pll_pdiv);
    printf("rgbContext->mif_pixelclk_div : %d\n", rgbContext->phyPllCfgIn.pixelclk_div);
    printf("rgbContext->mif_dhd_div      : %d\n", rgbContext->phyPllCfgIn.dhd_div);
    printf("rgbContext->mif_lane_clk     : %d\n", rgbContext->phyPllCfgIn.lane_clk);
    printf("\n");

    #endif

    EXIT:

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _rgb_deinit_param_cfg(rgb_deinit_in_t *inParam)
{
    int32_t rtn = SUCCESS;
    //rgb_mpi_context_t *rgbContext = rgb_mipi_context_get();

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "rgb_deinit_in_t is null ~_~\n");

    EXIT:

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t rgb_init(rgb_init_in_t *inParam, rgb_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    rgb_mpi_context_t *rgbContext = rgb_mipi_context_get();

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "rgb_init_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "rgb_init_out_t is null ~_~\n");
    CHECK_PTR(rgbContext, NULL, "rgbContext is null ~_~!\n");

    if (RGB_INITED == rgbContext->status) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_init status(%d) ~_~!\n", rgbContext->status);
    }

    rtn = _rgb_init_param_cfg(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_rgb_init_param_cfg ~_~!\n");

    reg_log_enable(0);

    rtn = lcm_bus_pin_cfg(rgbContext->initIn.pin_base_addr, rgbContext->lcm_pin_type);
    EXIT_IF_FAIL(rtn, NULL, "lcm_bus_pin_cfg ~_~!\n");

    rtn = lcm_clk_on(&rgbContext->lcmClkOnIn, &rgbContext->lcmClkOnOut);
    EXIT_IF_FAIL(rtn, NULL, "lcm_clk_on ~_~!\n");

    rtn = dsi_phy_pll_v1_on(&rgbContext->phyPllCfgIn, &rgbContext->phyPllCfgOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_pll_cfg ~_~!\n");

    rtn =  vou_lcd_src_sel(rgbContext->vou_sel_addr, rgbContext->dhd_id);
    EXIT_IF_FAIL(rtn, NULL, "vou_lcd_src_sel ~_~!\n");

    rtn = vou_dhd_src_sel(rgbContext->vou_sel_addr, rgbContext->dhd_id, 1);
    EXIT_IF_FAIL(rtn, NULL, "vou_dhd_src_sel ~_~!\n");

    reg_log_enable(0);

    rgbContext->status = RGB_INITED;

    EXIT:

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t rgb_deinit(rgb_deinit_in_t *inParam, rgb_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    rgb_mpi_context_t *rgbContext = rgb_mipi_context_get();

    #if RGB_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "rgb_deinit_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "rgb_deinit_out_t is null ~_~\n");
    CHECK_PTR(rgbContext, NULL, "rgbContext is null ~_~!\n");

    if (RGB_UN_INITED == rgbContext->status) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_init status(%d) ~_~!\n", rgbContext->status);
    }

    rtn = _rgb_deinit_param_cfg(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_rgb_init_param_cfg ~_~!\n");

#if 1
    rtn = lcm_clk_off(&rgbContext->lcmClkOffIn, &rgbContext->lcmClkOffOut);
    EXIT_IF_FAIL(rtn, NULL, "lcm_clk_off ~_~!\n");
#else
    rtn = vou_clk_off(rgbContext->initIn.clk_base_addr);
    EXIT_IF_FAIL(rtn, NULL, "vou_clk_off ~_~!\n");
#endif

    rtn = lcm_bus_pin_default_cfg(rgbContext->initIn.pin_base_addr, rgbContext->lcm_pin_type);
    EXIT_IF_FAIL(rtn, NULL, "lcm_bus_pin_default_cfg ~_~!\n");

    rgbContext->status = RGB_UN_INITED;

    EXIT:

    #if RGB_FUN_DEBUG
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

