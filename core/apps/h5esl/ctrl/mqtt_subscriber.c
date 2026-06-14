#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
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
#include "../utils/utils.h"
#include "esl_socket_service.h"
#include "esl_socket_udp_service.h"
#include "esl_service.h"
#include "esl_file.h"
#include "../view/page_esl.h"
#include "../utils/esl_sign.h"
#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_device_setting.h"
#include "../conf/conf.h"
#include "esl_message_queue.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "logo_img.h"
#endif

#ifndef CONFIG_QM_VERSION
#define CONFIG_QM_VERSION "V1.00.00"
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
    LV_LOG_USER("mqtt_subscriber net state =%d \n",params->val );
    LV_LOG_USER("mqtt_subscriber message =%s \n",params->data2 );
    char *message = params->data2;
    cJSON *result_json = cJSON_Parse(message);
    if(!result_json){
        LV_LOG_USER("mqtt_subscriber convert json fail!\n");
        return;
    }
    cJSON *deviceNoItem = cJSON_GetObjectItem(result_json, "device_no");
    if(deviceNoItem == NULL || !cJSON_IsString(deviceNoItem)){
        cJSON_Delete(result_json);
        LV_LOG_USER("mqtt_subscriber get device_no fail!\n");
        return;
    }
    char *deviceNo = strdup(deviceNoItem->valuestring);
    int index = 0;
    if(ends_with(deviceNo,"_1")){
        index = 1;
    }else{
        index = 2;
    }
    char * master_device_no = get_master_device_sn();
    LV_LOG_USER("mqtt_subscriber deviceNo:%s master_device_no:%s\n",deviceNo,master_device_no);
    int master_index = 0;
    if(ends_with(master_device_no,"_1")){
        master_index == 1;
    }else{
        master_index == 2;
    }
    //消息状态上报
    post_push_message("1",result_json);
    #if 0
    if(starts_with(deviceNo,deviceSerialNumber)){
    #else
    if(strcmp(deviceNo,master_device_no) == 0){
    #endif
        free(deviceNo);
        char * device_sn = get_device_sn(index);
        cJSON *dateTypeItem = cJSON_GetObjectItem(result_json, "date_type");
        if(dateTypeItem == NULL || !cJSON_IsNumber(dateTypeItem)){
            LV_LOG_USER("mqtt_subscriber get dt fail!\n");
            cJSON_Delete(result_json);
            return;
        }
        int dt = dateTypeItem->valueint;
        LV_LOG_USER("mqtt_subscriber dt = %d\n", dt);
        switch (dt){
            case 1:
                {
                    //设备信息修改
                    /* {"device_no":"D1S920H73ODI667_2","date_type":1,"device_setting":"{\"SN\":\"D1S920H73ODI667_2\",\"pos_ip\":\"\",\"pos_id\":\"\",\"server_ip\":\"\",\"speech_play\":\"0\",\"store_name\":\"价签工厂测试\",\"store_id\":\"000000000002185\",\"show_msg\":\"您已进入监控区域\",\"pos_version\":\"\",\"video_muted\":\"0\",\"show_led_id\":\"07\",\"device_tpl_id\":\"7007\",\"swiper_time\":\"10\",\"other_1\":\"\",\"other_2\":\"clound\",\"other_3\":\"21028,78965\",\"other_4\":\"sale_price\",\"other_5\":\"\",\"other_6\":\"\",\"merchant_id\":1602,\"local_up_tag\":\"0\",\"branch_no\":\"ALL\",\"branch_name\":\"所有门店\",\"wifi_SSID\":\"MAOESL\",\"wifi_pwd\":\"a12345678B.\",\"screen_direction\":\"portrait-primary\",\"video_url\":\"\",\"wxwork_role\":\"0\"}","other_function":"","device_tpl_id":7007,"device_model":"D1S920H73ODI667_2号屏","agent_id":62,"device_md5":"f872984803a0f013887b1c7c6c6a24ee","push_id":"250233861"} */
                    /* cJSON *data = cJSON_GetObjectItem(result_json, "device_setting");
                    esl_device_setting_t * device_setting = parse_device_setting(data);
                    set_device_setting(device_setting,index); */
                    break;
                }
            case 2:
                {
                    //模板内容修改
                    /* cJSON *data = cJSON_GetObjectItem(result_json, "tpl_info");
                    esl_show_model_t * model = parse_product_template(data);
                    set_product_model(model,index); */
                    cJSON *batch_data = cJSON_GetObjectItem(result_json, "batch");
                    if(batch_data == NULL || !cJSON_IsNumber(batch_data)){
                        LV_LOG_USER("mqtt_subscriber get batch fail!\n");
                        cJSON_Delete(result_json);
                        return;
                    }

                    int batch = batch_data->valueint;
                    if(batch != 1){
                        LV_LOG_USER("mqtt_subscriber batch = %d\n", batch);
                        cJSON_Delete(result_json);
                        return;
                    }
                    time_t current_time = time(NULL);
                    if(index == 1){
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_1) <= 5) {
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            return;
                        }
                        last_message_time_1 = current_time;
                    }else{
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_2) <= 5) {
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            return;
                        }
                        last_message_time_2 = current_time;
                    }
                    show_green_led(index);
                    //批量更新
                    cJSON *data = cJSON_GetObjectItem(result_json, "tpl_info");
                    esl_show_model_t * model = parse_product_template(data);

                    char * setting_sign = generate_sign(device_sn,"setting");
                    esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                    free(setting_sign);
                    if (device_setting == NULL) {
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }
                    
                    //模板商品数据
                    int count = 0;
                    char * product_info_sign = generate_sign(device_sn,"item");
                    esl_product_info_t ** product_info_list = query_product_infos(device_sn,"item",product_info_sign,&count);
                    free(product_info_sign);
                    if(product_info_list == NULL || count == 0){
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }

                    //同步到本地
                    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                    if (args == NULL) {
                        free_esl_product_info_array(product_info_list,count);
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        printf("Failed to allocate memory for thread arguments\n");
                        clear_green_led(index);
                        return;
                    }
                    args->index = index;
                    args->model = model;
                    args->product_info_list = product_info_list;
                    args->product_info_count = count;
                    args->device_setting = device_setting;
                    args->device_sn = device_sn;
                    save_data_thread(args);

                    download_image_or_video(model,device_setting,product_info_list,count,index);

                    current_time = time(NULL);
                    show_ui_args_t *show_ui_args = malloc(sizeof(show_ui_args_t));
                    show_ui_args->model = model;
                    show_ui_args->device_setting = device_setting;
                    show_ui_args->product_info_list = product_info_list;
                    show_ui_args->count = count;
                    show_ui_args->index = index;

                    if(index == 1){
                        last_message_time_1 = current_time;
                        lv_async_call(refresh_ui_1,show_ui_args);
                    }else{
                        last_message_time_2 = current_time;
                        lv_async_call(refresh_ui_2,show_ui_args);
                    }
                    break;
                }
            case 3:
                {
                    time_t current_time = time(NULL);
                    if(index == 1){
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_1) <= 5) {
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            return;
                        }
                        last_message_time_1 = current_time;
                    }else{
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_2) <= 5) {
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            return;
                        }
                        last_message_time_2 = current_time;
                    }
                    show_green_led(index);
                    if (index == 1){
                        if(logo_image_is_exist(k_path_boot_bg_img_1) == 0){
                            char ip[NI_MAXHOST];
                            get_local_ip(ip, NI_MAXHOST);
                            printf("Local IP address: %s\n", ip);
                            char version[32] = {0};
                            #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
                                #define CONFIG_BUILD_ID "1112136"
                                #define CONFIG_QM_VERSION "v_1.00.01"
                            #else
                                #ifndef CONFIG_BUILD_ID
                                #define CONFIG_BUILD_ID "000"
                                #endif
                            #endif
                            snprintf(version,sizeof(version),"%s-J%s",CONFIG_QM_VERSION,CONFIG_BUILD_ID);
                            char * sign = generate_push_server_sign(device_sn,"push_server","MQTT",ip,version);
                            LV_LOG_USER("sign :%s",sign);
                            esl_push_server_data * data = query_push_server(device_sn,"push_server",sign,"MQTT",ip,version);
                            free(sign);
                            if (data != NULL && data -> boot_logo_url != NULL) {
                                download_logo_image(data -> boot_logo_url,k_path_boot_bg_img_1);
                                #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                                extern int xos_logo_update(uint8_t index, const char *path);
                                xos_logo_update(index - 1, k_path_boot_bg_img_1);
                                #endif
                            }
                            free_esl_push_server_data(data);
                        }
                    }else if (index == 2){
                        if(logo_image_is_exist(k_path_boot_bg_img_2) == 0){
                            char ip[NI_MAXHOST];
                            get_local_ip(ip, NI_MAXHOST);
                            printf("Local IP address: %s\n", ip);
                            char version[32] = {0};
                            #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
                                #define CONFIG_BUILD_ID "1112136"
                                #define CONFIG_QM_VERSION "v_1.00.01"
                            #else
                                #ifndef CONFIG_BUILD_ID
                                #define CONFIG_BUILD_ID "000"
                                #endif
                            #endif
                            snprintf(version,sizeof(version),"%s-J%s",CONFIG_QM_VERSION,CONFIG_BUILD_ID);
                            char * sign = generate_push_server_sign(device_sn,"push_server","MQTT",ip,version);
                            LV_LOG_USER("sign :%s",sign);
                            esl_push_server_data * data = query_push_server(device_sn,"push_server",sign,"MQTT",ip,version);
                            free(sign);
                            if (data != NULL && data -> boot_logo_url != NULL) {
                                download_logo_image(data -> boot_logo_url,k_path_boot_bg_img_2);
                                #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                                extern int xos_logo_update(uint8_t index, const char *path);
                                xos_logo_update(index - 1, k_path_boot_bg_img_2);
                                #endif
                            }
                            free_esl_push_server_data(data);
                        }
                    }

                    //TODO 临时解决方案，后续需要优化
                    char * template_sign = generate_sign(device_sn,"tpl");
                    esl_show_model_t *model = query_product_template(device_sn,"tpl",template_sign);
                    free(template_sign);
                    if(model == NULL){
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }

                    char * setting_sign = generate_sign(device_sn,"setting");
                    esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                    free(setting_sign);
                    if (device_setting == NULL) {
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }
                    
                    //模板商品数据
                    int count;
                    cJSON *dataItem = cJSON_GetObjectItem(result_json, "item");
                    esl_product_info_t ** product_info_list = parse_product_infos(dataItem,&count);
                    if(product_info_list == NULL || count == 0){
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }

                    //同步到本地
                    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                    if (args == NULL) {
                        free_esl_product_info_array(product_info_list,count);
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        printf("Failed to allocate memory for thread arguments\n");
                        clear_green_led(index);
                        return;
                    }
                    args->index = index;
                    args->model = model;
                    args->product_info_list = product_info_list;
                    args->product_info_count = count;
                    args->device_setting = device_setting;
                    args->device_sn = device_sn;
                    save_data_thread(args);

                    download_image_or_video(model,device_setting,product_info_list,count,index);

                    current_time = time(NULL);
                    show_ui_args_t *show_ui_args = malloc(sizeof(show_ui_args_t));
                    show_ui_args->model = model;
                    show_ui_args->device_setting = device_setting;
                    show_ui_args->product_info_list = product_info_list;
                    show_ui_args->count = count;
                    show_ui_args->index = index;

                    if(index == 1){
                        last_message_time_1 = current_time;
                        lv_async_call(refresh_ui_1,show_ui_args);
                    }else{
                        last_message_time_2 = current_time;
                        lv_async_call(refresh_ui_2,show_ui_args);
                    }
                    break;
                }
            case 4:
                {
                    time_t current_time = time(NULL);
                    if(index == 1){
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_1) <= 5) {
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            return;
                        }
                        last_message_time_1 = current_time;
                    }else{
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_2) <= 5) {
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            return;
                        }
                        last_message_time_2 = current_time;
                    }
                    show_green_led(index);
                    //模板商品数据修改 TODO 临时解决方案，后续需要优化
                    char * template_sign = generate_sign(device_sn,"tpl");
                    esl_show_model_t *model = query_product_template(device_sn,"tpl",template_sign);
                    free(template_sign);
                    if(model == NULL){
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }

                    char * setting_sign = generate_sign(device_sn,"setting");
                    esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                    free(setting_sign);
                    if (device_setting == NULL) {
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }

                    int count,old_count;
                    cJSON *dataItem = cJSON_GetObjectItem(result_json, "item");
                    char * product_info_sign = generate_sign(device_sn,"item");
                    esl_product_info_t ** old_product_info_list = query_product_infos(device_sn,"item",product_info_sign,&old_count);
                    free(product_info_sign);
                    if(old_product_info_list == NULL || old_count == 0){
                        free_device_setting(device_setting);
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        clear_green_led(index);
                        return;
                    }
                    esl_product_info_t ** product_info_list = parse_product_infos(dataItem,&count);
                    update_product_info_batch(product_info_list,count,old_product_info_list,old_count);
                    
                    //同步到本地
                    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                    if (args == NULL) {
                        free_esl_product_info_array(old_product_info_list,old_count);
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        printf("Failed to allocate memory for thread arguments\n");
                        clear_green_led(index);
                        return;
                    }
                    args->index = index;
                    args->model = model;
                    args->product_info_list = old_product_info_list;
                    args->product_info_count = old_count;
                    args->device_setting = device_setting;
                    args->device_sn = device_sn;
                    save_data_thread(args);

                    download_image_or_video(model,device_setting,old_product_info_list,old_count,index);

                    current_time = time(NULL);
                    
                    show_ui_args_t *show_ui_args = malloc(sizeof(show_ui_args_t));
                    show_ui_args->model = model;
                    show_ui_args->device_setting = device_setting;
                    show_ui_args->product_info_list = old_product_info_list;
                    show_ui_args->count = old_count;
                    show_ui_args->index = index;
                    if(index == 1){
                        last_message_time_1 = current_time;
                        lv_async_call(refresh_ui_1,show_ui_args);
                    }else{
                        last_message_time_2 = current_time;
                        lv_async_call(refresh_ui_2,show_ui_args);
                    }
                    break;
                }
            case 5:
                //暂时没有
                break;
            case 6:
                {
                    //wife修改
                    //{"device_no":"D1S920H73ODI667_1","date_type":6,"wifi_SSID":"aaa","wifi_pwd":"bbb","device_md5":"3a5b03322680b7d0262adfa487e086b4","push_id":"25
                    cJSON *wifiSSIDItem = cJSON_GetObjectItem(result_json, "wifi_SSID");
                    if(wifiSSIDItem == NULL || !cJSON_IsString(wifiSSIDItem)){
                        cJSON_Delete(result_json);
                        LV_LOG_USER("mqtt_subscriber get wifi_ssid fail!\n");
                        return;
                    }
                    char *wifi_SSID = strdup(wifiSSIDItem->valuestring);
    
                    cJSON *wifiPwdItem = cJSON_GetObjectItem(result_json, "wifi_pwd");
                    if(wifiPwdItem == NULL || !cJSON_IsString(wifiPwdItem)){
                        free(wifi_SSID);
                        cJSON_Delete(result_json);
                        LV_LOG_USER("mqtt_subscriber get wifi_pwd fail!\n");
                        return;
                    }
                    char *wifi_pwd = strdup(wifiPwdItem->valuestring);

                    show_green_led(index);
                    char cmd[100]={0};
                    snprintf(cmd,sizeof(cmd),"wifi_start.sh %s %s",wifi_SSID,wifi_pwd);
                    printf("run: %s",cmd);
                    int ret = system(cmd);
                    printf("process %s ,ret:%d",cmd,ret);
                    system("reboot");
                    break;
                }
            case 7:
                //设备解绑 删除数据，还需要删除本地文件
                show_green_led(index);
                if(index == 1){
                    lv_async_call(delete_ui_and_local_data_1,NULL);
                }else{
                    lv_async_call(delete_ui_and_local_data_2,NULL);
                }
                break;
            case 8:
                //广告添加
                break;
            case 9:
                //广告删除
                break;
            case 10:
                //系统配置，重启，解绑，音量配置等
                /* 重启：Restart
                    刷新：refresh
                    更新：version_update
                    清除：clear_file
                    设备重启：deviceRestart
                    背光关机：deviceShutdown
                    背光开机：devicePowerOn
                    清除推送消息：clearAllMsg
                    开始搜索：searchDevice */
                    /* {"device_no":"ss123456789_2","date_type":10,"config":"{\"type\":\"Restart\"}","device_md5":"7405e39e4e8721841c95ac638e95581b","push_id":"250232610"} */
                    /* {"device_no":"sss12345678901_2","date_type":10,"config":"{\"type\":\"refresh\"}","device_md5":"99ee4b548d47e0e60c54fb109ca3cea1","push_id":"250272157"} */
                {
                    show_green_led(index);
                    cJSON *configItem = cJSON_GetObjectItem(result_json, "config");
                    if(configItem == NULL || !cJSON_IsString(configItem)){
                        cJSON_Delete(result_json);
                        LV_LOG_USER("mqtt_subscriber get config fail!\n");
                        clear_green_led(index);
                        return;
                    }
                    char *config = configItem->valuestring;

                    cJSON *config_json = cJSON_Parse(config);
                    if(config_json == NULL){
                        cJSON_Delete(result_json);
                        LV_LOG_USER("mqtt_subscriber convert config json fail!\n");
                        clear_green_led(index);
                        return;
                    }
                    cJSON *typeItem = cJSON_GetObjectItem(config_json, "type");
                    if(typeItem == NULL || !cJSON_IsString(typeItem)){
                        cJSON_Delete(config_json);
                        cJSON_Delete(result_json);
                        LV_LOG_USER("mqtt_subscriber get type fail!\n");
                        clear_green_led(index);
                        return;
                    }
                    char *type = typeItem->valuestring;

                    LV_LOG_USER("mqtt_subscriber config:%s\n",config);
                    if(strcmp(type,"Restart") == 0){
                        if(index == 1){
                            lv_async_call(restart_device_1,NULL);
                        }else{
                            lv_async_call(restart_device_2,NULL);
                        }
                        pthread_t thread_id;
                        pthread_create(&thread_id, NULL, reboot_thread, NULL);
                        pthread_detach(thread_id);
                    }else if(strcmp(type,"refresh") == 0){
                        int product_count = 0;
                        char * product_info_sign = generate_sign(device_sn,"item");
                        esl_product_info_t ** product_info_list = query_product_infos(device_sn,"item",product_info_sign,&product_count);
                        free(product_info_sign);

                        if(product_info_list){
                            char * template_sign = generate_sign(device_sn,"tpl");
                            esl_show_model_t *model = query_product_template(device_sn,"tpl",template_sign);
                            free(template_sign);
                            if(model == NULL){
                                free_esl_product_info_array(product_info_list,product_count);
                                cJSON_Delete(result_json);
                                clear_green_led(index);
                                return;
                            }
                            char * setting_sign = generate_sign(device_sn,"setting");
                            esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                            free(setting_sign);
                            if (device_setting == NULL) {
                                free_esl_show_model(model);
                                free_esl_product_info_array(product_info_list,product_count);
                                cJSON_Delete(result_json);
                                clear_green_led(index);
                                return;
                            }

                            //同步到本地
                            esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                            if (args == NULL) {
                                free_esl_product_info_array(product_info_list,product_count);
                                free_esl_show_model(model);
                                free_device_setting(device_setting);
                                cJSON_Delete(config_json);
                                cJSON_Delete(result_json);
                                printf("Failed to allocate memory for thread arguments\n");
                                clear_green_led(index);
                                return;
                            }
                            args->index = index;
                            args->model = model;
                            args->product_info_list = product_info_list;
                            args->product_info_count = product_count;
                            args->device_setting = device_setting;
                            args->device_sn = device_sn;
                            save_data_thread(args);

                            download_image_or_video(model,device_setting,product_info_list,product_count,index);

                            show_ui_args_t *show_ui_args = malloc(sizeof(show_ui_args_t));
                            show_ui_args->model = model;
                            show_ui_args->device_setting = device_setting;
                            show_ui_args->product_info_list = product_info_list;
                            show_ui_args->count = product_count;
                            show_ui_args->index = index;
                            if(index == 1){
                                lv_async_call(refresh_ui_1,show_ui_args);
                            }else{
                                lv_async_call(refresh_ui_2,show_ui_args);
                            }
                        }
                    }else if(strcmp(type,"version_update") == 0){
                        char cmd[100]={0};
                        snprintf(cmd,sizeof(cmd),"downloader.sh %s",CONFIG_QM_VERSION);
                        printf("run: %s",cmd);
                        int ret =system(cmd);
                        cJSON_Delete(config_json);
                        if(ret == 0){
                            printf("run command success: %s\n", cmd);
                            sleep(3);
                            system("reboot");
                        }
                        break;
                        //版本更新
                    }else if(strcmp(type,"clear_file") == 0){
                        //清除文件
                        system("rm -rf /data/*.lzma");
                        system("rm -rf /data/script*");
                        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                        system("rm -rf /data/res/img");
                        system("rm -rf /data/res/json");
                        #endif

                        pthread_t thread_id;
                        pthread_create(&thread_id, NULL, reboot_thread, NULL);
                        pthread_detach(thread_id);
                    }else if(strcmp(type,"deviceRestart") == 0){
                        //设备重启
                        if(index == 1){
                            lv_async_call(restart_device_1,NULL);
                        }else{
                            lv_async_call(restart_device_2,NULL);
                        }
                        pthread_t thread_id;
                        pthread_create(&thread_id, NULL, reboot_thread, NULL);
                        pthread_detach(thread_id);
                    }else if(strcmp(type,"deviceShutdown") == 0){
                        //背光关机
                    }else if(strcmp(type,"devicePowerOn") == 0){
                        //背光开机
                    }else if(strcmp(type,"clearAllMsg") == 0){
                        //清除推送消息
                    }else if(strcmp(type,"searchDevice") == 0){
                        //开始搜索
                    }
                    cJSON_Delete(config_json);
                    break;
                }
            case 11:
                //促销商品数据
                break;
            case 12:
                //本地ip配置
                break;
            case 13:
                //热点名称
                break;
            case 14:
                //分组修改
                {
                    show_green_led(index);
                    cJSON *groupIdItem = cJSON_GetObjectItem(result_json, "group_id");
                    if(groupIdItem == NULL || !cJSON_IsNumber(groupIdItem)){
                        cJSON_Delete(result_json);
                        LV_LOG_USER("mqtt_subscriber get group_id fail!\n");
                        clear_green_led(index);
                        return;
                    }
                    system("reboot");
                }
                break;
            default:
                break;
        }
    }else{
        //设备编码不匹配 转发消息
        //show_green_led(master_index);
        LV_LOG_USER("mqtt_subscriber deviceNo is not match!\n");
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
            post_msg_to_queue(1, deviceNo,message);
        #else
        post_push_message("2",result_json);
            int flag = create_send_socket(deviceNo,message);
            if(flag == 0){
                post_push_message("3",result_json);
            }
        #endif
        LV_LOG_USER("mqtt_subscriber deviceNo is not match!\n");
        //clear_green_led(master_index);
        free(deviceNo);
    }
    cJSON_Delete(result_json);
    LV_LOG_USER("mqtt_subscriber end  \n");
}

void mqtt_connect_response(const AiMessage* params)
{
    // LV_LOG_USER("mqtt_connect_response net state =%d \n",params->val );
    // if(params->val == 2){
    //     image_register_device(deviceSerialNumber,"","");
    // }
}
#endif
