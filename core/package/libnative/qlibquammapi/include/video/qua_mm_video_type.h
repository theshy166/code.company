/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_MM_VIDEO_TYPE_H__
#define __QUA_MM_VIDEO_TYPE_H__

#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

typedef struct qua_venc_attr_h264_cbr_t                 qua_venc_attr_h265_cbr_t;
typedef struct qua_venc_attr_h264_vbr_t                 qua_venc_attr_h265_vbr_t;
typedef struct qua_venc_attr_h264_avbr_t                qua_venc_attr_h265_avbr_t;
typedef struct qua_venc_attr_h264_fixqp_t               qua_venc_attr_h265_fixqp_t;
typedef struct qua_venc_attr_h264_qvbr_t                qua_venc_attr_h265_qvbr_t;
typedef struct qua_venc_attr_h264_qpmap_t               qua_venc_attr_h265_qpmap_t;
typedef struct qua_venc_stream_advance_info_h264_t      qua_venc_stream_advance_info_h265_t;

typedef enum {
    QUA_VIDEO_CodingUnused,
    QUA_VIDEO_CodingAVC = 96,
    QUA_VIDEO_CodingHEVC = 265,
    QUA_VIDEO_CodingJpeg = 26,
    QUA_VIDEO_CodingMJpeg = 1002,
    QUA_VIDEO_CodingMax,
} qua_coding_type_t;

typedef enum {
    QUA_JPEG = (1 << 0),
    QUA_MJPEG = (1 << 1),
    QUA_NORMAL_H264 = (1 << 2),
    QUA_SMART_H264 = (1 << 3),
    QUA_NORMAL_H265 = (1 << 4),
    QUA_SMART_H265 = (1 << 5),
    QUA_VENC_TYPE_DUMMY = 0xFFFFFFFF
} qua_venc_type_t;

typedef enum {
    QUA_H264_PROFILE_BASELINE = 66,
    QUA_H264_PROFILE_MAIN = 77,
    QUA_H264_PROFILE_HIGH = 100,
    QUA_H264_PROFILE_DUMMY = 0xFFFFFFFF
} qua_h264_profile_t;

typedef enum {
    QUA_H265_PROFILE_MAIN = 1,
    QUA_H265_PROFILE_DUMMY = 0xFFFFFFFF
} qua_h265_profile_t;

typedef enum {
    QUA_STREAM_JPEG = (1 << 0),
    QUA_STREAM_MJPEG = (1 << 1),
    QUA_STREAM_H264 = (1 << 2),
    QUA_STREAM_H265 = (1 << 3),
    QUA_STREAM_ALL = 0xFFFFFFFF,
    QUA_STREAM_TYPE_DUMMY = 0xFFFFFFFF
} qua_stream_type_t;

typedef enum {
    QUA_FRAME_P = 0,
    QUA_FRAME_B = 1,
    QUA_FRAME_I = 2,
    QUA_FRAME_FRESH_P = 20,
    QUA_FRAME_FRESH_B = 21,
    QUA_FRAME_SKIP_P = 30,
    QUA_ENC_FRAME_TYPE_DUMMY = 0xFFFFFFFF
} qua_enc_frame_type_t;

typedef enum {
    QUA_REF_IDR = 0,
    QUA_REF_P_REF_IDR = 1,
    QUA_REF_P_REF_M = 2,
    QUA_REF_P_REF_N = 3,
    QUA_REF_P_NOT_REF = 4,
    QUA_ENC_REF_TYPE_DUMMY = 0xFFFFFFFF
} qua_enc_ref_type_t;

typedef enum {
    QUA_NALU_P_SLICE = 0,
    QUA_NALU_B_SLICE = 1,
    QUA_NALU_I_SLICE = 2,
    QUA_NALU_IDR = 5,
    QUA_NALU_SEI = 6,
    QUA_NALU_SPS = 7,
    QUA_NALU_PPS = 8,
    QUA_NALU_AUD = 9,
    QUA_NALU_H265_VPS_NUT = 32,
    QUA_NALU_H265_SPS_NUT = 33,
    QUA_NALU_H265_PPS_NUT = 34,
    QUA_NALU_H265_PREFIX_SEI_NUT = 39,
    QUA_ENC_NALU_TYPE_DUMMY = 0xFFFFFFFF
} qua_enc_nalu_type_t;

