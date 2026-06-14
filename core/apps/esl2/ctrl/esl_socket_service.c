#ifdef CONFIG_XOS_APP_ESL2
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "esl_socket_service.h"
#include "esl_socket_udp_service.h"
#include "cJSON.h"
#include "esl_file.h"
#include "../conf/conf.h"
#include "../utils/hashmap.h"
#include "esl_service.h"
#include "../view/page_esl.h"
#include "../utils/esl_sign.h"
#include "generated/autoconf.h"
#include "../conf/conf.h"
#include "utils.h"
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

#define BUFFER_SIZE 1024

// 定义互斥锁
pthread_mutex_t create_send_socket_slave_mutex = PTHREAD_MUTEX_INITIALIZER;

// 为每个链表定义一个互斥锁
pthread_mutex_t push_message_mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t push_message_mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t push_message_mutex_3 = PTHREAD_MUTEX_INITIALIZER;

static time_t last_message_time_1 = 0;
static time_t last_message_time_2 = 0;

static int is_running = 1;

static int is_close_finish = 0;

//===========================init map===========================
static int esl_master_socket_compare(const void *a, const void *b, void *udata){
    const struct esl_socket_t *ua = a;
    const struct esl_socket_t *ub = b;
    return strcmp(ua->key, ub->key);
}

static bool esl_master_socket_iter(const void *item, void *udata) {
    const struct esl_socket_t *user = item;
    return true;
}

static uint64_t esl_master_socket_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct esl_socket_t *user = item;
    return hashmap_sip(user->key, strlen(user->key), seed0, seed1);
}

static void esl_master_socket_free(void *item) {
    struct esl_socket_t *user = item;
    free(user->key);
    free(user->device_host);
}

static struct hashmap *map = NULL;

static void _init_map(){
    map = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, NULL, NULL);
}

static struct hashmap *check_life_map = NULL;

static void _init_check_life_map(){
    check_life_map = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, esl_master_socket_free, NULL);
}
//===========================end init map===========================

//===========================init group map  key为group_id===========================
static struct hashmap *group_map_1 = NULL;

static struct hashmap *group_map_2 = NULL;

void init_group_map(){
    group_map_1 = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, esl_master_socket_free, NULL);
    group_map_2 = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, esl_master_socket_free, NULL);
}
//===========================end init group map===========================

//===========================init push id map ============================
static struct hashmap *push_id_map_1 = NULL;
static struct hashmap *push_id_map_2 = NULL;
static struct hashmap *push_id_map_3 = NULL;

void init_push_id_map(){
    push_id_map_1 = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, esl_master_socket_free, NULL);
    push_id_map_2 = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, esl_master_socket_free, NULL);
    push_id_map_3 = hashmap_new(sizeof(struct esl_socket_t), 0, 0, 0, esl_master_socket_hash, esl_master_socket_compare, esl_master_socket_free, NULL);
}

//===========================end push id map ============================

void * cancel_current_listen_socket(){
    /* if(sockfd < 0){
        return NULL;
    } */
    QM_ESL2_LOG("=====================cancel_current_listen_socket entry===========================\n");
    is_running = 0;
    //close(sockfd);
    QM_ESL2_LOG("=====================cancel_current_listen_socket exit===========================\n");
    return NULL;
}

