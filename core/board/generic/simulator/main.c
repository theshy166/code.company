//#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
//#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "qua_los_2d.h"

#include "qua_font.h"
#include "xos_app_conf.h"
#if XOS_USE_APP_MANAGER != 0
#include "appmanager.h"
#include "appmessage.h"
#endif

#ifdef CONFIG_XOS_FWK_PARAM
#include "param.h"
#define QUA_PARAM_FILE_PATH		"./out/simulator/xos/res/config/param.ini"
#endif

#define DISP_BUF_SIZE (LV_USE_VER_SIZE/2 * LV_USE_HOR_SIZE)

lv_group_t * g_indev_group;
lv_obj_t * bgImageObj = NULL;

pthread_t xos_thread_boot_handle = -1;

#if XOS_USE_APP_MANAGER != 0
pthread_t xos_thread_appmgr_handle = -1;
void* xos_thread_appmanager_entity(void * args);
#endif

void *xos_thread_boot_entity(void * args);
extern void xos_init_phase_1(void);
uint32_t custom_tick_get(void);


static void _init_disp_inputs(lv_display_t *disp)
{
    lv_group_set_default(lv_group_create());

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
    lv_obj_t * cursor_obj;
    cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t * kb = lv_sdl_keyboard_create();
    lv_indev_set_display(kb, disp);
    lv_indev_set_group(kb, lv_group_get_default());
}

static void hal_init_v9(void)
{
    qm_effect_inits(LV_USE_HOR_SIZE, LV_USE_VER_SIZE, 0);

    lv_display_t * disp1 = lv_sdl_window_create(LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_display_set_1st_scr(disp1);
    _init_disp_inputs(disp1);

#ifdef CONFIG_XOS_LCM_2ND_SCR
    lv_display_t * disp2 = lv_sdl_window_create(LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    _init_disp_inputs(disp2);
    lv_display_set_2nd_scr(disp2);
#endif
    return;
}

int main(void)
{
    /* init boot phase1*/
    xos_init_phase_1();

	#ifdef CONFIG_XOS_FWK_PARAM
	/* system param.ini */
	param_init(QUA_PARAM_FILE_PATH);
    printf("product_name:%s, vendor:%s, b_num=%d\n",
	param_get_string("sys.devinfo:product_name", NULL),
	param_get_string("sys.devinfo:vendor", NULL),
	param_get_int("sys.devinfo:b_num", 0)
	);
	#endif

    /*LittlevGL init*/
    lv_init();
    
    #if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9
    hal_init_v9();
    #else
    hal_init_v8();
    #endif

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);

#if XOS_USE_APP_MANAGER != 0
    app_manager_init();
    extern void AppsRegister(void);
    AppsRegister();
    
    pthread_create(&xos_thread_appmgr_handle, 0, xos_thread_appmanager_entity, NULL);
#endif
    pthread_create(&xos_thread_boot_handle, 0, xos_thread_boot_entity, NULL);

    while(1) {
        uint32_t next_ttl = lv_timer_handler();
        if (next_ttl < 1) next_ttl = 1;
        usleep(next_ttl*1000);
    }
    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

static void xos_gui(void* p)
{
    (void*)p;

#ifdef CONFIG_XOS_FWK_TEST_LCMD
    extern int qua_sys_lcmd_test(void);
    return qua_sys_lcmd_test();
#else
    extern void AppsFactories(void);
    AppsFactories();
#endif

   //extern void page_init(void);
   //page_init();
}

#if XOS_USE_APP_BOOT_ANIMATE != 0
static void xos_boot_gui(void *p)
{
    (void*)p;
    extern void bootanimate_entry(void);
    bootanimate_entry();
}
#endif

void* xos_thread_boot_entity(void * args)
{
#if XOS_USE_APP_BOOT_ANIMATE != 0
    extern bool is_bootanimate_done(void);

    lv_async_call(xos_boot_gui, NULL);

    /* Waiting for until the boot animation is done. */
    do{
        if(is_bootanimate_done())
            break;
        sleep(1);
    }while(1);
#endif /*XOS_USE_APP_BOOT_ANIMATE*/

    lv_async_call(xos_gui, NULL);

    return NULL;
}

#if XOS_USE_APP_MANAGER != 0
void* xos_thread_appmanager_entity(void * args) {
    
    extern void xos_qua_boot_init(void);
    xos_qua_boot_init();
    
    return NULL;
}
#endif

#if !(XOS_USE_APP_MANAGER != 0)
__attribute__((weak))  void app_manager_run(void) { ;/* dummy function link image */ }

__attribute__((weak)) int am_message_queue_init(int buffer_len){ return 0;/* dummy function link image */ }
#endif

__attribute__((weak)) int get_wifi_started(void) {return 0;}