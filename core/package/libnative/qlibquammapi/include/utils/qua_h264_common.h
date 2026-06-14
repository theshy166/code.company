/*
 *  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_H264_COMMON_H__
#define __QUA_H264_COMMON_H__

#define QUA_MAX_SPS      32
#define QUA_MAX_PPS      32

#define QUA_VAL_CHECK(ret, val, ...)\
do{\
    if (!(val)) {\
        ret = -1;\
        printf("value error(%d).\n", __LINE__);\
        goto __FAILED;\
    }\
} while (0)

#define QUA_FUN_CHECK(val)\
do{\
    if ((val) < 0) {\
        printf("Function error(%d).\n", __LINE__);\
        goto __FAILED;\
    }\
} while (0)

typedef enum {
    QUA_H264_Scaling_List_4x4_Length = 16,
    QUA_H264_Scaling_List_8x8_Length = 64,
} qua_scaling_list_length_t;

#endif // __QUA_H264_COMMON_H__