/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_GL_H__
#define __QUA_GL_H__

#include "common/qua_type.h"
#include "system/qua_mm_system.h"
#include "common/qua_mm_video_common_type.h"

typedef struct qua_gl_context {

} qua_gl_context_t;

typedef struct qua_gl_surface {
    QUA_U8 *data;
    QUA_ULONG phy_addr;
    QUA_U32 width : 16;
    QUA_U32 height : 16;
    QUA_U32 stride : 16;
    qua_pixel_fmt_t format : 16;
    QUA_U32 crop_x : 16;
    QUA_U32 crop_y : 16;
    QUA_U32 crop_w : 16;
    QUA_U32 crop_h : 16;
} qua_gl_surface_t;

typedef enum qua_gl_rotation {
    QUA_GL_ROTATION_0 = 0,
    QUA_GL_ROTATION_90,
    QUA_GL_ROTATION_180,
    QUA_GL_ROTATION_270,
    QUA_GL_ROTATION_MIRROR,
    QUA_GL_ROTATION_FLIP
} qua_gl_rotation_t;

typedef struct qua_gl_transform {
    QUA_S8 rotate : 1;
    QUA_S8 scale : 1;
    QUA_S8 conversion : 1;
    qua_gl_rotation_t rotation;
} qua_gl_transform_t;

typedef enum qua_gl_param_type {
    QUA_GL_PARAM_VGS_SCALE_MODE,    // qua_vgs_scale_mode_t (qua_mm_filter.h)
    QUA_GL_PARAM_BUTT
} qua_gl_param_type_t;

#ifdef __cplusplus
extern "C" {
#endif

QUA_BOOL qua_gl_init(qua_mm_system_ops_t *sys_ops);
QUA_BOOL qua_gl_deinit();
QUA_BOOL qua_gl_transform(qua_gl_transform_t *transform, qua_gl_surface_t *src, qua_gl_surface_t *dst);
QUA_BOOL qua_gl_blend(qua_gl_transform_t *transform, qua_gl_surface_t *src, qua_gl_surface_t *dst);
QUA_BOOL qua_gl_fill(qua_gl_surface_t *src, QUA_U32 fill_data);
QUA_BOOL qua_gl_set_param(qua_gl_param_type_t param, void *value);
#ifdef __cplusplus
}
#endif

#endif
