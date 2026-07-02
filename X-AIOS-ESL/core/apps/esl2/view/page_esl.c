#ifdef CONFIG_XOS_APP_ESL2
#include "page_esl.h"

#include <inttypes.h>  // 包含PRIu32等宏定义
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "appmanager.h"
#include "../conf/conf.h"
#include "cJSON.h"
#include "qua_font.h"
#include "http_utils.h"
#include "pic_utils.h"
#include "../utils/hashmap.h"
#include "page_esl_image.h"
#include "page_esl_html.h"
#include "page_esl_text.h"
#include "../utils/esl_sign.h"
#include "../utils/md5.h"
#include "utils.h"
#include "serial_number.h"
#include "../ctrl/esl_file.h"
#include "../ctrl/esl_service.h"
#include "../ctrl/common_service.h"
#include "../ctrl/esl_socket_service.h"
#include "../ctrl/mqtt_control.h"
#include "../ctrl/mqtt_subscriber.h"
#include "../ctrl/esl_socket_udp_service.h"
#include "../utils/esl_json_utils.h"
#include "../utils/esl_ui_utils.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "qm_wifi_cfg.h"
#include "logo_img.h"
#include "generated/autoconf.h"
#include "qm_gpio.h"
#endif
#include "page_footer.h"
/* #include "page_overlay.h" -- replaced with native MQTT-path demo overlay */
#include "qua_param.h"
#include "esl_ui_show.h"
#include "esl_ui_struct.h"
#include "../model/esl_token_model.h"
#include "uirender.h"

extern lv_font_t *ft_font_ttf_30;
extern lv_font_t *ft_font_ttf_20;

extern esl_ui_struct_t g_ui_struct_1;
extern esl_ui_struct_t g_ui_struct_2;

extern void draw_border_for_debug(lv_obj_t *obj);
extern void qme_set_render_status(int index, bool status);

/* ================================================================
 * Demo Override 配置：复用 MQTT 模板的图片/视频组件路径
 * 通过标记文件控制是否启用演示覆盖显示
 * ================================================================ */

#define DEMO_VIDEO_SOURCE "d425b305eccd909c6072b25232130e2d_h264_800x640.mp4"  // 演示视频文件名
#define DEMO_VIDEO_RAW_PATH "/data/res/img/" DEMO_VIDEO_SOURCE  // 演示视频完整路径
#define DEMO_VIDEO_MARKER "/data/.enable_video_overlay"         // 视频覆盖开关标记
#define DEMO_VIDEO_SCREEN_FILE "/data/.video_overlay_screen"    // 视频显示屏幕：0/3=双屏，1/2=单屏
#define DEMO_IMAGE_MARKER "/data/.enable_image_overlay"         // 图片覆盖开关标记

// 外部视频播放器函数声明（los = local os player）
extern void *los_player_create2(lv_obj_t *parent, const char *path, lv_area_t area,
                                bool auto_restart, bool auto_center, int display_idx, int rotation);
extern void los_player_stop(void *player);
extern void los_player_destroy2(void *player, int display_idx);
extern void los_videoplayer_quit(int display_id);

// UI 对象声明
static lv_obj_t * tip_bg_1;           // 屏幕1的提示背景
static lv_obj_t * tip_bg_2;           // 屏幕2的提示背景
static lv_obj_t * master_tip_bg;      // 主设备提示背景

static lv_obj_t * empty_bg_1;         // 屏幕1的空界面背景
static lv_obj_t * empty_bg_2;         // 屏幕2的空界面背景

static lv_obj_t * green_led_1;        // 屏幕1的绿色指示灯
static lv_obj_t * green_led_2;        // 屏幕2的绿色指示灯

static lv_anim_t *green_led_anim_1 = NULL;  // 屏幕1指示灯动画
static lv_anim_t *green_led_anim_2 = NULL;  // 屏幕2指示灯动画

// 状态变量
static int _network_connected_status = -1;  // 网络连接状态（-1=未知, 0=断开, 1=连接）
static int _init_mqtt_status = 0;           // MQTT 初始化状态（0=未初始化, 1=已连接）
static int thread_created = 0;              // 线程创建标志
static int thread_chech_life_created = 0;   // 探活线程创建标志

static int _is_master = 0;                 // 是否为主设备（0=从设备, 1=主设备）
static char * _master_device_no;           // 主设备序列号
static int _master_group_id = -1;          // 主设备分组ID
static int reconnect_count = 0;            // MQTT 重连计数
static int _group_id_1 = 0;                // 屏幕1分组ID
static int _group_id_2 = 0;                // 屏幕2分组ID

// 定义互斥锁
pthread_mutex_t broadcast_mutex = PTHREAD_MUTEX_INITIALIZER;  // 广播互斥锁
pthread_mutex_t download_mutex = PTHREAD_MUTEX_INITIALIZER;   // 下载互斥锁

// 函数声明
static void _init_ui(lv_obj_t * bg,int bg_index,int type);
static void _quit_empty_ui(int bg_index);
static void _start_check_life_thread();
static void update_param_ipaddr(void);

static int esl_product_item_compare(const void *a, const void *b, void *udata);
static bool esl_product_item_iter(const void *item, void *udata);
static uint64_t esl_product_item_hash(const void *item, uint64_t seed0, uint64_t seed1);
static void* _download_image_or_video_in_thread(void *arg);
static void _download_image_or_video_and_create_thread(esl_base_model_t * model,int index);
static void _remove_tip_ui(int index);

// 字符串拼接辅助函数
static char* _concat_strings(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);
    if (result == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

void set_label_text(lv_obj_t *label, const char *bind, const esl_product_info_t *product_info, const char *default_text,esl_device_setting_t * device_setting,int index) {
    if(product_info == NULL){
        lv_label_set_text(label, default_text);
        return;
    }
    if (strcmp(bind, "$item_name") == 0) {
        lv_label_set_text(label, product_info->item_name);
    } else if (strcmp(bind, "$item_no") == 0) {
        lv_label_set_text(label, product_info->item_no);
    } else if (strcmp(bind, "$item_subno") == 0) {
        lv_label_set_text(label, product_info->item_subno);
    } else if (strcmp(bind, "$item_size") == 0) {
        lv_label_set_text(label, product_info->item_size);
    } else if (strcmp(bind, "$item_unit") == 0) {
        lv_label_set_text(label, product_info->unit_no);
    } else if (strcmp(bind, "$sup_name") == 0) {
        lv_label_set_text(label, product_info->main_supcust);
    } else if (strcmp(bind, "$cu_name") == 0) {
        lv_label_set_text(label, "促销时间"); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$area_address") == 0) {
        lv_label_set_text(label, product_info->product_area);
    } else if (strcmp(bind, "$shop_exp") == 0) {
        lv_label_set_text(label, "保质天数"); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$device_sn") == 0) {
        char *device_sn = get_device_sn(index);
        lv_label_set_text(label, device_sn); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$device_ip") == 0) {
        lv_label_set_text(label, device_setting ->pos_ip); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$pos_version") == 0) {
        lv_label_set_text(label, device_setting ->pos_version); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$store_name") == 0) {
        lv_label_set_text(label, device_setting ->store_name); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$show_msg") == 0) {
        lv_label_set_text(label, device_setting ->show_msg); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$resolution") == 0) {
        lv_label_set_text(label, "分辨率"); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$device_name") == 0) {
        lv_label_set_text(label, "设备名称"); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$wifi_SSID") == 0) {
        lv_label_set_text(label, device_setting -> wifi_SSID); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$sale_price") == 0) {
        lv_label_set_text(label, product_info->sale_price);
    } else if (strcmp(bind, "$vip_price") == 0) {
        lv_label_set_text(label, product_info->vip_price);
    } else if (strcmp(bind, "$line_price") == 0) {
        char price_str[50];
        snprintf(price_str, sizeof(price_str), "%.2f", product_info->base_price);
        lv_label_set_text(label, price_str);
    } else if (strcmp(bind, "$cu_price") == 0) {
        lv_label_set_text(label, "促销价"); // Placeholder, replace with actual value if available
    } else if (strcmp(bind, "$price") == 0) {
        lv_label_set_text(label, product_info->price);
    } else {
        lv_label_set_text(label, default_text);
    }
}

static int esl_product_item_compare(const void *a, const void *b, void *udata){
    const struct esl_product_item_key_t *ua = a;
    const struct esl_product_item_key_t *ub = b;
    return strcmp(ua->product_item_key, ub->product_item_key);
}

static bool esl_product_item_iter(const void *item, void *udata) {
    const struct esl_product_item_key_t *user = item;
    QM_ESL2_LOG("%s (age=%d)\n", user->product_item_key, user->count);
    return true;
}

static uint64_t esl_product_item_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct esl_product_item_key_t *user = item;
    return hashmap_sip(user->product_item_key, strlen(user->product_item_key), seed0, seed1);
}

