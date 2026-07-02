#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "common/qua_sys_platform.h"
#include "common/qua_type.h"
#include "display/qua_mm_display.h"
#include "player/qua_decoder_inf.h"
#include "player/qua_mm_player_common.h"
#include "player/qua_vppo_inf.h"
#include "system/qua_mm_system.h"
#include "utils/qua_display_parser.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 1280

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
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }
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
    if (remain >= 4 && p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1) {
        return 4;
    }
    if (remain >= 3 && p[0] == 0 && p[1] == 0 && p[2] == 1) {
        return 3;
    }
    return 0;
}

static size_t find_start_code(const unsigned char *buf, size_t size, size_t from)
{
    for (size_t i = from; i + 3 < size; i++) {
        if (start_code_len(buf + i, size - i) > 0) {
            return i;
        }
    }
    return size;
}

static int next_nal(const unsigned char *buf, size_t size, size_t *offset,
                    size_t *nal_start, size_t *nal_size)
{
    size_t pos = find_start_code(buf, size, *offset);
    if (pos >= size) {
        return 0;
    }

    int sc = start_code_len(buf + pos, size - pos);
    size_t next = find_start_code(buf, size, pos + (size_t)sc);
    *nal_start = pos;
    *nal_size = next - pos;
    *offset = next;
    return *nal_size > 0;
}

static void packet_defaults(qua_packet_t *pkt)
{
    pkt->offset = 0;
    pkt->pts = -1;
    pkt->dts = -1;
    pkt->duration = 0;
    pkt->baseTime = 0;
    pkt->streamIndex = -1;
    pkt->end_of_stream = 0;
}

static int h264_nal_type(const unsigned char *buf, size_t nal_start, size_t nal_size)
{
    int sc = start_code_len(buf + nal_start, nal_size);
    if (sc <= 0 || (size_t)sc >= nal_size) {
        return -1;
    }
    return buf[nal_start + (size_t)sc] & 0x1f;
}

static int h264_is_vcl(int nal_type)
{
    return nal_type >= 1 && nal_type <= 5;
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
        fprintf(stderr, "sys_init returned %d, continue\n", ret);
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

static int set_fb_visible(const qua_mm_module_t *display_module, const char *display_id, QUA_BOOL visible)
{
    QUA_CONST_CHAR *vo_id = NULL;
    QUA_CONST_CHAR *fb_id = NULL;
    QUA_S32 vo_num = 0;
    QUA_S32 fb_num = 0;
    if (qua_parse_display_id(display_id, &vo_id, &vo_num, &fb_id, &fb_num) != QUA_SUCCESS) {
        return -1;
    }

    qua_mm_device_t *mm_device = NULL;
    QUA_S32 ret = display_module->open_device(display_module, fb_id, fb_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open fb failed show=%d ret=%d\n", display_id, visible, ret);
        return -1;
    }

    qua_mm_fb_device_t *fb_dev = (qua_mm_fb_device_t *)mm_device;
    ret = fb_dev->show(fb_dev, visible);
    fb_dev->parent.close((qua_mm_device_t *)fb_dev);
    (void)vo_id;
    (void)vo_num;
    return ret == QUA_SUCCESS ? 0 : -1;
}

static int init_vo(const qua_mm_module_t *display_module, const char *display_id)
{
    QUA_CONST_CHAR *vo_id = NULL;
    QUA_CONST_CHAR *fb_id = NULL;
    QUA_S32 vo_num = 0;
    QUA_S32 fb_num = 0;
    QUA_S32 ret = qua_parse_display_id(display_id, &vo_id, &vo_num, &fb_id, &fb_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s parse display failed: %d\n", display_id, ret);
        return -1;
    }

    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, vo_id, vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open vo failed: %d\n", display_id, ret);
        return -1;
    }

    qua_mm_vo_device_t *vo_dev = (qua_mm_vo_device_t *)mm_device;
    qua_vo_pub_attr_t dev_attr;
    memset(&dev_attr, 0, sizeof(dev_attr));
    dev_attr.intf_type = QUA_VO_INTF_LCD;
    dev_attr.bg_color = 0;
    dev_attr.intf_sync = QUA_VO_OUTPUT_800x1280_60;
    ret = vo_dev->set_public_attr(vo_num, &dev_attr);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s set_public_attr ret=%d, continue\n", display_id, ret);
    }
    ret = vo_dev->enable(vo_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s vo enable ret=%d, continue\n", display_id, ret);
    }
    vo_dev->parent.close((qua_mm_device_t *)vo_dev);
    (void)fb_id;
    (void)fb_num;
    return 0;
}

