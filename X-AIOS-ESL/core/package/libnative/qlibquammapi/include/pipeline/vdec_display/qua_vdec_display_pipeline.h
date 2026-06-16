/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_VDEC_DISPLAY_PIPELINE_H__
#define __QUA_VDEC_DISPLAY_PIPELINE_H__

#include "video/qua_mm_video_type.h"
#include "common/qua_mm_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* qua_vdec_display_pipeline_t;
typedef struct qua_vdec_display_pipeline_attr_t {
    QUA_U32 stream_width;              /* video stream width */
    QUA_U32 stream_height;             /* video stream height */
    qua_coding_type_t coding_type;     /* video stream encode type */
    QUA_U32 fps;                       /* video stream frame rate */

    QUA_U32 x;                         /* display x */
    QUA_U32 y;                         /* display y */
    QUA_U32 display_width;             /* display width */
    QUA_U32 display_height;            /* display height */
} qua_vdec_display_pipeline_attr_t;

typedef enum {
    QUA_VDEC_DISPLAY_FILTER_BUTT,
    QUA_VDEC_DISPLAY_VO_BUTT
} qua_vdec_display_pipeline_cmd_t;

typedef struct qua_vdec_display_pipeline_ops_t {
    QUA_S32 (*pipeline_create)(qua_vdec_display_pipeline_t *pipeline, const qua_vdec_display_pipeline_attr_t *attr);
    QUA_S32 (*pipeline_destroy)(qua_vdec_display_pipeline_t pipeline);
    QUA_S32 (*pipeline_start)(qua_vdec_display_pipeline_t pipeline);
    QUA_S32 (*pipeline_stop)(qua_vdec_display_pipeline_t pipeline);
    QUA_S32 (*pipeline_send_stream)(qua_vdec_display_pipeline_t pipeline,
        const qua_vdec_stream_t *packet, QUA_U32 timeout_millis);
    QUA_S32 (*pipeline_reset)(qua_vdec_display_pipeline_t pipeline);
    QUA_S32 (*pipeline_control)(qua_vdec_display_pipeline_t pipeline,
        const qua_vdec_display_pipeline_cmd_t cmd, QUA_VOID_PTR payload);
} qua_vdec_display_pipeline_ops_t;

QUA_S32 qua_vdec_display_pipeline_init(qua_vdec_display_pipeline_ops_t *pipeline_ops);
#ifdef __cplusplus
}
#endif

#endif /* __QUA_VDEC_DISPLAY_PIPELINE_H__ */