static int compare_product_info_asc(void *a, void *b) {
    esl_product_info_t *info_a = (esl_product_info_t *)a;
    esl_product_info_t *info_b = (esl_product_info_t *)b;
    int sort_a = 0;
    if(info_a->sort_id){
        sort_a = atoi(info_a->sort_id);
    }
    int sort_b = 0;
    if(info_b->sort_id){
        sort_b = atoi(info_b->sort_id);
    }
    return (sort_a - sort_b);
}

/* ================================================================
 * Demo Override 辅助函数
 *
 * 演示模式通过标记文件控制，复用 MQTT 模板的图片/视频组件路径。
 * 如果标记文件或资源文件不存在，这些函数不做任何操作，保持原有 UI 显示。
 * ================================================================ */

// 演示图片候选结构：文件系统路径 + LVGL 虚拟路径
typedef struct {
    const char *fs_path;    // 实际文件系统路径（用于检查文件是否存在）
    const char *lvgl_path;  // LVGL 使用的虚拟路径（H:/ 开头，用于加载图片）
} demo_image_candidate_t;

// 演示图片候选列表：按优先级排列，找到第一个存在的文件
static const demo_image_candidate_t s_demo_images[] = {
    {"/data/res/img/video_frames/frame_001.png", "H:/data/res/img/video_frames/frame_001.png"},
    {"/data/res/img/video_frames/frame_001.jpg", "H:/data/res/img/video_frames/frame_001.jpg"},
};

// 演示视频播放器实例数组（双屏设备，最多两个播放器）
static void *s_demo_player[2] = {NULL, NULL};

// 将屏幕索引转换为显示设备 ID
// screen_index: 1/2 -> display_id: 0/1
static int _demo_display_id(int screen_index)
{
    if (screen_index <= 1) {
        return 0;  // 屏幕1对应显示设备0
    }
    return 1;      // 屏幕2对应显示设备1
}

// 检查文件是否可读（辅助函数）
static bool _demo_file_readable(const char *path)
{
    return path != NULL && access(path, R_OK) == 0;
}

static int _demo_video_target_screen(void)
{
    int target = 0;  // 默认双屏显示
    FILE *fp = fopen(DEMO_VIDEO_SCREEN_FILE, "r");
    if (fp != NULL) {
        int value = 0;
        if (fscanf(fp, "%d", &value) == 1) {
            if (value == 1) {
                target = 1;
            } else if (value == 2) {
                target = 2;
            } else {
                target = 0;
            }
        }
        fclose(fp);
    }
    return target;
}

// 生成指定显示设备的本地视频路径
// 格式: /data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640_0.mp4 (屏幕1)
//       /data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640_1.mp4 (屏幕2)
static void _demo_native_video_path(int display_id, char *path, size_t size)
{
    snprintf(path, size, "/data/res/img/d425b305eccd909c6072b25232130e2d_h264_800x640_%d.mp4", display_id);
}

// 查找可用的演示图片路径（按优先级顺序）
static const char *_find_demo_image_path(void)
{
    size_t count = sizeof(s_demo_images) / sizeof(s_demo_images[0]);
    for (size_t i = 0; i < count; i++) {
        if (_demo_file_readable(s_demo_images[i].fs_path)) {
            return s_demo_images[i].lvgl_path;  // 返回 LVGL 虚拟路径
        }
    }
    return NULL;  // 没有找到可用的图片
}

// 只停止演示视频播放器（不退出视频播放器模块）
static void _stop_demo_player_only(int screen_index)
{
    int display_id = _demo_display_id(screen_index);
    void **player = &s_demo_player[display_id];
    if (*player != NULL) {
        los_player_stop(*player);          // 停止播放
        los_player_destroy2(*player, display_id);  // 销毁播放器实例
        *player = NULL;                    // 清空播放器指针
        printf("[DEMO] stopped direct player: screen=%d\n", screen_index);
    }
}

// 完全退出演示覆盖模式：停止播放器 + 退出视频播放器模块
static void _quit_demo_override(int screen_index)
{
    _stop_demo_player_only(screen_index);           // 停止演示播放器
    los_videoplayer_quit(_demo_display_id(screen_index));  // 退出视频播放器模块
}

// 显示演示视频覆盖
// 优先使用设备特定的视频文件（_0.mp4 / _1.mp4），其次使用通用视频文件
static bool _show_demo_video(lv_obj_t *bg, int screen_index)
{
    int display_id = _demo_display_id(screen_index);
    char native_path[256] = {0};
    const char *video_path = DEMO_VIDEO_RAW_PATH;
    _demo_native_video_path(display_id, native_path, sizeof(native_path));

    // 优先检查设备特定的视频文件
    if (_demo_file_readable(native_path)) {
        video_path = native_path;
    } else if (!_demo_file_readable(DEMO_VIDEO_RAW_PATH)) {
        printf("[DEMO] video marker set but file missing: %s or %s\n",
               native_path, DEMO_VIDEO_RAW_PATH);
        return false;
    }

    lv_area_t area = {.x1 = 0, .y1 = 0, .x2 = 799, .y2 = 1279};  // 全屏区域
    _stop_demo_player_only(screen_index);
    s_demo_player[display_id] = los_player_create2(bg, video_path, area,
                                                    true, false, display_id, 0);
    if (s_demo_player[display_id] == NULL) {
        printf("[DEMO] direct video player create failed: screen=%d path=%s\n",
               screen_index, video_path);
        return false;
    }

    lv_obj_invalidate(bg);
    lv_refr_now(lv_obj_get_display(bg));
    printf("[DEMO] direct video override: screen=%d player=%p path=%s\n",
           screen_index, s_demo_player[display_id], video_path);
    return true;
}

// 显示演示图片覆盖
static bool _show_demo_image(lv_obj_t *bg, int screen_index)
{
    int display_id = _demo_display_id(screen_index);
    const char *image_path = _find_demo_image_path();

    // 检查图片文件是否存在
    if (image_path == NULL) {
        printf("[DEMO] image marker set but no demo image found\n");
        return false;
    }

    // 构造图片组件参数
    esl_base_model_child_t item;
    memset(&item, 0, sizeof(item));
    item.type = EslCompentTypeIMAGE;   // 组件类型：图片
    item.content = (char *)image_path; // 图片路径
    item.x = 0;                        // 左上角 X 坐标
    item.y = 0;                        // 左上角 Y 坐标
    item.w = 800;                      // 宽度（屏幕分辨率）
    item.h = 1280;                     // 高度（屏幕分辨率）

    _quit_demo_override(screen_index);                 // 先退出之前的覆盖模式
    handle_image_extension2(bg, &item, display_id);    // 使用 MQTT 模板的图片组件显示
    printf("[DEMO] image override: screen=%d path=%s\n", screen_index, image_path);
    return true;
}

/**
 * 检查演示标记文件并根据优先级显示覆盖内容
 * 
 * 优先级顺序：
 *   1. 视频标记 + 可读视频资源 → 显示视频覆盖
 *   2. 图片标记 + 可读图片资源 → 显示图片覆盖
 *   3. 无标记或资源缺失 → 保持原有 UI 不变
 * 
 * @param bg          LVGL 背景对象
 * @param screen_index 屏幕索引（1=屏幕1, 2=屏幕2）
 * @return true=成功显示覆盖, false=未显示覆盖
 */
static bool _maybe_show_demo_override(lv_obj_t *bg, int screen_index)
{
    // 检查背景对象是否有效
    if (bg == NULL || !lv_obj_is_valid(bg)) {
        return false;
    }

    // 先停止之前的演示播放器（防止资源泄漏）
    _stop_demo_player_only(screen_index);

    // 优先级1：检查视频覆盖标记
    if (access(DEMO_VIDEO_MARKER, F_OK) == 0) {
        int target_screen = _demo_video_target_screen();
        if (target_screen == 0 || screen_index == target_screen) {
            if (_show_demo_video(bg, screen_index)) {
                return true;
            }
        } else {
            printf("[DEMO] video overlay skip screen=%d target=%d\n",
                   screen_index, target_screen);
        }
    }

    // 优先级2：检查图片覆盖标记
    if (access(DEMO_IMAGE_MARKER, F_OK) == 0) {
        if (_show_demo_image(bg, screen_index)) {
            return true;
        }
    }

    // 没有启用任何覆盖，保持原有 UI
    return false;
}

