/*
 * gc2053_mipi.c
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "isp_sensor_if.h"
#include "sensor_ops.h"
#include "clock_ops.h"
#include "isp_enum.h"
#include "mipi_api.h"
#include "sensor.h"
#include "gc2053_mipi.h"
#include "sensor_std.h"

#define _NAME(n, s) n##_##s
#define NAME(n) _NAME(n, gc2053_mipi)

#define CCI_ID (0x6e >> 1)
#define CCI_MODE 0

#define REWRITE_CalcSnsValidGain
#define REWRITE_GetExposureRatio
#define REWRITE_Sensor_Isconnect
#include "sns_com.h"

FH_UINT8 regValTable[29][4] = {
    {0x00, 0x00, 0x01, 0x00},
    {0x00, 0x10, 0x01, 0x0c},
    {0x00, 0x20, 0x01, 0x1b},
    {0x00, 0x30, 0x01, 0x2c},
    {0x00, 0x40, 0x01, 0x3f},
    {0x00, 0x50, 0x02, 0x16},
    {0x00, 0x60, 0x02, 0x35},
    {0x00, 0x70, 0x03, 0x16},
    {0x00, 0x80, 0x04, 0x02},
    {0x00, 0x90, 0x04, 0x31},
    {0x00, 0xa0, 0x05, 0x32},
    {0x00, 0xb0, 0x06, 0x35},
    {0x00, 0xc0, 0x08, 0x04},
    {0x00, 0x5a, 0x09, 0x19},
    {0x00, 0x83, 0x0b, 0x0f},
    {0x00, 0x93, 0x0d, 0x12},
    {0x00, 0x84, 0x10, 0x00},
    {0x00, 0x94, 0x12, 0x3a},
    {0x01, 0x2c, 0x1a, 0x02},
    {0x01, 0x3c, 0x1b, 0x20},
    {0x00, 0x8c, 0x20, 0x0f},
    {0x00, 0x9c, 0x26, 0x07},
    {0x02, 0x64, 0x36, 0x21},
    {0x02, 0x74, 0x37, 0x3a},
    {0x00, 0xc6, 0x3d, 0x02},
    {0x00, 0xdc, 0x3f, 0x3f},
    {0x02, 0x85, 0x3f, 0x3f},
    {0x02, 0x95, 0x3f, 0x3f},
    {0x00, 0xce, 0x3f, 0x3f},
};

FH_UINT32 gainLevelTable[30] = {
    64,
    74,
    89,
    102,
    127,
    147,
    177,
    203,
    260,
    300,
    361,
    415,
    504,
    581,
    722,
    832,
    1027,
    1182,
    1408,
    1621,
    1990,
    2291,
    2850,
    3282,
    4048,
    5180,
    5500,
    6744,
    7073,
    0xffffffff};

struct privateParaT{
    FH_UINT32 min_Vblank_adjust;
};

static int Sensor_Isconnect(void *pvObj, FH_UINT8 u8DeviceId)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    pstCtrlPara->u8CciDeviceId = u8DeviceId;
    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId );

    SensorDevice_Close(pstCci);

    return 0;
}

//可根据sns_cfg->max_lane_num确定当前使用2lane配置还是4lane配置
static int SetSensorFmt(void *pvObj, FH_SINT32 s32Format)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_SINT32 s32Ret = SnsCom_SetFmt(pvObj, s32Format, CCI_ID, CCI_MODE);

    if (s32Ret)
        return s32Ret;

    Sensor_Write(pstCci, 0xfd, 0x01);
    pstCtrlPara->u32Vcycle = Sensor_Read(pstCci, 0x4e) << 8;
    pstCtrlPara->u32Vcycle |= Sensor_Read(pstCci, 0x4f);
    pstCtrlPara->u32FullLineStd = pstCtrlPara->u32Vcycle;

    ((struct privateParaT *)pstCtrlPara->pvPrivatePara)->min_Vblank_adjust = Sensor_Read(pstCci, 0x05) << 8;
    ((struct privateParaT *)pstCtrlPara->pvPrivatePara)->min_Vblank_adjust |= Sensor_Read(pstCci, 0x06);

    if (pstCtrlPara->u32WdrFlag == 0)
    {
        pstCtrlPara->u32MinRstLine = 5;
    }
    else
    {
        pstCtrlPara->u32MinRstLine = pstCtrlPara->u32Vcycle / 7 + 5;
    }
    pstCtrlPara->u32MaxIntt = pstCtrlPara->u32Vcycle - pstCtrlPara->u32MinRstLine;
    SetIntt(pvObj, pstCtrlPara->u32MaxIntt, 0);
    SetExposureRatio(pvObj, pstCtrlPara->u32CurrExposureRatio);
    SetGain(pvObj, pstCtrlPara->u32MinAgain, 0);
    pstCtrlPara->u32CurrSnsGain = pstCtrlPara->u32MinAgain;

    return 0;
}

static int GetAEDefault(void *pvObj, Sensor_AE_Default_S *sensAEDefault)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    sensAEDefault->minIntt = 1;
    sensAEDefault->maxIntt = pstCtrlPara->u32MaxIntt;
    sensAEDefault->minAGain = pstCtrlPara->u32MinAgain;
    sensAEDefault->maxAGain = 0xfe80;
    sensAEDefault->fullLineStd = pstCtrlPara->u32FullLineStd;
    sensAEDefault->MinRstLine = pstCtrlPara->u32MinRstLine;

    return 0;
}

// GetGain仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetGain(void *pvObj)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    return pstCtrlPara->u32CurrSnsGain;
}

static int CalcSnsValidGain(void *pvObj, FH_UINT32 *u32Gain)
{
    *u32Gain = (*u32Gain >> 2) << 2;  // U.6->U.4->U.6
    return 0;
}

static int SetGain(void *pvObj, FH_UINT32 again, FH_UINT8 fNo)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    int i;
    int total;
    pstCtrlPara->u32CurrSnsGain = again;
    FH_UINT32 tol_dig_gain = 0;

    total = sizeof(gainLevelTable) / sizeof(FH_UINT32);
    for (i = 0; i < total; i++)
    {
        if ((gainLevelTable[i] <= again) && (again < gainLevelTable[i + 1]))
            break;
    }
    tol_dig_gain = again * 64 / gainLevelTable[i];
    //Sensor_Write(pstCci,0xfe,0x00);

    if (fNo == 0)
    {
        Sensor_Write(pstCci,0xfe, 0x00);
        //Sensor_Write(pstCci,0x87,0x58);//[6] aec delay mode
        Sensor_Write(pstCci,0xb4, regValTable[i][0]);
        Sensor_Write(pstCci,0xb3, regValTable[i][1]);
        Sensor_Write(pstCci,0xb8, regValTable[i][2]);
        Sensor_Write(pstCci,0xb9, regValTable[i][3]);

        Sensor_Write(pstCci,0xb1, (tol_dig_gain >> 6));
        Sensor_Write(pstCci,0xb2, ((tol_dig_gain & 0x3f) << 2));
    }

    return 0;
}

// GetIntt仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetIntt(void *pvObj)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    int intt_h, intt_l, intt;

    Sensor_Write(pstCci, 0xfe, 0x00);
    intt_h = Sensor_Read(pstCci, 0x03) & 0xff;
    intt_l = Sensor_Read(pstCci, 0x04) & 0xff;

    intt = (intt_h << 8) | intt_l;

    return intt;
}

static int SetIntt(void *pvObj, FH_UINT32 intt, FH_UINT8 fNo)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    FH_UINT32 intt_h, intt_l;

    intt = MAX(1, intt);
    intt_h = (intt >> 8) & 0xff;
    intt_l = intt & 0xff;

    Sensor_Write(pstCci, 0xfe, 0x00);
    Sensor_Write(pstCci, 0x03, intt_h);
    Sensor_Write(pstCci, 0x04, intt_l);

    return 0;
}

static int SetSnsFrameH(void *pvObj, FH_UINT32 frameH)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    // SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    // FH_UINT32 vblank, vblank_h, vblank_l;

    // Sensor_Write(pstCci, 0xfd, 0x01);
    // pstCtrlPara->u32Vcycle = Sensor_Read(pstCci, 0x4e) << 8;
    // pstCtrlPara->u32Vcycle |= Sensor_Read(pstCci, 0x4f);
    // vblank = Sensor_Read(pstCci, 0x05) << 8;
    // vblank |= Sensor_Read(pstCci, 0x06);

    // vblank = MAX(((struct privateParaT *)pstCtrlPara->pvPrivatePara)->min_Vblank_adjust, (vblank + frameH - pstCtrlPara->u32Vcycle));

    // vblank_h = (vblank >> 8) & 0xff;
    // vblank_l = vblank & 0xff;

    // Sensor_Write(pstCci, 0xfd, 0x01);
    // Sensor_Write(pstCci, 0x05, vblank_h);
    // Sensor_Write(pstCci, 0x06, vblank_l);
    // Sensor_Write(pstCci, 0x01, 0x01);

    pstCtrlPara->u32Vcycle = frameH;

    return 0;
}

static int GetExposureRatio(void *pvObj, FH_UINT32 *exposure_ratio)
{
    // SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    // *exposure_ratio = (pstCtrlPara->u32CurrSnsIntt  << 4) / MAX(1, pstCtrlPara->u32CurrSnsShortIntt);
    *exposure_ratio = 0x100;
    return 0;
}

static int SetSensorFlipMirror(void *pvObj, FH_UINT32 sensor_en_stat)
{
    // SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    // FH_UINT8 tmp1;
    // FH_UINT8 flip, mirror;

    // flip = sensor_en_stat & 0x1;
    // mirror = (sensor_en_stat >> 1) & 0x1;

    // Sensor_Write(pstCci, 0xfd, 0x01);
    // tmp1 = Sensor_Read(pstCci, 0x3f);  // bit[0]:mirror  bit[1]:flip
    // tmp1 = (tmp1&0xfc) | (1-mirror) | (flip << 1);

    // Sensor_Write(pstCci, 0xfd, 0x01);
    // Sensor_Write(pstCci, 0x3f, tmp1);
    // Sensor_Write(pstCci, 0x01, 0x01);

    return 0;
}

static int GetSensorFlipMirror(void *pvObj, FH_UINT32 *sensor_en_stat)
{
    // SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    // FH_UINT8 mirror, flip;

    // Sensor_Write(pstCci, 0xfd, 0x01);
    // mirror = 1 - (Sensor_Read(pstCci, 0x3f) & 0x1);
    // Sensor_Write(pstCci, 0xfd, 0x01);
    // flip = (Sensor_Read(pstCci, 0x3f)>>1) & 0x1;
    // *sensor_en_stat = (mirror << 1) | flip;

    return 0;
}

/***********************not used for now end************************************/

