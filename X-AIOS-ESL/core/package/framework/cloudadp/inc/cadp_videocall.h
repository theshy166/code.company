#ifndef _CADP_VIDEO_CALL_h_
#define _CADP_VIDEO_CALL_h_
#include <stdint.h> //uint16_t
#include <stddef.h> //size_t

typedef enum {
    STATUS_DISCONNECTED,
    STATUS_CONNECTED,
    STATUS_FAILED,
    STATUS_PEER_DISCONNECTED,  // 对方断开
    STATUS_NETWORK_DISCONNECTED // 网络断开
} connection_status_t;

typedef struct {
  void (*on_status_changed)(int connection_status, void *cb_context); 
  void (*on_recv_audio)(unsigned char  data_type, const void *data, size_t size, void *cb_context);
  void (*on_recv_video)(unsigned char  data_type, const void *data, size_t size, uint16_t frame_rate, void *cb_context);
  void * cb_context; // 用于传递上下文信息
}app_callback_t;

typedef struct {
    char appid[128];
    char channel_name[128];
    int  ischeck;
    char resolution[16];
    char video_exist[2];
    char audio_volume[4];
}qua_videocall_config_t;

typedef struct {
    int     (*call)(int argc, char **argv, void * callback);
    int     (*end)(void);
    int     (*config)(int* argc, char*** argv, qua_videocall_config_t* cf);
    app_callback_t callback;
}qua_videocall_t;


int qm_video_call_init(app_callback_t callback, qua_videocall_t* videocall);

#endif //_CADP_VIDEO_CALL_h_
