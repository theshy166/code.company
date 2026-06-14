/*******************************************************************************************
#  Copyright (c) 2025~2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_DVR_H__
#define __QUA_DVR_H__

#include "qua_dvr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *qua_dvr_handle_t;

int32_t qua_dvr_create(qua_dvr_create_param_t *create_param, qua_dvr_handle_t *out_handle);
void qua_dvr_destroy(qua_dvr_handle_t handle);
int32_t qua_dvr_init(qua_dvr_handle_t handle);
int32_t qua_dvr_uninit(qua_dvr_handle_t handle);
int32_t qua_dvr_stream_on(qua_dvr_handle_t handle);
int32_t qua_dvr_stream_off(qua_dvr_handle_t handle);

qua_dvr_error_code_e qua_dvr_get_hw_status();
#ifdef __cplusplus
}
#endif
#endif