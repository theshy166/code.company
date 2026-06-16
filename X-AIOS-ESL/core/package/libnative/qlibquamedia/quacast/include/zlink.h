#ifndef ZLINK_API_H
#define ZLINK_API_H
#include "quacast.h"

typedef struct {
    LIBZLINK_HANDLE libzlink_handle;
    enum LIBZLINK_SESSION_STATE session_state;
    enum PHONE_TYPE phone_type;
    // int width;
    // int height;
    qua_rect_t display_region;
    int fps;
    qua_cast_type_e cast_type;
    qua_cast_callback_t ui_callback;
    char url[128];
    bool is_visible;
} zlink_data_t;

// Add this enum definition at the top of the file
typedef enum {
    ZLINK_STATUS_SUCCESS = 0,
    ZLINK_STATUS_ALREADY_INITIALIZED,
    ZLINK_STATUS_INVALID_PARAMS,
    ZLINK_STATUS_INIT_FAILED,
    ZLINK_STATUS_MEMORY_ERROR,
    ZLINK_STATUS_ERROR
} zlink_status_e;

typedef void (*libzlink_session_state_cb_init_t)(session_state_f fun);
typedef void (*libzlink_video_data_cb_init_t)(video_data_f fun);
typedef void (*libzlink_main_audio_start_cb_init_t)(main_audio_start_f fun);
typedef void (*libzlink_main_data_cb_init_t)(main_audio_data_f fun);
typedef void (*libzlink_main_audio_stop_cb_init_t)(main_audio_stop_f fun);
typedef void (*libzlink_video_focus_request_cb_init_t)(video_focus_request_f fun);
typedef void (*libzlink_audio_focus_request_cb_init_t)(audio_focus_request_f fun);
typedef void (*libzlink_request_p2p_start_cb_init_t)(request_p2p_start_f fun);
typedef void (*libzlink_request_wifi_info_cb_init_t)(request_wifi_info_f fun);
typedef void (*libzlink_mic_start_cb_init_t)(mic_start_f fun);
typedef void (*libzlink_mic_stop_cb_init_t)(mic_stop_f fun);
typedef LIBZLINK_HANDLE (*libzlink_init_t)(void *user_data);
typedef void (*libzlink_deinit_t)(LIBZLINK_HANDLE handle);
typedef int (*libzlink_restart_session_t)();
typedef int (*libzlink_check_ready_t)(LIBZLINK_HANDLE handle);
typedef void (*libzlink_pincode_data_cb_init_t)(pincode_data_f fun);

typedef int (*libzlink_wifi_info_t)(char *wifi_name, char *wifi_passwd, char *ip_addr, int wifi_channel);
typedef int (*libzlink_station_info_send_t)(char *phone_ip);
typedef int (*libzlink_request_state_t)();
typedef int (*libzlink_init_session_2_t)(struct SESSION_DATA *session_data); //replace the libzlink_init_session()
typedef int (*libzlink_touch_event_t)(int x, int y, int is_touch_down);
typedef int (*libzlink_video_focus_t)(int is_hu_video_focus_on);

typedef struct {
    libzlink_init_t init;
    libzlink_deinit_t deinit;
    libzlink_restart_session_t restart_session;
    libzlink_check_ready_t check_ready;
    libzlink_session_state_cb_init_t session_state_init;
    libzlink_video_data_cb_init_t video_data_init;
    libzlink_main_audio_start_cb_init_t main_audio_start_init;
    libzlink_main_data_cb_init_t main_data_init;
    libzlink_main_audio_stop_cb_init_t main_audio_stop_init;
    libzlink_video_focus_request_cb_init_t video_focus_request_init;
    libzlink_audio_focus_request_cb_init_t audio_focus_request_init;
    libzlink_request_p2p_start_cb_init_t request_p2p_start_init;
    libzlink_request_wifi_info_cb_init_t request_wifi_info_init;
    libzlink_mic_start_cb_init_t mic_start_init;
    libzlink_mic_stop_cb_init_t mic_stop_init;
    libzlink_pincode_data_cb_init_t pincode_data_init;
    libzlink_wifi_info_t wifi_info;
    libzlink_station_info_send_t station_info_send;
    libzlink_request_state_t request_state;
    libzlink_init_session_2_t init_session_2;
    libzlink_touch_event_t touch_event;
    libzlink_video_focus_t video_focus;
} zlink_api_t;

void zlink_callback_register(void);
zlink_api_t* zlink_load_library(void);
void zlink_unload_library(void);
int zlink_wait_libzlink_ready(int timeout_seconds);
int zlink_start_network(void);
int zlink_stop_network(void);
void zlink_restart_session(void);
void zlink_set_visiblity(bool visble);
int zlink_data_init(qua_cast_init_params_t params, qua_cast_type_e cast_type);
int zlink_handle_touch_event(int x, int y, int is_touch_down);

#endif