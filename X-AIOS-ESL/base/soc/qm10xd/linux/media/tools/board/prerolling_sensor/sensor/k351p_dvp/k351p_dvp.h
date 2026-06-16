#ifndef K351P_DVP_H_
#define K351P_DVP_H_

#define K351P_FRAM_W_ALGIN 1

#if K351P_FRAM_W_ALGIN
#define K351P_FRAM_W_500 480
#else
#define K351P_FRAM_W_500 500
#endif

#include "cfg/k351p_dvp_500p2.h"
#include "cfg/k351p_dvp_500p5.h"

#define current_sns_conf sns_conf.conf[sns_cfg.sns_conf_num]
#define current_sns_attr sns_conf.conf[sns_cfg.sns_conf_num].sns_attr
#define DEVICE_ID 0x80 >> 1
#define CCI_MODE 0

static SensorConfigPtr sns_conf;

static SensorConfig sns_conf0[] =
{
    {FORMAT_500P2, sizeof(Sensor_Cfg_K351P_DVP_500P2) / 4, Sensor_Cfg_K351P_DVP_500P2, &attr_dvp_500P2},
    {FORMAT_500P5, sizeof(Sensor_Cfg_K351P_DVP_500P5) / 4, Sensor_Cfg_K351P_DVP_500P5, &attr_dvp_500P5},
};

#endif /* JXF23_DVP_H_ */
