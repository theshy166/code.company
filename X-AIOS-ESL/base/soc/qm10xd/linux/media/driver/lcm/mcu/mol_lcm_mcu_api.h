#ifndef _MOL_LCM_MCU_API_H_
#define _MOL_LCM_MCU_API_H_

#include "data_type.h"
#include "mol_lcm_mcu_drv.h"
#include "vou_clk_v01_drv.h"
#include "pin_lcd_v01_drv.h"
#include "dsi_phy_v0_pll_calc_param.h"
#include "mol_lcm_mcu.h"

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

typedef struct
{
    ADDR rst_base_addr;
    ADDR clk_base_addr;
    ADDR vou_sel_addr;
    ADDR vou_base_addr;
    ADDR mif_base_addr;
    ADDR pin_base_addr;
    uint32_t lcm_id;

    void *lcm_cfg;
    void* mcuSpec;
}lcm_mcu_aip_t;

/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#define LCM_MCU_HANDLE_NUM 2
#define LCM_MCU_STATUS_TIMEOUT 5
#define LCM_MCU_OPS_TIME 10

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct
{
    uint32_t flag;

    ADDR rst_base_addr;
    ADDR clk_base_addr;
    ADDR vou_sel_addr;
    ADDR mif_base_addr;
    ADDR vou_base_addr;
    ADDR pin_base_addr;

    uint32_t dhd_id;
    lcm_pin_type_e lcm_pin_type;
    uint32_t host_sync_div;
    lcm_mcu_clk_t clk;

    vou_clk_calc_in_t vouClkIn;
    vou_clk_calc_out_t vouClkOut;

    dsi_phy_pll_calc_in_t mifClkIn;
    dsi_phy_pll_calc_out_t mifClkOut;

    lcm_clk_on_in_t lcmClkOnIn;
    lcm_clk_on_out_t lcmClkOnOut;
    lcm_clk_off_in_t lcmClkOffIn;
    lcm_clk_off_out_t lcmClkOffOut;

    dsi_phy_power_on_in_t phyPowerOnIn;
    dsi_phy_power_on_out_t phyPowerOnOut;

    dsi_phy_pll_on_in_t phyPllCfgIn;
    dsi_phy_pll_on_out_t phyPllCfgOut;

    lcm_mcu_drv_t lcmMcuDrv;
    uint32_t funDebug;
    uint32_t paramDebug;
}lcm_mcu_api_local_t;

typedef struct
{
    lcm_mcu_api_local_t handle[LCM_MCU_HANDLE_NUM];
}lcm_mcu_api_context_t;

void *lcmMcuApiInit(lcm_mcu_aip_t *inParam);
int32_t lcmMcuApiDeInit(void *handle);
int32_t lcmMcuApiWriteCmd(void *handle, uint32_t cmd);
int32_t lcmMcuApiWriteData(void *handle, uint32_t data);
int32_t lcmMcuApiWriteBurstData(void *handle, uint32_t *data, uint32_t num);
int32_t lcmMcuApiReadData(void *handle, uint32_t *data);
int32_t lcmMcuApiReadRegData(void *handle, uint32_t *data, uint32_t num);
int32_t lcmMcuApiReadBurstData(void *handle, uint32_t *data, uint32_t num);
int32_t lcmMcuApiPushTable(void *handle, void *setting, uint32_t num);
int32_t lcmMcuApiSwitchShow(void *handle);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

