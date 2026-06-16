#include "qua_font.h"
#include "lvgl/lvgl.h"
#include <stdio.h>


static void qm_msg_box_hidden_callback(lv_timer_t *t)
{
    lv_obj_t * label = t->user_data;
    printf("[qm_msg_box]label=%p\n", label);
    if (label) {
        lv_obj_del(label);
    }
}

lv_obj_t * qm_message_box_ex(const char *show_tip, short duration_ms, const char* font_path,short font_size)
{
    if (show_tip == NULL || duration_ms < 500) {
        return;
    }
    lv_obj_t *tip_obj = lv_label_create(lv_layer_sys());
    lv_label_set_text(tip_obj, show_tip);
    lv_obj_set_style_text_color(tip_obj, lv_color_hex(0xffffff), 0);
    if(font_size > 0 && font_path != NULL)
    {
        lv_font_t *ft_font_ttf = NULL;
        qua_xos_ft_init_with_size_path(&ft_font_ttf, font_size, font_path, 0);
        lv_obj_set_style_text_font(tip_obj, ft_font_ttf, 0);
    }
    lv_obj_align(tip_obj, LV_ALIGN_CENTER, 0, 0);
    lv_timer_t *t = lv_timer_create(qm_msg_box_hidden_callback, duration_ms, tip_obj);
    lv_timer_set_repeat_count(t, 1);
    return tip_obj;
}

void qm_message_box(const char *show_tip, short duration_ms)
{
    qm_message_box_ex(show_tip, duration_ms, NULL, 0);
}