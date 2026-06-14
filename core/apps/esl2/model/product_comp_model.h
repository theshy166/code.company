#ifndef _ESL_SHELF_COMP_H_
#define _ESL_SHELF_COMP_H_

#include "lvgl/lvgl.h"
#include "three_label_comp_model.h"

//
typedef struct {
    lv_obj_t *parent;
    lv_obj_t *title;
    //
    three_label_comp_t three_label1;//价格条：售价
    three_label_comp_t three_label2;//价格条：特价
    //
    lv_obj_t *image;//产品图片
    //
    lv_obj_t *product_address;//产地
    lv_obj_t *product_level;//等级
} product_comp_t;

#endif // _ESL_SHELF_COMP_H_
