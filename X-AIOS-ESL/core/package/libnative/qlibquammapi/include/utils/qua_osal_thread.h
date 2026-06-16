/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_OSAL_THREAD_H__
#define __QUA_OSAL_THREAD_H__

#include "common/qua_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*qua_osal_thread_proc)(void *);

typedef enum qua_osal_thread_priority_t {
    QUA_OSAL_THREAD_PRI_LOW,
    QUA_OSAL_THREAD_PRI_DEFAULT,
    QUA_OSAL_THREAD_PRI_HIGH,
    QUA_OSAL_THREAD_PRI_HIGHEST
} qua_osal_thread_priority_t;

typedef enum qua_osal_thread_policy_t {
    QUA_OSAL_THREAD_SCHED_OTHER,
    QUA_OSAL_THREAD_SCHED_FIFO,
    QUA_OSAL_THREAD_SCHED_RR
} qua_osal_thread_policy_t;

typedef struct qua_osal_thread_param_t {
    QUA_CHAR *name;
    QUA_S32 stack_size;
    QUA_BOOL b_detach;
    qua_osal_thread_policy_t policy;
    qua_osal_thread_priority_t priority;
} qua_osal_thread_param_t;

typedef struct qua_osal_thread_t {
    QUA_VOID_PTR real_thread;
} qua_osal_thread_t;

QUA_S32 qua_osal_thread_create(qua_osal_thread_t *thread, qua_osal_thread_proc thread_proc, QUA_VOID_PTR thread_args);
QUA_S32 qua_osal_thread_destroy(qua_osal_thread_t *thread);
QUA_S32 qua_osal_thread_start(qua_osal_thread_t *thread, const qua_osal_thread_param_t *param);
QUA_S32 qua_osal_thread_stop(qua_osal_thread_t *thread);

#ifdef __cplusplus
}
#endif

#endif // __QUA_OSAL_THREAD_H__
