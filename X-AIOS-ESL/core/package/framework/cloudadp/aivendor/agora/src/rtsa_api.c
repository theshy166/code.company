/*************************************************************
 * File  :  rtsa_api.c
 * Module:  Agora SD-RTN SDK RTC C API demo application.
 *
 * This is a part of the Agora RTC Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifdef CONFIG_XOS_FWK_CADP_AGORA
#include "app_config.h"
#include "rtsa_api.h"
#include "vc_ability.h"
#include "qua_camera.h"

#include "cadp_videocall.h"
#include "../utility/file_writer.h"
#include <sys/prctl.h>
#include "qua_capture_audio.h"



#define CFG_ENABLE_DUMP_VIDEO 0
#define CFG_ENABLE_DUMP_AUDIO 0
#define CFG_ENABLE_DUMP_SEND_AUDIO 0
#define CFG_ENABLE_DUMP_SEND_VIDEO 0

#define PUSH_H264_STREAM 1
#define CFG_ENABLE_SUBSTREAM_VIDEO 1

#ifdef CONFIG_XOS_USE_APP_MOMO_TALK
#define CFG_ENABLE_SUBSTREAM_AUDIO 0  //TODO: not send audio data here, for AEC (echo cancellation)
#else //CONFIG_XOS_USE_APP_MOMO_TALK
#define CFG_ENABLE_SUBSTREAM_AUDIO 1
#endif //CONFIG_XOS_USE_APP_MOMO_TALK

#define CFG_ENABLE_AUDIO_SIMULATOR 0 //zgr temp, not test, don't use, it will be removed later

#define START_WRITE_FIEL 0

typedef struct
{
  app_config_t config;

  void *video_file_parser;
  void *video_file_writer;

  void *audio_file_parser;
  void *audio_file_writer;
  void *audio_send_file_writer;
  void *video_send_file_writer;

  connection_id_t conn_id;
  bool b_stop_flag;
  bool b_stop_completed_flag;
  bool b_connected_flag;
  bool remote_connected_flag;
  app_callback_t callback;
  pthread_cond_t b_stop_cond_var;
  pthread_mutex_t b_stop_mutex;
} app_t;

static app_t g_app;

ResolutionStream resolutions[] = {
    {640, 480, 512 * 1024},    // 标清
    {1280, 720, DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE},  // 高清
    {1920, 1080, DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE}, // 超清
};

void reset_app_t_structure(void)
{
  app_t app = {
      .config = {
          // common config
          .p_sdk_log_dir = XOS_RES_RTC_LOG_PATH, // todo
          .log_level = -1,                       // RTC_LOG_DEBUG,
          .p_appid = "",
          .p_channel = DEFAULT_CHANNEL_NAME,
          .p_token = "",
          .p_license = "",
          .uid = 0,
          .area = AREA_CODE_GLOB,

          // video related config
          .video_data_type = VIDEO_DATA_TYPE_H264,
          .send_video_frame_rate = DEFAULT_SEND_VIDEO_FRAME_RATE,
          .send_video_file_path = NULL,

          // audio related config
          .audio_data_type = AUDIO_DATA_TYPE_PCM,
          .audio_codec_type = AUDIO_CODEC_TYPE_OPUS, //zgr
          .send_audio_file_path = NULL,

          // pcm related config
          .pcm_sample_rate = DEFAULT_PCM_SAMPLE_RATE,
          .pcm_channel_num = DEFAULT_PCM_CHANNEL_NUM,
          .pcm_duration = DEFAULT_SEND_AUDIO_FRAME_PERIOD_MS,

          // advanced config
          .enable_audio_mixer = false,
          .receive_data_only = false,
          .local_ap = "",
          .domain_limit = false,
          .video_handle = -1,
          .audio_handle = -1,
          .video_mask = 0,
          .audio_mask = 0,
          .pVideobuffer = NULL,
          .videoBuffersize = 0,
          .pAudiobuffer = NULL,
          .audioBuffersize = 0,
          .resolution = 1,
          .video_exist = 0,
          .audio_volume = -1,
      },

      .video_file_parser = NULL,
      .video_file_writer = NULL,

      .audio_file_parser = NULL,
      .audio_file_writer = NULL,

      .audio_send_file_writer = NULL,
      .video_send_file_writer = NULL,

      .b_stop_flag = false,
      .b_stop_completed_flag = true,
      .b_connected_flag = false,
      .b_stop_cond_var = PTHREAD_COND_INITIALIZER,
      .b_stop_mutex = PTHREAD_MUTEX_INITIALIZER,
      .callback = {.on_status_changed = NULL, 
                   .on_recv_audio = NULL, 
                   .on_recv_video = NULL,
                   .cb_context = NULL}
  };
  memcpy(&g_app, &app, sizeof(app_t));
}

static void app_signal_handler(int sig)
{
  switch (sig)
  {
  case SIGINT:
    g_app.b_stop_flag = true;
    break;
  default:
    LOGW("no handler, sig=%d", sig);
  }
}

static void app_callback_init(qua_videocall_t *cb)
{
  g_app.callback = cb->callback;
}

static int app_init(void)
{
#if 0
  app_config_t *config = &g_app.config;
  parser_cfg_t parser_cfg;

  signal(SIGINT, app_signal_handler);

  memset(&parser_cfg, 0, sizeof(parser_cfg_t));
  if (!config->receive_data_only)
  {
    g_app.video_file_parser =
        create_file_parser(video_data_type_to_file_type(config->video_data_type), config->send_video_file_path, NULL);
    if (!g_app.video_file_parser)
    {
      LOGE("create file parser for video failed");
      return -1;
    }

    parser_cfg.u.audio_cfg.sampleRateHz = config->pcm_sample_rate;
    parser_cfg.u.audio_cfg.numberOfChannels = config->pcm_channel_num;
    parser_cfg.u.audio_cfg.framePeriodMs = config->pcm_duration;

    g_app.audio_file_parser =
        create_file_parser(audio_data_type_to_file_type(config->audio_data_type), config->send_audio_file_path, &parser_cfg);
    if (!g_app.audio_file_parser)
    {
      LOGE("create file parser for audio failed");
      return -1;
    }
  }
#endif

#if CFG_ENABLE_DUMP_VIDEO
  g_app.video_file_writer = create_file_writer(FILE_TYPE_VIDEO, DEFAULT_RECV_VIDEO_BASENAME);
#endif

#if CFG_ENABLE_DUMP_AUDIO
  g_app.audio_file_writer = create_file_writer(FILE_TYPE_AUDIO, DEFAULT_RECV_AUDIO_BASENAME);
#endif

#if CFG_ENABLE_DUMP_SEND_AUDIO
  g_app.audio_send_file_writer = create_file_writer(FILE_TYPE_AUDIO, DEFAULT_SEND_AUDIO_BASENAME);
#endif

#if CFG_ENABLE_DUMP_SEND_VIDEO
  g_app.video_send_file_writer = create_file_writer(FILE_TYPE_VIDEO, DEFAULT_SEND_VIDEO_BASENAME);
#endif
  return 0;
}

static void unsubscribe_stream()
{
  LOGI("unsubscribe_stream\n");
  int ret = 0;
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#if PUSH_H264_STREAM
#if CFG_ENABLE_SUBSTREAM_VIDEO
  if (g_app.config.video_exist && g_app.config.video_handle != -1)
  {
    ret = qua_camera_unsubscribe_stream(g_app.config.video_handle, g_app.config.video_mask);
    if (ret != 0)
      LOGI("qua_camera_unsubscribe_stream failed!\n");
    if (g_app.config.pVideobuffer != NULL)
    {
      free(g_app.config.pVideobuffer);
      g_app.config.pVideobuffer = NULL;
    }
    g_app.config.video_handle = -1;
  }
#endif
#if CFG_ENABLE_SUBSTREAM_AUDIO
  if (g_app.config.audio_handle != -1)
  { // audio
    //ret = qua_camera_unsubscribe_stream(g_app.config.audio_handle, g_app.config.audio_mask);
    ret = qua_capture_audio_stop(0);
    if (ret != 0)
      LOGI("qua_camera_unsubscribe_stream audio failed!\n");
    if (g_app.config.pAudiobuffer != NULL)
    {
      free(g_app.config.pAudiobuffer);
      g_app.config.pAudiobuffer = NULL;
    }
    g_app.config.audio_handle = -1;
  }
#endif
  // qua_camera_action(0, QUA_CA_RECORD);
#endif
#endif
}

static void *sendAudio_thread(void *param)
{
  audio_frame_info_t info = {0};
  qua_camera_stream_info_t *stream_info = (qua_camera_stream_info_t *)param;
  info.data_type = AUDIO_DATA_TYPE_PCMA;
  // write_data_to_file((void *)stream_info, stream_type, output_audio);
  if (stream_info->frame_len > g_app.config.audioBuffersize)
  {
    g_app.config.pAudiobuffer = (unsigned char *)realloc(g_app.config.pAudiobuffer, stream_info->frame_len);
    if (g_app.config.pAudiobuffer == NULL)
    {
      LOGE("Failed to realloc pAudiobuffer memory!!!!");
      return;
    }
    g_app.config.pAudiobuffer = stream_info->frame_len;
  }
  memcpy(g_app.config.pAudiobuffer, stream_info->frame_data, g_app.config.pAudiobuffer);
  int rval = agora_rtc_send_audio_data(g_app.conn_id, g_app.config.pAudiobuffer, g_app.config.pAudiobuffer, &info);
  if (rval < 0)
  {
    LOGE("Failed to send audio data, reason: %s", agora_rtc_err_2_str(rval));
    // file_parser_release_frame(g_app.audio_file_parser, &frame);
    return;
  }
}

static int s_audio_frame = 0;
static int app_send_audio(int channel, qua_steram_type_t stream_type, qua_camera_stream_info_t *stream_info)
{
  // LOGI("[AGORA Master] app_send_audio %d-%d-%d-%d, len=%d\n", channel, stream_type, stream_info->stream_format, stream_info->frame_format, stream_info->frame_len);
  //  API: send audio data
  if (g_app.b_connected_flag && g_app.remote_connected_flag && !g_app.b_stop_flag)
  {
    s_audio_frame++;
  if(s_audio_frame % 500 == 0)
  {
    printf(">>>>>>>s_audio_frame<<<<<<< %d-%d-%d-%d, len=%d\n", channel, stream_type, stream_info->stream_format, stream_info->frame_format, stream_info->frame_len);
  }
      audio_frame_info_t info = {0};

    switch(stream_info->frame_format)
    {
      case QUA_CAF_PCM:
        info.data_type = AUDIO_DATA_TYPE_PCM;
        if(g_app.config.audio_data_type != AUDIO_DATA_TYPE_PCM)
        {
          LOGI("**[rtsa] audio data type different from %d to %d\n", g_app.config.audio_data_type, info.data_type);
        }
        break;
      case QUA_CAF_ALAW:
        info.data_type = AUDIO_DATA_TYPE_PCMA; 
        break;
      case QUA_CAF_ULAW:
        info.data_type = AUDIO_DATA_TYPE_PCMU; 
        break;
      default:
        LOGI("**[rtsa] frame format still not supported: %d\n", stream_info->frame_format);
        break;
    }
    // LOGI("[AGORA Master app_send_audio] g_app.b_connected_flag %d, g_app.remote_connected_flag %d, g_app.b_stop_flag %d", g_app.b_connected_flag, g_app.remote_connected_flag, g_app.b_stop_flag);
    //  pthread_t tid;
    //  pthread_create(&tid, NULL, sendAudio_thread, (void *)stream_info);
    //  pthread_join(tid, NULL);
#if CFG_ENABLE_DUMP_SEND_AUDIO
    write_file(g_app.audio_send_file_writer, info.data_type, stream_info->frame_data, stream_info->frame_len);
#endif
    
    // write_data_to_file((void *)stream_info, stream_type, output_audio);
    int rval = agora_rtc_send_audio_data(g_app.conn_id, stream_info->frame_data, stream_info->frame_len, &info);
    if (rval < 0)
    {
      LOGE("Failed to send audio data, reason: %s", agora_rtc_err_2_str(rval));
      // file_parser_release_frame(g_app.audio_file_parser, &frame);
      return;
    }
  }

  // file_parser_release_frame(g_app.audio_file_parser, &frame);
  return 0;
}

#ifdef CONFIG_XOS_USE_APP_MOMO_TALK
int rtsa_send_audio(int channel, qua_steram_type_t stream_type, qua_camera_stream_info_t *stream_info)
{
  // printf("rtsa_send_audio\n");
  return app_send_audio(channel, stream_type, stream_info);
}
#endif //CONFIG_XOS_USE_APP_MOMO_TALK

FILE *output_h264 = NULL;
void write_data_to_file(void *args)
{
  qua_camera_stream_info_t *stream_info = (qua_camera_stream_info_t *)args;
  int w_len = 0;
  if (output_h264 != NULL)
  {
    w_len = fwrite(stream_info->frame_data, 1, stream_info->frame_len, output_h264);
    printf("[mx]write size=%d, written=%d\n", stream_info->frame_len, w_len);
  }
  else
  {
    printf("[mx]error open file.\n");
  }
}

static void *sendVideo_thread(void *param)
{
  frame_t frame;
  qua_camera_stream_info_t *stream_info = (qua_camera_stream_info_t *)param;
  video_frame_info_t info = {0};
  frame.u.video.is_key_frame = stream_info->frame_format ? 0 : 1;
  info.frame_type = frame.u.video.is_key_frame ? VIDEO_FRAME_KEY : VIDEO_FRAME_DELTA;
  info.frame_rate = DEFAULT_SEND_VIDEO_FRAME_RATE; // config->send_video_frame_rate;
  info.data_type = VIDEO_DATA_TYPE_H264;           // config->video_data_type;
  info.stream_type = VIDEO_STREAM_HIGH;

  if (stream_info->frame_len > g_app.config.videoBuffersize)
  {
    g_app.config.pVideobuffer = (unsigned char *)realloc(g_app.config.pVideobuffer, stream_info->frame_len);
    if (g_app.config.pVideobuffer == NULL)
    {
      LOGE("Failed to realloc pVideobuffer memory!!!!");
      return;
    }
    g_app.config.videoBuffersize = stream_info->frame_len;
  }
  memcpy(g_app.config.pVideobuffer, stream_info->frame_data, g_app.config.videoBuffersize);
  int rval = agora_rtc_send_video_data(g_app.conn_id, g_app.config.pVideobuffer, g_app.config.videoBuffersize, &info);
  if (rval < 0)
  {
    LOGE("Failed to send video data, reason: %s", agora_rtc_err_2_str(rval));
    // file_parser_release_frame(g_app.video_file_parser, &frame);
    return;
  }
}
static int s_video_frame= 0;
static int app_send_video(int channel, qua_steram_type_t stream_type, qua_camera_stream_info_t *stream_info)
{
  // LOGI("[AGORA Master] app_send_video %d-%d-%d-%d, len=%d\n", channel, stream_type, stream_info->stream_format, stream_info->frame_format, stream_info->frame_len);
  app_config_t *config = &g_app.config;
  // uint8_t stream_id = 0;

#if 0
  if (file_parser_obtain_frame(g_app.video_file_parser, &frame) < 0) {
    LOGE("The file parser failed to obtain video frame");
    return -1;
  }
#endif
    s_video_frame++;
    if(s_video_frame % 250 == 0)
  {
    printf(">>>>>>>app_send_video<<<<<<< %d-%d-%d-%d, len=%d\n", channel, stream_type, stream_info->stream_format, stream_info->frame_format, stream_info->frame_len);
    // LOGI("[AGORA Master app_send_video] g_app.b_connected_flag %d, g_app.remote_connected_flag %d, g_app.b_stop_flag %d", g_app.b_connected_flag, g_app.remote_connected_flag, g_app.b_stop_flag);

  }
  if (g_app.b_connected_flag && g_app.remote_connected_flag && !g_app.b_stop_flag)
  {
    // LOGI("[AGORA Master app_send_video] g_app.b_connected_flag %d, g_app.remote_connected_flag %d, g_app.b_stop_flag %d", g_app.b_connected_flag, g_app.remote_connected_flag, g_app.b_stop_flag);

    // API: send vido data
    // pthread_t tid;
    // pthread_create(&tid, NULL, sendVideo_thread, (void *)stream_info);
    // pthread_join(tid, NULL);
    // file_parser_release_frame(g_app.video_file_parser, &frame);
    frame_t frame;

    video_frame_info_t info = {0};
    frame.u.video.is_key_frame = stream_info->frame_format ? 0 : 1;
    info.frame_type = frame.u.video.is_key_frame ? VIDEO_FRAME_KEY : VIDEO_FRAME_DELTA;
    info.frame_rate = 0; // config->send_video_frame_rate;
    info.data_type = VIDEO_DATA_TYPE_H264;           // config->video_data_type;
    info.stream_type = VIDEO_STREAM_HIGH;

#if CFG_ENABLE_DUMP_SEND_VIDEO
    write_file(g_app.video_send_file_writer, info.data_type, stream_info->frame_data, stream_info->frame_len);
#endif

    int rval = agora_rtc_send_video_data(g_app.conn_id, stream_info->frame_data, stream_info->frame_len, &info);
    if (rval < 0)
    {
      LOGE("Failed to send video data, reason: %s", agora_rtc_err_2_str(rval));
      // file_parser_release_frame(g_app.video_file_parser, &frame);
      return;
    }
  }
  return 0;
}

#if CFG_ENABLE_AUDIO_SIMULATOR
static void simulator_audio_by_pacer(void * args){
  app_config_t *config = &g_app.config;
  parser_cfg_t parser_cfg;

  signal(SIGINT, app_signal_handler);

  memset(&parser_cfg, 0, sizeof(parser_cfg_t));
  if (!config->receive_data_only)
  {
    parser_cfg.u.audio_cfg.sampleRateHz = config->pcm_sample_rate;
    parser_cfg.u.audio_cfg.numberOfChannels = config->pcm_channel_num;
    parser_cfg.u.audio_cfg.framePeriodMs = config->pcm_duration;

    g_app.audio_file_parser =
        create_file_parser(audio_data_type_to_file_type(config->audio_data_type), config->send_audio_file_path, &parser_cfg);
    if (!g_app.audio_file_parser)
    {
      LOGE("create file parser for audio failed");
      return -1;
    }

    frame_t frame;
    int count = 0;
    int maxCount = 1000;
    while (!g_app.b_stop_flag && (count++ < maxCount)){
      if (file_parser_obtain_frame(g_app.audio_file_parser, &frame) < 0) {
        LOGE("The file parser failed to obtain audio frame");
        break;
      }
      if (g_app.b_connected_flag && g_app.remote_connected_flag &&!g_app.b_stop_flag){
        qua_camera_stream_info_t stream_info = {0};
        stream_info.frame_data = frame.ptr;
        stream_info.frame_len = frame.len;
        stream_info.stream_format = 0;
        stream_info.frame_format = 0;
        
        app_send_audio(0, QUA_STREAM_TYPE_AUDIO, &stream_info);
      }else{
        usleep(100*1000);
      }
    }

    destroy_file_parser(g_app.audio_file_parser);
    g_app.audio_file_parser = NULL;
  }  
}
#endif //CFG_ENABLE_AUDIO_SIMULATOR

static void sendVideoPackets_subscribe(void *args)
{
  prctl(PR_SET_NAME, "sendVideoPackets_subscribe");
  LOGI("sendVideoPackets_subscribe...............");
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#if PUSH_H264_STREAM
#if CFG_ENABLE_SUBSTREAM_VIDEO
  // qua_camera_set_record_size(0, 640, 480, 512*1024);
  if (g_app.config.video_exist)
  {
    qua_camera_set_record_size(0, resolutions[g_app.config.resolution].width, resolutions[g_app.config.resolution].height, resolutions[g_app.config.resolution].bitrate);
    LOGI("qua_camera_subscribe_stream app_send_video Resolutin: width:%dxheight:%d,bitrate:%d ", resolutions[g_app.config.resolution].width,
         resolutions[g_app.config.resolution].height, resolutions[g_app.config.resolution].bitrate);
    g_app.config.video_mask = ((1 << QUA_CRF_H264) | (1 << QUA_CRF_H265));
    g_app.config.video_handle = qua_camera_subscribe_stream(0, QUA_STREAM_TYPE_VIDEO, g_app.config.video_mask, app_send_video);
    LOGI("qua_camera_subscribe_stream app_send_video handle_video %d", g_app.config.video_handle);
  }
#endif
#if CFG_ENABLE_SUBSTREAM_AUDIO
#if CFG_ENABLE_AUDIO_SIMULATOR
  pthread_t tid;
  pthread_create(&tid, NULL, simulator_audio_by_pacer, NULL);
  // pthread_join(tid, NULL); //not join it
#else //CFG_ENABLE_AUDIO_SIMULATOR
  //g_app.config.audio_mask = 1 << QUA_CRF_AUDIO;
  //g_app.config.audio_handle = qua_camera_subscribe_stream(0, QUA_STREAM_TYPE_AUDIO, g_app.config.audio_mask, app_send_audio);
  g_app.config.audio_handle = qua_capture_audio_start(0,  app_send_audio);
  LOGI("qua_camera_subscribe_stream app_send_audio handle_audio %d", g_app.config.audio_handle);
  if(g_app.config.audio_volume != -1) 
    set_audioCapture_volume(g_app.config.audio_volume);
#endif //CFG_ENABLE_AUDIO_SIMULATOR
#endif //CFG_ENABLE_SUBSTREAM_AUDIO
  // qua_camera_action(0, QUA_CA_RECORD);
#endif
#endif
}

static void __on_join_channel_success(connection_id_t conn_id, uint32_t uid, int elapsed)
{
  g_app.b_connected_flag = true;
  connection_info_t conn_info = {0};
  agora_rtc_get_connection_info(g_app.conn_id, &conn_info);
  LOGI("[conn-%u] Join the channel %s successfully, uid %u elapsed %d ms", conn_id,
       conn_info.channel_name, uid, elapsed);
  if (g_app.callback.on_status_changed)
  {
    g_app.callback.on_status_changed(STATUS_CONNECTED, g_app.callback.cb_context);
  }
}

static void __on_reconnecting(connection_id_t conn_id)
{
  g_app.b_connected_flag = false;
  LOGW("[conn-%u] connection timeout, reconnecting", conn_id);
  unsubscribe_stream();
  if (g_app.callback.on_status_changed)
  {
    //g_app.callback.on_status_changed(false);
  }
}

static void __on_connection_lost(connection_id_t conn_id)
{
  g_app.b_connected_flag = false;
  LOGW("[conn-%u] Lost connection from the channel", conn_id);
  unsubscribe_stream();
  if (g_app.callback.on_status_changed)
  {
    g_app.callback.on_status_changed(STATUS_NETWORK_DISCONNECTED, g_app.callback.cb_context);
  }
}

static void __on_rejoin_channel_success(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{
  g_app.b_connected_flag = true;
  LOGI("[conn-%u] Rejoin the channel successfully, uid %u elapsed %d ms", conn_id, uid, elapsed_ms);
  if (g_app.callback.on_status_changed)
  {
    //g_app.callback.on_status_changed(true);
  }
}

static void __on_user_joined(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{

  g_app.remote_connected_flag = true;
  LOGI("[conn-%u] Remote user \"%u\" has joined the channel, elapsed %d ms", conn_id, uid,
       elapsed_ms);
  // pthread_t tid;
  // pthread_create(&tid, NULL, sendVideoPackets_subscribe, NULL);
  // pthread_join(tid, NULL);
}

static void __on_user_offline(connection_id_t conn_id, uint32_t uid, int reason)
{
  g_app.remote_connected_flag = false;
  LOGI("[conn-%u] Remote user \"%u\" has left the channel, reason %d", conn_id, uid, reason);
  unsubscribe_stream();
  if (g_app.callback.on_status_changed)
  {
    g_app.callback.on_status_changed(STATUS_PEER_DISCONNECTED, g_app.callback.cb_context);
  }
}

static void __on_user_mute_audio(connection_id_t conn_id, uint32_t uid, bool muted)
{
  LOGI("[conn-%u] audio: uid=%u muted=%d", conn_id, uid, muted);
}

static void __on_user_mute_video(connection_id_t conn_id, uint32_t uid, bool muted)
{
  LOGI("[conn-%u] video: uid=%u muted=%d", conn_id, uid, muted);
}

static void __on_error(connection_id_t conn_id, int code, const char *msg)
{
  if (code == ERR_SEND_VIDEO_OVER_BANDWIDTH_LIMIT || code == 300)
  {
    LOGE("Not enough uplink bandwdith. Error msg \"%s\"", msg);
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    qua_camera_set_record_size(0, resolutions[g_app.config.resolution].width, resolutions[g_app.config.resolution].height, DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE);
#endif
    //return;
  }

  if (code == ERR_INVALID_APP_ID)
  {
    LOGE("Invalid App ID. Please double check. Error msg \"%s\"", msg);
  }
  else if (code == ERR_INVALID_CHANNEL_NAME)
  {
    LOGE("Invalid channel name for conn_id %u. Please double check. Error msg \"%s\"", conn_id,
         msg);
  }
  else if (code == ERR_INVALID_TOKEN || code == ERR_TOKEN_EXPIRED)
  {
    LOGE("Invalid token. Please double check. Error msg \"%s\"", msg);
  }
  else if (code == ERR_DYNAMIC_TOKEN_BUT_USE_STATIC_KEY)
  {
    LOGE("Dynamic token is enabled but is not provided. Error msg \"%s\"", msg);
  }
  else
  {
    LOGW("Error %d is captured. Error msg \"%s\"", code, msg);
  }

  //g_app.b_stop_flag = true;
}

static void __on_license_failed(connection_id_t conn_id, int reason)
{
  LOGE("License verified failed, reason: %d", reason);
  //g_app.b_stop_flag = true;
}

static void __on_audio_data(connection_id_t conn_id, const uint32_t uid, uint16_t sent_ts,
                            const void *data, size_t len, const audio_frame_info_t *info_ptr)
{
  // static int count = 0;
  // if((count ++) % 100 == 0){
  //   LOGD("[conn-%u] on_audio_data, uid %u sent_ts %u data_type %d, len %zu. g_app.b_stop_flag=%d\n", conn_id, uid, sent_ts,
  //   info_ptr->data_type, len, g_app.b_stop_flag);
  // }

#if CFG_ENABLE_DUMP_AUDIO
  write_file(g_app.audio_file_writer, info_ptr->data_type, data, len);
#endif
  if (g_app.b_stop_flag == false && g_app.callback.on_recv_audio != NULL)
  {
    g_app.callback.on_recv_audio(info_ptr->data_type, data, len, g_app.callback.cb_context);
  }
}

static void __on_mixed_audio_data(connection_id_t conn_id, const void *data, size_t len,
                                  const audio_frame_info_t *info_ptr)
{
  LOGD("[conn-%u] on_mixed_audio_data, data_type %d, len %zu", conn_id, info_ptr->data_type, len);
  write_file(g_app.audio_file_writer, info_ptr->data_type, data, len);
}

static void __on_video_data(connection_id_t conn_id, const uint32_t uid, uint16_t sent_ts,
                            const void *data, size_t len, const video_frame_info_t *info_ptr)
{
  // LOGD("[conn-%u] on_video_data: uid %u sent_ts %u data_type %d frame_type %d stream_type %d len %zu",
  //  conn_id, uid, sent_ts, info_ptr->data_type, info_ptr->frame_type, info_ptr->stream_type,
  //  len);
#if CFG_ENABLE_DUMP_VIDEO
  write_file(g_app.video_file_writer, info_ptr->data_type, data, len);
#endif
  if (g_app.callback.on_recv_video != NULL)
  {
    g_app.callback.on_recv_video(info_ptr->data_type, data, len, info_ptr->frame_rate, g_app.callback.cb_context);
  }
}

static void __on_target_bitrate_changed(connection_id_t conn_id, uint32_t target_bps)
{
  LOGI("[conn-%u] Bandwidth change detected. Please adjust encoder bitrate to %u kbps", conn_id,
       target_bps / 1000);
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
  qua_camera_set_record_size(0, resolutions[g_app.config.resolution].width, resolutions[g_app.config.resolution].height, target_bps*3/4);
#endif
}

static void __on_key_frame_gen_req(connection_id_t conn_id, uint32_t uid,
                                   video_stream_type_e stream_type)
{
  LOGW("[conn-%u] Frame loss detected. Please notify the encoder to generate key frame immediately",
       conn_id);
}

static void app_init_event_handler(agora_rtc_event_handler_t *event_handler, app_config_t *config)
{
  event_handler->on_join_channel_success = __on_join_channel_success;
  event_handler->on_reconnecting = __on_reconnecting,
  event_handler->on_connection_lost = __on_connection_lost;
  event_handler->on_rejoin_channel_success = __on_rejoin_channel_success;
  event_handler->on_user_joined = __on_user_joined;
  event_handler->on_user_offline = __on_user_offline;
  event_handler->on_user_mute_audio = __on_user_mute_audio;
  event_handler->on_user_mute_video = __on_user_mute_video;
  event_handler->on_target_bitrate_changed = __on_target_bitrate_changed;
  event_handler->on_key_frame_gen_req = __on_key_frame_gen_req;
  event_handler->on_video_data = __on_video_data;
  event_handler->on_error = __on_error;
  event_handler->on_license_validation_failure = __on_license_failed;

  if (config->enable_audio_mixer)
  {
    event_handler->on_mixed_audio_data = __on_mixed_audio_data;
  }
  else
  {
    event_handler->on_audio_data = __on_audio_data;
  }
}

int rtsa_api_stop(void)
{
  LOGI("rtsa_api_stop-->in\n");
  if (g_app.b_stop_flag)
  {
    LOGI("rtsa_api_stop-->already stop.\n");
    return 0;
  }
  pthread_mutex_lock(&g_app.b_stop_mutex);
  g_app.b_stop_flag = true;// 表示任务已完成
  pthread_cond_signal(&g_app.b_stop_cond_var); // 唤醒等待线程
  pthread_mutex_unlock(&g_app.b_stop_mutex);
  
  while (1)
  {
    usleep(1000 * 100);
    if (g_app.b_stop_completed_flag)
    {
      break;
    }
    LOGI("rtsa_api_stop-->waiting.\n");
  }
#if CFG_ENABLE_DUMP_VIDEO
  if(g_app.video_file_writer)
  {
    destroy_file_writer(g_app.video_file_writer);
    g_app.video_file_writer = NULL;
  }
#endif

#if CFG_ENABLE_DUMP_AUDIO
  if (g_app.audio_file_writer)
  {
    destroy_file_writer(g_app.audio_file_writer);
    g_app.audio_file_writer = NULL;
  }
#endif // CFG_ENABLE_DUMP_AUDIO


#if CFG_ENABLE_DUMP_SEND_AUDIO
  if (g_app.audio_send_file_writer)
  {
    destroy_file_writer(g_app.audio_send_file_writer);
    g_app.audio_send_file_writer = NULL;
  }
#endif

#if CFG_ENABLE_DUMP_SEND_VIDEO
  if (g_app.video_send_file_writer)
  {
    destroy_file_writer(g_app.video_send_file_writer);
    g_app.video_send_file_writer = NULL;
  }
#endif

  LOGI("rtsa_api_stop-->out.\n");
  return 0;
}

// hello
int rtsa_api_go(int argc, char **argv, void *app_cb)
{
  LOGI("rtsa_api_go-->in\n");
  int rval = 0;
  char priv_params[512] = {0};

  reset_app_t_structure();
  app_config_t *config = &g_app.config;

  // #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
  //   LOGI("sendVideoPackets_subscribe...............");
  //   // qua_camera_set_record_size(0, 640, 480, 512*1024);
  //   qua_camera_set_record_size(0, DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_BITRATE);
  //   g_app.config.video_mask = ((1 << QUA_CRF_H264) | (1 << QUA_CRF_H265));
  //   g_app.config.video_handle = qua_camera_subscribe_stream(0, QUA_STREAM_TYPE_VIDEO, g_app.config.video_mask, app_send_video);
  //   qua_camera_action(0, QUA_CA_RECORD);
  // #endif

#if START_WRITE_FIEL
  output_h264 = fopen("/mnt/sdcard/casting_video.h264", "wb");
#endif
  LOGI("init v-SDK v%s", agora_rtc_get_version());
  // 1. app parse args
  rval = app_parse_args(argc, argv, config);
  if (rval < 0)
  {
    app_print_usage(argc, argv);
    return -1;
  }

  app_print_config(config);

  // 1.1 app callback
  app_callback_init((qua_videocall_t *)app_cb);

  // 2. app init
  rval = app_init();
  if (rval < 0)
  {
    LOGE("Failed to initialize application");
    return -1;
  }

  // 4. API: init agora rtc sdk
  int appid_len = strlen(config->p_appid);
  void *p_appid = (void *)(appid_len == 0 ? NULL : config->p_appid);

  agora_rtc_event_handler_t event_handler = {0};
  app_init_event_handler(&event_handler, config);

  rtc_service_option_t service_opt = {0};
  service_opt.area_code = config->area;
  service_opt.log_cfg.log_path = config->p_sdk_log_dir;
  service_opt.log_cfg.log_disable = config->log_level == -1;
  service_opt.log_cfg.log_level = config->log_level;
  snprintf(service_opt.license_value, sizeof(service_opt.license_value), "%s", config->p_license);

  rval = agora_rtc_init(p_appid, &event_handler, &service_opt);
  if (rval < 0)
  {
    LOGE("Failed to initialize Agora sdk, reason: %s", agora_rtc_err_2_str(rval));
    return -1;
  }

  rval = agora_rtc_set_bwe_param(CONNECTION_ID_ALL, DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE,
                                 DEFAULT_BANDWIDTH_ESTIMATE_MAX_BITRATE,
                                 DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE);
  if (rval != 0)
  {
    LOGE("Failed set bwe param, reason: %s", agora_rtc_err_2_str(rval));
    return -1;
  }

  if (config->local_ap && config->local_ap[0])
  {
    snprintf(priv_params, sizeof(priv_params), "{\"rtc.local_ap_list\": %s}", config->local_ap);
    rval = agora_rtc_set_params(priv_params);
    if (rval != 0)
    {
      LOGE("set local_ap_list %s failed, reason: %s", config->local_ap, agora_rtc_err_2_str(rval));
      return -1;
    }
  }

#if 0 // can set mtu when network mtu is small to avoid udp fragment
  snprintf(priv_params, sizeof(priv_params), "{\"rtc.net_mtu\": %d}", 1260);
  rval = agora_rtc_set_params(priv_params);
  if (rval != 0) {
    LOGE("set net_mtu failed, reason: %s", agora_rtc_err_2_str(rval));
    return -1;
  }
#endif

  // 5. API: Create connection
  rval = agora_rtc_create_connection(&g_app.conn_id);
  if (rval < 0)
  {
    LOGE("Failed to create connection, reason: %s", agora_rtc_err_2_str(rval));
    return -1;
  }

  // 6. API: join channel
  int token_len = strlen(config->p_token);
  void *p_token = (void *)(token_len == 0 ? NULL : config->p_token);

  rtc_channel_options_t channel_options = {0};
  memset(&channel_options, 0, sizeof(channel_options));
  channel_options.auto_subscribe_audio = true;
  channel_options.auto_subscribe_video = true;
  channel_options.enable_audio_mixer = config->enable_audio_mixer;

  //channel_options.audio_codec_opt.pcm_duration = config->pcm_duration;
  if (config->audio_data_type == AUDIO_DATA_TYPE_PCM)
  {
    /* If we want to send PCM data instead of encoded audio like AAC or Opus, here please enable
           audio codec, as well as configure the PCM sample rate and number of channels */
    channel_options.audio_codec_opt.audio_codec_type = config->audio_codec_type;
    channel_options.audio_codec_opt.pcm_sample_rate = config->pcm_sample_rate;
    channel_options.audio_codec_opt.pcm_channel_num = config->pcm_channel_num;
  }

