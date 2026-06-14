#ifdef CONFIG_XOS_APP_ESL2
#include "lvgl/lvgl.h"
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <pthread.h>
#include "cJSON.h"
#include "mqtt_subscriber.h"
#include "mqtt_control.h"
#include "serial_number.h"
#include "utils.h"
#include "esl_socket_service.h"
#include "esl_socket_udp_service.h"
#include "esl_service.h"
#include "esl_file.h"
#include "../view/page_esl.h"
#include "../utils/esl_sign.h"
#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_device_setting.h"
#include "generated/autoconf.h"
#include "../conf/conf.h"
#include "esl_message_queue.h"
#include "../utils/esl_json_utils.h"
#include "../utils/esl_ui_utils.h"
#include "uirender.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "qm_wifi_cfg.h"
#include "logo_img.h"
#include "generated/autoconf.h"
#endif

#ifndef CONFIG_QM_VERSION
#define CONFIG_QM_VERSION "V1.0.0"
#endif

#ifndef CONFIG_PLATFORM_ID
#define CONFIG_PLATFORM_ID "ESL10D"
#endif

static int is_empty(const char *str) {
    return *str == '\0';
}

static time_t last_message_time_1 = 0;
static time_t last_message_time_2 = 0;

static pthread_mutex_t s_mqtt_subscriber_mutex_1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_mqtt_subscriber_mutex_2 = PTHREAD_MUTEX_INITIALIZER;

