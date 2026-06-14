#ifndef __QUA_CAMERA_CONFIG_H__
#define __QUA_CAMERA_CONFIG_H__

#define QUA_CAMERA_SNS_MAX_NUM   4
#define QUA_CAMERA_DEV_MAX_NUM   4

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  
{
    char       sns_name[32]; 
    int        sns_width;
    int        sns_height;
    int        sns_fps;
    int        sns_wdr;
    int        sns_iic;

    int        ircut_enable;
    int        ircut_debug;
    int        d2n_gain;
    int        n2d_gain;
    int        n2d_value;
    int        ircut_gpio0;
    int        ircut_gpio1;
    int        irled_gpio;

    int        lane_num;
    int        vicap_offline;
}qua_camera_sns_config_t;

typedef struct  
{
    int camera_id;
    int dev_id;
    int dev_iic;
    int sns_num;
    qua_camera_sns_config_t sns_config[QUA_CAMERA_SNS_MAX_NUM];
}qua_camera_dev_config_t;

typedef struct  
{
    int dev_num;
    qua_camera_dev_config_t dev_config[QUA_CAMERA_DEV_MAX_NUM];
}qua_camera_config_t;

qua_camera_config_t* qua_camera_config_get();

#ifdef __cplusplus
}
#endif

#endif