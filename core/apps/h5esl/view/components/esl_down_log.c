#include "esl_down_log.h"
#include "qm_updater.h"
#include "../../conf/conf.h"
#include "qua_font.h"

extern void draw_border_for_debug(lv_obj_t * obj);

//
lv_obj_t * esl_down_log_create(lv_obj_t * parent){
    if (NULL == parent){
        return NULL;
    }
    lv_obj_t * download_info_label = lv_label_create(parent);
    lv_obj_set_size(download_info_label, LV_USE_HOR_SIZE/2, 20); 
    lv_obj_set_style_bg_color(download_info_label, lv_color_hex(0xffffff), 0); // 设置为红色
    lv_obj_align(download_info_label, LV_ALIGN_TOP_RIGHT, 30, 10); // 右上角对齐
    lv_label_set_text(download_info_label, "");
    lv_obj_move_to_index(parent, 0);
    lv_font_t *sn_font = NULL;
    qua_xos_ft_init_with_size_path(&sn_font, 12, k_path_ttf, 0);
    lv_obj_set_style_text_font(download_info_label, sn_font, 0);
    lv_obj_set_style_text_color(download_info_label, lv_color_hex(0x000000), 0);
    // draw_border_for_debug(download_info_label);

    return download_info_label;
}

void esl_down_log_update(lv_obj_t * log_obj, char * file_name){
    if(log_obj == NULL || file_name == NULL){
        return;
    }
    qm_updater_change_text_label(log_obj, file_name);
}

void esl_down_log_destroy(lv_obj_t * log_obj){
    if (NULL == log_obj){
        return;
    }
    lv_obj_del(log_obj);
}

