/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_COND_H__
#define __QUA_COND_H__

#include <pthread.h>
#include "common/qua_type.h"
#include "qua_mutex.h"

typedef pthread_cond_t qua_cond_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static inline QUA_BOOL qua_cond_create(qua_cond_t *cond) {
    int ret = pthread_cond_init(cond, NULL);

    return ret == 0;
}

static inline QUA_BOOL qua_cond_destroy(qua_cond_t *cond) {
    int ret = pthread_cond_destroy(cond);

    return ret == 0;
}

static inline QUA_BOOL qua_cond_wait(qua_cond_t *cond, qua_mutex_t *mutex) {
    int ret = pthread_cond_wait(cond, mutex);

    return ret == 0;
}

static inline QUA_BOOL qua_cond_timedwait(qua_cond_t *cond, qua_mutex_t *mutex, const struct timespec *abstime) {
    int ret = pthread_cond_timedwait(cond, mutex, abstime);

    return ret == 0;
}

static inline QUA_BOOL qua_cond_signal(qua_cond_t *cond) {
    int ret = pthread_cond_signal(cond);

    return ret == 0;
}

static inline QUA_BOOL qua_cond_broadcast(qua_cond_t *cond) {
    int ret = pthread_cond_broadcast(cond);

    return ret == 0;
}


#ifdef __cplusplus
};
#endif

#endif
