#include "data_type.h"
#include "reg_pool.h"
#include "reg.h"
#include "dsi_host_v01_reg_def.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
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
reg_t s_dsi_host_reg_default[] = {
{DSI_HOST_VERSION              , 0x3133302A},
{DSI_HOST_PWR_UP               , 0x00000000},
{DSI_HOST_CLKMGR_CFG           , 0x00000000},
{DSI_HOST_DPI_VCID             , 0x00000000},
{DSI_HOST_DPI_COLOR_CODING     , 0x00000000},
{DSI_HOST_DPI_CFG_POL          , 0x00000000},
{DSI_HOST_DPI_LP_CMD_TIM       , 0x00000000},
{DSI_HOST_DBI_VCID             , 0x00000000},
{DSI_HOST_DBI_CFG              , 0x00000000},
{DSI_HOST_DBI_PARTITIONING_EN  , 0x00000000},
{DSI_HOST_DBI_CMDSIZE          , 0x00000000},
{DSI_HOST_PCKHDL_CFG           , 0x00000000},
{DSI_HOST_GEN_VCID             , 0x00000000},
{DSI_HOST_MODE_CFG             , 0x00000001},
{DSI_HOST_VID_MODE_CFG         , 0x00000000},
{DSI_HOST_VID_PKT_SIZE         , 0x00000000},
{DSI_HOST_VID_NUM_CHUNKS       , 0x00000000},
{DSI_HOST_VID_NULL_SIZE        , 0x00000000},
{DSI_HOST_VID_HSA_TIME         , 0x00000000},
{DSI_HOST_VID_HBP_TIME         , 0x00000000},
{DSI_HOST_VID_HLINE_TIME       , 0x00000000},
{DSI_HOST_VID_VSA_LINES        , 0x00000000},
{DSI_HOST_VID_VBP_LINES        , 0x00000000},
{DSI_HOST_VID_VFP_LINES        , 0x00000000},
{DSI_HOST_VID_VACTIVE_LINES    , 0x00000000},
{DSI_HOST_EDPI_CMD_SIZE        , 0x00000000},
{DSI_HOST_CMD_MODE_CFG         , 0x00000000},
{DSI_HOST_GEN_HDR              , 0x00000000},
{DSI_HOST_GEN_PLD_DATA         , 0x00000000},
{DSI_HOST_CMD_PKT_STATUS       , 0x00001515},
{DSI_HOST_TO_CNT_CFG           , 0x00000000},
{DSI_HOST_HS_RD_TO_CNT         , 0x00000000},
{DSI_HOST_LP_RD_TO_CNT         , 0x00000000},
{DSI_HOST_HS_WR_TO_CNT         , 0x00000000},
{DSI_HOST_LP_WR_TO_CNT         , 0x00000000},
{DSI_HOST_BTA_TO_CNT           , 0x00000000},
{DSI_HOST_SDF_3D               , 0x00000000},
{DSI_HOST_LPCLK_CTRL           , 0x00000000},
{DSI_HOST_PHY_TMR_LPCLK_CFG    , 0x00000000},
{DSI_HOST_PHY_TMR_CFG          , 0x00000000},
{DSI_HOST_PHY_RSTZ             , 0x00000000},
{DSI_HOST_PHY_IF_CFG           , 0x00000000},
{DSI_HOST_PHY_ULPS_CTRL        , 0x00000000},
{DSI_HOST_PHY_TX_TRIGGERS      , 0x00000000},
{DSI_HOST_PHY_STATUS           , 0x00001528},
{DSI_HOST_PHY_TST_CTRL0        , 0x00000001},
{DSI_HOST_PHY_TST_CTRL1        , 0x00000000},
{DSI_HOST_INT_ST0              , 0x00000000},
{DSI_HOST_INT_ST1              , 0x00000000},
{DSI_HOST_INT_MSK0             , 0x00000000},
{DSI_HOST_INT_MSK1             , 0x00000000},
{DSI_HOST_PHY_CAL              , 0x00000000},
{DSI_HOST_INT_FORCE0           , 0x00000000},
{DSI_HOST_INT_FORCE1           , 0x00000000},
{DSI_HOST_DSC_PARAMETER        , 0x00000000},
{DSI_HOST_TMR_RD_CFG           , 0x00000000},
{DSI_HOST_VID_SHADOW_CTRL      , 0x00000000},
{DSI_HOST_DPI_VCID_ACT         , 0x00000000},
{DSI_HOST_DPI_COLOR_CODING_ACT , 0x00000000},
{DSI_HOST_DPI_LP_CMD_TIM_ACT   , 0x00000000},
{DSI_HOST_VID_MODE_CFG_ACT     , 0x00000000},
{DSI_HOST_VID_PKT_SIZE_ACT     , 0x00000000},
{DSI_HOST_VID_NUM_CHUNKS_ACT   , 0x00000000},
{DSI_HOST_VID_NULL_SIZE_ACT    , 0x00000000},
{DSI_HOST_VID_HSA_TIME_ACT     , 0x00000000},
{DSI_HOST_VID_HBP_TIME_ACT     , 0x00000000},
{DSI_HOST_VID_HLINE_TIME_ACT   , 0x00000000},
{DSI_HOST_VID_VSA_LINES_ACT    , 0x00000000},
{DSI_HOST_VID_VBP_LINES_ACT    , 0x00000000},
{DSI_HOST_VID_VFP_LINES_ACT    , 0x00000000},
{DSI_HOST_VID_VACTIVE_LINES_ACT, 0x00000000},
{DSI_HOST_SDF_3D_ACT           , 0x00000000},
{MODULE_INVALID      , MODULE_INVALID}
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
void *dsi_host_get_reg_default_tab(void)
{
    return (void *)&s_dsi_host_reg_default;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

