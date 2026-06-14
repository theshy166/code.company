#include "esl_progress_bar.h"
#include "lvgl/lvgl.h"

//
lv_obj_t * create_progress_bar(lv_obj_t * parent) {
    if (NULL == parent){
        return NULL;
    }
    // 创建一个进度条对象
    lv_obj_t * progress_bar = lv_bar_create(parent);
    lv_obj_set_size(progress_bar, LV_USE_HOR_SIZE, 8); // 设置进度条的大小
    lv_obj_set_style_radius(progress_bar, 0, 0);// 设置圆角
    // lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0xffffff), LV_PART_MAIN); // 白色底
    // //设置进度条为绿色
    // lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0x00ff00), LV_PART_INDICATOR); // 设置为绿色
    lv_obj_align(progress_bar, LV_ALIGN_TOP_RIGHT, 0, 0); // 将进度条居中对齐  LV_ALIGN_OUT_BOTTOM_MID
    lv_obj_move_to_index(parent, 0);
    // 设置进度条的范围和初始值
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);
    return progress_bar;
}

void update_progress_bar(lv_obj_t * progress_bar, int value){
    if (NULL == progress_bar){
        return;
    }
    int current_value = lv_bar_get_value(progress_bar);
    if(current_value != value){
        lv_bar_set_value(progress_bar, value, LV_ANIM_ON);
    }
}

void destroy_progress_bar(lv_obj_t * progress_bar){
    if (NULL == progress_bar){
        return;
    }
    lv_obj_del(progress_bar);
}

