/*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file appcommon.h
 *
 */
#ifndef _APP_COMMON_H_
#define _APP_COMMON_H_
#include <stdlib.h>
/*********************
 *      DEFINES
 *********************/
#define SUCCESS                 0
#define ERR_INVALID_PARAM       -1
#define ERR_QUEUE_FULL          -2
#define ERR_MALLOC_FAIL         -3

#define ENABLE_APPMGR_DEBUG 1

#if ENABLE_APPMGR_DEBUG
#define APPMGR_PRINT(...) do { \
    LV_LOG_USER(__VA_ARGS__); \
} while (0)
#else
#define APPMGR_PRINT(...)
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef enum app_state
{
    INIT,
    RUNNING,
    SLEEP,
}app_state;

typedef struct _msg_param
{
    int val;       //a direct value like 123
    void *data;    //a certain type defined by app
} msg_param;

typedef void* app_data_ptr;
typedef unsigned int msg_wparam;
typedef unsigned int msg_lparam;
typedef int* msg_handle;

typedef struct qua_app_ops {
    app_data_ptr (*on_create)(void* intent);
    void (*on_destory)(app_data_ptr data);
    void (*on_sleep)();
    void (*on_wakeup)();
    int (*on_message)(int msg, msg_param param);
} qua_app_ops;

typedef struct app_info {
    const char* app_name;
    qua_app_ops ops;
} app_info;

typedef struct app_manager_info {
    app_info  *app_ptr;
    void* app_data;
    app_state state;
} app_manager_info;

#endif
