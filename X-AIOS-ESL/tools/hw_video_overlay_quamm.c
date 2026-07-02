// ============================================================
// hw_video_overlay_quamm.c — 底层 VDEC + 双屏 VO 视频覆盖
// ============================================================
// 当前方案：
//   1. MP4 在部署脚本中转成低内存 raw AnnexB H.264，默认 800x640。
//   2. 板端只创建一个 VDEC 通道解码低分辨率帧。
//   3. 将解码帧拷贝/填充到独立 VB pool 的显示尺寸帧，默认 800x1280。
//   4. 同一显示尺寸帧发送到 display0/display1 的 VO channel。
//
// 这样避开 800x1280 VDEC NOMEM，也满足 VO 要求 src/dst 尺寸一致的限制。
// ============================================================

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "common/qua_sys_platform.h"
#include "display/qua_mm_display.h"
#include "system/qua_mm_system.h"
#include "utils/qua_display_parser.h"
#include "video/qua_mm_video.h"

// ============================================================
// 常量
// ============================================================

#define DEFAULT_STREAM_WIDTH   800
#define DEFAULT_STREAM_HEIGHT  640
#define DEFAULT_DISPLAY_WIDTH  800
#define DEFAULT_DISPLAY_HEIGHT 1280
#define VO_BUF_NUM             3
#define MAX_COPY_BUF_NUM       3

// ============================================================
// 数据结构
// ============================================================

// 单个屏幕的 VO 输出配置
typedef struct {
    const char         *display_id;
    QUA_CONST_CHAR     *vo_id;
    QUA_CONST_CHAR     *fb_id;
    QUA_S32             vo_num;
    QUA_S32             fb_num;
    QUA_VO_LAYER        layer;
    QUA_VO_CHN          chn;
    qua_mm_vo_device_t *vo_dev;
    qua_mm_vo_channel_t*vo_chn;
    qua_mm_fb_device_t *fb_dev;
    int                 ready;
} vo_output_t;

// VB 帧槽位
typedef struct {
    QUA_U32 handle;
    QUA_U64 phy;
    QUA_U8 *vir;
    size_t size;
} frame_slot_t;

static void free_frame_slots(qua_mm_system_ops_t *sys_ops, QUA_VB_POOL pool_id,
                             frame_slot_t *slots, int count);

// ============================================================
// 全局变量
// ============================================================

static volatile sig_atomic_t g_running = 1;

static const char *s_display_ids[] = {
    "id:display0",
    "id:display1",
};

// ============================================================
// 信号处理
// ============================================================

static void on_signal(int sig)
{
    (void)sig;
    g_running = 0;
}

// ============================================================
// 工具函数
// ============================================================

static long long now_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

static QUA_U32 align_up_u32(QUA_U32 value, QUA_U32 align)
{
    return (value + align - 1) & ~(align - 1);
}

static int clamp_int(int value, int min_value, int max_value, int fallback)
{
    if (value < min_value || value > max_value) {
        return fallback;
    }
    return value;
}

// 读取整个文件到内存
static int read_file(const char *path, unsigned char **out, size_t *out_size)
{
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "open h264 failed: %s\n", path);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    if (size <= 0) {
        fclose(fp);
        return -1;
    }
    rewind(fp);
    unsigned char *buf = (unsigned char *)malloc((size_t)size);
    if (buf == NULL) {
        fclose(fp);
        return -1;
    }
    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        free(buf);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    *out = buf;
    *out_size = (size_t)size;
    return 0;
}

// ============================================================
// H.264 AnnexB NAL 解析
// ============================================================

static int start_code_len(const unsigned char *p, size_t remain)
{
    if (remain >= 4 && p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1) return 4;
    if (remain >= 3 && p[0] == 0 && p[1] == 0 && p[2] == 1) return 3;
    return 0;
}

static size_t find_start_code(const unsigned char *buf, size_t size, size_t from)
{
    for (size_t i = from; i + 3 < size; i++) {
        if (start_code_len(buf + i, size - i) > 0) return i;
    }
    return size;
}