typedef enum {
    QUA_STM_PUB_BUF = 0,
    QUA_STM_CHN_BUF = 1,
    QUA_STM_NOT_INIT = 2,
    QUA_ENC_STREAM_BUF_MODE_DUMMY = 0xFFFFFFFF
} qua_enc_stream_buf_mode_t;

typedef enum {
    QUA_ENC_PARAM_CMD_NONE,
    QUA_ENC_PARAM_CMD_H264_LINE_RC = 1,
    QUA_ENC_PARAM_CMD_LINE_RC = 1,
    QUA_ENC_PARAM_CMD_H264_SEARCH_RANGE = 2,
    QUA_ENC_PARAM_CMD_H264_CHROMAQP_DELTA = 3,
    QUA_ENC_PARAM_CMD_H264_ENCODE_STYLE = 4,
    QUA_ENC_PARAM_CMD_TEXTUREQP_RANGE = 5,
    QUA_ENC_PARAM_CMD_RC_ADV_PARAM = 6,
    QUA_ENC_PARAM_CMD_H265_LAMBDA = 7,
    QUA_ENC_PARAM_CMD_LAMBDA = 7,
    QUA_ENC_PARAM_CMD_H265_CHROMAQP_DELTA = 8,
    QUA_ENC_PARAM_CMD_SMART_GOPMODE = 9,
    QUA_ENC_PARAM_CMD_BGMMODE = 10,
    QUA_ENC_PARAM_CMD_ADV_BKGQP = 11,
    QUA_ENC_PARAM_CMD_BIGPFRM = 12,
    QUA_ENC_PARAM_CMD_GOP = 13,
    QUA_ENC_PARAM_CMD_DUMMY = 0xFFFFFFFF
} qua_enc_param_cmd_t;

typedef struct qua_enc_param_h264_line_rc_t {
    QUA_U32 linerc_en;
    QUA_U32 linerc_iadd;
    QUA_U32 linerc_iminus;
    QUA_U32 linerc_padd;
    QUA_U32 linerc_pminus;
} qua_enc_param_h264_line_rc_t;

typedef struct qua_enc_param_h264_search_range_t {
    QUA_U32 search_x;
    QUA_U32 search_y;
} qua_enc_param_h264_search_range_t;

typedef struct qua_enc_param_h265_lambda_t {
    QUA_U32 i_lambda_weight;
    QUA_U32 p_lambda_weight;
    QUA_U32 rp_lambda_weight;
} qua_enc_param_h265_lambda_t;

typedef struct qua_enc_param_h264_chroma_qp_delta_t {
    QUA_S32 chroma_qp_delta;
    QUA_S32 second_chroma_qp_delta;
} qua_enc_param_h264_chroma_qp_delta_t;

typedef struct qua_enc_param_h265_chroma_qp_delta_t {
    QUA_S32 chroma_cb_qp_delta;
    QUA_S32 chroma_cr_qp_delta;
} qua_enc_param_h265_chroma_qp_delta_t;

typedef struct qua_enc_param_texture_qp_range_t {
    QUA_U32 text_min_qp_minus;
    QUA_U32 text_max_qp_add;
} qua_enc_param_texture_qp_range_t;

typedef struct qua_enc_param_rc_adv_param_t {
    QUA_S32 i_qp_weight_tbit;
    QUA_S32 i_qp_weight_pqp;
    QUA_S32 p_qp_limit;
    QUA_U32 vbuf_depth;
    QUA_U32 avbr_strength;
} qua_enc_param_rc_adv_param_t;

typedef struct qua_enc_param_smart_gop_mode_t {
    QUA_U32 smart_gop_mode;
    QUA_U32 dualp_interval;
} qua_enc_param_smart_gop_mode_t;

typedef struct qua_enc_param_bgm_mode_t {
    QUA_U32 use_bgm_info;
    QUA_U32 cycle_no_bgm;
    QUA_U32 bgm_chn;
} qua_enc_param_bgm_mode_t;

typedef struct qua_enc_param_adv_bkg_qp_t {
    QUA_U32 idx;
    QUA_U32 bkg_delta_qp;
    QUA_U32 bkg_max_qp;
} qua_enc_param_adv_bkg_qp_t;

