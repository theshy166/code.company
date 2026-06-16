#ifndef _DSI_PHY_V01_DRV_PARAM_H_
#define _DSI_PHY_V01_DRV_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef enum {
    DSI_PHY_POWER_ON_SOFTWARE = 0,
    DSI_PHY_POWER_ON_CONTROLLER,
    DSI_PHY_POWER_ON_SEL_MAX
}dsi_phy_power_on_sel_e;

typedef enum {
    DSI_PHY_LP_CD_THR_VOL_175_425 = 1,
    DSI_PHY_LP_CD_THR_VOL_200_450 = 2,
    DSI_PHY_LP_CD_THR_VOL_225_475 = 4,
    DSI_PHY_LP_CD_THR_VOL_250_500 = 8,
    DSI_PHY_LP_CD_IN_THR_MAX
}dsi_phy_lp_cd_thr_e;

typedef enum {
    DSI_PHY_LP_RX_THR_VOL_275_855 = 1,
    DSI_PHY_LP_RX_THR_VOL_300_880 = 2,
    DSI_PHY_LP_RX_THR_VOL_325_905 = 4,
    DSI_PHY_LP_RX_THR_VOL_350_930 = 8,
    DSI_PHY_LP_RX_THR_MAX
}dsi_phy_lp_rx_thr_e;

typedef enum {
    DSI_PHY_HS_TX_VOL_237 = 0,
    DSI_PHY_HS_TX_VOL_231,
    DSI_PHY_HS_TX_VOL_225,
    DSI_PHY_HS_TX_VOL_217,
    DSI_PHY_HS_TX_VOL_212,
    DSI_PHY_HS_TX_VOL_206,
    DSI_PHY_HS_TX_VOL_200,
    DSI_PHY_HS_TX_VOL_193,
    DSI_PHY_HS_TX_VOL_MAX
}dsi_phy_hs_tx_vol_e;

typedef enum {
    DSI_PHY_FT_LP11_EN_LPTX = 1,
    DSI_PHY_FT_LP10_EN_LPTX,
    DSI_PHY_FT_LP01_EN_LPTX,
    DSI_PHY_FT_LP00_EN_LPTX,
    DSI_PHY_FT_HSTX1_EN_HSTX,
    DSI_PHY_FT_HSTX0_EN_HSTX,
    DSI_PHY_FT_NORMAL,
    DSI_PHY_FT_SEL_MAX
}dsi_phy_ft_sel_e;

typedef enum {
    DSI_PHY_ATEST_TX_LDO_REF = 0,
    DSI_PHY_ATEST_TX_PRE_LDO_REF,
    DSI_PHY_ATEST_TX_PRE_LDO_OUT,
    DSI_PHY_ATEST_TX_LDO_OUT,
}dsi_phy_atest_sel_e;


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

