/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_MM_VIDEO_H__
#define __QUA_MM_VIDEO_H__

#include "qua_mm_video_type.h"
#include "common/qua_mm_common.h"
#include "common/qua_mm_video_common_type.h"

#define QUA_MM_VIDEO_DEV_DEC "video.dec"
#define QUA_MM_VIDEO_DEV_ENC "video.enc"

typedef enum qua_video_channel_t {
    QUA_MM_VIDEO_VENC_CHN,
    QUA_MM_VIDEO_JPEGE_CHN
} qua_video_channel_t;

typedef struct qua_mm_vdec_chn_attr_t {
    qua_vdec_chn_attr_t chn_attr;

    QUA_S32 vb_cnt;
} qua_mm_vdec_chn_attr_t;

typedef struct qua_mm_venc_chn_attr_t {
    union {
        qua_venc_chn_attr_t chn_attr;
        qua_venc_chn_cap_t chn_cap;
    };
    qua_video_channel_t video_chn;
} qua_mm_venc_chn_attr_t;

typedef struct qua_mm_video_module_t {
    struct qua_mm_module_t parent;
} qua_mm_video_module_t;

typedef struct qua_mm_vdec_device_t {
    struct qua_mm_device_t parent;
} qua_mm_vdec_device_t;

typedef struct qua_mm_vdec_channel_t {
    struct qua_mm_channel_t parent;

    QUA_S32 (*start_chn)(QUA_S32 chn_id);
    QUA_S32 (*stop_chn)(QUA_S32 chn_id);
    QUA_S32 (*reset_chn)(QUA_S32 chn_id);
    QUA_S32 (*get_chn_fd)(QUA_S32 chn_id);
    QUA_S32 (*send_stream)(QUA_S32 chn_id, const qua_vdec_stream_t *packet, QUA_S32 timeout_millis);
    QUA_S32 (*get_frame)(QUA_S32 chn_id, qua_video_frame_info_t *frame_info, QUA_S32 timeout_millis);
    QUA_S32 (*release_frame)(QUA_S32 chn_id, qua_video_frame_info_t *frame_info);
    QUA_S32 (*get_chn_attr)(QUA_S32 chn_id, qua_vdec_chn_attr_t *chn_attr);
    QUA_S32 (*query_chn)(QUA_S32 chn_id, qua_vdec_chn_stat_t *chn_stat);
    QUA_S32 (*get_chn_param)(QUA_S32 chn_id, qua_vdec_chn_param_t* chn_param);
    QUA_S32 (*set_chn_param)(QUA_S32 chn_id, qua_vdec_chn_param_t* chn_param);
    QUA_S32 (*set_chn_drop_frame)(QUA_S32 chn_id, qua_vdec_drop_frame_cfg_t *cfg);
} qua_mm_vdec_channel_t;

typedef struct qua_mm_venc_device_t {
    struct qua_mm_device_t parent;
} qua_mm_venc_device_t;

