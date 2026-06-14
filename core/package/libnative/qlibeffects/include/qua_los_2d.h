
/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef _QUA_XOS_2D_H__
#define _QUA_XOS_2D_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>

#include "qm_effects.h"

int los_effects_intercept_flush(int act_x1, int act_y1, int act_x2, int act_y2, int *color_p);
void effect_show_app_logo(void *img_addr, int src_width, int src_height, int dst_width, int dst_height);

int qua_los_tv_open(void);
int qua_los_tv_close(void);
int qua_los_tv_beginjob(void);
int qua_los_tv_endjob(int s32Handle);
int qua_los_tv_canceljob(int s32Handle);
int qua_los_2d_copy(unsigned long , int, int, qm_area_t *,
        unsigned long, int, int, qm_area_t *);
int qua_los_2d_copy_task(int s32Handle, 
    unsigned long , int , int , qm_area_t *, 
    unsigned long , int , int , qm_area_t *);
int qua_xos_fill_color(int job_id, unsigned long phyAddr,
    int dst_w, int dst_h, int color_data, int x, int y, int w, int h);
void qua_los_save_file(int *color_p, int size);

#endif //_QUA_XOS_2D_H__
