#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_


/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "xos_app_conf.h"


#ifdef CONFIG_XOS_FWK_CADP_AGORA

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "agora_rtc_api.h"
#include "../third-party/file_parser/src/file_parser.h"
#include "../utility/file_writer.h"
#include "../utility/utility.h"
#include "../utility/pacer.h"
#include "../utility/log.h"

#define DEFAULT_CHANNEL_NAME "qm_demo_sz"
#define DEFAULT_CERTIFACTE_FILENAME "certificate.bin"
#define DEFAULT_SEND_AUDIO_FILENAME_PCM_16K "send_audio_16k_1ch.pcm"
#define DEFAULT_SEND_AUDIO_FILENAME_PCM_08K "send_audio_8k_1ch.pcm"
#define DEFAULT_SEND_VIDEO_FILENAME "send_video.h264"
#define DEFAULT_SEND_AUDIO_FILENAME "send_audio_16k_1ch.pcm"
#define DEFAULT_SEND_AUDIO_BASENAME "/mnt/sdcard/send_audio"
#define DEFAULT_SEND_VIDEO_BASENAME "/mnt/sdcard/send_video"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#define DEFAULT_RECV_AUDIO_BASENAME "/mnt/sdcard/recv_audio"
#else //not BUILD_SIMULATOR
#define DEFAULT_RECV_AUDIO_BASENAME "./out/simulator/xos/res/recv_audio"
#endif //not BUILD_SIMULATOR
#define DEFAULT_RECV_VIDEO_BASENAME "/mnt/sdcard/recv_video"
#define DEFAULT_SEND_VIDEO_FRAME_RATE (15)
#define DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE  (400000)       //(10000)
#define DEFAULT_BANDWIDTH_ESTIMATE_MAX_BITRATE  (4200000)              //(1000000)
#define DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE  (500000)        //(1000000)
#define DEFAULT_SEND_AUDIO_FRAME_PERIOD_MS (20)
#define DEFAULT_PCM_SAMPLE_RATE (16000)
#define DEFAULT_PCM_CHANNEL_NUM (1)

typedef struct {
  // common config
  const char *p_sdk_log_dir;
  int32_t  log_level;
  const char *p_appid;
  const char *p_token;
  const char *p_channel;
  const char *p_license;
  uint32_t uid;
  uint32_t area;

  // video related config
  video_data_type_e video_data_type;
  int send_video_frame_rate;
  const char *send_video_file_path;

  // audio related config
  audio_data_type_e audio_data_type;
  audio_codec_type_e audio_codec_type;
  const char *send_audio_file_path;
  uint32_t pcm_sample_rate;
  uint32_t pcm_channel_num;
  uint32_t pcm_duration;

  // advanced config
  bool enable_audio_mixer;
  bool receive_data_only;
  const char *local_ap;
  bool domain_limit;
  int conn_cnt;
  int video_handle;
  int video_mask;
  int audio_handle;
  int audio_mask;
  unsigned char* pVideobuffer;
  int videoBuffersize;
  unsigned char* pAudiobuffer;
  int audioBuffersize;
  int resolution;
  int video_exist;
  int audio_volume;
} app_config_t;

// 定义分辨率和码流的结构体
typedef struct {
    int width;      // 分辨率宽度
    int height;     // 分辨率高度
    int bitrate;    // 码流（kbps）
} ResolutionStream;


void app_print_usage(int argc, char **argv);
void app_print_config(app_config_t *config);
int app_parse_args(int argc, char **argv, app_config_t *config);

media_file_type_e video_data_type_to_file_type(video_data_type_e type);
media_file_type_e audio_data_type_to_file_type(audio_data_type_e type);

#endif //CONFIG_XOS_FWK_CADP_AGORA

#endif //head_define
