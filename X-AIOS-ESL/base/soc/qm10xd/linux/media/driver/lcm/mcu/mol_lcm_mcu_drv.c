#include "reg_pool.h"
#include "data_type.h"
#include "mol_lcm_mcu_reg.h"
#include "mol_lcm_mcu_drv.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef     __cplusplus
extern     "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

reg_description_t s_lcm_mcu_reg[] = {
{{DHD_DISPLAY_FROAMT,0x00000000}, 0x00, "DHD_DISPLAY_FROAMT"},
{{DHD_INTERRUPT_CLR, 0x00000000}, 0x00, "DHD_INTERRUPT_CLR"},
{{DHD_INTERRUPT_RAW, 0x00000000}, 0x00, "DHD_INTERRUPT_RAW"},
{{DHD_DISPLAY_CFG,   0x00000000}, 0x00, "DHD_DISPLAY_CFG"},
{{DHD_DISPLAY_DELAY, 0x00000000}, 0x00, "DHD_DISPLAY_DELAY"},
{{LCM_CFG,           0x00000000}, 0x00, "LCM_CFG"},
{{LCM_BITMAP0,       0x00000000}, 0x00, "LCM_BITMAP0"},
{{LCM_BITMAP1,       0x00000000}, 0x00, "LCM_BITMAP1"},
{{LCM_BITMAP2,       0x00000000}, 0x00, "LCM_BITMAP2"},
{{LCM_BITMAP3,       0x00000000}, 0x00, "LCM_BITMAP3"},
{{LCM_BITMAP4,       0x00000000}, 0x00, "LCM_BITMAP4"},
{{LCM_BITMAP5,       0x00000000}, 0x00, "LCM_BITMAP5"},
{{LCM_STEPCYCLE,     0x00000000}, 0x00, "LCM_STEPCYCLE"},
{{LCM_STAGE,         0x00000000}, 0x00, "LCM_STAGE"},
{{LCM_CMD_RUN,       0x00000000}, 0x00, "LCM_CMD_RUN"},
{{LCM_CMD_WDATA,     0x00000000}, 0x00, "LCM_CMD_WDATA"},
{{LCM_CMD_RDATA,     0x00000000}, 0x00, "LCM_CMD_RDATA"},
{{LCM_CMD_ADDR,      0x00000000}, 0x00, "LCM_CMD_ADDR"},
{{DHD1_INTERRUPT_CLR, 0x00000000}, 0x00, "DHD1_INTERRUPT_CLR"},
{{DHD1_INTERRUPT_RAW, 0x00000000}, 0x00, "DHD1_INTERRUPT_RAW"},
{{DHD1_DISPLAY_CFG,   0x00000000}, 0x00, "DHD1_DISPLAY_CFG"},
{{DHD1_DISPLAY_DELAY, 0x00000000}, 0x00, "DHD1_DISPLAY_DELAY"},
{{LCM1_CFG,           0x00000000}, 0x00, "LCM1_CFG"},
{{LCM1_BITMAP0,       0x00000000}, 0x00, "LCM1_BITMAP0"},
{{LCM1_BITMAP1,       0x00000000}, 0x00, "LCM1_BITMAP1"},
{{LCM1_BITMAP2,       0x00000000}, 0x00, "LCM1_BITMAP2"},
{{LCM1_BITMAP3,       0x00000000}, 0x00, "LCM1_BITMAP3"},
{{LCM1_BITMAP4,       0x00000000}, 0x00, "LCM1_BITMAP4"},
{{LCM1_BITMAP5,       0x00000000}, 0x00, "LCM1_BITMAP5"},
{{LCM1_STEPCYCLE,     0x00000000}, 0x00, "LCM1_STEPCYCLE"},
{{LCM1_STAGE,         0x00000000}, 0x00, "LCM1_STAGE"},
{{LCM1_CMD_RUN,       0x00000000}, 0x00, "LCM1_CMD_RUN"},
{{LCM1_CMD_WDATA,     0x00000000}, 0x00, "LCM1_CMD_WDATA"},
{{LCM1_CMD_RDATA,     0x00000000}, 0x00, "LCM1_CMD_RDATA"},
{{LCM1_CMD_ADDR,      0x00000000}, 0x00, "LCM1_CMD_ADDR"},
{{0xffffffff,        0xffffffff}, 0x00, ""}
};

