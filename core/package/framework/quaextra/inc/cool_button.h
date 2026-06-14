
#ifndef COOL_BUTTON_H
#define COOL_BUTTON_H

#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

lv_obj_t *los_zoomable_button(lv_obj_t *parent, const char *btn_name,
        lv_align_t align, lv_coord_t off_x, lv_coord_t off_y, int16_t width, int16_t height);

#endif
