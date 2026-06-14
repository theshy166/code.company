/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_COMMON_H__
#define __QUA_MM_COMMON_H__

#include "qua_type.h"

#define QUA_MM_MODULE_VIDEO "quamm.video"
#define QUA_MM_MODULE_AUDIO "quamm.audio"
#define QUA_MM_MODULE_DISPLAY "quamm.display"
#define QUA_MM_MODULE_CAMERA "quamm.camera"
#define QUA_MM_MODULE_FILTER "quamm.filter"
#define QUA_MM_MODULE_AVEXT "quamm.avext"

#define QUA_MM_MAKE_API_VERSION(maj, min) ((((maj)&0xff) << 8) | ((min)&0xff))

struct qua_mm_device_t;
struct qua_mm_channel_t;

typedef struct qua_mm_module_t {
    /** API version of module */
    QUA_U16 api_version;

    /** Identifier of module */
    QUA_CONST_CHAR *id;

    /** module's dso */
    QUA_VOID_PTR dso;

    /** Open a specific device of this module */
    QUA_S32 (*open_device)(const struct qua_mm_module_t* module, QUA_CONST_CHAR* dev_id,
            QUA_S32 dev_num, struct qua_mm_device_t** device);
} qua_mm_module_t;

typedef struct qua_mm_device_t {
    /** Identifier of device */
    QUA_CONST_CHAR *id;

    /** reference to the module this device belongs to */
    struct qua_mm_module_t* module;

    /** Close this device */
    QUA_S32 (*close)(struct qua_mm_device_t* device);

    /** Create a channel of this device if it has */
    QUA_S32 (*create_channel)(const struct qua_mm_device_t* device, QUA_S32* chn_id,
            QUA_VOID_PTR chn_attr, struct qua_mm_channel_t** channel);
} qua_mm_device_t;

typedef struct qua_mm_channel_t {
    /** Identifier of channel */
    QUA_S32 id;

    /** reference to the device this channel belongs to */
    struct qua_mm_device_t* device;

    /** Release this channel */
    QUA_S32 (*release)(struct qua_mm_channel_t* channel);
} qua_mm_channel_t;

typedef struct qua_mm_system_t {
    /** Module info related to system,
     * the application should not care it.
     */
    struct qua_mm_module_t mod_info;

    /** The version of QuaMM SDK */
    QUA_CONST_CHAR *sdk_version;
} qua_mm_system_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * QuaMM low-level initialization and return the system functions for use.
 *
 * @return: 0 == success, <0 == error and *sys_ops == NULL
 */
QUA_S32 qua_mm_init(QUA_BOOL primary_user, QUA_CONST_CHAR *platform, qua_mm_system_t **system);

/**
 * Load the module info associated with a module by id.
 *
 * @return: 0 == success, <0 == error and *module == NULL
 */
QUA_S32 qua_mm_load_module(QUA_CONST_CHAR *id, const qua_mm_module_t **module);

/**
 * Get the platform name with format <CHIP>_<OS>.
 *
 * @return: the string of platform name or NULL
 */
QUA_CONST_CHAR *qua_mm_get_platform();
#ifdef __cplusplus
}
#endif

#endif
