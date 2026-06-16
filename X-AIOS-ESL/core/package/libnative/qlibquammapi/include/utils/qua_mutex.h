/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_C_MUTEX_H__
#define __QUA_C_MUTEX_H__

#include <pthread.h>
#include "common/qua_type.h"

typedef pthread_mutex_t qua_mutex_t;
#define QUA_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static inline QUA_BOOL qua_mutex_create(qua_mutex_t *mutex)
{
    int ret = pthread_mutex_init(mutex, NULL);

    return ret == 0;
}

static inline QUA_BOOL qua_mutex_destroy(qua_mutex_t *mutex)
{
    int ret = pthread_mutex_destroy(mutex);

    return ret == 0;
}

static inline QUA_BOOL qua_mutex_lock(qua_mutex_t *mutex)
{
    int ret = pthread_mutex_lock(mutex);

    return ret == 0;
}

static inline QUA_BOOL qua_mutex_trylock(qua_mutex_t *mutex)
{
    int ret = pthread_mutex_trylock(mutex);

    return ret == 0;
}

static inline QUA_BOOL qua_mutex_unlock(qua_mutex_t *mutex)
{
    int ret = pthread_mutex_unlock(mutex);

    return ret == 0;
}

#ifdef __cplusplus
};
#endif

#endif /* __QUA_C_MUTEX_H__ */

