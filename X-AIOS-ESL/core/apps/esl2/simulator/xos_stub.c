#define _GNU_SOURCE
#include "xos_stub.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <jpeglib.h>
#include <png.h>
#include <curl/curl.h>
#include <dlfcn.h>
#include "appcommon.h"
#include "cadp_aiskill.h"

#define SIM_DEFAULT_SERIAL "SIM-ESL2-001"

const char *sim_get_device_serial(void)
{
    const char *serial = getenv("ESL2_SIM_SN");
    return (serial && serial[0] != '\0') ? serial : SIM_DEFAULT_SERIAL;
}

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

static int mkdir_p_for_file(const char *file_path)
{
    char tmp[512];
    char *slash;

    if (!file_path || file_path[0] == '\0') return -1;
    snprintf(tmp, sizeof(tmp), "%s", file_path);
    slash = strrchr(tmp, '/');
    if (!slash) return 0;
    *slash = '\0';

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

static const char *resolve_resource_path(const char *path, char *buf, size_t buf_size)
{
    const char *stripped;
    const char *res_part;
    const char *xos_root;

    if (!path || path[0] == '\0') return NULL;
    if (file_exists(path)) return path;

    stripped = path;
    if (path[1] == ':' && path[2] != '\0') {
        stripped = path + 2;
    }
    while (stripped[0] == '.' && stripped[1] == '/') {
        stripped += 2;
    }
    if (file_exists(stripped)) return stripped;

    xos_root = getenv("XOS_ROOT");
    if (xos_root) {
        snprintf(buf, buf_size, "%s/%s", xos_root, stripped);
        if (file_exists(buf)) return buf;

        res_part = strstr(stripped, "/res/");
        if (!res_part) {
            res_part = strstr(stripped, "res/");
        }
        if (res_part) {
            if (res_part[0] == '/') res_part++;
            if (strncmp(res_part, "res/", 4) == 0) res_part += 4;

            snprintf(buf, buf_size, "%s/core/res/%s", xos_root, res_part);
            if (file_exists(buf)) return buf;

            snprintf(buf, buf_size, "%s/out/qm10xd_linux/xos/res/%s", xos_root, res_part);
            if (file_exists(buf)) return buf;
        }
    }

    return NULL;
}

static const char *resolve_output_path(const char *path, char *buf, size_t buf_size)
{
    const char *stripped;

    if (!path || path[0] == '\0') return NULL;

    stripped = path;
    if (path[1] == ':' && path[2] != '\0') {
        stripped = path + 2;
    }
    while (stripped[0] == '.' && stripped[1] == '/') {
        stripped += 2;
    }

    if (strncmp(stripped, "/data/res/", 10) == 0) {
        snprintf(buf, buf_size, "./out/simulator/xos/res/%s", stripped + 10);
    } else {
        snprintf(buf, buf_size, "%s", stripped);
    }

    return buf;
}

struct sim_response_data {
    char *data;
    size_t size;
};

static size_t sim_write_memory_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
    struct sim_response_data *data = (struct sim_response_data *)userp;
    size_t realsize = size * nmemb;
    char *ptr = realloc(data->data, data->size + realsize + 1);
    if (!ptr) return 0;
    data->data = ptr;
    memcpy(data->data + data->size, contents, realsize);
    data->size += realsize;
    data->data[data->size] = '\0';
    return realsize;
}

static char *read_text_file_dup(const char *path)
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

static long sim_env_long(const char *name, long def)
{
    const char *value = getenv(name);
    char *end = NULL;
    long parsed;

    if (!value || value[0] == '\0') return def;
    parsed = strtol(value, &end, 10);
    if (end == value || parsed <= 0) return def;
    return parsed;
}

static const char *sim_response_override_path(const char *url)
{
    if (url && strstr(url, "/config")) {
        return getenv("ESL2_SIM_CONFIG_JSON");
    } else if (url && strstr(url, "/content")) {
        return getenv("ESL2_SIM_CONTENT_JSON");
    }
    return NULL;
}

static char *sim_read_response_override(const char *url)
{
    const char *path = sim_response_override_path(url);
    if (path && path[0] != '\0') {
        char *text = read_text_file_dup(path);
        printf("[SIM] HTTP override: %s -> %s (%s)\n",
               url ? url : "NULL", path, text ? "OK" : "FAIL");
        return text;
    }
    return NULL;
}

