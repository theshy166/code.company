#include "mol_lcm_mcu_drv.h"
#include "mol_lcm_mcu_api.h"
#include "../../lcm_module/include/data_type.h"
#include "../reg/reg.h"
#include "../lcm_com.h"
#include "../../lcm_module/include/mol_lcm_local.h"
#include "../os.h"
#include "../debug.h"
#include "../clk/vou_clk_v01_drv.h"
#include "../pin_mux/pin_lcd_v01_drv.h"
#include "../dsi/dsi_host_v01_drv_param.h"
#include "../dsi/dsi_phy_v0_pll_calc.h"
#include "../dsi/dsi_phy_v01_drv_mpi.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef     __cplusplus
extern     "C"
{
#endif
#define LCM_MCU_API_DEBUG 0
#define LCM_MCU_FUN_DEBUG 0
#define LCM_MCU_PARAM_DEBUG 0
#define LCM_MCU_SETTING_DEBUG 0

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
lcm_mcu_api_context_t s_lcm_mcu_api_context = {0};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/

lcm_mcu_api_context_t *_lcmMcuApiGetConext(void)
{
    return &s_lcm_mcu_api_context;
}

lcm_mcu_api_local_t *_lcmMcuApiGetHandle(void)
{
    lcm_mcu_api_context_t *context = _lcmMcuApiGetConext();

    return context->handle;
}

int32_t _lcmMcuApiCheckHandle(lcm_mcu_api_local_t *handle)
{
    int32_t rtn = 1;
    uint32_t i = 0;
    lcm_mcu_api_local_t *_localHandle = _lcmMcuApiGetHandle();

    if (NULL == handle) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu handle is null ~_~!");
    }

    for (i = 0; i < LCM_MCU_HANDLE_NUM; i++) {
        //printk("handle->flag = %d----%d, %p, %p\n", handle->flag, i, _localHandle, handle);
        if (handle == _localHandle) {
            if (0 != _localHandle->flag){
                rtn = 0;
                break;
            }
            break;
        }
        _localHandle++;
    }

    EXIT:

    return rtn;
}

lcm_mcu_api_local_t *_lcmMcuApiGetValidHandle(void)
{
    int32_t rtn = 0;
    uint32_t i = 0;
    lcm_mcu_api_local_t *handle = _lcmMcuApiGetHandle();
    lcm_mcu_api_local_t *validHandle = NULL;

    if (NULL == handle) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu handle is null ~_~!");
    }

    for (i = 0; i < LCM_MCU_HANDLE_NUM; i++) {
        //printk("handle->flag = %d----%d\n", handle->flag, i);
        if (0 == handle->flag) {
            handle->flag = 1;
            validHandle = handle;
            break;
        }
        handle++;
    }

    EXIT:

    return validHandle;
}