reg_list_t s_lcm_mcu_reg_list = {0x00};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/

reg_list_t *_lcmMcuDrvGetRegList(void)
{
    s_lcm_mcu_reg_list.regNum = sizeof(s_lcm_mcu_reg)/sizeof(reg_t) - 1;
    s_lcm_mcu_reg_list.addrLen = LCM_MCU_REG_ADDR_BITS;
    s_lcm_mcu_reg_list.dataLen = LCM_MCU_DATA_BITS;
    s_lcm_mcu_reg_list.reg_des = s_lcm_mcu_reg;

    return &s_lcm_mcu_reg_list;
}

uint32_t _lcmGetRegDefault(lcm_mcu_drv_t *instance, uint32_t offset)
{
    int32_t rtn = 0;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(offset, regList);

    reg->data = instance->regRead(instance->baseAddr + offset);

    return rtn;
}

uint32_t lcmGetRegDefault(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;

    rtn = _lcmGetRegDefault(instance, DHD_DISPLAY_FROAMT);
    if (instance->lcd_id == 0) {
        rtn = _lcmGetRegDefault(instance, DHD_INTERRUPT_CLR);
        rtn = _lcmGetRegDefault(instance, DHD_INTERRUPT_RAW);
        rtn = _lcmGetRegDefault(instance, DHD_DISPLAY_CFG);
        rtn = _lcmGetRegDefault(instance, DHD_DISPLAY_DELAY);
        rtn = _lcmGetRegDefault(instance, LCM_CFG);
        rtn = _lcmGetRegDefault(instance, LCM_BITMAP0);
        rtn = _lcmGetRegDefault(instance, LCM_BITMAP1);
        rtn = _lcmGetRegDefault(instance, LCM_BITMAP2);
        rtn = _lcmGetRegDefault(instance, LCM_BITMAP3);
        rtn = _lcmGetRegDefault(instance, LCM_BITMAP4);
        rtn = _lcmGetRegDefault(instance, LCM_BITMAP5);
        rtn = _lcmGetRegDefault(instance, LCM_STEPCYCLE);
        rtn = _lcmGetRegDefault(instance, LCM_STAGE);
        rtn = _lcmGetRegDefault(instance, LCM_CMD_RUN);
        rtn = _lcmGetRegDefault(instance, LCM_CMD_WDATA);
        rtn = _lcmGetRegDefault(instance, LCM_CMD_RDATA);
        rtn = _lcmGetRegDefault(instance, LCM_CMD_ADDR);
    } else {
        rtn = _lcmGetRegDefault(instance, DHD1_INTERRUPT_CLR);
        rtn = _lcmGetRegDefault(instance, DHD1_INTERRUPT_RAW);
        rtn = _lcmGetRegDefault(instance, DHD1_DISPLAY_CFG);
        rtn = _lcmGetRegDefault(instance, DHD1_DISPLAY_DELAY);
        rtn = _lcmGetRegDefault(instance, LCM1_CFG);
        rtn = _lcmGetRegDefault(instance, LCM1_BITMAP0);
        rtn = _lcmGetRegDefault(instance, LCM1_BITMAP1);
        rtn = _lcmGetRegDefault(instance, LCM1_BITMAP2);
        rtn = _lcmGetRegDefault(instance, LCM1_BITMAP3);
        rtn = _lcmGetRegDefault(instance, LCM1_BITMAP4);
        rtn = _lcmGetRegDefault(instance, LCM1_BITMAP5);
        rtn = _lcmGetRegDefault(instance, LCM1_STEPCYCLE);
        rtn = _lcmGetRegDefault(instance, LCM1_STAGE);
        rtn = _lcmGetRegDefault(instance, LCM1_CMD_RUN);
        rtn = _lcmGetRegDefault(instance, LCM1_CMD_WDATA);
        rtn = _lcmGetRegDefault(instance, LCM1_CMD_RDATA);
        rtn = _lcmGetRegDefault(instance, LCM1_CMD_ADDR);
    }
    return rtn;
}

