#include "page_footer.h"

#include "serial_number.h"
#include "xos_string_ids.h"
#include "../conf/conf.h"
#include <time.h>
#include "../ctrl/esl_service.h"
#ifdef CONFIG_XOS_INN_QUANTP
#include "recv_ntp_time.h"
#endif


typedef struct {
    lv_obj_t *page_footer;
    lv_obj_t *datetime_label;
    lv_timer_t *datetime_timer;
    bool has_init;
} pagefooter_lvobjs;


#define COUNT_ELEMET  4
#define FOOTER_HEIGHT 28
#define FOOTER_UPDATE_INTERVAL    3000    // 更新时间间隔(ms)
#define FOOTER_FONT_SIZE         16      // 字体大小
#define FOOTER_TIME_BUF_SIZE     32      // 时间字符串缓冲区大小
#define FOOTER_TEXT_BUF_SIZE     32      // 文本缓冲区大小

//static
static pagefooter_lvobjs s_pagefooter_objs[3] = {0};

//static
static void add_icons_or_text(page_footer_t *pagefooterArray, int bg_index);
static void _update_date_time(pagefooter_lvobjs* pagefooter_lvobjs);

// 2. 优化定时器回调函数，使用宏来生成
// #define GENERATE_UPDATE_TIME_FUNC(index) \
//     static void _update_date_time_##index(lv_timer_t *t) { \
        
//         _update_date_time(&s_pagefooter_objs[index]); \
//     }

// GENERATE_UPDATE_TIME_FUNC(1)
// GENERATE_UPDATE_TIME_FUNC(2)

static void _update_date_time_1(lv_timer_t *t) { 
        
    _update_date_time(&s_pagefooter_objs[1]); 
}

static void _update_date_time_2(lv_timer_t *t) { 
        
    _update_date_time(&s_pagefooter_objs[2]); 
}

// 3. 创建一个函数指针数组来存储回调函数
typedef void (*update_time_func_t)(lv_timer_t *);
static const update_time_func_t update_time_funcs[] = {
    NULL,               // index 0 不使用
    _update_date_time_1,
    _update_date_time_2
};

