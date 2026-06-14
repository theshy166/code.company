#include "../../lcm_module/include/data_type.h"
#include "../reg/reg_pool.h"
#include "../reg/reg.h"
#include "dsi_phy_v01_reg_def.h"

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
reg_t s_dsi_phy_reg_default[] = {
    {CLANE0_PARA0     , 0x00000000},
    {CLANE0_PARA1     , 0x00000000},
    {CLANE0_PARA2     , 0x00000000},
    {CLANE0_PARA3     , 0x00000000},
    {CLANE1_PARA0     , 0x00000000},
    {CLANE1_PARA1     , 0x00000000},
    {CLANE1_PARA2     , 0x00000000},
    {CLANE1_PARA3     , 0x00000000},
    {DLANE0_PARA0     , 0x00000000},
    {DLANE0_PARA1     , 0x00000000},
    {DLANE0_PARA2     , 0x00000000},
    {DLANE0_PARA3     , 0x00000000},
    {DLANE0_PARA4     , 0x00000000},
    {DLANE1_PARA0     , 0x00000000},
    {DLANE1_PARA1     , 0x00000000},
    {DLANE1_PARA2     , 0x00000000},
    {DLANE1_PARA3     , 0x00000000},
    {DLANE2_PARA0     , 0x00000000},
    {DLANE2_PARA1     , 0x00000000},
    {DLANE2_PARA2     , 0x00000000},
    {DLANE2_PARA3     , 0x00000000},
    {DLANE2_PARA4     , 0x00000000},
    {DLANE3_PARA0     , 0x00000000},
    {DLANE3_PARA1     , 0x00000000},
    {DLANE3_PARA2     , 0x00000000},
    {DLANE3_PARA3     , 0x00000000},
    {COMMON_PARA0     , 0x00000000},
    {MISC_PARA0       , 0x00000000},
    {MISC_PARA1       , 0x00000000},
    {MODE_CFG         , 0x00000000},
    {CLANE0_PARA4     , 0x00000000},
    {CLANE_PARA4      , 0x00000000},
    {CLANE_PARA5      , 0x00000000},
    {PCS_PHY_SYNC     , 0x00000000},
    {DESKEW_RCAL_MISC0, 0x00000000},
    {DESKEW_RCAL_MISC1, 0x00000000},
    {DESKEW_RCAL_MISC2, 0x00000000},
    {EXT_LOOP_CFG0    , 0x00000000},
    {EXT_LOOP_CFG1    , 0x00000000},
    {CL_DESKEW        , 0x00000000},
    {PHY_PARA0        , 0x00000000},
    {PHY_PARA1        , 0x00000000},
    {PHY_PARA2        , 0x00000000},
    {PHY_PARA3        , 0x00000000},
    {PHY_PARA4        , 0x00000000},
    {PHY_PARA5        , 0x00000000},
    {PHY_PARA6        , 0x00000000},
    {PHY_PARA7        , 0x00000000},
    {PHY_PARA8        , 0x00000000},
    {PHY_PARA9        , 0x00000000},
    {PHY_PARA10       , 0x00000000},
    {PHY_PARA11       , 0x00000000},
    {PHY_PARA12       , 0x00000000},
    {PHY_PARA13       , 0x00000000},
    {PHY_PARA14       , 0x00000000},
    {PHY_PARA15       , 0x00000000},
    {PHY_PARA16       , 0x00000000},
    {PRBS_INFO0       , 0x00000000},
    {SSC_CTRL0        , 0x00000000},
    {SSC_CTRL1        , 0x00000000},
    {PHY_RDY_CTRL0    , 0x00000000},
    {PHY_RDY_CTRL1    , 0x00000000},
    {PHY_RDY_INFO0    , 0x00000000},
    {PHY_RDY_INFO1    , 0x00000000},
    {PRBS_INFO1       , 0x00000000},
    {PRBS_INFO2       , 0x00000000},
    {PRBS_INFO3       , 0x00000000},
    {PRBS_INFO4       , 0x00000000},
    {PRBS_INFO5       , 0x00000000},
    {PRBS_INFO6       , 0x00000000},
    {PRBS_INFO7       , 0x00000000},
    {PRBS_INFO8       , 0x00000000},
    {DPHY_DEBUG_CFG0  , 0x00000000},
    {DPHY_DEBUG_CFGl  , 0x00000000},
    {DPHY_DEBUG_CFG2  , 0x00000000},
    {DPHY_DEBUG_CFG3  , 0x00000000},
    {DPHY_DEBUG_CFG4  , 0x00000000},
    {DPHY_DEBUG_CFG5  , 0x00000000},
    {DPHY_DEBUG_CFG6  , 0x00000000},
    {DPHY_DEBUG_CFG7  , 0x00000000},
    {DPHY_DEBUG_CFG8  , 0x00000000},
    {DPHY_DEBUG_CFG9  , 0x00000000},
    {DPHY_STATUS_INFO0, 0x00000000},
    {DPHY_STATUS_INFO1, 0x00000000},
    {DPHY_DEBUG0      , 0x00000000},
    {DPHY_DEBUGl      , 0x00000000},
    {DPHY_DEBUG2      , 0x00000000},
    {DPHY_DEBUG3      , 0x00000000},
    {DPHY_DEBUG4      , 0x00000000},
    {DPHY_DEBUGS      , 0x00000000},
    {DPHY_DEBUG6      , 0x00000000},
    {DPHY_DEBUG7      , 0x00000000},
    {DPHY_DEBUG8      , 0x00000000},
    {DPHY_DEBUG9      , 0x00000000},
    {DPHY_DEBUG10     , 0x00000000},
    {DPHY_DEBUG11     , 0x00000000},
    {DPHY_DEBUG12     , 0x00000000},
    {DPHY_DEBUG13     , 0x00000000},
    {DPHY_DEBUG14     , 0x00000000},
    {DPHY_DEBUG15     , 0x00000000},
    {DPHY_DEBUG16     , 0x00000000},
    {DPHY_DEBUG17     , 0x00000000},
    {DPHY_DEBUG18     , 0x00000000},
    {DPHY_DEBUG19     , 0x00000000},
    {DPHY_DEBUG20     , 0x00000000},
    {DPHY_DEBUG21     , 0x00000000},
    {DPHY_DEBUG_CFG10 , 0x00000000},
    {DPHY_DEBUG_CFG11 , 0x00000000},
    {DPHY_DEBUG_CFG12 , 0x00000000},
    {DPHY_DEBUG_CFG13 , 0x00000000},
    {DPHY_DEBUG_CFG14 , 0x00000000},
    {DPHY_DEBUG_CFG15 , 0x00000000},
    {DPHY_DEBUG_CFG16 , 0x00000000},
    {DPHY_DEBUG_CFG17 , 0x00000000},
    {DPHY_DEBUG_CFG18 , 0x00000000},
    {DPHY_DEBUG_CFG19 , 0x00000000},
    {DPHY_DEBUG_CFG20 , 0x00000000},
    {DPHY_DEBUG_CFG21 , 0x00000000},
    {DPHY_DEBUG_CFG22 , 0x00000000},
    {DPHY_DEBUG_CFG23 , 0x00000000},
    {DPHY_DEBUG_CFG24 , 0x00000000},
    {DPHY_DEBUG_CFG25 , 0x00000000},
    {DPHY_DEBUG_CFG26 , 0x00000000},
    {DPHY_DEBUG_CFG27 , 0x00000000},
    {DPHY_DEBUG_CFG28 , 0x00000000},
    {DPHY_DEBUG_CFG29 , 0x00000000},
    {DPHY_DEBUG_CFG30 , 0x00000000},
    {DPHY_DEBUG_CFG31 , 0x00000000},
    {DPHY_DEBUG_CFG32 , 0x00000000},
    {DPHY_STATUS_INFO2, 0x00000000},
    {DPHY_STATUS_INFO3, 0x00000000},
    {MODULE_INVALID   , MODULE_INVALID}
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
void *dsi_phy_get_reg_default_tab(void)
{
    return (void *)&s_dsi_phy_reg_default;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

