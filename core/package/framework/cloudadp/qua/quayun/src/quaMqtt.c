#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "quaMqtt.h"
#include "MQTTClient.h"
#include "comInteraction.h"
#include "lvgl/lvgl.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include "MQTTLinux.h"
#include "MQTTPacket.h"
#include <sys/prctl.h>
#include <pthread.h>  
#include "cadp_aiskill.h"
#include "cadp_aiskill_template.h"

#define MQTT_SEND_BUFF_LEN       (2048)
#if defined(CONFIG_XOS_USE_APP_NATIVE_ESL) || defined(CONFIG_XOS_APP_ESL2)
	#define MQTT_RECV_BUFF_LEN       (2048 * 20*10)
#else	
	#define MQTT_RECV_BUFF_LEN       (2048)
#endif
#ifdef CONFIG_XOS_FWK_QUAAITALK_RTC
#define MQTT_TOPIC_LEN       (1000)
#else //CONFIG_XOS_FWK_QUAAITALK_RTC
#define MQTT_TOPIC_LEN       (200)
#endif //CONFIG_XOS_FWK_QUAAITALK_RTC
#define MQTT_PAYLOAD_LEN       (1024)
#define MQTT_STRING_LEN       (50)
#define MQTT_PWD_STRING_LEN       (100)

#ifdef CONFIG_XOS_USE_APP_MOMO_TALK
#define MQTT_TOPIC_LEN       (1000)
#define MQTT_SEND_BUFF_LEN       (2048*500)
#define MQTT_PAYLOAD_LEN         (1024*1024)
#define MQTT_RECV_BUFF_LEN       (2048*1024)
#endif

extern void queue_init(PbQueue* q);
extern void queue_destroy(PbQueue* q);
extern void queue_enqueue(PbQueue* q, const char* str1, const char* str2);
extern int  queue_dequeue(PbQueue* q,char* out_str1,char* out_str2);

static MQTTClient g_mqttClient = { 0 };
static Network g_network = { 0 };
static MQTTPacket_connectData g_connectData = MQTTPacket_connectData_initializer;

unsigned char* g_mqttSendbuf = NULL;
unsigned char* g_mqttReadbuf = NULL;
unsigned char* g_mqttReadMessageData = NULL;
unsigned char* g_mqttReadMessageTopic = NULL;
MqttInfo g_MqttInfo = {0};

void* qua_mqtt_entry_recv(void *arg);
void* qua_mqtt_entry_pub(void *arg);
pthread_t create_mqtt_recv_thread();
pthread_t create_mqtt_pub_thread();

static void updateNetStatus(AiNeTType status){
	AiMessage msg = { 0 };
	msg.val = status;
	g_MqttInfo.net_state = status; 
	
	LV_LOG_WARN("updateNetStatus %d\n",status);
	if(g_MqttInfo.connectCallback != NULL ){
			g_MqttInfo.connectCallback(&msg);	
	}

}


static void mqttInit(const char* pstrClientId, const char* pstrUserName, const char* pstrPassword, unsigned short nkeepAliveInterval)
{
	g_connectData.MQTTVersion = 4;
	g_connectData.clientID.cstring = pstrClientId;
	g_connectData.username.cstring = pstrUserName;
	g_connectData.password.cstring = pstrPassword;
	g_connectData.keepAliveInterval = nkeepAliveInterval;//120;
	//cleansession设置为0的客户端在重新连接后会收到所有在它连接断开期间发布的QoS1和QoS2级别的消息
	g_connectData.cleansession = 0;
	printf("mqttInit pstrClientId=%s pstrUserName=%s pstrPassword=%s", pstrClientId, pstrUserName, pstrPassword);
}