typedef struct qua_enc_param_big_p_frm_t {
    QUA_U32 delta_p_qp_interval;
    QUA_U32 delta_p_qp_delta;
    QUA_U32 delta_p_qp_lambda;
} qua_enc_param_big_p_frm_t;

typedef struct qua_enc_param_gop_t {
    QUA_U32 i_interval;
} qua_enc_param_gop_t;

typedef struct qua_h264_entropy_t {
    QUA_U32 entropy_coding_mode;
    QUA_U32 cabac_init_idc;
} qua_h264_entropy_t;

typedef struct qua_h265_entropy_t {
    QUA_U32 cabac_init_flag;
    QUA_U32 cabac_init_idc;
} qua_h265_entropy_t;

typedef struct qua_h264_dblk_t {
    QUA_U32 deblocking_filter;
    QUA_U32 disable_deblocking;
    QUA_S32 slice_alpha;
    QUA_S32 slice_beta;
} qua_h264_dblk_t;

typedef struct qua_h265_dblk_t {
    QUA_U32 disable_flag;
    QUA_S32 beta_offset_div2;
    QUA_S32 tc_offset_div2;
} qua_h265_dblk_t;

typedef struct qua_debreath_t {
    QUA_U32 debreath_en;
    QUA_U32 debreath_ratio;
} qua_debreath_t;

typedef enum {
    QUA_ENCODE_STYLE_NORMAL = 0,
    QUA_ENCODE_STYLE_NIGHT = 1,
    QUA_ENC_PARAM_H264_ENCODE_STYLE_DUMMY = 0xFFFFFFFF
} qua_enc_param_h264_encode_style_t;

typedef struct qua_enc_param_h264_line_rc_t qua_enc_line_rc_t;
typedef struct qua_enc_param_h265_lambda_t qua_enc_param_lambda_t;

typedef union qua_enc_param_union_t {
    qua_enc_param_h264_line_rc_t h264_line_rc;
    qua_enc_line_rc_t line_rc;
    qua_enc_param_h264_search_range_t h264_search_range;
    qua_enc_param_h264_chroma_qp_delta_t h264_chroma_qp;
    qua_enc_param_h265_chroma_qp_delta_t h265_chroma_qp;
    qua_enc_param_h264_encode_style_t h264_encode_style;
    qua_enc_param_texture_qp_range_t texture_qp_range;
    qua_enc_param_rc_adv_param_t rc_adv;
    qua_enc_param_h265_lambda_t h265_lambda;
    qua_enc_param_lambda_t lambda;
    qua_enc_param_smart_gop_mode_t gop_mode;
    qua_enc_param_bgm_mode_t bgm_mode;
    qua_enc_param_adv_bkg_qp_t adv_bkg_qp;
    qua_enc_param_big_p_frm_t big_p_frm;
    qua_enc_param_gop_t gop;
} qua_enc_param_union_t;

typedef struct qua_venc_stm_config_t {
    qua_enc_stream_buf_mode_t stm_mode;
    QUA_U64 stm_addr;
    QUA_U32 stm_size;
    QUA_U32 max_stm_num;
} qua_venc_stm_config_t;

typedef enum {
    QUA_VIDEO_MODE_STREAM,
    QUA_VIDEO_MODE_FRAME,
    QUA_VIDEO_MODE_BUTT,
} qua_video_mode_t;

#define QUA_MAX_TH_NUM  5
typedef struct qua_gop_th_t{
    QUA_U32 gop_th_num;
    QUA_U32 th_val[QUA_MAX_TH_NUM];
    QUA_U32 min_gop[QUA_MAX_TH_NUM + 1];
} qua_gop_th_t;

typedef enum {
    QUA_JPG_COLOR_FMT_YCBCR400,
    QUA_JPG_COLOR_FMT_YCBCR420,
    QUA_JPG_COLOR_FMT_YCBCR422BHP,
    QUA_JPG_COLOR_FMT_YCBCR422BVP,
    QUA_JPG_COLOR_FMT_YCBCR444,
    QUA_JPG_COLOR_FMT_BUTT,
} qua_vdec_jpeg_color_fmt_t;

typedef struct qua_vdec_attr_jpeg_t {
    qua_video_mode_t mode;
    qua_vdec_jpeg_color_fmt_t jpeg_fmt;
} qua_vdec_attr_jpeg_t;

