#ifndef XOS_RES_CONF_H
#define XOS_RES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/*Video player resouce directory path*/
#define XOS_RES_CONF_DCIM_SDC_DIR   "H:/sdcard/DCIM"
#define XOS_RES_CONF_DCIM_NFS_DIR   "H:/nfs/DCIM"


//qetv
#define XOS_RES_CONF_QETV_DIR_PATH  "H:apps/qetv/"

//lottie
#define XOS_RES_LOTTIE_CAR              "./out/simulator/res/rotties/lottie_car.json"
#define XOS_RES_LOTTIE_CELLEBRATION     "./out/simulator/res/rotties/lottie_cellebration.json"
#define XOS_RES_LOTTIE_COFFIE           "./out/simulator/res/rotties/lottie_coffie.json"
#define XOS_RES_LOTTIE_RAINING          "./out/simulator/res/rotties/lottie_raining.json"
#define XOS_RES_LOTTIE_THUNDERSTORM     "./out/simulator/res/rotties/lottie_thunderstorm.json"


//videocall local video and audio
// #define XOS_RES_VIDEOCALL_H264          "./out/simulator/res/videocall/send_video_384x384.h264"
// #define XOS_RES_VIDEOCALL_H264          "./out/simulator/res/videocall/send_video_640x320.h264"
#define XOS_RES_VIDEOCALL_H264          "./out/simulator/res/videocall/send_video_256x256.h264"
#define XOS_RES_VIDEOCALL_16KPCM        "./out/simulator/res/videocall/send_audio_16k_1ch.pcm"
#define XOS_RES_RTC_LOG_PATH            "./out/io.agora.rtc_sdk"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*XOS_RES_CONF_H*/
