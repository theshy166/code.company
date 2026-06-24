/**
 * @file h5esl.c
 *
 * Flatter structure
 */

/*********************
 *      INCLUDES
 *********************/
#include "h5esl.h"

#if defined(CONFIG_XOS_APP_ESL2)

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "qua_los_loading.h"
#include "qua_font.h"
#include "../conf/conf.h"
#include "../model/esl_page_model.h"
#include "../model/product_data_model.h"
#include "../model/three_label_comp_model.h"
#include "../model/product_comp_model.h"
#include "utils.h"
#include "qm_lv_obj.h"
#include "uirender.h"
#include "../ctrl/esl_message_queue.h"
#include "../ctrl/esl_socket_service.h"
#ifdef CONFIG_XOS_FWK_APPMANAGER
#include "appcommon.h"
#include "appmanager.h"
#endif
#include "losplayer.h"
#include "qm_3_label.h"
#include "qm_product_comp.h"
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "player/qua_mm_player_common.h"
#include "qm_wifi_cfg.h"
#endif
#endif //CONFIG_XOS_FWK_PLAYER

/*********************
 *      DEFINES
 *********************/
#define SUPPORT_SCROLL 0 //will cause abnormal
#define kWidth LV_HOR_RES
#define kHeight LV_VER_RES
#define kRedPanelWidth kWidth * 2/3
#define kRedPanelHeight kHeight
#define kMargin 30
/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  STATIC PROTOTYPES
 **********************/
//callback function
static void app_event_handler(lv_event_t *event);
static void create_ui(void);


#ifdef CONFIG_XOS_FWK_APPMANAGER
static app_data_ptr los_h5esl_ui_init(void *start_data);
#endif
/**********************
 *  STATIC VARIABLES
 **********************/
static lvobj_h5esl *s_h5esl_objs = NULL;

static lv_obj_t * label_1 = NULL;
static lv_obj_t * label_2 = NULL;


shelf_data_t s_shelf_data[] = {
    {"李子", 20.0, 10.0, k_path_img_lizi, "一", "大连"},
    {"杨桃", 30.0, 20.0, k_path_img_carambola, "一", "浙江"}
    };
/**********************
 *      dependencies
 **********************/


/*=====================
 *  STATIC functions
 *====================*/

/**
 * @brief 销毁 ESL 应用 UI 资源
 * 
 * 清理 product1/product2 组件，释放 s_h5esl_objs 内存
 */
void los_h5_esl_quit(void) {
    QM_ESL2_LOG("los_h5_esl_quit.");
    if (s_h5esl_objs != NULL) {
        product_comp_destroy(s_h5esl_objs->product1);  // 销毁商品组件1
        product_comp_destroy(s_h5esl_objs->product2);  // 销毁商品组件2
        lv_mem_free(s_h5esl_objs);  // 释放结构体内存
        s_h5esl_objs = NULL;
    }
}


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/**
 * @brief ESL 应用入口（旧版兼容，内部调用 create_ui）
 */
void los_h5esl_entry(void)
{
    QM_ESL2_LOG("----xos_h5esl_entry-->in.");

    // 初始化 LVGL FreeType 字体引擎
    qua_los_ft_font_init();

    create_ui();
}

/**
 * @brief ESL 应用退出，反初始化 UI 渲染器
 */
void xos_h5esl_quit(void) {
    QM_ESL2_LOG("los_h5esl_quit.");
    uirender_quit();  // 释放渲染器资源
}

#ifdef CONFIG_XOS_FWK_APPMANAGER
/**
 * @brief app_manager 回调：创建 UI 时被框架调用
 */
static app_data_ptr los_h5esl_ui_init(void *start_data) {
    los_h5esl_entry();  // 调用 ESL 入口
    return NULL;
}

/**
 * @brief 注册 h5esl（旧版）到 app_manager 框架
 * 
 * 注册回调：on_create → los_h5esl_ui_init, on_destory → xos_h5esl_quit
 */
void h5esl_init(void) {
    qua_app_ops *ops = malloc(sizeof(qua_app_ops));  // 分配操作结构体
    if (ops) {
        memset(ops, 0x0, sizeof(qua_app_ops));
        ops->on_create = los_h5esl_ui_init;  // 启动回调
        ops->on_destory = xos_h5esl_quit;    // 退出回调
        app_manager_register("h5esl", ops);  // 注册到框架
        free(ops);
    }
}

