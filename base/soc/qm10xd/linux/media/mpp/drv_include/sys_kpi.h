#ifndef __SYS_KPI_H__
#define __SYS_KPI_H__


#include "fh_type.h"
#include "fh_comm.h"
#include "fh_video.h"
//#include "fy_comm_aio.h"
#include "fh_common.h"


#include "sys_bind.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define SYS_DDR_MAXNUM 16
typedef struct hiMPP_SYS_DDR_NAME_S
{
    FY_U32    u32Num;
    FY_VOID *apMmzName[SYS_DDR_MAXNUM];
}MPP_SYS_DDR_NAME_S;

typedef struct _SYS_EXPORT_FUNC_S
{
    FY_U64  (*pfnSYSGetTimeStamp)  (FY_VOID);
    FY_VOID (*pfnSYSSyncTimeStamp) (FY_U64 u64Base, FY_BOOL bInit);
    FY_U32  (*pfnSysGetChipVersion)(FY_VOID);

    FY_S32  (*pfnSysRegisterSender)     (MPP_BIND_SENDER *pstInfo);
    FY_S32  (*pfnSysUnRegisterSender)   (FH_BIND_OBJ_ID enModId);
    FY_S32  (*pfnSysRegisterReceiver)   (MPP_BIND_RECEIVER *pstInfo);
    FY_S32  (*pfnSysUnRegisterReceiver) (FH_BIND_OBJ_ID enModId);
    FY_S32  (*pfnSysSendData)      (FH_BIND_OBJ_ID enModId, FY_S32 s32DevId, FY_S32 s32ChnId, FY_BOOL bBlock,
        								MPP_DATA_TYPE_E enDataType, FY_VOID *pvData);


    FY_S32 (*pfnGetBindbySrc)(MPP_CHN_S *pstSrcChn,MPP_BIND_DEST_S *pstBindSrc);
    FY_S32 (*pfnGetBindbyDest)(MPP_CHN_S *pstDestChn,MPP_CHN_S *pstSrcChn);

    FY_S32 (*pfnGetMmzName)(MPP_CHN_S *pstChn,char *pMmzName);
    FY_S32 (*pfnGetChanMMzName)(MPP_CHN_S *pstMppChn, const char** pmmz_name);
    FY_S32 (*pfnGetMemIndex)(FY_VOID);//(FY_U32 u32PhyAddr,FY_U32 *pu32MemIndex);
    FY_S32 (*pfnGetMemDdr)(MPP_CHN_S *pstChn,MPP_SYS_DDR_NAME_S *pstMemDdr);

	FY_U64  (*pfnSYSGetSchedClock)  (FY_VOID);
	FY_S32  (*pfnSYSGetFullChipId)  (FY_U32 *pu32ChipId);
} SYS_EXPORT_FUNC_S;


unsigned long long SYS_GetTimeStamp(void);
int Sys_GetMemConf(MPP_CHN_S *pstMppChn, int* ddr_idx);
int SYS_GetMmzName(MPP_CHN_S *pstMppChn, char* mmz_name);
int Sys_GetChanMMzName(MPP_CHN_S *pstMppChn, const char** pmmz_name);

//SYS common lock is not for sys itself, it's provided for use across modules.
// 1st lock(lock==0) is used by vdu/jpeg/bgm reset
void SYS_Lock(int lock);
void SYS_UnLock(int lock);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__SYS_KPI_H__

