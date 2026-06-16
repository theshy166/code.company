
#include "qua_los_loading.h"
#include "lv_conf.h"

//===================
static void lv_arc_set_rotation_angle(lv_obj_t *arc, int32_t angle)
{
    lv_arc_set_angles(arc, angle, angle+270);
}

void qua_los_hide_loading(lv_obj_t *ui_loading)
{
    lv_anim_del_all();//must
    lv_obj_del(ui_loading);
}

lv_obj_t * qua_los_show_loading(lv_obj_t *parent, int32_t width, int32_t height)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_arc_width(&style, 10);
    lv_style_set_arc_color(&style, lv_color_hex(0xbb6666));//lite pink
    // lv_style_set_arc_color(&style, lv_color_hex(0x8822dd));//lite purple
    // lv_style_set_arc_color(&style, lv_color_hex(0xdd2288));//lite red

    //the background is hidden for arc ui control.

    // Create an arc object
    lv_obj_t * arc = lv_arc_create(parent != NULL ? parent : lv_layer_top());
    lv_obj_add_style(arc, &style, LV_PART_INDICATOR);

    //
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_remove_style(arc, NULL, LV_PART_MAIN);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    // lv_arc_set_rotation(arc, 90);
    lv_obj_set_size(arc, width, height); // Set the desired size for the spinner
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0); // Align the spinner to the center of the parent
    //
    // Create an animation to rotate the spinner
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, arc);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)&lv_arc_set_rotation_angle);
    lv_anim_set_values(&a, 0, 360);
    lv_anim_set_time(&a, 1000); // 1 second for a full rotation
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // Repeat the animation indefinitely
    lv_anim_start(&a); // Start the animation
    return arc;
}
