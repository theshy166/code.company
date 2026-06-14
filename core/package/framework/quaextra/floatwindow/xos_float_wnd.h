/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file xos_float_wnd.h
 *
 */
#ifndef XOS_FLOAT_WND_H
#define XOS_FLOAT_WND_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

#if XOS_USE_FLOAT_WND != 0

//if delay > 0, will show float window after 'delay' seconds.
void init_float_window(uint8_t delay);


#endif /*XOS_USE_FLOAT_WND*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*XOS_FLOAT_WND_H*/
