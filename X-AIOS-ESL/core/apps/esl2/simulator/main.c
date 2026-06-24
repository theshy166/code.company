/**
 * @file main.c
 * @brief ESL2 模拟器 — 用真实 view 代码渲染 UI，支持 SDL 窗口实时查看
 *
 * 架构说明：
 *   这个 main.c 使用 SDL2 创建 LVGL 显示器和输入设备，然后启动真实的
 *   esl2 app_manager 流程：
 *     esl2_init() -> app_manager_start("esl2") -> create_ui()
 *       -> xos_esl_ui_init() -> page_esl.c -> esl_show_ui()
 *
 *   模拟器只负责提供平台桩函数，并可把 ESL2_SIM_MODEL_JSON 指定的
 *   真实模板 JSON 放到产品代码会读取的本地缓存路径。
 *
 * 使用方法：
 *   1. cd core/apps/esl2/simulator/build && cmake .. && make
 *   2. ./esl2_sim
 *   3. SDL 窗口会打开，显示 page_esl.c 真实渲染链路
 *
 * 编译依赖：
 *   cmake, pkg-config, SDL2 开发库, curl 开发库
 *   sudo apt install cmake pkg-config libsdl2-dev libcurl4-openssl-dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "lvgl/lvgl.h"

// ── 设 XOS_ROOT 环境变量，供 resolve_font_path 查找字体用 ──
#define XOS_ROOT_PATH "/home/luoxin/work/X-AIOS-ESL"

// ── LVGL v9 SDL 驱动 ──
#include "drivers/sdl/lv_sdl_window.h"
#include "drivers/sdl/lv_sdl_mouse.h"
#include "drivers/sdl/lv_sdl_mousewheel.h"
#include "drivers/sdl/lv_sdl_keyboard.h"

#include "../conf/conf.h"
#include "appmanager.h"

// ── 模拟器专用桩声明 ──
#include "xos_stub.h"

// ======================== 屏幕参数 ========================
#define SIM_DEFAULT_HOR_RES 800
#define SIM_DEFAULT_VER_RES 1280

static int sim_hor_res = SIM_DEFAULT_HOR_RES;
static int sim_ver_res = SIM_DEFAULT_VER_RES;

// ======================== LVGL Tick ========================
static uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if (start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;
    return (uint32_t)(now_ms - start_ms);
}

// ======================== SDL 初始化 ========================
static void hal_init_sdl(void)
{
    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);

    /* 创建 SDL 窗口作为显示器 */
    lv_display_t *disp = lv_sdl_window_create(sim_hor_res, sim_ver_res);
    lv_display_set_default(disp);
    lv_display_set_1st_scr(disp);
    lv_display_set_2nd_scr(NULL);

    /* 鼠标输入 */
    lv_indev_t *mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, g);
    lv_indev_set_display(mouse, disp);

    /* 滚轮输入 */
    lv_indev_t *mw = lv_sdl_mousewheel_create();
    lv_indev_set_group(mw, g);
    lv_indev_set_display(mw, disp);

    /* 键盘输入（按 ESC 退出） */
    lv_indev_t *kb = lv_sdl_keyboard_create();
    lv_indev_set_group(kb, g);
    lv_indev_set_display(kb, disp);
}

static int mkdir_p(const char *path)
{
    char tmp[512];
    size_t len;

    if (!path || path[0] == '\0') return -1;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (len == 0) return -1;

    if (tmp[len - 1] == '/') tmp[len - 1] = '\0';

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0775) != 0 && errno != EEXIST) return -1;
            *p = '/';
        }
    }
    if (mkdir(tmp, 0775) != 0 && errno != EEXIST) return -1;
    return 0;
}

static char *read_text_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    long size;
    char *buf;

    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (size < 0) {
        fclose(fp);
        return NULL;
    }
    buf = malloc((size_t)size + 1);
    if (!buf) {
        fclose(fp);
        return NULL;
    }
    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        free(buf);
        fclose(fp);
        return NULL;
    }
    buf[size] = '\0';
    fclose(fp);
    return buf;
}

static int write_text_file(const char *path, const char *text)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return -1;
    if (text) fwrite(text, 1, strlen(text), fp);
    fclose(fp);
    return 0;
}

static void prepare_local_model_cache(void)
{
    char cache_path[512];
    char cache_dir[] = "./out/simulator/xos/res/json";
    char device_sn[128];
    const char *source_path = getenv("ESL2_SIM_MODEL_JSON");
    char *source_text = NULL;
    const char *serial = sim_get_device_serial();

    snprintf(device_sn, sizeof(device_sn), "%s_1", serial);
    snprintf(cache_path, sizeof(cache_path), k_path_json_product_base_model, device_sn);

    if (!source_path || source_path[0] == '\0') {
        if (access(cache_path, R_OK) == 0) {
            printf("[SIM] using existing local model cache: %s\n", cache_path);
        } else {
            printf("[SIM] no local model cache for %s, product code will show empty UI\n", device_sn);
        }
        return;
    }

    source_text = read_text_file(source_path);
    if (!source_text) {
        fprintf(stderr, "[SIM] failed to read ESL2_SIM_MODEL_JSON=%s\n", source_path);
        return;
    }

    mkdir_p(cache_dir);
    if (write_text_file(cache_path, source_text) == 0) {
        printf("[SIM] copied real model json: %s -> %s\n", source_path, cache_path);
    } else {
        fprintf(stderr, "[SIM] failed to copy model json to cache: %s\n", cache_path);
    }

    free(source_text);
}

static int env_int(const char *name, int def)
{
    const char *value = getenv(name);
    char *end = NULL;
    long parsed;

    if (!value || value[0] == '\0') return def;
    parsed = strtol(value, &end, 10);
    if (end == value || parsed <= 0 || parsed > 4096) return def;
    return (int)parsed;
}

// ======================== 主函数 ========================
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("=== ESL2 Simulator with Real ESL2 View Flow ===\n");

    sim_hor_res = env_int("ESL2_SIM_WIDTH", SIM_DEFAULT_HOR_RES);
    sim_ver_res = env_int("ESL2_SIM_HEIGHT", SIM_DEFAULT_VER_RES);

    // ----- 1. LVGL 初始化 -----
    lv_init();

    // ----- 2. SDL 显示+输入初始化 -----
    hal_init_sdl();

    // ----- 3. 设置 Tick 回调（LVGL 需要时间基准）-----
    lv_tick_set_cb(custom_tick_get);

    // ----- 4. 设置默认背景色 -----
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), 0);

    // ----- 4.5. 设置 XOS_ROOT、准备字体和本地模板缓存 -----
    setenv("XOS_ROOT", XOS_ROOT_PATH, 1);
    sim_init_fonts();
    prepare_local_model_cache();

    // ----- 5. 启动真实 esl2 app_manager 流程 -----
    extern void esl2_init(void);
    esl2_init();
    app_manager_start("esl2", NULL);

    // ----- 6. LVGL 主循环（SDL 窗口持续刷新）-----
    printf("SDL window opened (%dx%d). Press Ctrl+C to exit.\n", sim_hor_res, sim_ver_res);
    while (1) {
        // lv_timer_handler 返回下一次需要刷新的毫秒数
        uint32_t next_ttl = lv_timer_handler();
        if (next_ttl < 1) next_ttl = 1;
        usleep(next_ttl * 1000);
    }

    return 0;
}
