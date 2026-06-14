#ifndef __ESL_JSON_UTILS_H__
#define __ESL_JSON_UTILS_H__
#include "cJSON.h"
#include "../model/esl_other_info.h"
#include "../model/esl_product_info.h"

esl_product_info_t ** parse_product_list_second(const char *json_string,int * count);

esl_other_info_t *parse_other_info_second(const char *json_string);

void free_esl_product_info_array(esl_product_info_t **product_info_list, int count);

void free_esl_other_info(esl_other_info_t *info);

#endif // __ESL_JSON_UTILS_H__