typedef struct qua_vdec_attr_video_t {
    QUA_U32 ref_frame_num;
    qua_video_mode_t mode;
    QUA_BOOL temporal_mvp_enable;
} qua_vdec_attr_video_t;

typedef struct qua_vdec_chn_attr_t {
    qua_coding_type_t coding_type;
    QUA_U32 in_stream_buf_size;
    QUA_U32 priority;
    QUA_U32 pic_width;
    QUA_U32 pic_height;

    union {
        qua_vdec_attr_jpeg_t vdec_jpeg_attr;
        qua_vdec_attr_video_t vdec_video_attr;
    };
} qua_vdec_chn_attr_t;

typedef struct qua_vdec_stream_t {
    QUA_U64 in_data;
    QUA_U32 in_size;
    QUA_U64 pts;
    QUA_BOOL end_of_frame;
    QUA_BOOL end_of_stream;
} qua_vdec_stream_t;

typedef struct qua_decoder_error_t {
    QUA_S32 format_err;
    QUA_S32 pic_size_err_set;
    QUA_S32 stream_unsupport;
    QUA_S32 pack_err;
    QUA_S32 protocol_num_err_set;
    QUA_S32 ref_err_set;
    QUA_S32 pic_buf_size_err_set;
    QUA_S32 vdec_stream_not_release;
} qua_decoder_error_t;

typedef struct qua_vdec_chn_stat_t {
    qua_coding_type_t coding_type;
    QUA_U32 left_stream_bytes;
    QUA_U32 left_stream_frames;
    QUA_U32 left_pics;
    QUA_BOOL start_recv_stream;
    QUA_U32 recv_stream_frames;
    QUA_U32 decode_stream_frames;
    qua_decoder_error_t dec_err;
} qua_vdec_chn_stat_t;

typedef struct qua_vdec_chn_param_t {
    QUA_S32 display_frame_num;
    QUA_S32 chan_err_thr;
    QUA_S32 chan_strm_of_thr;
    QUA_S32 dec_mode;
    QUA_S32 dec_order_output;
    qua_video_format_t video_fmt;
    qua_compress_mode_t compress_mode;
} qua_vdec_chn_param_t;

typedef struct qua_vdec_drop_frame_cfg_t {
    QUA_BOOL enable;
    QUA_U8 max_num_ref_frames;
    qua_coding_type_t coding_type;
} qua_vdec_drop_frame_cfg_t;

typedef struct qua_venc_attr_mjpeg_t {
    QUA_U32 max_pic_width;
    QUA_U32 max_pic_height;
    QUA_U32 stream_buf_size;
    QUA_BOOL by_frame;

    QUA_U32 pic_width;
    QUA_U32 pic_height;
} qua_venc_attr_mjpeg_t;

typedef struct qua_venc_attr_jpeg_t {
    QUA_U32 max_pic_width;
    QUA_U32 max_pic_height;
    QUA_U32 stream_buf_size;
    QUA_BOOL by_frame;

    QUA_U32 pic_width;
    QUA_U32 pic_height;
    QUA_BOOL support_dcf;
} qua_venc_attr_jpeg_t;

typedef struct qua_venc_attr_h264_t {
    QUA_U32 max_pic_width;
    QUA_U32 max_pic_height;

    QUA_U32 stream_buf_size;
    qua_h264_profile_t profile;
    QUA_BOOL by_frame;

    QUA_U32 pic_width;
    QUA_U32 pic_height;

    QUA_U32 i_frame_interval;
} qua_venc_attr_h264_t;

typedef struct qua_venc_attr_h265_t {
    QUA_U32 max_pic_width;
    QUA_U32 max_pic_height;

    QUA_U32 stream_buf_size;
    qua_h265_profile_t profile;
    QUA_BOOL by_frame;

    QUA_U32 pic_width;
    QUA_U32 pic_height;

    QUA_U32 i_frame_interval;
} qua_venc_attr_h265_t;

typedef struct qua_venc_attr_s264_t {
    qua_h264_profile_t profile;
    QUA_U32 refresh_frame_interval;
    qua_size_t size;

    QUA_BOOL smart_en;
    QUA_BOOL texture_en;
    QUA_BOOL bgm_en;
    QUA_BOOL fresh_ltref_en;
    qua_gop_th_t gop_th;
    QUA_U32 bgm_chn;
} qua_venc_attr_s264_t;

