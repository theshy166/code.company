/**
 * @file esl_down_log.h
 * @brief ESL (Electronic Shelf Label) download log UI component
 */
#ifndef __ESL_DOWN_LOG_H__
#define __ESL_DOWN_LOG_H__

#include "lvgl/lvgl.h"

/**
 * @brief Create a download log UI component
 * @param parent Pointer to the parent LVGL object
 * @return Pointer to the newly created download log object
 */
lv_obj_t * esl_down_log_create(lv_obj_t * parent);

/**
 * @brief Update the download log with a new file name
 * @param obj Pointer to the download log object
 * @param file_name Pointer to the file name string to display
 */
void esl_down_log_update(lv_obj_t * obj, char * file_name);

#endif