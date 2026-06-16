/*
 * gc2053_dvp.c
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
#include "gc2053_dvp.h"

static FH_UINT32 m_gain = 0x40;
static FH_UINT32 m_intt = 0x40;

static struct sns_cfg_s sns_cfg = {
    .inited = 0,
    .MinRstLine = 5,
    .minAGain = 0x40,
    .devIdx = 0,
};

FH_UINT8 regValTable[29][4] = {
    {0x00, 0x00,0x01,0x00},
    {0x00, 0x10,0x01,0x0c},
    {0x00, 0x20,0x01,0x1b},
    {0x00, 0x30,0x01,0x2c},
    {0x00, 0x40,0x01,0x3f},
    {0x00, 0x50,0x02,0x16},
    {0x00, 0x60,0x02,0x35},
    {0x00, 0x70,0x03,0x16},
    {0x00, 0x80,0x04,0x02},
    {0x00, 0x90,0x04,0x31},
    {0x00, 0xa0,0x05,0x32},
    {0x00, 0xb0,0x06,0x35},
    {0x00, 0xc0,0x08,0x04},
    {0x00, 0x5a,0x09,0x19},
    {0x00, 0x83,0x0b,0x0f},
    {0x00, 0x93,0x0d,0x12},
    {0x00, 0x84,0x10,0x00},
    {0x00, 0x94,0x12,0x3a},
    {0x01, 0x2c,0x1a,0x02},
    {0x01, 0x3c,0x1b,0x20},
    {0x00, 0x8c,0x20,0x0f},
    {0x00, 0x9c,0x26,0x07},
    {0x02, 0x64,0x36,0x21},
    {0x02, 0x74,0x37,0x3a},
    {0x00, 0xc6,0x3d,0x02},
    {0x00, 0xdc,0x3f,0x3f},
    {0x02, 0x85,0x3f,0x3f},
    {0x02, 0x95,0x3f,0x3f},
    {0x00, 0xce,0x3f,0x3f},
};

FH_UINT32 gainLevelTable[30] =
{
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
    0xffffffff
};

static int SetIntt(FH_UINT32 intt);
static int GetIntt();
static int SetGain(FH_UINT32 again);
static int GetGain();
static int SetExposureRatio(FH_UINT32 exposure_ratio);
static int GetExposureRatio(FH_UINT32 *exposure_ratio);
/********************sensor flow related start****************************/
static void SensorReset(void)
{
    return;
}

static int Sensor_Init(FH_UINT32 devIdx)
{
    sns_cfg.devIdx = devIdx;
    SensorDevice_Init(DEVICE_ID, 0, sns_cfg.devIdx);
    SensorReset();
    sns_cfg.inited = 1;
    return 0;
}

