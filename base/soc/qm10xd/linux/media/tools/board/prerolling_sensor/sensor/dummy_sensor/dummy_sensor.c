#include "dummy_sensor.h"
#include "isp_func_def.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "isp_sensor_if.h"
#include "sensor_ops.h"

#include "isp_enum.h"

static FH_BOOL inited = 0;

static FH_UINT32 sensor_fmt;
static FH_UINT32 wdr_flag;
static FH_UINT32 curr_sensor_gain    = 0;
static int       curr_exposure_ratio = 256;  // U.4

// new add interface
static FH_UINT32 curr_hsf;
static FH_UINT32 curr_frame_h;
static FH_UINT32 max_intt;
static FH_UINT32 max_again;
static FH_UINT32 fullLineStd;
static FH_UINT32 dev_idx;

#define SNS_DEV_ID (0x30)

static int SetMipiLaneMax(FH_UINT32 lane_num)
{
    return 0;
}

static int GetSensorViAttr(ISP_VI_ATTR_S *vi_attr)
{
    if (vi_attr == NULL)
        return FH_ERR_NULL_POINTER;

    switch (sensor_fmt)
    {
    case FORMAT_720P25: // 720p25
        vi_attr->u16WndWidth    = FRAM_W_720P25;
        vi_attr->u16WndHeight   = FRAM_H_720P25;
        vi_attr->u16InputWidth  = ACTIVE_W_720P25;
        vi_attr->u16InputHeight = ACTIVE_H_720P25;
        vi_attr->u16PicWidth    = PIC_IN_W_720P25;
        vi_attr->u16PicHeight   = PIC_IN_H_720P25;
        vi_attr->u16OffsetX     = OFFSET_X_720P25;
        vi_attr->u16OffsetY     = OFFSET_Y_720P25;
        vi_attr->enBayerType    = BAYER_GRBG;
        curr_hsf = 25*vi_attr->u16WndHeight;
        break;
    case FORMAT_720P30: // 720p30
        vi_attr->u16WndWidth    = FRAM_W_720P30;
        vi_attr->u16WndHeight   = FRAM_H_720P30;
        vi_attr->u16InputWidth  = ACTIVE_W_720P30;
        vi_attr->u16InputHeight = ACTIVE_H_720P30;
        vi_attr->u16PicWidth    = PIC_IN_W_720P30;
        vi_attr->u16PicHeight   = PIC_IN_H_720P30;
        vi_attr->u16OffsetX     = OFFSET_X_720P30;
        vi_attr->u16OffsetY     = OFFSET_Y_720P30;
        vi_attr->enBayerType    = BAYER_GRBG;
        curr_hsf = 30*vi_attr->u16WndHeight;
        break;
    case FORMAT_1080P25:
        vi_attr->u16WndWidth    = FRAM_W_1080P25;
        vi_attr->u16WndHeight   = FRAM_H_1080P25;
        vi_attr->u16InputWidth  = ACTIVE_W_1080P25;
        vi_attr->u16InputHeight = ACTIVE_H_1080P25;
        vi_attr->u16PicWidth    = PIC_IN_W_1080P25;
        vi_attr->u16PicHeight   = PIC_IN_H_1080P25;
        vi_attr->u16OffsetX     = OFFSET_X_1080P25;
        vi_attr->u16OffsetY     = OFFSET_Y_1080P25;
        vi_attr->enBayerType    = BAYER_GRBG;
        curr_hsf = 25*vi_attr->u16WndHeight;
        break;
    case FORMAT_1080P30:
        vi_attr->u16WndWidth    = FRAM_W_1080P30;
        vi_attr->u16WndHeight   = FRAM_H_1080P30;
        vi_attr->u16InputWidth  = ACTIVE_W_1080P30;
        vi_attr->u16InputHeight = ACTIVE_H_1080P30;
        vi_attr->u16PicWidth    = PIC_IN_W_1080P30;
        vi_attr->u16PicHeight   = PIC_IN_H_1080P30;
        vi_attr->u16OffsetX     = OFFSET_X_1080P30;
        vi_attr->u16OffsetY     = OFFSET_Y_1080P30;
        vi_attr->enBayerType    = BAYER_GRBG;
        curr_hsf = 30*vi_attr->u16WndHeight;
        break;

    default:
        return FH_ERR_ISP_NO_SUPPORT_FORMAT;
    }

	max_intt = vi_attr->u16WndHeight - 5;
	fullLineStd = vi_attr->u16WndHeight;
    return 0;
}

