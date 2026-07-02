// ============================================================
// fb_video_player_quamm.c — 基于 QUA MM 框架的 framebuffer 视频播放器
// ============================================================
// 功能：读取目录中的 BGRA 帧文件，通过 QUA MM 显示模块将帧渲染到屏幕，
//       实现逐帧播放效果（类似幻灯片/视频播放）
// 用法：./fb_video_player [帧目录] [开关标记文件] [帧间隔微秒]
//       默认：帧目录=/data/res/img/video_frames_bgra
//             标记文件=/data/.enable_video_overlay
//             帧间隔=30000us（约33fps）
// ============================================================

#include <dirent.h>     // 目录操作（opendir/readdir/closedir）
#include <errno.h>      // errno 错误码
#include <signal.h>     // 信号处理（signal/SIGTERM/SIGINT）
#include <stdio.h>      // 标准输入输出（printf/fprintf/perror）
#include <stdlib.h>     // 标准库（malloc/free/atoi/qsort）
#include <string.h>     // 字符串操作（strcmp/strlen/strncmp/memset/memcpy）
#include <unistd.h>     // POSIX 系统调用（access/usleep）

// QUA MM（多媒体框架）头文件
#include "common/qua_mm_common.h"       // QUA MM 公共定义（QUA_S32/QUA_TRUE/QUA_NULL等）
#include "common/qua_sys_platform.h"    // 平台工具函数（qua_make_platform）
#include "display/qua_mm_display.h"     // 显示模块接口（打开设备/设置属性）
#include "system/qua_mm_system.h"       // MM 系统初始化（qua_mm_init/qua_mm_load_module）
#include "utils/qua_display_parser.h"   // 显示ID解析工具（qua_parse_display_id）

// ============================================================
// 常量定义
// ============================================================

#define FRAME_WIDTH  800      // 帧宽度（屏幕分辨率宽）
#define FRAME_HEIGHT 1280     // 帧高度（屏幕分辨率高）
#define BYTES_PER_PIXEL 4     // 每像素字节数（BGRA = 4字节）
#define FRAME_SIZE ((size_t)FRAME_WIDTH * FRAME_HEIGHT * BYTES_PER_PIXEL)  // 单帧总字节数 = 800*1280*4 = 4096000

// ============================================================
// 全局变量
// ============================================================

// 运行状态标志（volatile sig_atomic_t 保证信号处理函数中安全读写）
static volatile sig_atomic_t g_running = 1;

// ============================================================
// 渲染目标结构体
// ============================================================

typedef struct {
    const char *display_id;          // 显示设备ID（如 "id:display0"）
    QUA_S32 vo_num;                  // 视频输出编号
    QUA_S32 fb_num;                  // framebuffer 编号
    QUA_CONST_CHAR *vo_id;           // 视频输出ID字符串
    QUA_CONST_CHAR *fb_id;           // framebuffer ID字符串
    qua_mm_vo_device_t *vo_device;   // 视频输出设备句柄
    qua_mm_fb_device_t *fb_device;   // framebuffer 设备句柄
    QUA_CHAR *vaddr;                 // framebuffer 映射后的虚拟地址（可直接写入像素数据）
    qua_fb_fix_screeninfo_t fix_info; // framebuffer 固定信息（内存大小/行长度等）
    qua_fb_var_screeninfo_t var_info; // framebuffer 可变信息（分辨率/色深/位域等）
} render_target_t;

// ============================================================
// 信号处理函数
// ============================================================

// 收到 SIGTERM/SIGINT 信号时，设置 g_running=0 优雅退出主循环
static void on_signal(int sig)
{
    (void)sig;           // 忽略信号编号参数（未使用）
    g_running = 0;       // 设置运行标志为0，通知主循环退出
}

// ============================================================
// 工具函数
// ============================================================

// 检查字符串 text 是否以 suffix 结尾
// 返回 1=是，0=否
static int has_suffix(const char *text, const char *suffix)
{
    size_t text_len = strlen(text);          // 计算原字符串长度
    size_t suffix_len = strlen(suffix);      // 计算后缀长度
    // 比较 text 末尾 suffix_len 个字符是否等于 suffix
    return text_len >= suffix_len && strcmp(text + text_len - suffix_len, suffix) == 0;
}