#ifdef CONFIG_XOS_APP_ESL2
/**
 * @brief 注册 esl2（当前版本）到 app_manager 框架
 * 
 * 这是 esl2 应用的「注册入口」—— 由 AppsFactories() → AppsRegister() 调用。
 * 
 * 完整启动链路：
 *   main() → lv_async_call(__init_gui) → AppsFactories()
 *         → esl2_init() → app_manager_register("esl2", ops)
 *         → app_manager 在适当时机回调 ops->on_create 即 create_ui()
 * 
 * 1. 设置 on_create → create_ui（直接创建 UI，不需要中间层）
 * 2. 根据屏幕数量设置背光亮度：
 *    - 单屏：40%
 *    - 双屏：80%
 */
void esl2_init(void) {
    // ========== 1. 分配 app_manager 操作结构体 ==========
    qua_app_ops *ops = malloc(sizeof(qua_app_ops));  // 分配操作结构体
    if (ops) {
        extern void lvgl_test(void);                 // 测试函数声明（未在 init 中使用）
        memset(ops, 0x0, sizeof(qua_app_ops));       // 清零结构体
        ops->on_create = create_ui;                  // ★ 核心：注册 create_ui 为启动回调
        // 此后 app_manager 框架会在系统就绪后调用 create_ui()
        app_manager_register("esl2", ops);           // 以 "esl2" 为名注册到框架
        free(ops);                                   // 框架内部已拷贝，释放本地内存
    }

    // ========== 2. 设置屏幕背光亮度 ==========
   {
        #if defined(CONFIG_XOS_HW_QUADRV) && !defined(BUILD_SIMULATOR)
        extern int qm_backlight_setbri(unsigned int brightness);  // 设置背光
        extern int qm_backlight_getbri(void);                      // 读取背光
        extern int xos_lcm_get_screen_num(void);                   // 获取屏幕数量

        if (xos_lcm_get_screen_num() == 1) {
            qm_backlight_setbri(40);  // 单屏：40% 亮度
        }
        if (xos_lcm_get_screen_num() == 2) {
            qm_backlight_setbri(80);  // 双屏：80% 亮度
        }
        #endif
    }
}
#endif

#endif

/**
 * @brief UI 初始化第二阶段（LVGL 异步回调）
 * 
 * 此函数在 LVGL 主线程上下文中运行（由 lv_async_call 调度）。
 * 调用屏幕1和屏幕2的 UI 初始化函数：
 * - xos_esl_ui_init():  屏幕1（page_esl.c） — 读配置、建 Socket、拉模板、渲染
 * - xos_esl_ui_init_2(): 屏幕2（仅双屏配置）— 同上逻辑操作屏幕2
 * 
 * 为什么分两阶段？
 *   create_ui() 可以直接被 app_manager 回调，但此时系统可能未完全就绪。
 *   通过 _first_start_ui_function 延时 100ms 后再投递本函数，
 *   确保 WiFi、文件系统等底层服务已初始化完毕。
 * 
 * @param p 未使用（lv_async_call 要求 void* 签名）
 */
static void _do_create_ui_stage2(void *p)
{
    (void)p;  // 未使用参数，消除编译警告
    long current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    QM_ESL2_LOG("----_do_create_ui_stage2-->in.");

    // ★ 初始化屏幕1 — 这是主显示器的完整初始化入口
    extern void xos_esl_ui_init(void);
    xos_esl_ui_init();  // 到 page_esl.c 中：读配置 → 建 Socket → _init_ui → 拉模板

#ifdef CONFIG_XOS_LCM_2ND_SCR
    // ★ 双屏模式：初始化屏幕2（通常用于货架背面显示）
    extern void xos_esl_ui_init_2(void);
    xos_esl_ui_init_2();  // 到 page_esl.c 中：同上逻辑，操作屏幕2
#endif
    QM_ESL2_LOG("----_do_create_ui_stage2-->out.");
    current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
}

