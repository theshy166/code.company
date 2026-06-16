#ifndef _qm_updater_h_
#define _qm_updater_h_

#include "lvgl/lvgl.h"

void qm_updater_hidden_status(lv_obj_t * obj, bool visible);
void qm_updater_change_img_src(lv_obj_t * imgobj, char *src);
void qm_updater_change_style_bg_img_src(lv_obj_t * imgobj, char *new_src, lv_style_selector_t selector);
void qm_updater_change_text_label(lv_obj_t * textobj, char *new_text);
void qm_updater_change_text_main_color(lv_obj_t *textobj, lv_color_t new_color);
void qm_updater_clean(lv_obj_t *obj);
void qm_updater_opa(lv_obj_t *obj, lv_opa_t opa);
void qm_updater_set_border_width(lv_obj_t *obj, lv_coord_t width, lv_style_selector_t selector);
void qm_updater_change_style_img_recolor_with_opa(lv_obj_t *imgobj, lv_color_t new_color, lv_opa_t opa, lv_style_selector_t selector);
void qm_updater_set_obj_recolor_white(lv_obj_t *obj, bool recolor);
//text long mode
void qm_updater_set_text_long_mode(lv_obj_t *textobj, lv_label_long_mode_t mode);

#endif //_qm_updater_h_
