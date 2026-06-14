/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_SYS_PLATFORM_H__
#define __QUA_SYS_PLATFORM_H__

#include "qua_type.h"

#define QUA_SYS_OS_ANDROID "android"
#define QUA_SYS_OS_HARMONY "harmony"
#define QUA_SYS_OS_LINUX "linux"
#define QUA_SYS_OS_RTT "rtt"
#define QUA_SYS_CHIP_MC6870 "mc6870"
#define QUA_SYS_CHIP_MC331x "mc331x"
#define QUA_SYS_CHIP_QM10XH "qm10xh"
#define QUA_SYS_CHIP_QM10XV "qm10xv"
#define QUA_SYS_CHIP_QM10XD "qm10xd"

#define QUA_SYS_MAKE_PLATFORM(chip, os) (chip "_" os)

#define QUA_SYS_ARCH_ARM "arm"
#define QUA_SYS_ARCH_ARM64 "arm64"
#define QUA_MAX_PLATFORM_STR_LEN 128

#ifdef __cplusplus
extern "C" {
#endif

typedef enum qua_arg_key {
    QUA_ARG_CHIP_NAME = 0,
    QUA_ARG_OS_NAME,
    QUA_ARG_IMAGE_WIDTH,
    QUA_ARG_IMAGE_HEIGHT,
    QUA_ARG_INTF_TYPE,
    QUA_ARG_INTF_SYNC,
    QUA_ARG_FRAME_RATE,
    QUA_ARG_PRIMARY_USER,
    QUA_ARG_INPUT_FILE,
    QUA_ARG_OUTPUT_FILE,
    QUA_ARG_CODING_FORMAT,
    QUA_ARG_VB_COUNT,
    QUA_ARG_CHANNEL_ID,
    QUA_ARG_TEST_MODE,
    QUA_ARG_BUTT
} qua_arg_key_t;

/**
 * Make platform name.
 *
 * @return: QUA_SUCCESS or QUA_FAILURE
 */
QUA_S32 qua_make_platform(QUA_CONST_CHAR *chip_name, QUA_CONST_CHAR *os_name, QUA_CHAR *platform, QUA_S32 size);

/**
 * Check whether the platform contains the specified chip.
 *
 * @return: QUA_TRUE or QUA_FALSE
 */
QUA_BOOL qua_check_chip(QUA_CONST_CHAR *chip_name);

/**
 * Check whether the platform contains the specified OS.
 *
 * @return: QUA_TRUE or QUA_FALSE
 */
QUA_BOOL qua_check_os(QUA_CONST_CHAR *os_name);

/**
 * Get one arg in command line once.
 *
 * @return: QUA_SUCCESS or QUA_FAILURE
 */
QUA_S32 qua_get_cmdline_arg(QUA_S32 argc, QUA_CHAR** argv, qua_arg_key_t key, QUA_CHAR *value, QUA_S32 size);

QUA_CONST_CHAR* qua_sys_arch();
QUA_CONST_CHAR* qua_sys_os();
#ifdef __cplusplus
}
#endif

#endif
