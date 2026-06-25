#ifdef CONFIG_XOS_APP_ESL2

#include "page_overlay.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "lvgl/lvgl.h"
#include "page_esl_image.h"

#define PAGE_OVERLAY_WIDTH 800
#define PAGE_OVERLAY_HEIGHT 1280

#define PAGE_OVERLAY_DEMO_VIDEO_NAME "13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4"
#define PAGE_OVERLAY_DEMO_IMAGE_NAME "frame_001.jpg"
#define PAGE_OVERLAY_DEMO_DEFAULT_SCREEN 2
#define PAGE_OVERLAY_FRAME_PERIOD_MS 200
#define PAGE_OVERLAY_MAX_FRAMES 300

#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
#define PAGE_OVERLAY_DEMO_VIDEO_PATH \
    "/home/luoxin/work/X-AIOS-ESL/core/apps/esl2/view/" PAGE_OVERLAY_DEMO_VIDEO_NAME
#define PAGE_OVERLAY_DEMO_IMAGE_PATH \
    "/home/luoxin/work/X-AIOS-ESL/core/apps/esl2/view/video_frames/" PAGE_OVERLAY_DEMO_IMAGE_NAME
#define PAGE_OVERLAY_FRAME_PLAYER_DIR \
    "/home/luoxin/work/X-AIOS-ESL/core/apps/esl2/view/video_frames"
#define PAGE_OVERLAY_FRAME_FS_DIR PAGE_OVERLAY_FRAME_PLAYER_DIR
#define PAGE_OVERLAY_ENABLE_MARKER ""          /* 模拟器不使用 marker 文件 */
#define PAGE_OVERLAY_ENABLE_IMAGE_MARKER ""     /* 模拟器不使用 marker 文件 */
/* DEBUG: 0=full, 1=player no extra hole, 2=no player root only, 3=disabled */
#define PAGE_OVERLAY_DEBUG_LEVEL 0
#else
#define PAGE_OVERLAY_DEMO_VIDEO_PATH "/data/res/img/" PAGE_OVERLAY_DEMO_VIDEO_NAME
#define PAGE_OVERLAY_DEMO_IMAGE_PATH "/data/res/img/video_frames/" PAGE_OVERLAY_DEMO_IMAGE_NAME
#define PAGE_OVERLAY_FRAME_PLAYER_DIR "H:/data/res/img/video_frames"
#define PAGE_OVERLAY_FRAME_FS_DIR "/data/res/img/video_frames"
#define PAGE_OVERLAY_ENABLE_MARKER "/data/.enable_video_overlay"
#define PAGE_OVERLAY_ENABLE_IMAGE_MARKER "/data/.enable_image_overlay"
/* DEBUG: 0=full, 1=player no extra hole, 2=no player root only, 3=disabled */
#define PAGE_OVERLAY_DEBUG_LEVEL 0
#endif

void *los_player_create2(lv_obj_t *parent, const char *path, lv_area_t area,
                         bool auto_restart, bool auto_center, int display_idx, int rotation);
void los_player_stop(void *player);
void los_player_destroy2(void *player, int display_idx);

typedef struct {
    lv_obj_t *root;
    void *player;
    lv_obj_t *frame_img;
    lv_timer_t *frame_timer;
    int frame_index;
    int frame_count;
    int screen_index;
    char video_path[256];
    char image_path[512];
} page_overlay_state_t;

static page_overlay_state_t s_overlay[2];

static void page_overlay_log(const char *fmt, ...)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long long ms = (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    va_list ap;

    va_start(ap, fmt);
    printf("[%6lld] ", ms);
    vprintf(fmt, ap);
    va_end(ap);

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    FILE *fp = fopen("/data/overlay_debug.log", "a");
    if (fp != NULL) {
        fprintf(fp, "[%6lld] ", ms);
        va_start(ap, fmt);
        vfprintf(fp, fmt, ap);
        va_end(ap);
        fclose(fp);
    }
#endif
}

static lv_display_t *page_overlay_get_display(int screen_index)
{
    extern lv_display_t *lv_display_get_1st_scr(void);
    extern lv_display_t *lv_display_get_2nd_scr(void);

    return screen_index == 2 ? lv_display_get_2nd_scr() : lv_display_get_1st_scr();
}

static int page_overlay_get_state_index(int screen_index)
{
    return screen_index == 2 ? 1 : 0;
}

static int page_overlay_get_display_id(int screen_index)
{
    return screen_index == 2 ? 1 : 0;
}

static bool page_overlay_is_remote_path(const char *path)
{
    return path != NULL &&
           (strncmp(path, "http://", 7) == 0 || strncmp(path, "https://", 8) == 0);
}

