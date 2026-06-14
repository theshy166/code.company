/**
 * @file h5esl.c
 *
 * Flatter structure
 */

/*********************
 *      INCLUDES
 *********************/
#include "h5esl.h"

#if XOS_USE_APP_H5_ESL != 0

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "web_page_wrapper.h"
#include "qua_los_loading.h"
#include "qua_font.h"
#include "h5esl_conf.h"
#ifdef CONFIG_XOS_FWK_APPMANAGER
#include "appcommon.h"
#include "appmanager.h"
#endif

/*********************
 *      DEFINES
 *********************/
#define FEATURE_HEARER 0
#define SUPPORT_SCROLL 0 //will cause abnormal

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lvobj_h5esl {
    lv_obj_t *browser_wnd;
    lv_obj_t *loading_ui;
    lv_obj_t *canvas;
}lvobj_h5esl;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void* h5esl_thread_entry(void *para);
//callback function
static void set_caption(const char *caption);
static void app_event_handler(lv_event_t *event);
static void open_url_app(const char *url);
static void create_list(lv_obj_t *bg_parent);

#ifdef CONFIG_XOS_FWK_APPMANAGER
static app_data_ptr los_h5esl_ui_init(void *start_data);
#endif
/**********************
 *  STATIC VARIABLES
 **********************/
lvobj_h5esl *s_h5esl_objs = NULL;
lv_obj_t *g_html_canvas = NULL;
lv_obj_t *g_video_parent = NULL;

static html_widget *html_widget_ptr;


/**********************
 *      dependencies
 **********************/
extern lv_font_t* ft_font_ttf_30;

/*=====================
 *  STATIC functions
 *====================*/
static void list_event_handler(lv_event_t *event)
{
    if (event->code == LV_EVENT_VALUE_CHANGED) {
        uint16_t row = -1;
        uint16_t col = -1;
        lv_obj_t *list = event->target;
        lv_table_get_selected_cell(list, &row, &col);

        if (row > 10000 || col > 10000) {
            QM_ESL2_LOG("list row or col > 10000");
            return;
        }
        char *value = lv_table_get_cell_value(list, row, col);
        if (value == NULL) {
            QM_ESL2_LOG("list value is NULL\n");
            return;
        }

        open_url_app(value);
    }
}

static void* h5esl_thread_entry(void *para)
{
    //init
    html_widget_ptr = (html_widget *)malloc(sizeof(struct html_widget));
    if (html_widget_ptr == NULL) {
        QM_ESL2_LOG("h5esl_thread_entry exit. OOM.\n");
        return NULL;
    }
    memset(html_widget_ptr, 0, sizeof(struct html_widget));
    html_widget_ptr->set_caption = set_caption;

    //create web
    web_page* web_page_obj = web_page_new(html_widget_ptr, 1);
    //store web obj
    html_widget_ptr->page = web_page_obj;
    QM_ESL2_LOG("h5esl_thread_entry, open url =%s.\n", (char *)para);

    web_page_open(web_page_obj, para, "");

    //
    web_page_runloop(web_page_obj, true);

    //delete web_page
    if (html_widget_ptr->page != NULL) {
        web_page_delete(html_widget_ptr->page);
        html_widget_ptr->page = NULL;
    }

    free(html_widget_ptr);

    if (s_h5esl_objs != NULL) {
        lv_mem_free(s_h5esl_objs);
        s_h5esl_objs = NULL;
    }

    QM_ESL2_LOG("[h5-thread] exit.\n");

    return NULL;
}

static void set_caption(const char *caption)
{
    QM_ESL2_LOG("set_caption:%s\n", caption);
    if (caption != NULL) {
        // lv_label_set_text(s_h5esl_objs->header_title, caption);
    }
    if (s_h5esl_objs->loading_ui != NULL) {
        qua_los_hide_loading(s_h5esl_objs->loading_ui);
        s_h5esl_objs->loading_ui;
    }
}

void los_h5_esl_quit(void) {
    QM_ESL2_LOG("los_h5_esl_quit.");
    if (html_widget_ptr != NULL && html_widget_ptr->page != NULL) {
        web_page_runloop(html_widget_ptr->page, false);
    }
}


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void los_h5esl_entry(void)
{
    QM_ESL2_LOG("los_h5esl_entry-->in.");

    //create browser ui
    lv_obj_t *parent = lv_scr_act();
    lv_obj_t *bg_wnd = lv_obj_create(parent);
    lv_obj_set_size(bg_wnd, LV_PCT(100), LV_PCT(100));
    //
    //init font
    qua_los_ft_font_init();

    create_list(bg_wnd);

    //unique project, add float btn.
#if !defined(CONFIG_XOS_USE_APP_LAUNCHER) ||CONFIG_XOS_USE_APP_LAUNCHER == 0
#if XOS_USE_FLOAT_WND != 0
    extern void init_float_window(uint8_t);
    init_float_window(2);
#endif
#endif
}

#ifdef CONFIG_XOS_FWK_APPMANAGER
static app_data_ptr los_h5esl_ui_init(void *start_data) {
    los_h5esl_entry();
    return NULL;
}