// 显示 UI 主函数
static void _show_ui(esl_base_model_t * model,lv_obj_t * top,int width,int height,int index)
{
    if(model == NULL){
        QM_ESL2_LOG("model maybe null!, return!");
        // 模型为空时显示空界面（二维码界面）
        if (index == 1) {
            show_empty_ui_1(top);
        } else if (index == 2) {
            show_empty_ui_2(top);
        }
        set_show_green_led(index,0);
        return;
    }

    // 调用 esl_show_ui 显示 MQTT 模板界面
    esl_show_ui(model,top,width,height,index);

    // 开启线程删除本地资源（清理旧资源）
    #if 1
        pthread_t download_thread;
        compare_data_t *compare_data_t = malloc(sizeof(compare_data_t));
        compare_data_t->index = index;
        pthread_create(&download_thread, NULL, compare_local_resouce, compare_data_t);
        pthread_detach(download_thread);
    #endif
    free_esl_base_model(model);           // 释放模型内存
    set_show_green_led(index,0);          // 关闭绿色指示灯
    
    // 获取背景对象（优先使用 get_bg，失败则使用 top）
    lv_obj_t *bg = get_bg(index);
    if (bg == NULL || !lv_obj_is_valid(bg)) {
        bg = top;
    }
    
    // ★ 检查演示覆盖标记，如启用则显示图片/视频覆盖原有 UI
    _maybe_show_demo_override(bg, index);
    
    QM_ESL2_LOG("show_ui EXIT!!");
}

void async_show_ui(esl_base_model_t * model,lv_obj_t * top,int index){
    QM_ESL2_LOG("async_show_ui ENTRY!!");
    int display_id = index - 1;
    _stop_demo_player_only(index);
    los_videoplayer_quit(display_id);
    set_top_hidden(index);
    QM_ESL2_LOG("--[recylemem]---qme_save_time_callback %d", display_id);
    qme_set_render_status(display_id, false);
    
    _quit_empty_ui(index);
    QM_ESL2_LOG("async_show_ui _quit_empty_ui success!!");

    int width = 0;
    int height = 0;
    char * device_sn = get_device_sn(index);

    _show_ui(model,top,width,height,index);

    QM_ESL2_LOG("[start render:%d] 8888888888888", display_id);
    qme_set_render_status(display_id, true);
}

static void _query_data_and_show_ui(lv_obj_t * act,int bg_index){
    int width = 0;
    int height = 0;
    int product_count = 0;
    char * device_sn = get_device_sn(bg_index);

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
        return;
    }
    
    if(esl_type == 2  || esl_type == 3){
        esl_base_model_t * model = parse_esl_base_model(content);
        _download_image_or_video_and_create_thread(model,bg_index);
    }else{
        cJSON *json = cJSON_Parse(content);
        if(json == NULL){
            free(content);
            return;
        }
        cJSON *tpl_jn = cJSON_GetObjectItem(json, "tpl_jn");
        cJSON *tpl_pro = cJSON_GetObjectItem(json, "tpl_pro");
        cJSON *tpl_txt_jn = cJSON_GetObjectItem(json, "tpl_txt_jn");
        cJSON *tpl_bg_url = cJSON_GetObjectItem(json, "tpl_bg_url");
        cJSON *dsp_pro_num = cJSON_GetObjectItem(json, "dsp_pro_num");
        if(tpl_jn && tpl_pro){
            char * product_template = cJSON_PrintUnformatted(tpl_jn);
            char * product_list = cJSON_PrintUnformatted(tpl_pro);
            char * other_info_str = cJSON_PrintUnformatted(tpl_txt_jn);
            char * temp = (tpl_bg_url && cJSON_IsString(tpl_bg_url) && tpl_bg_url->valuestring) ? strdup(tpl_bg_url->valuestring) : strdup("");
            int display_product_num = cJSON_IsNumber(dsp_pro_num) ? dsp_pro_num->valueint : 0;
            
            esl_base_model_t * model = the_second_to_model(product_template,product_list,other_info_str,temp,bg_index,display_product_num);
            _download_image_or_video_and_create_thread(model,bg_index);
            free(temp);
            free(other_info_str);
            free(product_list);
            free(product_template);
        }else{
            esl_base_model_t * model = the_second_to_model(content,NULL,NULL,NULL,bg_index,0);
            _download_image_or_video_and_create_thread(model,bg_index);
        }
        cJSON_Delete(json);
    }
    free(content);
}

static void _download_image_or_video_and_create_thread(esl_base_model_t * model,int index){
    QM_ESL2_LOG("download_image_or_video_and_create_thread ENTRY!!");
    if (model == NULL) {
        QM_ESL2_LOG("Invalid template or product infos.\n");
        return;
    }

    // 开启线程下载资源
    show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
    args->model = model;
    args->index = index;

    set_top_show(index);
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, _download_image_or_video_in_thread, (void *)args) != 0) {
        perror("Failed to create thread");
        free_esl_base_model(model);
        free(args);
        return;
    }

    // 分离线程，使其在终止时自动释放资源
    pthread_detach(thread_id);
}

//这个tip的移除可能没有背景了。。需要放到最后（有新图时）来移除
static void _remove_tip_ui(int index){
    //删除ui
    if (index == 1){
        if (lv_obj_is_valid(tip_bg_1) && tip_bg_1 != NULL){
            QM_ESL2_LOG("_network_connected tip_bg_1 is valid!!");
            lv_obj_delete(tip_bg_1);
            tip_bg_1 = NULL;
        }
    }else if (index == 2){
        if (lv_obj_is_valid(tip_bg_2) && tip_bg_2 != NULL){
            QM_ESL2_LOG("_network_connected tip_bg_2 is valid!!");
            lv_obj_delete(tip_bg_2);
            tip_bg_2 = NULL;
        }
    }
}

static void _async_show_ui_after_download(void * arg){
    show_ui_args_t *args = (show_ui_args_t *)arg;
    esl_base_model_t * model = args->model;
    int index = args->index;

    int display_id = index - 1;
    _stop_demo_player_only(index);
    los_videoplayer_quit(display_id);
    set_top_hidden(index);
    _quit_empty_ui(index);

    QM_ESL2_LOG("--[recylemem]---qme_save_time_callback %d", display_id);

    lv_obj_t * bg = get_bg(index);
    if(bg != NULL && lv_obj_is_valid(bg)) {
        lv_obj_delete(bg);
        set_bg(NULL,index);
    }

    lv_obj_t * top = get_top(index);
    int width = 0;
    int height = 0;
    _show_ui(model,top,width,height,index);
    QM_ESL2_LOG("[start render:%d] 8888888888888", index - 1);

    qme_set_render_status(index - 1, true);
    free(args);
    QM_ESL2_LOG("========_async_show_ui_after_download ----!!\n");
}

void download_image_or_video(esl_base_model_t * model,int index){
    if(model == NULL){
        return;
    }
    set_is_downloading(1,index);
    pthread_mutex_lock(&download_mutex);
    esl_img_t * img_urls = model -> img_urls;
    int img_count = model -> img_count;
    if(img_count > 0 && img_urls !=NULL){
        for (int i = 0; i < img_count; i++) {
            if (&img_urls[i] == NULL) {
                continue;
            }
            QM_ESL2_LOG("========handle_image_extension_download2 start index:%d ----!!\n",img_count);
            handle_image_extension_download2(img_urls[i].img_url,img_urls[i].width,img_urls[i].height,index - 1);
            QM_ESL2_LOG("========handle_image_extension_download2 end index:%d ----!!\n",img_count);
        }
    }

    char ** video_urls = model -> video_urls;
    int video_count = model -> video_count;

    for (int i = 0; i < video_count; i++) {
        char * video_url = video_urls[i];
        if (is_string_empty(video_url)) {
            continue; // 跳过没有组件类型的项
        }
        QM_ESL2_LOG("========handle_video_extension_download2 start index:%d ----!!\n",i);
        handle_video_extension_download2(video_url,index - 1);
        QM_ESL2_LOG("========handle_video_extension_download2 end index:%d ----!!\n",i);
    }
    QM_ESL2_LOG("========download_image_or_video ----end!!\n");
    //remove download log
    if (index == 1){
        g_ui_struct_1.down_log_visible = 0;
        g_ui_struct_1.bar_visible = 0;
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        extern void fbdev_set_statusbar(int x, int y, int width, int height);
        fbdev_set_statusbar( 0, 0, 0, 0);
        #endif
    }else{
        g_ui_struct_2.down_log_visible = 0;
        g_ui_struct_2.bar_visible = 0;
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        extern void fbdev2_set_statusbar(int x, int y, int width, int height);
        fbdev2_set_statusbar( 0, 0, 0, 0);
        #endif
    }
    pthread_mutex_unlock(&download_mutex);
    set_is_downloading(0,index);
}

