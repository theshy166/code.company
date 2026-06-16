/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __LV_QUA_HWJPEG_H_
#define __LV_QUA_HWJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../lv_conf_internal.h"

#if LV_USE_HW_JPEG

/**
 * Register qua hardware jpeg decoder functions in lvgl.
 */
void lv_qua_hwjpeg_init(void);
void lv_qua_hwjpeg_deinit(void);

#endif /* CONFIG_XOS_USE_HW_JPEG */

#ifdef __cplusplus
}
#endif

#endif // __LV_QUA_HWJPEG_H_