#ifndef __PAGE_ESL_H__
#define __PAGE_ESL_H__
#include "lvgl/lvgl.h"
#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_push_server.h"
#include "../model/esl_device_setting.h"
typedef struct {
    int group_id;
    char *device_sn;
    char *ip;
} socket_send_info_t;

typedef struct {
    esl_show_model_t *model;
    esl_device_setting_t *device_setting;
    esl_product_info_t **product_info_list;
    int count;
    int index;
} show_ui_args_t;

typedef struct {
    char * boot_logo_url;
    char * bind_logo_url;
    int bg_index;
}download_image_args_t;

/* 
    未完成/待完善
    1.获取模版和商品信息
        1.价格的对齐方式
        2.文本的对齐方式
        3.轮询播放图片
        4.视频的播放
        5.图片的放大缩小
        6.旋转
        7.样式的错乱
        8.通过网络请求数据
        9.版本号需要从设备接口获取
    2.获取设备配置
    3.获取推送协议
    4.在线设备上报
    5.消息状态上报
 */
void xos_esl_ui_init(void);
void xos_esl_ui_init_2(void);

void async_show_ui(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,lv_obj_t * top,int index);

esl_product_info_t* find_product_info_by_item_no(const char *item_no, esl_product_info_t **product_info_list, int count);

void show_empty_ui_1(lv_obj_t * top);

void show_empty_ui_2(lv_obj_t * top);

void* show_green_led(int index);

void* clear_green_led(int index);

void show_download_log_1(char *arg);

void set_bar_value(int index,int value);

void show_download_log_2(char *arg);

void download_image_or_video(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,int index);

void set_master_group_id(int group_id);

void get_local_ip(char *buffer, size_t buflen);

void restart_device_1(void * user_data);

void restart_device_2(void * user_data);

lv_obj_t * get_green_led(int index);


#endif