static void* _download_image_or_video_in_thread(void *arg) {
    show_ui_args_t *args = (show_ui_args_t *)arg;
    esl_base_model_t * model = args->model;
    int index = args->index;

    esl_file_save_info2_t *esl_file_save_info_args = (esl_file_save_info2_t *)malloc(sizeof(esl_file_save_info2_t));
    if (esl_file_save_info_args == NULL) {
        free_esl_base_model(model);
        free(args);
        return;
    }
    esl_file_save_info_args->index = index;
    esl_file_save_info_args->model = model;
    esl_file_save_info_args->device_sn = get_device_sn(index);
    save_data_thread(esl_file_save_info_args);

    //pthread_mutex_lock(&download_mutex);
    download_image_or_video(model,index);
    //pthread_mutex_unlock(&download_mutex);

    lv_async_call(_async_show_ui_after_download, args);
    QM_ESL2_LOG("========_download_image_or_video_in_thread: ----end!!\n");
    return NULL;
}

// 显示空界面（二维码界面）：包含设备SN、IP、蓝牙名称和二维码
static void _show_empty_ui(lv_obj_t * bg,int bg_index){
    char * device_sn = get_device_sn(bg_index);

    QM_ESL2_LOG("_show_empty_ui ENTRY!! bg_index:%d",bg_index);
    
    // 设置背景样式（无边距、无圆角、无边框、无滚动条）
    lv_obj_set_style_pad_all(bg, 0, 0);
    lv_obj_set_style_radius(bg, 0, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_scrollbar_mode(bg, LV_SCROLLBAR_MODE_OFF);

    // 设置背景尺寸为全屏（800x1280）
    lv_obj_set_size(bg, 800, 1280);

    // 创建默认背景图片
    lv_obj_t *img = lv_img_create(bg);
    _set_image_src(img,k_path_default_bg_img,800,1280,bg_index - 1);

    // 创建设备SN标签
    lv_obj_t *sn_label = lv_label_create(bg);
    lv_label_set_text(sn_label, device_sn);
    lv_font_t *sn_font = NULL;
    qua_xos_ft_init_with_size_path(&sn_font, 30, k_path_ttf, 0);
    lv_obj_set_style_text_font(sn_label, sn_font, 0);

    // 生成二维码数据（包含设备SN）
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "SN", device_sn);
    char *data = cJSON_Print(json);
    
    // 创建二维码组件
    lv_obj_t * qr = lv_qrcode_create(bg);
    lv_qrcode_set_size(qr, 300);
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_set_size(qr, 300, 300);
    lv_obj_set_pos(qr, 250, 250);  // 居中显示

    // 将SN标签对齐到二维码上方
    lv_obj_align_to(sn_label, qr, LV_ALIGN_OUT_TOP_MID, 0, -15);

    // 获取并显示本地IP地址
    char ip[NI_MAXHOST] = {0};
    get_local_ip(ip, NI_MAXHOST);
    QM_ESL2_LOG("Local IP address: %s\n", ip);
    if(!is_string_empty(ip)){
        lv_obj_t *ip_label = lv_label_create(bg);
        lv_label_set_text(ip_label, ip);
        lv_font_t *ip_font = NULL;
        qua_xos_ft_init_with_size_path(&ip_font, 30, k_path_ttf, 0);
        lv_obj_set_style_text_font(ip_label, ip_font, 0);
        lv_obj_align_to(ip_label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);  // 对齐到二维码下方
    }else{
        // 如果无法获取IP，从参数系统获取
        char *ipaddr = NULL;
        ipaddr = param_get_string("net:ipaddr", NULL);
        if (ipaddr != NULL) {
            lv_obj_t *ip_label = lv_label_create(bg);
            lv_label_set_text(ip_label,ipaddr);
            lv_font_t *ip_font = NULL;
            qua_xos_ft_init_with_size_path(&ip_font, 30, k_path_ttf, 0);
            lv_obj_set_style_text_font(ip_label, ip_font, 0);
            lv_obj_align_to(ip_label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
        } 
    }
    
    // 创建蓝牙名称标签（格式：tag-设备序列号）
    char bluetooth_name[30];
    sprintf(bluetooth_name, "tag-%s", deviceSerialNumber); 

    lv_obj_t * bluetooth_label = lv_label_create(bg);
    lv_label_set_text(bluetooth_label, bluetooth_name);
    lv_font_t *bluetooth_font = NULL;
    qua_xos_ft_init_with_size_path(&bluetooth_font, 15, k_path_ttf, 0);
    lv_obj_set_style_text_font(bluetooth_label, bluetooth_font, 0);
    lv_obj_align_to(bluetooth_label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 80);  // 在IP下方显示

    // 释放临时内存
    free(data);
    cJSON_Delete(json);
    
    // ★ 检查演示覆盖标记，如启用则显示图片/视频覆盖空界面
    _maybe_show_demo_override(bg, bg_index);
    
    QM_ESL2_LOG("_show_empty_ui EXIT!! bg_index:%d",bg_index);
}

void show_empty_ui_1(lv_obj_t * top){
    if(lv_obj_is_valid(empty_bg_1) || empty_bg_1 != NULL){
        _quit_empty_ui(1);
    }
    QM_ESL2_LOG("show_empty_ui_1 ENTRY!! bg_index:%d",1);
    empty_bg_1 = lv_obj_create(top);
    _show_empty_ui(empty_bg_1,1);
}

void show_empty_ui_2(lv_obj_t * top){
    if(lv_obj_is_valid(empty_bg_2) || empty_bg_2 != NULL){
        _quit_empty_ui(2);
    }
    QM_ESL2_LOG("show_empty_ui_2 ENTRY!! bg_index:%d",2);
    empty_bg_2 = lv_obj_create(top);
    _show_empty_ui(empty_bg_2,2);
}

static void _query_data_and_show_ui_local(lv_obj_t * act,int bg_index){
    char * device_sn = get_device_sn(bg_index);
    int width = 0;
    int height = 0;
    int display_id = bg_index - 1;
    _stop_demo_player_only(bg_index);
    los_videoplayer_quit(display_id);

    esl_base_model_t * model = query_esl_base_model_local(bg_index,device_sn);
    if(model == NULL ){
        QM_ESL2_LOG("Failed to parse device model JSON.\n");
        if (bg_index == 1) {
            show_empty_ui_1(act);
        } else if (bg_index == 2) {
            show_empty_ui_2(act);
        }
        return;
    }

    _show_ui(model,act,width,height,bg_index);
    QM_ESL2_LOG("[start render:%d] 8888888888888", display_id);
}

void get_local_ip(char *buffer, size_t buflen) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    memset(buffer, 0, buflen);

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                QM_ESL2_LOG("getnameinfo() failed: %s\n", gai_strerror(s));
                return;
            }

            if (strcmp(ifa->ifa_name, "lo") != 0) { // Skip loopback interface
                strncpy(buffer, host, buflen);
                break;
            }
        }
    }
    freeifaddrs(ifaddr);
}