static int next_nal(const unsigned char *buf, size_t size, size_t *offset,
                    size_t *nal_start, size_t *nal_size)
{
    size_t pos = find_start_code(buf, size, *offset);
    if (pos >= size) return 0;
    int sc = start_code_len(buf + pos, size - pos);
    size_t next = find_start_code(buf, size, pos + (size_t)sc);
    *nal_start = pos;
    *nal_size  = next - pos;
    *offset    = next;
    return *nal_size > 0;
}

static int h264_nal_type(const unsigned char *buf, size_t nal_start, size_t nal_size)
{
    int sc = start_code_len(buf + nal_start, nal_size);
    if (sc <= 0 || (size_t)sc >= nal_size) return -1;
    return buf[nal_start + (size_t)sc] & 0x1f;
}

static int h264_is_vcl(int nal_type)
{
    return nal_type >= 1 && nal_type <= 5;
}

static void vdec_stream_defaults(qua_vdec_stream_t *s)
{
    memset(s, 0, sizeof(*s));
    s->end_of_frame = QUA_TRUE;
}

// ============================================================
// QUA MM 系统初始化
// ============================================================

static int init_quamm(qua_mm_system_ops_t **out_sys_ops,
                       const qua_mm_module_t **out_display)
{
    char platform[32] = {0};
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");
        return -1;
    }

    qua_mm_system_t *system = NULL;
    if (qua_mm_init(QUA_FALSE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init failed\n");
        return -1;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)system;
    // SLAVE 模式不调用 sys_init（qxosui 已初始化系统），但需要 display_parser
    qua_init_display_parser();
    const qua_mm_module_t *display_module = NULL;
    if (qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module) != QUA_SUCCESS ||
        display_module == NULL) {
        fprintf(stderr, "load display module failed\n");
        return -1;
    }

    *out_sys_ops = sys_ops;
    *out_display = display_module;
    return 0;
}

// ============================================================
// VO 输出初始化（借鉴 hw_h264_vo_dual_overlay_quamm.c）
// ============================================================

static void set_fb_hidden(vo_output_t *out, QUA_BOOL show)
{
    if (out->fb_dev == NULL) return;
    qua_fb_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.alpha_enable  = QUA_TRUE;
    alpha.alpha_channel = QUA_FALSE;
    alpha.alpha0        = 0;
    alpha.alpha1        = 0;
    alpha.global_alpha  = 0;
    out->fb_dev->put_alpha(out->fb_dev, &alpha);
    out->fb_dev->show(out->fb_dev, show);
}

static void restore_fb(vo_output_t *out)
{
    if (out->fb_dev == NULL) return;
    qua_fb_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.alpha_enable  = QUA_TRUE;
    alpha.alpha_channel = QUA_FALSE;
    alpha.alpha0        = 0;
    alpha.alpha1        = 255;
    alpha.global_alpha  = 255;
    out->fb_dev->put_alpha(out->fb_dev, &alpha);
    out->fb_dev->show(out->fb_dev, QUA_TRUE);
}

