
#ifndef _QM_TIME_LABEL_H_
#define _QM_TIME_LABEL_H_

#include "lvgl/lvgl.h"
//STOP WATCH
lv_obj_t *qm_time_label_init(lv_obj_t *bg_wnd);
void qm_time_label_update(lv_obj_t *time_label, const char *time_str);

#endif //_TIME_LABEL_H_
