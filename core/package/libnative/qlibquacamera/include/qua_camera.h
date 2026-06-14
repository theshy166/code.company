#ifndef __QUA_CAMERA_H__
#define __QUA_CAMERA_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

typedef enum
{
    QUA_CA_RECORD = 0,
    QUA_CA_SNAP,
    QUA_CA_AI,
    QUA_CA_OCR,
    QUA_CA_SWITCH,
    QUA_CA_PREVIEW_START,
    QUA_CA_PREVIEW_STOP,
    QUA_CA_NUM,
} qua_camera_action_t;

typedef enum
{
    QUA_CAMERA_ERR_BASE = -100,
    QUA_CAMERA_ERR_NO_MEMORY = QUA_CAMERA_ERR_BASE - 1,
    QUA_CAMERA_ERR_EPERM = QUA_CAMERA_ERR_BASE - 2,
    QUA_CAMERA_ERR_NOT_SUPPORT = QUA_CAMERA_ERR_BASE - 3,
} qua_camera_error_type_t;

typedef enum
{
    QUA_SWITCH_CHN_SWITCH = 0,  //前后依次切换
    QUA_SWITCH_CHN_FRONT = 1,   //强制切换到前摄
    QUA_SWITCH_CHN_REAR = 2,    //强制切换到后摄
    QUA_SWITCH_CHN_NUM,
} qua_camera_switch_chn_t;

typedef struct
{
    void *data;
    int length;
} qua_camera_action_payload_t;

#define QUA_CAMERA_AI_OBJECT_MAX_NUM   16
typedef struct
{
    int x;         //base LCD width height 1024x600
    int y;
    int width;
    int height;
    int confidence;   //0-100
    char class_name[32];
}qua_camera_AI_object_t;

// 姿势分析结果状态
// 关键点结构体
typedef struct {
    float x;
    float y;
    float score;
} qua_camera_Keypoint;

// 姿势分析结果结构体
typedef struct {
    char status[32];  //"success" or "error","warning"
    char posture_type[32];          // "Sitting" or "Standing"
    int score;                      // 0-100
    int error_type;            // 错误类型
    char image_data[1024];          // Base64编码的图像数据
    struct {
        float back_angle;           // 背部角度
        float head_angle;           // 头部角度
        float hip_y;                // 髋部Y坐标
        float shoulder_y;           // 肩膀Y坐标
        float sensitivity;          // 灵敏度
    } details;
    struct {
        char feedback[3][128];      // 最多3条反馈信息
        int feedback_count;         // 实际反馈数量
    } feedback_info;
    int keypoints_count;            // 关键点数量
    qua_camera_Keypoint keypoints[17];            

} qua_camera_AI_posture_result_t;

// AI结果类型枚举
typedef enum {
    QUA_CAMERA_AI_RESULT_OBJECT = 0,
    QUA_CAMERA_AI_RESULT_POSTURE = 1,
    QUA_CAMERA_AI_RESULT_CLASSIFICATION = 2
} qua_camera_AI_result_type_t;

typedef struct
{
    qua_camera_AI_result_type_t type;
    int                    num;
    union{
        qua_camera_AI_object_t object[QUA_CAMERA_AI_OBJECT_MAX_NUM];
        qua_camera_AI_posture_result_t posture_result;     // 姿势分析结果
    };
}qua_camera_AI_result_t;

typedef struct qua_camera_ocr_line_item_t {
    char *data;
    int size;
    float score;
} qua_camera_ocr_line_item_t;

typedef struct qua_camera_ocr_items_t {
    int line_cnt;
    qua_camera_ocr_line_item_t *line_items;
} qua_camera_ocr_items_t;

typedef enum
{
    QUA_CTFS_OK = 0,
    QUA_CTFS_ERROR,
    QUA_CTFS_FULL,
    QUA_CTFS_NO,
} qua_camera_tfcad_status_t;

typedef struct
{
    int        encoder_status;//0-sucess other-fail
    int        tf_status; //qua_camera_tfcad_status_t
}qua_camera_record_result_t;

typedef struct
{
    int status;            //0-sucess other-fail
    
    union
    {
        qua_camera_record_result_t record_result;
        qua_camera_AI_result_t AI_result;
        qua_camera_ocr_items_t ocr_result;
    };
} qua_ca_result_t;

