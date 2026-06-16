/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_MM_FILTER_H__
#define __QUA_MM_FILTER_H__

#include "common/qua_mm_common.h"
#include "common/qua_mm_video_common_type.h"
#include "filter/qua_mm_tde.h"
#include "audio/qua_mm_audio.h"

#define QUA_MM_FILTER_DEV_VPPU "filter.vppu"
#define QUA_MM_FILTER_DEV_TDE  "filter.tde"
#define QUA_MM_FILTER_DEV_VGS  "filter.vgs"
#define QUA_MM_FILTER_DEV_VGS2 "filter.vgs2"
#define QUA_MM_FILTER_DEV_SWAV "filter.swav"
#define QUA_MM_FILTER_DEV_SWPROC "filter.swproc"
#define QUA_MM_FILTER_DEV_VISE "filter.vise"
#define QUA_MM_FILTER_DEV_OSD "filter.osd"

typedef QUA_S32 QUA_VGS_HANDLE;
typedef QUA_S32 QUA_GROUP;
typedef QUA_ULONG QUA_SWAV_HANDLE;

#define QUA_VPSS_MAX_MASK_AREA                    (8)
#define QUA_VPSS_MAX_LOGO_AREA                    (16)
#define QUA_VPSS_MAX_GOSD_COLOR_IDX               (256)

typedef enum qua_filter_chn {
    QUA_MM_FILTER_VPU_CHN,
    QUA_MM_FILTER_VPSS_CHN,
    QUA_MM_FILTER_VGS_CHN,
} qua_filter_chn_t;

typedef struct qua_filter_chn_attr {
    qua_filter_chn_t filter_chn;
    QUA_GROUP group_id;
    QUA_VOID_PTR attr;
} qua_filter_chn_attr_t;

typedef enum qua_vgs_rotation {
    QUA_VGS_ROTATION_NONE,
    QUA_VGS_ROTATION_90,
    QUA_VGS_ROTATION_180,
    QUA_VGS_ROTATION_270,
    QUA_VGS_ROTATION_MIRROR,
    QUA_VGS_ROTATION_FLIP,
    QUA_VGS_ROTATION_MIR_ROT,
    QUA_VGS_ROTATION_FLP_ROT,
    QUA_VGS_ROTATION_BUTT
} qua_vgs_rotation_t;

typedef enum qua_vgs_scale_mode {
    QUA_VGS_SCALE_FILTER,
    QUA_VGS_SCALE_LINEAR,
    QUA_VGS_SCALE_MINPROX,
    QUA_VGS_SCALE_BUTT
} qua_vgs_scale_mode_t;

typedef enum qua_vpss_die_mode {
    QUA_VPSS_DIE_MODE_NODIE = 0,
    QUA_VPSS_DIE_MODE_AUTO  = 1,
    QUA_VPSS_DIE_MODE_DIE   = 2,
    QUA_VPSS_DIE_MODE_BUTT,
} qua_vpss_die_mode_t;

typedef enum qua_vpu_vi_mode {
    QUA_VPU_MODE_ISP            = 0,
    QUA_VPU_MODE_MEM            = 1,
    QUA_VPU_MODE_OFFLINE_VPU    = 1,
    QUA_VPU_MODE_OFFLINE_2DLUT  = 2,
    QUA_VPU_MODE_ONLINE_2DLUT   = 3,
    QUA_VPU_VI_MODE_DUMMY    = 0xffffffff,
} qua_vpu_vi_mode_t;

typedef enum qua_vpu_vo_mode {
    QUA_VPU_VOMODE_BLK          = 0,
    QUA_VPU_VOMODE_SCAN         = 1,
    QUA_VPU_VOMODE_TILE192      = 2,
    QUA_VPU_VOMODE_TILE224      = 3,
    QUA_VPU_VOMODE_TILE256      = 4,
    QUA_VPU_VOMODE_RGB888       = 5,
    QUA_VPU_VOMODE_ARGB888      = 5,
    QUA_VPU_VOMODE_YUYV         = 6,
    QUA_VPU_VOMODE_RRGGBB       = 7,
    QUA_VPU_VOMODE_NV16         = 10,
    QUA_VPU_VOMODE_Y            = 11,
    QUA_VPU_VOMODE_UYVY         = 12,
    QUA_VPU_VOMODE_YU12         = 13,
    QUA_VPU_VOMODE_RGB          = 14,
    QUA_VPU_VOMODE_BAYER_RAW    = 15,
    QUA_VPU_VOMODE_10BIT_NV12   = 16,
    QUA_VPU_VOMODE_10BIT_NV16   = 17,
    QUA_VPU_VOMODE_BUTT,
    QUA_VPU_VOMODE_DUMMY    = 0xffffffff,
} qua_vpu_vo_mode_t;

typedef enum qua_chn_mode {
    QUA_CHN_MODE_AUTO = 0,
    QUA_CHN_MODE_USER
} qua_chn_mode_t;

typedef enum qua_ds_mode {
    QUA_DS_NONE          = 1,
    QUA_DS_ONE_FOURTH    = 4,
    QUA_DS_ONE_EIGHTH    = 8,
    QUA_DS_ONE_SIXTEENTH = 16
} qua_ds_mode_t;

