/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#define LOG_NDEBUG 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "common/qua_sys_platform.h"
#include "audio/qua_mm_audio.h"

#define QUA_DEFAULT_AI_CACHE_NUM    6
#define QUA_DEFAULT_AO_CACHE_NUM    6

typedef struct _thread_codec_param
{
    QUA_U32 i2s_uDevID;
    QUA_U32 uFrmSize;
    QUA_U32 hdmi_uDevID;
    unsigned char* ai_vAddr;
    unsigned long   ai_phyAddr;
    unsigned char* ao_vAddr;
    unsigned long   ao_phyAddr;
    char file_name[128];
}thread_codec_param;

pthread_t g_audio_record_thread = -1;
pthread_t g_audio_play_thread = -1;
pthread_t g_audio_record_play_thread = -1;
pthread_t g_audio_play_file_record_thread = -1;

qua_mm_audio_device_t *g_audio_dev;
qua_mm_aenc_device_t *g_aenc_dev = NULL;
qua_mm_adec_device_t *g_adec_dev = NULL;

char *filename_48k_16bit_stereo = "/data/media/48k_16bit_stereo.pcm";
char *play_record_file = "/data/media/play_record.pcm";
int g_audio_play_thread_exist = 0;
int g_audio_play_file_record_thread_exist = 0;

