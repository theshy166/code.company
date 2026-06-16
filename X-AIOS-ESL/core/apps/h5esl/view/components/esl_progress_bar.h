
#ifndef __ESL_PROGRESS_BAR_H__
#define __ESL_PROGRESS_BAR_H__

#include "lvgl/lvgl.h"

lv_obj_t * create_progress_bar(lv_obj_t * parent);

void update_progress_bar(lv_obj_t * progress_bar, int value);
void destroy_progress_bar(lv_obj_t * progress_bar);

#endif