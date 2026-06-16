/*
 * jxf23_dvp.c
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "isp_func_def.h"
#include "clock_ops.h"

#include "isp_sensor_if.h"
#include "sensor_ops.h"

#include "isp_enum.h"
#include "sensor.h"
#include "jxf23_dvp.h"

#define JXF23_DGAIN_ITEM_NUM (5)
#define JXF23_AGAIN_ITEM_NUM (4)

static FH_UINT32 m_gain = 0x40;
static FH_UINT32 m_intt = 0x40;

static struct sns_cfg_s sns_cfg = {
    .inited = 0,
    .MinRstLine = 4,
    .minAGain = 0x40,
    .devIdx = 0,
};

static int SetIntt(FH_UINT32 intt);
static int GetIntt();
static int SetGain(FH_UINT32 again);
static int GetGain();
/********************sensor flow related start****************************/
static void SensorReset(void)
{
    return;
}

static int Sensor_Init(FH_UINT32 devIdx)
{
    sns_cfg.devIdx = devIdx;
    SensorDevice_Init(DEVICE_ID, CCI_MODE, sns_cfg.devIdx);
    SensorReset();
    sns_cfg.inited = 1;
    return 0;
}

static int Sensor_Isconnect(void)
{
    unsigned int pid1, pid2;
    SensorDevice_Init(DEVICE_ID, CCI_MODE, sns_cfg.devIdx);

    pid1 = Sensor_Read(0xa);
    pid2 = Sensor_Read(0xb);

    printf("pid1:0x%x, pid2:0x%x\n", pid1, pid2);
    SensorDevice_Close();

    if (pid1 == 0xf && pid2 == 0x23)
        return 1;
    else
    {
        return 0;
    }
}

static int Sensor_DeInit(void)
{
	if(sns_cfg.sensor_fmt == FORMAT_480P7)
		Sensor_Write(0x12, 0x47);
	else
		Sensor_Write(0x12, 0x40);

    SensorReset();

    return SensorDevice_Close();
}

static int GetSensorViAttr(ISP_VI_ATTR_S *vi_attr)
{
    if (vi_attr == NULL)
        return FH_ERR_NULL_POINTER;

    vi_attr->u16WndWidth = current_sns_attr->u16WndWidth;
    vi_attr->u16WndHeight = current_sns_attr->u16WndHeight;
    vi_attr->u16InputWidth = current_sns_attr->u16InputWidth;
    vi_attr->u16InputHeight = current_sns_attr->u16InputHeight;
    vi_attr->u16PicWidth = current_sns_attr->u16PicWidth;
    vi_attr->u16PicHeight = current_sns_attr->u16PicHeight;
    vi_attr->u16OffsetX = current_sns_attr->u16OffsetX;
    vi_attr->u16OffsetY = current_sns_attr->u16OffsetY;
    vi_attr->enBayerType = current_sns_attr->enBayerType;
    sns_cfg.curr_hsf = current_sns_attr->u16FrameRate * vi_attr->u16WndHeight;

    return 0;
}

static int getSensorConfType(void)
{
    sns_conf.conf = sns_conf0;
    sns_conf.conf_size = sizeof(sns_conf0) / sizeof(SensorConfig);

    return 0;
}

static int getSensorConf(void)
{
    int i;

    getSensorConfType();

    for (i = 0; i < sns_conf.conf_size; i++)
    {
        if (sns_cfg.sensor_fmt == sns_conf.conf[i].sns_fmt)
        {
            return i;
        }
    }

    return -1;
}

//可根据sns_cfg.max_lane_num确定当前使用2lane配置还是4lane配置
static int SetSensorFmt(int format)
{
    printf("set sensor formate!\n");
    SensorDevice_Close();
    sns_cfg.sensor_fmt = format;
    // sns_cfg.wdr_flag = (format >> 16) & 1;
    SensorDevice_Init(DEVICE_ID, CCI_MODE, sns_cfg.devIdx);
    SensorReset();

    if (sns_cfg.inited)
    {
        int i;

        sns_cfg.sns_conf_num = getSensorConf();

        if (sns_cfg.sns_conf_num < 0)
            return -1;

        for (i = 0; i < current_sns_conf.conf_size; i++)
        {
            Sensor_Write(current_sns_conf.conf_addr[2 * i], current_sns_conf.conf_addr[2 * i + 1]);
        }
    }
    else
        return -1;

    sns_cfg.v_cycle = (Sensor_Read(0x23) << 8) | (Sensor_Read(0x22));
    sns_cfg.fullLineStd = sns_cfg.v_cycle;

    sns_cfg.MinRstLine = 4;
    sns_cfg.max_intt = sns_cfg.v_cycle - sns_cfg.MinRstLine;
    SetIntt(480);
    SetGain(80);
    sns_cfg.curr_sensor_gain = 80;//sns_cfg.minAGain;

    return 0;
}
/********************sensor flow related end*************************************/

