/******************************************************************************

 ******************************************************************************/
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <dirent.h>
#endif

#if defined(__KERNEL__)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#endif

#include "fy_osal.h"

#ifndef printf
#define printf	fhosal_printk
#endif

#ifndef fprintf
#define fprintf
#endif

#ifdef MM_DEBUG
#define LCM_LOG_ENABLE
#endif

#ifndef DATA_TYPE_H
#define DATA_TYPE_H


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* ------------------------------------------------------------------------
** Constants
** ------------------------------------------------------------------------ */

/* -----------------------------------------------------------------------
** Standard Types
** ----------------------------------------------------------------------- */
typedef unsigned char BOOLEAN;
#if 0
typedef signed char  int8_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

#if defined(WIN32)
    typedef signed long int int32_t;
    typedef unsigned long int uint32_t;
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#elif defined(_WIN64)
    typedef signed long int int32_t;
    typedef unsigned long int uint32_t;
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#else
    typedef signed int int32_t;
    typedef unsigned int uint32_t;
    typedef long long int64_t;
    typedef unsigned long long uint64_t;
#endif
#endif

#if defined(WIN32)
    typedef unsigned long int ADDR;
    typedef unsigned long int _ADDR;
    typedef unsigned long int DATA;
    typedef unsigned long int _DATA;
#elif defined(_WIN64)
    typedef unsigned long long ADDR;
    typedef unsigned long long _ADDR;
    typedef unsigned long long DATA;
    typedef unsigned long long _DATA;
#else
    typedef unsigned int ADDR;
    typedef unsigned int _ADDR;
    typedef unsigned int DATA;
    typedef unsigned int _DATA;
#endif

#if defined(WIN32)
    #define STRICMP(a, b)     stricmp(a, b)
    #define STRNICMP(a, b, c) strnicmp(a, b, c)
    #define MKDIR(path, mode) mkdir(path)
#elif defined(_WIN64)
    #define STRICMP(a, b)     stricmp(a, b)
    #define STRNICMP(a, b, c) strnicmp(a, b, c)
    #define MKDIR(path, mode) mkdir(path)
#else
    #define STRICMP(a, b)     strcasecmp(a, b)
    #define STRNICMP(a, b, c) strncasecmp(a, b, c)
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

#ifndef SET_ONE
    #define SET_ONE 1
#endif

#ifndef CLEAN_ZERO
    #define CLEAN_ZERO 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef PNULL
    #define PNULL 0
#endif

#ifndef LOCAL
#define LOCAL static
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef VOLATILE
#define VOLATILE volatile
#endif

#ifndef PUBLIC
#define PUBLIC public
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x) {if(PNULL != x) {free(x); x = PNULL;}}
#endif

#ifndef BIT
#define BIT(bit) (1 << bit)
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef CLIP
#define CLIP(_x, _min, _max) (((_x) < (_min)) ? (_min) : (((_x) > (_max)) ? (_max) : (_x)))
#endif

#ifndef CLIP_MIN
#define CLIP_MIN(_x, _min) (((_x) < (_min)) ? (_min) : (_x))
#endif

#ifndef CLIP_MAX
#define CLIP_MAX(_x, _max) (((_x) > (_max)) ? (_x) : (_max))
#endif

#ifndef ARR_SIZE
#define ARR_SIZE(_a) (sizeof((_a)) / sizeof((_a[0])))
#endif

#ifdef LCM_LOG_ENABLE

#ifndef RTN_IF_FAIL
//#define RTN_IF_FAIL(exp, fp, ...) do { if(exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); if(fp) { fprintf(fp, "file:%s, line:%d, ", __FILE__, __LINE__); fprintf(fp, __VA_ARGS__); } return exp; } } while(0)
#define RTN_IF_FAIL(exp, fp, ...) do { \
    if(exp) { \
        printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); \
        printf(__VA_ARGS__); return exp; \
    }\
} while(0)
#endif

#ifndef EXIT_IF_SUCCESS
//#define EXIT_IF_FAIL(exp, fp, ...) do { if(exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); if(fp) { fprintf(fp, "file:%s, line:%d, ", __FILE__, __LINE__); fprintf(fp, __VA_ARGS__); } goto EXIT; } } while(0)
#define EXIT_IF_SUCCESS(exp, fp, ...) do { if(SUCCESS == exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); goto EXIT; } } while(0)
#endif


#ifndef EXIT_IF_FAIL
//#define EXIT_IF_FAIL(exp, fp, ...) do { if(exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); if(fp) { fprintf(fp, "file:%s, line:%d, ", __FILE__, __LINE__); fprintf(fp, __VA_ARGS__); } goto EXIT; } } while(0)
#define EXIT_IF_FAIL(exp, fp, ...) do { if(exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); goto EXIT; } } while(0)
#endif

#ifndef EXIT_IF_NULL
#define EXIT_IF_NULL(exp, file, warning) do {if(NULL == exp) {printf(warning); rtn = FAIL_PARAM_NULL; goto EXIT;}}while(0)
#endif

#ifndef EXIT_IF_ZERO
#define EXIT_IF_ZERO(exp, file, warning) do {if(0 == exp) {printf(warning); rtn = FAIL_PARAM_ZERO; goto EXIT;}}while(0)
#endif

