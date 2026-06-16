#ifndef __FY_COMM_H__
#define __FY_COMM_H__

#include "fh_defines.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef VER_X
    #define VER_X 1
#endif

#ifndef VER_Y
    #define VER_Y 0
#endif

#ifndef VER_Z
    #define VER_Z 0
#endif

#ifndef VER_P
    #define VER_P 0
#endif

#ifndef VER_B
    #define VER_B 0
#endif

#define __DEFINE_TO_STR(s)   #s
#define DEFINE_TO_STR(s)     __DEFINE_TO_STR(s)

#define __MK_VERSION(x,y,z) #x"."#y"."#z
#define MK_VERSION(x,y,z) __MK_VERSION(x,y,z)

#define BUILD_VERSION DEFINE_TO_STR(VER_B)
#define BUILDBY_INFO DEFINE_TO_STR(VER_P)

#define MPP_VERSION  "[MPI] Version:[v" MK_VERSION(VER_X,VER_Y,VER_Z) " (" BUILD_VERSION ")] Build BY: "  BUILDBY_INFO " Time:[" __DATE__ "-" __TIME__ "]"

#define FYMODULE_VERSION(name, version) \
			"[" name "] Version:[v" version " (" BUILD_VERSION ")] Build BY: "  BUILDBY_INFO " Time:[" __DATE__ "-" __TIME__ "]"


#define VERSION_NAME_MAXLEN 64
typedef struct fyMPP_VERSION_S
{
	FH_CHAR aVersion[VERSION_NAME_MAXLEN];
}MPP_VERSION_S;


typedef struct
{
    FH_UINT32 base;
    void * vbase;
    FH_UINT32 size;
}MEM_INFO_S;


typedef struct fyMPP_CHN_S
{
    FH_BIND_OBJ_ID enModId;
    FH_SINT32      s32DevId;
    FH_SINT32      s32ChnId;
} MPP_CHN_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  /* _FY_COMM_H_ */

