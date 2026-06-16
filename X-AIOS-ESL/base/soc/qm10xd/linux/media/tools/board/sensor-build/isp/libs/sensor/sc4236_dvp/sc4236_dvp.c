/*
 * sc4236_dvp.c
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
#include "sc4236_dvp.h"
#include "sensor_std.h"

#define _NAME(n, s) n##_##s
#define NAME(n) _NAME(n, sc4236_dvp)
#define sc4236_dvp_intt_step_max 0xa0

#define CCI_ID (0x60 >> 1)
#define CCI_MODE 2

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
#define REWRITE_SensorCommonIf
#include "sns_com.h"
/*
typedef enum __VICAP_BAYER_TYPE__ {
    VICAP_BAYER_RGGB = 0x0, // 色彩模式RGGB
    VICAP_BAYER_GRBG = 0x1, // 色彩模式GRBG
    VICAP_BAYER_BGGR = 0x2, // 色彩模式BGGR
    VICAP_BAYER_GBRG = 0x3, // 色彩模式GBRG
    VICAP_BAYER_TYPE_DUMMY=0xffffffff,
} FH_VICAP_BAYER_TYPE_E;
*/

#define SC4236_AGAIN_ITEM_NUM (4)

typedef struct{
    FH_UINT32 maxGain; //U 6
    struct{
        FH_UINT32 coarseGainReg;
        FH_UINT32 fineGainRegStart;
        FH_UINT32 gainMin;
        FH_UINT32 gainMax;
        FH_UINT32 gainStep;
        FH_UINT32 regStep;
    }gainItem[SC4236_AGAIN_ITEM_NUM];
}SC4236_Again_S;

SC4236_Again_S sc4236_again_info = {
    .maxGain = 0x3E0,//15.5x 0x40;
    .gainItem = {
        {0, 0x10, 0x40, 0x7c, 4, 1},
        {1, 0x10, 0x80, 0xf8, 8, 1},
        {3, 0x10, 0x100, 0x1f0, 16, 1},
        {7, 0x10, 0x200, 0x3e0, 32, 1},
    }
};


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
        case FORMAT_480P2:
            stViAttr->u16WndWidth = FRAM_W_480P2;
            stViAttr->u16WndHeight = FRAM_H_480P2;
            stViAttr->u16InputWidth = ACTIVE_W_480P2;
            stViAttr->u16InputHeight = ACTIVE_H_480P2;
            stViAttr->u16PicWidth = PIC_IN_W_480P2;
            stViAttr->u16PicHeight = PIC_IN_H_480P2;
            stViAttr->u16OffsetX = OFFSET_X_480P2;
            stViAttr->u16OffsetY = OFFSET_Y_480P2;
            stViAttr->u16FrameRate = 2,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_480P5:
            stViAttr->u16WndWidth = FRAM_W_480P5;
            stViAttr->u16WndHeight = FRAM_H_480P5;
            stViAttr->u16InputWidth = ACTIVE_W_480P5;
            stViAttr->u16InputHeight = ACTIVE_H_480P5;
            stViAttr->u16PicWidth = PIC_IN_W_480P5;
            stViAttr->u16PicHeight = PIC_IN_H_480P5;
            stViAttr->u16OffsetX = OFFSET_X_480P5;
            stViAttr->u16OffsetY = OFFSET_Y_480P5;
            stViAttr->u16FrameRate = 5,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_480P10:
            stViAttr->u16WndWidth = FRAM_W_480P10;
            stViAttr->u16WndHeight = FRAM_H_480P10;
            stViAttr->u16InputWidth = ACTIVE_W_480P10;
            stViAttr->u16InputHeight = ACTIVE_H_480P10;
            stViAttr->u16PicWidth = PIC_IN_W_480P10;
            stViAttr->u16PicHeight = PIC_IN_H_480P10;
            stViAttr->u16OffsetX = OFFSET_X_480P10;
            stViAttr->u16OffsetY = OFFSET_Y_480P10;
            stViAttr->u16FrameRate = 10,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_1080P30:
            stViAttr->u16WndWidth = FRAM_W_1080P30;
            stViAttr->u16WndHeight = FRAM_H_1080P30;
            stViAttr->u16InputWidth = ACTIVE_W_1080P30;
            stViAttr->u16InputHeight = ACTIVE_H_1080P30;
            stViAttr->u16PicWidth = PIC_IN_W_1080P30;
            stViAttr->u16PicHeight = PIC_IN_H_1080P30;
            stViAttr->u16OffsetX = OFFSET_X_1080P30;
            stViAttr->u16OffsetY = OFFSET_Y_1080P30;
            stViAttr->u16FrameRate = 25,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_1536P15:
            stViAttr->u16WndWidth = FRAM_W_1536P15;
            stViAttr->u16WndHeight = FRAM_H_1536P15;
            stViAttr->u16InputWidth = ACTIVE_W_1536P15;
            stViAttr->u16InputHeight = ACTIVE_H_1536P15;
            stViAttr->u16PicWidth = PIC_IN_W_1536P15;
            stViAttr->u16PicHeight = PIC_IN_H_1536P15;
            stViAttr->u16OffsetX = OFFSET_X_1536P15;
            stViAttr->u16OffsetY = OFFSET_Y_1536P15;
            stViAttr->u16FrameRate = 15,
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