typedef enum qua_vgs_crop_coordinate {
    QUA_VGS_CROP_RATIO_COOR = 0,
    QUA_VGS_CROP_ABS_COOR
} qua_vgs_crop_coordinate_t;

typedef enum qua_vpu_crop_sel {
    QUA_VPU_CROP_SCALER = 0,
    QUA_VPU_CROP_VI     = 1,
    QUA_VPU_CROP_CHN    = 2,
} qua_vpu_crop_sel_t;

typedef enum qua_scale_algorithm {
    QUA_SCALE_ALGO_DEFAULT,
    QUA_SCALE_ALGO_FASTBILINEAR = QUA_SCALE_ALGO_DEFAULT,
    QUA_SCALE_ALGO_NEAST,
    QUA_SCALE_ALGO_BILINEAR,
    QUA_SCALE_ALGO_BICUBIC,
    QUA_SCALE_ALGO_LANCZOS,
} qua_scale_algorithm_t;

typedef enum qua_filter_mode {
  QUA_FILTER_MODE_NONE = 0,
  QUA_FILTER_MODE_LINEAR = 1,
  QUA_FILTER_MODE_BILINEAR = 2,
  QUA_FILTER_MODE_BOX = 3
} qua_filter_mode_t;

typedef struct qua_vpss_grp_attr {
    QUA_U32 max_width;
    QUA_U32 max_height;
    QUA_U32 ycmean_en;
    QUA_U32 ycmean_ds;
    qua_pixel_fmt_t pix_format;
    qua_vpss_die_mode_t die_mode;
    QUA_BOOL nr_enable;
    QUA_BOOL en_enable;
    QUA_BOOL apc_enable;
    QUA_BOOL y_gamma_enable;
    QUA_BOOL purple_enable;
    QUA_BOOL cnr_enable;
    QUA_BOOL chroma_enable;
    QUA_BOOL lc_enable;
    QUA_BOOL hist_enable;
    QUA_BOOL ie_enable;
    QUA_BOOL dci_enable;
    QUA_BOOL es_enable;
} qua_vpss_grp_attr_t;

typedef struct qua_vpss_ie_param {
    QUA_U32 ie_ctr;
    QUA_U32 ie_brt;
    QUA_U32 ie_husin;
    QUA_U32 ie_hucos;
    QUA_U32 ie_Sat;
} qua_vpss_ie_param_t;

typedef struct qua_vpss_chrome_info {
    QUA_U32 sat_para_r1;
} qua_vpss_chrome_info_t;

typedef struct qua_lc_cfg {
    QUA_U32 ellipse_coef_d;
    QUA_U32 ellipse_coef_e;
    QUA_U32 ellipse_coef_f;
    QUA_U32 flag_a;
    QUA_U32 flag_b;
    QUA_U32 flag_c;
    QUA_U32 tan_b;
    QUA_U32 tan_c;
} qua_lc_cfg_t;

typedef struct qua_vpss_lc_info {
    qua_lc_cfg_t lc_cfg[3];
} qua_vpss_lc_info_t;

typedef struct qua_vpss_grp_effect {
    QUA_U32 brightness;
    QUA_U32 contrast;
    QUA_U32 saturation;
    QUA_U32 ie_strength;
} qua_vpss_grp_effect_t;

typedef struct qua_split_info {
    qua_rect_t rect;
    QUA_U64 phy_addr_out[2];
    QUA_U32 stride_out[2];
} qua_split_info_t;

typedef struct qua_vpss_src_split_info {
    QUA_BOOL split_enable;
    QUA_U32 split_num;
    qua_split_info_t dst_split_info[8];
    qua_size_t src_size;
    qua_size_t dst_size;
} qua_vpss_src_split_info_t;

typedef struct qua_vpss_chn_attr {
    QUA_BOOL sp_enable;
    QUA_BOOL uv_invert;
    QUA_BOOL border_enable;
    qua_border_t border;
    qua_crop_info_t crop_info;
    QUA_BOOL scaler_crop_enable;
    qua_size_t src_size;
    qua_size_t dst_size;
} qua_vpss_chn_attr_t;

typedef struct qua_vpss_chn_globle_cfg {
    QUA_U32 use_globle;
    QUA_S32 glob_idx;
    QUA_U32 total_num;
    QUA_U32 index;
    qua_point_t pic_pos;
    qua_size_t pic_size;
} qua_vpss_chn_globle_cfg_t;

typedef struct qua_vpss_frame_rate {
    QUA_S32 src_frmrate;
    QUA_S32 dst_frmrate;
} qua_vpss_frame_rate_t;

typedef struct qua_main_chn_info {
    QUA_U32 ycmean_enable;
    qua_ds_mode_t ycmean_mode;
    QUA_U32 bgm_y_enable;
    QUA_U32 cpy_enable;
} qua_main_chn_info_t;

typedef struct qua_ai_ycbcr_to_rgb {
    QUA_U32 y2r_gain;
    QUA_U32 cb2r_gain;
    QUA_U32 cr2r_gain;
    QUA_U32 y2g_gain;
    QUA_U32 cb2g_gain;
    QUA_U32 cr2g_gain;
    QUA_U32 y2b_gain;
    QUA_U32 cb2b_gain;
    QUA_U32 cr2b_gain;
    QUA_U32 compensate_r;
    QUA_U32 compensate_g;
    QUA_U32 compensate_b;
} qua_ai_ycbcr_to_rgb_t;

