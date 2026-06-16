/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * 跨操作系统函数定义
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-25     chenjn       the first version
 *
 */
#ifndef __STD_USR_OSAL_H__
#define __STD_USR_OSAL_H__

#if defined(OS_RTT)
	#include "os_user/_rtt_osal_usr.h"
#elif defined(OS_LINUX)
	#include "os_user/_linux_osal_usr.h"
#elif defined(OS_NONE)
	#include "os_user/_osnone_osal_usr.h"
#else
	#error "Need to define os-type"
#endif

#endif