typedef struct qua_venc_attr_s265_t {
    qua_h265_profile_t profile;
    QUA_U32 refresh_frame_interval;
    qua_size_t size;

    QUA_BOOL smart_en;
    QUA_BOOL texture_en;
    QUA_BOOL bgm_en;
    QUA_BOOL fresh_ltref_en;
    qua_gop_th_t gop_th;
    QUA_U32 bgm_chn;
} qua_venc_attr_s265_t;

typedef enum qua_venc_rc_mode_t {
    QUA_VENC_RC_MODE_NONE,
    QUA_VENC_RC_MODE_H264CBR,
    QUA_VENC_RC_MODE_H264VBR,
    QUA_VENC_RC_MODE_H264AVBR,
    QUA_VENC_RC_MODE_H264QVBR,
    QUA_VENC_RC_MODE_H264FIXQP,
    QUA_VENC_RC_MODE_H264QPMAP,

    QUA_VENC_RC_MODE_H265CBR,
    QUA_VENC_RC_MODE_H265VBR,
    QUA_VENC_RC_MODE_H265AVBR,
    QUA_VENC_RC_MODE_H265QVBR,
    QUA_VENC_RC_MODE_H265FIXQP,
    QUA_VENC_RC_MODE_H265QPMAP,

    QUA_VENC_RC_MODE_BUTT,
} qua_venc_rc_mode_t;

typedef struct qua_venc_attr_h264_cbr_t {
    QUA_U32 gop;
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 bitRate;
    QUA_U32 fluctuate_level;

    QUA_U32 init_qp;
    QUA_U32 max_rate_percent;
    QUA_U32 i_frm_max_bits;
    QUA_S32 ip_qp_delta;
    QUA_S32 i_bit_prop;
    QUA_S32 p_bit_prop;
} qua_venc_attr_h264_cbr_t;

typedef struct qua_venc_attr_h264_vbr_t {
    QUA_U32 gop;
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 max_bitRate;
    QUA_U32 max_qp;
    QUA_U32 min_qp;
    QUA_U32 min_iqp;

    QUA_U32 max_iqp;
    QUA_U32 init_qp;
    QUA_U32 max_rate_percent;
    QUA_U32 i_frm_max_bits;
    QUA_S32 ip_qp_delta;
    QUA_S32 i_bit_prop;
    QUA_S32 p_bit_prop;
    QUA_U32 fluctuate_level;
} qua_venc_attr_h264_vbr_t;

typedef struct qua_venc_attr_h264_avbr_t {
    QUA_U32 gop;
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 max_bitrate;

    QUA_U32 max_qp;
    QUA_U32 min_qp;
    QUA_U32 min_iqp;
    QUA_U32 max_Iqp;
    QUA_U32 init_qp;
    QUA_U32 max_rate_percent;
    QUA_U32 i_frm_max_bits;
    QUA_S32 ip_qp_delta;
    QUA_S32 i_bit_prop;
    QUA_S32 p_bit_prop;
    QUA_U32 fluctuate_level;
    QUA_U32 still_rate_percent;
    QUA_U32 max_still_qp;
} qua_venc_attr_h264_avbr_t;

typedef struct qua_venc_attr_h264_qvbr_t {
    QUA_U32 gop;
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 target_bitrate;

    QUA_U32 init_qp;
    QUA_U32 max_rate_percent;
    QUA_U32 i_frm_max_bits;
    QUA_S32 ip_qp_delta;
    QUA_S32 i_bit_prop;
    QUA_S32 p_bit_prop;
    QUA_U32 fluctuate_level;
    QUA_S32 bit_percent_ul;
    QUA_S32 bit_percent_ll;
    QUA_S32 psnr_fluctuate_ul;
    QUA_S32 psnr_fluctuatell;
} qua_venc_attr_h264_qvbr_t;

typedef struct qua_venc_attr_h264_fixqp_t {
    QUA_U32 gop;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 i_qp;
    QUA_U32 p_qp;
    QUA_U32 b_qp;
} qua_venc_attr_h264_fixqp_t;