typedef struct qua_ai_rgb_clamp {
    QUA_U32 r_offset;
    QUA_U32 g_offset;
    QUA_U32 b_offset;
} qua_ai_rgb_clamp_t;

typedef struct qua_ai_chn_info {
    QUA_U32 ai_out_sel;
    QUA_U32 rgb_out_mode;
    QUA_U32 rgb_clamp_mode;
    qua_ai_ycbcr_to_rgb_t y_cbcr_to_rgb_cfg;
    qua_ai_rgb_clamp_t rgb_clamp_cfg;
} qua_ai_chn_info_t;

typedef struct qua_vpss_chn_mode {
    qua_chn_mode_t chn_mode;
    QUA_U32 width;
    QUA_U32 height;
    QUA_BOOL field_frame_transfer;
    qua_aspect_ratio_info_t aspect_ratio_info;
    qua_pixel_fmt_t pixel_format;
    qua_compress_mode_t compress_mode;
    qua_compress_ratio_t comp_ratio;
    qua_vpss_chn_globle_cfg_t globe_cfg;
    qua_vpss_frame_rate_t frame_rate;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 yc_mean_sel;
    QUA_U32 out_clip_short;

    union {
        qua_main_chn_info_t main_cfg_info;
        qua_ai_chn_info_t ai_cfg_info;
    };
} qua_vpss_chn_mode_t;

typedef struct qua_mask_cfg {
    QUA_U32 masaic_mode;
    QUA_U32 color;
    QUA_U32 masaic_size;
} qua_mask_cfg_t;

typedef struct qua_vpss_mask_info {
    QUA_U32 mask_enable[QUA_VPSS_MAX_MASK_AREA];
    qua_rect_t area_value[QUA_VPSS_MAX_MASK_AREA];
    qua_mask_cfg_t masaic;
} qua_vpss_mask_info_t;

typedef struct qua_vpss_logo_cfg {
    qua_pixel_fmt_t logo_piexlfmt;
    QUA_U32 cmp_enable;
    QUA_U64 logo_addr;
    qua_point_t logo_start_pos;
    qua_point_t logo_cut_pos;
    QUA_U32 stride;
    qua_size_t logo_size;
    qua_point_t cmp_logo_startpos;
    qua_size_t cmp_logo_size;
    QUA_U32 to_fullen;
    QUA_U32 ext_mode;
    QUA_U32 pic_hdtv;
    QUA_U32 rgb_mode;
    QUA_U32 global_alpha_enable;
    QUA_U32 global_alpha;
    QUA_U32 magic_color_enable;
    QUA_U32 magic_color;
} qua_vpss_logo_cfg_t;

typedef struct qua_vpss_logo_info {
    QUA_U32 logo_idx;
    QUA_U32 logo_enable;
    qua_vpss_logo_cfg_t logo_cfg;
} qua_vpss_logo_info_t;

typedef struct qua_vpss_logo_color_info {
    QUA_U32 logo_idx;
    QUA_U32 idx_offset;
    QUA_U32 color[QUA_VPSS_MAX_GOSD_COLOR_IDX/2];
} qua_vpss_logo_color_info_t;

typedef struct qua_vgs_chn_mode {
    qua_chn_mode_t chn_mode;
    QUA_U32 width;
    QUA_U32 height;
    qua_pixel_fmt_t pixel_format;
} qua_vgs_chn_mode_t;

typedef struct qua_vgs_chn_status {
    QUA_U32 got_frm_num;
    QUA_U32 skip_frm_num;
    QUA_U32 send_frm_ok;
    QUA_U32 send_frm_fail;
} qua_vgs_chn_status_t;

typedef struct qua_vgs_crop {
    qua_crop_info_t crop_info;
    qua_vgs_crop_coordinate_t crop_coordinate;
} qua_vgs_crop_t;

typedef struct qua_vpu_size {
    qua_size_t size;
    qua_crop_info_t crop_info;
} qua_vpu_size_t;

typedef struct qua_vpu_chn_info {
    QUA_U32 bgm_enable;
    QUA_U32 cpy_enable;
    QUA_U32 sad_enable;
    QUA_U32 bgm_ds;
    QUA_U32 out_mode;
    QUA_U32 support_mode;
    qua_size_t chn_max_size;
    QUA_U32 bufnum;
    QUA_U32 max_stride;
} qua_vpu_chn_info_t;

typedef struct qua_vpu_chn_config {
    qua_size_t chn_size;
    qua_crop_info_t crop_area;
    QUA_U32 stride;
    QUA_U32 offset;
    QUA_U32 depth;
} qua_vpu_chn_config_t;

typedef struct qua_vpu_ext_chn_config {
    QUA_U32 bind_chn;
    qua_vpu_chn_config_t chn_info;
} qua_vpu_ext_chn_config_t;

typedef struct qua_addr_info
{
    QUA_U8* addr;
    QUA_U32 size;
} qua_addr_info_t;

