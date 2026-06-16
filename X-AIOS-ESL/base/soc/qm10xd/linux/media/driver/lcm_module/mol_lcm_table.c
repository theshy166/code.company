
#include "mol_lcm_local.h"

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
#if defined(__KERNEL__)
#define OS_LINUX 1
#else
#define OS_RTT 1
#endif

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/*mipi lcm*/
extern lcm_cfg_t s_lcm_mipi_ek79007_info;
extern lcm_cfg_t s_lcm_mipi_st7701s_info;
extern lcm_cfg_t s_lcm_mipi_sample_1920_info;
extern lcm_cfg_t s_lcm_mipi_sample_1280_info;
extern lcm_cfg_t s_lcm_mipi_qfh24004_info;

/*rgb lcm*/
extern lcm_cfg_t s_lcm_rgb_at070tn94_info;
extern lcm_cfg_t s_lcm_rgb18_at070tn94_info;
extern lcm_cfg_t s_lcm_rgb_sample_640_info;
extern lcm_cfg_t qua_lcm_rgb_ek79001h_tg78550b3_info;

/*mcu lcm*/
extern lcm_cfg_t s_lcm_mcu_st7796s_info;
extern lcm_cfg_t s_lcm_mcu16_st7796s_info;
extern lcm_cfg_t qua_lcm_mipi_jd9365d_jlm101b021_info;
extern lcm_cfg_t qua_lcm_rgb_ek79001h_tg78550b3_24bits_info;

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
lcm_cfg_t *lcm_main_tab[] = {
    // &s_lcm_mipi_ek79007_info,
#if 0
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

    // only for maxloading
    &s_lcm_mipi_sample_1920_info,
    &s_lcm_mipi_sample_1280_info,

    /* new */
    &s_lcm_mipi_qfh24004_info,
#endif
    /* qua lcm */
    &qua_lcm_mipi_jd9365d_jlm101b021_info,
    &qua_lcm_rgb_ek79001h_tg78550b3_info,
    &s_lcm_rgb18_at070tn94_info,
    &s_lcm_rgb_at070tn94_info,
    &qua_lcm_rgb_ek79001h_tg78550b3_24bits_info,

    /*end lcm*/
    0,
};

lcm_cfg_t *lcm_sub_tab[] = {
#if 0
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

    // only for maxloading
    &s_lcm_rgb_sample_640_info,
    &s_lcm_mipi_sample_1280_info,
#endif
    &qua_lcm_mipi_jd9365d_jlm101b021_info,
    &qua_lcm_rgb_ek79001h_tg78550b3_info,
    &s_lcm_rgb18_at070tn94_info,
    &s_lcm_rgb_at070tn94_info,
    &qua_lcm_rgb_ek79001h_tg78550b3_24bits_info,

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
#if defined(OS_LINUX)
EXPORT_SYMBOL(get_lcm_cfg_tab);
#elif defined(OS_RTT)
RTM_EXPORT(get_lcm_cfg_tab);
#endif

 /**----------------------------------------------------------------------------*
 **                         Compiler Flag                                      **
 **----------------------------------------------------------------------------*/
#ifdef __cplusplus
 }
#endif