static void SensorReset(void)
{
    return;
}

static int SetSnsFrameH(FH_UINT32 frameH)
{
    curr_frame_h = frameH;
    return 0;
}

static int CalcSnsValidGain(FH_UINT32 *u32Gain)
{
    return 0;
}

static int SetGain(FH_UINT32 again)
{
    curr_sensor_gain = again;

    return 0;
}

static int CalcSnsValidIntt(FH_UINT32 *u32Intt) {
    return 0;
}

static int SetIntt(FH_UINT32 intt)
{
    return 0;
}

static int SetExposureRatio(FH_UINT32 exposure_ratio)
{
    return 0;
}

static int GetExposureRatio(FH_UINT32 *exposure_ratio)
{
    *exposure_ratio = curr_exposure_ratio;
    return 0;
}

// new add interface
static int GetIntt()
{
    return 0x100;
}

static int GetGain()
{
    return 0x40;
}

static int GetAEInfo(Sensor_AE_INFO_S *sensAEInfo)
{
    sensAEInfo->currIntt = GetIntt();
    sensAEInfo->currAGain = GetGain();
    sensAEInfo->currHsf = curr_hsf;
    sensAEInfo->currFrameH = curr_frame_h;
    return 0;
}

static int GetAEDefault(Sensor_AE_Default_S *sensAEDefault)
{
    sensAEDefault->minIntt = 1;
    sensAEDefault->maxIntt = max_intt;
    sensAEDefault->minAGain = 0x100;
    sensAEDefault->maxAGain = max_again;
    sensAEDefault->fullLineStd = fullLineStd;
    if (wdr_flag) {
        sensAEDefault->MinRstLine = (fullLineStd * 1 / 7);
    } else {
        sensAEDefault->MinRstLine = 5;
    }
    return 0;
}

static int GetSensorAttribute(char *name, FH_UINT32 *value)
{
    if (strcmp(name, "WDR") == 0)
    {
        *value = wdr_flag;
        return 0;
    }

    return -1;
}
static int Sensor_Init(FH_UINT32 devIdx)
{
    dev_idx = devIdx;
    SensorDevice_Init((SNS_DEV_ID >> 1), 2, dev_idx);
    SensorReset();
    inited = 1;
    return 0;
}

static int Sensor_DeInit(void)
{
    SensorReset();

    return SensorDevice_Close();
}

static void SetSensorMipiCfg(void)
{
#ifdef EMU
    struct mipi_conf config;
    config.raw_type = RAW10;
    config.sensor_mode = NOT_SONY_WDR_USE_WDR;
    config.lane_num = 4;
    config.frq_range = R_900_949;
    if (wdr_flag) {
        config.lf_vc_id = 0;
        config.sf_vc_id = 1;
    }

    else {
        config.lf_vc_id = 0;
        config.sf_vc_id = 0;
    }
    mipi_init(&config);
#endif
    return;
}

static int SetSensorFmt(int format)
{
    SensorDevice_Close();
    sensor_fmt = format;
    wdr_flag   = (format >> 16) & 1;

    SetSensorMipiCfg();
    SensorDevice_Init((SNS_DEV_ID >> 1), 2, dev_idx);
    SensorReset();
    if (inited)
    {
        // config sensor
    }

    curr_frame_h = 0x100;
    // init intt,sensor_gain,exposure_ratio

    return 0;
}

static int Sensor_Kick()
{
    // int i;
    if (inited)
    {
        // enable sensor output
    }
    return 0;
}

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
    return 0;
}
static int GetSensorFlipMirror(FH_UINT32 *sensor_en_stat)
{
    return 0;
}

