#ifndef GC1054_DVP_H_
#define GC1054_DVP_H_

#include "cfg/gc1054_dvp_720p.h"
#define current_sns_conf sns_conf.conf[sns_cfg.sns_conf_num]
#define current_sns_attr sns_conf.conf[sns_cfg.sns_conf_num].sns_attr
#define DEVICE_ID 0x42 >> 1

static SensorConfigPtr sns_conf;

static SensorConfig sns_conf0[] =
{
    {FORMAT_720P30, sizeof(Sensor_Cfg_GC1054_DVP_720p) / 4, Sensor_Cfg_GC1054_DVP_720p, &attr_dvp_720p},

};

#endif /* GC1054_DVP_H_ */
