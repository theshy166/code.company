#ifndef QUA_MQTT_H
#define QUA_MQTT_H
#include <stdio.h>  
#include <string.h> // 为了使用strcpy等字符串处理函数  
#include "cadp_aiskill.h"
#include <pthread.h>  

 
typedef struct PbNode {  
    char* str1;  
    char* str2;  
    struct PbNode* next;  
} PbNode;  
  
typedef struct {  
    PbNode* front;  
    PbNode* rear;  
    pthread_mutex_t lock;  
    int size;  
} PbQueue;  
  

typedef struct MqttInfo
{
	char* deviceHost;
	int  deviceHostPort;
	int  deviceType;
	char* deviceCode;
	char* clientId;
	char* mqttAcc;
	char* mqttPwd;
	char appPhone[64];
	char* subscriber_topic;
	char* publish_topic;
	int  qos;
	pthread_t recv_thread;

	pthread_t pub_thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond ;

	AiNeTType net_state;
	int toStopPub;
	int toStopRecv;
	int toPubReady;

	
	void (*respose)(const AiMessage* params); 
    void (*connectCallback)(const AiMessage* params); 

	PbQueue pubQ;
} MqttInfo;
#endif