#ifndef _DSI_PIXEL_INFO_H_
#define _DSI_PIXEL_INFO_H_

#include "data_type.h"
#include "dsi_pixel_info_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
int32_t dsi_pixel_clk_get(dsi_get_pixel_clk_in_t *inParam, dsi_get_pixel_clk_out_t *outParam);
int32_t dsi_bits_per_pixel_get(uint32_t format, uint32_t *bpp);
int32_t mcu_pixel_clk_get(dsi_get_pixel_clk_in_t *inParam, dsi_get_pixel_clk_out_t *outParam);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
