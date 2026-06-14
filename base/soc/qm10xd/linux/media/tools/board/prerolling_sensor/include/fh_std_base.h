/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * 定义标准化模块的一些默认配置
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */
#ifndef __FH_STD_BASE__
#define __FH_STD_BASE__
#include "fh_std_typedef.h"
#include "fh_std_osal.h"
#include "fh_std_error.h"

#ifdef _FHK_OBJ_ZONE
#error "Don't include fh_std_base.h"
#endif
/* 本文件为fh_std模块内部参数定义,请勿引用此头文件 */
#define  _FHK_OBJ_ZONE FHK_STD_MODID2ZONE(FHK_STD_MOD_STD)

/**
 * STD模块如何决定消息是否被打印
 *
 * 1. 打印级别是否>=模块打印方式的级别       Y ----> 打印
 *           N ↓
 * 2. 对应属于的ZONE是否被MASK             Y ----> 不打印
 *           N ↓
 * 3. 打印级别是否>=对应ZONE的打印级别       Y ----> 打印
 *           N ↓
 *          不打印
 */

/* 追踪代码的相关级别 */

// 默认使用的系统打印函数
#define FHK_STD_DEF_PRT                fh_trace_msg
// 默认使用的内存信息打印函数
#define FHK_STD_DEF_DPRT(sfile,...)    seq_printf(sfile,__VA_ARGS__)
// 因为RTT没有vprintf的实现,这里的VPRT其实只是一个字符串的打印,不包含可变参数。
#define FHK_STD_DEF_VPRT               fh_vprintk

// 默认屏蔽的全局功能打印
#define FHK_STD_ZONE_MASK              (FHK_STD_ZONE_ALL)

// 默认的打印级别,只有大于等于此打印级别的消息进入对于输出
#define FHK_STD_DEF_MOD_PRT_LEVEL     (FHK_STD_LEV_NOTC)
#define FHK_STD_DEF_MOD_MPRT_LEVEL    (FHK_STD_LEV_INFO)
#define FHK_STD_DEF_ZONE_PRT_LEVEL    (FHK_STD_LEV_NOTC)
#define FHK_STD_DEF_ZONE_MPRT_LEVEL   (FHK_STD_LEV_DEBG)

// 单个打印最大字符长度
#define FH_STD_MAX_STR_LEN             (256)

/* 追踪相关功能开关 */

// 是否在每个打印消息之前加上系统时间
#define FHK_STD_TRACE_TIME             (0)
#define FHK_STD_MTRACE_TIME            (1)


// 消息级别使用的打印颜色
#define PRT_BK                "\033[30m" //黑
#define PRT_RD                "\033[31m" //红
#define PRT_GN                "\033[32m" //绿
#define PRT_YL                "\033[33m" //黄
#define PRT_BU                "\033[34m" //蓝
#define PRT_PL                "\033[35m" //紫
#define PRT_WT                "\033[37m" //白
#define PRT_DEF               "\033[0m"  //重置颜色

#endif // !__FH_STD_BASE__
