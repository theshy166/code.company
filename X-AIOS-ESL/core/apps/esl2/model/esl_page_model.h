#ifndef _ESL_PAGE_OBJS_H_
#define _ESL_PAGE_OBJS_H_

#include "lvgl/lvgl.h"
#include "product_comp_model.h"

//
typedef struct lvobj_h5esl {
    lv_obj_t *browser_wnd;
    lv_obj_t *loading_ui;
    void *player;
    //
    product_comp_t *product1;
    product_comp_t *product2;
}lvobj_h5esl;

#endif // _ESL_PAGE_OBJS_H_