typedef struct qua_mem_info
{
    QUA_U32 base;
    QUA_VOID_PTR vbase;
    QUA_U32 size;
} qua_mem_info_t;

typedef struct qua_vpu_onebuf_data {
    qua_mem_info_t data;
    QUA_U32 stride;
} qua_vpu_onebuf_data_t;

typedef struct qua_vpu_yc_data {
    qua_vpu_onebuf_data_t luma;
    qua_vpu_onebuf_data_t chroma;
} qua_vpu_yc_data_t;

typedef struct qua_vpu_rrggbb_data
{
    qua_vpu_onebuf_data_t r_data;
    qua_vpu_onebuf_data_t g_data;
    qua_vpu_onebuf_data_t b_data;
} qua_vpu_rrggbb_data_t;

typedef struct qua_vpu_stream_adv {
    qua_vpu_vo_mode_t data_format;
    qua_size_t size;
    QUA_U64 pts;
    QUA_U32 pool_id;
    union
    {
        qua_vpu_yc_data_t frm_blk;
        qua_vpu_yc_data_t frm_scan;
        qua_vpu_yc_data_t frm_nv16;
        qua_vpu_onebuf_data_t frm_tile192;
        qua_vpu_onebuf_data_t frm_tile224;
        qua_vpu_onebuf_data_t frm_tile256;
        qua_vpu_onebuf_data_t frm_rgb888;
        qua_vpu_onebuf_data_t frm_yuyv;
        qua_vpu_onebuf_data_t frm_y;
        qua_vpu_rrggbb_data_t frm_rrggbb;
    };
} qua_vpu_stream_adv_t;

typedef struct qua_vpu_apc {
    QUA_U32 enable;
    QUA_U32 mergesel;
    QUA_U32 p_gain;
    QUA_U32 n_gain;
    QUA_U32 detail_p_gain;
    QUA_U32 detail_n_gain;
    QUA_U32 detail_th_l;
    QUA_U32 detail_th_h;
    QUA_U32 detail_filter_idx;
    QUA_U32 detail_lut_idx;
    QUA_U32 edge_p_gain;
    QUA_U32 edge_n_gain;
    QUA_U32 edge_th_l;
    QUA_U32 edge_th_h;
    QUA_U32 edge_filter_idx;
    QUA_U32 edge_lut_idx;
} qua_vpu_apc_t;

typedef struct qua_vpu_rgb_pre {
    QUA_U32 enable;
    QUA_U32 means_r;
    QUA_U32 means_g;
    QUA_U32 means_b;
    QUA_U32 gain;
} qua_vpu_rgb_pre_t;

typedef struct qua_vpu_user_pic {
    qua_size_t pic_size;
    QUA_U32 yluma;
    QUA_U32 chroma;
    QUA_U32 ystride;
    QUA_U32 cstride;
    QUA_U32 pts;
    qua_vpu_vo_mode_t data_format;
    QUA_U32 work_mode;
    QUA_U32 pool_id;
} qua_vpu_user_pic_t;

typedef struct qua_vgs_task_attr {
    qua_video_frame_info_t img_in;
    qua_video_frame_info_t img_out;
    QUA_U32 private_data[4];
    QUA_U32 reserved;
} qua_vgs_task_attr_t;

typedef struct qua_vgs_line {
    qua_point_t start_point;
    qua_point_t end_point;
    QUA_U32 thick;
    QUA_U32 color;
} qua_vgs_line_t;

typedef struct qua_vgs_rotate {
    QUA_BOOL mirror;
    QUA_BOOL flip;
    QUA_BOOL rotate;
} qua_vgs_rotate_t;

typedef struct qua_vgs_osd {
    qua_rect_t rect;
    QUA_U32 bg_color;
    qua_pixel_fmt_t pixel_fmt;
    QUA_U32 phy_addr;
    QUA_U32 stride;
    QUA_U32 bg_alpha;
    QUA_U32 fg_alpha;
} qua_vgs_osd_t;

typedef struct qua_logov2_cfg {
    QUA_U32 magic_color_en;
    QUA_U32 magic_color;
    QUA_U32 global_alpha_en;
    QUA_U32 global_alpha;
    QUA_U32 rgb16_type;
    QUA_U32 extmode;
    QUA_U32 dtvmode;
    QUA_U32 rgbmode;
    qua_size_t logo_size;
} qua_logov2_cfg_t;

typedef struct qua_logov2 {
    QUA_U32 logo_enable;
    QUA_U32 logo_idx;
    QUA_U32 logo_pixdepth;
    QUA_U32 logo_addr;
    qua_logov2_cfg_t logo_cfg;
    qua_point_t logo_startpos;
    QUA_U32 stride_value;
    QUA_U32 color[256];
} qua_logov2_t;

typedef struct qua_ldc {
    QUA_U32 bEnable;
    QUA_U32 centerX;
    QUA_U32 centerY;
    QUA_U32 distortion_ratio;
} qua_ldc_t;

typedef struct qua_frame_rate{
    QUA_U16 frame_count;
    QUA_U16 frame_time;
} qua_frame_rate_t;

typedef struct qua_frame_ctr{
    QUA_U32           enable;
    qua_frame_rate_t  src_fps;
    qua_frame_rate_t  dst_fps;
} qua_frame_ctrl_t;