static void * process_server_response(void *arg) {
    prctl(PR_SET_NAME,"process_server_response"); // 设置线程名称
    char *response = (char *)arg;
    QM_ESL2_LOG("Processing server response: %s\n", response);
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        QM_ESL2_LOG("Failed to parse JSON response\n");
        free(response); // 释放内存
        return NULL;
    }
    // {"status":"success","type":2,"push_id":"25046866"}
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (status == NULL || !cJSON_IsString(status)) {
        QM_ESL2_LOG("Invalid JSON response: missing or invalid 'status'\n");
        cJSON_Delete(json);
        free(response); // 释放内存
        return NULL;
    }
    char *status_value = status->valuestring;
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if (type == NULL || !cJSON_IsNumber(type)) {
        QM_ESL2_LOG("Invalid JSON response: missing or invalid 'type'\n");
        cJSON_Delete(json);
        free(response); // 释放内存
        return NULL;
    }
    int type_value = type->valueint;
    if(type_value == 3 || type_value == 4){
        release_consumer_semaphore();
    }
    /* if(type_value != 999){
        release_consumer_semaphore();
    } */
    cJSON *push_id = cJSON_GetObjectItemCaseSensitive(json, "push_id");
    if (push_id == NULL || !cJSON_IsString(push_id)) {
        QM_ESL2_LOG("Invalid JSON response: missing or invalid 'push_id'\n");
        cJSON_Delete(json);
        free(response); // 释放内存
        return NULL;
    }
    char *push_id_value = push_id->valuestring;
    QM_ESL2_LOG("Server response: status=%s, type=%d, push_id=%s\n", status_value, type_value, push_id_value);
    cJSON_Delete(json); // 释放 JSON 对象
    free(response); // 释放内存
    return NULL;
}

static char * _get_group_map(char * key ,struct hashmap * map){
    struct esl_socket_t * user_copy = hashmap_get(map, &(struct esl_socket_t){.key = key});
    if(user_copy != NULL){
        return user_copy->device_host;
    }else{
        return NULL;
    }
}

static char * _get_group_map_by_index(char * key,int index){
    if(index == 1){
        return _get_group_map(key,group_map_1);
    }else if(index == 2){
        return _get_group_map(key,group_map_2);
    }
    return NULL;
}

static void send_response(int index,int group_id, const char *status,const int type, const char *push_id,const char *device_no) {
    #if 0
        char response[256];
        if(type != 999 && push_id != NULL){
            snprintf(response, sizeof(response), "{\"status\":\"%s\",\"type\":%d,\"date_type\":%d,\"device_no\":\"%s\",\"push_id\":\"%s\"}", status, type,990,device_no, push_id);
        }else{
            snprintf(response, sizeof(response), "{\"status\":\"%s\",\"type\":%d,\"date_type\":%d,\"device_no\":\"%s\"}", status, type,990,device_no);
        }
        char buffer[16]; // 假设整数不会超过 15 位
        snprintf(buffer, sizeof(buffer), "%d", group_id);
        char * device_ip = _get_group_map_by_index(buffer,index);
        if(!is_string_empty(device_ip)){
            QM_ESL2_LOG("Response sent ip : %s ,msg: %s, group_id:%d\n",device_ip, response,group_id);
            create_send_socket_slave(device_ip,response);
        }
        QM_ESL2_LOG("Response sent: %s\n", response);
    #endif
}

