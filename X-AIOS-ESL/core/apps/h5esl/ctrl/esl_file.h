#ifndef __ESL_FILE_H__
#define __ESL_FILE_H__

#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_push_server.h"
#include "../model/esl_device_setting.h"

typedef struct {
    int index;
    esl_show_model_t *model;
    esl_product_info_t **product_info_list;
    esl_device_setting_t *device_setting;
    int product_info_count;
    char * device_sn;
} esl_file_save_info_t;

void *save_data_thread(void *arg);

esl_product_info_t ** query_product_infos_local(int bg_index,char * device_sn,int * count);

esl_show_model_t * query_product_template_local(int bg_index,char * device_sn);

esl_device_setting_t * query_device_setting_local(int bg_index,char * device_sn);

void *delete_local_data(int bg_index,char * device_sn);

void *delete_local_logo_image(int bg_index);

int save_update(esl_push_server_data *data);
#endif // __ESL_FILE_H__