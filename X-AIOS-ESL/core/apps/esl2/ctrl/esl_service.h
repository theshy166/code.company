#ifndef _ESL_SERVICE_H_
#define _ESL_SERVICE_H_
#include "lvgl/lvgl.h"
#include "cJSON.h"
#include "../model/esl_show_model.h"
#include "../model/esl_other_info.h"
#include "../model/product_template.h"
#include "../model/esl_product_info.h"
#include "../model/esl_push_server.h"
#include "../model/esl_device_setting.h"
#include "../model/esl_token_model.h"

esl_push_server_data * query_push_server(const char * device_no, const char *type , const char * sign,const char *protocol_type,const char * local_ip,const char * version_no);

esl_product_info_t ** query_product_infos(const char * device_no, const char *type , const char * sign, int *count);

esl_show_model_t *query_product_template(const char * device_no, const char *query_type , const char * sign);

esl_device_setting_t * query_device_setting(const char * device_no, const char *type , const char * sign);

void push_online_report(const char * device_no,const char * type,const char * device_list,const char * time,const char * sign);

void push_message_report(const char * device_no, const char *type, const char *push_id, const char *push_type, const char *time, const char * sign);

/**
 * 消息状态上报
 * appid	是	string	账户AppID
    deviceno	是	string	主设备序列号
    type	是	string	固定push_report
    push_id	是	string	推送消息id，推送的消息有带，原样返回,多条消息用逗号隔开，示例：122,123,145
    push_type	是	string	消息状态类型
                        1:主设备收到消息
                        2:主设备向子设备转发消息
                        3:子设备收到主设备消息
    time	是	string	设备当前时间，时间戳
    sign	是	string	签名值

 */
//void push_report(const char * device_no,const char * push_id,int push_type,const char * time,const char * sign);

esl_show_model_t * parse_product_template(cJSON *data);
product_template_t *parse_product_template2(const char *json_string,const char * tpl_bg_url);
esl_product_info_t ** parse_product_list(const char *json_string,int * count);
esl_product_info_t ** parse_product_infos(cJSON *dataItem,int * count);
esl_device_setting_t * parse_device_setting(cJSON *data);
esl_other_info_t *parse_other_info2(const char *json_string);


void free_esl_push_server_data(esl_push_server_data *data);
void free_product_template(product_template_t *template);
void free_device_setting(esl_device_setting_t *device_setting);



void set_top(lv_obj_t * top,int index);

void set_ip(char * ip);

/* void set_product_info_list(esl_product_info_t ** product_info_list,int index);

void set_product_model(esl_show_model_t * product_model,int index);

void set_product_count(int product_count,int index);

void set_device_setting(esl_device_setting_t *device_setting, int index); */

void set_device_sn(char * device_sn,int index);

void set_master_device_sn(char * master_device_sn);
void set_http_url(char * http_url);

void set_userName(char * userName);
void set_userPW(char * userPW);
void set_store(char * store);
void set_token(char * toekn);
void set_width(int width);
void set_height(int height);
void set_screens(int screens);
void set_ssid_2(char * ssid);
void set_ssid_pwd(char * ssid_pwd);


lv_obj_t * get_top(int index);
/* esl_product_info_t ** get_product_info_list(int index);
int get_product_count(int index);
esl_show_model_t * get_product_model(int index);
esl_device_setting_t * get_device_setting(int index); */
char * get_device_sn(int index);
char * get_master_device_sn();
char * get_http_url();

char * get_userName();
char * get_userPW();
char * get_store();
char * get_token();
int get_width();
int get_height();
int get_screens();
char * get_ssid_2();
char * get_ssid_pwd();

void refresh_ui_1(void *user_data);
void delete_ui_and_local_data_1(void *user_data);

void refresh_ui_2(void *user_data);
void delete_ui_and_local_data_2(void *user_data);

void update_product_info_batch(esl_product_info_t ** new_infos,int new_count,esl_product_info_t ** old_infos,int old_count);

void* reboot_thread(void* arg);

esl_push_server_data2 * query_push_server2(const char * device_no, const char *wifi_ssid,const char * local_ip,const char * version_no,const int type,const char *token);
void free_esl_push_server_data2(esl_push_server_data2 *data);
esl_token_model_t * query_token(char * device_no,char *wifi_ssid,char * local_ip,char * version_no,char *token);
void free_esl_token_model(esl_token_model_t *data);
int push_online_report2(const char * device_no,const char * postInfo, const char *wifi_ssid,const char * local_ip,const char * version_no,const int type,const char *token);
void push_msgack_message(const char * device_no,const char * postInfo, const char *wifi_ssid,const char * local_ip,const char * version_no,const int type,const char *token);
char * get_content_info(const char * device_no, const char *wifi_ssid,const char * local_ip,const char * version_no,const int type,const char *token);
char * get_version_info(const char * device_no, const char *wifi_ssid,const char * local_ip,const char * version_no,const int type,const char *token);
void push_msgack_message_2(const char * device_no,const char * message_id);
#endif // _ESL_SERVICE_H_