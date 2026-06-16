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
#include "avext/qua_media_extractor.h"
#include "avext/qua_media_decoder.h"
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
        printf("OOM audioFrame calloc\n");
        return NULL;
    }
    uint8_t *buffer = (uint8_t *)calloc(size, sizeof(uint8_t));
    videoFrame->data[0] = buffer;
    if (videoFrame->data[0] == NULL) {
        free(videoFrame);
        videoFrame = NULL;
        return NULL;
    }
    videoFrame->data[1] = buffer + size * 2 / 3;
    printf("data [%p, %p] \n", videoFrame->data[0], videoFrame->data[1]);
    printf("linesize [%d, %d] \n", videoFrame->linesize[0], videoFrame->linesize[1]);
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

qua_mm_decoder_device_t* get_dec_device(char *chip, char *os)
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

    printf("Module %s, API version %d\n", ext_module->id, ext_module->api_version);
    qua_mm_device_t *mm_device = NULL;
    ret = ext_module->open_device(ext_module, QUA_MM_EXT_DECODER, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return NULL;
    qua_mm_decoder_device_t *dec_device = (qua_mm_decoder_device_t *)mm_device;

    printf("Module %s, API version %d dec_device: %p\n",
        ext_module->id, ext_module->api_version, dec_device);
    return dec_device;
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
    ret = qua_mm_load_module(QUA_MM_MODULE_AVEXT, &ext_module);
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

static QUA_U64 get_ms()
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
    char *src_filename = NULL;
    char *video_dst_filename = NULL;
    char *audio_dst_filename = NULL;
    FILE *video_dst_file = NULL, *audio_dst_file = NULL;

    int trackCnt = 0, videoTrackIndex = -1, audioTrackIndex = -1;
    int audioCnt = 0, videoCnt = 0;
    int audioSize = 0, videoSize = 0;
    int audioeof = 1, videoeof = 1;
    int width = 0, height = 0;
    int fps = 0, profile = 0, level = 0;
    qua_metaformat_t *meta = NULL, *audioTrackMeta = NULL, *videoTrackMeta = NULL;
    ssize_t trackSize = 0;
    qua_packet_t *quaPacket = NULL;
    qua_audio_frame_ext_t *audioFrame = NULL;
    qua_video_frame_ext_t *videoFrame = NULL;
    qua_mm_channel_t *mm_chn = NULL;
    qua_mm_extractor_device_t *extractor_device = NULL;
    qua_mm_extractor_channel_t *extra_chn = NULL;
    qua_mm_decoder_device_t *dec_device = NULL;
    qua_mm_decoder_channel_t *dec_chn = NULL;
    qua_mm_decoder_channel_t *video_dec_chn = NULL;
    qua_ext_codec_chn_attr_t codec_chn_attr;
    QUA_S32 chn_id = 0;
    QUA_U64 pos = 0;
    int ret = 0;

    if (argc != 6) {
        fprintf(stderr, "usage: %s \n"
                "chip name and os name.\n"
                "input_file video_output_file audio_output_file.\n"
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
    if (extra_chn == NULL) {
        printf("extra_chn is null\n");
        exit(1);
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
        ret = extra_chn->track_meta_data(extra_chn->extractor, videoTrackIndex, &videoTrackMeta);;
        if (ret == 0) {
            printf_meta(videoTrackMeta);
        }
        int32_t bit_rate = 0;
        qua_video_config_t videoCfg;
        char mime[128] = { 0 };
        void *extra_data = NULL;
        size_t extra_data_size = 0;

        videoTrackMeta->get_int32(videoTrackMeta, "width", &width);
        videoTrackMeta->get_int32(videoTrackMeta, "height", &height);
        videoTrackMeta->get_int32(videoTrackMeta, "profile", &profile);
        videoTrackMeta->get_int32(videoTrackMeta, "level", &level);
        videoTrackMeta->get_int32(videoTrackMeta, "fps", &fps);
        videoTrackMeta->get_string(videoTrackMeta, "mime", &mime[0]);
        videoTrackMeta->get_int32(videoTrackMeta, "bitrate", &bit_rate);
        videoTrackMeta->get_buffer(videoTrackMeta, "csd-avc", &extra_data, &extra_data_size);
        videoCfg.width = width;
        videoCfg.height = height;
        videoCfg.fps = fps;
        videoCfg.bitrate = bit_rate;
        videoCfg.pix_fmt = 23;
        videoCfg.profile = profile;
        videoCfg.level = level;
        memcpy(videoCfg.extra_data, (uint8_t*)extra_data, extra_data_size);
        videoCfg.extra_data_size = extra_data_size;

        dec_device = get_dec_device(chip_name, os_name);
        if (dec_device == NULL) {
            printf("dec_device is NULL\n");
            exit(1);
        }
        codec_chn_attr.mimetype = &mime[0];
        codec_chn_attr.mime_extra = NULL;
        codec_chn_attr.param = (void *)&videoCfg;
        ret = dec_device->parent.create_channel(&dec_device->parent, &chn_id, (QUA_VOID_PTR)&codec_chn_attr, &mm_chn);
        if (ret != 0 || mm_chn == NULL) {
            printf("create_channel dec_chn fail\n");
            exit(1);
        }
        video_dec_chn = (qua_mm_decoder_channel_t *)mm_chn;
        videoeof = 0;
        videoFrame = alloc_quaVideoFrame(width*height*3/2);
        if (videoFrame == NULL) {
            printf("alloc videoFrame fail OOM??\n");
            exit(1);
        }
        videoFrame->linesize[0] = videoCfg.width;
        videoFrame->linesize[1] = videoCfg.width;
    }
    if (audioTrackIndex >= 0) {
        ret = extra_chn->track_meta_data(extra_chn->extractor, audioTrackIndex, &audioTrackMeta);
        if (ret == 0) {
            printf_meta(audioTrackMeta);
        }
        char mime[128] = { 0 };
        qua_audio_config_t audioCfg;
        void *extra_data = NULL;
        size_t extra_data_size = 0;

        int32_t nb_channels, bit_rate, sample_rate, sample_fmt, bits_per_coded_sample, block_align;
        audioTrackMeta->get_int32(audioTrackMeta, "channel-count", &nb_channels);
        audioTrackMeta->get_int32(audioTrackMeta, "bitrate", &bit_rate);
        audioTrackMeta->get_int32(audioTrackMeta, "sample-rate", &sample_rate);
        audioTrackMeta->get_int32(audioTrackMeta, "sample-format", &sample_fmt);
        audioTrackMeta->get_int32(audioTrackMeta, "bits_per_coded_sample", &bits_per_coded_sample);
        audioTrackMeta->get_int32(audioTrackMeta, "block-align", &block_align);
        audioTrackMeta->get_string(audioTrackMeta, "mime", &mime[0]);
        audioTrackMeta->get_buffer(audioTrackMeta, "raw-codec-specific-data", &extra_data, &extra_data_size);

        audioCfg.out_nb_channels = audioCfg.in_nb_channels = nb_channels;
        audioCfg.out_sample_rate = audioCfg.in_sample_rate = sample_rate;
        audioCfg.in_sample_fmt = sample_fmt;
        audioCfg.out_sample_fmt = sample_fmt; // QUA_SAMPLE_FMT_S16;
        audioCfg.bit_rate = bit_rate;
        audioCfg.bits_per_coded_sample = bits_per_coded_sample;
        audioCfg.block_align = block_align;
        memcpy(audioCfg.extra_data, (uint8_t*)extra_data, extra_data_size);
        audioCfg.extra_data_size = extra_data_size;

        dec_device = get_dec_device(chip_name, os_name);
        if (dec_device == NULL) {
            printf("dec_device is NULL\n");
            exit(1);
        }
        codec_chn_attr.mimetype = &mime[0];
        codec_chn_attr.mime_extra = NULL;
        codec_chn_attr.param = (void *)&audioCfg;
        ret = dec_device->parent.create_channel(&dec_device->parent, &chn_id, (QUA_VOID_PTR)&codec_chn_attr, &mm_chn);
        if (ret != 0 || mm_chn == NULL) {
            printf("create_channel dec_chn fail\n");
            exit(1);
        }
        dec_chn = (qua_mm_decoder_channel_t *)mm_chn;
        printf("dec_device: %p, dec_chn: %p, \n", dec_device, dec_chn);
        audioeof = 0;
        audioFrame = alloc_quaAudioFrame(128*1000);
        if (audioFrame == NULL) {
            printf("alloc audioFrame fail OOM??\n");
            exit(1);
        }
    }
    if (videoTrackIndex >= 0) {
        quaPacket = alloc_quaPacket(width * height);
    } else if (audioTrackIndex >= 0) {
        quaPacket = alloc_quaPacket(128*1000);
    }
    if (quaPacket == NULL) {
        printf("alloc quaPacket fail OOM??\n");
        exit(1);
    }

    while (true) {
        quaPacket_set_default(quaPacket);
        if (videoTrackIndex >= 0 && videoeof == 0) {
            trackSize = extra_chn->read_sample_data(extra_chn->extractor, videoTrackIndex, quaPacket);
            if (trackSize > 0) {
                videoSize += quaPacket->capacity;
                // printf("[video] == trackSizeTotle: %d, pts: %ld\n", quaPacket->capacity, quaPacket->pts);
                // if (quaPacket->streamIndex == videoTrackIndex) {
                //     fwrite(quaPacket->data, 1, quaPacket->capacity, video_dst_file);
                // }
                videoCnt++;
                ret = video_dec_chn->send_packet(video_dec_chn->decoder, quaPacket);
                while (ret >= 0) {
                    pos = get_ms();
                    ret = video_dec_chn->receive_frame(video_dec_chn->decoder, videoFrame);
                    printf("cost %lld us\n", get_ms() - pos);
                    if (ret < 0) {
                        printf("Error during decoding ret = %d \n", ret);
                    } else {
                        printf("[video] wxh: %dx%d, pts: %llu\n", videoFrame->width, videoFrame->height, videoFrame->pts);
                        fwrite(videoFrame->data[0], 1, videoFrame->width * videoFrame->height, video_dst_file);
                        fwrite(videoFrame->data[1], 1, videoFrame->width * videoFrame->height / 2, video_dst_file);
                        break;
                    }
                }
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
                audioCnt++;
                ret = dec_chn->send_packet(dec_chn->decoder, quaPacket);
                // printf("qua_avdecoder_sendPacket ret = %d \n", ret);
                if (ret < 0) {
                    printf("Error submitting a packet for decoding ret = %d \n", ret);
                    // return ret;
                }
                while (ret >= 0) {
                    audioFrame_set_default(audioFrame);
                    ret = dec_chn->receive_frame(dec_chn->decoder, audioFrame);
                    // printf("qua_avdecoder_receiveFrame ret = %d \n", ret);
                    if (ret < 0) {
                        printf("Error during decoding ret = %d \n", ret);
                        //if (ret == -11) {
                        //    continue;
                        //}
                        //return ret;
                    } else {
                        printf("[audio] pts = %llu, capacity = %d \n",
                            audioFrame->pts, audioFrame->capacity);
                        fwrite(audioFrame->data, 1, audioFrame->capacity, audio_dst_file);
                        break;
                    }
                }
            } else {
                printf("audio EOF!\n");
                audioeof = 1;
            }
        }
        if (audioeof == 1 && videoeof == 1)
            break;
    };
    dec_chn->flush(dec_chn->decoder);

    free_quaPacket(quaPacket);
    free_audioFrame(audioFrame);

    dec_chn->parent.release(&dec_chn->parent);
    dec_device->parent.close((qua_mm_device_t *)dec_device);

    extra_chn->parent.release(&extra_chn->parent);
    extractor_device->parent.close((qua_mm_device_t *)extractor_device);
    printf("test work end audioCnt = %d, audioSize = %d, videoCnt = %d, videoSize = %d<<<<\n",
        audioCnt, audioSize, videoCnt, videoSize);
    if (audio_dst_file)
        fclose(audio_dst_file);
    if (video_dst_file)
        fclose(video_dst_file);

    printf("well done. good job!\n\n\n");

    return 0;
}