// 初始化单个屏幕的视频层 + VO 通道 + FB 隐藏
// hide_fb: 1=隐藏FB, 0=保持显示
static int init_output(const qua_mm_module_t *display_module,
                        vo_output_t *out, const char *display_id, int hide_fb,
                        int display_width, int display_height, int fps,
                        int video_priority)
{
    memset(out, 0, sizeof(*out));
    out->display_id = display_id;

    QUA_S32 ret = qua_parse_display_id(display_id, &out->vo_id, &out->vo_num,
                                       &out->fb_id, &out->fb_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s parse failed ret=%d\n", display_id, ret);
        return -1;
    }

    // display1 用 layer=1, display0 用 layer=0（参考双屏管线）
    out->layer = out->vo_num == 1 ? 1 : 0;
    out->chn   = out->vo_num;

    // ---- 打开并初始化 VO 设备 ----
    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, out->vo_id, out->vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open vo failed ret=%d\n", display_id, ret);
        return -1;
    }
    out->vo_dev = (qua_mm_vo_device_t *)mm_device;

    qua_vo_pub_attr_t dev_attr;
    memset(&dev_attr, 0, sizeof(dev_attr));
    dev_attr.bg_color   = 0;
    dev_attr.intf_type  = QUA_VO_INTF_LCD;
    dev_attr.intf_sync  = QUA_VO_OUTPUT_800x1280_60;
    out->vo_dev->set_public_attr(out->vo_num, &dev_attr);
    out->vo_dev->enable(out->vo_num);

    // ---- 配置视频层 ----
    if (out->vo_dev->bind_video_layer != NULL) {
        out->vo_dev->bind_video_layer(out->layer, out->vo_num);
    }

    qua_vo_video_layer_attr_t layer_attr;
    qua_vo_compress_attr_t    compress_attr;
    memset(&layer_attr,    0, sizeof(layer_attr));
    memset(&compress_attr, 0, sizeof(compress_attr));
    layer_attr.disp_rect.x      = 0;
    layer_attr.disp_rect.y      = 0;
    layer_attr.disp_rect.width  = display_width;
    layer_attr.disp_rect.height = display_height;
    layer_attr.image_size.width  = display_width;
    layer_attr.image_size.height = display_height;
    layer_attr.disp_frmrt       = fps;
    layer_attr.pix_format       = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    layer_attr.double_frame     = QUA_FALSE;
    layer_attr.cluster_mode     = QUA_TRUE;
    compress_attr.support_compress = QUA_FALSE;
    out->vo_dev->set_video_layer_attr(out->layer, &layer_attr, &compress_attr);
    out->vo_dev->set_video_disp_buflen(out->layer, VO_BUF_NUM);

    if (out->vo_dev->set_video_layer_priority != NULL) {
        out->vo_dev->set_video_layer_priority(out->layer, video_priority);
    }
    out->vo_dev->enable_video_layer(out->layer);

    // ---- 创建并配置 VO 通道 ----
    qua_mm_channel_t *mm_channel = NULL;
    QUA_S32 chn = out->chn;
    ret = out->vo_dev->parent.create_channel(
        (qua_mm_device_t *)out->vo_dev, &chn, NULL, &mm_channel);
    if (ret != QUA_SUCCESS || mm_channel == NULL) {
        fprintf(stderr, "%s create vo chn failed ret=%d\n", display_id, ret);
        return -1;
    }
    out->chn    = chn;
    out->vo_chn = (qua_mm_vo_channel_t *)mm_channel;

    qua_vo_chn_attr_t chn_attr;
    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.priority   = 0;
    chn_attr.rect.x     = 0;
    chn_attr.rect.y     = 0;
    chn_attr.rect.width  = display_width;
    chn_attr.rect.height = display_height;
    chn_attr.deflicker  = QUA_FALSE;
    out->vo_chn->set_chn_attr(out->layer, out->chn, &chn_attr);
    out->vo_chn->set_chn_frame_rate(out->layer, out->chn, fps);
    out->vo_chn->enable_chn(out->layer, out->chn);
    out->vo_chn->clear_chn_buffer(out->layer, out->chn, QUA_TRUE);
    out->vo_chn->show_chn(out->layer, out->chn);

    // ---- 隐藏 FB 层 ----
    if (hide_fb) {
        mm_device = NULL;
        ret = display_module->open_device(display_module, out->fb_id, out->fb_num, &mm_device);
        if (ret == QUA_SUCCESS && mm_device != NULL) {
            out->fb_dev = (qua_mm_fb_device_t *)mm_device;
            set_fb_hidden(out, QUA_TRUE);
        } else {
            fprintf(stderr, "%s open fb failed ret=%d\n", display_id, ret);
        }
    }

    out->ready = 1;
    fprintf(stderr, "%s output ready layer=%d chn=%d rect=%dx%d fps=%d\n",
            display_id, out->layer, out->chn, display_width, display_height, fps);
    return 0;
}

// 释放单个屏幕的 VO 资源
static void deinit_output(vo_output_t *out)
{
    if (out->fb_dev != NULL) {
        restore_fb(out);
        out->fb_dev->parent.close((qua_mm_device_t *)out->fb_dev);
        out->fb_dev = NULL;
    }
    if (out->vo_chn != NULL) {
        out->vo_chn->disable_chn(out->layer, out->chn);
        out->vo_chn->parent.release((qua_mm_channel_t *)out->vo_chn);
        out->vo_chn = NULL;
    }
    if (out->vo_dev != NULL) {
        out->vo_dev->disable_video_layer(out->layer);
        if (out->vo_dev->unbind_video_layer != NULL) {
            out->vo_dev->unbind_video_layer(out->layer, out->vo_num);
        }
        out->vo_dev->parent.close((qua_mm_device_t *)out->vo_dev);
        out->vo_dev = NULL;
    }
    out->ready = 0;
}

