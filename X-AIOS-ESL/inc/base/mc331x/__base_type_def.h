/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 */
#ifndef __QUA_XOS_INTERNEL_HEADER_FILE__
#error "Please don't include this file directly, please include base_include.h"
#endif

#ifndef __BASE_TYPE_DEF_H__
#define __BASE_TYPE_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
 * The common data type
 *----------------------------------------------*/

typedef unsigned char           FY_U8;
typedef unsigned short          FY_U16;
typedef unsigned int            FY_U32;
typedef double                  FY_DOUBLE;
typedef signed char             FY_S8;
typedef short                   FY_S16;
typedef int                     FY_S32;

typedef unsigned long long      FY_U64;
typedef long long               FY_S64;

typedef char                    FY_CHAR;
#define FY_VOID                 void

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum {
    FY_FALSE = 0,
    FY_TRUE  = 1,
} FY_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define FY_NULL     0L
#define FY_SUCCESS  0
#define FY_FAILURE  (-1)

typedef FY_S32 VO_DEV;

#ifdef __cplusplus
}
#endif

#endif /* __BASE_TYPE_DEF_H__ */

