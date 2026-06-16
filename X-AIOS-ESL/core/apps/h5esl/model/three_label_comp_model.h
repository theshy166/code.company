#ifndef _THREE_LABEL_OBJS_H_
#define _THREE_LABEL_OBJS_H_

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t *parent;
    lv_obj_t *label;
    lv_obj_t *value;
    lv_obj_t *unit;
} three_label_comp_t;


#endif // _THREE_LABEL_OBJS_H_
