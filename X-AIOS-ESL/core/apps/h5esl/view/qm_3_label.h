#ifndef  _QM_3_LABEL_H_
#define  _QM_3_LABEL_H_

#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

#if XOS_USE_APP_NATIVE_ESL != 0

#include "../model/three_label_comp_model.h"

void qm_3_label_create(lv_obj_t *parent, int w, int h, three_label_comp_t *qm_3_label_comp,
    const char *label_str, const char *value_str, const char *unit_str, lv_color_t color);

#endif //XOS_USE_APP_NATIVE_ESL

#endif  /* _QM_3_LABEL_H_ */
