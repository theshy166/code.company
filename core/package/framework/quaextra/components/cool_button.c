
#include "cool_button.h"
#include "lv_conf.h"

lv_obj_t* los_zoomable_button(lv_obj_t *parent, const char *btn_name,
        lv_align_t align, lv_coord_t off_x, lv_coord_t off_y, int16_t width, int16_t height)
{
    static lv_style_prop_t props[] = {
        LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT, LV_STYLE_TEXT_LETTER_SPACE, 0
    };

    static lv_style_transition_dsc_t transition_dsc_def;
    lv_style_transition_dsc_init(&transition_dsc_def, props, lv_anim_path_overshoot, 250, 100, NULL);

    static lv_style_transition_dsc_t transition_dsc_pr;
    lv_style_transition_dsc_init(&transition_dsc_pr, props, lv_anim_path_ease_in_out, 250, 0, NULL);

    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &transition_dsc_def);

    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 10);
    lv_style_set_transform_height(&style_pr, 10);
    lv_style_set_text_letter_space(&style_pr, 10);
    lv_style_set_transition(&style_pr, &transition_dsc_pr);

    lv_obj_t *btn1 = lv_btn_create(parent? parent:lv_scr_act());
    lv_obj_align(btn1, align, off_x, off_y);
    if (width != -1 && height != -1) {
        lv_obj_set_size(btn1, width, height);
    } else if (width != -1) {
        lv_obj_set_width(btn1, width);
    } else if (height != -1) {
        lv_obj_set_height(btn1, height);
    }
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn1, &style_def, 0);

    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, btn_name);
    lv_obj_center(label);
    return btn1;
}
