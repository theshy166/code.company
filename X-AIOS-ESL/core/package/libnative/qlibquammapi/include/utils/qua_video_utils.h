/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_VIDEO_UTILS_H__
#define __QUA_VIDEO_UTILS_H__

#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

#ifdef __cplusplus
extern "C" {
#endif

QUA_S32 qua_video_image_buf_size(QUA_S32 stride, QUA_S32 height,
    QUA_S32 hor_align, QUA_S32 ver_align, qua_pixel_fmt_t format);

#ifdef __cplusplus
}
#endif

#endif // __QUA_VIDEO_UTILS_H__