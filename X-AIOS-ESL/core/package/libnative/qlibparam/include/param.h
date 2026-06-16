/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file param.h
 *
 */

#ifndef XOS_PARAM_H
#define XOS_PARAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iniparser.h"
#include <pthread.h>

#define PARAM_FILE_NAME "/data/config/param.ini"

typedef struct {
    char *entry;
    int  *val;
} IntArgs;

typedef struct {
    char *entry;
    char *val;
} StringArgs;

extern dictionary *g_ini_d_;

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

#endif /*XOS_PARAM_H*/