/*
 * sc4236_dvp.c
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
#include "sc4236_dvp.h"

#define SC4236_DGAIN_ITEM_NUM (5)
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
		{0, 0x10, 0x40, 124, 4, 1},
		{1, 0x10, 0x80, 248, 8, 1},
		{3, 0x10, 0x100, 496, 16, 1},
		{7, 0x10, 0x200, 992, 32, 1},
	}
};

typedef struct{
	FH_UINT32 maxGain; //U 6
	struct{
		FH_UINT32 coarseGainReg;
		FH_UINT32 fineGainRegStart;
		FH_UINT32 gainMin;
		FH_UINT32 gainMax;
		FH_UINT32 gainStep;
		FH_UINT32 regStep;
	}gainItem[SC4236_DGAIN_ITEM_NUM];
}SC4236_Dgain_S;
SC4236_Dgain_S sc4236_dgain_info = {
	.maxGain = 1984,//31 x 0x40;
	.gainItem = {
		{0, 0x80, 0x40, 124, 4, 8},
		{1, 0x80, 0x80, 248, 8, 8},
		{3, 0x80, 0x100, 496, 16, 8},
		{7, 0x80, 0x200, 992, 32, 8},
		{15, 0x80, 0x400, 1984, 64, 8},
	}
};

static FH_UINT32 m_gain = 0x40;
static FH_UINT32 m_intt = 0x40;

static struct sns_cfg_s sns_cfg = {
    .inited = 0,
    .MinRstLine = 4,
    .minAGain = 0x40,
    .devIdx = 1,
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
    SensorDevice_Init(DEVICE_ID, 2, sns_cfg.devIdx);
    SensorReset();
    sns_cfg.inited = 1;
    return 0;
}

static int Sensor_Isconnect(void)
{
    unsigned int sensor_chip_id;
    SensorDevice_Init(DEVICE_ID, 2, sns_cfg.devIdx);

    sensor_chip_id = Sensor_Read(0x3107);
    sensor_chip_id |= Sensor_Read(0x3108) << 8;

    //printf("sensor_chip_id:0x%x\n", sensor_chip_id);
    SensorDevice_Close();

    if (sensor_chip_id == 0x3235)
        return 1;
    else
    {
        return 0;
    }
}

static int Sensor_DeInit(void)
{
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
    //printf("set sensor formate!\n");
    SensorDevice_Close();
    sns_cfg.sensor_fmt = format;
    // sns_cfg.wdr_flag = (format >> 16) & 1;
    SensorDevice_Init(DEVICE_ID, 2, sns_cfg.devIdx);
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

    sns_cfg.v_cycle = (Sensor_Read(0x320e) << 8) | (Sensor_Read(0x320f));
    sns_cfg.fullLineStd = sns_cfg.v_cycle;

    sns_cfg.MinRstLine = 4;
    sns_cfg.max_intt = sns_cfg.v_cycle - sns_cfg.MinRstLine;
    //printf("1 sns_cfg.max_intt:%d\n", sns_cfg.max_intt);
    if(sns_cfg.max_intt > (0xfff / 2)) sns_cfg.max_intt = 0xfff / 2;
    //printf("2 sns_cfg.max_intt:%d\n", sns_cfg.max_intt);
    SetIntt(1100);
    SetGain(0x40);
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
    //printf("sensAEDefault->maxIntt:%d\n", sensAEDefault->maxIntt);
    return 0;
}

// GetGain仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetGain()
{
	int gain = 0;
	FH_UINT32 again = 0, i = 0;
	//FH_UINT32 dgain = 0;
	FH_UINT32 againCoarse = 0, againFine = 0;
#if 0
	FH_UINT32 dgainCoarse = 0, dgainFine = 0;
#endif
	FH_UINT32 fineGainRegStart;
	FH_UINT32 gainMin;
	FH_UINT32 gainStep;
	FH_UINT32 regStep;

	againCoarse = Sensor_Read(0x3e08);
	againFine = Sensor_Read(0x3e09);
#if 0
	dgainCoarse = Sensor_Read(0x3e06);
	dgainFine = Sensor_Read(0x3e07);
#endif

	printf("REG: againCoarse:%d, againFine:%d\n", againCoarse, againFine);
	againCoarse = (againCoarse >> 2) & 0x7;
	for(i = 0; i < SC4236_AGAIN_ITEM_NUM; i++){
		if(againCoarse == sc4236_again_info.gainItem[i].coarseGainReg){
			fineGainRegStart = sc4236_again_info.gainItem[i].fineGainRegStart;
			gainMin = sc4236_again_info.gainItem[i].gainMin;
			gainStep = sc4236_again_info.gainItem[i].gainStep;
			regStep = sc4236_again_info.gainItem[i].regStep;
			again = gainMin + (againFine - fineGainRegStart) / regStep * gainStep;
		}
	}
	
	if(again == 0)
		printf("ERROR\n");
	printf("again:%d, againCoarse:%d, againFine:%d\n", again, againCoarse, againFine);

#if 0
	dgainCoarse = dgainCoarse & 0xF;
	for(i = 0; i < SC4236_DGAIN_ITEM_NUM; i++){
		if(dgainCoarse == sc4236_dgain_info.gainItem[i].coarseGainReg){
			fineGainRegStart = sc4236_dgain_info.gainItem[i].fineGainRegStart;
			gainMin = sc4236_dgain_info.gainItem[i].gainMin;
			gainStep = sc4236_dgain_info.gainItem[i].gainStep;
			regStep = sc4236_dgain_info.gainItem[i].regStep;
			dgain = gainMin + (dgainFine - fineGainRegStart) / regStep * gainStep;
		}
	}

	if(dgain == 0)
		printf("ERROR\n");
	printf("dgain:%d, dgainCoarse:%d, dgainFine:%d\n", dgain, dgainCoarse, dgainFine);

	gain = again * dgain;
#else
	gain = again;
#endif
	printf("gain:%d\n", gain);
	return gain;
}

static int CalcSnsValidGain(FH_UINT32 *u32Gain)
{
    return 0;
}

static int SetGain(FH_UINT32 again)
{
	FH_UINT32 fineGainReg = 0;
	FH_UINT32 i = 0;
	FH_UINT32 coarseGainReg = 0;
	FH_UINT32 fineGainRegStart = 0;
	FH_UINT32 gainMin = 0;
	FH_UINT32 gainStep = 0;
	FH_UINT32 regStep = 0;
	FH_UINT32 found = 0;

	m_gain = again;

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
	printf("again:0x%x, coarseGainReg:0x%x, fineGainReg:0x%x\n", again, coarseGainReg, fineGainReg);

	//gain = {16 h3e08,16 h3e09}/8 h10
	Sensor_Write(0x3e08,coarseGainReg);
	Sensor_Write(0x3e09,fineGainReg);

	coarseGainReg = coarseGainReg >> 2;
	if(coarseGainReg == 0){//
		sns_cfg.curr_sensor_gain = sc4236_again_info.gainItem[0].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[0].gainStep;
		Sensor_Write(0x3301,0x0c); 
		Sensor_Write(0x3633,0x23);
		Sensor_Write(0x3630,0x80);
		Sensor_Write(0x3622,0xf6);
	}else if(coarseGainReg == 1){//
		sns_cfg.curr_sensor_gain = sc4236_again_info.gainItem[1].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[1].gainStep;
		Sensor_Write(0x3301,0x18);
		Sensor_Write(0x3633,0x23);
		Sensor_Write(0x3630,0x80);
		Sensor_Write(0x3622,0xf6);
	}else if(coarseGainReg == 3){//
		sns_cfg.curr_sensor_gain = sc4236_again_info.gainItem[2].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[2].gainStep;
		Sensor_Write(0x3301,0x40);
		Sensor_Write(0x3633,0x23);
		Sensor_Write(0x3630,0x80);
		Sensor_Write(0x3622,0xf6);
	}else if(coarseGainReg == 7){//
		sns_cfg.curr_sensor_gain = sc4236_again_info.gainItem[3].gainMin + (fineGainReg - 0x10) * sc4236_again_info.gainItem[3].gainStep;
		Sensor_Write(0x3301,0x40);
		Sensor_Write(0x3633,0x23);
		Sensor_Write(0x3630,0x80);
		Sensor_Write(0x3622,0xf6);
	}
	printf("pstCtrlPara->u32CurrSnsGain:0x%x\n", sns_cfg.curr_sensor_gain);
	return 0;
}

// GetIntt仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetIntt()
{
	int intt, Reg02, Reg01;

	//{16’h 3e01[7:0], 16’h 3e02[7:4]}
	Reg01 = Sensor_Read(0x3e01) & 0xff;
	Reg02 = Sensor_Read(0x3e02) & 0xff;
	intt = ((Reg01<<4)|(Reg02 >> 4)) / 2;
	printf("Reg01:0x%x, Reg02:0x%x, intt:0x%x\n", Reg01, Reg02, intt);

	return intt;
}

static int CalcSnsValidIntt(FH_UINT32 *u32Intt)
{
    return 0;
}

static int SetIntt(FH_UINT32 intt)
{
    FH_UINT32 intt_reg, intt_h, intt_l;
    printf("intt:%d\n", intt);
    m_intt = intt;
    intt_reg = intt * 2;
    intt_h = (intt_reg >> 4) & 0xff;
    intt_l = (intt_reg << 4) & 0xf0;

    Sensor_Write(0x3e01, intt_h);
    Sensor_Write(0x3e02, intt_l);

    sns_cfg.curr_sensor_intt = intt;

    return 0;
}

static int SetSnsFrameH(FH_UINT32 frameH) {
    Sensor_Write(0x320e, frameH>>8);
    Sensor_Write(0x320f, frameH&0xff);
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
        // enable sensor output
        Sensor_Write(0x0100, 0x01);
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
	sensorParam->regAddrCnt = 2;
	sensorParam->regValueCnt = 1;
	sensorParam->u8I2cIndex = 0;
	sensorParam->u8SlaveAddr = DEVICE_ID << 1;
	sensorParam->streamOn.cnt = 1;
	sensorParam->streamOn.regArray[0].addr = 0x0100;
	sensorParam->streamOn.regArray[0].reg = 0x01;

	sensorParam->streamOff.cnt = 1;
	sensorParam->streamOff.regArray[0].addr = 0x0100;
	sensorParam->streamOff.regArray[0].reg = 0x00;
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

#define SENSOR_SC4236_DVP    "sc4236_dvp"
#ifdef OS_RTT
struct isp_sensor_if sc4236_dvp_sensor_if __attribute__((section(".sensor_drv"))) = {
#else
struct isp_sensor_if sc4236_dvp_sensor_if = {
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
    return &sc4236_dvp_sensor_if;
}

void (Sensor_Destroy)(struct isp_sensor_if* s_if) {
    memset(&sc4236_dvp_sensor_if, 0, sizeof(struct isp_sensor_if));
}
#endif
