/*
 * imx415_mipi.c
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "clock_ops.h"

#include "isp_sensor_if.h"
#include "sensor_ops.h"

#include "isp_enum.h"
#include "mipi_api.h"
#include "sensor.h"
#include "imx415_mipi.h"
#include "sensor_std.h"

#define _NAME(n, s) n##_##s
#define NAME(n) _NAME(n, imx415_mipi)
#define CCI_ID (0x34) >> 1
#define CCI_MODE 2

#define REWRITE_Sensor_Init
#define REWRITE_Sensor_Isconnect
#include "sns_com.h"

static unsigned int RHS1 = 0;
static int err_sum = 0;
extern const unsigned short IMX415RegToGain[201];


static int Sensor_Init(void *pvObj, Sensor_Init_t* stInitConf)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    pstCtrlPara->u8CciDeviceId  = stInitConf->u8CciDeviceId;
    pstCtrlPara->u8CsiDeviceId  = stInitConf->u8CsiDeviceId;

    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId);

    SensorReset(pvObj);

    pstCtrlPara->bInited = 1;


    return 0;
}

static int Sensor_Isconnect(void *pvObj, FH_UINT8 u8DeviceId)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 u32SnsChipId;

    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId);

    u32SnsChipId = Sensor_Read(pstCci, 0x300c);
    u32SnsChipId |= Sensor_Read(pstCci, 0x300b) << 8;
    u32SnsChipId |= Sensor_Read(pstCci, 0x300a) << 16;
    SensorDevice_Close(pstCci);

    if (u32SnsChipId == 0x530541)
      return 1;
    else
    {
      return 0;
    }
}

static int SetSensorFmt(void *pvObj, FH_SINT32 s32Format)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_SINT32 s32Ret = SnsCom_SetFmt(pvObj, s32Format, CCI_ID, CCI_MODE);

    if (s32Ret)
        return s32Ret;

    pstCtrlPara->u32Vcycle = Sensor_Read(pstCci, 0x3024);
    pstCtrlPara->u32Vcycle |= Sensor_Read(pstCci, 0x3025) << 8;
    pstCtrlPara->u32Vcycle |= (Sensor_Read(pstCci, 0x3026) & 0xf) << 16;
    pstCtrlPara->u32FullLineStd = pstCtrlPara->u32Vcycle;

    //GET RHS1
    RHS1 = Sensor_Read(pstCci, 0x3030);
    RHS1 |= Sensor_Read(pstCci, 0x3031) << 8;
    RHS1 |= (Sensor_Read(pstCci, 0x3032) & 0xf) << 16;

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

static int GetAEDefault(void* pvObj, Sensor_AE_Default_S *sensAEDefault)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);

    sensAEDefault->minIntt = 1;
    sensAEDefault->maxIntt = pstCtrlPara->u32MaxIntt;
    sensAEDefault->minAGain = pstCtrlPara->u32MinAgain;
    sensAEDefault->maxAGain = 0x3f;
    sensAEDefault->fullLineStd = pstCtrlPara->u32FullLineStd;
    sensAEDefault->MinRstLine = pstCtrlPara->u32MinRstLine;
    return 0;

}

static int GetGain(void* pvObj)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    FH_UINT32 gain_val, gain;

    gain_val  = Sensor_Read(pstCci, 0x3090);
    if (gain_val >200)
        gain_val = 200;

    if(gain_val <0)
        gain_val = 0;

    gain = IMX415RegToGain[gain_val];

    return gain;

}

static int SetGain(void* pvObj, FH_UINT32 again, FH_UINT8 fNo)
{
    FH_UINT32 gain_val;
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    int i;

    again += err_sum;

    for(i=0; i<201; i++)
    {
        if(IMX415RegToGain[i] > again)
            break;
    }
    gain_val = i - 1;

    err_sum = again - IMX415RegToGain[gain_val];

    Sensor_Write(pstCci, 0x3090, gain_val);

    return 0;

}

static int GetIntt(void* pvObj)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);

    return pstCtrlPara->u32CurrSnsIntt;
}

static int Set_short_exp_intt(void* pvObj, FH_UINT32 intt)
{
    return 0;

}

static int SetIntt(void* pvObj, FH_UINT32 intt, FH_UINT8 fNo)
{
    FH_UINT32 intt_invert = 0;
    FH_UINT16 intt_low, intt_high, intt_hh, line;
    FH_UINT16 line_low, line_high;

    FH_UINT32 FSC;
    FH_UINT32 SHS2, SHS2_l, SHS2_h, SHS2_hh;
    FH_UINT32 Intt_LEF;

    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    pstCtrlPara->u32CurrSnsIntt = intt;
    if (pstCtrlPara->u32WdrFlag)
    {
        Intt_LEF = intt;//U.4->U.0
        FSC  = pstCtrlPara->u32Vcycle * 2;
        SHS2 = FSC - Intt_LEF - 1;
        if(SHS2 < (RHS1+2))
        {
            SHS2 = RHS1+2;
        }
        else if(SHS2 > (FSC-2))
        {
            SHS2 = FSC-2;
        }
        SHS2_l = SHS2&0xff;
        SHS2_h = (SHS2>>8)&0xff;
        SHS2_hh = (SHS2>>16)&0xf;

        Sensor_Write(pstCci, 0x3024,SHS2_l);
        Sensor_Write(pstCci, 0x3025,SHS2_h);
        Sensor_Write(pstCci, 0x3026,SHS2_hh);

        //WDR mode: set short frame intt
        FH_UINT32 short_exp_intt;
        short_exp_intt = MAX(1, ((MIN(intt, pstCtrlPara->u32MaxIntt) << 4) + (pstCtrlPara->u32CurrExposureRatio >> 1)) / pstCtrlPara->u32CurrExposureRatio);
        Set_short_exp_intt(pvObj, short_exp_intt);
    }
    else
    {
        line_low = Sensor_Read(pstCci, 0x3024);
        line_high= Sensor_Read(pstCci, 0x3025);
        line = ((line_high<<8)|line_low);
        intt_invert = line - intt;

        intt_low  =  (FH_UINT16)(intt_invert&0x000000ff);
        intt_high =  (FH_UINT16)((intt_invert>>8)&0x000000ff);
        intt_hh   =  (FH_UINT16)((intt_invert>>16)&0x000000ff);

        Sensor_Write(pstCci, 0x3050, intt_low);
        Sensor_Write(pstCci, 0x3051, intt_high);
        Sensor_Write(pstCci, 0x3052, intt_hh);
    }

    return 0;

}

static int SetSnsFrameH(void* pvObj, FH_UINT32 frameH)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    Sensor_Write(pstCci, 0x3025, frameH>>8);
    Sensor_Write(pstCci, 0x3024, frameH&0xff);
    pstCtrlPara->u32Vcycle = frameH;
    return 0;

}


static int SetSensorFlipMirror(void* pvObj, FH_UINT32 sensor_en_stat)
{
    return 0;

}

static int GetSensorFlipMirror(void* pvObj, FH_UINT32 *sensor_en_stat)
{
    return 0;

}

#define SENSOR_IMX415_MIPI    "imx415_mipi"
#ifdef OS_RTT
struct isp_sensor_if imx415_mipi_sensor_if __attribute__((section(".sensor_drv"))) = {
#else
struct isp_sensor_if imx415_mipi_sensor_if = {
#endif

    .name = SENSOR_IMX415_MIPI,
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
    .common_if = SensorCommonIf,
    .get_sns_ae_default = GetAEDefault,
    .get_sns_ae_info = GetAEInfo,
    .set_sns_intt = SetIntt,
    .calc_sns_valid_intt = CalcSnsValidIntt,
    .set_sns_gain =  SetGain,
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
    memcpy(s_if, &imx415_mipi_sensor_if, sizeof(struct isp_sensor_if));

    pstPara = (SNS_PARA_S *)malloc(sizeof(SNS_PARA_S));
    memcpy(&pstPara->stCtrlPara, &stSnsDefaultCtrlPara, sizeof(SNS_CTRL_PARA_S));
    memset(&pstPara->stCci, 0, sizeof(SensorDevice));

    s_if->para = pstPara;

    return s_if;
}
void NAME(Sensor_Destroy)(struct isp_sensor_if *s_if)
{
    free(s_if->para);
    s_if->para = 0;
    free(s_if);
    s_if = 0;
}