void *socket_listener(void *arg) {
    int sockfd = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    prctl(PR_SET_NAME, "consume_tcp_message"); // 设置线程名称
    QM_ESL2_LOG("socket_listener start\n");
    while (1) {
        usleep(100*1000);
        int client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        QM_ESL2_LOG("client_sock: %d\n", client_sock);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }
        int buffer_size = BUFFER_SIZE;//1024
        char *buffer = (char *)malloc(buffer_size);
        if (buffer == NULL) {
            perror("malloc");
            close(client_sock);
            continue;
        }
        int total_received = 0;
        int bytes_received;
        while (1) {
            bytes_received = recv(client_sock, buffer + total_received, buffer_size - total_received, 0);
            if (bytes_received == -1) {
                perror("recv");
                free(buffer);
                break;
            } else if (bytes_received == 0) {
                // 对方关闭连接
                break;
            }
            total_received += bytes_received;
            // 如果接收的数据填满了缓冲区，扩大缓冲区
            if (total_received == buffer_size) {
                buffer_size *= 2;
                char *temp_buffer = (char *)realloc(buffer, buffer_size);
                if (temp_buffer == NULL) {
                    perror("realloc");
                    free(buffer);
                    break;
                }
                buffer = temp_buffer;
            }
        }
        buffer[total_received] = '\0';
        QM_ESL2_LOG("Received message: %s\n", buffer);
        close(client_sock);
        int esl_type = get_esl_type();
        esl_message_model_t * message_model = parse_esl_message_model(buffer);
        if(message_model == NULL){
            free(buffer);
            QM_ESL2_LOG("socket_listener convert dto fail!\n");
            continue;
        }
        char *deviceNo = message_model -> device_no;
        int index = 0;
        if(ends_with(deviceNo,"_1")){
            index = 1;
        }else{
            index = 2;
        }
        int group_id = get_local_group_id(index);
        char * device_sn = get_device_sn(index);
        int ignore = message_model -> ignore;
        if(ignore == 1){
            //本次消息忽略
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
            free_esl_message_model(message_model);
            free(buffer);
            QM_ESL2_LOG("mqtt_subscriber get dt fail!\n");
            continue;
        }
        int type = message_model -> type;
        QM_ESL2_LOG("socket_listener type = %d\n", type);
        if(type == EslMessageTypeCONTENT){
            //模版更新
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                free(buffer);
                continue;
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
                int esl_type = get_esl_type();
                if(esl_type != 2){
                    //意味着 自动组网或更新模版 需要调用下config接口
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
                    char * token = get_token();
                    esl_push_server_data2 * data = query_push_server2(device_sn,ssid,ip,version,esl_type,token);
                    free_esl_push_server_data2(data);
                }
                cJSON *contentItem = cJSON_GetObjectItem(dataItem,"content");
                if(contentItem == NULL && !cJSON_IsString(contentItem)){
                    cJSON_Delete(dataItem);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                    free(buffer);
                    continue;
                }
                char * content = strdup(contentItem->valuestring);
                if(is_string_empty(content)){
                    if(content != NULL) free(content);
                    cJSON_Delete(dataItem);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber get content fail!\n");
                    free(buffer);
                    continue;
                }
                if(esl_type == 2 || esl_type == 3){
                    esl_base_model_t * model = parse_esl_base_model(content);
                    esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                    if (esl_file_save_info_args == NULL) {
                        free_esl_base_model(model);
                        free(content);
                        cJSON_Delete(dataItem);
                        free(buffer);
                        free_esl_message_model(message_model);
                        set_show_green_led(index,0);
                        continue;
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
                        free(buffer);
                        continue;
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
                            cJSON_Delete(dataItem);
                            free_esl_message_model(message_model);
                            free(buffer);
                            set_show_green_led(index,0);
                            continue;
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
                            free(buffer);
                            continue;
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
                        QM_ESL2_LOG("......................................................");
                    }
                    cJSON_Delete(json);
                }
                cJSON_Delete(dataItem);
                free(content);
            }
        }else if(type == EslMessageTypeRESTRAT){
            //设备重启
            /* if(index == 1){
                lv_async_call(restart_device_1,NULL);
            }else{
                lv_async_call(restart_device_2,NULL);
            } */
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
                free(buffer);
                continue;
            }
            if(esl_type == 2 || esl_type == 3){
                esl_base_model_t * model = parse_esl_base_model(content);
                esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
                if (esl_file_save_info_args == NULL) {
                    free_esl_base_model(model);
                    free(content);
                    free_esl_message_model(message_model);
                    free(buffer);
                    set_show_green_led(index,0);
                    continue;
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
                    free(buffer);
                    continue;
                }
                cJSON *unbind = cJSON_GetObjectItem(json,"unbind");
                if(unbind && cJSON_IsNumber(unbind) && unbind->valueint == 1){
                    //设备解绑 删除数据，还需要删除本地文件
                    set_show_green_led(index,1);
                    if(index == 1){
                        lv_async_call(delete_ui_and_local_data_1,NULL);
                    }else{
                        lv_async_call(delete_ui_and_local_data_2,NULL);
                    }   
                }else{
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
                            free(buffer);
                            set_show_green_led(index,0);
                            continue;
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
                            free(buffer);
                            continue;
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
                QM_ESL2_LOG("socket_listener get data fail!\n");
                free_esl_message_model(message_model);
                free(buffer);
                continue;
            }
            cJSON *wifiSSIDItem = cJSON_GetObjectItem(dataItem, "name");
            if(wifiSSIDItem == NULL || !cJSON_IsString(wifiSSIDItem)){
                QM_ESL2_LOG("socket_listener get wifi_ssid fail!\n");
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                free(buffer);
                continue;
            }
            char *wifi_SSID = strdup(wifiSSIDItem->valuestring);

            cJSON *wifiPwdItem = cJSON_GetObjectItem(dataItem, "password");
            if(wifiPwdItem == NULL || !cJSON_IsString(wifiPwdItem)){
                free(wifi_SSID);
                QM_ESL2_LOG("socket_listener get wifi_pwd fail!\n");
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                free(buffer);
                continue;
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
                QM_ESL2_LOG("mqtt_subscriber get dataItem fail!\n");
                set_show_green_led(index,0);
                free_esl_message_model(message_model);
                free(buffer);
                continue;
            }
            cJSON *groupIdItem = cJSON_GetObjectItem(dataItem, "groupId");
            if(groupIdItem == NULL || !cJSON_IsNumber(groupIdItem)){
                QM_ESL2_LOG("mqtt_subscriber get group_id fail!\n");
                set_show_green_led(index,0);
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                free(buffer);
                continue;
            }
            cJSON *groupNameItem = cJSON_GetObjectItem(dataItem, "groupName");
            if(groupNameItem == NULL || !cJSON_IsString(groupNameItem)){
                QM_ESL2_LOG("mqtt_subscriber get group_name fail!\n");
                set_show_green_led(index,0);
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                free(buffer);
                continue;
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
            QM_ESL2_LOG("EslMessageTypeUPDATE Local IP address: %s,device_sn:%s\n", ip,device_sn);
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
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get version info fail!\n");
                if(verBuffer) free(verBuffer);
                continue;
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
                free(buffer);
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
                free(buffer);
                continue;
            }
            cJSON *valueItem = cJSON_GetObjectItem(dataItem, "value");
            if(valueItem == NULL || !cJSON_IsNumber(valueItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get valueItem fail!\n");
                free(buffer);
                continue;
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
        }else if(type == EslMessageTypeCHECKLIFE){
            //连接主设备
            char *deviceSn = strdup(message_model -> device_no);
            char *deviceHost = strdup(message_model -> data);
            //在这之前 需要判断下是否发生过变化，如果发生过变化 则需要释放老的内存，如果没有发生变化 则忽略本次操作 TODO
            #if 1
            struct esl_socket_t * user = hashmap_get(map, &(struct esl_socket_t){.key = deviceSn});
            if(user != NULL){
                if(strcmp(user->device_host,deviceHost) != 0){
                    free(user->device_host);
                    user->device_host = strdup(deviceHost);
                }
            }else{
                struct esl_socket_t *socket_instance = (struct esl_socket_t *)malloc(sizeof(struct esl_socket_t));
                if (socket_instance == NULL) {
                    perror("Failed to allocate memory");
                    free(deviceSn);
                    free(deviceHost);
                    free_esl_message_model(message_model);
                    free(buffer);
                    continue;
                }
                socket_instance->key = strdup(deviceSn);
                socket_instance->device_host = strdup(deviceHost);
                hashmap_set(map, socket_instance);
                free(socket_instance);
            }
            if(check_life_map != NULL){
                //在这之前 需要判断下是否发生过变化，如果发生过变化 则需要释放老的内存，如果没有发生变化 则忽略本次操作 TODO
                struct esl_socket_t * user_copy = hashmap_get(check_life_map, &(struct esl_socket_t){.key = deviceSn});
                if(user_copy != NULL){
                    if(strcmp(user_copy->device_host,deviceHost) != 0){
                        free(user_copy->device_host);
                        user_copy->device_host = strdup(deviceHost);
                    }
                }else{
                    struct esl_socket_t *socket_instance_copy = (struct esl_socket_t *)malloc(sizeof(struct esl_socket_t));
                    if (socket_instance_copy == NULL) {
                        perror("Failed to allocate memory");
                        free(deviceSn);
                        free(deviceHost);
                        free_esl_message_model(message_model);
                        free(buffer);
                        continue;
                    }
                    socket_instance_copy->key = strdup(deviceSn);
                    socket_instance_copy->device_host = strdup(deviceHost);
                    hashmap_set(check_life_map, socket_instance_copy);
                    free(socket_instance_copy);
                }
            }
            #endif
            free(deviceSn);
            free(deviceHost);
        }
        free_esl_message_model(message_model);
        free(buffer);
    }
        
    QM_ESL2_LOG("==============================exit socket_listener ====================================\n");
    sleep(3);
    is_close_finish = 1;
    return NULL;
}