int32_t lcmSetMcuBitDepth(lcm_mcu_drv_t *instance, uint32_t bitDepth)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_BIT_DEPTH = bitDepth;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuRgbOrder(lcm_mcu_drv_t *instance, uint32_t rgbOrder)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_RGB_ORDER = rgbOrder;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuMode(lcm_mcu_drv_t *instance, uint32_t mcuMode)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_MCU_mode = mcuMode;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTimingDataPolarity(lcm_mcu_drv_t *instance, uint32_t pol)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_DATAEN_ACTIVE_LOW = pol;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTimingHsyncPolarity(lcm_mcu_drv_t *instance, uint32_t pol)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_HSYNC_ACTIVE_LOW = pol;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTimingVsyncPolarity(lcm_mcu_drv_t *instance, uint32_t pol)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_VSYNC_ACTIVE_LOW = pol;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTimingShutDownPolarity(lcm_mcu_drv_t *instance, uint32_t pol)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DPI_SHUTDN_ACTIVE_LOW = pol;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTimingColorRmPolarity(lcm_mcu_drv_t *instance, uint32_t pol)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DPI_COLORM_ACTIVE_LOW = pol;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTeEnable(lcm_mcu_drv_t *instance, uint32_t eb)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_TE_EN = eb;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuDitherEnable(lcm_mcu_drv_t *instance, uint32_t eb)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.Dithering_en = eb;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuBitSwapEnable(lcm_mcu_drv_t *instance, uint32_t eb)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.Bitswap_en = eb;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuClkLockDataEdge(lcm_mcu_drv_t *instance, uint32_t edge)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.USE_CLK_NEGEDGE = edge;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuTeEdge(lcm_mcu_drv_t *instance, uint32_t edge)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.USE_TE_NEGEDGE = edge;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuRgbMap(lcm_mcu_drv_t *instance, uint32_t map)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_CFG : DHD1_DISPLAY_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_CFG\n");
    regPtr = (union _lcm_display_cfg_tag *)&reg->data;
    regPtr->mBits.DISPLAY_RGB_MAP = map;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetDisplayDelay(lcm_mcu_drv_t *instance, uint32_t delay)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_DISPLAY_DELAY : DHD1_DISPLAY_DELAY;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_display_delay_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_DISPLAY_DELAY\n");
    regPtr = (union _lcm_display_delay_tag *)&reg->data;
    regPtr->mBits.DISPLAY_DELAY_CYCLE = delay;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetMcuIfType(lcm_mcu_drv_t *instance, uint32_t ifType)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_SYS_IF_TYPE = ifType;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetRgbIfType(lcm_mcu_drv_t *instance, uint32_t ifType)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_RGB_IF_TYPE = ifType;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetCsType(lcm_mcu_drv_t *instance, uint32_t csType)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_MCU_TIMING_TYPE = csType;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTeType(lcm_mcu_drv_t *instance, uint32_t teType)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_SW_USE_TE = teType;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetSendEndain(lcm_mcu_drv_t *instance, uint32_t endian)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_MCU_HIGHBYTE_FIRST = endian;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTrans3(lcm_mcu_drv_t *instance, uint32_t eb)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;
    regPtr->mBits.LCM_MCU_3_TRANS = eb;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

uint32_t lcmGetTransFifoReady(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CFG : LCM1_CFG;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cfg_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CFG\n");
    regPtr = (union _lcm_cfg_tag *)&reg->data;

    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);
    rtn = regPtr->mBits.LCM_WAIT_FIFO;

    EXIT:

    return rtn;
}

