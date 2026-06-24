#include "page_footer.h"

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "generated/autoconf.h"
#endif
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


#define COUNT_ELEMET  5
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

/**
 * @brief 初始化底部状态栏
 * 
 * 功能说明：
 * 1. 在屏幕右下角创建状态栏容器 (530px 宽, FOOTER_HEIGHT 高)
 * 2. 使用 Flex 布局，元素左右分布 (SPACE_BETWEEN)
 * 3. 容器背景透明，无边框
 * 4. 添加图标或文字元素
 * 
 * 状态栏 UI 布局：
 * +--------------------------------------------+
 * |  [网络图标]           [日期时间]    |
 * +--------------------------------------------+
 * 
 * @param act_screen 父屏幕对象 (sys_layer)
 * @param pagefooterArray 底部栏配置数组
 * @param bg_index 屏幕索引 (1=屏幕1, 2=屏幕2)
 */
static void init_page_footer(void *act_screen, page_footer_t *pagefooterArray, int bg_index)
{
    if ( pagefooterArray == NULL) {
        QM_ESL2_ELOG("init page footering failed.");
        return;
    }

    lv_obj_t *bg_wnd = (lv_obj_t *)act_screen;
    
    // ========== 1. 创建底部栏容器 ==========
    lv_obj_t *pagefooter_bg = lv_obj_create(bg_wnd);
    lv_obj_set_size(pagefooter_bg, 530, FOOTER_HEIGHT);  // 固定宽度530px
    lv_obj_set_style_bg_opa(pagefooter_bg, 0, 0);  // 背景透明
    
    // 禁用边框和滚动
    lv_obj_set_style_border_width(pagefooter_bg, 0, 0);
    lv_obj_clear_flag(pagefooter_bg, LV_OBJ_FLAG_SCROLLABLE);

    // 定位到右下角
    lv_obj_align(pagefooter_bg, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    
    // ========== 2. Flex 布局设置 ==========
    // 水平布局，元素左右分布
    lv_obj_set_flex_flow(pagefooter_bg, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(pagefooter_bg, 
        LV_FLEX_ALIGN_SPACE_BETWEEN,  // 两端对齐
        LV_FLEX_ALIGN_CENTER,          // 垂直居中
        LV_FLEX_ALIGN_CENTER);         // 元素间居中
    
    s_pagefooter_objs[bg_index].page_footer = pagefooter_bg;
    //draw_border_for_debug(pagefooter_bg);

    // ========== 3. 添加图标和文字 ==========
    add_icons_or_text(pagefooterArray,bg_index);
}

/**
 * @brief 获取底部栏中的图标对象（第一个 lv_image 子元素）
 */
static lv_obj_t * get_child_icon(int bg_index){

    lv_obj_t *target_img = NULL;

    uint32_t child_cnt = lv_obj_get_child_cnt(s_pagefooter_objs[bg_index].page_footer);  // 子元素数量
    for (uint32_t i = 0; i < child_cnt ; i++) {
        lv_obj_t * child =  lv_obj_get_child(s_pagefooter_objs[bg_index].page_footer, i);
        const lv_obj_class_t * obj_class = lv_obj_get_class(child);
        if (obj_class == &lv_image_class) {  // 判断是否为图片类型
            target_img = child; 
            return target_img;
        }
    }
    return NULL;
}

/**
 * @brief 线程函数：异步更新底部栏图标为断开状态
 */
static void update_footer_icon_thread(void* param)
{
    int bg_index = (int)param;
    if (1 == bg_index || 2 == bg_index) {
        update_page_footer_icon(bg_index, FOOTER_STATUS_DISCONNECT_NET);  // 断开网络图标
    }
}

/**
 * @brief 异步更新底部栏网络图标（创建线程）
 */
void async_update_footer_icon(int bg_index)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, update_footer_icon_thread, bg_index);
    pthread_detach(thread_id);
}

