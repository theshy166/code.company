#ifndef  __QM_SHELF_LABEL_H__
#define  __QM_SHELF_LABEL_H__

#include "lvgl/lvgl.h"
#include "xos_app_conf.h"
#include "../model/product_comp_model.h"
#include "../model/three_label_comp_model.h"

#if XOS_USE_APP_NATIVE_ESL != 0

product_comp_t *product_comp_create(lv_obj_t *parent, int w, int h);
void product_comp_destroy(product_comp_t *panel);
//
void update_product_comp_image(product_comp_t *product_comp, const char *image_path);
void update_product_comp_title(product_comp_t *product_comp, const char *title);
void update_product_comp_price(three_label_comp_t *product_comp, float price);

#endif //XOS_USE_APP_NATIVE_ESL

#endif // __QM_SHELF_LABEL_H__
