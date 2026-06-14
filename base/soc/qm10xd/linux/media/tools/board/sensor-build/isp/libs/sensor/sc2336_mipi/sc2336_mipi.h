/*
 * sc2336_mipi.h
 *
 *  Created on: Aug 11, 2020
 *      Author: hl
 */

#ifndef SC2336_MIPI_H_
#define SC2336_MIPI_H_

#include "cfg/sc2336_mipi_2lane_540p60.h"
#include "cfg/sc2336_mipi_2lane_720p25.h"
#include "cfg/sc2336_mipi_2lane_720p30.h"
#include "cfg/sc2336_mipi_2lane_1080p15.h"
#include "cfg/sc2336_mipi_2lane_1080p25.h"
#include "cfg/sc2336_mipi_2lane_1080p30.h"
#include "cfg/sc2336_mipi_2lane_1080p15_slave.h"

// 用于0x20的sensor的配置
static SNS_CONFIG_S stSnsConf[] =
{
    {
        .u32SnsFmt      = FORMAT_540P60,
        .u32ConfSize    = sizeof(Sensor_Cfg_SC2336_MIPI_540P60_2L) / 4,
        .pu16CciConf    = Sensor_Cfg_SC2336_MIPI_540P60_2L,
        .u8MipiClkRate  = R_200_249,
        .u8MipiLaneNum  = 2,
        .stSnsAttr      = &attr_2l_540P60
    },

    {
        .u32SnsFmt      = FORMAT_720P25,
        .u32ConfSize    = sizeof(Sensor_Cfg_SC2336_MIPI_720P25_2L) / 4,
        .pu16CciConf    = Sensor_Cfg_SC2336_MIPI_720P25_2L,
        .u8MipiClkRate  = R_350_499,
        .u8MipiLaneNum  = 2,
        .stSnsAttr      = &attr_2l_720P25
    },
    {
        .u32SnsFmt      = FORMAT_720P30,
        .u32ConfSize    = sizeof(Sensor_Cfg_SC2336_MIPI_720P30_2L) / 4,
        .pu16CciConf    = Sensor_Cfg_SC2336_MIPI_720P30_2L,
        .u8MipiClkRate  = R_350_499,
        .u8MipiLaneNum  = 2,
        .stSnsAttr      = &attr_2l_720P30
    },
    {
        .u32SnsFmt      = FORMAT_1080P15_SLAVE,
        .u32ConfSize    = sizeof(Sensor_Cfg_SC2336_MIPI_1080P15_SLAVE_2L) / 4,
        .pu16CciConf    = Sensor_Cfg_SC2336_MIPI_1080P15_SLAVE_2L,
        .u8MipiClkRate  = R_350_499,
        .u8MipiLaneNum  = 2,
        .stSnsAttr      = &attr_2l_1080P15_SLAVE
    },
    {
        .u32SnsFmt		= FORMAT_1080P15,
        .u32ConfSize	= sizeof(Sensor_Cfg_SC2336_MIPI_1080P15_2L) / 4,
        .pu16CciConf	= Sensor_Cfg_SC2336_MIPI_1080P15_2L,
        .u8MipiClkRate	= R_350_499,
        .u8MipiLaneNum	= 2,
        .stSnsAttr		= &attr_2l_1080P15
    },

    {
        .u32SnsFmt      = FORMAT_1080P25,
        .u32ConfSize    = sizeof(Sensor_Cfg_SC2336_MIPI_1080P25_2L) / 4,
        .pu16CciConf    = Sensor_Cfg_SC2336_MIPI_1080P25_2L,
        .u8MipiClkRate  = R_350_499,
        .u8MipiLaneNum  = 2,
        .stSnsAttr      = &attr_2l_1080P25
    },

    {
        .u32SnsFmt      = FORMAT_1080P30,
        .u32ConfSize    = sizeof(Sensor_Cfg_SC2336_MIPI_1080P30_2L) / 4,
        .pu16CciConf    = Sensor_Cfg_SC2336_MIPI_1080P30_2L,
        .u8MipiClkRate  = R_350_499,
        .u8MipiLaneNum  = 2,
        .stSnsAttr      = &attr_2l_1080P30
    },
};
#endif // SC2336_MIPI_H_






