#ifndef __VIDEO_BUF_H__
#define __VIDEO_BUF_H__

#include "fh_comm.h"
#include "vb_drv_ioc.h"

typedef  unsigned int VB_BLKHANDLE;

typedef struct {
    MPP_CHN_S    stMod;
    int      (*pCallBack)(MPP_CHN_S stMod, FH_PHYADDR u64PhyAddr);
} VB_CREATE_EX_INFO_S;


typedef struct {
    int          (*pfnVB_AddBlkToPool)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Size, unsigned int u32Uid);
    int          (*pfnVB_CreatePool)(unsigned int *pu32PoolId, unsigned int u32BlkCnt, unsigned int u32BlkSize,
                                                        char *pcMmzName,  char *pool_type, unsigned int remap_mode);
    int          (*pfnVB_CreatePoolExt)(unsigned int *pu32PoolId, unsigned int u32BlkCnt, int bUser, int owner, VB_CREATE_EX_INFO_S * pstVbRegInfo);
    int          (*pfnVB_DestroyPool)(unsigned int u32PoolId);
    VB_BLKHANDLE (*pfnVB_GetBlkByPoolId)(unsigned int u32PoolId, unsigned int u32Uid);
    VB_BLKHANDLE (*pfnVB_GetBlkBySize)(unsigned int u32BlkSize, unsigned int u32Uid, char *pcMmzName);
    VB_BLKHANDLE (*pfnVB_GetBlkBySizeAndModule)(unsigned int u32BlkSize, unsigned int u32Uid, void *mmz_name);
    VB_BLKHANDLE (*pfnVB_GetBlkBySizeAndPoolId)(unsigned int u32PoolId, unsigned int u32BlkSize, unsigned int u32Uid);
    int          (*pfnVB_GetPoolInfo)(VB_POOL_INFO_S *pstInfo);
    unsigned int (*pfnVB_Handle2BlkId)(VB_BLKHANDLE Handle);
    unsigned int (*pfnVB_Handle2BlkSize)(VB_BLKHANDLE Handle);
    void*        (*pfnVB_Handle2Kern)(VB_BLKHANDLE Handle);
    FH_PHYADDR   (*pfnVB_Handle2Phys)(VB_BLKHANDLE Handle);
    unsigned int (*pfnVB_Handle2PoolId)(VB_BLKHANDLE Handle);
    unsigned int (*pfnVB_Handle2Supplement)(VB_BLKHANDLE Handle);
    unsigned int (*pfnVB_InquireBlkCnt)(unsigned int u32Uid, FH_BOOL bIsCommPool);
    unsigned int (*pfnVB_InquireOneUserCnt)(VB_BLKHANDLE Handle, unsigned int u32Uid);
    int          (*pfnVB_InquirePool)(unsigned int u32PoolId, VB_POOL_STATUS_S *pstPoolStatus);
    int          (*pfnVB_InquirePoolBusyBlkCnt)(unsigned int u32PoolId, unsigned int *pu32Cnt);
    int          (*pfnVB_InquirePoolUserCnt)(unsigned int u32PoolId, unsigned int u32Uid, unsigned int *pu32Cnt);
    unsigned int (*pfnVB_InquireUserCnt)(VB_BLKHANDLE Handle);
    int          (*pfnVB_InquireSumCnt)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr);
    FH_BOOL      (*pfnVB_IsBlkValid)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr);
    int          (*pfnVB_IsSupplementSupport)(int a1);
    unsigned int (*pfnVB_Phy2Handle)(FH_PHYADDR u64PhyAddr);
    int          (*pfnVB_PutBlk)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr);
    int          (*pfnVB_UserAdd)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Uid);
    int          (*pfnVB_UserSub)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Uid);
    unsigned int (*pfnVB_Phy2Handle_Virt)(FH_PHYADDR u64PhyAddr);
    int          (*pfnVB_CreatePoolExt2)(unsigned int *pu32PoolId, unsigned int u32BlkCnt, int bUser, int owner);
    int          (*pfnVB_AddBlkToPoolExt2)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, FH_VOID *VirtAddr, unsigned int u32Size, unsigned int u32Uid);
    int          (*pfnVB_RemoveBlkFromPoolExt2)(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Uid);
}VB_EXPORT_FUNC_S;


/*****************************************************************************
 Prototype    : VB_CreatePool
 Description  : create a private VB pool
 Input        : u32BlkCnt      : count of buffer blocks
                u32BlkSize     : size of buffer blocks
                pcMmzName      : mmz name
 Output       : pu32PoolId     : pointer to the ID of pool created
 Return Value : int         : 0 -- success; !0 : failure
*****************************************************************************/
int VB_CreatePool(unsigned int *pu32PoolId, unsigned int u32BlkCnt, unsigned int u32BlkSize,
                     char *pcMmzName,  char *pool_type, unsigned int remap_mode);