static int sim_should_use_fast_http(const char *url)
{
    const char *path = sim_response_override_path(url);
    const char *fast = getenv("ESL2_SIM_FAST_HTTP");

    if (path && path[0] != '\0') return 1;
    return fast && strcmp(fast, "1") == 0;
}

static char *sim_http_get(const char *url, int max_retries, int retry_interval_ms)
{
    long connect_timeout_ms = sim_env_long("ESL2_SIM_HTTP_CONNECT_TIMEOUT_MS", 3000);
    long timeout_ms = sim_env_long("ESL2_SIM_HTTP_TIMEOUT_MS", 8000);
    int retries = max_retries > 0 ? max_retries : 1;

    char *override = sim_read_response_override(url);
    if (override) return override;

    for (int attempt = 1; attempt <= retries; attempt++) {
        CURL *curl = curl_easy_init();
        struct sim_response_data data = {NULL, 0};
        CURLcode res;

        if (!curl) return NULL;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sim_write_memory_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, connect_timeout_ms);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms);

        const char *insecure_tls = getenv("ESL2_SIM_INSECURE_TLS");
        if (insecure_tls && strcmp(insecure_tls, "1") == 0) {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        }

        printf("[SIM] HTTP GET attempt %d/%d: %s\n", attempt, retries, url ? url : "NULL");
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            curl_easy_cleanup(curl);
            if (http_code == 0 || (http_code >= 200 && http_code < 300)) {
                return data.data ? data.data : strdup("");
            }
            fprintf(stderr, "[SIM] HTTP status %ld for %s\n", http_code, url ? url : "NULL");
        } else {
            fprintf(stderr, "[SIM] HTTP GET failed: %s (%s)\n",
                    url ? url : "NULL", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
        }

        free(data.data);
        if (attempt < retries && retry_interval_ms > 0) {
            usleep((useconds_t)retry_interval_ms * 1000);
        }
    }

    return NULL;
}

char *esl_get_remote_url(const char *url, const char *wifi_ssid, const char *local_ip,
                         const char *version_no, int max_retries, int retry_interval_ms,
                         char *platform)
{
    if (sim_should_use_fast_http(url)) {
        return sim_http_get(url, max_retries, retry_interval_ms);
    }

    typedef char *(*real_get_fn)(const char *, const char *, const char *, const char *, int, int, char *);
    real_get_fn real_get = (real_get_fn)dlsym(RTLD_NEXT, "esl_get_remote_url");
    if (real_get) {
        return real_get(url, wifi_ssid, local_ip, version_no, max_retries, retry_interval_ms, platform);
    }

    return sim_http_get(url, max_retries, retry_interval_ms);
}

char *esl_get_remote_url2(char *url, char *wifi_ssid, char *wifi_psw, char *local_ip,
                          char *version_no, int width, int height, int screens,
                          char *token, char *platform)
{
    if (sim_should_use_fast_http(url)) {
        return sim_http_get(url, 1, 100);
    }

    typedef char *(*real_get2_fn)(char *, char *, char *, char *, char *, int, int, int, char *, char *);
    real_get2_fn real_get2 = (real_get2_fn)dlsym(RTLD_NEXT, "esl_get_remote_url2");
    if (real_get2) {
        return real_get2(url, wifi_ssid, wifi_psw, local_ip, version_no, width, height, screens, token, platform);
    }

    return sim_http_get(url, 1, 100);
}

