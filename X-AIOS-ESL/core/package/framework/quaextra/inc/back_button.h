
#ifndef _back_button_h_
#define _back_button_h_
#include "lvgl/lvgl.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
} back_button_desc_t;

typedef void (*float_wnd_callback)(lv_event_t *);
lv_obj_t* create_back_button(const lv_img_dsc_t *img_dsc, float_wnd_callback cb);
lv_obj_t* create_back_button_ex(const lv_img_dsc_t *img_dsc, float_wnd_callback cb, back_button_desc_t area);

#endif //
