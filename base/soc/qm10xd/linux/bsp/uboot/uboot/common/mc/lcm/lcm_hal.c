#include "lcm_hal.h"
#include "../lcm_module/include/mol_lcm.h"
#include "mcu/mol_lcm_mcu_api.h"
#include "mc_lcm.h"
#include "debug.h"

#define LCM_HAL_DEBUG 0

int lcm_probe_info_update(lcm_cfg_t *lcmCfg, lcm_probe_info_t *info)
{
    int rtn = 0;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__,__LINE__, "start");
    #endif

    info->output.width = lcmCfg->width;
    info->output.height = lcmCfg->height;
    info->output.frame_rate = lcmCfg->fps;
    info->type = lcmCfg->type;

    info->dp_sync.hor_active_pixel = lcmCfg->dp_sync->hor_active_pixel;
    info->dp_sync.ver_active_line = lcmCfg->dp_sync->ver_active_line;
    info->dp_sync.hor_front_porch = lcmCfg->dp_sync->hor_front_porch;
    info->dp_sync.hor_sync_active = lcmCfg->dp_sync->hor_sync_active;
    info->dp_sync.hor_back_porch = lcmCfg->dp_sync->hor_back_porch;
    info->dp_sync.ver_front_porch = lcmCfg->dp_sync->ver_front_porch;
    info->dp_sync.ver_sync_active = lcmCfg->dp_sync->ver_sync_active;
    info->dp_sync.ver_back_porch = lcmCfg->dp_sync->ver_back_porch;

    if (_LCM_TYPE_RGB == info->type) {
        info->lcm_config.bit_depth = lcmCfg->infor.rgb->format;
        info->lcm_config.if_type = lcmCfg->infor.rgb->if_type;
        info->lcm_config.rgb_order = lcmCfg->infor.rgb->order;
        info->lcm_config.dithering_en = lcmCfg->infor.rgb->dither;
        info->lcm_config.bitswap_en = lcmCfg->infor.rgb->bitswap;
        info->lcm_config.info.rgb.h_sync_pol = lcmCfg->infor.rgb->h_sync_pol;
        info->lcm_config.info.rgb.v_sync_pol = lcmCfg->infor.rgb->v_sync_pol;
        info->lcm_config.info.rgb.de_pol = lcmCfg->infor.rgb->de_pol;
    }

    if (_LCM_TYPE_MCU == info->type) {
        info->lcm_config.bit_depth = lcmCfg->infor.mcu->format;
        info->lcm_config.if_type = lcmCfg->infor.mcu->if_type;
        info->lcm_config.rgb_order = lcmCfg->infor.mcu->order;
        info->lcm_config.dithering_en = lcmCfg->infor.mcu->dither;
        info->lcm_config.bitswap_en = lcmCfg->infor.mcu->bitswap;
        info->lcm_config.info.mcu.trans_mode = lcmCfg->infor.mcu->trans;
        info->lcm_config.info.mcu.cs_type = lcmCfg->infor.mcu->timing->csType;
        info->lcm_config.info.mcu.te_eb = lcmCfg->infor.mcu->te.eb;
        info->lcm_config.info.mcu.te_mode = lcmCfg->infor.mcu->te.mode;
        info->lcm_config.info.mcu.te_edge = lcmCfg->infor.mcu->te.edge;
    }

#if LCM_HAL_DEBUG
    printf("\t|--------------------|\n");
    switch (info->type) {
        case LCM_TYPE_MIPI:
            printf("\t|    mipi-%d lcd      |\n", info->lcm_id);
        break;
        case LCM_TYPE_MCU:
            printf("\t|    mcu-%d lcd       |\n", info->lcm_id);
        break;
        case LCM_TYPE_RGB:
            printf("\t|    rgb-%d lcd       |\n", info->lcm_id);
        break;
        case LCM_TYPE_SPI:
            printf("\t|    spi-%d lcd       |\n", info->lcm_id);
        break;
        default:
            printf("\t|     no lcd          |\n");
            printf("\t|       %d            |\n", info->type);
        break;
    }

    printf("\t|                    |\n");
    printf("\t|   %4d %4d %3d    |\n", info->output.width, info->output.height, info->output.frame_rate);
    printf("\t|--------------------|\n");
#endif