// qsort 比较函数：按字符串字母顺序排序文件名
static int compare_names(const void *a, const void *b)
{
    const char * const *sa = (const char * const *)a;  // 解引用获取字符串指针
    const char * const *sb = (const char * const *)b;  // 解引用获取字符串指针
    return strcmp(*sa, *sb);                            // 字符串比较
}

// ============================================================
// 帧文件加载
// ============================================================

// 从指定目录加载所有以 "frame_" 开头、".bgra" 结尾的帧文件
// 返回：文件路径数组（已排序），out_count 返回帧数量
// 失败返回 NULL
static char **load_frame_paths(const char *dir_path, int *out_count)
{
    DIR *dir = opendir(dir_path);            // 打开帧目录
    if (dir == NULL) {
        perror("opendir frame dir");         // 打开失败，输出错误信息
        return NULL;                         // 返回 NULL 表示失败
    }

    int capacity = 16;                       // 初始容量：可容纳16个帧路径
    int count = 0;                           // 当前已加载帧数量
    char **paths = calloc((size_t)capacity, sizeof(char *));  // 分配路径指针数组
    if (paths == NULL) {
        closedir(dir);                       // 分配失败，关闭目录后返回
        return NULL;
    }

    struct dirent *entry;                    // 目录项指针
    // 遍历目录中的所有文件
    while ((entry = readdir(dir)) != NULL) {
        // 只处理以 "frame_" 开头且以 ".bgra" 结尾的文件
        if (strncmp(entry->d_name, "frame_", 6) != 0 || !has_suffix(entry->d_name, ".bgra")) {
            continue;                        // 跳过不符合命名规则的文件
        }

        // 如果数组已满，扩容为原来的2倍
        if (count == capacity) {
            capacity *= 2;                   // 容量翻倍
            char **new_paths = realloc(paths, (size_t)capacity * sizeof(char *));  // 重新分配内存
            if (new_paths == NULL) {
                break;                       // 扩容失败，停止加载
            }
            paths = new_paths;               // 更新指针
        }

        // 构造完整文件路径：目录路径 + "/" + 文件名 + "\0"
        size_t len = strlen(dir_path) + 1 + strlen(entry->d_name) + 1;
        paths[count] = malloc(len);          // 分配路径字符串内存
        if (paths[count] == NULL) {
            break;                           // 分配失败，停止加载
        }
        snprintf(paths[count], len, "%s/%s", dir_path, entry->d_name);  // 格式化路径字符串
        count++;                             // 帧计数加1
    }
    closedir(dir);                           // 关闭目录

    // 按文件名排序，确保帧按 frame_001, frame_002, ... 的顺序播放
    qsort(paths, (size_t)count, sizeof(char *), compare_names);
    *out_count = count;                      // 返回帧数量
    return paths;                            // 返回路径数组
}

// ============================================================
// 帧文件读取
// ============================================================

// 从文件读取完整的帧数据到 buf 缓冲区
// 返回：0=成功, -1=失败
static int read_full_file(const char *path, unsigned char *buf, size_t size)
{
    FILE *fp = fopen(path, "rb");            // 以二进制只读方式打开文件
    if (fp == NULL) {
        perror("fopen frame");               // 打开失败，输出错误信息
        return -1;                           // 返回失败
    }
    size_t n = fread(buf, 1, size, fp);      // 读取 size 字节到 buf
    fclose(fp);                              // 关闭文件
    return n == size ? 0 : -1;               // 如果读取的字节数等于预期大小则成功，否则失败
}

// ============================================================
// 渲染目标初始化
// ============================================================

