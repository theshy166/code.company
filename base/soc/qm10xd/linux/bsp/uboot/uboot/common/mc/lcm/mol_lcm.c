/*
 * Platform driver for molchip  lcm module
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "../lcm_module/include/mol_lcm.h"

#include "rgb/rgb_mpi.h"
#include "../lcm_module/include/mol_lcm_spi.h"
#include "pin_mux/pin_lcd_v01_drv.h"
#include "mcu/mol_lcm_mcu_api.h"
//#include "mol_lcm_spi_api.h"
#include "dsi/dsi_mpi.h"
#include "ctrl/ctrl_lcm_v01_drv.h"
#include "../lcm_module/include/mol_lcm_table.h"
#include "lcm_work_mgr.h"
#include "os.h"
#include "debug.h"
//#include "../lcm_module/include/mol_lcm_sfc.h"

#define MOL_LCM_DEBUG 0

typedef enum {
    LCM_STATUS_NONE = 0,
    LCM_PROBLE,
    LCM_UNINITED,
    LCM_WORKING,
    LCM_SUMPEND,
    LCM_STATUS_MAX,
}lcm_status_e;

note_t s_lcm_status_note[] = {
    {LCM_STATUS_NONE, "LCM_STATUS_NONE"},
    {LCM_PROBLE,      "LCM_PROBLE"     },
    {LCM_UNINITED,    "LCM_UNINITED"   },
    {LCM_WORKING,     "LCM_WORKING"    },
    {LCM_SUMPEND,     "LCM_SUMPEND"    },
    {LCM_STATUS_MAX,  "LCM_STATUS_MAX" },
};

static char *_mol_lcm_get_status_name(lcm_status_e status)
{
    note_t *note = s_lcm_status_note;
    char *name = s_lcm_status_note[LCM_STATUS_MAX].name;

    do {
        if (LCM_STATUS_MAX > note->id) {
            if (status == note->id) {
                name = note->name;
            }
        } else {
            break;
        }
        note++;
    }while(1);

    return name;
}

static lcm_status_e _mol_lcm_get_status(void *handle)
{
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;

    return lcmHandle->status;
}

static int32_t _mol_lcm_set_status(void *handle, lcm_status_e status)
{
    int32_t rtn = SUCCESS;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;

    if (status != lcmHandle->status) {
        lcmHandle->status = status;
    }

    return rtn;
}

static int32_t _mol_lcm_checker_status(void *handle, lcm_status_e status)
{
    int32_t rtn = SUCCESS;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;

    if (status != lcmHandle->status) {
        rtn = FAIL_STATUS_NO_MATCH;
    }
    return rtn;
}

static int32_t _mipi_bus_lp_cfg(void *handle)
{
    int32_t rtn = SUCCESS;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;
    dsi_lp_mode_in_t dsi_lp_mode_in = {0x00};
    dsi_lp_mode_out_t dsi_lp_mode_out = {0x00};

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");

    dsi_lp_mode_in.hostId = lcmHandle->lcm_id;

    rtn = dsi_lp_mode(&dsi_lp_mode_in, &dsi_lp_mode_out);

EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _mipi_bus_hs_cfg(void *handle)
{
    int32_t rtn = SUCCESS;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;
    dsi_hs_mode_in_t dsi_hs_mode_in = {0x00};
    dsi_hs_mode_out_t dsi_hs_mode_out = {0x00};

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");

    dsi_hs_mode_in.hostId = lcmHandle->lcm_id;

    rtn = dsi_hs_mode(&dsi_hs_mode_in, &dsi_hs_mode_out);

EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _mipi_push_table(uint32_t hostId, void *table, uint32_t count)
{
    int32_t rtn = 0;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(table, NULL, "table is null ~_~!\n");

    if (0x00 == count) {
        rtn = 1;
        goto EXIT;
    }

    rtn = dsi_push_table((host_core_t)hostId, (dsi_setting_tab_t *)table, count);

EXIT:

    return rtn;

}

static int32_t _mol_lcm_bus_init(void *handle, void *lcm_cfg)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;
    lcm_cfg_t *lcmCfg = (lcm_cfg_t *)lcm_cfg;
    void *lcmModuleHandle = NULL;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    CHECK_PTR(lcm_cfg, NULL, "lcm_cfg is null ~_~!\n");

    switch(lcmCfg->type){
        case LCM_TYPE_SPI:
        {
/*
            spi_init_in_t spiInitIn = {0};
            spi_init_out_t spiInitout = {0};

            spiInitIn.id = lcmHandle->lcm_probe.lcm_id;
            spiInitIn.pin_ctrl = lcmHandle->lcm_probe.pin_ctrl; // 0:auto mode 1:manual mode
            spiInitIn.spi_addr = (ADDR)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            spiInitIn.gpio_addr = (ADDR)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            spiInitIn.sys_addr = (ADDR)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            spiInitIn.clk_addr = (ADDR)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            spiInitIn.dma_addr = (ADDR)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            lcmModuleHandle = lcmSpiApiInit(&spiInitIn, &spiInitout);
*/
        }
        break;
        case LCM_TYPE_MCU:
        {
            lcm_mcu_aip_t mcuApiParam = {0};
            mcuApiParam.rst_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            mcuApiParam.clk_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_TOPCLK];
            mcuApiParam.vou_sel_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_VOU_SEL];
            mcuApiParam.mif_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_DSI_DPHY];
            mcuApiParam.vou_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_VOU];
            mcuApiParam.pin_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_PIN_MUX];
            mcuApiParam.lcm_id = lcmHandle->lcm_probe.lcm_id;
            mcuApiParam.lcm_cfg = (void *)lcm_cfg;

            lcmModuleHandle = lcmMcuApiInit(&mcuApiParam);
            lcmHandle->lcm_init.handle = lcmModuleHandle;
            CHECK_PTR(lcmModuleHandle, NULL, "lcm bus init handle is null ~_~!\n");
            lcmHandle->handle = lcmModuleHandle;
        }
        break;
        case LCM_TYPE_RGB:
        {
            rgb_init_in_t rgbInitIn;
            rgb_init_out_t rgbInitOut;

            memset((void *)&rgbInitIn, 0x00, sizeof(rgb_init_in_t));
            memset((void *)&rgbInitOut, 0x00, sizeof(rgb_init_out_t));

            rgbInitIn.rst_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
            rgbInitIn.clk_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_TOPCLK];
            rgbInitIn.vou_sel_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_VOU_SEL];
            rgbInitIn.mif_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_DSI_DPHY];
            rgbInitIn.vou_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_VOU];
            rgbInitIn.pin_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_PIN_MUX];

            rgbInitIn.lcd_id = lcmHandle->lcm_probe.lcm_id;
            rgbInitIn.fps = lcmCfg->fps;
            rgbInitIn.format = lcmCfg->infor.rgb->format;
            rgbInitIn.if_type = lcmCfg->infor.rgb->if_type;
            memcpy((void *)&rgbInitIn.clk, (void *)lcmCfg->infor.rgb->clk, sizeof(rgb_clk_t));
            memcpy((void *)&rgbInitIn.dp_sync_info, (void *)lcmCfg->dp_sync, sizeof(lcm_display_sync_info_t));
            rtn = rgb_init(&rgbInitIn, &rgbInitOut);
