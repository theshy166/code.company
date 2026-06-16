/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "avext/qua_media_extractor.h"
#include "avext/qua_mm_avext.h"

void printf_meta(qua_metaformat_t* tinyformat)
{
    if (!tinyformat)
        return;
    printf("[%s] meta start=============\n", tinyformat->name);
    Pairs *head = tinyformat->list;
    Pairs *cur = head;
    while(cur != NULL) {
        if (cur->valueType == TinyFormat_kTypeInt32) {
            printf("key: %s value: %d \n", cur->key, cur->u.value_int32);
        } else if (cur->valueType == TinyFormat_kTypeInt64) {
            printf("key: %s value: %d \n", cur->key, (int32_t)cur->u.value_int64);
        } else if (cur->valueType == TinyFormat_kTypeString) {
            printf("key: %s value: %s \n", cur->key, (char *)(cur->u.value));
        } else {
            printf("key: %s \n", cur->key);
        }
        cur = cur->next;
    }
    printf("[%s] meta end=============\n", tinyformat->name);
}

void quaPacket_set_default(qua_packet_t* quaPkt)
{
    memset(quaPkt->data, 0x00, quaPkt->size);
    quaPkt->offset = 0;
    quaPkt->capacity = 0;
    quaPkt->pts = -1;
    quaPkt->dts = -1;
    quaPkt->duration = 0;
    quaPkt->baseTime = 0;
    quaPkt->streamIndex = -1;
}

qua_packet_t* alloc_quaPacket(int size)
{
    qua_packet_t* quaPkt = (qua_packet_t*)calloc(1, sizeof(qua_packet_t));
    if (quaPkt == NULL)
        return NULL;

    quaPkt->data = (uint8_t*)calloc(size, sizeof(uint8_t));
    quaPkt->size = size;

    quaPacket_set_default(quaPkt);
    return quaPkt;
}

void free_quaPacket(qua_packet_t* quaPkt)
{
    if (quaPkt && quaPkt->data) {
        free(quaPkt->data);
        quaPkt->data = NULL;
    }
    free(quaPkt);
    quaPkt = NULL;
}