int create_listen_socket() {
    _init_map();
    int sockfd;
    struct sockaddr_in server_addr;
    prctl(PR_SET_NAME,"create_tcp_socket"); // 设置线程名称

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    // 设置套接字选项以允许地址重用
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(OPEN_HOST);
    QM_ESL2_LOG("create_listen_socket sin_port: %d\n",OPEN_HOST);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }
    if (listen(sockfd, 50) < 0) {
        perror("listen");
        close(sockfd);
        return -1;
    }
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, socket_listener, &sockfd) != 0) {
        perror("pthread_create");
        close(sockfd);
        return -1;
    }
    // Main thread can perform other tasks here
    pthread_join(thread_id, NULL);
    //pthread_detach(thread_id);
    QM_ESL2_LOG("=====================Thread finished===========================\n");
    close(sockfd);
    return 0;
}

int reconnect_socket_listener() {

    while(!is_close_finish){
        QM_ESL2_LOG("reconnect_socket_listener wait close finish\n");
        sleep(1);
    }

    is_running = 1;
    hashmap_clear(map, 1);
    int sockfd;
    struct sockaddr_in server_addr;
    prctl(PR_SET_NAME,"reconnect_socket_listener"); // 设置线程名称

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    // 设置套接字选项以允许地址重用
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(OPEN_HOST);

    QM_ESL2_LOG("reconnect_socket_listener sin_port: %d\n",OPEN_HOST);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 10) < 0) {
        perror("listen");
        close(sockfd);
        return -1;
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, socket_listener, &sockfd) != 0) {
        perror("pthread_create");
        close(sockfd);
        return -1;
    }

    // Main thread can perform other tasks here
    pthread_join(thread_id, NULL);
    //pthread_detach(thread_id);
    QM_ESL2_LOG("=====================Thread finished===========================\n");
    close(sockfd);
    return 0;
}

