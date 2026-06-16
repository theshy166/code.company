
#ifndef _QME_LV_H_
#define _QME_LV_H_

#include "lvgl/lvgl.h"

//Clip area if it has bg image
int qme_set_clip_area(lv_display_t *disp, lv_area_t *area);

//screen buffer
int qme_init_scr_buf(void *virt, unsigned int phy, lv_display_t *disp, int screen_index);

#endif