static void rgba_to_bgra(unsigned char *pixels, size_t pixel_count)
{
    for (size_t i = 0; i < pixel_count; i++) {
        unsigned char *p = pixels + i * 4;
        unsigned char r = p[0];
        p[0] = p[2];
        p[2] = r;
    }
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

void qua_xos_ft_init_with_size_path(lv_font_t **font_out, short size, const char *path, short style)
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

void qua_los_ft_font_init(void)
{
    sim_init_fonts();
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
/* 注意：xos_esl_ui_init / xos_esl_ui_init_2 由 page_esl.c 提供真实实现，此处不做桩 */
const char *xos_get_sn(void) { return sim_get_device_serial(); }

/* ========================
   app_manager 最小实现
   ======================== */
static char s_app_name[32];
static qua_app_ops s_app_ops;
static app_data_ptr s_app_data;

void app_manager_register(const char *name, qua_app_ops *ops)
{
    if (!name || !ops) return;
    snprintf(s_app_name, sizeof(s_app_name), "%s", name);
    memcpy(&s_app_ops, ops, sizeof(s_app_ops));
    printf("[STUB] app_manager_register: %s\n", s_app_name);
}

void app_manager_start(const char *app_name, void *start_data)
{
    if (!app_name || strcmp(app_name, s_app_name) != 0 || !s_app_ops.on_create) {
        printf("[STUB] app_manager_start failed: %s\n", app_name ? app_name : "NULL");
        return;
    }
    printf("[STUB] app_manager_start: %s\n", app_name);
    s_app_data = s_app_ops.on_create(start_data);
}

/* ========================
   serial_number.h
   ======================== */
const char *serial_number_get(void) { return sim_get_device_serial(); }

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
    const char *value = NULL;
    char *end = NULL;
    long parsed;

    if (key && strcmp(key, "setting.esl2:esl_parse_type") == 0) {
        value = getenv("ESL2_SIM_PARSE_TYPE");
    } else if (key && strcmp(key, "setting.photo:show_hor") == 0) {
        value = getenv("ESL2_SIM_ROTATION");
    } else if (key && strcmp(key, "setting.photo:show_hor2") == 0) {
        value = getenv("ESL2_SIM_ROTATION2");
    }

    if (value && value[0] != '\0') {
        parsed = strtol(value, &end, 10);
        if (end != value) {
            printf("[STUB] param_get_int/env: %s -> %ld\n", key, parsed);
            return (int)parsed;
        }
    }

    printf("[STUB] param_get_int: %s -> %d\n", key, def);
    return def;
}
const char *param_get_string(const char *key, const char *def)
{
    const char *value = NULL;

    if (key && strcmp(key, "setting.esl2:host") == 0) {
        value = getenv("ESL2_SIM_HOST");
    } else if (key && strcmp(key, "setting.esl2:userName") == 0) {
        value = getenv("ESL2_SIM_USERNAME");
    } else if (key && strcmp(key, "setting.esl2:userPW") == 0) {
        value = getenv("ESL2_SIM_PASSWORD");
    } else if (key && strcmp(key, "setting.esl2:store") == 0) {
        value = getenv("ESL2_SIM_STORE");
    }

    if (value && value[0] != '\0') {
        printf("[STUB] param_get_string/env: %s -> %s\n", key, value);
        return value;
    }

    printf("[STUB] param_get_string: %s -> %s\n", key, def ? def : "NULL");
    return def;
}
int param_set_int(const char *key, int val)
{
    printf("[STUB] param_set_int: %s = %d\n", key ? key : "NULL", val);
    return 0;
}
int param_set_string(const char *key, const char *val)
{
    printf("[STUB] param_set_string: %s = %s\n", key ? key : "NULL", val ? val : "NULL");
    return 0;
}
int param_save(void)
{
    printf("[STUB] param_save\n");
    return 0;
}

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
unsigned char *decode_jpeg_with_turbo(const char *path, int *w, int *h, bool use_mmz, unsigned int *mmz_phy_buf)
{
    char resolved[512] = {0};
    const char *real_path = resolve_resource_path(path, resolved, sizeof(resolved));
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *fp;
    JSAMPARRAY row;
    unsigned char *pixels = NULL;
    int row_stride;

    (void)use_mmz;
    if (mmz_phy_buf) *mmz_phy_buf = 0;
    if (w) *w = 0;
    if (h) *h = 0;

    printf("[STUB] decode_jpeg_with_turbo: %s -> %s\n",
           path ? path : "NULL", real_path ? real_path : "NOT_FOUND");
    if (!real_path) return NULL;

    fp = fopen(real_path, "rb");
    if (!fp) return NULL;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_RGB;
    jpeg_start_decompress(&cinfo);

    if (w) *w = (int)cinfo.output_width;
    if (h) *h = (int)cinfo.output_height;
    row_stride = (int)cinfo.output_width * (int)cinfo.output_components;
    pixels = malloc((size_t)cinfo.output_width * cinfo.output_height * 4);
    row = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, (JDIMENSION)row_stride, 1);

    if (pixels) {
        while (cinfo.output_scanline < cinfo.output_height) {
            JDIMENSION y = cinfo.output_scanline;
            jpeg_read_scanlines(&cinfo, row, 1);
            for (JDIMENSION x = 0; x < cinfo.output_width; x++) {
                unsigned char *src = row[0] + x * 3;
                unsigned char *dst = pixels + ((size_t)y * cinfo.output_width + x) * 4;
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = 255;
            }
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
    return pixels;
}
int download_image(const char *url, const char *save_path)
{
    char resolved_save[512] = {0};
    char resolved_source[512] = {0};
    const char *real_save = resolve_output_path(save_path, resolved_save, sizeof(resolved_save));
    const char *local_source = NULL;
    FILE *out = NULL;
    int ret = -1;

    printf("[SIM] download_image: %s -> %s\n",
           url ? url : "NULL", real_save ? real_save : "NULL");

    if (!url || url[0] == '\0' || !real_save) return -1;
    if (mkdir_p_for_file(real_save) != 0) {
        fprintf(stderr, "[SIM] failed to create parent dir for %s\n", real_save);
        return -1;
    }

    if (strncmp(url, "file://", 7) == 0) {
        local_source = url + 7;
    } else if (file_exists(url)) {
        local_source = url;
    } else {
        local_source = resolve_resource_path(url, resolved_source, sizeof(resolved_source));
    }

    if (local_source) {
        FILE *in = fopen(local_source, "rb");
        out = fopen(real_save, "wb");
        if (in && out) {
            unsigned char buf[8192];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
                if (fwrite(buf, 1, n, out) != n) break;
            }
            ret = ferror(in) || ferror(out) ? -1 : 0;
        }
        if (in) fclose(in);
        if (out) fclose(out);
        printf("[SIM] copied local image: %s -> %s (%s)\n",
               local_source, real_save, ret == 0 ? "OK" : "FAIL");
        return ret;
    }

    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    out = fopen(real_save, "wb");
    if (!out) {
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

    const char *insecure_tls = getenv("ESL2_SIM_INSECURE_TLS");
    if (insecure_tls && strcmp(insecure_tls, "1") == 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        ret = (http_code == 0 || (http_code >= 200 && http_code < 300)) ? 0 : -1;
        if (ret != 0) {
            fprintf(stderr, "[SIM] image HTTP status %ld for %s\n", http_code, url);
        }
    } else {
        fprintf(stderr, "[SIM] image download failed: %s (%s)\n",
                url, curl_easy_strerror(res));
    }

    fclose(out);
    curl_easy_cleanup(curl);
    if (ret != 0) unlink(real_save);
    return ret;
}

/* ========================
   LOS 视频播放器
   ======================== */
void *los_player_create2(lv_obj_t *parent, const char *path, lv_area_t area,
                         bool auto_restart, bool auto_center, int display_idx, int rotation)
{
    (void)parent;
    printf("[STUB] los_player_create2: path=%s area=(%d,%d,%d,%d) restart=%d center=%d display=%d rotation=%d\n",
           path ? path : "NULL", area.x1, area.y1, area.x2, area.y2,
           auto_restart, auto_center, display_idx, rotation);
    return NULL;
}
void los_set_progress_callback(void (*progress_cb)(void *user, int purpose, int64_t ct))
{
    (void)progress_cb;
    printf("[STUB] los_set_progress_callback\n");
}
void los_player_stop(void *player)
{
    (void)player;
    printf("[STUB] los_player_stop\n");
}
void los_player_destroy2(void *player, int display_idx)
{
    (void)player;
    printf("[STUB] los_player_destroy2 display=%d\n", display_idx);
}
void los_videoplayer_quit(int display_id)
{
    printf("[STUB] los_videoplayer_quit display=%d\n", display_id);
}

/* ========================
   PNG 编码/处理
   ======================== */
int qm_png2param_create(const char *path, uint8_t **out, int *w, int *h, lv_color_format_t *cf,
                        bool use_mmz, unsigned int *mmz_phy_buf, bool need_reencode)
{
    char resolved[512] = {0};
    const char *real_path = resolve_resource_path(path, resolved, sizeof(resolved));
    FILE *fp = NULL;
    png_structp png = NULL;
    png_infop info = NULL;
    png_bytep *rows = NULL;
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int color_type;
    int bit_depth;
    int ret = -1;

    (void)use_mmz;
    (void)need_reencode;
    if (mmz_phy_buf) *mmz_phy_buf = 0;
    if (out) *out = NULL;
    if (w) *w = 0;
    if (h) *h = 0;
    if (cf) *cf = LV_COLOR_FORMAT_ARGB8888;

    printf("[STUB] qm_png2param_create: %s -> %s\n",
           path ? path : "NULL", real_path ? real_path : "NOT_FOUND");
    if (!real_path || !out) return -1;

    fp = fopen(real_path, "rb");
    if (!fp) return -1;

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) goto cleanup;
    info = png_create_info_struct(png);
    if (!info) goto cleanup;
    if (setjmp(png_jmpbuf(png))) goto cleanup;

    png_init_io(png, fp);
    png_read_info(png, info);
    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xff, PNG_FILLER_AFTER);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);

    png_read_update_info(png, info);
    *out = malloc((size_t)width * height * 4);
    rows = malloc(sizeof(png_bytep) * height);
    if (!*out || !rows) goto cleanup;

    for (png_uint_32 y = 0; y < height; y++) {
        rows[y] = *out + (size_t)y * width * 4;
    }
    png_read_image(png, rows);

    rgba_to_bgra(*out, (size_t)width * height);
    if (w) *w = (int)width;
    if (h) *h = (int)height;
    ret = 0;