int32_t lcmSetBitMap(lcm_mcu_drv_t *instance, uint32_t* bitMap)
{
    int32_t rtn = 0;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = NULL;
    union _lcm_bitmap_tag *regPtr = NULL;

    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_BITMAP0 : LCM1_BITMAP0;
    reg = getRegPtr(reg_Offest, regList);
    EXIT_IF_NULL(reg, NULL, "LCM_BITMAP0\n");
    regPtr = (union _lcm_bitmap_tag *)&reg->data;
    regPtr->mBits.LCM_BIT_SRC_SEL0 = bitMap[0];
    regPtr->mBits.LCM_BIT_SRC_SEL1 = bitMap[1];
    regPtr->mBits.LCM_BIT_SRC_SEL2 = bitMap[2];
    regPtr->mBits.LCM_BIT_SRC_SEL3 = bitMap[3];
    regPtr->mBits.LCM_BIT_SRC_SEL4 = bitMap[4];
    regPtr->mBits.LCM_BIT_SRC_SEL5 = bitMap[5];
    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    reg_Offest = (instance->lcd_id == 0) ? LCM_BITMAP1 : LCM1_BITMAP1;
    reg = getRegPtr(reg_Offest, regList);
    EXIT_IF_NULL(reg, NULL, "LCM_BITMAP1\n");
    regPtr = (union _lcm_bitmap_tag *)&reg->data;
    regPtr->mBits.LCM_BIT_SRC_SEL0 = bitMap[6];
    regPtr->mBits.LCM_BIT_SRC_SEL1 = bitMap[7];
    regPtr->mBits.LCM_BIT_SRC_SEL2 = bitMap[8];
    regPtr->mBits.LCM_BIT_SRC_SEL3 = bitMap[9];
    regPtr->mBits.LCM_BIT_SRC_SEL4 = bitMap[10];
    regPtr->mBits.LCM_BIT_SRC_SEL5 = bitMap[11];
    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    reg_Offest = (instance->lcd_id == 0) ? LCM_BITMAP2 : LCM1_BITMAP2;
    reg = getRegPtr(reg_Offest, regList);
    EXIT_IF_NULL(reg, NULL, "LCM_BITMAP2\n");
    regPtr = (union _lcm_bitmap_tag *)&reg->data;
    regPtr->mBits.LCM_BIT_SRC_SEL0 = bitMap[12];
    regPtr->mBits.LCM_BIT_SRC_SEL1 = bitMap[13];
    regPtr->mBits.LCM_BIT_SRC_SEL2 = bitMap[14];
    regPtr->mBits.LCM_BIT_SRC_SEL3 = bitMap[15];
    regPtr->mBits.LCM_BIT_SRC_SEL4 = bitMap[16];
    regPtr->mBits.LCM_BIT_SRC_SEL5 = bitMap[17];
    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    reg_Offest = (instance->lcd_id == 0) ? LCM_BITMAP3 : LCM1_BITMAP3;
    reg = getRegPtr(reg_Offest, regList);
    EXIT_IF_NULL(reg, NULL, "LCM_BITMAP3\n");
    regPtr = (union _lcm_bitmap_tag *)&reg->data;
    regPtr->mBits.LCM_BIT_SRC_SEL0 = bitMap[18];
    regPtr->mBits.LCM_BIT_SRC_SEL1 = bitMap[19];
    regPtr->mBits.LCM_BIT_SRC_SEL2 = bitMap[20];
    regPtr->mBits.LCM_BIT_SRC_SEL3 = bitMap[21];
    regPtr->mBits.LCM_BIT_SRC_SEL4 = bitMap[22];
    regPtr->mBits.LCM_BIT_SRC_SEL5 = bitMap[23];
    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    reg_Offest = (instance->lcd_id == 0) ? LCM_BITMAP4 : LCM1_BITMAP4;
    reg = getRegPtr(reg_Offest, regList);
    EXIT_IF_NULL(reg, NULL, "LCM_BITMAP4\n");
    regPtr = (union _lcm_bitmap_tag *)&reg->data;
    regPtr->mBits.LCM_BIT_SRC_SEL0 = bitMap[24];
    regPtr->mBits.LCM_BIT_SRC_SEL1 = bitMap[25];
    regPtr->mBits.LCM_BIT_SRC_SEL2 = bitMap[26];
    regPtr->mBits.LCM_BIT_SRC_SEL3 = bitMap[27];
    regPtr->mBits.LCM_BIT_SRC_SEL4 = bitMap[28];
    regPtr->mBits.LCM_BIT_SRC_SEL5 = bitMap[29];
    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    reg_Offest = (instance->lcd_id == 0) ? LCM_BITMAP5 : LCM1_BITMAP5;
    reg = getRegPtr(reg_Offest, regList);
    EXIT_IF_NULL(reg, NULL, "LCM_BITMAP5\n");
    regPtr = (union _lcm_bitmap_tag *)&reg->data;
    regPtr->mBits.LCM_BIT_SRC_SEL0 = bitMap[30];
    regPtr->mBits.LCM_BIT_SRC_SEL1 = bitMap[31];
    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTimingWrSetup(lcm_mcu_drv_t *instance, uint32_t wrSetup)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STEPCYCLE : LCM1_STEPCYCLE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_step_cycle_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STEPCYCLE\n");

    regPtr = (union _lcm_step_cycle_tag *)&reg->data;
    regPtr->mBits.LCM_WSTEP0_CYCLE = wrSetup;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTimingWrHold(lcm_mcu_drv_t *instance, uint32_t wrHold)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STEPCYCLE : LCM1_STEPCYCLE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_step_cycle_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STEPCYCLE\n");

    regPtr = (union _lcm_step_cycle_tag *)&reg->data;
    regPtr->mBits.LCM_WSTEP1_CYCLE = wrHold;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTimingWrDestory(lcm_mcu_drv_t *instance, uint32_t wrDestory)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STEPCYCLE : LCM1_STEPCYCLE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_step_cycle_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STEPCYCLE\n");

    regPtr = (union _lcm_step_cycle_tag *)&reg->data;
    regPtr->mBits.LCM_WSTEP2_CYCLE = wrDestory;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTimingRdSetup(lcm_mcu_drv_t *instance, uint32_t rdSetup)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STEPCYCLE : LCM1_STEPCYCLE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_step_cycle_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STEPCYCLE\n");

    regPtr = (union _lcm_step_cycle_tag *)&reg->data;
    regPtr->mBits.LCM_RSTEP0_CYCLE = rdSetup;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTimingRdHold(lcm_mcu_drv_t *instance, uint32_t rdHold)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STEPCYCLE : LCM1_STEPCYCLE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_step_cycle_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STEPCYCLE\n");

    regPtr = (union _lcm_step_cycle_tag *)&reg->data;
    regPtr->mBits.LCM_RSTEP1_CYCLE = rdHold;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetTimingRdDestory(lcm_mcu_drv_t *instance, uint32_t rdDestory)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STEPCYCLE : LCM1_STEPCYCLE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_step_cycle_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STEPCYCLE\n");

    regPtr = (union _lcm_step_cycle_tag *)&reg->data;
    regPtr->mBits.LCM_RSTEP2_CYCLE = rdDestory;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetStatusSwitchTrigger(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STAGE : LCM1_STAGE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_stage_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STAGE\n");

    regPtr = (union _lcm_stage_tag *)&reg->data;
    regPtr->mBits.LCM_STAGE_SWITCH = 1;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);
    regPtr->mBits.LCM_STAGE_SWITCH = 0;

    EXIT:

    return rtn;
}

