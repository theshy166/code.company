/*******************************************************************************************
#  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_KEYED_VECTOR_H__

#include <stdlib.h>
#include <string.h>
#include "common/qua_type.h"

typedef struct {
    QUA_CHAR *key;
    QUA_VOID_PTR value;
} key_value_pair;

typedef struct {
    key_value_pair *data;
    QUA_U16 size;
    QUA_U16 capacity;
} qua_keyed_vector_t;

#ifdef __cplusplus
extern "C" {
#endif

qua_keyed_vector_t *qua_kv_create();
QUA_VOID qua_kv_destroy(qua_keyed_vector_t *vector);
QUA_VOID qua_kv_clear(qua_keyed_vector_t *vector);
QUA_BOOL qua_kv_push(qua_keyed_vector_t *vector, QUA_CONST_CHAR *key, QUA_VOID_PTR value);
QUA_VOID *qua_kv_get_by_index(qua_keyed_vector_t *vector, QUA_S32 index);
QUA_VOID *qua_kv_get_by_key(qua_keyed_vector_t *vector, QUA_CONST_CHAR *key);
QUA_S32 qua_kv_get_index(qua_keyed_vector_t *vector, QUA_CONST_CHAR *key);
QUA_BOOL qua_kv_remove_index(qua_keyed_vector_t *vector, QUA_U16 index);
QUA_BOOL qua_kv_remove_key(qua_keyed_vector_t *vector, QUA_CONST_CHAR *key);
QUA_U16 qua_kv_size(qua_keyed_vector_t *vector);

#ifdef __cplusplus
}
#endif

#endif