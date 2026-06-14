/*
 * gc2053_mipi.h
 *
 *  Created on: Aug 11, 2020
 *      Author: hl
 */

#ifndef GC2053_MIPI_H_
#define GC2053_MIPI_H_

#include "cfg/gc2053_mipi_2lane_1080p25.h"

// 用于0x20的sensor的配置
static SNS_CONFIG_S stSnsConf0[]  =
{
    {
        .u32SnsFmt       = FORMAT_1080P25,
        .u32ConfSize      = sizeof(Sensor_Cfg_GC2053_MIPI_1080P25_2L) / 4,
        .pu16CciConf      = Sensor_Cfg_GC2053_MIPI_1080P25_2L,
        .u8MipiClkRate  = R_500_599,
        .u8MipiLaneNum  = 2,
        .stSnsAttr       = &attr_2l_1080p25
    },
};

#endif // GC2053_MIPI_H_






