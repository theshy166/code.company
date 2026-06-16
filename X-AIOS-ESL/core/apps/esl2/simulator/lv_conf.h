#if 1
#ifndef LV_CONF_H
#define LV_CONF_H
#include <stdio.h>
#define LV_USE_HOR_SIZE         800
#define LV_USE_VER_SIZE         480
#define LV_STDLIB_CLIB   1
#define LV_STDLIB_BUILTIN   0
#define LV_COLOR_DEPTH 32
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_CLIB
#define LV_USE_STDLIB_STRING    LV_STDLIB_CLIB
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_CLIB
#define LV_STDINT_INCLUDE       <stdint.h>
#define LV_STDDEF_INCLUDE       <stddef.h>
#define LV_STDBOOL_INCLUDE      <stdbool.h>
#define LV_INTTYPES_INCLUDE     <inttypes.h>
#define LV_LIMITS_INCLUDE       <limits.h>
#define LV_STDARG_INCLUDE       <stdarg.h>
#define LV_DEF_REFR_PERIOD  33
#define LV_DPI_DEF 130
#define LV_USE_OS   LV_OS_NONE
#define LV_DRAW_BUF_STRIDE_ALIGN                1
#define LV_DRAW_BUF_ALIGN                       4
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE    (24 * 1024)
#define LV_USE_DRAW_SW 1
#if LV_USE_DRAW_SW == 1
    #define LV_DRAW_SW_DRAW_UNIT_CNT    1
    #define LV_DRAW_SW_COMPLEX          1
#endif
#define LV_USE_LOG 1
#define LV_USE_THEME_DEFAULT 1
#define LV_USE_THEME_SIMPLE 1
#define LV_USE_THEME_MONO 1
#define LV_USE_LABEL 1
#define LV_USE_FREETYPE 1
#define LV_USE_SDL 0
#endif
#endif