/*
            if (NULL != lcmCfg->infor.rgb->serial) {
                lcm_spi_init_in_t lcm_spi_init_in = {0x00};
                lcm_spi_init_in.pin_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_PIN_MUX];
                lcm_spi_init_in.info = lcmCfg->infor.rgb->serial;
                rtn = lcm_spi_init(&lcm_spi_init_in, NULL);
            }*/
        }
        break;
        case LCM_TYPE_MIPI:
            {
                dsi_init_in_t dsiInitIn;
                dsi_init_out_t dsiInitOut = {0x00};

                dsiInitIn.rst_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_TOPRST];
                dsiInitIn.clk_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_TOPCLK];
                dsiInitIn.vou_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_VOU];
                if (LCM_ID_MAIN == lcmHandle->lcm_probe.lcm_id) {
                    dsiInitIn.host_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_DSI_HOST_4LANE];
                } else {
                    dsiInitIn.host_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_DSI_HOST_2LANE];
                }
                dsiInitIn.phy_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_DSI_DPHY];
                dsiInitIn.pin_base_addr = (uint32_t)lcmHandle->reg->base[LCM_REGBASE_PIN_MUX];

                dsiInitIn.hostId = lcmHandle->lcm_probe.lcm_id;
                dsiInitIn.fps = lcmCfg->fps;
                dsiInitIn.format = lcmCfg->infor.mipi->format;
                dsiInitIn.laneNum = lcmCfg->infor.mipi->lane_num;
                memcpy((void *)&dsiInitIn.timing, (void *)lcmCfg->infor.mipi->timing, sizeof(dsi_timing_t));
                memcpy((void *)&dsiInitIn.dp_sync_info, (void *)lcmCfg->dp_sync, sizeof(lcm_display_sync_info_t));
                rtn = dsi_init(&dsiInitIn, &dsiInitOut);
            }
            break;

        default:
        break;
    }

EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;

}

static int32_t _mol_lcm_bus_deinit(void *handle, void *lcm_cfg)
{
    int32_t rtn = 0;
    lcm_handle_t *lcmHandle = (lcm_handle_t *)handle;
    lcm_cfg_t *lcmCfg = (lcm_cfg_t *)lcm_cfg;
//    void *lcmModuleHandle = NULL;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    CHECK_PTR(lcm_cfg, NULL, "table is null ~_~!\n");

//    lcmModuleHandle = lcmHandle->handle;

    switch(lcmCfg->type){
        case LCM_TYPE_SPI:
        {
/*
            spi_deinit_in_t spiDeInitIn = {0};
            spi_deinit_out_t spiDeInitout = {0};

            spiDeInitIn.handle = lcmModuleHandle;
            rtn = lcmSpiApiDeInit(&spiDeInitIn, &spiDeInitout);
            EXIT_IF_FAIL(rtn, NULL, "spi bus deinit ~_~!\n");
*/
        }
        break;
        case LCM_TYPE_MCU:
        {
            rtn = lcmMcuApiDeInit(lcmHandle->lcm_init.handle);
            EXIT_IF_FAIL(rtn, NULL, "mcu bus deinit ~_~!\n");
        }
        break;
        case LCM_TYPE_RGB:
        {
            rgb_deinit_in_t rgbDeinitIn = {0x00};
            rgb_deinit_out_t rgbDeinitOut = {0x00};
            rtn = rgb_deinit(&rgbDeinitIn, &rgbDeinitOut);
            EXIT_IF_FAIL(rtn, NULL, "rgb bus deinit ~_~!\n");
/*
            if (NULL != lcmCfg->infor.rgb->serial) {
                lcm_spi_deinit_in_t lcm_spi_deinit_in;
                memset((void *)&lcm_spi_deinit_in, 0x00, sizeof(lcm_spi_deinit_in_t));
                rtn = lcm_spi_deInit(&lcm_spi_deinit_in, NULL);
                EXIT_IF_FAIL(rtn, NULL, "spi ctrl bus deinit ~_~!\n");
            }*/
        }
        break;
        case LCM_TYPE_MIPI:
        {
            dsi_deinit_in_t dsiDeInitIn;
            dsi_deinit_out_t dsiDeInitOut = {0x00};

            dsiDeInitIn.hostId = lcmHandle->lcm_probe.lcm_id;
            rtn = dsi_deinit(&dsiDeInitIn, &dsiDeInitOut);
        }
        break;
        default:
        break;
    }

    lcmHandle->handle = NULL;

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    //rtn = lcm_bus_default_cfg((uint32_t)lcmHandle->reg->base[LCM_REGBASE_PIN_MUX]);

    return rtn;

}

