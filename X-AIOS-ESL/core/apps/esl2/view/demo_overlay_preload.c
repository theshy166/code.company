#define _GNU_SOURCE
// 启用 GNU 扩展功能，必须定义在所有头文件之前

#include <dlfcn.h>     // 动态链接库操作（dlsym）
#include <stdbool.h>   // bool 类型
#include <stdint.h>    // 标准整数类型
#include <stdio.h>     // 文件操作
#include <string.h>    // 字符串操作
#include <unistd.h>    // access() 系统调用

// 图片覆盖开关标记文件
#define DEMO_IMAGE_MARKER "/data/.enable_image_overlay"
// 预加载调试日志路径
#define DEMO_LOG_PATH "/data/preload_debug.log"

// ============================================================
// 数据结构定义（与 qxosui 主程序中的结构保持一致）
// ============================================================

// ESL UI 组件子项结构（用于图片/文本等组件）
typedef struct esl_base_model_child {
    int type;              // 组件类型：1=图片, 2=视频, 3=文本等
    char *content;         // 内容（图片路径/文本内容等）
    char *color;           // 颜色
    int fontSize;          // 字体大小
    char *textAlign;       // 文本对齐方式
    char *fontFamily;      // 字体家族
    char *bgColor;         // 背景颜色
    char *textDecoration;  // 文本装饰（下划线等）
    float borderRadius;    // 圆角半径
    float borderWidth;     // 边框宽度
    char *borderColor;     // 边框颜色
    int x;                 // 组件左上角 X 坐标
    int y;                 // 组件左上角 Y 坐标
    int w;                 // 组件宽度
    int h;                 // 组件高度
    float angle;           // 旋转角度
    float opacity;         // 透明度
    int padding;           // 内边距
    int lineSpace;         // 行间距
    int letterSpace;       // 字间距
    int intFontSize;       // 整数字体大小
    int floatFontSize;     // 浮点字体大小
    char *numFloatStyle;   // 数字浮点样式
} esl_base_model_child_t;

// ESL 图片结构
typedef struct esl_img {
    char *img_url;   // 图片 URL/路径
    int width;       // 图片宽度
    int height;      // 图片高度
} esl_img_t;

// ESL UI 基础模型结构（整个页面的数据模型）
typedef struct esl_base_model {
    int width;                 // 页面宽度
    int height;                // 页面高度
    char *color;               // 文字颜色
    int opacity;               // 透明度
    char *bgColor;             // 背景颜色
    char **video_urls;         // 视频 URL 数组
    int video_count;           // 视频数量
    esl_img_t *img_urls;       // 图片数组
    int img_count;             // 图片数量
    int child_count;           // 子组件数量
    esl_base_model_child_t *child_list; // 子组件列表
} esl_base_model_t;

// ============================================================
// 演示图片候选配置
// ============================================================

// 演示图片候选结构：文件系统路径 + LVGL 虚拟路径
typedef struct {
    const char *fs_path;    // 实际文件系统路径
    const char *lvgl_path;  // LVGL 使用的虚拟路径（H:/ 开头）
} demo_image_candidate_t;

// 演示图片候选列表：按优先级排列，找到第一个存在的文件
static const demo_image_candidate_t s_demo_images[] = {
    {"/data/res/img/video_frames/frame_001.png", "H:/data/res/img/video_frames/frame_001.png"},
    {"/data/res/img/video_frames/frame_001.jpg", "H:/data/res/img/video_frames/frame_001.jpg"},
};

// ============================================================
// 函数指针声明（从主程序中获取）
// ============================================================

// 原始的 esl_show_ui 函数指针（被 hook 的函数）
static void (*real_esl_show_ui)(esl_base_model_t *model, void *top, int width, int height, int index);
// 获取背景对象的函数指针
static void *(*p_get_bg)(int index);
// 处理图片扩展的函数指针（用于显示图片）
static void (*p_handle_image_extension2)(void *bg, esl_base_model_child_t *item, int display_id);
// 初始化标志，防止重复初始化
static bool s_inited = false;

// ============================================================
// 工具函数
// ============================================================

// 写入预加载调试日志
static void preload_log(const char *msg)
{
    FILE *fp = fopen(DEMO_LOG_PATH, "a");  // 追加模式打开日志文件
    if (fp != NULL) {
        fputs(msg, fp);  // 写入日志内容
        fclose(fp);      // 关闭文件
    }
}