/* void h5esl_init(void) {
    qua_app_ops *ops = malloc(sizeof(qua_app_ops));
    if (ops) {
        memset(ops, 0x0, sizeof(qua_app_ops));
        ops->on_create = los_h5esl_ui_init;
        app_manager_register("h5esl", ops);
        free(ops);
    }
} */
#endif
static void create_list(lv_obj_t *bg_parent)
{
    const char *demo_app_array[] = {
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
            SERVER_URL"landscape_96",
            SERVER_URL"landscape_96_2",
            SERVER_URL"landscape_96_3",
            SERVER_URL"landscape_96_4",
            SERVER_URL"landscape_96_5",
            SERVER_URL"landscape_96_6",
            SERVER_URL"landscape_96_7",
            SERVER_URL"landscape_96_8",
            SERVER_URL"landscape_96_9",
#else
            H5ESL_URL_BASE,
            H5ESL_URL_BASE_WHITE,
            H5ESL_URL_PIC_TEXT,
            H5ESL_URL_TB_BOILER,
            H5ESL_URL_BASE_2,
            H5ESL_URL_BASE_3,
            H5ESL_URL_NO_USE
#endif
    };
    const uint8_t table_size = sizeof(demo_app_array)/sizeof(char*);
    QM_ESL2_LOG("[Lottie_list].table_size=%d", table_size);
    //create a ta// 目录内容展示列表

    lv_obj_t *demolist = lv_table_create(bg_parent);
    lv_obj_set_size(demolist, LV_PCT(100), LV_PCT(100));
    lv_obj_align(demolist, LV_ALIGN_CENTER, 0, 0);
    lv_table_set_col_width(demolist, 0, LV_PCT(100));
    lv_table_set_col_cnt(demolist, 1);
    lv_table_set_row_cnt(demolist, table_size);
    //
    lv_obj_add_event_cb(demolist, list_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
        // only scroll up and down
    lv_obj_set_scroll_dir(demolist, LV_DIR_TOP | LV_DIR_BOTTOM);

    for (int i = 0; i < table_size; i++) {
        lv_table_set_cell_value(demolist, i, 0, demo_app_array[i]);
    }
    //
    static lv_style_t cell_style;
    lv_style_init(&cell_style);
    lv_style_set_text_font(&cell_style, ft_font_ttf_30); // 设置字体大小为28
    //
    lv_obj_add_style(demolist, &cell_style, LV_PART_ITEMS); // 设置第一行第一列的样式
}

static void open_url_app(const char *url)
{
    s_h5esl_objs = lv_mem_alloc(sizeof(lvobj_h5esl));
    lv_memset_00(s_h5esl_objs, sizeof(lvobj_h5esl));

    //create browser ui
    lv_obj_t *parent = lv_scr_act();
    //0.browser_wnd
    lv_obj_t *browser_wnd = lv_obj_create(parent);
    g_video_parent = browser_wnd;
    #if SUPPORT_SCROLL
    lv_obj_set_scroll_dir(browser_wnd, LV_DIR_TOP | LV_DIR_BOTTOM);
    #else
    lv_obj_clear_flag(browser_wnd, LV_OBJ_FLAG_SCROLLABLE);
    #endif
    lv_obj_set_size(browser_wnd, LV_HOR_RES, LV_VER_RES);       // 设置到屏幕大小
    // lv_obj_set_style_bg_color(browser_wnd, lv_color_hex(0x0000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(browser_wnd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	// lv_obj_set_style_border_width(browser_wnd, 0, LV_STATE_DEFAULT);
    lv_obj_center(browser_wnd);

    //2.canvas
    lv_obj_t *canvas = lv_canvas_create(browser_wnd);
    g_html_canvas = canvas;
    // lv_obj_add_flag(canvas, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(canvas, LV_HOR_RES, LV_VER_RES);       // for show html canvas
    //test:
    // lv_obj_set_style_bg_color(canvas, lv_color_hex(0xffff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(canvas, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(canvas, lv_color_hex(0xff00ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(canvas, 2, LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(canvas, LV_OPA_100, LV_STATE_DEFAULT);

    lv_obj_set_align(canvas, LV_ALIGN_CENTER);//draw normal, but can't see top part

    //canvas buf
    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(LV_USE_HOR_SIZE, LV_USE_VER_SIZE)];//设置buf存放画板数据
    lv_canvas_set_buffer(canvas, cbuf, LV_USE_HOR_SIZE, LV_USE_VER_SIZE, LV_IMG_CF_TRUE_COLOR);

    //
    s_h5esl_objs->browser_wnd = browser_wnd;
    s_h5esl_objs->canvas = canvas;

    //monitor window deleted
    lv_obj_add_event_cb(browser_wnd, app_event_handler, LV_EVENT_DELETE, NULL);
    //show loading
    s_h5esl_objs->loading_ui = qua_los_show_loading(NULL, 80, 80);

    //create thread for load h5
    pthread_t lvgl_tid = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&lvgl_tid, &attr, h5esl_thread_entry, (char *)url);
}

static void app_event_handler(lv_event_t *event)
{
    if (event->code == LV_EVENT_DELETE) {
        los_h5_esl_quit();
    }
}


#endif  /*XOS_USE_APP_H5_ESL*/
