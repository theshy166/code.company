
#include "back_button.h"
#include "lvgl/lvgl.h"

#define kSize 50

LV_IMG_DECLARE(icon_float_normal);

lv_obj_t* create_back_button(const lv_img_dsc_t *img_dsc, float_wnd_callback cb)
{
    lv_obj_t *layer_top = lv_layer_sys();

    lv_obj_t *exit_btn = lv_btn_create(layer_top);
    // lv_obj_set_style_radius(exit_btn, kSize, 0);
    #ifdef CONFIG_PRODUCT_STUDYPILOT
    lv_obj_remove_style_all(exit_btn);
    lv_obj_set_size(exit_btn, kSize+20, kSize);
    #else
    lv_obj_set_size(exit_btn, kSize, kSize);
    #endif
    
    lv_obj_set_style_border_width(exit_btn, 0, 0);
    lv_obj_set_style_border_opa(exit_btn, LV_OPA_0, 0);
    // lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_radius(&style, LV_RADIUS_CIRCLE);
    // lv_obj_add_style(exit_btn, &style, 0);
    lv_obj_set_pos(exit_btn, 0, 0);
    // LV_IMG_DECLARE(icon_float_pressing);
    // lv_obj_set_style_bg_img_src(exit_btn, &icon_float_pressing, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(exit_btn, img_dsc == NULL ? &icon_float_normal : img_dsc, LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(exit_btn, lv_color_hex(0x000000), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(exit_btn, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(exit_btn, cb, LV_EVENT_CLICKED, NULL);
    return exit_btn;
}

lv_obj_t* create_back_button_ex(const lv_img_dsc_t *img_dsc, float_wnd_callback cb, back_button_desc_t area)
{
    lv_obj_t *layer_top = lv_layer_sys();

    lv_obj_t *exit_btn = lv_btn_create(layer_top);
    // lv_obj_set_style_radius(exit_btn, kSize, 0);
    int w = area.w;
    int h = area.h;
    lv_obj_remove_style_all(exit_btn);
    #ifdef CONFIG_PRODUCT_STUDYPILOT
    if (w > 0 || h > 0) {
        lv_obj_set_size(exit_btn, w, h);
    } else {
        lv_obj_set_size(exit_btn, kSize+20, kSize);
    }
    #else
    if (w > 0 || h > 0) {
        lv_obj_set_size(exit_btn, w, h);
    } else {
        lv_obj_set_size(exit_btn, kSize, kSize);
    }
    #endif

    lv_obj_set_style_border_width(exit_btn, 0, 0);
    lv_obj_set_style_border_opa(exit_btn, LV_OPA_0, 0);
    // lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_radius(&style, LV_RADIUS_CIRCLE);
    // lv_obj_add_style(exit_btn, &style, 0);
    lv_obj_set_pos(exit_btn, area.x, area.y);
    // LV_IMG_DECLARE(icon_float_pressing);
    // lv_obj_set_style_bg_img_src(exit_btn, &icon_float_pressing, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(exit_btn, img_dsc == NULL ? &icon_float_normal : img_dsc, LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(exit_btn, lv_color_hex(0x000000), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(exit_btn, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(exit_btn, cb, LV_EVENT_CLICKED, NULL);
    return exit_btn;
}