#define QUA_VISE_MAX_CANVAS_NU (4)

typedef struct qua_vise_canvas_area{
    QUA_U32      layer;
    qua_rect_t   area;
    QUA_U32      extcopy;
}qua_vise_canvas_area_t;

typedef struct qua_vise_canvas_attr{
    qua_size_t             size;
    QUA_U32                offset;
    QUA_U32                stride;
    QUA_U32                depth;
    QUA_U32                pipeline_num;
    qua_vpu_vo_mode_t      data_format;
    QUA_U32                strict_pts_chk;
    qua_frame_ctrl_t       frame_ctrl;
    QUA_U32                canvas_area_num;
    qua_vise_canvas_area_t canvas_area[QUA_VISE_MAX_CANVAS_NU];
    QUA_U32                canvas_lpbuf_en;
}qua_vise_canvas_attr_t;

typedef struct qua_vise_canvas_status{
    QUA_U32    finish_cnt;
    QUA_U32    error_lost_cnt;
    QUA_U32    nobuf_lost_cnt;
    qua_frame_rate_t status_fps;
    QUA_U32    area_recv_cnt[QUA_VISE_MAX_CANVAS_NU];
    QUA_U32    area_finish_cnt[QUA_VISE_MAX_CANVAS_NU];
    QUA_U32    area_fail_cnt[QUA_VISE_MAX_CANVAS_NU];
}qua_vise_canvas_status_t;

typedef struct qua_mm_filter_module {
    struct qua_mm_module_t parent;
} qua_mm_filter_module_t;

typedef struct qua_mm_vgs_device {
    struct qua_mm_device_t parent;

    QUA_S32 (*qua_vgs_begin_job)(QUA_VGS_HANDLE *handle);
    QUA_S32 (*qua_vgs_end_job)(QUA_VGS_HANDLE handle);
    QUA_S32 (*qua_vgs_cancel_job)(QUA_VGS_HANDLE handle);
    QUA_S32 (*qua_vgs_add_scale_task)(QUA_VGS_HANDLE handle, qua_vgs_task_attr_t *task_attr);
    QUA_S32 (*qua_vgs_add_draw_line_task)(QUA_VGS_HANDLE handle, qua_vgs_task_attr_t *task_attr, qua_vgs_line_t lines[], QUA_U32 num);
    QUA_S32 (*qua_vgs_add_osd_task)(QUA_VGS_HANDLE handle, qua_vgs_task_attr_t *task_attr, qua_vgs_osd_t osd[], QUA_U32 num);
    QUA_S32 (*qua_vgs_add_combine_task)(QUA_VGS_HANDLE handle, qua_video_frame_t *frame_in, qua_rect_t *rect_in, qua_video_frame_t *frame_out, qua_rect_t *rect_out);
    QUA_S32 (*qua_vgs_decompress)(QUA_VGS_HANDLE handle, const qua_video_frame_t *frame_in, qua_video_frame_t *frame_out);
    QUA_S32 (*qua_vgs_format_convert)(QUA_VGS_HANDLE handle, qua_video_frame_t *frame, qua_tde2_surface_t *surface, QUA_BOOL frm_to_sur);
    QUA_S32 (*qua_vgs_format_convert_new)(QUA_VGS_HANDLE handle, qua_video_frame_t *frame_in, qua_video_frame_t *frame_out);
    QUA_S32 (*qua_vgs_rotate)(qua_vgs_task_attr_t *task_attr, qua_vgs_rotate_t *rotate);
    QUA_S32 (*qua_vgs_rotate_new)(qua_vgs_task_attr_t *task_attr, qua_vgs_rotation_t rotation);
    QUA_S32 (*qua_vgs_fill_color)(qua_video_frame_info_t *frame_info, qua_rect_t *rect, QUA_U32 rgb);
    QUA_S32 (*qua_vgs_do_copy)(qua_vgs_task_attr_t *task_attr);
    QUA_S32 (*qua_vgs_set_scale_mode)(qua_vgs_scale_mode_t scale_mode);
} qua_mm_vgs_device_t;

