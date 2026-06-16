
/**
 * @file esl_progress_bar.h
 * @brief ESL (Electronic Shelf Label) progress bar UI component
 */
#ifndef __ESL_PROGRESS_BAR_H__
#define __ESL_PROGRESS_BAR_H__

#include "lvgl/lvgl.h"

/**
 * @brief Create a new progress bar UI component
 * @param parent Pointer to the parent LVGL object
 * @return Pointer to the newly created progress bar object
 */
lv_obj_t * create_progress_bar(lv_obj_t * parent);

/**
 * @brief Update the value of a progress bar
 * @param progress_bar Pointer to the progress bar object
 * @param value New value to set (typically 0-100 for percentage)
 */
void update_progress_bar(lv_obj_t * progress_bar, int value);

/**
 * @brief Destroy a progress bar UI component and free its resources
 * @param progress_bar Pointer to the progress bar object to destroy
 */
void destroy_progress_bar(lv_obj_t * progress_bar);

#endif