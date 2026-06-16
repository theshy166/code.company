#ifndef JXF23_DVP_H_
#define JXF23_DVP_H_

#include "cfg/jxf23_dvp_480p7.h"
//#include "cfg/jxf23_dvp_480p30.h"
#include "cfg/jxf23_dvp_720p2.h"
#include "cfg/jxf23_dvp_720p25.h"
#define current_sns_conf sns_conf.conf[sns_cfg.sns_conf_num]
#define current_sns_attr sns_conf.conf[sns_cfg.sns_conf_num].sns_attr
#define DEVICE_ID 0x80 >> 1
#define CCI_MODE 0

static SensorConfigPtr sns_conf;

static SensorConfig sns_conf0[] =
{
    {FORMAT_480P7, sizeof(Sensor_Cfg_JXF23_DVP_480P7) / 4, Sensor_Cfg_JXF23_DVP_480P7, &attr_dvp_480p7},
    {FORMAT_720P2, sizeof(Sensor_Cfg_JXF23_DVP_720P2) / 4, Sensor_Cfg_JXF23_DVP_720P2, &attr_dvp_720p2},
    {FORMAT_720P25, sizeof(Sensor_Cfg_720P25FPS) / 4, Sensor_Cfg_720P25FPS, &attr_dvp_720p25},
};

#endif /* JXF23_DVP_H_ */
