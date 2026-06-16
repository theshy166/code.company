/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_THREAD_H__
#define __QUA_THREAD_H__

#include "common/qua_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* qua_thread_t;
typedef void* (*qua_thread_entry)(void *);

typedef enum qua_thread_priority_t {
    QUA_THREAD_PRI_LOW,
    QUA_THREAD_PRI_DEFAULT,
    QUA_THREAD_PRI_HIGH,
    QUA_THREAD_PRI_HIGHEST
} qua_thread_priority_t;

typedef enum qua_thread_policy_t {
    QUA_THREAD_SCHED_NORMAL,
    QUA_THREAD_SCHED_FIFO,
    QUA_THREAD_SCHED_RR
} qua_thread_policy_t;

typedef struct qua_osal_thread_param_t {
    QUA_CONST_CHAR *name;
    QUA_S32 stack_size;
    QUA_BOOL b_detach;
    qua_thread_policy_t policy;
    qua_thread_priority_t priority;
} qua_thread_param_t;

QUA_S32 qua_thread_create(qua_thread_t *thread, qua_thread_entry thread_proc, QUA_VOID_PTR thread_args);
QUA_S32 qua_thread_destroy(qua_thread_t thread);
QUA_S32 qua_thread_start(qua_thread_t thread, const qua_thread_param_t *param);
QUA_S32 qua_thread_stop(qua_thread_t thread);

#ifdef __cplusplus
}
#endif

#endif // __QUA_OSAL_THREAD_H__
