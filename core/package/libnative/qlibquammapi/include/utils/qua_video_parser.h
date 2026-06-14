/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_VIDEO_PARSER_H__
#define __QUA_VIDEO_PARSER_H__

#include "common/qua_type.h"
#include "avext/common/qua_mm_avcommon.h"
#include "video/qua_mm_video_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct qua_jpeg_info_entry_t {
    QUA_U32 width;
    QUA_U32 height;
    qua_vdec_jpeg_color_fmt_t format;
} qua_jpeg_info_entry_t;

qua_packet_t *qua_alloc_video_packet(QUA_U64 size);
QUA_VOID qua_set_video_packet_default(qua_packet_t* pkt);
QUA_VOID qua_free_video_packet(qua_packet_t *pkt);
QUA_S32 qua_get_jpeg_info(const QUA_U8 *data, QUA_U32 length, qua_jpeg_info_entry_t *entry);
QUA_S32 qua_get_jpeg_info_by_fd(QUA_S32 fd, qua_jpeg_info_entry_t *entry);
QUA_S32 qua_parse_h2645_csd_info(QUA_U8 *data, QUA_U32 size, QUA_BOOL is_avc,
    QUA_U32 *width, QUA_U32 *height, QUA_U32 *num_ref_frame);
QUA_BOOL qua_is_jpeg_file(QUA_CHAR *url);

#ifdef __cplusplus
}
#endif

#endif /* __QUA_VIDEO_PARSER_H__ */
