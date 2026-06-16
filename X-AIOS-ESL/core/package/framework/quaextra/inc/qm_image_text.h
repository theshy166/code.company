
#ifndef _QM_IMG_TEXT_H_
#define _QM_IMG_TEXT_H_

#include "lvgl/lvgl.h"

lv_obj_t *qm_image_text_init(lv_obj_t *p, int w, int h, const void *img_normal, const void *img_selected, const char *label_name, bool is_selected);
void qm_image_text_update(lv_obj_t *cont, const void *img_normal, const void *img_selected, bool is_selected);

#endif //_QM_IMG_TEXT_H_
