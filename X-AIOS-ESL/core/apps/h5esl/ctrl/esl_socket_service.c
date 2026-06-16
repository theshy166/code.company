#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
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
#include "../conf/conf.h"
#include "../utils/utils.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "logo_img.h"
#endif
#ifndef CONFIG_QM_VERSION
#define CONFIG_QM_VERSION "V1.00.00"
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
    printf("=====================cancel_current_listen_socket entry===========================\n");
    is_running = 0;
    //close(sockfd);
    printf("=====================cancel_current_listen_socket exit===========================\n");
    return NULL;
}

static void * process_server_response(void *arg) {
    prctl(PR_SET_NAME,"process_server_response"); // 设置线程名称
    char *response = (char *)arg;
    printf("Processing server response: %s\n", response);
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        printf("Failed to parse JSON response\n");
        free(response); // 释放内存
        return NULL;
    }
    // {"status":"success","type":2,"push_id":"25046866"}
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (status == NULL || !cJSON_IsString(status)) {
        printf("Invalid JSON response: missing or invalid 'status'\n");
        cJSON_Delete(json);
        free(response); // 释放内存
        return NULL;
    }
    char *status_value = status->valuestring;
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    if (type == NULL || !cJSON_IsNumber(type)) {
        printf("Invalid JSON response: missing or invalid 'type'\n");
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
        printf("Invalid JSON response: missing or invalid 'push_id'\n");
        cJSON_Delete(json);
        free(response); // 释放内存
        return NULL;
    }
    char *push_id_value = push_id->valuestring;
    printf("Server response: status=%s, type=%d, push_id=%s\n", status_value, type_value, push_id_value);
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
            printf("Response sent ip : %s ,msg: %s, group_id:%d\n",device_ip, response,group_id);
            create_send_socket_slave(device_ip,response);
        }
        printf("Response sent: %s\n", response);
    #endif
}