int create_send_socket_slave(const char * ip,const char * message){
    // 加锁
    //QM_ESL2_LOG("create_send_socket_slave ip: %s,message:%s\n",ip, message);
    int sock;
	struct sockaddr_in server_addr;
	char buffer[1024] = {0};

	// 创建 socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation error");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(OPEN_HOST);

	// 将 IP 地址转换为二进制形式
    QM_ESL2_LOG("create_send_socket_slave sin_port: %d\n",OPEN_HOST);
	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
        close(sock);
		return -1;
	}

	// 连接到服务器
    /* int retry_count = 3;
    do {
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection failed");
            sleep(1);
        }
        else {
            break;
        }
}while(retry_count-- > 0);
retry_count == 0 ? close(socket);return -1 : 0; */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        QM_ESL2_LOG("Connection failed, ip:%s\n",ip);
        perror("Connection failed");
        close(sock);
        return -1;
    }
	// 发送消息
	if(send(sock, message, strlen(message), 0) < 0){
        perror("Send failed");
        close(sock);  // 确保关闭套接字
        return -1;
    }
	//QM_ESL2_LOG("Message sent: %s\n", message);

	// 接收服务器的响应
    /* int valread = read(sock, buffer, sizeof(buffer) - 1); // 读取响应
    if (valread > 0) {
        buffer[valread] = '\0'; // 确保字符串以 null 结尾
        char * recv_message = strdup(buffer);
        QM_ESL2_LOG("========================Server response: %s\n", recv_message);
        //开启线程，处理接收到的消息
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, process_server_response, recv_message) != 0) {
            perror("pthread_create");
            free(recv_message); // 释放内存
            close(sock);
            return -1;
        }
        pthread_detach(thread_id);
    } else if (valread == 0) {
        QM_ESL2_LOG("======================Server closed the connection.\n");
    } else {
        perror("=======================Error reading server response");
    } */

	// 关闭 socket
    //QM_ESL2_LOG("Message sent to %s: %s\n", ip, message);

	close(sock);
    // 解锁
    //pthread_mutex_unlock(&create_send_socket_slave_mutex);
	return 0;
}