/*****************************************************************************
 Prototype    : VB_CreatePoolExt
 Description  : create a private VB pool,but it's not malloc buffer
 Input        : stVbRegInfo      : register info
                u32BlkCnt      : count of buffer blocks
 Output       : pu32PoolId     : pointer to the ID of pool created
 Return Value : int         : 0 -- success; !0 : failure
*****************************************************************************/
int VB_CreatePoolExt(unsigned int *pu32PoolId, unsigned int u32BlkCnt, int bUser, int owner, VB_CREATE_EX_INFO_S * pstVbRegInfo);
int VB_CreatePoolExt2(unsigned int *pu32PoolId, unsigned int u32BlkCnt, int bUser, int owner);

/*****************************************************************************
 Prototype    : VB_AddBlkToPool
 Description  : add a buffer block to a specified pool
 Input        : u32PoolId      : ID of a pool
                u64PhyAddr    : physcial address of the buffer
                u32Uid         : ID of user getting the buffer
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
int  VB_AddBlkToPool(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Size, unsigned int u32Uid);
int  VB_AddBlkToPoolExt2(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, FH_VOID *VirtAddr, unsigned int u32Size, unsigned int u32Uid);
int  VB_RemoveBlkFromPoolExt2(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Uid);



/*****************************************************************************
 Prototype    : VB_GetPoolInfo
 Description  : Get pool info
 Input        : pstInfo(u32PoolId)   : ID of a pool
 Output       : pstInfo
 Return Value :
                success : 0
                failure : Not 0
*****************************************************************************/
int  VB_GetPoolInfo(VB_POOL_INFO_S *pstInfo);



/*****************************************************************************
 Prototype    : VB_DestroyPool
 Description  : Destroy a VB pool
 Input        : u32PoolId      : ID of a pool
 Output       : NONE
 Return Value : int         : 0 -- success; !0 : failure
 *****************************************************************************/
int  VB_DestroyPool(unsigned int u32PoolId);


/*****************************************************************************
 Prototype    : VB_GetPoolId
 Description  : Get the first one in common pools in which block size is greater
                than  'u32BlkSize'
 Input        :
 Output       :
 Return Value : int
                success : 0
                failure : Not 0
*****************************************************************************/
int  VB_GetPoolId(unsigned int u32BlkSize, unsigned int *pu32PoolId, char *pcMmzName);

/*****************************************************************************
 Prototype    : VB_GetBlkByPoolId
 Description  : Get a buffer block from a specified pool
 Input        : u32PoolId      : ID of a pool
                u32Uid         : ID of user getting the buffer
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
VB_BLKHANDLE  VB_GetBlkByPoolId(unsigned int u32PoolId, unsigned int u32Uid);


/*****************************************************************************
 Prototype    : VB_GetBlkBySize
 Description  : Get a block in the first fit common pool
 Input        : u32BlkSize : size of buffer block
                u32Uid     : ID of user who will use the buffer
 Output       : NONE
 Return Value : VB_BLKHANDLE
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
VB_BLKHANDLE  VB_GetBlkBySize(unsigned int u32BlkSize, unsigned int u32Uid, char *pcMmzName);

FH_UINT32 VB_Handle2Supplement(VB_BLKHANDLE Handle);
int  VB_InquirePoolBusyBlkCnt(unsigned int u32PoolId, unsigned int *pu32Cnt);
/*****************************************************************************
 Prototype    : VB_GetBlkByPoolId
 Description  : Get a buffer block from a specified pool and specified block
 Input        : u32PoolId      : ID of a pool
                u32Uid         : ID of user getting the buffer
                u32BlkSize     : size of buffer block
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/

VB_BLKHANDLE VB_GetBlkBySizeAndPoolId(unsigned int u32PoolId, unsigned int u32BlkSize, unsigned int u32Uid);



/*****************************************************************************
 Prototype    : VB_GetBlkByPoolId
 Description  : Get a buffer block from a specified pool in which block size is "u32BlkSize"
 Input        : u32PoolId      : ID of a pool
                u32Uid         : ID of user getting the buffer
                pcMmzName      : mmz name
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/

VB_BLKHANDLE VB_GetBlkBySizeAndModule(unsigned int u32BlkSize, unsigned int u32Uid, void *pcMmzName);

/*****************************************************************************
 Prototype    : VB_Phy2Handle
 Description  : convert physical address to handle in physical and virtual pool
 Input        :
 Output       :
 Return Value : unsigned int
******************************************************************************/
VB_BLKHANDLE  VB_Phy2Handle(FH_PHYADDR u64PhyAddr);

/*****************************************************************************
 Prototype    : VB_Phy2Handle_Virt
 Description  : convert physical address to handle in virtual pool
 Input        :
 Output       :
 Return Value : unsigned int
******************************************************************************/
VB_BLKHANDLE  VB_Phy2Handle_Virt(FH_PHYADDR u64PhyAddr);

/*****************************************************************************
 Prototype    : VB_Handle2Phys
 Description  : convert handle to physical address of a buffer
 Input        :
 Output       :
 Return Value : unsigned long long
******************************************************************************/
FH_PHYADDR VB_Handle2Phys(VB_BLKHANDLE Handle);