lv_obj_t * _show_tip_ui(lv_obj_t * bg,char * content,int font_size,int top_pad){
    // lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);
    //lv_obj_set_style_bg_opa(bg, LV_OPA_0, 0);
    lv_obj_set_style_pad_all(bg, 0, 0);
    lv_obj_set_style_radius(bg, 0, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_scrollbar_mode(bg, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *tip_ui_bg = lv_obj_create(bg);
    lv_obj_set_size(tip_ui_bg, 800, 1280);
    lv_obj_set_style_bg_color(tip_ui_bg, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(tip_ui_bg, LV_OPA_50, 0); 
    lv_obj_set_style_radius(tip_ui_bg, 0, 0);
    lv_obj_set_style_border_width(tip_ui_bg, 0, 0);

    lv_obj_t * label = lv_label_create(tip_ui_bg);
    lv_label_set_text(label,content);
    lv_obj_set_size(label,800,200);
    lv_obj_set_style_bg_color(label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(label, LV_OPA_10, 0); 
    lv_obj_align(label,LV_ALIGN_CENTER,0,0);
    lv_font_t *int_font = NULL;
    qua_xos_ft_init_with_size_path(&int_font, font_size, k_path_ttf, 0);
    lv_obj_set_style_text_font(label, int_font, 0);
    lv_obj_set_style_radius(label, 0, 0);
    lv_obj_set_style_border_width(label, 0, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_top(label, top_pad, 0);

    return tip_ui_bg;
}

static void *timer_thread_function(void *arg) {
    // 执行线程任务
    //说明是从设备，先接收广播消息再发送信息
    prctl(PR_SET_NAME, "udp_bind"); // 设置线程名称
    QM_ESL2_LOG("udp_bind groupId1: %d,groupId2:%d, deviceSerialNumber: %s\n", _group_id_1,_group_id_2, deviceSerialNumber);
    recevice_msg_and_send(_group_id_1,_group_id_2);
    return NULL;
}

static void * _download_image_thread(void * user_data){
    download_image_args_t *download_image_args = (download_image_args_t *)user_data;
    char * boot_logo_url = download_image_args->boot_logo_url;
    int bg_index = download_image_args->bg_index;
    if(!is_string_empty(boot_logo_url)){
        QM_ESL2_LOG("boot_logo_url:%s",boot_logo_url);
        if (bg_index == 1){
            if(logo_image_is_exist(k_path_boot_bg_img_1) == 0){
                download_logo_image(boot_logo_url,k_path_boot_bg_img_1);
                #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                extern int xos_logo_update(uint8_t index, const char *path);
                xos_logo_update(bg_index - 1, k_path_boot_bg_img_1);
                #endif
            }
        }else if (bg_index == 2){
            if(logo_image_is_exist(k_path_boot_bg_img_2) == 0){
                download_logo_image(boot_logo_url,k_path_boot_bg_img_2);
                #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                extern int xos_logo_update(uint8_t index, const char *path);
                xos_logo_update(bg_index - 1, k_path_boot_bg_img_2);
                #endif
            }
        }
    }
    free(download_image_args -> boot_logo_url);
    free(download_image_args);
    return NULL;
}

void get_ssid(char *Ssid){
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    FILE *file = NULL;
    char line[QM_MAX_CFG_SSID_STR_LEN];
    char *finStr = NULL;

    // 打开文件
    file = fopen("/data/wifi/wpa_supplicant.conf", "r");
    if (file == NULL) {
        QM_ESL2_LOG("Failed to open wpa_supplicant.conf\n");
        return; // 返回错误码
    }

    // 逐行读取文件
    while (fgets(line, QM_MAX_CFG_SSID_STR_LEN, file) != NULL) {
        // 去掉行末的换行符
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // 查找 SSID
        finStr = strstr(line, "ssid=\"");
        if (finStr != NULL) {
            finStr += strlen("ssid=\"");
            while (*finStr != '"' && *finStr != '\0') {
                *Ssid++ = *finStr++;
            }
            *Ssid = '\0'; // 添加字符串结束符
        }
    }
    // 关闭文件
    fclose(file);
    return; // 成功返回
    #else
    // 模拟获取SSID
    const char *mock_ssid = "MAOESL";
    if (Ssid != NULL) {
        strncpy(Ssid, mock_ssid, ESL_MAX_CFG_SSID_STR_LEN - 1);
        Ssid[ESL_MAX_CFG_SSID_STR_LEN - 1] = '\0'; // 确保字符串以'\0'结尾
    }
    return; // 成功返回
    #endif
}

static void _init_esl_token(int bg_index){
    char * device_sn = get_device_sn(bg_index);
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
    esl_token_model_t * data = query_token(device_sn,ssid,ip,version,NULL);
    if(data && data->token){
        set_token(data->token);
    }
    free_esl_token_model(data);
}

/**
 * @brief ★ 初始化 ESL 设备的 MQTT 连接 — 设备启动时的核心初始化流程
 *
 * ┌──────────────── 初始化流程图 ────────────────┐
 * │                                                │
 * │  1. 收集设备信息（SN、IP、版本、WiFi、token）   │
 * │      ↓                                         │
 * │  2. HTTP 请求云平台 → query_push_server2()      │
 * │      ↓ 返回 esl_push_server_data2               │
 * │      │  ├─ master: 是否为主设备                  │
 * │      │  ├─ mqtt:   MQTT的连接参数（host/port/   │
 * │      │  │         用户名/密码/client_id/topic）  │
 * │      │  ├─ bootLogo: 开机Logo下载地址            │
 * │      │  └─ group:  设备分组信息                  │
 * │      │                                         │
 * │  3. 检查 query 结果：                           │
 * │      ├─ data == NULL       → 重启设备          │
 * │      ├─ query_success == -1 → 返回失败         │
 * │      └─ 正常 → 下载Logo + 保存分组ID            │
 * │      ↓                                         │
 * │  4. 判断设备角色：                              │
 * │      ├─ master == false → 从设备（暂未处理）    │
 * │      └─ master == true  → ★ 连接 MQTT          │
 * │           ├─ 填充 AiDevInfo（MQTT 连接参数）     │
 * │           ├─ init_mqtt()     → 建立 MQTT 连接   │
 * │           ├─ register_connect_cb() → 连接回调   │
 * │           ├─ register_repose_cb() → ★ 消息回调  │
 * │           └─ _start_check_life_thread() → 探活  │
 * └────────────────────────────────────────────────┘
 *
 * 关键概念：
 *   - 「主设备」：负责 MQTT 连接的设备（双屏中只有一个）
 *   - 「从设备」：不直连 MQTT，由主设备通过 Socket 转发消息
 *   - 本函数只在主设备上执行 MQTT 初始化，从设备走 broadcast 路径
 *
 * @param bg        LVGL 背景对象
 * @param bg_index  屏幕索引（1=屏幕1, 2=屏幕2）
 * @return 1=成功, 0=将要重启, -1=失败
 */
static int _init_esl_mqtt(lv_obj_t * bg,int bg_index){

    // ===== Step 1: 收集设备信息 =====
    char * device_sn = get_device_sn(bg_index);          // 设备序列号（如 D1S920H73ODI667_1）
    char ip[NI_MAXHOST] = {0};
    get_local_ip(ip, NI_MAXHOST);                        // 本机 IP 地址
    QM_ESL2_LOG("Local IP address: %s\n", ip);

    // 构造版本号字符串（格式：V2.0.3.000）
    char version[32] = {0};
    #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
        #define CONFIG_BUILD_ID "1112136"                // 模拟器的构建ID
        #define CONFIG_QM_VERSION "V1.0.1"
    #else
        #ifndef CONFIG_BUILD_ID
        #define CONFIG_BUILD_ID "000"                    // 默认构建ID
        #endif
    #endif
    snprintf(version,sizeof(version),"%s.%s",CONFIG_QM_VERSION,CONFIG_BUILD_ID);

    char * ssid = get_ssid_2();           // 当前连接的 WiFi SSID
    int esl_type = get_esl_type();        // ESL 协议类型（1=旧协议, 2/3=新协议）
    char * token = get_token();           // 设备认证 Token

    // ===== Step 2: ★ HTTP 请求云平台，获取推送服务器配置 =====
    // 返回的数据包含 MQTT 连接参数、设备角色、分组信息、开机Logo等
    esl_push_server_data2 * data = query_push_server2(device_sn,ssid,ip,version,esl_type,token);

    // ===== Step 3: 处理 query 结果 =====
    if(data == NULL){
        // ★ 查询失败：无法获取推送服务器配置 → 重启设备重试
        #if 0
            // 旧方案：走 Socket 广播获取配置（已废弃）
            // 检查线程是否已经创建
            ...
        #endif
        QM_ESL2_LOG("query_push_server2 is NULL, restarting device!");
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, reboot_thread, NULL);  // 开线程重启
        pthread_detach(thread_id);
        return 0;

    }else if(data->query_success == -1){
        // ★ 服务器返回失败（-1）：通常是认证/授权问题
        free(data);
        QM_ESL2_LOG("data->query_success is -1!!!!!!!!!!!!!");
        return -1;

    }else {
        // ★ 查询成功：处理返回的数据
        #if 1
            // 下载开机 Logo 图片（如果服务器配置了的话）
            char * boot_logo_url = data->bootLogo.url;
            if(!is_string_empty(boot_logo_url)){
                // 开启线程异步下载Logo图片，不阻塞当前线程
                pthread_t download_thread;
                download_image_args_t *download_image_args = malloc(sizeof(download_image_args_t));
                download_image_args->boot_logo_url = strdup(boot_logo_url);
                download_image_args->bg_index = bg_index;
                pthread_create(&download_thread, NULL, _download_image_thread, download_image_args);
                pthread_detach(download_thread);  // 分离线程，自动回收资源
            }
        #endif
        // 保存分组 ID（用于后续的广播/子设备管理）
        if(bg_index == 1){
            _group_id_1 = data->group.id;          // 屏幕1的分组ID
        }else if(bg_index == 2){
            _group_id_2 = data->group.id;          // 屏幕2的分组ID
        }
    }

    // ===== Step 4: ★ 判断设备角色 → 初始化 MQTT =====
    if(!data->master){
        // 从设备：不直连 MQTT，等待主设备通过 Socket 转发消息
        // （当前版本从设备逻辑被 #if 0 禁用了）
        #if 0
            // 旧方案：从设备通过广播获取配置
            ...
        #endif
    }else{
        // ★★★ 主设备：初始化 MQTT 连接 ★★★
        _master_device_no = strdup(device_sn);     // 保存主设备SN
        _master_group_id = data->group.id;          // 保存分组ID
        _is_master = 1;                             // 标记本机为主设备
        set_master_device_sn(device_sn);            // 设置全局主设备SN

        // 避免重复初始化：如果 MQTT 已经连接，直接返回
        if(_init_mqtt_status == 1){
            return 1;
        }

        // ★ 填充 MQTT 连接参数（全部来自服务器返回的配置）
        AiDevInfo dev = {0};
        dev.server_type = 1;                        // 服务器类型
        dev.hosturl = data->mqtt.host;             // MQTT Broker 地址
        dev.port = data->mqtt.port;                // MQTT 端口（1883/8883）
        dev.ac = data->mqtt.username;              // MQTT 认证用户名
        dev.pw = data->mqtt.password;              // MQTT 认证密码
        dev.devsn = device_sn;                     // 设备SN（用作部分标识）
        dev.clientid = data->mqtt.client_id;       // MQTT Client ID（唯一标识）
        dev.subscriber_topic = data->mqtt.topic;   // ★ MQTT 订阅主题（服务器推送消息到此 topic）
        dev.product_id = 3;                         // 产品类型ID

        // ★ 建立 MQTT 连接（见 mqtt_control.c 的 init_mqtt）
        init_mqtt(&dev);

        // ★ 注册连接状态回调：当 MQTT 连接/断开时通知
        register_connect_cb(mqtt_connect_response);

        // ★★★ 注册消息接收回调：MQTT Broker 推送消息 → mqtt_subscriber() ★★★
        // 这是整个系统最重要的回调注册，所有云端指令都从这里进来
        register_repose_cb(mqtt_subscriber);

        // 开启探活线程：定期检查 MQTT 连接是否存活，断线自动重连
        _start_check_life_thread();

        _init_mqtt_status = 1;                      // 标记已初始化，防止重复
    }

    free_esl_push_server_data2(data);               // 释放服务器返回的数据
    return 1;
}
//网络状态指示灯控制接口函数
static void _network_connected_status_led(int status){
    //指示灯控制管脚是GPIO4_7
    //GPIO拉低灯亮，拉高灯灭
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        uint32_t out_pin = 39;//GPIO(QM_GPIO7, QM_GG4); // GPIO4_7
        QM_ESL2_LOG("_network_connected_status_led ENTRY  status:%d!!",status);
        qm_gpio_export_direction(out_pin, false);
        bool result = CheckSerialNumber();
        // 序列号不存在或者wifi未连接时，指示灯灭
        if(status == 1 && result){
            qm_gpio_set_value(out_pin, 0); // 拉低，指示灯亮
        }else{
            qm_gpio_set_value(out_pin, 1); // 拉高，指示灯灭
        }
    #endif
}
static void _network_connected(void * user_data){
    int flag = (int)(intptr_t)user_data;
    QM_ESL2_LOG("_network_connected ENTRY  flag:%d!!",flag);
    _network_connected_status_led(1); // 亮灯表示网络连接成功
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp = lv_display_get_1st_scr();
    if(_disp != NULL){
        _remove_tip_ui(1);
        //初始化ui
        if(flag == 1){
            lv_obj_t * top = get_top(1);
            _init_ui(top,1,2);
        }
        update_page_footer_icon(1,FOOTER_STATUS_CONNECTED_NET);
    }

    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 != NULL){
        _remove_tip_ui(2);
        //初始化ui
        if(flag == 1){
            lv_obj_t * top2 = get_top(2);
            _init_ui(top2,2,2);
        }
        update_page_footer_icon(2,FOOTER_STATUS_CONNECTED_NET);
    }

    if(flag == 1){
        // 创建线程
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, timer_thread_function, NULL) != 0) {
            QM_ESL2_ELOG("Failed to create thread");
            return;
        }
        // 分离线程，使其在终止时自动释放资源
        pthread_detach(thread_id);
    }
    QM_ESL2_LOG("_network_connected EXIT!!");
}

static void _network_disconnected(void * user_data){
    int flag = (int)(intptr_t)user_data;
    _network_connected_status_led(0); // 灭灯说明网络断开
    QM_ESL2_LOG("_network_disconnected ENTRY  flag:%d!!",flag);
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp = lv_display_get_1st_scr();
    if(_disp != NULL){
        if(flag == 1){
            tip_bg_1 = _show_tip_ui(_disp->act_scr,"等待网络连接",100,45);
        }
        update_page_footer_icon(1,FOOTER_STATUS_DISCONNECT_NET);
    }
    

    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 != NULL){
        // 检查网络
        //显示ui  等待网络连接
        if(flag == 1){
            tip_bg_2 = _show_tip_ui(_disp2->act_scr,"等待网络连接",100,45);
        }
        update_page_footer_icon(2,FOOTER_STATUS_DISCONNECT_NET);
    }
}

