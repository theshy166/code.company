/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_METADATA_RETRIEVER_H__
#define __QUA_METADATA_RETRIEVER_H__

#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

typedef void* qua_retriever_t;

typedef enum qua_retriever_output_type_t {
    QUA_RETRIEVER_OUTPUT_ORIGINAL,
    QUA_RETRIEVER_OUTPUT_JPEG_FILE
} qua_retriever_output_type_t;

typedef enum qua_retriever_media_type_t {
    QUA_RETRIEVER_MEDIA_TYPE_VIDEO,
    QUA_RETRIEVER_MEDIA_TYPE_IMAGE,
    QUA_RETRIEVER_MEDIA_TYPE_BUTT
} qua_retriever_media_type_t;

typedef struct qua_retriever_attr_t {
    QUA_CHAR *in_url;
    QUA_CHAR *out_url;
    qua_retriever_media_type_t media_type;
    qua_retriever_output_type_t output_type;
} qua_retriever_attr_t;

typedef struct qua_retriever_scale_info_t {
    QUA_BOOL enable;
    qua_size_t size;
} qua_retriever_scale_info_t;

typedef enum qua_retriever_param_key_t {
    QUA_RETRIEVER_PARAM_SCALE_INFO = 0x1,
    QUA_RETRIEVER_PARAM_DIMENTION = 0x10
} qua_retriever_param_key_t;

#ifdef __cplusplus
extern "C" {
#endif

QUA_S32 qua_retriever_create(qua_retriever_attr_t *retriever_attr, qua_retriever_t *retriever);
QUA_S32 qua_retriever_destroy(qua_retriever_t retriever);
QUA_S32 qua_retriever_get_frame_at_time(qua_retriever_t retriever, QUA_S32 time_us, qua_video_frame_info_t *frame);
QUA_S32 qua_retriever_set_param(qua_retriever_t retriever, qua_retriever_param_key_t key, QUA_VOID *param);
QUA_S32 qua_retriever_get_param(qua_retriever_t retriever, qua_retriever_param_key_t key, QUA_VOID *param);

#ifdef __cplusplus
}
#endif

#endif // __QUA_METADATA_RETRIEVER_H__