// 从动态链接库中加载符号（函数）
// next_only: true=只从下一个库查找, false=先从默认库查找，找不到再从下一个库查找
static void *load_symbol(const char *name, bool next_only)
{
    void *p = next_only ? dlsym(RTLD_NEXT, name) : dlsym(RTLD_DEFAULT, name);
    if (p == NULL && !next_only) {
        p = dlsym(RTLD_NEXT, name);  // 默认库找不到时，尝试从下一个库查找
    }
    return p;
}

// ============================================================
// 初始化函数
// ============================================================

// 预加载模块初始化：获取需要的函数指针
static void preload_init(void)
{
    if (s_inited) {
        return;  // 已经初始化过，直接返回
    }
    s_inited = true;

    // 获取原始的 esl_show_ui 函数（必须从下一个库查找，因为我们要 hook 它）
    real_esl_show_ui = load_symbol("esl_show_ui", true);
    // 获取 get_bg 函数（获取背景对象）
    p_get_bg = load_symbol("get_bg", false);
    // 获取 handle_image_extension2 函数（显示图片）
    p_handle_image_extension2 = load_symbol("handle_image_extension2", false);

    preload_log("[PRELOAD] image-only hook init\n");
}

// 检查文件是否可读
static bool file_readable(const char *path)
{
    return path != NULL && access(path, R_OK) == 0;
}

// 查找可用的演示图片路径
static const char *find_demo_image_path(void)
{
    size_t count = sizeof(s_demo_images) / sizeof(s_demo_images[0]);
    for (size_t i = 0; i < count; i++) {
        if (file_readable(s_demo_images[i].fs_path)) {
            return s_demo_images[i].lvgl_path;  // 返回 LVGL 虚拟路径
        }
    }
    return NULL;  // 没有找到可用的图片
}

// ============================================================
// 演示图片显示逻辑
// ============================================================

// 如果启用了图片覆盖标记，则显示演示图片覆盖原有 UI
static void maybe_show_demo_image(void *top, int screen_index)
{
    // 检查图片覆盖开关标记是否存在
    if (access(DEMO_IMAGE_MARKER, F_OK) != 0) {
        preload_log("[PRELOAD] image marker missing\n");
        return;  // 标记不存在，不显示覆盖图片
    }

    // 查找可用的演示图片
    const char *image_path = find_demo_image_path();
    if (image_path == NULL) {
        preload_log("[PRELOAD] image file missing\n");
        return;  // 图片文件不存在
    }

    // 检查 handle_image_extension2 函数是否可用
    if (p_handle_image_extension2 == NULL) {
        preload_log("[PRELOAD] handle_image_extension2 missing\n");
        return;
    }

    // 获取背景对象：优先使用 get_bg()，如果失败则使用 top
    void *bg = NULL;
    if (p_get_bg != NULL) {
        bg = p_get_bg(screen_index);
    }
    if (bg == NULL) {
        bg = top;
    }
    if (bg == NULL) {
        preload_log("[PRELOAD] bg missing\n");
        return;  // 没有背景对象，无法显示图片
    }

    // 构造图片组件参数
    esl_base_model_child_t item;
    memset(&item, 0, sizeof(item));  // 清零初始化
    item.type = 1;                   // 类型 1 = 图片
    item.content = (char *)image_path;  // 图片路径
    item.x = 0;                      // 左上角 X 坐标
    item.y = 0;                      // 左上角 Y 坐标
    item.w = 800;                    // 宽度（屏幕分辨率）
    item.h = 1280;                   // 高度（屏幕分辨率）

    // 调用图片处理函数显示图片
    // display_id: screen_index <= 1 时为 0，否则为 1（双屏设备）
    p_handle_image_extension2(bg, &item, screen_index <= 1 ? 0 : 1);
    preload_log("[PRELOAD] image override done\n");
}

// ============================================================
// Hook 函数（LD_PRELOAD 注入的核心）
// ============================================================

// Hook esl_show_ui 函数：在原始函数执行后，尝试显示演示图片
void esl_show_ui(esl_base_model_t *model, void *top, int width, int height, int index)
{
    preload_init();  // 确保已初始化

    // 如果找不到原始函数，记录错误并返回
    if (real_esl_show_ui == NULL) {
        preload_log("[PRELOAD] real esl_show_ui missing\n");
        return;
    }

    // 调用原始的 esl_show_ui 函数，正常显示模板/二维码界面
    real_esl_show_ui(model, top, width, height, index);

    // 在原始 UI 显示后，尝试覆盖显示演示图片
    maybe_show_demo_image(top, index);
}
