#ifndef _MOL_LCM_MCU_DRV_H_
#define _MOL_LCM_MCU_DRV_H_

#include "../../lcm_module/include/data_type.h"

#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define LCM_MCU_REG_ADDR_BITS 32
#define LCM_MCU_DATA_BITS 32

#define LCM_MCU_CMD_FIFO_MAX 1
#define LCM_MCU_DATA_FIFO_MAX 16

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
typedef enum {
    LCM_MCU_IF_18BITS = 0,
    LCM_MCU_IF_16BITS,
    LCM_MCU_IF_9BITS,
    LCM_MCU_IF_8BITS,
    LCM_MCU_IF_MAX,
}lcm_mcu_if;

typedef enum {
    LCM_RGB_IF_18BITS = 0,
    LCM_RGB_IF_16BITS,
    LCM_RGB_IF_6BITS,
    LCM_RGB_IF_24BITS,
    LCM_RGB_IF_MAX,
}lcm_rgb_if;

typedef enum {
    LCM_BGR = 0,
    LCM_GBR,
    LCM_RBG,
    LCM_BRG,
    LCM_GRB,
    LCM_RGB,
    LCM_RGB_ORDER_MAX,
}lcm_rgb_order;

typedef enum {
    LCM_DATA_WR_RD = 0,
    LCM_DATA_CS,
    LCM_DATA_CS_MAX,
}lcm_cs_cfg;

typedef enum {
    LCM_RGB_MODE = 0,
    LCM_MCU_MODE,
    LCM_MCU_MODE_MAX,
}lcm_mcu_mode;

typedef enum {
    LCM_LSB_FIRST = 0,
    LCM_HSB_FIRST,
    LCM_SEND_MAX,
}lcm_send_cfg;

typedef enum {
    LCM_READ = 0,
    LCM_WRITE,
    LCM_OPS_MAX,
}lcm_ops_cfg;

typedef enum {
    LCM_RISE_EDGE = 0,
    LCM_FALL_EDGE,
    LCM_EDGE_TYPE_MAX,
}lcm_edge;

typedef enum {
    LCM_BUS_IDLE = 0,
    LCM_BUS_CMD,
    LCM_BUS_WR_DATA,
    LCM_BUS_RD_DATA,
    LCM_BUS_STATUS_MAX,
}lcm_bus_status;

typedef struct
{
    uint32_t csType;
    uint32_t wrSetup;
    uint32_t wrHold;
    uint32_t wrDestory;
    uint32_t rdSetup;
    uint32_t rdHold;
    uint32_t rdDestory;
}lcm_mcu_timing_t;

typedef struct
{
    uint32_t bitDepth;
    uint32_t rgbOrder;
}lcm_mcu_format_t;

typedef struct
{
    uint32_t mapTab[32];
    uint32_t mcuMode;
    uint32_t mcuIfType;
    uint32_t rgbIfType;
    uint32_t transNum;
    uint32_t transEndian;
}lcm_mcu_if_t;

typedef struct
{
    uint32_t eb;
    uint32_t mode;
    uint32_t edge;
    uint32_t type;
}lcm_mcu_te_t;


typedef struct
{
    uint32_t lcd_id;
    uint32_t globalBaseAddr;
    uint32_t baseAddr;
    lcm_mcu_timing_t timing;
    lcm_mcu_if_t port;
    lcm_mcu_format_t format;
    lcm_mcu_te_t te;
    uint32_t (*regRead) (ADDR addr);
    void (*regWrite) (ADDR addr, uint32_t data);
}lcm_mcu_drv_t;

