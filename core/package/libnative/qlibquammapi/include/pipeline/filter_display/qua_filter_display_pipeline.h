/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_FILTER_DISPLAY_PIPELINE_H__
#define __QUA_FILTER_DISPLAY_PIPELINE_H__

#include "video/qua_mm_video_type.h"
#include "common/qua_mm_common.h"
#include "system/qua_mm_system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct qua_filter_display_pipeline_attr_t {
    QUA_U32 in_width;                 /* in width */
    QUA_U32 in_height;                /* in height */
    qua_pixel_fmt_t in_format;        /* in format */

    QUA_U32 x;                        /* output x position */
    QUA_U32 y;                        /* ouput y position */
    QUA_U32 display_width;            /* output width */
    QUA_U32 display_height;           /* output height */
} qua_filter_display_pipeline_attr_t;

typedef struct qua_filter_display_pipeline_t {
    qua_mm_channel_t *filter_channel;
    qua_mm_channel_t *vo_channel;

    QUA_U32 index;
} qua_filter_display_pipeline_t;

typedef struct qua_filter_display_pipeline_frame_t {
    QUA_ULONG phyAddr;
    qua_video_frame_info_t video_frame;
} qua_filter_display_pipeline_frame_t;

typedef struct qua_filter_display_pipeline_ops_t {
    qua_filter_display_pipeline_t*(*pipeline_create)(const qua_filter_display_pipeline_attr_t *attr);
    QUA_S32 (*pipeline_destroy)(const qua_filter_display_pipeline_t *pipeline);
    QUA_S32 (*pipeline_start)(const qua_filter_display_pipeline_t *pipeline);
    QUA_S32 (*pipeline_stop)(const qua_filter_display_pipeline_t *pipeline);
    QUA_S32 (*pipeline_send_frame)(const qua_filter_display_pipeline_t *pipeline, const qua_filter_display_pipeline_frame_t *frame);
} qua_filter_display_pipeline_ops_t;

QUA_S32 qua_filter_display_pipeline_init(qua_filter_display_pipeline_ops_t *pipeline_ops);
#ifdef __cplusplus
}
#endif

#endif /* __QUA_FILTER_DISPLAY_PIPELINE_H__ */

