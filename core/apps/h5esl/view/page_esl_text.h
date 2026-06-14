#ifndef __PAGE_ESL_TEXT_H__
#define __PAGE_ESL_TEXT_H__
#include "../utils/hashmap.h"
#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_device_setting.h"
#include "lvgl/lvgl.h"

void handle_text_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int index,char **other_3_arr,int show_count);

#endif // __PAGE_ESL_TEXT_H__