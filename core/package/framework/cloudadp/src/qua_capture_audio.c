#include "qua_capture_audio.h"
#include "pthread.h"
#include <sys/prctl.h>

#define MAX_PIPELINE_ID   2

#define AUDIO_MIME_TYPE "audio/L16"

#if defined(CONFIG_XOS_FWK_CADP_AGORA)
#define AUDIO_MIME_TYPE "audio/g711-alaw"
#define AUDIO_FRAME_FORMAT  QUA_CAF_ALAW
#define AUDIO_SAMPLE_FMT QUA_SAMPLE_FMT_S16
#define AUDIO_CAP_FRAME_CNT (20 * AUDIO_CAP_SAMPLE_RATE) / 1000
#define AUDIO_CAP_BIT_WIDTH 16
#endif

#if defined(CONFIG_XOS_FWK_CADP_KVS)
#define AUDIO_MIME_TYPE "audio/g711-alaw"
#define AUDIO_FRAME_FORMAT  QUA_CAF_ALAW
#define AUDIO_SAMPLE_FMT QUA_SAMPLE_FMT_S16
#define AUDIO_CAP_FRAME_CNT (20 * AUDIO_CAP_SAMPLE_RATE) / 1000
#define AUDIO_CAP_BIT_WIDTH 8
#endif

//cap_info->audio_cap.sample_rate = 8000;
//    cap_info->audio_cap.chns = 1;
//    cap_info->audio_cap.bit_width = 16;
//    cap_info->audio_cap.frame_cnt = cap_info->audio_cap.frame_size = (20 *cap_info->audio_cap.sample_rate) / 1000; // 40ms

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0

#define ENABLE_AUDIO_START_AND_STOP 1
#define ENABLE_SEND_PCM_DIRECTLY    1 //todo: set by config params
#define ENALBE_DUMP_AUDIO_PCM       0

#if ENABLE_SEND_PCM_DIRECTLY
#define AUDIO_CAP_SAMPLE_RATE 16000
#define AUDIO_FRAME_FORMAT  QUA_CAF_PCM
#else //ENABLE_SEND_PCM_DIRECTLY
#define AUDIO_CAP_SAMPLE_RATE 8000
#endif //ENABLE_SEND_PCM_DIRECTLY

#define AUDIO_CAP_CHNS 1

static qua_mm_audio_device_t *g_audio_dev = NULL;
static qua_mm_encoder_device_t* g_avenc_dev = NULL;
static qua_mm_encoder_channel_t *g_avenc_chn = NULL;

static pthread_t thiz_thread;
static pthread_cond_t thiz_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t thiz_mutex = PTHREAD_MUTEX_INITIALIZER;

static volatile QUA_BOOL g_run_stop = QUA_TRUE;
static qua_capture_stream_cb g_audio_stream_cb= NULL;

static qua_packet_t* g_packet = NULL;
static qua_audio_frame_ext_t* g_frame = NULL;

static int g_dev_id = QUA_DEV_ACW0;

#if ENALBE_DUMP_AUDIO_PCM
static FILE * g_dump_file = NULL;
#endif //ENALBE_DUMP_AUDIO_PCM

static void packet_set_default(qua_packet_t* qua_pkt)
{
    memset(qua_pkt->data, 0x00, qua_pkt->size);
    qua_pkt->offset = 0;
    qua_pkt->capacity = 0;
    qua_pkt->pts = -1;
    qua_pkt->dts = -1;
    qua_pkt->duration = 0;
    qua_pkt->baseTime = 0;
    qua_pkt->streamIndex = -1;
}

static qua_packet_t* alloc_packet(int size)
{
    qua_packet_t* qua_pkt = (qua_packet_t*)calloc(1, sizeof(qua_packet_t));
    if (qua_pkt == NULL)
        return NULL;

    qua_pkt->data = (uint8_t*)calloc(size, sizeof(uint8_t));
    if (qua_pkt->data == NULL) {
        free(qua_pkt);
        qua_pkt = NULL;
        return NULL;
    }
    qua_pkt->size = size;
    packet_set_default(qua_pkt);
    return qua_pkt;
}

