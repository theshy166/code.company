#ifndef _DSI_HOST_V01_DRV_PARAM_H_
#define _DSI_HOST_V01_DRV_PARAM_H_

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
    DSI_DPI_RGB565 = 0, // 16bit cfg1
    DSI_DPI_RGB565_LOW,   // 16bit cfg1
    DSI_DPI_RGB565_MID,// 16bit cfg1
    DSI_DPI_RGB666,// 18bit cfg1
    DSI_DPI_RGB666_LOW, // 18bit cfg1
    DSI_DPI_RGB888, // 24bit
    //DSI_DPI_YUVA_422,// 20bit YCbCr 422
    //DSI_DPI_YUVC_422,// 24bit YCbCr 422
    //DSI_DPI_YUV8_422,// 16bit YCbCr 422
    //DSI_DPI_RGBAAA,// 30bit
    //DSI_DPI_RGBCCC,// 36bit
    //DSI_DPI_YUV8_420,// 12bit YCbCr 422
    DSI_DPI_color_format_max
}dsi_host_dpi_color_format_e;

typedef enum {
    dsi_8bit_8_bpp = 0,
    dsi_8bit_12_bpp,
    dsi_8bit_16_bpp,
    dsi_8bit_18_bpp,
    dsi_8bit_24_bpp,
    dsi_9bit_18_bpp,
    dsi_16bit_8_bpp,
    dsi_16bit_12_bpp,
    dsi_16bit_16_bpp,
    dsi_16bit_18_bpp_option1,
    dsi_16bit_18_bpp_option2,
    dsi_16bit_24_bpp_option1,
    dsi_16bit_24_bpp_option2,
    dsi_dbi_input_data_max
}dsi_host_dbi_data_e;

typedef enum {
    dsi_16bit_color_display = 0,
    dsi_18bit_color_display,
    dsi_24bit_color_display,
    dsi_16bit_color_display_1,
    dsi_write_lut_size_max
}dsi_host_write_lut_size_e;

typedef enum {
    nonburst_with_sync_pulses = 0,
    nonburst_with_sync_events,
    burst_mode,
    dsi_host_vid_mode_type_max
}dsi_host_vid_mode_type_e;

typedef enum {
    dsi_3D_mode_off = 0,
    dsi_3D_mode_on_portrait,
    dsi_3D_mode_on_landscape,
    dsi_mode_3d_max
}dsi_host_mode_3d_e;

typedef enum {
    line = 0,
    frame,
    pixel,
    dsi_host_format_3d_max
}dsi_host_format_3d_e;

typedef enum {
    one_data_lane = 0,
    tow_data_lane,
    three_data_lane,
    four_data_lane,
    dsi_host_n_lanes_max
}dsi_host_n_lanes_e;

typedef enum {
    pps_table_1 = 0,
    pps_table_2,
    pps_table_3,
    pps_table_4,
    dsi_host_pps_sel_max
}dsi_host_pps_sel_e;

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

