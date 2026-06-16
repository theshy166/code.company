
#include "qm_row_container.h"
#include "qm_lv_obj.h"

lv_obj_t *qm_row_container_create(lv_obj_t *super_obj, lv_flex_flow_t flow, lv_flex_align_t main, lv_flex_align_t cross)
{
    lv_obj_t *container = qm_lv_obj_create(super_obj);
    lv_obj_remove_style_all(container);
    // lv_obj_set_size(container, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    // lv_obj_set_pos(container, area->x1, area->y1);
    lv_obj_set_flex_flow(container, flow);
    lv_obj_set_flex_align(container, main, cross, LV_FLEX_ALIGN_END);
    lv_obj_set_style_pad_column(container, 4, LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(container, lv_color_hex(0x000000), 0);
    // lv_obj_set_style_bg_opa(container, 255, 0);
    return container;
}
