/*
 * jxf23_dvp.c
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
#include "jxf23_dvp.h"
#include "sensor_std.h"

#define _NAME(n, s) n##_##s
#define NAME(n) _NAME(n, jxf23_dvp)
#define jxf23_dvp_intt_step_max 0xa0

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
#define REWRITE_SensorCommonIf

#include "sns_com.h"


static unsigned int MirrorFlipBayerFormat[4] =
{
    2, 0, 3, 1
};
static int sensorFmt;

struct privateParaT{
    FH_UINT8 sun_spot_status;
    FH_BOOL sun_spot_flag;
};

static struct privateParaT private_para_default = {0,1};

static int Sensor_get_sys_info(Sensor_Param_S *sensorParam){
	sensorParam->regAddrCnt = 1;
	sensorParam->regValueCnt = 1;
	sensorParam->u8I2cIndex = 0;
	sensorParam->u8SlaveAddr = DEVICE_ID << 1;
	sensorParam->streamOn.cnt = 3;
	if(sensorFmt == FORMAT_480P7){
		sensorParam->streamOn.regArray[0].addr = 0x12;
		sensorParam->streamOn.regArray[0].reg = 0x07;
	}else{
		sensorParam->streamOn.regArray[0].addr = 0x12;
		sensorParam->streamOn.regArray[0].reg = 0x00;
	}
	sensorParam->streamOn.regArray[1].addr = 0x48;
	sensorParam->streamOn.regArray[1].reg = 0x85;
	sensorParam->streamOn.regArray[2].addr = 0x48;
	sensorParam->streamOn.regArray[2].reg = 0x05;

	sensorParam->streamOff.cnt = 1;
	if(sensorFmt == FORMAT_480P7){
		sensorParam->streamOff.regArray[0].addr = 0x12;
		sensorParam->streamOff.regArray[0].reg = 0x47;
	}else{
		sensorParam->streamOff.regArray[0].addr = 0x12;
		sensorParam->streamOff.regArray[0].reg = 0x40;
	}
	return 0;
}

static int SensorCommonIf(void *pvObj, FH_UINT32 cmd, ISP_SENSOR_COMMON_CMD_DATA0* data0, ISP_SENSOR_COMMON_CMD_DATA1* data1)
{
	int ret = -1;
	switch(cmd){
		case CMD_GET_I2C_INFO:
			{
				Sensor_Param_S *p = (Sensor_Param_S *)data0;
				ret = Sensor_get_sys_info(p);
			}
			break;
		default:
			break;
	}
	return ret;
}

static int GetSensorViAttr(void *pvObj, ISP_VI_ATTR_S *stViAttr)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    if (stViAttr == NULL)
        return FH_RET_SENSOR_NULL_POINTER;

    switch (pstCtrlPara->u32SnsFmt)
    {
        case FORMAT_480P7:
            stViAttr->u16WndWidth = FRAM_W_480P7;
            stViAttr->u16WndHeight = FRAM_H_480P7;
            stViAttr->u16InputWidth = ACTIVE_W_480P7;
            stViAttr->u16InputHeight = ACTIVE_H_480P7;
            stViAttr->u16PicWidth = PIC_IN_W_480P7;
            stViAttr->u16PicHeight = PIC_IN_H_480P7;
            stViAttr->u16OffsetX = OFFSET_X_480P7;
            stViAttr->u16OffsetY = OFFSET_Y_480P7;
            stViAttr->u16FrameRate = 7,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_720P2:
            stViAttr->u16WndWidth = FRAM_W_720P2;
            stViAttr->u16WndHeight = FRAM_H_720P2;
            stViAttr->u16InputWidth = ACTIVE_W_720P2;
            stViAttr->u16InputHeight = ACTIVE_H_720P2;
            stViAttr->u16PicWidth = PIC_IN_W_720P2;
            stViAttr->u16PicHeight = PIC_IN_H_720P2;
            stViAttr->u16OffsetX = OFFSET_X_720P2;
            stViAttr->u16OffsetY = OFFSET_Y_720P2;
            stViAttr->u16FrameRate = 2,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_1080P15:
#ifdef FPGA_ENV
            stViAttr->u16WndWidth = FRAM_W_1080P7;
            stViAttr->u16WndHeight = FRAM_H_1080P7;
            stViAttr->u16InputWidth = ACTIVE_W_1080P7;
            stViAttr->u16InputHeight = ACTIVE_H_1080P7;
            stViAttr->u16PicWidth = PIC_IN_W_1080P7;
            stViAttr->u16PicHeight = PIC_IN_H_1080P7;
            stViAttr->u16OffsetX = OFFSET_X_1080P7;
            stViAttr->u16OffsetY = OFFSET_Y_1080P7;
            stViAttr->u16FrameRate = 7,
#else
            stViAttr->u16WndWidth = FRAM_W_1080P15;
            stViAttr->u16WndHeight = FRAM_H_1080P15;
            stViAttr->u16InputWidth = ACTIVE_W_1080P15;
            stViAttr->u16InputHeight = ACTIVE_H_1080P15;
            stViAttr->u16PicWidth = PIC_IN_W_1080P15;
            stViAttr->u16PicHeight = PIC_IN_H_1080P15;
            stViAttr->u16OffsetX = OFFSET_X_1080P15;
            stViAttr->u16OffsetY = OFFSET_Y_1080P15;
            stViAttr->u16FrameRate = 15,
#endif
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        case FORMAT_1080P25:
            stViAttr->u16WndWidth = FRAM_W_1080P25;
            stViAttr->u16WndHeight = FRAM_H_1080P25;
            stViAttr->u16InputWidth = ACTIVE_W_1080P25;
            stViAttr->u16InputHeight = ACTIVE_H_1080P25;
            stViAttr->u16PicWidth = PIC_IN_W_1080P25;
            stViAttr->u16PicHeight = PIC_IN_H_1080P25;
            stViAttr->u16OffsetX = OFFSET_X_1080P25;
            stViAttr->u16OffsetY = OFFSET_Y_1080P25;
            stViAttr->u16FrameRate = 25,
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
		case FORMAT_720P25:
			stViAttr->u16WndWidth = FRAM_W_720P25;
			stViAttr->u16WndHeight = FRAM_W_720P25;
			stViAttr->u16InputWidth = ACTIVE_W_720P25;
			stViAttr->u16InputHeight = ACTIVE_H_720P25;
			stViAttr->u16PicWidth = PIC_IN_W_720P25;
			stViAttr->u16PicHeight = PIC_IN_H_720P25;
			stViAttr->u16OffsetX = OFFSET_X_720P25;
			stViAttr->u16OffsetY = OFFSET_Y_720P25;
			stViAttr->u16FrameRate = 25,
			stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
			break;
		case FORMAT_640X480P30:
			stViAttr->u16WndWidth = FRAM_W_480P30;
			stViAttr->u16WndHeight = FRAM_H_480P30;
			stViAttr->u16InputWidth = ACTIVE_W_480P30;
			stViAttr->u16InputHeight = ACTIVE_H_480P30;
			stViAttr->u16PicWidth = PIC_IN_W_480P30;
			stViAttr->u16PicHeight = PIC_IN_H_480P30;
			stViAttr->u16OffsetX = OFFSET_X_480P30;
			stViAttr->u16OffsetY = OFFSET_Y_480P30;
			stViAttr->u16FrameRate = 30,
            stViAttr->enBayerType = BAYER_BGGR;  // BAYER_GBRG;
            break;
        default:
            return FH_RET_SENSOR_NO_SUPPORT_FORMAT;
    }

    pstCtrlPara->u32CurrHsf = stViAttr->u16FrameRate * stViAttr->u16WndHeight;

    return 0;
}

static void SetBlcKSun(void *pvObj)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT8  *sun_spot_status  = &((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_status;
    FH_BOOL   *sun_spot_flag    = &((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_flag;

    FH_UINT32 tmp = Sensor_Read(pstCci, 0x1f);
    if (*sun_spot_status == 0)
    {
        Sensor_Write(pstCci, 0xc0, 0x66);
        Sensor_Write(pstCci, 0xc1, 0x04);
        Sensor_Write(pstCci, 0xc2, 0x0c);
        Sensor_Write(pstCci, 0xc3, 0x00);
        Sensor_Write(pstCci, 0x1f, 0x80 | tmp);
    }
    else if (*sun_spot_status == 1)
    {
        Sensor_Write(pstCci, 0xc0, 0x0c);
        Sensor_Write(pstCci, 0xc1, 0x40);
        Sensor_Write(pstCci, 0xc2, 0x66);
        Sensor_Write(pstCci, 0xc3, 0x44);
        Sensor_Write(pstCci, 0x1f, 0x80 | tmp);
    }

    *sun_spot_flag = 0;
}

static int GetGain(void *pvObj)  // gain U10.6
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    FH_BOOL   sun_spot_flag    = ((struct privateParaT *)pstCtrlPara->pvPrivatePara)->sun_spot_flag;

    if (sun_spot_flag == 1)
    {
        SetBlcKSun(pvObj);
    }
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

    // intt = intt>>4;  // U.4->U.0
    if (intt > (jxf23_dvp_intt_step_max + pstCtrlPara->u32CurrSnsIntt))
    {
        intt = pstCtrlPara->u32CurrSnsIntt + jxf23_dvp_intt_step_max;
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
    set_clk_rate(SNS_CLK_NAME(pstCtrlPara->u8CciDeviceId ), CLK_27M);
// #endif

    SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId );
    SensorReset(pvObj);
    pstCtrlPara->bInited = 1;
    return 0;
}

static int Sensor_Kick(void* pvObj)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    if(pstCtrlPara->u32SnsFmt == FORMAT_480P7)
        Sensor_Write(pstCci, 0x12, 0x07);
    else
        Sensor_Write(pstCci, 0x12, 0x00);
    Sensor_Write(pstCci, 0x48, 0x85);
    Sensor_Write(pstCci, 0x48, 0x05);
    return 0;
}

static int Sensor_DeInit(void *pvObj)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    if(pstCtrlPara->u32SnsFmt == FORMAT_480P7)
        Sensor_Write(pstCci, 0x12, 0x07);
    else
        Sensor_Write(pstCci, 0x12, 0x00);
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
    sensorFmt = format;
    SensorDevice_Init(pstCci, (0x80 >> 1), 0, pstCtrlPara->u8CciDeviceId );
    SensorReset(pvObj);

    *sun_spot_status = 0;
    *sun_spot_flag = 1;

    if (pstCtrlPara->bInited)
    {
        switch(format)
        {
        case FORMAT_480P7:
            for (i = 0; i < sizeof(Sensor_Cfg_JXF23_DVP_480P7) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_JXF23_DVP_480P7[2 * i], Sensor_Cfg_JXF23_DVP_480P7[2 * i + 1]);
            break;
        case FORMAT_720P2:
            for (i = 0; i < sizeof(Sensor_Cfg_JXF23_DVP_720P2) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_JXF23_DVP_720P2[2 * i], Sensor_Cfg_JXF23_DVP_720P2[2 * i + 1]);
            break;
        case FORMAT_1080P15:
#ifdef FPGA_ENV
            for (i = 0; i < sizeof(Sensor_Cfg_JXF23_DVP_1080P7) / 4; i++)
            {
                Sensor_Write(pstCci, Sensor_Cfg_JXF23_DVP_1080P7[2 * i], Sensor_Cfg_JXF23_DVP_1080P7[2 * i + 1]);
            }
#else
            for (i = 0; i < sizeof(Sensor_Cfg_JXF23_DVP_1080P15) / 4; i++)
            {
                Sensor_Write(pstCci, Sensor_Cfg_JXF23_DVP_1080P15[2 * i], Sensor_Cfg_JXF23_DVP_1080P15[2 * i + 1]);
            }
#endif
            break;
        case FORMAT_1080P25:
			fh_printf("===========FORMAT_1080P25=============\r\n");
            for (i = 0; i < sizeof(Sensor_Cfg_1080P25FPS) / 4; i++)
                Sensor_Write(pstCci, Sensor_Cfg_1080P25FPS[2 * i], Sensor_Cfg_1080P25FPS[2 * i + 1]);
            break;
        case FORMAT_1080P30:
			fh_printf("===========FORMAT_1080P30=============\r\n");
                for (i = 0; i < sizeof(Sensor_Cfg_1080P30FPS) / 4; i++)
                    Sensor_Write(pstCci, Sensor_Cfg_1080P30FPS[2 * i], Sensor_Cfg_1080P30FPS[2 * i + 1]);
            break;
		case FORMAT_720P25:
			fh_printf("===========FORMAT_720P25=============\r\n");
			for (i = 0; i < sizeof(Sensor_Cfg_720P25FPS) / 4; i++)
				Sensor_Write(pstCci, Sensor_Cfg_720P25FPS[2 * i], Sensor_Cfg_720P25FPS[2 * i + 1]);
			break;
		case FORMAT_640X480P30:
			fh_printf("===========FORMAT_640X480P30=============\r\n");
			for (i = 0; i < sizeof(Sensor_Cfg_480P30FPS) / 4; i++)
				Sensor_Write(pstCci, Sensor_Cfg_480P30FPS[2 * i], Sensor_Cfg_480P30FPS[2 * i + 1]);
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
    //SetSensorIntt(pvObj, 0x04, 0);
    //SetSensorGain(pvObj, 0x40, 0);
    return 0;
}

static int SetSensorFlipMirror(void *pvObj, FH_UINT32 sensor_en_stat)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 mirror, flip;
    mirror = (sensor_en_stat >> 1) & 0x1;
    flip = sensor_en_stat & 0x1;
    if ((mirror == 1) && (flip == 1))
    {
        Sensor_Write(pstCci, 0xC0, 0x12);
        Sensor_Write(pstCci, 0xC1, 0x30);
        Sensor_Write(pstCci, 0xC2, 0x48);
        Sensor_Write(pstCci, 0xC3, 0x85);
        // Sensor_Write(pstCci, 0xC4, 0x74);
        // Sensor_Write(pstCci, 0xC5, 0x12);
        Sensor_Write(pstCci, 0xC4, 0x28);
        Sensor_Write(pstCci, 0xC5, 0x26);
        Sensor_Write(pstCci, 0xC6, 0x48);
        Sensor_Write(pstCci, 0xC7, 0x05);
    }
    else if (mirror)
    {
        Sensor_Write(pstCci, 0xC0, 0x12);
        Sensor_Write(pstCci, 0xC1, 0x20);
        Sensor_Write(pstCci, 0xC2, 0x48);
        Sensor_Write(pstCci, 0xC3, 0x85);
        // Sensor_Write(pstCci, 0xC4, 0x74);
        // Sensor_Write(pstCci, 0xC5, 0x12);
        Sensor_Write(pstCci, 0xC4, 0x28);
        Sensor_Write(pstCci, 0xC5, 0x1D);
        Sensor_Write(pstCci, 0xC6, 0x48);
        Sensor_Write(pstCci, 0xC7, 0x05);
    }
    else if (flip)
    {
        Sensor_Write(pstCci, 0xC0, 0x12);
        Sensor_Write(pstCci, 0xC1, 0x10);
        Sensor_Write(pstCci, 0xC2, 0x48);
        Sensor_Write(pstCci, 0xC3, 0x85);
        // Sensor_Write(pstCci, 0xC4, 0x74);
        // Sensor_Write(pstCci, 0xC5, 0x02);
        Sensor_Write(pstCci, 0xC4, 0x28);
        Sensor_Write(pstCci, 0xC5, 0x26);
        Sensor_Write(pstCci, 0xC6, 0x48);
        Sensor_Write(pstCci, 0xC7, 0x05);
    }
    else
    {
        Sensor_Write(pstCci, 0xC0, 0x12);
        Sensor_Write(pstCci, 0xC1, 0x00);
        Sensor_Write(pstCci, 0xC2, 0x48);
        Sensor_Write(pstCci, 0xC3, 0x85);
        // Sensor_Write(pstCci, 0xC4, 0x74);
        // Sensor_Write(pstCci, 0xC5, 0x02);
        Sensor_Write(pstCci, 0xC4, 0x28);
        Sensor_Write(pstCci, 0xC5, 0x1D);
        Sensor_Write(pstCci, 0xC6, 0x48);
        Sensor_Write(pstCci, 0xC7, 0x05);
    }
    Sensor_Write(pstCci, 0x1F, 0x80);

    return 0;
}
static int GetSensorFlipMirror(void *pvObj, FH_UINT32 *sensor_en_stat)
{
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);
    FH_UINT32 tmp;

    tmp = Sensor_Read(pstCci, 0x12);
    *sensor_en_stat = (tmp >> 4) & 0x3;
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
    SensorDevice_Init(pstCci, (0x80 >> 1), 0, pstCtrlPara->u8CciDeviceId);

    pid1 = Sensor_Read(pstCci, 0xa);
    pid2 = Sensor_Read(pstCci, 0xb);
    SensorDevice_Close(pstCci);
    if (pid1 == 0xf && pid2 == 0x23)
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

#define SENSOR_JXF23_DVP "jxf23_dvp"
#ifdef OS_RTT
struct isp_sensor_if jxf23_dvp_sensor_if_default= {
#else
struct isp_sensor_if jxf23_dvp_sensor_if_default = {
#endif
    .name = SENSOR_JXF23_DVP,
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
    memcpy(s_if, &jxf23_dvp_sensor_if_default, sizeof(struct isp_sensor_if));

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