int32_t _lcmMcuApiCheckStatus(lcm_mcu_drv_t *instance, uint32_t status, uint32_t timeOut)
{
    int32_t rtn = 0;
    uint32_t lcmBusStatus = 0;
    uint32_t i = 0;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    lcmBusStatus = lcmGetStageStatus(instance);
    while(lcmBusStatus == 0) {
        if (i==1){
            printf("wait to lcm switch sts done\n");
        }

        lcmBusStatus = lcmGetStageStatus(instance);
        if (status == lcmBusStatus) {
            break;
        }
    }

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiCheckCmdDone(lcm_mcu_drv_t *instance, uint32_t status)
{
    int32_t rtn = 0;
    uint32_t lcmBusStatus = 0;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    lcmBusStatus = lcmGetCmdStatus(instance);
    while(lcmBusStatus == 0) {
        lcmBusStatus = lcmGetCmdStatus(instance);

        if (status == lcmBusStatus) {
            break;
        }
    }

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiCheckRdFifo(lcm_mcu_drv_t *instance, uint32_t num, uint32_t timeOut)
{
    int32_t rtn = 0;
    uint32_t dataNum = 0;
    uint32_t i = 0;
    uint32_t _timeOut = LCM_MCU_STATUS_TIMEOUT * timeOut;
    uint32_t delayTime = LCM_MCU_OPS_TIME * timeOut;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    while(_timeOut >= i) {
        dataNum = lcmGetCmdRfifoDepth(instance);
        if (num == dataNum) {
            break;
        }
        delayUs(delayTime);
        if (_timeOut == i) {
            rtn = 1;
            EXIT_IF_FAIL(rtn, NULL, "lcm mcu api check rd fifo ~_~!");
        }
        i++;
    }

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiSetStatus(lcm_mcu_drv_t *instance, uint32_t status)
{
    int32_t rtn = 0;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    rtn = lcmSetStageStatus(instance, status);
    rtn = lcmSetStatusSwitchTrigger(instance);
    rtn = _lcmMcuApiCheckStatus(instance, status, 1);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api set status ~_~!");

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiSetTiming(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    lcm_mcu_drv_t *lcmMcuDrv = (lcm_mcu_drv_t *)instance;
    lcm_mcu_timing_t *timing = &lcmMcuDrv->timing;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    rtn = lcmSetCsType(instance, timing->csType);
    rtn = lcmSetTimingWrSetup(instance, timing->wrSetup);
    rtn = lcmSetTimingWrHold(instance, timing->wrHold);
    rtn = lcmSetTimingWrDestory(instance, timing->wrDestory);
    rtn = lcmSetTimingRdSetup(instance, timing->rdSetup);
    rtn = lcmSetTimingRdHold(instance, timing->rdHold);
    rtn = lcmSetTimingRdDestory(instance, timing->rdDestory);

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiSetIf(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    lcm_mcu_drv_t *lcmMcuDrv = (lcm_mcu_drv_t *)instance;
    lcm_mcu_if_t *port = &lcmMcuDrv->port;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    rtn = lcmSetBitMap(instance, port->mapTab);
    rtn = lcmSetMcuIfType(instance, port->mcuIfType);
    rtn = lcmSetRgbIfType(instance, port->rgbIfType);
    rtn = lcmSetSendEndain(instance, port->transEndian);
    rtn = lcmSetTrans3(instance, port->transNum);

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiSetFormat(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    lcm_mcu_drv_t *lcmMcuDrv = (lcm_mcu_drv_t *)instance;
    lcm_mcu_format_t *format = &lcmMcuDrv->format;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }


    rtn = lcmSetMcuBitDepth(instance, format->bitDepth);
    rtn = lcmSetMcuRgbOrder(instance, format->rgbOrder);
    rtn = lcmSetMcuMode(instance, LCM_MCU_MODE);

    EXIT:

    return rtn;
}


int32_t _lcmMcuApiSetTeType(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    lcm_mcu_drv_t *lcmMcuDrv = (lcm_mcu_drv_t *)instance;
    lcm_mcu_te_t *te = &lcmMcuDrv->te;

    if (NULL == instance) {
        rtn = 1;
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu instance is null ~_~!");
    }

    rtn = lcmSetTeType(instance, te->type);
    rtn = lcmSetMcuTeEdge(instance, te->edge);
    rtn = lcmSetMcuTeEnable(instance, te->eb);

    EXIT:

    return rtn;
}

int32_t _lcmMcuApiMapParamCfg(void* mcuSpec, lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    lcm_mcu_if_t *port = NULL;
    uint32_t i = 0;

    CHECK_PTR(instance, NULL, "lcm mcu api handle is null ~_~!");

    port = &instance->port;

    for (i = 0; i < 32; i++)
    {
        port->mapTab[i] = i;
    }

    EXIT:

    return rtn;
}

static int32_t _mcu_lane_num_format_get(lcm_mcu_if_type_e if_type, lcm_display_format_e rgb_format, uint32_t *lane_num, uint32_t *dsi_dpi_format)
{
    int32_t rtn = SUCCESS;

    switch(if_type) {
        case MCU_IF_18BITS:
            *lane_num = 8;
            *dsi_dpi_format = DSI_DPI_RGB666;
        break;
        case MCU_IF_16BITS:
            *lane_num = 7;
            *dsi_dpi_format = DSI_DPI_RGB565;
        break;
        case MCU_IF_9BITS:
        {
            *lane_num = 3;
            *dsi_dpi_format = DSI_DPI_RGB565;
        }
        break;
        case MCU_IF_8BITS:
            *lane_num = 3;
            *dsi_dpi_format = DSI_DPI_RGB565;
        break;
        default:
        break;
    }

    return rtn;
}


static lcm_pin_type_e _mcu_pin_type_get(lcm_mcu_if_type_e if_type)
{
    lcm_pin_type_e lcm_pin_type;

    switch(if_type) {
        case MCU_IF_18BITS:
            lcm_pin_type = lcm_pin_mcu_18;
        break;
        case MCU_IF_16BITS:
            lcm_pin_type = lcm_pin_mcu_18;
        break;
        case MCU_IF_9BITS:
            lcm_pin_type = lcm_pin_mcu_18;
        break;
        case MCU_IF_8BITS:
            lcm_pin_type = lcm_pin_mcu_18;
        break;
        default:
        break;
    }

    return lcm_pin_type;
}

lcm_mcu_api_local_t *_lcmMcuApiHandleInit(lcm_mcu_aip_t *inParam)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = _lcmMcuApiGetValidHandle();
    lcm_mcu_drv_t *instance = NULL;
    lcm_mcu_timing_t *timing = NULL;
    lcm_mcu_if_t *port = NULL;
    lcm_mcu_format_t *format = NULL;
    lcm_mcu_te_t *te = NULL;
    lcm_cfg_t *lcm_cfg = NULL;
    uint32_t frame_total_w = 0x00;
    uint32_t frame_total_h = 0x00;

    CHECK_PTR(localHandle, NULL, "lcm mcu api handle is null ~_~!");

    instance = &localHandle->lcmMcuDrv;
    timing = &instance->timing;
    port = &instance->port;
    format = &instance->format;
    te = &instance->te;

    instance->baseAddr = inParam->vou_base_addr;
    instance->lcd_id = inParam->lcm_id;
    lcm_cfg = (lcm_cfg_t *)inParam->lcm_cfg;

    localHandle->rst_base_addr = inParam->rst_base_addr;
    localHandle->clk_base_addr = inParam->clk_base_addr;
    localHandle->vou_sel_addr = inParam->vou_sel_addr;
    localHandle->mif_base_addr = inParam->mif_base_addr;
    localHandle->vou_base_addr = inParam->vou_base_addr;
    localHandle->pin_base_addr = inParam->pin_base_addr;
    localHandle->dhd_id = inParam->lcm_id;

    localHandle->lcm_pin_type = _mcu_pin_type_get(lcm_cfg->infor.mcu->if_type);

    localHandle->vouClkIn.clkInfo.mode = 1/*lcm_cfg->infor.mcu->clk->vou.mode*/;
    localHandle->vouClkIn.clkInfo.cfg.sel = 0x3/*lcm_cfg->infor.mcu->clk->vou.sel*/;
    localHandle->vouClkIn.clkInfo.cfg.div = 0/*lcm_cfg->infor.mcu->clk->vou.div*/;

    frame_total_w = lcm_cfg->dp_sync->hor_sync_active
                    + lcm_cfg->dp_sync->hor_back_porch
                    + lcm_cfg->dp_sync->hor_active_pixel
                    + lcm_cfg->dp_sync->hor_front_porch;

    frame_total_h = lcm_cfg->dp_sync->ver_sync_active
                    + lcm_cfg->dp_sync->ver_back_porch
                    + lcm_cfg->dp_sync->ver_active_line
                    + lcm_cfg->dp_sync->ver_front_porch;

    localHandle->vouClkIn.total_pixel = frame_total_w * frame_total_h * lcm_cfg->fps;

    if ((MCU_IF_9BITS == lcm_cfg->infor.mcu->if_type)
        || (MCU_IF_8BITS == lcm_cfg->infor.mcu->if_type)) {
        localHandle->vouClkIn.tx_frq_pixel = 2;
    } else {
        localHandle->vouClkIn.tx_frq_pixel = 1;
    }
    rtn = vou_clk_calc(&localHandle->vouClkIn, &localHandle->vouClkOut);

    localHandle->lcmClkOnIn.clk_base_addr = localHandle->clk_base_addr;
    localHandle->lcmClkOnIn.rst_base_addr = localHandle->rst_base_addr;
    localHandle->lcmClkOnIn.clk_sel = localHandle->vouClkOut.cfg.sel;
    localHandle->lcmClkOnIn.clk_div = localHandle->vouClkOut.cfg.div;

    localHandle->lcmClkOffIn.clk_base_addr = localHandle->clk_base_addr;
    localHandle->lcmClkOffIn.rst_base_addr = localHandle->rst_base_addr;

    localHandle->mifClkIn.dp_sync = lcm_cfg->dp_sync;
    localHandle->mifClkIn.fps = lcm_cfg->fps;
    localHandle->mifClkIn.clk = &lcm_cfg->infor.mcu->clk->mif;
    rtn = _mcu_lane_num_format_get(lcm_cfg->infor.mcu->if_type, lcm_cfg->infor.mcu->format, &localHandle->mifClkIn.laneNum, &localHandle->mifClkIn.format);
    localHandle->mifClkOut.host_sync_div = &localHandle->host_sync_div;
    rtn = dsi_phy_pll_cfg_get(&localHandle->mifClkIn, &localHandle->mifClkOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_pll_cfg_get ~_~!\n");

    localHandle->phyPllCfgIn.phyId = localHandle->dhd_id;
    localHandle->phyPllCfgIn.phy_addr = localHandle->mif_base_addr;
    localHandle->phyPllCfgIn.vou_addr = localHandle->vou_base_addr;
    localHandle->phyPllCfgIn.pll_doubler = localHandle->mifClkOut.pll_doubler;
    localHandle->phyPllCfgIn.pll_div_s = localHandle->mifClkOut.pll_div_s;
    localHandle->phyPllCfgIn.pll_n = localHandle->mifClkOut.pll_n;
    localHandle->phyPllCfgIn.pll_kint = localHandle->mifClkOut.pll_kint;
    localHandle->phyPllCfgIn.pll_nint = localHandle->mifClkOut.pll_nint;
    localHandle->phyPllCfgIn.pll_pdiv = localHandle->mifClkOut.pll_pdiv;
    localHandle->phyPllCfgIn.pixelclk_div = localHandle->mifClkOut.pixelclk_div;
    localHandle->phyPllCfgIn.dhd_div = localHandle->mifClkOut.dhd_div;
    localHandle->phyPllCfgIn.lane_clk = localHandle->mifClkOut.lane_clk;

    timing->csType = lcm_cfg->infor.mcu->timing->csType;
    timing->wrSetup = lcm_cfg->infor.mcu->timing->wrSetup;
    timing->wrHold = lcm_cfg->infor.mcu->timing->wrHold;
    timing->wrDestory = lcm_cfg->infor.mcu->timing->wrDestory;
    timing->rdSetup = lcm_cfg->infor.mcu->timing->rdSetup;
    timing->rdHold = lcm_cfg->infor.mcu->timing->rdHold;
    timing->rdDestory = lcm_cfg->infor.mcu->timing->rdDestory;

    rtn = _lcmMcuApiMapParamCfg(inParam->mcuSpec, instance);

    port->mcuIfType = lcm_cfg->infor.mcu->if_type;
    port->rgbIfType = 0;
    port->transNum = 0;
    port->transEndian = 1;

    format->bitDepth = lcm_cfg->infor.mcu->format;
    format->rgbOrder = lcm_cfg->infor.mcu->order;

    te->eb = lcm_cfg->infor.mcu->te.eb;
    te->mode = lcm_cfg->infor.mcu->te.mode;
    te->edge = lcm_cfg->infor.mcu->te.edge;
    te->type = 0;

    instance->regRead = regRead32;
    instance->regWrite = regWrite32;

#if LCM_MCU_PARAM_DEBUG
    printf("rst_base_addr     : 0x%08x\n", inParam->rst_base_addr);
    printf("clk_base_addr     : 0x%08x\n", inParam->clk_base_addr);
    printf("vou_sel_addr      : 0x%08x\n", inParam->vou_sel_addr);
    printf("vou_base_addr     : 0x%08x\n", inParam->vou_base_addr);
    printf("mif_base_addr     : 0x%08x\n", inParam->mif_base_addr);
    printf("pin_base_addr     : 0x%08x\n", inParam->pin_base_addr);

    printf("hor_sync_active   : %d\n", lcm_cfg->dp_sync->hor_sync_active);
    printf("hor_back_porch    : %d\n", lcm_cfg->dp_sync->hor_back_porch);
    printf("hor_active_pixel  : %d\n", lcm_cfg->dp_sync->hor_active_pixel);
    printf("hor_front_porch   : %d\n", lcm_cfg->dp_sync->hor_front_porch);
    printf("ver_sync_active   : %d\n", lcm_cfg->dp_sync->ver_sync_active);
    printf("ver_back_porch    : %d\n", lcm_cfg->dp_sync->ver_back_porch);
    printf("ver_active_line   : %d\n", lcm_cfg->dp_sync->ver_active_line);
    printf("ver_front_porch   : %d\n", lcm_cfg->dp_sync->ver_front_porch);
    printf("lcm_cfg->fps      : %d\n", lcm_cfg->fps);
    printf("mcu->if_type      : %d\n", lcm_cfg->infor.mcu->if_type);
    printf("mcu->format       : %d\n", lcm_cfg->infor.mcu->format);
    printf("mcu->clk->mode    : %d\n", lcm_cfg->infor.mcu->clk->vou.mode);
    printf("mcu->clk->sel     : %d\n", lcm_cfg->infor.mcu->clk->vou.sel);
    printf("mcu->clk->div     : %d\n", lcm_cfg->infor.mcu->clk->vou.div);
    printf("timing->csType    : %d\n", timing->csType);
    printf("timing->wrSetup   : %d\n", timing->wrSetup);
    printf("timing->wrHold    : %d\n", timing->wrHold);
    printf("timing->wrDestory : %d\n", timing->wrDestory);
    printf("timing->rdSetup   : %d\n", timing->rdSetup);
    printf("timing->rdHold    : %d\n", timing->rdHold);
    printf("timing->rdDestory : %d\n", timing->rdDestory);
    printf("port->mcuIfType   : %d\n", port->mcuIfType);
    printf("port->rgbIfType   : %d\n", port->rgbIfType);
    printf("port->transNum    : %d\n", port->transNum);
    printf("port->transEndian : %d\n", port->transEndian);
    printf("format->bitDepth  : %d\n", format->bitDepth);
    printf("format->rgbOrder  : %d\n", format->rgbOrder);
    printf("te->eb            : %d\n", te->eb);
    printf("te->mode          : %d\n", te->mode);
    printf("te->edge          : %d\n", te->edge);
    printf("te->type          : %d\n", te->type);

    printf("pll_sel          : %d\n", localHandle->vouClkOut.cfg.sel);
    printf("pll_div          : %d\n", localHandle->vouClkOut.cfg.div);

    printf("mif_pll_doubler  : %d\n", localHandle->phyPllCfgIn.pll_doubler);
    printf("mif_pll_div_s    : %d\n", localHandle->phyPllCfgIn.pll_div_s);
    printf("mif_pll_n        : %d\n", localHandle->phyPllCfgIn.pll_n);
    printf("mif_pll_kint     : %d\n", localHandle->phyPllCfgIn.pll_kint);
    printf("mif_pll_nint     : %d\n", localHandle->phyPllCfgIn.pll_nint);
    printf("mif_pll_pdiv     : %d\n", localHandle->phyPllCfgIn.pll_pdiv);
    printf("mif_pixelclk_div : %d\n", localHandle->phyPllCfgIn.pixelclk_div);
    printf("mif_dhd_div      : %d\n", localHandle->phyPllCfgIn.dhd_div);
    printf("mif_lane_clk     : %d\n", localHandle->phyPllCfgIn.lane_clk);
    printf("vou_if_clk        : %d\n", localHandle->vouClkOut.if_clk);

#endif

    EXIT:

    return localHandle;
}

void *lcmMcuApiInit(lcm_mcu_aip_t *inParam)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = NULL;
    lcm_mcu_drv_t *instance = NULL;

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "lcm mcu api inParam is null- ~_~!");

    localHandle = _lcmMcuApiHandleInit(inParam);
    CHECK_PTR(localHandle, NULL, "lcm mcu api handle is null-- ~_~!");

    instance = &localHandle->lcmMcuDrv;

    rtn = lcm_bus_pin_cfg(localHandle->pin_base_addr, localHandle->lcm_pin_type);
    EXIT_IF_FAIL(rtn, NULL, "lcm_bus_pin_cfg ~_~!\n");

    rtn = lcm_clk_on(&localHandle->lcmClkOnIn, &localHandle->lcmClkOnOut);
    EXIT_IF_FAIL(rtn, NULL, "lcm_clk_on ~_~!\n");

    rtn = dsi_phy_pll_v1_on(&localHandle->phyPllCfgIn, &localHandle->phyPllCfgOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_pll_cfg ~_~!\n");

    rtn =  vou_lcd_src_sel(localHandle->vou_sel_addr, instance->lcd_id);
    EXIT_IF_FAIL(rtn, NULL, "vou_lcd_src_sel ~_~!\n");

    rtn = vou_dhd_src_sel(localHandle->vou_sel_addr, instance->lcd_id, 1);
    EXIT_IF_FAIL(rtn, NULL, "vou_dhd_src_sel ~_~!\n");

    rtn = vou_dhd_out_mode(localHandle->vou_base_addr, instance->lcd_id, VOU_OUT_LCM);
    EXIT_IF_FAIL(rtn, NULL, "vou_dhd_out_mode ~_~!\n");

    rtn = lcmGetRegDefault(instance);
    rtn = _lcmMcuApiSetTiming(instance);
    rtn = _lcmMcuApiSetIf(instance);
    rtn = _lcmMcuApiSetFormat(instance);
    rtn = _lcmMcuApiSetTeType(instance);

    rtn = vou_dhd_force_update(localHandle->vou_base_addr, instance->lcd_id, 1);
    EXIT_IF_FAIL(rtn, NULL, "vou_dhd_force_update ~_~!\n");

    EXIT:

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return (void *)localHandle; // lcm_mcu_api_local_t
}

int32_t lcmMcuApiDeInit(void *handle)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = NULL;

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcmMcuApiCheckHandle(handle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    localHandle = (lcm_mcu_api_local_t *)handle;

    #if 1
    rtn = lcm_clk_off(&localHandle->lcmClkOffIn, &localHandle->lcmClkOffOut);
    EXIT_IF_FAIL(rtn, NULL, "lcm_clk_off ~_~!\n");
    #else
    rtn = vou_clk_off(localHandle->clk_base_addr);
    EXIT_IF_FAIL(rtn, NULL, "vou_clk_off ~_~!\n");
    #endif

    rtn = lcm_bus_pin_default_cfg(localHandle->pin_base_addr, localHandle->lcm_pin_type);
    EXIT_IF_FAIL(rtn, NULL, "lcm_bus_pin_default_cfg ~_~!\n");

    memset((void *)handle, 0x00, sizeof(lcm_mcu_api_local_t));

    EXIT:

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t lcmMcuApiWriteCmd(void *handle, uint32_t cmd)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_CMD);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr cmd status switch ~_~!");

    rtn = lcmSetCmdRW(instance, LCM_WRITE);
    rtn = lcmSetCmdDummyNum(instance, 1);
    rtn = lcmSetCmdAddrNum(instance, 1);
    rtn = lcmSetCmdDataNum(instance, 0);
    rtn = lcmSetWCmd(instance, &cmd, 1);
    rtn = lcmSetCmdRun(instance);

    rtn = _lcmMcuApiCheckCmdDone(instance, 1);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr cmd idle status switch ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmMcuApiWriteData(void *handle, uint32_t data)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_CMD);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr data status switch ~_~!");

    rtn = lcmSetCmdRW(instance, LCM_WRITE);
    rtn = lcmSetCmdAddrNum(instance, 0);
    rtn = lcmSetCmdDataNum(instance, 1);
    rtn = lcmSetWData(instance, &data, 1);
    rtn = lcmSetCmdRun(instance);

    rtn = _lcmMcuApiCheckCmdDone(instance, 1);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr data idle status switch ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmMcuApiWriteBurstData(void *handle, uint32_t *data, uint32_t num)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;
    //uint32_t lcmBusStatus = 0;
    uint32_t i = 0;
    uint32_t wrNum = 0;

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = lcmSetCmdRW(instance, LCM_BUS_CMD);
    rtn = lcmSetCmdAddrNum(instance, 0);

    for (i = 0; i < num; i += LCM_MCU_DATA_FIFO_MAX) {
        if((i + LCM_MCU_DATA_FIFO_MAX) < num) {
            wrNum = LCM_MCU_DATA_FIFO_MAX;
        } else {
            wrNum = num - i;
        }

        rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_CMD);
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr data status switch ~_~!");

        rtn = lcmSetCmdDataNum(instance, wrNum);
        rtn = lcmSetWData(instance, data, wrNum);
        rtn = lcmSetCmdRun(instance);

        /*check status*/
        rtn = _lcmMcuApiCheckCmdDone(instance, (10 * wrNum));
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr cmd status switch ~_~!");
    }

    EXIT:

    return rtn;
}

int32_t lcmMcuApiReadData(void *handle, uint32_t *data)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;
    uint32_t rdNum = 1;

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_CMD);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api rd data status switch timeout ~_~!");

    rtn = lcmSetCmdRW(instance, LCM_READ);
    rtn = lcmSetCmdAddrNum(instance, 0);
    rtn = lcmSetCmdDataNum(instance, 1);
    rtn = lcmSetCmdRun(instance);

    rtn = _lcmMcuApiCheckRdFifo(instance, 1, 1);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api rd fifo timeout ~_~!");

    rtn = lcmGetRData(instance, data, rdNum);

    /*check status*/
    rtn = _lcmMcuApiCheckCmdDone(instance, 1);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api read idle status switch ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmMcuApiReadRegData(void *handle, uint32_t *data, uint32_t num)
{
#define MAX_READ_DATA 16
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;
    uint32_t read_data[MAX_READ_DATA]={};

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_CMD);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api rd data status switch timeout ~_~!");
    delayMs(100);
    regWrite32((ADDR)(0xc801018), 0xffffffff);  //clear interrupt
    delayMs(5);

    rtn = lcmSetWCmd(instance, data, 1);

    rtn = lcmSetCmdRW(instance, LCM_READ);
    rtn = lcmSetCmdAddrNum(instance, 1);
    rtn = lcmSetCmdDataNum(instance, num);
    rtn = lcmSetCmdRun(instance);

    /*check status*/
    rtn = _lcmMcuApiCheckCmdDone(instance, 1);

    rtn = lcmGetRData(instance, &read_data[0], num);
    printf("data0=0x%x,data1=0x%x,data2=0x%x,data3=0x%x\n",read_data[0],read_data[1],read_data[2],read_data[3]);


    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api read idle status switch ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmMcuApiReadBurstData(void *handle, uint32_t *data, uint32_t num)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;
    uint32_t i = 0;
    uint32_t rdNum = 0;

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = lcmSetCmdRW(instance, LCM_READ);
    rtn = lcmSetCmdAddrNum(instance, 0);

    for (i = 0; i < num; i += LCM_MCU_DATA_FIFO_MAX) {
        if((i + LCM_MCU_DATA_FIFO_MAX) < num) {
            rdNum = LCM_MCU_DATA_FIFO_MAX;
        } else {
            rdNum = num - i;
        }

        rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_CMD);
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu api rd burst data status switch ~_~!");

        rtn = lcmSetCmdDataNum(instance, rdNum);
        rtn = lcmSetCmdRun(instance);

        rtn = _lcmMcuApiCheckRdFifo(instance, rdNum, rdNum);
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu api rd burst data fifo timeout ~_~!");

        rtn = lcmGetRData(instance, data, rdNum);

        /*check status*/
        rtn = _lcmMcuApiCheckCmdDone(instance, (10 * rdNum));
        EXIT_IF_FAIL(rtn, NULL, "lcm mcu api rd burst data status switch ~_~!");
    }

    EXIT:

    return rtn;
}

