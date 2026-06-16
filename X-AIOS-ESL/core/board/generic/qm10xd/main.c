/*
** Test main entities.
*/

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>

#include "lvgl/lvgl.h"
#include "fbdev.h"
#include "lvgl/src/drivers/evdev/lv_evdev.h"
#include "xos_app_conf.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "filter/qua_mm_filter.h"
#include "utils/qua_display_parser.h"
#include "bootanimation/qua_bootanimation.h"
#include "quagl/qua_gl.h"

#if XOS_USE_APP_MANAGER != 0
#include "appmanager.h"
#include "appmessage.h"
#endif

#ifdef CONFIG_XOS_FWK_PARAM
#include "param.h"
#define QUA_PARAM_FILE_PATH		"/data/config/param.ini"
#endif

#ifndef CONFIG_XOS_USE_FB_DIRECT
#define DISP_BUF_SIZE  (LV_LOGIC_HOR_SIZE * LV_LOGIC_VER_SIZE / 2)
#else
#define DISP_BUF_SIZE  (LV_LOGIC_HOR_SIZE * LV_LOGIC_VER_SIZE * 4)
#endif

#if LV_USE_PROFILER
FILE *g_trace_file = NULL;
#endif

qua_mm_system_ops_t *g_sys_ops = NULL;
int g_screen_rotation = 0;
unsigned long g_lvgl_phy = 0;
void * g_lvgl_virt = NULL;
lv_group_t * g_indev_group = NULL;
static bool g_stop = false;

#if XOS_USE_APP_MANAGER != 0
pthread_t xos_thread_appmgr_handle = -1;
void* xos_thread_appmanager_entity(void * args);
#endif

// declare static function
static void __init_gui(void *user_data);
static uint32_t __tick_get(void);

extern int xos_lcm_1st_is_existed(void);
extern int xos_lcm_2nd_is_existed(void);
extern int xos_lcm_chk_screens(void);

// Start Generation Here
// 全局变量，用于判断屏幕是否启动初始化
static int qm_screen_global_enabled = 2;
// 通过读取 /proc/cmdline 中的 "screen=" 参数来设置全局变量qm_screen_global_enabled
void init_screen_setting(void)
{
    FILE *fp = fopen("/proc/cmdline", "r");
    if (fp == NULL) {
        LV_LOG_USER("无法打开 /proc/cmdline\n");
        return;
    }

    char buffer[1024] = {0};
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // 查找 "screen=" 参数
        char *p = strstr(buffer, "screen=");
        if (p != NULL) {
            p += strlen("screen=");
            qm_screen_global_enabled = atoi(p);
        }
    }
    LV_LOG_USER("qm_screen_global_enabled = %d\n", qm_screen_global_enabled);
    fclose(fp);
}

#ifndef CONFIG_XOS_USE_DLMPI
static QUA_S32 init_common_sys()
{
    QUA_S32 ret = QUA_SUCCESS;
    char platform[16];
    qua_mm_system_t *system = NULL;

    qua_init_display_parser();
    qua_make_platform("qm10xd", qua_sys_os(), platform, sizeof(platform));

#ifdef CONFIG_XOS_PLAY_BOOTANIMATION
    ret = qua_init_bootanimation_client();
    if (ret != QUA_SUCCESS) {
        LV_LOG_USER("Error: init bootanimation failed 0x%x\n", ret);
        return ret;
    }

    ret = qua_mm_init(QUA_FALSE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        LV_LOG_USER("Error: qua mm init failed 0x%x\n", ret);
        return ret;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)system;
    g_sys_ops = sys_ops;
#else

    ret = qua_mm_init(QUA_TRUE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        LV_LOG_USER("Error: qua mm init failed 0x%x\n", ret);
        return ret;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)system;
    g_sys_ops = sys_ops;

    qua_vb_config_t vb_cfg;
    memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
    vb_cfg.max_pool_cnt = 128;
    vb_cfg.common_pools[0].block_size = 32768;
    vb_cfg.common_pools[0].block_cnt = 2;
#ifdef CONFIG_XOS_FB_USE_ARGB1555
    vb_cfg.common_pools[1].block_size = DISP_BUF_SIZE;
    vb_cfg.common_pools[1].block_cnt = 1;
#endif
#ifdef CONFIG_XOS_USE_HW_JPEG
    vb_cfg.common_pools[2].block_size = LV_USE_HOR_SIZE * LV_USE_VER_SIZE * 4;
    vb_cfg.common_pools[2].block_cnt = 1;
#endif
    ret = sys_ops->sys_init(&vb_cfg);
    if (ret != QUA_SUCCESS) {
        LV_LOG_USER("Error: sys init failed 0x%x\n", ret);
        return ret;
    }
#endif

#ifdef CONFIG_XOS_FB_USE_ARGB1555
    QUA_U32 blk_handle;

    blk_handle = sys_ops->vb_get_block(QUA_VB_INVALID_POOLID, DISP_BUF_SIZE, NULL);
    if (blk_handle == QUA_VB_INVALID_HANDLE) {
        LV_LOG_USER("Error: get lvgl vb failed");
        return QUA_FAILURE;
    }

    g_lvgl_phy = sys_ops->vb_handle_to_phyaddr(blk_handle);
    if (g_lvgl_phy == 0) {
        LV_LOG_USER("Error: get lvgl phy addr failed");
        return QUA_FAILURE;
    }

    g_lvgl_virt = sys_ops->sys_mmap(g_lvgl_phy, DISP_BUF_SIZE);
    if (g_lvgl_virt == NULL) {
        LV_LOG_USER("Error: get lvgl virt addr failed");
        return QUA_FAILURE;
    }
#endif

    return QUA_SUCCESS;
}
#endif