static int SetGain(void *pvObj, FH_UINT32 again, FH_UINT8 fNo) // gain U10.6
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 fineGainReg = 0;
    FH_UINT32 i = 0;
    FH_UINT32 coarseGainReg = 0;
    FH_UINT32 fineGainRegStart = 0;
    FH_UINT32 gainMin = 0;
    FH_UINT32 gainStep = 0;
    FH_UINT32 regStep = 0;
    FH_UINT32 found = 0;

    if(fNo != 0) return 0;
    if(again < 0x40) {
        fh_printf("error again:0x%x", again);
        return -1;
    }

    for(i = 0; i < SC4236_AGAIN_ITEM_NUM - 1; i++){
        if((again >= sc4236_again_info.gainItem[i].gainMin) && (again < sc4236_again_info.gainItem[i + 1].gainMin)){
            coarseGainReg = sc4236_again_info.gainItem[i].coarseGainReg;
            fineGainRegStart = sc4236_again_info.gainItem[i].fineGainRegStart;
            gainMin = sc4236_again_info.gainItem[i].gainMin;
            gainStep = sc4236_again_info.gainItem[i].gainStep;
            regStep = sc4236_again_info.gainItem[i].regStep;
            found = 1;
            break;
        }
    }

    if(found == 0){
        i = SC4236_AGAIN_ITEM_NUM - 1;
        coarseGainReg = sc4236_again_info.gainItem[i].coarseGainReg;
        fineGainRegStart = sc4236_again_info.gainItem[i].fineGainRegStart;
        gainMin = sc4236_again_info.gainItem[i].gainMin;
        gainStep = sc4236_again_info.gainItem[i].gainStep;
        regStep = sc4236_again_info.gainItem[i].regStep;
    }

    if(again > sc4236_again_info.maxGain){
        coarseGainReg = 0x7 << 2;
        fineGainReg = 0x1f;
    }else{
        coarseGainReg = (coarseGainReg & 0x7) << 2;
        fineGainReg = fineGainRegStart + (again - gainMin) / gainStep * regStep;
    }
    //fh_printf("again:0x%x, coarseGainReg:0x%x, fineGainReg:0x%x\n", again, coarseGainReg, fineGainReg);
    Sensor_Write(pstCci, 0x3e08,coarseGainReg);
    Sensor_Write(pstCci, 0x3e09,fineGainReg);

    coarseGainReg = coarseGainReg >> 2;
    if(coarseGainReg == 0){//
        pstCtrlPara->u32CurrSnsGain = sc4236_again_info.gainItem[0].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[0].gainStep;
        Sensor_Write(pstCci,0x3301,0x0c);
        Sensor_Write(pstCci,0x3633,0x23);
        Sensor_Write(pstCci,0x3630,0x80);
        Sensor_Write(pstCci,0x3622,0xf6);
    }else if(coarseGainReg == 1){//
        pstCtrlPara->u32CurrSnsGain = sc4236_again_info.gainItem[1].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[1].gainStep;
        Sensor_Write(pstCci,0x3301,0x18);
        Sensor_Write(pstCci,0x3633,0x23);
        Sensor_Write(pstCci,0x3630,0x80);
        Sensor_Write(pstCci,0x3622,0xf6);
    }else if(coarseGainReg == 3){//
        pstCtrlPara->u32CurrSnsGain = sc4236_again_info.gainItem[2].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[2].gainStep;
        Sensor_Write(pstCci,0x3301,0x40);
        Sensor_Write(pstCci,0x3633,0x23);
        Sensor_Write(pstCci,0x3630,0x80);
        Sensor_Write(pstCci,0x3622,0xf6);
    }else if(coarseGainReg == 7){//
        pstCtrlPara->u32CurrSnsGain = sc4236_again_info.gainItem[3].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[3].gainStep;
        Sensor_Write(pstCci,0x3301,0x40);
        Sensor_Write(pstCci,0x3633,0x23);
        Sensor_Write(pstCci,0x3630,0x80);
        Sensor_Write(pstCci,0x3622,0xf6);
    }
    fh_printf("sc4236 pstCtrlPara->u32CurrSnsGain:0x%x\n", pstCtrlPara->u32CurrSnsGain);
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
    if (intt > (sc4236_dvp_intt_step_max + pstCtrlPara->u32CurrSnsIntt))
    {
        intt = pstCtrlPara->u32CurrSnsIntt + sc4236_dvp_intt_step_max;
    }
    if(intt > pstCtrlPara->u32Vcycle - 4) intt = pstCtrlPara->u32Vcycle - 4;
    intt = intt << 1;  // U.4->U.0

    Sensor_Write(pstCci, 0x3e01, (intt >> 4) & 0xff);
    Sensor_Write(pstCci, 0x3e02, (intt << 4) & 0xf0);
    pstCtrlPara->u32CurrSnsIntt = intt >> 1;

    return 0;
}

