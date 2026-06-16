
#ifndef __qua_font_h__
#define __qua_font_h__

//
#include "lvgl/lvgl.h"
//
//Deprecated
void qua_los_ft_font_init(void);
//Deprecated
void qua_los_ft_init_with_size(lv_font_t **ppFont, short font_size, short style);

//init with font path
void qua_xos_ft_init_with_path(const char *ttf_path);
//init with font path and size
void qua_xos_ft_init_with_size_path(lv_font_t **ppFont, short font_size, const char *ttf_path, short style);

#endif //__qua_font_h__