static void update_param_ipaddr(void) {
	char *ipaddr = NULL;
	char ip[NI_MAXHOST] = {0};
       get_local_ip(ip, NI_MAXHOST);
	ipaddr = param_get_string("net:ipaddr", NULL);
	if (ipaddr == NULL) {
	       param_set_string("net:ipaddr", ip);
	} else if (strcmp(ip,ipaddr) !=0) {
	       param_set_string("net:ipaddr", ip);
	}
}

static void * _reconnect_socket_listener(void * user_data){
    reconnect_socket_listener();
    return NULL;
}

static int ping_host(const char *host) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s > /dev/null 2>&1", host);
    int ret = system(cmd);
    return ret == 0 ? 1 : 0;
}

static int esl_check_network(){
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        int ret = qm_wifi_connect_status();
        if(ret != 0){
            // 已连接，检测外网
            int ok_baidu = ping_host("www.baidu.com");
            int ok_tencent = ping_host("www.qq.com");
            int ok_aliyun = ping_host("www.aliyun.com");
            QM_ESL2_LOG("External ping: baidu=%d, tencent=%d, aliyun=%d\n", ok_baidu, ok_tencent, ok_aliyun);
            if (ok_baidu == 0 && ok_tencent == 0 && ok_aliyun == 0) {
                QM_ESL2_LOG("Network connected but cannot access external network, restarting device...");
                return 0;
            }else{
                QM_ESL2_LOG("Network connected and can access external network, niubi plus!!!!!");
                return 1;
            }
        }else{
            return 0;
        }
    #endif
}