typedef struct qua_mm_vppu_device {
    struct qua_mm_device_t parent;

    QUA_S32 (*init)();
    QUA_S32 (*exit)();

    QUA_S32 (*get_grp_fd)(QUA_GROUP group_id);
    QUA_S32 (*set_vi_attr)(QUA_GROUP group_id, qua_vpu_size_t *size);
    QUA_S32 (*get_vi_attr)(QUA_GROUP group_id, qua_vpu_size_t *size);
    QUA_S32 (*create_grp)(QUA_GROUP group_id, qua_vpss_grp_attr_t *grp_attr);
    QUA_S32 (*destroy_grp)(QUA_GROUP group_id);
    QUA_S32 (*enable)(QUA_GROUP group_id, qua_vpu_vi_mode_t vi_mode);
    QUA_S32 (*disable)(QUA_GROUP group_id);
    QUA_S32 (*freeze_video)(QUA_GROUP group_id);
    QUA_S32 (*unfreeze_video)(QUA_GROUP group_id);
    QUA_S32 (*set_global_ldc)(QUA_GROUP group_id, qua_ldc_t *ldc);
    QUA_S32 (*get_global_ldc)(QUA_GROUP group_id, qua_ldc_t *ldc);
    QUA_S32 (*set_global_graph_v2_timeout)(QUA_GROUP group_id, QUA_U32 timeout, qua_logov2_t *logov2);
    QUA_S32 (*set_global_graph_v2)(QUA_GROUP group_id, qua_logov2_t *logov2);
    QUA_S32 (*get_global_graph_v2)(QUA_GROUP group_id, qua_logov2_t *logov2);
    QUA_S32 (*set_global_crop)(QUA_GROUP group_id, qua_vpu_crop_sel_t sel, qua_crop_info_t *crop_info);
    QUA_S32 (*get_global_crop)(QUA_GROUP group_id, qua_vpu_crop_sel_t sel, qua_crop_info_t *crop_info);
    QUA_S32 (*get_grp_attr)(QUA_GROUP group_id, qua_vpss_grp_attr_t *grp_attr);
    QUA_S32 (*set_grp_attr)(QUA_GROUP group_id,  qua_vpss_grp_attr_t *grp_attr);
    QUA_S32 (*start_grp)(QUA_GROUP group_id);
    QUA_S32 (*stop_grp)(QUA_GROUP group_id);
    QUA_S32 (*set_ie)(QUA_GROUP group_id, qua_vpss_ie_param_t *ie_param);
    QUA_S32 (*get_ie)(QUA_GROUP group_id, qua_vpss_ie_param_t *ie_param);
    QUA_S32 (*set_chroma)(QUA_GROUP group_id, qua_vpss_chrome_info_t *chroma_info);
    QUA_S32 (*get_chroma)(QUA_GROUP group_id, qua_vpss_chrome_info_t *chroma_info);
    QUA_S32 (*set_lc)(QUA_GROUP group_id, qua_vpss_lc_info_t *lc_info);
    QUA_S32 (*get_lc)(QUA_GROUP group_id, qua_vpss_lc_info_t *lc_info);
    QUA_S32 (*set_grp_effect)(QUA_GROUP group_id, qua_vpss_grp_effect_t *grp_effect);
    QUA_S32 (*get_grp_effect)(QUA_GROUP group_id, qua_vpss_grp_effect_t *grp_effect);
    QUA_S32 (*set_src_split_info)(QUA_GROUP group_id, qua_vpss_src_split_info_t *split_info);
    QUA_S32 (*get_src_split_info)(QUA_GROUP group_id, qua_vpss_src_split_info_t *split_info);
    QUA_S32 (*send_frame)(QUA_GROUP group_id, qua_video_frame_info_t *frame, QUA_S32 timeout);
    QUA_S32 (*send_user_pic)(QUA_GROUP group_id, qua_vpu_user_pic_t *pic);
    QUA_S32 (*get_frame_blk_size)(QUA_U32 width, QUA_U32 height, QUA_U32 support_mode, QUA_U32 *blk_size);
    QUA_S32 (*enable_user_frame_rate_ctrl)(QUA_GROUP group_id);
    QUA_S32 (*disable_user_frame_rate_ctrl)(QUA_GROUP group_id);
    QUA_S32 (*get_grp_frame)(QUA_GROUP group_id, qua_video_frame_info_t *frame, QUA_S32 frame_index);
    QUA_S32 (*release_grp_frame)(QUA_GROUP group_id, qua_video_frame_info_t *frame);
    QUA_S32 (*image_convert)(const qua_video_frame_t *frame_in, qua_video_frame_t *frame_out);
    // QUA_S32 (*format_convert)(qua_video_frame_t *frame, TDE2_SURFACE_S *surface, QUA_BOOL frm_to_sur);
} qua_mm_vppu_device_t;

typedef struct qua_mm_swav_device {
    struct qua_mm_device_t parent;
    QUA_SWAV_HANDLE handle;

    QUA_S32 (*begin_job)(QUA_SWAV_HANDLE handle);
    QUA_S32 (*end_job)(QUA_SWAV_HANDLE handle);
    // video
    QUA_S32 (*add_scale_task)(QUA_SWAV_HANDLE handle, qua_size_t *in_size, qua_size_t *out_size, qua_scale_algorithm_t flags);
    QUA_S32 (*add_crop_task)(QUA_SWAV_HANDLE handle, qua_rect_t *rect);
    QUA_S32 (*add_vflip_task)(QUA_SWAV_HANDLE handle);
    QUA_S32 (*add_hflip_task)(QUA_SWAV_HANDLE handle);
    QUA_S32 (*process_video)(QUA_SWAV_HANDLE handle, qua_video_frame_t *in, qua_video_frame_t *out, QUA_S32 in_align, QUA_S32 out_align);
    // audio
    QUA_S32 (*add_resample_task)(QUA_SWAV_HANDLE handle, QUA_S32 in_sr, QUA_S32 out_sr);
    QUA_S32 (*process_audio)(QUA_SWAV_HANDLE handle, qua_audio_frame_t *in, qua_audio_frame_t *out);
} qua_mm_swav_device_t;

