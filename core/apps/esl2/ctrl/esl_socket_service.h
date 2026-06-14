#ifndef __ESL_SOCKET_SERVICE_H__
#define __ESL_SOCKET_SERVICE_H__
#include "cJSON.h"
#include "../model/esl_message_model.h"

struct esl_socket_t{
    char * key;
    char * device_host;
};

int create_listen_socket();

int create_send_socket(const char * device_no,const char *message);

int create_send_socket_slave(const char * ip,const char * message);

void* check_life_thread(void *arg);

void * cancel_current_listen_socket();

int reconnect_socket_listener();

void put_group_map(char * key,char *value,int index);

void init_group_map();

void init_push_id_map();

void post_push_message(const char * type,cJSON* json);

void post_push_message2(const char * type,esl_message_model_t * message_model);

#endif//__ESL_SOCKET_SERVICE_H__