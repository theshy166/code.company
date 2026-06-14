/*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file appmessage.h
 *
 */
#ifndef _APP_MESSAGE_H_
#define _APP_MESSAGE_H_

#include <pthread.h>
#include "appcommon.h"
#include <semaphore.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      DEFINES
 *********************/
#define DEF_MSGQUEUE_LEN    32

/**********************
 *      TYPEDEFS
 **********************/
typedef struct appmsg_t
{
    /** The handle to the app which receives this message. */
    msg_handle              hwnd;
    /** The message identifier. */
    int               message;
    /** The parameter of the message. */
    msg_param        param;
} appmsg;

typedef struct msg_queue_t
{
    pthread_mutex_t lock;       // lock
    //pthread_mutex_t wait;       // wait for queue
    sem_t wait_s;       // wait for queue
    appmsg* msg;                   /* post message buffer */
    int len;                    /* buffer len */
    int readpos, writepos;      /* positions for reading and writing */

} msg_queue;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
int am_message_queue_init(int buffer_len);
int am_get_message(appmsg *app_msg);
int am_post_message(msg_handle hwnd, int msg, msg_param param);
int am_broadcast_message(int msg, msg_param param);
int am_dispatch_message(msg_handle hwnd, int msg, msg_param param);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*_APP_STACK_H_*/

