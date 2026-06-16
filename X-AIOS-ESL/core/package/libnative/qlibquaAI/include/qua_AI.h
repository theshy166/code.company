#ifndef _QUA_AI_H
#define _QUA_AI_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

// 定义最大检测对象数量
#define OUT_OBJECT_MAX_NUM 16
#define MAX_SCALE_VALUES 4  // 根据实际需要调整最大数量检测头
// 对象检测结果结构体
typedef struct {
    int class_id;
    float confidence;
    int x;
    int y;
    int width;
    int height;
} qua_AI_object_t;
//分类模型结构体
typedef struct {
    int class_id;
    float confidence;
} qua_AI_class_t;
// 姿势分析结果状态


// 关键点结构体
typedef struct {
    float x;
    float y;
    float score;
} Keypoint;

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
    Keypoint keypoints[17];            

} qua_AI_posture_result_t;

// AI结果类型枚举
typedef enum {
    QUA_AI_RESULT_OBJECT = 0,
    QUA_AI_RESULT_POSTURE = 1,
    QUA_AI_RESULT_CLASSIFICATION = 2
} qua_AI_result_type_t;


// AI结果结构体
typedef struct {
    qua_AI_result_type_t type;      // 结果类型
    int has_detect;                 // 是否有检测结果
    qua_video_frame_info_t input;   // 输入帧信息
    union {
        qua_AI_object_t object[OUT_OBJECT_MAX_NUM]; // 对象检测结果
        qua_AI_class_t class_result;  // 分类结果
        qua_AI_posture_result_t posture_result;     // 姿势分析结果

    };
    int num;                        // 结果数量
} qua_AI_result_t;

typedef enum
{
    QUA_AI_MODEL_DETECT = 0, 
    QUA_AI_MODEL_CLASS,
    QUA_AI_MODEL_PLATE,
    QUA_AI_MODEL_GESTURE_RECOG,
    QUA_AI_MODEL_POSTURE, //ligui
    QUA_AI_MODEL_TEST = 255,
} qua_AI_model_type_t;


typedef struct  
{
    char       model_path[256]; 
    int        model_type; //0-detect 1-class 3-gesture
    int        model_width;
    int        model_height;
    int        model_class_num;
    int        model_conf_threshold;
    int        model_iou_threshold;
    int        sensitivity;

    int        confidence;//*100
    int        in_buf_num;
    int        debug_level;//1-ERROR 2-INFO 3-DEBUG
    int        npu_clk;  //100-600
    //
    int        out_tensor_width;
    int        out_tensor_height;
    int        out_tensor_channel;
    int        out_top_class_num;
    //float        out_tensor_scale;
    int out_actual_channels[MAX_SCALE_VALUES];
    float out_tensor_scale[MAX_SCALE_VALUES];  // 量化scale参数数组
    int out_tensor_scale_count;  // 实际有效的scale数量
    float        out_tensor_zero_point[MAX_SCALE_VALUES];  
    int out_keypoints; // 0-不返回关键点，1-返回关键点
    int keypoints_count; // 关键点数量
    int out_num_outputs; // 输出张量数量
    
    bool       load_model_on_init; //default 0, load model on first process frame;
}qua_AI_Config_t;

typedef int (*qua_AI_result_cb)(int channel, qua_AI_result_t* result);

typedef struct qua_AI_module_t {
    int (*init)(int channel, char* config_file, qua_AI_result_cb cb);
    int (*exit)(int channel);
    int (*push_frame)(int channel, qua_video_frame_info_t *input);
    int (*get_model_config)(int channel, qua_AI_Config_t *config);
} qua_AI_module_t;

QUA_S32 qua_AI_load_module(qua_AI_module_t *module);

#endif

