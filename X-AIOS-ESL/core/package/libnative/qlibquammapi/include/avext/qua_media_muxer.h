/*
 *  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MEDIAMUXER_API_H__
#define __QUA_MEDIAMUXER_API_H__

#include "common/qua_type.h"
#include "avext/common/qua_mm_metaformat.h"
#include "avext/common/qua_mm_avcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

extern QUA_VOID* qua_mediamuxer_new(const QUA_CHAR* url, qua_output_format_t format);
extern QUA_S32 qua_mediamuxer_trackMetadata(QUA_VOID* mux, qua_mediastream_type_t stream_type, qua_metaformat_t* track_meta);
extern QUA_S32 qua_mediamuxer_start(QUA_VOID* mux);
extern QUA_S32 qua_mediamuxer_writePacket(QUA_VOID* mux, qua_mediastream_type_t stream_type, qua_packet_t* packet);
extern QUA_S32 qua_mediamuxer_setParam(QUA_VOID* mux, QUA_S32 key, QUA_VOID_PTR param);
extern QUA_S32 qua_mediamuxer_getParam(QUA_VOID* mux, QUA_S32 key, QUA_VOID_PTR param);
extern QUA_S32 qua_mediamuxer_stop(QUA_VOID* mux);
extern QUA_S32 qua_mediamuxer_delete(QUA_VOID* mux);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __QUA_MEDIAMUXER_API_H__