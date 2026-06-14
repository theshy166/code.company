/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file los_float_wnd.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "xos_float_wnd.h"

#if XOS_USE_FLOAT_WND != 0
#include "../ability/xos_ability.h"
#include "../debugger/xos_log.h"
#if XOS_USE_APP_MANAGER != 0
#include "appmanager.h"
#endif

/*****************
defines
*****************/
#define kSize 44
const uint8_t kOffset = kSize/2;

/****************
static variables
****************/
//don't do auto exit when already in launcher.
static lv_obj_t *last_scr_top_child = NULL;
static lv_obj_t *last_scr = NULL;
static lv_obj_t *exit_btn = NULL;
static bool dragging = false;//if dragging, don't deal as click.
static lv_point_t origin_point;

/**********************
 * static prototypes
*************************/
static void float_win_event_handler(lv_event_t *e);
static void timer_callback(struct _lv_timer_t *timer);
static void _create_float_window(void);
static void pin_float_button_to_edge(lv_event_t *e);
/*******
 * global prototypes
******/
void quit_top_app(void);

/*********************
 * function implements
**********************/

void init_float_window(uint8_t delay)
{
    LV_LOG_INFO("[float_wnd] init --> in. delay=%d", delay);
    if (delay > 0 && delay < 59) {
        lv_timer_t *timer = lv_timer_create(timer_callback, delay, NULL);
        lv_timer_set_repeat_count(timer, 1);
    } else {
        _create_float_window();
    }
}

static void timer_callback(struct _lv_timer_t *timer)
{
    lv_timer_pause(timer);
    lv_timer_del(timer);
    LV_LOG_INFO("[float_wnd] timer cb.");
    _create_float_window();
}

