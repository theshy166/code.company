#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include "page_esl.h"


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
#include "../utils/pic_utils.h"
#include "../utils/hashmap.h"
#include "page_esl_image.h"
#include "page_esl_html.h"
#include "page_esl_text.h"
#include "../utils/esl_sign.h"
#include "../utils/md5.h"
#include "../utils/utils.h"
#include "serial_number.h"
#include "../ctrl/esl_file.h"
#include "../ctrl/esl_service.h"
#include "../ctrl/common_service.h"
#include "../ctrl/esl_socket_service.h"
#include "../ctrl/mqtt_control.h"
#include "../ctrl/mqtt_subscriber.h"
#include "../ctrl/esl_socket_udp_service.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "qm_wifi_cfg.h"
#include "logo_img.h"
#endif
#include "page_footer.h"
#include "qua_param.h"
#include "../model/esl_ui_struct.h"
#include "../ctrl/uirender.h"

extern lv_font_t *ft_font_ttf_30;
extern lv_font_t *ft_font_ttf_20;

extern esl_ui_struct_t g_ui_struct_1;
extern esl_ui_struct_t g_ui_struct_2;

extern void draw_border_for_debug(lv_obj_t *obj);
extern void qme_set_render_status(int index, bool status);

static lv_obj_t * tip_bg_1;
static lv_obj_t * tip_bg_2;
static lv_obj_t * master_tip_bg;

static lv_obj_t * empty_bg_1;
static lv_obj_t * empty_bg_2;

static lv_obj_t * green_led_1;
static lv_obj_t * green_led_2;


static lv_anim_t *green_led_anim_1 = NULL;
static lv_anim_t *green_led_anim_2 = NULL;

static int _network_connected_status = -1;
static int _init_mqtt_status = 0;
static int thread_created = 0;
static int thread_chech_life_created = 0;

static int _is_master = 0;
static char * _master_device_no;
static int _master_group_id = -1;
static int reconnect_count = 0;
static int _group_id_1 = 0;
static int _group_id_2 = 0;

// 定义互斥锁
pthread_mutex_t broadcast_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t download_mutex = PTHREAD_MUTEX_INITIALIZER;

static void _init_ui(lv_obj_t * bg,int bg_index,int type);
static void _quit_empty_ui(int bg_index);
static void _start_check_life_thread();
static void _clear_green_led(int index);

static int esl_product_item_compare(const void *a, const void *b, void *udata);
static bool esl_product_item_iter(const void *item, void *udata);
static uint64_t esl_product_item_hash(const void *item, uint64_t seed0, uint64_t seed1);
static void* _download_image_or_video_in_thread(void *arg);
static void _download_image_or_video_and_create_thread(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,int index);


esl_product_info_t* find_product_info_by_item_no(const char *item_no, esl_product_info_t **product_info_list, int count) {
    if (item_no == NULL || product_info_list == NULL) {
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        if (product_info_list[i] != NULL && strcmp(product_info_list[i]->item_no, item_no) == 0) {
            return product_info_list[i];
        }
    }

    return product_info_list[0];
}

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

// 创建线程并保存数据
static void _save_data_in_thread(int index, esl_show_model_t *model,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, int product_info_count,char * device_sn) {
    // 创建参数结构体
    esl_file_save_info_t *args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
    if (args == NULL) {
        printf("Failed to allocate memory for thread arguments\n");
        return;
    }
    args->index = index;
    args->model = model;
    args->product_info_list = product_info_list;
    args->product_info_count = product_info_count;
    args->device_setting = device_setting;
    args->device_sn = device_sn;

    // 创建线程
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, save_data_thread, args) != 0) {
        printf("Failed to create thread\n");
        free(args);
        return;
    }
    // 分离线程，使其在终止时自动释放资源
    pthread_detach(thread_id);
}

static int esl_product_item_compare(const void *a, const void *b, void *udata){
    const struct esl_product_item_key_t *ua = a;
    const struct esl_product_item_key_t *ub = b;
    return strcmp(ua->product_item_key, ub->product_item_key);
}

static bool esl_product_item_iter(const void *item, void *udata) {
    const struct esl_product_item_key_t *user = item;
    printf("%s (age=%d)\n", user->product_item_key, user->count);
    return true;
}

static uint64_t esl_product_item_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct esl_product_item_key_t *user = item;
    return hashmap_sip(user->product_item_key, strlen(user->product_item_key), seed0, seed1);
}