static int Sensor_Isconnect(void)
{
    unsigned int sensor_chip_id;
    SensorDevice_Init(DEVICE_ID, 2, sns_cfg.devIdx);

    sensor_chip_id = Sensor_Read(0xf1);
    sensor_chip_id |= Sensor_Read(0xf0) << 8;


    SensorDevice_Close();

    if (sensor_chip_id == 0x2053)
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

// static void SetSensorMipiCfg(void)
// {
//     struct mipi_conf config;
//     config.frq_range = current_sns_conf.mipi_clk_rate;
//     config.sensor_mode = NOT_SONY_WDR_USE_WDR;
//     config.raw_type = RAW10;
//     config.lane_num = current_sns_conf.mipi_lane_num;

//     if (sns_cfg.wdr_flag == 1)
//     {
//         config.lf_vc_id = 0;
//         config.sf_vc_id = 1;
//     }
//     else
//     {
//         config.lf_vc_id = 0;
//         config.sf_vc_id = 0;
//     }

//     mipi_init(&config);
//     return;
// }

// static int SetLaneNumMax(FH_UINT32 lane_num)
// {
// #ifdef FPGA_ENV
//     sns_cfg.maxLaneNum = 4;
// #else
//     sns_cfg.maxLaneNum = 2;
// #endif
    // return 0;
// }

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

    // if (current_sns_attr->u16FrameRate == 12)
    //     sns_cfg.curr_hsf = 25 * vi_attr->u16WndHeight / 2;
    // else
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
            // if (sns_cfg.maxLaneNum >= sns_conf.conf[i].mipi_lane_num)
            // {
                return i;
            // }
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
    SensorDevice_Init(DEVICE_ID, 0, sns_cfg.devIdx);
    SensorReset();

    if (sns_cfg.inited)
    {
        int i;

        sns_cfg.sns_conf_num = getSensorConf();

        if (sns_cfg.sns_conf_num < 0)
            return -1;

        // SetSensorMipiCfg();

        for (i = 0; i < current_sns_conf.conf_size; i++)
        {
            Sensor_Write(current_sns_conf.conf_addr[2 * i], current_sns_conf.conf_addr[2 * i + 1]);
        }

        // Sensor_Write(0x0100,0x01);
    }
    else
        return -1;

    Sensor_Write(0xfe, 0x00);
    sns_cfg.v_cycle = (Sensor_Read(0x0d) << 8) | (Sensor_Read(0x0e));
    sns_cfg.fullLineStd = sns_cfg.v_cycle;

    // if (sns_cfg.wdr_flag == 0)
    // {
        sns_cfg.MinRstLine = 5;
    // }
    // else
    // {
    //     sns_cfg.MinRstLine = sns_cfg.v_cycle / 7 + 5;
    // }
    sns_cfg.max_intt = sns_cfg.v_cycle - sns_cfg.MinRstLine;
    SetIntt(480);
    SetExposureRatio(sns_cfg.curr_exposure_ratio);
    SetGain(80);
    sns_cfg.curr_sensor_gain = sns_cfg.minAGain;

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
    sensAEDefault->maxAGain = 0xff7;
    sensAEDefault->fullLineStd = sns_cfg.fullLineStd;
    sensAEDefault->MinRstLine = sns_cfg.MinRstLine;

    return 0;
}

// GetGain仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetGain()
{
    return m_gain;
}

static int CalcSnsValidGain(FH_UINT32 *u32Gain)
{
    return 0;
}

static int total = sizeof(gainLevelTable) / sizeof(FH_UINT32);
static int SetGain(FH_UINT32 again)
{
    FH_UINT32  tol_dig_gain = 0;
    m_gain = again;

    int i;
    int temperature_value;
    int temperature_value0,temperature_value1,temperature_value2,temperature_value3;

    total = sizeof(gainLevelTable) / sizeof(FH_UINT32);

    temperature_value0=Sensor_Read(0x0c);
    temperature_value1=Sensor_Read(0x0d);
    temperature_value2=Sensor_Read(0x0e);
    temperature_value3=Sensor_Read(0x0f);
    Sensor_Write(0xfe,0x00);
    temperature_value=(temperature_value0+temperature_value1+temperature_value2+temperature_value3)>>2;
    if(temperature_value >= 0x78)
    {
        total=total-1;
        if(total<=12) total=12;  
    }

    if(temperature_value<0x20)
    {
        total= sizeof(gainLevelTable) / sizeof(FH_UINT32);
    }

    for(i = 0;i < total;i++ )
    {
        if((gainLevelTable[i] <= again)&&(again < gainLevelTable[i+1]))
            break;
    }

    tol_dig_gain = again*64/gainLevelTable[i];
    //Sensor_Write(0xfe,0x00);
    Sensor_Write(0xfe,0x00);
    //Sensor_Write(0x87,0x58);//[6] aec delay mode
    Sensor_Write(0xb4,regValTable[i][0]);
    Sensor_Write(0xb3,regValTable[i][1]);
    Sensor_Write(0xb8,regValTable[i][2]);
    Sensor_Write(0xb9,regValTable[i][3]);

    Sensor_Write(0xb1,(tol_dig_gain>>6));
    Sensor_Write(0xb2,((tol_dig_gain&0x3f)<<2));

    return 0;
}