/*****************************************************************************
 Prototype    : VB_Handle2Kern
 Description  : convert handle to kernel virtual address of a buffer
 Input        : Handle     : hanle of a buffer
 Output       :
 Return Value : void*
*****************************************************************************/
FH_VOID *VB_Handle2Kern(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2UsrVir
 Description  : convert handle to user virtual address of a buffer
 Input        : Handle     : hanle of a buffer
 Output       :
 Return Value : void*
*****************************************************************************/
FH_VOID *VB_Handle2UsrVir(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2PoolId
 Description  : convert Handle to pool ID
 Input        : VB_BLKHANDLE   : handle of the buffer
 Output       :
 Return Value : unsigned int
*****************************************************************************/
unsigned int VB_Handle2PoolId(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2BlkId
 Description  : convert Handle to pool ID
 Input        : VB_BLKHANDLE   : handle of the buffer
 Output       :
 Return Value : unsigned int
*****************************************************************************/
unsigned int VB_Handle2BlkId(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2BlkId
 Description  : convert Handle to block size
 Input        : VB_BLKHANDLE   : handle of the buffer
 Output       :
 Return Value : unsigned int
*****************************************************************************/
unsigned int VB_Handle2BlkSize(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_InquireUserCnt
 Description  : Inquire how many users are using the buffer
 Input        : Handle     : hanle of a buffer
 Output       :
 Return Value : unsigned int
******************************************************************************/
unsigned int VB_InquireUserCnt(VB_BLKHANDLE Handle);


/*****************************************************************************
 Prototype    : VB_InquireUserCnt
 Description  : Inquire how many times the user is using the specified handle
 Input        : Handle     : hanle of a buffer
 Output       :
 Return Value : unsigned int
******************************************************************************/
unsigned int   VB_InquireOneUserCnt(VB_BLKHANDLE Handle, unsigned int u32Uid);

int VB_InquireSumCnt(unsigned int u32PoolId, FH_PHYADDR u32PhyAddr);

/*****************************************************************************
 Prototype    : VB_InquireBlkCnt
 Description  : Inquire how many blocks the user is using
 Input        : u32Uid
                bIsCommPool
 Output       :
 Return Value : unsigned int
*****************************************************************************/
unsigned int   VB_InquireBlkCnt(unsigned int u32Uid, FH_BOOL bIsCommPool);

/*****************************************************************************
 Prototype    : VB_InquirePool
 Description  : Inquire the statistic of a pool
 Input        : u32PoolId     : ID of pool the buffer is in
 Output       : pstPoolStatus : pool status
 Return Value : int
******************************************************************************/
int  VB_InquirePool(unsigned int u32PoolId, VB_POOL_STATUS_S *pstPoolStatus);

/*****************************************************************************
 Prototype    : VB_InquirePoolUserCnt
 Description  : Inquire how many blocks the user used in this pool
 Input        : u32PoolId     : ID of pool the buffer is in
                u32Uid        : ID of the user
 Output       : pu32Cnt       : Count of vb used by user
 Return Value : unsigned int
******************************************************************************/
int   VB_InquirePoolUserCnt(unsigned int u32PoolId, unsigned int u32Uid, unsigned int *pu32Cnt);

/*****************************************************************************
 Prototype    : VB_IsBlkValid
 Description  : Check if the address is valid
 Input        : u32PoolId
                u64PhyAddr
 Output       :
 Return Value : FH_BOOL
*****************************************************************************/
FH_BOOL   VB_IsBlkValid(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr);

/*****************************************************************************
 Prototype    : VB_Init
 Description  : Initialize video buffer modules
 Input        :
 Output       :
 Return Value : int
                success    : 0
                failure    : Not 0
*****************************************************************************/
int VB_Init(void);

/*****************************************************************************
 Prototype    : VB_Exit
 Description  : cleanup video buffer module
 Input        :
 Output       :
 Return Value :int
               success    : 0
               failure    : Not 0 , we never reture failure in fact.
*****************************************************************************/
int  VB_Exit(void);

/*****************************************************************************
 Prototype    : VB_UserAdd
 Description  : increase one to reference counter of  a buffer
 Input        : u32PoolId      : ID of pool the buffer is in
                u64PhyAddr    : physcial address of the buffer
                u32Uid         : ID of the user
 Output       :
 Return Value : int
                success    : 0
                failure    : Not 0
*****************************************************************************/
extern int VB_UserAdd(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Uid);


/*****************************************************************************
 Prototype    : VB_UserSub
 Description  : decrease one to reference counter of a buffer
 Input        : u32PoolId      : ID of pool the buffer is in
                u64PhyAddr    : physcial address of the buffer
                u32Uid         : ID of the user
 Output       :
 Return Value : int
*****************************************************************************/
extern int VB_UserSub(unsigned int u32PoolId, FH_PHYADDR u64PhyAddr, unsigned int u32Uid);



extern int VB_InitModVb(VB_CONF_S *pstVbModConf,VB_UID_E enVbUid);
extern int VB_ExitModVb(VB_UID_E enVbUid);

#endif //__VIDEO_BUF_H__
