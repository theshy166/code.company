/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file los_ability.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "xos_ability.h"

#if XOS_USE_ABILITY != 0

#include <stdio.h>
#include <stdlib.h>
#include "qua_font.h"
#include <string.h>

#define MY_CLASS &lv_ability_class

/**********************
 *      TYPEDEFS
 **********************/
#define LV_ABILITY_NIMATION_TIME    50 //200
#define LV_ABILITY_HIDE_THRESHOLD   80

#define __XOS_SELF_GJY_ABILITY_BGCONT__     0
#define __XOS_SELF_GJY_ABILITY__            1

/*Drag throw slow-down in [%]. Greater value -> faster slow-down*/
#define LV_INDEV_DEF_SCROLL_THROW         10

#define XOS_LOGU(fmt, ...)    LV_LOG_USER(fmt, ##__VA_ARGS__)
#define XOS_LOGI(fmt, ...)    LV_LOG_INFO(fmt, ##__VA_ARGS__)
#define XOS_LOGE(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_ability_set_slide_anim_callback(lv_obj_t * obj);
static void lv_ability_right_pressing_cb(lv_event_t * e);
static void lv_ability_right_press_lost_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
*   Global VARIABLES
**********************/
lv_style_t g_theme_style;
lv_style_t g_theme_style_transp;
lv_style_t g_theme_style_transp_fit;
lv_theme_t g_theme;

lv_style_t g_theme_style_circle;
lv_style_t g_theme_style_circle_pressed;

xos_ability_power_ctrl_t g_power_ctrl = {0};
/**********************
 *  STATIC FUNCTIONS
 **********************/

static void lv_ability_prepare_destory(lv_obj_t * obj)
{
    if(lv_obj_is_valid(obj) == false) return;
    lv_ability_t * ability = (lv_ability_t *)obj->user_data;
    if(ability == NULL) return;

    xos_ability_cool_out(obj, XOS_ABILITY_COOL_OUT_TIME, XOS_ABILITY_COOL_OUT_DELAY);

    if(ability->user_data) {
        lv_mem_free(ability->user_data);
        ability->user_data = NULL;
    }
    lv_ability_ext_t *ext = ability->top_obj_ext;
    if(ext) {
        lv_mem_free(ext);
        ability->top_obj_ext = NULL;
    }
    lv_mem_free(ability);
    obj->user_data = NULL;
}

static void lv_ability_hide_anim_end(lv_anim_t * a)
{
    XOS_LOGI("e");
    lv_obj_t * act_obj = a->var;
    LV_ASSERT_NULL(act_obj);
    if(act_obj == NULL) return;
    XOS_LOGI("%d %p __GJY__", __LINE__, act_obj);
    lv_obj_t * cont = (lv_obj_t *)lv_obj_get_child(lv_obj_get_child(act_obj, 0), 0);
    LV_ASSERT_NULL(cont);
    if(cont == NULL) return;
    lv_ability_prepare_destory(cont);
    //lv_obj_move_background(act_obj);
    if(act_obj) lv_obj_del(act_obj);
    //lv_obj_clean(lv_scr_act());
    XOS_LOGI("x");
}

static lv_res_t lv_ability_hor_hide_action(lv_obj_t * obj, lv_coord_t new_x)
{
    LV_ASSERT_NULL(obj);
    if(obj == NULL) return LV_RES_INV;
    lv_ability_t *ext = (lv_ability_t *)obj->user_data;
    LV_ASSERT_NULL(ext);
    if(ext == NULL) return LV_RES_INV;

#if LV_USE_ANIMIMG != 0
    XOS_LOGI("new_x=%d %p __GJY__", new_x, ext->top_obj);
    lv_ability_set_x_anim(ext->top_obj, new_x, lv_ability_hide_anim_end);
    return LV_RES_OK;
#else
    lv_ability_prepare_destory(obj);
    if(obj) lv_obj_del(obj);
    return LV_RES_INV;
#endif
}

