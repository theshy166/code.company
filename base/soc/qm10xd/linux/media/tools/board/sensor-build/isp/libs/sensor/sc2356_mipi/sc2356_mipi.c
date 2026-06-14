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
#include "sc2356_mipi.h"
#include "sensor_std.h"

#define _NAME(n, s)		n##_##s
#define NAME(n)			_NAME(n, sc2356_mipi)
#define CCI_ID			(0x6C >> 1)
#define CCI_ID_1		(0x20 >> 1)
#define CCI_MODE		2

#define REWRITE_getSensorConfType
#define REWRITE_CalcSnsValidGain
#define REWRITE_Sensor_Isconnect
#define FUN_EN_Sensor_WriteEx
#if CHIP_XGM
#define REWRITE_SensorCommonIf
#define FUN_EN_CommonIfGetRegsInfo
#endif
#define REWRITE_Sensor_DeInit
#define REWRITE_Sensor_Kick
#define REWRITE_Sensor_Init

static int Sensor_Isconnect(void *pvObj, FH_UINT8 u8DeviceId)
{
	SNS_CTRL_PARA_S*	pstCtrlPara = _sns_get_ctrl_para(pvObj);
	SensorDevice*		pstCci		= _sns_get_cci_(pvObj);
	FH_UINT32 u32SnsChipId;

	pstCtrlPara->u8CciDeviceId = u8DeviceId;

	if(pstCtrlPara->u8CsiDeviceId == 0){
		SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId);
	}else{
		SensorDevice_Init(pstCci, CCI_ID_1, CCI_MODE, pstCtrlPara->u8CciDeviceId);
	}


	u32SnsChipId = Sensor_Read(pstCci, 0x3108);
	u32SnsChipId |= Sensor_Read(pstCci, 0x3107) << 8;

	SensorDevice_Close(pstCci);

	//printf("%s u32SnsChipId:0x%x\n", __func__, u32SnsChipId);

	if (u32SnsChipId == 0xcb3a)
		return 1;
	else
	{
		return 0;
	}
}

static int Sensor_Kick(void* pvObj)
{
	SensorDevice* pstCci = _sns_get_cci_(pvObj);
	Sensor_Write(pstCci, 0x0100, 0x01);
	return 0;
}


static SNS_CONFIG_S* getSensorConfType(void *pvObj)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);

	pstCtrlPara->u32CurrConfSize  = sizeof(stSnsConf) / sizeof(SNS_CONFIG_S);
	return stSnsConf;
}

#include "sns_com.h"

#if CHIP_XGM
static int SensorCommonIf(void* pvObj, FH_UINT32 cmd, ISP_SENSOR_COMMON_CMD_DATA0* data0, ISP_SENSOR_COMMON_CMD_DATA1* data1)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);
	int ret = -1;
	switch (cmd)
	{
		case CMD_GET_REGS_INFO:
			if(!pstCtrlPara->bGrpSync)
			{
				printf("not support CMD_GET_REGS_INFO \n");
				break;
			}
			ret = CommonIfGetRegsInfo(pvObj, data0, data1);
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}
#endif

static int Sensor_Init(void *pvObj, Sensor_Init_t* stInitConf)
{
    SNS_CTRL_PARA_S*    pstCtrlPara = _sns_get_ctrl_para(pvObj);
    SensorDevice*       pstCci      = _sns_get_cci_(pvObj);

    pstCtrlPara->u8CciDeviceId  = stInitConf->u8CciDeviceId;
    pstCtrlPara->u8CsiDeviceId  = stInitConf->u8CsiDeviceId;
#if CHIP_XGM
    pstCtrlPara->bGrpSync  = stInitConf->bGrpSync;
#endif

    if(pstCtrlPara->u8CsiDeviceId == 0){
        SensorDevice_Init(pstCci, CCI_ID, CCI_MODE, pstCtrlPara->u8CciDeviceId);
    }else{
        SensorDevice_Init(pstCci, CCI_ID_1, CCI_MODE, pstCtrlPara->u8CciDeviceId);
    }
    SensorReset(pvObj);

    pstCtrlPara->bInited = 1;

    return 0;
}

