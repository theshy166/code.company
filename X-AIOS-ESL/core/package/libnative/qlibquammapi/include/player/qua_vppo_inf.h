/*
 *  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

QUA_VOID* qua_vppo_init(QUA_CONST_CHAR *display_id);
QUA_S32 qua_vppo_set_param(QUA_VOID *vppo, QUA_S32 key, QUA_VOID *value);
QUA_S32 qua_vppo_get_param(QUA_VOID *vppo, QUA_S32 key, QUA_VOID *value);
QUA_S32 qua_vppo_prepare(QUA_VOID *vppo);
QUA_S32 qua_vppo_start(QUA_VOID *vppo);
QUA_S32 qua_vppo_send_frame(QUA_VOID *vppo, qua_video_frame_info_t *frame_info);
QUA_S32 qua_vppo_stop(QUA_VOID *vppo);
QUA_S32 qua_vppo_reset(QUA_VOID *vppo);
QUA_S32 qua_vppo_deinit(QUA_VOID *vppo);