static QUA_S32 quamm_audio_device_init() 
{
    const qua_mm_module_t *audio_module = NULL;
    qua_mm_device_t *mm_device = NULL;
    QUA_S32 ret = QUA_SUCCESS;

    ret = qua_mm_load_module(QUA_MM_MODULE_AUDIO, &audio_module);
    if (ret != QUA_SUCCESS || audio_module == NULL) {
        printf("open audio module fail");
        return -1;
    }
    ret = audio_module->open_device(audio_module, QUA_MM_AUDIO_DEV, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open audio device fail");
        return -1;
    }
    g_audio_dev = (qua_mm_audio_device_t *)mm_device;

    const qua_mm_module_t *ext_module = NULL;
    ret = qua_mm_load_module(QUA_MM_MODULE_AVEXT, &ext_module);
    if (ret != QUA_SUCCESS || ext_module == NULL) {
        printf("open avext module fail");
        return -1;
    }
    ret = ext_module->open_device(ext_module, QUA_MM_EXT_ENCODER, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open audio encoder device fail");
        return -1;
    }
    g_avenc_dev = (qua_mm_encoder_device_t *)mm_device;
    return 0;
}

static void free_packet(qua_packet_t* qua_pkt)
{
    if (qua_pkt && qua_pkt->data) {
        free(qua_pkt->data);
        qua_pkt->data = NULL;
    }
    free(qua_pkt);
    qua_pkt = NULL;
}

static void frame_set_default(qua_audio_frame_ext_t* frame)
{
    //const qua_media_platform_capability_info_t *cap_info = qua_media_platform_capability_query();
    memset(frame->data, 0x00, frame->size);
    frame->offset = 0;
    frame->capacity = 0;
    frame->nb_samples = 0;
    frame->sample_fmt = AUDIO_SAMPLE_FMT;
    frame->sample_rate = AUDIO_CAP_SAMPLE_RATE;
    frame->nb_channels = AUDIO_CAP_CHNS;
    frame->flags = 0;
}

static qua_audio_frame_ext_t* alloc_frame(int size)
{
    qua_audio_frame_ext_t *frame = (qua_audio_frame_ext_t *)calloc(1, sizeof(qua_audio_frame_ext_t));
    if (frame == NULL) {
        printf("OOM? frame calloc\n");
        return NULL;
    }
    frame->data = (uint8_t *)calloc(size, sizeof(uint8_t));
    if (frame->data == NULL) {
        printf("OOM? frame->data calloc\n");
        free(frame);
        frame = NULL;
        return NULL;
    }
    frame->size = size;

    frame_set_default(frame);
    return frame;
}

static void free_frame(qua_audio_frame_ext_t *frame)
{
    if (frame && frame->data) {
        free(frame->data);
        frame->data = NULL;
    }
    free(frame);
    frame = NULL;
}

static int get_pts_duration_us(int frame_cnt, int sr)
{
    return  1000 * frame_cnt * 1000 / sr;
}

static int get_bytes_by_bit(int chn_bit)
{
    int bytes = 0;
    switch (chn_bit) {
        case QUA_BW_8:
            bytes = 1;
            break;
        case QUA_BW_32:
            bytes = 4;
            break;
        case QUA_BW_16:
        default:
            bytes = 2;
            break;
    }
    return bytes;
}

