#ifdef CONFIG_XOS_APP_ESL2
#if 0 /* -- page_overlay replaced by native MQTT-path demo (_maybe_show_demo_overlay) -- */

#include "page_overlay.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "page_esl_image.h"

#define PAGE_OVERLAY_WIDTH 800
#define PAGE_OVERLAY_HEIGHT 1280

#define PAGE_OVERLAY_DEMO_VIDEO_NAME "13ad34351bdf701e9db0a1c53f2838ef.mp4"

#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
#define PAGE_OVERLAY_DEMO_VIDEO_PATH \
    "/home/luoxin/work/X-AIOS-ESL/core/apps/esl2/view/" PAGE_OVERLAY_DEMO_VIDEO_NAME
#else
#define PAGE_OVERLAY_DEMO_VIDEO_PATH "/data/res/img/" PAGE_OVERLAY_DEMO_VIDEO_NAME
#endif

void *los_player_create2(lv_obj_t *parent, const char *path, lv_area_t area,
                         bool auto_restart, bool auto_center, int display_idx, int rotation);
void los_player_stop(void *player);
void los_player_destroy2(void *player, int display_idx);

typedef struct {
    lv_obj_t *root;
    void *player;
} page_overlay_state_t;

static page_overlay_state_t s_overlay[2];

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

static lv_obj_t *page_overlay_create_root(int screen_index)
{
    lv_display_t *disp = page_overlay_get_display(screen_index);
    if (disp == NULL) {
        return NULL;
    }

    lv_obj_t *parent = disp->sys_layer ? disp->sys_layer : disp->top_layer;
    if (parent == NULL) {
        return NULL;
    }

    lv_obj_t *root = lv_obj_create(parent);
    lv_obj_remove_style_all(root);
    lv_obj_set_pos(root, 0, 0);
    lv_obj_set_size(root, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT);
    lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(root, lv_color_black(), 0);
    lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);
    page_overlay_move_to_front(root);

    return root;
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
        state->player = NULL;
        return;
    }

    page_overlay_move_to_front(state->root);
}

int page_overlay_show_video(int screen_index, const char *video_path)
{
    int state_index = page_overlay_get_state_index(screen_index);
    int display_id = page_overlay_get_display_id(screen_index);

    printf("[OVERLAY] show_video entry: screen=%d display=%d path=%s\n",
           screen_index, display_id, video_path ? video_path : "NULL");

    /* 1. 清理旧层 */
    page_overlay_hide(screen_index);

    /* 2. ★ 始终先创建全屏黑色遮罩层（即使视频文件不存在） */
    page_overlay_state_t *state = &s_overlay[state_index];
    state->root = page_overlay_create_root(screen_index);
    if (state->root == NULL) {
        printf("[OVERLAY] create_root failed for screen %d\n", screen_index);
        return -1;
    }
    printf("[OVERLAY] root created at %p on screen %d\n", (void *)state->root, screen_index);

    /* 3. 仅在视频文件存在时尝试播放 */
    bool has_video = (video_path != NULL && video_path[0] != '\0');
    if (has_video && !page_overlay_is_remote_path(video_path)) {
        if (access(video_path, R_OK) != 0) {
            printf("[OVERLAY] video file not found, showing black overlay only: %s\n", video_path);
            has_video = false;
        }
    }

    if (has_video) {
        lv_area_t area = {
            .x1 = 0,
            .y1 = 0,
            .x2 = PAGE_OVERLAY_WIDTH,
            .y2 = PAGE_OVERLAY_HEIGHT,
        };
        printf("[OVERLAY] creating player: display=%d path=%s\n", display_id, video_path);
        state->player = los_player_create2(state->root, video_path, area,
                                           true, false, display_id, 0);
        if (state->player == NULL) {
            printf("[OVERLAY] video player returned NULL, black overlay still visible\n");
        } else {
            printf("[OVERLAY] video player created successfully\n");
        }
    } else {
        state->player = NULL;
        printf("[OVERLAY] no video to play, black overlay shown\n");
    }

    page_overlay_raise(screen_index);
    return 0;
}

int page_overlay_show_image(int screen_index, const char *image_path)
{
    char lvgl_path[512] = {0};

    if (image_path != NULL && image_path[0] == '/' &&
        !page_overlay_is_remote_path(image_path) && access(image_path, R_OK) != 0) {
        printf("[OVERLAY] image file not found: %s\n", image_path);
        return -1;
    }

    const char *src = page_overlay_prepare_image_path(image_path, lvgl_path, sizeof(lvgl_path));
    if (src == NULL) {
        return -1;
    }

    page_overlay_hide(screen_index);

    int state_index = page_overlay_get_state_index(screen_index);
    int display_id = page_overlay_get_display_id(screen_index);
    page_overlay_state_t *state = &s_overlay[state_index];

    state->root = page_overlay_create_root(screen_index);
    if (state->root == NULL) {
        return -1;
    }

    lv_obj_t *img = lv_img_create(state->root);
    lv_obj_set_pos(img, 0, 0);
    lv_obj_set_size(img, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT);

    printf("[OVERLAY] show image display=%d path=%s\n", display_id, src);
    _set_image_src(img, src, PAGE_OVERLAY_WIDTH, PAGE_OVERLAY_HEIGHT, display_id);
    page_overlay_raise(screen_index);

    return 0;
}

void page_overlay_show_demo_video(void)
{
    page_overlay_show_video(1, PAGE_OVERLAY_DEMO_VIDEO_PATH);
}

#endif /* 0 */

#endif /* CONFIG_XOS_APP_ESL2 */