// GetIntt仅在ae refresh时会重新获取,所以尽量直接从sensor端获取,保证intt的正确性
static int GetIntt()
{
    int intt, Reg02, Reg01;
    if(m_intt == 0){
        Reg01 = Sensor_Read(0x01) & 0xff;
        Reg02 = Sensor_Read(0x02) & 0xff;
        intt = (Reg02<<8)|Reg01;
    }
    intt =m_intt;
    return intt;
}

static int CalcSnsValidIntt(FH_UINT32 *u32Intt)
{
    return 0;
}

// static int Set_short_exp_intt(FH_UINT32 intt)
// {
//     FH_UINT32 intt_h, intt_l;

//     intt_h = (intt >> 8) & 0xff;
//     intt_l = intt & 0xff;

//     Sensor_Write(0x3511, intt_h);
//     Sensor_Write(0x3512, intt_l);
//     sns_cfg.curr_sensor_short_intt = intt;
//     return 0;
// }

static int SetIntt(FH_UINT32 intt)
{

    FH_UINT32 intt_h, intt_l;
    m_intt = intt;
    intt_h = (intt >> 8) & 0xff;
    intt_l = intt & 0xff;

    Sensor_Write(0xfe, 0x00);
    Sensor_Write(0x03, intt_h);
    Sensor_Write(0x04, intt_l);

    sns_cfg.curr_sensor_intt = intt;
    // if(sns_cfg.wdr_flag)
    // {
    //     FH_UINT32 short_exp_intt;
    //     short_exp_intt = MAX(1, ((MIN(intt, sns_cfg.max_intt) << 4) + (sns_cfg.curr_exposure_ratio >> 1)) / sns_cfg.curr_exposure_ratio);
    //     Set_short_exp_intt(short_exp_intt);
    // }

    return 0;
}

static int SetSnsFrameH(FH_UINT32 frameH) {
    Sensor_Write(0xfe, 0x00);
    Sensor_Write(0x0d, frameH>>8);
    Sensor_Write(0x0e, frameH&0xff);
    sns_cfg.v_cycle = frameH;

    return 0;
}

static int SetExposureRatio(FH_UINT32 exposure_ratio)
{
//     sns_cfg.curr_exposure_ratio = exposure_ratio;
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

    return -1;
}
/************************sensor attr related end*******************************/

/***********************fastboot related start*********************************/
static int Sensor_Kick()
{
    if (sns_cfg.inited)
    {
        // enable sensor output
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

static int SensorCommonIf(FH_UINT32 cmd, ISP_SENSOR_COMMON_CMD_DATA0* data0, ISP_SENSOR_COMMON_CMD_DATA1* data1)
{
    int ret = -1;
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

#define SENSOR_GC2053_DVP    "gc2053_dvp"
#ifdef OS_RTT
struct isp_sensor_if gc2053_dvp_sensor_if __attribute__((section(".sensor_drv"))) = {
#else
struct isp_sensor_if gc2053_dvp_sensor_if = {
#endif
    .name = SENSOR_GC2053_DVP,
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
    // .set_lane_num_max = SetLaneNumMax,
    .get_sns_reg = GetSensorReg,
    // .get_awb_gain = GetSensorAwbGain,
    // .set_awb_gain = SetSensorAwbGain,
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
    // .get_user_ltm_curve = GetSensorLtmCurve,
    .is_sensor_connect = Sensor_Isconnect,
};

#ifdef OS_RTT
#else
struct isp_sensor_if* (Sensor_Create)() {
    return &gc2053_dvp_sensor_if;
}

void (Sensor_Destroy)(struct isp_sensor_if* s_if) {
    memset(&gc2053_dvp_sensor_if, 0, sizeof(struct isp_sensor_if));
}
#endif