static void messageArrived(MessageData* data)
{
	#if !defined(CONFIG_XOS_USE_APP_MOMO_TALK)
	#if !defined(CONFIG_XOS_FWK_QUAAITALK_RTC) //not log for rtc, it's too much
	LV_LOG_WARN("topic=%d Data=%d Qos=%d\n", data->topicName->lenstring.len, data->message->payloadlen, data->message->qos);
	#endif //not CONFIG_XOS_FWK_QUAAITALK_RTC
	if( NULL != g_MqttInfo.respose ){

		char* topic  = malloc(256);
		char* payload  = malloc( MQTT_RECV_BUFF_LEN);
		memset(payload,0,MQTT_RECV_BUFF_LEN);
		memset(topic,0,256);
		memcpy(payload, data->message->payload, data->message->payloadlen);
		memcpy(topic, data->topicName->lenstring.data, data->topicName->lenstring.len + 1);
		
		AiMessage msg = { 0 };
		msg.data1 = topic;
		msg.data2 = payload;
		g_MqttInfo.respose(&msg);	

		free( topic );
		free( payload );

	}
	#else
		if( NULL != g_MqttInfo.respose ){
			AiMessage msg = { 0 };
			msg.data1 = data->topicName->lenstring.data;
			msg.data2 = data->message->payload;
			g_MqttInfo.respose(&msg);	
		}
	#endif		

}

pid_t get_mqtt_tid(void)
{
	return (pid_t)syscall(SYS_gettid);
}


void malloc_all(){

	if( g_mqttSendbuf == NULL ){
		g_mqttSendbuf = (char*)malloc(MQTT_SEND_BUFF_LEN);	
	}

	if( g_mqttReadbuf == NULL ){
		g_mqttReadbuf = (char*)malloc(MQTT_RECV_BUFF_LEN);	
	}	

    if( g_mqttReadMessageData == NULL ){
		g_mqttReadMessageData = (char*)malloc(MQTT_PAYLOAD_LEN);	
	}

	if( g_mqttReadMessageTopic == NULL ){
		g_mqttReadMessageTopic = (char*)malloc(MQTT_TOPIC_LEN);	
	}
	if(g_MqttInfo.clientId == NULL && g_MqttInfo.deviceHost== NULL && 
	g_MqttInfo.deviceCode==NULL&& g_MqttInfo.mqttAcc==NULL&&g_MqttInfo.mqttPwd==NULL)
	{
		g_MqttInfo.clientId = (char*)malloc(MQTT_STRING_LEN);
		g_MqttInfo.deviceHost = (char*)malloc(MQTT_STRING_LEN);
		g_MqttInfo.deviceCode = (char*)malloc(MQTT_STRING_LEN);
		g_MqttInfo.mqttAcc = (char*)malloc(MQTT_STRING_LEN);
		g_MqttInfo.mqttPwd = (char*)malloc(MQTT_PWD_STRING_LEN);		
	}
	if(g_MqttInfo.subscriber_topic == NULL){
		g_MqttInfo.subscriber_topic = (char*)malloc(MQTT_TOPIC_LEN);
	}
}

void free_all(){

	if( NULL != g_mqttSendbuf){
		free(g_mqttSendbuf);
		g_mqttSendbuf = NULL;
	}

	if( NULL != g_mqttReadbuf ){
		free(g_mqttReadbuf);
		g_mqttReadbuf = NULL;
	}	

	if( NULL != g_mqttReadMessageData ){
		free(g_mqttReadMessageData);
		g_mqttReadMessageData = NULL;
	}

	if( NULL != g_mqttReadMessageTopic ){
		free(g_mqttReadMessageTopic);
		g_mqttReadMessageTopic = NULL;
	}	

	if(g_MqttInfo.clientId != NULL && g_MqttInfo.deviceHost!= NULL && 
	g_MqttInfo.deviceCode!=NULL&& g_MqttInfo.mqttAcc!=NULL&&g_MqttInfo.mqttPwd!=NULL){
		free(g_MqttInfo.clientId);
		free(g_MqttInfo.deviceHost);
		free(g_MqttInfo.deviceCode);
		free(g_MqttInfo.mqttAcc);
		free(g_MqttInfo.mqttPwd);
	}
	if(g_MqttInfo.subscriber_topic != NULL){
		free(g_MqttInfo.subscriber_topic);
		g_MqttInfo.subscriber_topic = NULL;
	}

}