static void lv_ability_pressed_handler(lv_event_t * e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_ability_t *ext = (lv_ability_t *)obj->user_data;

    lv_indev_t * indev = lv_indev_get_act();
    lv_indev_get_point(indev, &ext->point_last);
    ext->point_start.x = ext->point_last.x;
    ext->point_start.y = ext->point_last.y;
    ext->point_now.x = ext->point_last.x;
    ext->point_now.y = ext->point_last.y;
    //XOS_LOGI("[pressed]: point_now.x=%d, point_now.y=%d, point_start.x=%d, point_start.y=%d", ext->point_now.x, ext->point_now.y, ext->point_start.x, ext->point_start.y);
}

static void lv_ability_pressing_handler(lv_event_t * e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_ability_t *ext = (lv_ability_t *)obj->user_data;

    lv_indev_t * indev = lv_indev_get_act();
    lv_point_t point_act;
    lv_indev_get_point(indev, &point_act);
    lv_coord_t x_diff = point_act.x - ext->point_last.x;
    lv_coord_t y_diff = point_act.y - ext->point_last.y;
    XOS_LOGI("x_diff=%d, y_diff=%d", x_diff, y_diff);

    ext->point_now.x = point_act.x;
    ext->point_now.y = point_act.y;

    //XOS_LOGI("[pressing]: point_now.x=%d, point_now.y=%d, point_start.x=%d, point_start.y=%d\n",ext->point_now.x, ext->point_now.y, ext->point_start.x, ext->point_start.y);
    if(ext->draging == 0) {
        if(abs(x_diff) > abs(y_diff) && abs(x_diff) > ext->hor_drag_start) {
            ext->drag_hor = 1;
            ext->draging = 1;
            ext->is_drag_right = (x_diff >= ext->hor_drag_start) ? 1 : 0;
        } else if(abs(y_diff) >= ext->ver_drag_start) {
            ext->drag_hor = 0;
            ext->draging = 1;
            ext->is_drag_up = (y_diff >= ext->ver_drag_start) ? 0 : 1;
        }
    }

    if(ext->drag_hor) {
        /* hor sliding*/
        if(ext->is_drag_right && ext->slide_right_pressing_cb) {
            ext->slide_right_pressing_cb(e);
        }
    } else if(ext->draging) {
        if(ext->is_drag_up && ext->slide_up_pressing_cb) {
            ext->slide_up_pressing_cb(e);
        }
    }

    ext->point_last.x = point_act.x;
    ext->point_last.y = point_act.y;
}

static void lv_ability_press_lost_handler(lv_event_t * e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_ability_t *ext = (lv_ability_t *)obj->user_data;

    if(ext->drag_hor) {
        /* hor sliding*/
        if(ext->is_drag_right && ext->slide_right_press_lost_cb) {
            ext->slide_right_press_lost_cb(e);
        }
    } else if(ext->draging) {
        /* ver sliding*/
        if(ext->is_drag_up && ext->slide_up_press_lost_cb) {
            ext->slide_up_press_lost_cb(e);
        }
    }

    ext->drag_hor = 0;
    ext->draging = 0;
}

static void lv_ability_right_pressing_cb(lv_event_t * e)
{
    XOS_LOGI("e");
    // lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    lv_ability_t * ability = (lv_ability_t *)obj->user_data;
    LV_ASSERT_NULL(ability);
    if(ability == NULL) return;
    lv_obj_t * act_obj = ability->top_obj;

    lv_coord_t new_x = 0 +  ability->point_now.x - ability->point_start.x;
    new_x = (new_x > LV_USE_HOR_SIZE) ? LV_USE_HOR_SIZE : new_x;
    new_x = (new_x < 0) ? 0 : new_x;
    XOS_LOGI("m %p %d __GJY__", act_obj, new_x);
    lv_obj_set_x(act_obj, new_x);
    XOS_LOGI("x");
}