static int init_vppo_output(void **out, const char *display_id, int width, int height)
{
    void *vppo = qua_vppo_init((QUA_CONST_CHAR *)display_id);
    if (vppo == NULL) {
        fprintf(stderr, "%s qua_vppo_init failed\n", display_id);
        return -1;
    }

    qua_rect_t display_rect = {0, 0, (QUA_U32)width, (QUA_U32)height};
    qua_rect_t chn_rect = {0, 0, (QUA_U32)width, (QUA_U32)height};
    qua_vppo_set_param(vppo, KEY_PARAMETER_VO_DISPLAY_RECT, &display_rect);
    qua_vppo_set_param(vppo, KEY_PARAMETER_VO_CHN_RECT, &chn_rect);

    QUA_S32 ret = qua_vppo_prepare(vppo);
    if (ret != 0) {
        fprintf(stderr, "%s qua_vppo_prepare failed: %d\n", display_id, ret);
        qua_vppo_deinit(vppo);
        return -1;
    }
    ret = qua_vppo_start(vppo);
    if (ret != 0) {
        fprintf(stderr, "%s qua_vppo_start failed: %d\n", display_id, ret);
        qua_vppo_reset(vppo);
        qua_vppo_deinit(vppo);
        return -1;
    }

    fprintf(stderr, "%s vppo ready\n", display_id);
    *out = vppo;
    return 0;
}

