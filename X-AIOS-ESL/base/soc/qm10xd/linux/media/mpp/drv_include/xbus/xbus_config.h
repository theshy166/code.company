#ifndef __XBUS_CONFIG_H__
#define __XBUS_CONFIG_H__

#include "rpc_hdr.h"

/************************************************************************************
***** C2C
*************************************************************************************/
#define FH_XBUS_THREAD_STACK_SIZE 8192
#define FH_XBUS_THREAD_NUM         RPC_PRIO_LVL_MAX

#if(defined(ARC0) )

#define PMU_ARC_IRQn            27 //  CA7_to_arc0
#define ARM_INT_RAWSTATUS      0x19100000
#define ARC_INT_RAWSTATUS      0x19100004
#define ARC_INT_ENABLE      0x1910000C
#define XBUS_MEMORY_REG     0x10000500

#elif(defined(ARC1) )


#if 0
#define PMU_ARC_IRQn            27 //  CA7_to_arc01
#define ARM_INT_RAWSTATUS      0x19300000
#define ARC_INT_RAWSTATUS      0x19300004
#define ARC_INT_ENABLE      0x1930000C
#else
#define PMU_ARC_IRQn            22 //  ARC1_to_arc01
#define ARM_INT_RAWSTATUS      0x19500000
#define ARC_INT_RAWSTATUS      0x19500004
#define ARC_INT_ENABLE      0x1950000C
#endif
#define XBUS_MEMORY_REG     0x10000504
#else
#error "not defined ARC0 or ARC1"
#endif

#endif
