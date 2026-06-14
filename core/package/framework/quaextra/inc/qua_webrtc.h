#ifndef __QUA_WEBRTC_H__
#define __QUA_WEBRTC_H__
#include <stdint.h>

typedef struct {
    int mediaType;
    int uid;
    int frametype;
    int nb;
    int64_t pts;
    int64_t dts;
    uint8_t* payload;
}QuaRtcFrame;
typedef void (*qua_data_recv_cb)(QuaRtcFrame *frame);

void qua_webrtc_sender_create(int listen_port);
void qua_webrtc_sender_destory(void);

int qua_webrtc_publishMsg(QuaRtcFrame *msgFrame);
int qua_webrtc_publishAudioData(QuaRtcFrame *audioFrame);
int qua_webrtc_publishVideoData(QuaRtcFrame *videoFrame);

int qua_webrtc_receiver_create(char *remoteIp, qua_data_recv_cb video_cb, qua_data_recv_cb audio_cb, qua_data_recv_cb msg_cb);

#endif

