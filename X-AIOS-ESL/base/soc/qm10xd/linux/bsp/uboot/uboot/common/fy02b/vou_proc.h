#ifndef _VOU_PROC_H_
#define _VOU_PROC_H_

#include "vou_base.h"


#define DDR_START 0x40000000U
#define DDR_END 0xffffffffU
#define BLKCNT 8
#define HC_SIZE 1023

typedef enum {
	E_ARGB1555 = 0,
	E_ARGB4444 = 1,
	E_ARGB8888 = 2
} argb_type_e;

typedef enum {
	E_YUV420 = 0,
	E_YUV422 = 1,
} yuv_format_e;

typedef enum {
	E_YUV = 0,
	E_RGB = 1,
} wbc_format_e;

typedef enum {
	E_BIG_ENDIAN = 0,
	E_LITTLE_ENDIAN = 1,
} endian_type_e;

typedef enum {
	E_LSB = 0,
	E_MSB = 1,
} alpha_order_e;

typedef enum {
	E_U_FIRST = 0, // 0: u first, v msb, nv12/nv16
	E_V_FIRST = 1, // 1: v first, u msb, nv21/nv61
} uv_order_type_e;

typedef enum {
	E_WBC_SRC_BLENDED_MIX = 0,
	E_WBC_SRC_V0_V1 ,
	E_WBC_SRC_V0,
	E_WBC_SRC_V1,
} wbc_src_e;

#define VOU_TIMEOUT_MS 100

typedef struct {
	uint32_t PixelRate;
	uint32_t reg2;
} vou_hdmi_clk_cfg_t;

typedef struct {
	uint16_t offset;
	uint8_t value;
} vou_hdmi_audio_cfg_t;

typedef enum {
	UPDATE_CTRL_IDX0 = 0,
	UPDATE_CTRL_IDX1,
	UPDATE_CTRL_IDX2,
	UPDATE_CTRL_IDX_NUM,
} update_ctrl_idx_e;

typedef struct {
	uint32_t update_value[VOU_DEV_IDX_NUM][UPDATE_CTRL_IDX_NUM];
} update_ctrl_t;

typedef enum {
	VOU_REGSET_IDX_DEVICE_ARG_RIM = 0x1,
	VOU_REGSET_IDX_DEVICE_ARG_BG = 0x2,
	VOU_REGSET_IDX_DEVICE_ARG_CHN = 0x4,
	VOU_REGSET_IDX_DEVICE_ARG_LAYER = 0x8,
	VOU_REGSET_IDX_DEVICE_ARG_APPLIED_RIM   = VOU_REGSET_IDX_DEVICE_ARG_RIM << 16,
	VOU_REGSET_IDX_DEVICE_ARG_APPLIED_BG    = VOU_REGSET_IDX_DEVICE_ARG_BG << 16,
	VOU_REGSET_IDX_DEVICE_ARG_APPLIED_CHN   = VOU_REGSET_IDX_DEVICE_ARG_CHN << 16,
	VOU_REGSET_IDX_DEVICE_ARG_APPLIED_LAYER = VOU_REGSET_IDX_DEVICE_ARG_LAYER << 16,
} vou_regset_idx_device_arg;

typedef enum {
	VOU_REGSET_IDX_DEVICE = VOU_LAYER_NUM, // global (treat as DHD regset)
	VOU_REGSET_IDX_GLOBAL,

#if 0
	VOU_REGSET_IDX_V0_GLOBAL,
	VOU_REGSET_IDX_V1_GLOBAL,
	VOU_REGSET_IDX_TVE,
	VOU_REGSET_IDX_V0_SCALAR_COEF,
	VOU_REGSET_IDX_V1_SCALAR_COEF,
	VOU_REGSET_IDX_WBC_SCALAR_COEF,
#endif
	VOU_REGSET_IDX_NUM,
} vou_regset_idx_e;

typedef struct {
	bool     color_eb;
	uint32_t color[8];		// bit[23:16] = V; bit[15:8] = U; bit[7:0] = Y
	uint16_t blk_w;
	uint16_t blk_h;
} pattern_info_t;