void setMqttParams(AiDevInfo* devinfo ){

	printf("setMqttParams\n");
	printf("product_id:%d\n",devinfo->product_id);
	printf("clientid:%s\n",devinfo->clientid);
	printf("devsn:%s\n",devinfo->devsn);
	//printf("hosturl:%s\n",devinfo->hosturl);
	//printf("ac:%s\n",devinfo->ac);
	//printf("pw:%s\n",devinfo->pw);
	//printf("port:%d\n",devinfo->port);
	printf("appPhone:%s\n",devinfo->appPhone);
	if(devinfo->subscriber_topic){
		printf("topic:%s\n",devinfo->subscriber_topic);
	}

	g_MqttInfo.deviceType = devinfo->product_id;
	strcpy(g_MqttInfo.clientId, devinfo->clientid);
	strcpy(g_MqttInfo.deviceCode, devinfo->devsn);	
	strcpy(g_MqttInfo.deviceHost, devinfo->hosturl);
	strcpy(g_MqttInfo.mqttAcc, devinfo->ac);	
	strcpy(g_MqttInfo.mqttPwd, devinfo->pw);
	strcpy(g_MqttInfo.appPhone, devinfo->appPhone[0] != '\0' ? devinfo->appPhone: "");
	if(devinfo->subscriber_topic){
		strcpy(g_MqttInfo.subscriber_topic, devinfo->subscriber_topic);
	}else{
		strcpy(g_MqttInfo.subscriber_topic, "");
	}

	g_MqttInfo.deviceHostPort = devinfo->port;

}


void qua_init( AiDevInfo* devinfo ){
	malloc_all();
	LV_LOG_WARN("--qua_prepare_to_publish---\n");
	queue_init(&g_MqttInfo.pubQ);
	setMqttParams( devinfo );
	mqttInit(g_MqttInfo.clientId, g_MqttInfo.mqttAcc, g_MqttInfo.mqttPwd, 30);
	//g_MqttInfo.mutex = PTHREAD_MUTEX_INITIALIZER;
	//g_MqttInfo.cond = PTHREAD_COND_INITIALIZER;
	create_mqtt_recv_thread();
	create_mqtt_pub_thread();
}


void qua_deinit(){
	free_all();
	LV_LOG_WARN("--qua_deinit--start-\n");
	g_MqttInfo.toStopPub = 1;
	pthread_cond_signal(&g_MqttInfo.cond);
	g_MqttInfo.toStopRecv = 1;
	queue_destroy(&g_MqttInfo.pubQ);
	memset(&g_MqttInfo,0x00,sizeof(MqttInfo));
	LV_LOG_WARN("--qua_deinit--==%d \n",sizeof(g_MqttInfo));
}



void qua_register_respose(respose callback){
	g_MqttInfo.respose = callback;
}


void qua_register_connect_callback(connectCallback callback){
	
	g_MqttInfo.connectCallback = callback;

}


int qua_prepare_to_publish(AiMessage* msg){

	char* topic;

	printf("--qua_prepare_to_publish---\n");

	if( NULL == msg ){
		return -1;
	}
	if(msg->topic){
		topic = msg->topic;
	}else{
		switch( msg->val ){
			case 0:
			topic = getPublishDeviceInformation();
			break;
			case 1:
			topic = getPublishDeviceProperty();
			break;
			
			case 2:
			topic = getPublishDeviceFunction();
			break;

			case 3:
			topic = getPublishDeviceEvent();
			break;		
			case 5:
			topic = "fromDevice";
			break;
			case 6:
			topic = getIpcHartInfo();
			break;
		}
	}
	

	//printf("--queue_enqueue %s %s---\n",topic,msg->data1);
	queue_enqueue(&g_MqttInfo.pubQ, topic, msg->data1);  
}

int qua_publish_now(AiMessage* msg){
	g_MqttInfo.toPubReady = 1;
	pthread_cond_signal(&g_MqttInfo.cond);
}