static void stop_vppo(void *vppo)
{
    if (vppo != NULL) {
        qua_vppo_stop(vppo);
        qua_vppo_reset(vppo);
        qua_vppo_deinit(vppo);
    }
}

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "/data/res/img/video.h264";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_hw_h264_overlay";
    int mode_both = argc > 3 ? atoi(argv[3]) : 1;
    int fps = argc > 4 ? atoi(argv[4]) : 30;
    int hide_fb = argc > 5 ? atoi(argv[5]) : 1;
    int width = argc > 6 ? atoi(argv[6]) : DEFAULT_SCREEN_WIDTH;
    int height = argc > 7 ? atoi(argv[7]) : DEFAULT_SCREEN_HEIGHT;
    if (fps <= 0 || fps > 60) {
        fps = 30;
    }
    if (width <= 0 || width > DEFAULT_SCREEN_WIDTH) {
        width = DEFAULT_SCREEN_WIDTH;
    }
    if (height <= 0 || height > DEFAULT_SCREEN_HEIGHT) {
        height = DEFAULT_SCREEN_HEIGHT;
    }

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);

    unsigned char *stream = NULL;
    size_t stream_size = 0;
    if (read_file(path, &stream, &stream_size) != 0) {
        return 1;
    }
    fprintf(stderr, "loaded h264 stream: %s size=%u\n", path, (unsigned)stream_size);

    qua_mm_system_ops_t *sys_ops = NULL;
    const qua_mm_module_t *display_module = NULL;
    if (init_quamm(&sys_ops, &display_module) != 0) {
        free(stream);
        return 1;
    }
    (void)sys_ops;

    int output_count = mode_both ? 2 : 1;
    void *vppos[2] = {NULL, NULL};
    for (int i = 0; i < output_count; i++) {
        init_vo(display_module, s_display_ids[i]);
        if (hide_fb) {
            set_fb_visible(display_module, s_display_ids[i], QUA_FALSE);
        }
        if (init_vppo_output(&vppos[i], s_display_ids[i], width, height) != 0) {
            for (int j = 0; j < i; j++) {
                stop_vppo(vppos[j]);
            }
            for (int j = 0; j < output_count; j++) {
                if (hide_fb) set_fb_visible(display_module, s_display_ids[j], QUA_TRUE);
            }
            free(stream);
            return 1;
        }
    }

    void *decoder = qua_decoder_create("video/avc", NULL);
    if (decoder == NULL) {
        fprintf(stderr, "decoder create failed\n");
        for (int i = 0; i < output_count; i++) stop_vppo(vppos[i]);
        free(stream);
        return 1;
    }

    qua_video_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.width = (QUA_U32)width;
    cfg.height = (QUA_U32)height;
    cfg.fps = fps;
    cfg.bitrate = 900000;
    if (qua_decoder_prepare(decoder, &cfg) != 0 || qua_decoder_start(decoder) != 0) {
        fprintf(stderr, "decoder prepare/start failed\n");
        qua_decoder_reset(decoder);
        qua_decoder_destroy(decoder);
        for (int i = 0; i < output_count; i++) stop_vppo(vppos[i]);
        free(stream);
        return 1;
    }

    fprintf(stderr, "dual h264 overlay start outputs=%d fps=%d hide_fb=%d size=%dx%d\n",
            output_count, fps, hide_fb, width, height);

    long long frame_interval = 1000000LL / fps;
    QUA_U64 pts = 0;
    unsigned int frame_count = 0;

    while (g_running && access(marker, F_OK) == 0) {
        size_t off = 0;
        unsigned int loop_frame_count = 0;
        long long loop_start_us = now_us();
        while (g_running && access(marker, F_OK) == 0) {
            size_t nal_start = 0;
            size_t nal_size = 0;
            if (!next_nal(stream, stream_size, &off, &nal_start, &nal_size)) {
                break;
            }

            int nal_type = h264_nal_type(stream, nal_start, nal_size);
            qua_packet_t pkt;
            memset(&pkt, 0, sizeof(pkt));
            packet_defaults(&pkt);
            pkt.data = stream + nal_start;
            pkt.size = nal_size;
            pkt.capacity = nal_size;
            pkt.pts = pts;
            pkt.dts = pts;
            pkt.duration = h264_is_vcl(nal_type) ? frame_interval : 0;
            qua_decoder_queue_buffer(decoder, &pkt);

            if (!h264_is_vcl(nal_type)) {
                continue;
            }
            pts += (QUA_U64)frame_interval;

            {
                qua_video_frame_info_t frame;
                memset(&frame, 0, sizeof(frame));
                QUA_S32 ret = qua_decoder_dequeue_buffer(decoder, &frame);
                if (ret != 0) {
                    continue;
                }

                for (int i = 0; i < output_count; i++) {
                    QUA_S32 sret = qua_vppo_send_frame(vppos[i], &frame);
                    if (sret != 0) {
                        fprintf(stderr, "%s send_frame failed: %d\n", s_display_ids[i], sret);
                    }
                }
                qua_decoder_release_buffer(decoder, &frame);
                frame_count++;
                loop_frame_count++;
                if (frame_count % (unsigned int)fps == 0) {
                    fprintf(stderr, "frames=%u\n", frame_count);
                }

                long long target_us = loop_start_us + (long long)loop_frame_count * frame_interval;
                long long delay = target_us - now_us();
                if (delay > 2000 && delay < 100000) {
                    usleep((useconds_t)delay);
                }
            }
        }
        fprintf(stderr, "loop restart frames=%u\n", frame_count);
    }

    qua_decoder_stop(decoder);
    qua_decoder_reset(decoder);
    qua_decoder_destroy(decoder);
    for (int i = 0; i < output_count; i++) {
        stop_vppo(vppos[i]);
        if (hide_fb) {
            set_fb_visible(display_module, s_display_ids[i], QUA_TRUE);
        }
    }
    free(stream);
    fprintf(stderr, "dual h264 overlay stop frames=%u\n", frame_count);
    return 0;
}