//可根据sns_cfg->max_lane_num确定当前使用2lane配置还是4lane配置
static int SetSensorFmt(void *pvObj, FH_SINT32 s32Format)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);
	SensorDevice* pstCci = _sns_get_cci_(pvObj);

	FH_SINT32 s32Ret = 0;

	if(pstCtrlPara->u8CsiDeviceId == 0){
		s32Ret = SnsCom_SetFmt(pvObj, s32Format, CCI_ID, CCI_MODE);
	}else{
		s32Ret = SnsCom_SetFmt(pvObj, s32Format, CCI_ID_1, CCI_MODE);
	}


	if (s32Ret)
		return s32Ret;

	pstCtrlPara->u32Vcycle = Sensor_Read(pstCci, 0x320e) << 8;
	pstCtrlPara->u32Vcycle |= Sensor_Read(pstCci, 0x320f);
	pstCtrlPara->u32FullLineStd = pstCtrlPara->u32Vcycle;

	pstCtrlPara->u32MinRstLine = 6;
	pstCtrlPara->u32MaxIntt = pstCtrlPara->u32Vcycle - pstCtrlPara->u32MinRstLine;
	SetIntt(pvObj, pstCtrlPara->u32MaxIntt, 0);
	pstCtrlPara->u32MinAgain = 0x40;
	SetGain(pvObj, pstCtrlPara->u32MinAgain, 0);
	pstCtrlPara->u32CurrSnsGain = pstCtrlPara->u32MinAgain;

	return 0;
}

static int GetAEDefault(void *pvObj, Sensor_AE_Default_S *stSnsAEDefault)
{
	SNS_CTRL_PARA_S*	pstCtrlPara = _sns_get_ctrl_para(pvObj);

	stSnsAEDefault->minIntt = 1;
	stSnsAEDefault->maxIntt = pstCtrlPara->u32MaxIntt;
	stSnsAEDefault->minAGain = pstCtrlPara->u32MinAgain;
	stSnsAEDefault->maxAGain = 0xffff;
	stSnsAEDefault->fullLineStd = pstCtrlPara->u32FullLineStd;
	stSnsAEDefault->MinRstLine = pstCtrlPara->u32MinRstLine;

	return 0;
}

// GetGain仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetGain(void *pvObj)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);

	return pstCtrlPara->u32CurrSnsGain;
}

static int CalcSnsValidGain(void *pvObj, FH_UINT32 *u32Gain)
{
	*u32Gain = (*u32Gain >> 2) << 2;  // U.6->U.4->U.6

	return 0;
}

static int SetGain(void *pvObj, FH_UINT32 again, FH_UINT8 fNo)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);
	//SensorDevice* pstCci  = _sns_get_cci_(pvObj);
	// gain U10.6
	FH_UINT8 Reg3e09;//analog
	FH_UINT8 Reg3e06;//digital
	FH_UINT8 Reg3e07;//digital fine
	FH_UINT8 again_rel = 0;
	FH_UINT8 dgain_rel = 0; //u5

	//printf(" set gain = %d\n", again);
	again = again >> 1; //U5
	if (fNo == 0) {
		if (again < 2 * 0x20){
			again_rel = 1;
			Reg3e09 = 0;
		} else if (again < 4 * 0x20){
			again_rel = 2;
			Reg3e09 = 8;
		} else if (again < 8 * 0x20){
			again_rel = 4;
			Reg3e09 = 9;
		} else if (again < 16 * 0x20){
			again_rel = 8;
			Reg3e09 = 0xb;
		} else if (again < 32 * 0x20){
			again_rel = 16;
			Reg3e09 = 0xf;
		} else {
			again_rel = 32;
			Reg3e09 = 0x1f;
		}

		Reg3e06 = 0x0;
		if ((again / again_rel) >= 0x3f){
			dgain_rel = 0x3f;
			Reg3e07 = 0xfc;
		} else {
			dgain_rel = again / again_rel;
			Reg3e07 = 0x80 + (dgain_rel - 0x20) * 4;
		}
		Sensor_WriteEx(pvObj, 0x3e06, Reg3e06, SNS_INT_POS_START); // group 0 hold start
		Sensor_WriteEx(pvObj, 0x3e07, Reg3e07, SNS_INT_POS_START);
		Sensor_WriteEx(pvObj, 0x3e09, Reg3e09, SNS_INT_POS_START);
		pstCtrlPara->u32CurrSnsGain = (again_rel * dgain_rel) << 1;//u6
		//printf("[Reg3e06 Reg3e07 Reg3e09 u32CurrSnsGain]:[0x%x 0x%x 0x%x 0x%x]\n", Reg3e06, Reg3e07, Reg3e09, pstCtrlPara->u32CurrSnsGain);
	}
	return 0;
}