int create_send_socket(const char * device_no,const char *message) {
    //QM_ESL2_LOG("device_no:%s,message:%s",device_no,message);
	struct esl_socket_t * user = hashmap_get(map, &(struct esl_socket_t){.key = device_no});
    if(user == NULL){
        return -1;
    }
	QM_ESL2_LOG("key : %s,value:%s \n",user->key,user->device_host);
	create_send_socket_slave(user->device_host,message);
}

static char * map_keys_to_json_array(){
    if (!check_life_map) return NULL;
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        QM_ESL2_LOG("Failed to create JSON object for model\n");
        return NULL;
    }
    cJSON_AddStringToObject(json, "Tag-Platform", CONFIG_PLATFORM_ID);
    cJSON *json_array = cJSON_CreateArray();
    size_t iter = 0;
    void *item;
    while (hashmap_iter(check_life_map, &iter, &item)) {
        struct esl_socket_t *user = item;
        if (user && user->key) {
            cJSON_AddItemToArray(json_array, cJSON_CreateString(user->key));
        }
    }
    cJSON_AddItemToObject(json, "Device-List", json_array);
    char *json_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_str; // 用完后记得 free
}

static char * map_keys_to_json_array_dto(char * stage,struct hashmap *map){
    if(!map || is_string_empty(stage)){
        return NULL;
    }
    size_t count = hashmap_count(map);
    if(count <= 0){
        return NULL;
    }
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        QM_ESL2_LOG("Failed to create JSON object for model\n");
        return NULL;
    }
    cJSON_AddStringToObject(json, "stage", stage);
    cJSON *json_array = cJSON_CreateArray();
    size_t iter = 0;
    void *item;
    while (hashmap_iter(map, &iter, &item)) {
        struct esl_socket_t *user = item;
        if (user && user->key) {
            cJSON_AddItemToArray(json_array, cJSON_CreateString(user->key));
        }
    }
    cJSON_AddItemToObject(json, "messages", json_array);
    char *json_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_str; // 用完后记得 free
}

static void check_out_life_map(){
    if (hashmap_count(map) != hashmap_count(check_life_map)) {
        size_t iter = 0;
        void *item;
        char *to_delete[512];
        int del_count = 0;
        while (hashmap_iter(map, &iter, &item)) {
            struct esl_socket_t *user = item;
            if (user && user->key) {
                struct esl_socket_t *user_check = hashmap_get(check_life_map, &(struct esl_socket_t){.key = user->key});
                if (user_check == NULL) {
                    char cmd[128];
                    snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s > /dev/null 2>&1", user->device_host);
                    int ret = system(cmd);
                    if (ret == 0) {
                        struct esl_socket_t *new_user = malloc(sizeof(struct esl_socket_t));
                        new_user->key = strdup(user->key);
                        new_user->device_host = strdup(user->device_host);
                        hashmap_set(check_life_map, new_user);
                        free(new_user);
                    } else {
                        to_delete[del_count++] = strdup(user->key);
                    }
                }
            }
        }
        for (int i = 0; i < del_count; i++) {
            hashmap_delete(map, &(struct esl_socket_t){.key = to_delete[i]});
            free(to_delete[i]);
        }
    }
}

