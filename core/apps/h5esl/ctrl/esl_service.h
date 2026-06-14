#ifndef _ESL_SERVICE_H_
#define _ESL_SERVICE_H_
#include "lvgl/lvgl.h"
#include "cJSON.h"
#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_push_server.h"
#include "../model/esl_device_setting.h"

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
esl_product_info_t ** parse_product_infos(cJSON *dataItem,int * count);
esl_device_setting_t * parse_device_setting(cJSON *data);


void free_esl_push_server_data(esl_push_server_data *data);
void free_esl_product_info_array(esl_product_info_t **product_info_list, int count);
void free_esl_show_model(esl_show_model_t *model);
void free_device_setting(esl_device_setting_t *device_setting);

void set_bg(lv_obj_t * bg,int index);

void set_top(lv_obj_t * top,int index);

void set_ip(char * ip);

/* void set_product_info_list(esl_product_info_t ** product_info_list,int index);

void set_product_model(esl_show_model_t * product_model,int index);

void set_product_count(int product_count,int index);

void set_device_setting(esl_device_setting_t *device_setting, int index); */

void set_device_sn(char * device_sn,int index);

void set_master_device_sn(char * master_device_sn);

lv_obj_t * get_bg(int index);
lv_obj_t * get_top(int index);
/* esl_product_info_t ** get_product_info_list(int index);
int get_product_count(int index);
esl_show_model_t * get_product_model(int index);
esl_device_setting_t * get_device_setting(int index); */
char * get_device_sn(int index);
char * get_master_device_sn();

void refresh_ui_1(void *user_data);
void delete_ui_and_local_data_1(void *user_data);

void refresh_ui_2(void *user_data);
void delete_ui_and_local_data_2(void *user_data);

void update_product_info_batch(esl_product_info_t ** new_infos,int new_count,esl_product_info_t ** old_infos,int old_count);

void* reboot_thread(void* arg);

#endif // _ESL_SERVICE_H_