typedef struct {
	pattern_info_t pattern_info;	//test pattern information
	vou_rim_info_t rim_info;
	vou_bg_color_t bg_info;
} vou_misc_info_t;

typedef struct {
    int16_t hor_y[16][8];
	int16_t hor_c[16][4];
    int16_t vert[16][4];
	int16_t vert_c[16][4];//backup for vppu.in vppu case.input yuv420/yuv422,scalar down vert_uv coef is diff whit vert_y
} vou_scaling_coef_t;

typedef struct {
	// start stored in image_buffer_t
	uint16_t src_x;
	uint16_t src_y;
	uint16_t src_pitch;
	uint16_t src_orig_height;
	uint16_t src_w;
	uint16_t src_h;

	yuv_format_e format;
	uv_order_type_e uv_order;

	uint8_t scalar_eb;
	uint8_t scalar_coef_curidx;
	uint8_t scalar_coef_preidx;

	uint8_t compress;

	uint32_t y_img;
	uint32_t uv_img;
	uint32_t y_ot_img;
	uint32_t uv_ot_img;
	// end stored in image_buffer_t

	uint16_t dst_x;
	uint16_t dst_y;
	uint16_t dst_w;
	uint16_t dst_h;

	bool blk_eb;

	bool rim_eb;
	uint8_t rim_color_idx;

	cm_param_t cm_info;
	bool cm_pending;

	uint8_t glb_alpha;		// global alpha value, range from 0-128
} vou_yuv_layer_t;

typedef struct {
	// stored in image_buffer_t
	uint16_t src_x;
	uint16_t src_y;
	uint16_t src_pitch;
	uint16_t src_orig_height;
	uint16_t src_w;
	uint16_t src_h;

	alpha_order_e alpha_order; //argb1555, argb4444 always in msb(argb). argb8888: 0: alpha msb(argb), 1: alpha lsb(rgba)
	uint8_t extend_mode;// 00: lsb, 01: msb, 10: msbs, 11: 0
	uint8_t alpha0;
	uint8_t alpha1;

	argb_type_e format; // 00: argb1555, 01: argb4444, 10: argb8888
	uint8_t compress;

	uint8_t pixel_alpha_premultiplied;	// 0: not premultiplied; 1: premultiplied
	uint8_t pixel_alpha_eb;	// pixel alpha enable
	uint8_t pixel_alpha_range_255; // 0: 0-128; 1: 0-256

	uint32_t argb_img;
	// end stored in image_buffer_t

	// not available
	endian_type_e byte_order; // 0: big endian, 1: little endian

	uint16_t dst_x;
	uint16_t dst_y;

	uint8_t glb_alpha; // global alpha value, range from 0-128
} vou_argb_layer_t;

typedef struct {
	bool dhd_chn_ck_eb;
	bool dsd_chn_ck_eb;
	bool dhd1_chn_ck_eb;
	bool vga_if_eb;
	bool vga_src_sel; //0:dhd0 1:dhd1
	bool hdmi_if_eb;
	bool hdmi_src_sel; //0:dhd0 1:dhd1
	uint32_t arqos_low;
	uint32_t arqos_high;
	uint32_t arqos_thr_low;
	uint32_t arqos_thr_high;
	uint32_t awqos_low;
	uint32_t awqos_high;
	uint32_t awqos_thr_low;
	uint32_t awqos_thr_high;
	dhd_output_dst_e dhd_output_dst;
} vou_global_t;

