/*******************************************************************************************
#  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_OSD_H__
#define __QUA_MM_OSD_H__

#include "filter/qua_mm_filter.h"

#define QUA_OSD_CTRL_DEFAULT (0)  /*(QUA_OSD_CTRL_LOGO_BEFORE_VP | QUA_OSD_CTRL_TOSD_BEFORE_VP)*/
#define QUA_OSD_CTRL_LOGO_BEFORE_VP (1<<0)
#define QUA_OSD_CTRL_TOSD_BEFORE_VP (1<<1)
#define QUA_OSD_CTRL_GBOX_BEFORE_VP (1<<2)
#define QUA_OSD_CTRL_MASK_BEFORE_VP (1<<3)
#define QUA_OSD_CTRL_CIRCLE_BEFORE_VP (1<<4)
#define QUA_OSD_CTRL_LOGO_AFTER_VP (1<<16)
#define QUA_OSD_CTRL_TOSD_AFTER_VP (1<<17)
#define QUA_OSD_CTRL_GBOX_AFTER_VP (1<<18)
#define QUA_OSD_CTRL_MASK_AFTER_VP (1<<19)
#define QUA_OSD_CTRL_CIRCLE_AFTER_VP (1<<20)

#define QUA_OSD_TIME_BIT_HOUR12 (1<<0)
#define QUA_OSD_TIME_BIT_WEEK   (1<<1)
#define QUA_OSD_TIME_BIT_AFTER  (1<<2)

#define QUA_OSD_TEXT_DEFAULT_ALIGN (0)
#define QUA_OSD_TEXT_RIGHT_ALIGN   (1<<0)
#define QUA_OSD_TEXT_TWINKLE       (1<<1)

#define QUA_OSD_FLAG_DEFAULT      (0)
#define QUA_OSD_FLAG_USE_TWO_BUF  (1 << 0)
#define QUA_OSD_FLAG_TWINKLE      (FHT_OSD_TEXT_TWINKLE)
#define QUA_OSD_FLAG_EXTERNAL_MEM (1<<2)

typedef enum qua_osd_font_type
{
    QUA_FONT_TYPE_ASC = 0,
    QUA_FONT_TYPE_CHINESE,
} qua_osd_font_type_t;

typedef enum qua_osd_time_fmt
{
    QUA_OSD_TimeFmt0 = 0,
    QUA_OSD_TimeFmt1,
    QUA_OSD_TimeFmt2,
    QUA_OSD_TimeFmt3,
    QUA_OSD_TimeFmt4,
    QUA_OSD_TimeFmt5,
    QUA_OSD_TimeFmt6,
    QUA_OSD_TimeFmt7,
    QUA_OSD_TimeFmt8,
    QUA_OSD_TimeFmt9,
    QUA_OSD_TimeFmt10,
    QUA_OSD_TimeFmt11,
    QUA_OSD_TimeFmt12,
    QUA_OSD_TimeFmt13,
    QUA_OSD_TimeFmt14,
    QUA_OSD_TimeFmt_BUTT,
} qua_osd_time_fmt_t;

typedef enum qua_osd_invert_config
{
    QUA_OSD_INVERT_DISABLE = 0,
    QUA_OSD_INVERT_BY_CHAR,
    QUA_OSD_INVERT_BY_LINE,
} qua_osd_invert_config_t;

typedef struct qua_rgb_color
{
	QUA_U8 red;
	QUA_U8 green;
    QUA_U8 blue;
    QUA_U8 alpha;
} qua_rgb_color_t;

typedef struct qua_osd_invert_threshold
{
	QUA_U8 high_level;
	QUA_U8 low_level;
} qua_osd_invert_threshold_t;

typedef struct qua_osd_layer_config
{
    QUA_U16 start_x;
    QUA_U16 start_y;
    QUA_U16 max_width;
    QUA_U16 max_height;
    QUA_U32 text_size;
    qua_rgb_color_t normal_color;
    qua_rgb_color_t invert_color;
    qua_rgb_color_t edge_color;
    qua_rgb_color_t bg_color;
    QUA_U8 edge_pixel;
    qua_osd_invert_config_t invert_config;
    qua_osd_invert_threshold_t invert_threshold;
	QUA_U8 flag;
	QUA_U8 id;
    qua_mem_info_t external_mminfo;
} qua_osd_layer_config_t;

typedef struct qua_osd_config
{
    qua_rotate_t rotate;
    QUA_U8 layer_num;
    qua_osd_layer_config_t *layer_config;
} qua_osd_config_t;

typedef struct qua_osd_text_line
{
	const QUA_CHAR *user_text;
	QUA_BOOL enable_user_text;
    QUA_BOOL enable_time_osd;
    QUA_U8 time_osd_flag;
    qua_osd_time_fmt_t time_osd_format;
    QUA_U32 start_x;
    QUA_U32 start_y;
    QUA_U32 line_size;
    QUA_U8 text_flag;
    QUA_U8 id;
    QUA_BOOL enable;
} qua_osd_text_line_t;

typedef struct qua_mm_osd_device {
    struct qua_mm_device_t parent;

    QUA_S32 (*init)(QUA_GROUP group_id, QUA_U32 ctrl);
    QUA_S32 (*deinit)(QUA_GROUP group_id);
    QUA_S32 (*load_font)(qua_osd_font_type_t type, QUA_U32 size, const QUA_U8 *data);
    QUA_S32 (*unload_font)(qua_osd_font_type_t type);
    QUA_S32 (*set_text)(QUA_GROUP group_id, QUA_S32 chn_id, qua_osd_config_t *osd_config);
    QUA_S32 (*set_text_line)(QUA_GROUP group_id, QUA_S32 chn_id, QUA_S32 layer_id, qua_osd_text_line_t *text_line);
} qua_mm_osd_device_t;

#endif
