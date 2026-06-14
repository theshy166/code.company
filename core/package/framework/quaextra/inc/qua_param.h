/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file qua_param.h
 *
 */

#ifndef QUA_PARAM_H
#define QUA_PARAM_H

#ifdef __cplusplus
extern "C" {
#endif

int param_get_int(const char *entry, int default_val);
int param_set_int(const char *entry, int val);
const char *param_get_string(const char *entry, const char *default_val);
int param_set_string(const char *entry, const char *val);
int param_save();
int param_init(char *ini_path);
int param_deinit();
int param_reload();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*QUA_PARAM_H*/