void* check_life_thread(void *arg) {
    _init_check_life_map();
    prctl(PR_SET_NAME, "check_life_thread"); // 设置线程名称
    int discount_count = 0;
    int http_count = 0;
    while(1) {
        sleep(45);
        check_out_life_map();
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
        char *all_device_sn = map_keys_to_json_array();
        QM_ESL2_LOG("All keys: %s\n", all_device_sn); // 打印拼接后的 keys
        int count = hashmap_count(check_life_map);
        
        if(count == 0){
            discount_count++;
        }else if(count == 1){
            char *master_device_no = get_master_device_sn();
            size_t iter = 0;
            void *item;
            int only_master = 0;
            while (hashmap_iter(check_life_map, &iter, &item)) {
                struct esl_socket_t *user = item;
                if(user && user->key && strcmp(user->key, master_device_no) == 0){
                    only_master = 1;
                    break;
                }
            }
            if(only_master){
                QM_ESL2_LOG("check_life_thread: count == 1 and only master device, discount_count++\n");
                discount_count++;
            }else{
                discount_count = 0;
            }
        }else{
            discount_count = 0;
        }
        hashmap_clear(check_life_map, 1);
        if(discount_count >= 3){
            char * master_device_no = get_master_device_sn();
            int master_index = 0;
            if(ends_with(master_device_no,"_1")){
                master_index = 1;
                QM_ESL2_LOG("master_device_no is 1 ... %s,%d\n",master_device_no,master_index);
            }else{
                master_index = 2;
                QM_ESL2_LOG("master_device_no is 2 ... %s,%d\n",master_device_no,master_index);
            }
            QM_ESL2_LOG("check_life_thread no device connected or only master left, rebooting...\n");
            if(master_index == 1){
                lv_async_call(discount_restart_device_1,NULL);
            }else{
                lv_async_call(discount_restart_device_2,NULL);
            }
        }

        int esl_type = get_esl_type();
        char *device_no = (char *)arg;
        char * token = get_token();
        int http_result = push_online_report2(device_no,all_device_sn,ssid,ip,version,esl_type,token);
        if(http_result != 0){
            http_count++;
            if(http_count >= 3){
                char * master_device_no = get_master_device_sn();
                int master_index = 0;
                if(ends_with(master_device_no,"_1")){
                    master_index = 1;
                    QM_ESL2_LOG("master_device_no is 1 ... %s,%d\n",master_device_no,master_index);
                }else{
                    master_index = 2;
                    QM_ESL2_LOG("master_device_no is 2 ... %s,%d\n",master_device_no,master_index);
                }
                QM_ESL2_LOG("check_life_thread no device connected, rebooting...\n");
                if(master_index == 1){
                    lv_async_call(discount_restart_device_1,NULL);
                }else{
                    lv_async_call(discount_restart_device_2,NULL);
                }
            }
        }else{
            http_count = 0;
        }
        free(all_device_sn); // 释放动态分配的内存
        sleep(5);
        
        char * master_device_no = get_master_device_sn();

        //===========================push id list 1 start===================================
        pthread_mutex_lock(&push_message_mutex_1);
        char *push_id_list_1 = map_keys_to_json_array_dto("received",push_id_map_1);
        hashmap_clear(push_id_map_1, 1);
        pthread_mutex_unlock(&push_message_mutex_1);
        if(!is_string_empty(push_id_list_1)){
            push_msgack_message(master_device_no, push_id_list_1,ssid,ip,version,esl_type,token);
        }
        free(push_id_list_1);
        //===========================push id list 1 end===================================

        //===========================push id list 2 start===================================
        pthread_mutex_lock(&push_message_mutex_2);
        char *push_id_list_2 = map_keys_to_json_array_dto("forward",push_id_map_2);
        hashmap_clear(push_id_map_2, 1);
        pthread_mutex_unlock(&push_message_mutex_2);
        if(!is_string_empty(push_id_list_2)){
            push_msgack_message(master_device_no, push_id_list_2,ssid,ip,version,esl_type,token);
        }
        free(push_id_list_2);
        //===========================push id list 2 end===================================

        //===========================push id list 3 start===================================
        pthread_mutex_lock(&push_message_mutex_3);
        char *push_id_list_3 = map_keys_to_json_array_dto("success",push_id_map_3);
        hashmap_clear(push_id_map_3, 1);
        pthread_mutex_unlock(&push_message_mutex_3);
        if(!is_string_empty(push_id_list_3)){
            push_msgack_message(master_device_no, push_id_list_3,ssid,ip,version,esl_type,token);
        }
        free(push_id_list_3);
        //===========================push id list 3 end===================================
        sleep(130);
    }
}

