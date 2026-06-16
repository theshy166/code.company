/*
 * k351p_dvp.c
 *
 *  Created on: Sep 14, 2015
 *      Author: duobao
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
#include "k351p_dvp.h"
#include "sensor_std.h"


#define _NAME(n, s) n##_##s
#define NAME(n) _NAME(n, k351p_dvp)
#define k351p_dvp_intt_step_max 0xa0

#define CCI_ID (0x80 >> 1)
#define CCI_MODE 0

#define REWRITE_GetSensorViAttr
#define REWRITE_Sensor_Init
#define REWRITE_GetMirrorFlipBayerFormat
#define REWRITE_GetSensorAttribute
#define REWRITE_CalcSnsValidGain
#define REWRITE_SnsCom_SetFmt
#define REWRITE_SetSensorMipiCfg
#define REWRITE_getSensorConf
#define REWRITE_Sensor_Isconnect
#define REWRITE_getSensorConfType
#define REWRITE_Sensor_Kick
#define REWRITE_Sensor_DeInit
#include "sns_com.h"

static unsigned int MirrorFlipBayerFormat[4] =
{
    2, 3, 1, 0
};

struct privateParaT{
    FH_UINT8 sun_spot_status;
    FH_BOOL sun_spot_flag;
};

static struct privateParaT private_para_default = {0,1};

static int GetSensorViAttr(void *pvObj, ISP_VI_ATTR_S *stViAttr)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    if (stViAttr == NULL)
        return FH_RET_SENSOR_NULL_POINTER;

    switch (pstCtrlPara->u32SnsFmt)
    {
        case FORMAT_500P2:
            stViAttr->u16WndWidth = FRAM_W_500P2;
            stViAttr->u16WndHeight = FRAM_H_500P2;
            stViAttr->u16InputWidth = ACTIVE_W_500P2;
            stViAttr->u16InputHeight = ACTIVE_H_500P2;
            stViAttr->u16PicWidth = PIC_IN_W_500P2;
            stViAttr->u16PicHeight = PIC_IN_H_500P2;
            stViAttr->u16OffsetX = OFFSET_X_500P2;
            stViAttr->u16OffsetY = OFFSET_Y_500P2;
            stViAttr->u16FrameRate = 2,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_500P5:
            stViAttr->u16WndWidth = FRAM_W_500P5;
            stViAttr->u16WndHeight = FRAM_H_500P5;
            stViAttr->u16InputWidth = ACTIVE_W_500P5;
            stViAttr->u16InputHeight = ACTIVE_H_500P5;
            stViAttr->u16PicWidth = PIC_IN_W_500P5;
            stViAttr->u16PicHeight = PIC_IN_H_500P5;
            stViAttr->u16OffsetX = OFFSET_X_500P5;
            stViAttr->u16OffsetY = OFFSET_Y_500P5;
            stViAttr->u16FrameRate = 5,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_400WP15:
            stViAttr->u16WndWidth = FRAM_W_2000P15;
            stViAttr->u16WndHeight = FRAM_H_2000P15;
            stViAttr->u16InputWidth = ACTIVE_W_2000P15;
            stViAttr->u16InputHeight = ACTIVE_H_2000P15;
            stViAttr->u16PicWidth = PIC_IN_W_2000P15;
            stViAttr->u16PicHeight = PIC_IN_H_2000P15;
            stViAttr->u16OffsetX = OFFSET_X_2000P15;
            stViAttr->u16OffsetY = OFFSET_Y_2000P15;
            stViAttr->u16FrameRate = 15,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_400WP15_SLAVE:
            stViAttr->u16WndWidth = FRAM_W_2000P15;
            stViAttr->u16WndHeight = FRAM_H_2000P15;
            stViAttr->u16InputWidth = ACTIVE_W_2000P15;
            stViAttr->u16InputHeight = ACTIVE_H_2000P15;
            stViAttr->u16PicWidth = PIC_IN_W_2000P15;
            stViAttr->u16PicHeight = PIC_IN_H_2000P15;
            stViAttr->u16OffsetX = OFFSET_X_2000P15;
            stViAttr->u16OffsetY = OFFSET_Y_2000P15;
            stViAttr->u16FrameRate = 15,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_250X250P60:
            stViAttr->u16WndWidth = FRAM_W_250X250P60;
            stViAttr->u16WndHeight = FRAM_H_250X250P60;
            stViAttr->u16InputWidth = ACTIVE_W_250X250P60;
            stViAttr->u16InputHeight = ACTIVE_H_250X250P60;
            stViAttr->u16PicWidth = PIC_IN_W_250X250P60;
            stViAttr->u16PicHeight = PIC_IN_H_250X250P60;
            stViAttr->u16OffsetX = OFFSET_X_250X250P60;
            stViAttr->u16OffsetY = OFFSET_Y_250X250P60;
            stViAttr->u16FrameRate = 60,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        default:
            return FH_RET_SENSOR_NO_SUPPORT_FORMAT;
    }

    pstCtrlPara->u32CurrHsf = stViAttr->u16FrameRate * stViAttr->u16WndHeight;

    return 0;
}

static int GetGain(void *pvObj)  // gain U10.6
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);

    return pstCtrlPara->u32CurrSnsGain;
}

static int SetGain(void *pvObj, FH_UINT32 gain, FH_UINT8 fNo) // gain U10.6
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT8  *sun_spot_status  = &((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_status;
    FH_BOOL   *sun_spot_flag    = &((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_flag;

    FH_UINT32 real_gain = gain >> 2; /* U.6-->U.4 */
    FH_UINT32 integer = real_gain >> 4;
    FH_UINT32 fine;
    FH_UINT8 i = 0;


    if (fNo != 0)
        return 0;

    while (integer >>= 1)
    {
        i++;
    }
    fine = (real_gain >> i) - 0x10;

    Sensor_Write(pstCci, 0x00, ((i & 0x7) << 4) | fine);

    /* Applied gain */
    pstCtrlPara->u32CurrSnsGain = (((fine + 0x10) << (i & 0x7)) << 2);

    if (gain <= 0x200)
    {
        if (*sun_spot_status != 0)
        {
            *sun_spot_flag = 1;
            *sun_spot_status = 0;
        }
    }
    else
    {
        if (*sun_spot_status != 1)
        {
            *sun_spot_flag = 1;
            *sun_spot_status = 1;
        }
    }

    return 0;
}


