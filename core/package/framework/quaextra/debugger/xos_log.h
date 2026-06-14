
#ifndef _XOS_LOG_H_
#define _XOS_LOG_H_

#include <stdio.h>
#include "lvgl/lvgl.h"

#define XOS_LOGU(fmt, ...)    LV_LOG_USER(fmt, ##__VA_ARGS__)
#define XOS_LOGI(fmt, ...)    LV_LOG_INFO(fmt, ##__VA_ARGS__)
#define XOS_LOGE(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)

#define LOSLOG //printf

#endif // !_XOS_LOG_H_
