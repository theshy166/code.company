/*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file appmanager.h
 *
 */
#ifndef _APP_MANAGER_H_
#define _APP_MANAGER_H_
#include "appmessage.h"
#include "lvgl/src/misc/lv_ll.h"

#ifdef __cplusplus
extern "C" {
#endif
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
const char* app_manager_topname(void);
void app_manager_start(const char *app_name, void *start_data);
void app_manager_register(const char *name, qua_app_ops *ops);
bool app_manager_node_exist(app_manager_info *app);
bool app_manager_name_exist(const char *name);
void app_manager_exit(const char *app_name);
void app_manager_init(void);
void app_manager_run(void);
lv_ll_t* app_manager_get_reglist(void);
app_manager_info* app_manager_get_node(const char *name);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*_APP_MANAGER_H_*/