static int GetIntt(void *pvObj)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    return pstCtrlPara->u32CurrSnsIntt; //<<= 4;  // U.4
}

static int SetIntt(void *pvObj, FH_UINT32 intt, FH_UINT8 fNo)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    if (fNo != 0)
        return 0;

    intt = MAX(1, intt);
    if (intt > (k351p_dvp_intt_step_max + pstCtrlPara->u32CurrSnsIntt))
    {
        intt = pstCtrlPara->u32CurrSnsIntt + k351p_dvp_intt_step_max;
    }
    Sensor_Write(pstCci, 0x02, (intt >> 8) & 0xff);
    Sensor_Write(pstCci, 0x01, intt & 0xff);
    pstCtrlPara->u32CurrSnsIntt = intt;

    return 0;
}

static int GetAEDefault(void *pvObj, Sensor_AE_Default_S *sensAEDefault)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    sensAEDefault->minIntt = 1;
    sensAEDefault->maxIntt = pstCtrlPara->u32MaxIntt;
    sensAEDefault->minAGain = pstCtrlPara->u32MinAgain;
    sensAEDefault->maxAGain = 0xffff;
    sensAEDefault->fullLineStd = pstCtrlPara->u32FullLineStd;
    sensAEDefault->MinRstLine = pstCtrlPara->u32MinRstLine;
    return 0;
}

static int SetSnsFrameH(void *pvObj, FH_UINT32 frameH)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    Sensor_Write(pstCci, 0x22, frameH & 0xff);
    Sensor_Write(pstCci, 0x23, (frameH >> 8) & 0xff);
    pstCtrlPara->u32Vcycle = frameH;
    return 0;
}

static int Sensor_Init(void *pvObj, Sensor_Init_t* stInitConf)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    pstCtrlPara->u8CciDeviceId  = stInitConf->u8CciDeviceId;
    pstCtrlPara->u8CsiDeviceId  = stInitConf->u8CsiDeviceId;

// #ifndef FPGA_ENV
    set_clk_rate(SNS_CLK_NAME(pstCtrlPara->u8CciDeviceId ), CLK_24M);
// #endif
    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId );
    SensorReset(pvObj);
    pstCtrlPara->bInited = 1;
    return 0;
}

static int Sensor_Kick(void* pvObj)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT16 sensor_data = 0x00;

    sensor_data = Sensor_Read(pstCci, 0x12);
    sensor_data &= ~0x40;
    Sensor_Write(pstCci, 0x12, sensor_data);

    return 0;
}

static int Sensor_DeInit(void *pvObj)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT16 sensor_data = 0x00;

    sensor_data = Sensor_Read(pstCci, 0x12);
    sensor_data |= 0x40;
    Sensor_Write(pstCci, 0x12, sensor_data);

    SensorReset(pvObj);

    return SensorDevice_Close(pstCci);
}