static void* _thread_function(void* arg) {
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    prctl(PR_SET_NAME, "check_network"); // 设置线程名称
    int count = 0;
    int disconnect_count = 0; // 新增断开计数器
    while (1) {
        //1处于连接状态，返回0断开
        int ret = esl_check_network();
        if (ret == 0){
            disconnect_count++; // 网络断开计数+1
            QM_ESL2_LOG("Network disconnected 12 times, now disconnect_count:%d...",disconnect_count);
            if(reconnect_count > 0){
                sleep(5);
            }
            ret = esl_check_network();
            if(ret == 0){
                if(_network_connected_status != 0){
                    //断开
                    if(reconnect_count > 0){
                        lv_async_call(_network_disconnected,(void *)(intptr_t)0);
                    }else{
                        #if 1
                        lv_async_call(_network_disconnected,(void *)(intptr_t)1);
                        #endif
                    }
                }
                _network_connected_status = 0;
                // 连续18次断开则重启
                if(disconnect_count >= 12){
                    QM_ESL2_LOG("Network disconnected 12 times, restarting device...");
                    pthread_t thread_id;
                    pthread_create(&thread_id, NULL, reboot_thread, NULL);
                    pthread_detach(thread_id);
                    disconnect_count = 0; // 重置计数
                }
            }else {
                disconnect_count = 0; // 连接成功重置计数
                QM_ESL2_LOG("Network disconnected 12 times, now disconnect_count is recount...");
                if(_network_connected_status != 1){
                    update_param_ipaddr();
                    if(reconnect_count > 0){
                        lv_async_call(_network_connected,(void *)(intptr_t)0);
                    }else{
                        lv_async_call(_network_connected,(void *)(intptr_t)1);
                    }
                    reconnect_count++;
                }
                _network_connected_status = 1;
                if(_is_master == 1){
                    count = count % 5;
                    if(count == 0){
                        char ip[NI_MAXHOST] = {0};
                        get_local_ip(ip, NI_MAXHOST);
                        if(!is_string_empty(ip)){
                            char json_str[256];
                            snprintf(json_str, sizeof(json_str), 
                                    "{\"date_type\":998,\"master_group_id\":%d,\"master_ip\":\"%s\"}", 
                                    _master_group_id, ip);
                            char new_ip[NI_MAXHOST] = {0}; 
                            int a, b, c, d; 
                            sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
                            d = k_master_ip_post;
                            sprintf(new_ip, "%d.%d.%d.%d", a, b, c, d); 
                            send_broadcast_message(new_ip,json_str);
                        }
                    }
                    count++;
                }
            }
        }else {
            disconnect_count = 0; // 连接成功重置计数
            QM_ESL2_LOG("Network disconnected 12 times, now disconnect_count:%d...",disconnect_count);
            if(_network_connected_status != 1){
                update_param_ipaddr();
                if(reconnect_count > 0){
                    lv_async_call(_network_connected,(void *)(intptr_t)0);
                }else{
                    lv_async_call(_network_connected,(void *)(intptr_t)1);
                }
                reconnect_count++;
            }
            _network_connected_status = 1;
            if(_is_master == 1){
                count = count % 5;
                if(count == 0){
                    char ip[NI_MAXHOST] = {0};
                    get_local_ip(ip, NI_MAXHOST);
                    if (!is_string_empty(ip)) {
                        char json_str[256]; // Buffer for the JSON string
                        snprintf(json_str, sizeof(json_str), 
                             "{\"date_type\":998,\"master_group_id\":%d,\"master_ip\":\"%s\"}", 
                            _master_group_id, ip);
                        char new_ip[NI_MAXHOST]= {0}; 
                        int a, b, c, d; 
                        sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
                        d = k_master_ip_post;
                        sprintf(new_ip, "%d.%d.%d.%d", a, b, c, d); 
                        send_broadcast_message(new_ip,json_str);
                    }
                }
                count++;
            }
        }
        sleep(10); // 每3秒执行一次
    }
    #else 
    int count = 4;
    while (1) {
        //sleep(10); // 每3秒执行一次
        if(_network_connected_status != 1){
            lv_async_call(_network_connected,(void *)(intptr_t)1);
        }
        _network_connected_status = 1;
        if(_is_master == 1){
            count = count % 5;
            if(count == 0){
                char * ip = "172.16.1.202";
                cJSON *json = cJSON_CreateObject();
                if (json == NULL) {
                    return;
                }
                cJSON_AddNumberToObject(json,"date_type",998);
                cJSON_AddNumberToObject(json,"master_group_id",_master_group_id);
                cJSON_AddStringToObject(json,"master_ip",ip);
                char *json_str = cJSON_PrintUnformatted(json);
                char new_ip[NI_MAXHOST]= {0}; 
                int a, b, c, d; 
                sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
                d = k_master_ip_post;
                sprintf(new_ip, "%d.%d.%d.%d", a, b, c, d); 
                send_broadcast_message(new_ip,json_str);
                cJSON_Delete(json);
            }
        }
        sleep(10); // 每3秒执行一次
        count++;
    }
    #endif
    return NULL;
}

static void _start_thread() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, _thread_function, NULL);
    pthread_detach(thread_id); // 分离线程，使其在终止时自动释放资源
}

static void _start_check_life_thread() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, check_life_thread, _master_device_no);
    pthread_detach(thread_id); // 分离线程，使其在终止时自动释放资源
}

void xos_esl_ui_init(void){
    QM_ESL2_LOG("xos_esl_ui_init");
    //=======公共==========
    initializeSerialNumber();
    common_service_init();
    boot2linux();
    //初始化 获取一些配置信息
    char * http_url = param_get_string("setting.esl2:host", "https://esl-service.agppay.com");
    //char * http_url = param_get_string("setting.esl2:host", "http://8.130.19.30");
    set_http_url(http_url);
    int esl_parse_type = param_get_int("setting.esl2:esl_parse_type", 3);
    //int esl_parse_type = param_get_int("setting.esl2:esl_parse_type", 1);
    set_esl_type(esl_parse_type);
    //lv_ll_init();
    char * userName = param_get_string("setting.esl2:userName", NULL);
    QM_ESL2_LOG("userName =======================%s\n",userName);
    char * userPW = param_get_string("setting.esl2:userPW", NULL);
    QM_ESL2_LOG("userPW =======================%s\n",userPW);
    char * store = param_get_string("setting.esl2:store", NULL);
    QM_ESL2_LOG("store =======================%s\n",store);
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    char ssid[QM_MAX_CFG_SSID_STR_LEN];
    #else
    char ssid[ESL_MAX_CFG_SSID_STR_LEN];
    #endif
    get_ssid(&ssid);
    set_userName(userName);
    set_userPW(userPW);
    set_store(store);
    set_ssid_2(ssid);
    int rotation = param_get_int("setting.photo:show_hor", 0);
    printf("rotation1=%d=========================\n",rotation);
    set_rotation_1(rotation);
    rotation = param_get_int("setting.photo:show_hor2", 0);
    printf("rotation2=%d===========================\n",rotation);
    set_rotation_2(rotation);

    //主/从设备  双屏只需要开启一次
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, create_listen_socket, NULL) != 0) {
        perror("pthread_create");
    }
    pthread_detach(thread_id);
    //=======公共==========
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp = lv_display_get_1st_scr();
    if(_disp == NULL){
        //开启线程
        //_start_thread();
        return;
    }
    QM_ESL2_LOG("_disp->act_scr111111111111=%p\n",_disp->act_scr);
    _init_ui(_disp->act_scr,1,1);
    init_show_page_footer_ui(_disp->sys_layer,1);
}

void xos_esl_ui_init_2(void){
    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp = lv_display_get_2nd_scr();
    if(_disp == NULL){
        //开启线程
        _start_thread();
        return;
    }
    QM_ESL2_LOG("_disp->act_scr2222222222222222=%p\n",_disp->act_scr);
    _init_ui(_disp->act_scr,2,1); 
    init_show_page_footer_ui(_disp->sys_layer,2);
    /* page_overlay_raise removed: demo override is attached during UI rendering. */
    //开启线程
    _start_thread();
}

