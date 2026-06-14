#ifndef _DSI_HOST_V01_REG_DEF_H_
#define _DSI_HOST_V01_REG_DEF_H_

#include "data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
#define DIS_HOST_2LANE_BASE_ADDR        (0x25700000)
#define DIS_HOST_4LANE_BASE_ADDR        (0x25C00000)

#define DSI_HOST_VERSION               (0x000) //R Version of the DSI host controller
#define DSI_HOST_PWR_UP                (0x004) //R/W Core power-up
#define DSI_HOST_CLKMGR_CFG            (0x008) //R/W Configuration of the internal clock dividers
#define DSI_HOST_DPI_VCID              (0x00C) //R/W DPI virtual channel id
#define DSI_HOST_DPI_COLOR_CODING      (0x010) //R/W DPI color coding
#define DSI_HOST_DPI_CFG_POL           (0x014) //R/W DPI polarity configuration
#define DSI_HOST_DPI_LP_CMD_TIM        (0x018) //R/W Low-power command timing configuration
#define DSI_HOST_DBI_VCID              (0x01C) //R/W DBI virtual channel id
#define DSI_HOST_DBI_CFG               (0x020) //R/W DBI interface configuration
#define DSI_HOST_DBI_PARTITIONING_EN   (0x024) //R/W DBI partitioning enable
#define DSI_HOST_DBI_CMDSIZE           (0x028) //R/W DBI command size configuration
#define DSI_HOST_PCKHDL_CFG            (0x02C) //R/W Packet handler configuration
#define DSI_HOST_GEN_VCID              (0x030) //R/W Generic interface virtual channel id
#define DSI_HOST_MODE_CFG              (0x034) //R/W Video or Command mode selection
#define DSI_HOST_VID_MODE_CFG          (0x038) //R/W Video mode configuration
#define DSI_HOST_VID_PKT_SIZE          (0x03C) //R/W Video packet size
#define DSI_HOST_VID_NUM_CHUNKS        (0x040) //R/W Number of chunks
#define DSI_HOST_VID_NULL_SIZE         (0x044) //R/W Null packet size
#define DSI_HOST_VID_HSA_TIME          (0x048) //R/W Horizontal Sync Active time
#define DSI_HOST_VID_HBP_TIME          (0x04C) //R/W Horizontal Back Porch time
#define DSI_HOST_VID_HLINE_TIME        (0x050) //R/W Line time
#define DSI_HOST_VID_VSA_LINES         (0x054) //R/W Vertical Sync Active period
#define DSI_HOST_VID_VBP_LINES         (0x058) //R/W Vertical Back Porch period
#define DSI_HOST_VID_VFP_LINES         (0x05C) //R/W Vertical Front Porch period
#define DSI_HOST_VID_VACTIVE_LINES     (0x060) //R/W Vertical resolution
#define DSI_HOST_EDPI_CMD_SIZE         (0x064) //R/W Size for eDPI packets
#define DSI_HOST_CMD_MODE_CFG          (0x068) //R/W Command mode configuration
#define DSI_HOST_GEN_HDR               (0x06C) //R/W Generic packet header configuration
#define DSI_HOST_GEN_PLD_DATA          (0x070) //R/W Generic payload data in and out
#define DSI_HOST_CMD_PKT_STATUS        (0x074) //R Command packet status
#define DSI_HOST_TO_CNT_CFG            (0x078) //R/W Timeout timers configuration
#define DSI_HOST_HS_RD_TO_CNT          (0x07C) //R/W Peripheral response timeout definition after high-speed read operations
#define DSI_HOST_LP_RD_TO_CNT          (0x080) //R/W Peripheral response timeout definition after low-power read operations
#define DSI_HOST_HS_WR_TO_CNT          (0x084) //R/W Peripheral response timeout definition after high-speed write operations
#define DSI_HOST_LP_WR_TO_CNT          (0x088) //R/W Peripheral response timeout definition after low-power write operations
#define DSI_HOST_BTA_TO_CNT            (0x08C) //R/W Peripheral response timeout definition after bus turnaround
#define DSI_HOST_SDF_3D                (0x090) //R/W 3D control configuration
#define DSI_HOST_LPCLK_CTRL            (0x094) //R/W Low-power in clock lane
#define DSI_HOST_PHY_TMR_LPCLK_CFG     (0x098) //R/W D-PHY timing configuration for the clock lane
#define DSI_HOST_PHY_TMR_CFG           (0x09C) //R/W D-PHY timing configuration for data lanes
#define DSI_HOST_PHY_RSTZ              (0x0A0) //R/W D-PHY reset control
#define DSI_HOST_PHY_IF_CFG            (0x0A4) //R/W D-PHY interface configuration DSI_HOST_NUMBER_OF_LANES-1
#define DSI_HOST_PHY_ULPS_CTRL         (0x0A8) //R/W D-PHY Ultra Low-Power control
#define DSI_HOST_PHY_TX_TRIGGERS       (0x0AC) //R/W D-PHY transmit triggers
#define DSI_HOST_PHY_STATUS            (0x0B0) //R D-PHY PPI status interface
#define DSI_HOST_PHY_TST_CTRL0         (0x0B4) //R/W D-PHY test interface control 0
#define DSI_HOST_PHY_TST_CTRL1         (0x0B8) //R/W D-PHY test interface control 1
#define DSI_HOST_INT_ST0               (0x0BC) //R Contains the status of interrupt sources from acknowledge reports and the D-PHY
#define DSI_HOST_INT_ST1               (0x0C0) //R Contains the status of interrupt sources related to timeouts, ECC, CRC, packet size
#define DSI_HOST_INT_MSK0              (0x0C4) //R/W Masks the interrupt generation triggered by the INT_ST0 register
#define DSI_HOST_INT_MSK1              (0x0C8) //R/W Masks the interrupt generation triggered by the INT_ST1 register
#define DSI_HOST_PHY_CAL               (0x0C8) //R/W controls the skew calibration of D-PHY
#define DSI_HOST_INT_FORCE0            (0x0D8) //W Force interrupts that affect the INT_ST0 register
#define DSI_HOST_INT_FORCE1            (0x0DC) //W Force interrupts that affect the INT_ST1 register
#define DSI_HOST_DSC_PARAMETER         (0x0F0) //W Configures Display Stream Compression
#define DSI_HOST_TMR_RD_CFG            (0x0F4) //W configures times related to PHY to perform some operations in lane byte block
#define DSI_HOST_VID_SHADOW_CTRL       (0x100) //R/W Controls DPI shadow register mechanism feature
#define DSI_HOST_DPI_VCID_ACT          (0x10C) //R Configures the virtual channel id for the DPI traffic
#define DSI_HOST_DPI_COLOR_CODING_ACT  (0x110) //R Configures DPI color coding
#define DSI_HOST_DPI_LP_CMD_TIM_ACT    (0x118) //R Configures the timing for low-power commands sent while in video mode
#define DSI_HOST_VID_MODE_CFG_ACT      (0x138) //R Configures several aspects of Video mode operation, the transmission mode, switching to low-power in the middle of a frame, enabling acknowledge and whether to send commands in low-power.
#define DSI_HOST_VID_PKT_SIZE_ACT      (0x13C) //R Configures the video packet size
#define DSI_HOST_VID_NUM_CHUNKS_ACT    (0x140) //R Configures the number of chunks to use
#define DSI_HOST_VID_NULL_SIZE_ACT     (0x144) //R Configures the size of null packets
#define DSI_HOST_VID_HSA_TIME_ACT      (0x148) //R Configures the video HSA time
#define DSI_HOST_VID_HBP_TIME_ACT      (0x14C) //R Configures the video HBP time
#define DSI_HOST_VID_HLINE_TIME_ACT    (0x150) //R Configures the overall time for each video line
#define DSI_HOST_VID_VSA_LINES_ACT     (0x154) //R Configures the VSA period
#define DSI_HOST_VID_VBP_LINES_ACT     (0x158) //R Configures the VBP period
#define DSI_HOST_VID_VFP_LINES_ACT     (0x15C) //R Configures the VFP period
#define DSI_HOST_VID_VACTIVE_LINES_ACT (0x160) //R Configures the vertical resolution of video
#define DSI_HOST_SDF_3D_ACT            (0x190) //R Configures 3D control information for VSS packets in video mode

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif