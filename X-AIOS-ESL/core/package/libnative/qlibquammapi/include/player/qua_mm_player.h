/*
 *  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MM_PLAYER_H__
#define __QUA_MM_PLAYER_H__

#include "common/qua_type.h"
#include "player/qua_mm_player_common.h"

QUA_VOID* qua_mm_player_create(QUA_CONST_CHAR* name);
QUA_S32 qua_mm_player_set_data_source(QUA_VOID* player, QUA_CONST_CHAR* url);
QUA_S32 qua_mm_player_set_loop(QUA_VOID* player, QUA_BOOL loop);
QUA_S32 qua_mm_player_prepare(QUA_VOID* player);
QUA_S32 qua_mm_player_set_parameter(QUA_VOID* player, QUA_S32 key, QUA_VOID* value);
QUA_S32 qua_mm_player_get_parameter(QUA_VOID* player, QUA_S32 key, QUA_VOID* value);
QUA_S32 qua_mm_player_start(QUA_VOID* player);
QUA_S32 qua_mm_player_pause(QUA_VOID* player);
QUA_S32 qua_mm_player_resume(QUA_VOID* player);
QUA_S32 qua_mm_player_stop(QUA_VOID* player);
QUA_S32 qua_mm_player_reset(QUA_VOID* player);
QUA_S32 qua_mm_player_seek(QUA_VOID* player, QUA_U64 timeUs);
QUA_S64 qua_mm_player_get_durationUs(QUA_VOID* player);
QUA_S32 qua_mm_player_get_file_meta(QUA_VOID* player, QUA_VOID** meta);
QUA_U64 qua_mm_player_get_current_position(QUA_VOID* player);
QUA_S32 qua_mm_player_set_volume(QUA_VOID* player, QUA_S32 volume);
QUA_S32 qua_mm_player_set_callback(QUA_VOID* player, CallBack cb, QUA_VOID* user);
QUA_S32 qua_mm_player_destroy(QUA_VOID* player);

// async api
QUA_S32 qua_mm_player_stop_async(QUA_VOID* player);
QUA_S32 qua_mm_player_destroy_async(QUA_VOID* player);

#endif // __QUA_MM_PLAYER_H__