#ifndef _VOU_INTERFACE_H_
#define _VOU_INTERFACE_H_

#include "vou_proc.h"

typedef enum {
	VOU_ERR_NONE = 0,
	VOU_ERR_PREV_BUFF_PENDING = -1,
	VOU_ERR_LAYER_DISABLED = -2,
	VOU_ERR_INVALID_BUF_PTR = -3,
	VOU_ERR_INVALID_BUF_FORMAT = -4,
	VOU_ERR_INVALID_BUF_SIZE = -5,
	VOU_ERR_INVALID_BUF_ALIGN = -6,
	VOU_ERR_INVALID_BUF_COMPRESSION = -7,
	VOU_ERR_INVALID_BUF_ROI = -8,
	VOU_ERR_INVALID_ZOOM_SIZE = -9,
	VOU_ERR_INVALID_WBC_SRC = -10,
	VOU_ERR_INVALID_PARAM = -11,
	VOU_ERR_INVALID_SYNC_INFO = -12,
	VOU_ERR_INVALID_DEVICE_ID = -13,
	VOU_ERR_INVALID_LAYER_ID = -14,
	VOU_ERR_INVALID_LAYER_ALIGN = -15,
	VOU_ERR_INVALID_LAYER_REGION = -16,
	VOU_ERR_TIMEOUT = -17,
	VOU_ERR_INVALID_OPERATION = -18,
	VOU_ERR_INVALID_BUF_UUID = -19,
	VOU_ERR_INVALID_BUF_ADDR = -20,
	VOU_ERR_DEVICE_DISABLED = -21,
} vou_error_type_e;

//VOU_CMD_ENABLE = 0
    //arg: NULL

//VOU_CMD_DISABLE
    //arg: NULL

typedef struct {
	uint8_t vga_if_detected;
	uint8_t hdmi_if_detected;
	uint8_t cvbs_if_detected;
} vou_intf_detect_t;

//VOU_CMD_DETECT_VO_INTF
    //arg: vou_intf_detect_t

//VOU_CMD_TOGGLE_VO_INTF
    //arg: vou_intf_eb_t

typedef struct {
	vou_dev_idx_e devId;
	uint8_t eb;
} vou_dev_toggle_t;

//VOU_CMD_TOGGLE_DEV
    //arg: vou_dev_toggle_t

typedef struct {
	uint16_t width;
	uint16_t height;
	uint32_t frame_rate;
	uint8_t progressive;
} vou_output_info_t;

typedef struct {
	vou_dev_idx_e devId;
	vou_output_info_t output_info;
} vou_dev_output_info_t;

//VOU_CMD_GET_DEV_OUTPUT_INFO
    //arg: vou_dev_output_info_t

//VOU_CMD_SET_DEV_OUTPUT_INFO
    //arg: vou_dev_output_info_t

typedef struct { // alpha range: 0 ~ 128
	vou_dev_idx_e devId;
	uint8_t img0_g_alpha;
	uint8_t img1_g_alpha;
	uint8_t osd_g_alpha; // need to refresh after changing this factor
	uint8_t hc_g_alpha; // need to refresh after changing this factor
	uint8_t img0_on_top;
} vou_galpha_info_t;

//VOU_CMD_SET_GALPHA_INFO
    //arg: vou_galpha_info_t

typedef struct {
	vou_dev_idx_e devId;
	uint8_t gray_mode_eb;
} vou_graymode_info_t;

//VOU_CMD_SET_GRAYMODE_INFO
    //arg: vou_graymode_info_t

typedef struct {
	vou_dev_idx_e devId;
	uint8_t test_mode_eb;
	test_mode_e test_mode;
	uint16_t blk_w;
	uint16_t blk_h;
} vou_testmode_info_t;

//VOU_CMD_SET_TESTMODE_INFO
    //arg: vou_testmode_info_t

typedef struct {
	vou_dev_idx_e devId;
	image_yuv_t video_yuv;
	image_rgb_t blend_rgb;
} vou_bg_color_cfg_t;

//VOU_CMD_SET_BG_COLOR
    //arg: vou_bg_color_cfg_t

typedef struct {
	vou_dev_idx_e devId;
	vou_rim_info_t rim_info;
} vou_rim_cfg_t;

//VOU_CMD_SET_RIM_INFO
    //arg: vou_rim_cfg_t

typedef struct {
	vou_dev_idx_e devId;
	vou_layer_idx_e layerId;
	uint8_t eb;
	image_roi_t window;

	uint8_t rim_eb;
	uint8_t rim_color_idx;
} vou_video_layer_cfg_t;

//VOU_CMD_GET_VIDEO_LAYER_INFO
    //arg: vou_video_layer_cfg_t

//VOU_CMD_SET_VIDEO_LAYER_INFO
    //arg: vou_video_layer_cfg_t

typedef struct{
	vou_dev_idx_e devId;
	vou_layer_idx_e layerId;
	uint8_t eb;
	cm_param_t cm_param;
} vou_yuv_csc_cfg_t; // need to refresh after changing this factor