static void _init_ui(lv_obj_t * top,int bg_index,int type){
    lv_obj_set_style_pad_all(top, 0, 0);
    lv_obj_set_style_radius(top, 0, 0);
    lv_obj_set_style_border_width(top, 0, 0);
    set_top(top,bg_index);
    char * device_sn = "";
    if(bg_index == 1){
        device_sn = _concat_strings(deviceSerialNumber,"_1");
        set_device_sn(device_sn,1);
    }else{
        device_sn = _concat_strings(deviceSerialNumber,"_2");
        set_device_sn(device_sn,2);
    }
    QM_ESL2_LOG("device_sn : %s\n",device_sn);
    free(device_sn);
    if(type == 1){
        extern void clear_label(int index);
        clear_label(bg_index);
        _query_data_and_show_ui_local(top,bg_index);
    }else if(type == 2){
        int esl_type = get_esl_type();
        if(esl_type == 2){
            char * token = get_token();
            if(token == NULL){
                _init_esl_token(bg_index);
            }
        }
        int flag = _init_esl_mqtt(top,bg_index);
        if(flag == -1){
            //删除本地信息
            if (bg_index == 1) {
                delete_ui_and_local_data_1(1);
            }else{
                delete_ui_and_local_data_2(1);
            }
            return;
        }else if(flag == 0){
            return;
        }

        async_update_footer_icon(bg_index);
        _query_data_and_show_ui(top,bg_index);
    }
}

static void _quit_empty_ui(int bg_index){
    if(bg_index == 1){
        if (lv_obj_is_valid(empty_bg_1) && empty_bg_1 != NULL){
            lv_obj_delete(empty_bg_1);
            empty_bg_1 = NULL;
        }
    }else{
        if (lv_obj_is_valid(empty_bg_2) && empty_bg_2 != NULL){
            lv_obj_delete(empty_bg_2);
            empty_bg_2 = NULL;
        }
    }
}

static void _toggle_green_led_visibility_anim(void * var, int32_t v) {
    lv_obj_t *green_led = (lv_obj_t *)var;
    if (lv_obj_is_valid(green_led) && green_led != NULL) {
        lv_obj_set_style_opa(green_led, v, 0);
    }
}

static void* _show_green_led_1(void * user_data){
    set_top_show(1);
    lv_obj_t * top = get_top_layer1(1);
    if(top == NULL || !lv_obj_is_valid(top)){
        return NULL;
    }
    if(green_led_anim_1 != NULL){
        /* QM_ESL2_LOG("green_led_anim_1  delete:%p=========================",green_led_anim_1);
        lv_anim_delete(green_led_anim_1,NULL); */
        return NULL;
    }
    green_led_1 = lv_obj_create(top);
    lv_obj_set_size(green_led_1, 20, 20); // 设置小圆点的大小
    lv_obj_set_style_bg_color(green_led_1, lv_color_hex(0x00FF00), 0); // 设置为绿色
    lv_obj_set_style_radius(green_led_1, LV_RADIUS_CIRCLE, 0); // 设置为圆形
    lv_obj_set_scrollbar_mode(green_led_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(green_led_1, LV_ALIGN_TOP_LEFT, 10, 10); // 左上角对齐

    QM_ESL2_LOG("green_led_1  create:%p=========================",green_led_1);

    // 创建动画
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, green_led_1);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_time(&a, 500); // 动画持续时间
    lv_anim_set_playback_time(&a, 500); // 回放时间
    //lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // 无限次重复
    lv_anim_set_repeat_count(&a, 3); // 无限次重复
    lv_anim_set_exec_cb(&a, _toggle_green_led_visibility_anim);
    lv_anim_start(&a);
    green_led_anim_1 = &a;
    QM_ESL2_LOG("green_led_anim_1  create:%p=========================",green_led_anim_1);

    return green_led_1;
}

static void* _show_green_led_2(void * user_data){
    set_top_show(2);
    lv_obj_t * top = get_top_layer2(2);
    if(top == NULL || !lv_obj_is_valid(top)){
        return NULL;
    }
    if(green_led_anim_2 != NULL){
        /* QM_ESL2_LOG("green_led_anim_2  delete:%p=========================",green_led_anim_2);
        lv_anim_delete(green_led_anim_2,NULL); */
        return NULL;
    }
    green_led_2 = lv_obj_create(top);
    lv_obj_set_size(green_led_2, 20, 20); // 设置小圆点的大小
    lv_obj_set_style_bg_color(green_led_2, lv_color_hex(0x00FF00), 0); // 设置为绿色
    lv_obj_set_style_radius(green_led_2, LV_RADIUS_CIRCLE, 0); // 设置为圆形
    lv_obj_set_scrollbar_mode(green_led_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(green_led_2, LV_ALIGN_TOP_LEFT, 10, 10); // 左上角对齐

    QM_ESL2_LOG("green_led_2  create:%p=========================",green_led_2);
    // 创建动画
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, green_led_2);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_time(&a, 500); // 动画持续时间
    lv_anim_set_playback_time(&a, 500); // 回放时间
    //lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // 无限次重复
    lv_anim_set_repeat_count(&a, 3); // 无限次重复
    lv_anim_set_exec_cb(&a, _toggle_green_led_visibility_anim);
    //lv_
    lv_anim_start(&a);
    green_led_anim_2 = &a;
    QM_ESL2_LOG("green_led_anim_2  create:%p=========================",green_led_anim_2);

    return green_led_2;
}

void show_green_led_sync(int index){
    if(index == 1){
        _show_green_led_1(NULL);
    }else if(index == 2){
        _show_green_led_2(NULL);
    }
}

static void _clear_green_led_1(void * user_data){
    if(green_led_anim_1 != NULL){
        QM_ESL2_LOG("green_led_anim_1  delete:%p=========================",green_led_anim_1);
        lv_anim_delete(green_led_anim_1,NULL);
        green_led_anim_1 = NULL;
    }
    if(lv_obj_is_valid(green_led_1) && green_led_1 != NULL){
        QM_ESL2_LOG("green_led_1  delete:%p=========================",green_led_1);
        lv_obj_delete(green_led_1);
        green_led_1 = NULL;
    }
    return;
}

static void _clear_green_led_2(void * user_data){
    if(green_led_anim_2 != NULL){
        QM_ESL2_LOG("green_led_anim_2  delete:%p=========================",green_led_anim_2);
        lv_anim_delete(green_led_anim_2,NULL);
        green_led_anim_2 = NULL;
    }
    if(lv_obj_is_valid(green_led_2) && green_led_2 != NULL){
        QM_ESL2_LOG("green_led_2  delete:%p=========================",green_led_2);
        lv_obj_delete(green_led_2);
        green_led_2 = NULL;
    }
}

void clear_green_led_sync(int index){
    if(index == 1){
        _clear_green_led_1(NULL);
    }else if(index == 2){
        _clear_green_led_2(NULL);
    }
}

void set_master_group_id(int group_id){
    _master_group_id = group_id;
}

void restart_device_1(void * user_data){
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp = lv_display_get_1st_scr();
    if(_disp == NULL){
        return;
    }
    tip_bg_1 = _show_tip_ui(_disp->top_layer,"收到重启消息，即将重启",50,60);
}

void restart_device_2(void * user_data){
    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 == NULL){
        return;
    }
    // 检查网络
    //显示ui  等待网络连接
    tip_bg_2 = _show_tip_ui(_disp2->top_layer,"收到重启消息，即将重启",50,60);
}

void discount_restart_device_1(void * user_data){
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp = lv_display_get_1st_scr();
    if(_disp == NULL){
        QM_ESL2_LOG("discount_restart_device_1 is null\n");
        return;
    }
    QM_ESL2_LOG("discount_restart_device_1 is not null\n");
    tip_bg_1 = _show_tip_ui(_disp->top_layer,"网络发送异常，即将重启",50,60);
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, reboot_thread, NULL);
    pthread_detach(thread_id);
}

void discount_restart_device_2(void * user_data){
    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 == NULL){
        QM_ESL2_LOG("discount_restart_device_2 is null\n");
        return;
    }
    QM_ESL2_LOG("discount_restart_device_2 is not null\n");
    tip_bg_2 = _show_tip_ui(_disp2->top_layer,"网络发送异常，即将重启",50,60);
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, reboot_thread, NULL);
    pthread_detach(thread_id);
}

lv_obj_t * get_green_led(int index){
    if(index == 1){
        return green_led_1;
    }else if(index == 2){
        return green_led_2;
    }
    return NULL;
}

void set_http_url_to_param_ini(char * http_url){
    param_set_string("setting.esl2:http_url", http_url);
    set_http_url(http_url);
}
void set_esl_parse_type_to_param_ini(int esl_parse_type){
    param_set_int("setting.esl2:esl_parse_type", esl_parse_type);
    set_esl_type(esl_parse_type);
}

#endif