static int32_t _mol_lcm_ops_cfg(lcm_ops *ops, lcm_type_e lcm_type)
{
    int32_t rtn = 0;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(ops, NULL, "handle is null ~_~!\n");

    if (LCM_TYPE_MAX > lcm_type) {
        ops->bus_init = _mol_lcm_bus_init;
        ops->bus_deinit = _mol_lcm_bus_deinit;
        ops->delayMs = delayMs;
        ops->debug_fun = debug_fun;

        ops->power.dvdd = lcm_dvdd_set;
        ops->power.avdd = lcm_avdd_set;
        ops->power.iovdd = lcm_iovdd_set;
        ops->power.vcom = lcm_vcom_set;
        ops->power.vgh = lcm_vgh_set;
        ops->power.vgl = lcm_vgl_set;
        ops->power.reset = lcm_reset_set;
        ops->power.standby = lcm_standby_set;
        ops->power.back_light_gpio = lcm_back_light_gpio_set;
        ops->power.back_light_pwm = lcm_back_light_pwm_set;
        ops->power.power_pwm = lcm_power_pwm_set;

        switch(lcm_type){
            case LCM_TYPE_SPI:
/*
                ops->ctrl_fun.spi.write_cmd        = lcmSpiApiWriteCmd;
                ops->ctrl_fun.spi.write_data       = lcmSpiApiWriteData;
                ops->ctrl_fun.spi.write_burst_data = lcmSpiApiWriteBurstData;
                ops->ctrl_fun.spi.read_data        = lcmSpiApiReadData;
                ops->ctrl_fun.spi.read_burst_data  = lcmSpiApiReadBurstData;
                ops->ctrl_fun.spi.io_ctrl          = lcmSpiApiIoCtrl;
                ops->ctrl_fun.spi.push_tab         = lcmSpiApiPushTable;
*/
            break;

            case LCM_TYPE_MCU:
                ops->ctrl_fun.mcu.write_cmd        = lcmMcuApiWriteCmd;
                ops->ctrl_fun.mcu.write_data       = lcmMcuApiWriteData;
                ops->ctrl_fun.mcu.write_burst_data = lcmMcuApiWriteBurstData;
                ops->ctrl_fun.mcu.read_data        = lcmMcuApiReadRegData;
                ops->ctrl_fun.mcu.push_tab         = lcmMcuApiPushTable;
            break;

            case LCM_TYPE_RGB:
/*
                ops->ctrl_fun.spi_ctrl.write_cmd        = write_cmd;
                ops->ctrl_fun.spi_ctrl.write_data       = write_data;
                ops->ctrl_fun.spi_ctrl.read             = read_data;
                ops->ctrl_fun.spi_ctrl.write_burst_cmd  = NULL;
                ops->ctrl_fun.spi_ctrl.write_burst_data = NULL;
                ops->ctrl_fun.spi_ctrl.read_burst       = NULL;
*/
            break;
            case LCM_TYPE_MIPI:
                ops->ctrl_fun.mipi.dcs_write = dsi_dcs_write;
                ops->ctrl_fun.mipi.dcs_read = dsi_dcs_read;
                ops->ctrl_fun.mipi.push_tab = _mipi_push_table;
                ops->ctrl_fun.mipi.bus_init = _mol_lcm_bus_init;
                ops->ctrl_fun.mipi.bus_deinit = _mol_lcm_bus_deinit;
                ops->ctrl_fun.mipi.lp_mode = _mipi_bus_lp_cfg;
                ops->ctrl_fun.mipi.hs_mode = _mipi_bus_hs_cfg;
                break;

            default:
            break;
        }
    } else {
        rtn = LCM_ID_ERR;
        goto EXIT;
    };

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t mol_lcm_init(lcm_handle_t *handle)
{
    int32_t rtn = SUCCESS;
    lcm_status_e lcm_status = LCM_STATUS_NONE;
    lcm_cfg_t *lcm_config;
    lcm_ops *ops;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_init.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_init.lcm_id);

    #if MOL_LCM_DEBUG
    printf("handle->lcm_init.lcm_id:%d\n", handle->lcm_init.lcm_id);
    #endif

    handle->lcm_id = handle->lcm_init.lcm_id;
    lcm_config = handle->lcmCfg;
    ops = &handle->ops;

    rtn = _mol_lcm_ops_cfg(ops, lcm_config->type);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_ops_cfg ~_~\n");

    lcm_status = _mol_lcm_get_status(handle);

    if (LCM_PROBLE == lcm_status) {
        #if MOL_LCM_DEBUG
        printf("%s:%d, probe init lcm_id %d\n",__FUNCTION__,__LINE__, handle->lcm_init.lcm_id);
        printf("%s:%d, handle %px, ops: %px\n",__FUNCTION__,__LINE__, handle, ops);
        #endif

        CHECK_FUN(lcm_config->fun->init, NULL, "lcm_config->fun->init ~_~!\n");
        rtn = lcm_config->fun->init(handle, ops);
        EXIT_IF_FAIL(rtn, NULL, "lcm_config->fun->init ~_~\n");

    } else if (LCM_UNINITED == lcm_status) {
        #if MOL_LCM_DEBUG
        printf("%s:%d, lcm reinit lcm_id %d\n",__FUNCTION__,__LINE__, handle->lcm_init.lcm_id);
        #endif

        CHECK_FUN(lcm_config->fun->poweron, NULL, "lcm_config->fun->poweron ~_~!\n");
        rtn = lcm_config->fun->poweron(handle, ops);
        EXIT_IF_FAIL(rtn, NULL, "lcm_config->fun->poweron ~_~\n");

        CHECK_FUN(lcm_config->fun->init, NULL, "lcm_config->fun->init ~_~!\n");
        rtn = lcm_config->fun->init(handle, ops);
        EXIT_IF_FAIL(rtn, NULL, "lcm_config->fun->init ~_~\n");
    } else {
        printf("%s:%d, -- %s\n",__FUNCTION__, __LINE__, _mol_lcm_get_status_name(lcm_status));
        goto EXIT;
    }

    rtn = _mol_lcm_set_status(handle, LCM_WORKING);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_set_status ~_~\n");

    if (lcm_config->type == LCM_TYPE_MCU){
        lcmMcuApiSwitchShow(handle->lcm_init.handle);
        EXIT_IF_FAIL(rtn, NULL, "lcmMcuApiSwitchShow ~_~\n");
    } else if(lcm_config->type == LCM_TYPE_MIPI){
        _mipi_bus_hs_cfg(handle);
    }

    EXIT :

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t read_tab_index(lcm_handle_t *handle)
{
    int32_t rtn = 0;
    uint32_t lcm_index = 0;
    lcm_ops *ops;
    serial_bus_info *info;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_probe.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_probe.lcm_id);

    ops = &handle->lcm_init.ops;
    info = &handle->info;
    /*TODO lcm_serial_bus_info init*/

    /*step 1:read LCD index store in norflash*/
    CHECK_FUN(ops->sfc.read, NULL, "ops->sfc.read ~_~!\n");
    rtn = ops->sfc.read(info, handle->lcm_probe.lcm_id, &lcm_index);
    EXIT_IF_FAIL(rtn, NULL, "ops->sfc.read ~_~\n");

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return lcm_index;
}

