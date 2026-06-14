#ifndef  _QM_2_LABEL_H_
#define  _QM_2_LABEL_H_

#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

#if defined(CONFIG_XOS_APP_ESL2)
lv_obj_t *qm_2_label_create(lv_obj_t *parent, int w, int h, const char *label_str, const char *value_str);
#endif //CONFIG_XOS_APP_ESL2

#endif  /* _QM_2_LABEL_H_ */
