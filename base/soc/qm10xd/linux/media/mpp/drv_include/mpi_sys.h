#ifndef __MPI_SYS_H__
#define __MPI_SYS_H__

#include "fh_type.h"
#include "fh_common.h"
#include "fy_comm_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

FY_S32 FY_MPI_SYS_Init();
FY_S32 FY_MPI_SYS_Exit();

FY_S32 FY_MPI_SYS_SetConf(const MPP_SYS_CONF_S *pstSysConf);
FY_S32 FY_MPI_SYS_GetConf(MPP_SYS_CONF_S *pstSysConf);

FY_S32  FY_MPI_SYS_Bind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);
FY_S32  FY_MPI_SYS_UnBind(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);
FY_S32  FY_MPI_SYS_GetBindbyDest(MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn);

FY_S32 FY_MPI_SYS_GetVersion(MPP_VERSION_S *pstVersion);

/*
** u64Base is the global PTS of the system.
** ADVICE:
** 1. Better to call FY_MPI_SYS_GetCurPts on the host board to get u64Base.
** 2. When os start up, call FY_MPI_SYS_InitPtsBase to set the init PTS.
** 3. When media bussines is running, synchronize the PTS one time per minute 
**     by calling FY_MPI_SYS_SyncPts.
*/
FY_S32 FY_MPI_SYS_GetCurPts(FY_U64 *pu64CurPts);
FY_S32 FY_MPI_SYS_InitPtsBase(FY_U64 u64PtsBase);
FY_S32 FY_MPI_SYS_SyncPts(FY_U64 u64PtsBase);

/* alloc mmz memory in user context                                         */
FY_S32 FY_MPI_SYS_MmzAlloc(unsigned long *pu32PhyAddr, FY_VOID **ppVirtAddr, 
        const FY_CHAR *strMmb, const FY_CHAR *strZone, FY_U32 u32Len);

/* alloc mmz memory with cache */
FY_S32 FY_MPI_SYS_MmzAlloc_Cached(unsigned long *pu32PhyAddr, FY_VOID **ppVitAddr, 
        const FY_CHAR *pstrMmb, const FY_CHAR *pstrZone, FY_U32 u32Len);

/* free mmz memory in user context                                          */
FY_S32 FY_MPI_SYS_MmzFree(unsigned long u32PhyAddr, FY_VOID *pVirtAddr);

/* fulsh cache */
FY_S32 FY_MPI_SYS_MmzFlushCache(unsigned long u32PhyAddr, FY_VOID *pVitAddr, FY_U32 u32Size);

/*
** Call the mmap function to map physical address to virtual address
** The system function mmap is too complicated, so we packge it.
*/
FY_VOID * FY_MPI_SYS_Mmap(unsigned long u32PhyAddr, FY_U32 u32Size);
FY_VOID* FY_MPI_SYS_MmapCache(unsigned long u32PhyAddr, FY_U32 u32Size);
FY_S32 FY_MPI_SYS_Munmap(FY_VOID* pVirAddr, FY_U32 u32Size);
FY_S32 FY_MPI_SYS_MflushCache(unsigned long u32PhyAddr, FY_VOID *pVirAddr, FY_U32 u32Size);


/*
** Access the physical address.
** You can use this function to access memory address or register address.
*/
FY_S32 FY_MPI_SYS_SetReg(FY_U32 u32Addr, FY_U32 u32Value);
FY_S32 FY_MPI_SYS_GetReg(FY_U32 u32Addr, FY_U32 *pu32Value);

FY_S32 FY_MPI_SYS_SetMemConf(MPP_CHN_S *pstMppChn,const FY_CHAR *pcMmzName);
FY_S32 FY_MPI_SYS_GetMemConf(MPP_CHN_S *pstMppChn,FY_CHAR *pcMmzName);

/* Close all the FD which is used by sys module */
FY_S32 FY_MPI_SYS_CloseFd(FY_VOID);

int FY_MPI_Mmz_Vir2Phy(void *virtAddr, unsigned long *phyAddr);
int FY_MPI_Mmz_QueryCache(void *virtAddr, unsigned char *cached);
FY_S32 FY_MPI_Mmz_CleanCache(unsigned long u32PhyAddr, FY_VOID *pVirAddr, FY_U32 u32Size);
FY_S32 FY_MPI_Mmz_InvalidCache(unsigned long u32PhyAddr, FY_VOID *pVirAddr, FY_U32 u32Size);
FY_VOID *FY_MPI_Mmz_Mmap(unsigned long u32PhyAddr, FY_U32 u32Size, FY_U32 cache);
FY_S32 FY_MPI_Mmz_Munmap(FY_VOID* pVirAddr, FY_U32 u32Size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*__MPI_SYS_H__ */

