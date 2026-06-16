/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_BUFFER_QUEUE_H__
#define __QUA_BUFFER_QUEUE_H__

#include "common/qua_mm_video_common_type.h"
#include "qua_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct qua_mm_buffer_t {
    qua_list_node_t node;
    qua_video_frame_t frame;
} qua_mm_buffer_t;

typedef struct qua_buffer_queue_t {
    qua_list_node_t free_buffers;
    qua_list_node_t active_buffers;

    pthread_mutex_t lock;
    pthread_cond_t cond;
} qua_buffer_queue_t;

QUA_S32 qua_buffer_queue_init(qua_buffer_queue_t **buffer_queue);
void qua_buffer_queue_destroy(qua_buffer_queue_t *buffer_queue);
qua_mm_buffer_t * qua_buffer_queue_dequeue(qua_buffer_queue_t *buffer_queue, QUA_S64 timeout_us);
QUA_S32 qua_buffer_queue_enqueue(qua_buffer_queue_t *buffer_queue, qua_mm_buffer_t *mm_buffer);
qua_mm_buffer_t * qua_buffer_queue_acquire(qua_buffer_queue_t *buffer_queue);
QUA_S32 qua_buffer_queue_release(qua_buffer_queue_t *buffer_queue, qua_mm_buffer_t *mm_buffer);

#ifdef __cplusplus
}
#endif

#endif /* __QUA_BUFFER_QUEUE_H__ */