// ============================================================
// VB Frame Pool 管理（帧拷贝用）
// ============================================================

static int alloc_frame_slots(qua_mm_system_ops_t *sys_ops, QUA_VB_POOL *pool_id,
                             frame_slot_t *slots, int count, size_t frame_size)
{
    *pool_id = sys_ops->vb_create_pool((QUA_U32)frame_size, count, NULL);
    if (*pool_id == QUA_VB_INVALID_POOLID) {
        fprintf(stderr, "vb_create_pool failed size=%u count=%d\n",
                (unsigned)frame_size, count);
        return -1;
    }
    for (int i = 0; i < count; i++) {
        slots[i].handle = sys_ops->vb_get_block(*pool_id, (QUA_U32)frame_size, NULL);
        if (slots[i].handle == QUA_VB_INVALID_HANDLE) {
            fprintf(stderr, "vb_get_block failed i=%d\n", i);
            free_frame_slots(sys_ops, *pool_id, slots, count);
            return -1;
        }
        slots[i].phy = sys_ops->vb_handle_to_phyaddr(slots[i].handle);
        if (slots[i].phy == 0) {
            fprintf(stderr, "vb_handle_to_phyaddr failed i=%d\n", i);
            free_frame_slots(sys_ops, *pool_id, slots, count);
            return -1;
        }
        slots[i].vir = (QUA_U8 *)sys_ops->sys_mmap(slots[i].phy, (QUA_U32)frame_size);
        if (slots[i].vir == NULL) {
            fprintf(stderr, "sys_mmap slot failed i=%d\n", i);
            free_frame_slots(sys_ops, *pool_id, slots, count);
            return -1;
        }
        slots[i].size = frame_size;
    }
    return 0;
}

static void free_frame_slots(qua_mm_system_ops_t *sys_ops, QUA_VB_POOL pool_id,
                             frame_slot_t *slots, int count)
{
    for (int i = 0; i < count; i++) {
        if (slots[i].vir != NULL) {
            sys_ops->sys_munmap(slots[i].vir, (QUA_U32)slots[i].size);
            slots[i].vir = NULL;
        }
        if (slots[i].handle != QUA_VB_INVALID_HANDLE) {
            sys_ops->vb_release_block(slots[i].handle);
            slots[i].handle = QUA_VB_INVALID_HANDLE;
        }
    }
    if (pool_id != QUA_VB_INVALID_POOLID) {
        sys_ops->vb_destroy_pool(pool_id);
    }
}

