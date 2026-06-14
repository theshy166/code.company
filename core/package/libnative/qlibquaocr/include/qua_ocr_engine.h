/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef QUA_OCR_ENGINE_H__
#define QUA_OCR_ENGINE_H__

#include "common/qua_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum qua_ocr_image_fmt_t {
    QUA_OCR_IMAGE_FMT_GRAY,
} qua_ocr_image_fmt_t;

typedef struct qua_ocr_image_t {
    QUA_U8 *buffer;
    QUA_U32 width;
    QUA_U32 height;
    qua_ocr_image_fmt_t format;
} qua_ocr_image_t;

typedef struct qua_ocr_line_item_t {
    char *data;
    QUA_U32 size;
    QUA_FLOAT score;
} qua_ocr_line_item_t;

typedef struct qua_ocr_item_t {
    QUA_U32 line_cnt;
    qua_ocr_line_item_t *line_items;
} qua_ocr_item_t;

typedef struct qua_ocr_module_t {
    QUA_S32 (*recognize_image)(qua_ocr_image_t *image, qua_ocr_item_t *item);
    QUA_VOID (*release_items)(qua_ocr_item_t *item);
} qua_ocr_module_t;

QUA_S32 qua_ocr_load_module(qua_ocr_module_t *module);

#ifdef __cplusplus
}
#endif

#endif /* QUA_OCR_ENGINE_H__ */