// 初始化一个渲染目标：打开显示设备、配置 framebuffer 参数、映射内存
// 参数：
//   display_module - QUA MM 显示模块句柄
//   target         - 渲染目标结构体（输出）
//   display_id     - 显示设备ID（如 "id:display0"）
// 返回：0=成功, -1=失败
static int init_render_target(const qua_mm_module_t *display_module,
                              render_target_t *target,
                              const char *display_id)
{
    memset(target, 0, sizeof(*target));      // 清零初始化渲染目标结构体
    target->display_id = display_id;         // 保存显示设备ID

    // 解析显示设备ID，提取 VO（视频输出）和 FB（framebuffer）的ID和编号
    QUA_S32 ret = qua_parse_display_id(display_id, &target->vo_id, &target->vo_num,
                                       &target->fb_id, &target->fb_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s parse display id failed: %d\n", display_id, ret);
        return -1;                           // 解析失败，返回错误
    }

    // 打开视频输出设备（VO）
    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, target->vo_id, target->vo_num, &mm_device);
    if (ret == QUA_SUCCESS && mm_device != NULL) {
        target->vo_device = (qua_mm_vo_device_t *)mm_device;  // 保存 VO 设备句柄

        // 配置 VO 公共属性
        qua_vo_pub_attr_t dev_attr = {0};
        dev_attr.bg_color = 0;               // 背景颜色：黑色
        dev_attr.intf_type = QUA_VO_INTF_LCD; // 接口类型：LCD
        dev_attr.intf_sync = QUA_VO_OUTPUT_800x1280_60;  // 输出时序：800x1280 @ 60Hz
        ret = target->vo_device->set_public_attr(target->vo_num, &dev_attr);  // 设置 VO 属性
        if (ret != QUA_SUCCESS) {
            fprintf(stderr, "%s set_public_attr returned %d, continue\n", display_id, ret);
        }
        ret = target->vo_device->enable(target->vo_num);  // 启用 VO 设备
        if (ret != QUA_SUCCESS) {
            fprintf(stderr, "%s enable vo returned %d, continue\n", display_id, ret);
        }
    } else {
        fprintf(stderr, "%s open vo failed: %d, continue with fb\n", display_id, ret);
        // VO 打开失败不致命，继续尝试 FB
    }

    // 打开 framebuffer 设备（FB）
    mm_device = NULL;
    ret = display_module->open_device(display_module, target->fb_id, target->fb_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open fb failed: %d\n", display_id, ret);
        return -1;                           // FB 打开失败是致命错误
    }

    target->fb_device = (qua_mm_fb_device_t *)mm_device;  // 保存 FB 设备句柄

    // 隐藏 framebuffer（先不显示，等配置完成后再显示）
    target->fb_device->show(target->fb_device, QUA_FALSE);
    // 启用压缩（减少内存带宽占用）
    target->fb_device->compress(target->fb_device, QUA_TRUE);

    // 设置 framebuffer 显示原点（左上角）
    qua_point_t pos = {0, 0};
    target->fb_device->put_origin(target->fb_device, &pos);

    // 设置 framebuffer 显示尺寸（全屏）
    qua_size_t size = {FRAME_WIDTH, FRAME_HEIGHT};
    target->fb_device->put_size(target->fb_device, &size);

    // 获取当前 framebuffer 可变信息
    if (target->fb_device->get_var_info(target->fb_device, &target->var_info) != QUA_SUCCESS) {
        fprintf(stderr, "%s get var failed\n", display_id);
        return -1;
    }

    // 配置 BGRA8888 颜色格式的位域定义
    // struct {offset, length, msb_right} 表示颜色分量在32位中的位置
    qua_fb_bitfield_t stA = {24, 8, 0};      // Alpha: 位24-31, 8位
    qua_fb_bitfield_t stR = {16, 8, 0};      // Red:   位16-23, 8位
    qua_fb_bitfield_t stG = {8, 8, 0};       // Green: 位8-15,  8位
    qua_fb_bitfield_t stB = {0, 8, 0};       // Blue:  位0-7,   8位

    // 设置 framebuffer 可变参数
    target->var_info.xres_virtual = target->var_info.xres = FRAME_WIDTH;   // 水平分辨率
    target->var_info.yres_virtual = target->var_info.yres = FRAME_HEIGHT;  // 垂直分辨率
    target->var_info.activate = QUA_FB_ACTIVATE_NOW;  // 立即激活配置
    target->var_info.bits_per_pixel = 32;              // 32位色深
    target->var_info.xoffset = 0;                      // 水平偏移为0
    target->var_info.yoffset = 0;                      // 垂直偏移为0
    target->var_info.transp = stA;   // 透明度位域（BGRA的A）
    target->var_info.red = stR;      // 红色位域（BGRA的R）
    target->var_info.green = stG;    // 绿色位域（BGRA的G）
    target->var_info.blue = stB;     // 蓝色位域（BGRA的B）

    // 写入 framebuffer 可变参数
    if (target->fb_device->put_var_info(target->fb_device, &target->var_info) != QUA_SUCCESS) {
        fprintf(stderr, "%s put var failed\n", display_id);
        return -1;
    }

    // 获取 framebuffer 固定信息（内存大小/行长度等）
    if (target->fb_device->get_fix_info(target->fb_device, &target->fix_info) != QUA_SUCCESS) {
        fprintf(stderr, "%s get fix failed\n", display_id);
        return -1;
    }

    // 配置 framebuffer Alpha 混合参数
    qua_fb_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));        // 清零初始化
    alpha.alpha_enable = QUA_TRUE;            // 启用 Alpha 混合
    alpha.alpha_channel = QUA_FALSE;         // 不使用像素内 Alpha 通道（使用全局 Alpha）
    alpha.alpha0 = 0;                         // Alpha 下限
    alpha.alpha1 = 255;                       // Alpha 上限
    alpha.global_alpha = 255;                // 全局 Alpha 值：255=完全不透明
    target->fb_device->put_alpha(target->fb_device, &alpha);  // 设置 Alpha 参数

    // 映射 framebuffer 内存到用户空间（获取可直接写入的虚拟地址）
    target->vaddr = target->fb_device->map(target->fb_device, target->fix_info.smem_len, 0);
    if (target->vaddr == QUA_NULL) {
        fprintf(stderr, "%s fb map failed\n", display_id);
        return -1;                           // 映射失败
    }

    // 清空 framebuffer（全黑）
    memset(target->vaddr, 0, target->fix_info.smem_len);

    // 显示 framebuffer（之前隐藏了，现在显示）
    if (target->fb_device->show(target->fb_device, QUA_TRUE) != QUA_SUCCESS) {
        fprintf(stderr, "%s fb show failed\n", display_id);
        return -1;
    }

    // 触发渲染（将 framebuffer 内容输出到屏幕）
    target->fb_device->render(target->fb_device, &target->var_info);
    fprintf(stderr, "%s render target ready, fb_len=%u\n", display_id,
            (unsigned)target->fix_info.smem_len);  // 打印初始化成功信息
    return 0;                                // 初始化成功
}

