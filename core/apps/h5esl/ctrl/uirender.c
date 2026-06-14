#include "lvgl/lvgl.h"
#include "uirender.h"
#include "../model/esl_ui_struct.h"
#include "qm_updater.h"
#include "../view/components/esl_progress_bar.h"
#include "../view/components/esl_down_log.h"
#include "../view/page_esl.h"

static lv_obj_t * top_layer_container_1 = NULL;
static lv_obj_t * top_layer_container_2 = NULL;



// 1:bar1 2:bar2
lv_obj_t * get_top_layer1(int index) {
    /* lv_display_t * _disp1 = lv_display_get_1st_scr();
    if(_disp1 != NULL){
        return _disp1->sys_layer;
    }
    return NULL; */
    return top_layer_container_1;
}
lv_obj_t * get_top_layer2(int index) {
    /* lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 != NULL){
        return _disp2->sys_layer;
    }
    return NULL; */
    return top_layer_container_2;
}

//
esl_ui_struct_t g_ui_struct_1 = {0};
esl_ui_struct_t g_ui_struct_2 = {0};
static lv_timer_t *s_timer = NULL;

static void render_timer_callback(lv_timer_t *timer)
{
    //bar1
    if (g_ui_struct_1.bar_visible) {
        if (g_ui_struct_1.bar == NULL) {
            g_ui_struct_1.bar = create_progress_bar(get_top_layer1(1));
            lv_obj_set_style_radius(g_ui_struct_1.bar, 0, LV_PART_MAIN);
            lv_obj_invalidate(top_layer_container_1);
        }
        else {
            //g_ui_struct_1.bar_value += 5;
            update_progress_bar(g_ui_struct_1.bar, g_ui_struct_1.bar_value);
        }
    }
    else {
        destroy_progress_bar(g_ui_struct_1.bar);
        g_ui_struct_1.bar = NULL;
        g_ui_struct_1.bar_value = 0;
    }
    //bar2
    if (g_ui_struct_2.bar_visible) {
        if (g_ui_struct_2.bar == NULL) {
            g_ui_struct_2.bar = create_progress_bar(get_top_layer2(2));
            lv_obj_set_style_radius(g_ui_struct_2.bar, 0, LV_PART_MAIN);
            lv_obj_invalidate(top_layer_container_2);
        }
        else {
            //g_ui_struct_2.bar_value += 5;
            update_progress_bar(g_ui_struct_2.bar, g_ui_struct_2.bar_value);
        }
    }
    else {
        destroy_progress_bar(g_ui_struct_2.bar);
        g_ui_struct_2.bar = NULL;
        g_ui_struct_2.bar_value = 0;
    }
    //show down log 1
    if(g_ui_struct_1.down_log_visible){
        if(g_ui_struct_1.down_log == NULL) {
            g_ui_struct_1.down_log = esl_down_log_create(get_top_layer1(1));
        }
        else {
            esl_down_log_update(g_ui_struct_1.down_log, g_ui_struct_1.down_log_file_name);
        }
    }
    else {
        esl_down_log_destroy(g_ui_struct_1.down_log);
        g_ui_struct_1.down_log = NULL;
    }
    //show down log 2
    if(g_ui_struct_2.down_log_visible){
        if(g_ui_struct_2.down_log == NULL){
            g_ui_struct_2.down_log = esl_down_log_create(get_top_layer2(2));
        }
        else {
            esl_down_log_update(g_ui_struct_2.down_log, g_ui_struct_2.down_log_file_name);
        }
    }
    else {
        esl_down_log_destroy(g_ui_struct_2.down_log);
        g_ui_struct_2.down_log = NULL;
    }
}

static void init_container(){
    lv_display_t * _disp1 = lv_display_get_1st_scr();
    if(_disp1 != NULL){
        top_layer_container_1 = lv_obj_create(_disp1->sys_layer);
        lv_obj_set_size(top_layer_container_1, LV_HOR_RES, 30);
        lv_obj_set_style_bg_color(top_layer_container_1, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_radius(top_layer_container_1, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(top_layer_container_1, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(top_layer_container_1, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(top_layer_container_1, LV_OPA_50, LV_PART_MAIN);
        lv_obj_set_scrollbar_mode(top_layer_container_1, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_flag(top_layer_container_1, LV_OBJ_FLAG_HIDDEN);
    }

    lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 != NULL){
        top_layer_container_2 = lv_obj_create(_disp2->sys_layer);
        lv_obj_set_size(top_layer_container_2, LV_HOR_RES, 30);
        lv_obj_set_style_bg_color(top_layer_container_2, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_radius(top_layer_container_2, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(top_layer_container_2, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(top_layer_container_2, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(top_layer_container_2, LV_OPA_50, LV_PART_MAIN);
        lv_obj_set_scrollbar_mode(top_layer_container_2, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_flag(top_layer_container_2, LV_OBJ_FLAG_HIDDEN);
    }
}

void set_top_hidden(int index){
    if (index == 1) {
        lv_obj_add_flag(top_layer_container_1, LV_OBJ_FLAG_HIDDEN);
    } else if (index == 2) {
        lv_obj_add_flag(top_layer_container_2, LV_OBJ_FLAG_HIDDEN);
    }
}

void set_top_show(int index){
    if (index == 1) {
        lv_obj_clear_flag(top_layer_container_1, LV_OBJ_FLAG_HIDDEN);
    } else if (index == 2) {
        lv_obj_clear_flag(top_layer_container_2, LV_OBJ_FLAG_HIDDEN);
    }
}

void uirender_init(void)
{
    memset(&g_ui_struct_1, 0, sizeof(esl_ui_struct_t));
    memset(&g_ui_struct_2, 0, sizeof(esl_ui_struct_t));
    //初始化高度30的container
    init_container();
    s_timer = lv_timer_create(render_timer_callback, 500, NULL);
    lv_timer_set_repeat_count(s_timer, -1);
}

void uirender_quit(void)
{
    if (s_timer) {
        lv_timer_del(s_timer);
        s_timer = NULL;
    }
}