/**
 * @brief 更新底部状态栏图标
 * 
 * 根据网络连接状态更新 WiFi 图标：
 * - 断开: k_path_pagefooter_disconnet
 * - 连接且为主设备: k_path_pagefooter_wifi_connet_master
 * - 连接且为从设备: k_path_pagefooter_wifi_connet_slave
 */
void update_page_footer_icon(int bg_index, page_footer_connet_status_t IsConnect_Status)
{
    if (!s_pagefooter_objs[bg_index].has_init && !s_pagefooter_objs[bg_index].page_footer) {
        LV_LOG_WARN("Page footer not initialized");
        return;
    }
    lv_obj_t *img = get_child_icon(bg_index);  // 获取图标对象
    if(!img) return;
    switch (IsConnect_Status)
    {
    case FOOTER_STATUS_DISCONNECT_NET:
        lv_image_set_src(img, k_path_pagefooter_disconnet);  // 断开图标
        break;

    case FOOTER_STATUS_CONNECTED_NET:
        {
            char * device_sn = get_device_sn(bg_index);       // 当前屏幕设备SN
            char * master_device_sn = get_master_device_sn(); // 主设备SN
            if(master_device_sn != NULL && strcmp(device_sn, master_device_sn) == 0){
                lv_image_set_src(img, k_path_pagefooter_wifi_connet_master);  // 主设备图标
            }else{
                lv_image_set_src(img, k_path_pagefooter_wifi_connet_slave);   // 从设备图标
            }
        }
        break;
    default:
        break;
    }
}

/**
 * @brief 更新日期时间文字
 * 
 * 按 24小时/12小时 格式，格式为 "MM-DD HH:MM"
 * 只在文字内容变化时才更新（减少 LVGL 重绘）
 */
static void _update_date_time(pagefooter_lvobjs* pagefooter_lvobjs)
{
    time_t timer;
    struct tm* ptTm = NULL;

    if (NULL == pagefooter_lvobjs->datetime_label) {
        return;  // 无标签对象
    }

    time(&timer);  // 获取当前时间戳
    ptTm = localtime(&timer);
    if (!ptTm) {
        return;
    }

    char pTimeBuf[FOOTER_TIME_BUF_SIZE];
    int model_24 = 1;  // 默认 24 小时制
#ifdef CONFIG_XOS_INN_QUANTP
    model_24 = get_xos_24hour_mode();  // 从系统配置读取
#endif

    const char* format = model_24 ? 
        "%02d-%02d %02d:%02d" :       // 24h: 06-16 14:30
        "%s %02d-%02d %02d:%02d";     // 12h: AM 06-16 02:30

    if (model_24) {
        snprintf(pTimeBuf, FOOTER_TIME_BUF_SIZE, format,
                ptTm->tm_mon + 1, ptTm->tm_mday,
                ptTm->tm_hour, ptTm->tm_min);
    } else {
        const char* period = (ptTm->tm_hour < 12) ? 
            GetTextRes(STRID_CALENDAR_CLK_IDLE_AM) :   // "AM"
            GetTextRes(STRID_CALENDAR_CLK_IDLE_PM);    // "PM"
        
        snprintf(pTimeBuf, FOOTER_TIME_BUF_SIZE, format,
                period, ptTm->tm_mon + 1, ptTm->tm_mday,
                (ptTm->tm_hour % 12), ptTm->tm_min);
    }
    char * pTimeBuf_temp = lv_label_get_text(pagefooter_lvobjs->datetime_label);  // 当前文字
    if(strcmp(pTimeBuf_temp,pTimeBuf) != 0){  // 文字变化时才更新
        lv_label_set_text(pagefooter_lvobjs->datetime_label, pTimeBuf);
    }
}

/**
 * @brief 创建日期时间更新定时器（每 3 秒刷新一次）
 */
