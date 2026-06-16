#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL  
#include "lvgl/lvgl.h"
#include "appcommon.h"
#include "appmanager.h"
#include "cadp_aiskill.h"
#include "mqtt_control.h"

extern  AiModule* create_module(AiModuleType type);
AiModule* t_QuaModule = NULL;

int init_mqtt(AiDevInfo* dev) {
   if( t_QuaModule != NULL && t_QuaModule->deinit != NULL ){
       t_QuaModule->deinit();
    }
    LV_LOG_USER("hosturl:%s,port:%d,ac:%s,pw:%s,devsn:%s,clientid:%s,subscriber_topic:%s",dev->hosturl,dev->port,dev->ac,dev->pw,dev->devsn,dev->clientid,dev->subscriber_topic);
    t_QuaModule = create_module( MODULE_QUA );
    t_QuaModule->init(dev);
    return 0;
}

void register_connect_cb(connectCallback callback){
    t_QuaModule->register_conn_respose(callback);
}

void register_repose_cb(respose callback){
    t_QuaModule->register_respose(callback);
}

void destroy_mqtt(void){
    if( t_QuaModule != NULL && t_QuaModule->deinit != NULL ){
      t_QuaModule->deinit();
    }
}

void send_message_to_server(int msg_type,char* msg0 ,char* msg1){
    AiMessage msg = {0};
    msg.val = msg_type;
    msg.data1 =msg0;
    msg.data2 =msg1;
    t_QuaModule->input_content(&msg);
    t_QuaModule->send_cmd(&msg);
}
#endif