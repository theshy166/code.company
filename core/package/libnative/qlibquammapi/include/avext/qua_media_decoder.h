/*
 *  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_AV_DECODER_API_H__
#define __QUA_AV_DECODER_API_H__

#include "common/qua_type.h"
#include "avext/common/qua_mm_avcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

extern QUA_VOID* qua_avdecoder_new(QUA_CONST_CHAR* mimetype, QUA_VOID* mime_extra, QUA_VOID* param);
extern QUA_S32 qua_avdecoder_start(QUA_VOID* dec);
extern QUA_S32 qua_avdecoder_sendPacket(QUA_VOID* dec, qua_packet_t* pkt);
extern QUA_S32 qua_avdecoder_receiveFrame(QUA_VOID* dec, QUA_VOID* frame);
extern QUA_S32 qua_avdecoder_query(QUA_VOID* dec);
extern QUA_S32 qua_avdecoder_flush(QUA_VOID* dec);
extern QUA_S32 qua_avdecoder_stop(QUA_VOID* dec);
extern QUA_S32 qua_avdecoder_delete(QUA_VOID* dec);
extern QUA_S32 qua_avdecoder_setParam(QUA_VOID* dec, const QUA_CHAR *key, QUA_VOID* param);
extern QUA_S32 qua_avdecoder_getParam(QUA_VOID* dec, const QUA_CHAR *key, QUA_VOID* param);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __QUA_AV_DECODER_API_H__
