/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MPP_ELEMENT_H__
#define __QUA_MPP_ELEMENT_H__

#include "common/qua_mm_common.h"
#include "qua_mm_param.h"
#include "qua_mpp_frame.h"
#include "utils/qua_mutex.h"

typedef enum mpp_element_state {
    MPP_ELE_STATE_UNINIT,
    MPP_ELE_STATE_INIT,
    MPP_ELE_STATE_STARTED,
} mpp_element_state_t;

typedef struct mpp_global_base {
    qua_mutex_t lock;
    QUA_U32 ref_cnt;
    QUA_BOOL is_init;
    QUA_VOID *global_data;

    mpp_element_chn_t ele_chn;
    qua_list_node_t list;
} mpp_global_base_t;

typedef struct qua_mpp_element {
    QUA_CHAR *name;
    const qua_mm_module_t *quamm_module;
    QUA_VOID *pipeline;
    mpp_element_chn_t ele_chn;
    mpp_element_state_t state;
    qua_mutex_t lock;
    QUA_U32 ref_cnt;
    QUA_VOID *priv_data;
    mpp_global_base_t *global_base;

    // The 4 functions below cannot be acquired the lock in this struct,
    // instead, the callers managed the lock
    QUA_BOOL (*start)(struct qua_mpp_element *element);
    QUA_BOOL (*stop)(struct qua_mpp_element *element);
    QUA_BOOL (*init)(struct qua_mpp_element *element);
    QUA_BOOL (*deinit)(struct qua_mpp_element *element);

    // The functions below can be called by mpp pipeline directly, in which implementation
    // the lock can be acquired if necessary
    QUA_BOOL (*set_param)(struct qua_mpp_element *element, quamm_param_t *param);
    QUA_BOOL (*get_param)(struct qua_mpp_element *element, quamm_param_t *param);
    QUA_BOOL (*send_frame)(struct qua_mpp_element *element, qua_mpp_frame_t *frame);
    QUA_BOOL (*get_frame)(struct qua_mpp_element *element, QUA_S32 timeout, qua_mpp_frame_t *frame);
    QUA_BOOL (*release_frame)(struct qua_mpp_element *element, qua_mpp_frame_t *frame);
} qua_mpp_element_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

qua_mpp_element_t* qua_create_element(QUA_CONST_CHAR *name, QUA_VOID *pipeline, mpp_element_chn_t ele_chn);
QUA_BOOL qua_start_element(qua_mpp_element_t *element);
QUA_BOOL qua_stop_element(qua_mpp_element_t *element);
QUA_CHAR* element_label(qua_mpp_element_t *element, QUA_U32 len, QUA_CHAR *label);

#ifdef __cplusplus
};
#endif

#endif /* __QUA_MPP_ELEMENT_H__ */
