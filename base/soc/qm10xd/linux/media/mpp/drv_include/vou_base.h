#ifndef _VOU_BASE_H_
#define _VOU_BASE_H_

#include "vou_osal.h"

typedef enum {
	VOU_WBC_SRC_V0 = 0,
	VOU_WBC_SRC_BLENDED_RGB,
	VOU_WBC_SRC_BLENDED_YUV,
} vou_wbc_src_idx_e;

typedef enum {
	VOU_DEV_IDX_DHD = 0,
	VOU_DEV_IDX_DHD1,
	VOU_DEV_IDX_DSD,
	VOU_DEV_IDX_NUM,
} vou_dev_idx_e;

typedef enum {
	VOU_SC_IDX_VIDEO = 0,
	VOU_SC_IDX_WBC = 1,
	VOU_SC_IDX_NUM,
} vou_sc_idx_e;

typedef enum {
	VOU_LAYER_V0B00 = 0, // V0BLKxx must be put at top
	VOU_LAYER_V0B01,
	VOU_LAYER_V0B02,
	VOU_LAYER_V0B03,
	VOU_LAYER_V0B10,
	VOU_LAYER_V0B11,
	VOU_LAYER_V0B12,
	VOU_LAYER_V0B13,
	VOU_LAYER_V0B20,
	VOU_LAYER_V0B21,
	VOU_LAYER_V0B22,
	VOU_LAYER_V0B23,
	VOU_LAYER_V0B30,
	VOU_LAYER_V0B31,
	VOU_LAYER_V0B32,
	VOU_LAYER_V0B33,
	VOU_LAYER_V0, // V0BLKxx must be followed by VOU_LAYER_V0, for DSD V0 only
	VOU_LAYER_V1,
	VOU_LAYER_OSD,
	VOU_LAYER_OSD1,
	VOU_LAYER_HC,
	VOU_LAYER_WBC,
	VOU_LAYER_NUM,
} vou_layer_idx_e;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} image_roi_t;

typedef struct {
	uint16_t w;
	uint16_t h;
} image_size_t;

typedef struct {
	int16_t x;
	int16_t y;
} image_point_t;

typedef struct {
	uint8_t y;
	uint8_t u;
	uint8_t v;
} image_yuv_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} image_rgb_t;

typedef enum { // from high bits to low bits:
	PIXEL_FORMAT_RGBA8888 = 0,
	PIXEL_FORMAT_ARGB8888,
	PIXEL_FORMAT_ARGB4444,     // argb4444
	PIXEL_FORMAT_ARGB1555,     // argb1555

	PIXEL_FORMAT_ABGR8888,
	PIXEL_FORMAT_BGRA8888,
	PIXEL_FORMAT_ARGB4444_BE,  // GBAR4444
	PIXEL_FORMAT_ARGB1555_BE,  // G3B5 - A1R5G2

	PIXEL_FORMAT_YUV_422SP,
	PIXEL_FORMAT_YVU_422SP,
	PIXEL_FORMAT_YUV_420SP,
	PIXEL_FORMAT_YVU_420SP,

//	PIXEL_FORMAT_JPEG,

	PIXEL_FORMAT_NUM,
} image_pixel_format_e;

typedef enum {
	ARGB_EXTEND_MODE_LSB = 0,
	ARGB_EXTEND_MODE_MSB,
	ARGB_EXTEND_MODE_MSBS,
	ARGB_EXTEND_MODE_0,
} argb_extend_mode_e;

typedef enum {
    VIDEO_FIELD_MODE_FRAME = 0,
    VIDEO_FIELD_MODE_INTERLACED,
    VIDEO_FIELD_MODE_TOP_FIELD,
    VIDEO_FIELD_MODE_BOTTOM_FIELD,
} video_field_mode_e;

typedef enum {
	DHD_OUTPUT_DST_HDMI = 0,
	DHD_OUTPUT_DST_LCD,
	DHD_OUTPUT_DST_BT,
	DHD_OUTPUT_DST_AVE,
} dhd_output_dst_e;

typedef struct {
	uint32_t              width;
	uint32_t              height;
	uint32_t              pitch;
	uint32_t              orig_height;
	image_pixel_format_e  format;
	uint8_t               compressed;

	uint32_t              phyAddr[2];
//	void                  *pVirAddr[2];
	uint32_t              headerPhyAddr[2];
//	void                  *pHeaderVirAddr[2];

//	video_field_mode_e    field_mode;
	image_roi_t           roi;

	argb_extend_mode_e    argb_extend_mode;
	uint8_t               argb_pixel_alpha_eb;
	uint8_t               argb_alpha0_mapping;
	uint8_t               argb_alpha1_mapping;
	uint8_t               argb_pixel_alpha_range_255;
	uint8_t               argb_alpha_premultiplied;

//	uint64_t              timestamp;
	uint32_t              uuid;
} image_buffer_t;

#define IMG_UUID_NULL 0

typedef enum {
	HDMI_AUDIO_SAMPLE_RATE_DISABLE = 0,
	HDMI_AUDIO_SAMPLE_RATE_32K,
	HDMI_AUDIO_SAMPLE_RATE_48K,
	HDMI_AUDIO_SAMPLE_RATE_44K
} hdmi_audio_sample_rate_e;

typedef enum {
	TEST_MODE_COLOR_BLOCK = 0,
	TEST_MODE_COLOR_BAR = 1,
	TEST_MODE_PURE_COLOR = 2,
	TEST_MODE_CHESS_BOARD = 3,
} test_mode_e;

typedef enum {
	RIM_WIDTH_1PIXEL = 0,
	RIM_WIDTH_2PIXEL = 1,
	RIM_WIDTH_3PIXEL = 2,
	RIM_WIDTH_4PIXEL = 3,
} rim_width_e;