static void _put_group_map(char * key,char *value,struct hashmap * map){
    struct esl_socket_t * user_copy = hashmap_get(map, &(struct esl_socket_t){.key = key});
    if(user_copy != NULL){
        if(strcmp(user_copy->device_host,value) != 0){
            free(user_copy->device_host);
            user_copy->device_host = strdup(value);
        }
    }else{
        struct esl_socket_t *socket_instance_copy = (struct esl_socket_t *)malloc(sizeof(struct esl_socket_t));
        if (socket_instance_copy == NULL) {
            perror("Failed to allocate memory");
            return;
        }
        socket_instance_copy->key = strdup(key);
        socket_instance_copy->device_host = strdup(value);
        hashmap_set(map, socket_instance_copy);
        free(socket_instance_copy);
    }
}

void put_group_map(char * key,char *value,int index){
    if(index == 1){
        _put_group_map(key,value,group_map_1);
    }else if(index == 2){
        _put_group_map(key,value,group_map_2);
    }
}

static void _put_push_message(char * key,char *value,struct hashmap * map){
    QM_ESL2_LOG("put_push_message key : %s, value: %s ,map :%p  ====================================\n",key,value,map);
    struct esl_socket_t * user_copy = hashmap_get(map, &(struct esl_socket_t){.key = key});
    if(user_copy != NULL){
        if(strcmp(user_copy->device_host,value) != 0){
            free(user_copy->device_host);
            user_copy->device_host = strdup(value);
        }
    }else{
        struct esl_socket_t *socket_instance_copy = (struct esl_socket_t *)malloc(sizeof(struct esl_socket_t));
        if (socket_instance_copy == NULL) {
            perror("Failed to allocate memory");
            return;
        }
        socket_instance_copy->key = strdup(key);
        socket_instance_copy->device_host = strdup(value);
        hashmap_set(map, socket_instance_copy);
        free(socket_instance_copy);
    }
}

void post_push_message(const char * type,cJSON* json){
    if(json == NULL){
        return;
    }
    cJSON *pushIdItem = cJSON_GetObjectItem(json, "message_id");
    if(pushIdItem == NULL || !cJSON_IsString(pushIdItem)){
        QM_ESL2_LOG("post_push_message get push id fail!\n");
        return;
    }
    char *push_id = strdup(pushIdItem->valuestring);
    if(strcmp(type,"1") == 0){
        pthread_mutex_lock(&push_message_mutex_1);
        _put_push_message(push_id,"1",push_id_map_1);
        pthread_mutex_unlock(&push_message_mutex_1);
    }else if(strcmp(type,"2") == 0){
        pthread_mutex_lock(&push_message_mutex_2);
        _put_push_message(push_id,"2",push_id_map_2);
        pthread_mutex_unlock(&push_message_mutex_2);
    }else if(strcmp(type,"3") == 0){
        pthread_mutex_lock(&push_message_mutex_3);
        _put_push_message(push_id,"3",push_id_map_3);
        pthread_mutex_unlock(&push_message_mutex_3);
    }else{
        QM_ESL2_LOG("post_push_message type is not match!\n");
    }
    free(push_id);
}

void post_push_message2(const char * type,esl_message_model_t * message_model){
    if(message_model == NULL){
        return;
    }
    char *push_id = strdup(message_model->message_id);
    if(strcmp(type,"1") == 0){
        pthread_mutex_lock(&push_message_mutex_1);
        _put_push_message(push_id,"1",push_id_map_1);
        pthread_mutex_unlock(&push_message_mutex_1);
    }else if(strcmp(type,"2") == 0){
        pthread_mutex_lock(&push_message_mutex_2);
        _put_push_message(push_id,"2",push_id_map_2);
        pthread_mutex_unlock(&push_message_mutex_2);
    }else if(strcmp(type,"3") == 0){
        pthread_mutex_lock(&push_message_mutex_3);
        _put_push_message(push_id,"3",push_id_map_3);
        pthread_mutex_unlock(&push_message_mutex_3);
    }else{
        QM_ESL2_LOG("post_push_message type is not match!\n");
    }
    free(push_id);
}

#endif
