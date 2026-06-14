/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "audio/qua_mm_audio.h"
#include "system/qua_mm_system.h"

enum {
    TEST_NULL     = 0,
    TEST_PLAYBACK = 1,
    TEST_CAPTURE  = 2,
    TEST_LOOPBACK = 3,
};

static int g_dev_id = QUA_DEV_ACW0;
#define SAMPLES_PER_FRAME 320 // 40ms
#define DEFAULT_TIMEOUT 10 // 10secs

static qua_mm_audio_device_t* get_audio_device(char *chip, char *os)
{
    QUA_S32 ret = 0;
    QUA_CHAR platform[32] = { 0 };
    ret = qua_make_platform(chip, os, platform, sizeof(platform));
    if (ret == QUA_FAILURE) {
        printf("make platform name error!\n");
        return NULL;
    }
    const qua_mm_module_t *ext_module = NULL;
    qua_mm_system_t *system = NULL;
    ret = qua_mm_init(QUA_TRUE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        ALOGD("system module init fail");
        return NULL;
    }
    ret = qua_mm_load_module(QUA_MM_MODULE_AUDIO, &ext_module);
    if (ret != QUA_SUCCESS || ext_module == NULL)
        return NULL;

    printf("Module %s, API version %d\n", ext_module->id, ext_module->api_version);
    qua_mm_device_t *mm_device = NULL;
    ret = ext_module->open_device(ext_module, QUA_MM_AUDIO_DEV, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return NULL;
    qua_mm_audio_device_t *audio_device = (qua_mm_audio_device_t *)mm_device;

    printf("Module %s, API version %d audio_device: %p\n",
        ext_module->id, ext_module->api_version, audio_device);
    return audio_device;
}

static QUA_S32 file_playback(qua_mm_audio_device_t *audio_dev, char *src, QUA_S32 sr,
                        QUA_S32 chns, QUA_S32 bitwidth, QUA_S32 timeout_sec)
{
    FILE * playback_fd = NULL;
    qua_audio_cfg_arg audio_cfg;
    QUA_U8 *buf = NULL;
    QUA_U32 buf_size = 0;
    QUA_U32 total_size = 0;
    QUA_S32 ret = QUA_SUCCESS;

    playback_fd = fopen(src, "rb");
    if (playback_fd == NULL) {
        printf("the source file is error \n");
        return QUA_FAILURE;
    }

    audio_cfg.io_type = QUA_LINE_OUT;
    audio_cfg.frame_size = SAMPLES_PER_FRAME;
    audio_cfg.frame_cnt = 6;
    audio_cfg.channels = chns;
    audio_cfg.channel_bit = bitwidth;
    audio_cfg.rate = sr;

    audio_dev->play_init(g_dev_id, &audio_cfg);
    audio_dev->play_start(g_dev_id);
    buf_size = audio_cfg.frame_size * (audio_cfg.channel_bit / 8) * audio_cfg.channels;
    buf = (QUA_U8 *)calloc(1, buf_size);
    if (buf == NULL) {
        printf("buf is NULL \n");
        return QUA_FAILURE;
    }
    total_size = timeout_sec * sr * (audio_cfg.channel_bit / 8) * audio_cfg.channels;
    while (1) {
        memset(buf, 0x00, buf_size);
        if(fread(buf, 1, buf_size, playback_fd) != buf_size) {
            break;
        }
        ret = audio_dev->write_frame(g_dev_id, buf, buf_size);
        if (ret != QUA_SUCCESS) {
            printf("write frame fail \n");
            usleep(20*1000);
        }
        total_size -= buf_size;
        if (total_size <= 0)
            break;
    }
    if (buf) {
        free(buf);
        buf = NULL;
    }
    audio_dev->play_stop(g_dev_id);
    audio_dev->play_deinit(g_dev_id);
    if (playback_fd) {
        fclose(playback_fd);
        playback_fd = NULL;
    }
    return QUA_SUCCESS;
}

static QUA_S32 file_capture(qua_mm_audio_device_t *audio_dev, char *src, QUA_S32 sr,
                        QUA_S32 chns, QUA_S32 bitwidth, QUA_S32 timeout_sec)
{
    FILE * capture_fd = NULL;
    qua_audio_cfg_arg audio_cfg;
    QUA_U8 *buf = NULL;
    QUA_U32 buf_size = 0;
    QUA_S32 total_size = 0;
    QUA_S32 ret = QUA_SUCCESS;
    QUA_U64 pts = 0;

    capture_fd = fopen(src, "wb+");
    if (capture_fd == NULL) {
        printf("the source file is error \n");
        return QUA_FAILURE;
    }
    audio_cfg.io_type = QUA_MIC_IN;
    audio_cfg.frame_size = SAMPLES_PER_FRAME;
    audio_cfg.frame_cnt = 6;
    audio_cfg.channels = chns;
    audio_cfg.channel_bit = bitwidth;
    audio_cfg.rate = sr;

    audio_dev->capture_init(g_dev_id, &audio_cfg);
    audio_dev->capture_start(g_dev_id);
    buf_size = audio_cfg.frame_size * (audio_cfg.channel_bit / 8) * audio_cfg.channels;
    buf = (QUA_U8 *)calloc(1, buf_size);
    if (buf == NULL) {
        printf("buf is NULL \n");
        return QUA_FAILURE;
    }
    total_size = timeout_sec * sr * (audio_cfg.channel_bit / 8) * audio_cfg.channels;
    while (1) {
        memset(buf, 0x00, audio_cfg.frame_size);
        ret = audio_dev->read_frame(g_dev_id, buf, buf_size, &pts);
        if (ret != QUA_SUCCESS) {
            printf("read frame fail \n");
            usleep(20*1000);
            continue;
        }
        if (audio_cfg.frame_size > 0) {
            fwrite(buf, 1, buf_size, capture_fd);
            total_size -= buf_size;
        }
        if (total_size <= 0)
            break;
    }
    if (buf) {
        free(buf);
        buf = NULL;
    }
    audio_dev->capture_stop(g_dev_id);
    audio_dev->capture_deinit(g_dev_id);
    if (capture_fd) {
        fclose(capture_fd);
        capture_fd = NULL;
    }
    return QUA_SUCCESS;
}

static QUA_S32 file_loopback(qua_mm_audio_device_t *audio_dev, char *src, QUA_S32 sr,
                    QUA_S32 chns, QUA_S32 bitwidth, QUA_S32 timeout_sec, char *ref_src)
{
    FILE * capture_fd = NULL;
    FILE * playback_fd = NULL;
    qua_audio_cfg_arg audio_cfg;
    QUA_U8 *buf = NULL;
    QUA_U32 buf_size = 0;
    QUA_S32 total_size = 0;
    QUA_S32 ret = QUA_SUCCESS;
    QUA_U64 pts = 0;

    capture_fd = fopen(src, "wb+");
    if (capture_fd == NULL) {
        printf("the source file is error \n");
        return QUA_FAILURE;
    }
    playback_fd = fopen(ref_src, "rb");
    if (playback_fd == NULL) {
        printf("the source file is error \n");
        return QUA_FAILURE;
    }
    audio_cfg.io_type = QUA_LINE_OUT;
    audio_cfg.frame_size = SAMPLES_PER_FRAME;
    audio_cfg.frame_cnt = 6;
    audio_cfg.channels = chns;
    audio_cfg.channel_bit = bitwidth;
    audio_cfg.rate = sr;

    audio_dev->play_init(g_dev_id, &audio_cfg);
    audio_cfg.io_type = QUA_MIC_IN;
    audio_dev->capture_init(g_dev_id, &audio_cfg);
    buf_size = audio_cfg.frame_size * (audio_cfg.channel_bit / 8) * audio_cfg.channels;
    buf = (QUA_U8 *)calloc(1, buf_size);
    if (buf == NULL) {
        printf("buf is NULL \n");
        return QUA_FAILURE;
    }
    total_size = timeout_sec * sr * (audio_cfg.channel_bit / 8) * audio_cfg.channels;
    while (1) {
        memset(buf, 0x00, audio_cfg.frame_size);
        ret = audio_dev->read_frame(g_dev_id, buf, buf_size, &pts);
        if (ret != QUA_SUCCESS) {
            printf("read frame fail \n");
            usleep(20*1000);
            continue;
        }
        if (audio_cfg.frame_size > 0) {
            fwrite(buf, 1, buf_size, capture_fd);
            total_size -= buf_size;
        }
        memset(buf, 0x00, buf_size);
        if(fread(buf, 1, buf_size, playback_fd) != buf_size) {
            break;
        }
        ret = audio_dev->write_frame(g_dev_id, buf, buf_size);
        if (ret != QUA_SUCCESS) {
            printf("write frame fail \n");
            usleep(20*1000);
            continue;
        }
        if (total_size <= 0)
            break;
    }
    if (buf) {
        free(buf);
        buf = NULL;
    }
    // audio_dev->capture_stop(g_dev_id);
    audio_dev->capture_deinit(g_dev_id);
    // audio_dev->play_stop(g_dev_id);
    audio_dev->play_deinit(g_dev_id);
    if (capture_fd) {
        fclose(capture_fd);
        capture_fd = NULL;
    }
    if (playback_fd) {
        fclose(playback_fd);
        playback_fd = NULL;
    }
    return QUA_SUCCESS;
}

int main(int argc, char* argv[])
{
    char *chip_name = NULL;
    char *os_name = NULL;
    qua_mm_audio_device_t *audio_dev;
    char *src_filename = NULL;
    char *ref_filename = NULL;
    QUA_S32 sr, chns, bitwidth;
    QUA_S32 test_type = 0;
    QUA_S32 timeout = DEFAULT_TIMEOUT;

    if (argc < 9) {
        fprintf(stderr, "usage: %s with input_file \n", argv[0]);
        fprintf(stderr, "playback ./qua_audio_sample qm10xv linux 1 record_pcm_16000_1_16.pcm 16000 1 16 10 \n");
        fprintf(stderr, "capture ./qua_audio_sample qm10xv linux 2 record_pcm_16000_1_16.pcm 16000 1 16 10 \n");
        fprintf(stderr, "loopback ./qua_audio_sample qm10xv linux 3 record_pcm_16000_1_16.pcm 16000 1 16 10 ref.pcm \n");
        exit(1);
    }
    chip_name = argv[1];
    os_name = argv[2];
    test_type = atoi(argv[3]);
    src_filename = argv[4];
    sr = atoi(argv[5]);
    chns = atoi(argv[6]);
    bitwidth = atoi(argv[7]);
    timeout = atoi(argv[8]);
    timeout = (timeout == 0) ? DEFAULT_TIMEOUT : timeout;
    ref_filename = argv[9];

    printf("test_type: %s \n", (test_type == TEST_PLAYBACK) ? "playback" : ((test_type == TEST_CAPTURE) ? "recoder" : "loopback"));
    printf("file name: %s \n", src_filename);
    printf("sr: %d, chns: %d, bitwidth: %d, timeout: %d sec\n", sr, chns, bitwidth, timeout);

    audio_dev = get_audio_device(chip_name, os_name);
    if (audio_dev == NULL) {
        printf("audio_dev is null \n");
        exit(1);
    }
    switch (test_type)
    {
    case TEST_PLAYBACK:
        file_playback(audio_dev, src_filename, sr, chns, bitwidth, timeout);
        break;
    case TEST_CAPTURE:
        file_capture(audio_dev, src_filename, sr, chns, bitwidth, timeout);
        break;
    case TEST_LOOPBACK:
        file_loopback(audio_dev, src_filename, sr, chns, bitwidth, timeout, ref_filename);
        break;
    case TEST_NULL:
    default:
        printf("input the error test type \n");
        break;
    }
    // delete audio dev
    audio_dev->parent.close((qua_mm_device_t *)audio_dev);
    printf("well done, good job \n");
    return 0;
}