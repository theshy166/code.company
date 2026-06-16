#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/list.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "cconfig.h"
#include "rtdef.h"
#include "rthw.h"
#include "rtthread.h"
#include "rtdbg.h"
//#include "ioremap.h"
//#include "dfs_file.h"
//#include "boot_args.h"
#endif
#include "fh_type.h"
#include "fh_errno.h"
#include "fh_comm.h"


#ifndef __MOD_EXT_H__
#define __MOD_EXT_H__

#define FYL_MMZ_NAME_LEN 32
#define FYL_MMB_NAME_LEN 16

#define MAX_MPP_MODULES FH_ID_LOG_BUTT

#define VERSION_MAGIC	20150804

#define MAX_MOD_NAME    16

typedef enum fyMOD_NOTICE_ID_E
{
    MOD_NOTICE_STOP = 0x11,
}MOD_NOTICE_ID_E;

typedef enum fyMOD_STATE_E
{
    MOD_STATE_FREE = 0x11,
    MOD_STATE_BUSY = 0X22,
}MOD_STATE_E;

typedef FH_SINT32 FN_MOD_Init(FH_VOID *);
typedef FH_VOID FN_MOD_Exit(FH_VOID);
typedef FH_VOID FN_MOD_Notify(MOD_NOTICE_ID_E enNoticeId);
typedef FH_VOID FN_MOD_QueryState(MOD_STATE_E *pstState);
typedef FH_UINT32 FN_MOD_VerChecker(FH_VOID);

typedef struct fyMPP_MODULE_S
{
#ifdef __KERNEL__
    struct list_head list;
    struct module *pstOwner;
#else
    struct rt_list_node list;
    void *pstOwner;
#endif
    FH_CHAR aModName[MAX_MOD_NAME];
    MOD_ID_E enModId;

    FN_MOD_Init         *pfnInit;
    FN_MOD_Exit         *pfnExit;
    FN_MOD_QueryState   *pfnQueryState;
    FN_MOD_Notify       *pfnNotify;
    FN_MOD_VerChecker   *pfnVerChecker;

    FY_BOOL  bInited;

    FH_VOID *pstExportFuncs;
    FH_VOID *pData;

    FH_CHAR *pVersion;
}UMAP_MODULE_S;


extern FH_CHAR *CMPI_GetModuleName(MOD_ID_E enModId);
extern UMAP_MODULE_S *CMPI_GetModuleById(MOD_ID_E enModId);
extern FH_VOID *CMPI_GetModuleFuncById(MOD_ID_E enModId);

extern FH_VOID CMPI_StopModules(FH_VOID);
extern FH_SINT32  CMPI_QueryModules(FH_VOID);
extern FH_SINT32  CMPI_InitModules(FH_VOID);
extern FH_VOID CMPI_ExitModules(FH_VOID);
extern FH_SINT32  CMPI_RegisterModule(UMAP_MODULE_S *pstModules);
extern FH_VOID CMPI_UnRegisterModule(MOD_ID_E enModId);

#define FUNC_ENTRY(type,id)  ((type*)CMPI_GetModuleFuncById(id))
#define CHECK_FUNC_ENTRY(id) (CMPI_GetModuleFuncById(id) != NULL)
#define FUNC_ENTRY_NULL(id)  (!CHECK_FUNC_ENTRY(id))


#endif /*  __MOD_EXT_H__ */

