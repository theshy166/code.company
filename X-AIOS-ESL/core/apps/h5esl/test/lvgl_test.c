#include "lvgl_test.h"
#include "lvgl/lvgl.h"
#include "../utils/pic_utils.h"

void lvgl_test(void)
{
    lv_obj_t *active_screen = lv_scr_act();
    lv_obj_set_style_bg_opa(active_screen, LV_OPA_0, 0);
    lv_obj_t *bg = lv_obj_create(active_screen);
    lv_obj_remove_style_all(bg);
    //lv_obj_set_style_bg_opa(bg, LV_OPA_0, 0);
    //lv_obj_set_style_pad_all(bg, 0, 0);
    lv_color_t bgColor3 = lv_color_hex(hex_to_uint32("#000000"));
    lv_obj_set_style_bg_color(bg,bgColor3, 0);
    lv_obj_set_size(bg, 400, 400);
    draw_border_for_debug(bg);
    
    lv_obj_t *rect = lv_obj_create(bg);
    lv_obj_set_size(rect, 2, 400);
    lv_obj_set_pos(rect, 200, 0);
    lv_color_t bgColor = lv_color_hex(hex_to_uint32("#333333"));
    lv_obj_set_style_bg_color(rect, bgColor, 0);
    lv_obj_set_style_radius(rect, 1, 0);
    /* lv_obj_set_style_transform_angle(rect, 90.27 * 10, 0);
    lv_obj_set_style_transform_pivot_y(rect, 100, 0);
    lv_obj_set_style_transform_pivot_x(rect, 100, 0); */
    //draw_border_for_debug(rect);

    lv_obj_t *rect1 = lv_obj_create(bg);
    lv_obj_set_size(rect1, 2, 400);
    lv_obj_set_pos(rect1, 200, 0);
    /* lv_color_t bgColor1 = lv_color_hex(hex_to_uint32("#FFFFFF"));
    lv_obj_set_style_bg_color(rect1, bgColor1, 0); */
    lv_obj_set_style_radius(rect1, 1, 0);
    lv_obj_set_style_transform_angle(rect1, 90 * 10, 0);
    lv_obj_set_style_transform_pivot_y(rect1, 200, 0);
    /* lv_obj_set_style_transform_pivot_y(rect1, 100, 0);
    lv_obj_set_style_transform_pivot_x(rect1, 100, 0); */
    //draw_border_for_debug(rect1);
}