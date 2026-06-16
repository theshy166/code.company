/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_GL_BUFFER_H__
#define __QUA_GL_BUFFER_H__

#include "common/qua_mm_video_common_type.h"
#include "common/qua_type.h"

typedef struct qua_gl_buffer {
    char *name;
    QUA_ULONG phy_addr;
    QUA_VOID_PTR vir_addr;
    QUA_U32 length;
} qua_gl_buffer_t;

#define qua_gl_buffer_alloc(name, width, height, pix_fmt, cache) \
        qua_gl_buffer_alloc_with_caller(name, width, height, pix_fmt, cache, __func__)

#define qua_gl_buffer_free(gl_buffer) \
        qua_gl_buffer_free_with_caller(gl_buffer, __func__)

#define qua_gl_buffer_get_hor_stride(gl_buffer) \
        qua_gl_buffer_get_hor_stride_with_caller(gl_buffer, __func__)

#define qua_gl_buffer_get_ver_stride(gl_buffer) \
        qua_gl_buffer_get_ver_stride_with_caller(gl_buffer, __func__)

#ifdef __cplusplus
extern "C" {
#endif

qua_gl_buffer_t *qua_gl_buffer_alloc_with_caller(QUA_CONST_CHAR *name, QUA_U32 width, QUA_U32 height,
                                                     qua_pixel_fmt_t pix_fmt, QUA_BOOL cache, const char *caller);
QUA_S32 qua_gl_buffer_free_with_caller(qua_gl_buffer_t *gl_buffer, const char *caller);
QUA_S32 qua_gl_buffer_get_hor_stride_with_caller(qua_gl_buffer_t *gl_buffer, const char *caller);
QUA_S32 qua_gl_buffer_get_ver_stride_with_caller(qua_gl_buffer_t *gl_buffer, const char *caller);

#ifdef __cplusplus
}
#endif

#endif // __QUA_GL_BUFFER_H__