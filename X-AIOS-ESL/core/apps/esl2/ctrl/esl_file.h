#ifndef __ESL_FILE_H__
#define __ESL_FILE_H__

#include "../model/esl_base_model.h"
#include "../model/esl_push_server.h"

typedef struct {
    int index;
    esl_base_model_t * model;
    char * device_sn;
} esl_file_save_info2_t;

void save_data_thread(esl_file_save_info2_t *args);

esl_base_model_t * query_esl_base_model_local(int bg_index,char * device_sn);

void *delete_local_data(int bg_index,char * device_sn);

void *delete_local_logo_image(int bg_index);

int save_update(version_info *data);
#endif // __ESL_FILE_H__