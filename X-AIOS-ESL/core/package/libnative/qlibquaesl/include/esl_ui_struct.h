

/**
 * @file esl_ui_struct.h
 * @brief ESL (Electronic Shelf Label) UI structure definitions
 */
#ifndef __ESL_UI_STRUCT_H__
#define __ESL_UI_STRUCT_H__

#include <stdio.h>

/**
 * @brief ESL UI structure
 * Contains UI elements and their states for ESL display
 */
typedef struct {
    // Status LED
    lv_obj_t *led;             /**< Pointer to the LED LVGL object */
    bool led_visible;          /**< LED visibility status */
    
    // Progress bar
    lv_obj_t *bar;             /**< Pointer to the progress bar LVGL object */
    bool bar_visible;          /**< Progress bar visibility status */
    uint32_t bar_value;        /**< Progress bar current value */
    
    // Download log
    lv_obj_t *down_log;        /**< Pointer to the download log LVGL object */
    bool down_log_visible;     /**< Download log visibility status */
    char down_log_file_name[128];  /**< Download log file name buffer */
} esl_ui_struct_t;

#endif