int32_t store_tab_index(lcm_handle_t *handle,int32_t lcm_index)
{
    int rtn = LCM_OK;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_init.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_init.lcm_id);

    if (LCM_ID_MAX > handle->lcm_probe.lcm_id) {
        lcm_ops *ops =NULL;
        serial_bus_info *info = NULL;

        ops = &handle->lcm_init.ops;
        info = &handle->info;

        CHECK_FUN(ops->sfc.earse, NULL, "ops->sfc.earse ~_~!\n");
        rtn = ops->sfc.earse(info, 2, lcm_index);/*write main lcm_index commmand:2*/
        EXIT_IF_FAIL(rtn, NULL, "ops->sfc.earse ~_~\n");

        rtn = ops->sfc.write(info, handle->lcm_probe.lcm_id, lcm_index);/*write main lcm_index commmand:2*/
        EXIT_IF_FAIL(rtn, NULL, "ops->sfc.write ~_~\n");
    } else {
        rtn = LCM_ID_ERR;
    }

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t mol_lcm_scan(lcm_handle_t *handle,lcm_cfg_t **lcm_tab)
{
    int32_t rtn = SUCCESS;
    int32_t probe_rtn = FAIL_PROBE;
    int32_t lcm_index = 0xFF;
    lcm_ops *ops  = NULL;
    lcm_work_mode_e lcmWorkMode = LCM_WORK_NONE;


    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    CHECK_PTR(lcm_tab, NULL, "lcm_tab is null ~_~!\n");
    CHECK_PTR(*lcm_tab, NULL, "*lcm_tab is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_probe.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_init.lcm_id);

    ops = &handle->lcm_init.ops;

    rtn = lcm_sfc_init(handle,NULL);
    EXIT_IF_FAIL(rtn, NULL, "lcm_sfc_init ~_~\n");

    lcm_index = read_tab_index(handle);

    if (lcm_index != INVALID_INDEX) {
        if (0 != lcm_tab[lcm_index]) {
            // need checker lcm_tab[lcm_index]->verder_name
            rtn = os_strcmp(VENDER_NAME, lcm_tab[lcm_index]->verder_name);
            if (SUCCESS != rtn) {
#if MOL_LCM_DEBUG
                printf("%s:%d default lcm-%d-%d verder_name:%s checker ~_~!\r\n",__FUNCTION__, __LINE__, handle->lcm_probe.lcm_id, lcm_index, lcm_tab[lcm_index]->verder_name);
#endif
                goto SCAN_START;
            }
#if MOL_LCM_DEBUG
            printf("%s:%d default lcm-%d-%d %s :identify start\r\n",__FUNCTION__, __LINE__, handle->lcm_probe.lcm_id, lcm_index, lcm_tab[lcm_index]->name);
#endif
            lcmWorkMode = get_lcm_work_mode(lcm_tab[lcm_index]);
            rtn = check_lcm_work_mode(handle->lcm_probe.lcm_id, lcmWorkMode);
            if (SUCCESS != rtn) {
#if MOL_LCM_DEBUG
                printf("default lcm-%d work mode : %s support ~_~\n", handle->lcm_probe.lcm_id, get_lcm_work_mode_str(lcmWorkMode));
#endif
                goto SCAN_START;
            }

            rtn = _mol_lcm_ops_cfg(ops, lcm_tab[lcm_index]->type);
            EXIT_IF_FAIL(rtn, NULL, "ops->sfc.write ~_~\n");

            CHECK_FUN(lcm_tab[lcm_index]->fun->poweron, NULL, "lcm_tab[lcm_index]->fun->poweron ~_~!\n");
            rtn = lcm_tab[lcm_index]->fun->poweron(handle, ops);
            EXIT_IF_FAIL(rtn, NULL, "ops->sfc.write ~_~\n");

            probe_rtn = lcm_tab[lcm_index]->fun->identify(handle, ops);
            if (0 == probe_rtn) {
#if MOL_LCM_DEBUG
                printf("%s:%d default lcm %s identify success\r\n", __FUNCTION__, __LINE__, lcm_tab[lcm_index]->name);
#endif
                rtn = set_lcm_work_mode(handle->lcm_probe.lcm_id, lcmWorkMode);
                EXIT_IF_FAIL(rtn, NULL, "default identify set_lcm_work_mode ~_~\n");

                handle->lcmCfg = lcm_tab[lcm_index];
                goto EXIT;
            } else {
                printf("%s:%d default lcm %s identify fail\r\n", __FUNCTION__, __LINE__, lcm_tab[lcm_index]->name);

                CHECK_FUN(lcm_tab[lcm_index]->fun->poweroff, NULL, "lcm_tab[lcm_index]->fun->poweroff ~_~!\n");
                rtn = lcm_tab[lcm_index]->fun->poweroff(handle, ops);
                EXIT_IF_FAIL(rtn, NULL, "lcm->fun->poweroff ~_~\n");
            }
        }else {
            if (0 == lcm_tab[lcm_index]->verder_name) {
                printf("%s:%d default lcm identify lcm_tab[%d]->vender_name is void ~_~!\r\n",__FUNCTION__, __LINE__, lcm_index);
            }
            if (0 == lcm_tab[lcm_index]) {
                printf("%s:%d default lcm identify lcm_tab[%d] is void ~_~!\r\n",__FUNCTION__, __LINE__, lcm_index);
            }
        }
    }else{
        printf("probe the lcm first time,no lcm_index is writen\r\n");
    }

    SCAN_START:

    printf("%s:%d --lcm probe all--\r\n", __FUNCTION__, __LINE__);

    for(lcm_index = 0; lcm_tab[lcm_index] != 0; lcm_index++) {

        // need checker lcm_tab[lcm_index]->verder_name
        rtn = os_strcmp(VENDER_NAME, lcm_tab[lcm_index]->verder_name);
        if (SUCCESS != rtn) {
            printf("%s:%d scan lcm-%d-%d verder_name:%s checker ~_~!\r\n",__FUNCTION__, __LINE__, handle->lcm_probe.lcm_id, lcm_index, lcm_tab[lcm_index]->verder_name);
            continue;
        }
        printf("%s:%d scan lcm %s identify start\r\n", __FUNCTION__, __LINE__, lcm_tab[lcm_index]->name);

        lcmWorkMode = get_lcm_work_mode(lcm_tab[lcm_index]);
        rtn = check_lcm_work_mode(handle->lcm_probe.lcm_id, lcmWorkMode);
        if (SUCCESS == rtn) {
            WARNING_IF_FAIL(rtn, NULL, "scan lcm-%d work mode : %s support ~_~\n", handle->lcm_probe.lcm_id, get_lcm_work_mode_str(lcmWorkMode));
            continue;
        }

        rtn = _mol_lcm_ops_cfg(ops, lcm_tab[lcm_index]->type);
        EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_ops_cfg ~_~\n");

        CHECK_FUN(lcm_tab[lcm_index]->fun->poweron, NULL, "lcm_tab[lcm_index]->fun->poweron ~_~!\n");
        rtn = lcm_tab[lcm_index]->fun->poweron(handle, ops);
        EXIT_IF_FAIL(rtn, NULL, "lcm->fun->poweron ~_~\n");

        CHECK_FUN(lcm_tab[lcm_index]->fun->identify, NULL, "lcm_tab[lcm_index]->fun->identify ~_~!\n");
        probe_rtn = lcm_tab[lcm_index]->fun->identify(handle, ops);
        if(0 == probe_rtn){
            printf("%s:%d scan lcm %s identify success\r\n", __FUNCTION__, __LINE__, lcm_tab[lcm_index]->name);

            rtn = set_lcm_work_mode(handle->lcm_probe.lcm_id, lcmWorkMode);
            EXIT_IF_FAIL(rtn, NULL, "scan set_lcm_work_mode ~_~\n");

            handle->lcmCfg = lcm_tab[lcm_index];
            rtn = store_tab_index(handle,lcm_index);
            EXIT_IF_FAIL(rtn, NULL, "store_tab_index ~_~\n");
            break;
        } else {

            CHECK_FUN(lcm_tab[lcm_index]->fun->poweroff, NULL, "lcm_tab[lcm_index]->fun->poweroff ~_~!\n");
            rtn = lcm_tab[lcm_index]->fun->poweroff(handle, ops);
            EXIT_IF_FAIL(rtn, NULL, "lcm->fun->poweroff ~_~\n");

            printf("%s:%d scan lcm %s identify fail\r\n", __FUNCTION__, __LINE__, lcm_tab[lcm_index]->name);
            delayMs(100);
        }
    }

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    if (SUCCESS != rtn) probe_rtn = rtn;

    return probe_rtn;
}

int32_t mol_lcm_probe(lcm_handle_t *handle)
{
    int32_t rtn = SUCCESS;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_probe.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_probe.lcm_id);

    #if MOL_LCM_DEBUG
    printf("handle->lcm_probe.lcm_id:%d\n", handle->lcm_probe.lcm_id);
    #endif

    rtn = mol_lcm_scan(handle, get_lcm_cfg_tab(handle->lcm_probe.lcm_id));
    EXIT_IF_FAIL(rtn, NULL, "mol_lcm_scan ~_~\n");

    rtn = _mol_lcm_set_status(handle, LCM_PROBLE);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_set_status ~_~\n");

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t mol_lcm_deinit(lcm_handle_t *handle)
{
    int32_t rtn = 0;
    lcm_cfg_t *lcm_config = NULL;
    lcm_ops *ops = NULL;
    lcm_status_e lcm_status = LCM_STATUS_NONE;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_deinit.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_deinit.lcm_id);
    CHECK_PTR(handle->lcmCfg, NULL, "lcmCfg is null ~_~!\n");

    #if MOL_LCM_DEBUG
    printf("handle->lcm_deinit.lcm_id:%d\n", handle->lcm_deinit.lcm_id);
    #endif

    lcm_status = _mol_lcm_get_status(handle);
    if ((LCM_STATUS_NONE == lcm_status)
        || (LCM_UNINITED == lcm_status)) {
        printf("%s:%d -- %s \n", __FUNCTION__, __LINE__, _mol_lcm_get_status_name(lcm_status));
        goto EXIT;
    }

    handle->lcm_id = handle->lcm_deinit.lcm_id;
    lcm_config = handle->lcmCfg;
    ops = &handle->ops;

    rtn = _mol_lcm_ops_cfg(ops, lcm_config->type);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_ops_cfg ~_~\n");

    CHECK_FUN(lcm_config->fun->deinit, NULL, "lcm_config->fun->deinit ~_~!\n");
    rtn = lcm_config->fun->deinit(handle, ops);
    EXIT_IF_FAIL(rtn, NULL, "lcm_config->fun->deinit ~_~\n");

    rtn = set_lcm_work_mode(handle->lcm_probe.lcm_id, LCM_WORK_NONE);
    EXIT_IF_FAIL(rtn, NULL, "deinit set_lcm_work_mode ~_~\n");

    rtn = _mol_lcm_set_status(handle, LCM_UNINITED);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_set_status ~_~\n");

    EXIT :

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t mol_lcm_suspend(lcm_handle_t *handle)
{
    int32_t rtn = 0;
    lcm_cfg_t *lcm_config = NULL;
    lcm_ops *ops = NULL;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_suspend.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_suspend.lcm_id);
    CHECK_PTR(handle->lcmCfg, NULL, "lcmCfg is null ~_~!\n");

    rtn = _mol_lcm_checker_status(handle, LCM_WORKING);
    if (SUCCESS != rtn) {
        printf("%s:%d lcm status %d no LCM_INITED \n", __FUNCTION__, __LINE__, _mol_lcm_get_status(handle));
        goto EXIT;
    }

    handle->lcm_id = handle->lcm_suspend.lcm_id;
    lcm_config = handle->lcmCfg;
    ops = &handle->ops;

    rtn = _mol_lcm_ops_cfg(ops, lcm_config->type);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_ops_cfg ~_~\n");

    CHECK_FUN(lcm_config->fun->suspend, NULL, "lcm_config->fun->suspend ~_~!\n");
    rtn = lcm_config->fun->suspend(handle, ops);
    EXIT_IF_FAIL(rtn, NULL, "lcm_config->fun->suspend ~_~\n");

    rtn = _mol_lcm_set_status(handle, LCM_SUMPEND);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_set_status ~_~\n");

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;

}