static void _create_float_window()
{
    //remember launcher
    lv_obj_t *act_screen = lv_scr_act();
    last_scr = act_screen;
    uint32_t child_cnt = lv_obj_get_child_cnt(act_screen);
    if (child_cnt > 0) {
        lv_obj_t *last_screen = lv_obj_get_child(act_screen, child_cnt - 1);
        LV_LOG_INFO("[float_wnd]launcher_screen = %p", last_screen);
        last_scr_top_child = last_screen;
    }
    //#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    //create float btn
    lv_obj_t *layer_top = lv_layer_sys();

    exit_btn = lv_btn_create(layer_top);
    lv_obj_set_style_radius(exit_btn, kSize, 0);
    lv_obj_set_size(exit_btn, kSize, kSize);
    // lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_radius(&style, LV_RADIUS_CIRCLE);
    // lv_obj_add_style(exit_btn, &style, 0);
    lv_obj_set_pos(exit_btn, LV_USE_HOR_SIZE - kSize - 300, 0);
    LV_IMG_DECLARE(icon_float_normal);
    LV_IMG_DECLARE(icon_float_pressing);
    lv_obj_set_style_bg_img_src(exit_btn, &icon_float_pressing, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(exit_btn, &icon_float_normal, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(exit_btn, lv_color_hex(0x000000), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(exit_btn, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(exit_btn, float_win_event_handler, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(exit_btn, float_win_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(exit_btn, float_win_event_handler, LV_EVENT_PRESSED, NULL);

    // lv_obj_t * exit_label = lv_label_create(exit_btn);
    // lv_label_set_text(exit_label, LV_SYMBOL_EJECT);
    // lv_obj_set_align(exit_label, LV_ALIGN_CENTER);

    LV_LOG_INFO("[float_win] create::btnimg=%p", exit_btn);
    //#endif
}

static void anim_float_wnd_cb_x(void * obj, int32_t v)
{
    lv_obj_set_x(obj, v);
}
static void anim_float_wnd_cb_y(void *obj, int32_t v)
{
    lv_obj_set_y(obj, v);
}
static void anim_float_wnd_cb_free(lv_anim_t * a)
{
    a->user_data = NULL;
}

static void float_win_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_PRESSING) {
        lv_obj_t *obj = lv_event_get_target(e);

        // LV_LOG_INFO("[float_win]obj=%p", obj);

        lv_indev_t * indev = lv_indev_get_act();
        lv_point_t point_act;
        lv_indev_get_point(indev, &point_act);
        // lv_coord_t x_diff = point_act.x - ext->point_last.x;
        // lv_coord_t y_diff = point_act.y - ext->point_last.y;
        // LV_LOG_INFO("[LV_EVENT_PRESSING]x=%d, y=%d, obj=%p", point_act.x, point_act.y, obj);
        lv_coord_t diffx = point_act.x - origin_point.x;
        lv_coord_t diffy = point_act.y - origin_point.y;

        // LV_LOG_INFO("[float_win] diffX=%d, diffY=%d.", diffx, diffy);
        if (diffx != 0 || diffy != 0) {
            dragging = true;
        }

        lv_obj_set_pos(obj, point_act.x-kOffset, point_act.y-kOffset);
        // lv_obj_update_layout(obj);
    }
    else if (code == LV_EVENT_PRESSED) {
        LV_LOG_INFO("LV_EVENT_PRESSED: dragging status = %d", dragging);

        lv_indev_t * indev = lv_indev_get_act();
        lv_point_t point_act;
        lv_indev_get_point(indev, &point_act);
        dragging = false;
        LV_LOG_INFO("[LV_EVENT_PRESSED]x=%d, y=%d.", point_act.x, point_act.y);
        origin_point.x = point_act.x;
        origin_point.y = point_act.y;
    }
    else if (code == LV_EVENT_CLICKED) {
        LOSLOG("CLICKED: dragging status = %d", dragging);
        if (dragging) {
            pin_float_button_to_edge(e);
            return;
        }

        quit_top_app();
    }
}

void quit_top_app()
{
    lv_obj_t *act_scr = NULL;
    lv_disp_t *disp_default = lv_disp_get_default();
    #if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9
    act_scr = lv_display_get_screen_active(disp_default);
    #else
    act_scr = disp_default->act_scr;
    LV_LOG_INFO("screen_cnt=%d. act_scr=%p, lv_scr_act()=%p", disp_default->screen_cnt, act_scr, lv_scr_act());
    #endif

    if (last_scr != act_scr) {
        #ifndef CONFIG_LVGL_V9
        LV_LOG_INFO("[float_wnd] remove act_scr=%p", disp_default->act_scr);
        #endif
        lv_disp_load_scr(last_scr);
        return;
    }

    lv_obj_t *top_screen = lv_scr_act();
    uint32_t child_cnt = lv_obj_get_child_cnt(top_screen);
    if (child_cnt > 0) {
        lv_obj_t *last_screen = lv_obj_get_child(top_screen, child_cnt - 1);
        if (last_scr_top_child == last_screen) {
            lv_obj_t *first_screen = lv_obj_get_child(top_screen, 0);
            LV_UNUSED(first_screen);
            LV_LOG_INFO("child_cnt=%d, last=%p, first=%p", child_cnt, last_screen, first_screen);
            return;
        }

        //LV_LOG_INFO("[float_wnd] will del panel(%d, %d), (%d,%d)", last_screen->coords.x1, last_screen->coords.y1, last_screen->coords.x2, last_screen->coords.y2);
        //LV_LOG_INFO("[float_wnd] will del panel = %p. child_cnt=%d", last_screen, child_cnt);
        // lv_obj_set_style_bg_color(last_screen, lv_color_hex(0xff0000), 0);
        // lv_obj_set_style_bg_opa(last_screen, 255, 0);
        // lv_obj_set_style_opa(last_screen, 255, 0);
        // child 
#if XOS_USE_APP_MANAGER != 0
        {
            const char* top_name = app_manager_topname();
            if (top_name) {
                app_manager_exit(top_name);
            }

            #if XOS_USE_APP_DEMOLIST != 0
                if (0 == strcmp(top_name, "A")
                    || 0 == strcmp(top_name, "B")
                    || 0 == strcmp(top_name, "C")
                    )
                {
                    return;
                }
            #endif
        }
#endif
        xos_ability_cool_out(last_screen, XOS_ABILITY_COOL_OUT_TIME, XOS_ABILITY_COOL_OUT_DELAY);
        lv_obj_del(last_screen);

        #if 0 //some test
        //after delete, check again.
        uint32_t child_cnt2 = lv_obj_get_child_cnt(top_screen);
        last_screen = lv_obj_get_child(top_screen, child_cnt2 - 1);
        first_screen = lv_obj_get_child(top_screen, 0);

        LV_LOG_INFO("[after]child_cnt=%d, last=%p, first=%p", child_cnt2, last_screen, first_screen);
        #endif
    }
}

static void pin_float_button_to_edge(lv_event_t *e)
{
    lv_indev_t * indev = lv_indev_get_act();
    lv_point_t point_act;
    lv_indev_get_point(indev, &point_act);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_coord_t newX, newY;
    if (point_act.x < LV_USE_HOR_SIZE/2) {
        if (point_act.y < LV_USE_VER_SIZE/2) {
            newX = kOffset;
            newY = kSize + kOffset;
        } else {
            newX = kOffset;
            newY = LV_USE_VER_SIZE - kSize - kOffset;
        }
    }
    else {
        if (point_act.y < LV_USE_VER_SIZE/2) {
            newX = LV_USE_HOR_SIZE - kOffset;
            newY = kSize + kOffset;
        } else {
            newX = LV_USE_HOR_SIZE - kOffset;
            newY = LV_USE_VER_SIZE - kSize - kOffset;
        }
    }
    lv_anim_t move_anim_x;
    lv_anim_init(&move_anim_x);
    lv_anim_set_var(&move_anim_x, obj);
    lv_anim_set_time(&move_anim_x, 300);
    lv_anim_set_values(&move_anim_x, point_act.x, newX - kOffset);
    lv_anim_set_exec_cb(&move_anim_x, anim_float_wnd_cb_x);
    // lv_anim_set_deleted_cb(&move_anim_x, anim_float_wnd_cb_free);
    lv_anim_set_repeat_count(&move_anim_x, 0);
    lv_anim_start(&move_anim_x);
    //
    lv_anim_t move_anim_y;
    lv_anim_init(&move_anim_y);
    lv_anim_set_var(&move_anim_y, obj);
    lv_anim_set_time(&move_anim_y, 300);
    lv_anim_set_values(&move_anim_y, point_act.y, newY - kOffset);
    lv_anim_set_exec_cb(&move_anim_y, anim_float_wnd_cb_y);
    // lv_anim_set_deleted_cb(&move_anim_y, anim_float_wnd_cb_free);
    lv_anim_set_repeat_count(&move_anim_x, 0);
    lv_anim_start(&move_anim_y);
}

#endif