#if LCM_HAL_DEBUG
    printf("info->idx                         : %d\n", info->lcm_id);
    printf("info->output.width                : %d\n", info->output.width     );
    printf("info->output.height               : %d\n", info->output.height    );
    printf("info->output.frame_rate           : %d\n", info->output.frame_rate);
    printf("info->lcd_type                    : %d\n", info->type);
    printf("info->lcm_config->bit_depth       : %d\n", info->lcm_config.bit_depth  );
    printf("info->lcm_config->if_type         : %d\n", info->lcm_config.if_type    );
    printf("info->lcm_config->rgb_order       : %d\n", info->lcm_config.rgb_order  );
    printf("info->lcm_config->dithering_en    : %d\n", info->lcm_config.dithering_en  );
    printf("info->lcm_config->bitswap_en      : %d\n", info->lcm_config.bitswap_en );
    switch (info->type) {
        case LCM_TYPE_MCU:
        {
            printf("info->lcm_config->trans_mode      : %d\n", info->lcm_config.info.mcu.trans_mode );
            printf("info->lcm_config->te_eb           : %d\n", info->lcm_config.info.mcu.te_eb );
            printf("info->lcm_config->te_mode         : %d\n", info->lcm_config.info.mcu.te_mode     );
            printf("info->lcm_config->te_edge         : %d\n", info->lcm_config.info.mcu.te_edge     );
        }
        break;
        case LCM_TYPE_RGB:
        {
            printf("info->lcm_config->h_sync_pol      : %d\n", info->lcm_config.info.rgb.h_sync_pol );
            printf("info->lcm_config->v_sync_pol      : %d\n", info->lcm_config.info.rgb.v_sync_pol );
            printf("info->lcm_config->de_pol          : %d\n", info->lcm_config.info.rgb.de_pol     );
        }
        break;
        case LCM_TYPE_SPI:

        break;
        default:
        break;
    }
    printf("info->lcm_config->bit_depth       : %d\n", info->lcm_config.bit_depth  );
    printf("info->lcm_config->if_type         : %d\n", info->lcm_config.if_type    );
    printf("info->lcm_config->rgb_order       : %d\n", info->lcm_config.rgb_order  );
    printf("info->lcm_config->dithering_en    : %d\n", info->lcm_config.dithering_en  );
    printf("info->lcm_config->bitswap_en      : %d\n", info->lcm_config.bitswap_en );

    printf("info->dp_sync.hor_active_pixel    :%d\n", info->dp_sync.hor_active_pixel);
    printf("info->dp_sync.ver_active_line     :%d\n", info->dp_sync.ver_active_line );
    printf("info->dp_sync.hor_front_porch     :%d\n", info->dp_sync.hor_front_porch );
    printf("info->dp_sync.hor_sync_active     :%d\n", info->dp_sync.hor_sync_active );
    printf("info->dp_sync.hor_back_porch      :%d\n", info->dp_sync.hor_back_porch  );
    printf("info->dp_sync.ver_front_porch     :%d\n", info->dp_sync.ver_front_porch );
    printf("info->dp_sync.ver_sync_active     :%d\n", info->dp_sync.ver_sync_active );
    printf("info->dp_sync.ver_back_porch      :%d\n", info->dp_sync.ver_back_porch  );

#endif

    return rtn;
}

int lcm_func_probe(lcm_probe_in_t * inParam, lcm_probe_out_t *outParam)
{
    int rtn = SUCCESS;
    lcm_handle_t *lcmHandlePtr = NULL;
    lcm_probe_info_t * lcm_probe_info = NULL;
    int lcm_id = 0;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__,__LINE__, "start");
    #endif
    CHECK_PTR(inParam, NULL, "lcm_probe_in_t is null ~_~!\n");
    CHECK_PTR(outParam, NULL, "lcm_probe_out_t is null ~_~!\n");

    memset((void *)outParam, 0x00, sizeof(lcm_probe_out_t));

    printf("\n");

    do {

        if (LCM_SUB < lcm_id) {

            if ((NULL != outParam->prob_info[LCM_MAIN])
                || (NULL != outParam->prob_info[LCM_SUB])) {
                rtn = SUCCESS;
                //EXIT_IF_SUCCESS(rtn, NULL, "--lcm probe is end--\n");
#if LCM_HAL_DEBUG
                printf("--lcm probe is end--\n");
#endif
                goto EXIT;
            } else {
                rtn = FAIL_PROBE;
                EXIT_IF_FAIL(rtn, NULL, "lcm id is over ~_~!\n");
            }
        }

        #if LCM_HAL_DEBUG
        printf("------lcm_id-%d probe start ------\n", lcm_id);
        #endif
        lcmHandlePtr = get_lcm_handle(lcm_id);
        CHECK_PTR(lcmHandlePtr, NULL, "lcmHandlePtr is null ~_~!\n");

        if (LCM_ID_DISABLE_OPS == lcmHandlePtr->lcm_id_cfg) {
            goto LCM_ID_ADD;
        }

        lcmHandlePtr->lcm_probe.lcm_id = lcm_id;
        rtn = mol_lcm_probe(lcmHandlePtr);

        if (SUCCESS == rtn) {
            #if LCM_HAL_DEBUG
            printf("lcm_id-%d probe success\n", lcm_id);
            #endif
        } else {
            #if LCM_HAL_DEBUG
            printf("lcm_id-%d probe ~_~!\n", lcm_id);
            #endif
            goto LCM_ID_ADD;
        }

        lcm_probe_info = (lcm_probe_info_t *)lcmHandlePtr->prob_info;
        lcm_probe_info->lcm_id = lcm_id;
        rtn = lcm_probe_info_update(lcmHandlePtr->lcmCfg, lcm_probe_info);
        EXIT_IF_FAIL(rtn, NULL, "lcm_probe_info_update ~_~!\n");

        outParam->prob_info[lcm_id] = (lcm_probe_info_t *)lcmHandlePtr->prob_info;

        LCM_ID_ADD:
        #if LCM_HAL_DEBUG
        printf("------lcm_id-%d probe end ------\n\n", lcm_id);
        #endif
        lcm_id++;
#if defined (CONFIG_UBOOT_XKPHOTO)
        /*scan only lcm id 0, skip others*/
        lcm_id = LCM_ID_MAX;
#endif
    }while(1);

    EXIT:

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__,__LINE__, "end");
    #endif

    return rtn;
}