int32_t lcmMcuApiPushTable(void *handle, void *setting, uint32_t num)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_setting_tab_t *setting_ptr = (lcm_setting_tab_t *)setting;
    uint32_t i = 0;

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");
    CHECK_PTR(setting, NULL, "setting is null ~_~!\n");

    for(i = 0; i < num; i++, setting_ptr++) {

        switch (setting_ptr->type)
        {
            case LCM_SETTING_CMD:
                #if LCM_MCU_SETTING_DEBUG
                printf("mcu write _cmd: 0x%08x\n", setting_ptr->val);
                #endif
                rtn = lcmMcuApiWriteCmd(localHandle, setting_ptr->val);
                EXIT_IF_FAIL(rtn, NULL, "lcmMcuApiWriteCmd ~_~!");
            break;
            case LCM_SETTING_DATA:
                #if LCM_MCU_SETTING_DEBUG
                printf("mcu write data: 0x%08x\n", setting_ptr->val);
                #endif
                rtn = lcmMcuApiWriteData(localHandle, setting_ptr->val);
                EXIT_IF_FAIL(rtn, NULL, "lcmMcuApiWriteData ~_~!");
            break;
            case LCM_DELAY:
                delayMs(setting_ptr->val);
            break;
            case LCM_END_OF_TABLE:
                goto EXIT;
            break;
            default:
            break;
        }
    }

    EXIT:

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t lcmMcuApiSwitchShow(void *handle)
{
    int32_t rtn = 0;
    lcm_mcu_api_local_t *localHandle = (lcm_mcu_api_local_t *)handle;
    lcm_mcu_drv_t *instance = &localHandle->lcmMcuDrv;

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = _lcmMcuApiCheckHandle(localHandle);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    rtn = _lcmMcuApiSetStatus(instance, LCM_BUS_WR_DATA);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api wr cmd status switch ~_~!");

    EXIT:

    #if LCM_MCU_FUN_DEBUG
    debug_fun(LCM_MCU_FUN_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}


/**----------------------------------------------------------------------------*
*           Compiler Flag                       *
**----------------------------------------------------------------------------*/
#ifdef     __cplusplus
 }
#endif
/**---------------------------------------------------------------------------*/