uint32_t lcmGetRegDefault(lcm_mcu_drv_t *instance);
int32_t lcmSetMcuBitDepth(lcm_mcu_drv_t *instance, uint32_t bitDepth);
int32_t lcmSetMcuRgbOrder(lcm_mcu_drv_t *instance, uint32_t rgbOrder);
int32_t lcmSetMcuMode(lcm_mcu_drv_t *instance, uint32_t mcuMode);
int32_t lcmSetMcuTimingDataPolarity(lcm_mcu_drv_t *instance, uint32_t pol);
int32_t lcmSetMcuTimingHsyncPolarity(lcm_mcu_drv_t *instance, uint32_t pol);
int32_t lcmSetMcuTimingVsyncPolarity(lcm_mcu_drv_t *instance, uint32_t pol);
int32_t lcmSetMcuTimingShutDownPolarity(lcm_mcu_drv_t *instance, uint32_t pol);
int32_t lcmSetMcuTimingColorRmPolarity(lcm_mcu_drv_t *instance, uint32_t pol);
int32_t lcmSetMcuTeEnable(lcm_mcu_drv_t *instance, uint32_t eb);
int32_t lcmSetMcuDitherEnable(lcm_mcu_drv_t *instance, uint32_t eb);
int32_t lcmSetMcuBitSwapEnable(lcm_mcu_drv_t *instance, uint32_t eb);
int32_t lcmSetMcuClkLockDataEdge(lcm_mcu_drv_t *instance, uint32_t edge);
int32_t lcmSetMcuTeEdge(lcm_mcu_drv_t *instance, uint32_t edge);
int32_t lcmSetMcuRgbMap(lcm_mcu_drv_t *instance, uint32_t map);
int32_t lcmSetDisplayDelay(lcm_mcu_drv_t *instance, uint32_t delay);
int32_t lcmSetMcuIfType(lcm_mcu_drv_t *instance, uint32_t ifType);
int32_t lcmSetRgbIfType(lcm_mcu_drv_t *instance, uint32_t ifType);
int32_t lcmSetCsType(lcm_mcu_drv_t *instance, uint32_t csType);
int32_t lcmSetTeType(lcm_mcu_drv_t *instance, uint32_t teType);
int32_t lcmSetSendEndain(lcm_mcu_drv_t *instance, uint32_t endian);
int32_t lcmSetTrans3(lcm_mcu_drv_t *instance, uint32_t eb);
uint32_t lcmGetTransFifoReady(lcm_mcu_drv_t *instance);
int32_t lcmSetBitMap(lcm_mcu_drv_t *instance, uint32_t* bitMap);
int32_t lcmSetTimingWrSetup(lcm_mcu_drv_t *instance, uint32_t wrSetup);
int32_t lcmSetTimingWrHold(lcm_mcu_drv_t *instance, uint32_t wrHold);
int32_t lcmSetTimingWrDestory(lcm_mcu_drv_t *instance, uint32_t wrDestory);
int32_t lcmSetTimingRdSetup(lcm_mcu_drv_t *instance, uint32_t rdSetup);
int32_t lcmSetTimingRdHold(lcm_mcu_drv_t *instance, uint32_t rdHold);
int32_t lcmSetTimingRdDestory(lcm_mcu_drv_t *instance, uint32_t rdDestory);
int32_t lcmSetStatusSwitchTrigger(lcm_mcu_drv_t *instance);
int32_t lcmSetStageStatus(lcm_mcu_drv_t *instance, uint32_t status);
uint32_t lcmGetStageStatus(lcm_mcu_drv_t *instance);
uint32_t lcmGetCmdStatus(lcm_mcu_drv_t *instance);
int32_t lcmSetCmdRun(lcm_mcu_drv_t *instance);
int32_t lcmSetCmdFifoClr(lcm_mcu_drv_t *instance);
int32_t lcmSetCmdRW(lcm_mcu_drv_t *instance, uint32_t rw);
int32_t lcmSetCmdDummyNum(lcm_mcu_drv_t *instance, uint32_t num);
int32_t lcmSetCmdDataNum(lcm_mcu_drv_t *instance, uint32_t num);
int32_t lcmSetCmdAddrNum(lcm_mcu_drv_t *instance, uint32_t num);
uint32_t lcmGetCmdWfifoDepth(lcm_mcu_drv_t *instance);
uint32_t lcmGetCmdWfifoFull(lcm_mcu_drv_t *instance);
uint32_t lcmGetCmdRfifoDepth(lcm_mcu_drv_t *instance);
uint32_t lcmGetCmdRfifoEmpty(lcm_mcu_drv_t *instance);
uint32_t lcmSetWData(lcm_mcu_drv_t *instance, uint32_t *data, uint32_t num);
uint32_t lcmGetRData(lcm_mcu_drv_t *instance, uint32_t *data, uint32_t num);
uint32_t lcmSetWCmd(lcm_mcu_drv_t *instance, uint32_t *data, uint32_t num);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

