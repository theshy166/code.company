#ifndef XOS_RES_CONF_H
#define XOS_RES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/*Video player resouce directory path*/
#define XOS_RES_CONF_DCIM_SDC_DIR   "/sdcard/DCIM/"
#define XOS_RES_CONF_DCIM_NFS_DIR   "/nfs/DCIM/"


//qetv
#define XOS_RES_CONF_QETV_DIR_PATH  "H:/sdcard/qetv/"

//lottie demo
#define XOS_RES_LOTTIE_CAR              "/nfs/lottie_car.json"
#define XOS_RES_LOTTIE_CELLEBRATION     "/nfs/lottie_cellebration.json"
#define XOS_RES_LOTTIE_COFFIE           "/nfs/lottie_coffie.json"
#define XOS_RES_LOTTIE_RAINING          "/nfs/lottie_raining.json"
#define XOS_RES_LOTTIE_THUNDERSTORM     "/nfs/lottie_thunderstorm.json"

//videocall local video and audio
#define XOS_RES_VIDEOCALL_H264          "%s/videocall/send_video_352x288.h264"
#define XOS_RES_VIDEOCALL_16KPCM        "%s/videocall/send_audio_16k_1ch.pcm"
#define XOS_RES_RTC_LOG_PATH            "/nfs/videocall/io.agora.rtc_sdk"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*XOS_RES_CONF_H*/