/************************AE&WDR Related start************************************/
static int GetAEInfo(Sensor_AE_INFO_S *sensAEInfo)
{
    sensAEInfo->currIntt = GetIntt();
    sensAEInfo->currAGain = GetGain();
    sensAEInfo->currHsf = sns_cfg.curr_hsf;
    sensAEInfo->currFrameH = sns_cfg.v_cycle;
    return 0;
}

static int GetAEDefault(Sensor_AE_Default_S *sensAEDefault)
{
    sensAEDefault->minIntt = 1;
    sensAEDefault->maxIntt = sns_cfg.max_intt;
    sensAEDefault->minAGain = sns_cfg.minAGain;
    sensAEDefault->maxAGain = 992;//again 15.5
    sensAEDefault->fullLineStd = sns_cfg.fullLineStd;
    sensAEDefault->MinRstLine = sns_cfg.MinRstLine;

    return 0;
}

// GetGain仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetGain()
{
	printf("gain:%d\n", m_gain);
	return m_gain;
}

static int CalcSnsValidGain(FH_UINT32 *u32Gain)
{
    *u32Gain = (*u32Gain >> 2) << 2;  // U.6->U.4->U.6
    return 0;
}

static int SetGain(FH_UINT32 gain)
{
    FH_UINT32 real_gain = gain >> 2; /* U.6-->U.4 */
    FH_UINT32 integer = real_gain >> 4;
    FH_UINT32 fine;
    FH_UINT8 i = 0;


    while (integer >>= 1)
    {
        i++;
    }
    fine = (real_gain >> i) - 0x10;

    Sensor_Write(0x00, ((i & 0x7) << 4) | fine);

    /* Applied gain */
    m_gain = (((fine + 0x10) << (i & 0x7)) << 2);

    return 0;
}

// GetIntt仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetIntt()
{
	printf("m_intt:0x%x\n", m_intt);

	return m_intt;
}

static int CalcSnsValidIntt(FH_UINT32 *u32Intt)
{
    return 0;
}

static int SetIntt(FH_UINT32 intt)
{
    intt = MAX(1, intt);

    Sensor_Write(0x02, (intt >> 8) & 0xff);
    Sensor_Write(0x01, intt & 0xff);
    m_intt = intt;
    sns_cfg.curr_sensor_intt = intt;

    return 0;
}

static int SetSnsFrameH(FH_UINT32 frameH) {
    Sensor_Write(0x22, frameH & 0xff);
    Sensor_Write(0x23, (frameH >> 8) & 0xff);

    sns_cfg.v_cycle = frameH;

    return 0;
}

static int SetExposureRatio(FH_UINT32 exposure_ratio)
{
//     *exposure_ratio = (sns_cfg.curr_sensor_intt  << 4) / MAX(1, sns_cfg.curr_sensor_short_intt);
    return 0;
}


static int GetExposureRatio(FH_UINT32 *exposure_ratio)
{
//     *exposure_ratio = (sns_cfg.curr_sensor_intt  << 4) / MAX(1, sns_cfg.curr_sensor_short_intt);
    return 0;
}
/************************AE&WDR Related End************************************/

/************************sensor attr related start*****************************/
static int GetSensorAttribute(char * name, FH_UINT32 *value)
{
    if (strcmp(name, "WDR") == 0) {
        *value = sns_cfg.wdr_flag;
        return 0;
    }

    if (strcmp(name, "VI_POL") == 0)
    {
        *value = 0 | (1 << 4) | (12 << 8);
        return 0;
    }

    return -1;
}
/************************sensor attr related end*******************************/

/***********************fastboot related start*********************************/
static int Sensor_Kick()
{
    if (sns_cfg.inited)
    {
		if(sns_cfg.sensor_fmt == FORMAT_480P7)
			Sensor_Write(0x12, 0x07);
		else
			Sensor_Write(0x12, 0x00);
		Sensor_Write(0x48, 0x85);
		Sensor_Write(0x48, 0x05);
    }
    return 0;
}
/***********************fastboot related end**********************************/

/************************sensor func related start****************************/
static int SetSensorReg(unsigned short addr, unsigned short data)
{
    Sensor_Write(addr, data);
    return 0;
}

