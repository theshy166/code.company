#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include "utils/qua_display_parser.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "player/qua_vppo_inf.h"
#include "player/qua_decoder_inf.h"
#include "player/qua_mm_player_common.h"

static QUA_VOID set_video_packet_default(qua_packet_t* pkt)
{
    memset(pkt->data, 0x00, pkt->size);
    pkt->offset = 0;
    pkt->capacity = 0;
    pkt->pts = -1;
    pkt->dts = -1;
    pkt->duration = 0;
    pkt->baseTime = 0;
    pkt->streamIndex = -1;
    pkt->end_of_stream = 0;
}

static qua_packet_t *alloc_video_packet(QUA_U64 size)
{
    qua_packet_t* pkt = (qua_packet_t*)calloc(1, sizeof(qua_packet_t));
    if (pkt == NULL)
        return NULL;

    pkt->data = (uint8_t*)calloc(size, sizeof(uint8_t));
    pkt->size = size;

    set_video_packet_default(pkt);
    return pkt;
}

static QUA_VOID free_video_packet(qua_packet_t *pkt)
{
    if (pkt && pkt->data) {
        free(pkt->data);
        pkt->data = NULL;
    }
    free(pkt);
}

static int32_t qua_h2645_split_codec_nal(QUA_U8 *data, QUA_U32 size, QUA_U32 *nalSize) {
    if (data == NULL || size < 4) {
        return -1;
    }

    QUA_U8 nal_length = 4;
    if (memcmp(data, "\x00\x00\x00\x01", 4)) {
        if (memcmp(data, "\x00\x00\x01", 3)) {
            return -1;
        }
        nal_length = 3;
    }

    if (size < 10) {
        *nalSize = size;
        return 0;
    }

    const QUA_U8 *nalu_data = data + nal_length;
    QUA_U32 nalu_size = size - nal_length;
    QUA_U8 *nextStart = NULL;

    nextStart = (QUA_U8 *)memmem(nalu_data, nalu_size - 1, "\x00\x00\x00\x01", 4);
    if (nextStart != NULL && nextStart < nalu_data + nalu_size - 4) {
        *nalSize = nextStart - nalu_data + 4;
    } else {
        *nalSize = size;
    }

    return 0;
}

static const QUA_CHAR* display_id[] = {
    "id:display0",
    "id:display1",
    "id:display2",
    "id:display3",
};

int main(int argc, char *argv[])
{
    QUA_S32 ret = 0;
    qua_mm_system_t *system = NULL;
    qua_mm_system_ops_t *sys_ops;
    qua_mm_vo_device_t *vo_dev;
    const qua_mm_module_t *disp_module = NULL;
    qua_mm_device_t *mm_device = NULL;
    qua_vo_pub_attr_t dev_attr = { 0 };
    QUA_U32 blkSize;
    qua_vb_config_t vbCfg;
    QUA_CONST_CHAR *vo_id = NULL;
    QUA_CONST_CHAR *fb_id = NULL;
    QUA_S32 vo_num;
    QUA_S32 fb_num;
    QUA_S32 display_idx = 0;

    QUA_VOID *video_dec = NULL;
    QUA_VOID *display = NULL;
    qua_video_config_t video_cfg;
    qua_rect_t rect = { };
    qua_video_frame_info_t video_frame_info;
    qua_packet_t *video_pkt;
    QUA_U32 buf_len = 0;
    FILE *fp_strm = NULL;
    char *chip_name = NULL;
    char *os_name = NULL;
    char *src_filename = NULL;
    QUA_U64 pts = 0;
    QUA_CHAR platform[32] = { 0 };
    QUA_U32 used_bytes = 0;

    chip_name = argv[1];
    os_name = argv[2];
    src_filename = argv[3];

    // sys related
    ret = qua_make_platform(chip_name, os_name, platform, sizeof(platform));
    if (ret == QUA_FAILURE) {
        printf("make platform name error!\n");
        exit(1);
    }
    ret = qua_mm_init(QUA_TRUE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        printf("qua_mm_init error!\n");
        return ret;
    }
    sys_ops = (qua_mm_system_ops_t *)system;
    memset(&vbCfg, 0, sizeof(qua_vb_config_t));
    blkSize = 1024;
    vbCfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vbCfg.common_pools[0].block_size = blkSize;
    vbCfg.common_pools[0].block_cnt = 6;
    ret = sys_ops->sys_init(&vbCfg);
    if (ret != QUA_SUCCESS) {
        printf("sys_init error!\n");
        return ret;
    }

    // vo module related
    ret = qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &disp_module);
    if (ret != QUA_SUCCESS || disp_module == NULL) {
        printf("ret = %#x, get disp_module fail", ret);
        exit(1);
    }
    mm_device = NULL;
    qua_init_display_parser();
    ret = qua_parse_display_id(display_id[display_idx], &vo_id, &vo_num, &fb_id, &fb_num);
    printf("vo_id = %s vo_num = %d", vo_id, vo_num);
    if (ret != QUA_SUCCESS) {
        printf("%s display_id = %s error", __func__, display_id[display_idx]);
        exit(1);
    }
    ret = disp_module->open_device(disp_module, vo_id, vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("ret = %#x, get vo_dev fail", ret);
        exit(1);
    }
    vo_dev = (qua_mm_vo_device_t *)mm_device;

    dev_attr.intf_type = QUA_VO_INTF_LCD;
    dev_attr.bg_color = 0xffffff;
    dev_attr.intf_sync = QUA_VO_OUTPUT_1024x600_60; // no matters
    ret = vo_dev->set_public_attr(vo_num, &dev_attr);
    if (ret != QUA_SUCCESS) {
        printf("vo_dev set attr fail ret = %#x", ret);
        exit(1);
    }
    ret = vo_dev->enable(vo_num);
    if (ret != QUA_SUCCESS) {
        printf("vo_dev enable chn fail ret = %#x", ret);
        exit(1);
    }

    // player start
    fp_strm = fopen(src_filename, "rb");
    if (fp_strm == NULL) {
        printf("open file failed\n");
        return -1;
    }
    // H264
    video_dec = qua_decoder_create("video/avc", NULL);
    if (video_dec == NULL) {
        printf("qua_decoder_create failed\n");
        return -1;
    }
    printf("qua_decoder_create ok\n");
    display = qua_vppo_init("id:display0");
    if (display == NULL) {
        printf("qua_vppo_init failed\n");
        return -1;
    }
    printf("qua_vppo_init ok\n");
    video_cfg.width = 320;
    video_cfg.height = 240;
    video_cfg.fps = 25;
    video_cfg.bitrate = 128000;
