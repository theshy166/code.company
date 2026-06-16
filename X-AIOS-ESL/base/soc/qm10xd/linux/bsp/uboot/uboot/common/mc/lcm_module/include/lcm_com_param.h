#ifndef _LCM_COM_PARAM_H_
#define _LCM_COM_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef struct {
    int32_t hor_sync_active;
    int32_t hor_back_porch;
    int32_t hor_active_pixel;
    int32_t hor_front_porch;

    int32_t ver_sync_active;
    int32_t ver_back_porch;
    int32_t ver_active_line;
    int32_t ver_front_porch;
}lcm_display_sync_info_t;

typedef struct {
    uint32_t mode;
    uint32_t sel; // 0:24M  1:120M 2:150M 3:200M
    uint32_t div; // n+1
}vou_clk_t;

typedef struct {
    uint32_t mode;
    uint32_t pll_doubler;
    uint32_t pll_div_s;
    uint32_t pll_n;
    uint32_t pll_kint;
    uint32_t pll_nint;
    uint32_t pixelclk_div; // post div
    uint32_t pll_pdiv;
    uint32_t dhd_div;
    uint32_t data_rate; // data_rate per lane
}mif_clk_t;

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

