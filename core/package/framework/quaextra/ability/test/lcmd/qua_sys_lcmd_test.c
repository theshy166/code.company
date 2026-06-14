/*
 *
 * Copyright (c) Quaming Intelligent Technology Co., Ltd.
 * (C) Copyright All Rights Reserved.
 *
 */
#include "qua_sys_lcmd_adap.h"
#include <lvgl/lvgl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// 文件路径定义
#define qua_sys_path_sufix      ".bmp"
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
#define qua_sys_path_prefix     "H:./out/simulator/xos/res/res_lcmd/"
#else
#define qua_sys_path_prefix     "H:/udisk0/res_lcmd/"
#endif

// 图片路径宏定义
#define qua_sys_path_test_white     qua_sys_path_prefix "test_white" qua_sys_path_sufix
#define qua_sys_path_test_black     qua_sys_path_prefix "test_black" qua_sys_path_sufix
#define qua_sys_path_test_red       qua_sys_path_prefix "test_red" qua_sys_path_sufix
#define qua_sys_path_test_green     qua_sys_path_prefix "test_green" qua_sys_path_sufix
#define qua_sys_path_test_blue      qua_sys_path_prefix "test_blue" qua_sys_path_sufix
#define qua_sys_path_test_color1    qua_sys_path_prefix "test_color1" qua_sys_path_sufix
#define qua_sys_path_test_color2    qua_sys_path_prefix "test_color2" qua_sys_path_sufix
#define qua_sys_path_test_color3    qua_sys_path_prefix "test_color3" qua_sys_path_sufix
#define qua_sys_path_test_halfbw    qua_sys_path_prefix "test_halfbw" qua_sys_path_sufix

// 声明屏幕分辨率获取接口
extern int _get_scr_width(void);
extern int _get_scr_height(void);

// 要显示的BMP图片列表
const char *qua_sys_bmp_files[] = {
    qua_sys_path_test_black,  // 黑色图片
    qua_sys_path_test_white,  // 白色图片
    qua_sys_path_test_red,    // 红色图片
    qua_sys_path_test_green,  // 绿色图片
    qua_sys_path_test_blue,   // 蓝色图片
    qua_sys_path_test_color1, // 彩色图片1
    qua_sys_path_test_color2, // 彩色图片2
    qua_sys_path_test_color3  // 彩色图片3
};

#define QUA_SYS_BMP_FILE_COUNT      (sizeof(qua_sys_bmp_files) / sizeof(qua_sys_bmp_files[0]))
#define QUA_SYS_THREAD_STACK_SIZE   4096
#define QUA_SYS_THREAD_PRIORITY     25
#define QUA_SYS_THREAD_TIMESLICE    5
#define QUA_SYS_MAX_DISPLAY_ROUNDS  10 // 最大轮询次数

// 全局变量
static lv_obj_t *img_obj = NULL;
static qua_sys_thread_t bmp_display_thread = NULL;
static volatile bool thread_running = false;

// 检查文件是否存在
static bool qua_sys_check_file_exist(const char *path)
{
    // struct stat statbuf;
    // return !(qua_sys_stat(path, &statbuf) == 0);
    return true;
}

// 检查资源目录是否存在
static bool qua_sys_check_resource_dir(void)
{
    // 提取路径前缀的目录部分
    // char dir_path[256];
    // const char *last_slash = strrchr(qua_sys_path_prefix, '/');
    // if (!last_slash) {
    //     qua_sys_printf("Invalid path prefix: %s\n", qua_sys_path_prefix);
    //     return false;
    // }

    // // 复制目录路径
    // size_t dir_len = last_slash - qua_sys_path_prefix;
    // strncpy(dir_path, qua_sys_path_prefix, dir_len);
    // dir_path[dir_len] = '\0';

    // // 检查目录是否存在
    // struct stat statbuf;
    // // bool rtn = (qua_sys_stat(dir_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode));
    // bool rtn = (qua_sys_stat(dir_path, &statbuf) == 0);
    // if (!rtn) {
    //     qua_sys_printf("Resource directory not found: %s\n", dir_path);
    // }
    // return rtn;
    return true;
}

