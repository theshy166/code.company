#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "display/qua_mm_display.h"
#include "system/qua_mm_system.h"
#include "utils/qua_display_parser.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1280
#define BYTES_PER_PIXEL 4

static volatile sig_atomic_t g_running = 1;

typedef struct {
    const char *display_id;
    QUA_S32 vo_num;
    QUA_S32 fb_num;
    QUA_CONST_CHAR *vo_id;
    QUA_CONST_CHAR *fb_id;
    qua_mm_vo_device_t *vo_device;
    qua_mm_fb_device_t *fb_device;
    QUA_CHAR *vaddr;
    qua_fb_fix_screeninfo_t fix_info;
    qua_fb_var_screeninfo_t var_info;
    int ready;
} render_target_t;

static void on_signal(int sig)
{
    (void)sig;
    g_running = 0;
}

static int has_suffix(const char *text, const char *suffix)
{
    size_t text_len = strlen(text);
    size_t suffix_len = strlen(suffix);
    return text_len >= suffix_len && strcmp(text + text_len - suffix_len, suffix) == 0;
}

static int compare_names(const void *a, const void *b)
{
    const char * const *sa = (const char * const *)a;
    const char * const *sb = (const char * const *)b;
    return strcmp(*sa, *sb);
}

static char **load_frame_paths(const char *dir_path, int *out_count)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir frame dir");
        return NULL;
    }

    int capacity = 16;
    int count = 0;
    char **paths = calloc((size_t)capacity, sizeof(char *));
    if (paths == NULL) {
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "frame_", 6) != 0 || !has_suffix(entry->d_name, ".bgra")) {
            continue;
        }

        if (count == capacity) {
            capacity *= 2;
            char **new_paths = realloc(paths, (size_t)capacity * sizeof(char *));
            if (new_paths == NULL) {
                break;
            }
            paths = new_paths;
        }

        size_t len = strlen(dir_path) + 1 + strlen(entry->d_name) + 1;
        paths[count] = malloc(len);
        if (paths[count] == NULL) {
            break;
        }
        snprintf(paths[count], len, "%s/%s", dir_path, entry->d_name);
        count++;
    }
    closedir(dir);

    qsort(paths, (size_t)count, sizeof(char *), compare_names);
    *out_count = count;
    return paths;
}

static int read_full_file(const char *path, unsigned char *buf, size_t size)
{
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("fopen frame");
        return -1;
    }

    size_t n = fread(buf, 1, size, fp);
    fclose(fp);
    return n == size ? 0 : -1;
}

static void close_render_target(render_target_t *target)
{
    if (target->fb_device != NULL) {
        if (target->vaddr != NULL) {
            target->fb_device->unmap(target->fb_device, target->vaddr, target->fix_info.smem_len);
            target->vaddr = NULL;
        }
        target->fb_device->show(target->fb_device, QUA_FALSE);
        target->fb_device->parent.close((qua_mm_device_t *)target->fb_device);
        target->fb_device = NULL;
    }

    if (target->vo_device != NULL) {
        target->vo_device->parent.close((qua_mm_device_t *)target->vo_device);
        target->vo_device = NULL;
    }
    target->ready = 0;
}