static int create_avenc_chn(void)
{
    qua_audio_config_t audio_cfg = { 0 };
    qua_mm_channel_t *mm_chn = NULL;
    qua_ext_codec_chn_attr_t codec_chn_attr = { 0 };
    QUA_S32 chn_id = 0;
    QUA_S32 ret = 0;
    //const qua_media_platform_capability_info_t *cap_info = qua_media_platform_capability_query();

    //codec_chn_attr.mimetype = "audio/L16";
    codec_chn_attr.mimetype = AUDIO_MIME_TYPE;
    codec_chn_attr.mime_extra = NULL;

    audio_cfg.out_nb_channels = audio_cfg.in_nb_channels = AUDIO_CAP_CHNS;
    audio_cfg.out_sample_rate = audio_cfg.in_sample_rate = AUDIO_CAP_SAMPLE_RATE;
    audio_cfg.out_sample_fmt = audio_cfg.in_sample_fmt = AUDIO_SAMPLE_FMT;
    audio_cfg.profile = 1;
    audio_cfg.bit_rate = 128000;
    audio_cfg.nb_samples = AUDIO_CAP_FRAME_CNT;
    if (!strncmp(codec_chn_attr.mimetype, "audio/mp4a-latm", strlen((char*)"audio/mp4a-latm"))) {
        audio_cfg.trans_type = QUA_TT_MP4_RAW;
    }
    codec_chn_attr.param = (void *)&audio_cfg;
    ret = g_avenc_dev->parent.create_channel(&g_avenc_dev->parent, &chn_id, (QUA_VOID_PTR)&codec_chn_attr, &mm_chn);
    if (ret != QUA_SUCCESS || mm_chn == NULL) {
        printf("enc mm_chn is NULL\n");
        return -1;
    }
    g_avenc_chn = (qua_mm_encoder_channel_t *)mm_chn;

    return 0;
}

static int qua_media_ai_start(int id)
{
    QUA_S32 ret = QUA_SUCCESS;
    qua_audio_cfg_arg ai_cfg = { 0 };
    //const qua_media_platform_capability_info_t *cap_info = qua_media_platform_capability_query();
    if (!g_audio_dev) {
        printf("start g_audio_dev is NULL!");
        return -1;
    }

    ai_cfg.io_type = QUA_MIC_IN;
    ai_cfg.frame_cnt = 6;// QUA_DEFAULT_AI_CACHE_NUM;
    ai_cfg.channel_bit = QUA_BW_16;
    ai_cfg.rate = AUDIO_CAP_SAMPLE_RATE;
    ai_cfg.channels = AUDIO_CAP_CHNS;
    ai_cfg.frame_size = AUDIO_CAP_FRAME_CNT;

    // printf("qua_media_ai_start sample_rate: %d, channels: %d, frame_cnt: %d, frame_size: %d\n", ai_cfg.rate, ai_cfg.channels, ai_cfg.frame_cnt, ai_cfg.frame_size);
    ret = g_audio_dev->capture_init(id, &ai_cfg);
    if (ret != QUA_SUCCESS) {
        printf("g_audio_dev capture_init fail!");
        return -1;
    }
    // ret = g_audio_dev->capture_volume(id, QUA_VOLUME_TYPE_ANA, 31);
    // if (ret != QUA_SUCCESS) {
    //     printf("g_audio_dev set ana volume fail!");
    //     return -1;
    // }
    // ret = g_audio_dev->capture_volume(id, QUA_VOLUME_TYPE_DIG, 0x7f);
    // if (ret != QUA_SUCCESS) {
    //     printf("g_audio_dev set dig volume fail!");
    //     return -1;
    // }
#if ENABLE_AUDIO_START_AND_STOP
    ret = g_audio_dev->capture_start(id);
    if (ret != QUA_SUCCESS) {
        printf("g_audio_dev set dig volume fail!");
        return -1;
    }
#endif
    g_frame = alloc_frame(AUDIO_CAP_FRAME_CNT * get_bytes_by_bit(ai_cfg.channel_bit) * ai_cfg.channels);
    if (g_frame == NULL) {
        printf("g_frame is NULL!");
        return -1;
    }
    //g_dev_id = id;

    return 0;
}