// 显示指定的BMP图片
static int qua_sys_display_bmp_image(const char *img_path)
{
    // 检查文件是否存在
    if (!qua_sys_check_file_exist(img_path))
    {
        qua_sys_printf("BMP file not found: %s\n", img_path);
        return -1;
    }

    // 删除之前的图片对象
    if (img_obj)
    {
        lv_obj_del(img_obj);
        img_obj = NULL;
    }

    // 创建新的图片对象
    img_obj = lv_img_create(lv_scr_act());
    if (!img_obj)
    {
        qua_sys_printf("Failed to create LVGL image object\n");
        return -1;
    }

    // 获取屏幕分辨率
    int scr_width = _get_scr_width();
    int scr_height = _get_scr_height();
    if (scr_width <= 0 || scr_height <= 0)
    {
        qua_sys_printf("Invalid screen resolution: %dx%d\n", scr_width, scr_height);
        return -1;
    }

    // 设置图片源（BMP格式）
    lv_img_set_src(img_obj, img_path);

    // 设置为全屏显示
    lv_obj_set_size(img_obj, scr_width, scr_height);
    lv_obj_align(img_obj, LV_ALIGN_CENTER, 0, 0);

    qua_sys_printf("Displayed BMP image: %s (resolution: %dx%d)\n", img_path, scr_width, scr_height);
    return 0;
}

// BMP图片显示线程函数 - 轮询10次后自动结束
static void qua_sys_bmp_display_thread_func(void *parameter)
{
    int i, round_count = 0;

    // 确保LVGL初始化完成（需支持BMP解码）
    while (!lv_is_initialized())
    {
        qua_sys_msleep(100); // 等待100ms
    }

    // 检查资源目录是否存在
    if (!qua_sys_check_resource_dir())
    {
        qua_sys_printf("Resource directory not found: %s\n", qua_sys_path_prefix);
        thread_running = false;
        return;
    }
    // extern void pause_render_timer(void);
    // pause_render_timer();
    // 循环显示所有BMP图片，最多轮询10次
    while (thread_running && round_count < QUA_SYS_MAX_DISPLAY_ROUNDS)
    {
        qua_sys_printf("Starting display round %d/%d\n", round_count + 1, QUA_SYS_MAX_DISPLAY_ROUNDS);

        for (i = 0; i < QUA_SYS_BMP_FILE_COUNT && thread_running; i++)
        {
            qua_sys_display_bmp_image(qua_sys_bmp_files[i]);
            // 每张图片显示5秒
            qua_sys_msleep(5000);
        }

        round_count++;
    }

    // 轮询完成后自动结束
    qua_sys_printf("Completed %d display rounds, process will exit\n", round_count);

    // 清理工作
    if (img_obj)
    {
        lv_obj_del(img_obj);
        img_obj = NULL;
    }
    // extern void resume_render_timer(void);
    // resume_render_timer();
    thread_running = false;
}

// 启动BMP显示进程
int qua_sys_start_bmp_display_process(void)
{
    if (thread_running)
    {
        qua_sys_printf("BMP display process is already running\n");
        return -1;
    }

    // 初始化系统适配层
    qua_sys_adapter_init();

    // 创建线程
    bmp_display_thread = qua_sys_thread_create("bmp_disp",
                                               qua_sys_bmp_display_thread_func,
                                               NULL,
                                               QUA_SYS_THREAD_STACK_SIZE,
                                               QUA_SYS_THREAD_PRIORITY,
                                               QUA_SYS_THREAD_TIMESLICE);

    if (bmp_display_thread == NULL)
    {
        qua_sys_printf("Failed to create BMP display thread\n");
        return -1;
    }

    // 启动线程
    thread_running = true;
    qua_sys_thread_startup(bmp_display_thread);

    qua_sys_printf("BMP display process started successfully (will auto exit after %d rounds)\n",
                   QUA_SYS_MAX_DISPLAY_ROUNDS);
    return 0;
}

// 停止BMP显示进程
int qua_sys_stop_bmp_display_process(void)
{
    if (!thread_running || bmp_display_thread == NULL)
    {
        qua_sys_printf("BMP display process is not running\n");
        return -1;
    }

    thread_running = false;
    qua_sys_thread_join(bmp_display_thread);
    qua_sys_thread_delete(bmp_display_thread);
    bmp_display_thread = NULL;

    qua_sys_printf("BMP display process stopped\n");
    return 0;
}

int qua_sys_lcmd_test(void)
{
    return qua_sys_start_bmp_display_process();
}

int qua_sys_lcmd_test_entry(void)
{
    int scr_width = _get_scr_width();
    int scr_height = _get_scr_height();
    lv_obj_t *image = lv_img_create(lv_scr_act());
    if (!image)
    {
        qua_sys_printf("Failed to create LVGL image object\n");
        return -1;
    }

    if (scr_width <= 0 || scr_height <= 0)
    {
        qua_sys_printf("Invalid screen resolution: %dx%d\n", scr_width, scr_height);
        return -1;
    }

    lv_img_set_src(image, qua_sys_path_test_halfbw);
    lv_obj_set_size(image, scr_width, scr_height);
    lv_obj_align(image, LV_ALIGN_CENTER, 0, 0);
    return 0;
}

