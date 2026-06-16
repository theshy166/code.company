
#ifndef __QUA_XOS_LOADING_H__
#define __QUA_XOS_LOADING_H__

#include "lvgl/lvgl.h"

lv_obj_t * qua_los_show_loading(lv_obj_t *parent, int32_t width, int32_t height);
void qua_los_hide_loading(lv_obj_t *ui_loading);

#endif //__QUA_XOS_LOADING_H__