static int GetSensorReg(unsigned short addr, unsigned short *data)
{
    *data = Sensor_Read(addr);
    return 0;
}

static int SetSensorFlipMirror(FH_UINT32 sensor_en_stat)
{
    // FH_UINT8 tmp1, tmp2;
    // FH_UINT8 flip, mirror;

    // flip = sensor_en_stat & 0x1;
    // mirror = !(((sensor_en_stat >> 1)) & 0x1);

    // tmp1 = Sensor_Read(0x3820);
    // tmp2 = Sensor_Read(0x3821);

    // tmp1 = (tmp1&0xfb) | (flip << 2);
    // tmp2 = (tmp2&0xfb) | (mirror << 2);

    // Sensor_Write(0x3820, tmp1);
    // Sensor_Write(0x3821, tmp2);

    return 0;
}

static int GetSensorFlipMirror(FH_UINT32 *sensor_en_stat)
{
    // FH_UINT8 mirror, flip;

    // flip = (Sensor_Read(0x3820))>>2 & 0x1;
    // mirror = !((Sensor_Read(0x3821))>>2 & 0x1);
    // *sensor_en_stat = (mirror << 1) | flip;

    return 0;
}

static int Sensor_get_sys_info(Sensor_Param_S *sensorParam){
	sensorParam->regAddrCnt = 1;
	sensorParam->regValueCnt = 1;
	sensorParam->u8I2cIndex = 0;
	sensorParam->u8SlaveAddr = DEVICE_ID << 1;
	sensorParam->streamOn.cnt = 3;
	if(sns_cfg.sensor_fmt == FORMAT_480P7){
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
	if(sns_cfg.sensor_fmt == FORMAT_480P7){
		sensorParam->streamOff.regArray[0].addr = 0x12;
		sensorParam->streamOff.regArray[0].reg = 0x47;
	}else{
		sensorParam->streamOff.regArray[0].addr = 0x12;
		sensorParam->streamOff.regArray[0].reg = 0x40;
	}
	return 0;
}


static int SensorCommonIf(FH_UINT32 cmd, ISP_SENSOR_COMMON_CMD_DATA0* data0, ISP_SENSOR_COMMON_CMD_DATA1* data1)
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
/***********************sensor func related end**********************************/

/***********************adv api related start************************************/
static FH_UINT32* GetMirrorFlipBayerFormat(void){return 0;}
static FH_UINT32* GetUserSensorAwbGain(FH_UINT32 idx)
{
    // if(sns_cfg.wdr_flag)
    //     return (AwbGainWdr)[idx];
    // else
    //     return (AwbGain)[idx];
    return 0;
}

// static FH_UINT32* GetSensorLtmCurve(FH_UINT32 idx)
// {

//     return 0;
// }
/***********************adv api related end*************************************/

/***********************not used for now start**********************************/
// static int SetSensorAwbGain(FH_UINT32 *awb_gain){return 0;}
// static int GetSensorAwbGain(FH_UINT32 *awb_gain){return 0;}
static int SetSensorIris(FH_UINT32 iris){return 0;}
/***********************not used for now end************************************/

#define SENSOR_JXF23_DVP    "jxf23_dvp"
#ifdef OS_RTT
struct isp_sensor_if jxf23_dvp_sensor_if __attribute__((section(".sensor_drv"))) = {
#else
struct isp_sensor_if jxf23_dvp_sensor_if = {
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
    .get_sns_reg = GetSensorReg,
    .data = 0,
    .common_if = SensorCommonIf,
    .get_sns_ae_default = GetAEDefault,
    .get_sns_ae_info = GetAEInfo,
    .set_sns_intt  = SetIntt,
    .calc_sns_valid_intt = CalcSnsValidIntt,
    .set_sns_gain = SetGain,
    .calc_sns_valid_gain = CalcSnsValidGain,
    .set_sns_frame_height = SetSnsFrameH,
    .get_sensor_mirror_flip_bayer_format = GetMirrorFlipBayerFormat,
    .get_user_awb_gain = GetUserSensorAwbGain,
    .is_sensor_connect = Sensor_Isconnect,
};

#ifdef OS_RTT
#else
struct isp_sensor_if* (Sensor_Create)() {
    return &jxf23_dvp_sensor_if;
}

void (Sensor_Destroy)(struct isp_sensor_if* s_if) {
    memset(&jxf23_dvp_sensor_if, 0, sizeof(struct isp_sensor_if));
}
#endif
