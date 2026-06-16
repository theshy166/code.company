/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_C_TINY_FORMAT_H__
#define __QUA_C_TINY_FORMAT_H__

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    TinyFormat_kTypeInt32     = 0,
    TinyFormat_kTypeInt64     = 1,
    TinyFormat_kTypeString    = 2,
    TinyFormat_kTypeBuffer    = 3,
};

enum {
    TinyFormat_OK                       = 0,
    TinyFormat_FAIL_TO_INSERT           = -1,
    TinyFormat_KEY_NOT_EXIST            = -2,
    TinyFormat_INVALID_VALUE_TYPE       = -3,
    TinyFormat_INVALID_ARGUMENT         = -4,
};

typedef struct __PAIRS__ {
    char *key;
    int valueType;
    union {
        int32_t value_int32;
        int64_t value_int64;
        void *value;
        struct {
            void *data;
            size_t offset;
            size_t size;
        } buffer;
    } u;
    struct __PAIRS__ *next;
} Pairs;

typedef struct qua_metaformat qua_metaformat_t;

typedef int (*Set_string)(qua_metaformat_t* tinyformat, const char* key, const char* value);

typedef int (*Set_Int32)(qua_metaformat_t* tinyformat, const char* key, int32_t value);

typedef int (*Set_Int64)(qua_metaformat_t* tinyformat, const char* key, int64_t value);

typedef int (*Set_Buffer)(qua_metaformat_t* tinyformat, const char* key, const void* data, size_t size);

typedef int (*Remove)(qua_metaformat_t* tinyformat, const char* key);

typedef bool (*Empty)(qua_metaformat_t* tinyformat);

typedef int (*Get_string)(qua_metaformat_t* tinyformat, const char* key, char *out);

typedef int (*Get_Int32)(qua_metaformat_t* tinyformat, const char* key, int32_t *value);

typedef int (*Get_Int64)(qua_metaformat_t* tinyformat, const char* key, int64_t *value);

typedef int (*Get_Buffer)(qua_metaformat_t* tinyformat, const char* key, void **out, size_t *outSize);

typedef size_t (*Size)(qua_metaformat_t* tinyformat);

typedef int (*Clear)(qua_metaformat_t* tinyformat);

struct qua_metaformat {
    Pairs* list;
    size_t listSize;

    char *name;
    pthread_mutex_t thizmutex;

    Set_string set_string;
    Set_Int32 set_int32;
    Set_Int64 set_int64;
    Set_Buffer set_buffer;
    Remove remove;
    Get_string get_string;
    Get_Int32 get_int32;
    Get_Int64 get_int64;
    Get_Buffer get_buffer;
    Empty empty;
    Size size;
    Clear clear;
};

// extern API
extern qua_metaformat_t *tinyformat_new(char* name);

extern int tinyformat_delete(qua_metaformat_t* tinyformat);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __QUA_C_TINY_FORMAT_H__