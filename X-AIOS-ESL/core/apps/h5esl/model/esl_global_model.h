#ifndef __ESL_GLOBAL_MODEL_H__
#define __ESL_GLOBAL_MODEL_H__
#include "esl_show_model.h"
#include "esl_product_info.h"
#include "esl_device_setting.h"
#include "lvgl/lvgl.h"

typedef struct {
    char * ip;

    esl_product_info_t ** product_info_list_first;
    esl_show_model_t * product_model_first;
    esl_device_setting_t * device_setting_first;
    lv_obj_t * bg_first;
    lv_obj_t * top_first;
    int product_count_first;
    char * device_sn_first;

    esl_product_info_t ** product_info_list_second;
    esl_show_model_t * product_model_second;
    esl_device_setting_t * device_setting_second;
    lv_obj_t * bg_second;
    lv_obj_t * top_second;
    int product_count_second;
    char * device_sn_second;

    char * master_device_sn;
} esl_global_model_t;
#endif // __ESL_GLOBAL_MODEL_H__