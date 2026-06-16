#ifndef __QUA_FONT_LL_H__
#define __QUA_FONT_LL_H__

#include "lvgl/lvgl.h"

extern void __init_font_list(void);
extern void __add_font_list(lv_font_t *font, const char* font_path, short font_size);
extern void __clear_font_list(void);
extern lv_font_t* __get_font_by_size(const char* font_path, short font_size);
extern void __prt_font_by_size(void);

#endif /*__QUA_FONT_LL_H__*/

