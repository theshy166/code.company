/*
 * sc2356_mipi.h
 */

#ifndef SC2356_MIPI_H_
#define SC2356_MIPI_H_

#include "cfg/sc2356_mipi_1lane_1200p30.h"
#include "cfg/sc2356_mipi_1lane_1200p30_slave.h"

// 用于0x6c的sensor的配置
static SNS_CONFIG_S stSnsConf[] =
{
    {
        .u32SnsFmt      = FORMAT_1200P30,
        .u32ConfSize    = sizeof(SNS_CFG_SC2356_MIPI_1200P30_1L) / 4,
        .pu16CciConf    = SNS_CFG_SC2356_MIPI_1200P30_1L,
        .u8MipiClkRate  = R_700_799,
        .u8MipiLaneNum  = 1,
        .stSnsAttr      = &ATTR_1L_1200P30
    },
    {
        .u32SnsFmt      = FORMAT_1200P30_SLAVE,
        .u32ConfSize    = sizeof(SNS_CFG_SC2356_MIPI_1200P30_1L_SLAVE) / 4,
        .pu16CciConf    = SNS_CFG_SC2356_MIPI_1200P30_1L_SLAVE,
        .u8MipiClkRate  = R_700_799,
        .u8MipiLaneNum  = 1,
        .stSnsAttr      = &ATTR_1L_1200P30_SLAVE
    },
};
#endif // SC2356_MIPI_H_

