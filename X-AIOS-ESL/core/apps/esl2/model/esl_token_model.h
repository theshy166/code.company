#ifndef __ESL_TOKEN_MODEL_H__
#define __ESL_TOKEN_MODEL_H__

typedef struct esl_token_model{
    int error_code;
    char * token;
    char * store_code;
    int cmd_version[2];
    char * client_id;
    char * mqtt_host;
    int mqtt_port;
    char * mqtt_pub_topics;
    char * mqtt_sub_topics;
    int mqtt_version[2];
} esl_token_model_t;

#endif //__ESL_TOKEN_MODEL_H__