static int init_render_target(const qua_mm_module_t *display_module,
                              render_target_t *target,
                              const char *display_id)
{
    memset(target, 0, sizeof(*target));
    target->display_id = display_id;

    QUA_S32 ret = qua_parse_display_id(display_id, &target->vo_id, &target->vo_num,
                                       &target->fb_id, &target->fb_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s parse display id failed: %d\n", display_id, ret);
        return -1;
    }

    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, target->vo_id, target->vo_num, &mm_device);
    if (ret == QUA_SUCCESS && mm_device != NULL) {
        target->vo_device = (qua_mm_vo_device_t *)mm_device;

        qua_vo_pub_attr_t dev_attr;
        memset(&dev_attr, 0, sizeof(dev_attr));
        dev_attr.bg_color = 0;
        dev_attr.intf_type = QUA_VO_INTF_LCD;
        dev_attr.intf_sync = QUA_VO_OUTPUT_800x1280_60;

        ret = target->vo_device->set_public_attr(target->vo_num, &dev_attr);
        if (ret != QUA_SUCCESS) {
            fprintf(stderr, "%s set_public_attr returned %d, continue\n", display_id, ret);
        }

        ret = target->vo_device->enable(target->vo_num);
        if (ret != QUA_SUCCESS) {
            fprintf(stderr, "%s enable vo returned %d, continue\n", display_id, ret);
        }
    } else {
        fprintf(stderr, "%s open vo failed: %d, continue with fb\n", display_id, ret);
    }

    mm_device = NULL;
    ret = display_module->open_device(display_module, target->fb_id, target->fb_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open fb failed: %d\n", display_id, ret);
        close_render_target(target);
        return -1;
    }

    target->fb_device = (qua_mm_fb_device_t *)mm_device;
    target->fb_device->show(target->fb_device, QUA_FALSE);
    target->fb_device->compress(target->fb_device, QUA_TRUE);

    qua_point_t pos = {0, 0};
    target->fb_device->put_origin(target->fb_device, &pos);

    qua_size_t size = {SCREEN_WIDTH, SCREEN_HEIGHT};
    target->fb_device->put_size(target->fb_device, &size);

    if (target->fb_device->get_var_info(target->fb_device, &target->var_info) != QUA_SUCCESS) {
        fprintf(stderr, "%s get var failed\n", display_id);
        close_render_target(target);
        return -1;
    }

    qua_fb_bitfield_t stA = {24, 8, 0};
    qua_fb_bitfield_t stR = {16, 8, 0};
    qua_fb_bitfield_t stG = {8, 8, 0};
    qua_fb_bitfield_t stB = {0, 8, 0};
    target->var_info.xres_virtual = target->var_info.xres = SCREEN_WIDTH;
    target->var_info.yres_virtual = target->var_info.yres = SCREEN_HEIGHT;
    target->var_info.activate = QUA_FB_ACTIVATE_NOW;
    target->var_info.bits_per_pixel = 32;
    target->var_info.xoffset = 0;
    target->var_info.yoffset = 0;
    target->var_info.transp = stA;
    target->var_info.red = stR;
    target->var_info.green = stG;
    target->var_info.blue = stB;

    if (target->fb_device->put_var_info(target->fb_device, &target->var_info) != QUA_SUCCESS) {
        fprintf(stderr, "%s put var failed\n", display_id);
        close_render_target(target);
        return -1;
    }

    if (target->fb_device->get_fix_info(target->fb_device, &target->fix_info) != QUA_SUCCESS) {
        fprintf(stderr, "%s get fix failed\n", display_id);
        close_render_target(target);
        return -1;
    }

    qua_fb_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.alpha_enable = QUA_TRUE;
    alpha.alpha_channel = QUA_FALSE;
    alpha.alpha0 = 0;
    alpha.alpha1 = 255;
    alpha.global_alpha = 255;
    target->fb_device->put_alpha(target->fb_device, &alpha);

    target->vaddr = target->fb_device->map(target->fb_device, target->fix_info.smem_len, 0);
    if (target->vaddr == QUA_NULL) {
        fprintf(stderr, "%s fb map failed\n", display_id);
        close_render_target(target);
        return -1;
    }

    memset(target->vaddr, 0, target->fix_info.smem_len);
    if (target->fb_device->show(target->fb_device, QUA_TRUE) != QUA_SUCCESS) {
        fprintf(stderr, "%s fb show failed\n", display_id);
        close_render_target(target);
        return -1;
    }

    target->fb_device->render(target->fb_device, &target->var_info);
    target->ready = 1;
    fprintf(stderr, "%s render target ready, fb_len=%u line=%u\n",
            display_id, (unsigned)target->fix_info.smem_len,
            (unsigned)target->fix_info.line_length);
    return 0;
}

static void render_frame(render_target_t *target,
                         const unsigned char *frame,
                         int frame_width,
                         int frame_height)
{
    if (!target->ready || target->fb_device == NULL || target->vaddr == NULL) {
        return;
    }

    int copy_width = frame_width > SCREEN_WIDTH ? SCREEN_WIDTH : frame_width;
    int copy_height = frame_height > SCREEN_HEIGHT ? SCREEN_HEIGHT : frame_height;
    int x_offset = (SCREEN_WIDTH - copy_width) / 2;
    int y_offset = (SCREEN_HEIGHT - copy_height) / 2;
    size_t dst_stride = target->fix_info.line_length != 0 ?
                        target->fix_info.line_length :
                        (size_t)SCREEN_WIDTH * BYTES_PER_PIXEL;
    size_t src_stride = (size_t)frame_width * BYTES_PER_PIXEL;
    size_t row_bytes = (size_t)copy_width * BYTES_PER_PIXEL;

    if (dst_stride * SCREEN_HEIGHT > target->fix_info.smem_len) {
        dst_stride = (size_t)SCREEN_WIDTH * BYTES_PER_PIXEL;
    }

    unsigned char *dst = (unsigned char *)target->vaddr;
    const unsigned char *src = frame;

    for (int y = 0; y < copy_height; y++) {
        unsigned char *dst_row = dst + (size_t)(y + y_offset) * dst_stride +
                                 (size_t)x_offset * BYTES_PER_PIXEL;
        const unsigned char *src_row = src + (size_t)y * src_stride;
        memcpy(dst_row, src_row, row_bytes);
    }

    target->fb_device->render(target->fb_device, &target->var_info);
}