FH_UINT32 *NAME(GetDefaultParam)()
{
    return 0;
}

FH_UINT32 *NAME(GetContrast)()
{
    return 0;
}
FH_UINT32 *NAME(GetSaturation)()
{
    return 0;
}
FH_UINT32 *NAME(GetSharpness)()
{
    return 0;
}

FH_UINT32 NAME(*GetMirrorFlipBayerFormat)()
{
	return 0;
}
FH_UINT32 *NAME(GetSensorAwbGain)(int idx)
{
    return 0;
}

FH_UINT32 *NAME(GetSensorLtmCurve)(int idx)
{
    return 0;
}

#if MULTI_SENSOR
static int Sensor_Isconnect(void) {return 0;}
#endif
#define DUMMY_SENSOR "dummy_sensor"
struct isp_sensor_if dummy_sensor;

struct isp_sensor_if *NAME(Sensor_Create)()
{
#if MULTI_SENSOR
    if (!Sensor_Isconnect())
        return 0;
#endif
	memset(&dummy_sensor, 0, sizeof(struct isp_sensor_if));
	dummy_sensor.name                 = DUMMY_SENSOR;
	dummy_sensor.init                 = Sensor_Init;
	dummy_sensor.get_vi_attr          = GetSensorViAttr;
	dummy_sensor.set_sns_fmt          = SetSensorFmt;
    dummy_sensor.kick                 = Sensor_Kick;
	dummy_sensor.reset                = SensorReset;
	dummy_sensor.set_sns_reg          = SetSensorReg;
	dummy_sensor.set_flipmirror       = SetSensorFlipMirror;
	dummy_sensor.get_flipmirror       = GetSensorFlipMirror;
	dummy_sensor.set_exposure_ratio   = SetExposureRatio;
    dummy_sensor.get_exposure_ratio   = GetExposureRatio;
    dummy_sensor.set_lane_num_max     = SetMipiLaneMax;
    dummy_sensor.get_sensor_attribute = GetSensorAttribute;
    dummy_sensor.get_sns_reg          = GetSensorReg;
    dummy_sensor.get_sns_ae_default   = GetAEDefault;
    dummy_sensor.get_sns_ae_info      = GetAEInfo;
    dummy_sensor.set_sns_intt         = SetIntt;
    dummy_sensor.calc_sns_valid_intt  = CalcSnsValidIntt;
    dummy_sensor.set_sns_gain         = SetGain;
    dummy_sensor.calc_sns_valid_gain  = CalcSnsValidGain;
    dummy_sensor.set_sns_frame_height = SetSnsFrameH;
    dummy_sensor.deinit               = Sensor_DeInit;

    return &dummy_sensor;
}

void NAME(Sensor_Destroy)(struct isp_sensor_if* s_if)
{
	dummy_sensor.name                 = 0;
	dummy_sensor.init                 = 0;
	dummy_sensor.get_vi_attr          = 0;
	dummy_sensor.set_sns_fmt          = 0;
    dummy_sensor.kick                 = 0;
	dummy_sensor.set_sns_gain        = 0;
	dummy_sensor.reset                = 0;
	dummy_sensor.set_sns_intt         = 0;
	dummy_sensor.set_sns_reg          = 0;
	dummy_sensor.set_flipmirror       = 0;
	dummy_sensor.get_flipmirror       = 0;
	dummy_sensor.set_exposure_ratio   = 0;
    dummy_sensor.get_exposure_ratio   = 0;
    dummy_sensor.set_lane_num_max     = 0;
    dummy_sensor.get_sensor_attribute = 0;
    dummy_sensor.get_sns_reg          = 0;
    dummy_sensor.get_sns_ae_default   = 0;
    dummy_sensor.set_sns_intt         = 0;
    dummy_sensor.calc_sns_valid_intt  = 0;
    dummy_sensor.set_sns_gain         = 0;
    dummy_sensor.calc_sns_valid_gain  = 0;
    dummy_sensor.set_sns_frame_height = 0;
    dummy_sensor.get_sns_ae_info      = 0;
	dummy_sensor.deinit               = 0;
}


