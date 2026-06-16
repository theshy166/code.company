/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MPP_BASE_H__
#define __QUA_MPP_BASE_H__

typedef enum mpp_element_type {
    MPP_ELEMENT_INVALID = -1,
    MPP_ELEMENT_VICAP,
    MPP_ELEMENT_ISP,
    MPP_ELEMENT_VPROC,
    MPP_ELEMENT_VENC,
    MPP_ELEMENT_VDEC,
    MPP_ELEMENT_VO,
    MPP_ELEMENT_AI,
    MPP_ELEMENT_AO,
    MPP_ELEMENT_MAX
} mpp_element_type_t;

typedef struct mpp_element_chn {
    mpp_element_type_t type;
    QUA_S32 device;
    QUA_S32 channel;
} mpp_element_chn_t;

#endif

