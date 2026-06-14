
#ifndef _qm_realtimer_label_h_
#define _qm_realtimer_label_h_

#include "lvgl/lvgl.h"

/**
 * time_style:
 * 0: only time
 * 1: only date
 * 2: date and time
 */
lv_obj_t* qm_realtime_label_create(lv_obj_t *superview, int time_style);


#endif //_qm_realtimer_label_h_
