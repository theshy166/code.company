
#include "include/mol_lcm_local.h"

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

/*mipi lcm*/
extern lcm_cfg_t s_lcm_mipi_ek79007_info;
extern lcm_cfg_t s_lcm_mipi_st7701s_info;
extern lcm_cfg_t qua_lcm_mipi_jd9365d_jlm101b021_info;

/*rgb lcm*/
extern lcm_cfg_t s_lcm_rgb_at070tn94_info;
extern lcm_cfg_t s_lcm_rgb18_at070tn94_info;
extern lcm_cfg_t qua_lcm_rgb_ek79001h_tg78550b3_24bits_info;

/*mcu lcm*/
extern lcm_cfg_t s_lcm_mcu_st7796s_info;
extern lcm_cfg_t s_lcm_mcu16_st7796s_info;

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
lcm_cfg_t *lcm_main_tab[] = {
    /*qua lcm*/
#ifdef CONFIG_QUA_LOGO_MIPI_JD9365D
    &qua_lcm_mipi_jd9365d_jlm101b021_info,
#endif

#ifdef CONFIG_QUA_LOGO_RGB_EK79001H
    &qua_lcm_rgb_ek79001h_tg78550b3_24bits_info,
#endif

#ifdef CONFIG_QUA_LOGO_MORE
    /*mipi lcm*/
    &s_lcm_mipi_ek79007_info,
    &s_lcm_mipi_st7701s_info,

    /*rgb lcm*/

    /*mcu lcm*/
    &s_lcm_mcu_st7796s_info,
    &s_lcm_mcu16_st7796s_info,

    /* ------ non id read ------*/
    /*rgb lcm*/
    &s_lcm_rgb_at070tn94_info,
    &s_lcm_rgb18_at070tn94_info,
#endif
    /*end lcm*/
    0,
};

lcm_cfg_t *lcm_sub_tab[] = {
    /*qua lcm*/
#ifdef CONFIG_QUA_LOGO_MIPI_JD9365D
    &qua_lcm_mipi_jd9365d_jlm101b021_info,
#endif

#ifdef CONFIG_QUA_LOGO_RGB_EK79001H
    &qua_lcm_rgb_ek79001h_tg78550b3_24bits_info,
#endif

#ifdef CONFIG_QUA_LOGO_MORE
    /*mipi lcm*/
    &s_lcm_mipi_st7701s_info,

    /*rgb lcm*/

    /*mcu lcm*/
    &s_lcm_mcu_st7796s_info,
    &s_lcm_mcu16_st7796s_info,

    /* ------ non id read ------*/
    /*rgb lcm*/
    &s_lcm_rgb_at070tn94_info,
    &s_lcm_rgb18_at070tn94_info,
#endif
    /*end lcm*/
    0,
};

lcm_cfg_t **s_lcm_tab[] = {
    lcm_main_tab,
    lcm_sub_tab,
    0,
};

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
lcm_cfg_t **get_lcm_cfg_tab(uint32_t lcm_id)
{
    return s_lcm_tab[lcm_id];
}

 /**----------------------------------------------------------------------------*
 **                         Compiler Flag                                      **
 **----------------------------------------------------------------------------*/
#ifdef __cplusplus
 }
#endif