typedef struct {
	bool wbc_eb;					// write back eb
	wbc_src_e wbc_src; 			// 00:video0 yuv 01:blended ARGB8888 10:reserved 11:blended YUV
	yuv_format_e format;		//img chn 0:y_uv420: 1:y_uv422
	wbc_format_e wbc_format;
	uv_order_type_e uv_order;			// 0:v msb 1:v lsb
	bool wbc_scalar_eb;
	bool wbc_img_crop_eb;
	uint16_t wbc_crop_x;				// scaler trim x max 4095
	uint16_t wbc_crop_y;				// scaler trim y max 4095
	uint16_t wbc_crop_width;		// scaler trim width max 4096
	uint16_t wbc_crop_height;		// scaler trim height max 4095
	uint16_t wbc_scaling_width;			// scaler dst width max 2048x2048
	uint16_t wbc_scaling_height;		// scaler dst height max 2048x2048
	uint32_t y_img;
	uint32_t uv_img;
} vou_write_back_layer_t;

typedef struct {
	uint16_t PCLK_10000HZ;
	uint16_t VFREQ_HZ;
	uint16_t PIXEL_PER_LINE;
	uint16_t LINES;
} hdmi_tx_info_t;

typedef struct {
	uint16_t h_len;
	uint16_t start_b_len;
	uint16_t tb_line_ofs;
	uint16_t bb_line_ofs;
	uint16_t tv_line;
	uint16_t bv_line;
	hdmi_tx_info_t hdmi_tx_info;
} vou_blanking_info_t;

typedef struct {
	uint32_t hsync;
	uint32_t vsync0;
	uint32_t vsync1;
} tve_sync_t;

typedef struct {
	uint16_t yrgb_black_level;
	uint16_t yrgb_white_level;
	uint16_t yrgb_blank_level;
	uint16_t cbcr_peak_level_low;
	uint16_t cbcr_peak_level_high;
	uint16_t cbcr_blank_level;
	uint16_t yrgb_ofs0;
	uint16_t yrgb_ofs1;
	uint16_t cbcr_ofs0;
	uint16_t cbcr_ofs1;
} tve_clamp_info_t;

typedef struct {
	uint8_t yrgb_clip_low;
	uint8_t yrgb_clip_high;
	uint8_t uv_clip_low;
	uint8_t uv_clip_high;
	uint8_t mode;
} vou_clip_info_t;

typedef struct {
	uint8_t hdmi_src;	//0: RGB; 1:YUV
	bool hsync_pol;
	bool vsync_pol;
	bool de_pol;
	bool vtotal_even;
	uint8_t r2y_matrix_sel;//[1:0]: 00:BT601 01:BT709 10:BT2020 11:reserved.DHD only
	bool clip_eb;//0: output is full range  1：clip to Narrow range.for HDMI only in DHD, CVBS always has clip
	uint8_t clip_mode;//0:direct clip  1:clamp clip.for HDMI only in DHD
	uv_order_type_e hdmi_uv_order;//0:ycbcr yuv; 1:ycrcb yvu
} vou_hdmi_info_t;

typedef struct {
	int16_t coef[3][3];
	int16_t offset[3];
} y2r_matrix_t;

typedef struct {
	tve_clamp_info_t clamp_info;
	tve_sync_t tve_sync_info;
	bool tve_fir_bypass;
	vou_blanking_info_t blank_info;
	uint32_t RealPixelRate;
	uint32_t hdmireg2;
	vou_hdmi_info_t hdmi_info;
	vou_clip_info_t clip_info;
	bool cvbs_ntsc_mode;
	y2r_matrix_t y2r_matrix;
} vou_intf_info_t;

typedef struct {
	uint16_t width;
	uint16_t height;
	uint32_t frame_rate;
	bool progressive;
	bool gray_mode;
	bool img0_gray_mode;
	bool img1_gray_mode;
	bool img0_eb;
	bool img1_eb;
	bool osd_eb;
	bool hc_eb;
	uint8_t img0_g_alpha;
	uint8_t img1_g_alpha;
	uint8_t osd_g_alpha; // not in other rdy
	uint8_t osd1_g_alpha; // not in other rdy
	uint8_t hc_g_alpha; // not in other rdy
	uint8_t clip_eb;
	uint8_t clip_mode;
	bool img0_on_top;
	bool test_mode_eb;
	test_mode_e test_mode;
} vou_dev_info_t;