/**
 * @brief 后台线程入口：延时 100ms 后异步投递 UI 初始化第二阶段
 * 
 * 为什么需要这个线程？
 *   1. create_ui() 由 app_manager 的 on_create 回调触发，此时 WiFi 等底层服务
 *      可能尚未完成初始化
 *   2. 如果在当前线程等待，会阻塞 LVGL 主循环，导致屏幕无响应
 *   3. 开分离线程 sleep(100ms)，既不阻塞 LVGL，又给系统预留了初始化时间
 *   4. 100ms 后通过 lv_async_call() 安全地将 _do_create_ui_stage2 投递回 LVGL 主线程
 * 
 * 关键 API：
 *   lv_async_call(cb, user_data) — 将回调投递到 LVGL 主循环的下一次迭代中执行。
 *   这是非 LVGL 线程更新 UI 的唯一安全方式。
 * 
 * @param p 未使用
 * @return NULL
 */
static void* _first_start_ui_function(void *p)
{
    (void)p;
    long current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    QM_ESL2_LOG("----_first_start_ui_function-->in.");
    usleep(100*1000);  // ★ 等 100ms，确保 WiFi 和文件系统就绪
    lv_async_call(_do_create_ui_stage2, NULL);  // ★ 投递到 LVGL 主线程安全执行第二阶段
    QM_ESL2_LOG("----_first_start_ui_function-->out.");
    current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    return NULL;
}

/**
 * @brief ESL 应用 UI 初始化入口（★ 第一个被调用的业务函数）
 * 
 * 此函数由 app_manager 框架通过 esl2_init() 中注册的 on_create 回调触发。
 * 它是在 LVGL 主线程上下文中执行的。
 * 
 * ┌──────────────────────── 完整调用链 ────────────────────────┐
 * │                                                            │
 * │  main() [qm10xd/main.c]                                    │
 * │    → lv_init() + hal_init_v9()                             │
 * │    → lv_async_call(__init_gui) → AppsFactories()           │
 * │        → AppsRegister() [appsmain.c]                       │
 * │            → esl2_init() [★ 注册 create_ui 到 app_manager] │
 * │                → app_manager 回调 create_ui() [★ 本函数]   │
 * │                                                            │
 * │  create_ui():                                              │
 * │    1. uirender_init() — 初始化渲染器（top_layer 等）       │
 * │    2. msg_queue_init() — 初始化消息队列（OTA 缓冲）        │
 * │    3. init_group_map() — 初始化设备分组映射表              │
 * │    4. WiFi 连接重启                                         │
 * │    5. 屏幕1/2 显示 "正在加载中..."                          │
 * │    6. 创建后台线程 → 100ms 后 lv_async_call                │
 * │       → _do_create_ui_stage2()                              │
 * │           → xos_esl_ui_init() [page_esl.c — 真正拉模板]    │
 * │           → xos_esl_ui_init_2() [双屏]                      │
 * └────────────────────────────────────────────────────────────┘
 * 
 * 功能说明：
 * 1. 初始化 UI 渲染器 (uirender_init) — 创建 top_layer 等基础设施
 * 2. 初始化消息队列和分组映射 — 为 MQTT/Socket 通信做准备
 * 3. 启动 WiFi 连接 — 仅在真机模式（非模拟器）
 * 4. 在两个屏幕的 top_layer 上显示 "正在加载中..." — 给用户视觉反馈
 * 5. 创建后台线程处理首次 UI 加载 — 延迟 100ms 等系统就绪
 */
