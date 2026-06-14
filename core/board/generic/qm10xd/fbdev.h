/**
 * @file fbdev.h
 *
 */

#ifndef FBDEV_H
#define FBDEV_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_FBDEV || USE_BSD_FBDEV

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void fbdev_init(void);
void fbdev2_init(void);
void fbdev_exit(void);
void fbdev2_exit(void);
#if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9
void fbdev_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p);
void fbdev2_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p);
#else
void fbdev_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p);
void fbdev2_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p);
#endif
void *fbdev_req_mem(int size);
void *fbdev2_req_mem(int size);
void fbdev_set_hole(int x, int y, int width, int height);
void fbdev2_set_hole(int x, int y, int width, int height);
//
void devfb_flush(void *virtualAddr, uint32_t offsetx, uint32_t offsety, uint32_t width, uint32_t height);
void devfb2_flush(void *virtualAddr, uint32_t offsetx, uint32_t offsety, uint32_t width, uint32_t height);

/**********************
 *      MACROS
 **********************/

#endif  /*USE_FBDEV*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*FBDEV_H*/
