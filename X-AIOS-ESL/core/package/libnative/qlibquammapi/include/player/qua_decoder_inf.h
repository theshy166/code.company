/*
 *  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include "common/qua_type.h"
#include "avext/common/qua_mm_avcommon.h"

QUA_VOID *qua_decoder_create(QUA_CONST_CHAR *mimetype, QUA_VOID *mime_extra);
QUA_S32 qua_decoder_prepare(QUA_VOID *dec, QUA_VOID *param);
QUA_S32 qua_decoder_start(QUA_VOID *dec);
QUA_S32 qua_decoder_stop(QUA_VOID *dec);
QUA_S32 qua_decoder_reset(QUA_VOID *dec);
QUA_S32 qua_decoder_pause(QUA_VOID *dec);
QUA_S32 qua_decoder_resume(QUA_VOID *dec);
QUA_S32 qua_decoder_set_param(QUA_VOID *dec, QUA_S32 key, QUA_VOID *value);
QUA_S32 qua_decoder_get_param(QUA_VOID *dec, QUA_S32 key, QUA_VOID *value);
QUA_S32 qua_decoder_queue_buffer(QUA_VOID *dec, qua_packet_t *packet);
QUA_S32 qua_decoder_dequeue_buffer(QUA_VOID *dec, QUA_VOID *frame);
QUA_S32 qua_decoder_release_buffer(QUA_VOID *dec, QUA_VOID *frame);
QUA_S32 qua_decoder_flush(QUA_VOID *dec);
QUA_S32 qua_decoder_destroy(QUA_VOID *dec);