cleanup:
    if (ret != 0 && out && *out) {
        free(*out);
        *out = NULL;
    }
    free(rows);
    if (png || info) png_destroy_read_struct(&png, &info, NULL);
    if (fp) fclose(fp);
    return ret;
}

int qm_png2param_create_v2(const char *path, uint8_t **out, int scale_width, int scale_height, bool need_reencode)
{
    int w = 0;
    int h = 0;
    lv_color_format_t cf = LV_COLOR_FORMAT_ARGB8888;

    (void)scale_width;
    (void)scale_height;
    return qm_png2param_create(path, out, &w, &h, &cf, false, NULL, need_reencode);
}
void *qm_pngencoder(const unsigned char *data, int w, int h, int bpp, int *len)
{
    printf("[STUB] qm_pngencoder\n");
    *len = 0;
    return NULL;
}
int qm_get_png_size(const char *path, int *w, int *h)
{
    char resolved[512] = {0};
    const char *real_path = resolve_resource_path(path, resolved, sizeof(resolved));
    FILE *fp = NULL;
    png_structp png = NULL;
    png_infop info = NULL;
    int ret = -1;

    if (w) *w = 0;
    if (h) *h = 0;

    printf("[SIM] qm_get_png_size: %s -> %s\n",
           path ? path : "NULL", real_path ? real_path : "NOT_FOUND");
    if (!real_path) return -1;

    fp = fopen(real_path, "rb");
    if (!fp) return -1;

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) goto cleanup;
    info = png_create_info_struct(png);
    if (!info) goto cleanup;
    if (setjmp(png_jmpbuf(png))) goto cleanup;

    png_init_io(png, fp);
    png_read_info(png, info);
    if (w) *w = (int)png_get_image_width(png, info);
    if (h) *h = (int)png_get_image_height(png, info);
    ret = 0;

