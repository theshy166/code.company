#ifndef __CMPI_BASE_API__H__
#define __CMPI_BASE_API__H__
#include "fymedia.h"

#if 1

//proc functions
#define CMPI_PROC_SHOW CMPI_PROC_READ
#ifdef __KERNEL__
typedef FY_S32 (*CMPI_PROC_READ)(struct seq_file *, FY_VOID *);
typedef FY_S32 (*CMPI_PROC_WRITE)(struct file * file,
                                  const char __user * buf, size_t count, loff_t *ppos);
#else
typedef FY_S32 (*CMPI_PROC_READ)(FY_VOID *, FY_VOID *);
typedef FY_S32 (*CMPI_PROC_WRITE)(struct rt_device * file,
                                  const char * buf, size_t count, FY_VOID *ppos);
#endif

typedef struct CMPI_PROC_ITEM {
    FY_CHAR                 name[32];
    struct proc_dir_entry   *pdir_entry;

    CMPI_PROC_READ          read;
    CMPI_PROC_WRITE         write;
    FY_BOOL                 bDefault;
    FY_VOID                 *pData;
} CMPI_PROC_ITEM_S;


CMPI_PROC_ITEM_S * CMPI_CreateProc(FY_CHAR *name, CMPI_PROC_SHOW pfnRead , int data);
int  CMPI_RemoveProc(const char* proc_name);

//device functions
int CMPI_RegisterDevice(struct fymedia_device *dev);
int CMPI_UnRegisterDevice(struct fymedia_device *dev);
#ifdef __KERNEL__
int CMPI_UserCopy(struct file *file, unsigned int cmd, unsigned long arg, int (*callback)(int, unsigned int, unsigned long));
#else
int CMPI_UserCopy(struct rt_device *file, unsigned int cmd, unsigned long arg, int (*callback)(int, unsigned int, unsigned long));
#endif

// module functions
FY_CHAR *CMPI_GetModuleName(MOD_ID_E enModId);
UMAP_MODULE_S *CMPI_GetModuleById(MOD_ID_E enModId);
FY_VOID *CMPI_GetModuleFuncById(MOD_ID_E enModId);
FY_S32  CMPI_RegisterModule(UMAP_MODULE_S *pstModules);
FY_VOID CMPI_UnRegisterModule(MOD_ID_E enModId);
int CMPI_RegisterLogModule(MOD_ID_E enModId, char* sModName, FY_U32 level);
FY_VOID CMPI_UnRegisterLogModule(MOD_ID_E enModId);
#endif

// mmz functions
int  CMPI_MmzMallocNocache(FY_CHAR * mmz_name, FY_CHAR *buf_name,  FH_PHYADDR *phy_addr, FH_VOID **virt_addr, FY_U32 buf_size);
int  CMPI_MmzMallocCached(FY_CHAR * mmz_name, FY_CHAR *buf_name,  FH_PHYADDR *phy_addr, FH_VOID **virt_addr, FY_U32 buf_size);
FH_PHYADDR CMPI_MmzMalloc(FY_CHAR *pMmzName,  FY_CHAR *pBufName, FY_U32 size);
int  CMPI_MmzFree(FH_PHYADDR phy_addr, FH_VOID *virt_addr);



#endif //__CMPI_BASE_API__H__