#if 0 // enable media encryption
  // get key from server by cmd: openssl rand -hex 32
  const char *key_str = "dba643c8ba6b6dc738df43d9fd624293b4b12d87a60f518253bd10ba98c48453";
  // get salt from server by cmd: openssl rand -base64 32
  const char *salt_base64_str = "X5w9T+50kzxVOnkJKiY/lUk82/bES2kATOt3vBuGEDw=";
  // enable
  channel_options.crypto_opt.enable = true;
  // mode
  channel_options.crypto_opt.mode =AES_128_GCM2;
  // key
  sprintf(channel_options.crypto_opt.key, "%s", key_str);
  // salt
  uint8_t *salt_bytes = NULL;
  uint32_t salt_bytes_len = 0;
  salt_bytes = util_base64_decode(salt_base64_str, strlen(salt_base64_str), &salt_bytes_len);
  memcpy(channel_options.crypto_opt.salt, salt_bytes, salt_bytes_len);
  free(salt_bytes);
#endif

  rval = agora_rtc_join_channel(g_app.conn_id, config->p_channel, config->uid, p_token,
                                &channel_options);
  if (rval < 0)
  {
    LOGE("Failed to join channel \"%s\", reason: %s", config->p_channel, agora_rtc_err_2_str(rval));
    return -1;
  }

  // 7. wait until we join channel successfully
  while (!g_app.b_connected_flag && !g_app.b_stop_flag)
  {
    usleep(100 * 1000);
  }
  
  pthread_t tid_cb_audio_video;
  pthread_create(&tid_cb_audio_video, NULL, sendVideoPackets_subscribe, NULL);
  pthread_join(tid_cb_audio_video, NULL);

  // 8. send video and audio data in a loop
  int audio_send_interval_us = config->pcm_duration * 1000;
  int video_send_interval_us = 1000000 / config->send_video_frame_rate;
  if (config->audio_data_type == AUDIO_DATA_TYPE_AACLC_8K ||
      config->audio_data_type == AUDIO_DATA_TYPE_AACLC_16K ||
      config->audio_data_type == AUDIO_DATA_TYPE_AACLC ||
      config->audio_data_type == AUDIO_DATA_TYPE_HEAAC)
  {
    audio_send_interval_us = 1000000 * 1024 / config->pcm_sample_rate;
  }
  void *pacer = pacer_create(audio_send_interval_us, video_send_interval_us);
  g_app.b_stop_completed_flag = false;

  pthread_mutex_lock(&g_app.b_stop_mutex);
  while (!g_app.b_stop_flag)
  {
    // skip sending data if receive_data_only flag is on
    if (config->receive_data_only)
    {
      util_sleep_ms(3000);
      // LOGD("mem_used=%d\n", agora_rtc_get_memory_used());
      continue;
    }
#if 0
    if (g_app.b_connected_flag && is_time_to_send_audio(pacer)) {
      app_send_audio();
    }
    if (g_app.b_connected_flag && is_time_to_send_video(pacer)) {
      app_send_video();
    }
#endif
    // sleep and wait until time is up for next send
    //wait_before_next_send(pacer);
    LOGI("+++++++++++++++++++++++pthread_cond_wait<<<<<<<<<<<<");
    pthread_cond_wait(&g_app.b_stop_cond_var, &g_app.b_stop_mutex); // 等待条件变量
  }
  pthread_mutex_unlock(&g_app.b_stop_mutex);
  g_app.b_stop_completed_flag = true;
  LOGI("[rtsa_api_go]stop_flag=%d", g_app.b_stop_flag);
  
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
  if (g_app.remote_connected_flag)
  {
    g_app.remote_connected_flag = false;
    unsubscribe_stream();
  }
//
#endif

  // 9. API: leave channel
  agora_rtc_leave_channel(g_app.conn_id);

  // 10: API: Destroy connection
  agora_rtc_destroy_connection(g_app.conn_id);

  // 11. API: fini rtc sdk
  agora_rtc_fini();

  // 12. app clean up
  pacer_destroy(pacer);
#if START_WRITE_FIEL
  if (output_h264)
    fclose(output_h264);
#endif
#if 0
  if (g_app.video_file_parser != NULL) {
    destroy_file_parser(g_app.video_file_parser);
    g_app.video_file_parser = NULL;
  }
  if (g_app.audio_file_parser != NULL) {
    destroy_file_parser(g_app.audio_file_parser);
    g_app.audio_file_parser = NULL;
  }
#endif
  LOGI("rtsa_api_go-->out\n");
  return 0;
}

#endif //CONFIG_XOS_FWK_CADP_AGORA
