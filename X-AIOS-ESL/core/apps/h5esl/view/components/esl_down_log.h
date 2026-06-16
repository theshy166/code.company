#ifndef __ESL_DOWN_LOG_H__
#define __ESL_DOWN_LOG_H__

#include "lvgl/lvgl.h"

lv_obj_t * esl_down_log_create(lv_obj_t * parent);
void esl_down_log_update(lv_obj_t * obj, char * file_name);

#endif