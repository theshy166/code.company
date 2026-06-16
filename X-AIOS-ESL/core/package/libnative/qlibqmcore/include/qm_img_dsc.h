#ifndef __QM_IMG_DSC_H__
#define __QM_IMG_DSC_H__

#include "lvgl/lvgl.h"

void qme_get_page_image_dsc(char *img_path, int display_width, int display_height, lv_image_dsc_t **image_dsc);
void qme_remove_page_image_dsc(char *img_path);

#endif

