/*
 *  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "avext/qua_media_encoder.h"
#include "avext/qua_mm_avext.h"

#define WAV_HEADER_SIZE 44

#define STRIDE_ALIGN_UP(x, a)      ((x+a-1)&(~(a-1)))

int sample_rate_idx(int sample_rate) {
    int i = 0;
    int sample_rates[] = {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000};
    for (i = 0; i < sizeof(sample_rates) / sizeof(sample_rates[0]); i++) {
        if (sample_rate == sample_rates[i]) {
            return i;
        }
    }
    return -1; // sample rate not supported
}

void get_adts_header(uint8_t *header, int size, int sample_rate, int chns)
{
    // (void)sample_rate;
    // (void)chns;
    int profile = 2;  // AAC LC
    int freqIdx = sample_rate_idx(sample_rate);  // 44.1KHz
    int chanCfg = chns;  // Stereo

    int fullLength = size + 7; // 7 = header length
    int adtsLength = fullLength - 7;

    header[0] = 0xFF;
    header[1] = 0xF9;
    header[2] = (((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));
    header[3] = (((chanCfg & 3) << 6) + (adtsLength >> 8));
    header[4] = (adtsLength & 0xFF);
    header[5] = 0x7F;
    header[6] = 0xFC;
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
    if (quaPkt->data == NULL) {
        free(quaPkt);
        quaPkt = NULL;
        return NULL;
    }
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

void audioFrame_set_default(qua_audio_frame_ext_t* frame)
{
    memset(frame->data, 0x00, frame->size);
    frame->offset = 0;
    frame->capacity = 0;
    frame->nb_samples = 0;
    frame->sample_fmt = 0;
    frame->sample_rate = 0;
    frame->nb_channels = 0;
    frame->flags = 0;
    frame->pts = 0;
}

void videoFrame_set_default(qua_video_frame_ext_t* frame)
{
    frame->width = 0;
    frame->height = 0;
    frame->data[0] = NULL;
    frame->data[1] = NULL;
    frame->data[2] = NULL;
    frame->linesize[0] = 0;
    frame->linesize[1] = 0;
    frame->linesize[2] = 0;
    frame->pts = 0;
    frame->dts = 0;
    frame->pix_fmt = 0;
}

qua_audio_frame_ext_t* alloc_quaAudioFrame(int size)
{
    qua_audio_frame_ext_t *audioFrame = (qua_audio_frame_ext_t *)calloc(1, sizeof(qua_audio_frame_ext_t));
    if (audioFrame == NULL) {
        printf("OOM audioFrame calloc\n");
        return NULL;
    }
    audioFrame->data = (uint8_t *)calloc(size, sizeof(uint8_t));
    if (audioFrame->data == NULL) {
        printf("OOM audioFrame->data calloc\n");
        free(audioFrame);
        audioFrame = NULL;
        return NULL;
    }
    audioFrame->size = size;

    audioFrame_set_default(audioFrame);
    return audioFrame;
}

qua_video_frame_ext_t* alloc_quaVideoFrame(int size)
{
    qua_video_frame_ext_t *videoFrame = (qua_video_frame_ext_t *)calloc(1, sizeof(qua_video_frame_ext_t));
    if (videoFrame == NULL) {
        printf("OOM videoFrame calloc\n");
        return NULL;
    }
    videoFrame_set_default(videoFrame);
    return videoFrame;
}

void free_audioFrame(qua_audio_frame_ext_t *frame)
{
    if (frame && frame->data) {
        free(frame->data);
        frame->data = NULL;
    }
    free(frame);
    frame = NULL;
}

void free_videoFrame(qua_video_frame_ext_t *frame)
{
    free(frame);
    frame = NULL;
}

qua_mm_encoder_device_t* get_enc_device(char *chip, char *os)
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
    ret = qua_mm_load_module(QUA_MM_MODULE_AVEXT, &ext_module);
    if (ret != QUA_SUCCESS || ext_module == NULL)
        return NULL;

    qua_mm_device_t *mm_device = NULL;
    ret = ext_module->open_device(ext_module, QUA_MM_EXT_ENCODER, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return NULL;
    qua_mm_encoder_device_t *enc_device = (qua_mm_encoder_device_t *)mm_device;
    printf("Module %s, API version %d enc_device: %p\n",
        ext_module->id, ext_module->api_version, enc_device);
    return enc_device;
}

QUA_U64 get_microsecond()
{
    // us
    struct timeval timestamp = {};
    if (0 == gettimeofday(&timestamp, NULL)) {
        return timestamp.tv_sec * 1000000 + timestamp.tv_usec;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    char *chip_name = NULL;
    char *os_name = NULL;
    char *stream_type = NULL;
    char *src_filename = NULL;
    char *dst_filename = NULL;
    FILE *src_fd = NULL;
    FILE *dst_fd = NULL;
    // uint8_t wav_header[WAV_HEADER_SIZE] = { 0 };
    qua_packet_t *quaPacket = NULL;
    qua_audio_frame_ext_t *audioFrame = NULL;
    qua_video_frame_ext_t *videoFrame = NULL;
    qua_audio_config_t audioCfg = { };
    qua_video_config_t videoCfg = { };
    qua_mm_channel_t *mm_chn = NULL;
    qua_mm_encoder_device_t *enc_device = NULL;
    qua_mm_encoder_channel_t *enc_chn = NULL;
    QUA_S32 chn_id = 0;
    qua_ext_codec_chn_attr_t codec_chn_attr = { };
    int is_audio = 0;
    QUA_S32 w_align, h_align;
    QUA_S32 w, h;
    QUA_U64 pos = 0;
    int ret = 0;

    if (argc != 8) {
        fprintf(stderr, "usage: %s \n"
                "chip name .\n"
                "os name .\n"
                "source file .\n"
                "dest file .\n",
                argv[0]);
        exit(1);
    }
    chip_name = argv[1];
    os_name = argv[2];
    stream_type = argv[3];
    src_filename = argv[4];
    dst_filename = argv[5];
    w = atoi(argv[6]);
    h = atoi(argv[7]);

    printf("\n\n");
    printf("src_filename: %s \n", src_filename);
    printf("dst_filename: %s \n", dst_filename);

    printf("test work start >>>>\n");
    if (strncmp(stream_type, "audio", strlen("audio")) == 0) {
        is_audio = 1;
    }
    src_fd = fopen(src_filename, "rb");
    if (src_fd == NULL) {
        printf("open %s file failed\n", src_filename);
        exit(1);
    }
    // if (fread(wav_header, 1, WAV_HEADER_SIZE, src_fd) != WAV_HEADER_SIZE) {
    //     printf("Failed to read WAV header\n");
    //     exit(1);
    // }
    dst_fd = fopen(dst_filename, "wb+");
    if (dst_fd == NULL) {
        printf("open %s file failed\n", dst_filename);
        exit(1);
    }
    enc_device = get_enc_device(chip_name, os_name);
    if (enc_device == NULL) {
        printf("enc_device is NULL\n");
        exit(1);
    }

    if (is_audio) {
        audioCfg.out_nb_channels = audioCfg.in_nb_channels = 2;
        audioCfg.out_sample_rate = audioCfg.in_sample_rate = 44100;
        audioCfg.out_sample_fmt = audioCfg.in_sample_fmt = QUA_SAMPLE_FMT_S16;
        audioCfg.profile = 1;
        audioCfg.bit_rate = 128000;

        codec_chn_attr.mimetype = "audio/mp4a-latm";
        codec_chn_attr.mime_extra = NULL;
        codec_chn_attr.param = (void *)&audioCfg;
    } else {
        videoCfg.width = w;
        videoCfg.height = h;
        w_align = STRIDE_ALIGN_UP(videoCfg.width, 16);
        h_align = STRIDE_ALIGN_UP(videoCfg.height, 16);
        videoCfg.bitrate = 4000000;
        codec_chn_attr.mimetype = "image/jpeg";
        codec_chn_attr.mime_extra = NULL;
        codec_chn_attr.param = (void *)&videoCfg;
    }
    ret = enc_device->parent.create_channel(&enc_device->parent,
        &chn_id, (QUA_VOID_PTR)&codec_chn_attr, &mm_chn);
    if (ret != QUA_SUCCESS || mm_chn == NULL) {
        printf("enc mm_chn is NULL\n");
        exit(1);
    }
    enc_chn = (qua_mm_encoder_channel_t *)mm_chn;
    if (is_audio == 1) {
        audioFrame = alloc_quaAudioFrame(128*1000);
        if (audioFrame == NULL) {
            printf("alloc audioFrame fail OOM??\n");
            exit(1);
        }
    } else {
        videoFrame = alloc_quaVideoFrame(w_align * h_align * 3 / 2);
        if (videoFrame == NULL) {
            printf("alloc videoFrame fail OOM??\n");
            exit(1);
        }
    }
    quaPacket = alloc_quaPacket(1280*1000);
    if (quaPacket == NULL) {
        printf("alloc quaPacket fail OOM??\n");
        exit(1);
    }

    int frame_size = 1024;
    int data_size = 0;
    uint8_t *data = NULL;
    int pts = 0;
    int y_size = 0;
    if (is_audio == 1) {
        data_size = 2 * frame_size * sizeof(int16_t);
        data = (uint8_t *)malloc(data_size);
    } else {
        data_size = w_align * h_align * 3 / 2;
        y_size = w_align * h_align;
        data = (uint8_t *)malloc(data_size);
    }
    if (data == NULL) {
        printf("alloc data fail OOM??\n");
        exit(1);
    }
    memset(data, 0x00, data_size);
    while (fread(data, 1, data_size, src_fd) == data_size) {
        quaPacket_set_default(quaPacket);
        if (is_audio == 1) {
            audioFrame_set_default(audioFrame);
            audioFrame->offset = 0;
            audioFrame->nb_samples = frame_size;
            audioFrame->sample_fmt = audioCfg.in_sample_fmt;
            audioFrame->nb_channels = audioCfg.in_nb_channels;
            audioFrame->sample_rate = audioCfg.in_sample_rate;
            audioFrame->capacity = data_size;
            pts += 23000;
            audioFrame->pts = pts;
            // printf("data: [%#x,%#x,%#x,%#x,%#x,%#x,%#x,%#x]\n",
            //     data[0], data[1], data[2], data[3],
            //     data[4], data[5], data[6], data[7]);
            memcpy(audioFrame->data, data, data_size);
            ret = enc_chn->send_frame(enc_chn->encoder, audioFrame);
        } else {
            videoFrame_set_default(videoFrame);
            videoFrame->width = videoCfg.width;
            videoFrame->height = videoCfg.height;
            videoFrame->data[0] = data;
            videoFrame->data[1] = data + y_size;
            videoFrame->data[2] = NULL;
            videoFrame->linesize[0] = w_align;
            videoFrame->linesize[1] = w_align;
            videoFrame->linesize[2] = 0;
            videoFrame->pts = 0;
            videoFrame->dts = 0;
            pos = get_microsecond();
            ret = enc_chn->send_frame(enc_chn->encoder, videoFrame);
            printf(" >>>>> send frame ret = %d \n", ret);
        }
        ret = enc_chn->receive_packet(enc_chn->encoder, quaPacket);
        printf(" <<<<< receive frame ret = %d cost = %llu\n", ret, get_microsecond() - pos);
        if (quaPacket->capacity > 0) {
            if (is_audio == 1) {
                uint8_t adts_header[7] = { 0 };
                get_adts_header(&adts_header[0], quaPacket->capacity, audioFrame->sample_rate, audioFrame->nb_channels);
                // fwrite(&adts_header, 1, sizeof(adts_header), dst_fd);
                // printf("bin -> quaPacket: [%#x,%#x,%#x,%#x,%#x,%#x,%#x,%#x]\n",
                //     quaPacket->data[0], quaPacket->data[1], quaPacket->data[2], quaPacket->data[3],
                //     quaPacket->data[4], quaPacket->data[5], quaPacket->data[6], quaPacket->data[7]);
            }
            fwrite(quaPacket->data, 1, quaPacket->capacity, dst_fd);
        }
        memset(data, 0x00, data_size);
    }
    enc_chn->flush(enc_chn->encoder);
    free_quaPacket(quaPacket);
    if (is_audio == 1) {
        free_audioFrame(audioFrame);
    } else {
        free_videoFrame(videoFrame);
    }
    enc_chn->parent.release(&enc_chn->parent);
    enc_device->parent.close((qua_mm_device_t *)enc_device);

    if (data) {
        free(data);
        data = NULL;
    }
    if (src_fd)
        fclose(src_fd);

    if (dst_fd)
        fclose(dst_fd);

    printf("well done. good job!\n\n\n");
    return 0;
}
