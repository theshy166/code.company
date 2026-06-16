/**
 * @file evdev.h
 *
 */

#ifndef EVDEV_H
#define EVDEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_drv_conf.h"
#include "lvgl.h"

void xos_evdev_indev_init(void);
void xos_evdev_mouse_find(void);
void xos_evdev_touch_find(void);
void xos_evdev_keypad_find(void);

void xos_evdev_keypad_init(void);
void xos_evdev_mouse_init(void);
void xos_evdev_touch_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*EVDEV_H*/
