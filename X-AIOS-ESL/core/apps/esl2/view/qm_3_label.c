#include "qm_3_label.h"
#include "qm_lv_obj.h"

#if defined(CONFIG_XOS_APP_ESL2)

extern lv_font_t *ft_font_ttf_30;

void qm_3_label_create(lv_obj_t *parent, int w, int h, three_label_comp_t *qm_3_label_comp,
    const char *label_str, const char *value_str, const char *unit_str, lv_color_t color)
{
    lv_obj_t *parent_panel = qm_lv_obj_create(parent);
    qm_3_label_comp->parent = parent_panel;
    lv_obj_set_size(parent_panel, w, h);
    //
    lv_obj_t *label = lv_label_create(parent_panel);
    lv_label_set_text(label, label_str);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_font(label, ft_font_ttf_30, 0);
    lv_obj_set_style_text_color(label, color, 0);
    qm_3_label_comp->label = label;
    //
    lv_obj_t *value_label = lv_label_create(parent_panel);
    lv_label_set_text(value_label, value_str);
    lv_obj_align_to(value_label, label, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
    lv_obj_set_style_text_font(value_label, ft_font_ttf_30, 0);
    lv_obj_set_style_text_color(value_label, color, 0);
    qm_3_label_comp->value = value_label;
    //
    lv_obj_t *unit_label = lv_label_create(parent_panel);
    lv_label_set_text(unit_label, unit_str);
    lv_obj_align_to(unit_label, value_label, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
    lv_obj_set_style_text_font(unit_label, ft_font_ttf_30, 0);
    lv_obj_set_style_text_color(unit_label, color, 0);
    qm_3_label_comp->unit = unit_label;
}

#endif //CONFIG_XOS_APP_ESL2