// ============================================================
// 渲染目标关闭
// ============================================================

// 释放渲染目标资源：取消映射、关闭设备
static void close_render_target(render_target_t *target)
{
    // 关闭 framebuffer 设备
    if (target->fb_device != NULL) {
        if (target->vaddr != NULL) {
            // 取消 framebuffer 内存映射
            target->fb_device->unmap(target->fb_device, target->vaddr, target->fix_info.smem_len);
            target->vaddr = NULL;            // 清空指针，防止悬空引用
        }
        // 关闭 framebuffer 设备（通过父类 close 方法）
        target->fb_device->parent.close((qua_mm_device_t *)target->fb_device);
        target->fb_device = NULL;            // 清空指针
    }
    // 关闭视频输出设备
    if (target->vo_device != NULL) {
        // 关闭 VO 设备（通过父类 close 方法）
        target->vo_device->parent.close((qua_mm_device_t *)target->vo_device);
        target->vo_device = NULL;            // 清空指针
    }
}

// ============================================================
// 帧渲染
// ============================================================

// 将一帧数据渲染到指定的渲染目标
// 参数：
//   target - 渲染目标
//   frame  - 帧数据（BGRA8888 格式，800x1280x4 字节）
static void render_frame(render_target_t *target, const unsigned char *frame)
{
    // 如果设备或映射地址无效，直接返回
    if (target->fb_device == NULL || target->vaddr == NULL) {
        return;
    }

    // 计算实际拷贝大小（不超过 framebuffer 内存大小）
    size_t copy_size = FRAME_SIZE;           // 默认拷贝完整帧
    if (target->fix_info.smem_len > 0 && (size_t)target->fix_info.smem_len < copy_size) {
        copy_size = (size_t)target->fix_info.smem_len;  // 限制为 framebuffer 实际大小
    }
    // 将帧数据拷贝到 framebuffer 映射内存
    memcpy(target->vaddr, frame, copy_size);
    // 触发渲染输出（将 framebuffer 内容刷新到屏幕）
    target->fb_device->render(target->fb_device, &target->var_info);
}

// ============================================================
// 主函数
// ============================================================

