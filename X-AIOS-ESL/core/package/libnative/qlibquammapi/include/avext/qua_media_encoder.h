/*
 *  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_AV_ENCODER_API_H__
#define __QUA_AV_ENCODER_API_H__

#include "common/qua_type.h"
#include "avext/common/qua_mm_avcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

extern QUA_VOID* qua_avencoder_new(QUA_CONST_CHAR* mimetype, QUA_VOID* mime_extra, QUA_VOID* param);
extern QUA_S32 qua_avencoder_start(QUA_VOID* enc);
extern QUA_S32 qua_avencoder_sendFrame(QUA_VOID* enc, QUA_VOID* frame);
extern QUA_S32 qua_avencoder_receivePacket(QUA_VOID* enc, qua_packet_t* pkt);
extern QUA_S32 qua_avencoder_query(QUA_VOID* enc);
extern QUA_S32 qua_avencoder_flush(QUA_VOID* enc);
extern QUA_S32 qua_avencoder_stop(QUA_VOID* enc);
extern QUA_S32 qua_avencoder_delete(QUA_VOID* enc);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __QUA_AV_ENCODER_API_H__