
#include "xn_time.h"
#include <time.h>
#include <stdio.h>

int get_current_time(char *buf, int buf_len)
{
    #if 0
    struct tm *time_info;
    time_t time;
    //
    struct tm *p_tm;
 
    // time = rt_tick_get() / RT_TICK_PER_SECOND; // 将tick数转换为秒数
    // p_tm = localtime(&time); // 将秒数转换为本地时间结构体
    #elif 0
    int hour = 0;
    int minute = 0;
    time_t seconds = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (seconds >= 3600) {
        hour = seconds/3600;
        seconds = seconds % 3600;
    }
    if (seconds >= 60) {
        minute = seconds/60;
        seconds = seconds % 60;
    }

    #else

    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    // printf("当前时间为：%s", asctime(tm_info));
    int n = snprintf(buf, buf_len, "%02d:%02d", tm_info->tm_hour, tm_info->tm_min);
    return n > 0;

    #endif    
}
