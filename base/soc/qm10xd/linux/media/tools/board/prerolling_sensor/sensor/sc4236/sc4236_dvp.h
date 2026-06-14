#ifndef SC4236_DVP_H_
#define SC4236_DVP_H_

#include "cfg/sc4236_dvp_480p2.h"
#include "cfg/sc4236_dvp_480p5.h"
#include "cfg/sc4236_dvp_480p10.h"
#define current_sns_conf sns_conf.conf[sns_cfg.sns_conf_num]
#define current_sns_attr sns_conf.conf[sns_cfg.sns_conf_num].sns_attr
#define DEVICE_ID 0x60 >> 1

static SensorConfigPtr sns_conf;

static SensorConfig sns_conf0[] =
{
    {FORMAT_480P2, sizeof(Sensor_Cfg_SC4236_DVP_480p2) / 4, Sensor_Cfg_SC4236_DVP_480p2, &attr_dvp_480p2},
    {FORMAT_480P5, sizeof(Sensor_Cfg_SC4236_DVP_480p5) / 4, Sensor_Cfg_SC4236_DVP_480p5, &attr_dvp_480p5},
    {FORMAT_480P10, sizeof(Sensor_Cfg_SC4236_DVP_480p10) / 4, Sensor_Cfg_SC4236_DVP_480p10, &attr_dvp_480p10}
};

#endif /* SC4236_DVP_H_ */
