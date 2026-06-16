#include "dsi_host_v01_drv_param.h"
#include "dsi_pixel_info_param.h"

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
#define DSI_PIXEL_DEBUG_LOG 0
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

int32_t dsi_pixel_clk_get(dsi_get_pixel_clk_in_t *inParam, dsi_get_pixel_clk_out_t *outParam)
{
    int32_t rtn = 0;
    lcm_display_sync_info_t *dp_sync = inParam->dp_sync;
    uint32_t Htotal = 0x00;
    uint32_t Vtotal = 0x00;

    Htotal = dp_sync->hor_back_porch
                + dp_sync->hor_sync_active
                + dp_sync->hor_front_porch
                + dp_sync->hor_active_pixel;

    Vtotal = dp_sync->ver_sync_active
                + dp_sync->ver_back_porch
                + dp_sync->ver_front_porch
                + dp_sync->ver_active_line;


    outParam->pixel_clk = Htotal * Vtotal * inParam->fps;

    #if DSI_PIXEL_DEBUG_LOG
    printf("Htotal:%d, Vtotal:%d, fps:%d, pixel_clk:%d.%dM\n", Htotal, Vtotal, inParam->fps,
                                                            (outParam->pixel_clk)/1000000, (outParam->pixel_clk)%1000000);
    #endif

    return rtn;
}

int32_t dsi_bits_per_pixel_get(uint32_t format, uint32_t *bpp)
{
    int32_t rtn = 0;
    uint32_t data_bits[][3] = {
        {DSI_DPI_RGB565,     22, 16},
        {DSI_DPI_RGB565_LOW, 22, 16},
        {DSI_DPI_RGB565_MID, 22, 16},
        {DSI_DPI_RGB666,     24, 18},
        {DSI_DPI_RGB666_LOW, 24, 18},
        {DSI_DPI_RGB888,     32, 24},
        //{DSI_DPI_YUVA_422,   28, 20},
        //{DSI_DPI_YUVC_422,   32, 24},
        //{DSI_DPI_YUV8_422,   22, 16},
        //{DSI_DPI_RGBAAA,     40, 30},
        //{DSI_DPI_RGBCCC,     48, 36},
        //{DSI_DPI_YUV8_420,   16, 12},
    };

    if (data_bits[format][0] == format) {
        *bpp = data_bits[format][2];
    }

    #if DSI_PIXEL_DEBUG_LOG
    printf("%s line:%d -------------------------------------bpp:%d\n", __FUNCTION__, __LINE__, *bpp);
    #endif

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