#if 0
    video_cfg.profile = profile;
    video_cfg.level = level;
    // sps pps
    if (extra_data_size > 0) {
        memcpy(video_cfg.extra_data, (uint8_t*)extra_data, extra_data_size);
        video_cfg.extra_data_size = extra_data_size;
    }
#endif

    ret = qua_decoder_prepare(video_dec, &video_cfg);
    if (ret != 0) {
        printf("qua_decoder_prepare failed\n");
        return -1;
    }
    printf("qua_decoder_prepare ok\n");
    rect.x = 0;
    rect.y = 0;
    rect.width = 320;
    rect.height = 240;
    qua_vppo_set_param(display, KEY_PARAMETER_VO_DISPLAY_RECT, (QUA_VOID_PTR)&rect);
    qua_vppo_set_param(display, KEY_PARAMETER_VO_CHN_RECT, (QUA_VOID_PTR)&rect);
    ret = qua_vppo_prepare(display);
    if (ret != 0) {
        printf("qua_vppo_prepare failed\n");
        return -1;
    }
    printf("qua_vppo_prepare ok\n");
    ret = qua_decoder_start(video_dec);
    if ( ret!= 0) {
        printf("qua_decoder_start failed\n");
        return -1;
    }
    ret = qua_vppo_start(display);
    if (ret != 0) {
        printf("qua_vppo_start failed\n");
        return -1;
    }
    // max input size 
    video_pkt = alloc_video_packet(video_cfg.width * video_cfg.height);
    if (video_pkt == NULL) {
        printf("alloc_video_packet failed\n");
        return -1;
    }
    buf_len = video_cfg.width * video_cfg.height;
    do {
        QUA_U32 nalu_size = 0;
        set_video_packet_default(video_pkt);
        fseek(fp_strm, used_bytes, SEEK_SET);
        QUA_U32 read_len = fread(video_pkt->data, 1, buf_len, fp_strm);
        if (read_len == 0) {
            printf("EOF\n");
            used_bytes = 0;
            continue;
        }
        qua_h2645_split_codec_nal(video_pkt->data, buf_len, &nalu_size);
        used_bytes += nalu_size;
        video_pkt->capacity = nalu_size;
        video_pkt->dts = video_pkt->pts = pts;
        pts += 40000;
        ret = qua_decoder_queue_buffer(video_dec, video_pkt);
        // if (ret != 0) {
        //     printf("qua_decoder_queue_buffer failed, ret = %#x\n", ret);
        //     if (ret == QUA_EAGAIN)
        //         continue;
        //     break;
        // }
        ret = qua_decoder_dequeue_buffer(video_dec, &video_frame_info);
        if (ret != 0) {
            printf("qua_decoder_dequeue_buffer warning, ret = %#x\n", ret);
        } else {
            ret = qua_vppo_send_frame(display, &video_frame_info);
            if (ret != 0) {
                printf("qua_vppo_send_frame failed, ret = %#x\n", ret);
                break;
            }
        }
        ret = qua_decoder_release_buffer(video_dec, &video_frame_info);
        if (ret != 0) {
            printf("qua_decoder_release_buffer failed, ret = %#x\n", ret);
            break;
        }
    } while(1);

    ret = qua_vppo_stop(display);
    if (ret != 0) {
        printf("qua_vppo_stop failed, ret = %#x\n", ret);
        return -1;
    }
    ret = qua_decoder_stop(video_dec);
    if (ret != 0) {
        printf("qua_decoder_stop failed, ret = %#x\n", ret);
        return -1;
    }
    qua_decoder_reset(video_dec);
    qua_decoder_destroy(video_dec);
    video_dec = NULL;
    qua_vppo_reset(display);
    qua_vppo_deinit(display);
    display = NULL;
    free_video_packet(video_pkt);
    video_pkt = NULL;
    if (fp_strm) {
        fclose(fp_strm);
        fp_strm = NULL;
    }

    // release vo device
    if (vo_dev) {
        vo_dev->disable(vo_num);
        vo_dev->parent.close((qua_mm_device_t *)vo_dev);
        vo_dev = NULL;
    }
    printf("well done\n");
    return 0;
}