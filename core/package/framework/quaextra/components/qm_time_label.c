
#include "qm_time_label.h"

// extern lv_font_t *ft_font_ttf_20;

lv_obj_t *qm_time_label_init(lv_obj_t *bg_wnd)
{
    lv_obj_t *time_label = lv_label_create(bg_wnd);
    lv_obj_align_to(time_label, bg_wnd, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_color(time_label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(time_label, "00:00:00");
    // lv_obj_set_style_text_font(time_label, ft_font_ttf_20, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(time_label, 5, 0);

    return time_label;
}

void qm_time_label_update(lv_obj_t *time_label, const char *time_str)
{
    if (time_str == NULL || strlen(time_str) == 0) {
        //reset
        lv_label_set_text(time_label, "00:00:00");
        lv_obj_set_style_bg_opa(time_label, LV_OPA_0, 0);
        return;
    }

    lv_label_set_text(time_label, time_str);
    lv_obj_set_style_bg_color(time_label, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_bg_opa(time_label, LV_OPA_100, 0);
}

        