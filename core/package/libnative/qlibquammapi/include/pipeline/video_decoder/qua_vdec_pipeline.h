/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_VDEC_PIPELINE_H__
#define __QUA_VDEC_PIPELINE_H__

#include "video/qua_mm_video_type.h"
#include "common/qua_mm_common.h"
#include "system/qua_mm_system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* qua_vdec_pipeline_t;

typedef struct qua_vdec_pipeline_attr_t {
    QUA_U32 stream_width;              /* video stream width */
    QUA_U32 stream_height;             /* video stream height */
    qua_coding_type_t coding_type;     /* video stream encode type */

    QUA_U32 output_width;             /* output width */
    QUA_U32 output_height;            /* output height */
    qua_pixel_fmt_t output_format;    /* output format */

    QUA_BOOL zero_copy;             /* if using zero copy mode, only support jpeg decoder */
    QUA_S32 fd;                     /* jpeg file fd handle */

    QUA_BOOL external_buffer_mode;
    QUA_ULONG external_phy;
    QUA_VOID_PTR external_virt;
    QUA_U32 external_size;
} qua_vdec_pipeline_attr_t;

typedef struct qua_vdec_pipeline_frame_info_t {
    qua_video_frame_info_t frame;
    QUA_U8 *data;
    QUA_U32 length;
} qua_vdec_pipeline_frame_info_t;

typedef struct qua_vdec_pipeline_crop_info_t {
    QUA_BOOL enable;
} qua_vdec_pipeline_crop_info_t;

typedef enum qua_vdec_pipeline_key_t {
    QUA_VDEC_PIPELINE_KEY_GET_ATTR,
    QUA_VDEC_PIPELINE_KEY_SET_CROP_INFO,
    QUA_VDEC_PIPELINE_KEY_BUTT,
} qua_vdec_pipeline_key_t;

typedef struct qua_vdec_pipeline_ops_t {
    QUA_S32 (*pipeline_create)(qua_vdec_pipeline_t *pipeline, const qua_vdec_pipeline_attr_t *attr);
    QUA_S32 (*pipeline_destroy)(qua_vdec_pipeline_t pipeline);
    QUA_S32 (*pipeline_start)(qua_vdec_pipeline_t pipeline);
    QUA_S32 (*pipeline_stop)(qua_vdec_pipeline_t pipeline);
    QUA_S32 (*pipeline_send_stream)(qua_vdec_pipeline_t pipeline,
        const qua_vdec_stream_t *packet, QUA_U32 timeout_millis);
    QUA_S32 (*pipeline_get_frame)(qua_vdec_pipeline_t pipeline,
        qua_vdec_pipeline_frame_info_t *frame);
    QUA_S32 (*pipeline_release_frame)(qua_vdec_pipeline_t pipeline,
        qua_vdec_pipeline_frame_info_t *frame);
    QUA_S32 (*pipeline_reset)(qua_vdec_pipeline_t pipeline);
    QUA_S32 (*pipeline_get_param)(qua_vdec_pipeline_t pipeline, qua_vdec_pipeline_key_t key, QUA_VOID_PTR param);
    QUA_S32 (*pipeline_set_param)(qua_vdec_pipeline_t pipeline, qua_vdec_pipeline_key_t key, QUA_VOID_PTR param);
} qua_vdec_pipeline_ops_t;

QUA_S32 qua_vdec_pipeline_init(qua_vdec_pipeline_ops_t *pipeline_ops);
#ifdef __cplusplus
}
#endif

#endif /* __QUA_VDEC_PIPELINE_H__ */

