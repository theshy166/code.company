#ifndef _MQTT_CONTROL_H
#define _MQTT_CONTROL_H
#ifdef CONFIG_XOS_FWK_CADP_QUA  
#include "quaMqtt.h"
#include "comInteraction.h"
int init_mqtt(AiDevInfo* dev);
void destroy_mqtt(void);
void register_connect_cb(connectCallback callback);
void register_repose_cb(respose callback);
void send_message_to_server(int msg_type,char* msg ,char* msg1);
void mqtt_input_params_to_init(char* url ,int port,char* ac,char* pw,int proid,char* sn,char* cid);
void get_devinfo(AiDevInfo* dev);
#endif //CONFIG_XOS_FWK_CADP_QUA
#endif //MQTT_CONTROL_H