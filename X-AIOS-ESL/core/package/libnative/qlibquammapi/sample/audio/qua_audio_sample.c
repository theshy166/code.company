/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef __ANDROID__
#include <cutils/properties.h>
#endif
#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "audio/qua_mm_audio.h"

typedef struct _thread_codec_param
{
    QUA_U32 i2s_uDevID;
    QUA_U32 uFrmSize;
    QUA_U32 hdmi_uDevID;
    unsigned char* ai_vAddr;
    unsigned long   ai_phyAddr;
    unsigned char* ao_vAddr;
    unsigned long   ao_phyAddr;
}thread_codec_param;

typedef struct _qua_frame_s
{
    QUA_U8 *bufVirAddr;
    QUA_U32 u32Len;
}qua_frame_s;

pthread_t g_audio_record_thread = -1;
pthread_t g_audio_play_thread = -1;
pthread_t g_audio_record_play_thread = -1;
pthread_t g_hdmi_play_thread = -1;
pthread_t g_hdmi_record_play_thread = -1;
pthread_t g_hdmi_play_file_record_thread = -1;
pthread_t g_audio_play_file_record_thread = -1;

qua_mm_audio_device_t *g_audio_dev;
char *filename_48k_32bit_stereo = "/data/media/48k_32bit_stereo.pcm";

