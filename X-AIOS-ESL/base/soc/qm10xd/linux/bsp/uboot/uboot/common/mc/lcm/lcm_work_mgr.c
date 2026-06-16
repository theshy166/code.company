#include "../lcm_module/include/data_type.h"
#include "../lcm_module/include/mol_lcm_local.h"
#include "../lcm_module/include/mol_lcm.h"
#include "lcm_work_mgr_param.h"
#include "debug.h"

#define LCM_MGR_FUN_DEBUG 0
#define LCM_MGR_PARAM_DEBUG 0

typedef struct {
    lcm_work_mode_e mode;
    char name[64];
}lcm_work_mode_description_t;

lcm_work_mode_description_t s_lcm_work_mode_description[] ={
    {LCM_WORK_NONE,          "LCM_WORK_NONE"         },
    {LCM_WORK_1C4D,          "LCM_WORK_1C4D"         },
    {LCM_WORK_1C3D,          "LCM_WORK_1C3D"         },
    {LCM_WORK_1C2D,          "LCM_WORK_1C2D"         },
    {LCM_WORK_1C1D,          "LCM_WORK_1C1D"         },
    {LCM_WORK_RGB_6,         "LCM_WORK_RGB_6"        },
    {LCM_WORK_RGB_16,        "LCM_WORK_RGB_16"       },
    {LCM_WORK_RGB_18,        "LCM_WORK_RGB_18"       },
    {LCM_WORK_RGB_24,        "LCM_WORK_RGB_24"       },
    {LCM_WORK_MCU_8,         "LCM_WORK_MCU_8"        },
    {LCM_WORK_MCU_9,         "LCM_WORK_MCU_9"        },
    {LCM_WORK_MCU_16,        "LCM_WORK_MCU_16"       },
    {LCM_WORK_MCU_18,        "LCM_WORK_MCU_18"       },
    {LCM_WORK_MCU_8_NO_RD,   "LCM_WORK_MCU_8_NO_RD"  },
    {LCM_WORK_MCU_9_NO_RD,   "LCM_WORK_MCU_9_NO_RD"  },
    {LCM_WORK_MCU_16_NO_RD,  "LCM_WORK_MCU_16_NO_RD" },
    {LCM_WORK_MCU_18_NO_RD,  "LCM_WORK_MCU_18_NO_RD" },
    {LCM_WORK_MODE_MAX,      "LCM_WORK_MODE_MAX"     },
};

lcm_work_mode_e s_lcm_work_mode[2] = {0x00};

lcm_work_mode_e *get_lcm_work_mode_context(void)
{
    return s_lcm_work_mode;
};

char *get_lcm_work_mode_str(uint32_t lcmWorkMode)
{
    char *name_str = NULL;

    if (LCM_WORK_MODE_MAX > lcmWorkMode) {
        name_str = s_lcm_work_mode_description[lcmWorkMode].name;
    }

    return name_str;
};