static int GetAEDefault(void *pvObj, Sensor_AE_Default_S *sensAEDefault)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    sensAEDefault->minIntt = 1;
    sensAEDefault->maxIntt = pstCtrlPara->u32MaxIntt;
    sensAEDefault->minAGain = pstCtrlPara->u32MinAgain;
    sensAEDefault->maxAGain = 992;
    sensAEDefault->fullLineStd = pstCtrlPara->u32FullLineStd;
    sensAEDefault->MinRstLine = pstCtrlPara->u32MinRstLine;
    return 0;
}

static int SetSnsFrameH(void *pvObj, FH_UINT32 frameH)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    Sensor_Write(pstCci, 0x320e, frameH >> 8);
    Sensor_Write(pstCci, 0x320f, frameH & 0xff);
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
    Sensor_Write(pstCci, 0x0100, 0x01);
    return 0;
}

static int SensorCommonIf(void* pvObj, FH_UINT32 cmd, ISP_SENSOR_COMMON_CMD_DATA0* data0, ISP_SENSOR_COMMON_CMD_DATA1* data1)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    int ret = -1;
    switch (cmd)
    {
        case CMD_MAP_INTT_GAIN:
            {
                MAP_INTT_GAIN_CFG *map_intt_gain;
                FH_UINT32 intt_new, curr_frame_height, des_frame_height;
                FH_UINT32 curr_format_param = 0;
                FH_UINT32 des_format_param = 0;

                map_intt_gain = (MAP_INTT_GAIN_CFG*)data0;
                if(map_intt_gain == NULL)
                    return ret;
                switch (map_intt_gain->currFormat)
                {
                    case FORMAT_1080P30:
                        curr_format_param = 30;
                        curr_frame_height = FRAM_H_1080P30;
                        break;
                    default:
                        ret = -1;
                }

                switch (map_intt_gain->desFormat)
                {
                    case FORMAT_1536P15:
                        des_format_param = 15;
                        des_frame_height = FRAM_H_1536P15;
                        break;
                    default:
                        ret = -1;
                }
                intt_new = (des_format_param * des_frame_height * (FH_UINT64)(map_intt_gain->currIntt) * (FH_UINT64)(map_intt_gain->currTotalGain))/(curr_format_param * curr_frame_height);
                if(intt_new < (16 * 64))
                {
                    map_intt_gain->desIntt = MAX(1, intt_new >> 6);
                    map_intt_gain->desTotalGain = MAX(pstCtrlPara->u32MinAgain, intt_new / (map_intt_gain->desIntt));
                }
                else
                {
                    intt_new = intt_new >> 6;
                    map_intt_gain->desIntt = MIN(pstCtrlPara->u32MaxIntt, intt_new);
                    map_intt_gain->desTotalGain = MAX(pstCtrlPara->u32MinAgain, (intt_new<<6)/ map_intt_gain->desIntt);
                }
                ret = 0;
                break;
            }

        default:
            ret = -1;
            break;
    }
    return ret;
}

static int Sensor_DeInit(void *pvObj)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    Sensor_Write(pstCci, 0x0100, 0x00);
    SensorReset(pvObj);

    return SensorDevice_Close(pstCci);
}

