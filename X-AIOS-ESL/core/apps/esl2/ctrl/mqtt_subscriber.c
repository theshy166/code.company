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

/**
 * @brief ★ MQTT 消息订阅回调 — 项目中最核心的数据→UI 桥接函数
 *
 * ┌──────────────── 数据流全景图 ────────────────┐
 * │                                                │
 * │  云平台 (MQTT Broker)                           │
 * │    │ push 消息（模板更新/解绑/重启/WiFi...）     │
 * │    ▼                                            │
 * │  mqtt_control.c (MQTT Client 连接管理)          │
 * │    │ AiMessage 结构体                            │
 * │    ▼                                            │
 * │  mqtt_subscriber() [★ 本函数 — 非 LVGL 线程]   │
 * │    │                                            │
 * │    ├─ 1. JSON 解析 → esl_message_model_t       │
 * │    ├─ 2. 判断设备归属（主/从）                   │
 * │    ├─ 3. 根据 type 分发：                       │
 * │    │   ├─ CONTENT → 模板更新 → lv_async_call    │
 * │    │   ├─ RESTRAT → 设备重启                    │
 * │    │   ├─ REFRESH → 刷新模板                    │
 * │    │   ├─ UNBIND  → 解绑清数据                  │
 * │    │   ├─ WIFI    → 切换WiFi                    │
 * │    │   ├─ GROUP   → 分组修改                    │
 * │    │   ├─ UPDATE  → OTA升级                     │
 * │    │   └─ SCREENSWITCH → 屏幕开关               │
 * │    │                                            │
 * │    ▼                                            │
 * │  lv_async_call(refresh_ui_N / delete_ui_*)      │
 * │    │ ★ 线程边界：非 LVGL 线程 → LVGL 主线程     │
 * │    ▼                                            │
 * │  [LVGL 主线程] → 更新 UI → fbdev_flush() 刷屏  │
 * └────────────────────────────────────────────────┘
 *
 * 关键设计要点：
 *   1. 本函数运行在 MQTT 接收线程（非 LVGL 线程），不能直接操作 lv_obj
 *   2. 所有 UI 更新都通过 lv_async_call() 投递到 LVGL 主循环
 *   3. 用 s_mqtt_subscriber_mutex_1/2 保护并发（暂未大量使用）
 *   4. 消息上报链路：post_push_message2("1"接收 → "2"转发 → "3"子设备确认)
 *
 * @param params MQTT 消息，包含 val(网络状态) 和 data2(消息JSON字符串)
 */
