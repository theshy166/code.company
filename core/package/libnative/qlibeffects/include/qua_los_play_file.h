/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef _qua_los_play_file_h_
#define _qua_los_play_file_h_


typedef struct {
    int x1;//play position
    int y1;
    int loop;
    char *path;
    /*out param*/
    int width;
    int height;
    int duration;
}qua_los_media_data;

//if show frame with video file
int qua_los_play_file_init(qua_los_media_data * working_data);
int qua_los_play_file_dispose(qua_los_media_data * working_data);


#endif // !_qua_los_play_file_h_
