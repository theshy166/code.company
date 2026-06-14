/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
#ifndef __QUARESTYPES_H__
#define __QUARESTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short ResID;

typedef int QBool;
typedef char QChar;
typedef unsigned char QByte;
typedef char* QPStr;

typedef const char* QCPStr;
typedef int QInt;
typedef signed int QSInt;
typedef unsigned int QUInt;
typedef long QLong;

typedef int QInt32;
typedef unsigned int QUInt32;
typedef unsigned long long  QUInt64;
typedef long long QInt64;
typedef short QInt16;
typedef unsigned short QUInt16;
typedef char QInt8;
typedef unsigned char QUInt8;
typedef float QFloat;
typedef unsigned int QSize;
typedef unsigned int QSize_t;
typedef void* QPointer;

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

#ifdef __cplusplus
#define NAMESPACE_BEGIN     namespace QUAX {   
#define NAMESPACE_END       }   /*namespace QUAX*/
#define USE_QUAX_NAMESPACE  using namespace QUAX;
#else
#define NAMESPACE_BEGIN
#define NAMESPACE_END
#define USE_QUAX_NAMESPACE
#endif

#ifndef NULL
#define NULL    ((void *) 0)
#endif

#ifndef TRUE
#define TRUE	(0 == 0)
#endif

#ifndef FALSE
#define FALSE	(0 == 1)
#endif

enum enumResType {
	R_TYPE_VOID = 0,
	R_TYPE_TEXT,
	R_TYPE_IMAGE,
	NR_RES_TYPE_MAX,
};

#define PKG_ID_MASK 0xFFF00000
#define RES_TYPE_MASK 0x000F0000
#define RES_IDX_MASK 0x0000FFFF

#define PKG_ID_SHIFT 20
#define RES_TYPE_SHIFT 16
#define RES_IDX_SHIFT 0

#define MAKE_RES_ID(pkgID, type, idx)             \
    ((pkgID << PKG_ID_SHIFT)   & PKG_ID_MASK  ) | \
    ((type  << RES_TYPE_SHIFT) & RES_TYPE_MASK) | \
    ((idx   << RES_IDX_SHIFT)  & RES_IDX_MASK )

#define RPKGID(resID)   ((resID & PKG_ID_MASK  ) >> PKG_ID_SHIFT  )
#define RESTYPE(resID)  ((resID & RES_TYPE_MASK) >> RES_TYPE_SHIFT)
#define RESINDEX(resID) ((resID & RES_IDX_MASK ) >> RES_IDX_SHIFT )

typedef struct INNER_RES_INFO_T
{
    unsigned int res_id;
    const char *res_name;
    unsigned char *data;
    unsigned int size;
} INNER_RES_INFO;

typedef struct INNER_RES_ARRAY_T
{
    int type;
    const INNER_RES_INFO *pInner_res_info;
    int number;
} INNER_RES_ARRAY;

typedef struct _TResouceEntry
{
	int id;
	const char *value;
} TResourceEntry;

typedef const TResourceEntry *CONST_PTR_TResourceEntry;

#ifdef __cplusplus
}
#endif

#endif /*__QUARESTYPES_H__*/