pthread_t create_mqtt_recv_thread(){
	pthread_t tid2;
    pthread_create(&tid2, NULL, qua_mqtt_entry_recv, NULL);
	return tid2;
}

pthread_t create_mqtt_pub_thread(){
	pthread_t tid;
    int ret = pthread_create(&tid, NULL, qua_mqtt_entry_pub, NULL);
	if(ret != 0 ){
			
	}
	return tid;
}


void* qua_mqtt_entry_recv(void *arg)
{
	prctl(PR_SET_NAME, "mqtt_recv");
	mqtt_connect_now();
	LV_LOG_WARN("qua_mqtt_entry_recv  thread  exit now %d\n",get_mqtt_tid());
    return NULL;
}


void* qua_mqtt_entry_pub(void *arg){

	int connect_try = 2;
	g_MqttInfo.toStopPub = 0;
	g_MqttInfo.toPubReady = 0;
	#ifdef CONFIG_XOS_USE_APP_MOMO_TALK
	char topic[256] ={0}, payload[MQTT_SEND_BUFF_LEN]={0};
	#elif defined(CONFIG_XOS_FWK_QUAAITALK_RTC)
	char topic[128] ={0}, payload[1024]={0};
	#else //
	char topic[50] ={0}, payload[1024]={0};
	#endif

	LV_LOG_WARN("--start qua_mqtt_entry_pub---\n");
	prctl(PR_SET_NAME, "mqtt_pub");
	
wait_net:
	if( g_MqttInfo.net_state < CONNECTED  &&  g_MqttInfo.toStopPub == 0 ){
		sleep(connect_try);
		//updateNetStatus(CONNECTING);
		goto wait_net;
	}

	while (!g_MqttInfo.toStopPub) { // 常驻循环  
	
       pthread_mutex_lock(&g_MqttInfo.mutex);  
       while (!g_MqttInfo.toPubReady && g_MqttInfo.pubQ.size == 0 ) {  
           pthread_cond_wait(&g_MqttInfo.cond, &g_MqttInfo.mutex);  
		   if(g_MqttInfo.toStopPub){
				pthread_mutex_unlock( &g_MqttInfo.mutex);
				goto end;
		   }
        }  
        g_MqttInfo.toPubReady = 0;  
		memset(topic,0,sizeof(topic));
		memset(payload,0,sizeof(payload));
 		int ret = queue_dequeue(&g_MqttInfo.pubQ,topic,payload); 
		if( ret < 0){
			sleep(3);
			pthread_mutex_unlock( &g_MqttInfo.mutex);  
			continue;
		}
		quaMqttSendData(topic,payload,0);
        pthread_mutex_unlock( &g_MqttInfo.mutex);  
    }  

	end:
	updateNetStatus(DISCONNECTED);
	LV_LOG_WARN("--qqua_mqtt_entry_pub end-\n");
}


void quaMqttSendData(const char* pstrTopic, const char* pstrMessage, unsigned char nQs)
{
	int nResult = 0;
	if (MQTTIsConnected(&g_mqttClient) != 0 ){
		MQTTMessage message;
		message.qos = nQs;
		message.retained = 0;
		message.payload = pstrMessage;
		message.payloadlen = strlen(pstrMessage);
		printf("--mqttSendData 5 topic l=%s \n", pstrTopic);
		printf("--mqttSendData 5 l=%d \n", message.payloadlen);
		if (nResult = MQTTPublish(&g_mqttClient, pstrTopic, &message) != 0)
			printf("MQTTPublish %d\n", nResult);
		//printf("--mqttSendData Topic=%s Message=%s  ,nResult=%d\n", pstrTopic, pstrMessage,nResult);
	}else{
		LV_LOG_WARN("--quaMqttSendData  fail \n");
	}
}

