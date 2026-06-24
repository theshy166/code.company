#ifndef __PAGE_ESL_H__
#define __PAGE_ESL_H__
#include "lvgl/lvgl.h"
#include "../model/esl_base_model.h"

// ======================== 数据结构 ========================

/** Socket 发送信息 */
typedef struct {
    int group_id;   // 分组 ID
    char *device_sn; // 设备序列号
    char *ip;        // IP 地址
} socket_send_info_t;

/** UI 显示参数（在线程间传递） */
typedef struct {
    esl_base_model_t * model;  // 商品数据模型
    int index;                  // 屏幕索引 (1=屏幕1, 2=屏幕2)
} show_ui_args_t;

/** 图片下载参数 */
typedef struct {
    char * boot_logo_url;  // 启动 logo URL
    char * bind_logo_url;  // 绑定 logo URL
    int bg_index;          // 屏幕索引
}download_image_args_t;

// ======================== 函数声明 ========================

/** 屏幕1 UI 初始化（读配置 → 创建Socket → _init_ui → 状态栏） */
void xos_esl_ui_init(void);
/** 屏幕2 UI 初始化（双屏模式） */
void xos_esl_ui_init_2(void);

/** 异步显示 UI（服务器推送新模板时调用） */
void async_show_ui(esl_base_model_t * model,lv_obj_t * top,int index);

/** 显示屏幕1空状态（未绑定/无数据时） */
void show_empty_ui_1(lv_obj_t * top);
/** 显示屏幕2空状态 */
void show_empty_ui_2(lv_obj_t * top);

/** 绿色指示灯控制：显示/隐藏 */
void show_green_led_sync(int index);
void clear_green_led_sync(int index);

/** 下载模型中的图片和视频资源 */
void download_image_or_video(esl_base_model_t * model,int index);

/** 设置主设备分组 ID */
void set_master_group_id(int group_id);

/** 获取本机 IPv4 地址 */
void get_local_ip(char *buffer, size_t buflen);
/** 获取 WiFi SSID */
void get_ssid(char *Ssid);

/** 重启设备（屏幕1/屏幕2） */
void restart_device_1(void * user_data);
void restart_device_2(void * user_data);
/** 折扣更新后重启设备 */
void discount_restart_device_1(void * user_data);
void discount_restart_device_2(void * user_data);

/** 获取绿色指示灯对象 */
lv_obj_t * get_green_led(int index);

/** 写入 system 配置 */
void set_http_url_to_param_ini(char * http_url);
void set_esl_parse_type_to_param_ini(int esl_parse_type);

#endif