QUA_VOID* audio_record_file_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    //static I2S_FRAME_S i2s_ai_frame;
    FILE * record_fp = NULL;

    thread_codec_param* p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    printf("audio_record_file_Callback dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);

    record_fp = fopen("/data/media/record.pcm", "wb+");
    if (NULL == record_fp) {
        printf("fopen record_fp is fail!\n");
        goto exit;
    } else {
        printf("record open file succeed!\n");
    }

    while (1) {
        if (g_audio_record_thread != -1) {
            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, buf, uparm.uFrmSize, NULL);
            if (ret != QUA_SUCCESS) {
                printf("%s: Dev:%d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(buf, 1, uparm.uFrmSize, record_fp);
            uCnt++;
        } else {
            printf("audio_record_file_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->capture_deinit(uparm.i2s_uDevID);

    fclose(record_fp);
    pthread_exit(0);
    return NULL;

}

int audio_record_file(int id, int sample, int channels)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg;

    //i2s_ai_cfg.io_type = SELECT_TYPE_AI;
    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_cnt = QUA_DEFAULT_AI_CACHE_NUM;
    i2s_ai_cfg.channel_bit = QUA_BW_16;
    if (sample > 0) {
        i2s_ai_cfg.rate = sample;
    } else {
        i2s_ai_cfg.rate = QUA_SR_48K;
    }

    if (channels > 0) {
        i2s_ai_cfg.channels = channels;
    } else {
        i2s_ai_cfg.channels = QUA_CHANNEL_STEREO;
    }

    i2s_ai_cfg.frame_size = i2s_ai_cfg.rate * (i2s_ai_cfg.channel_bit / 8) * 40 / 1000 * i2s_ai_cfg.channels;

    result = g_audio_dev->capture_init(id, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        printf("g_audio_dev capture_init failed!\n");
        return QUA_FAILURE;
    }

    result = g_audio_dev->capture_volume(id, QUA_VOLUME_TYPE_ANA, 31);
    if (result != QUA_SUCCESS) {
        printf("g_audio_dev set ana volume fail!\n");
        return QUA_FAILURE;
    }

    result = g_audio_dev->capture_volume(id, QUA_VOLUME_TYPE_DIG, 0x7f);
    if (result != QUA_SUCCESS) {
        printf("g_audio_dev set dig volume fail!\n");
        return QUA_FAILURE;
    }

    result = g_audio_dev->capture_start(id);
    if (result != QUA_SUCCESS) {
        printf("g_audio_dev capture_start failed!\n");
        return QUA_FAILURE;
    }

    if (g_audio_record_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;
        pthread_create(&g_audio_record_thread, 0, audio_record_file_Callback, &uparm);
        printf("%s create thread:i2s_dev_id:%d, frame_size:%d\n", __func__, id, i2s_ai_cfg.frame_size);
    }

    pthread_join(g_audio_record_thread, 0);
    g_audio_record_thread = -1;
    return 0;
}

QUA_VOID* audio_play_file_Callback(QUA_VOID* pAtr)
{
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    //static I2S_FRAME_S i2s_ao_frame;

    FILE * play_fp = NULL;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);
    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));

    g_audio_play_thread_exist = 1;
    printf("audio_play_file_Callback dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);
    if (strlen(uparm.file_name) > 1) {
        play_fp = fopen(uparm.file_name, "rb");
    } else {
        play_fp = fopen(filename_48k_16bit_stereo, "rb");
    }
    if (NULL == play_fp) {
        printf("fopen play_fp is fail!\n");
        goto exit;
    } else {
        printf("audio play open file succeed!\n");
    }

    while (1) {
        if (g_audio_play_thread != -1) {
            while (fread(buf, 1, uparm.uFrmSize, play_fp) != uparm.uFrmSize) {
                fseek(play_fp, 0, SEEK_SET);
            }
            ret = g_audio_dev->write_frame(uparm.i2s_uDevID, buf, uparm.uFrmSize);
            if (ret != QUA_SUCCESS) {
                printf("g_audio_dev write_frame is failed!\n");
                goto exit;
            }
        } else {
            printf("audio_play_file_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_play_thread_exist = 0;
    g_audio_dev->play_stop(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->play_deinit(uparm.i2s_uDevID);
    fclose(play_fp);

    pthread_exit(0);
    return NULL;
}

int audio_play_file(int id, int sample, int channels, char *file, int time)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ao_cfg;

    printf("sample is %d, channels is %d\n", sample, channels);

    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_cnt = QUA_DEFAULT_AO_CACHE_NUM;
    i2s_ao_cfg.channel_bit = QUA_BW_16;
    if (sample > 0) {
        i2s_ao_cfg.rate = sample;
    } else {
        i2s_ao_cfg.rate = QUA_SR_48K;
    }

    if (channels > 0) {
        i2s_ao_cfg.channels = channels;
    } else {
        i2s_ao_cfg.channels = QUA_CHANNEL_STEREO;
    }
    i2s_ao_cfg.frame_size = i2s_ao_cfg.rate * (i2s_ao_cfg.channel_bit / 8) * 40 / 1000 * i2s_ao_cfg.channels;

    result = g_audio_dev->play_init(id, &i2s_ao_cfg);
    if (result != QUA_SUCCESS) {
        printf("g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_play_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ao_cfg.frame_size;
        if (file && strlen(file) > 0) {
            strncpy(uparm.file_name, file, strlen(file));
        }
        pthread_create(&g_audio_play_thread, 0, audio_play_file_Callback, &uparm);
        printf("%s create thread:i2s_dev_id:%d, frame_size:%d\n", __func__, id, i2s_ao_cfg.frame_size);
    }
    if (time > 0) {
        sleep(time);
        if (g_audio_play_thread_exist ) {
            pthread_cancel(g_audio_play_thread);
            g_audio_dev->play_stop(uparm.i2s_uDevID);
            g_audio_dev->exit(uparm.i2s_uDevID);
            g_audio_dev->play_deinit(uparm.i2s_uDevID);
        }
    } else {
        pthread_join(g_audio_play_thread, 0);
    }
    g_audio_play_thread_exist = 0;
    g_audio_play_thread = -1;
    return QUA_SUCCESS;
}

QUA_VOID* audio_record_play_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    QUA_U8   *ai_buf;
    QUA_U8   *ao_buf;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    printf("audio_record_file_Callback dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);

    ai_buf = ao_buf = buf;

    while (1) {
        if (g_audio_record_play_thread != -1) {
            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, ai_buf, uparm.uFrmSize, NULL);
            if (ret != QUA_SUCCESS) {
                printf("%s: Dev %d,read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }

            ret = g_audio_dev->write_frame(uparm.i2s_uDevID, ao_buf, uparm.uFrmSize);
            if (ret != QUA_SUCCESS) {
                printf("g_audio_dev write_frame is failed!\n");
                goto exit;
            }

            uCnt++;
        } else {
            printf("audio_record_file_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.i2s_uDevID);
    g_audio_dev->play_stop(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);

    g_audio_dev->play_deinit(uparm.i2s_uDevID);
    g_audio_dev->capture_deinit(uparm.i2s_uDevID);

    pthread_exit(0);
    return NULL;

}

int audio_record_play(int id, int sample, int channels)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg, i2s_ao_cfg;

    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_cnt = QUA_DEFAULT_AI_CACHE_NUM;
    i2s_ai_cfg.channel_bit = QUA_BW_16;

    if (sample > 0) {
        i2s_ai_cfg.rate = sample;
    } else {
        i2s_ai_cfg.rate = QUA_SR_48K;
    }

    if (channels > 0) {
        i2s_ai_cfg.channels = channels;
    } else {
        i2s_ai_cfg.channels = QUA_CHANNEL_STEREO;
    }
    i2s_ai_cfg.frame_size = i2s_ai_cfg.rate * (i2s_ai_cfg.channel_bit / 8) * 40 / 1000 * i2s_ai_cfg.channels;

    result = g_audio_dev->capture_init(id, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        printf("g_audio_dev capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id);

    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_cnt = QUA_DEFAULT_AO_CACHE_NUM;
    i2s_ao_cfg.channel_bit = QUA_BW_16;

    if (sample > 0) {
        i2s_ao_cfg.rate = sample;
    } else {
        i2s_ao_cfg.rate = QUA_SR_48K;
    }

    if (channels > 0) {
        i2s_ao_cfg.channels = channels;
    } else {
        i2s_ao_cfg.channels = QUA_CHANNEL_STEREO;
    }
    i2s_ao_cfg.frame_size = i2s_ao_cfg.rate * (i2s_ao_cfg.channel_bit / 8) * 40 / 1000 * i2s_ao_cfg.channels;
    result = g_audio_dev->play_init(id, &i2s_ao_cfg);

    if (result != QUA_SUCCESS) {
        printf("g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_record_play_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;
        pthread_create(&g_audio_record_play_thread, 0, audio_record_play_Callback, &uparm);
        printf("%s create thread:i2s_dev_id:%d,frame_size:%d\n", __func__, id, i2s_ai_cfg.frame_size);
    }
    pthread_join(g_audio_record_play_thread, 0);
    g_audio_record_play_thread = -1;
    return QUA_SUCCESS;

}

QUA_VOID* audio_play_file_record_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    QUA_U8   *ai_buf, *ao_buf;

    FILE *play_fp = NULL;
    FILE *record_fp = NULL;
    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);
    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    printf("audio_record_play_Callback i2s_dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);
    g_audio_play_file_record_thread_exist = 1;
    ai_buf = ao_buf = buf;

    record_fp = fopen(play_record_file, "wb+");
    if (NULL == record_fp) {
        printf("fopen record_fp is fail!\n");
        goto exit;
    } else {
        printf("record open file succeed!\n");
    }

    play_fp = fopen(filename_48k_16bit_stereo, "rb");
    if (NULL == play_fp) {
        printf("fopen play_fp is fail!\n");
        goto exit;
    } else {
        printf("i2s0 open file succeed!\n");
    }

    while (1) {
        if (g_audio_play_file_record_thread != -1) {
            while (fread(ao_buf, 1, uparm.uFrmSize, play_fp) != uparm.uFrmSize) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.i2s_uDevID, ao_buf, uparm.uFrmSize);
            if (ret != QUA_SUCCESS) {
                printf("g_audio_dev write_frame is failed!\n");
                goto exit;
            }

            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, ai_buf, uparm.uFrmSize, NULL);
            if (ret != QUA_SUCCESS) {
                printf("%s: Dev %d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(ai_buf, 1, uparm.uFrmSize, record_fp);  /**/
            uCnt++;
        } else {
            printf("audio_play_file_record_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_play_file_record_thread_exist = 0;
    g_audio_dev->capture_stop(uparm.i2s_uDevID);
    g_audio_dev->play_stop(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->capture_deinit(uparm.i2s_uDevID);
    g_audio_dev->play_deinit(uparm.i2s_uDevID);
    pthread_exit(0);
    return NULL;

}

int audio_play_file_record(int id, int sample, int channels, int time)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg, i2s_ao_cfg;

    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_cnt = QUA_DEFAULT_AI_CACHE_NUM;
    i2s_ai_cfg.channel_bit = QUA_BW_16;

    if (sample > 0) {
        i2s_ai_cfg.rate = sample;
    } else {
        i2s_ai_cfg.rate = QUA_SR_48K;
    }

    if (channels > 0) {
        i2s_ai_cfg.channels = channels;
    } else {
        i2s_ai_cfg.channels = QUA_CHANNEL_STEREO;
    }
    i2s_ai_cfg.frame_size = i2s_ai_cfg.rate * (i2s_ai_cfg.channel_bit / 8) * 40 / 1000 * i2s_ai_cfg.channels;
    result = g_audio_dev->capture_init(id, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        printf("capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id);

    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_cnt = QUA_DEFAULT_AO_CACHE_NUM;
    i2s_ao_cfg.channel_bit = QUA_BW_16;

    if (sample > 0) {
        i2s_ao_cfg.rate = sample;
    } else {
        i2s_ao_cfg.rate = QUA_SR_48K;
    }

    if (channels > 0) {
        i2s_ao_cfg.channels = channels;
    } else {
        i2s_ao_cfg.channels = QUA_CHANNEL_STEREO;
    }
    i2s_ao_cfg.frame_size = i2s_ao_cfg.rate * (i2s_ao_cfg.channel_bit / 8) * 40 / 1000 * i2s_ao_cfg.channels;

    result = g_audio_dev->play_init(id, &i2s_ao_cfg);
    if (result != QUA_SUCCESS) {
        printf("play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_play_file_record_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;

        pthread_create(&g_audio_play_file_record_thread, 0, audio_play_file_record_Callback, &uparm);
        printf("%s create thread:i2s_dev_id:%d, frame_size:%d\n", __func__, id, i2s_ai_cfg.frame_size);
    }

    if (time > 0) {
        sleep(time);
        if ( g_audio_play_file_record_thread_exist ) {
            pthread_cancel(g_audio_play_file_record_thread);
            g_audio_dev->capture_stop(uparm.i2s_uDevID);
            g_audio_dev->play_stop(uparm.i2s_uDevID);
            g_audio_dev->exit(uparm.i2s_uDevID);
            g_audio_dev->capture_deinit(uparm.i2s_uDevID);
            g_audio_dev->play_deinit(uparm.i2s_uDevID);
        }
    } else {
        pthread_join(g_audio_play_file_record_thread, 0);
    }
    g_audio_play_file_record_thread_exist = 0;
    g_audio_play_file_record_thread = -1;
    return 0;
}

int audio_enc_dec()
{
    QUA_S32 ret;
    QUA_AENC_CHN ae_chn = 0;
    QUA_ADEC_CHN ad_chn = 0;
    FILE *src = NULL;
    FILE *enc_dest = NULL;
    FILE *dec_dest = NULL;
    QUA_U32 index = 0;
    QUA_U8 buf[6400] = { 0 };
    QUA_S32 bufLen = 640; // 20ms
    qua_audio_frame_t frame;
    qua_audio_stream_t stream;
    qua_mm_channel_t *mm_channel = NULL;
    qua_mm_aenc_channel_t *aenc_chn = NULL;
    qua_mm_adec_channel_t *adec_chn = NULL;
    qua_aenc_chn_attr_t aenc_attr = {
        .enc_type = QUA_PT_G711A,
        .frame_cnt = 2,
        .frame_size = 640,
        .sample_rate = QUA_SR_8K,
        .bit_width = QUA_BW_16,
        .channel = QUA_CHANNEL_STEREO,
        .value = NULL,
    };
    qua_adec_chn_attr_t adec_attr = {
        .enc_type = QUA_PT_G711A,
        .frame_cnt = 2,
        .frame_size = 640,
        .sample_rate = QUA_SR_8K,
        .bit_width = QUA_BW_16,
        .mode = QUA_ADEC_MODE_PACK,
        .value = NULL,
    };

    ret = g_aenc_dev->parent.create_channel((qua_mm_device_t *)g_aenc_dev, (QUA_S32*)&ae_chn, &aenc_attr, &mm_channel);
    if (ret != QUA_SUCCESS) {
        printf("Error: create channel return %d\n", ret);
        return -1;
    }
    aenc_chn = (qua_mm_aenc_channel_t *)mm_channel;

    mm_channel = NULL;
    ret = g_adec_dev->parent.create_channel((qua_mm_device_t *)g_adec_dev, (QUA_S32*)&ae_chn, &adec_attr, &mm_channel);
    if (ret != QUA_SUCCESS) {
        printf("Error: create channel return %d\n", ret);
        return -1;
    }
    adec_chn = (qua_mm_adec_channel_t *)mm_channel;

    src = fopen("/usr/bin/8k_16bit_stereo.pcm", "rb");
    if (!src) {
        printf("open src file fail\n");
        return -1;
    }
    enc_dest = fopen("/usr/bin/g711_16k_enc.format", "wb+");
    if (!enc_dest) {
        printf("open enc dest file fail\n");
        return -1;
    }
    dec_dest = fopen("/usr/bin/g711_16k_dec.pcm", "wb+");
    if (!dec_dest) {
        printf("open dec dest file fail\n");
        return -1;
    }

    do {
        memset(buf, 0x00, sizeof(buf));
        if (fread(buf, 1, bufLen, src) != bufLen) {
            printf("read end\n");
            break;
        }

        frame.vir_addr = &buf;
        frame.seq = index++;
        frame.len = bufLen;
        frame.timeout = 20;
        frame.pts += 20000;
        frame.sample_rate = QUA_SR_8K;
        frame.bit_width = QUA_BW_16;
        frame.channel = QUA_CHANNEL_STEREO;
        frame.exp_id = 0;

        ret = aenc_chn->send_frame(ae_chn, &frame, 20);
        if (ret != QUA_SUCCESS) {
            printf("send aenc frame fail index = %d\n", index);
            break;
        }
        ret = aenc_chn->get_stream(ae_chn, &stream, 20);
        if (ret != QUA_SUCCESS ) {
            printf("get aenc stream fail index = %d\n", index);
            break;
        }
        if (stream.len > 0) {
            fwrite(stream.stream, 1, stream.len, enc_dest);
        }
        ret = adec_chn->send_stream(ad_chn, &stream, 20);
        if (ret != QUA_SUCCESS) {
            printf("send adec frame fail index = %d\n", index);
            break;
        }
        memset(&frame, 0x00, sizeof(frame));
        frame.vir_addr = &buf;
        frame.len = bufLen;
        ret = adec_chn->get_frame(ad_chn, &frame, 20);
        if (ret != QUA_SUCCESS ) {
            printf("get adec frame fail index = %d\n", index);
            break;
        }
        if (frame.len > 0) {
            fwrite(frame.vir_addr, 1, frame.len, dec_dest);
        }
    } while (QUA_TRUE);

    aenc_chn->release_frame(ae_chn, &frame);
    aenc_chn->clear_chn_buf(ae_chn);
    aenc_chn->parent.release(&aenc_chn->parent);
    adec_chn->release_frame(ad_chn, &frame);
    adec_chn->clear_chn_buf(ad_chn);
    adec_chn->parent.release(&adec_chn->parent);
    fclose(src);
    fclose(enc_dest);
    fclose(dec_dest);
    return 0;
}

void audio_stop_clr(void)
{
    g_audio_record_thread = -1;
    g_audio_play_thread = -1;
    g_audio_record_play_thread = -1;
    g_audio_play_file_record_thread = -1;
    g_audio_play_file_record_thread_exist = 0;
    usleep(500 * 1000);
}

QUA_S32 sample_aio_test(QUA_U32 cmd, int argc, char * const argv[])
{
    int rate, channel, result;
    if (cmd != 8)
        audio_stop_clr();

    while( (result = getopt(argc, argv, "s:c:")) != -1 )
    {
        switch(result){
                case 's':
                    rate = atoi(optarg);
                    printf("sample rate is %d\n", rate);
                    break;

                case 'c':
                    channel = atoi(optarg);
                    printf("channel is %d\n", channel);
                    break;

                break;
        }
    }

    switch(cmd)
    {
        case 1:
        {
            audio_play_file(QUA_DEV_I2S1, rate, channel, NULL, 0);
            printf("\n\033[31m ########  [AUDIO] QUA_DEV_I2S1 START PLAY FILES!  ########\033[0m\n");
        }
        break;

        case 2:
        {
            audio_play_file(QUA_DEV_ACW0, rate, channel, NULL, 0);
            printf("\n\033[31m ########  [AUDIO] QUA_DEV_ACW0 START PLAY FILES!  ########\033[0m\n");
        }
        break;

        case 3:
        {
            audio_record_file(QUA_DEV_I2S1, rate, channel);
            printf("\n\033[31m ########  [AUDIO] DEV_I2S0 START RECORD MUSIC!  ########\033[0m\n");
        }
        break;

        case 4:
        {
            audio_record_file(QUA_DEV_ACW0, rate, channel);
            printf("\n\033[31m ########  [AUDIO] QUA_DEV_ACW0 START RECORD FILES!  ########\033[0m\n");
        }
        break;

        case 5:
        {
            audio_stop_clr();
            printf("\n\033[31m ########  [AUDIO]STOP ALL AUDIO!  ########\033[0m\n");
        }
        break;

        case 6:
        {
            audio_record_play(QUA_DEV_I2S1, rate, channel);
            printf("\n\033[31m ########  [AUDIO]DEV_I2S0 START RECORD PLAY MUSIC!  ########\033[0m\n");
        }
        break;

        case 7:
        {
             audio_play_file_record(QUA_DEV_I2S1, rate, channel, 0);
             printf("\n\033[31m ########  [AUDIO_I2S]I2S0 playfile and I2S0 record! ########\033[0m\n");
        }
        break;

        case 8:
        {
            audio_record_file(QUA_DEV_DMIC1, rate, channel);
            printf("\n\033[31m ########  [AUDIO] QUA_DEV_DMIC1 START RECORD FILES!  ########\033[0m\n");
        }
        break;
        case 9:
        {
            audio_enc_dec();
            printf("\n\033[31m ########  [AUDIO] START ENC DEC FILES!  ########\033[0m\n");
        }
        break;
        default:
        break;
    }

    return QUA_SUCCESS;
}

static void signal_handler(int sig)
{
    audio_stop_clr();
    (void)sig;
}
static int audio_dev_init(void) {
    qua_mm_device_t *mm_device = NULL;
    char platform[QUA_MAX_PLATFORM_STR_LEN];

    sprintf(platform, "%s_%s", QUA_SYS_CHIP_QM10XH, "debian");
    const qua_mm_module_t *audio_module = NULL;
    qua_mm_system_t *system = NULL;
    QUA_S32 ret = qua_mm_init(QUA_TRUE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        ALOGD("system module init fail");
        return QUA_EINVAL;
    }
    ret = qua_mm_load_module(QUA_MM_MODULE_AUDIO, &audio_module);
    if (ret != QUA_SUCCESS || audio_module == NULL) {
        printf("audio_dev_init get module fail\n");
        return QUA_EINVAL;
    }

    ret = audio_module->open_device(audio_module, QUA_MM_AUDIO_DEV, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("audio_dev_init open device fail\n");
        return QUA_EINVAL;
    }

    g_audio_dev = (qua_mm_audio_device_t *)mm_device;

    mm_device = NULL;
    ret = audio_module->open_device(audio_module, QUA_MM_AUDIO_ENC_DEV, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("audio_dev_init open device QUA_MM_AUDIO_ENC_DEV fail\n");
        return QUA_EINVAL;
    }
    g_aenc_dev = (qua_mm_aenc_device_t *)mm_device;

    mm_device = NULL;
    ret = audio_module->open_device(audio_module, QUA_MM_AUDIO_DEC_DEV, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("audio_dev_init open device QUA_MM_AUDIO_DEC_DEV fail\n");
        return QUA_EINVAL;
    }
    g_adec_dev = (qua_mm_adec_device_t *)mm_device;

    return QUA_SUCCESS;
}

static int audio_dev_deinit(void) {
    g_audio_dev->parent.close(&g_audio_dev->parent);
    g_audio_dev = NULL;
    g_adec_dev->parent.close(&g_adec_dev->parent);
    g_adec_dev = NULL;
    g_aenc_dev->parent.close(&g_aenc_dev->parent);
    g_aenc_dev = NULL;
    return QUA_SUCCESS;
}

QUA_S32 factory_test_entry(void)
{
    //audio_play_file_record(QUA_DEV_ACW0, 48000, QUA_CHANNEL_STEREO, 10);
    audio_play_file(QUA_DEV_ACW0, 48000, QUA_CHANNEL_STEREO, NULL, 10);
    printf("audio test done\n");
    return QUA_SUCCESS;
}

int main(int argc, char * const argv[])
{
    int cmd = 0;
    int ret = -1;
    if (argc > 1) {
        cmd = atoi(argv[1]);
    } else {
        //printf("please input like below:./qua_audio_sample 1\n");
        printf("entry audio test mode\n");
    }
    ret = audio_dev_init();
    if (ret != QUA_SUCCESS) {
        printf("please audio dev init err\n");
        goto end;
    }
    fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | O_NONBLOCK);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    if (argc > 1) {
        sample_aio_test(cmd, argc, argv);
    } else {
        factory_test_entry();
    }
    (void)audio_dev_deinit();
end:
    return 0;
}
