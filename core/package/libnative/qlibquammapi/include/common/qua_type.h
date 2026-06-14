/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_TYPE_H__
#define __QUA_TYPE_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if defined(__ANDROID__)
#include <utils/Log.h>
#else

#ifndef LOG_NDEBUG
#define LOG_NDEBUG 0
#endif

#ifndef ALOGI
#define ALOGI(format, ...) printf(format"\n", ##__VA_ARGS__)
#endif

#ifndef ALOGD
#define ALOGD(format, ...) printf(format"\n", ##__VA_ARGS__)
#endif

#ifndef ALOGE
#define ALOGE(format, ...) printf(format"\n", ##__VA_ARGS__)
#endif

#ifndef ALOGW
#define ALOGW(format, ...) printf(format"\n", ##__VA_ARGS__)
#endif

#ifndef ALOGV
#if LOG_NDEBUG
#define ALOGV(format, ...) ((void)0)
#else
#define ALOGV(format, ...) printf(format"\n", ##__VA_ARGS__)
#endif
#endif

#endif

#define ALIGNTO(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define MULTIUP(value, alignment) (((value) + (alignment) - 1) / (alignment) * (alignment))
#define QUA_MAX(a, b) ((a) > (b) ? (a) : (b))
#define QUA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define QUA_ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/

typedef unsigned char           QUA_U8;
typedef unsigned short          QUA_U16;
typedef unsigned int            QUA_U32;
typedef float                   QUA_FLOAT;
typedef double                  QUA_DOUBLE;
typedef signed char             QUA_S8;
typedef short                   QUA_S16;
typedef int                     QUA_S32;
typedef unsigned long           QUA_ULONG;
typedef long                    QUA_LONG;
typedef unsigned long long      QUA_U64;
typedef long long               QUA_S64;
typedef const char              QUA_CONST_CHAR;
typedef char                    QUA_CHAR;
typedef void                    QUA_VOID;
typedef void*                   QUA_VOID_PTR;
typedef size_t                  QUA_SIZE_T;
typedef ssize_t                 QUA_SSIZE_T;
// typedef off64_t                 QUA_OFF64_T;
typedef off_t                   QUA_OFF_T;
typedef int                     QUA_BOOL;
/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
#define QUA_FALSE 0
#define QUA_TRUE 1

#ifndef NULL
    #define NULL    0L
#endif

#define QUA_NULL      0L
#define QUA_SUCCESS   0
#define QUA_ERR_BASE  -10000
#define QUA_FAILURE   QUA_ERR_BASE
#define QUA_UNKNOWN_ERR (QUA_ERR_BASE+1)

#define QUA_EPERM        (QUA_ERR_BASE-1)  /* Operation not permitted */
#define QUA_ENOENT       (QUA_ERR_BASE-2)  /* No such file or directory */
#define QUA_ESRCH        (QUA_ERR_BASE-3)  /* No such process */
#define QUA_EINTR        (QUA_ERR_BASE-4)  /* Interrupted system call */
#define QUA_EIO          (QUA_ERR_BASE-5)  /* I/O error */
#define QUA_ENXIO        (QUA_ERR_BASE-6)  /* No such device or address */
#define QUA_E2BIG        (QUA_ERR_BASE-7)  /* Argument list too long */
#define QUA_ENOEXEC      (QUA_ERR_BASE-8)  /* Exec format error */
#define QUA_EBADF        (QUA_ERR_BASE-9)  /* Bad file number */
#define QUA_ECHILD      (QUA_ERR_BASE-10)  /* No child processes */
#define QUA_EAGAIN      (QUA_ERR_BASE-11)  /* Try again */
#define QUA_ENOMEM      (QUA_ERR_BASE-12)  /* Out of memory */
#define QUA_EACCES      (QUA_ERR_BASE-13)  /* Permission denied */
#define QUA_EFAULT      (QUA_ERR_BASE-14)  /* Bad address */
#define QUA_ENOTBLK     (QUA_ERR_BASE-15)  /* Block device required */
#define QUA_EBUSY       (QUA_ERR_BASE-16)  /* Device or resource busy */
#define QUA_EEXIST      (QUA_ERR_BASE-17)  /* File exists */
#define QUA_EXDEV       (QUA_ERR_BASE-18)  /* Cross-device link */
#define QUA_ENODEV      (QUA_ERR_BASE-19)  /* No such device */
#define QUA_ENOTDIR     (QUA_ERR_BASE-20)  /* Not a directory */
#define QUA_EISDIR      (QUA_ERR_BASE-21)  /* Is a directory */
#define QUA_EINVAL      (QUA_ERR_BASE-22)  /* Invalid argument */
#define QUA_ENFILE      (QUA_ERR_BASE-23)  /* File table overflow */
#define QUA_EMFILE      (QUA_ERR_BASE-24)  /* Too many open files */
#define QUA_ENOTTY      (QUA_ERR_BASE-25)  /* Not a typewriter */
#define QUA_ETXTBSY     (QUA_ERR_BASE-26)  /* Text file busy */
#define QUA_EFBIG       (QUA_ERR_BASE-27)  /* File too large */
#define QUA_ENOSPC      (QUA_ERR_BASE-28)  /* No space left on device */
#define QUA_ESPIPE      (QUA_ERR_BASE-29)  /* Illegal seek */
#define QUA_EROFS       (QUA_ERR_BASE-30)  /* Read-only file system */
#define QUA_EMLINK      (QUA_ERR_BASE-31)  /* Too many links */
#define QUA_EPIPE       (QUA_ERR_BASE-32)  /* Broken pipe */
#define QUA_EDOM        (QUA_ERR_BASE-33)  /* Math argument out of domain of func */
#define QUA_ERANGE      (QUA_ERR_BASE-34)  /* Math result not representable */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __QUA_TYPE_H__ */

