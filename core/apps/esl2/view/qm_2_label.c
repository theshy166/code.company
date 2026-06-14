#include "qm_2_label.h"
#include "qm_lv_obj.h"

#if defined(CONFIG_XOS_APP_ESL2)

extern lv_font_t *ft_font_ttf_20;

lv_obj_t *qm_2_label_create(lv_obj_t *parent, int w, int h, const char *label_str, const char *value_str)
{
    lv_obj_t *parent_panel = qm_lv_obj_create(parent);
    lv_obj_set_size(parent_panel, w, h);
    //
    lv_obj_t *label = lv_label_create(parent_panel);
    lv_label_set_text(label, label_str);
    lv_obj_align(label, LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);
    lv_obj_set_style_text_font(label, ft_font_ttf_20, 0);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    // draw_border_for_debug(label);
    //
    lv_obj_t *value_label = lv_label_create(parent_panel);
    lv_label_set_text(value_label, value_str);
    lv_obj_align_to(value_label, label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_text_font(value_label, ft_font_ttf_20, 0);
    lv_obj_set_style_text_color(value_label, lv_color_black(), 0);
    lv_obj_set_style_text_align(value_label, LV_TEXT_ALIGN_CENTER, 0);
    // draw_border_for_debug(value_label);
    //
    return parent_panel;
}

#endif //CONFIG_XOS_APP_ESL2
