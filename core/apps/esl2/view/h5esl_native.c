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

void los_h5_esl_quit(void) {
    QM_ESL2_LOG("los_h5_esl_quit.");
    if (s_h5esl_objs != NULL) {
        product_comp_destroy(s_h5esl_objs->product1);
        product_comp_destroy(s_h5esl_objs->product2);
        lv_mem_free(s_h5esl_objs);
        s_h5esl_objs = NULL;
    }
}


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void los_h5esl_entry(void)
{
    QM_ESL2_LOG("----xos_h5esl_entry-->in.");

    //
    //init font
    qua_los_ft_font_init();

    // create_list(bg_wnd);
    create_ui();
}

void xos_h5esl_quit(void) {
    QM_ESL2_LOG("los_h5esl_quit.");
    uirender_quit();
}

#ifdef CONFIG_XOS_FWK_APPMANAGER
static app_data_ptr los_h5esl_ui_init(void *start_data) {
    los_h5esl_entry();
    return NULL;
}

void h5esl_init(void) {
    qua_app_ops *ops = malloc(sizeof(qua_app_ops));
    if (ops) {
        memset(ops, 0x0, sizeof(qua_app_ops));
        ops->on_create = los_h5esl_ui_init;
        ops->on_destory = xos_h5esl_quit;
        app_manager_register("h5esl", ops);
        free(ops);
    }
}

#ifdef CONFIG_XOS_APP_ESL2
void esl2_init(void) {
    qua_app_ops *ops = malloc(sizeof(qua_app_ops));
    if (ops) {
        extern void lvgl_test(void);
        memset(ops, 0x0, sizeof(qua_app_ops));
        //ops->on_create = lvgl_test;
        ops->on_create = create_ui;
        app_manager_register("esl2", ops);
        free(ops);
    }
    /*
     Set display brightness
     Set the brightness of each single lcm screen to 40%,
     and set the brightness to 80% when there are two lcm.
    */
   {
        #if defined(CONFIG_XOS_HW_QUADRV) && !defined(BUILD_SIMULATOR)
        extern int qm_backlight_setbri(unsigned int brightness);
        extern int qm_backlight_getbri(void);
        extern int xos_lcm_get_screen_num(void);

        if (xos_lcm_get_screen_num() == 1) {
            qm_backlight_setbri(40);
        }
        if (xos_lcm_get_screen_num() == 2) {
            qm_backlight_setbri(80);
        }
        #endif
    }
}
#endif

#endif

static void _do_create_ui_stage2(void *p)
{
    long current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    QM_ESL2_LOG("----_do_create_ui_stage2-->in.");
    extern void xos_esl_ui_init(void);
    xos_esl_ui_init();
#ifdef CONFIG_XOS_LCM_2ND_SCR
    extern void xos_esl_ui_init_2(void);
    xos_esl_ui_init_2();
#endif
    QM_ESL2_LOG("----_do_create_ui_stage2-->out.");
    current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
}

static void* _first_start_ui_function(void *p)
{
    long current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    QM_ESL2_LOG("----_first_start_ui_function-->in.");
    usleep(100*1000);
    lv_async_call(_do_create_ui_stage2, NULL);
    QM_ESL2_LOG("----_first_start_ui_function-->out.");
    current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    return NULL;
}

static void create_ui(void)
{
    long current_time = get_milliseconds();
    uirender_init();
    msg_queue_init(MAX_MESSAGE_CONSUMER_COUNT);
    init_group_map();
    init_push_id_map();
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        qm_connection_wifi_restart();
    #endif
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    //1.screen 1 and screen set bg to white
    extern lv_display_t * lv_display_get_1st_scr(void);
    lv_display_t * _disp1 = lv_display_get_1st_scr();
    if(_disp1 != NULL){
        lv_obj_set_style_bg_color(_disp1->act_scr, lv_color_white(), LV_PART_MAIN);
        //lv_obj_set_style_bg_color(_disp1->act_scr,  lv_color_hex(0xff0000), LV_PART_MAIN);
        //lv_obj_set_style_bg_opa(_disp1->act_scr, LV_OPA_0, 0);
        QM_ESL2_LOG("_disp->act_scr111111111111=%p\n",_disp1->act_scr);
        if (_disp1->top_layer != NULL) {
            label_1 = lv_label_create(_disp1->top_layer);
            lv_font_t *font = NULL;
            qua_xos_ft_init_with_size_path(&font, 80, k_path_ttf, 0);
            lv_obj_set_style_text_font(label_1, font, 0);
            lv_label_set_text(label_1, "正在加载中...");
            lv_obj_align(label_1, LV_ALIGN_CENTER, 0, 0);
        }
    }

    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp2 = lv_display_get_2nd_scr();
    if(_disp2 != NULL){
        lv_obj_set_style_bg_color(_disp2->act_scr, lv_color_white(), LV_PART_MAIN);
        //lv_obj_set_style_bg_color(_disp2->act_scr, lv_color_hex(0xff0000), LV_PART_MAIN);
        //lv_obj_set_style_bg_opa(_disp2->act_scr, LV_OPA_0, 0);
        QM_ESL2_LOG("_disp->act_scr2222222222222222=%p\n",_disp2->act_scr);
        if (_disp2->top_layer != NULL) {
            label_2 = lv_label_create(_disp2->top_layer);
            lv_font_t *font = NULL;
            qua_xos_ft_init_with_size_path(&font, 80, k_path_ttf, 0);
            lv_obj_set_style_text_font(label_2, font, 0);
            lv_label_set_text(label_2, "正在加载中...");
            lv_obj_align(label_2, LV_ALIGN_CENTER, 0, 0);
        }
    }
    current_time = get_milliseconds();
    QM_ESL2_LOG("============= func:%s ,line:%d ==================当前时间 current_time:%ld\n",__func__, __LINE__,current_time);
    //创建线程
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, _first_start_ui_function, NULL);
    pthread_detach(thread_id); // 分离线程，使其在终止时自动释放资源
}

static void app_event_handler(lv_event_t *event)
{
    if (event->code == LV_EVENT_DELETE) {
        los_h5_esl_quit();
    }
}


void clear_label(int index){
    if(index == 1){
        if(lv_obj_is_valid(label_1) && label_1 != NULL){
            lv_obj_delete(label_1);
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