cleanup:
    if (png || info) png_destroy_read_struct(&png, &info, NULL);
    if (fp) fclose(fp);
    return ret;
}

/* ========================
   qme 图片解码 (硬件加速桩)
   ======================== */

/* 硬件缓存查询 — 模拟器无硬件缓存，返回 NULL 让调用者走软件解码路径 */
void qme_img_fetch_by_path(const char *path, int display_id, int *width, int *height, lv_img_dsc_t **out_img_dsc)
{
    (void)path;
    (void)display_id;
    printf("[STUB] qme_img_fetch_by_path: %s display=%d → not cached\n",
           path ? path : "NULL", display_id);
    if (width) *width = 0;
    if (height) *height = 0;
    if (out_img_dsc) *out_img_dsc = NULL;
}

/* JPEG 解码桩 — 用 libjpeg-turbo 模拟硬件解码，生成 BGRA8888 格式（与 qm_png2param_create 一致） */
bool qme_img_get_jpeg(const char *image_path, int disp_width, int disp_height,
                      int display_id, lv_img_dsc_t **out_img_dsc)
{
    char resolved[512] = {0};
    const char *real_path = resolve_resource_path(image_path, resolved, sizeof(resolved));

    (void)disp_width;
    (void)disp_height;
    (void)display_id;

    printf("[STUB] qme_img_get_jpeg: %s -> %s display=%d\n",
           image_path ? image_path : "NULL", real_path ? real_path : "NOT_FOUND", display_id);

    if (!real_path || !out_img_dsc) return false;

    int w = 0, h = 0;
    unsigned char *pixels = decode_jpeg_with_turbo(real_path, &w, &h, false, NULL);
    if (!pixels || w <= 0 || h <= 0) return false;

    /* decode_jpeg_with_turbo 输出 RGBA → 转成 BGRA 与 PNG 路径一致 */
    rgba_to_bgra(pixels, (size_t)w * h);

    lv_img_dsc_t *img_dsc = calloc(1, sizeof(lv_img_dsc_t));
    if (!img_dsc) {
        free(pixels);
        return false;
    }

    img_dsc->header.magic  = LV_IMAGE_HEADER_MAGIC;
    img_dsc->header.cf     = LV_COLOR_FORMAT_ARGB8888;
    img_dsc->header.w      = (uint32_t)w;
    img_dsc->header.h      = (uint32_t)h;
    img_dsc->header.stride = (uint32_t)(w * 4);
    img_dsc->data          = pixels;
    img_dsc->data_size     = (uint32_t)(w * h * 4);

    *out_img_dsc = img_dsc;
    printf("[STUB] qme_img_get_jpeg: success %dx%d %p\n", w, h, (void *)img_dsc);
    return true;
}