qua_mm_extractor_device_t* get_extractor_device(char *chip, char *os)
{
    int ret = 0;
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
    ret = ext_module->open_device(ext_module, QUA_MM_EXT_EXTRACTOR, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return NULL;
    qua_mm_extractor_device_t *extractor_device = (qua_mm_extractor_device_t *)mm_device;

    printf("Module %s, API version %d extractor_device: %p\n",
        ext_module->id, ext_module->api_version, extractor_device);
    return extractor_device;
}

int main(int argc, char* argv[])
{
    char *chip_name = NULL;
    char *os_name = NULL;
    char *src_filename = NULL;
    char *video_dst_filename = NULL;
    char *audio_dst_filename = NULL;
    FILE *video_dst_file = NULL, *audio_dst_file = NULL;
    int trackCnt = 0, videoTrackIndex = -1, audioTrackIndex = -1;
    int audioCnt = 0, videoCnt = 0;
    int audioSize = 0, videoSize = 0;
    int audioeof = 1, videoeof = 1;
    int width = 0, height = 0;
    qua_metaformat_t *meta = NULL, *audioTrackMeta = NULL, *videoTrackMeta = NULL;
    ssize_t trackSize = 0;
    qua_packet_t *quaPacket = NULL;
    qua_mm_extractor_device_t *extractor_device = NULL;
    qua_mm_channel_t *mm_chn;
    qua_mm_extractor_channel_t *extra_chn = NULL;
    QUA_S32 chn_id = 0;
    int ret = 0;

    if (argc != 6) {
        fprintf(stderr, "usage: %s  using module dlopen type or not\n"
                "input_file video_output_file audio_output_file\n"
                "API example program to show how to read frames from an input file.\n"
                "This program reads avpacket from a file, decodes them, and writes demuxer\n"
                "video packet to a video file named video_output_file, and demuxer\n"
                "audio packet to a audio file named audio_output_file.\n",
                argv[0]);
        exit(1);
    }
    chip_name = argv[1];
    os_name = argv[2];
    src_filename = argv[3];
    video_dst_filename = argv[4];
    audio_dst_filename = argv[5];
    printf("\n\n");
    printf("src_filename: %s \n", src_filename);
    printf("video_dst_filename: %s \n", video_dst_filename);
    printf("audio_dst_filename: %s \n", audio_dst_filename);
    printf("test work start >>>>\n");
    extractor_device = get_extractor_device(chip_name, os_name);
    if (extractor_device != NULL) {
        printf("using moudle type\n");
        ret = extractor_device->parent.create_channel(&extractor_device->parent, &chn_id, (QUA_VOID_PTR)src_filename, &mm_chn);
        if (ret != 0) {
            printf("create chn failed ret = %d\n", ret);
            exit(1);
        }
        extra_chn = (qua_mm_extractor_channel_t *)mm_chn;
    }
    trackCnt = extra_chn->track_count(extra_chn->extractor);
    if (trackCnt == 0) {
        printf("WARNING %s has not any track !!! \n", src_filename);
        extra_chn->parent.release(&extra_chn->parent);
        extractor_device->parent.close((qua_mm_device_t *)extractor_device);
        exit(1);
    }
    video_dst_file = fopen(video_dst_filename, "wb+");
    if (video_dst_file == NULL) {
        printf("open %s file failed\n", video_dst_filename);
        exit(1);
    }
    audio_dst_file = fopen(audio_dst_filename, "wb+");
    if (audio_dst_file == NULL) {
        printf("open %s file failed\n", audio_dst_filename);
        exit(1);
    }
    videoTrackIndex = extra_chn->get_video_track_index(extra_chn->extractor);
    audioTrackIndex = extra_chn->get_audio_track_index(extra_chn->extractor);
    printf("trackCnt: %d, videoTrackIndex: %d, audioTrackIndex: %d\n",
        trackCnt, videoTrackIndex, audioTrackIndex);

    ret = extra_chn->meta_data(extra_chn->extractor, &meta);
    if (ret == 0) {
        printf_meta(meta);
    }
    if (videoTrackIndex >= 0) {
        ret = extra_chn->track_meta_data(extra_chn->extractor, videoTrackIndex, &videoTrackMeta);

        /*void *extradata = NULL;
        size_t outSize;
        */

        videoTrackMeta->get_int32(videoTrackMeta, "width", &width);
        videoTrackMeta->get_int32(videoTrackMeta, "height", &height);
        /*
        videoTrackMeta->get_buffer(videoTrackMeta, "csd-avc", &extradata, &outSize);
        ret = fwrite((uint8_t*)extradata, 1, outSize, video_dst_file);
        printf("extradata: %p, outSize: %d, ret: %d \n", extradata, (int32_t)outSize, ret);
        */
        if (ret == 0) {
            printf_meta(videoTrackMeta);
        }
        videoeof = 0;
    }
    if (audioTrackIndex >= 0) {
        ret = extra_chn->track_meta_data(extra_chn->extractor, audioTrackIndex, &audioTrackMeta);
        if (ret == 0) {
            printf_meta(audioTrackMeta);
        }
        audioeof = 0;
    }
    if (videoTrackIndex >= 0) {
        quaPacket = alloc_quaPacket(width * height);
    } else if (audioTrackIndex >= 0) {
        quaPacket = alloc_quaPacket(128*1000);
    }
    if (quaPacket == NULL) {
        printf("alloc quaPacket fail OOM??");
        exit(1);
    }
    // uint8_t start_code[] = {0x00,0x00,0x00,0x01};
    while (true) {
        quaPacket_set_default(quaPacket);
        if (videoTrackIndex >= 0 && videoeof == 0) {
            trackSize = extra_chn->read_sample_data(extra_chn->extractor, videoTrackIndex, quaPacket);
            if (trackSize > 0) {
                videoSize += quaPacket->capacity;
                // printf("[video] == trackSizeTotle: %d, pts: %ld\n", quaPacket->capacity, quaPacket->pts);
                if (quaPacket->streamIndex == videoTrackIndex) {
                    //fwrite(start_code, 1, 4, video_dst_file);
                    fwrite(quaPacket->data, 1, quaPacket->capacity, video_dst_file);
                }
                videoCnt++;
            } else {
                printf("video EOF!\n");
                videoeof = 1;
            }
        }
        quaPacket_set_default(quaPacket);
        if (audioTrackIndex >= 0 && audioeof == 0) {
            trackSize = extra_chn->read_sample_data(extra_chn->extractor, audioTrackIndex, quaPacket);
            if (trackSize > 0) {
                audioSize += quaPacket->capacity;
                // printf("[audio] == trackSizeTotle: %d, pts: %ld\n", audioSize, quaPacket->pts);
                if (quaPacket->streamIndex == audioTrackIndex) {
                    fwrite(quaPacket->data, 1, quaPacket->capacity, audio_dst_file);
                }
                audioCnt++;
            } else {
                printf("audio EOF!\n");
                audioeof = 1;
            }
        }
        if (audioeof == 1 && videoeof == 1)
            break;
    };
    free_quaPacket(quaPacket);
    extra_chn->parent.release(&extra_chn->parent);
    extractor_device->parent.close((qua_mm_device_t *)extractor_device);

    printf("test work end audioCnt = %d, audioSize = %d, videoCnt = %d, videoSize = %d<<<<\n", audioCnt, audioSize, videoCnt, videoSize);
    if (audio_dst_file)
        fclose(audio_dst_file);
    if (video_dst_file)
        fclose(video_dst_file);
    printf("well done. good job!\n\n\n");

    return 0;
}
