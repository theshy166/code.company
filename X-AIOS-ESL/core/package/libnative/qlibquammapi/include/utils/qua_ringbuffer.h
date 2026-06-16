/*******************************************************************************************
#  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __qua_ringbuffer_H__
#define __qua_ringbuffer_H__

#include <pthread.h>
#include "common/qua_type.h"

typedef struct _qua_ringbuffer_ {
    QUA_U8 *data;
    QUA_U32 read_pos;
    QUA_U32 write_pos;
    QUA_U32 data_size;
    QUA_U32 ringbuffer_capacity;
    QUA_U32 underrun_size;
    QUA_U32 overrun_size;
    pthread_mutex_t mutex;

    QUA_S32 (*size)(struct _qua_ringbuffer_* ring_buffer);
    QUA_S32 (*capacity)(struct _qua_ringbuffer_* ring_buffer);
    QUA_S32 (*remaining)(struct _qua_ringbuffer_* ring_buffer);
    QUA_S32 (*read)(struct _qua_ringbuffer_* ring_buffer, QUA_U8 *buffer, QUA_U32 size, QUA_BOOL block);
    QUA_S32 (*write)(struct _qua_ringbuffer_* ring_buffer, QUA_U8 *buffer, QUA_U32 size, QUA_BOOL block);
    QUA_S32 (*flush)(struct _qua_ringbuffer_* ring_buffer);
    QUA_S32 (*get_underrun_size)(struct _qua_ringbuffer_* ring_buffer);
    QUA_S32 (*get_overrun_size)(struct _qua_ringbuffer_* ring_buffer);
} qua_ringbuffer_t;

extern qua_ringbuffer_t* qua_ringbuffer_create(QUA_U32 capacity);
extern QUA_S32 qua_ringbuffer_destroy(qua_ringbuffer_t *ring_buffer);

 #endif // __qua_ringbuffer_H__