/* 将已解码的 ARGB/BGRA 像素缓冲区封装为 lv_img_dsc_t */
void qme_img_encape_bits_to_imgdsc(char *file_path, unsigned char *buffer, unsigned int phy_buf,
                                    int w, int h, int display_id, lv_img_dsc_t **out_img_dsc)
{
    (void)file_path;
    (void)display_id;
    (void)phy_buf;

    printf("[STUB] qme_img_encape_bits_to_imgdsc: %dx%d buf=%p display=%d\n",
           w, h, (void *)buffer, display_id);

    if (!buffer || !out_img_dsc || w <= 0 || h <= 0) return;

    lv_img_dsc_t *img_dsc = calloc(1, sizeof(lv_img_dsc_t));
    if (!img_dsc) return;

    img_dsc->header.magic  = LV_IMAGE_HEADER_MAGIC;
    img_dsc->header.cf     = LV_COLOR_FORMAT_ARGB8888;
    img_dsc->header.w      = (uint32_t)w;
    img_dsc->header.h      = (uint32_t)h;
    img_dsc->header.stride = (uint32_t)(w * 4);
    img_dsc->data          = buffer;
    img_dsc->data_size     = (uint32_t)(w * h * 4);

    *out_img_dsc = img_dsc;
    printf("[STUB] qme_img_encape_bits_to_imgdsc: success %p\n", (void *)img_dsc);
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

/* ========================
   ESL Service / Controller 桩
   (page_esl.c / esl_service.c 需要的符号)
   ======================== */

/** 设备序列号全局变量（serial_number.h 中 extern 声明） */
static char sim_device_serial[128] = SIM_DEFAULT_SERIAL;
char *deviceSerialNumber = sim_device_serial;

void initializeSerialNumber(void)
{
    snprintf(sim_device_serial, sizeof(sim_device_serial), "%s", sim_get_device_serial());
    deviceSerialNumber = sim_device_serial;
    printf("[STUB] initializeSerialNumber -> %s\n", deviceSerialNumber);
}

bool CheckSerialNumber(void)
{
    return deviceSerialNumber && deviceSerialNumber[0] != '\0';
}

/* 注意：get_device_sn/get_local_ip/show_empty_ui/async_show_ui 由真实源码提供，此处不做桩 */

/** Base64 编码 */
int util_base64_encode(const unsigned char *src, size_t src_len, char *dst, size_t dst_len)
{
    printf("[STUB] util_base64_encode\n");
    if (dst_len > 0) dst[0] = '\0';
    return 0;
}

/* ========================
   QUA/MQTT 模块最小实现
   ======================== */
static void sim_ai_init(AiDevInfo *dev)
{
    printf("[STUB] AiModule.init host=%s sn=%s\n",
           dev && dev->hosturl ? dev->hosturl : "NULL",
           dev && dev->devsn ? dev->devsn : "NULL");
}

static void sim_ai_deinit(void)
{
    printf("[STUB] AiModule.deinit\n");
}

static void sim_ai_register_respose(respose callback)
{
    (void)callback;
    printf("[STUB] AiModule.register_respose\n");
}

static void sim_ai_register_conn_respose(connectCallback callback)
{
    (void)callback;
    printf("[STUB] AiModule.register_conn_respose\n");
}

static int sim_ai_send_cmd(AiMessage *command)
{
    printf("[STUB] AiModule.send_cmd val=%d\n", command ? command->val : -1);
    return 0;
}

static int sim_ai_input_content(AiMessage *content)
{
    printf("[STUB] AiModule.input_content val=%d\n", content ? content->val : -1);
    return 0;
}

AiModule *create_module(AiModuleType type)
{
    static AiModule module = {
        .init = sim_ai_init,
        .deinit = sim_ai_deinit,
        .register_respose = sim_ai_register_respose,
        .register_conn_respose = sim_ai_register_conn_respose,
        .send_cmd = sim_ai_send_cmd,
        .input_content = sim_ai_input_content,
    };
    printf("[STUB] create_module type=%d\n", type);
    return &module;
}

/* ========================
   字体全局变量
   (qm_product_comp.c / qm_2_label.c / qm_3_label.c 中的 extern 声明)
   ★ 必须用 qua_xos_ft_init_with_size_path 实际加载，不能设为 NULL
     否则 lv_obj_set_style_text_font(label, ft_font_ttf_30, 0) 会崩溃
   ======================== */
lv_font_t *ft_font_ttf_40 = NULL;
lv_font_t *ft_font_ttf_30 = NULL;
lv_font_t *ft_font_ttf_20 = NULL;

/**
 * @brief ★ 初始化模拟器字体 — 必须在 create_mock_ui() 之前调用
 *
 * 问题背景：
 *   qm_3_label.c / qm_product_comp.c 中使用 extern 声明直接引用
 *   ft_font_ttf_20/30/40 全局变量，如果为 NULL 会导致 LVGL 段错误。
 *
 * 加载策略：
 *   1. qua_xos_ft_init_with_size_path 内部会尝试 conf.h 中的路径
 *      (BUILD_SIMULATOR 下为 "H:./out/simulator/xos/res/ttf/2312_v9.ttf")
 *   2. resolve_font_path() 会去掉 "H:" 前缀，并通过 XOS_ROOT 环境变量查找
 *   3. 如果都找不到，回退到系统的 DroidSansFallbackFull.ttf
 */
void sim_init_fonts(void)
{
    printf("[FONT] Initializing simulator fonts...\n");

    qua_xos_ft_init_with_size_path(&ft_font_ttf_20, 20, NULL, 0);
    qua_xos_ft_init_with_size_path(&ft_font_ttf_30, 30, NULL, 0);
    qua_xos_ft_init_with_size_path(&ft_font_ttf_40, 40, NULL, 0);

    printf("[FONT] ft_font_ttf_20 = %p\n", (void*)ft_font_ttf_20);
    printf("[FONT] ft_font_ttf_30 = %p\n", (void*)ft_font_ttf_30);
    printf("[FONT] ft_font_ttf_40 = %p\n", (void*)ft_font_ttf_40);

    if (!ft_font_ttf_20 || !ft_font_ttf_30 || !ft_font_ttf_40) {
        fprintf(stderr, "[ERROR] Some fonts failed to load!\n");
    }
}

/* ========================
   qm_image_text / qm_lv_obj 桩
   ======================== */
lv_obj_t *qm_image_text_init(lv_obj_t *parent, int w, int h,
    const char *src_nor, const char *src_prs,
    const char *text, bool is_p1)
{
    printf("[STUB] qm_image_text_init: text=%s\n", text ? text : "NULL");
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, w, h);
    lv_obj_t *label = lv_label_create(obj);
    if (text) lv_label_set_text(label, text);
    return obj;
}

lv_obj_t *qm_lv_obj_create(lv_obj_t *parent)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_style_all(obj);
    return obj;
}

/* ========================
   字符串资源桩
   ======================== */
const char *GetTextRes(int id)
{
    static char buf[64];
    snprintf(buf, sizeof(buf), "STR_%d", id);
    printf("[STUB] GetTextRes(%d) = %s\n", id, buf);
    return buf;
}