// GetIntt仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetIntt(void *pvObj)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);

	return pstCtrlPara->u32CurrSnsGain;
}

static int SetIntt(void *pvObj, FH_UINT32 intt, FH_UINT8 fNo)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);
	/****** Set Intt_LEF ******/
	FH_UINT32 Reg3e00, Reg3e01, Reg3e02;
	intt = MAX(1, intt);
	if (fNo == 0)
	{
		Reg3e00 = (intt >> 12) & 0xf;
		Reg3e01 = (intt >> 4) & 0xff;
		Reg3e02 = (intt << 4) & 0xf0;
		Sensor_WriteEx(pvObj, 0x3e00, Reg3e00, SNS_INT_POS_START);
		Sensor_WriteEx(pvObj, 0x3e01, Reg3e01, SNS_INT_POS_START);
		Sensor_WriteEx(pvObj, 0x3e02, Reg3e02, SNS_INT_POS_START);
		pstCtrlPara->u32CurrSnsIntt = intt;
	}
	return 0;
}

static int SetSnsFrameH(void *pvObj, FH_UINT32 frameH)
{
	SNS_CTRL_PARA_S* pstCtrlPara = _sns_get_ctrl_para(pvObj);
	SensorDevice* pstCci = _sns_get_cci_(pvObj);
	Sensor_Write(pstCci, 0x320e, frameH >> 8);
	Sensor_Write(pstCci, 0x320f, frameH & 0xff);
	pstCtrlPara->u32Vcycle = frameH;

	return 0;
}

static int SetSensorFlipMirror(void *pvObj, FH_UINT32 sensor_en_stat)
{
	SensorDevice* pstCci = _sns_get_cci_(pvObj);
	//FH_UINT8 tmp1;
	FH_UINT8 flip, mirror;
	FH_UINT16 value = 0;

	Sensor_Write(pstCci, 0x100, 0x0);  // stream off
	mirror = (sensor_en_stat >> 1) & 0x1;
	flip = sensor_en_stat & 0x1;
	value = Sensor_Read(pstCci, 0x3221);
	if ((mirror == 1) && (flip == 1)) {
		Sensor_Write(pstCci, 0x3221, value | 0x66);
	} else if (mirror) {
		Sensor_Write(pstCci, 0x3221, (value | 0x6) & ~(0x60));
	} else if (flip) {
		Sensor_Write(pstCci, 0x3221, (value | 0x60) & ~(0x6));
	} else {
		Sensor_Write(pstCci, 0x3221, value & ~(0x66));
	}

	Sensor_Write(pstCci, 0x100, 0x1);  // stream on

	return 0;
}

static int GetSensorFlipMirror(void *pvObj, FH_UINT32 *sensor_en_stat)
{
	SensorDevice* pstCci = _sns_get_cci_(pvObj);
	FH_UINT8 mirror, flip;
	FH_UINT32 tmp;

	tmp = Sensor_Read(pstCci, 0x3221);
	mirror = (tmp & 0x06)?1:0;
	flip = (tmp & 0x60)?1:0;
	*sensor_en_stat = mirror << 1 | flip;

	return 0;
}

static int Sensor_DeInit(void *pvObj)
{
	SensorDevice* pstCci = _sns_get_cci_(pvObj);

	Sensor_Write(pstCci, 0x0100, 0x00);
	SensorReset(pvObj);

	return SensorDevice_Close(pstCci);
}

#define SENSOR_SC2356_MIPI "sc2356_mipi"
#ifdef OS_RTT
struct isp_sensor_if sc2356_mipi_sensor_if_default = {
#else
struct isp_sensor_if sc2356_mipi_sensor_if_default = {
#endif
	.name = SENSOR_SC2356_MIPI,
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
	memcpy(s_if, &sc2356_mipi_sensor_if_default, sizeof(struct isp_sensor_if));

	pstPara = (SNS_PARA_S*)malloc(sizeof(SNS_PARA_S));
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

