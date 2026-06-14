
#ifndef _QUA_XOS_GRAD_ARC_H_
#define _QUA_XOS_GRAD_ARC_H_


#include "xos_app_conf.h"
#include "lvgl/lvgl.h"

//
lv_obj_t * qua_los_grad_arc_common(lv_obj_t *parent, int16_t size, int32_t inner_range,
    lv_color_t bg_color, lv_align_t align_pos, bool hasGap,
    lv_color_t start_color, lv_color_t end_color);
//
void qua_los_grad_arc_update(lv_obj_t *meter, int16_t inner_range, int16_t percent);


#endif // !_QUA_XOS_GRAD_ARC_H_