static int SetSensorFmt(void *pvObj, int format)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    int i;

    SensorDevice_Close(pstCci);
    pstCtrlPara->u32SnsFmt = format;
    pstCtrlPara->u32WdrFlag = (format >> 16) & 1;

    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId );
    SensorReset(pvObj);
    if (pstCtrlPara->bInited)
    {
        switch(format)
        {
        case FORMAT_480P2:
            for (i = 0; i < sizeof(Sensor_Cfg_SC4236_DVP_480P2) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_SC4236_DVP_480P2[2 * i], Sensor_Cfg_SC4236_DVP_480P2[2 * i + 1]);
            break;
        case FORMAT_480P5:
            for (i = 0; i < sizeof(Sensor_Cfg_SC4236_DVP_480P5) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_SC4236_DVP_480P5[2 * i], Sensor_Cfg_SC4236_DVP_480P5[2 * i + 1]);
            break;
        case FORMAT_480P10:
            for (i = 0; i < sizeof(Sensor_Cfg_SC4236_DVP_480P10) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_SC4236_DVP_480P10[2 * i], Sensor_Cfg_SC4236_DVP_480P10[2 * i + 1]);
            break;
        case FORMAT_1080P30:
                for (i = 0; i < sizeof(Sensor_Cfg_SC4236_DVP_1080P30FPS) / 4; i++)
                    Sensor_Write(pstCci, Sensor_Cfg_SC4236_DVP_1080P30FPS[2 * i], Sensor_Cfg_SC4236_DVP_1080P30FPS[2 * i + 1]);
            break;
        case FORMAT_1536P15:
                for (i = 0; i < sizeof(Sensor_Cfg_1536P15FPS) / 4; i++)
                    Sensor_Write(pstCci, Sensor_Cfg_1536P15FPS[2 * i], Sensor_Cfg_1536P15FPS[2 * i + 1]);
            break;
        default:
            fh_printf("===========UNKNOW FORMAT=============\r\n");
            return -1;
        }
    }
    else
        return -1;

    pstCtrlPara->u32Vcycle = Sensor_Read(pstCci, 0x320e) << 8;
    pstCtrlPara->u32Vcycle |= Sensor_Read(pstCci, 0x320f);
    pstCtrlPara->u32FullLineStd = pstCtrlPara->u32Vcycle;

    if (pstCtrlPara->u32WdrFlag == 0)
    {
        pstCtrlPara->u32MinRstLine = 5;
    }
    else
    {
        pstCtrlPara->u32MinRstLine = 4;
    }
    pstCtrlPara->u32MaxIntt = pstCtrlPara->u32Vcycle - pstCtrlPara->u32MinRstLine;
    if(pstCtrlPara->u32MaxIntt > (0xfff / 2)) pstCtrlPara->u32MaxIntt = 0xfff / 2;
    pstCtrlPara->u32MinAgain = 0x40;
    SetIntt(pvObj, pstCtrlPara->u32MaxIntt, 0);
    SetGain(pvObj, pstCtrlPara->u32MinAgain, 0);
    pstCtrlPara->u32CurrSnsGain = pstCtrlPara->u32MinAgain;
    //SetSensorIntt(pvObj, 0x04, 0);
    //SetSensorGain(pvObj, 0x40, 0);
    return 0;
}

static int SetSensorFlipMirror(void *pvObj, FH_UINT32 sensor_en_stat)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 mirror, flip;
    FH_UINT16 value = 0;
    mirror = (sensor_en_stat >> 1) & 0x1;
    flip = sensor_en_stat & 0x1;

    value = Sensor_Read(pstCci, 0x3221);
    if ((mirror == 1) && (flip == 1))
    {
        Sensor_Write(pstCci, 0x3221, value | 0x66);
    }
    else if (mirror)
    {
        Sensor_Write(pstCci, 0x3221, (value | 0x6) & ~(0x60));
    }
    else if (flip)
    {
        Sensor_Write(pstCci, 0x3221, (value | 0x60) & ~(0x6));
    }
    else
    {
        Sensor_Write(pstCci, 0x3221, value & ~(0x66));
    }

    return 0;
}
static int GetSensorFlipMirror(void *pvObj, FH_UINT32 *sensor_en_stat)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 tmp;
    FH_UINT32 mirror, flip;

    tmp = Sensor_Read(pstCci, 0x3221);
    mirror = (tmp & 0x06)?1:0;
    flip = (tmp & 0x60)?1:0;
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

    pid1 = Sensor_Read(pstCci, 0x3107);
    pid2 = Sensor_Read(pstCci, 0x3108);
    SensorDevice_Close(pstCci);
    if (pid1 == 0x35 && pid2 == 0x32)
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

#define SENSOR_SC4236_DVP "sc4236_dvp"
#ifdef OS_RTT
struct isp_sensor_if sc4236_dvp_sensor_if_default = {
#else
struct isp_sensor_if sc4236_dvp_sensor_if_default = {
#endif
    .name = SENSOR_SC4236_DVP,
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
    memcpy(s_if, &sc4236_dvp_sensor_if_default, sizeof(struct isp_sensor_if));

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


