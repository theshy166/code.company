#ifndef __ESL_UI_UTILS_H__
#define __ESL_UI_UTILS_H__
#include "../model/esl_base_model.h"
#include "../model/esl_message_model.h"


esl_base_model_t * the_second_to_model(const char * product_template,const char * product_list,const char * other_info_str,const char * tpl_bg_url,int index,int display_product_num);

void free_esl_base_model(esl_base_model_t *model);

esl_base_model_t * parse_esl_base_model(const char * json_str);


//==========消息转化===================
esl_message_model_t * the_second_to_message_model(const char * message);

void free_esl_message_model(esl_message_model_t *message);

esl_message_model_t * parse_esl_message_model(const char * message);

char * els_message_model_to_json(esl_message_model_t * model);
//==========消息转化===================

#endif //__ESL_UI_UTILS_H__