static int qua_media_ai_stop(int id)
{
    if (g_audio_dev == NULL) {
        printf("stop g_audio_dev is NULL\n");
        return -1;
    }
#if ENABLE_AUDIO_START_AND_STOP
    g_audio_dev->capture_stop(id);    
#endif
    g_audio_dev->exit(id);
    g_audio_dev->capture_deinit(id);
    free_frame(g_frame);
    g_frame = NULL;
    // printf("qua_media_ai_stop success\n");
    return 0;
}

static int qua_media_avenc_start(void)
{
    if (create_avenc_chn() < 0) {
        printf("create_avenc_chn fail");
        return -1;
    }
    g_packet = alloc_packet(128 * 1000); // 128k estimate maybe enough
    if (g_packet == NULL) {
        printf("g_packet is NULL");
        return -1;
    }

    return g_avenc_chn->start(g_avenc_chn->encoder);
}

static int qua_media_aenc_stop(void)
{
    if (g_avenc_chn == NULL) {
        printf("stop g_avenc_chn is NULL\n");
        return -1;
    }
    if (g_avenc_chn->stop(g_avenc_chn->encoder) < 0) {
        printf("stop avenc_chn fail");
    }

    g_avenc_chn->parent.release(&g_avenc_chn->parent);
    g_avenc_chn = NULL;
    free_packet(g_packet);
    g_packet = NULL;
    return 0;
}

static int send_stream(qua_packet_t* packet)
{
    if (g_audio_stream_cb == NULL) {
        return -1;
    }
    // printf("packet->pts: %llu, packet->capacity: %d", packet->pts, packet->capacity);
    //const qua_media_platform_capability_info_t *cap_info = qua_media_platform_capability_query();
    qua_camera_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.stream_format = QUA_CRF_AUDIO;
    stream_info.frame_format = AUDIO_FRAME_FORMAT;
    stream_info.frame_pts = packet->pts;
    stream_info.frame_data =  packet->data;
    stream_info.frame_len =  packet->capacity;

    for (int idx = 0; idx < MAX_PIPELINE_ID; idx++) {
        g_audio_stream_cb(idx, QUA_STREAM_TYPE_AUDIO,  &stream_info);
        //if (!cap_info->video_cap.has_sub)
            break;
    }

    return 0;
}

static void *audio_stream_theadloop(QUA_VOID *arg)
{
    char name[32] = { 0 };
    QUA_S32 ret = -1;
    //const qua_media_platform_capability_info_t *cap_info = qua_media_platform_capability_query();
    unsigned long long pts_duration = get_pts_duration_us(AUDIO_CAP_FRAME_CNT, AUDIO_CAP_SAMPLE_RATE);
    unsigned long long pts = 0;

    strncpy(name, "audio_stream_loop", strlen("audio_stream_loop"));
    prctl(PR_SET_NAME, name);
    g_frame->pts = 0;
    while (!g_run_stop) {
        frame_set_default(g_frame);
        ret = g_audio_dev->read_frame(g_dev_id, g_frame->data, g_frame->size, &pts);
        // static int count = 0;
        // if(count++%200 == 0){
        //     printf("read_frame ret = %d, g_frame->size: %d\n", ret, g_frame->size);    
        // }
        if (ret != QUA_SUCCESS) {
            printf("read_frame failed ret = %d\n", ret);
            usleep(20*1000);
            continue;
        }else{
#if ENALBE_DUMP_AUDIO_PCM
            if (g_dump_file != NULL) {
                fwrite(g_frame->data, g_frame->size, 1, g_dump_file);
            }
#endif //ENALBE_DUMP_AUDIO_PCM
        }
        g_frame->nb_samples = AUDIO_CAP_FRAME_CNT;
        g_frame->capacity = g_frame->size;
        if (pts == 0) {
            g_frame->pts += pts_duration;
        } else {
            g_frame->pts = pts;
        }
        #if ENABLE_SEND_PCM_DIRECTLY == 0 //zgr
        ret = g_avenc_chn->send_frame(g_avenc_chn->encoder, g_frame);
        // printf("send_frame ret = %d", ret);
        if (ret != QUA_SUCCESS) {
            // warning
            printf("avenc send frame failed: %d\n", ret);
            continue;
        }
        packet_set_default(g_packet);
        ret = g_avenc_chn->receive_packet(g_avenc_chn->encoder, g_packet);
        // printf("receive_packet ret = %d", ret);
        #else //ENABLE_SEND_PCM_DIRECTLY
            memcpy(g_packet->data, g_frame->data, g_frame->size);
            g_packet->pts = g_frame->pts;
            g_packet->dts = g_frame->pts;
            g_packet->capacity = g_frame->size;
        #endif //ENABLE_SEND_PCM_DIRECTLY zgr temp
        if (ret != QUA_SUCCESS) {
            // warning
            printf("avenc receive packet failed: %d\n", ret);
            continue;
        } else {
            send_stream(g_packet);
        }
    }
    g_avenc_chn->flush(g_avenc_chn->encoder);
    printf("audio_stream_theadloop eixt!\n");
    return NULL;
}