typedef struct qua_mm_swproc_device {
    struct qua_mm_device_t parent;

    QUA_S32 (*scale)(qua_video_frame_t *in, qua_video_frame_t *out, qua_filter_mode_t mode);
    QUA_S32 (*convert)(qua_video_frame_t *in, qua_video_frame_t *out);
    QUA_S32 (*attenuate)(qua_video_frame_t *in, qua_video_frame_t *out);
    QUA_S32 (*blend)(qua_video_frame_t *fg, qua_video_frame_t *bg, qua_video_frame_t *out);
    QUA_S32 (*copy)(qua_video_frame_t *in, qua_video_frame_t *out);
    QUA_S32 (*rotate)(qua_video_frame_t *in, qua_video_frame_t *out, int angle);
} qua_mm_swproc_device_t;

typedef struct qua_mm_vpss_channel {
    struct qua_mm_channel_t parent;

    QUA_S32 (*get_chn_fd)(QUA_GROUP group_id, QUA_S32 chn_id);
    QUA_S32 (*enable_chn)(QUA_GROUP group_id, QUA_S32 chn_id);
    QUA_S32 (*disable_chn)(QUA_GROUP group_id, QUA_S32 chn_id);
    QUA_S32 (*set_chn_attr)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_chn_attr_t *chn_attr);
    QUA_S32 (*get_chn_attr)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_chn_attr_t *chn_attr);
    QUA_S32 (*set_chn_mode)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_chn_mode_t *chn_mode);
    QUA_S32 (*get_chn_mode)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_chn_mode_t *chn_mode);
    QUA_S32 (*set_mosaic)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_mask_info_t *mosaic);
    QUA_S32 (*get_mosaic)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_mask_info_t *mosaic);
    QUA_S32 (*set_pic)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_logo_info_t *logo);
    QUA_S32 (*get_pic)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_logo_info_t *logo);
    QUA_S32 (*set_pic_color)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_logo_color_info_t *color_info);
    QUA_S32 (*get_pic_color)(QUA_GROUP group_id, QUA_S32 chn_id, qua_vpss_logo_color_info_t *color_info);
    QUA_S32 (*set_depth)(QUA_GROUP group_id, QUA_S32 chn_id, QUA_U32 depth);
    QUA_S32 (*get_depth)(QUA_GROUP group_id, QUA_S32 chn_id, QUA_U32 *depth);
    QUA_S32 (*get_chn_frame)(QUA_GROUP group_id, QUA_S32 chn_id, qua_video_frame_info_t *frame, QUA_S32 timeout);
    QUA_S32 (*release_chn_frame)(QUA_GROUP group_id, QUA_S32 chn_id, qua_video_frame_info_t *frame);
} qua_mm_vpss_channel_t;

typedef struct qua_mm_vpu_channel {
    struct qua_mm_channel_t parent;
    QUA_S16 chn_id;
    QUA_GROUP group_id;

    QUA_S32 (*get_chn_frame_adv)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_stream_adv_t *stream_adv, QUA_U32 timeout_ms);
    QUA_S32 (*lock_chn_frame_adv)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_stream_adv_t *stream_adv, QUA_U32 timeout_ms, QUA_U32 *handle_lock);
    QUA_S32 (*get_chn_frame_adv_norpt)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_stream_adv_t *stream_adv, QUA_U32 timeout_ms);
    QUA_S32 (*lock_chn_frame_adv_norpt)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_stream_adv_t *stream_adv, QUA_U32 timeout_ms, QUA_U32 *handle_lock);
    QUA_S32 (*unlock_chn_frame_adv)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_stream_adv_t *stream_adv, QUA_U32 lock_handle);
    QUA_S32 (*set_chn_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_chn_config_t *chn_cfg);
    QUA_S32 (*get_chn_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_chn_config_t *chn_cfg);
    QUA_S32 (*set_ext_chn_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_ext_chn_config_t *ext_chn_cfg);
    QUA_S32 (*get_ext_chn_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_ext_chn_config_t *ext_chn_cfg);
    QUA_S32 (*set_vo_mode)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_vo_mode_t mode);
    QUA_S32 (*set_vo_rotate)(QUA_GROUP group_id, QUA_U32 chn_id, qua_rotate_t rotate);
    QUA_S32 (*open_chn)(QUA_GROUP group_id, QUA_U32 chn_id);
    QUA_S32 (*close_chn)(QUA_GROUP group_id, QUA_U32 chn_id);
    QUA_S32 (*set_frame_ctrl)(QUA_GROUP group_id, QUA_U32 chn_id,QUA_DOUBLE *frame_ctrl);
    QUA_S32 (*get_frame_ctrl)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_DOUBLE *frame_ctrl);
    QUA_S32 (*get_framerate)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_DOUBLE *fps);
    QUA_S32 (*set_chn_crop)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_crop_sel_t sel, qua_crop_info_t *crop_info);
    QUA_S32 (*get_chn_crop)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_crop_sel_t sel, qua_crop_info_t *crop_info);
    QUA_S32 (*set_scaler_coeff)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_U32 level);
    QUA_S32 (*get_frame_buffer_size)(QUA_GROUP group_id, QUA_U32 chn_id,QUA_U32 bufnum, QUA_U32 *needsize);
    QUA_S32 (*framebuffer_register)(QUA_GROUP group_id, QUA_U32 chn_id,QUA_U32 bufnum, qua_mem_info_t buf);
    QUA_S32 (*framebuffer_unRegister)(QUA_GROUP group_id, QUA_U32 chn_id, qua_mem_info_t *buf);
    QUA_S32 (*attach_vb_pool)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_U32 vbpool);
    QUA_S32 (*detach_vb_pool)(QUA_GROUP group_id, QUA_U32 chn_id);
    QUA_S32 (*set_comm_vb_enable)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_U32 enable);
    QUA_S32 (*set_chn_graph_v2_timeout)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_U32 timeout, qua_logov2_t *logov2);
    QUA_S32 (*set_chn_graph_v2)(QUA_GROUP group_id, QUA_U32 chn_id, qua_logov2_t *logov2);
    QUA_S32 (*get_chn_graph_v2)(QUA_GROUP group_id, QUA_U32 chn_id, qua_logov2_t *logov2);
    QUA_S32 (*check_addr)(QUA_GROUP group_id, QUA_U32* addr, QUA_U32 size, QUA_U32 timeout_ms, QUA_U32 *isidle);
    QUA_S32 (*set_chn_vi_sel)(QUA_GROUP group_id, QUA_U32 chn_id, QUA_U32 sel);
    QUA_S32 (*set_chn_apc_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_apc_t *attr);
    QUA_S32 (*get_chn_apc_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_apc_t *attr);
    QUA_S32 (*set_rgb_pre_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_rgb_pre_t *attr);
    QUA_S32 (*get_rgb_pre_attr)(QUA_GROUP group_id, QUA_U32 chn_id, qua_vpu_rgb_pre_t *attr);
    QUA_S32 (*set_chn_rotation)(QUA_GROUP group_id, QUA_U32 chn_id, qua_rotate_t rotate);
} qua_mm_vpu_channel_t;

