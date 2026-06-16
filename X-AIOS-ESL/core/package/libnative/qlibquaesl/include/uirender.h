
/**
 * @file uirender.h
 * @brief UI rendering functions for ESL (Electronic Shelf Label)
 */

#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#include "lvgl/lvgl.h"

/**
 * @brief Initialize the UI renderer
 */
void uirender_init(void);

/**
 * @brief Quit the UI renderer and release resources
 */
void uirender_quit(void);

/**
 * @brief Get the top layer object for the first display
 * @param index Layer index
 * @return Pointer to the LVGL object representing the top layer
 */
lv_obj_t * get_top_layer1(int index);

/**
 * @brief Get the top layer object for the second display
 * @param index Layer index
 * @return Pointer to the LVGL object representing the top layer
 */
lv_obj_t * get_top_layer2(int index);

/**
 * @brief Set the top layer as hidden for a specific display
 * @param index Display index
 */
void set_top_hidden(int index);

/**
 * @brief Set the top layer as visible for a specific display
 * @param index Display index
 */
void set_top_show(int index);

/**
 * @brief Set the visibility of the green LED
 * @param index Display index
 * @param is_show LED visibility (1 for visible, 0 for hidden)
 */
void set_show_green_led(int index, int is_show);

#endif