static void _show_ui(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,lv_obj_t * top,int width,int height,int index)
{
    long current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    LV_LOG_USER("show_ui ENTRY!!");
    struct hashmap *map = hashmap_new(sizeof(struct esl_product_item_key_t), 0, 0, 0, esl_product_item_hash, esl_product_item_compare, NULL, NULL);
    lv_obj_set_style_pad_all(top, 0, 0);
    lv_obj_set_style_radius(top, 0, 0);
    lv_obj_set_style_border_width(top, 0, 0);
    lv_obj_set_scrollbar_mode(top, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_bg_opa(top,0,LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_t *bg = lv_obj_create(top);
    lv_obj_set_style_pad_all(bg, 0, 0);
    lv_obj_set_style_radius(bg, 0, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_scrollbar_mode(bg, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_bg_opa(bg,0,LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_size(bg, model->width, model->height);
    set_bg(bg,index);
    char * other_3 = device_setting->other_3;
    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    if (is_string_empty(other_3)) {
        free_esl_show_model(model);
        free_device_setting(device_setting);
        free_esl_product_info_array(product_info_list, count);
        hashmap_free(map);
        return;
    }
    int other_3_count = 0;
    char **other_3_arr = convert_string_to_char_array(other_3, &other_3_count);
    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    for (int i = 0; i < model->child_count; i++) {
        LV_LOG_USER("Child %d:\n", i + 1);
        esl_show_model_child_t *child = &model->child_list[i];
        char *type = child->type;
        if (type == NULL) {
            continue;
        }
        if (strcmp(type, "image") == 0) {
            handle_image_extension(bg, child,device_setting, product_info_list, map,count,index - 1,other_3_arr,other_3_count);
        } else if (strcmp(type, "text") == 0) {
            handle_text_extension(bg, child,device_setting, product_info_list, map,count,index,other_3_arr,other_3_count);
        } else if (strcmp(type, "rect") == 0 || strcmp(type, "circle") == 0) {
            lv_obj_t *rect = lv_obj_create(bg);
            lv_obj_set_style_pad_all(rect, 0, 0);
            lv_obj_set_style_radius(rect, 0, 0);
            lv_obj_set_style_border_width(rect, 0, 0);
            lv_obj_set_scrollbar_mode(rect, LV_SCROLLBAR_MODE_OFF);
            
            lv_obj_set_size(rect, child->w, child->h);
            lv_obj_set_pos(rect, child->x, child->y);
            if (!is_string_empty(child->desc.bgColor)) {
                lv_color_t bgColor = lv_color_hex(hex_to_uint32(child->desc.bgColor));
                lv_obj_set_style_bg_color(rect, bgColor, 0);
            }
            if (child->desc.borderRadius > 0) {
                lv_obj_set_style_radius(rect, child->desc.borderRadius, 0);
            }
            if (child->angle != 0) {
                lv_obj_set_style_transform_pivot_y(rect, child->h / 2, 0);
                lv_obj_set_style_transform_pivot_x(rect, child->w / 2, 0);
                lv_obj_set_style_transform_angle(rect, child->angle * 10, 0);
            }
        } else if (strcmp(type, "html") == 0 ){
            handle_html_extension(bg, child,device_setting, product_info_list, map,count,other_3_arr,other_3_count);
        }
    }

    // 视频
    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    /* for (int i = 0; i < model->child_count; i++) {
        LV_LOG_USER("Child %d:\n", i + 1);
        esl_show_model_child_t *child = &model->child_list[i];
        char *type = child->type;
        if (type == NULL) {
            continue;
        }
        if (strcmp(type, "image") == 0) {
            handle_video_extension(bg, child,device_setting, product_info_list, map,count,index - 1,other_3_arr,other_3_count);
        }
    }
    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time); */
    //开启线程 删除本地资源
    #if 1
        pthread_t download_thread;
        compare_data_t *compare_data_t = malloc(sizeof(compare_data_t));
        compare_data_t->index = index;
        pthread_create(&download_thread, NULL, compare_local_resouce, compare_data_t);
        pthread_detach(download_thread);
    #endif
    for (int i = 0; i < other_3_count; i++) {
        printf("array[%d] = %s\n", i, other_3_arr[i]);
        free(other_3_arr[i]); // 释放每个字符串
    }
    free_esl_show_model(model);
    free_device_setting(device_setting);
    free_esl_product_info_array(product_info_list, count);
    free(other_3_arr); // 释放数组指针
    hashmap_free(map);
    LV_LOG_USER("show_ui EXIT!!");
    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
}

void async_show_ui(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,lv_obj_t * top,int index){
    LV_LOG_USER("async_show_ui ENTRY!!");
    int display_id = index - 1;
    los_videoplayer_quit(display_id);
    set_top_hidden(index);
    LV_LOG_USER("--[recylemem]---qme_save_time_callback %d", display_id);
    qme_set_render_status(display_id, false);
    //
    _quit_empty_ui(index);
    LV_LOG_USER("async_show_ui _quit_empty_ui success!!");

    int width = 0;
    int height = 0;
    char * device_sn = get_device_sn(index);

    _show_ui(model,device_setting,product_info_list,count,top,width,height,index);

    LV_LOG_USER("[start render:%d] 8888888888888", display_id);
    _clear_green_led(index);
    qme_set_render_status(display_id, true);
}

static void _query_data_and_show_ui(lv_obj_t * act,int bg_index){
    int width = 0;
    int height = 0;
    int product_count = 0;
    char * device_sn = get_device_sn(bg_index);

    char * product_info_sign = generate_sign(device_sn,"item");
    esl_product_info_t ** product_info_list = query_product_infos(device_sn,"item",product_info_sign,&product_count);
    free(product_info_sign);

    if(product_info_list){
        char * template_sign = generate_sign(device_sn,"tpl");
        esl_show_model_t *model = query_product_template(device_sn,"tpl",template_sign);
        free(template_sign);
        if(model == NULL){
            free_esl_product_info_array(product_info_list,product_count);
            return;
        }
        char * setting_sign = generate_sign(device_sn,"setting");
        esl_device_setting_t * device_setting = query_device_setting(device_sn,"setting",setting_sign);
        free(setting_sign);
        if (device_setting == NULL) {
            free_esl_show_model(model);
            free_esl_product_info_array(product_info_list,product_count);
            return;
        }
        //开启线程  下载资源
        _download_image_or_video_and_create_thread(model,device_setting,product_info_list,product_count,bg_index);
    }else{
        printf("9454687654354676.\n");
    }
}

static void _download_image_or_video_and_create_thread(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,int index){
   // 开启线程下载资源
    show_ui_args_t *args = malloc(sizeof(show_ui_args_t));
    args->model = model;
    args->device_setting = device_setting;
    args->product_info_list = product_info_list;
    args->count = count;
    args->index = index;

    set_top_show(index);
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, _download_image_or_video_in_thread, (void *)args) != 0) {
        perror("Failed to create thread");
        free_esl_product_info_array(product_info_list,count);
        free_esl_show_model(model);
        free_device_setting(device_setting);
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
            LV_LOG_USER("_network_connected tip_bg_1 is valid!!");
            lv_obj_delete(tip_bg_1);
            tip_bg_1 = NULL;
        }
    }else if (index == 2){
        if (lv_obj_is_valid(tip_bg_2) && tip_bg_2 != NULL){
            LV_LOG_USER("_network_connected tip_bg_2 is valid!!");
            lv_obj_delete(tip_bg_2);
            tip_bg_2 = NULL;
        }
    }
}

static void _async_show_ui_after_download(void * arg){
    show_ui_args_t *args = (show_ui_args_t *)arg;
    esl_show_model_t *model = args->model;
    esl_device_setting_t *device_setting = args->device_setting;
    esl_product_info_t **product_info_list = args->product_info_list;
    int count = args->count;
    int index = args->index;

    int display_id = index - 1;
    los_videoplayer_quit(display_id);
    set_top_hidden(index);
    _quit_empty_ui(index);

    LV_LOG_USER("--[recylemem]---qme_save_time_callback %d", display_id);

    lv_obj_t * bg = get_bg(index);
    if(bg != NULL && lv_obj_is_valid(bg)) {
        lv_obj_delete(bg);
        set_bg(NULL,index);
    }

    lv_obj_t * top = get_top(index);
    int width = 0;
    int height = 0;
    _show_ui(model,device_setting,product_info_list,count,top,width,height,index);
    LV_LOG_USER("[start render:%d] 8888888888888", index - 1);

    qme_set_render_status(index - 1, true);
    free(args);
    printf("========_async_show_ui_after_download ----!!\n");
}

void download_image_or_video(esl_show_model_t *model,esl_device_setting_t * device_setting,esl_product_info_t ** product_info_list,int count,int index){
   struct hashmap *map = hashmap_new(sizeof(struct esl_product_item_key_t), 0, 0, 0, esl_product_item_hash, esl_product_item_compare, NULL, NULL);
    char * other_3 = device_setting->other_3;
    if (is_string_empty(other_3)) {
        hashmap_free(map);
        return;
    }
    int other_3_count = 0;
    char **other_3_arr = convert_string_to_char_array(other_3, &other_3_count);
    for (int i = 0; i < model->child_count; i++) {
        esl_show_model_child_t *child = &model->child_list[i];
        char *type = child->type;
        if (type == NULL) {
            continue;
        }
        if (strcmp(type, "image") == 0) {
            handle_image_extension_download(child,device_setting, product_info_list, map,count,index - 1,other_3_arr,other_3_count);
        }
    }
    for (int i = 0; i < other_3_count; i++) {
        printf("array[%d] = %s\n", i, other_3_arr[i]);
        free(other_3_arr[i]); // 释放每个字符串
    }
    free(other_3_arr); // 释放数组指针
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
    hashmap_free(map);
}

static void* _download_image_or_video_in_thread(void *arg) {
    show_ui_args_t *args = (show_ui_args_t *)arg;
    esl_show_model_t *model = args->model;
    esl_device_setting_t *device_setting = args->device_setting;
    esl_product_info_t **product_info_list = args->product_info_list;
    int count = args->count;
    int index = args->index;

    esl_file_save_info_t *esl_file_save_info_args = (esl_file_save_info_t *)malloc(sizeof(esl_file_save_info_t));
    if (esl_file_save_info_args == NULL) {
        free_esl_product_info_array(product_info_list,count);
        free_esl_show_model(model);
        free_device_setting(device_setting);
        free(args);
        printf("Failed to allocate memory for thread arguments\n");
        return;
    }
    esl_file_save_info_args->index = index;
    esl_file_save_info_args->model = model;
    esl_file_save_info_args->product_info_list = product_info_list;
    esl_file_save_info_args->product_info_count = count;
    esl_file_save_info_args->device_setting = device_setting;
    esl_file_save_info_args->device_sn = get_device_sn(index);
    save_data_thread(esl_file_save_info_args);

    pthread_mutex_lock(&download_mutex);
    download_image_or_video(model,device_setting,product_info_list,count,index);
    pthread_mutex_unlock(&download_mutex);

    lv_async_call(_async_show_ui_after_download, args);
    printf("========_download_image_or_video_in_thread: ----end!!\n");
    return NULL;
}

static void _show_empty_ui(lv_obj_t * bg,int bg_index){
    char * device_sn = get_device_sn(bg_index);

    LV_LOG_USER("_show_empty_ui ENTRY!! bg_index:%d",bg_index);
    lv_obj_set_style_pad_all(bg, 0, 0);
    lv_obj_set_style_radius(bg, 0, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_scrollbar_mode(bg, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_size(bg, 800, 1280);

    lv_obj_t *img = lv_img_create(bg);
    _set_image_src(img,k_path_default_bg_img,800,1280,bg_index - 1);

    lv_obj_t *sn_label = lv_label_create(bg);
    lv_label_set_text(sn_label, device_sn);
    lv_font_t *sn_font = NULL;
    qua_xos_ft_init_with_size_path(&sn_font, 30, k_path_ttf, 0);
    lv_obj_set_style_text_font(sn_label, sn_font, 0);

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "SN", device_sn);
    char *data = cJSON_Print(json);
    
    
    lv_obj_t * qr = lv_qrcode_create(bg);
    lv_qrcode_set_size(qr, 300);
    lv_qrcode_update(qr, data, strlen(data));
    //lv_obj_set_style_bg_opa(qr, LV_OPA_0,0);
    lv_obj_set_size(qr, 300, 300);
    lv_obj_set_pos(qr, 250, 250);

    lv_obj_align_to(sn_label, qr, LV_ALIGN_OUT_TOP_MID, 0, -15);

    char ip[NI_MAXHOST];
    param_init(NULL);
    get_local_ip(ip, NI_MAXHOST);
    printf("Local IP address: %s\n", ip);
    if(!is_string_empty(ip)){
        lv_obj_t *ip_label = lv_label_create(bg);
        lv_label_set_text(ip_label, ip);
        lv_font_t *ip_font = NULL;
        qua_xos_ft_init_with_size_path(&ip_font, 30, k_path_ttf, 0);
        lv_obj_set_style_text_font(ip_label, ip_font, 0);
        lv_obj_align_to(ip_label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    }else{
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
      param_deinit();
    char bluetooth_name[30];
    sprintf(bluetooth_name, "tag-%s", deviceSerialNumber); 

    lv_obj_t * bluetooth_label = lv_label_create(bg);
    lv_label_set_text(bluetooth_label, bluetooth_name);
    lv_font_t *bluetooth_font = NULL;
    qua_xos_ft_init_with_size_path(&bluetooth_font, 15, k_path_ttf, 0);
    lv_obj_set_style_text_font(bluetooth_label, bluetooth_font, 0);
    lv_obj_align_to(bluetooth_label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 80);

    free(data);
    cJSON_Delete(json);
    LV_LOG_USER("_show_empty_ui EXIT!! bg_index:%d",bg_index);
}

void show_empty_ui_1(lv_obj_t * top){
    if(lv_obj_is_valid(empty_bg_1) || empty_bg_1 != NULL){
        _quit_empty_ui(1);
    }
    LV_LOG_USER("show_empty_ui_1 ENTRY!! bg_index:%d",1);
    empty_bg_1 = lv_obj_create(top);
    _show_empty_ui(empty_bg_1,1);
}

void show_empty_ui_2(lv_obj_t * top){
    if(lv_obj_is_valid(empty_bg_2) || empty_bg_2 != NULL){
        _quit_empty_ui(2);
    }
    LV_LOG_USER("show_empty_ui_2 ENTRY!! bg_index:%d",2);
    empty_bg_2 = lv_obj_create(top);
    _show_empty_ui(empty_bg_2,2);
}

static void _query_data_and_show_ui_local(lv_obj_t * act,int bg_index){
    long current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    char * device_sn = get_device_sn(bg_index);
    int width = 0;
    int height = 0;
    int product_count = 0;
    int display_id = bg_index - 1;
    los_videoplayer_quit(display_id);

    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    esl_product_info_t ** product_info_list = query_product_infos_local(bg_index,device_sn,&product_count);
    current_time = get_milliseconds();
    printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    if(product_info_list == NULL || product_count == 0){
        LV_LOG_USER("Failed to parse product info JSON.\n");
        if (bg_index == 1) {
            show_empty_ui_1(act);
        } else if (bg_index == 2) {
            show_empty_ui_2(act);
        }
        return;
    }

    if(product_info_list){
        current_time = get_milliseconds();
        printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
        esl_show_model_t *model = query_product_template_local(bg_index,device_sn);
        current_time = get_milliseconds();
        printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
        if(model == NULL ){
            LV_LOG_USER("Failed to parse device model JSON.\n");
            free_esl_product_info_array(product_info_list, product_count);
            if (bg_index == 1) {
                show_empty_ui_1(act);
            } else if (bg_index == 2) {
                show_empty_ui_2(act);
            }
            return;
        }
        current_time = get_milliseconds();
        printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
        esl_device_setting_t * device_setting = query_device_setting_local(bg_index,device_sn);
        current_time = get_milliseconds();
        printf("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
        if(device_setting == NULL){
            LV_LOG_USER("Failed to parse device setting JSON.\n");
            free_esl_product_info_array(product_info_list, product_count);
            free(model);
            if (bg_index == 1) {
                show_empty_ui_1(act);
            } else if (bg_index == 2) {
                show_empty_ui_2(act);
            }
            return;
        }

        _show_ui(model,device_setting,product_info_list,product_count,act,width,height,bg_index);
        LV_LOG_USER("[start render:%d] 8888888888888", display_id);
    }else{
        printf("Failed to parse product info JSON.\n");
    }
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
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
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
    printf("udp_bind groupId1: %d,groupId2:%d, deviceSerialNumber: %s\n", _group_id_1,_group_id_2, deviceSerialNumber);
    recevice_msg_and_send(_group_id_1,_group_id_2);
    return NULL;
}

static void * _download_image_thread(void * user_data){
    download_image_args_t *download_image_args = (download_image_args_t *)user_data;
    char * boot_logo_url = download_image_args->boot_logo_url;
    int bg_index = download_image_args->bg_index;
    if(!is_string_empty(boot_logo_url)){
        LV_LOG_USER("boot_logo_url:%s",boot_logo_url);
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

static int _init_esl_mqtt(lv_obj_t * bg,int bg_index){

    char * device_sn = get_device_sn(bg_index);
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
    if(data == NULL){
        #if 0
            // 检查线程是否已经创建
            pthread_mutex_lock(&broadcast_mutex);
            if (!thread_created) {
                // 创建参数结构体
                socket_send_info_t *args = (socket_send_info_t *)malloc(sizeof(socket_send_info_t));
                if (args == NULL) {
                    LV_LOG_ERROR("Failed to allocate memory for thread arguments");
                    pthread_mutex_unlock(&broadcast_mutex);
                    return -1;
                }
                args->group_id = 0;
                args->device_sn = device_sn;
                args->ip = strdup(ip);
                // 创建线程
                pthread_t thread_id;
                if (pthread_create(&thread_id, NULL, timer_thread_function, args) != 0) {
                    LV_LOG_ERROR("Failed to create thread");
                    pthread_mutex_unlock(&broadcast_mutex);
                    return -1;
                }
                // 分离线程，使其在终止时自动释放资源
                pthread_detach(thread_id);
                thread_created = 1; // 标记线程已创建
            }
            pthread_mutex_unlock(&broadcast_mutex);
        #endif
        return 0;
    }else if(data->query_success == -1){
        free(data);
        return -1;
    }else {
        #if 1
            char * boot_logo_url = data->boot_logo_url;
            //开启线程下载图片，并且替换原来的图片
            //boot_logo_url:http://xunmaotemp.oss-cn-hangzhou.aliyuncs.com/uploads/undefined/20241107/a47cf0979e963fd176d5a02b08a2fb97.jpg
            pthread_t download_thread;
            download_image_args_t *download_image_args = malloc(sizeof(download_image_args_t));
            download_image_args->boot_logo_url = strdup(boot_logo_url);
            download_image_args->bg_index = bg_index;
            pthread_create(&download_thread, NULL, _download_image_thread, download_image_args);
            pthread_detach(download_thread);
        #endif
        if(bg_index == 1){
            _group_id_1 = data->group_id;
        }else if(bg_index == 2){
            _group_id_2 = data->group_id;
        }
    }
    if(!data->master){
        #if 0
            pthread_mutex_lock(&broadcast_mutex);
            // 检查线程是否已经创建
            if (!thread_created) {
                // 创建参数结构体
                socket_send_info_t *args = (socket_send_info_t *)malloc(sizeof(socket_send_info_t));
                if (args == NULL) {
                    LV_LOG_ERROR("Failed to allocate memory for thread arguments");
                    pthread_mutex_unlock(&broadcast_mutex);
                    return -1;
                }
                args->group_id = data->group_id;
                args->device_sn = device_sn;
                args->ip = strdup(ip);
                // 创建线程
                pthread_t thread_id;
                if (pthread_create(&thread_id, NULL, timer_thread_function, args) != 0) {
                    LV_LOG_ERROR("Failed to create thread");
                    pthread_mutex_unlock(&broadcast_mutex);
                    return -1;
                }
                // 分离线程，使其在终止时自动释放资源
                pthread_detach(thread_id);
                thread_created = 1; // 标记线程已创建
            }
            pthread_mutex_unlock(&broadcast_mutex);
        #endif
    }else{
        _master_device_no = strdup(device_sn);
        _master_group_id = data->group_id;
        _is_master = 1;
        set_master_device_sn(device_sn);
        if(_init_mqtt_status == 1){
            return 1;
        }
        //连接mqtt
        AiDevInfo dev = {0};
        dev.server_type = 1;
        dev.hosturl = data->host;
        dev.port = data->port;
        dev.ac = data->username;
        dev.pw = data->password;
        dev.devsn = device_sn;
        dev.clientid = data->client_id;
        dev.subscriber_topic = data->topic;
        dev.product_id = 3;
        init_mqtt(&dev);
        register_connect_cb(mqtt_connect_response);
        register_repose_cb(mqtt_subscriber);

        //开启探活线程
        _start_check_life_thread();
        _init_mqtt_status = 1;
    }
    free_esl_push_server_data(data);
    return 1;
}

static void _network_connected(void * user_data){
    int flag = (int)(intptr_t)user_data;
    LV_LOG_USER("_network_connected ENTRY  flag:%d!!",flag);
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
            LV_LOG_ERROR("Failed to create thread");
            return;
        }
        // 分离线程，使其在终止时自动释放资源
        pthread_detach(thread_id);
    }
    LV_LOG_USER("_network_connected EXIT!!");
}

static void _network_disconnected(void * user_data){
    int flag = (int)(intptr_t)user_data;
    LV_LOG_USER("_network_disconnected ENTRY  flag:%d!!",flag);
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
	char ip[NI_MAXHOST];
       param_init(NULL);
       get_local_ip(ip, NI_MAXHOST);
	ipaddr = param_get_string("net:ipaddr", NULL);
	if (ipaddr == NULL) {
	       param_set_string("net:ipaddr", ip);
	} else if (strcmp(ip,ipaddr) !=0) {
	       param_set_string("net:ipaddr", ip);
	}
       param_deinit();
}

static void * _reconnect_socket_listener(void * user_data){
    reconnect_socket_listener();
    return NULL;
}

static void* _thread_function(void* arg) {
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    prctl(PR_SET_NAME, "check_network"); // 设置线程名称
    int count = 0;
    while (1) {
        //LV_LOG_USER("_network_connected_status : %d \n",_network_connected_status);
        //1处于连接状态，返回0断开
        int ret = qm_wifi_connect_status();
        //LV_LOG_USER("qm_wifi_connect_status ret : %d \n",ret);
        if (ret == 0){
            if(reconnect_count > 0){
                sleep(5);
            }
            ret = qm_wifi_connect_status();
            if(ret == 0){
                if(_network_connected_status != 0){
                    //断开
                    if(reconnect_count > 0){
                        //cancel_current_listen_socket();
                        lv_async_call(_network_disconnected,(void *)(intptr_t)0);
                    }else{
                        #if 1
                        lv_async_call(_network_disconnected,(void *)(intptr_t)1);
                        #endif
                    }
                }
                _network_connected_status = 0;
            }else {
                if(_network_connected_status != 1){
                    update_param_ipaddr();
                    if(reconnect_count > 0){
                        /* pthread_t thread_id;
                        pthread_create(&thread_id, NULL, _reconnect_socket_listener, NULL);
                        pthread_detach(thread_id); */
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
                        char ip[NI_MAXHOST];
                        get_local_ip(ip, NI_MAXHOST);
                        if(!is_string_empty(ip)){
                            char json_str[256]; // Buffer for the JSON string
                            snprintf(json_str, sizeof(json_str), 
                                    "{\"date_type\":998,\"master_group_id\":%d,\"master_ip\":\"%s\"}", 
                                    _master_group_id, ip);
                            char new_ip[NI_MAXHOST]; 
                            int a, b, c, d; 
                            sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
                            d = k_master_ip_post;
                            sprintf(new_ip, "%d.%d.%d.%d", a, b, c, d); 
                            send_broadcast_message(new_ip,json_str);
                        }
                    }
                }
            }
        }else {
            if(_network_connected_status != 1){
                update_param_ipaddr();
                if(reconnect_count > 0){
                    /* pthread_t thread_id;
                    pthread_create(&thread_id, NULL, _reconnect_socket_listener, NULL);
                    pthread_detach(thread_id); */
                    lv_async_call(_network_connected,(void *)(intptr_t)0);
                }else{
                    lv_async_call(_network_connected,(void *)(intptr_t)1);
                }
                reconnect_count++;
            }
            _network_connected_status = 1;
            if(_is_master == 1){
                #if 0
                cJSON *json = cJSON_CreateObject();
                if (json == NULL) {
                    return;
                }
                cJSON_AddNumberToObject(json,"date_type",998);
                cJSON_AddNumberToObject(json,"master_group_id",_master_group_id);
                cJSON_AddStringToObject(json,"master_ip",_master_ip);
                char *json_str = cJSON_PrintUnformatted(json);
                #else
                count = count % 5;
                if(count == 0){
                    char ip[NI_MAXHOST];
                    get_local_ip(ip, NI_MAXHOST);
                    if (!is_string_empty(ip)) {
                        char json_str[256]; // Buffer for the JSON string
                        snprintf(json_str, sizeof(json_str), 
                             "{\"date_type\":998,\"master_group_id\":%d,\"master_ip\":\"%s\"}", 
                            _master_group_id, ip);
                        char new_ip[NI_MAXHOST]; 
                        int a, b, c, d; 
                        sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
                        d = k_master_ip_post;
                        sprintf(new_ip, "%d.%d.%d.%d", a, b, c, d); 
                        send_broadcast_message(new_ip,json_str);
                    }
                }
                #endif
                #if 0
                cJSON_Delete(json);
                #endif
            }
        }
        sleep(10); // 每3秒执行一次
        count++;
    }
    #else 
    int count = 0;
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
                char new_ip[NI_MAXHOST]; 
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
    LV_LOG_USER("xos_esl_ui_init");
    //=======公共==========
    initializeSerialNumber();
    common_service_init();
    //lv_ll_init();
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
    printf("_disp->act_scr111111111111=%p\n",_disp->act_scr);
    //lv_obj_set_style_bg_color(_disp->act_scr, lv_color_white(), LV_PART_MAIN);
    int rotation = param_get_int("setting.photo:show_hor", 0);
    printf("rotation1=%d=========================\n",rotation);
    set_rotation_1(rotation);
    _init_ui(_disp->act_scr,1,1);
    init_show_page_footer_ui(_disp->sys_layer,1);  
    boot2linux();
}

void xos_esl_ui_init_2(void){
    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp = lv_display_get_2nd_scr();
    if(_disp == NULL){
        //开启线程
        _start_thread();
        return;
    }
    printf("_disp->act_scr2222222222222222=%p\n",_disp->act_scr);
    int rotation = param_get_int("setting.photo:show_hor2", 0);
    printf("rotation2=%d===========================\n",rotation);
    set_rotation_2(rotation);
    //lv_obj_set_style_bg_color(_disp->act_scr, lv_color_white(), LV_PART_MAIN);
    _init_ui(_disp->act_scr,2,1); 
    init_show_page_footer_ui(_disp->sys_layer,2);
    //开启线程
    _start_thread();
}

static void _init_ui(lv_obj_t * top,int bg_index,int type){
    //lv_obj_set_style_bg_opa(top, LV_OPA_0, 0);
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
    LV_LOG_USER("device_sn : %s\n",device_sn);
    free(device_sn);
    if(type == 1){
        extern void clear_label(int index);
        clear_label(bg_index);
        _query_data_and_show_ui_local(top,bg_index);
    }else if(type == 2){
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

static void _delete_green_led(lv_timer_t * timer)
{
    lv_obj_t * green_led = (lv_obj_t *)timer->user_data;
    if(lv_obj_is_valid(green_led) && green_led != NULL){
        lv_obj_delete(green_led);
        green_led = NULL;
    }
}

#if 0
static void _toggle_green_led_visibility(lv_timer_t * timer) {
    lv_obj_t *green_led = (lv_obj_t *)timer->user_data;
    if (lv_obj_is_valid(green_led) && green_led != NULL) {
        bool visible = lv_obj_get_style_opa(green_led, LV_PART_MAIN) != LV_OPA_TRANSP;
        lv_obj_set_style_opa(green_led, visible ? LV_OPA_TRANSP : LV_OPA_COVER, 0);
    }
}

void* show_green_led_1(){
    lv_obj_t * top = get_top(1);
    if(top == NULL || !lv_obj_is_valid(top)){
        return NULL;
    }
    if(green_led_timer_1 != NULL){
        lv_timer_pause(green_led_timer_1);
        lv_timer_delete(green_led_timer_1);
        green_led_timer_1 == NULL;
    }
    if(lv_obj_is_valid(green_led_1) && green_led_1 != NULL){
        lv_obj_delete(green_led_1);
        green_led_1 == NULL;
    }
    green_led_1 = lv_obj_create(top);
    lv_obj_set_size(green_led_1, 20, 20); // 设置小圆点的大小
    lv_obj_set_style_bg_color(green_led_1, lv_color_hex(0x00FF00), 0); // 设置为绿色
    lv_obj_set_style_radius(green_led_1, LV_RADIUS_CIRCLE, 0); // 设置为圆形
    lv_obj_set_scrollbar_mode(green_led_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(green_led_1, LV_ALIGN_TOP_LEFT, 10, 10); // 左上角对齐

    // 创建定时任务在200毫秒后删除小圆点
    green_led_timer_1 = lv_timer_create(_toggle_green_led_visibility, 500, green_led_1);
    lv_timer_set_repeat_count(green_led_timer_1, -1); // 只执行一次
    return green_led_1;
}

void* show_green_led_2(){
    lv_obj_t * top = get_top(2);
    if(top == NULL || !lv_obj_is_valid(top)){
        return NULL;
    }
    if(green_led_timer_2 != NULL){
        lv_timer_pause(green_led_timer_2);
        lv_timer_delete(green_led_timer_2);
        green_led_timer_2 == NULL;
    }
    if(lv_obj_is_valid(green_led_2) && green_led_2 != NULL){
        lv_obj_delete(green_led_2);
        green_led_2 == NULL;
    }
    green_led_2 = lv_obj_create(top);
    lv_obj_set_size(green_led_2, 20, 20); // 设置小圆点的大小
    lv_obj_set_style_bg_color(green_led_2, lv_color_hex(0x00FF00), 0); // 设置为绿色
    lv_obj_set_style_radius(green_led_2, LV_RADIUS_CIRCLE, 0); // 设置为圆形
    lv_obj_set_scrollbar_mode(green_led_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(green_led_2, LV_ALIGN_TOP_LEFT, 10, 10); // 左上角对齐

    // 创建定时任务在200毫秒后删除小圆点
    green_led_timer_2 = lv_timer_create(_toggle_green_led_visibility, 500, green_led_2);
    lv_timer_set_repeat_count(green_led_timer_2, -1); // 只执行一次
    return green_led_2;
}

void* clear_green_led(int index){
    if(index == 1){
        if(green_led_timer_1 != NULL){
            lv_timer_pause(green_led_timer_1);
            lv_timer_delete(green_led_timer_1);
            green_led_timer_1 == NULL;
        }
        if(lv_obj_is_valid(green_led_1) && green_led_1 != NULL){
            lv_obj_delete(green_led_1);
            green_led_1 == NULL;
        }
    }else if(index == 2){
        if(green_led_timer_2 != NULL){
            lv_timer_pause(green_led_timer_2);
            lv_timer_delete(green_led_timer_2);
            green_led_timer_2 == NULL;
        }
        if(lv_obj_is_valid(green_led_2) && green_led_2 != NULL){
            lv_obj_delete(green_led_2);
            green_led_2 == NULL;
        }
    }
}

#else
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
        LV_LOG_USER("green_led_anim_1  delete:%p=========================",green_led_anim_1);
        lv_anim_del(green_led_anim_1,NULL);
        green_led_anim_1 == NULL;
    }
    if(lv_obj_is_valid(green_led_1) && green_led_1 != NULL){
        LV_LOG_USER("green_led_1  delete:%p=========================",green_led_1);
        lv_obj_delete(green_led_1);
        green_led_1 == NULL;
    }
    green_led_1 = lv_obj_create(top);
    lv_obj_set_size(green_led_1, 20, 20); // 设置小圆点的大小
    lv_obj_set_style_bg_color(green_led_1, lv_color_hex(0x00FF00), 0); // 设置为绿色
    lv_obj_set_style_radius(green_led_1, LV_RADIUS_CIRCLE, 0); // 设置为圆形
    lv_obj_set_scrollbar_mode(green_led_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(green_led_1, LV_ALIGN_TOP_LEFT, 10, 10); // 左上角对齐

    LV_LOG_USER("green_led_1  create:%p=========================",green_led_1);

    // 创建动画
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, green_led_1);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_time(&a, 500); // 动画持续时间
    lv_anim_set_playback_time(&a, 500); // 回放时间
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // 无限次重复
    lv_anim_set_exec_cb(&a, _toggle_green_led_visibility_anim);
    lv_anim_start(&a);
    green_led_anim_1 = &a;
    LV_LOG_USER("green_led_anim_1  create:%p=========================",green_led_anim_1);

    return green_led_1;
}

static void* _show_green_led_2(void * user_data){
    set_top_show(2);
    lv_obj_t * top = get_top_layer2(2);
    if(top == NULL || !lv_obj_is_valid(top)){
        return NULL;
    }
    if(green_led_anim_2 != NULL){
        LV_LOG_USER("green_led_anim_2  delete:%p=========================",green_led_anim_2);
        lv_anim_del(green_led_anim_2,NULL);
        green_led_anim_2 == NULL;
    }
    if(lv_obj_is_valid(green_led_2) && green_led_2 != NULL){
        LV_LOG_USER("green_led_2  delete:%p=========================",green_led_2);
        lv_obj_delete(green_led_2);
        green_led_2 == NULL;
    }
    green_led_2 = lv_obj_create(top);
    lv_obj_set_size(green_led_2, 20, 20); // 设置小圆点的大小
    lv_obj_set_style_bg_color(green_led_2, lv_color_hex(0x00FF00), 0); // 设置为绿色
    lv_obj_set_style_radius(green_led_2, LV_RADIUS_CIRCLE, 0); // 设置为圆形
    lv_obj_set_scrollbar_mode(green_led_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(green_led_2, LV_ALIGN_TOP_LEFT, 10, 10); // 左上角对齐

    LV_LOG_USER("green_led_2  create:%p=========================",green_led_2);
    // 创建动画
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, green_led_2);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_time(&a, 500); // 动画持续时间
    lv_anim_set_playback_time(&a, 500); // 回放时间
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // 无限次重复
    lv_anim_set_exec_cb(&a, _toggle_green_led_visibility_anim);
    //lv_
    lv_anim_start(&a);
    green_led_anim_2 = &a;
    LV_LOG_USER("green_led_anim_2  create:%p=========================",green_led_anim_2);

    return green_led_2;
}

void* show_green_led(int index){
    LV_LOG_USER("show_green_led  ENTRY index:%d=========================",index);
    if(index == 1){
        //_show_green_led_1();
        lv_async_call(_show_green_led_1,NULL);
    }else if(index == 2){
        //_show_green_led_2();
        lv_async_call(_show_green_led_2,NULL);
    }
    LV_LOG_USER("show_green_led  EXIT index:%d=========================",index);
}

static void _clear_green_led_1(void * user_data){
    if(green_led_anim_1 != NULL){
        LV_LOG_USER("green_led_anim_1  delete:%p=========================",green_led_anim_1);
        lv_anim_del(green_led_anim_1,NULL);
        green_led_anim_1 == NULL;
    }
    if(lv_obj_is_valid(green_led_1) && green_led_1 != NULL){
        LV_LOG_USER("green_led_1  delete:%p=========================",green_led_1);
        lv_obj_delete(green_led_1);
        green_led_1 == NULL;
    }
    return;
}

static void* _clear_green_led_2(void * user_data){
    if(green_led_anim_2 != NULL){
        LV_LOG_USER("green_led_anim_2  delete:%p=========================",green_led_anim_2);
        lv_anim_del(green_led_anim_2,NULL);
        green_led_anim_2 == NULL;
    }
    if(lv_obj_is_valid(green_led_2) && green_led_2 != NULL){
        LV_LOG_USER("green_led_2  delete:%p=========================",green_led_2);
        lv_obj_delete(green_led_2);
        green_led_2 == NULL;
    }
}

void* clear_green_led(int index){
    LV_LOG_USER("clear_green_led  ENTRY index:%d=========================",index);
    if(index == 1){
        lv_async_call(_clear_green_led_1,NULL);
    }else if(index == 2){
        lv_async_call(_clear_green_led_2,NULL);
    }
    LV_LOG_USER("clear_green_led  EXIT index:%d=========================",index);
}

static void _clear_green_led(int index){
    LV_LOG_USER("clear_green_led  ENTRY index:%d=========================",index);
    if(index == 1){
        _clear_green_led_1(NULL);
    }else if(index == 2){
        _clear_green_led_2(NULL);
    }
    LV_LOG_USER("clear_green_led  EXIT index:%d=========================",index);
}
#endif

void show_download_log_1(char* file_name){
    LV_LOG_USER("show_download_log file_name : %s\n",file_name);
    if (file_name != NULL && strlen(file_name) > 0){
        snprintf(g_ui_struct_1.down_log_file_name, sizeof(g_ui_struct_1.down_log_file_name), "正在下载:%s", file_name);
        g_ui_struct_1.down_log_visible = 1;
        g_ui_struct_1.bar_visible = 1;
        g_ui_struct_1.bar_value = 0;
    }
    LV_LOG_USER("show_download_log file_name : %s\n",file_name);
}

void show_download_log_2(char* file_name){
    LV_LOG_USER("show_download_log file_name : %s\n",file_name);
    if (file_name != NULL && strlen(file_name) > 0){
        snprintf(g_ui_struct_2.down_log_file_name, sizeof(g_ui_struct_2.down_log_file_name), "正在下载:%s", file_name);
        g_ui_struct_2.down_log_visible = 1;
        g_ui_struct_2.bar_visible = 1;
        g_ui_struct_2.bar_value = 0;
    }
    LV_LOG_USER("show_download_log file_name : %s\n",file_name);
}

void set_bar_value(int index,int value){
    if(index == 1){
        g_ui_struct_1.bar_value = value;
    }else if(index == 2){
        g_ui_struct_2.bar_value = value;
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

lv_obj_t * get_green_led(int index){
    if(index == 1){
        return green_led_1;
    }else if(index == 2){
        return green_led_2;
    }
    return NULL;
}

#endif