typedef struct {
	rim_width_e width;
	image_yuv_t color[2];
} vou_rim_info_t;

typedef struct {
	image_yuv_t video_yuv;
	image_rgb_t blend_rgb;
} vou_bg_color_t;

typedef struct{
// Y += brightness
    int8_t brightness; // -128~127
// U = (U-128) * uvSatvXContrs / 512 + 128
// V = (V-128) * uvSatvXContrs / 512 + 128
    uint16_t uvSatvXContrs; // 2.9bits
// U = (U-128) * hueCos / 32  - (V-128) * hueSin / 32 + 128
// V = (V-128) * hueCos / 32  - (U-128) * hueSin / 32 + 128
    int8_t hueSin; // -32~32
    int8_t hueCos; // -32~32
// deltaY: Y difference between current pixel and surrounding pixels
// if ( deltaY >= sharpThr[1] )
//     Y = ( Y * 2 + sharpK[1] * deltaY ) / 2
// eles if ( deltaY >= sharpThr[0] )
//     Y = ( Y * 2 + sharpK[0] * deltaY ) / 2
    uint8_t sharpThr[2]; // 0~255
    uint8_t sharpK[2]; // 3.1bits
// Y = yContrastSlope * ( Y - yContrastOffset ) / 16 + yContrastOffset
    int8_t yContrastOffset; // -128~127
    uint8_t yContrastSlope; // 1.4bits
} cm_param_t;

typedef struct {
	vou_dev_idx_e devId;
	uint8_t vga_if_eb;
	uint8_t hdmi_if_eb;
	uint8_t cvbs_if_eb;
	uint8_t lcm_if_eb;
	uint8_t ahd_if_eb;
	uint8_t bt_if_eb;
} vou_intf_eb_t;

typedef enum {
	LCM_IDLE,
	LCM_CMD,
	LCM_WR_DATA,
	LCM_RD_DATA,
}lcm_stage_type_t;

typedef enum {
	MCU_IF_18BIT = 0,
	MCU_IF_16BIT,
	MCU_IF_9BIT,
	MCU_IF_8BIT,
}lcm_mcu_if_type_e;

typedef enum {
	RGB_BITS_RGB888 = 0,
	RGB_BITS_RGB666,
	RGB_BITS_RGB565,
}lcm_bit_depth_e;

typedef enum {
	RGB_IF_18BIT = 0,
	RGB_IF_16BIT,
	RGB_IF_6BIT,
	RGB_IF_24BIT,
}lcm_rgb_if_type_e;

typedef enum {
	RGB_ORDER_BGR = 0,
	RGB_ORDER_GBR,
	RGB_ORDER_RBG,
	RGB_ORDER_BRG,
	RGB_ORDER_GRB,
	RGB_ORDER_RGB,
}lcm_rgb_if_order_e;

typedef enum {
    _LCM_TYPE_MCU = 0,
    _LCM_TYPE_RGB,
    _LCM_TYPE_MIPI,
    _LCM_TYPE_MAX
}lcm_type_e;

typedef enum {
	UV_ORDER_VU = 0,
	UV_ORDER_UV,
}lcm_uv_order_e;

typedef enum {
	YUV422_8BIT = 0,
	YUV422_16BIT,
	YUV444_24BIT,
}lcm_yuv_if_type_e;

typedef struct {
	lcm_bit_depth_e    bit_depth;
	lcm_rgb_if_type_e  if_type;
	lcm_uv_order_e	   uv_order;
	lcm_rgb_if_order_e rgb_order;
	lcm_yuv_if_type_e  yuv_mode;
	bool dithering_en;
	bool bitswap_en;
	bool use_clk_negedge;
	lcm_mcu_if_type_e  mcu_if_type;	
}lcm_dis_info;

typedef struct {
	char     *Name;
	uint32_t Hactvie;
	uint32_t Vactive;
	uint32_t FrameMode;
	uint32_t FrameRate;
	uint32_t PixelRate;
	uint32_t Htotal;
	uint32_t Vtotal;
	uint32_t Hblank;
	uint32_t Vblank;
	uint32_t Hfront;
	uint32_t Hsync;
	uint32_t Hback;
	uint32_t Hpol;
	uint32_t Vfront;
	uint32_t Vsync;
	uint32_t Vback;
	uint32_t Vpol;
	uint32_t Alen;
	uint32_t Aline;
	uint32_t Hlen;
	uint32_t StartBlen;
	uint32_t EndBlen;
	uint32_t TVLine;
	uint32_t TBLineOfs;
	uint32_t TEndBLine;
	uint32_t BVLine;
	uint32_t BBLineOfs;
	uint32_t BEndBLine;
	uint32_t HSyncLength;
	uint32_t VSyncLength0;
	uint32_t Fpol;
} vou_sync_info_t;

typedef struct {
	uint32_t h_sync_pol;
	uint32_t v_sync_pol;
	uint32_t de_pol;
}_rgb_display_cfg_t;

typedef struct {
    uint32_t trans_mode; // 8/9bit lsb/hsb first
    uint32_t cs_type;
    uint32_t te_eb;
    uint32_t te_mode;
    uint32_t te_edge;
}_mcu_display_cfg_t;

typedef struct {
	uint8_t bit_depth;
	uint8_t if_type;
	uint8_t rgb_order;
	uint8_t dithering_en;
	uint8_t bitswap_en;
    union {
        _rgb_display_cfg_t rgb;
        _mcu_display_cfg_t mcu;
    }info;
//}lcm_display_info_t;
}_lcm_display_cfg_t;

#endif