static void init_page_footer(void *act_screen, page_footer_t *pagefooterArray, int bg_index)
{
    if ( pagefooterArray == NULL) {
        LV_LOG_ERROR("init page footering failed.");
        return;
    }

    lv_obj_t *bg_wnd = (lv_obj_t *)act_screen;
    //
    lv_obj_t *pagefooter_bg = lv_obj_create(bg_wnd);
    lv_obj_set_size(pagefooter_bg, 480, FOOTER_HEIGHT);
    lv_obj_set_style_bg_opa(pagefooter_bg, 0, 0);
    
    // 添加以下代码来禁用边框和滚动
    lv_obj_set_style_border_width(pagefooter_bg, 0, 0);
    lv_obj_clear_flag(pagefooter_bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(pagefooter_bg, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    //
    lv_obj_set_flex_flow(pagefooter_bg, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(pagefooter_bg, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    s_pagefooter_objs[bg_index].page_footer = pagefooter_bg;
    //draw_border_for_debug(pagefooter_bg);

    add_icons_or_text(pagefooterArray,bg_index);
}

static lv_obj_t * get_child_icon(int bg_index){

    lv_obj_t *target_img = NULL;

    // 查找目标图标
    uint32_t child_cnt = lv_obj_get_child_cnt(s_pagefooter_objs[bg_index].page_footer);
    for (uint32_t i = 0; i < child_cnt ; i++) {
        lv_obj_t * child =  lv_obj_get_child(s_pagefooter_objs[bg_index].page_footer, i);
        const lv_obj_class_t * obj_class = lv_obj_get_class(child);
        if (obj_class == &lv_image_class) {
            target_img = child; 
            //const void *current_src = lv_img_get_src(target_img);
            //printf(">>>>>>>>>>>>>>>>>>>>>>>>> current: %s\n", current_src);
            return target_img;
        }
    }
    return NULL;
}

static void update_footer_icon_thread(void* param)
{
    int bg_index = (int)param;
    if (1 == bg_index || 2 == bg_index) {
        update_page_footer_icon(bg_index, FOOTER_STATUS_DISCONNECT_NET);
    }
}

void async_update_footer_icon(int bg_index)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, update_footer_icon_thread, bg_index);
    pthread_detach(thread_id); // 分离线程，使其在终止
}

void update_page_footer_icon(int bg_index, page_footer_connet_status_t IsConnect_Status)
{
    if (!s_pagefooter_objs[bg_index].has_init && !s_pagefooter_objs[bg_index].page_footer) {
        LV_LOG_WARN("Page footer not initialized");
        return;
    }
    lv_obj_t *img = get_child_icon(bg_index);
    if(!img) return;
    switch (IsConnect_Status)
    {
    case FOOTER_STATUS_DISCONNECT_NET:
        {
            lv_image_set_src(img, k_path_pagefooter_disconnet);
        }
        break;

    case FOOTER_STATUS_CONNECTED_NET:
        {
            char * device_sn = get_device_sn(bg_index);
            char * master_device_sn = get_master_device_sn();
            if(master_device_sn != NULL && strcmp(device_sn, master_device_sn) == 0){
                lv_image_set_src(img, k_path_pagefooter_wifi_connet_master);
            }else{
                lv_image_set_src(img, k_path_pagefooter_wifi_connet_slave);
            }
        }
        break;
    default:
        break;
    }
}

// 5. 优化时间更新函数
static void _update_date_time(pagefooter_lvobjs* pagefooter_lvobjs)
{
    time_t timer;
    struct tm* ptTm = NULL;

    if (NULL == pagefooter_lvobjs->datetime_label) {
        return;
    }

    time(&timer);
    ptTm = localtime(&timer);
    if (!ptTm) {
        return;
    }

    char pTimeBuf[FOOTER_TIME_BUF_SIZE];
    int model_24 = 1;
#ifdef CONFIG_XOS_INN_QUANTP
    model_24 = get_xos_24hour_mode();
#endif

    const char* format = model_24 ? 
        "%02d-%02d %02d:%02d" : 
        "%s %02d-%02d %02d:%02d";

    if (model_24) {
        snprintf(pTimeBuf, FOOTER_TIME_BUF_SIZE, format,
                ptTm->tm_mon + 1, ptTm->tm_mday,
                ptTm->tm_hour, ptTm->tm_min);
    } else {
        const char* period = (ptTm->tm_hour < 12) ? 
            GetTextRes(STRID_CALENDAR_CLK_IDLE_AM) : 
            GetTextRes(STRID_CALENDAR_CLK_IDLE_PM);
        
        snprintf(pTimeBuf, FOOTER_TIME_BUF_SIZE, format,
                period, ptTm->tm_mon + 1, ptTm->tm_mday,
                (ptTm->tm_hour % 12), ptTm->tm_min);
    }

    lv_label_set_text(pagefooter_lvobjs->datetime_label, pTimeBuf);
}

// 4. 优化定时器创建逻辑
static void create_datetime_timer(int bg_index, pagefooter_lvobjs* obj)
{
    if (obj->datetime_timer != NULL) {
        lv_timer_del(obj->datetime_timer);
        obj->datetime_timer = NULL;
    }

    if (bg_index > 0 && bg_index < sizeof(update_time_funcs)/sizeof(update_time_funcs[0])) {
        obj->datetime_timer = lv_timer_create(update_time_funcs[bg_index], 
                                            FOOTER_UPDATE_INTERVAL, NULL);
        update_time_funcs[bg_index](NULL);
    }
}


static void add_icons_or_text(page_footer_t *pagefooterArray, int bg_index)
{
    if (s_pagefooter_objs[bg_index].has_init) {
        LV_LOG_WARN("Pagefooter already created");
        return;
    }
    bool has_datetime = false;
    lv_font_t *ft_font_ttf = NULL;
    qua_xos_ft_init_with_size_path(&ft_font_ttf, 16, k_path_ttf, 0);
   
    
    for (short i = 0; i < COUNT_ELEMET; i++) {
        page_footer_t *pagefooter = &pagefooterArray[i];
        if (!has_datetime && pagefooter->enable_datetime) {
            //has_datetime = true;
            // if(s_stabarobjs.datetime_label != NULL){
            //     lv_obj_del(s_stabarobjs.datetime_label);
            //     s_stabarobjs.datetime_label = NULL;
            // }
            s_pagefooter_objs[bg_index].datetime_label = lv_label_create(s_pagefooter_objs[bg_index].page_footer);
            lv_obj_set_style_pad_left(s_pagefooter_objs[bg_index].datetime_label, 5, 0);
            lv_label_set_text(s_pagefooter_objs[bg_index].datetime_label, "");
            #ifdef CONFIG_XOS_DARK_MODE
            lv_obj_set_style_text_color(s_pagefooter_objs[bg_index].datetime_label, lv_color_white(), 0);
            #else
            lv_obj_set_style_text_color(s_pagefooter_objs[bg_index].datetime_label, lv_color_black(), 0);
            #endif
            
            lv_obj_set_style_text_font(s_pagefooter_objs[bg_index].datetime_label, ft_font_ttf, 0);
            //
            create_datetime_timer(bg_index, &s_pagefooter_objs[bg_index]);
            
        }
        else if (pagefooter->is_icon == 0 && pagefooter->ctn.text != NULL) { //
            lv_obj_t *label = lv_label_create(s_pagefooter_objs[bg_index].page_footer);
            #ifdef CONFIG_XOS_DARK_MODE
            lv_obj_set_style_text_color(label, lv_color_white(), 0);
            #else
            lv_obj_set_style_text_color(label, lv_color_black(), 0);
            #endif
            lv_label_set_text(label, pagefooter->ctn.text);
            
            lv_obj_set_style_text_font(label, ft_font_ttf, 0);
        }
        else if (pagefooter->is_icon == 1 && pagefooter->ctn.icon != NULL) { // icon
            lv_obj_t *img = lv_image_create(s_pagefooter_objs[bg_index].page_footer);
            lv_image_set_src(img, pagefooter->ctn.icon);
            // lv_obj_set_size(img, 20, 20);
            //lv_img_set_zoom(img, 100);

        }
        else {
            LV_LOG_ERROR("known.");
        }
    }
    s_pagefooter_objs[bg_index].has_init = true;
}

static void hidden_page_footer(int bg_index)
{
    if(s_pagefooter_objs[bg_index].page_footer != NULL){
        lv_obj_add_flag(s_pagefooter_objs[bg_index].page_footer, LV_OBJ_FLAG_HIDDEN);
    }
}

static void show_page_footer(int bg_index)
{
    if(s_pagefooter_objs[bg_index].page_footer != NULL){
        lv_obj_clear_flag(s_pagefooter_objs[bg_index].page_footer, LV_OBJ_FLAG_HIDDEN);
    }
}


static void _show_self_ui(void * active_screen,int bg_index)
{
    #define kSBarCnt 4
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
    char device_sn[32] = {0};
    sprintf(device_sn,"%s-%d",deviceSerialNumber,bg_index);
    page_footer_t page_footer_objs[kSBarCnt] = {
        { .ctn.text = version, .is_icon = 0, .enable_datetime = false},
        { .ctn.text = device_sn, .is_icon = 0, .enable_datetime = false},
        { .ctn.text = NULL, .is_icon = 0, .enable_datetime = true },
        {
            .ctn.icon = k_path_pagefooter_untie, 
            .is_icon = 1, .enable_datetime = false
        }
    };
    int k_pagefooter_height = 28;

    init_page_footer(active_screen, page_footer_objs,bg_index);
}

void init_show_page_footer_ui(void * active_screen,int bg_index)
{
    _show_self_ui(active_screen, bg_index);
}

void deinit_page_footer(int bg_index)
{

    if (s_pagefooter_objs[bg_index].datetime_timer) {
        lv_timer_del(s_pagefooter_objs[bg_index].datetime_timer);
        s_pagefooter_objs[bg_index].datetime_timer = NULL;
    }
    
    // 删除 page_footer 会自动删除所有子对象（包括 img）
    if (s_pagefooter_objs[bg_index].page_footer) {
        lv_obj_del(s_pagefooter_objs[bg_index].page_footer);
        s_pagefooter_objs[bg_index].page_footer = NULL;
    }
    
    s_pagefooter_objs[bg_index].datetime_label = NULL;
    s_pagefooter_objs[bg_index].has_init = false;
}

int set_pagefooter_time_from_internet(char* server_ip, int port, int time_zone)
{
#ifdef CONFIG_XOS_INN_QUANTP
    RecvNtpInputParameter input;
    struct timeval ntp_tv;

    strcpy(input.server_ip_, server_ip);
    input.server_port_ = port;
    input.time_zone_ = time_zone;
    input.time_out_ = 10;
    if(recv_ntp_time(&input, &ntp_tv) < 0) {
        LV_LOG_ERROR("%s %d read ntp time failed!\n", __func__, __LINE__);
        return -1;
    }
    
    if(set_ntp_time(&ntp_tv) < 0) {
        LV_LOG_ERROR("%s %d set ntp time failed!\n", __func__, __LINE__);
        return -1;
    }

    LV_LOG_USER("%s %d success\n", __func__, __LINE__);
#endif
    return 0;
}