typedef enum  {
    QUA_VENC_RC_QPMAP_MODE_MEANQP = 0,
    QUA_VENC_RC_QPMAP_MODE_MINQP,
    QUA_VENC_RC_QPMAP_MODE_MAXQP,

    QUA_VENC_RC_QPMAP_MODE_BUTT
} qua_venc_rc_qpMap_t;

typedef struct qua_venc_attr_h264_qpmap_t {
    QUA_U32 gop;
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    qua_venc_rc_qpMap_t qp_map_mode;
    QUA_BOOL qp_map_abs_qp;
    QUA_U32 reserved;
} qua_venc_attr_h264_qpmap_t;

typedef struct qua_venc_attr_mjpeg_cbr_t {
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 bitrate;
    QUA_U32 fluctuate_level;
} qua_venc_attr_mjpeg_cbr_t;

typedef struct qua_venc_attr_mjpeg_vbr_t {
    QUA_U32 stat_time;
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 max_bitrate;
    QUA_U32 max_qfactor;
    QUA_U32 min_qfactor;
} qua_venc_attr_mjpeg_vbr_t;

typedef struct qua_venc_attr_mjpeg_fixqp_t {
    QUA_U32 src_frmrate;
    QUA_U32 dst_frmrate;
    QUA_U32 qfactor;
} qua_venc_attr_mjpeg_fixqp_t;

typedef struct qua_venc_rc_attr_t {
    qua_venc_rc_mode_t rc_mode;

    union {
        qua_venc_attr_h264_cbr_t attr_h264_cbr;
        qua_venc_attr_h264_vbr_t attr_h264_vbr;
        qua_venc_attr_h264_avbr_t attr_h264_avbr;
        qua_venc_attr_h264_fixqp_t attr_h264_fixqp;
        qua_venc_attr_h264_qvbr_t attr_h264_qvbr;
        qua_venc_attr_h264_qpmap_t attr_h264_qpMap;

        qua_venc_attr_mjpeg_cbr_t attr_mjpeg_cbr;
        qua_venc_attr_mjpeg_vbr_t attr_mjpeg_vbr;
        qua_venc_attr_mjpeg_fixqp_t attr_mjpeg_fixqp;

        qua_venc_attr_h265_cbr_t attr_h265_cbr;
        qua_venc_attr_h265_vbr_t attr_h265_vbr;
        qua_venc_attr_h265_avbr_t attr_h265_avbr;
        qua_venc_attr_h265_fixqp_t attr_h265_fixqp;
        qua_venc_attr_h265_qvbr_t attr_h265_qvbr;
        qua_venc_attr_h265_qpmap_t attr_h265_qpmap;
    };
    QUA_VOID_PTR rc_attr;
} qua_venc_rc_attr_t;

typedef struct qua_venc_rc_change_param_t {
    QUA_U32 bitrate;
    QUA_U32 i_min_qp;
    QUA_U32 i_max_qp;
    QUA_U32 p_min_qp;
    QUA_U32 p_max_qp;
    QUA_U32 frame_cnt;
    QUA_U32 frame_time;
    QUA_U32 i_frm_max_bits;
    QUA_U32 max_rate_percent;
    QUA_U32 i_bit_prop;
    QUA_U32 p_bit_prop;
} qua_venc_rc_change_param_t;

typedef struct qua_jpeg_chn_cfg_t {
    QUA_U32 qp;
    QUA_U32 rotate;
    QUA_U32 encode_speed;
} qua_jpeg_chn_cfg_t;

typedef struct qua_venc_attr_t {
    qua_venc_type_t enc_type;

    union {
        qua_venc_attr_h264_t attr_h264;
        qua_venc_attr_s264_t attr_s264;
        qua_venc_attr_mjpeg_t attr_mjpeg;
        union {
            qua_venc_attr_jpeg_t attr_jpeg;
            qua_jpeg_chn_cfg_t cfg_jpeg;
        } jpeg_u;
        qua_venc_attr_h265_t attr_h265;
        qua_venc_attr_s265_t attr_s265;
    } attr_u;

    QUA_BOOL enable_smart;
    QUA_U32 skip_mode;
} qua_venc_attr_t;

typedef struct qua_venc_chn_attr_t {
    qua_venc_attr_t venc_attr;
    qua_venc_rc_attr_t venc_rc_attr;
} qua_venc_chn_attr_t;