void *socket_listener(void *arg) {
    int sockfd = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    prctl(PR_SET_NAME, "consume_tcp_message"); // 设置线程名称
    printf("socket_listener start\n");
    while (1) {
        usleep(100*1000);
        int client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        printf("client_sock: %d\n", client_sock);
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
        printf("Received message: %s\n", buffer);
        close(client_sock);
        
        cJSON *result_json = cJSON_Parse(buffer);
        if(!result_json){
            printf("socket_listener convert json fail!\n");
            free(buffer);
            continue;
        }

        cJSON *dateTypeItem = cJSON_GetObjectItem(result_json, "date_type");
        if(dateTypeItem == NULL || !cJSON_IsNumber(dateTypeItem)){
            LV_LOG_USER("socket_listener get dt fail!\n");
            cJSON_Delete(result_json);
            free(buffer);
            continue;
        }
        int dt = dateTypeItem->valueint;

        char *push_id = NULL;
        if(dt != 999){
            cJSON *pushIdItem = cJSON_GetObjectItem(result_json, "push_id");
            if(pushIdItem == NULL || !cJSON_IsString(pushIdItem)){
                LV_LOG_USER("socket_listener get push_id fail!\n");
                cJSON_Delete(result_json);
                free(buffer);
                continue;
            }
            push_id = strdup(pushIdItem->valuestring);
        }

        cJSON *deviceNoItem = cJSON_GetObjectItem(result_json, "device_no");
        if(deviceNoItem == NULL || !cJSON_IsString(deviceNoItem)){
            LV_LOG_USER("socket_listener get device_no fail!\n");
            if(push_id != NULL){
                free(push_id);
            }
            cJSON_Delete(result_json);
            free(buffer);
            continue;
        }
        char *deviceNo = strdup(deviceNoItem->valuestring);

        int index = 0;
        if(ends_with(deviceNo,"_1")){
            index = 1;
        }else{
            index = 2;
        }
        int group_id = get_local_group_id(index);
        char * device_sn = get_device_sn(index);
        
        if(dt != 999 && dt != 990){
            if(is_string_empty(device_sn)){
                LV_LOG_USER("device_sn is empty! index:%d\n",index);
                send_response(index, group_id, "error", dt, push_id,deviceNo);
                if(push_id != NULL){
                    free(push_id);
                }
                cJSON_Delete(result_json);
                free(buffer);
                free(deviceNo);
                continue;
            }
            if(strcmp(deviceNo,device_sn) != 0){
                printf("deviceNo:%s,device_sn:%s",deviceNo,device_sn);
                send_response(index, group_id, "error", dt, push_id,deviceNo);
                if(push_id != NULL){
                    free(push_id);
                }
                cJSON_Delete(result_json);
                free(buffer);
                free(deviceNo);
                continue;
            }
        }
        free(deviceNo);
        switch (dt){
            case 1:
                {
                    //设备信息修改
                    /* {"device_no":"D1S920H73ODI667_2","date_type":1,"device_setting":"{\"SN\":\"D1S920H73ODI667_2\",\"pos_ip\":\"\",\"pos_id\":\"\",\"server_ip\":\"\",\"speech_play\":\"0\",\"store_name\":\"价签工厂测试\",\"store_id\":\"000000000002185\",\"show_msg\":\"您已进入监控区域\",\"pos_version\":\"\",\"video_muted\":\"0\",\"show_led_id\":\"07\",\"device_tpl_id\":\"7007\",\"swiper_time\":\"10\",\"other_1\":\"\",\"other_2\":\"clound\",\"other_3\":\"21028,78965\",\"other_4\":\"sale_price\",\"other_5\":\"\",\"other_6\":\"\",\"merchant_id\":1602,\"local_up_tag\":\"0\",\"branch_no\":\"ALL\",\"branch_name\":\"所有门店\",\"wifi_SSID\":\"MAOESL\",\"wifi_pwd\":\"a12345678B.\",\"screen_direction\":\"portrait-primary\",\"video_url\":\"\",\"wxwork_role\":\"0\"}","other_function":"","device_tpl_id":7007,"device_model":"D1S920H73ODI667_2号屏","agent_id":62,"device_md5":"f872984803a0f013887b1c7c6c6a24ee","push_id":"250233861"} */
                    /* cJSON *data = cJSON_GetObjectItem(result_json, "device_setting");
                    esl_device_setting_t * device_setting = parse_device_setting(data);
                    set_device_setting(device_setting,index); */
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
                                set_local_group_id_1(data->group_id);
                                group_id = data->group_id;
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
                                set_local_group_id_2(data->group_id);
                                group_id = data->group_id;
                            }
                            free_esl_push_server_data(data);
                        }
                    }
                    send_response(index, group_id, "success", dt, push_id,device_sn);
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
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        LV_LOG_USER("mqtt_subscriber get batch fail!\n");
                        cJSON_Delete(result_json);
                        free(buffer);
                        continue;
                    }

                    int batch = batch_data->valueint;
                    if(batch != 1){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        LV_LOG_USER("mqtt_subscriber batch = %d\n", batch);
                        cJSON_Delete(result_json);
                        free(buffer);
                        continue;
                    }
                    time_t current_time = time(NULL);
                    if(index == 1){
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_1) <= 5) {
                            send_response(index, group_id, "success", dt, push_id,device_sn);
                            if(push_id != NULL){
                                free(push_id);
                            }
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            free(buffer);
                            continue;
                        }
                        last_message_time_1 = current_time;
                    }else{
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_2) <= 5) {
                            send_response(index, group_id, "success", dt, push_id,device_sn);
                            if(push_id != NULL){
                                free(push_id);
                            }
                            cJSON_Delete(result_json);
                            LV_LOG_USER("mqtt_subscriber: 忽略本次消息,时间间隔小于5秒\n");
                            free(buffer);
                            continue;
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
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    
                    //模板商品数据
                    int count = 0;
                    char * product_info_sign = generate_sign(device_sn,"item");
                    esl_product_info_t ** product_info_list = query_product_infos(device_sn,"item",product_info_sign,&count);
                    free(product_info_sign);
                    if(product_info_list == NULL || count == 0){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }

                    //同步到本地
                    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                    if (args == NULL) {
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_product_info_array(product_info_list,count);
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        free(buffer);
                        printf("Failed to allocate memory for thread arguments\n");
                        clear_green_led(index);
                        continue;
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
                    send_response(index, group_id, "success", dt, push_id,device_sn);
                    break;
                }
            case 3:
                {
                    time_t current_time = time(NULL);
                    if(index == 1){
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_1) <= 5) {
                            send_response(index, group_id, "success", dt, push_id,device_sn);
                            if(push_id != NULL){
                                free(push_id);
                            }
                            cJSON_Delete(result_json);
                            LV_LOG_USER("esl_socket_service: 忽略本次消息,时间间隔小于5秒\n");
                            free(buffer);
                            continue;
                        }
                        last_message_time_1 = current_time;
                    }else{
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_2) <= 5) {
                            send_response(index, group_id, "success", dt, push_id,device_sn);
                            if(push_id != NULL){
                                free(push_id);
                            }
                            cJSON_Delete(result_json);
                            LV_LOG_USER("esl_socket_service: 忽略本次消息,时间间隔小于5秒\n");
                            free(buffer);
                            continue;
                        }
                        last_message_time_2 = current_time;
                    }
                    show_green_led(index);
                    

                    //TODO 临时解决方案，后续需要优化
                    char * template_sign = generate_sign(device_sn,"tpl");
                    esl_show_model_t *model = query_product_template(device_sn,"tpl",template_sign);
                    free(template_sign);
                    if(model == NULL){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }

                    char * setting_sign = generate_sign(device_sn,"setting");
                    esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                    free(setting_sign);
                    if (device_setting == NULL) {
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    
                    //模板商品数据
                    int count;
                    cJSON *dataItem = cJSON_GetObjectItem(result_json, "item");
                    esl_product_info_t ** product_info_list = parse_product_infos(dataItem,&count);
                    if(product_info_list == NULL || count == 0){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    
                    //同步到本地
                    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                    if (args == NULL) {
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        free_esl_product_info_array(product_info_list,count);
                        printf("Failed to allocate memory for thread arguments\n");
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
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
                    send_response(index, group_id, "success", dt, push_id,device_sn);
                    break;
                }
            case 4:
                {
                    time_t current_time = time(NULL);
                    if(index == 1){
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_1) <= 5) {
                            send_response(index, group_id, "success", dt, push_id, device_sn);
                            if(push_id != NULL){
                                free(push_id);
                            }
                            cJSON_Delete(result_json);
                            LV_LOG_USER("esl_socket_service: 忽略本次消息,时间间隔小于5秒\n");
                            free(buffer);
                            continue;
                        }
                        last_message_time_1 = current_time;
                    }else{
                        // 检查时间间隔是否小于5秒
                        if (difftime(current_time, last_message_time_2) <= 5) {
                            send_response(index, group_id, "success", dt, push_id,device_sn);
                            if(push_id != NULL){
                                free(push_id);
                            }
                            cJSON_Delete(result_json);
                            LV_LOG_USER("esl_socket_service: 忽略本次消息,时间间隔小于5秒\n");
                            free(buffer);
                            continue;
                        }
                        last_message_time_2 = current_time;
                    }
                    show_green_led(index);
                    //模板商品数据修改 TODO 临时解决方案，后续需要优化
                    char * template_sign = generate_sign(device_sn,"tpl");
                    esl_show_model_t *model = query_product_template(device_sn,"tpl",template_sign);
                    free(template_sign);
                    if(model == NULL){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }

                    char * setting_sign = generate_sign(device_sn,"setting");
                    esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                    free(setting_sign);
                    if (device_setting == NULL) {
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    
                    int count,old_count;
                    cJSON *dataItem = cJSON_GetObjectItem(result_json, "item");
                    char * product_info_sign = generate_sign(device_sn,"item");
                    esl_product_info_t ** old_product_info_list = query_product_infos(device_sn,"item",product_info_sign,&old_count);
                    free(product_info_sign);
                    if(old_product_info_list == NULL || old_count == 0){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_device_setting(device_setting);
                        free_esl_show_model(model);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    esl_product_info_t ** product_info_list = parse_product_infos(dataItem,&count);
                    update_product_info_batch(product_info_list,count,old_product_info_list,old_count);
                    
                    //同步到本地
                    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                    if (args == NULL) {
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free_esl_show_model(model);
                        free_device_setting(device_setting);
                        free_esl_product_info_array(old_product_info_list,old_count);
                        printf("Failed to allocate memory for thread arguments\n");
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    args->index = index;
                    args->model = model;
                    args->product_info_list = old_product_info_list;
                    args->product_info_count = old_count;
                    args->device_setting = device_setting;
                    args->device_sn = device_sn;
                    save_data_thread(args);

                    download_image_or_video(model,device_setting,old_product_info_list,old_count,index);

                    //防止下载视频过大 超过时间，所以重置下时间
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
                    send_response(index, group_id, "success", dt, push_id,device_sn);
                    break;
                }
            case 5:
                //暂时没有
                send_response(index, group_id, "success", dt, push_id,device_sn);
                break;
            case 6:
                //wife修改
                {
                    //wife修改
                    //{"device_no":"D1S920H73ODI667_1","date_type":6,"wifi_SSID":"aaa","wifi_pwd":"bbb","device_md5":"3a5b03322680b7d0262adfa487e086b4","push_id":"25
                    cJSON *wifiSSIDItem = cJSON_GetObjectItem(result_json, "wifi_SSID");
                    if(wifiSSIDItem == NULL || !cJSON_IsString(wifiSSIDItem)){
                        //send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(result_json);
                        LV_LOG_USER("esl_socket_service get wifi_ssid fail!\n");
                        free(buffer);
                        continue;
                    }
                    char *wifi_SSID = strdup(wifiSSIDItem->valuestring);
    
                    cJSON *wifiPwdItem = cJSON_GetObjectItem(result_json, "wifi_pwd");
                    if(wifiPwdItem == NULL || !cJSON_IsString(wifiPwdItem)){
                        //send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free(wifi_SSID);
                        cJSON_Delete(result_json);
                        LV_LOG_USER("esl_socket_service get wifi_pwd fail!\n");
                        free(buffer);
                        continue;
                    }
                    char *wifi_pwd = strdup(wifiPwdItem->valuestring);
                    show_green_led(index);
                    char cmd[100]={0};
                    snprintf(cmd,sizeof(cmd),"wifi_start.sh %s %s",wifi_SSID,wifi_pwd,device_sn);
                    printf("run: %s",cmd);
                    int ret = system(cmd);
                    printf("process %s ,ret:%d",cmd,ret);
                    free(wifi_SSID);
                    free(wifi_pwd);
                    //send_response(index, group_id, "success", dt, push_id,device_sn);
                    sleep(3);
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
                send_response(index, group_id, "success", dt, push_id,device_sn);
                break;
            case 8:
                //广告添加
                send_response(index, group_id, "success", dt, push_id,device_sn);
                break;
            case 9:
                //广告删除
                send_response(index, group_id, "success", dt, push_id,device_sn);
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
                        LV_LOG_USER("esl_socket_service get config fail!\n");
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    char *config = configItem->valuestring;

                    cJSON *config_json = cJSON_Parse(config);
                    if(config_json == NULL){
                        LV_LOG_USER("esl_socket_service convert config json fail!\n");
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    cJSON *typeItem = cJSON_GetObjectItem(config_json, "type");
                    if(typeItem == NULL || !cJSON_IsString(typeItem)){
                        LV_LOG_USER("esl_socket_service get type fail!\n");
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(config_json);
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    char *type = typeItem->valuestring;

                    LV_LOG_USER("esl_socket_service config:%s\n",config);
                    if(strcmp(type,"Restart") == 0){
                        //设备重启
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
                                send_response(index, group_id, "success", dt, push_id,device_sn);
                                if(push_id != NULL){
                                    free(push_id);
                                }
                                free_esl_product_info_array(product_info_list,product_count);
                                cJSON_Delete(config_json);
                                cJSON_Delete(result_json);
                                free(buffer);
                                clear_green_led(index);
                                continue;
                            }
                            char * setting_sign = generate_sign(device_sn,"setting");
                            esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
                            free(setting_sign);
                            if (device_setting == NULL) {
                                send_response(index, group_id, "success", dt, push_id,device_sn);
                                if(push_id != NULL){
                                    free(push_id);
                                }
                                free_esl_show_model(model);
                                free_esl_product_info_array(product_info_list,product_count);
                                cJSON_Delete(config_json);
                                cJSON_Delete(result_json);
                                free(buffer);
                                clear_green_led(index);
                                continue;
                            }

                            esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
                            if (args == NULL) {
                                send_response(index, group_id, "success", dt, push_id,device_sn);
                                if(push_id != NULL){
                                    free(push_id);
                                }
                                free_device_setting(device_setting);
                                free_esl_show_model(model);
                                free_esl_product_info_array(product_info_list,product_count);
                                cJSON_Delete(config_json);
                                cJSON_Delete(result_json);
                                printf("Failed to allocate memory for thread arguments\n");
                                free(buffer);
                                clear_green_led(index);
                                continue;
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
                        //send_response(index, group_id, "success", dt, push_id,device_sn);
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
                    send_response(index, group_id, "success", dt, push_id,device_sn);
                    cJSON_Delete(config_json);
                    break;
                }
            case 11:
                //促销商品数据
                send_response(index, group_id, "success", dt, push_id,device_sn);
                break;
            case 12:
                //本地ip配置
                send_response(index, group_id, "success", dt, push_id,device_sn);
                break;
            case 13:
                //热点名称
                send_response(index, group_id, "success", dt, push_id,device_sn);
                break;
            case 14:
                //分组修改
                {
                    show_green_led(index);
                    cJSON *groupIdItem = cJSON_GetObjectItem(result_json, "group_id");
                    if(groupIdItem == NULL || !cJSON_IsNumber(groupIdItem)){
                        send_response(index, group_id, "success", dt, push_id,device_sn);
                        if(push_id != NULL){
                            free(push_id);
                        }
                        LV_LOG_USER("esl_socket_service get group_id fail!\n");
                        cJSON_Delete(result_json);
                        free(buffer);
                        clear_green_led(index);
                        continue;
                    }
                    send_response(index, group_id, "success", dt, push_id,device_sn);
                    sleep(3);
                    system("reboot");
                }
                break;
            case 990:
                {
                    char * recv_message = strdup(buffer);
                    pthread_t thread_id;
                    if (pthread_create(&thread_id, NULL, process_server_response, recv_message) != 0) {
                        perror("pthread_create");
                        free(recv_message); // 释放内存
                        continue;
                    }
                    pthread_detach(thread_id);
                }
                break;
            case 999:
                {
                    //连接主设备
                    cJSON *deviceNoItem = cJSON_GetObjectItem(result_json, "device_no");
                    if(deviceNoItem == NULL || !cJSON_IsString(deviceNoItem)){
                        printf("socket_listener get device_no fail!\n");
                        if(push_id != NULL){
                            free(push_id);
                        }
                        cJSON_Delete(result_json);
                        free(buffer);
                        continue;
                    }
                    char *deviceNo = strdup(deviceNoItem->valuestring);

                    cJSON *deviceHostItem = cJSON_GetObjectItem(result_json, "device_host");
                    if(deviceHostItem == NULL || !cJSON_IsString(deviceHostItem)){
                        printf("socket_listener get device_no fail!\n");
                        if(push_id != NULL){
                            free(push_id);
                        }
                        free(deviceNo);
                        cJSON_Delete(result_json);
                        free(buffer);
                        continue;
                    }
                    char *deviceHost = strdup(deviceHostItem->valuestring);
                    //在这之前 需要判断下是否发生过变化，如果发生过变化 则需要释放老的内存，如果没有发生变化 则忽略本次操作 TODO
                    #if 1
                    struct esl_socket_t * user = hashmap_get(map, &(struct esl_socket_t){.key = deviceNo});
                    if(user != NULL){
                        if(strcmp(user->device_host,deviceHost) != 0){
                            free(user->device_host);
                            user->device_host = strdup(deviceHost);
                        }
                    }else{
                        struct esl_socket_t *socket_instance = (struct esl_socket_t *)malloc(sizeof(struct esl_socket_t));
                        if (socket_instance == NULL) {
                            perror("Failed to allocate memory");
                            if(push_id != NULL){
                                free(push_id);
                            }
                            free(deviceNo);
                            free(deviceHost);
                            cJSON_Delete(result_json);
                            free(buffer);
                            continue;
                        }
                        socket_instance->key = strdup(deviceNo);
                        socket_instance->device_host = strdup(deviceHost);
                        hashmap_set(map, socket_instance);
                        free(socket_instance);
                    }
                    if(check_life_map != NULL){
                        //在这之前 需要判断下是否发生过变化，如果发生过变化 则需要释放老的内存，如果没有发生变化 则忽略本次操作 TODO
                        struct esl_socket_t * user_copy = hashmap_get(check_life_map, &(struct esl_socket_t){.key = deviceNo});
                        if(user_copy != NULL){
                            if(strcmp(user_copy->device_host,deviceHost) != 0){
                                free(user_copy->device_host);
                                user_copy->device_host = strdup(deviceHost);
                            }
                        }else{
                            struct esl_socket_t *socket_instance_copy = (struct esl_socket_t *)malloc(sizeof(struct esl_socket_t));
                            if (socket_instance_copy == NULL) {
                                perror("Failed to allocate memory");
                                if(push_id != NULL){
                                    free(push_id);
                                }
                                free(deviceNo);
                                free(deviceHost);
                                cJSON_Delete(result_json);
                                free(buffer);
                                continue;
                            }
                            socket_instance_copy->key = strdup(deviceNo);
                            socket_instance_copy->device_host = strdup(deviceHost);
                            hashmap_set(check_life_map, socket_instance_copy);
                            free(socket_instance_copy);
                        }
                    }
                    #endif
                    free(deviceNo);
                    free(deviceHost);
                }
                break;
            default:
                {
                    send_response(index, group_id, "success", 0, push_id,device_sn);
                }
                break;
        }
        if(push_id != NULL){
            free(push_id);
        }
        cJSON_Delete(result_json);
        free(buffer);
    }
    printf("==============================exit socket_listener ====================================\n");
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
    printf("create_listen_socket sin_port: %d\n",OPEN_HOST);
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
    printf("=====================Thread finished===========================\n");
    close(sockfd);
    return 0;
}

int reconnect_socket_listener() {

    while(!is_close_finish){
        printf("reconnect_socket_listener wait close finish\n");
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

    printf("reconnect_socket_listener sin_port: %d\n",OPEN_HOST);
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
    printf("=====================Thread finished===========================\n");
    close(sockfd);
    return 0;
}

int create_send_socket_slave(const char * ip,const char * message){
    // 加锁
    //printf("create_send_socket_slave ip: %s,message:%s\n",ip, message);
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
    printf("create_send_socket_slave sin_port: %d\n",OPEN_HOST);
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
        printf("Connection failed, ip:%s\n",ip);
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
	//printf("Message sent: %s\n", message);

	// 接收服务器的响应
    /* int valread = read(sock, buffer, sizeof(buffer) - 1); // 读取响应
    if (valread > 0) {
        buffer[valread] = '\0'; // 确保字符串以 null 结尾
        char * recv_message = strdup(buffer);
        printf("========================Server response: %s\n", recv_message);
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
        printf("======================Server closed the connection.\n");
    } else {
        perror("=======================Error reading server response");
    } */

	// 关闭 socket
    printf("Message sent to %s: %s\n", ip, message);

	close(sock);
    // 解锁
    //pthread_mutex_unlock(&create_send_socket_slave_mutex);
	return 0;
}

int create_send_socket(const char * device_no,const char *message) {
    //printf("device_no:%s,message:%s",device_no,message);
	struct esl_socket_t * user = hashmap_get(map, &(struct esl_socket_t){.key = device_no});
    if(user == NULL){
        return -1;
    }
	printf("key : %s,value:%s \n",user->key,user->device_host);
	create_send_socket_slave(user->device_host,message);
}

void* check_life_thread(void *arg) {
    _init_check_life_map();
    prctl(PR_SET_NAME, "check_life_thread"); // 设置线程名称
    while(1) {
        sleep(45);
        size_t iter = 0;
        void *item;
        size_t buffer_size = 1024;
        char *keys_buffer = (char *)malloc(buffer_size);
        if (keys_buffer == NULL) {
            perror("Failed to allocate memory");
            continue;
        }
        keys_buffer[0] = '\0'; // 初始化为空字符串

        while (hashmap_iter(check_life_map, &iter, &item)) {
            struct esl_socket_t *user = item;
            printf("check_life_thread key : %s, value: %s\n", user->key, user->device_host);

            size_t needed_size = strlen(keys_buffer) + strlen(user->key) + 2; // 1 for comma, 1 for null terminator
            if (needed_size > buffer_size) {
                buffer_size = needed_size * 2;
                keys_buffer = (char *)realloc(keys_buffer, buffer_size);
                if (keys_buffer == NULL) {
                    perror("Failed to reallocate memory");
                    continue;
                }
            }

            if (strlen(keys_buffer) > 0) {
                strcat(keys_buffer, ",");
            }
            strcat(keys_buffer, user->key);
        }

        printf("All keys: %s\n", keys_buffer); // 打印拼接后的 keys
        hashmap_clear(check_life_map, 1);

        char *device_no = (char *)arg;
        // 秒级时间戳
        time_t seconds;
        seconds = time(NULL);
        printf("Seconds: %ld\n", seconds);
        // 将 seconds 转换为字符串
        char time_str[20];
        snprintf(time_str, sizeof(time_str), "%ld", seconds);
        printf("Time string: %s\n", time_str);
        char *sign = generate_check_life_sign(device_no,"online_report",keys_buffer,time_str);
        printf("Sign: %s\n", sign);
        push_online_report(device_no,"online_report",keys_buffer,time_str,sign);
        free(sign);
        free(keys_buffer); // 释放动态分配的内存
        sleep(5);
        
        char * master_device_no = get_master_device_sn();

        //===========================push id list 1 start===================================
        pthread_mutex_lock(&push_message_mutex_1);
        size_t buffer_size_list_1 = 1024;
        size_t iter_list_1 = 0;
        char *push_id_list_1 = (char *)malloc(buffer_size_list_1);
        if (push_id_list_1 == NULL) {
            perror("Failed to allocate memory");
            continue;
        }
        push_id_list_1[0] = '\0'; // 初始化为空字符串

        while (hashmap_iter(push_id_map_1, &iter_list_1, &item)) {
            struct esl_socket_t *user = item;
            printf("check_life_thread key : %s, value: %s\n", user->key, user->device_host);

            size_t needed_size = strlen(push_id_list_1) + strlen(user->key) + 2; // 1 for comma, 1 for null terminator
            if (needed_size > buffer_size_list_1) {
                buffer_size_list_1 = needed_size * 2;
                push_id_list_1 = (char *)realloc(push_id_list_1, buffer_size_list_1);
                if (push_id_list_1 == NULL) {
                    perror("Failed to reallocate memory");
                    continue;
                }
            }

            if (strlen(push_id_list_1) > 0) {
                strcat(push_id_list_1, ",");
            }
            strcat(push_id_list_1, user->key);
        }
        hashmap_clear(push_id_map_1, 1);
        pthread_mutex_unlock(&push_message_mutex_1);
        if(!is_string_empty(push_id_list_1)){
            char *sign1 = generate_push_message_report(master_device_no,"push_report",push_id_list_1,"1",time_str);
            push_message_report(master_device_no,"push_report", push_id_list_1, "1", time_str, sign1);
            free(sign1);
        }
        free(push_id_list_1);
        //===========================push id list 1 end===================================

        //===========================push id list 2 start===================================
        pthread_mutex_lock(&push_message_mutex_2);
        size_t buffer_size_list_2 = 1024;
        size_t iter_list_2 = 0;
        char *push_id_list_2 = (char *)malloc(buffer_size_list_2);
        if (push_id_list_2 == NULL) {
            perror("Failed to allocate memory");
            continue;
        }
        push_id_list_2[0] = '\0'; // 初始化为空字符串

        while (hashmap_iter(push_id_map_2, &iter_list_2, &item)) {
            struct esl_socket_t *user = item;
            printf("check_life_thread key : %s, value: %s\n", user->key, user->device_host);

            size_t needed_size = strlen(push_id_list_2) + strlen(user->key) + 2; // 1 for comma, 1 for null terminator
            if (needed_size > buffer_size_list_2) {
                buffer_size_list_2 = needed_size * 2;
                push_id_list_2 = (char *)realloc(push_id_list_2, buffer_size_list_2);
                if (push_id_list_2 == NULL) {
                    perror("Failed to reallocate memory");
                    continue;
                }
            }

            if (strlen(push_id_list_2) > 0) {
                strcat(push_id_list_2, ",");
            }
            strcat(push_id_list_2, user->key);
        }
        hashmap_clear(push_id_map_2, 1);
        pthread_mutex_unlock(&push_message_mutex_2);
        if(!is_string_empty(push_id_list_2)){
            char *sign2 = generate_push_message_report(master_device_no,"push_report",push_id_list_2,"2",time_str);
            push_message_report(master_device_no,"push_report", push_id_list_2, "2", time_str, sign2);
            free(sign2);
        }
        free(push_id_list_2);
        //===========================push id list 2 end===================================

        //===========================push id list 3 start===================================
        pthread_mutex_lock(&push_message_mutex_3);
        size_t buffer_size_list_3 = 1024;
        size_t iter_list_3 = 0;
        char *push_id_list_3 = (char *)malloc(buffer_size_list_3);
        if (push_id_list_3 == NULL) {
            perror("Failed to allocate memory");
            continue;
        }
        push_id_list_3[0] = '\0'; // 初始化为空字符串

        while (hashmap_iter(push_id_map_3, &iter_list_3, &item)) {
            struct esl_socket_t *user = item;
            printf("check_life_thread key : %s, value: %s\n", user->key, user->device_host);

            size_t needed_size = strlen(push_id_list_3) + strlen(user->key) + 2; // 1 for comma, 1 for null terminator
            if (needed_size > buffer_size_list_3) {
                buffer_size_list_3 = needed_size * 2;
                push_id_list_3 = (char *)realloc(push_id_list_3, buffer_size_list_3);
                if (push_id_list_3 == NULL) {
                    perror("Failed to reallocate memory");
                    continue;
                }
            }

            if (strlen(push_id_list_3) > 0) {
                strcat(push_id_list_3, ",");
            }
            strcat(push_id_list_3, user->key);
        }
        hashmap_clear(push_id_map_3, 1);
        pthread_mutex_unlock(&push_message_mutex_3);
        if(!is_string_empty(push_id_list_3)){
            char *sign3 = generate_push_message_report(master_device_no,"push_report",push_id_list_3,"3",time_str);
            push_message_report(master_device_no,"push_report", push_id_list_3, "3", time_str, sign3);
            free(sign3);
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
    printf("put_push_message key : %s, value: %s ,map :%p  ====================================\n",key,value,map);
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
    cJSON *pushIdItem = cJSON_GetObjectItem(json, "push_id");
    if(pushIdItem == NULL || !cJSON_IsString(pushIdItem)){
        LV_LOG_USER("post_push_message get push id fail!\n");
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
        LV_LOG_USER("post_push_message type is not match!\n");
    }
    free(push_id);
}

#endif