#ifndef CHECK_PTR
//#define CHECK_PTR(ptr, fp, ...) do { if(NULL == ptr) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); if(fp) { fprintf(fp, "file:%s, line:%d, ", __FILE__, __LINE__); fprintf(fp, __VA_ARGS__); } rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#define CHECK_PTR(ptr, fp, ...) do { if(NULL == ptr) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#endif

#ifndef CHECK_FUN
//#define CHECK_PTR(ptr, fp, ...) do { if(NULL == ptr) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); if(fp) { fprintf(fp, "file:%s, line:%d, ", __FILE__, __LINE__); fprintf(fp, __VA_ARGS__); } rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#define CHECK_FUN(ptr, fp, ...) do { if(NULL == ptr) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); rtn = FAIL_FUN_PNULL; goto EXIT; } } while(0)
#endif

#ifndef CHECK_ENUM
//#define CHECK_ENUM(a, b, fp, ...) do { if(a => b) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); if(fp) { fprintf(fp, "file:%s, line:%d, ", __FILE__, __LINE__); fprintf(fp, __VA_ARGS__); } rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#define CHECK_ENUM(a, b, fp, ...) do { if(a >= b) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#endif


#ifndef RETURN_IF_FAIL
#define RETURN_IF_FAIL(exp, fp, ...) do {if(exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); return exp;} }while(0)
#endif

#ifndef WARNING_IF_FAIL
#define WARNING_IF_FAIL(exp, fp, ...) do {if(exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);} }while(0)
#endif

#ifndef WARNING_IF_SUCCESS
#define WARNING_IF_SUCCESS(exp, fp, ...) do {if(SUCCESS == exp) { printf("Function:%s, Line:%d, ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);} }while(0)
#endif

#else

#define RTN_IF_FAIL(exp, fp, ...)           do { if(exp) { return exp;}} while(0)
#define EXIT_IF_SUCCESS(exp, fp, ...)       do { if(SUCCESS == exp) {goto EXIT; } } while(0)
#define EXIT_IF_FAIL(exp, fp, ...)          do { if(exp) { goto EXIT; } } while(0)
#define EXIT_IF_NULL(exp, file, warning)    do { if(NULL == exp) { rtn = FAIL_PARAM_NULL; goto EXIT;}}while(0)
#define EXIT_IF_ZERO(exp, file, warning)    do { if(0 == exp) { rtn = FAIL_PARAM_ZERO; goto EXIT;}}while(0)
#define CHECK_PTR(ptr, fp, ...)             do { if(NULL == ptr) { rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#define CHECK_FUN(ptr, fp, ...)             do { if(NULL == ptr) { rtn = FAIL_FUN_PNULL; goto EXIT; } } while(0)
#define CHECK_ENUM(a, b, fp, ...)           do { if(a >= b) { rtn = FAIL_PARAM_NULL; goto EXIT; } } while(0)
#define RETURN_IF_FAIL(exp, fp, ...)        do { if(exp) { return exp;} }while(0)
#define WARNING_IF_FAIL(exp, fp, ...)       do { if(exp) { } }while(0)
#define WARNING_IF_SUCCESS(exp, fp, ...)    do { if(SUCCESS == exp) { } }while(0)

#endif

#define AE_GROUP_NUM 8

typedef enum {
    SUCCESS = 0,
    FAIL,
    FAIL_PARAM_NULL,
    FAIL_FUN_PNULL,
    FAIL_NO_MEMORY,
    FAIL_INVALID,
    FAIL_STR_NO_MATCH,
    FAIL_PARAM_NONE,
    FAIL_PARAM_ZERO,
    FAIL_NO_EQUAL,
    FAIL_UN_INIT,
    FAIL_RE_INIT,
    FAIL_RE_SET,
    FAIL_STATUS_NO_MATCH,
    FAIL_PROBE,
    FAIL_ID_NO_MATCH,
    FAIL_MAX
}result_e;

#define PIPE_SIZE_NUM 2
typedef enum {
    FRAME = 0,
    BINNING,
    PIPE_SIZE_MAX = PIPE_SIZE_NUM
}pipeSize_e;

typedef struct {
    uint32_t id;
    char name[64];
}note_t;

typedef struct {
    void *ptr;
    uint32_t len;
}buf_t;

typedef struct {
    uint32_t width;
    uint32_t height;
}frameSize_t;

typedef struct {
    uint32_t x;
    uint32_t y;
}framePos_t;

typedef struct {
    uint32_t startX;
    uint32_t startY;
    uint32_t endX;
    uint32_t endY;
}frameRoi_t;

typedef struct {
    uint32_t eb;
    uint32_t posX;
    uint32_t posY;
}debug_t;

typedef struct {
    uint32_t mode;
    char name[64];
}description_t;


#define MODULE_INVALID     0xffffffff
#define MODULE_INVALID_STR "0xffffffff"

#define PIPE_ID_MAX 2
#define MODULE_ID_MAX 4

typedef enum {
    PIPE_SIGNAL = 0,
    PIPE_DOUBLE,
    PIPE_HDR,
    PIPE_MODE_MAX
}pipeMode_e;

/*Pipeline0 Pipeline1*/
typedef enum {
    PIPE_ID_0,
    PIPE_ID_1,
    PIPE_ID_MAX_ = PIPE_ID_MAX,
}pipe_id_e;

/* sensor module main/front*/
typedef enum {
    MODULE_ID_0,
    MODULE_ID_1,
    MODULE_ID_2,
    MODULE_ID_3,
    MODULE_ID_MAX_ = MODULE_ID_MAX,
} module_id_e;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif


