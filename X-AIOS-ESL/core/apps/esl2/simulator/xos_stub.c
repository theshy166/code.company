#include "xos_stub.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

/* ========================
   qua_font.h
   ======================== */

#include "lvgl/src/libs/freetype/lv_freetype.h"
#include <sys/stat.h>

#define MAX_FONT_CACHE 16
static struct {
    int size;
    lv_font_t *font;
    char path[256];
} font_cache[MAX_FONT_CACHE];
static int font_cache_count = 0;
static int font_cache_inited = 0;

/* 检查常规文件是否存在 */
static int file_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

/* 解析路径：去掉驱动器字母前缀 (如 H: -> ), 并尝试真实路径 */
static const char *resolve_font_path(const char *path, char *buf, size_t buf_size)
{
    if (!path || path[0] == '\0') return NULL;

    /* 尝试原路径 */
    if (file_exists(path)) return path;

    /* 去掉驱动器字母前缀，如 "H:./out/..." -> "./out/..." */
    const char *stripped = path;
    if (path[1] == ':' && path[2] != '\0') {
        stripped = path + 2;
    }
    /* 跳过开头的 ./ 再试一次 */
    if (stripped[0] == '.' && stripped[1] == '/') {
        stripped += 2;
    }

    if (stripped != path && file_exists(stripped)) {
        return stripped;
    }

    /* 尝试基于工作目录拼接 */
    if (stripped[0] != '/') {
        /* 相对于 XOS_ROOT 尝试 */
        const char *xos_root = getenv("XOS_ROOT");
        if (xos_root) {
            snprintf(buf, buf_size, "%s/%s", xos_root, stripped);
            if (file_exists(buf)) return buf;
        }
        /* 相对于当前目录 */
        snprintf(buf, buf_size, "%s", stripped);
        if (file_exists(buf)) return buf;
    }

    return NULL;
}

static const char *get_default_font_path(void)
{
    /* 按优先级查找系统中文字体 */
    static const char *candidates[] = {
        "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",
        "/usr/share/fonts/opentype/noto/NotoSerifCJK-Bold.ttc",
        "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
        NULL
    };
    for (int i = 0; candidates[i]; i++) {
        if (file_exists(candidates[i])) return candidates[i];
    }
    return NULL;
}

void qua_xos_ft_init_with_size_path(lv_font_t **font_out, int size, const char *path, int style)
{
    if (!font_cache_inited) {
        memset(font_cache, 0, sizeof(font_cache));
        font_cache_count = 0;
        font_cache_inited = 1;
    }

    (void)style;

    /* 检查缓存 */
    for (int i = 0; i < font_cache_count; i++) {
        if (font_cache[i].size == size) {
            if (font_out) *font_out = font_cache[i].font;
            return;
        }
    }

    if (font_cache_count >= MAX_FONT_CACHE) {
        fprintf(stderr, "[FONT] cache full, skipping size %d\n", size);
        if (font_out) *font_out = NULL;
        return;
    }

    /* 确定字体路径 */
    char resolved[512] = {0};
    const char *font_path = resolve_font_path(path, resolved, sizeof(resolved));
    if (!font_path) {
        /* 回退到系统默认字体 */
        font_path = get_default_font_path();
    }
    if (!font_path) {
        fprintf(stderr, "[FONT] no font file found for size %d\n", size);
        if (font_out) *font_out = NULL;
        return;
    }

    printf("[FONT] loading '%s' size=%d\n", font_path, size);

    lv_font_t *font = lv_freetype_font_create(
        font_path,
        LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
        size,
        LV_FREETYPE_FONT_STYLE_NORMAL
    );

    if (font) {
        font_cache[font_cache_count].size = size;
        font_cache[font_cache_count].font = font;
        strncpy(font_cache[font_cache_count].path, font_path,
                sizeof(font_cache[font_cache_count].path) - 1);
        font_cache_count++;
        printf("[FONT] loaded size=%d OK\n", size);
    } else {
        fprintf(stderr, "[FONT] FAILED to load '%s' size=%d\n", font_path, size);
    }

    /* ★★★ 关键：通过输出参数返回字体指针 ★★★ */
    if (font_out) {
        *font_out = font;
        printf("[FONT] returning font=%p via output param\n", (void*)font);
    }
}

/* ========================
   qm_wifi_cfg.h
   ======================== */
int qm_wifi_connect_status(void)
{
    return 1;
}

void qm_connection_wifi_restart(void)
{
    printf("[STUB] qm_connection_wifi_restart\n");
}

/* ========================
   qm_gpio.h
   ======================== */
int qm_gpio_export_direction(int pin, int dir)
{
    printf("[STUB] qm_gpio_export_direction: pin=%d, dir=%d\n", pin, dir);
    return 0;
}

int qm_gpio_set_value(int pin, int val)
{
    printf("[STUB] qm_gpio_set_value: pin=%d, val=%d\n", pin, val);
    return 0;
}

/* ========================
   qm_backlight.h
   ======================== */
void qm_backlight_setbri(int brightness)
{
    printf("[STUB] qm_backlight_setbri: %d\n", brightness);
}

/* ========================
   xos 系统函数
   ======================== */
int xos_lcm_get_screen_num(void) { return 1; }
int xos_logo_update(const char *path) { printf("[STUB] xos_logo_update: %s\n", path); return 0; }
void xos_esl_ui_init(void) { printf("[STUB] xos_esl_ui_init\n"); }
void xos_esl_ui_init_2(void) { printf("[STUB] xos_esl_ui_init_2\n"); }
const char *xos_get_sn(void) { return "SIM-ESL2-001"; }

/* ========================
   serial_number.h
   ======================== */
const char *serial_number_get(void) { return "SIM-ESL2-001"; }

/* ========================
   qm_lv_obj.h
   ======================== */
lv_obj_t *qm_container_create(lv_obj_t *parent)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_style_all(obj);
    return obj;
}
lv_obj_t *qm_label_create(lv_obj_t *parent) { return lv_label_create(parent); }
void qm_label_set_text(lv_obj_t *label, const char *text) { lv_label_set_text(label, text); }
void qm_obj_set_pos(lv_obj_t *obj, int x, int y) { lv_obj_set_pos(obj, x, y); }
void qm_obj_set_size(lv_obj_t *obj, int w, int h) { lv_obj_set_size(obj, w, h); }
void qm_obj_set_style_text_color(lv_obj_t *obj, lv_color_t color) { lv_obj_set_style_text_color(obj, color, 0); }
void qm_obj_set_style_bg_color(lv_obj_t *obj, lv_color_t color) { lv_obj_set_style_bg_color(obj, color, 0); }
lv_obj_t *qm_image_create(lv_obj_t *parent) { return lv_image_create(parent); }

/* ========================
   qua_param.h
   ======================== */
int param_get_int(const char *key, int def)
{
    printf("[STUB] param_get_int: %s -> %d\n", key, def);
    return def;
}
const char *param_get_string(const char *key, const char *def)
{
    printf("[STUB] param_get_string: %s -> %s\n", key, def ? def : "NULL");
    return def;
}

/* ========================
   LED 状态
   ======================== */
void show_green_led_sync(void) { printf("[STUB] show_green_led_sync\n"); }
void clear_green_led_sync(void) { printf("[STUB] clear_green_led_sync\n"); }

/* ========================
   图片/多媒体
   ======================== */
int is_jpg(const char *path)
{
    const char *ext = path ? strrchr(path, '.') : NULL;
    return (ext && (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0)) ? 1 : 0;
}
int is_png(const char *path)
{
    const char *ext = path ? strrchr(path, '.') : NULL;
    return (ext && strcasecmp(ext, ".png") == 0) ? 1 : 0;
}
int decode_jpeg_with_turbo(const char *path, unsigned char **out, int *w, int *h)
{
    printf("[STUB] decode_jpeg_with_turbo: %s\n", path ? path : "NULL");
    *out = NULL;
    *w = 0;
    *h = 0;
    return -1;
}
int download_image(const char *url, const char *save_path)
{
    printf("[STUB] download_image: %s -> %s\n", url ? url : "NULL", save_path ? save_path : "NULL");
    return -1;
}

/* ========================
   LOS 视频播放器
   ======================== */
void *los_player_create2(int x, int y, int w, int h, const char *url, int idx)
{
    printf("[STUB] los_player_create2: %s\n", url ? url : "NULL");
    return NULL;
}
void los_set_progress_callback(void *player, void *cb)
{
    (void)player;
    (void)cb;
    printf("[STUB] los_set_progress_callback\n");
}
int los_videoplayer_quit(void *player)
{
    (void)player;
    printf("[STUB] los_videoplayer_quit\n");
    return 0;
}

/* ========================
   PNG 编码/处理
   ======================== */
int qm_png2param_create(const char *path, void **out, int *len)
{
    printf("[STUB] qm_png2param_create: %s\n", path ? path : "NULL");
    *out = NULL;
    *len = 0;
    return -1;
}
int qm_png2param_create_v2(const char *path, void **out, int *len, int w, int h)
{
    printf("[STUB] qm_png2param_create_v2: %s\n", path ? path : "NULL");
    *out = NULL;
    *len = 0;
    return -1;
}
void *qm_pngencoder(const unsigned char *data, int w, int h, int bpp, int *len)
{
    printf("[STUB] qm_pngencoder\n");
    *len = 0;
    return NULL;
}
int qm_get_png_size(const char *path, int *w, int *h)
{
    printf("[STUB] qm_get_png_size: %s\n", path ? path : "NULL");
    *w = 0;
    *h = 0;
    return -1;
}

/* ========================
   升级相关
   ======================== */
int qm_updater_change_text_label(const char *text)
{
    printf("[STUB] qm_updater_change_text_label: %s\n", text ? text : "NULL");
    return 0;
}

/* ========================
   其他
   ======================== */
int draw_border_for_debug(lv_obj_t *obj) { (void)obj; return 0; }

unsigned int hex_to_uint32(const char *hex_str)
{
    unsigned int val = 0;
    if (hex_str) {
        if (hex_str[0] == '#') sscanf(hex_str + 1, "%x", &val);
        else sscanf(hex_str, "%x", &val);
    }
    return val;
}