static void create_ui(void)
{
    long current_time = get_milliseconds();

    // ========== 1. 初始化阶段 ==========
    // 所有这些 init 都必须在 LVGL 主线程执行
    uirender_init();                           // ★ 初始化 UI 渲染器：创建顶层图层、状态栏等
    msg_queue_init(MAX_MESSAGE_CONSUMER_COUNT); // 初始化消息队列：用于 OTA 消息的批量缓冲
    init_group_map();                           // 初始化设备分组哈希表（主设备管理从设备）
    init_push_id_map();                         // 初始化推送消息 ID 去重映射表

    // 仅在真机模式（非模拟器）下启动 WiFi
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if (access("/data/.skip_wifi_restart", F_OK) == 0) {
            QM_ESL2_LOG("skip wifi restart for adb debug");
        } else {
            qm_connection_wifi_restart();      // 重启 WiFi 连接 —— 触发 wpa_supplicant
        }
    #endif

    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);

    // ========== 2. 屏幕1 初始化 - 显示 "正在加载中" ==========
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp1 = lv_display_get_1st_scr();  // 获取第一个物理屏幕的 display 对象
    if(_disp1 != NULL){
        // 设置屏幕1默认背景为白色（覆盖 LVGL 默认的灰色）
        lv_obj_set_style_bg_color(_disp1->act_scr, lv_color_white(), LV_PART_MAIN);
        QM_ESL2_LOG("_disp->act_scr111111111111=%p\n",_disp1->act_scr);

        // 在屏幕1的 top_layer 创建加载提示标签
        // top_layer 是 LVGL 的「系统覆盖层」，永远在最顶层，不受 act_scr 清理影响
        if (_disp1->top_layer != NULL) {
            label_1 = lv_label_create(_disp1->top_layer); // 在 top_layer 上创建标签
            lv_font_t *font = NULL;
            // 使用 80px 字号从 TTF 文件动态加载 FreeType 字体
            qua_xos_ft_init_with_size_path(&font, 80, k_path_ttf, 0);
            lv_obj_set_style_text_font(label_1, font, 0);
            lv_label_set_text(label_1, "正在加载中...");  // 加载中提示文字
            lv_obj_align(label_1, LV_ALIGN_CENTER, 0, 0);  // 居中显示
        }
    }

    // ========== 3. 屏幕2 初始化 - 显示 "正在加载中" ==========
    // 逻辑与屏幕1完全相同，操作对象为屏幕2的 display
    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp2 = lv_display_get_2nd_scr();  // 获取第二个物理屏幕的 display 对象
    if(_disp2 != NULL){
        // 设置屏幕2默认背景为白色
        lv_obj_set_style_bg_color(_disp2->act_scr, lv_color_white(), LV_PART_MAIN);
        QM_ESL2_LOG("_disp->act_scr2222222222222222=%p\n",_disp2->act_scr);

        // 在屏幕2的 top_layer 创建加载提示标签
        if (_disp2->top_layer != NULL) {
            label_2 = lv_label_create(_disp2->top_layer);
            lv_font_t *font = NULL;
            qua_xos_ft_init_with_size_path(&font, 80, k_path_ttf, 0);
            lv_obj_set_style_text_font(label_2, font, 0);
            lv_label_set_text(label_2, "正在加载中...");  // 加载中提示文字
            lv_obj_align(label_2, LV_ALIGN_CENTER, 0, 0);  // 居中显示
        }
    }

    current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);

    // ========== 4. 创建后台线程处理网络连接和 UI 加载 ==========
    // ★ 关键设计：不在当前线程阻塞等待系统就绪，而是开一个分离线程
    // → 该线程 sleep(100ms) 等待 WiFi 和文件系统初始化
    // → 然后通过 lv_async_call 安全投递 _do_create_ui_stage2 到 LVGL 主线程
    // 为什么是 100ms？经验值，足够 wpa_supplicant 完成首次扫描
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, _first_start_ui_function, NULL);
    pthread_detach(thread_id); // 分离线程：终止时自动释放资源，无需 pthread_join
}

/**
 * @brief LVGL 事件回调：对象被删除时清理资源
 */
static void app_event_handler(lv_event_t *event)
{
    if (event->code == LV_EVENT_DELETE) {  // 检测删除事件
        los_h5_esl_quit();  // 清理 UI 资源
    }
}

/**
 * @brief 清除加载中提示标签
 * 
 * @param index 屏幕索引 (1=屏幕1, 2=屏幕2)
 */
void clear_label(int index){
    if(index == 1){
        if(lv_obj_is_valid(label_1) && label_1 != NULL){  // 检查有效性
            lv_obj_delete(label_1);  // 删除标签对象
            label_1 = NULL;
        }
    }else if (index == 2){
        if(lv_obj_is_valid(label_2) && label_2 != NULL){
            lv_obj_delete(label_2);
            label_2 = NULL;
        }
    }
}


#endif  /* CONFIG_XOS_APP_ESL2 */