//VOU_CMD_SET_VIDEO_LAYER_CSC
    //arg: vou_yuv_csc_cfg_t

typedef struct {
	vou_dev_idx_e devId;
	vou_layer_idx_e layerId;
	uint8_t eb;
	image_roi_t window;
} vou_gui_layer_cfg_t;

//VOU_CMD_GET_GUI_LAYER_INFO
    //arg: vou_gui_layer_cfg_t

//VOU_CMD_SET_GUI_LAYER_INFO
    //arg: vou_gui_layer_cfg_t

typedef struct {
	vou_dev_idx_e devId;
	vou_wbc_src_idx_e src;
	image_roi_t window;
} vou_wbc_cfg_t;

//VOU_CMD_SET_WBC_INFO
    //arg: vou_wbc_cfg_t

typedef struct {
	vou_dev_idx_e devId;
	vou_layer_idx_e layerId;
	image_buffer_t *buffer;
} vou_display_buffer_t;

//VOU_CMD_DISPLAY_IMAGE
    //arg: vou_display_buffer_t

typedef struct {
	vou_dev_idx_e devId;
	vou_layer_idx_e layerId;
} vou_dev_layer_t;

//VOU_CMD_REFRESH_LAYER
    //arg: vou_dev_layer_t

typedef struct {
	image_release_cb release_cb;
	display_vsync_cb vsync_cb;
	void *priv_data;
} vou_register_cb_t;

//VOU_CMD_REGISTER_CALLBACKS
    //arg: vou_register_cb_t

typedef struct {
	vou_dev_idx_e devId;
	image_point_t position;
} vou_cursor_pos_t; // refresh is included in the command

//VOU_CMD_EXTRA_SET_CURSOR_POS
    //arg: vou_cursor_pos_t

typedef struct {
	vou_dev_idx_e devId;
	vou_layer_idx_e layerId;
	image_roi_t roi;
} vou_zoom_info_t; // need to refresh after changing this factor

//VOU_CMD_EXTRA_SET_ZOOM_INFO
    //arg: vou_zoom_info_t

//VOU_CMD_SET_HDMI_AUTO_SAMPLE_RATE
typedef struct {
	hdmi_audio_sample_rate_e rate;
} vou_hdmi_audio_info_t;

//---------------------------------------------------

//VOU_CMD_SET_CLIP_INFO
    //arg: vou_clip_info_t

//VOU_CMD_SET_Y2R_MATRIX
    //arg: y2r_matrix_t

//VOU_CMD_SET_TVE_INFO
    //arg: tve_clamp_info_t

//VOU_CMD_SET_HDMI_INFO
    //arg: vou_hdmi_info_t

typedef enum {
	VOU_CMD_ENABLE = 0,
	VOU_CMD_DISABLE,
	VOU_CMD_DETECT_VO_INTF,
	VOU_CMD_TOGGLE_VO_INTF,
	VOU_CMD_TOGGLE_DEV,
	VOU_CMD_GET_DEV_OUTPUT_INFO,
	VOU_CMD_SET_DEV_OUTPUT_INFO, // VOU_CMD_TOGGLE_DEV: do not kick off if VOU_CMD_SET_DEV_OUTPUT_INFO is not set
	VOU_CMD_SET_GALPHA_INFO,
	VOU_CMD_SET_GRAYMODE_INFO,
	VOU_CMD_SET_TESTMODE_INFO,
	VOU_CMD_SET_RIM_INFO,
	VOU_CMD_SET_BG_COLOR,
	VOU_CMD_GET_VIDEO_LAYER_INFO,
	VOU_CMD_SET_VIDEO_LAYER_INFO,
	VOU_CMD_SET_VIDEO_LAYER_CSC,
	VOU_CMD_GET_GUI_LAYER_INFO,
	VOU_CMD_SET_GUI_LAYER_INFO,
	VOU_CMD_SET_WBC_INFO,
	VOU_CMD_DISPLAY_IMAGE,
	VOU_CMD_REFRESH_LAYER,
	VOU_CMD_REGISTER_CALLBACKS,
	VOU_CMD_EXTRA_SET_CURSOR_POS, // EXTRA cmd type, VOU_CMD_EXTRA_SET_CURSOR_POS = VOU_CMD_SET_GUI_LAYER_INFO + VOU_CMD_REFRESH_LAYER, move hc image partly out of frame region is permitted
	VOU_CMD_EXTRA_SET_ZOOM_INFO,// EXTRA cmd type, override image buffer roi info, can also be applied to layers without scaler, just make sure crop size is equal to layer size
	VOU_CMD_SET_HDMI_AUTO_SAMPLE_RATE,
	VOU_CMD_NUM,
} vou_cmd_e;

vou_error_type_e vou_init_context(vou_cfg_t *context);
void _vou_clk_on(vou_cfg_t *context);
void _vou_soft_reset(vou_cfg_t *context, vou_dev_idx_e devId, bool device, bool interface);
void set_fill_cvbs_pal_sync(vou_cfg_t *context);
void set_fill_cvbs_ntsc_sync(vou_cfg_t *context);
#endif
