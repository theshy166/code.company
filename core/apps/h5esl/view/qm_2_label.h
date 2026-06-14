#ifndef  _QM_2_LABEL_H_
#define  _QM_2_LABEL_H_

#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

#if XOS_USE_APP_NATIVE_ESL != 0
lv_obj_t *qm_2_label_create(lv_obj_t *parent, int w, int h, const char *label_str, const char *value_str);
#endif //XOS_USE_APP_NATIVE_ESL

#endif  /* _QM_2_LABEL_H_ */
