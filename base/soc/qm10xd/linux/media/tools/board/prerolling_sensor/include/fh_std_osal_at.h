/**
 * 此文件对一些涉及系统资源的函数定义做二次封装，通过AUTOTEST的宏定义对这些函数进行重载。
 * 统计各模块对系统资源是否正常释放。检测内存泄露等问题。
 */
#ifndef __FH_STD_OSAL_AUTOTEST__
#define __FH_STD_OSAL_AUTOTEST__

#if defined(_FHK_API_LAYER)
#include "os_user/_osal_autotest.h"
#else //_FHK_DRV_LAYER
#include "os_kernel/_osal_autotest.h"
#endif

#endif // __FH_STD_OSAL_AUTOTEST__