// 将解码帧拷贝到 VB pool 槽位（YUV420/NV12），用于同一解码帧双屏复用。
static int copy_decoded_frame(qua_mm_system_ops_t *sys_ops,
                              const qua_video_frame_info_t *src,
                              frame_slot_t *slot,
                              qua_video_frame_info_t *dst,
                              QUA_VB_POOL pool_id,
                              QUA_U32 src_width,
                              QUA_U32 src_height,
                              QUA_U32 dst_width,
                              QUA_U32 dst_height,
                              QUA_U32 dst_stride)
{
    QUA_U32 src_stride_y  = src->video_frame.stride[0] ? src->video_frame.stride[0] : src_width;
    QUA_U32 src_stride_uv = src->video_frame.stride[1] ? src->video_frame.stride[1] : src_stride_y;
    size_t  src_map_size  = (size_t)src_stride_y * src_height + (size_t)src_stride_uv * src_height / 2;

    // 映射解码帧的虚拟地址
    QUA_U8 *src_map = NULL;
    QUA_U8 *src_y  = (QUA_U8 *)(uintptr_t)src->video_frame.vir_addr[0];
    QUA_U8 *src_uv = (QUA_U8 *)(uintptr_t)src->video_frame.vir_addr[1];
    if (src_y == NULL) {
        src_map = (QUA_U8 *)sys_ops->sys_mmap(src->video_frame.phy_addr[0],
                                              (QUA_U32)src_map_size);
        if (src_map == NULL) {
            fprintf(stderr, "map decoded frame failed\n");
            return -1;
        }
        src_y = src_map;
        if (src->video_frame.phy_addr[1] > src->video_frame.phy_addr[0] &&
            src->video_frame.phy_addr[1] - src->video_frame.phy_addr[0] < src_map_size) {
            src_uv = src_map + (src->video_frame.phy_addr[1] - src->video_frame.phy_addr[0]);
        } else {
            src_uv = src_map + (size_t)src_stride_y * src_height;
        }
    } else if (src_uv == NULL) {
        src_uv = src_y + (size_t)src_stride_y * src_height;
    }

    QUA_U8 *dst_y  = slot->vir;
    QUA_U8 *dst_uv = slot->vir + (size_t)dst_stride * dst_height;

    for (QUA_U32 y = 0; y < dst_height; y++) {
        memset(dst_y + (size_t)y * dst_stride, 16, dst_stride);
    }
    for (QUA_U32 y = 0; y < dst_height / 2; y++) {
        memset(dst_uv + (size_t)y * dst_stride, 128, dst_stride);
    }

    QUA_U32 copy_width = src_width < dst_width ? src_width : dst_width;
    QUA_U32 copy_height = src_height < dst_height ? src_height : dst_height;
    copy_width &= ~1U;
    copy_height &= ~1U;
    QUA_U32 dst_x = ((dst_width - copy_width) / 2) & ~1U;
    QUA_U32 dst_y_off = ((dst_height - copy_height) / 2) & ~1U;

    // Y plane copy (行拷贝处理 stride 差异)
    for (QUA_U32 y = 0; y < copy_height; y++) {
        memcpy(dst_y  + (size_t)(dst_y_off + y) * dst_stride + dst_x,
               src_y  + (size_t)y * src_stride_y, copy_width);
    }
    // UV plane copy
    for (QUA_U32 y = 0; y < copy_height / 2; y++) {
        memcpy(dst_uv + (size_t)(dst_y_off / 2 + y) * dst_stride + dst_x,
               src_uv + (size_t)y * src_stride_uv, copy_width);
    }

    if (src_map != NULL) {
        sys_ops->sys_munmap(src_map, (QUA_U32)src_map_size);
    }

    // 填充目标帧信息
    memset(dst, 0, sizeof(*dst));
    dst->pool_id                = pool_id;
    dst->video_frame.width      = dst_width;
    dst->video_frame.height     = dst_height;
    dst->video_frame.field      = QUA_VIDEO_FIELD_FRAME;
    dst->video_frame.pixel_fmt  = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    dst->video_frame.video_fmt  = QUA_VIDEO_FORMAT_LINEAR;
    dst->video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
    dst->video_frame.stride[0]  = dst_stride;
    dst->video_frame.stride[1]  = dst_stride;
    dst->video_frame.phy_addr[0]= slot->phy;
    dst->video_frame.phy_addr[1]= slot->phy + (size_t)dst_stride * dst_height;
    dst->video_frame.vir_addr[0]= (QUA_U64)(uintptr_t) dst_y;
    dst->video_frame.vir_addr[1]= (QUA_U64)(uintptr_t) dst_uv;
    return 0;
}

// ============================================================
// 主函数
// ============================================================

