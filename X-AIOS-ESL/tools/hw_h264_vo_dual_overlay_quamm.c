// ============================================================
// hw_h264_vo_dual_overlay_quamm.c — 方案二（参考实现）：VDEC + VB pool拷贝 + 双屏 VO
// ============================================================
// 【方案概述】
//   使用方式：底层解码器 API (qua_decoder_create) + VO 直接管线，
//            帧拷贝到独立 VB pool 后分别发给两个 VO 通道。
//   QUA MM 初始化模式：PRIMARY (QUA_TRUE)，独占系统。
//   输入：Raw H.264 AnnexB 流文件（NAL units 带 start code）。
//   管线：VDEC 解码 → VB pool 帧拷贝 → 双屏 VO send_frame
//
//   【设计思路】
//     - VDEC 解码输出帧在解码器内部 VB pool 中
//     - 通过 copy_decoded_frame() 拷贝到独立的 VB copy pool
//     - VDEC release_frame 后 buffer 立即归还解码池（可继续解码）
//     - VB copy pool 的帧发给 VO 显示，VO 占用的 buffer 不影响 VDEC
//     - 这是正确解耦 VDEC 和 VO 缓冲区的方式
//
//   【在本设备上的可行性】
//     完全参考此方案在目标设备上的问题：
//     - 需要在 sys_init 时预分配 VB pool（block_size=1024,block_cnt=6）
//     - 同时需要 VDEC vb_cnt 个内部帧缓冲 + VB copy pool 拷贝缓冲
//     - 设备 MMZ 内存有限，800x1280 视频同时分配两套池 导致 NOMEM (0xa001800c)
//     - 如果设备 MMZ 足够，此方案是正确解法
//
//   【与当前方案的关系】
//     本项目的 hw_video_overlay_quamm.c 尝试基于此参考代码改造，
//     但因 MMZ 限制无法创建 VB copy pool，最终改为直接 VDEC->VO 发帧，
//     导致了缓冲区耗尽问题（详见 hw_video_overlay_quamm.c 的注释）。
// ============================================================
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "common/qua_sys_platform.h"
#include "display/qua_mm_display.h"
#include "player/qua_decoder_inf.h"
#include "player/qua_mm_player_common.h"
#include "system/qua_mm_system.h"
#include "utils/qua_display_parser.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1280
#define VO_BUF_NUM 3
#define COPY_BUF_NUM 3

typedef struct {
    const char *display_id;
    QUA_CONST_CHAR *vo_id;
    QUA_CONST_CHAR *fb_id;
    QUA_S32 vo_num;
    QUA_S32 fb_num;
    QUA_VO_LAYER layer;
    QUA_VO_CHN chn;
    qua_mm_vo_device_t *vo_dev;
    qua_mm_vo_channel_t *vo_chn;
    qua_mm_fb_device_t *fb_dev;
    int ready;
} vo_output_t;

typedef struct {
    QUA_U32 handle;
    QUA_U64 phy;
    QUA_U8 *vir;
    size_t size;
} frame_slot_t;

static volatile sig_atomic_t g_running = 1;

static const char *s_display_ids[] = {
    "id:display0",
    "id:display1",
};

static void on_signal(int sig)
{
    (void)sig;
    g_running = 0;
}

static long long now_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

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
    *nal_size = next - pos;
    *offset = next;
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

static void packet_defaults(qua_packet_t *pkt)
{
    memset(pkt, 0, sizeof(*pkt));
    pkt->offset = 0;
    pkt->pts = -1;
    pkt->dts = -1;
    pkt->duration = 0;
    pkt->baseTime = 0;
    pkt->streamIndex = -1;
    pkt->end_of_stream = 0;
}

static int init_quamm(qua_mm_system_ops_t **out_sys_ops, const qua_mm_module_t **out_display)
{
    char platform[32] = {0};
    qua_mm_system_t *system = NULL;
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");
        return -1;
    }
    if (qua_mm_init(QUA_TRUE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init failed\n");
        return -1;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)system;
    qua_vb_config_t vb_cfg;
    memset(&vb_cfg, 0, sizeof(vb_cfg));
    vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vb_cfg.common_pools[0].block_size = 1024;
    vb_cfg.common_pools[0].block_cnt = 6;
    QUA_S32 ret = sys_ops->sys_init(&vb_cfg);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "sys_init ret=%d, continue\n", ret);
    }

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

