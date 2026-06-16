/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
#ifndef XOS_MISC_H
#define XOS_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "lang/QuaResManager.h"

/*********************
 *      DEFINES
 *********************/
#define CHK_OBJ_NULL_RTN_NULL(obj) do { \
    LV_ASSERT_NULL(obj); \
    if(obj == NULL) return NULL; \
}while(0)

#define CHK_OBJ_NULL_RTN_VOID(obj) do { \
    LV_ASSERT_NULL(obj); \
    if(obj == NULL) return; \
}while(0)

#define CHK_OBJ_GOTO_ERR(obj,err) do { \
    LV_ASSERT_NULL(obj); \
    if(obj == NULL) goto err; \
}while(0)

#define XOS_LOGU(fmt, ...)    LV_LOG_USER(fmt, ##__VA_ARGS__)
#define XOS_LOGI(fmt, ...)    LV_LOG_INFO(fmt, ##__VA_ARGS__)
#define XOS_LOGE(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t * lv_ability_create(lv_obj_t *obj);
lv_obj_t * xos_obj_create(lv_obj_t *obj);

void xos_cm_set_img_bg(lv_obj_t *img);
const char * xos_cm_event_str(lv_event_t *e);
const char * xos_cm_key_str(uint32_t key);

#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif /*XOS_MISC_H*/