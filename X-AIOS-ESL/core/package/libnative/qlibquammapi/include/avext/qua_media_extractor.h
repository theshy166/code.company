/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MEDIAEXTRACTOR_API_H__
#define __QUA_MEDIAEXTRACTOR_API_H__

#include "common/qua_type.h"
#include "avext/common/qua_mm_metaformat.h"
#include "avext/common/qua_mm_avcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

extern QUA_VOID* qua_mediaextractor_new(const QUA_CHAR* source);
extern QUA_SIZE_T qua_mediaextractor_trackCount(QUA_VOID* ex);
extern QUA_S32 qua_mediaextractor_getAudioTrackIndex(QUA_VOID* ex);
extern QUA_S32 qua_mediaextractor_getVideoTrackIndex(QUA_VOID* ex);
extern QUA_S32 qua_mediaextractor_metaData(QUA_VOID* ex, qua_metaformat_t** meta);
extern QUA_S32 qua_mediaextractor_trackMetaData(QUA_VOID* ex, QUA_SIZE_T trackIndex, qua_metaformat_t** trackMeta);
extern QUA_SSIZE_T qua_mediaextractor_readSampleData(QUA_VOID* ex, QUA_SIZE_T trackIndex, qua_packet_t* quaPacket);
extern QUA_S32 qua_mediaextractor_seekTo(QUA_VOID* ex, QUA_U64 timeUs, seek_mode_t mode);
extern QUA_BOOL qua_mediaextractor_advance(QUA_VOID* ex);
extern QUA_S32 qua_mediaextractor_delete(QUA_VOID* ex);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __QUA_MEDIAEXTRACTOR_API_H__