typedef struct {
	vou_dev_info_t dev_info;
	vou_intf_info_t intf_info;
	vou_misc_info_t misc_info;
	vou_argb_layer_t osd;
	vou_argb_layer_t osd1;
	vou_argb_layer_t hc;
	vou_write_back_layer_t wb_layer;
	uint32_t irq_mask;
} vou_dev_cfg_t;

typedef int (*image_release_cb)(vou_dev_idx_e devId, vou_layer_idx_e layerId, uint32_t uuid, void *priv_data);
typedef int (*display_vsync_cb)(vou_dev_idx_e devId, uint32_t updated[VOU_LAYER_NUM], void *priv_data);

typedef struct {
	image_release_cb release_cb;
	display_vsync_cb vsync_cb;
	void *priv_data;
	uint32_t vsync_update_buffer[VOU_DEV_IDX_NUM][VOU_LAYER_NUM];
	update_ctrl_t rdy;
} vou_postponed_t;

typedef struct {
	image_roi_t window;
	uint32_t locked; // buffer is locked by hw while shadow registers have taken places
	uint32_t loaded; // loaded to shadow registers
	uint32_t updated; // along with vou_layer_t, updated to vou_write_back_layer_t / vou_argb_layer_t / vou_yuv_layer_t
	union {
		struct { // video and gui layer do not have wbc_src
			uint8_t eb;
			uint8_t rim_eb;
			uint8_t rim_color_idx;
			image_roi_t zoom_roi;
		};
		struct { // wbc layer does not have eb/rim/idx
			vou_wbc_src_idx_e src;
		};

	};
	image_buffer_t buffer;
} vou_layer_t;

typedef struct {
	uint16_t width;
	uint16_t height;
	uint32_t frame_rate;
	uint8_t progressive;
	vou_sync_info_t *sync_info;
	vou_hdmi_clk_cfg_t *hdmi_clk_info;

	pattern_info_t pattern_info;
	bool test_mode_eb;
	test_mode_e test_mode;

	vou_rim_info_t rim_info;

	vou_bg_color_t bg_info;

	bool gray_mode;
	bool img0_gray_mode;
	bool img1_gray_mode;

	uint8_t img0_g_alpha;
	uint8_t img1_g_alpha;
	uint8_t osd_g_alpha;
	uint8_t osd1_g_alpha;
	uint8_t hc_g_alpha;
	bool img0_on_top;
	lcm_type_e lcm_type;
} vou_device_t;

typedef struct {
	bool keeplogo;
	bool restore_enable;
} vou_init_param_t;

typedef struct {
	vou_global_t glbcfg;
	cm_param_t cm_default_cfg;
	vou_dev_cfg_t devcfg[VOU_DEV_IDX_NUM];
	vou_yuv_layer_t v0hd[VOU_LAYER_V1];
	vou_yuv_layer_t v1hd;
	vou_yuv_layer_t v1sd;
	vou_scaling_coef_t fetch_sc[VOU_SC_COEF_IDX_NUM];
	vou_scaling_coef_t wbc_sc;
	vou_intf_eb_t intf_eb;
	vou_device_t device[VOU_DEV_IDX_NUM];
	vou_layer_t layer[VOU_DEV_IDX_NUM][VOU_LAYER_NUM];
	vou_postponed_t postponed;
	vou_reg_base_t regbase;
	int irq_dhd;
	int irq_dhd1;
	int irq_dsd;
	vou_init_param_t init_param;
} vou_cfg_t;

// register operation type:
typedef enum {
	VOU_REGOP_TYPE_DELAYED = 0,
	VOU_REGOP_TYPE_DELAYED_UNLOAD  = 1, // wbc unload
	VOU_REGOP_TYPE_IMMEDIATE_LOAD = 2, // immediate: for buffer update only, performed immediately
	VOU_REGOP_TYPE_IMMEDIATE_DISABLE = 3, // disable layer
	VOU_REGOP_TYPE_RESTORE = 4, // restore sequence
} vou_reg_op_type_e;

#endif