int lcm_func_init(lcm_init_in_t *inParam, lcm_init_out_t *outParam)
{
    int rtn = SUCCESS;
    lcm_handle_t *lcmHandlePtr = NULL;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "lcm_init_in_t is null ~_~!\n");
    CHECK_PTR(outParam, NULL, "lcm_init_out_t is null ~_~!\n");

    lcmHandlePtr = get_lcm_handle(inParam->lcm_id);
    CHECK_PTR(lcmHandlePtr, NULL, "lcmHandlePtr is null ~_~!\n");

    lcmHandlePtr->lcm_init.lcm_id = inParam->lcm_id;
    rtn = mol_lcm_init(lcmHandlePtr);
    EXIT_IF_FAIL(rtn, NULL, "mol_lcm_init ~_~!\n");

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    EXIT:

    return rtn;
}

int lcm_func_deinit(lcm_deinit_in_t *inParam, lcm_deinit_out_t *outParam)
{
    int rtn = SUCCESS;
    lcm_handle_t *lcmHandlePtr = NULL;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif
    CHECK_PTR(inParam, NULL, "lcm_deinit_in_t is null ~_~!\n");
    CHECK_PTR(outParam, NULL, "lcm_deinit_out_t is null ~_~!\n");

    lcmHandlePtr = get_lcm_handle(inParam->lcm_id);
    CHECK_PTR(lcmHandlePtr, NULL, "lcmHandlePtr is null ~_~!\n");

    lcmHandlePtr->lcm_deinit.lcm_id = inParam->lcm_id;
    rtn = mol_lcm_deinit(lcmHandlePtr);
    EXIT_IF_FAIL(rtn, NULL, "mol_lcm_deinit is null ~_~!\n");

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    EXIT:

    return rtn;
}

int lcm_func_suspend(lcm_suspend_in_t *inParam, lcm_suspend_out_t *outParam)
{
    int rtn = SUCCESS;
    lcm_handle_t *lcmHandlePtr = NULL;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    lcmHandlePtr = get_lcm_handle(inParam->lcm_id);
    CHECK_PTR(lcmHandlePtr, NULL, "lcmHandlePtr is null ~_~!\n");

    lcmHandlePtr->lcm_suspend.lcm_id = inParam->lcm_id;

    rtn = mol_lcm_suspend(lcmHandlePtr);

    EXIT:

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int lcm_func_resume(lcm_resume_in_t *inParam, lcm_resume_out_t *outParam)
{
    int rtn = SUCCESS;
    lcm_handle_t *lcmHandlePtr = NULL;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    lcmHandlePtr = get_lcm_handle(inParam->lcm_id);
    CHECK_PTR(lcmHandlePtr, NULL, "lcmHandlePtr is null ~_~!\n");

    lcmHandlePtr->lcm_resume.lcm_id = inParam->lcm_id;

    rtn = mol_lcm_resume(lcmHandlePtr);

    EXIT:

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int lcm_ioctrl(lcm_ioctrl_in_t *inParam, lcm_ioctrl_out_t *outParam)
{
    int rtn = SUCCESS;
    lcm_handle_t *lcmHandlePtr = NULL;

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif
    CHECK_PTR(inParam, NULL, "lcm_deinit_in_t is null ~_~!\n");
    CHECK_PTR(outParam, NULL, "lcm_deinit_out_t is null ~_~!\n");

    lcmHandlePtr = get_lcm_handle(inParam->lcm_id);
    CHECK_PTR(lcmHandlePtr, NULL, "lcmHandlePtr is null ~_~!\n");

    #if LCM_HAL_DEBUG
    debug_fun(LCM_HAL_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    EXIT:

    return rtn;
}


