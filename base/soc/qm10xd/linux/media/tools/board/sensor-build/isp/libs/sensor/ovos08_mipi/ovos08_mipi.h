/*
 * ovos08_mipi.h
 *
 *  Created on: Oct 30, 2018
 *      Author: zy
 */

#ifndef OVOS08_MIPI_H_
#define OVOS08_MIPI_H_

//---------------------------800WP15-------------------------------------------------------
#include "cfg/ovos08_mipi_4lane_10bit_800w15.h"
#include "cfg/ovos08_mipi_4lane_1080p60.h"
//#include "cfg/ovos08_mipi_4lane_800w15.h"
//#include "cfg/ovos08_mipi_4lane_800w25.h"
#include "cfg/ovos08_mipi_4lane_12bit_800w25.h"
#include "cfg/ovos08_mipi_4lane_12bit_800w30.h"
#include "cfg/ovos08_mipi_4lane_800w15_wdr.h"
#include "cfg/ovos08_mipi_4lane_800w25_wdr.h"
// #include "cfg/ovos08_mipi_4lane_800w30.h"
// #include "cfg/ovos08_mipi_4lane_3072X1728P30_wdr.h"
#include "cfg/ovos08_mipi_4lane_2592X1952p30_wdr.h"
#include "cfg/ovos08_mipi_4lane_12bit_3072X1728P30_wdr.h"

static SNS_CONFIG_S stSnsConf0[]  =
{
    {
        .u32SnsFmt       = FORMAT_800WP15,
        .u32ConfSize     = sizeof(Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_800W15) / 4,
        .pu16CciConf     = Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_800W15,
        .u8MipiClkRate   = R_800_899,
        .u8MipiLaneNum   = 4,
        .stSnsAttr       = &attr_4l_12bit_800w15,
        .u32SnsClk       = 27000000,
    },

    {
        .u32SnsFmt        = FORMAT_1080P60,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_1080P60) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_1080P60,
        .u8MipiClkRate    = R_800_899,
        .u8MipiLaneNum    = 4,
        .stSnsAttr        = &attr_4l_1080P60,
        .u32SnsClk        = 27000000,
    },

    {
        .u32SnsFmt        = FORMAT_WDR_800WP15,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_WDR_800WP15) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_WDR_800WP15,
        .u8MipiClkRate    = R_1000_1099,
        .u8MipiLaneNum    = 4,
        .stSnsAttr        = &attr_wdr_4l_800w15,
        .u32SnsClk        = 27000000,
    },

    {
        .u32SnsFmt        = FORMAT_WDR_800WP25,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_WDR_800WP25) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_WDR_800WP25,
        .u8MipiClkRate    = R_1400_1499,
        .u8MipiLaneNum    = 4,
        .stSnsAttr        = &attr_wdr_4l_800w25,
        .u32SnsClk        = 24000000,
    },

    {
        .u32SnsFmt        = FORMAT_800WP25,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_800W25) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_800W25,
        .u8MipiClkRate    = R_1400_1499,
        .u8MipiLaneNum    = 4,
        .u32SnsClk        = 24000000,
        .stSnsAttr        = &attr_4l_12bit_800w25,
    },

    {
        .u32SnsFmt        = FORMAT_800WP30,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_800WP30) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_800WP30,
        .u8MipiClkRate    = R_1400_1499,
        .u8MipiLaneNum    = 4,
        .stSnsAttr        = &attr_4l_12bit_800w30,
        .u32SnsClk        = 24000000,
    },

    {
        .u32SnsFmt        = FORMAT_WDR_3072X1728P30,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_WDR_3072X1728P30) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_4LANE_12BIT_WDR_3072X1728P30,
        .u8MipiClkRate    = R_1400_1499,
        .u8MipiLaneNum    = 4,
        .stSnsAttr        = &attr_wdr_4l_12bit_3072X1728P30,
        .u32SnsClk        = 24000000,
    },

    {
        .u32SnsFmt        = FORMAT_WDR_2592X1952P30,
        .u32ConfSize      = sizeof(Sensor_Cfg_OVOS08_MIPI_WDR_2592X1952P30) / 4,
        .pu16CciConf      = Sensor_Cfg_OVOS08_MIPI_WDR_2592X1952P30,
        .u8MipiClkRate    = R_1400_1499,
        .u8MipiLaneNum    = 4,
        .stSnsAttr        = &attr_4l_2592X1952P30,
        .u32SnsClk        = 27000000,
    },
};
#endif /* OVOS08_MIPI_H_ */