lcm_work_mode_e get_lcm_work_mode(lcm_cfg_t *inParam)
{
    lcm_work_mode_e lcm_work_mode = LCM_WORK_NONE;

    #if LCM_MGR_FUN_DEBUG
    debug_fun(LCM_MGR_FUN_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    if (LCM_TYPE_MIPI == inParam->type) {
        #if LCM_MGR_PARAM_DEBUG
        printf("lcm type LCM_TYPE_MIPI\n");
        printf("mipi->lane_num : %d\n", inParam->infor.mipi->lane_num);
        #endif
        if (DSI_ONE_LANE == inParam->infor.mipi->lane_num) {
            lcm_work_mode = LCM_WORK_1C1D;
        } else if (DSI_TWO_LANE == inParam->infor.mipi->lane_num) {
            lcm_work_mode = LCM_WORK_1C2D;
        } else if (DSI_THREE_LANE == inParam->infor.mipi->lane_num) {
            lcm_work_mode = LCM_WORK_1C3D;
        } else if (DSI_FOUR_LANE == inParam->infor.mipi->lane_num) {
            lcm_work_mode = LCM_WORK_1C4D;
        } else{
            printf("%s, lane num ：%d ~_~!\n", inParam->name, inParam->infor.mipi->lane_num);
        }
    } else if (LCM_TYPE_RGB == inParam->type) {
        #if LCM_MGR_PARAM_DEBUG
        printf("lcm type LCM_TYPE_RGB\n");
        printf("rgb->if_type : %d\n", inParam->infor.rgb->if_type);
        #endif
        if (RGB_IF_6BITS == inParam->infor.rgb->if_type) {
            lcm_work_mode = LCM_WORK_RGB_6;
        } else if (RGB_IF_16BITS == inParam->infor.rgb->if_type) {
            lcm_work_mode = LCM_WORK_RGB_16;
        } else if (RGB_IF_18BITS == inParam->infor.rgb->if_type) {
            lcm_work_mode = LCM_WORK_RGB_18;
        } else if (RGB_IF_24BITS == inParam->infor.rgb->if_type) {
            lcm_work_mode = LCM_WORK_RGB_24;
        } else{
            printf("%s, if_type ：%d ~_~!\n", inParam->name, inParam->infor.rgb->if_type);
        }
    } else if (LCM_TYPE_MCU == inParam->type) {
        #if LCM_MGR_PARAM_DEBUG
        printf("lcm type LCM_TYPE_MCU\n");
        printf("rgb->if_type : %d\n", inParam->infor.mcu->if_type);
        printf("mcu->rd_eb : %d\n", inParam->infor.mcu->rd_eb);
        #endif
        if (MCU_IF_8BITS == inParam->infor.mcu->if_type) {
            if (0 == inParam->infor.mcu->rd_eb) {
                lcm_work_mode = LCM_WORK_MCU_8_NO_RD;
            } else {
                lcm_work_mode = LCM_WORK_MCU_8;
            }
        } else if (MCU_IF_9BITS == inParam->infor.mcu->if_type) {
            if (0 == inParam->infor.mcu->rd_eb) {
                lcm_work_mode = LCM_WORK_MCU_9_NO_RD;
            } else {
                lcm_work_mode = LCM_WORK_MCU_9;
            }
        } else if (MCU_IF_16BITS == inParam->infor.mcu->if_type) {
            if (0 == inParam->infor.mcu->rd_eb) {
                lcm_work_mode = LCM_WORK_MCU_16_NO_RD;
            } else {
                lcm_work_mode = LCM_WORK_MCU_16;
            }
        } else if (MCU_IF_18BITS == inParam->infor.mcu->if_type) {
            if (0 == inParam->infor.mcu->rd_eb) {
                lcm_work_mode = LCM_WORK_MCU_18_NO_RD;
            } else {
                lcm_work_mode = LCM_WORK_MCU_18;
            }
        } else {
            printf("%s, if_type ：%d , rd_eb ：%d ~_~!\n", inParam->name, inParam->infor.mcu->if_type, inParam->infor.mcu->rd_eb);
        }
    }

    #if LCM_MGR_PARAM_DEBUG
    printf("lcm_work_mode: %s\n", get_lcm_work_mode_str(lcm_work_mode));
    #endif

    #if LCM_MGR_FUN_DEBUG
    debug_fun(LCM_MGR_FUN_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    return lcm_work_mode;
}

int32_t set_lcm_work_mode(uint32_t lcmId, lcm_work_mode_e lcmWorkMode)
{
    int32_t rtn = SUCCESS;

    lcm_work_mode_e *lcm_work_mode = get_lcm_work_mode_context();

    lcm_work_mode[lcmId] = lcmWorkMode;

    return rtn;
}

int32_t check_lcm_work_mode(uint32_t lcmId, lcm_work_mode_e lcmWorkMode)
{
    int32_t rtn = FAIL_INVALID;
    lcm_work_mode_e *lcm_work_mode = get_lcm_work_mode_context();

    #if LCM_MGR_FUN_DEBUG
    debug_fun(LCM_MGR_FUN_DEBUG, __FUNCTION__, __LINE__, "start");
    #endif

    #if LCM_MGR_PARAM_DEBUG
    printf("LCM_ID_MAIN: %s\n", get_lcm_work_mode_str(lcm_work_mode[LCM_ID_MAIN]));
    printf("LCM_ID_SUB: %s\n", get_lcm_work_mode_str(lcm_work_mode[LCM_ID_SUB]));
    #endif

    if ((LCM_WORK_NONE != lcm_work_mode[LCM_ID_MAIN])
        && (LCM_WORK_NONE != lcm_work_mode[LCM_ID_SUB])) {
        rtn = FAIL_RE_SET;
        EXIT_IF_FAIL(rtn, NULL, "LCM_ID_MAIN/LCM_ID_SUB is allready work ~_~!\n");
    } else if ((LCM_WORK_NONE == lcm_work_mode[LCM_ID_MAIN])
                && (LCM_WORK_NONE == lcm_work_mode[LCM_ID_SUB])) {
        if(LCM_ID_MAIN == lcmId) {
            if((LCM_WORK_1C4D == lcmWorkMode)
                ||(LCM_WORK_1C3D == lcmWorkMode)
                ||(LCM_WORK_1C2D == lcmWorkMode)
                ||(LCM_WORK_1C1D == lcmWorkMode)
                ||(LCM_WORK_RGB_6 == lcmWorkMode)
                ||(LCM_WORK_RGB_16 == lcmWorkMode)
                ||(LCM_WORK_RGB_18 == lcmWorkMode)
                ||(LCM_WORK_RGB_24 == lcmWorkMode)
                ||(LCM_WORK_MCU_8 == lcmWorkMode)
                ||(LCM_WORK_MCU_9 == lcmWorkMode)
                ||(LCM_WORK_MCU_16 == lcmWorkMode)
                ||(LCM_WORK_MCU_18 == lcmWorkMode)
                ||(LCM_WORK_MCU_8_NO_RD == lcmWorkMode)
                ||(LCM_WORK_MCU_9_NO_RD == lcmWorkMode)
                ||(LCM_WORK_MCU_16_NO_RD == lcmWorkMode)
                ||(LCM_WORK_MCU_18_NO_RD == lcmWorkMode)) {
                lcm_work_mode[LCM_ID_MAIN] = lcmWorkMode;
                rtn = SUCCESS;
            } else {
                rtn = FAIL_INVALID;
                EXIT_IF_FAIL(rtn, NULL, "LCM_ID_MAIN work %d ~_~!\n", lcmWorkMode);
            }
        }else if (LCM_ID_SUB == lcmId) {
                if((LCM_WORK_1C2D == lcmWorkMode)
                    ||(LCM_WORK_1C1D == lcmWorkMode)
                    ||(LCM_WORK_RGB_6 == lcmWorkMode)
                    ||(LCM_WORK_RGB_16 == lcmWorkMode)
                    ||(LCM_WORK_RGB_18 == lcmWorkMode)
                    ||(LCM_WORK_RGB_24 == lcmWorkMode)
                    ||(LCM_WORK_MCU_8 == lcmWorkMode)
                    ||(LCM_WORK_MCU_9 == lcmWorkMode)
                    ||(LCM_WORK_MCU_16 == lcmWorkMode)
                    ||(LCM_WORK_MCU_18 == lcmWorkMode)
                    ||(LCM_WORK_MCU_8_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_9_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_16_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_18_NO_RD == lcmWorkMode)) {
                    rtn = SUCCESS;
                } else {
                    rtn = FAIL_INVALID;
                    EXIT_IF_FAIL(rtn, NULL, "LCM_ID_SUB work %d ~_~!\n", lcmWorkMode);
                }
        }
    } else if ((LCM_WORK_NONE != lcm_work_mode[LCM_ID_MAIN])
        && (LCM_WORK_NONE == lcm_work_mode[LCM_ID_SUB])) {

        if (LCM_ID_SUB != lcmId) {
            rtn = FAIL_INVALID;
            EXIT_IF_FAIL(rtn, NULL, "LCM_ID_SUB work %d ~_~!\n", lcmWorkMode);
        }

        if ((LCM_WORK_1C4D == lcm_work_mode[LCM_ID_MAIN])
            ||(LCM_WORK_1C3D == lcm_work_mode[LCM_ID_MAIN])){
            if ((LCM_WORK_RGB_6 == lcmWorkMode)
                ||(LCM_WORK_RGB_16 == lcmWorkMode)
                ||(LCM_WORK_RGB_18 == lcmWorkMode)
                ||(LCM_WORK_MCU_8_NO_RD == lcmWorkMode)
                ||(LCM_WORK_MCU_9_NO_RD == lcmWorkMode)
                ||(LCM_WORK_MCU_16_NO_RD == lcmWorkMode)
                ||(LCM_WORK_MCU_18_NO_RD == lcmWorkMode)) {
                rtn = SUCCESS;
            }
        } else if ((LCM_WORK_1C2D == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_1C1D == lcm_work_mode[LCM_ID_MAIN])){
            if ((LCM_WORK_1C2D == lcmWorkMode)
                    ||(LCM_WORK_1C1D == lcmWorkMode)
                    ||(LCM_WORK_RGB_6 == lcmWorkMode)
                    ||(LCM_WORK_RGB_16 == lcmWorkMode)
                    ||(LCM_WORK_RGB_18 == lcmWorkMode)
                    ||(LCM_WORK_RGB_24 == lcmWorkMode)
                    ||(LCM_WORK_MCU_8 == lcmWorkMode)
                    ||(LCM_WORK_MCU_9 == lcmWorkMode)
                    ||(LCM_WORK_MCU_16 == lcmWorkMode)
                    ||(LCM_WORK_MCU_18 == lcmWorkMode)
                    ||(LCM_WORK_MCU_8_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_9_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_16_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_18_NO_RD == lcmWorkMode)) {
                rtn = SUCCESS;
            }
        } else if ((LCM_WORK_MCU_8 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_9 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_16 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_18 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_RGB_24 == lcm_work_mode[LCM_ID_MAIN])) {
                rtn = FAIL_INVALID;
        } else if ((LCM_WORK_MCU_8_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_9_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_16_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_18_NO_RD == lcm_work_mode[LCM_ID_MAIN])) {
            if ((LCM_WORK_1C2D == lcmWorkMode)
                || (LCM_WORK_1C1D == lcmWorkMode)) {
                rtn = SUCCESS;
            }
        } else if ((LCM_WORK_RGB_6 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_RGB_16 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_RGB_18 == lcm_work_mode[LCM_ID_MAIN])){
            if (LCM_WORK_1C2D == lcmWorkMode) {
                rtn = SUCCESS;
            }
        }
    } else if ((LCM_WORK_NONE != lcm_work_mode[LCM_ID_MAIN])
        && (LCM_WORK_NONE == lcm_work_mode[LCM_ID_SUB])) {

        if (LCM_ID_MAIN != lcmId) {
            rtn = FAIL_INVALID;
            EXIT_IF_FAIL(rtn, NULL, "LCM_ID_SUB work %d ~_~!\n", lcmWorkMode);
        }

        if ((LCM_WORK_1C2D == lcm_work_mode[LCM_ID_SUB])
            || (LCM_WORK_1C2D == lcm_work_mode[LCM_ID_SUB])){
            if ((LCM_WORK_1C2D == lcmWorkMode)
                    ||(LCM_WORK_1C1D == lcmWorkMode)
                    ||(LCM_WORK_RGB_6 == lcmWorkMode)
                    ||(LCM_WORK_RGB_16 == lcmWorkMode)
                    ||(LCM_WORK_RGB_18 == lcmWorkMode)
                    ||(LCM_WORK_MCU_8_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_9_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_16_NO_RD == lcmWorkMode)
                    ||(LCM_WORK_MCU_18_NO_RD == lcmWorkMode)) {
                rtn = SUCCESS;
            }
        } else if ((LCM_WORK_MCU_8 == lcm_work_mode[LCM_ID_SUB])
                    || (LCM_WORK_MCU_9 == lcm_work_mode[LCM_ID_SUB])
                    || (LCM_WORK_MCU_16 == lcm_work_mode[LCM_ID_SUB])
                    || (LCM_WORK_MCU_18 == lcm_work_mode[LCM_ID_SUB])
                    || (LCM_WORK_RGB_24 == lcm_work_mode[LCM_ID_SUB])) {
            if ((LCM_WORK_1C2D == lcmWorkMode)
                || (LCM_WORK_1C1D == lcmWorkMode)) {
                rtn = SUCCESS;
            }
        } else if ((LCM_WORK_MCU_8_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_9_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_16_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_MCU_18_NO_RD == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_RGB_6 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_RGB_16 == lcm_work_mode[LCM_ID_MAIN])
                    || (LCM_WORK_RGB_18 == lcm_work_mode[LCM_ID_MAIN])){
            if ((LCM_WORK_1C4D == lcmWorkMode)
                ||(LCM_WORK_1C3D == lcmWorkMode)
                ||(LCM_WORK_1C2D == lcmWorkMode)
                ||(LCM_WORK_1C1D == lcmWorkMode)){
                rtn = SUCCESS;
            }
        }
    }

    if (SUCCESS == rtn) {
        set_lcm_work_mode(lcmId, lcmWorkMode);
    } else {
        printf("--cur lcm_main_woek_mode: %s\n", get_lcm_work_mode_str(lcm_work_mode[LCM_ID_MAIN]));
        printf("--cur lcm_sub_work_mode: %s\n", get_lcm_work_mode_str(lcm_work_mode[LCM_ID_SUB]));
        if (LCM_ID_MAIN == lcmId) {
            printf("set lcm_main_woek_mode: %s ~_~!\n", get_lcm_work_mode_str(lcmWorkMode));
        } else if (LCM_ID_SUB == lcmId) {
            printf("--set lcm_sub_work_mode: %s ~_~!\n", get_lcm_work_mode_str(lcmWorkMode));
        }
    }

    #if LCM_MGR_PARAM_DEBUG
    printf("LCM_ID_MAIN: %s\n", get_lcm_work_mode_str(lcm_work_mode[LCM_ID_MAIN]));
    printf("LCM_ID_SUB: %s\n", get_lcm_work_mode_str(lcm_work_mode[LCM_ID_SUB]));
    #endif

    #if LCM_MGR_FUN_DEBUG
    debug_fun(LCM_MGR_FUN_DEBUG, __FUNCTION__, __LINE__, "end");
    #endif

    EXIT:

    return rtn;
}

