/*******************************************************************************************
#  Copyright (c) 2025~2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_DVR_COMMON_H__
#define __QUA_DVR_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "sys/time.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

typedef enum
{
    QUA_DVR_FMT_SP_YUV420 = 0,
    QUA_DVR_FMT_SP_YUV422,
    QUA_DVR_FMT_MJPEG,
    QUA_DVR_FMT_H264,
    QUA_DVR_FMT_MAX
} qua_dvr_format_e;

typedef enum
{
    QUA_DVR_CAMERA_FRONT = 0,
    QUA_DVR_CAMERA_REAR,
    QUA_DVR_CAMERA_USB,
    QUA_DVR_CAMERA_MAX
} qua_dvr_camera_id_e;

typedef enum
{
    QUA_DVR_EVENT_NONE = 0,
    QUA_DVR_EVENT_MAX
} qua_dvr_event_id_e;

typedef enum
{
    QUA_DVR_ERROR_CODE_SUCCESS = 0x00,
    QUA_DVR_ERROR_CODE_FRONT_CAMERA_NOT_DETECTED,
    QUA_DVR_ERROR_CODE_REAR_CAMERA_NOT_DETECTED,
    QUA_DVR_ERROR_CODE_USB_CAMERA_NOT_DETECTED,
    QUA_DVR_ERROR_CODE_DEFAULT = 0xFF,
} qua_dvr_error_code_e;

typedef struct
{
    int32_t frame_rate;
} qua_dvr_camera_desc_t;

typedef struct
{

} qua_dvr_event_info_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} qua_dvr_display_region_t;

typedef struct
{
    qua_dvr_camera_id_e camera_id;
    qua_dvr_camera_desc_t camera_desc;
    qua_dvr_display_region_t display_region;
} qua_dvr_image_window_t;

typedef int32_t (*pf_qua_dvr_cb)(qua_dvr_event_id_e event, qua_dvr_event_info_t *event_info, void *user_data);

typedef struct
{
    void *user_data;
    pf_qua_dvr_cb user_cb;
    int32_t screen_width;
    int32_t screen_height;
    int32_t image_window_number;
    qua_dvr_image_window_t image_window_list[QUA_DVR_CAMERA_MAX];
} qua_dvr_create_param_t;

#endif