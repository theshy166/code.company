

#include "qm_lv_obj.h"

lv_obj_t *qm_lv_obj_create(lv_obj_t *parent)
{
    // Create the statusbar container
    lv_obj_t *panel = lv_obj_create(parent);
    lv_obj_set_scroll_dir(panel, LV_DIR_NONE);
    lv_obj_set_style_pad_left(panel, 0 , 0);
    lv_obj_set_style_pad_right(panel, 0 , 0);
    lv_obj_set_style_pad_top(panel, 0 , 0);
    lv_obj_set_style_pad_bottom(panel, 0 , 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_radius(panel, 0, 0);
    return panel;
}