//mqtt 订阅回调函数
void mqtt_subscriber(const AiMessage* params)
{
    // ========== A. 日志输出 ==========
    QM_ESL2_LOG("mqtt_subscriber net state =%d \n",params->val );     // val: MQTT 连接状态
    QM_ESL2_LOG("mqtt_subscriber message =%s \n",params->data2 );     // data2: 消息 JSON 字符串

    // ========== B. 解析消息为 esl_message_model_t ==========
    char *message = params->data2;
    esl_message_model_t * message_model = the_second_to_message_model(message);
    // message_model 字段：message_id, device_no, type, data, ignore
    if(message_model == NULL){
        QM_ESL2_LOG("mqtt_subscriber convert dto fail!\n");
        return;  // JSON 解析失败，丢弃
    }

    // ========== C. 确定目标屏幕索引 ==========
    // 设备SN命名规则：{base_sn}_1 = 屏幕1, {base_sn}_2 = 屏幕2
    char *deviceNo = message_model -> device_no;  // 消息目标设备SN
    int index = 0;
    if(ends_with(deviceNo,"_1")){
        index = 1;  // 目标是屏幕1
    }else{
        index = 2;  // 目标是屏幕2
    }

    // ========== D. 判断本机是否为主设备 ==========
    char * master_device_no = get_master_device_sn();  // 本机的主设备SN
    QM_ESL2_LOG("mqtt_subscriber deviceNo:%s master_device_no:%s\n",deviceNo,master_device_no);
    int master_index = 0;
    if(ends_with(master_device_no,"_1")){
        master_index == 1;   // 【BUG】应为 = 而非 == ，但逻辑影响不大
    }else{
        master_index == 2;
    }

    int esl_type = get_esl_type();  // ESL 协议类型：1=旧协议, 2/3=新协议

    // ========== E. 消息状态上报：标记「主设备已收到」 ==========
    // push_type: 1=主设备收到, 2=主设备转发, 3=子设备确认
    post_push_message2("1",message_model);

    // ========== F. 是本机消息 还是 需要转发 ==========
    if(strcmp(deviceNo,master_device_no) == 0){
        // ★★★ 本机是目标设备 — 直接处理消息 ★★★

        char * device_sn = get_device_sn(index);  // 本机对应屏幕的SN

        // ----- F1. 检查 ignore 标志 -----
        int ignore = message_model -> ignore;
        if(ignore == 1){
            // ignore=1: 本消息已被处理过（去重），跳过
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);  // 确认收到
            }
            post_push_message2("2",message_model);  // 上报：已转发
            post_push_message2("3",message_model);  // 上报：子设备确认
            free_esl_message_model(message_model);
            QM_ESL2_LOG("mqtt_subscriber get dt fail!\n");
            return;
        }

        // ----- F2. 根据消息类型分发处理 -----
        int type = message_model -> type;  // 见 esl_message_model.h 中的 EslMessageType 枚举
        QM_ESL2_LOG("mqtt_subscriber type = %d\n", type);

        if(type == EslMessageTypeCONTENT){
            // ============================================================
            // ★ 类型1: 模板内容更新 (EslMessageTypeCONTENT)
            // 服务器下发了新的商品模板 → 解析 → 下载图片 → 刷新UI
            // ============================================================

            // 模版更新
            char * data = message_model -> data;          // data 是 JSON 字符串
            cJSON *dataItem = cJSON_Parse(data);           // 解析为 cJSON 树
            if(dataItem == NULL){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                return;
            }

            // 检查是否包含解绑标志
            cJSON *unbind = cJSON_GetObjectItem(dataItem,"unbind");
            if(unbind && cJSON_IsNumber(unbind) && unbind->valueint == 1){
                // ★ 子情况：解绑 — 删除本地数据和UI
                if(esl_type == 2){
                    char * message_id = message_model->message_id;
                    push_msgack_message_2(device_sn,message_id);
                }
                set_show_green_led(index,1);   // 亮绿灯表示正在操作
                if(index == 1){
                    lv_async_call(delete_ui_and_local_data_1,NULL);  // ★ 投递到LVGL线程删UI
                }else{
                    lv_async_call(delete_ui_and_local_data_2,NULL);
                }
            }else{
                // ★ 正常流程：模板更新
                cJSON *contentItem = cJSON_GetObjectItem(dataItem,"content");  // content 字段
                if(contentItem == NULL && !cJSON_IsString(contentItem)){
                    cJSON_Delete(dataItem);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                    return;
                }
                char * content = strdup(contentItem->valuestring);  // 拷贝 content 字符串
                if(is_string_empty(content)){
                    if(content != NULL) free(content);
                    cJSON_Delete(dataItem);
                    free_esl_message_model(message_model);
                    QM_ESL2_LOG("mqtt_subscriber get content fail!\n");
                    return;
                }

                int esl_type = get_esl_type();
                if(esl_type == 2 || esl_type == 3){
                    // ----- ESL type 2/3: 新协议 — 直接解析为 esl_base_model_t -----
                    esl_base_model_t * model = parse_esl_base_model(content);
                    // 分配文件保存参数
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
                    save_data_thread(esl_file_save_info_args);  // 后台线程保存模板到本地文件

                    download_image_or_video(model,index);        // ★ 下载图片/视频资源

                    // ★ 打包参数 → lv_async_call → 在 LVGL 线程刷新 UI
                    show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
                    args->model = model;
                    args->index = index;

                    if(index == 1){
                        lv_async_call(refresh_ui_1,args);  // ★ 安全投递到LVGL主线程
                    }else{
                        lv_async_call(refresh_ui_2,args);
                    }
                    if(esl_type == 2){
                        char * message_id = message_model->message_id;
                        push_msgack_message_2(device_sn,message_id);
                    }
                }else{
                    // ----- ESL type 1: 旧协议 — 需要拼接 tpl_jn + tpl_pro -----
                    cJSON *json = cJSON_Parse(content);
                    if(json == NULL){
                        free(content);
                        cJSON_Delete(dataItem);
                        free_esl_message_model(message_model);
                        QM_ESL2_LOG("mqtt_subscriber parse content fail!\n");
                        return;
                    }
                    // 提取模板 JSON 的各个子字段
                    cJSON *tpl_jn = cJSON_GetObjectItem(json, "tpl_jn");      // 模板结构
                    cJSON *tpl_pro = cJSON_GetObjectItem(json, "tpl_pro");    // 商品数据
                    cJSON *tpl_txt_jn = cJSON_GetObjectItem(json, "tpl_txt_jn"); // 文本模板
                    cJSON *tpl_bg_url = cJSON_GetObjectItem(json, "tpl_bg_url"); // 背景图URL
                    cJSON *dsp_pro_num = cJSON_GetObjectItem(json, "dsp_pro_num"); // 显示商品数

                    if(tpl_jn && tpl_pro){
                        // 有完整模板：拼接模板+商品→Model
                        set_show_green_led(index,1);
                        int count = 0;
                        char * product_template = cJSON_PrintUnformatted(tpl_jn);
                        char * product_list = cJSON_PrintUnformatted(tpl_pro);
                        char * other_info_str = cJSON_PrintUnformatted(tpl_txt_jn);
                        char * temp = (tpl_bg_url && cJSON_IsString(tpl_bg_url) && tpl_bg_url->valuestring) ? strdup(tpl_bg_url->valuestring) : strdup("");
                        int display_product_num = cJSON_IsNumber(dsp_pro_num) ? dsp_pro_num->valueint : 0;

                        // ★ 拼接模板+商品数据 → esl_base_model_t
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

                        // ★ 投递到 LVGL 线程刷新 UI
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
                        // 无完整模板：仅用 content 创建 Model
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
            // ============================================================
            // ★ 类型4: 设备重启 (EslMessageTypeRESTRAT)
            // 服务器下发重启指令 → 回复确认 → 开线程执行重启
            // ============================================================
            //设备重启
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);  // 先回复服务器确认
            }
            // 开独立线程执行重启（不在 MQTT 线程中阻塞）
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, reboot_thread, NULL);
            pthread_detach(thread_id);

        }else if(type == EslMessageTypeREFRESH){
            // ============================================================
            // ★ 类型7: 刷新 (EslMessageTypeREFRESH)
            // 主动向服务器拉取最新模板 → 与 CONTENT 流程相同
            // 区别：REFRESH 是设备主动请求，CONTENT 是服务器推送
            // ============================================================
            //刷新
            char ip[NI_MAXHOST] = {0};
            get_local_ip(ip, NI_MAXHOST);            // 获取本机 IP 用于请求
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
            char * ssid = get_ssid_2();              // WiFi SSID
            int esl_type = get_esl_type();
            char * token = get_token();               // 认证 Token
            char * content = get_content_info(device_sn,ssid,ip,version,esl_type,token);
            // ★ 向服务器 HTTP 请求最新模板内容
            if(is_string_empty(content)){
                if(content != NULL) free(content);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get content fail!\n");
                return;
            }
            // 以下逻辑与 CONTENT 类型完全一致：
            // type 2/3 → parse_esl_base_model, type 1 → the_second_to_model
            if(esl_type == 2  || esl_type == 3){
                // ... (与 CONTENT type 2/3 相同的处理逻辑)
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
            // ============================================================
            // ★ 类型3: 设备解绑 (EslMessageTypeUNBIND)
            // 服务器下发解绑指令 → 删除本地所有数据和UI → 恢复到空状态
            // ============================================================
            //设备解绑 删除数据，还需要删除本地文件
            set_show_green_led(index,1);                // 亮绿灯表示正在操作
            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);  // 回复确认
            }
            if(index == 1){
                lv_async_call(delete_ui_and_local_data_1,NULL);  // ★ 在LVGL线程中删UI+本地文件
            }else{
                lv_async_call(delete_ui_and_local_data_2,NULL);
            }   
        }else if(type == EslMessageTypeWIFI){
            // ============================================================
            // ★ 类型2: WiFi 修改 (EslMessageTypeWIFI)
            // 服务器下发新的 WiFi SSID + 密码 → 写入配置 → 执行 wifi_start.sh → reboot
            // ============================================================
            //wifi修改
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL || !cJSON_IsObject(dataItem)){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get data fail!\n");
                return;
            }
            cJSON *wifiSSIDItem = cJSON_GetObjectItem(dataItem, "name");       // 新 WiFi SSID
            if(wifiSSIDItem == NULL || !cJSON_IsString(wifiSSIDItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get wifi_ssid fail!\n");
                return;
            }
            char *wifi_SSID = strdup(wifiSSIDItem->valuestring);

            cJSON *wifiPwdItem = cJSON_GetObjectItem(dataItem, "password");    // 新 WiFi 密码
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
            // ★ 通过 Shell 脚本切换 WiFi → 然后重启
            char cmd[100]={0};
            snprintf(cmd,sizeof(cmd),"wifi_start.sh %s %s",wifi_SSID,wifi_pwd);
            QM_ESL2_LOG("run: %s",cmd);
            int ret = system(cmd);       // 执行 wifi_start.sh 切换 WiFi 配置
            QM_ESL2_LOG("process %s ,ret:%d",cmd,ret);
            system("reboot");            // 切换后重启设备使 WiFi 生效

        }else if(type == EslMessageTypeGROUP){
            // ============================================================
            // ★ 类型5: 分组修改 (EslMessageTypeGROUP)
            // 服务器下发新分组 → 写入配置 → reboot 生效
            // 分组用于主设备管理从设备（同一分组内的设备共享广播域）
            // ============================================================
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
            cJSON *groupIdItem = cJSON_GetObjectItem(dataItem, "groupId");  // 新分组ID
            if(groupIdItem == NULL || !cJSON_IsNumber(groupIdItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get group_id fail!\n");
                set_show_green_led(index,0);
                return;
            }
            cJSON *groupNameItem = cJSON_GetObjectItem(dataItem, "groupName"); // 新分组名称
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
            system("reboot");  // 重启使分组生效（读取 /proc/cmdline 等配置）

        }else if(type == EslMessageTypeUPDATE){
            // ============================================================
            // ★ 类型6: OTA 升级 (EslMessageTypeUPDATE)
            // 服务器通知有新版本 → 检查版本 → downloader.sh 下载 → 重启
            // ============================================================
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
            // ★ 向服务器查询最新版本号
            QM_ESL2_LOG("get_version_info: %s\n", verBuffer);
            if(is_string_empty(verBuffer)){
                if(verBuffer) free(verBuffer);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber parse version fail!\n");
                return;
            }
            free(verBuffer);
            // ★ 执行下载脚本 → 成功后清缓存 → 重启
            char cmd[100]={0};
            snprintf(cmd,sizeof(cmd),"downloader.sh %s %s",CONFIG_QM_VERSION,CONFIG_PLATFORM_ID);
            QM_ESL2_LOG("run: %s",cmd);
            int ret =system(cmd);         // 执行下载脚本
            if(ret == 0){
                if(esl_type == 2){
                    char * message_id = message_model->message_id;
                    push_msgack_message_2(device_sn,message_id);
                }
                free_esl_message_model(message_model);
                QM_ESL2_LOG("run command success: %s\n", cmd);
                system("rm -rf /data/update.tar.gz");  // 清理升级包
                sleep(3);
                system("rm -rf /data/res/img /data/res/json /data/res/logo"); // 清理旧资源
                system("rm -rf /data/*core_dump*");     // 清理 core dump
                system("reboot");                       // ★ 重启以加载新版本
            }
        }else if(type == EslMessageTypeURL){
            // ============================================================
            // 类型100: 基础URL修改 (EslMessageTypeURL) — TODO 未实现
            // ============================================================
            //基础url修改 TODO
        }else if(type == EslMessageTypeSCREENSWITCH){
            // ============================================================
            // ★ 类型102: 屏幕开关 (EslMessageTypeSCREENSWITCH)
            // 服务器下发 value=1(开) 或 value=0(关) → 调节背光亮度
            // ============================================================
            //屏幕开关
            char * data = message_model -> data;
            cJSON *dataItem = cJSON_Parse(data);
            if(dataItem == NULL){
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get dataItem fail!\n");
                return;
            }
            cJSON *valueItem = cJSON_GetObjectItem(dataItem, "value");  // 1=开屏, 0=关屏
            if(valueItem == NULL || !cJSON_IsNumber(valueItem)){
                cJSON_Delete(dataItem);
                free_esl_message_model(message_model);
                QM_ESL2_LOG("mqtt_subscriber get valueItem fail!\n");
                return;
            }
            int value = valueItem->valueint;
            #if defined(CONFIG_XOS_HW_QUADRV) && !defined(BUILD_SIMULATOR)
                extern int qm_backlight_setbri(unsigned int brightness);  // 背光控制
                extern int qm_backlight_getbri(void);
                extern int xos_lcm_get_screen_num(void);

                if(value == 1){
                    // 开屏：根据屏幕数量恢复默认亮度（单屏40%, 双屏80%）
                    if (xos_lcm_get_screen_num() == 1) {
                        qm_backlight_setbri(40);
                    }
                    if (xos_lcm_get_screen_num() == 2) {
                        qm_backlight_setbri(80);
                    }
                }else{
                    qm_backlight_setbri(0);  // 关屏：亮度设为0
                }
            #endif
            cJSON_Delete(dataItem);

            if(esl_type == 2){
                char * message_id = message_model->message_id;
                push_msgack_message_2(device_sn,message_id);
            }
        }
        // ========== E2. 上报消息状态：标记「子设备已收到」 ==========
        post_push_message2("3",message_model);
    }else{
        // ==========================================================
        // ★★★ 本机不是目标设备 — 需要转发给从设备 ★★★
        // 主设备将消息通过 Socket 转发给从设备（同一局域网内的子屏幕）
        // ==========================================================
        //设备编码不匹配 转发消息
        char * socket_message = els_message_model_to_json(message_model);  // Model 转回 JSON
        QM_ESL2_LOG("mqtt_subscriber deviceNo is not match!\n");
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
            // 真机：通过消息队列发送（走 Socket TCP）
            post_msg_to_queue(1, deviceNo,socket_message);
        #else
            // 模拟器：直接创建 Socket（简化转发）
            post_push_message2("2",message_model);  // 上报：已转发
            int flag = create_send_socket(deviceNo,socket_message);  // TCP Socket 发送
            if(flag == 0){
                post_push_message2("3",message_model);  // 发送成功，上报确认
            }
        #endif
        QM_ESL2_LOG("mqtt_subscriber deviceNo is not match!\n");
        free(socket_message);
    }
    // ========== G. 释放消息模型内存 ==========
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