int32_t mol_lcm_resume(lcm_handle_t *handle)
{
    int32_t rtn = 0;
    lcm_cfg_t *lcm_config = NULL;
    lcm_ops *ops = NULL;

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(handle, NULL, "handle is null ~_~!\n");
    EXIT_IF_FAIL((LCM_ID_MAX <= handle->lcm_resume.lcm_id), NULL, "lcm_id : %d is over range ~_~\n", handle->lcm_resume.lcm_id);
    CHECK_PTR(handle->lcmCfg, NULL, "lcmCfg is null ~_~!\n");

    rtn = _mol_lcm_checker_status(handle, LCM_SUMPEND);
    if (SUCCESS != rtn) {
        printf("%s:%d lcm status %d no LCM_SUMPEND \n", __FUNCTION__, __LINE__, _mol_lcm_get_status(handle));
        goto EXIT;
    }

    handle->lcm_id = handle->lcm_resume.lcm_id;
    lcm_config = handle->lcmCfg;
    ops = &handle->ops;

    rtn = _mol_lcm_ops_cfg(ops, lcm_config->type);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_ops_cfg ~_~\n");

    CHECK_FUN(lcm_config->fun->resume, NULL, "lcm_config->fun->resume ~_~!\n");
    rtn = lcm_config->fun->resume(handle ,ops);
    EXIT_IF_FAIL(rtn, NULL, "lcm_config->fun->resume ~_~\n");

    rtn = _mol_lcm_set_status(handle, LCM_WORKING);
    EXIT_IF_FAIL(rtn, NULL, "_mol_lcm_set_status ~_~\n");

    EXIT:

    #if MOL_LCM_DEBUG
    debug_fun(MOL_LCM_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}
