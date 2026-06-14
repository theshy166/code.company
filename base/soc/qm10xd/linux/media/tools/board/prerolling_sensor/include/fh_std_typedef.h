/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * 类型定义,将之前SDK API使用的接口沿用到驱动中
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */

#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef unsigned long long   FH_UINT64;
typedef long long            FH_SINT64;
typedef unsigned int         FH_UINT32;
typedef int                  FH_SINT32;
typedef unsigned short       FH_UINT16;
typedef short                FH_SINT16;
typedef unsigned char        FH_UINT8;
typedef signed char          FH_SINT8;
typedef FH_UINT8*            FH_ADDR;
typedef unsigned long        FH_ULONG32;
typedef long                 FH_SLONG32;
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

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