static void set_fb_alpha(vo_output_t *out, QUA_U8 alpha_value, QUA_BOOL show)
{
    if (out->fb_dev == NULL) return;
    qua_fb_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.alpha_enable = QUA_TRUE;
    alpha.alpha_channel = QUA_FALSE;
    alpha.alpha0 = 0;
    alpha.alpha1 = alpha_value;
    alpha.global_alpha = alpha_value;
    out->fb_dev->put_alpha(out->fb_dev, &alpha);
    out->fb_dev->show(out->fb_dev, show);
}

static int init_output(const qua_mm_module_t *display_module, vo_output_t *out, const char *display_id)
{
    memset(out, 0, sizeof(*out));
    out->display_id = display_id;
    QUA_S32 ret = qua_parse_display_id(display_id, &out->vo_id, &out->vo_num,
                                       &out->fb_id, &out->fb_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s parse failed ret=%d\n", display_id, ret);
        return -1;
    }
    out->layer = out->vo_num == 1 ? 1 : 0;
    out->chn = out->vo_num;

    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, out->vo_id, out->vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open vo failed ret=%d\n", display_id, ret);
        return -1;
    }
    out->vo_dev = (qua_mm_vo_device_t *)mm_device;

    qua_vo_pub_attr_t dev_attr;
    memset(&dev_attr, 0, sizeof(dev_attr));
    dev_attr.bg_color = 0;
    dev_attr.intf_type = QUA_VO_INTF_LCD;
    dev_attr.intf_sync = QUA_VO_OUTPUT_800x1280_60;
    ret = out->vo_dev->set_public_attr(out->vo_num, &dev_attr);
    fprintf(stderr, "%s set_public_attr ret=%d\n", display_id, ret);
    ret = out->vo_dev->enable(out->vo_num);
    fprintf(stderr, "%s vo enable ret=%d\n", display_id, ret);

    if (out->vo_dev->bind_video_layer != NULL) {
        ret = out->vo_dev->bind_video_layer(out->layer, out->vo_num);
        fprintf(stderr, "%s bind layer=%d dev=%d ret=%d\n",
                display_id, out->layer, out->vo_num, ret);
    }

    qua_vo_video_layer_attr_t layer_attr;
    qua_vo_compress_attr_t compress_attr;
    memset(&layer_attr, 0, sizeof(layer_attr));
    memset(&compress_attr, 0, sizeof(compress_attr));
    layer_attr.disp_rect.x = 0;
    layer_attr.disp_rect.y = 0;
    layer_attr.disp_rect.width = SCREEN_WIDTH;
    layer_attr.disp_rect.height = SCREEN_HEIGHT;
    layer_attr.image_size.width = SCREEN_WIDTH;
    layer_attr.image_size.height = SCREEN_HEIGHT;
    layer_attr.disp_frmrt = 30;
    layer_attr.pix_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    layer_attr.double_frame = QUA_FALSE;
    layer_attr.cluster_mode = QUA_TRUE;
    compress_attr.support_compress = QUA_FALSE;
    ret = out->vo_dev->set_video_layer_attr(out->layer, &layer_attr, &compress_attr);
    fprintf(stderr, "%s set layer=%d attr ret=%d\n", display_id, out->layer, ret);
    ret = out->vo_dev->set_video_disp_buflen(out->layer, VO_BUF_NUM);
    fprintf(stderr, "%s set layer=%d buflen ret=%d\n", display_id, out->layer, ret);
    if (out->vo_dev->set_video_layer_priority != NULL) {
        ret = out->vo_dev->set_video_layer_priority(out->layer, 3);
        fprintf(stderr, "%s set layer=%d priority ret=%d\n", display_id, out->layer, ret);
    }
    ret = out->vo_dev->enable_video_layer(out->layer);
    fprintf(stderr, "%s enable layer=%d ret=%d\n", display_id, out->layer, ret);

    qua_mm_channel_t *mm_channel = NULL;
    QUA_S32 chn = out->chn;
    ret = out->vo_dev->parent.create_channel((qua_mm_device_t *)out->vo_dev, &chn, NULL, &mm_channel);
    if (ret != QUA_SUCCESS || mm_channel == NULL) {
        fprintf(stderr, "%s create vo chn=%d failed ret=%d\n", display_id, out->chn, ret);
        return -1;
    }
    out->chn = chn;
    out->vo_chn = (qua_mm_vo_channel_t *)mm_channel;

    qua_vo_chn_attr_t chn_attr;
    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.priority = 0;
    chn_attr.rect.x = 0;
    chn_attr.rect.y = 0;
    chn_attr.rect.width = SCREEN_WIDTH;
    chn_attr.rect.height = SCREEN_HEIGHT;
    chn_attr.deflicker = QUA_FALSE;
    ret = out->vo_chn->set_chn_attr(out->layer, out->chn, &chn_attr);
    fprintf(stderr, "%s set chn layer=%d chn=%d ret=%d\n",
            display_id, out->layer, out->chn, ret);
    ret = out->vo_chn->set_chn_frame_rate(out->layer, out->chn, 30);
    fprintf(stderr, "%s set fps ret=%d\n", display_id, ret);
    ret = out->vo_chn->enable_chn(out->layer, out->chn);
    fprintf(stderr, "%s enable chn ret=%d\n", display_id, ret);
    ret = out->vo_chn->clear_chn_buffer(out->layer, out->chn, QUA_TRUE);
    fprintf(stderr, "%s clear chn ret=%d\n", display_id, ret);
    ret = out->vo_chn->show_chn(out->layer, out->chn);
    fprintf(stderr, "%s show chn ret=%d\n", display_id, ret);

    mm_device = NULL;
    ret = display_module->open_device(display_module, out->fb_id, out->fb_num, &mm_device);
    if (ret == QUA_SUCCESS && mm_device != NULL) {
        out->fb_dev = (qua_mm_fb_device_t *)mm_device;
        set_fb_alpha(out, 0, QUA_TRUE);
    } else {
        fprintf(stderr, "%s open fb failed ret=%d\n", display_id, ret);
    }

    out->ready = 1;
    return 0;
}