#define SENSOR_GC2053_MIPI "gc2053_mipi"
#ifdef OS_RTT
struct isp_sensor_if gc2053_mipi_sensor_if_default __attribute__((section(".sensor_drv"))) = {
#else
struct isp_sensor_if gc2053_mipi_sensor_if_default = {
#endif
    .name = SENSOR_GC2053_MIPI,
    .get_vi_attr = GetSensorViAttr,
    .set_flipmirror = SetSensorFlipMirror,
    .get_flipmirror = GetSensorFlipMirror,
    .set_iris = SetSensorIris,
    .init = Sensor_Init,
    .reset = SensorReset,
    .deinit = Sensor_DeInit,
    .set_sns_fmt = SetSensorFmt,
    .kick = Sensor_Kick,
    .set_sns_reg = SetSensorReg,
    .set_exposure_ratio = SetExposureRatio,
    .get_exposure_ratio = GetExposureRatio,
    .get_sensor_attribute = GetSensorAttribute,
    .set_lane_num_max = SetLaneNumMax,
    .get_sns_reg = GetSensorReg,
    .get_awb_gain = GetSensorAwbGain,
    .set_awb_gain = SetSensorAwbGain,
    .data = 0,
    .para = 0,
    .common_if = SensorCommonIf,
    .get_sns_ae_default = GetAEDefault,
    .get_sns_ae_info = GetAEInfo,
    .set_sns_intt = SetIntt,
    .calc_sns_valid_intt = CalcSnsValidIntt,
    .set_sns_gain = SetGain,
    .calc_sns_valid_gain = CalcSnsValidGain,
    .set_sns_frame_height = SetSnsFrameH,
    .get_sensor_mirror_flip_bayer_format = GetMirrorFlipBayerFormat,
    .get_user_awb_gain = GetUserSensorAwbGain,
    .get_user_ltm_curve = GetSensorLtmCurve,
    .is_sensor_connect = Sensor_Isconnect,
};

struct isp_sensor_if *NAME(Sensor_Create)()
{
    struct isp_sensor_if *s_if;
    SNS_PARA_S* pstPara;

    s_if = (struct isp_sensor_if *)malloc(sizeof(struct isp_sensor_if));
    memcpy(s_if, &gc2053_mipi_sensor_if_default, sizeof(struct isp_sensor_if));

    pstPara = (SNS_PARA_S *)malloc(sizeof(SNS_PARA_S));
    memcpy(&pstPara->stCtrlPara, &stSnsDefaultCtrlPara, sizeof(SNS_CTRL_PARA_S));
    memset(&pstPara->stCci, 0, sizeof(SensorDevice));

    struct privateParaT *privPara = (struct privateParaT *)malloc(sizeof(struct privateParaT));
    pstPara->stCtrlPara.pvPrivatePara = (void *)privPara;

    s_if->para = pstPara;

    return s_if;
}

void NAME(Sensor_Destroy)(struct isp_sensor_if *s_if)
{
    free(((SNS_PARA_S *)s_if->para)->stCtrlPara.pvPrivatePara);
    ((SNS_PARA_S *)s_if->para)->stCtrlPara.pvPrivatePara = 0;
    free(s_if->para);
    s_if->para = 0;
    free(s_if);
    s_if = 0;
}