QUA_VOID* audio_record_file_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_ai_frame;
    FILE * record_fp = NULL;

    thread_codec_param* p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_record_file_Callback dev_id:%d,frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);

    i2s_ai_frame.u32Len = uparm.uFrmSize;
    i2s_ai_frame.bufVirAddr = buf;

    record_fp = fopen("/data/media/record.pcm", "wb+");
    if (NULL == record_fp) {
        ALOGD("fopen record_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("record open file succeed!\n");
    }

    while (1) {
        if (g_audio_record_thread != -1) {
            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, i2s_ai_frame.bufVirAddr, i2s_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev:%d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(i2s_ai_frame.bufVirAddr, 1, i2s_ai_frame.u32Len, record_fp);  /**/
            uCnt++;
        } else {
            ALOGD("audio_record_file_Callback thread exit!\n");
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

int audio_record_file(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg;

    i2s_ai_cfg.enc_type = QUA_PT_LPCM;
    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_size = 10240;
    i2s_ai_cfg.frame_cnt = 3;
    i2s_ai_cfg.channel_bit = QUA_BW_32;
    i2s_ai_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->capture_init(id, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id);

    if (g_audio_record_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;
        pthread_create(&g_audio_record_thread, 0, audio_record_file_Callback, &uparm);
        ALOGD("%s create thread:i2s_dev_id:%d, frame_size:%d\n", __func__, id, i2s_ai_cfg.frame_size);
    }

    pthread_join(g_audio_record_thread, 0);
    g_audio_record_thread = -1;
    return 0;
}

QUA_VOID* audio_play_file_Callback(QUA_VOID* pAtr)
{
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_ao_frame;

    FILE * play_fp = NULL;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_play_file_Callback dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);

    i2s_ao_frame.u32Len = uparm.uFrmSize;
    i2s_ao_frame.bufVirAddr = buf;

    play_fp = fopen(filename_48k_32bit_stereo, "rb");
    if (NULL == play_fp) {
        ALOGD("fopen play_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("i2s0 open file succeed!\n");
    }

    while (1) {
        if (g_audio_play_thread != -1) {
            while (fread(i2s_ao_frame.bufVirAddr, 1, i2s_ao_frame.u32Len, play_fp) != i2s_ao_frame.u32Len) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.i2s_uDevID, i2s_ao_frame.bufVirAddr, i2s_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("g_audio_dev write_frame is failed!\n");
                goto exit;
            }
        } else {
            ALOGD("audio_play_file_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->play_stop(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->play_deinit(uparm.i2s_uDevID);
    fclose(play_fp);
    pthread_exit(0);
    return NULL;
}

int audio_play_file(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ao_cfg;

    i2s_ao_cfg.enc_type = QUA_PT_LPCM;
    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_size = 1280;
    i2s_ao_cfg.frame_cnt = 3;
    i2s_ao_cfg.channel_bit = QUA_BW_32;
    i2s_ao_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->play_init(id, &i2s_ao_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_play_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ao_cfg.frame_size;
        pthread_create(&g_audio_play_thread, 0, audio_play_file_Callback, &uparm);
        ALOGD("%s create thread:i2s_dev_id:%d, frame_size:%d\n", __func__, id, i2s_ao_cfg.frame_size);
    }
    pthread_join(g_audio_play_thread, 0);
    g_audio_play_thread = -1;
    return 0;
}

QUA_VOID* audio_record_play_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_ai_frame, i2s_ao_frame;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_record_file_Callback dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);

    i2s_ai_frame.u32Len = uparm.uFrmSize;
    i2s_ao_frame.u32Len = uparm.uFrmSize;
    i2s_ai_frame.bufVirAddr = buf;
    i2s_ao_frame.bufVirAddr = buf;

    while (1) {
        if (g_audio_record_play_thread != -1) {
            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, i2s_ai_frame.bufVirAddr, i2s_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev %d,read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }

            ret = g_audio_dev->write_frame(uparm.i2s_uDevID, i2s_ao_frame.bufVirAddr, i2s_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("g_audio_dev write_frame is failed!\n");
                goto exit;
            }

            uCnt++;
        } else {
            ALOGD("audio_record_file_Callback thread exit!\n");
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

int audio_record_play(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg, i2s_ao_cfg;

    i2s_ai_cfg.enc_type = QUA_PT_LPCM;
    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_size = 2560;
    i2s_ai_cfg.frame_cnt = 3;
    i2s_ai_cfg.channel_bit = QUA_BW_32;
    i2s_ai_cfg.rate = QUA_SR_48K;
    result = g_audio_dev->capture_init(id, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id);

    i2s_ao_cfg.enc_type = QUA_PT_LPCM;
    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_size = 2560;
    i2s_ao_cfg.frame_cnt = 3;
    i2s_ao_cfg.channel_bit = QUA_BW_32;
    i2s_ao_cfg.rate = QUA_SR_48K;
    result = g_audio_dev->play_init(id, &i2s_ao_cfg);

    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_record_play_thread == -1) {
        uparm.i2s_uDevID = id;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;
        pthread_create(&g_audio_record_play_thread, 0, audio_record_play_Callback, &uparm);
        ALOGD("%s create thread:i2s_dev_id:%d,frame_size:%d\n", __func__, id, i2s_ai_cfg.frame_size);
    }
    pthread_join(g_audio_record_play_thread, 0);
    g_audio_record_play_thread = -1;
    return 0;

}

QUA_VOID* hdmi_play_file_Callback(QUA_VOID* pAtr)
{
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_hdmi_ao_frame;
    FILE *play_fp = NULL;
    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("hdmi_play_file_Callback dev_id:%d, frame_size:%d\n", uparm.hdmi_uDevID, uparm.uFrmSize);

    i2s_hdmi_ao_frame.u32Len = uparm.uFrmSize;
    i2s_hdmi_ao_frame.bufVirAddr = buf;

    play_fp = fopen(filename_48k_32bit_stereo, "rb");
    if (NULL == play_fp) {
        ALOGD("fopen play_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("hdmi open file succeed!\n");
    }

    while (1) {
        if (g_hdmi_play_thread != -1) {
            while (fread(i2s_hdmi_ao_frame.bufVirAddr, 1, i2s_hdmi_ao_frame.u32Len, play_fp) != i2s_hdmi_ao_frame.u32Len) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.hdmi_uDevID, i2s_hdmi_ao_frame.bufVirAddr, i2s_hdmi_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("MPI_I2S_AO_send_frame is failed!\n");
                goto exit;
            }
        } else {
            ALOGD("hdmi_play_file_Callback thread exit!\n");
            goto exit;
        }
    }
exit:
    g_audio_dev->play_stop(uparm.hdmi_uDevID);
    g_audio_dev->exit(uparm.hdmi_uDevID);
    g_audio_dev->play_deinit(uparm.hdmi_uDevID);

    fclose(play_fp);
    pthread_exit(0);
    return NULL;
}

int hdmi_play_file(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ao_cfg;

    //hdmi只能选32k和48k采样率，位深只能是32
    i2s_ao_cfg.enc_type = QUA_PT_LPCM;
    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_size = 1280;  //2560 -> 1280
    i2s_ao_cfg.frame_cnt = 3;
    i2s_ao_cfg.channel_bit = QUA_BW_32;
    i2s_ao_cfg.rate = QUA_SR_48K;
    result = g_audio_dev->play_init(id, &i2s_ao_cfg);

    if (result != QUA_SUCCESS) {
        ALOGD("hdmi_play_file g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_hdmi_play_thread == -1) {
        uparm.hdmi_uDevID = id;
        uparm.uFrmSize = i2s_ao_cfg.frame_size;
        pthread_create(&g_hdmi_play_thread, 0, hdmi_play_file_Callback, &uparm);
        ALOGD("%s create thread:hdmi_dev_id:%d, frame_size:%d\n", __func__, id, i2s_ao_cfg.frame_size);
    }
    pthread_join(g_hdmi_play_thread, 0);
    g_hdmi_play_thread = -1;
    return 0;
}

QUA_VOID* hdmi_record_play_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_ai_frame, i2s_hdmi_ao_frame;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("hdmi_record_play_Callback i2s_dev_id:%d, hdmi_dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.hdmi_uDevID, uparm.uFrmSize);

    i2s_ai_frame.u32Len = uparm.uFrmSize;
    i2s_hdmi_ao_frame.u32Len = uparm.uFrmSize;
    i2s_ai_frame.bufVirAddr = buf;
    i2s_hdmi_ao_frame.bufVirAddr = buf;

    while (1) {
        if (g_hdmi_record_play_thread != -1) {
            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, i2s_ai_frame.bufVirAddr, i2s_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev %d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }

            ret = g_audio_dev->write_frame(uparm.hdmi_uDevID, i2s_hdmi_ao_frame.bufVirAddr, i2s_hdmi_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("MPI_I2S_AO_send_frame is failed!\n");
                goto exit;
            }

            uCnt++;
        } else {
            ALOGD("hdmi_record_play_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.i2s_uDevID);
    g_audio_dev->play_stop(uparm.hdmi_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.hdmi_uDevID);
    g_audio_dev->capture_deinit(uparm.i2s_uDevID);
    g_audio_dev->play_deinit(uparm.hdmi_uDevID);
    pthread_exit(0);
    return NULL;
}

int hdmi_record_play(int id_i2s,int id_hdmi)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg, i2s_ao_cfg;

    i2s_ai_cfg.enc_type = QUA_PT_LPCM;
    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_size = 2560;
    i2s_ai_cfg.frame_cnt = 3;
    i2s_ai_cfg.channel_bit = QUA_BW_32;
    i2s_ai_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->capture_init(id_i2s, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("hdmi_record_play capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id_i2s);

    i2s_ao_cfg.enc_type = QUA_PT_LPCM;
    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_size = 2560;
    i2s_ao_cfg.frame_cnt = 3;
    i2s_ao_cfg.channel_bit = QUA_BW_32;
    i2s_ao_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->play_init(id_hdmi, &i2s_ao_cfg);

    if (result != QUA_SUCCESS) {
        ALOGD("hdmi_record_play play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id_hdmi);

    if (g_hdmi_record_play_thread == -1) {
        uparm.i2s_uDevID = id_i2s;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;
        uparm.hdmi_uDevID = id_hdmi;
        pthread_create(&g_hdmi_record_play_thread, 0, hdmi_record_play_Callback, &uparm);
        ALOGD("%s create thread:i2s_dev_id:%d, hdmi_dev_id:%d, frame_size:%d\n", __func__, id_i2s, id_hdmi, i2s_ai_cfg.frame_size);
    }
    pthread_join(g_hdmi_record_play_thread, 0);
    g_hdmi_record_play_thread = -1;
    return 0;
}

QUA_VOID* hdmi_play_file_record_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_ai_frame, i2s_hdmi_ao_frame;
    FILE *play_fp = NULL;
    FILE *record_fp = NULL;
    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("hdmi_record_play_Callback i2s_dev_id:%d, hdmi_dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.hdmi_uDevID, uparm.uFrmSize);

    i2s_ai_frame.u32Len = uparm.uFrmSize;
    i2s_hdmi_ao_frame.u32Len = uparm.uFrmSize;
    i2s_ai_frame.bufVirAddr = buf;
    i2s_hdmi_ao_frame.bufVirAddr = buf;

    record_fp = fopen("/data/media/hdmi0_play_i2s0_record.pcm", "wb+");
    if (NULL == record_fp) {
        ALOGD("fopen record_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("record open file succeed!\n");
    }

    play_fp = fopen(filename_48k_32bit_stereo, "rb");
    if (NULL == play_fp) {
        ALOGD("fopen play_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("i2s0 open file succeed!\n");
    }

    while (1) {
        if (g_hdmi_play_file_record_thread != -1) {
            while (fread(i2s_hdmi_ao_frame.bufVirAddr, 1, i2s_hdmi_ao_frame.u32Len, play_fp) != i2s_hdmi_ao_frame.u32Len) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.hdmi_uDevID, i2s_hdmi_ao_frame.bufVirAddr, i2s_hdmi_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("g_audio_dev write_frame is failed!\n");
                goto exit;
            }

            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, i2s_ai_frame.bufVirAddr, i2s_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev %d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(i2s_ai_frame.bufVirAddr, 1, i2s_ai_frame.u32Len, record_fp);  /**/
            uCnt++;
        } else {
            ALOGD("hdmi_play_file_record_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.i2s_uDevID);
    g_audio_dev->play_stop(uparm.hdmi_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.hdmi_uDevID);
    g_audio_dev->capture_deinit(uparm.i2s_uDevID);
    g_audio_dev->play_deinit(uparm.hdmi_uDevID);

    pthread_exit(0);
    return NULL;

}


int hdmi_play_file_record(int id_i2s,int id_hdmi)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg, i2s_ao_cfg;

    i2s_ai_cfg.enc_type = QUA_PT_LPCM;
    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_size = 2560;
    i2s_ai_cfg.frame_cnt = 3;
    i2s_ai_cfg.channel_bit = QUA_BW_32;
    i2s_ai_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->capture_init(id_i2s, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("MPI_I2S_AI_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id_i2s);

    i2s_ao_cfg.enc_type = QUA_PT_LPCM;
    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_size = 2560;
    i2s_ao_cfg.frame_cnt = 3;
    i2s_ao_cfg.channel_bit = QUA_BW_32;
    i2s_ao_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->play_init(id_hdmi, &i2s_ao_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("MPI_I2S_AO_init failed!\n");
        return QUA_FAILURE;
    }

    g_audio_dev->play_start(id_hdmi);
    if (g_hdmi_play_file_record_thread == -1) {
        uparm.i2s_uDevID = id_i2s;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;
        uparm.hdmi_uDevID = id_hdmi;
        pthread_create(&g_hdmi_play_file_record_thread, 0, hdmi_play_file_record_Callback, &uparm);
        ALOGD("%s create thread:i2s_dev_id:%d, hdmi_dev_id:%d, frame_size:%d\n", __func__, id_i2s,id_hdmi, i2s_ai_cfg.frame_size);
    }
    pthread_join(g_hdmi_play_file_record_thread, 0);
    g_hdmi_play_file_record_thread = -1;
    return 0;
}


QUA_VOID* audio_play_file_record_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s i2s_ai_frame, i2s_ao_frame;
    FILE *play_fp = NULL;
    FILE *record_fp = NULL;
    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_record_play_Callback i2s_dev_id:%d, frame_size:%d\n", uparm.i2s_uDevID, uparm.uFrmSize);

    i2s_ai_frame.u32Len = uparm.uFrmSize;
    i2s_ao_frame.u32Len = uparm.uFrmSize;
    i2s_ai_frame.bufVirAddr = buf;
    i2s_ao_frame.bufVirAddr = buf;

    record_fp = fopen("/data/media/i2s0_play_i2s0_record.pcm", "wb+");
    if (NULL == record_fp) {
        ALOGD("fopen record_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("record open file succeed!\n");
    }

    play_fp = fopen(filename_48k_32bit_stereo, "rb");
    if (NULL == play_fp) {
        ALOGD("fopen play_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("i2s0 open file succeed!\n");
    }

    while (1) {
        if (g_audio_play_file_record_thread != -1) {
            while (fread(i2s_ao_frame.bufVirAddr, 1, i2s_ao_frame.u32Len, play_fp) != i2s_ao_frame.u32Len) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.i2s_uDevID, i2s_ao_frame.bufVirAddr, i2s_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("g_audio_dev write_frame is failed!\n");
                goto exit;
            }

            ret = g_audio_dev->read_frame(uparm.i2s_uDevID, i2s_ai_frame.bufVirAddr, i2s_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev %d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.i2s_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(i2s_ai_frame.bufVirAddr, 1, i2s_ai_frame.u32Len, record_fp);  /**/
            uCnt++;
        } else {
            ALOGD("audio_play_file_record_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.i2s_uDevID);
    g_audio_dev->play_stop(uparm.i2s_uDevID);
    g_audio_dev->exit(uparm.i2s_uDevID);
    g_audio_dev->capture_deinit(uparm.i2s_uDevID);
    g_audio_dev->play_deinit(uparm.i2s_uDevID);

    pthread_exit(0);
    return NULL;

}

int audio_play_file_record(int id_i2s)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg i2s_ai_cfg, i2s_ao_cfg;

    i2s_ai_cfg.enc_type = QUA_PT_LPCM;
    i2s_ai_cfg.io_type = QUA_MIC_IN;
    i2s_ai_cfg.frame_size = 2560;
    i2s_ai_cfg.frame_cnt = 3;
    i2s_ai_cfg.channel_bit = QUA_BW_32;
    i2s_ai_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->capture_init(id_i2s, &i2s_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id_i2s);

    i2s_ao_cfg.enc_type = QUA_PT_LPCM;
    i2s_ao_cfg.io_type = QUA_LINE_HPOUT;
    i2s_ao_cfg.frame_size = 2560;
    i2s_ao_cfg.frame_cnt = 3;
    i2s_ao_cfg.channel_bit = QUA_BW_32;
    i2s_ao_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->play_init(id_i2s, &i2s_ao_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id_i2s);

    if (g_audio_play_file_record_thread == -1) {
        uparm.i2s_uDevID = id_i2s;
        uparm.uFrmSize = i2s_ai_cfg.frame_size;

        pthread_create(&g_audio_play_file_record_thread, 0, audio_play_file_record_Callback, &uparm);
        ALOGD("%s create thread:i2s_dev_id:%d, frame_size:%d\n", __func__, id_i2s, i2s_ai_cfg.frame_size);
    }
    pthread_join(g_audio_play_file_record_thread, 0);
    g_audio_play_file_record_thread = -1;
    return 0;
}

void audio_hdmi_stop_clr(void)
{
    g_audio_record_thread = -1;
    g_audio_play_thread = -1;
    g_audio_record_play_thread = -1;
    g_hdmi_play_thread = -1;
    g_hdmi_record_play_thread = -1;
    g_hdmi_play_file_record_thread = -1;
    g_audio_play_file_record_thread = -1;
    usleep(500 * 1000);
}

QUA_S32 sample_aio_test(QUA_U32 cmd)
{
    if(cmd != 8)
        audio_hdmi_stop_clr();
    switch(cmd)
    {
        case 1:
        {
            audio_record_file(QUA_DEV_I2S0);
            ALOGD("\n\033[31m ########  [AUDIO]START RECORD FILES!  ########\033[0m\n");
        }
        break;

        case 2:
        {
            audio_play_file(QUA_DEV_I2S0);
            ALOGD("\n\033[31m ########  [AUDIO]START PLAY FILES!  ########\033[0m\n");
        }
        break;

        case 3:
        {
            audio_record_play(QUA_DEV_I2S0);
            ALOGD("\n\033[31m ########  [AUDIO]START PLAY MUSIC!  ########\033[0m\n");
        }
        break;

        case 4:
        {
            hdmi_play_file(QUA_DEV_I2S3);  //hdmi0接i2s3
            ALOGD("\n\033[31m ########  [AUDIO_HDMI0]START PLAY FILES!  ########\033[0m\n");
        }
        break;

        case 5:
        {
            hdmi_record_play(QUA_DEV_I2S0, QUA_DEV_I2S3);  //i2s0:record; i2s3:play;
            ALOGD("\n\n\033[31m ########  [AUDIO_HDMI0]START PLAY MUSIC!  ########\033[0m\n");
        }
        break;

        case 6:
            hdmi_play_file(QUA_DEV_I2S4);  //hdmi1接i2s4
            ALOGD("\n\n\033[31m ########  [AUDIO_HDMI1]START PLAY FILES!  ########\033[0m\n");
        break;

        case 7:
        {
            hdmi_record_play(QUA_DEV_I2S0, QUA_DEV_I2S4);  //i2s0:record; i2s4:play;
            ALOGD("\n\033[31m ########  [AUDIO_HDMI1]START PLAY MUSIC!  ########\033[0m\n");
        }
        break;

        case 8:
        {
            audio_hdmi_stop_clr();
            ALOGD("\n\033[31m ########  [AUDIO]STOP ALL AUDIO AND HDMI!  ########\033[0m\n");
        }
        break;

        case 9:
        {
             hdmi_play_file_record(QUA_DEV_I2S0, QUA_DEV_I2S3);
             ALOGD("\n\033[31m ########  [AUDIO_HDMI0]HDMI0 playfile and I2S0 record! ########\033[0m\n");
        }
        break;

        case 10:
        {
             audio_play_file_record(QUA_DEV_I2S0);
             ALOGD("\n\033[31m ########  [AUDIO_I2S]I2S0 playfile and I2S0 record! ########\033[0m\n");
        }
        break;

        default:
        break;
    }

    return QUA_SUCCESS;
}

static void signal_handler(int sig)
{
    audio_hdmi_stop_clr();
    (void)sig;
}
static int audio_dev_init(void) {
    qua_mm_device_t *mm_device = NULL;
    char platform[QUA_MAX_PLATFORM_STR_LEN];
#ifdef __ANDROID__
    const char *prop = "ro.hardware";
    char value[PROPERTY_VALUE_MAX];
    if (property_get(prop, value, NULL)) {
        sprintf(platform, "%s_%s", value, qua_sys_os());
    }
    else
#endif
    {
        sprintf(platform, "%s_%s", QUA_SYS_CHIP_MC6870, qua_sys_os());
    }

    const qua_mm_module_t *audio_module = NULL;
    qua_mm_system_t *system = NULL;
    QUA_S32 ret = qua_mm_init(QUA_FALSE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        ALOGD("audio_dev_init module init fail");
        return QUA_EINVAL;
    }

    ret = qua_mm_load_module(QUA_MM_MODULE_AUDIO, &audio_module);
    if (ret != QUA_SUCCESS || audio_module == NULL) {
        ALOGD("audio_dev_init get module fail");
        return QUA_EINVAL;
    }

    ret = audio_module->open_device(audio_module, QUA_MM_AUDIO_DEV, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        ALOGD("audio_dev_init open device fail");
        return QUA_EINVAL;
    }

    g_audio_dev = (qua_mm_audio_device_t *)mm_device;
    return QUA_SUCCESS;
}

static int audio_dev_deinit(void) {
    g_audio_dev->parent.close(&g_audio_dev->parent);
    g_audio_dev = NULL;
    return QUA_SUCCESS;
}

int main(int argc, char * const argv[])
{
    int cmd = 0;
    int ret = -1;
    if (argc > 1) {
        cmd = atoi(argv[1]);
    } else {
        ALOGD("please input like below:./qua_audio_sample 2\n");
        return 0;
    }
    ret = audio_dev_init();
    if (ret != QUA_SUCCESS) {
        ALOGD("please audio dev init err\n");
        goto end;
    }
    fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | O_NONBLOCK);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);

    sample_aio_test(cmd);
    (void)audio_dev_deinit();
end:
    return 0;
}