typedef struct qua_venc_chn_cap_t {
    qua_venc_type_t support_type;
    qua_size_t max_size;
    QUA_U32 bframe_num;
    QUA_U32 work_mode;
} qua_venc_chn_cap_t;

typedef struct qua_enc_stream_nalu_t {
    qua_enc_nalu_type_t type;
    QUA_U32 length;
    QUA_U8 *start;
} qua_enc_stream_nalu_t;

#define QUA_MAX_NALU_CNT    20
typedef struct qua_h264_stream_t {
    QUA_U8 *start;
    qua_enc_frame_type_t frame_type;
    QUA_U32 length;
    QUA_U64 time_stamp;
    QUA_U32 nalu_cnt;
    qua_enc_stream_nalu_t nalu[QUA_MAX_NALU_CNT];
    qua_enc_ref_type_t ref_type;
} qua_h264_stream_t;
typedef struct qua_h264_stream_t qua_h265_stream_t;

typedef struct qua_jpeg_stream_t {
    QUA_U64 phy_addr;
    QUA_U8 *vir_addr;
    QUA_U32 length;
    QUA_U64 time_stamp;
    QUA_U32 qp;
    QUA_U32 frame_id;
} qua_jpeg_stream_t;

typedef struct qua_venc_sse_info_t {
    QUA_BOOL sse_en;
    QUA_U32 sse_val;
} qua_venc_sse_info_t;

typedef struct qua_venc_stream_advance_info_h264_t {
    QUA_U32 mean_qp;
    QUA_U32 residual_bit_num;
    QUA_U32 head_bit_num;
    QUA_U32 madi_val;
    QUA_U32 madp_val;
    QUA_U64 psnr_val;
    QUA_U64 distortion;
    QUA_U32 mseLcu_cnt;
    QUA_U32 mse_sum;
    qua_venc_sse_info_t sst_info[8];
    QUA_U32 qp_hstgrm[52];
} qua_venc_stream_advance_info_h264_t;

typedef struct qua_venc_stream_advance_info_jpeg_t {
    QUA_U32 reserverd;
} qua_venc_stream_advance_info_jpeg_t;

typedef struct qua_venc_stream_t {
    qua_stream_type_t stream_type;
    QUA_S32 chn_id;

    union {
        qua_h264_stream_t h264_stream;
        qua_h265_stream_t h265_stream;
        qua_jpeg_stream_t jpeg_stream;
    };

    union {
        qua_venc_stream_advance_info_h264_t advance_h264_info;
        qua_venc_stream_advance_info_jpeg_t advance_jpeg_info;
        qua_venc_stream_advance_info_h265_t advance_h265_info;
    };
} qua_venc_stream_t;

typedef struct qua_venc_param_jpeg_t {
    QUA_U32 qfactor;
    QUA_U8 y_qt[64];
    QUA_U8 cb_qt[64];
    QUA_U8 cr_qt[64];
    QUA_U32 mcu_per_ecs;
} qua_venc_param_jpeg_t;

typedef struct qua_venc_crop_cfg_t {
    QUA_BOOL enable;
    qua_rect_t rect;
} qua_venc_crop_cfg_t;

typedef struct qua_venc_chn_stat_t {
    QUA_U32 left_pics;
    QUA_U32 left_stream_bytes;
    QUA_U32 left_stream_frames;
    QUA_U32 cur_packs;
    QUA_U32 left_recv_pics;
    QUA_U32 left_enc_pics;

    QUA_U32 enc_pics;
    QUA_U32 lost_pics;
    QUA_U32 last_qp;
    QUA_U32 last_iqp;
    QUA_U32 bps;
} qua_venc_chn_stat_t;

typedef struct qua_roi_map_t {
    QUA_S32 qp[7];
    QUA_BOOL is_delta_qp[7];
    QUA_U32 size;
    QUA_U64 roi_addr;
} qua_roi_map_t;

typedef struct qua_roi_t {
    QUA_U32 enable;
    QUA_S32 qp;
    qua_rect_t rect;
    QUA_U32 level;
    QUA_BOOL is_delta_qp;
} qua_roi_t;

#endif // __QUA_MM_VIDEO_TYPE_H__
