#ifndef __QUA_CAST_H__
#define __QUA_CAST_H__

#include <stdint.h>
#include <stdbool.h>
#include "libzlink.h"
#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"
#include "utils/qua_handler.h"
#define QUA_SUCCESS             0
#define QUA_ERR_INVALID_PARAM  -1
#define URL_MAX_LEN 128

typedef enum {
    QUA_CAST_TYPE_NONE,
    QUA_CAST_TYPE_CARPLAY,
    QUA_CAST_TYPE_HICAR,
    QUA_CAST_TYPE_CARLIFE,
    QUA_CAST_TYPE_MAPCAST,
    QUA_CAST_TYPE_MAX
} qua_cast_type_e;

typedef enum {
    QUA_FRAME_TYPE_VIDEO,
    QUA_FRAME_TYPE_AUDIO,
    QUA_FRAME_TYPE_MESSAGE
} qua_frame_type_e;

enum {
    QUA_MSG_CAST_INIT = 1,
    QUA_MSG_CAST_START,
    QUA_MSG_CAST_STOP,
    QUA_MSG_CAST_SHOW,
    QUA_MSG_CAST_HIDE,
    QUA_MSG_CAST_DEINIT,
    QUA_MSG_CAST_CLOSE_PREV, // Close previous cast session
};

typedef enum {
    WifiCastStage_none = 0,
    WifiCastStage_wifi_starting = 1,
    WifiCastStage_wifi_started = 2,
    WifiCastStage_wifi_joining = 3,
    WifiCastStage_wifi_joined = 4,
    WifiCastStage_tcp_waiting = 5,
    WifiCastStage_client_connected = 6,//tcp连接，但没有数据
    WifiCastStage_client_data_received = 7,//tcp连接，有数据
    WifiCastStage_disconnecting = 8,
} WifiCastStage;

typedef enum LIBZLINK_SESSION_STATE quacast_session_state;

typedef struct qua_cast_status {
    bool is_cast_started;
    quacast_session_state session_state;
    qua_rect_t stream_region;
    qua_rect_t display_region;
    uint32_t framerate;
    qua_cast_type_e cast_type;
} qua_cast_status_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t framerate;
} qua_cast_info_t;

typedef struct qua_cast_frame {
    qua_frame_type_e type;    // Frame type identifier
    uint8_t *data;            // Frame data pointer
    uint32_t size;            // Data size in bytes
    uint64_t timestamp;       // Frame timestamp
} qua_cast_frame_t;

typedef void (*qua_cast_callback_t)(quacast_session_state session_state, char *ui_msg);

// Add cast type selection function
typedef struct {
    char url[URL_MAX_LEN]; //no need for carplay/hicar
    qua_rect_t stream_region;
    qua_rect_t display_region;
    uint32_t framerate;
    QUA_BOOL is_server;
    int connect_timeout;
    char wifi_ssid[32];
    char wifi_password[32];
    QUA_BOOL is_ap;  // ap or station mode
    qua_cast_callback_t ui_callback;
} qua_cast_init_params_t;

// Interface functions
int qua_cast_init(qua_cast_init_params_t* params, qua_cast_type_e type);
int qua_cast_deinit(void);
int qua_cast_start(void);
int qua_cast_stop(void);
int qua_cast_visibility(bool visible);
int qua_cast_get_status(qua_cast_status_t *status);
int qua_cast_publish_frame(qua_cast_frame_t *frame);
quacast_session_state qua_cast_get_session_state();
void qua_cast_set_session_state(quacast_session_state session_state);

#endif /* __QUA_CAST_H__ */