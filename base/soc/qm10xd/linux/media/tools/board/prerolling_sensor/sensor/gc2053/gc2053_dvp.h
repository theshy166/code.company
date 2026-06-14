#ifndef GC2053_DVP_H_
#define GC2053_DVP_H_

#include "cfg/gc2053_dvp_1080p.h"
#define current_sns_conf sns_conf.conf[sns_cfg.sns_conf_num]
#define current_sns_attr sns_conf.conf[sns_cfg.sns_conf_num].sns_attr
#define DEVICE_ID 0x6e >> 1

static SensorConfigPtr sns_conf;

static SensorConfig sns_conf0[] =
{
    {FORMAT_1080P25, sizeof(Sensor_Cfg_GC2053_DVP_1080p) / 4, Sensor_Cfg_GC2053_DVP_1080p, &attr_dvp_1080p},

};

#endif /* GC2053_DVP_H_ */
