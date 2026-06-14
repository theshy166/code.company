/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_HANDLER_H__
#define __QUA_HANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include "common/qua_type.h"
#include "qua_msg_queue.h"

#include "utils/qua_thread.h"

typedef struct qua_handler_t {
    qua_msg_queue_t *queue;
    qua_msg_handle_fun callback;

    qua_thread_t thread;
    QUA_BOOL running;

    qua_mutex_t reply_lock;
    qua_cond_t reply_cond;
} qua_handler_t;

qua_handler_t *qua_handler_create(qua_msg_handle_fun callback, const char *name);
QUA_S32 qua_handler_destroy(qua_handler_t *handler);
QUA_S32 qua_handler_post_msg(qua_handler_t *handler, qua_msg_t *msg, QUA_S64 delayus);
QUA_S32 qua_handler_post_msg_exclude(qua_handler_t *handler, qua_msg_t *msg, QUA_S64 delayus);
QUA_S32 qua_handler_post_msg_and_wait_response(qua_handler_t *handler, qua_msg_t *msg);
QUA_S32 qua_handler_post_reply(qua_handler_t *handler, qua_msg_t *msg);
QUA_S32 qua_handler_msg_queue_length(qua_handler_t *handler);
QUA_S32 qua_handler_msg_update(qua_handler_t *handler, qua_msg_t *msg, QUA_S64 delayus);
QUA_S32 qua_handler_msg_remove(qua_handler_t *handler, QUA_U32 what);

#ifdef __cplusplus
}
#endif

#endif /* __QUA_HANDLER_H__ */