// 程序入口
// 参数：
//   argv[1] - 帧文件目录（默认 /data/res/img/video_frames_bgra）
//   argv[2] - 开关标记文件路径（默认 /data/.enable_video_overlay）
//   argv[3] - 帧间隔微秒数（默认 30000us ≈ 33fps）
int main(int argc, char **argv)
{
    // 解析命令行参数（如果未提供则使用默认值）
    const char *frame_dir = argc > 1 ? argv[1] : "/data/res/img/video_frames_bgra";  // 帧目录
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_video_overlay";          // 开关标记
    int delay_us = argc > 3 ? atoi(argv[3]) : 30000;                                   // 帧间隔(微秒)

    // 注册信号处理函数（SIGTERM 和 SIGINT 用于优雅退出）
    signal(SIGTERM, on_signal);              // kill 命令发送的终止信号
    signal(SIGINT, on_signal);               // Ctrl+C 中断信号

    // 加载帧文件路径列表
    int frame_count = 0;
    char **frames = load_frame_paths(frame_dir, &frame_count);
    if (frames == NULL || frame_count <= 0) {
        fprintf(stderr, "no bgra frames in %s\n", frame_dir);  // 没有找到帧文件
        return 1;                            // 退出
    }

    // 分配帧数据缓冲区（4096000 字节 = 800x1280x4）
    unsigned char *frame_buf = malloc(FRAME_SIZE);
    if (frame_buf == NULL) {
        fprintf(stderr, "malloc failed: %zu\n", FRAME_SIZE);  // 内存分配失败
        return 1;                            // 退出
    }

    // 生成平台标识字符串（用于 QUA MM 初始化）
    char platform[32] = {0};
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");  // 平台标识生成失败
        return 1;                            // 退出
    }

    // 初始化 QUA MM 系统（作为辅助进程，QUA_FALSE 表示非主进程）
    qua_mm_system_t *system = NULL;
    if (qua_mm_init(QUA_FALSE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init secondary failed\n");  // MM 系统初始化失败
        return 1;                            // 退出
    }

    // 初始化显示解析器
    qua_init_display_parser();

    // 加载显示模块
    const qua_mm_module_t *display_module = NULL;
    if (qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module) != QUA_SUCCESS || display_module == NULL) {
        fprintf(stderr, "load display module failed\n");  // 加载显示模块失败
        return 1;                            // 退出
    }

    // 初始化渲染目标（双屏设备最多两个目标）
    render_target_t targets[2];              // 渲染目标数组
    int target_count = 0;                    // 成功初始化的目标数量
    // 尝试初始化 display0（屏幕1）
    if (init_render_target(display_module, &targets[target_count], "id:display0") == 0) {
        target_count++;                      // display0 初始化成功
    }
    // 尝试初始化 display1（屏幕2，双屏设备）
    if (init_render_target(display_module, &targets[target_count], "id:display1") == 0) {
        target_count++;                      // display1 初始化成功
    }
    if (target_count == 0) {
        fprintf(stderr, "no render target ready\n");  // 没有任何渲染目标可用
        return 1;                            // 退出
    }

    // 打印启动信息
    fprintf(stderr, "quamm fb video player start: frames=%d targets=%d delay_us=%d\n",
            frame_count, target_count, delay_us);

    // ============================================================
    // 主播放循环
    // ============================================================
    // 条件：g_running==1（未被信号中断）且标记文件存在
    while (g_running && access(marker, F_OK) == 0) {
        // 遍历所有帧文件
        for (int i = 0; i < frame_count && g_running && access(marker, F_OK) == 0; i++) {
            // 读取帧文件数据
            if (read_full_file(frames[i], frame_buf, FRAME_SIZE) != 0) {
                fprintf(stderr, "skip bad frame: %s\n", frames[i]);  // 跳过损坏帧
                continue;                    // 继续下一帧
            }
            // 将帧数据渲染到所有渲染目标
            for (int t = 0; t < target_count; t++) {
                render_frame(&targets[t], frame_buf);  // 渲染到屏幕
            }
            // 帧间延时（控制播放速度）
            if (delay_us > 0) {
                usleep((useconds_t)delay_us);  // 微秒级延时
            }
        }
    }

    // ============================================================
    // 清理资源
    // ============================================================

    // 关闭所有渲染目标
    for (int t = 0; t < target_count; t++) {
        close_render_target(&targets[t]);    // 释放 VO/FB 设备资源
    }
    // 释放帧路径内存
    for (int i = 0; i < frame_count; i++) {
        free(frames[i]);                     // 释放每个路径字符串
    }
    free(frames);                            // 释放路径数组
    free(frame_buf);                         // 释放帧数据缓冲区
    fprintf(stderr, "quamm fb video player stop\n");  // 打印停止信息
    return 0;                                // 正常退出
}