//mqtt 订阅回调函数
void mqtt_subscriber(const AiMessage* params)
{
    QM_ESL2_LOG("mqtt_subscriber net state =%d \n",params->val );
    QM_ESL2_LOG("mqtt_subscriber message =%s \n",params->data2 );
    char *message = params->data2;
    esl_message_model_t * message_model = the_second_to_message_model(message);
    if(message_model == NULL){
        QM_ESL2_LOG("mqtt_subscriber convert dto fail!\n");
        return;
    }
    char *deviceNo = message_model -> device_no;
    int index = 0;
    if(ends_with(deviceNo,"_1")){
        index = 1;
    }else{
        index = 2;
    }
    char * master_device_no = get_master_device_sn();
    QM_ESL2_LOG("mqtt_subscriber deviceNo:%s master_device_no:%s\n",deviceNo,master_device_no);
    int master_index = 0;
    if(ends_with(master_device_no,"_1")){
        master_index == 1;
    }else{
        master_index == 2;
    }
    int esl_type = get_esl_type();
    //消息状态上报
    post_push_message2("1",message_model);
    if(strcmp(deviceNo,master_device_no) == 0){
        char * device_sn = get_device_sn(index);
        int ignore = message_model -> ignore;
        if(ignore == 1){
            //本次消息忽略
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
            post_push_message2("2",message_model);
            post_push_message2("3",message_model);
            free_esl_message_model(message_model);
            QM_ESL2_LOG("mqtt_subscriber get dt fail!\n");
            return;
        }
        int type = message_model -> type;
        QM_ESL2_LOG("mqtt_subscriber type = %d\n", type);
        if(type == EslMessageTypeCONTENT){
            //模版更新
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                return;
            }
            cJSON *unbind = cJSON_GetObjectItem(dataItem,"unbind");
            if(unbind && cJSON_IsNumber(unbind) && unbind->valueint == 1){
                //设备解绑 删除数据，还需要删除本地文件
                if(esl_type == 2){
                    char * message_id = message_model->message_id;
                    push_msgack_message_2(device_sn,message_id);
                }
                set_show_green_led(index,1);
                if(index == 1){
                    lv_async_call(delete_ui_and_local_data_1,NULL);
                }else{
                    lv_async_call(delete_ui_and_local_data_2,NULL);
                }   
            }else{
                cJSON *contentItem = cJSON_GetObjectItem(dataItem,"content");
                if(contentItem == NULL && !cJSON_IsString(contentItem)){
                    cJSON_Delete(dataItem);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                    return;
                }
                char * content = strdup(contentItem->valuestring);
                if(is_string_empty(content)){
                    if(content != NULL) free(content);
                    cJSON_Delete(dataItem);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber get content fail!\n");
                    return;
                }
                int esl_type = get_esl_type();
                if(esl_type == 2 || esl_type == 3){
                    esl_base_model_t * model = parse_esl_base_model(content);
                    esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                    if (esl_file_save_info_args == NULL) {
                        free_esl_base_model(model);
                        free(content);
                        cJSON_Delete(dataItem);
                        free_esl_message_model(message_model);
                        set_show_green_led(index,0);
                        return;
                    }
                    set_show_green_led(index,1);
                    esl_file_save_info_args->index = index;
                    esl_file_save_info_args->model = model;
                    esl_file_save_info_args->device_sn = device_sn;
                    save_data_thread(esl_file_save_info_args);

                    download_image_or_video(model,index);

                    // 开启线程下载资源
                    show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                    args->model = model;
                    args->index = index;
                    
                    if(index == 1){
                        lv_async_call(refresh_ui_1,args);
                    }else{
                        lv_async_call(refresh_ui_2,args);
                    }
                    if(esl_type == 2){
                        char * message_id = message_model->message_id;
                        push_msgack_message_2(device_sn,message_id);
                    }
                }else{
                    cJSON *json = cJSON_Parse(content);
                    if(json == NULL){
                        free(content);
                        cJSON_Delete(dataItem);
                        free_esl_message_model(message_model);
                        QM_ESL2_LOG("mqtt_subscriber parse content fail!\n");
                        return;
                    }
                    cJSON *tpl_jn = cJSON_GetObjectItem(json, "tpl_jn");
                    cJSON *tpl_pro = cJSON_GetObjectItem(json, "tpl_pro");
                    cJSON *tpl_txt_jn = cJSON_GetObjectItem(json, "tpl_txt_jn");
                    cJSON *tpl_bg_url = cJSON_GetObjectItem(json, "tpl_bg_url");
                    cJSON *dsp_pro_num = cJSON_GetObjectItem(json, "dsp_pro_num");
                    if(tpl_jn && tpl_pro){
                        set_show_green_led(index,1);
                        int count = 0;
                        char * product_template = cJSON_PrintUnformatted(tpl_jn);
                        char * product_list = cJSON_PrintUnformatted(tpl_pro);
                        char * other_info_str = cJSON_PrintUnformatted(tpl_txt_jn);
                        char * temp = (tpl_bg_url && cJSON_IsString(tpl_bg_url) && tpl_bg_url->valuestring) ? strdup(tpl_bg_url->valuestring) : strdup("");
                        int display_product_num = cJSON_IsNumber(dsp_pro_num) ? dsp_pro_num->valueint : 0;
                        esl_base_model_t * model = the_second_to_model(product_template,product_list,other_info_str,temp,index,display_product_num);
    
                        esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                        if (esl_file_save_info_args == NULL) {
                            free_esl_base_model(model);
                            free(product_template);
                            free(product_list);
                            free(other_info_str);
                            free(temp);
                            free(content);
                            cJSON_Delete(dataItem);
                            free_esl_message_model(message_model);
                            set_show_green_led(index,0);
                            return;
                        }
                        esl_file_save_info_args->index = index;
                        esl_file_save_info_args->model = model;
                        esl_file_save_info_args->device_sn = device_sn;
                        save_data_thread(esl_file_save_info_args);
    
                        download_image_or_video(model,index);
    
                        // 开启线程下载资源
                        show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                        args->model = model;
                        args->index = index;
                        
                        if(index == 1){
                            lv_async_call(refresh_ui_1,args);
                        }else{
                            lv_async_call(refresh_ui_2,args);
                        }
                        free(temp);
                        free(other_info_str);
                        free(product_list);
                        free(product_template);
                    }else{
                        set_show_green_led(index,1);
                        esl_base_model_t * model = the_second_to_model(content,NULL,NULL,NULL,index,0);
    
                        esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                        if (esl_file_save_info_args == NULL) {
                            free_esl_base_model(model);
                            cJSON_Delete(json);
                            free(content);
                            cJSON_Delete(dataItem);
                            free_esl_message_model(message_model);
                            set_show_green_led(index,0);
                            return;
                        }
                        esl_file_save_info_args->index = index;
                        esl_file_save_info_args->model = model;
                        esl_file_save_info_args->device_sn = device_sn;
                        save_data_thread(esl_file_save_info_args);
    
                        download_image_or_video(model,index);
    
                        // 开启线程下载资源
                        show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                        args->model = model;
                        args->index = index;
                        
                        if(index == 1){
                            lv_async_call(refresh_ui_1,args);
                        }else{
                            lv_async_call(refresh_ui_2,args);
                        }
                    }
                    cJSON_Delete(json);
                }
                cJSON_Delete(dataItem);
                free(content);
            }
        }else if(type == EslMessageTypeRESTRAT){
            //设备重启
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, reboot_thread, NULL);
            pthread_detach(thread_id);
        }else if(type == EslMessageTypeREFRESH){
            //刷新
            char ip[NI_MAXHOST] = {0};
            get_local_ip(ip, NI_MAXHOST);
            QM_ESL2_LOG("Local IP address: %s\n", ip);
            char version[32] = {0};
            #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
                #define CONFIG_BUILD_ID "1112136"
                #define CONFIG_QM_VERSION "V1.0.1"
            #else
                #ifndef CONFIG_BUILD_ID
                #define CONFIG_BUILD_ID "000"
                #endif
            #endif
            snprintf(version,sizeof(version),"%s.%s",CONFIG_QM_VERSION,CONFIG_BUILD_ID);
            char * ssid = get_ssid_2();
            int esl_type = get_esl_type();
            char * token = get_token();
            char * content = get_content_info(device_sn,ssid,ip,version,esl_type,token);
            if(is_string_empty(content)){
                if(content != NULL) free(content);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get content fail!\n");
                return;
            }
            if(esl_type == 2  || esl_type == 3){
                esl_base_model_t * model = parse_esl_base_model(content);
                esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                if (esl_file_save_info_args == NULL) {
                    free_esl_base_model(model);
                    free(content);
                    free_esl_message_model(message_model);
                    set_show_green_led(index,0);
                    return;
                }
                set_show_green_led(index,1);
                esl_file_save_info_args->index = index;
                esl_file_save_info_args->model = model;
                esl_file_save_info_args->device_sn = device_sn;
                save_data_thread(esl_file_save_info_args);

                download_image_or_video(model,index);

                // 开启线程下载资源
                show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                args->model = model;
                args->index = index;
                
                if(index == 1){
                    lv_async_call(refresh_ui_1,args);
                }else{
                    lv_async_call(refresh_ui_2,args);
                }
                if(esl_type == 2){
                    char * message_id = message_model->message_id;
                    push_msgack_message_2(device_sn,message_id);
                }
            }else{
                cJSON *json = cJSON_Parse(content);
                if(json == NULL){
                    free(content);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber parse content fail!\n");
                    return;
                }
                cJSON *tpl_jn = cJSON_GetObjectItem(json, "tpl_jn");
                cJSON *tpl_pro = cJSON_GetObjectItem(json, "tpl_pro");
                cJSON *tpl_txt_jn = cJSON_GetObjectItem(json, "tpl_txt_jn");
                cJSON *tpl_bg_url = cJSON_GetObjectItem(json, "tpl_bg_url");
                cJSON *dsp_pro_num = cJSON_GetObjectItem(json, "dsp_pro_num");
                if(tpl_jn && tpl_pro){
                    set_show_green_led(index,1);
                    int count = 0;
                    char * product_template = cJSON_PrintUnformatted(tpl_jn);
                    char * product_list = cJSON_PrintUnformatted(tpl_pro);
                    char * other_info_str = cJSON_PrintUnformatted(tpl_txt_jn);
                    char * temp = (tpl_bg_url && cJSON_IsString(tpl_bg_url) && tpl_bg_url->valuestring) ? strdup(tpl_bg_url->valuestring) : strdup("");
                    int display_product_num = cJSON_IsNumber(dsp_pro_num) ? dsp_pro_num->valueint : 0;
                    esl_base_model_t * model = the_second_to_model(product_template,product_list,other_info_str,temp,index,display_product_num);
    
                    esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                    if (esl_file_save_info_args == NULL) {
                        free_esl_base_model(model);
                        free(product_template);
                        free(product_list);
                        free(other_info_str);
                        free(temp);
                        cJSON_Delete(json);
                        free(content);
                        free_esl_message_model(message_model);
                        set_show_green_led(index,0);
                        return;
                    }
                    esl_file_save_info_args->index = index;
                    esl_file_save_info_args->model = model;
                    esl_file_save_info_args->device_sn = device_sn;
                    save_data_thread(esl_file_save_info_args);
    
                    download_image_or_video(model,index);
    
                    // 开启线程下载资源
                    show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                    args->model = model;
                    args->index = index;
                    
                    if(index == 1){
                        lv_async_call(refresh_ui_1,args);
                    }else{
                        lv_async_call(refresh_ui_2,args);
                    }
                    free(temp);
                    free(other_info_str);
                    free(product_list);
                    free(product_template);
                }else{
                    set_show_green_led(index,1);
                    esl_base_model_t * model = the_second_to_model(content,NULL,NULL,NULL,index,0);
    
                    esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                    if (esl_file_save_info_args == NULL) {
                        free_esl_base_model(model);
                        cJSON_Delete(json);
                        free(content);
                        free_esl_message_model(message_model);
                        set_show_green_led(index,0);
                        return;
                    }
                    esl_file_save_info_args->index = index;
                    esl_file_save_info_args->model = model;
                    esl_file_save_info_args->device_sn = device_sn;
                    save_data_thread(esl_file_save_info_args);
    
                    download_image_or_video(model,index);
    
                    // 开启线程下载资源
                    show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                    args->model = model;
                    args->index = index;
                    
                    if(index == 1){
                        lv_async_call(refresh_ui_1,args);
                    }else{
                        lv_async_call(refresh_ui_2,args);
                    }
                }
                cJSON_Delete(json);
            }
            free(content);
        }  else if(type == EslMessageTypeUNBIND){
            //设备解绑 删除数据，还需要删除本地文件
            set_show_green_led(index,1);
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
            if(index == 1){
                lv_async_call(delete_ui_and_local_data_1,NULL);
            }else{
                lv_async_call(delete_ui_and_local_data_2,NULL);
            }   
        }else if(type == EslMessageTypeWIFI){
            //wifi修改
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL || !cJSON_IsObject(dataItem)){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                return;
            }
            cJSON *wifiSSIDItem = cJSON_GetObjectItem(dataItem, "name");
            if(wifiSSIDItem == NULL || !cJSON_IsString(wifiSSIDItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get wifi_ssid fail!\n");
                return;
            }
            char *wifi_SSID = strdup(wifiSSIDItem->valuestring);

            cJSON *wifiPwdItem = cJSON_GetObjectItem(dataItem, "password");
            if(wifiPwdItem == NULL || !cJSON_IsString(wifiPwdItem)){
                free(wifi_SSID);
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get wifi_pwd fail!\n");
                return;
            }
            char *wifi_pwd = strdup(wifiPwdItem->valuestring);
            set_show_green_led(index,1);
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
            char cmd[100]={0};
            snprintf(cmd,sizeof(cmd),"wifi_start.sh %s %s",wifi_SSID,wifi_pwd);
            QM_ESL2_LOG("run: %s",cmd);
            int ret = system(cmd);
            QM_ESL2_LOG("process %s ,ret:%d",cmd,ret);
            system("reboot");
        }else if(type == EslMessageTypeGROUP){
            //分组修改
            set_show_green_led(index,1);
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get dataItem fail!\n");
                set_show_green_led(index,0);
                return;
            }
            cJSON *groupIdItem = cJSON_GetObjectItem(dataItem, "groupId");
            if(groupIdItem == NULL || !cJSON_IsNumber(groupIdItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get group_id fail!\n");
                set_show_green_led(index,0);
                return;
            }
            cJSON *groupNameItem = cJSON_GetObjectItem(dataItem, "groupName");
            if(groupNameItem == NULL || !cJSON_IsString(groupNameItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get group_name fail!\n");
                set_show_green_led(index,0);
                return;
            }
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
            system("reboot");
        }else if(type == EslMessageTypeUPDATE){
            //升级
            char ip[NI_MAXHOST] = {0};
            get_local_ip(ip, NI_MAXHOST);
            QM_ESL2_LOG("EslMessageTypeUPDATE Local IP address: %s\n", ip);
            char version[32] = {0};
            #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
                #define CONFIG_BUILD_ID "1112136"
                #define CONFIG_QM_VERSION "V1.0.1"
            #else
                #ifndef CONFIG_BUILD_ID
                #define CONFIG_BUILD_ID "000"
                #endif
            #endif
            snprintf(version,sizeof(version),"%s.%s",CONFIG_QM_VERSION,CONFIG_BUILD_ID);
            char * ssid = get_ssid_2();
            int esl_type = get_esl_type();
            char * token = get_token();
            char * verBuffer = get_version_info(device_sn,ssid,ip,version,esl_type,token);
            QM_ESL2_LOG("get_version_info: %s\n", verBuffer);
            if(is_string_empty(verBuffer)){
                if(verBuffer) free(verBuffer);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber parse version fail!\n");
                return;
            }
            free(verBuffer);
            char cmd[100]={0};
            snprintf(cmd,sizeof(cmd),"downloader.sh %s %s",CONFIG_QM_VERSION,CONFIG_PLATFORM_ID);
            QM_ESL2_LOG("run: %s",cmd);
            int ret =system(cmd);
            if(ret == 0){
                if(esl_type == 2){
                    char * message_id = message_model->message_id;
                    push_msgack_message_2(device_sn,message_id);
                }
                free_esl_message_model(message_model);
                QM_ESL2_LOG("run command success: %s\n", cmd);
                system("rm -rf /data/update.tar.gz");
                sleep(3);
                system("rm -rf /data/res/img /data/res/json /data/res/logo");
                system("rm -rf /data/*core_dump*");
                system("reboot");
            }
        }else if(type == EslMessageTypeURL){
            //基础url修改 TODO
        }else if(type == EslMessageTypeSCREENSWITCH){
            //屏幕开关
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get dataItem fail!\n");
                return;
            }
            cJSON *valueItem = cJSON_GetObjectItem(dataItem, "value");
            if(valueItem == NULL || !cJSON_IsNumber(valueItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get valueItem fail!\n");
                return;
            }
            int value = valueItem->valueint;
            #if defined(CONFIG_XOS_HW_QUADRV) && !defined(BUILD_SIMULATOR)
                extern int qm_backlight_setbri(unsigned int brightness);
                extern int qm_backlight_getbri(void);
                extern int xos_lcm_get_screen_num(void);

                if(value == 1){
                    if (xos_lcm_get_screen_num() == 1) {
                        qm_backlight_setbri(40);
                    }
                    if (xos_lcm_get_screen_num() == 2) {
                        qm_backlight_setbri(80);
                    }
                }else{
                    qm_backlight_setbri(0);
                }
            #endif
            cJSON_Delete(dataItem);

            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
        }
        post_push_message2("3",message_model);
    }else{
        //设备编码不匹配 转发消息
        char * socket_message = els_message_model_to_json(message_model);
        QM_ESL2_LOG("mqtt_subscriber deviceNo is not match!\n");
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
            post_msg_to_queue(1, deviceNo,socket_message);
        #else
            post_push_message2("2",message_model);
            int flag = create_send_socket(deviceNo,socket_message);
            if(flag == 0){
                post_push_message2("3",message_model);
            }
        #endif
        QM_ESL2_LOG("mqtt_subscriber deviceNo is not match!\n");
        free(socket_message);
    }
    free_esl_message_model(message_model);
    QM_ESL2_LOG("mqtt_subscriber end  \n");
}

void mqtt_connect_response(const AiMessage* params)
{
    // QM_ESL2_LOG("mqtt_connect_response net state =%d \n",params->val );
    // if(params->val == 2){
    //     image_register_device(deviceSerialNumber,"","");
    // }
}
#endif