// 用法:
//   hw_video_overlay_quamm h264 marker mode hide_fb fps stream_w stream_h display_w display_h vdec_vb copy_buf priority
int main(int argc, char **argv)
{
    setvbuf(stderr, NULL, _IONBF, 0);

    const char *path   = argc > 1 ? argv[1] :
        "/data/res/img/13ad34351bdf701e9db0a1c53f2838ef.h264";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_hw_video_overlay";
    const char *mode   = argc > 3 ? argv[3] : "both";
    int hide_fb        = argc > 4 ? atoi(argv[4]) : 1;
    int fps            = argc > 5 ? atoi(argv[5]) : 30;
    int stream_width   = argc > 6 ? atoi(argv[6]) : DEFAULT_STREAM_WIDTH;
    int stream_height  = argc > 7 ? atoi(argv[7]) : DEFAULT_STREAM_HEIGHT;
    int display_width  = argc > 8 ? atoi(argv[8]) : DEFAULT_DISPLAY_WIDTH;
    int display_height = argc > 9 ? atoi(argv[9]) : DEFAULT_DISPLAY_HEIGHT;
    int vdec_vb_cnt    = argc > 10 ? atoi(argv[10]) : 2;
    int copy_buf_cnt   = argc > 11 ? atoi(argv[11]) : 2;
    int video_priority = argc > 12 ? atoi(argv[12]) : 3;

    if (fps <= 0 || fps > 60) fps = 30;
    stream_width = clamp_int(stream_width, 64, DEFAULT_DISPLAY_WIDTH, DEFAULT_STREAM_WIDTH);
    stream_height = clamp_int(stream_height, 64, DEFAULT_DISPLAY_HEIGHT, DEFAULT_STREAM_HEIGHT);
    display_width = clamp_int(display_width, 64, DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_WIDTH);
    display_height = clamp_int(display_height, 64, DEFAULT_DISPLAY_HEIGHT, DEFAULT_DISPLAY_HEIGHT);
    vdec_vb_cnt = clamp_int(vdec_vb_cnt, 1, 4, 2);
    copy_buf_cnt = clamp_int(copy_buf_cnt, 0, MAX_COPY_BUF_NUM, 2);
    video_priority = clamp_int(video_priority, 0, 3, 3);

    signal(SIGTERM, on_signal);
    signal(SIGINT,  on_signal);

    // ---- 读取 raw H.264 流 ----
    unsigned char *stream      = NULL;
    size_t         stream_size = 0;
    if (read_file(path, &stream, &stream_size) != 0) return 1;
    fprintf(stderr, "loaded h264 stream: %s size=%u\n", path, (unsigned)stream_size);

    // ---- 初始化 QUA MM 系统 ----
    qua_mm_system_ops_t  *sys_ops       = NULL;
    const qua_mm_module_t *display_module = NULL;
    if (init_quamm(&sys_ops, &display_module) != 0) {
        free(stream);
        return 1;
    }

    // ---- 确定要显示的屏幕 ----
    int want_display[2] = {0, 0};
    if (strcmp(mode, "both") == 0) {
        want_display[0] = 1;
        want_display[1] = 1;
    } else if (strcmp(mode, "1") == 0 || strcmp(mode, "display1") == 0) {
        want_display[1] = 1;
    } else {
        want_display[0] = 1;
    }

    // ---- 初始化所有 VO 输出 ----
    vo_output_t outputs[2];
    int         output_count = 0;
    for (int i = 0; i < 2; i++) {
        if (!want_display[i]) continue;
        if (init_output(display_module, &outputs[output_count],
                        s_display_ids[i], hide_fb, display_width,
                        display_height, fps, video_priority) == 0) {
            output_count++;
        }
    }
    if (output_count == 0) {
        free(stream);
        return 1;
    }

    // ---- 使用底层 VDEC API 创建解码通道 ----
    // 参考 qua_vdec_sample.c + uvc_preview.c，绕过 qua_decoder_create 的 MMZ 内存不足问题
    const qua_mm_module_t *video_module = NULL;
    QUA_S32 load_ret = qua_mm_load_module(QUA_MM_MODULE_VIDEO, &video_module);
    if (load_ret != QUA_SUCCESS || video_module == NULL) {
        fprintf(stderr, "load video module failed ret=%d\n", load_ret);
        for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
        free(stream);
        return 1;
    }
    fprintf(stderr, "Module %s, API version %d\n", video_module->id, video_module->api_version);

    qua_mm_device_t *mm_device = NULL;
    load_ret = video_module->open_device(video_module, QUA_MM_VIDEO_DEV_DEC, 0, &mm_device);
    if (load_ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "open vdec device failed ret=%d\n", load_ret);
        for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
        free(stream);
        return 1;
    }
    qua_mm_vdec_device_t *vdec_device = (qua_mm_vdec_device_t *)mm_device;
    fprintf(stderr, "VDEC device opened: %s\n", vdec_device->parent.id);

    // 配置 VDEC 通道属性
    QUA_U32 stream_buf_size = (QUA_U32)stream_width * (QUA_U32)stream_height;
    qua_vdec_chn_attr_t chn_attr;
    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.coding_type       = QUA_VIDEO_CodingAVC;
    chn_attr.in_stream_buf_size = stream_buf_size;
    chn_attr.priority          = 5;
    chn_attr.pic_width         = (QUA_U32)stream_width;
    chn_attr.pic_height        = (QUA_U32)stream_height;
    chn_attr.vdec_video_attr.mode               = QUA_VIDEO_MODE_FRAME;
    chn_attr.vdec_video_attr.ref_frame_num      = 1;
    chn_attr.vdec_video_attr.temporal_mvp_enable = 0;

    qua_mm_vdec_chn_attr_t vdec_chn_attr;
    memset(&vdec_chn_attr, 0, sizeof(vdec_chn_attr));
    vdec_chn_attr.chn_attr = chn_attr;
    vdec_chn_attr.vb_cnt   = vdec_vb_cnt;

    QUA_S32 vdec_chn_id = 0;
    qua_mm_channel_t *mm_channel = NULL;
    load_ret = vdec_device->parent.create_channel(
        mm_device, &vdec_chn_id, (QUA_VOID_PTR)&vdec_chn_attr, &mm_channel);
    if (load_ret != QUA_SUCCESS || mm_channel == NULL) {
        fprintf(stderr, "create vdec channel failed ret=0x%x\n", load_ret);
        vdec_device->parent.close(mm_device);
        for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
        free(stream);
        return 1;
    }
    qua_mm_vdec_channel_t *vdec_chn = (qua_mm_vdec_channel_t *)mm_channel;

    qua_vdec_chn_param_t chn_param;
    memset(&chn_param, 0, sizeof(chn_param));
    if (vdec_chn->get_chn_param != NULL &&
        vdec_chn->set_chn_param != NULL &&
        vdec_chn->get_chn_param(vdec_chn_id, &chn_param) == QUA_SUCCESS) {
        chn_param.dec_order_output = 0;
        chn_param.chan_err_thr = 1;
        QUA_S32 pret = vdec_chn->set_chn_param(vdec_chn_id, &chn_param);
        fprintf(stderr, "set vdec param ret=%d\n", pret);
    }

    load_ret = vdec_chn->start_chn(vdec_chn_id);
    fprintf(stderr, "VDEC chn=%d start ret=%d (direct VDEC->VO mode)\n", vdec_chn_id, load_ret);

    frame_slot_t slots[MAX_COPY_BUF_NUM];
    memset(slots, 0, sizeof(slots));
    for (int i = 0; i < MAX_COPY_BUF_NUM; i++) {
        slots[i].handle = QUA_VB_INVALID_HANDLE;
    }
    QUA_VB_POOL copy_pool = QUA_VB_INVALID_POOLID;
    QUA_U32 copy_stride = align_up_u32((QUA_U32)display_width, 64);
    size_t copy_frame_size = (size_t)copy_stride * (size_t)display_height * 3 / 2;
    int use_copy_slots = 0;
    if (copy_buf_cnt > 0 &&
        alloc_frame_slots(sys_ops, &copy_pool, slots, copy_buf_cnt, copy_frame_size) == 0) {
        use_copy_slots = 1;
    } else if (copy_buf_cnt > 0) {
        fprintf(stderr, "copy VB unavailable; fallback to direct decoded frames\n");
    }

    fprintf(stderr,
            "direct vo overlay start: outputs=%d mode=%s hide_fb=%d fps=%d stream=%dx%d display=%dx%d vdec_vb=%d copy=%d stride=%u\n",
            output_count, mode, hide_fb, fps, stream_width, stream_height,
            display_width, display_height, vdec_vb_cnt,
            use_copy_slots ? copy_buf_cnt : 0, copy_stride);

    // ---- 主播放循环 ----
    long long     frame_interval = 1000000LL / fps;
    QUA_U64       pts            = 0;
    unsigned int  frame_count    = 0;
    unsigned int  loop_count     = 0;

    while (g_running && access(marker, F_OK) == 0) {
        size_t        off              = 0;
        unsigned int  loop_frame_count = 0;
        long long     loop_start_us    = now_us();

        // 喂入 NAL 数据到 VDEC
        while (g_running && access(marker, F_OK) == 0) {
            size_t nal_start = 0, nal_size = 0;
            if (!next_nal(stream, stream_size, &off, &nal_start, &nal_size)) break;

            int nal_type = h264_nal_type(stream, nal_start, nal_size);

            // 按官方 sample 的方式，每个 AnnexB 包都标记为一帧输入。
            qua_vdec_stream_t vdec_stream;
            vdec_stream_defaults(&vdec_stream);
            vdec_stream.in_data  = (QUA_U64)(uintptr_t)(stream + nal_start);
            vdec_stream.in_size  = (QUA_U32)nal_size;
            vdec_stream.pts      = pts;

            QUA_S32 sret = vdec_chn->send_stream(vdec_chn_id, &vdec_stream, -1);
            if (sret != 0) {
                fprintf(stderr, "send_stream fail ret=%d type=%d size=%u\n",
                        sret, nal_type, (unsigned)nal_size);
            }

            if (!h264_is_vcl(nal_type)) continue;
            pts += (QUA_U64)frame_interval;

            // 从 VDEC 取出解码帧
            qua_video_frame_info_t frame;
            memset(&frame, 0, sizeof(frame));
            QUA_S32 ret = vdec_chn->get_frame(vdec_chn_id, &frame, 500);
            if (ret != 0) {
                if (frame_count < 5)
                    fprintf(stderr, "get_frame ret=%d (warmup?)\n", ret);
                continue;
            }

            qua_video_frame_info_t display_frame;
            qua_video_frame_info_t *frame_to_send = &frame;
            if (use_copy_slots) {
                frame_slot_t *slot = &slots[frame_count % (unsigned int)copy_buf_cnt];
                if (copy_decoded_frame(sys_ops, &frame, slot, &display_frame, copy_pool,
                                       (QUA_U32)stream_width, (QUA_U32)stream_height,
                                       (QUA_U32)display_width, (QUA_U32)display_height,
                                       copy_stride) == 0) {
                    frame_to_send = &display_frame;
                } else {
                    fprintf(stderr, "copy decoded frame failed, use direct frame\n");
                }
            }

            // 发到所有 VO 通道
            for (int i = 0; i < output_count; i++) {
                QUA_S32 vsret = outputs[i].vo_chn->send_frame(
                    outputs[i].layer, outputs[i].chn, frame_to_send, 100);
                if (vsret != 0) {
                    fprintf(stderr, "%s send_frame fail layer=%d chn=%d ret=%d\n",
                            outputs[i].display_id, outputs[i].layer,
                            outputs[i].chn, vsret);
                }
            }
            vdec_chn->release_frame(vdec_chn_id, &frame);

            frame_count++;
            loop_frame_count++;
            if (frame_count < 5 || frame_count % (unsigned int)fps == 0) {
                fprintf(stderr, "frame=%u w=%u h=%u stride=%u,%u loops=%u\n",
                        frame_count, frame.video_frame.width, frame.video_frame.height,
                        frame.video_frame.stride[0], frame.video_frame.stride[1], loop_count);
            }

            // 帧率控制
            long long target_us = loop_start_us +
                                  (long long)loop_frame_count * frame_interval;
            long long delay = target_us - now_us();
            if (delay > 2000 && delay < 100000) {
                usleep((useconds_t)delay);
            }
        }

        loop_count++;
        fprintf(stderr, "loop %u complete, restarting... frames=%u\n",
                loop_count, frame_count);
    }

    // ---- 清理 ----
    fprintf(stderr, "stopping... loops=%u frames=%u\n", loop_count, frame_count);

    vdec_chn->stop_chn(vdec_chn_id);
    mm_channel->release(mm_channel);
    vdec_device->parent.close(mm_device);
    if (use_copy_slots) {
        free_frame_slots(sys_ops, copy_pool, slots, copy_buf_cnt);
    }
    for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
    free(stream);

    fprintf(stderr, "direct vo overlay stop\n");
    return 0;
}
