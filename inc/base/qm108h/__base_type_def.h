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

typedef unsigned long long   FH_UINT64;
typedef long long            FH_SINT64;
typedef unsigned int         FH_UINT32;
typedef int                  FH_SINT32;
typedef unsigned short       FH_UINT16;
typedef short                FH_SINT16;
typedef unsigned char        FH_UINT8;
typedef signed char          FH_SINT8;
typedef FH_UINT8*            FH_ADDR;
typedef float                FH_FLOAT;
typedef double               FH_DOUBLE;

typedef char                 FH_CHAR;
#define FH_VOID              void

typedef enum {
	FH_FALSE = 0,
	FH_TRUE  = 1,
	DUMMY = 0xffffffff,
} FH_BOOL;


#define FH_NULL     0L
#define FH_SUCCESS  0
#define FH_FAILURE  (-1)

typedef FH_SINT32 VO_INTF_TYPE_E;

typedef enum
{
	VO_DHD0 = 0,			//!< 高清设备
	VO_DSD0 = 1				//!< 标清设备
}VO_DEV_ID_E;

#ifdef __cplusplus
}
#endif

#endif /* __BASE_TYPE_DEF_H__ */