static void lv_ability_right_press_lost_cb(lv_event_t * e)
{
    XOS_LOGI("e");
    // lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    lv_ability_t * ability = (lv_ability_t *)obj->user_data;
    LV_ASSERT_NULL(ability);
    if(ability == NULL) return;
    lv_obj_t * act_obj = ability->top_obj;

    lv_indev_t * indev = lv_indev_get_act();
    lv_point_t point_act;
    lv_indev_get_point(indev, &point_act);
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
    lv_coord_t x_predict = 0;

    while(vect.x != 0)   {
        x_predict += vect.x;
        vect.x = vect.x * (100 - LV_INDEV_DEF_SCROLL_THROW) / 100;
    }

    lv_coord_t x = lv_obj_get_x(act_obj);

    // XOS_LOGI("old_x=%d, x_predict=%d", x, x_predict);
    if(x_predict < 0) {
        x = x + x_predict;
    }
    // XOS_LOGI("new_x=%d, x_predict=%d", x, x_predict);

    if(x > LV_ABILITY_HIDE_THRESHOLD) {
        lv_ability_hor_hide_action(obj, LV_USE_HOR_SIZE);
    } else {
        lv_ability_set_x_anim(act_obj, 0, NULL);
    }
    XOS_LOGI("x");
}

static void lv_ability_set_slide_anim_callback(lv_obj_t * obj)
{
    lv_ability_t * ability = (lv_ability_t*)obj->user_data;
    LV_ASSERT_NULL(ability);
    if(ability == NULL) return;

    if(ability->slide_anim_mode == XOS_ABILITY(ANIM_HOR_RIGHT_HIDE)) {
        ability->slide_right_pressing_cb = lv_ability_right_pressing_cb;
        ability->slide_right_press_lost_cb = lv_ability_right_press_lost_cb;
    }
    else
    {
        XOS_LOGE("%s WARNNING: callback is missed.", __func__);
    }
}

static void _anim_exec_xcb(void *var, int32_t v)
{
    XOS_LOGI("e");
    XOS_LOGI("%p %d", var, v);
    if(lv_obj_is_valid(var) == false) return;
    lv_obj_set_x((lv_obj_t * )var, (lv_coord_t)v);
    XOS_LOGI("x");
}

static void lv_ability_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    XOS_LOGI("%d %d", __LINE__, code);

    if(LV_EVENT_PRESSING == code)
    {
        lv_ability_pressing_handler(e);
    }
    if(LV_EVENT_PRESS_LOST == code || LV_EVENT_RELEASED == code)
    {
        lv_ability_press_lost_handler(e);
    }
    if(LV_EVENT_PRESSED == code)
    {
        lv_ability_pressed_handler(e);
    }
    if(LV_EVENT_DELETE == code)
    {
        XOS_LOGI("%d %d LV_EVENT_DELETE", __LINE__, code);
        lv_ability_prepare_destory(obj);
        //lv_obj_clean_style_list(obj, LV_PART_MAIN);
    }
    if( code != LV_EVENT_PRESSING && code != LV_EVENT_PRESSED && code != LV_EVENT_RELEASED
     && code != LV_EVENT_PRESS_LOST && code != LV_EVENT_DELETE )
    {
        XOS_LOGI("m");
    }
}
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_ability_ext_create(lv_obj_t * parent, lv_ability_ext_t * ext)
{
    XOS_LOGI("e");
    lv_obj_t *obj = lv_ability_create(parent);
    LV_ASSERT_NULL(obj);
    if(obj == NULL) return NULL;

    lv_ability_t * ability = (lv_ability_t *)obj->user_data;
    LV_ASSERT_NULL(ability);
    if(ability == NULL) return NULL;

    lv_obj_t *top = ability->top_obj;
    LV_ASSERT_NULL(top);
    if(top == NULL) return NULL;

    if(ext)
    {
        void * _ext = lv_mem_alloc(sizeof(lv_ability_ext_t));
        if(_ext == NULL) return NULL;
        lv_memset_00(_ext, sizeof(lv_ability_ext_t));

        ability->top_obj_ext = (lv_ability_ext_t *)_ext;
        ability->top_obj_ext->ability_id = ext->ability_id;
        ability->top_obj_ext->destory = ext->destory;
        ability->top_obj_ext->parent = ext->parent;
    }

    XOS_LOGI("x");
    return obj;
}