static void clear_targets(render_target_t *targets, int target_count)
{
    for (int i = 0; i < target_count; i++) {
        if (targets[i].ready && targets[i].vaddr != NULL) {
            memset(targets[i].vaddr, 0, targets[i].fix_info.smem_len);
            targets[i].fb_device->render(targets[i].fb_device, &targets[i].var_info);
        }
    }
}

int main(int argc, char **argv)
{
    const char *frame_dir = argc > 1 ? argv[1] : "/data/res/img/video_frames_bgra_800x640";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_video_overlay";
    int delay_us = argc > 3 ? atoi(argv[3]) : 66666;
    int frame_width = argc > 4 ? atoi(argv[4]) : 800;
    int frame_height = argc > 5 ? atoi(argv[5]) : 640;
    const char *display_mode = argc > 6 ? argv[6] : "both";

    if (frame_width <= 0 || frame_width > SCREEN_WIDTH ||
        frame_height <= 0 || frame_height > SCREEN_HEIGHT) {
        fprintf(stderr, "bad frame size: %dx%d\n", frame_width, frame_height);
        return 1;
    }

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);

    int frame_count = 0;
    char **frames = load_frame_paths(frame_dir, &frame_count);
    if (frames == NULL || frame_count <= 0) {
        fprintf(stderr, "no bgra frames in %s\n", frame_dir);
        return 1;
    }

    size_t frame_size = (size_t)frame_width * frame_height * BYTES_PER_PIXEL;
    unsigned char *frame_buf = malloc(frame_size);
    if (frame_buf == NULL) {
        fprintf(stderr, "malloc failed: %zu\n", frame_size);
        return 1;
    }

    char platform[32] = {0};
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");
        return 1;
    }

    qua_mm_system_t *system = NULL;
    if (qua_mm_init(QUA_FALSE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init secondary failed\n");
        return 1;
    }

    qua_init_display_parser();

    const qua_mm_module_t *display_module = NULL;
    if (qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module) != QUA_SUCCESS || display_module == NULL) {
        fprintf(stderr, "load display module failed\n");
        return 1;
    }

    render_target_t targets[2];
    int target_count = 0;
    if (strcmp(display_mode, "1") != 0 && strcmp(display_mode, "display1") != 0) {
        if (init_render_target(display_module, &targets[target_count], "id:display0") == 0) {
            target_count++;
        }
    }
    if (strcmp(display_mode, "0") != 0 && strcmp(display_mode, "display0") != 0) {
        if (init_render_target(display_module, &targets[target_count], "id:display1") == 0) {
            target_count++;
        }
    }

    if (target_count == 0) {
        fprintf(stderr, "no render target ready\n");
        return 1;
    }

    clear_targets(targets, target_count);
    fprintf(stderr,
            "video overlay start: frames=%d targets=%d size=%dx%d delay_us=%d mode=%s\n",
            frame_count, target_count, frame_width, frame_height, delay_us, display_mode);

    while (g_running && access(marker, F_OK) == 0) {
        for (int i = 0; i < frame_count && g_running && access(marker, F_OK) == 0; i++) {
            if (read_full_file(frames[i], frame_buf, frame_size) != 0) {
                fprintf(stderr, "skip bad frame: %s expected=%zu\n", frames[i], frame_size);
                continue;
            }

            for (int t = 0; t < target_count; t++) {
                render_frame(&targets[t], frame_buf, frame_width, frame_height);
            }

            if (delay_us > 0) {
                usleep((useconds_t)delay_us);
            }
        }
    }

    for (int t = 0; t < target_count; t++) {
        close_render_target(&targets[t]);
    }
    for (int i = 0; i < frame_count; i++) {
        free(frames[i]);
    }
    free(frames);
    free(frame_buf);
    fprintf(stderr, "video overlay stop\n");
    return 0;
}
