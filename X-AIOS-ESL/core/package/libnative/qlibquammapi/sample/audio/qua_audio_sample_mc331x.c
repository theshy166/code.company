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
#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "audio/qua_mm_audio.h"

typedef struct _thread_codec_param
{
    QUA_U32 acw_uDevID;
    QUA_U32 uFrmSize;
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

pthread_t g_audio_record_thread = NULL;
pthread_t g_audio_play_thread = NULL;
pthread_t g_audio_record_play_thread = NULL;
pthread_t g_audio_play_file_record_thread = NULL;

qua_mm_audio_device_t *g_audio_dev;
qua_audio_cfg_arg acw_ai_cfg, acw_ao_cfg;

char *filename_8k_16bit_stereo = "/data/media/8k_16bit_stereo.pcm";

QUA_VOID* audio_record_file_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s acw_ai_frame;
    FILE * record_fp = NULL;

    thread_codec_param* p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_record_file_Callback dev_id:%d,frame_size:%d\n", uparm.acw_uDevID, uparm.uFrmSize);

    acw_ai_frame.u32Len = uparm.uFrmSize;
    acw_ai_frame.bufVirAddr = buf;

    record_fp = fopen("/data/media/record.pcm", "wb+");
    if (NULL == record_fp) {
        ALOGD("fopen record_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("record open file succeed!\n");
    }

    while (1) {
        if (g_audio_record_thread != NULL) {
            ret = g_audio_dev->read_frame(uparm.acw_uDevID, acw_ai_frame.bufVirAddr, acw_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev:%d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.acw_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(acw_ai_frame.bufVirAddr, 1, acw_ai_frame.u32Len, record_fp);  /**/
            uCnt++;
        } else {
            ALOGD("audio_record_file_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.acw_uDevID);
    g_audio_dev->exit(uparm.acw_uDevID);
    g_audio_dev->capture_deinit(uparm.acw_uDevID);
    fclose(record_fp);
    pthread_exit(0);
    return NULL;

}

int audio_record_file(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg acw_ai_cfg;

    acw_ai_cfg.enc_type = QUA_PT_LPCM;
    acw_ai_cfg.io_type = QUA_MIC_IN;
    acw_ai_cfg.frame_size = 1280;
    acw_ai_cfg.frame_cnt = 3;
    acw_ai_cfg.channel_bit = QUA_BW_16;
    acw_ai_cfg.rate = QUA_SR_8K;

    result = g_audio_dev->capture_init(id, &acw_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id);

    if (g_audio_record_thread == NULL) {
        uparm.acw_uDevID = id;
        uparm.uFrmSize = acw_ai_cfg.frame_size;

        pthread_create(&g_audio_record_thread, 0, audio_record_file_Callback, &uparm);
        ALOGD("%s create thread:acw_dev_id:%d, frame_size:%d\n", __func__, id, acw_ai_cfg.frame_size);
    }

    pthread_join(g_audio_record_thread, 0);
    g_audio_record_thread = NULL;
    return 0;
}

QUA_VOID* audio_play_file_Callback(QUA_VOID* pAtr)
{
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s acw_ao_frame;

    FILE * play_fp = NULL;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_play_file_Callback dev_id:%d, frame_size:%d\n", uparm.acw_uDevID, uparm.uFrmSize);

    acw_ao_frame.u32Len = uparm.uFrmSize;
    acw_ao_frame.bufVirAddr = buf;

    play_fp = fopen(filename_8k_16bit_stereo, "rb");
    if (NULL == play_fp) {
        ALOGD("fopen play_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("open file succeed!\n");
    }

    while (1) {
        if (g_audio_play_thread != NULL) {
            while (fread(acw_ao_frame.bufVirAddr, 1, acw_ao_frame.u32Len, play_fp) != acw_ao_frame.u32Len) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.acw_uDevID, acw_ao_frame.bufVirAddr, acw_ao_frame.u32Len);
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
    g_audio_dev->play_stop(uparm.acw_uDevID);
    g_audio_dev->exit(uparm.acw_uDevID);
    g_audio_dev->play_deinit(uparm.acw_uDevID);
    fclose(play_fp);
    pthread_exit(0);
    return NULL;
}

int audio_play_file(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg acw_ao_cfg;

    acw_ao_cfg.enc_type = QUA_PT_LPCM;
    acw_ao_cfg.io_type = QUA_LINE_HPOUT;
    acw_ao_cfg.frame_size = 1280;
    acw_ao_cfg.frame_cnt = 3;
    acw_ao_cfg.channel_bit = QUA_BW_16;
    acw_ao_cfg.rate = QUA_SR_8K;
    result = g_audio_dev->play_init(id, &acw_ao_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_play_thread == NULL) {
        uparm.acw_uDevID = id;
        uparm.uFrmSize = acw_ao_cfg.frame_size;

        pthread_create(&g_audio_play_thread, 0, audio_play_file_Callback, &uparm);
        ALOGD("%s create thread:acw_dev_id:%d, frame_size:%d\n", __func__, id, acw_ao_cfg.frame_size);
    }
    pthread_join(g_audio_play_thread, 0);
    g_audio_play_thread = NULL;
    return 0;
}

QUA_VOID* audio_record_play_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s acw_ai_frame, acw_ao_frame;

    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_record_file_Callback dev_id:%d, frame_size:%d\n", uparm.acw_uDevID, uparm.uFrmSize);

    acw_ai_frame.u32Len = uparm.uFrmSize;
    acw_ao_frame.u32Len = uparm.uFrmSize;
    acw_ai_frame.bufVirAddr = buf;
    acw_ao_frame.bufVirAddr = buf;

    while (1) {
        if (g_audio_record_play_thread != NULL) {
            ret = g_audio_dev->read_frame(uparm.acw_uDevID, acw_ai_frame.bufVirAddr, acw_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev %d,read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.acw_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }

            ret = g_audio_dev->write_frame(uparm.acw_uDevID, acw_ao_frame.bufVirAddr, acw_ao_frame.u32Len);
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
    g_audio_dev->capture_stop(uparm.acw_uDevID);
    g_audio_dev->play_stop(uparm.acw_uDevID);
    g_audio_dev->exit(uparm.acw_uDevID);

    g_audio_dev->capture_deinit(uparm.acw_uDevID);
    g_audio_dev->play_deinit(uparm.acw_uDevID);

    pthread_exit(0);
    return NULL;

}

int audio_record_play(int id)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg acw_ai_cfg, acw_ao_cfg;

    acw_ai_cfg.enc_type = QUA_PT_LPCM;
    acw_ai_cfg.io_type = QUA_MIC_IN;
    acw_ai_cfg.frame_size = 7680;
    acw_ai_cfg.frame_cnt = 3;
    acw_ai_cfg.channel_bit = QUA_BW_16;
    acw_ai_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->capture_init(id, &acw_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_volume(id, QUA_VOLUME_TYPE_ANA, 1);
    g_audio_dev->capture_volume(id, QUA_VOLUME_TYPE_DIG, 0x64);
    g_audio_dev->capture_start(id);

    acw_ao_cfg.enc_type = QUA_PT_LPCM;
    acw_ao_cfg.io_type = QUA_LINE_HPOUT;
    acw_ao_cfg.frame_size = 7680;
    acw_ao_cfg.frame_cnt = 3;
    acw_ao_cfg.channel_bit = QUA_BW_16;
    acw_ao_cfg.rate = QUA_SR_48K;

    result = g_audio_dev->play_init(id, &acw_ao_cfg);

    if (result != QUA_SUCCESS) {
        ALOGD("g_audio_dev play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id);

    if (g_audio_record_play_thread == NULL) {
        uparm.acw_uDevID = id;
        uparm.uFrmSize = acw_ai_cfg.frame_size;

        pthread_create(&g_audio_record_play_thread, 0, audio_record_play_Callback, &uparm);
        ALOGD("%s create thread:acw_dev_id:%d,frame_size:%d\n", __func__, id, acw_ai_cfg.frame_size);
    }
    pthread_join(g_audio_record_play_thread, 0);
    g_audio_record_play_thread = NULL;
    return 0;

}

QUA_VOID* audio_play_file_record_Callback(QUA_VOID* pAtr)
{
    QUA_U32 uCnt = 0;
    QUA_S32 ret = 0;
    thread_codec_param uparm;
    static qua_frame_s acw_ai_frame, acw_ao_frame;
    FILE *play_fp = NULL;
    FILE *record_fp = NULL;
    thread_codec_param *p = pAtr;
    QUA_U8 buf[p->uFrmSize];
    memset(buf, 0, p->uFrmSize);

    memcpy(&uparm, (thread_codec_param*)pAtr, sizeof(thread_codec_param));
    ALOGD("audio_record_play_Callback acw_dev_id:%d, frame_size:%d\n", uparm.acw_uDevID, uparm.uFrmSize);

    acw_ai_frame.u32Len = uparm.uFrmSize;
    acw_ao_frame.u32Len = uparm.uFrmSize;
    acw_ai_frame.bufVirAddr = buf;
    acw_ao_frame.bufVirAddr = buf;

    record_fp = fopen("/data/media/play_and_record.pcm", "wb+");
    if (NULL == record_fp) {
        ALOGD("fopen record_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("record open file succeed!\n");
    }

    play_fp = fopen(filename_8k_16bit_stereo, "rb");
    if (NULL == play_fp) {
        ALOGD("fopen play_fp is fail!\n");
        goto exit;
    } else {
        ALOGD("open file succeed!\n");
    }

    while (1) {
        if (g_audio_play_file_record_thread != NULL) {
            while (fread(acw_ao_frame.bufVirAddr, 1, acw_ao_frame.u32Len, play_fp) != acw_ao_frame.u32Len) {
                fseek(play_fp, 0, SEEK_SET);
            }

            ret = g_audio_dev->write_frame(uparm.acw_uDevID, acw_ao_frame.bufVirAddr, acw_ao_frame.u32Len);
            if (ret != QUA_SUCCESS) {
                ALOGD("g_audio_dev write_frame is failed!\n");
                goto exit;
            }

            ret = g_audio_dev->read_frame(uparm.acw_uDevID, acw_ai_frame.bufVirAddr, acw_ai_frame.u32Len, NULL);
            if (ret != QUA_SUCCESS) {
                ALOGD("%s: Dev %d, read_frame failed with uCnt %d !\n", __FUNCTION__, uparm.acw_uDevID, uCnt);
                usleep(40 * 1000);
                continue;
            }
            fwrite(acw_ai_frame.bufVirAddr, 1, acw_ai_frame.u32Len, record_fp);  /**/
            uCnt++;
        } else {
            ALOGD("audio_play_file_record_Callback thread exit!\n");
            goto exit;
        }
    }

exit:
    g_audio_dev->capture_stop(uparm.acw_uDevID);
    g_audio_dev->play_stop(uparm.acw_uDevID);
    g_audio_dev->exit(uparm.acw_uDevID);
    g_audio_dev->capture_deinit(uparm.acw_uDevID);
    g_audio_dev->play_deinit(uparm.acw_uDevID);

    pthread_exit(0);
    return NULL;

}

int audio_play_file_record(int id_acw)
{
    QUA_S32 result;
    static thread_codec_param uparm;
    qua_audio_cfg_arg acw_ai_cfg, acw_ao_cfg;

    acw_ai_cfg.enc_type = QUA_PT_LPCM;
    acw_ai_cfg.io_type = QUA_MIC_IN;
    acw_ai_cfg.frame_size = 1280;
    acw_ai_cfg.frame_cnt = 3;
    acw_ai_cfg.channel_bit = QUA_BW_16;
    acw_ai_cfg.rate = QUA_SR_8K;

    result = g_audio_dev->capture_init(id_acw, &acw_ai_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("capture_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->capture_start(id_acw);

    acw_ao_cfg.enc_type = QUA_PT_LPCM;
    acw_ao_cfg.io_type = QUA_LINE_HPOUT;
    acw_ao_cfg.frame_size = 1280;
    acw_ao_cfg.frame_cnt = 3;
    acw_ao_cfg.channel_bit = QUA_BW_16;
    acw_ao_cfg.rate = QUA_SR_8K;

    result = g_audio_dev->play_init(id_acw, &acw_ao_cfg);
    if (result != QUA_SUCCESS) {
        ALOGD("play_init failed!\n");
        return QUA_FAILURE;
    }
    g_audio_dev->play_start(id_acw);

    if (g_audio_play_file_record_thread == NULL) {
        uparm.acw_uDevID = id_acw;
        uparm.uFrmSize = acw_ai_cfg.frame_size;

        pthread_create(&g_audio_play_file_record_thread, 0, audio_play_file_record_Callback, &uparm);
        ALOGD("%s create thread:acw_dev_id:%d, frame_size:%d\n", __func__, id_acw, acw_ai_cfg.frame_size);
    }
    pthread_join(g_audio_play_file_record_thread, 0);
    g_audio_play_file_record_thread = NULL;
    return 0;
}

void audio_stop_clr(void)
{
    g_audio_record_thread = NULL;
    g_audio_play_thread = NULL;
    g_audio_record_play_thread = NULL;
    g_audio_play_file_record_thread = NULL;
    usleep(500 * 1000);
}

QUA_S32 sample_aio_test(QUA_U32 cmd)
{
    if(cmd != 8)
        audio_stop_clr();
    switch(cmd)
    {
        case 1:
        {
            audio_record_file(QUA_DEV_ACW_STEREO);
            ALOGD("\n\033[31m ########  [AUDIO]START RECORD FILES!  ########\033[0m\n");
        }
        break;

        case 2:
        {
            audio_play_file(QUA_DEV_ACW_STEREO);
            ALOGD("\n\033[31m ########  [AUDIO]START PLAY FILES!  ########\033[0m\n");
        }
        break;

        case 3:
        {
             audio_record_play(QUA_DEV_ACW_STEREO);
             ALOGD("\n\033[31m ########  [AUDIO]ACW record and ACW play! ########\033[0m\n");
        }
        break;

        case 4:
        {
             audio_play_file_record(QUA_DEV_ACW_STEREO);
             ALOGD("\n\033[31m ########  [AUDIO]ACW playfile and ACW record! ########\033[0m\n");
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
    QUA_CONST_CHAR *platform = QUA_SYS_MAKE_PLATFORM(QUA_SYS_CHIP_MC331x, QUA_SYS_OS_LINUX);
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

    ret = audio_dev_init();
    if (ret != QUA_SUCCESS) {
        printf("audio dev init err\n");
        goto end;
    }

    if (argc > 1) {
        cmd = atoi(argv[1]);
    } else {
        audio_record_play(QUA_DEV_ACW_STEREO);
        (void)audio_dev_deinit();
        return 0;
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
