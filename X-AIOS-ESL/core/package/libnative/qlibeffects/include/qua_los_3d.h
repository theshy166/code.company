/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef _QUA_XOS_3D_H_
#define _QUA_XOS_3D_H_

typedef struct {
    //showing area
    //
    int x1;
    int y1;
    //
    int init_repeat_cnt;
    int error_code;
    int quit_flag;
    void* context;
} qua_los_3d_data;

//callback
/**
 * register by qua_los_3d_add_event
*/
typedef void(*qua_los_event_cb)(int status);


/**
 * @brief init
 * @param path path
 * @param area for show obj position, -1 will align to center.
 * @param init_repeat_count for first time, the count to repeat. default 1.
*/
qua_los_3d_data * qua_los_3d_init(const char *path, void * area, void *background, int init_repeat_count);

/**
 * @brief dispose
*/
int qua_los_3d_dispose(qua_los_3d_data * working_data);

/**
 * @brief Get total frame count
 * @todo hidden
*/
int qua_los_3d_get_duration_ms(qua_los_3d_data * working_data);

/**
 * @brief Go to a frame
 * @todo hidden
 */
int qua_los_3d_goto_timems(qua_los_3d_data * working_data, int from, int to, int usec);

/**
 * @brief Get current ms
*/
int qua_los_3d_get_current(void);


#endif // !_QUA_XOS_3D_H_
