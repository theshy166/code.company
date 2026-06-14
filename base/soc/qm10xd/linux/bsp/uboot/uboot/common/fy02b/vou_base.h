#ifndef _VOU_BASE_H_
#define _VOU_BASE_H_

#include "vou_osal.h"

typedef enum {
	VOU_WBC_SRC_V0 = 0,
	VOU_WBC_SRC_BLENDED_RGB,
	VOU_WBC_SRC_BLENDED_YUV,
	VOU_WBC_SRC_V0_V1 ,
	VOU_WBC_SRC_V1,
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
	VOU_SC_COEF_IDX_16 = 0,
	VOU_SC_COEF_IDX_15 = 1,
	VOU_SC_COEF_IDX_14 = 2,
	VOU_SC_COEF_IDX_13 = 3,
	VOU_SC_COEF_IDX_12 = 4,
	VOU_SC_COEF_IDX_11 = 5,
	VOU_SC_COEF_IDX_10 = 6,
	VOU_SC_COEF_IDX_09 = 7,
	VOU_SC_COEF_IDX_08 = 8,
	VOU_SC_COEF_IDX_NUM,
} vou_sc_coef_idx_e;

typedef enum {
	VOU_LAYER_V0B00 = 0,
	VOU_LAYER_V0B01,
	VOU_LAYER_V0B02,
	VOU_LAYER_V0B03,
	VOU_LAYER_V0B04,
	VOU_LAYER_V0B05,
	VOU_LAYER_V0B06,
	VOU_LAYER_V0B07,
	VOU_LAYER_V0B10,
	VOU_LAYER_V0B11,
	VOU_LAYER_V0B12,
	VOU_LAYER_V0B13,
	VOU_LAYER_V0B14,
	VOU_LAYER_V0B15,
	VOU_LAYER_V0B16,
	VOU_LAYER_V0B17,
	VOU_LAYER_V0B20,
	VOU_LAYER_V0B21,
	VOU_LAYER_V0B22,
	VOU_LAYER_V0B23,
	VOU_LAYER_V0B24,
	VOU_LAYER_V0B25,
	VOU_LAYER_V0B26,
	VOU_LAYER_V0B27,
	VOU_LAYER_V0B30,
	VOU_LAYER_V0B31,
	VOU_LAYER_V0B32,
	VOU_LAYER_V0B33,
	VOU_LAYER_V0B34,
	VOU_LAYER_V0B35,
	VOU_LAYER_V0B36,
	VOU_LAYER_V0B37,
	VOU_LAYER_V0B40,
	VOU_LAYER_V0B41,
	VOU_LAYER_V0B42,
	VOU_LAYER_V0B43,
	VOU_LAYER_V0B44,
	VOU_LAYER_V0B45,
	VOU_LAYER_V0B46,
	VOU_LAYER_V0B47,
	VOU_LAYER_V0B50,
	VOU_LAYER_V0B51,
	VOU_LAYER_V0B52,
	VOU_LAYER_V0B53,
	VOU_LAYER_V0B54,
	VOU_LAYER_V0B55,
	VOU_LAYER_V0B56,
	VOU_LAYER_V0B57,
	VOU_LAYER_V0B60,
	VOU_LAYER_V0B61,
	VOU_LAYER_V0B62,
	VOU_LAYER_V0B63,
	VOU_LAYER_V0B64,
	VOU_LAYER_V0B65,
	VOU_LAYER_V0B66,
	VOU_LAYER_V0B67,
	VOU_LAYER_V0B70,
	VOU_LAYER_V0B71,
	VOU_LAYER_V0B72,
	VOU_LAYER_V0B73,
	VOU_LAYER_V0B74,
	VOU_LAYER_V0B75,
	VOU_LAYER_V0B76,
	VOU_LAYER_V0B77,
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
	DHD_OUTPUT_DST_DSI,
	DHD_OUTPUT_DST_AVE,
	DHD_OUTPUT_DST_BT,
	DHD_OUTPUT_DST_LCM,
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
	uint8_t				  src_expand_info;
//	uint64_t              timestamp;
	uint32_t              uuid;
} image_buffer_t;

#define IMG_UUID_NULL 0

typedef enum {
	HDMI_AUDIO_SAMPLE_RATE_DISABLE = 0,
	HDMI_AUDIO_SAMPLE_RATE_32K,
	HDMI_AUDIO_SAMPLE_RATE_48K,
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
	bool vga_if_eb;
	bool vga_src_sel;
	bool hdmi_if_eb;
	bool hdmi_src_sel;
	bool cvbs_if_eb;
} vou_intf_eb_t;

typedef enum {
	LCM_IDLE,
	LCM_CMD,
	LCM_WR_DATA,
	LCM_RD_DATA,	
}lcm_stage_type_t;

typedef enum {
	RGB_IF_18BIT,
	RGB_IF_16BIT,
	RGB_IF_6BIT,
	RGB_IF_24BIT,
}lcm_rgb_if_type_t;

typedef enum {
	LCM_TYPE_MCU = 0,
	LCM_TYPE_RGB,
	LCM_TYPE_MIPI,
	LCM_TYPE_MAX
}lcm_type_e;

typedef struct {
	uint32_t Clock;
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
} vou_sync_info_t;

#endif
