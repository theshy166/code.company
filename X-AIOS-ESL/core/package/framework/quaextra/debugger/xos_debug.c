#include "xos_debug.h"
#include <stdio.h>

bool g_show_abs_pos_flag = 1;  //default show abs pos

static lv_coord_t lv_obj_get_absolute_x(const lv_obj_t *obj)
{
    lv_coord_t absolute_x = lv_obj_get_x(obj);
    const lv_obj_t *parent = lv_obj_get_parent(obj);
    while (parent!= NULL)
    {
        absolute_x += lv_obj_get_x(parent);
        parent = lv_obj_get_parent(parent);
    }
    return absolute_x;
}

static lv_coord_t lv_obj_get_absolute_y(const lv_obj_t *obj)
{
    lv_coord_t absolute_y = lv_obj_get_y(obj);
    const lv_obj_t *parent = lv_obj_get_parent(obj);
    while (parent!= NULL)
    {
        absolute_y += lv_obj_get_y(parent);
        parent = lv_obj_get_parent(parent);
    }
    return absolute_y;
}

void draw_border_show_abs_pos(bool flag) {
    g_show_abs_pos_flag = flag;
}

void draw_border_for_debug(lv_obj_t *obj)
{
    #if DEBUG_BORDER
    char coords_text[32];
    lv_obj_set_style_border_color(obj, lv_color_hex(0x00ff00), 0);
    lv_obj_set_style_border_width(obj, 1, 0);

    lv_obj_t *label = lv_label_create(lv_layer_top()); //label diplay at top
    lv_obj_set_style_text_color(label, lv_color_hex(0x00ff00), 0);

    lv_refr_now(NULL); //will decide obj pos at once
    lv_obj_align_to(label, obj, LV_ALIGN_TOP_LEFT, 2, 2);
    if (g_show_abs_pos_flag) {
        snprintf(coords_text, sizeof(coords_text), "x:%d y:%d w:%d h:%d", lv_obj_get_absolute_x(obj), lv_obj_get_absolute_y(obj), lv_obj_get_width(obj), lv_obj_get_height(obj));
    } else {
        snprintf(coords_text, sizeof(coords_text), "x:%d y:%d w:%d h:%d", lv_obj_get_x(obj), lv_obj_get_y(obj), lv_obj_get_width(obj), lv_obj_get_height(obj));
    }
    lv_label_set_text(label, coords_text);
    #endif
}
