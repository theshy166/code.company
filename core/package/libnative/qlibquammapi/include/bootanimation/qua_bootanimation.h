/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_BOOTANIMATION_H__
#define __QUA_BOOTANIMATION_H__

#include "display/qua_mm_display.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum qua_bootanimation_status {
    QUA_BOOTANIMAION_STATUS_UNKNOWN = 0,
    QUA_BOOTANIMAION_STATUS_SYS_INITED,
    QUA_BOOTANIMAION_STATUS_VO_INITED,
    QUA_BOOTANIMAION_STATUS_BOOT_FINISH,
    QUA_BOOTANIMAION_STATUS_CLEANING,
    QUA_BOOTANIMAION_STATUS_CLEAN_FINISH,
    QUA_BOOTANIMAION_STATUS_SHUTDOWNING,
    QUA_BOOTANIMAION_STATUS_SHUTDOWN_FINISH,
    QUA_BOOTANIMAION_STATUS_BUTT,
} qua_bootanimation_status_t;

typedef enum qua_bootanimation_cmd {
    QUA_BOOTANIMAION_CMD_INVALID = 0,
    QUA_BOOTANIMAION_CMD_QUERY_STATUS,
    QUA_BOOTANIMAION_CMD_CLEAN,
    QUA_BOOTANIMAION_CMD_SHUTDOWN,
    QUA_BOOTANIMAION_CMD_BUTT,
} qua_bootanimation_cmd_t;

QUA_S32 qua_init_bootanimation_client();
QUA_VOID qua_uninit_bootanimation_client();
QUA_S32 qua_request_bootanimation(qua_bootanimation_cmd_t cmd);
QUA_S32 qua_wait_until_bootanimation_status(qua_bootanimation_status_t status);
QUA_S32 qua_start_shutdownanimation(qua_mm_vo_device_t *vo_device, qua_mm_fb_device_t *fb_device);

#ifdef __cplusplus
}
#endif
#endif   /* __QUA_BOOTANIMATION_H__ */