typedef struct qua_mm_vgs_channel {
    struct qua_mm_channel_t parent;

    QUA_S32 (*set_chn_sync_mode)(QUA_S32 vgs_chn_id, QUA_BOOL a_sync, QUA_U32 buf_depth);
    QUA_S32 (*set_chn_mode)(QUA_S32 vgs_chn_id, QUA_S32 vgs_path, qua_vgs_chn_mode_t *chn_mode);
    QUA_S32 (*get_chn_mode)(QUA_S32 vgs_chn_id, QUA_S32 vgs_path, qua_vgs_chn_mode_t *chn_mode);
    QUA_S32 (*set_chn_crop)(QUA_S32 vgs_chn_id, qua_vgs_crop_t *crop_info);
    QUA_S32 (*get_chn_crop)(QUA_S32 vgs_chn_id, qua_vgs_crop_t *crop_info);
    QUA_S32 (*set_chn_rotate)(QUA_S32 vgs_chn_id, QUA_S32 vgs_path, qua_rotate_t rotate);
    QUA_S32 (*get_chn_rotate)(QUA_S32 vgs_chn_id, QUA_S32 vgs_path, qua_rotate_t *rotate);
    QUA_S32 (*send_frame)(QUA_S32 vgs_chn_id, const qua_video_frame_info_t *frame, QUA_BOOL play_mode, QUA_U32 timeout);
    QUA_S32 (*query)(QUA_S32 vgs_chn_id, qua_vgs_chn_status_t *chn_sts);
} qua_mm_vgs_channel_t;

typedef struct qua_mm_vise_device {
    struct qua_mm_device_t parent;
    QUA_S32 (*create_canvas)(QUA_S32 chn_id, qua_vise_canvas_attr_t *attr);
    QUA_S32 (*destory_canvas)(QUA_S32 chn_id);
    QUA_S32 (*get_canvas_attr)(QUA_S32 chn_id, qua_vise_canvas_attr_t *attr);
    QUA_S32 (*set_canvas_attr)(QUA_S32 chn_id, qua_vise_canvas_attr_t *attr);
    QUA_S32 (*start)(QUA_S32 chn_id);
    QUA_S32 (*stop)(QUA_S32 chn_id);
    QUA_S32 (*get_chn_frame)(QUA_S32 chn_id, qua_video_frame_t *frame, QUA_S32 timeout_ms, QUA_S32 is_norpt);
    QUA_S32 (*release_chn_frame)(QUA_S32 chn_id, qua_video_frame_t *frame);
    QUA_S32 (*set_frame_ctrl)(QUA_S32 chn_id, qua_frame_ctrl_t *ctrl);
    QUA_S32 (*get_frame_ctrl)(QUA_S32 chn_id, qua_frame_ctrl_t *ctrl);
    QUA_S32 (*get_status)(QUA_S32 chn_id, qua_vise_canvas_status_t *status);
    QUA_S32 (*attach_vb_pool)(QUA_S32 chn_id, QUA_S32 vb_pool);
    QUA_S32 (*deattach_vb_pool)(QUA_S32 chn_id);
    QUA_S32 (*get_chn_fd)(QUA_S32 chn_id, QUA_S32 *fd);
    QUA_S32 (*close_chn_fd)(QUA_S32 chn_id);
} qua_mm_vise_device_t;


#endif // __QUA_MM_FILTER_H__