static int qua_media_audio_startup(qua_camera_stream_cb stream_callback)
{
    int ret;
    pthread_attr_t attr;
    struct sched_param param;

    g_run_stop = QUA_FALSE;
    g_audio_stream_cb = stream_callback;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&attr, 8 * 1024);
#ifdef __RTTHREAD_OS__
    param.sched_priority = 130;
#endif
    pthread_attr_setschedparam(&attr, &param);
    ret = pthread_create(&thiz_thread, &attr, audio_stream_theadloop, NULL);
    pthread_attr_destroy(&attr);
    if (ret != 0){
        printf("create audio startup thiz_thread fail!\n");
        return -1;
    }

    return ret;
}

QUA_U32 set_audioCapture_volume(QUA_U32 volume)
{
    if (g_run_stop || g_audio_dev == NULL)
        return 0;
    int ret = 0;
    if(volume == 0){
        printf("*** attention here, the capture volume is set to 0 ***\n");
    }
    ret = g_audio_dev->capture_volume(g_dev_id, 0, volume);
    if (ret != QUA_SUCCESS)
    {
        printf("Failed to set mic capture_volume volume!\n");
        return -1;
    }
    printf("SUCESS to set mic capture_volume volume: %d!\n", volume);
    return ret;
}

QUA_S32 qua_capture_audio_start(int id, qua_camera_stream_cb stream_callback)
{
    id = g_dev_id;
#if ENALBE_DUMP_AUDIO_PCM
    if(g_dump_file == NULL) {
        g_dump_file = fopen("/mnt/sdcard/audio_dump.pcm", "wb");
    }
#endif //ENALBE_DUMP_AUDIO_PCM   
    //g_audio_dev = quamm_audio_device_get();
    //g_avenc_dev = quamm_avenc_device_get();
    quamm_audio_device_init();
    if (qua_media_ai_start(id) < 0) {
        printf("media ai start fail");
        return -1;
    }
    if (qua_media_avenc_start() < 0) {
        printf("media avenc start fail");
        return -1;
    }

    qua_media_audio_startup(stream_callback);
    return 1;
}

QUA_S32 qua_capture_audio_stop(int id)
{
    if (g_run_stop)
        return 0;

    id = g_dev_id;
    g_run_stop = QUA_TRUE;
    pthread_join(thiz_thread, NULL);

    if (qua_media_ai_stop(id) < 0) {
        printf("media ai stop fail");
        return -1;
    }
    if (qua_media_aenc_stop() < 0) {
        printf("media aenc stop fail");
        return -1;
    }
#if ENALBE_DUMP_AUDIO_PCM
    if(g_dump_file) {
        fclose(g_dump_file);
        g_dump_file = NULL;
    }
#endif //ENALBE_DUMP_AUDIO_PCM
    return 0;
}

#endif