/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_ALGO_MISC_H__
#define __QUA_ALGO_MISC_H__

#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

typedef struct qua_algo_img {
    qua_pixel_fmt_t fmt;
    QUA_U8 *data;
    QUA_U32 w;
    QUA_U32 h;
    QUA_U32 stride;
} qua_algo_img_t;

#ifdef __cplusplus
extern "C" {
#endif

QUA_S32 qua_region_magnify(qua_algo_img_t *qimg, QUA_U32 range);

QUA_S32 qua_region_pinch(qua_algo_img_t *qimg, QUA_U32 range);

QUA_S32 qua_filter_black_white(qua_algo_img_t *qimg);

QUA_S32 qua_filter_inverse(qua_algo_img_t *qimg);

QUA_S32 qua_filter_childhood(qua_algo_img_t *qimg);

QUA_S32 qua_filter_vintage(qua_algo_img_t *qimg);

QUA_S32 qua_filter_dreamy(qua_algo_img_t *qimg);

QUA_S32 qua_filter_handsome(qua_algo_img_t *qimg);

QUA_S32 qua_filter_stylization(qua_algo_img_t *qimg);

QUA_S32 qua_filter_pencil(qua_algo_img_t *qimg);

QUA_S32 qua_filter_snow(qua_algo_img_t *qimg);

QUA_S32 qua_filter_cartoon(qua_algo_img_t *qimg);

#ifdef __cplusplus
}
#endif

#endif

