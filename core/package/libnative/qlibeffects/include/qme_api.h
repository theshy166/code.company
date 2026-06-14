
#ifndef _QME_LV_H_
#define _QME_LV_H_

#include "lvgl/lvgl.h"

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} qm_area_t;

//Clip area if it has bg image
int qme_set_clip_area(lv_display_t *disp, lv_area_t *area);

//screen buffer
int qme_init_scr_buf(void *virt, unsigned long phy, lv_display_t *disp, int screen_index);

int qm_effect_init(int screen_w, int screen_h);
int qm_effect_inits(int screen_w, int screen_h, unsigned long phy);
int qm_effect_init_scr_info(int screen_w, int screen_h, void *virtAddr, unsigned long phy);

#endif
