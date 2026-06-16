/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MPP_FRAME_H__
#define __QUA_MPP_FRAME_H__

#include "common/qua_mm_video_common_type.h"
#include "audio/qua_mm_audio.h"
#include "utils/qua_list.h"
#include "pipeline/qua_mpp_base.h"
#include "video/qua_mm_video_type.h"

typedef enum mpp_frame_owner {
    MPP_FRAME_OWNER_QUAMM_HAL = 0,
    MPP_FRAME_OWNER_QUAMM_FWK,
    MPP_FRAME_OWNER_QUAMM_USER
} mpp_frame_owner_t;

typedef struct mpp_frame_provider {
    mpp_frame_owner_t owner;
    QUA_VOID *priv;
    mpp_element_type_t ele_type;
    QUA_BOOL (*dispose)(QUA_VOID *priv);
} mpp_frame_provider_t;

typedef enum mpp_frame_data_type {
    MPP_FRAME_VIDEO_RAW,
    MPP_FRAME_VIDEO_CODEC,
    MPP_FRAME_AUDIO_RAW,
    MPP_FRAME_AUDIO_CODEC,
    MPP_FRAME_BUTT
} mpp_frame_data_type_t;

typedef struct qua_mpp_frame {
    union {
        qua_video_frame_info_t video_frame;
        qua_audio_frame_t audio_frame;
        qua_venc_stream_t venc_stream;
        qua_vdec_stream_t vdec_stream;
    } frame;
    mpp_frame_data_type_t data_type;
    QUA_U32 ref_cnt;
    QUA_U64 id;
    QUA_CONST_CHAR *birth_name;
    mpp_frame_provider_t provider;
    qua_list_node_t list;
} qua_mpp_frame_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

qua_mpp_frame_t *qua_mpp_frame_acquire(QUA_CONST_CHAR *birth_name, mpp_frame_provider_t *prvoider);
QUA_BOOL qua_mpp_frame_retain(qua_mpp_frame_t *frame);
QUA_BOOL qua_mpp_frame_release(qua_mpp_frame_t *frame);

#ifdef __cplusplus
};
#endif


#endif

