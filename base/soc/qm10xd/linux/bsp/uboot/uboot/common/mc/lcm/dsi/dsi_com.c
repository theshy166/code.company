#include "../../lcm_module/include/data_type.h"
#include "../../lcm_module/include/dsi_com_param.h"

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

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
dsi_phy_work_mode_e get_phy_work_mode(phy_core_e phyId, dsi_phy_lane_num_e laneNum)
{
    dsi_phy_work_mode_e phyWorkMode = DSI_PHY_WORK_NONE;

    if (((dsi_phy_lane_1 > laneNum) && (dsi_phy_lane_4 < laneNum))
        || ((PHY1 == phyId) && (dsi_phy_lane_2 < laneNum))) {
        phyWorkMode = DSI_PHY_WORK_NONE;
        goto EXIT;
    }

    if (dsi_phy_lane_3 > laneNum) {
        if (PHY0 == phyId) {
            phyWorkMode = DSI_PHY0_WORK_1C2D;
        }else {
            phyWorkMode = DSI_PHY1_WORK_1C2D;
        }
    } else if (dsi_phy_lane_num_max > laneNum){
        phyWorkMode = DSI_PHY0_WORK_1C4D;
    }

    EXIT:

    return phyWorkMode;
}


dsi_phy_work_mode_e phy_work_mode_checker(phy_core_e phyId, dsi_phy_lane_num_e laneNum)
{
    dsi_phy_work_mode_e phyWorkMode = DSI_PHY_WORK_NONE;

    if (((dsi_phy_lane_1 > laneNum) && (dsi_phy_lane_4 < laneNum))
        || ((PHY1 == phyId) && (dsi_phy_lane_2 < laneNum))) {
        phyWorkMode = DSI_PHY_WORK_NONE;
        goto EXIT;
    }

    if (dsi_phy_lane_3 > laneNum) {
        if (PHY0 == phyId) {
            phyWorkMode = DSI_PHY0_WORK_1C2D;
        }else {
            phyWorkMode = DSI_PHY1_WORK_1C2D;
        }
    } else if (dsi_phy_lane_num_max > laneNum){
        phyWorkMode = DSI_PHY0_WORK_1C4D;
    }

    EXIT:

    return phyWorkMode;
}

 /**----------------------------------------------------------------------------*
 **                         Compiler Flag                                      **
 **----------------------------------------------------------------------------*/
#ifdef __cplusplus
 }
#endif