static void deinit_output(vo_output_t *out)
{
    if (out->fb_dev != NULL) {
        set_fb_alpha(out, 255, QUA_TRUE);
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

static int alloc_frame_slots(qua_mm_system_ops_t *sys_ops, QUA_VB_POOL *pool_id,
                             frame_slot_t *slots, int count, size_t frame_size)
{
    *pool_id = sys_ops->vb_create_pool((QUA_U32)frame_size, count, NULL);
    if (*pool_id == QUA_VB_INVALID_POOLID) {
        fprintf(stderr, "vb_create_pool failed size=%u count=%d\n", (unsigned)frame_size, count);
        return -1;
    }

    for (int i = 0; i < count; i++) {
        slots[i].handle = sys_ops->vb_get_block(*pool_id, (QUA_U32)frame_size, NULL);
        if (slots[i].handle == QUA_VB_INVALID_HANDLE) {
            fprintf(stderr, "vb_get_block failed i=%d\n", i);
            return -1;
        }
        slots[i].phy = sys_ops->vb_handle_to_phyaddr(slots[i].handle);
        if (slots[i].phy == 0) {
            fprintf(stderr, "vb_handle_to_phyaddr failed i=%d\n", i);
            return -1;
        }
        slots[i].vir = (QUA_U8 *)sys_ops->sys_mmap(slots[i].phy, (QUA_U32)frame_size);
        if (slots[i].vir == NULL) {
            fprintf(stderr, "sys_mmap slot failed i=%d\n", i);
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

static int copy_decoded_frame(qua_mm_system_ops_t *sys_ops,
                              const qua_video_frame_info_t *src,
                              frame_slot_t *slot,
                              qua_video_frame_info_t *dst,
                              QUA_VB_POOL pool_id)
{
    const QUA_U32 width = SCREEN_WIDTH;
    const QUA_U32 height = SCREEN_HEIGHT;
    const QUA_U32 dst_stride = SCREEN_WIDTH;
    QUA_U32 src_stride_y = src->video_frame.stride[0] ? src->video_frame.stride[0] : width;
    QUA_U32 src_stride_uv = src->video_frame.stride[1] ? src->video_frame.stride[1] : src_stride_y;
    size_t src_map_size = (size_t)src_stride_y * height + (size_t)src_stride_uv * height / 2;

    QUA_U8 *src_map = NULL;
    QUA_U8 *src_y = (QUA_U8 *)(uintptr_t)src->video_frame.vir_addr[0];
    QUA_U8 *src_uv = (QUA_U8 *)(uintptr_t)src->video_frame.vir_addr[1];
    if (src_y == NULL) {
        src_map = (QUA_U8 *)sys_ops->sys_mmap(src->video_frame.phy_addr[0], (QUA_U32)src_map_size);
        if (src_map == NULL) {
            fprintf(stderr, "map decoded frame failed phy=%llu size=%u\n",
                    (unsigned long long)src->video_frame.phy_addr[0], (unsigned)src_map_size);
            return -1;
        }
        src_y = src_map;
        if (src->video_frame.phy_addr[1] > src->video_frame.phy_addr[0] &&
            src->video_frame.phy_addr[1] - src->video_frame.phy_addr[0] < src_map_size) {
            src_uv = src_map + (src->video_frame.phy_addr[1] - src->video_frame.phy_addr[0]);
        } else {
            src_uv = src_map + (size_t)src_stride_y * height;
        }
    } else if (src_uv == NULL) {
        src_uv = src_y + (size_t)src_stride_y * height;
    }

    QUA_U8 *dst_y = slot->vir;
    QUA_U8 *dst_uv = slot->vir + (size_t)dst_stride * height;
    for (QUA_U32 y = 0; y < height; y++) {
        memcpy(dst_y + (size_t)y * dst_stride, src_y + (size_t)y * src_stride_y, width);
    }
    for (QUA_U32 y = 0; y < height / 2; y++) {
        memcpy(dst_uv + (size_t)y * dst_stride, src_uv + (size_t)y * src_stride_uv, width);
    }

    if (src_map != NULL) {
        sys_ops->sys_munmap(src_map, (QUA_U32)src_map_size);
    }

    memset(dst, 0, sizeof(*dst));
    dst->pool_id = pool_id;
    dst->video_frame.width = width;
    dst->video_frame.height = height;
    dst->video_frame.field = QUA_VIDEO_FIELD_FRAME;
    dst->video_frame.pixel_fmt = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    dst->video_frame.video_fmt = QUA_VIDEO_FORMAT_LINEAR;
    dst->video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
    dst->video_frame.stride[0] = dst_stride;
    dst->video_frame.stride[1] = dst_stride;
    dst->video_frame.phy_addr[0] = slot->phy;
    dst->video_frame.phy_addr[1] = slot->phy + (size_t)dst_stride * height;
    dst->video_frame.vir_addr[0] = (QUA_U64)(uintptr_t)dst_y;
    dst->video_frame.vir_addr[1] = (QUA_U64)(uintptr_t)dst_uv;
    return 0;
}

int main(int argc, char **argv)
{
    setvbuf(stderr, NULL, _IONBF, 0);
    const char *path = argc > 1 ? argv[1] : "/data/res/img/video.h264";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_hw_h264_vo_overlay";
    int fps = argc > 3 ? atoi(argv[3]) : 30;
    if (fps <= 0 || fps > 60) fps = 30;

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);

    unsigned char *stream = NULL;
    size_t stream_size = 0;
    if (read_file(path, &stream, &stream_size) != 0) return 1;
    fprintf(stderr, "loaded h264 stream: %s size=%u\n", path, (unsigned)stream_size);

    qua_mm_system_ops_t *sys_ops = NULL;
    const qua_mm_module_t *display_module = NULL;
    if (init_quamm(&sys_ops, &display_module) != 0) {
        free(stream);
        return 1;
    }
    vo_output_t outputs[2];
    int output_count = 0;
    for (size_t i = 0; i < sizeof(s_display_ids) / sizeof(s_display_ids[0]); i++) {
        if (init_output(display_module, &outputs[output_count], s_display_ids[i]) == 0) {
            output_count++;
        }
    }
    if (output_count == 0) {
        free(stream);
        return 1;
    }

    void *decoder = qua_decoder_create("video/avc", NULL);
    if (decoder == NULL) {
        fprintf(stderr, "decoder create failed\n");
        for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
        free(stream);
        return 1;
    }

    qua_video_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.width = SCREEN_WIDTH;
    cfg.height = SCREEN_HEIGHT;
    cfg.fps = fps;
    cfg.bitrate = 700000;
    if (qua_decoder_prepare(decoder, &cfg) != 0 || qua_decoder_start(decoder) != 0) {
        fprintf(stderr, "decoder prepare/start failed\n");
        qua_decoder_reset(decoder);
        qua_decoder_destroy(decoder);
        for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
        free(stream);
        return 1;
    }

    fprintf(stderr, "direct vo dual h264 overlay start outputs=%d fps=%d\n", output_count, fps);
    size_t frame_size = (size_t)SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2;
    frame_slot_t slots[COPY_BUF_NUM];
    memset(slots, 0, sizeof(slots));
    for (int i = 0; i < COPY_BUF_NUM; i++) slots[i].handle = QUA_VB_INVALID_HANDLE;
    QUA_VB_POOL copy_pool = QUA_VB_INVALID_POOLID;
    if (alloc_frame_slots(sys_ops, &copy_pool, slots, COPY_BUF_NUM, frame_size) != 0) {
        qua_decoder_stop(decoder);
        qua_decoder_reset(decoder);
        qua_decoder_destroy(decoder);
        for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
        free(stream);
        return 1;
    }

    long long frame_interval = 1000000LL / fps;
    QUA_U64 pts = 0;
    unsigned int frame_count = 0;
    unsigned int nal_debug_count = 0;
    unsigned int dequeue_debug_count = 0;

    while (g_running && access(marker, F_OK) == 0) {
        size_t off = 0;
        unsigned int loop_frame_count = 0;
        long long loop_start_us = now_us();
        while (g_running && access(marker, F_OK) == 0) {
            size_t nal_start = 0;
            size_t nal_size = 0;
            if (!next_nal(stream, stream_size, &off, &nal_start, &nal_size)) break;

            int nal_type = h264_nal_type(stream, nal_start, nal_size);
            qua_packet_t pkt;
            packet_defaults(&pkt);
            pkt.data = stream + nal_start;
            pkt.size = nal_size;
            pkt.capacity = nal_size;
            pkt.pts = pts;
            pkt.dts = pts;
            pkt.duration = h264_is_vcl(nal_type) ? frame_interval : 0;
            if (nal_debug_count < 12) {
                fprintf(stderr, "queue begin nal=%u type=%d size=%u vcl=%d\n",
                        nal_debug_count + 1, nal_type, (unsigned)nal_size,
                        h264_is_vcl(nal_type));
            }
            QUA_S32 qret = qua_decoder_queue_buffer(decoder, &pkt);
            if (nal_debug_count < 12) {
                fprintf(stderr, "queue end nal=%u ret=%d\n", nal_debug_count + 1, qret);
            }
            nal_debug_count++;

            if (!h264_is_vcl(nal_type)) continue;
            pts += (QUA_U64)frame_interval;

            qua_video_frame_info_t frame;
            memset(&frame, 0, sizeof(frame));
            if (dequeue_debug_count < 20) {
                fprintf(stderr, "dequeue begin attempt=%u frame=%u nal_type=%d nal_size=%u\n",
                        dequeue_debug_count + 1, frame_count + 1, nal_type, (unsigned)nal_size);
            }
            QUA_S32 ret = qua_decoder_dequeue_buffer(decoder, &frame);
            if (dequeue_debug_count < 20) {
                fprintf(stderr, "dequeue end attempt=%u frame=%u ret=%d width=%u height=%u fmt=%d\n",
                        dequeue_debug_count + 1, frame_count + 1, ret,
                        (unsigned)frame.video_frame.width,
                        (unsigned)frame.video_frame.height,
                        frame.video_frame.pixel_fmt);
            }
            dequeue_debug_count++;
            if (ret != 0) continue;

            frame_slot_t *slot = &slots[frame_count % COPY_BUF_NUM];
            qua_video_frame_info_t display_frame;
            if (copy_decoded_frame(sys_ops, &frame, slot, &display_frame, copy_pool) != 0) {
                qua_decoder_release_buffer(decoder, &frame);
                continue;
            }
            qua_decoder_release_buffer(decoder, &frame);

            for (int i = 0; i < output_count; i++) {
                if (frame_count < 8) {
                    fprintf(stderr, "%s send begin frame=%u layer=%d chn=%d\n",
                            outputs[i].display_id, frame_count + 1,
                            outputs[i].layer, outputs[i].chn);
                }
                QUA_S32 sret = outputs[i].vo_chn->send_frame(outputs[i].layer, outputs[i].chn, &display_frame, 0);
                if (frame_count < 8) {
                    fprintf(stderr, "%s send end frame=%u ret=%d\n",
                            outputs[i].display_id, frame_count + 1, sret);
                }
                if (sret != 0) {
                    fprintf(stderr, "%s direct vo send failed layer=%d chn=%d ret=%d\n",
                            outputs[i].display_id, outputs[i].layer, outputs[i].chn, sret);
                }
            }
            frame_count++;
            loop_frame_count++;
            if (frame_count % (unsigned int)fps == 0) {
                fprintf(stderr, "frames=%u\n", frame_count);
            }

            long long target_us = loop_start_us + (long long)loop_frame_count * frame_interval;
            long long delay = target_us - now_us();
            if (delay > 2000 && delay < 100000) usleep((useconds_t)delay);
        }
        fprintf(stderr, "loop restart frames=%u\n", frame_count);
    }

    qua_decoder_stop(decoder);
    qua_decoder_reset(decoder);
    qua_decoder_destroy(decoder);
    free_frame_slots(sys_ops, copy_pool, slots, COPY_BUF_NUM);
    for (int i = 0; i < output_count; i++) deinit_output(&outputs[i]);
    free(stream);
    fprintf(stderr, "direct vo dual h264 overlay stop frames=%u\n", frame_count);
    return 0;
}
