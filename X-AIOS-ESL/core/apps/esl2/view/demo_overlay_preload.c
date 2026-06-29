#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DEMO_IMAGE_MARKER "/data/.enable_image_overlay"
#define DEMO_LOG_PATH "/data/preload_debug.log"

typedef struct esl_base_model_child {
    int type;
    char *content;
    char *color;
    int fontSize;
    char *textAlign;
    char *fontFamily;
    char *bgColor;
    char *textDecoration;
    float borderRadius;
    float borderWidth;
    char *borderColor;
    int x;
    int y;
    int w;
    int h;
    float angle;
    float opacity;
    int padding;
    int lineSpace;
    int letterSpace;
    int intFontSize;
    int floatFontSize;
    char *numFloatStyle;
} esl_base_model_child_t;

typedef struct esl_img {
    char *img_url;
    int width;
    int height;
} esl_img_t;

typedef struct esl_base_model {
    int width;
    int height;
    char *color;
    int opacity;
    char *bgColor;
    char **video_urls;
    int video_count;
    esl_img_t *img_urls;
    int img_count;
    int child_count;
    esl_base_model_child_t *child_list;
} esl_base_model_t;

typedef struct {
    const char *fs_path;
    const char *lvgl_path;
} demo_image_candidate_t;

static const demo_image_candidate_t s_demo_images[] = {
    {"/data/res/img/video_frames/frame_001.png", "H:/data/res/img/video_frames/frame_001.png"},
    {"/data/res/img/video_frames/frame_001.jpg", "H:/data/res/img/video_frames/frame_001.jpg"},
};

static void (*real_esl_show_ui)(esl_base_model_t *model, void *top, int width, int height, int index);
static void *(*p_get_bg)(int index);
static void (*p_handle_image_extension2)(void *bg, esl_base_model_child_t *item, int display_id);
static bool s_inited = false;

static void preload_log(const char *msg)
{
    FILE *fp = fopen(DEMO_LOG_PATH, "a");
    if (fp != NULL) {
        fputs(msg, fp);
        fclose(fp);
    }
}

static void *load_symbol(const char *name, bool next_only)
{
    void *p = next_only ? dlsym(RTLD_NEXT, name) : dlsym(RTLD_DEFAULT, name);
    if (p == NULL && !next_only) {
        p = dlsym(RTLD_NEXT, name);
    }
    return p;
}

static void preload_init(void)
{
    if (s_inited) {
        return;
    }
    s_inited = true;

    real_esl_show_ui = load_symbol("esl_show_ui", true);
    p_get_bg = load_symbol("get_bg", false);
    p_handle_image_extension2 = load_symbol("handle_image_extension2", false);

    preload_log("[PRELOAD] image-only hook init\n");
}

static bool file_readable(const char *path)
{
    return path != NULL && access(path, R_OK) == 0;
}

static const char *find_demo_image_path(void)
{
    size_t count = sizeof(s_demo_images) / sizeof(s_demo_images[0]);
    for (size_t i = 0; i < count; i++) {
        if (file_readable(s_demo_images[i].fs_path)) {
            return s_demo_images[i].lvgl_path;
        }
    }
    return NULL;
}

static void maybe_show_demo_image(void *top, int screen_index)
{
    if (access(DEMO_IMAGE_MARKER, F_OK) != 0) {
        preload_log("[PRELOAD] image marker missing\n");
        return;
    }

    const char *image_path = find_demo_image_path();
    if (image_path == NULL) {
        preload_log("[PRELOAD] image file missing\n");
        return;
    }
    if (p_handle_image_extension2 == NULL) {
        preload_log("[PRELOAD] handle_image_extension2 missing\n");
        return;
    }

    void *bg = NULL;
    if (p_get_bg != NULL) {
        bg = p_get_bg(screen_index);
    }
    if (bg == NULL) {
        bg = top;
    }
    if (bg == NULL) {
        preload_log("[PRELOAD] bg missing\n");
        return;
    }

    esl_base_model_child_t item;
    memset(&item, 0, sizeof(item));
    item.type = 1;
    item.content = (char *)image_path;
    item.x = 0;
    item.y = 0;
    item.w = 800;
    item.h = 1280;

    p_handle_image_extension2(bg, &item, screen_index <= 1 ? 0 : 1);
    preload_log("[PRELOAD] image override done\n");
}

void esl_show_ui(esl_base_model_t *model, void *top, int width, int height, int index)
{
    preload_init();

    if (real_esl_show_ui == NULL) {
        preload_log("[PRELOAD] real esl_show_ui missing\n");
        return;
    }

    real_esl_show_ui(model, top, width, height, index);
    maybe_show_demo_image(top, index);
}