static int SetSensorFmt(void *pvObj, int format)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT8  *sun_spot_status  = &((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_status;
    FH_BOOL   *sun_spot_flag    = &((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_flag;
    int i;

    SensorDevice_Close(pstCci);
    pstCtrlPara->u32SnsFmt = format;
    pstCtrlPara->u32WdrFlag = (format >> 16) & 1;

    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId );
    SensorReset(pvObj);

    *sun_spot_status = 0;
    *sun_spot_flag = 1;

    if (pstCtrlPara->bInited)
    {
        switch(format)
        {
        case FORMAT_500P2:
            fh_printf("SetSensorFmt FORMAT_500P2\r\n");
            for (i = 0; i < sizeof(Sensor_Cfg_K351P_DVP_500P2) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_K351P_DVP_500P2[2 * i], Sensor_Cfg_K351P_DVP_500P2[2 * i + 1]);
            break;
        case FORMAT_500P5:
            fh_printf("SetSensorFmt FORMAT_500P5\r\n");
            for (i = 0; i < sizeof(Sensor_Cfg_K351P_DVP_500P5) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_K351P_DVP_500P5[2 * i], Sensor_Cfg_K351P_DVP_500P5[2 * i + 1]);
            break;
        case FORMAT_400WP15:
            fh_printf("SetSensorFmt FORMAT_400WP15\r\n");
            for (i = 0; i < sizeof(Sensor_Cfg_K351P_DVP_2000P15) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_K351P_DVP_2000P15[2 * i], Sensor_Cfg_K351P_DVP_2000P15[2 * i + 1]);
            break;
        case FORMAT_400WP15_SLAVE:
            fh_printf("SetSensorFmt FORMAT_400WP15_SLAVE\r\n");
            for (i = 0; i < sizeof(Sensor_Cfg_K351P_DVP_2000P15_Slave) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_K351P_DVP_2000P15_Slave[2 * i], Sensor_Cfg_K351P_DVP_2000P15_Slave[2 * i + 1]);
            break;
        case FORMAT_250X250P60:
            fh_printf("SetSensorFmt FORMAT_250X250P60\r\n");
            for (i = 0; i < sizeof(Sensor_Cfg_K351P_DVP_250X250P60) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_K351P_DVP_250X250P60[2 * i], Sensor_Cfg_K351P_DVP_250X250P60[2 * i + 1]);
            break;
        default:
            fh_printf("===========UNKNOW FORMAT=============\r\n");
            return -1;
        }
    }
    else
        return -1;

    pstCtrlPara->u32Vcycle = Sensor_Read(pstCci, 0x23) << 8;
    pstCtrlPara->u32Vcycle |= Sensor_Read(pstCci, 0x22);
    pstCtrlPara->u32FullLineStd = pstCtrlPara->u32Vcycle;

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
    SetGain(pvObj, pstCtrlPara->u32MinAgain, 0);
    pstCtrlPara->u32CurrSnsGain = pstCtrlPara->u32MinAgain;

    return 0;
}

static int SetSensorFlipMirror(void *pvObj, FH_UINT32 sensor_en_stat)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 mirror, flip;
    FH_UINT16 sensor_data = 0x00;
    mirror = (sensor_en_stat >> 1) & 0x1;
    flip = sensor_en_stat & 0x1;

    sensor_data = Sensor_Read(pstCci, 0x12);

    if (mirror == 1) {
        sensor_data |= 0x20;
    } else {
        sensor_data &= ~0x20;
    }

    if (flip == 1) {
        sensor_data |= 0x10;
    } else {
        sensor_data &= ~0x10;
    }

    Sensor_Write(pstCci, 0x12, sensor_data);


    return 0;
}
static int GetSensorFlipMirror(void *pvObj, FH_UINT32 *sensor_en_stat)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 tmp;
    FH_UINT32 mirror, flip;

    tmp = Sensor_Read(pstCci, 0x12);
    mirror = (tmp & 0x20)?1:0;
    flip = (tmp & 0x10)?1:0;
    *sensor_en_stat = mirror << 1 | flip;
    return 0;
}

static FH_UINT32 *GetMirrorFlipBayerFormat(void *pvObj)
{
    return MirrorFlipBayerFormat;
}

static int Sensor_Isconnect(void *pvObj, FH_UINT8 u8DeviceId)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    unsigned short pid1, pid2;
    pstCtrlPara->u8CciDeviceId = u8DeviceId;
    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId);

    pid1 = Sensor_Read(pstCci, 0x08);
    pid2 = Sensor_Read(pstCci, 0x56);
    SensorDevice_Close(pstCci);
    if (pid1 == 0xA && pid2 == 0xB)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*0:high active  1:low active
[0:3] hsync pol
[4:7] vsyncpol
[8~31] dvp bitmode
*/
static int GetSensorAttribute(void *pvObj, char * name, FH_UINT32 *value)
{
    if (strcmp(name, "VI_POL") == 0)
    {
        *value = 0 | (1 << 4) | (12 << 8);
        return 0;
    }

    return -1;
}

static int CalcSnsValidGain(void *pvObj, FH_UINT32 *u32Gain)
{
    *u32Gain = (*u32Gain >> 2) << 2;  // U.6->U.4->U.6
    return 0;
}

#define SENSOR_k351p_DVP "k351p_dvp"
#ifdef OS_RTT
struct isp_sensor_if k351p_dvp_sensor_if_default = {
#else
struct isp_sensor_if k351p_dvp_sensor_if_default = {
#endif
    .name = SENSOR_k351p_DVP,
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
    memcpy(s_if, &k351p_dvp_sensor_if_default, sizeof(struct isp_sensor_if));

    pstPara = (SNS_PARA_S *)malloc(sizeof(SNS_PARA_S));
    memcpy(&pstPara->stCtrlPara, &stSnsDefaultCtrlPara, sizeof(SNS_CTRL_PARA_S));

    struct privateParaT *privPara = (struct privateParaT *)malloc(sizeof(struct privateParaT));
    memcpy(privPara, &private_para_default, sizeof(struct privateParaT));
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

