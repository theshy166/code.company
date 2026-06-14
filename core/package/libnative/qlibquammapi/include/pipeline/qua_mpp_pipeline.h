/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MPP_PIPELINE_H__
#define __QUA_MPP_PIPELINE_H__

#include "utils/qua_mutex.h"
#include "system/qua_mm_system.h"
#include "pipeline/qua_mpp_base.h"
#include "qua_mpp_element.h"

#define MAX_ELEMENTS_IN_PIPELINE 6

typedef enum qua_mpp_pipeline_type {
    MPP_PIPELINE_CAMERA_STREAMOUT, // VICAP/ISP stream out
    MPP_PIPELINE_CAMERA_ENCODER,   // VICAP/ISP/VPROC encoder
    MPP_PIPELINE_CAMERA_DISPLAY,   // VICAP/ISP/VPROC display
    MPP_PIPELINE_VPROC_ENCODER,    // VPROC encoder
    MPP_PIPELINE_VPROC_DISPLAY,    // VPROC display
    MPP_PIPELINE_DECODER_DISPLAY,  // DECODER/VPROC display
    MPP_PIPELINE_AUDIO_STREAMOUT,  // stream out
    MPP_PIPELINE_AUDIO_STREAMIN,   // stream in
    MPP_PIPELINE_AUDIO_CODECOUT,   // AI/AO codec out
} qua_mpp_pipeline_type_t;

typedef struct mpp_pipeline_config {
    qua_mpp_pipeline_type_t pipeline_type;
    QUA_S32 index;
    QUA_CONST_CHAR *description;
    QUA_U32 element_num;
    mpp_element_chn_t element_order[MAX_ELEMENTS_IN_PIPELINE];
} mpp_pipeline_config_t;

typedef enum mpp_pipeline_state {
    MPP_PL_STATE_INITIAL,
    MPP_PL_STATE_STOPPED,
    MPP_PL_STATE_STARTED,
} mpp_pipeline_state_t;

typedef struct qua_mpp_pipeline {
    QUA_CHAR *name;
    qua_mpp_element_t *elements[MAX_ELEMENTS_IN_PIPELINE];
    qua_mutex_t lock;
    mpp_pipeline_state_t state;
    qua_mm_system_ops_t *sys_ops;
} qua_mpp_pipeline_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

qua_mpp_pipeline_t* qua_mm_pipeline_create(qua_mpp_pipeline_type_t pipeline_type, QUA_S32 pipeline_index, qua_mm_system_ops_t *sys_ops);
QUA_BOOL qua_mm_pipeline_start(qua_mpp_pipeline_t* pipeline);
QUA_BOOL qua_mm_pipeline_stop(qua_mpp_pipeline_t* pipeline);
QUA_BOOL qua_mm_pipeline_destroy(qua_mpp_pipeline_t* pipeline);
QUA_BOOL qua_mm_pipeline_set_param(qua_mpp_pipeline_t* pipeline, mpp_element_type_t ele_type, quamm_param_t *param);
QUA_BOOL qua_mm_pipeline_get_param(qua_mpp_pipeline_t* pipeline, mpp_element_type_t ele_type, quamm_param_t *param);
QUA_BOOL qua_mm_pipeline_send_frame(qua_mpp_pipeline_t* pipeline, mpp_element_type_t ele_type, qua_mpp_frame_t *frame);
QUA_BOOL qua_mm_pipeline_get_frame(qua_mpp_pipeline_t* pipeline, mpp_element_type_t ele_type, QUA_S32 timeout, qua_mpp_frame_t *frame);
QUA_BOOL qua_mm_pipeline_release_frame(qua_mpp_pipeline_t* pipeline, mpp_element_type_t ele_type, qua_mpp_frame_t *frame);
qua_mpp_element_t *qua_mm_element_in_pipeline(qua_mpp_pipeline_t* pipeline, mpp_element_type_t ele_type);
#ifdef __cplusplus
};
#endif

#endif /* __QUA_MPP_PIPELINE_H__ */