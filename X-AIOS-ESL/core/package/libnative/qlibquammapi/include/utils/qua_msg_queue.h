/*
 * Copyright (c) 2024~2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MSG_QUEUE_H__
#define __QUA_MSG_QUEUE_H__

#include "common/qua_type.h"
#include "qua_list.h"
#include "utils/qua_mutex.h"
#include "utils/qua_cond.h"

#ifdef __cplusplus
extern "C" {
#endif

struct qua_msg_t;
typedef void (*qua_msg_handle_fun)(struct qua_msg_t *msg);

typedef enum {
    QUA_MSG_PRIORITY_NORMAL,
    QUA_MSG_PRIORITY_HIGH
} qua_msg_priority_t;

typedef enum {
    QUA_MSG_IDLE,
    QUA_MSG_PENDING,
    QUA_MSG_ACTIVE
} qua_msg_status;

typedef struct qua_reply_token_t {
    QUA_BOOL reply;
    QUA_S32 result;
} qua_reply_token_t;

typedef struct qua_msg_t {
    qua_list_node_t node;
    QUA_U32 what;
    QUA_VOID *payload;
    QUA_U32 length;
    QUA_S64 whenus;
    qua_msg_priority_t priority;
    qua_msg_handle_fun notify;
    qua_msg_status status;

    qua_reply_token_t *token;
    QUA_S32 result;
} qua_msg_t;

typedef struct qua_msg_queue_t {
    qua_list_node_t list;

    qua_mutex_t lock;
    qua_cond_t cond;
    QUA_BOOL running;
} qua_msg_queue_t;

qua_msg_t *qua_create_msg(QUA_U32 what, QUA_VOID *payload, QUA_U32 length,
    qua_msg_priority_t priority, qua_msg_handle_fun notify);
void qua_destroy_msg(qua_msg_t *msg);
QUA_S32 qua_msg_queue_init(qua_msg_queue_t **msg_queue);
void qua_msg_queue_destroy(qua_msg_queue_t *msg_queue);
QUA_S32 qua_msg_queue_enqueue(qua_msg_queue_t *msg_queue, qua_msg_t *msg, QUA_S64 delayus, QUA_BOOL is_exclude);
qua_msg_t *qua_msg_queue_dequeue(qua_msg_queue_t *msg_queue);
QUA_U32 qua_msg_queue_length(qua_msg_queue_t *msg_queue);
QUA_S32 qua_msg_queue_update(qua_msg_queue_t *msg_queue, qua_msg_t *msg, QUA_S64 delayus);
QUA_S32 qua_msg_queue_remove(qua_msg_queue_t *msg_queue, QUA_U32 what);

#ifdef __cplusplus
}
#endif

#endif /* __QUA_MSG_QUEUE_H__ */
