/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 */
#ifndef __QUA_XOS_INTERNEL_HEADER_FILE__
#error "Please don't include this file directly, please include base_include.h"
#endif

#ifndef __BASE_INC_H__
#define __BASE_INC_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_QUA_XOS_BASE_MC331X_LINUX
#include "linux/__base_inc_linux.h"
#endif

#ifdef CONFIG_QUA_XOS_BASE_MC331X_RTTOS
#include "rtt/__base_inc_rtt.h"
#endif

#include "__base_vou_api.h"

#ifdef __cplusplus
}
#endif

#endif /*__BASE_INC_H__*/