int32_t lcmSetStageStatus(lcm_mcu_drv_t *instance, uint32_t status)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_STAGE : LCM1_STAGE;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_stage_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STAGE\n");

    regPtr = (union _lcm_stage_tag *)&reg->data;
    regPtr->mBits.LCM_STAGE_SET = status;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

uint32_t lcmGetStageStatus(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_INTERRUPT_RAW : DHD1_INTERRUPT_RAW;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _VOU_CHN_INT_STS_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_STAGE\n");
    regPtr = (union _VOU_CHN_INT_STS_tag *)&reg->data;

    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);
    rtn = regPtr->mBits.lcm_stage_switch_done;

    EXIT:

    return rtn;
}

int32_t lcmClearCmdStatus(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_INTERRUPT_CLR : DHD1_INTERRUPT_CLR;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_mcu_int_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_INTERRUPT_CLR\n");

    regPtr = (union _lcm_mcu_int_tag *)&reg->data;
    regPtr->dwValue = 0;
    regPtr->mBits.LCM_CMD_DONE = 1;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

uint32_t lcmGetCmdStatus(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? DHD_INTERRUPT_RAW : DHD1_INTERRUPT_RAW;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_mcu_int_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "DHD_INTERRUPT_RAW\n");
    regPtr = (union _lcm_mcu_int_tag *)&reg->data;

    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);
    rtn = regPtr->mBits.LCM_CMD_DONE;

    if (1 == rtn ) {
        lcmClearCmdStatus(instance);
    }

    EXIT:

    return rtn;
}