//photo continue mode
typedef enum
{
    QUA_CPM_1 = 0,
    QUA_CPM_3,
    QUA_CPM_5,
} qua_camera_photo_mode_t;

typedef enum
{
    QUA_CPOP_ADD = 0,
    QUA_CPOP_DEL,
    QUA_CPOP_UPDATE,
    QUA_CPOP_QUERY,
    QUA_CPOP_BUTT
} qua_camera_photo_op_t;

typedef enum
{
    QUA_CPQ_HIGH = 0,
    QUA_CPQ_STANDARD,
    QUA_CPQ_LOW,
} qua_camera_photo_quality_t;

typedef enum
{
    QUA_CAMERA_ROTATE_NONE,
    QUA_CAMERA_ROTATE_90,
    QUA_CAMERA_ROTATE_180,
    QUA_CAMERA_ROTATE_270,
    QUA_CAMERA_ROTATE_BUTT
} qua_camera_rotate_t;

typedef enum
{
    QUA_CIL_HIGH = 0,
    QUA_CIL_STANDARD,
    QUA_CIL_LOW,
} qua_camera_image_level_t;

typedef enum
{
    QUA_CIC_BRIGHT = 0,
    QUA_CIC_SHARPNESS,
    QUA_CIC_SATURATION,
    QUA_CIC_CONTRAST,
    QUA_CIC_NR,
} qua_camera_image_class_t;

typedef enum
{
    QUA_CAE_BYPASS,
    QUA_CAE_MAGNIFY,
    QUA_CAE_PINCH,
    QUA_CAE_BLACK_WHITE,
    QUA_CAE_INVERSE,
    QUA_CAE_CHILDHOOD,
    QUA_CAE_VINTAGE,
    QUA_CAE_HANDSOME,
    QUA_CAE_CARTOON,
    QUA_CAE_DREAMY,
    QUA_CAE_STYLIZATION,
    QUA_CAE_PENCIL,
    QUA_CAE_SNOW
} qua_camera_algos_effect_t;

//4K30@FPS/2K@60FPS/1080P@60FPS/1080P@30FPS/720P@60FPS/720P@30FPS
typedef enum
{
    QUA_CSF_4K_30 = 0,
    QUA_CSF_2K_60,
    QUA_CSF_1080P_120,
    QUA_CSF_1080P_60,
    QUA_CSF_1080P_30,
    QUA_CSF_720P_120,
    QUA_CSF_720P_60,
    QUA_CSF_720P_30,
} qua_camera_sensor_format_t;

typedef enum
{
    QUA_STREAM_TYPE_VIDEO = 0,
    QUA_STREAM_TYPE_AUDIO,
} qua_steram_type_t;
    
typedef enum
{
    QUA_CRF_H264 = 0,
    QUA_CRF_H265,
    QUA_CRF_JPEG,
    QUA_CRF_AUDIO,
    QUA_CRF_MJPEG,
} qua_camera_record_format_t;

typedef enum
{
    QUA_CFF_IFRAME = 0,
    QUA_CFF_PFRAME,
    QUA_CFF_BFRAME,
} qua_camera_frame_format_t;

typedef enum
{
    QUA_CAF_PCM = 0,
    QUA_CAF_ALAW,
    QUA_CAF_ULAW,
    QUA_CAF_AAC,
} qua_camera_audio_format_t;


//photo frame or sticker
#define PHOTO_GRAPH_ID_MAX 4
typedef struct
{
    qua_camera_photo_op_t op;
    int id;   //op=QUA_CPOP_ADD should return this value, otherwise should set this value
    int x;       //LCD 1024x600 base,photo frame set to 1024x600
    int y;
    int width;
    int height;
    char graph_name[64];
    int is_sticker;
} qua_camera_photo_graph_t;

typedef struct
{
    int x;       //LCD 1024x600 base
    int y;
    int width;
    int height;
} qua_camera_rect_t;

typedef struct
{
    int                        stream_format;  //qua_camera_record_format_t
    int                        frame_format;   //qua_camera_frame_format_t qua_camera_audio_format_t
    unsigned long long         frame_pts;
    unsigned char              *frame_data;
    int                        frame_len;
    int                        frame_end_flag;
    int                        stream_id;
} qua_camera_stream_info_t;