static const char *page_overlay_get_fs_path(const char *path)
{
    if (path != NULL && strncmp(path, "H:", 2) == 0) {
        return path + 2;
    }
    return path;
}

static void page_overlay_move_to_front(lv_obj_t *root)
{
    if (root == NULL || !lv_obj_is_valid(root)) {
        return;
    }

    lv_obj_t *parent = lv_obj_get_parent(root);
    if (parent == NULL) {
        return;
    }

    uint32_t child_count = lv_obj_get_child_count(parent);
    if (child_count > 0) {
        lv_obj_move_to_index(root, (int32_t)child_count - 1);
    }
    lv_obj_clear_flag(root, LV_OBJ_FLAG_HIDDEN);
    lv_obj_invalidate(root);
}

static void page_overlay_apply_full_video_hole(int display_id)
{
#if PAGE_OVERLAY_DEBUG_LEVEL >= 1
    (void)display_id;
    return;
#endif
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    extern void fbdev_set_hole(int x, int y, int width, int height);
    extern void fbdev2_set_hole(int x, int y, int width, int height);

    int hole_width = (PAGE_OVERLAY_WIDTH - 1) / 8 * 8;
    int hole_height = (PAGE_OVERLAY_HEIGHT - 1) / 8 * 8;

    if (display_id == 0) {
        fbdev_set_hole(0, 0, hole_width, hole_height);
    } else {
        fbdev2_set_hole(0, 0, hole_width, hole_height);
    }
#else
    (void)display_id;
#endif
}

static int page_overlay_get_marker_screen(const char *marker_path)
{
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    int screen = PAGE_OVERLAY_DEMO_DEFAULT_SCREEN;
    FILE *fp = fopen(marker_path, "r");
    if (fp != NULL) {
        int marker_screen = 0;
        if (fscanf(fp, "%d", &marker_screen) == 1 &&
            (marker_screen == 1 || marker_screen == 2)) {
            screen = marker_screen;
        }
        fclose(fp);
    }
    return screen;
#else
    return PAGE_OVERLAY_DEMO_DEFAULT_SCREEN;
#endif
}

static int page_overlay_get_demo_screen(void)
{
    return page_overlay_get_marker_screen(PAGE_OVERLAY_ENABLE_MARKER);
}

static int page_overlay_get_image_screen(void)
{
    return page_overlay_get_marker_screen(PAGE_OVERLAY_ENABLE_IMAGE_MARKER);
}

static void page_overlay_on_root_deleted(lv_event_t *e)
{
    lv_obj_t *root = lv_event_get_current_target_obj(e);
    int screen_index = (int)(intptr_t)lv_event_get_user_data(e);
    int state_index = page_overlay_get_state_index(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];

    page_overlay_log("[OVERLAY] LVGL deleted root=%p screen=%d player=%p → marking stale\n",
                     (void *)root, screen_index, state->player);
    state->root = NULL;
    state->frame_img = NULL;
}