static void hal_init_v9(void)
{
    LV_LOG_USER("%s e\n", __func__);

#ifdef CONFIG_XOS_RES_800x1280
if (qm_screen_global_enabled > 0 && xos_lcm_1st_is_existed()){
    lv_group_set_default(lv_group_create());

    //static char buf1_1[DISP_BUF_SIZE];
    //memset(buf1_1, 0, DISP_BUF_SIZE);
    uint8_t *buf1_1 = fbdev_req_mem(DISP_BUF_SIZE);
    LV_LOG_USER("fbdev1 FB pointer: %p\n", buf1_1);

    lv_display_t *disp;

    disp = lv_display_create(LV_LOGIC_HOR_SIZE, LV_LOGIC_VER_SIZE);
    lv_display_set_physical_resolution(disp, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    LV_LOG_USER("create display w=%d h=%d rotation=%d\n", LV_LOGIC_HOR_SIZE, LV_LOGIC_VER_SIZE, g_screen_rotation);
    if (g_screen_rotation != 0)
        lv_display_set_rotation(disp, g_screen_rotation);

    lv_display_set_screen_index(disp, 0);
#ifdef CONFIG_XOS_USE_FB_DIRECT
#ifdef CONFIG_XOS_FB_USE_ARGB1555
    lv_display_set_buffers(disp, buf1_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
#else
    lv_display_set_buffers(disp, buf1_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_DIRECT);
#endif
#else
    lv_display_set_buffers(disp, buf1_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    lv_display_set_flush_cb(disp, fbdev_flush);
    lv_display_set_1st_scr(disp);
} /*if (qm_screen_global_enabled > 0)*/

if (qm_screen_global_enabled > 1 && xos_lcm_2nd_is_existed()){
#ifdef CONFIG_XOS_LCM_2ND_SCR
    //static char buf2_1[DISP_BUF_SIZE];
    //memset(buf2_1, 0, DISP_BUF_SIZE);
    uint8_t *buf2_1 = fbdev2_req_mem(DISP_BUF_SIZE);
    LV_LOG_USER("fbdev2 FB pointer: %p\n", buf2_1);
    lv_display_t *disp2 = lv_display_create(LV_LOGIC_HOR_SIZE, LV_LOGIC_VER_SIZE);
    lv_display_set_physical_resolution(disp2, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_display_set_screen_index(disp2, 1);
#ifdef CONFIG_XOS_USE_FB_DIRECT
#ifdef CONFIG_XOS_FB_USE_ARGB1555
    lv_display_set_buffers(disp2, buf2_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
#else
    lv_display_set_buffers(disp2, buf2_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_DIRECT);
#endif
#else
    lv_display_set_buffers(disp2, buf2_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    lv_display_set_flush_cb(disp2, fbdev2_flush);
    lv_display_set_2nd_scr(disp2);
#endif
}/*if (qm_screen_global_enabled > 1)*/

#else

    lv_group_set_default(lv_group_create());
    /*A small buffer for LVGL to draw the screen's content*/
    //static char buf1_1[DISP_BUF_SIZE];
    // static lv_color_t buf1_2[DISP_BUF_SIZE];
    /*Initialize a descriptor for the buffer*/
    //memset(buf1_1, 0, DISP_BUF_SIZE);
    // memset(buf1_2, 0, DISP_BUF_SIZE);
    uint8_t *buf1_1 = fbdev_req_mem(DISP_BUF_SIZE);
    if (buf1_1 == NULL) {
        LV_LOG_USER("main.c [fbdev_req_mem]  failed\n");
    }

    lv_display_t *disp;

    disp = lv_display_create(LV_LOGIC_HOR_SIZE, LV_LOGIC_VER_SIZE);
    lv_display_set_physical_resolution(disp, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    LV_LOG_USER("create display w=%d h=%d rotation=%d\n", LV_LOGIC_HOR_SIZE, LV_LOGIC_VER_SIZE, g_screen_rotation);
    if (g_screen_rotation != 0)
        lv_display_set_rotation(disp, g_screen_rotation);

#ifdef CONFIG_XOS_USE_FB_DIRECT
#ifdef CONFIG_XOS_FB_USE_ARGB1555
    lv_display_set_buffers(disp, buf1_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
#else
    lv_display_set_buffers(disp, buf1_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_DIRECT);
#endif
#else
    lv_display_set_buffers(disp, buf1_1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    lv_display_set_flush_cb(disp, fbdev_flush);

    /* interrupt callback for DMA2D transfer */
    // hdma2d.XferCpltCallback = disp_flush_complete;
#if 0
#if 1
    lv_indev_t *indev_touchpad = lv_evdev_create(LV_INDEV_TYPE_POINTER, EVDEV_NAME);
    // lv_indev_set_mode(indev_touchpad, LV_INDEV_MODE_TIMER);
    lv_indev_set_display(indev_touchpad, disp);
    // lv_evdev_set_calibration(indev_touchpad, EVDEV_HOR_MIN, EVDEV_VER_MIN, EVDEV_HOR_MAX, EVDEV_VER_MAX);

#else
    /* basic LVGL driver initialization */
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, _evdev_read);
    lv_indev_set_display(indev, disp);
#endif
    lv_indev_set_group(indev_touchpad, lv_group_get_default());
#endif

#endif /* CONFIG_XOS_RES_800x1280 */

#ifdef CONFIG_XOS_USE_EVDEV
    xos_evdev_indev_init();
#endif

    LV_LOG_USER("%s x\n", __func__);
}

static void signal_handler(int sig)
{
    (void)sig;
    g_stop = 1;
    LV_LOG_USER("signal_handler:sig=%d", sig);
}

int main(void)
{
    int retry_count;
    LV_LOG_USER("main %d\n", __LINE__);
    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);

    #if LV_USE_PROFILER
    time_t rawtime;
    struct tm * timeinfo;
    char filename[64];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(filename, sizeof(filename), "/mnt/sdcard/%Y_%m_%d_%H_%M_%S.txt", timeinfo);
    g_trace_file = fopen(filename, "w");
    if (g_trace_file == NULL) {
        printf("cannot open g_trace_file=%s", filename);
        return -1;
    }
    #endif

    #ifdef CONFIG_XOS_FWK_PARAM
	/* system param.ini */
	param_init(QUA_PARAM_FILE_PATH);
	/* product info */
	LV_LOG_USER("product_name:%s, vendor:%s, b_num=%d\n",
	param_get_string("sys.devinfo:product_name", NULL),
	param_get_string("sys.devinfo:vendor", NULL),
	param_get_int("sys.devinfo:b_num", 0)
	);
    g_screen_rotation = param_get_int("setting.photo:show_hor", 0);
	#endif
    lv_init();
    init_screen_setting();
    xos_lcm_chk_screens();

#ifndef CONFIG_XOS_USE_DLMPI
    if (init_common_sys() != QUA_SUCCESS) {
        LV_LOG_USER("init_common_sys failed\n");
        return -1;
    }

    QUA_BOOL gl_ret = qua_gl_init(g_sys_ops);
    if (!gl_ret) {
        LV_LOG_USER("qua_gl_init error!\n");
        return -1;
    }
#endif

    if(qm_screen_global_enabled > 0 && xos_lcm_1st_is_existed())
    {
        retry_count = 0;
        while (retry_count < 5) {
            if (0 == access("/dev/fb0", F_OK)) {
                sleep(1);
                fbdev_init();
                break;
            } else {
                retry_count++;
                LV_LOG_USER("fbdev not ready, retry count %d\n", retry_count);
                sleep(1);
            }
        }
    }/*if(qm_screen_global_enabled > 0)*/

    if(qm_screen_global_enabled > 1 && xos_lcm_2nd_is_existed()){
    #ifdef CONFIG_XOS_LCM_2ND_SCR
        retry_count = 0;
        while (retry_count < 5) {
            if (0 == access("/dev/fb4", F_OK)) {
                sleep(1);
                fbdev2_init();
                break;
            } else {
                retry_count++;
                LV_LOG_USER("fbdev2 not ready, retry count %d\n", retry_count);
                sleep(1);
            }
        }
    #endif
    }/*if(qm_screen_global_enabled > 1)*/

    LV_LOG_USER("%s 1\n", __func__);

    hal_init_v9();
    lv_tick_set_cb(__tick_get);

#if (XOS_USE_APP_MANAGER != 0) && !defined(CONFIG_XOS_USE_DTEST)
    app_manager_init();
    
    extern void AppsRegister(void);
    AppsRegister();

    pthread_create(&xos_thread_appmgr_handle, 0, xos_thread_appmanager_entity, NULL);
#endif

    LV_LOG_USER("%s 2\n", __func__);
    lv_async_call(__init_gui, NULL);
    LV_LOG_USER("%s 3\n", __func__);

    while(!g_stop) {
        uint32_t next_ttl = lv_timer_handler();
        if (next_ttl < 1) next_ttl = 1;
        usleep(next_ttl*1000);
    }

    #if LV_USE_PROFILER
    lv_profiler_builtin_flush();
    fclose(g_trace_file);
    #endif
    if(qm_screen_global_enabled > 0 && xos_lcm_1st_is_existed()){
        fbdev_exit();
    }/*if(qm_screen_global_enabled > 0)*/

    if(qm_screen_global_enabled > 1 && xos_lcm_2nd_is_existed()){
    #ifdef CONFIG_XOS_LCM_2ND_SCR
        fbdev2_exit();
    #endif
    }/*if(qm_screen_global_enabled > 1)*/

#ifndef CONFIG_XOS_USE_DLMPI
    qua_gl_deinit();
#endif

#if (XOS_USE_APP_MANAGER != 0) && !defined(CONFIG_XOS_USE_DTEST)
    #ifdef CONFIG_XOS_APP_XIAOMING
    if(app_manager_name_exist("xmphoto")){
        app_manager_exit("xmphoto");
    }
    #endif
#endif 
    return 0;
}

static void __init_gui(void *user_data)
{
#ifdef CONFIG_XOS_USE_DTEST
    LV_LOG_USER("%s e\n", __func__);
    extern void __1st_lcm_test(void);
    __1st_lcm_test();
    LV_LOG_USER("%s m\n", __func__);
#ifdef CONFIG_XOS_LCM_2ND_SCR
    extern void __2nd_scr_test(void);
    __2nd_scr_test(); */
#endif
    LV_LOG_USER("%s x\n", __func__);
    return;
#endif

#ifdef CONFIG_XOS_PLAY_BOOTANIMATION
    QUA_S32 ret = qua_wait_until_bootanimation_status(QUA_BOOTANIMAION_STATUS_BOOT_FINISH);
    if (ret != QUA_SUCCESS) {
        LV_LOG_USER("wait for boot finish failed 0x%x\n", ret);
        return;
    }

    qua_request_bootanimation(QUA_BOOTANIMAION_CMD_CLEAN);
#endif

    extern void AppsFactories(void);
    AppsFactories();
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
static uint32_t __tick_get(void)
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

#if XOS_USE_APP_MANAGER != 0
void* xos_thread_appmanager_entity(void * args) {
    extern void xos_qua_boot_init(void);
    xos_qua_boot_init();
    return NULL;
}
#endif
