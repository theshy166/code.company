#ifndef _MQTT_SUBSCRIBER_H
#define _MQTT_SUBSCRIBER_H
#ifdef CONFIG_XOS_FWK_CADP_CURL
#ifdef CONFIG_XOS_FWK_CADP_QUA 
#include "cadp_aiskill.h"
typedef struct 
{
    int responseType;
    char *imageName;
} LikeImage;

void mqtt_subscriber(const AiMessage* params);

void mqtt_connect_response(const AiMessage* params);
#endif //CONFIG_XOS_FWK_CADP_QUA
#endif //CONFIG_XOS_FWK_CADP_CURL
#endif //MQTT_SUBSCRIBER_H