static void create_datetime_timer(int bg_index, pagefooter_lvobjs* obj)
{
    if (obj->datetime_timer != NULL) {
        lv_timer_del(obj->datetime_timer);  // 先删除旧定时器
        obj->datetime_timer = NULL;
    }

    if (bg_index > 0 && bg_index < sizeof(update_time_funcs)/sizeof(update_time_funcs[0])) {
        obj->datetime_timer = lv_timer_create(update_time_funcs[bg_index], 
                                            FOOTER_UPDATE_INTERVAL, NULL);  // 3s 间隔
        update_time_funcs[bg_index](NULL);  // 立即更新一次
    }
}


/**
 * @brief 添加底部栏元素（图标/文字/日期时间）
 * 
 * 遍历 pagefooterArray 数组，根据配置类型添加：
 * - enable_datetime: 日期时间标签 + 定时器
 * - is_icon=0: 文字标签
 * - is_icon=1: 图标（lv_image）
 */
static void add_icons_or_text(page_footer_t *pagefooterArray, int bg_index)
{
    if (s_pagefooter_objs[bg_index].has_init) {
        LV_LOG_WARN("Pagefooter already created");
        return;
    }
    bool has_datetime = false;
    lv_font_t *ft_font_ttf = NULL;
    qua_xos_ft_init_with_size_path(&ft_font_ttf, 16, k_path_ttf, 0);  // 16px 字体
   
    for (short i = 0; i < COUNT_ELEMET; i++) {  // 遍历所有元素
        page_footer_t *pagefooter = &pagefooterArray[i];
        if (!has_datetime && pagefooter->enable_datetime) {
            // 日期时间标签
            s_pagefooter_objs[bg_index].datetime_label = lv_label_create(s_pagefooter_objs[bg_index].page_footer);
            lv_obj_set_style_pad_left(s_pagefooter_objs[bg_index].datetime_label, 5, 0);  // 左内边距
            lv_label_set_text(s_pagefooter_objs[bg_index].datetime_label, "");
            #ifdef CONFIG_XOS_DARK_MODE
            lv_obj_set_style_text_color(s_pagefooter_objs[bg_index].datetime_label, lv_color_white(), 0);  // 暗色模式：白色
            #else
            lv_obj_set_style_text_color(s_pagefooter_objs[bg_index].datetime_label, lv_color_black(), 0);  // 亮色模式：黑色
            #endif
            lv_obj_set_style_text_font(s_pagefooter_objs[bg_index].datetime_label, ft_font_ttf, 0);
            create_datetime_timer(bg_index, &s_pagefooter_objs[bg_index]);  // 启动定时器
        }
        else if (pagefooter->is_icon == 0 && pagefooter->ctn.text != NULL) {
            // 文字元素（如 SSID、版本号、SN）
            lv_obj_t *label = lv_label_create(s_pagefooter_objs[bg_index].page_footer);
            #ifdef CONFIG_XOS_DARK_MODE
            lv_obj_set_style_text_color(label, lv_color_white(), 0);
            #else
            lv_obj_set_style_text_color(label, lv_color_black(), 0);
            #endif
            lv_label_set_text(label, pagefooter->ctn.text);
            lv_obj_set_style_text_font(label, ft_font_ttf, 0);
        }
        else if (pagefooter->is_icon == 1 && pagefooter->ctn.icon != NULL) {
            // 图标元素（WiFi 图标等）
            lv_obj_t *img = lv_image_create(s_pagefooter_objs[bg_index].page_footer);
            lv_image_set_src(img, pagefooter->ctn.icon);
        }
        else {
            QM_ESL2_ELOG("known.");
        }
    }
    s_pagefooter_objs[bg_index].has_init = true;  // 标记已初始化
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


/**
 * @brief 构建底部状态栏内容并初始化
 * 
 * 底部栏包含 5 个元素：
 * 1. WiFi SSID 文字
 * 2. 固件版本号文字
 * 3. 设备序列号文字
 * 4. 日期时间（自动更新）
 * 5. 网络状态图标
 */
static void _show_self_ui(void * active_screen,int bg_index)
{
    #define kSBarCnt 5
    char version[32] = {0};
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
#define CONFIG_BUILD_ID "1112136"
#define CONFIG_QM_VERSION "V1.0.1"
#define CONFIG_QM_HW_VERSION "10"
#else
#ifndef CONFIG_BUILD_ID
#define CONFIG_BUILD_ID "000"
#endif
#ifndef CONFIG_QM_HW_VERSION
#define CONFIG_QM_HW_VERSION "10"
#endif
#endif
    // 构建版本号字符串: "V1.0.1.1112136 10"
    snprintf(version,sizeof(version),"%s.%s %s",CONFIG_QM_VERSION,CONFIG_BUILD_ID,CONFIG_QM_HW_VERSION);
    char device_sn[32] = {0};
    sprintf(device_sn,"%s-%d",deviceSerialNumber,bg_index);  // 设备SN
    char * ssid = get_ssid_2();  // WiFi SSID
    
    // 配置底部栏 5 个元素
    page_footer_t page_footer_objs[kSBarCnt] = {
        { .ctn.text = ssid, .is_icon = 0, .enable_datetime = false},       // [0] WiFi名
        { .ctn.text = version, .is_icon = 0, .enable_datetime = false},     // [1] 版本号
        { .ctn.text = device_sn, .is_icon = 0, .enable_datetime = false},   // [2] SN
        { .ctn.text = NULL, .is_icon = 0, .enable_datetime = true },        // [3] 日期时间
        {
            .ctn.icon = k_path_pagefooter_untie,                             // [4] WiFi图标
            .is_icon = 1, .enable_datetime = false
        }
    };
    int k_pagefooter_height = 28;

    init_page_footer(active_screen, page_footer_objs,bg_index);  // 创建底部栏
}

/**
 * @brief 对外接口：初始化并显示底部状态栏
 */
void init_show_page_footer_ui(void * active_screen,int bg_index)
{
    _show_self_ui(active_screen, bg_index);
}

/**
 * @brief 销毁底部状态栏（定时器 + 容器 + 子元素）
 */
void deinit_page_footer(int bg_index)
{
    if (s_pagefooter_objs[bg_index].datetime_timer) {
        lv_timer_del(s_pagefooter_objs[bg_index].datetime_timer);  // 删除定时器
        s_pagefooter_objs[bg_index].datetime_timer = NULL;
    }
    
    if (s_pagefooter_objs[bg_index].page_footer) {
        lv_obj_del(s_pagefooter_objs[bg_index].page_footer);  // 删除容器（自动删除子元素）
        s_pagefooter_objs[bg_index].page_footer = NULL;
    }
    
    s_pagefooter_objs[bg_index].datetime_label = NULL;
    s_pagefooter_objs[bg_index].has_init = false;
}

/**
 * @brief 从互联网 NTP 服务器同步时间
 * 
 * @return 0=成功, -1=失败
 */
int set_pagefooter_time_from_internet(char* server_ip, int port, int time_zone)
{
#ifdef CONFIG_XOS_INN_QUANTP
    RecvNtpInputParameter input;
    struct timeval ntp_tv;

    strcpy(input.server_ip_, server_ip);
    input.server_port_ = port;
    input.time_zone_ = time_zone;
    input.time_out_ = 10;  // 10s 超时
    if(recv_ntp_time(&input, &ntp_tv) < 0) {  // NTP 请求
        QM_ESL2_ELOG("%s %d read ntp time failed!\n", __func__, __LINE__);
        return -1;
    }
    
    if(set_ntp_time(&ntp_tv) < 0) {  // 设置系统时间
        QM_ESL2_ELOG("%s %d set ntp time failed!\n", __func__, __LINE__);
        return -1;
    }

    QM_ESL2_LOG("%s %d success\n", __func__, __LINE__);
#endif
    return 0;
}