int mqtt_connect_now(){

	int connect_try = 2;

_mqtt_start:
	g_MqttInfo.toStopRecv = 0;
	updateNetStatus(CONNECTING);
	NetworkInit(&g_network);
	MQTTClientInit(&g_mqttClient, &g_network, 40000, g_mqttSendbuf, MQTT_SEND_BUFF_LEN, g_mqttReadbuf, MQTT_RECV_BUFF_LEN);
	printf("MQTT_RECV_BUFF_LEN : %d" ,MQTT_RECV_BUFF_LEN);
	
	printf("---start NetworkConnect--host=%s,port=%d",g_MqttInfo.deviceHost,g_MqttInfo.deviceHostPort);

re_conn:
	if ((NetworkConnect(&g_network, g_MqttInfo.deviceHost, g_MqttInfo.deviceHostPort)) != 0)
	{
		LV_LOG_WARN("MQTT NetworkConnect fail");
		g_mqttClient.keepAliveInterval = g_connectData.keepAliveInterval;
		g_mqttClient.ping_outstanding = 1;
		if(NULL != g_MqttInfo.connectCallback){
			updateNetStatus(CONNECTING);
		}
		sleep(connect_try);
		goto re_conn;
	}

    g_MqttInfo.net_state = 2; 
	LV_LOG_WARN("NetworkConnect success\n");
	if ((MQTTConnect(&g_mqttClient, &g_connectData)) != 0)
	{	
		LV_LOG_WARN("MQTTConnect fail\n");
		g_mqttClient.ping_outstanding = 1;
		if(NULL != g_MqttInfo.connectCallback){
			updateNetStatus(DISCONNECTED);
		}
		goto _mqtt_restart;
	}	

	g_MqttInfo.net_state = 3; 
	printf("--start MQTTSubscribe---\n");
	int nResult = 0;	
	int ret = 0;	
	char* pstrSubscriberAll = NULL;
	if( g_MqttInfo.subscriber_topic != NULL && strcmp(g_MqttInfo.subscriber_topic,"") != 0){
		pstrSubscriberAll = g_MqttInfo.subscriber_topic;
	}else{
		pstrSubscriberAll = getSubscriberAll();
	}
	
	
	printf("--start MQTTSubscribe--%s-\n",pstrSubscriberAll);

	char topic_x[MQTT_TOPIC_LEN] = {0};
	strncpy(topic_x, pstrSubscriberAll, MQTT_TOPIC_LEN - 1);
	topic_x[MQTT_TOPIC_LEN - 1] = '\0';
	for (char* pstr = strtok(topic_x, "|"); pstr != NULL; pstr = strtok(NULL, "|"))
	{
		LV_LOG_WARN("---MQTTSubscribe -%s--\n", pstr);
		
		if ((nResult = MQTTSubscribe(&g_mqttClient, pstr, 0, messageArrived)) != 0)
		{
			LV_LOG_WARN("pstr -%s- fail-\n", pstr);
			ret = -1;
		}else{
			ret = 0;
		}
	}
	printf("--end MQTTSubscribe---");
	if( ret == 0){
		updateNetStatus(CONNECTED);
	}else{
		updateNetStatus(DISCONNECTED);
		LV_LOG_WARN("--MQTTSubscribe DISCONNECTED--");
		goto _mqtt_restart;
	}
	
	// need callback to notice connect success

	while (!g_MqttInfo.toStopRecv)
	{
		int srt	= MQTTYield(&g_mqttClient, 10 * 1000);
		if( srt != 0 ){
			LV_LOG_WARN("MQTTYield!  ret = %d\n",srt);
			goto _mqtt_restart;
		}
	}
	
	LV_LOG_WARN("Stopping\n");


_mqtt_restart:

    NetworkDisconnect(&g_network);
    LV_LOG_WARN("restart!\n");
    goto _mqtt_start;

	end:
	MQTTDisconnect(&g_mqttClient);
	NetworkDisconnect(&g_network);	
	LV_LOG_WARN("--mqtt_connect_now end-\n");
	return 0;
}


AiModule g_QuaCMoudule = {
    .init = qua_init,
    .deinit = qua_deinit,
    .register_respose = qua_register_respose,
    .register_conn_respose = qua_register_connect_callback,
    .send_cmd = qua_publish_now,
    .input_content = qua_prepare_to_publish
};



