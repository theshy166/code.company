

#ifndef __ESL_UI_STRUCT_H__
#define __ESL_UI_STRUCT_H__

#include <stdio.h>

typedef struct {
    // 状态灯
    lv_obj_t *led;
    bool led_visible;
    // 进度条
    lv_obj_t *bar;
    bool bar_visible;
    uint32_t bar_value;
    // 下载日志
    lv_obj_t *down_log;
    bool down_log_visible;
    char down_log_file_name[128];
} esl_ui_struct_t;

#endif