typedef enum {
    QUA_CAMERA_MODEL_GESTURE_RECOG,
    QUA_CAMERA_MODEL_ALL_THINGS_RECOG,
} qua_camera_model_type_t;

typedef enum {
    QUA_CAMERA_PREVIEW_NONE,
    QUA_CAMERA_PREVIEW_PHOTO,
    QUA_CAMERA_PREVIEW_VIDEO,
    QUA_CAMERA_PREVIEW_BUTT
} qua_camera_preview_type_t;

typedef struct
{
    int enable;
    int x;
    int y;
    int size;
} qua_camera_osd_t;

typedef enum
{
    QUA_CAMERA_REC_NONE,
    QUA_CAMERA_REC_PRE_MODE,
    QUA_CAMERA_REC_REGULAR_MODE,
    QUA_CAMERA_REC_TIME_LAPSE_MODE,
    QUA_CAMERA_REC_SLOW_MOTION_MODE,
    QUA_CAMERA_REC_BUTT
} qua_camera_rec_type_t;

typedef struct
{
    int interval_sec;
} qua_camera_pre_rec_cfg_t;

typedef struct
{
    int loop_en;
    int max_duration_limit_us;
} qua_camera_regular_rec_cfg_t;

typedef struct
{
    int timelapse_ratio;
} qua_camera_timelapse_rec_cfg_t;

typedef struct
{
    qua_camera_rec_type_t rec_type;
    union {
        qua_camera_pre_rec_cfg_t pre_cfg;
        qua_camera_regular_rec_cfg_t regular_cfg;
        qua_camera_timelapse_rec_cfg_t timelapse_cfg;
    };
} qua_camera_record_cfg_t;

typedef int (*qua_camera_action_cb)(int channel, qua_camera_action_t action, qua_ca_result_t *result);
typedef int (*qua_camera_stream_cb)(int channel, qua_steram_type_t stream_type, qua_camera_stream_info_t *stream_info);

int qua_camera_init(const char *config_file, qua_camera_action_cb cb);
int qua_camera_exit();

int qua_camera_action(int channel, qua_camera_action_t action, qua_camera_action_payload_t *payload);
int qua_camera_preview(int channel, int b_start, qua_camera_action_payload_t *payload);

int qua_camera_set_photo_size(int channel, int width, int height);
int qua_camera_set_photo_mode(int channel, qua_camera_photo_mode_t mode);
int qua_camera_set_photo_count_down(int channel, int seconds);
int qua_camera_set_photo_graph(int channel, qua_camera_photo_graph_t *photo_graph);
int qua_camera_set_photo_quality(int channel, qua_camera_photo_quality_t quality);
int qua_camera_set_photo_rotate(int channel, qua_camera_rotate_t rotate);
int qua_camera_set_algos_effect(int channel, int algos);
int qua_camera_release_ocr_items(qua_camera_ocr_items_t *items);

int qua_camera_open_flash_lamp(int channel);
int qua_camera_close_flash_lamp(int channel);
int qua_camera_zoom_increase(int channel);
int qua_camera_zoom_decrease(int channel);
int qua_camera_set_osd(int channel, qua_camera_osd_t *osd);
int qua_camera_focus_rect(int channel, qua_camera_rect_t *rect);
int qua_camera_set_image(int channel, qua_camera_image_class_t image_class, qua_camera_image_level_t image_level);
int qua_camera_set_sensor_format(int channel, qua_camera_sensor_format_t sensor_format);
int qua_camera_set_record_format(int channel, qua_camera_record_format_t record_format);
int qua_camera_set_record_size(int channel, int width, int height, int bps);
int qua_camera_set_record_audio_enable(int channel, int enable);

int qua_camera_subscribe_stream(int channel, qua_steram_type_t stream_type, int stream_format_mask, qua_camera_stream_cb cb);
int qua_camera_unsubscribe_stream(int handle, int stream_format_mask);
int qua_camera_facedetect(int channel, int b_start);

void qua_camera_ai_detect_enable(qua_camera_model_type_t model_type, int enable);
//qua_camera_ai_set_user_cb: only implmented in 10xv now,
// when config_file is NULL, the config_file will be set to default value,
// return value is the last callback, store it and reset it back when necessary;
qua_camera_action_cb qua_camera_ai_set_user_cb(const char *config_file,qua_camera_action_cb cb);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

