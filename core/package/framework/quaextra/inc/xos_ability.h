/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file xos_ability.h
 *
 */
#ifndef XOS_ABILITY_H
#define XOS_ABILITY_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

#if XOS_USE_ABILITY != 0
/*********************
 *      DEFINES
 *********************/
#define XOS_ABILITY_HIDE_TIME 50
#define XOS_ABILITY(x)  LV_ABILITY_SLIDE_##x

#define XOS_ABILITY_COOL_IN_TIME    1000
#define XOS_ABILITY_COOL_IN_DELAY   0
#define XOS_ABILITY_COOL_OUT_TIME   1000
#define XOS_ABILITY_COOL_OUT_DELAY  0

#define XOS_ABILITY_FADE_IN_TIME    XOS_ABILITY_COOL_IN_TIME
#define XOS_ABILITY_FADE_IN_DELAY   XOS_ABILITY_COOL_IN_DELAY
#define XOS_ABILITY_FADE_OUT_TIME   XOS_ABILITY_COOL_OUT_TIME
#define XOS_ABILITY_FADE_OUT_DELAY  XOS_ABILITY_COOL_OUT_DELAY

/**********************
 *      TYPEDEFS
 **********************/
enum
{
    /*General animation mode*/
    LV_ABILITY_SLIDE_ANIM_HOR_RIGHT_HIDE,
    LV_ABILITY_SLIDE_ANIM_VER_UP_HIDE,
    LV_ABILITY_SLIDE_ANIM_NONE,
};

typedef uint8_t lv_ability_anim_mode_t;

enum
{
    LV_ABILITY_SLIDE_RIGHT_PRESSING_CB,
    LV_ABILITY_SLIDE_RIGHT_PRESS_LOST_CB,
};

typedef uint8_t lv_ability_anim_type_t;

typedef void (*lv_ability_cb_t)(void *);

typedef void (*ability_prepare_destory_t)(lv_obj_t *);
typedef void (*ability_destory_t)(lv_obj_t *);
typedef lv_obj_t * (*ability_create_t)(lv_obj_t *);

typedef struct _lv_ability_ext_t_{
    unsigned int ability_id;
    lv_obj_t* parent;
    ability_destory_t destory;
}lv_ability_ext_t;

typedef struct _lv_ability_t{
    lv_obj_t * obj;
    lv_obj_t * top_obj;

    lv_point_t point_last;
    lv_point_t point_start;
    lv_point_t point_now;

    uint16_t slide_anim_time;
    uint16_t anim_time;
    lv_ability_anim_mode_t slide_anim_mode;
    uint8_t hor_drag_start;
    uint8_t ver_drag_start;
    uint8_t slide_enable :1;
    uint8_t draging :1;
    uint8_t drag_hor :1;
    uint8_t is_drag_right :1;
    uint8_t is_drag_up :1;

    lv_event_cb_t slide_right_pressing_cb;
    lv_event_cb_t slide_right_press_lost_cb;
    lv_event_cb_t slide_up_pressing_cb;
    lv_event_cb_t slide_up_press_lost_cb;

    lv_ability_ext_t * top_obj_ext;

    void * user_data;
} lv_ability_t;

/**********************
 * GLOBAL VARIABLES
 **********************/
extern lv_style_t g_theme_style;
extern lv_font_t* ft_font_ttf_20;
extern lv_font_t* ft_font_ttf_30;
extern lv_font_t* ft_font_ttf_12;
extern lv_font_t* ft_font_ttf_14;
extern lv_font_t* ft_font_ttf_40;
extern lv_style_t g_theme_style;
extern lv_style_t g_theme_style_transp;
extern lv_style_t g_theme_style_transp_fit;
extern lv_theme_t g_theme;
extern lv_style_t g_theme_style_circle;
extern lv_style_t g_theme_style_circle_pressed;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lv_obj_t * lv_ability_create(lv_obj_t *obj);
lv_obj_t * lv_ability_ext_create(lv_obj_t * parent, lv_ability_ext_t * ext);
void lv_ability_destory(lv_obj_t * obj);
lv_obj_t * lv_ability_get_ability(lv_obj_t * obj);
void lv_ability_set_x_anim(lv_obj_t * obj, lv_coord_t x, void (*cb)(lv_anim_t *));

lv_obj_t * xos_obj_create(lv_obj_t *obj);

void __ex_freetype_0(void);
int __ft_font_init(lv_font_t ** font, const char * font_path, uint16_t font_size, uint16_t font_style);

void xos_cm_set_img_bg(lv_obj_t *img);
const char * xos_cm_event_str(lv_event_t *e);
const char * xos_cm_key_str(uint32_t key);

lv_theme_t * lv_ability_theme_init(void);
lv_res_t lv_ability_set_anim_mode(lv_obj_t * obj, lv_ability_anim_mode_t anim_mode);

uint32_t xos_ability_rand(uint32_t seed, uint32_t min, uint32_t max);
/*Cool special effects UI development*/
void xos_ability_cool_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay);
void xos_ability_cool_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay);
void xos_ability_cool_in(lv_obj_t * obj, uint32_t time, uint32_t delay);
void xos_ability_cool_out(lv_obj_t * obj, uint32_t time, uint32_t delay);

/**********************
 *      MACROS
 **********************/

#endif /*XOS_USE_ABILITY*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*XOS_ABILITY_H*/