typedef struct qua_mm_venc_channel_t {
    struct qua_mm_channel_t parent;

    QUA_S32 (*reset_chn)(QUA_S32 chn_id);
    QUA_S32 (*start_recv_pic)(QUA_S32 chn_id);
    QUA_S32 (*stop_recv_pic)(QUA_S32 chn_id);
    QUA_S32 (*query_chn)(QUA_S32 chn_id, qua_venc_chn_stat_t *chn_stat);
    QUA_S32 (*set_chn_attr)(QUA_S32 chn_id, const qua_venc_chn_attr_t *chn_attr);
    QUA_S32 (*get_chn_attr)(QUA_S32 chn_id, qua_venc_chn_attr_t *chn_attr);
    QUA_S32 (*get_stream)(QUA_S32 chn_id, qua_venc_stream_t *packet, QUA_S32 timeout_millis);
    QUA_S32 (*get_stream_all)(qua_stream_type_t request_type, qua_venc_stream_t *packet);
    QUA_S32 (*release_stream)(QUA_S32 chn_id, qua_venc_stream_t *packet);
    QUA_S32 (*send_frame)(QUA_S32 chn_id, qua_video_frame_info_t *frame_info, QUA_S32 timeout_millis);
    QUA_S32 (*set_rotate)(QUA_S32 chn_id, const qua_rotate_t rotate_ops);
    QUA_S32 (*get_rotate)(QUA_S32 chn_id, qua_rotate_t *rotate_ops);
    QUA_S32 (*check_in_addr)(QUA_S32 chn_id, QUA_U64 in_addr, QUA_U32 size, QUA_S32 timeout_millis, QUA_U32 *is_idle);
    QUA_S32 (*set_roi_map)(QUA_S32 chn_id, const qua_roi_map_t *roi_map);
    QUA_S32 (*set_roi_cfg)(QUA_S32 chn_id, const qua_roi_t *roi_cfg);
    QUA_S32 (*clear_roi)(QUA_S32 chn_id);
    QUA_S32 (*set_rc_attr)(QUA_S32 chn_id, const qua_venc_rc_attr_t *rc_attr);
    QUA_S32 (*get_rc_attr)(QUA_S32 chn_id, qua_venc_rc_attr_t *rc_attr);
    QUA_S32 (*set_rc_change_param)(QUA_S32 chn_id, const qua_venc_rc_change_param_t *rc_params);
    QUA_S32 (*set_enc_param)(QUA_S32 chn_id, qua_enc_param_cmd_t cmd, const qua_enc_param_union_t *param);
    QUA_S32 (*get_enc_param)(QUA_S32 chn_id, qua_enc_param_cmd_t cmd, const qua_enc_param_union_t *param);
    QUA_S32 (*set_h264_entropy)(QUA_S32 chn_id, const qua_h264_entropy_t *entropy);
    QUA_S32 (*get_h264_entropy)(QUA_S32 chn_id, qua_h264_entropy_t *entropy);
    QUA_S32 (*set_h264_dblk)(QUA_S32 chn_id, const qua_h264_dblk_t *dblk);
    QUA_S32 (*get_h264_dblk)(QUA_S32 chn_id, qua_h264_dblk_t *dblk);
    QUA_S32 (*set_h265_entropy)(QUA_S32 chn_id, const qua_h265_entropy_t entropy);
    QUA_S32 (*get_h265_entropy)(QUA_S32 chn_id, qua_h265_entropy_t *entropy);
    QUA_S32 (*set_h265_dblk)(QUA_S32 chn_id, const qua_h265_dblk_t *dblk);
    QUA_S32 (*get_h265_dblk)(QUA_S32 chn_id, qua_h265_dblk_t *dblk);
    QUA_S32 (*set_debreath_effect)(QUA_S32 chn_id, const qua_debreath_t *debreath);
    QUA_S32 (*get_debreath_effect)(QUA_S32 chn_id, qua_debreath_t *debreath);
    QUA_S32 (*clear_yuv_queue)(QUA_S32 chn_id);
    QUA_S32 (*request_idr)(QUA_S32 chn_id);
} qua_mm_venc_channel_t;

typedef struct qua_mm_jpege_channel_t {
    struct qua_mm_channel_t parent;

    QUA_S32 (*reset_chn)(QUA_S32 chn_id);
    QUA_S32 (*start_recv_pic)(QUA_S32 chn_id);
    QUA_S32 (*stop_recv_pic)(QUA_S32 chn_id);
    QUA_S32 (*query_chn)(QUA_S32 chn_id, qua_venc_chn_stat_t *chn_stat);
    QUA_S32 (*set_chn_attr)(QUA_S32 chn_id, const qua_venc_chn_attr_t *chn_attr);
    QUA_S32 (*get_chn_attr)(QUA_S32 chn_id, qua_venc_chn_attr_t *chn_attr);
    QUA_S32 (*get_stream)(QUA_S32 chn_id, qua_venc_stream_t *packet, QUA_S32 timeout_millis);
    QUA_S32 (*release_stream)(QUA_S32 chn_id, qua_venc_stream_t *packet);
    QUA_S32 (*send_frame)(QUA_S32 chn_id, qua_video_frame_info_t *frame_info, QUA_S32 timeout_millis);
    QUA_S32 (*set_max_stream_cnt)(QUA_S32 chn_id, QUA_U32 max_strm_cnt);
    QUA_S32 (*get_max_stream_cnt)(QUA_S32 chn_id, QUA_U32 *max_strm_cnt);
    QUA_S32 (*get_jpeg_param)(QUA_S32 chn_id, qua_venc_param_jpeg_t *jpeg_param);
    QUA_S32 (*set_jpeg_param)(QUA_S32 chn_id, qua_venc_param_jpeg_t *jpeg_param);
    QUA_S32 (*set_crop)(QUA_S32 chn_id, const qua_venc_crop_cfg_t *crop_cfg);
    QUA_S32 (*get_crop)(QUA_S32 chn_id, qua_venc_crop_cfg_t *crop_cfg);
    QUA_S32 (*set_rotate)(QUA_S32 chn_id, const qua_rotate_t rotate_ops);
    QUA_S32 (*get_rotate)(QUA_S32 chn_id, qua_rotate_t *rotate_ops);
    QUA_S32 (*get_chn_fd)(QUA_S32 chn_id);
} qua_mm_jpege_channel_t;

#endif // __QUA_MM_VIDEO_H__