int32_t lcmSetCmdRun(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->mBits.LCM_CMD_START = 1;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    regPtr->mBits.LCM_CMD_START = 0;

    EXIT:

    return rtn;
}

int32_t lcmSetCmdFifoClr(lcm_mcu_drv_t *instance)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->mBits.LCM_CMD_FIFO_CLR = 1;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetCmdRW(lcm_mcu_drv_t *instance, uint32_t rw)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->mBits.LCM_CMD_RW = rw;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetCmdDummyNum(lcm_mcu_drv_t *instance, uint32_t num)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->mBits.LCM_CMD_DUMMY_NUM = num;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetCmdDataNum(lcm_mcu_drv_t *instance, uint32_t num)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->mBits.LCM_CMD_DATA_NUM = num;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

int32_t lcmSetCmdAddrNum(lcm_mcu_drv_t *instance, uint32_t num)
{
    int32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");
    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->mBits.LCM_CMD_ADDR_NUM = num;

    instance->regWrite(instance->baseAddr + reg_Offest, regPtr->dwValue);

    EXIT:

    return rtn;
}

uint32_t lcmGetCmdWfifoDepth(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);
    rtn = regPtr->mBits.LCM_CMD_WFIFO_DEPTH;

    EXIT:

    return rtn;
}

uint32_t lcmGetCmdWfifoFull(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);

    rtn = regPtr->mBits.LCM_CMD_WFIFO_FULL;

    EXIT:

    return rtn;
}

uint32_t lcmGetCmdRfifoDepth(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);

    rtn = regPtr->mBits.LCM_CMD_RFIFO_DEPTH;

    EXIT:

    return rtn;
}

uint32_t lcmGetCmdRfifoEmpty(lcm_mcu_drv_t *instance)
{
    uint32_t rtn = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RUN : LCM1_CMD_RUN;
    reg_list_t *regList = _lcmMcuDrvGetRegList();
    reg_t *reg = getRegPtr(reg_Offest, regList);
    union _lcm_cmd_run_tag *regPtr = NULL;

    EXIT_IF_NULL(reg, NULL, "LCM_CMD_RUN\n");

    regPtr = (union _lcm_cmd_run_tag *)&reg->data;
    regPtr->dwValue = instance->regRead(instance->baseAddr + reg_Offest);

    rtn = regPtr->mBits.LCM_CMD_RFIFO_EMPTY;

    EXIT:

    return rtn;
}

uint32_t lcmSetWData(lcm_mcu_drv_t *instance, uint32_t *data, uint32_t num)
{
    int32_t rtn = 0;
    uint32_t i = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_WDATA : LCM1_CMD_WDATA;

    for (i = 0; i < num; i++) {
        instance->regWrite(instance->baseAddr + reg_Offest, data[i]);
    }

    return rtn;
}

uint32_t lcmGetRData(lcm_mcu_drv_t *instance, uint32_t *data, uint32_t num)
{
    int32_t rtn = 0;
    uint32_t i = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_RDATA : LCM1_CMD_RDATA;

    for (i = 0; i < num; i++) {
         data[i] = instance->regRead(instance->baseAddr + reg_Offest);
    }

    return rtn;
}

uint32_t lcmSetWCmd(lcm_mcu_drv_t *instance, uint32_t *data, uint32_t num)
{
    int32_t rtn = 0;
    uint32_t i = 0;
    ADDR reg_Offest = (instance->lcd_id == 0) ? LCM_CMD_ADDR : LCM1_CMD_ADDR;

    for (i = 0; i < num; i++) {
        instance->regWrite(instance->baseAddr + reg_Offest, data[i]);
    }

    return rtn;
}

/**----------------------------------------------------------------------------*
*           Compiler Flag                       *
**----------------------------------------------------------------------------*/
#ifdef     __cplusplus
 }
#endif
/**---------------------------------------------------------------------------*/

