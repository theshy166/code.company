/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * 跨操作系统函数定义
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */
#ifndef __STD_OSAL_H__
#define __STD_OSAL_H__

//过度期先保留这里的定义,后续使用fh_std_api.h中的相关宏
#define ALIGNTO(n,align)     (((n) + ((align) - 1)) & ~((align) - 1))
#define MAX(a,b)             ((a) > (b) ? (a) : (b))
#define MIN(a,b)             ((a) < (b) ? (a) : (b))
#define ABS(x)               ((x) > 0 ? (x) : -(x))
#define JUDGE(data,min,max)  ((data) >= (min) ? ((data) <= (max) ? 1 : 0) : 0)
#define CLIP(data,min,max)   ((data) >= (min) ? ((data) <= (max) ? (data) : (max)) : (min))
#define MINUS_ABS(a,b)       ((a) > (b) ? (a) - (b) : (b) - (a))
#define OVERFLOW_U32_INTERVAL(_new,_old) \
	({ \
		FH_UINT32 _new_ = (FH_UINT32)(_new); \
		FH_UINT32 _old_ = (FH_UINT32)(_old); \
		FH_UINT32 _diff_= ((_new_) >= (_old_) ? ((_new_) - (_old_)) : (((FH_UINT32)0xffffffff) - (_old_) + (_new_))); \
		_diff_; \
	})

#if defined(OS_RTT)
	#if defined(USE_RTT3_0)
		#include "os_kernel/_rtt_v3.0_osal.h"
	#else
		#include "os_kernel/_rtt_v2.1_osal.h"
	#endif
#elif defined(OS_LINUX)
	#include <linux/version.h>
	#if LINUX_VERSION_CODE > KERNEL_VERSION(4,0,0)
		#include "os_kernel/_linux_v4.9_osal.h"
	#else
		#include "os_kernel/_linux_v3.0.8_osal.h"
	#endif
#elif defined(OS_NONE)
	#include "os_kernel/_osnone_osal.h"
#else
	#error "Need to define os-type"
#endif

#endif