static lv_obj_t *page_overlay_create_root(int screen_index)
{
    lv_display_t *disp = page_overlay_get_display(screen_index);
    if (disp == NULL) {
        return NULL;
    }

    /* Try active screen first (definitely rendered), then sys_layer, then top_layer */
    lv_obj_t *parent = lv_disp_get_scr_act(disp);
    if (parent == NULL) {
        parent = lv_display_get_layer_sys(disp);
    }
    if (parent == NULL) {
        parent = lv_display_get_layer_top(disp);
    }
    if (parent == NULL) {
        return NULL;
    }

    lv_obj_t *root = lv_obj_create(parent);
    lv_obj_remove_style_all(root);
    lv_obj_set_pos(root, 0, 0);
    lv_obj_set_size(root, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT);
    lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(root, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(root, lv_color_black(), 0);
    lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_foreground(root);

    lv_obj_add_event_cb(root, page_overlay_on_root_deleted, LV_EVENT_DELETE,
                        (void *)(intptr_t)screen_index);

    page_overlay_log("[OVERLAY] root=%p parent=%p screen=%d\n",
                     (void *)root, (void *)parent, screen_index);

    return root;
}

static void page_overlay_get_frame_path(int frame_index, char *path_buf, size_t path_buf_size)
{
    snprintf(path_buf, path_buf_size, "%s/frame_%03d.jpg",
             PAGE_OVERLAY_FRAME_PLAYER_DIR, frame_index);
}

static int page_overlay_count_frames(void)
{
    int count = 0;
    char fs_path[256] = {0};

    for (int i = 1; i <= PAGE_OVERLAY_MAX_FRAMES; ++i) {
        snprintf(fs_path, sizeof(fs_path), "%s/frame_%03d.jpg", PAGE_OVERLAY_FRAME_FS_DIR, i);
        if (access(fs_path, R_OK) != 0) {
            break;
        }
        count = i;
    }

    return count;
}

static void page_overlay_set_frame(page_overlay_state_t *state)
{
    char frame_path[256] = {0};
    int display_id = page_overlay_get_display_id(state->screen_index);

    if (state->frame_img == NULL || !lv_obj_is_valid(state->frame_img)) {
        return;
    }

    page_overlay_get_frame_path(state->frame_index, frame_path, sizeof(frame_path));
    _set_image_src(state->frame_img, frame_path, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT, display_id);
}

static void page_overlay_frame_timer_cb(lv_timer_t *timer)
{
    page_overlay_state_t *state = (page_overlay_state_t *)lv_timer_get_user_data(timer);
    if (state == NULL || state->frame_count <= 0) {
        return;
    }

    state->frame_index++;
    if (state->frame_index > state->frame_count) {
        state->frame_index = 1;
    }

    page_overlay_set_frame(state);
}

static void page_overlay_pan_disp_timer_cb(lv_timer_t *timer)
{
    extern void fbdev2_pan_disp(void);
    fbdev2_pan_disp();
    page_overlay_log("[OVERLAY] pan_disp triggered\n");
    lv_timer_del(timer);
}

static const char *page_overlay_prepare_image_path(const char *image_path,
                                                   char *path_buf,
                                                   size_t path_buf_size)
{
    if (image_path == NULL || image_path[0] == '\0') {
        return NULL;
    }

    if (strncmp(image_path, "H:", 2) == 0 ||
        strncmp(image_path, "http://", 7) == 0 ||
        strncmp(image_path, "https://", 8) == 0) {
        return image_path;
    }

    if (image_path[0] == '/') {
        snprintf(path_buf, path_buf_size, "H:%s", image_path);
        return path_buf;
    }

    return image_path;
}

void page_overlay_hide(int screen_index)
{
    int state_index = page_overlay_get_state_index(screen_index);
    int display_id = page_overlay_get_display_id(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];

    if (state->frame_timer != NULL) {
        lv_timer_del(state->frame_timer);
        state->frame_timer = NULL;
    }
    state->frame_img = NULL;
    state->frame_index = 0;
    state->frame_count = 0;
    state->video_path[0] = '\0';
    state->image_path[0] = '\0';

    if (state->player != NULL) {
        los_player_stop(state->player);
        los_player_destroy2(state->player, display_id);
        state->player = NULL;
    }

    if (state->root != NULL) {
        lv_obj_delete(state->root);
        state->root = NULL;
    }
}

void page_overlay_raise(int screen_index)
{
    int state_index = page_overlay_get_state_index(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];

    if (state->root != NULL && !lv_obj_is_valid(state->root)) {
        state->root = NULL;
        return;
    }

    page_overlay_move_to_front(state->root);
}

int page_overlay_show_video(int screen_index, const char *video_path)
{
    int state_index = page_overlay_get_state_index(screen_index);
    int display_id = page_overlay_get_display_id(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];

    page_overlay_log("[OVERLAY] show_video entry: screen=%d display=%d path=%s\n",
                     screen_index, display_id, video_path ? video_path : "NULL");

    bool has_video = (video_path != NULL && video_path[0] != '\0');
    if (has_video && !page_overlay_is_remote_path(video_path)) {
        const char *fs_path = page_overlay_get_fs_path(video_path);
        if (access(fs_path, R_OK) != 0) {
            page_overlay_log("[OVERLAY] video file not found: player=%s fs=%s\n", video_path, fs_path);
            has_video = false;
        }
    }

    if (!has_video) {
        page_overlay_log("[OVERLAY] no video to play\n");
        page_overlay_hide(screen_index);
        return -1;
    }

    if (state->player != NULL &&
        state->screen_index == screen_index &&
        strncmp(state->video_path, video_path, sizeof(state->video_path) - 1) == 0) {
        if (state->root != NULL && lv_obj_is_valid(state->root)) {
            page_overlay_log("[OVERLAY] video already active, reuse: screen=%d player=%p\n",
                             screen_index, state->player);
            // hole already set by player creation, skip to avoid VGS conflict after playback complete
            page_overlay_raise(screen_index);
            return 0;
        }
        page_overlay_log("[OVERLAY] root invalid but player alive (%p), full destroy+recreate: screen=%d\n",
                         state->player, screen_index);
        /* player may have completed - destroy and fall through to full recreate */
        page_overlay_hide(screen_index);
        state = &s_overlay[state_index];  /* refresh after hide resets state */
    }

    page_overlay_hide(screen_index);

    state = &s_overlay[state_index];
    state->screen_index = screen_index;
    snprintf(state->video_path, sizeof(state->video_path), "%s", video_path);
    state->root = page_overlay_create_root(screen_index);
    if (state->root == NULL) {
        page_overlay_log("[OVERLAY] create_root failed for screen %d\n", screen_index);
        state->video_path[0] = '\0';
        return -1;
    }
    page_overlay_log("[OVERLAY] root created at %p on screen %d\n", (void *)state->root, screen_index);

#if PAGE_OVERLAY_DEBUG_LEVEL >= 2
    page_overlay_log("[OVERLAY] player creation skipped (debug_level=%d)\n", PAGE_OVERLAY_DEBUG_LEVEL);
    page_overlay_raise(screen_index);
    return 0;
#endif

    lv_area_t area = {
        .x1 = 0,
        .y1 = 0,
        .x2 = PAGE_OVERLAY_WIDTH - 1,
        .y2 = PAGE_OVERLAY_HEIGHT - 1,
    };
    page_overlay_log("[OVERLAY] creating player: display=%d path=%s\n", display_id, video_path);
    state->player = los_player_create2(state->root, video_path, area,
                                       false, false, display_id, 0);
    if (state->player == NULL) {
        page_overlay_log("[OVERLAY] video player returned NULL, remove overlay\n");
        page_overlay_hide(screen_index);
        return -1;
    }
    page_overlay_log("[OVERLAY] video player created successfully: %p\n", state->player);

    page_overlay_raise(screen_index);
    return 0;
}

int page_overlay_show_image(int screen_index, const char *image_path)
{
    int state_index = page_overlay_get_state_index(screen_index);
    int display_id = page_overlay_get_display_id(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];
    char lvgl_path[512] = {0};
    const char *src = NULL;
    const char *fs_path = NULL;

    if (image_path == NULL || image_path[0] == '\0') {
        page_overlay_log("[OVERLAY] no image path\n");
        return -1;
    }

    if (!page_overlay_is_remote_path(image_path)) {
        fs_path = page_overlay_get_fs_path(image_path);
        if (access(fs_path, R_OK) != 0) {
            page_overlay_log("[OVERLAY] image file not found: player=%s fs=%s\n",
                             image_path, fs_path);
            return -1;
        }
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        extern void qm_rewrite_jpg_to_420_if_needed(const char *file_path);
        qm_rewrite_jpg_to_420_if_needed(fs_path);
#endif
    }

    /* 清除之前视频 overlay 留下的 framebuffer hole，否则图片会被挡住 */
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    {
        extern void fbdev_set_hole(int x, int y, int width, int height);
        extern void fbdev2_set_hole(int x, int y, int width, int height);
        if (display_id == 0) {
            fbdev_set_hole(0, 0, 0, 0);
        } else {
            fbdev2_set_hole(0, 0, 0, 0);
        }
    }
#endif

    src = page_overlay_prepare_image_path(image_path, lvgl_path, sizeof(lvgl_path));
    if (src == NULL) {
        page_overlay_log("[OVERLAY] image path prepare failed: %s\n", image_path);
        return -1;
    }

    if (state->player == NULL && state->screen_index == screen_index &&
        strncmp(state->image_path, src, sizeof(state->image_path) - 1) == 0) {
        if (state->root != NULL && lv_obj_is_valid(state->root)) {
            page_overlay_log("[OVERLAY] image already active, reuse: screen=%d\n", screen_index);
            page_overlay_raise(screen_index);
            return 0;
        }
        page_overlay_log("[OVERLAY] image root stale, full recreate: screen=%d\n", screen_index);
    }

    page_overlay_hide(screen_index);

    state = &s_overlay[state_index];
    state->screen_index = screen_index;
    snprintf(state->image_path, sizeof(state->image_path), "%s", src);

    state->root = page_overlay_create_root(screen_index);
    if (state->root == NULL) {
        page_overlay_log("[OVERLAY] image root create failed: screen=%d\n", screen_index);
        state->image_path[0] = '\0';
        return -1;
    }

    /* DEBUG: solid red bg to verify root renders */
    lv_obj_set_style_bg_opa(state->root, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(state->root, lv_color_hex(0xFF0000), 0);

    lv_obj_t *img = lv_img_create(state->root);
    lv_obj_set_pos(img, 0, 0);
    lv_obj_set_size(img, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT);

    page_overlay_log("[OVERLAY] show image display=%d path=%s\n", display_id, src);
    _set_image_src(img, src, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT, display_id);

    page_overlay_raise(screen_index);
    lv_obj_invalidate(state->root);

    /* one-shot timer to force pan_disp on display 2 (flush_is_last never true) */
    if (display_id == 1) {
        lv_timer_create(page_overlay_pan_disp_timer_cb, 200, NULL);
    }

    page_overlay_log("[OVERLAY] image set complete: screen=%d root=%p\n",
                     screen_index, (void *)state->root);

    return 0;
}

static int page_overlay_show_frame_sequence(int screen_index)
{
    int state_index = page_overlay_get_state_index(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];

    page_overlay_log("[OVERLAY] show_frame_sequence entry: screen=%d\n", screen_index);

    state->screen_index = screen_index;
    state->frame_count = page_overlay_count_frames();
    state->frame_index = 1;
    if (state->frame_count <= 0) {
        page_overlay_log("[OVERLAY] no jpeg frames in %s\n", PAGE_OVERLAY_FRAME_FS_DIR);
        page_overlay_hide(screen_index);
        return -1;
    }

    page_overlay_hide(screen_index);

    state->root = page_overlay_create_root(screen_index);
    if (state->root == NULL) {
        page_overlay_log("[OVERLAY] frame root create failed: screen=%d\n", screen_index);
        return -1;
    }

    state->frame_img = lv_img_create(state->root);
    lv_obj_set_pos(state->frame_img, 0, 0);
    lv_obj_set_size(state->frame_img, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT);
    page_overlay_set_frame(state);

    state->frame_timer = lv_timer_create(page_overlay_frame_timer_cb,
                                         PAGE_OVERLAY_FRAME_PERIOD_MS, state);
    lv_timer_set_repeat_count(state->frame_timer, -1);
    page_overlay_raise(screen_index);

    page_overlay_log("[OVERLAY] frame sequence started: screen=%d frames=%d period=%dms\n",
                     screen_index, state->frame_count, PAGE_OVERLAY_FRAME_PERIOD_MS);
    return 0;
}

void page_overlay_show_demo_video_on_screen(int screen_index)
{
#if PAGE_OVERLAY_DEBUG_LEVEL >= 3
    page_overlay_log("[OVERLAY] demo overlay disabled (debug_level=%d): screen=%d\n",
                     PAGE_OVERLAY_DEBUG_LEVEL, screen_index);
    page_overlay_hide(screen_index);
    return;
#endif
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    /* 图片模式优先：/data/.enable_image_overlay 存在则显示图片 */
    if (access(PAGE_OVERLAY_ENABLE_IMAGE_MARKER, F_OK) == 0) {
        page_overlay_show_demo_image_on_screen(screen_index);
        return;
    }
    /* 视频模式：/data/.enable_video_overlay 存在则显示视频 */
    if (access(PAGE_OVERLAY_ENABLE_MARKER, F_OK) != 0) {
        page_overlay_hide(screen_index);
        page_overlay_log("[OVERLAY] demo overlay disabled: screen=%d marker=%s\n",
                         screen_index, PAGE_OVERLAY_ENABLE_MARKER);
        return;
    }
#endif

    int demo_screen = page_overlay_get_demo_screen();
    if (screen_index != demo_screen) {
        page_overlay_hide(screen_index);
        page_overlay_log("[OVERLAY] demo overlay skipped: screen=%d enabled_screen=%d\n",
                         screen_index, demo_screen);
        return;
    }

    if (page_overlay_show_video(screen_index, PAGE_OVERLAY_DEMO_VIDEO_PATH) != 0) {
        page_overlay_hide(screen_index);
    }
}

void page_overlay_show_demo_image_on_screen(int screen_index)
{
    int demo_screen = page_overlay_get_image_screen();
    if (screen_index != demo_screen) {
        page_overlay_hide(screen_index);
        page_overlay_log("[OVERLAY] image overlay skipped: screen=%d enabled_screen=%d\n",
                         screen_index, demo_screen);
        return;
    }
    if (page_overlay_show_image(screen_index, PAGE_OVERLAY_DEMO_IMAGE_PATH) != 0) {
        page_overlay_log("[OVERLAY] image overlay failed: screen=%d path=%s\n",
                         screen_index, PAGE_OVERLAY_DEMO_IMAGE_PATH);
        page_overlay_hide(screen_index);
    }
}

void page_overlay_show_demo_video(void)
{
    page_overlay_show_demo_video_on_screen(1);
    page_overlay_show_demo_video_on_screen(2);
}

#endif /* CONFIG_XOS_APP_ESL2 */