lv_obj_t * lv_ability_create(lv_obj_t * parent)
{
    XOS_LOGI("e");

    if(parent) lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t * obj = lv_obj_create(parent? parent:lv_scr_act());
    LV_ASSERT_NULL(obj);
    if(obj == NULL) return NULL;
    XOS_LOGI("%d %p __GJY__", __LINE__, obj);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_size(obj,  LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_obj_set_style_border_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);

#if 0
    //lv_obj_remove_style_all(ability);
    lv_obj_set_size(obj,  LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_obj_set_style_border_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
#endif
    lv_obj_t * img = lv_img_create(obj);
    LV_ASSERT_NULL(img);
    if(img == NULL) return NULL;
    lv_obj_set_size(img, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    xos_cm_set_img_bg(img);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);
    //lv_obj_set_style_border_opa(img, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(img, lv_color_black(), LV_PART_MAIN);
    //lv_obj_clear_flag(img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(img, LV_ALIGN_CENTER);

    lv_obj_t * cont = lv_obj_create(img);
    LV_ASSERT_NULL(cont);
    if(cont == NULL) return NULL;
    lv_obj_set_size(cont, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
#if 0
    lv_obj_set_style_border_opa(cont, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(cont, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(cont, LV_ALIGN_CENTER);
#endif
    lv_obj_add_style(cont, &g_theme_style_transp, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    void * user_data = lv_mem_alloc(sizeof(lv_ability_t));
    if(user_data == NULL) return NULL;
    lv_memset_00(user_data, sizeof(lv_ability_t));
    cont->user_data = user_data;

    lv_ability_t *ext = (lv_ability_t *)user_data;
    ext->obj = cont;
    ext->top_obj = obj;
    ext->hor_drag_start = 5;
    ext->ver_drag_start = 5;
    ext->is_drag_right = 1;
    ext->is_drag_up = 1;
    ext->slide_enable = 1;
    ext->slide_anim_mode = XOS_ABILITY(ANIM_HOR_RIGHT_HIDE);
    lv_ability_set_slide_anim_callback(cont);
    lv_obj_add_event_cb(cont, lv_ability_event_cb, LV_EVENT_PRESSING, obj);
    lv_obj_add_event_cb(cont, lv_ability_event_cb, LV_EVENT_PRESS_LOST, obj);
    lv_obj_add_event_cb(cont, lv_ability_event_cb, LV_EVENT_PRESSED, obj);
    lv_obj_add_event_cb(cont, lv_ability_event_cb, LV_EVENT_DELETE, obj);
    lv_obj_add_event_cb(cont, lv_ability_event_cb, LV_EVENT_RELEASED, obj);

    xos_ability_cool_in(obj, XOS_ABILITY_COOL_IN_TIME, XOS_ABILITY_COOL_IN_DELAY);

    XOS_LOGI("x");
    return cont;
}

void lv_ability_destory(lv_obj_t * obj)
{
    XOS_LOGI("e");
    if(!obj) return;

    lv_ability_prepare_destory(obj);

    lv_obj_t * act_obj = lv_ability_get_ability(obj);
    if(act_obj) {
        lv_obj_del(act_obj);
    }
    else {
        lv_obj_del(obj);
    }
    XOS_LOGI("x");
}

lv_obj_t * lv_ability_get_ability(lv_obj_t * obj)
{
//  lv_obj_t * tmp_obj = lv_obj_get_parent(obj);
//  if(tmp_obj == NULL) return NULL;
//  return lv_obj_get_parent(tmp_obj);
    LV_ASSERT_NULL(obj);
    if(obj == NULL) return NULL;
    lv_ability_t *ext = (lv_ability_t *)(obj->user_data);
    if(ext == NULL) return NULL;

    return ext->top_obj;
}

void lv_ability_set_x_anim(lv_obj_t * obj, lv_coord_t x, void (*cb)(lv_anim_t *))
{
    XOS_LOGI("e");
#if LV_USE_ANIMIMG != 0
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, lv_obj_get_x(obj), x);
    lv_anim_set_exec_cb(&a, _anim_exec_xcb);
    lv_anim_set_ready_cb(&a,(lv_anim_ready_cb_t)cb);
    lv_anim_set_time(&a, XOS_ABILITY_HIDE_TIME);
    lv_anim_set_delay(&a, 0);
    lv_anim_set_repeat_count(&a, 0);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
    lv_anim_start(&a);
#else
    lv_obj_set_x(obj, x);
#endif
    XOS_LOGI("x");
}

lv_res_t lv_ability_set_anim_mode(lv_obj_t * obj, lv_ability_anim_mode_t anim_mode)
{
    lv_ability_t *ability = (lv_ability_t *)(obj->user_data);
    if(ability == NULL) return LV_RES_INV;

    ability->slide_anim_mode = anim_mode;
    lv_ability_set_slide_anim_callback(obj);
    return LV_RES_OK;
}

lv_obj_t * xos_obj_create(lv_obj_t * parent)
{
    XOS_LOGI("e");
    lv_obj_t * obj = lv_obj_create(parent? parent:lv_scr_act());
    LV_ASSERT_NULL(obj);
    if(obj == NULL) return NULL;
    //lv_obj_remove_style_all(ability);
    lv_obj_set_size(obj,  LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_obj_set_style_border_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN);
    // lv_obj_set_style_radius(obj, 0, 0);
    //lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
    lv_obj_add_style(obj, &g_theme_style, LV_PART_MAIN);

    xos_ability_cool_in(obj, XOS_ABILITY_COOL_IN_TIME, XOS_ABILITY_COOL_IN_DELAY);

#if __XOS_SELF_GJY_ABILITY_BGCONT__ != 0
    lv_obj_t * img = lv_img_create(ability);
    LV_ASSERT_NULL(img);
    if(img == NULL) return NULL;
    lv_obj_set_size(img, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    //lv_img_set_src(img, watch_get_bg());
    //lv_obj_set_style_border_opa(img, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(img, lv_color_black(), LV_PART_MAIN);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(img, LV_ALIGN_CENTER);

    lv_obj_t * cont = lv_obj_create(ability);
    LV_ASSERT_NULL(cont);
    if(cont == NULL) return NULL;
    lv_obj_set_size(cont, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_obj_set_style_border_opa(cont, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_color(cont, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(cont, LV_ALIGN_CENTER);
#endif /*__XOS_SELF_GJY_ABILITY_BGCONT__*/
    XOS_LOGI("x");
    return obj;
}

void xos_cm_set_img_bg(lv_obj_t *img)
{
#if 0
#if LV_USE_FS_RES != 0
	lv_img_set_src(img, ICON_BACKGROUND);
#else
#if LV_USE_APP_BG_IMG != 0
	LV_IMG_DECLARE(img_background);
	lv_img_set_src(img, &img_background);
#endif
#endif
#endif
   // LV_IMG_DECLARE(img_background);
   // lv_img_set_src(img, &img_background);
}

const char *xos_cm_event_str(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_PRESSED:
        return "LV_EVENT_PRESSED";
    case LV_EVENT_PRESSING:
        return "LV_EVENT_PRESSING";
    case LV_EVENT_PRESS_LOST:
        return "LV_EVENT_PRESS_LOST";
    case LV_EVENT_SHORT_CLICKED:
        return "LV_EVENT_SHORT_CLICKED";
    case LV_EVENT_LONG_PRESSED:
        return "LV_EVENT_LONG_PRESSED";
    case LV_EVENT_LONG_PRESSED_REPEAT:
        return "LV_EVENT_LONG_PRESSED_REPEAT";
    case LV_EVENT_CLICKED:
        return "LV_EVENT_CLICKED";
    case LV_EVENT_RELEASED:
        return "LV_EVENT_RELEASED";
    case LV_EVENT_SCROLL_BEGIN:
        return "LV_EVENT_SCROLL_BEGIN";
    case LV_EVENT_SCROLL_END:
        return "LV_EVENT_SCROLL_END";
    case LV_EVENT_SCROLL:
        return "LV_EVENT_SCROLL";
    case LV_EVENT_GESTURE:
        return "LV_EVENT_GESTURE";
    case LV_EVENT_KEY:
        return "LV_EVENT_KEY";
    case LV_EVENT_FOCUSED:
        return "LV_EVENT_FOCUSED";
    case LV_EVENT_DEFOCUSED:
        return "LV_EVENT_DEFOCUSED";
    case LV_EVENT_LEAVE:
        return "LV_EVENT_LEAVE";
    case LV_EVENT_HIT_TEST:
        return "LV_EVENT_HIT_TEST";
    case LV_EVENT_COVER_CHECK:
        return "LV_EVENT_COVER_CHECK";
    case LV_EVENT_REFR_EXT_DRAW_SIZE:
        return "LV_EVENT_REFR_EXT_DRAW_SIZE";
    case LV_EVENT_DRAW_MAIN_BEGIN:
        return "LV_EVENT_DRAW_MAIN_BEGIN";
    case LV_EVENT_DRAW_MAIN:
        return "LV_EVENT_DRAW_MAIN";
    case LV_EVENT_DRAW_MAIN_END:
        return "LV_EVENT_DRAW_MAIN_END";
    case LV_EVENT_DRAW_POST_BEGIN:
        return "LV_EVENT_DRAW_POST_BEGIN";
    case LV_EVENT_DRAW_POST:
        return "LV_EVENT_DRAW_POST";
    case LV_EVENT_DRAW_POST_END:
        return "LV_EVENT_DRAW_POST_END";
#if !defined(CONFIG_LVGL_V9) || CONFIG_LVGL_V9 == 0
    case LV_EVENT_DRAW_PART_BEGIN:
        return "LV_EVENT_DRAW_PART_BEGIN";
    case LV_EVENT_DRAW_PART_END:
        return "LV_EVENT_DRAW_PART_END";
#endif
    case LV_EVENT_VALUE_CHANGED:
        return "LV_EVENT_VALUE_CHANGED";
    case LV_EVENT_INSERT:
        return "LV_EVENT_INSERT";
    case LV_EVENT_REFRESH:
        return "LV_EVENT_REFRESH";
    case LV_EVENT_READY:
        return "LV_EVENT_READY";
    case LV_EVENT_CANCEL:
        return "LV_EVENT_CANCEL";
    case LV_EVENT_DELETE:
        return "LV_EVENT_DELETE";
    case LV_EVENT_CHILD_CHANGED:
        return "LV_EVENT_CHILD_CHANGED";
    case LV_EVENT_CHILD_CREATED:
        return "LV_EVENT_CHILD_CREATED";
    case LV_EVENT_CHILD_DELETED:
        return "LV_EVENT_CHILD_DELETED";
    case LV_EVENT_SCREEN_UNLOAD_START:
        return "LV_EVENT_SCREEN_UNLOAD_START";
    case LV_EVENT_SCREEN_LOAD_START:
        return "LV_EVENT_SCREEN_LOAD_START";
    case LV_EVENT_SCREEN_LOADED:
        return "LV_EVENT_SCREEN_LOADED";
    case LV_EVENT_SCREEN_UNLOADED:
        return "LV_EVENT_SCREEN_UNLOADED";
    case LV_EVENT_SIZE_CHANGED:
        return "LV_EVENT_SIZE_CHANGED";
    case LV_EVENT_STYLE_CHANGED:
        return "LV_EVENT_STYLE_CHANGED";
    case LV_EVENT_LAYOUT_CHANGED:
        return "LV_EVENT_LAYOUT_CHANGED";
    case LV_EVENT_GET_SELF_SIZE:
        return "LV_EVENT_GET_SELF_SIZE";
    case _LV_EVENT_LAST:
        return "_LV_EVENT_LAST";
    case LV_EVENT_PREPROCESS:
        return "LV_EVENT_PREPROCESS";
    default:
        return "LV_EVENT_UNKOWN";
    }
}

const char * xos_cm_key_str(uint32_t key)
{
    switch(key){
    case LV_KEY_UP:
        return "LV_KEY_UP";
    case LV_KEY_DOWN:
        return "LV_KEY_DOWN";
    case LV_KEY_RIGHT:
        return "LV_KEY_RIGHT";
    case LV_KEY_LEFT:
        return "LV_KEY_LEFT";
    case LV_KEY_ESC:
        return "LV_KEY_ESC";
    case LV_KEY_DEL:
        return "LV_KEY_DEL";
    case LV_KEY_BACKSPACE:
        return "LV_KEY_BACKSPACE";
    case LV_KEY_ENTER:
        return "LV_KEY_ENTER";
    case LV_KEY_NEXT:
        return "LV_KEY_NEXT";
    case LV_KEY_PREV:
        return "LV_KEY_PREV";
    case LV_KEY_HOME:
        return "LV_KEY_HOME";
    case LV_KEY_END:
        return "LV_KEY_END";
    default :
        return "LV_KEY_UNKOWN";
    }
}

static void lv_ability_set_style(void)
{
    extern const lv_font_t lv_font_montserrat_12;
    /*normal obj style*/
    lv_style_t * style = &g_theme_style;
    lv_style_init(style);
    lv_style_set_border_width(style, 1);
    lv_style_set_border_color(style, lv_color_black());
    lv_style_set_pad_all(style, 2);
    lv_style_set_border_opa(style, LV_OPA_TRANSP);
    lv_style_set_bg_color(style, lv_color_black());
    lv_style_set_bg_opa(style, LV_OPA_COVER);

    /*Transparent style*/
    style = &g_theme_style_transp;
    lv_style_init(style);
    lv_style_set_border_width(style, 0);
    lv_style_set_border_side(style, LV_BORDER_SIDE_NONE);
    lv_style_set_bg_opa(style, LV_OPA_TRANSP);
    // lv_style_set_pad_inner(style, LV_DPI_DEF / 12);

    /*Transparent fit style*/
    style = &g_theme_style_transp_fit;
    lv_style_init(style);
    //lv_style_set_text_font(style, &lv_font_montserrat_12);
    lv_style_set_text_color(style, lv_color_hex(0xFFFFFF));
    lv_style_set_border_width(style, 0);
    lv_style_set_border_side(style, LV_BORDER_SIDE_NONE);
    lv_style_set_bg_opa(style, LV_OPA_TRANSP);
    // lv_style_set_pad_inner(style, LV_DPI_DEF / 12);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);


    style = &g_theme_style_circle;
    lv_style_init(style);
    lv_style_set_radius(style, LV_RADIUS_CIRCLE);
    lv_style_set_bg_opa(style, LV_OPA_100);
    lv_style_set_bg_color(style, lv_color_hex(0xffffff));
    lv_style_set_border_opa(style, LV_OPA_40);
    lv_style_set_border_width(style, 2);
    lv_style_set_border_color(style, lv_color_hex(0x000000));
    lv_style_set_outline_opa(style, LV_OPA_COVER);
    lv_style_set_outline_color(style, lv_color_hex(0x000000));
    lv_style_set_text_color(style, lv_color_white());
    lv_style_set_pad_all(style, 10);

    /*Init the pressed style*/
    style = &g_theme_style_circle_pressed;
    lv_style_init(style);
    /*Ad a large outline when pressed*/
    lv_style_set_outline_width(style, 15);
    lv_style_set_outline_opa(style, LV_OPA_TRANSP);
    lv_style_set_translate_y(style, 5);
    //lv_style_set_shadow_ofs_y(style, 3);
    lv_style_set_bg_color(style, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_color(style, lv_palette_main(LV_PALETTE_GREEN));
    /*Add a transition to the the outline*/
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, 0};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);
    lv_style_set_transition(style, &trans);
}

lv_theme_t * lv_ability_theme_init(void)
{
    lv_theme_t * th_act = lv_disp_get_theme(NULL);

    g_theme = *th_act;
    g_theme.color_primary = lv_palette_main(LV_PALETTE_BLUE);
    g_theme.color_secondary = lv_palette_main(LV_PALETTE_RED);
    g_theme.font_small = LV_FONT_DEFAULT;
    g_theme.font_normal = LV_FONT_DEFAULT;
    g_theme.flags = LV_THEME_DEFAULT_DARK;

    qua_los_ft_font_init();

    lv_ability_set_style();

    return &g_theme;
}

uint32_t xos_ability_rand(uint32_t seed, uint32_t min, uint32_t max)
{
    uint32_t a = seed; /*Seed*/

    /*Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"*/
    uint32_t x = a;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    a = x;

    return (a % (max - min + 1)) + min;
}

/*Cool special effects UI development*/
void xos_ability_cool_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    lv_obj_fade_in(obj, time, delay);
}

void xos_ability_cool_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    lv_obj_fade_out(obj, time, delay);
}

void xos_ability_cool_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    uint32_t _rand = xos_ability_rand(lv_tick_get(),0,10);
    //if(0 == _rand) return;
    if(1 == _rand) xos_ability_cool_fade_in(obj, time, delay);
    else xos_ability_cool_fade_in(obj, time, delay);
}

void xos_ability_cool_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    uint32_t _rand = xos_ability_rand(lv_tick_get(),0,10);
    //if(0 == _rand) return;
    if(1 == _rand) xos_ability_cool_fade_out(obj, time, delay);
    else xos_ability_cool_fade_out(obj, time, delay);
}

/*
   If the power control key is pressed briefly, the screen display will be turned off.
   When the screen is in the off state, if the power control key is pressed briefly
   again, the screen display will be turned on.
*/
void xos_ability_key_power_ctrl(void) {
    extern int qm_backlight_setbri(uint32_t);
    extern int qm_backlight_getbri();

    int brightness = 0;

    if (XOS_ABILITY_POWER_CTRL_OFF == g_power_ctrl.power_key_status) {
#if !(defined(BUILD_SIMULATOR)) && defined(CONFIG_XOS_HW_QUADRV)
        qm_backlight_setbri(g_power_ctrl.backlight_brightness);
        g_power_ctrl.power_key_status = XOS_ABILITY_POWER_CTRL_ON;
        XOS_LOGU("Restore screen brightness to %d\n", g_power_ctrl.backlight_brightness);
#endif
    } else if (XOS_ABILITY_POWER_CTRL_ON == g_power_ctrl.power_key_status) {
#if !(defined(BUILD_SIMULATOR)) && defined(CONFIG_XOS_HW_QUADRV)
        brightness = qm_backlight_getbri();
        if(brightness == -1) {
            XOS_LOGU("ERROR! qm_backlight_getbri = -1\n");
            return;
        }
        g_power_ctrl.backlight_brightness = brightness;
        XOS_LOGU("Saved screen brightness is %d\n", brightness);
        qm_backlight_setbri(0);
        g_power_ctrl.power_key_status = XOS_ABILITY_POWER_CTRL_OFF;
#endif
    }
}

static int setting_wifi_conn_state = 0;
int xos_get_wifi_connected_status(void) {
    // XOS_LOGU("wifi_conn_state is %d\n", setting_wifi_conn_state);
    return setting_wifi_conn_state;
}

void xos_set_wifi_connected_status(int bState) {
    setting_wifi_conn_state = (bState? true : false);
    //XOS_LOGU("wifi_conn_state is %d\n", setting_wifi_conn_state);
}

#define QM_MAX_CFG_SSID_STR_LEN 300

int xos_get_wifi_connected_ssid_psw(char *ssid, char *passwd) {
    FILE *file = NULL;
    char line[QM_MAX_CFG_SSID_STR_LEN];
    char *finStr = NULL;

    // 打开文件
    file = fopen("/data/wpa_supplicant.conf", "r");
    if (file == NULL) {
        printf("Failed to open wpa_supplicant.conf\n");
        return 1; // 返回错误码
    }

    // 逐行读取文件
    while (fgets(line, QM_MAX_CFG_SSID_STR_LEN, file) != NULL) {
        // 去掉行末的换行符
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // 查找 SSID
        finStr = strstr(line, "ssid=\"");
        if (finStr != NULL) {
            finStr += strlen("ssid=\"");
            while (*finStr != '"' && *finStr != '\0') {
                *ssid++ = *finStr++;
            }
            *ssid = '\0'; // 添加字符串结束符
        }

        // 查找密码
        finStr = strstr(line, "psk=\"");
        if (finStr != NULL) {
            finStr += strlen("psk=\"");
            while (*finStr != '"' && *finStr != '\0') {
                *passwd++ = *finStr++;
            }
            *passwd = '\0'; // 添加字符串结束符
        }
    }

    // 关闭文件
    fclose(file);
    return 0; // 成功返回
}

#endif /*XOS_USE_ABILITY*/
