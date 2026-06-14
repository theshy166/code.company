/**
 * @file esl_ui_show.h
 * @brief ESL (Electronic Shelf Label) UI display functions
 */
#ifndef __ESL_UI_SHOW_H__
#define __ESL_UI_SHOW_H__
#include "lvgl/lvgl.h"
#include "esl_base_model.h"

/**
 * @brief Show ESL UI based on the provided model
 * @param model Pointer to the ESL base model containing UI elements
 * @param top Pointer to the top LVGL object to render on
 * @param width Display width in pixels
 * @param height Display height in pixels
 * @param index Display index identifier
 */
void esl_show_ui(esl_base_model_t * model, lv_obj_t * top, int width, int height, int index);

/**
 * @brief Create a LVGL UI component based on the provided model item
 * @param obj Pointer to the parent LVGL object
 * @param item Pointer to the ESL base model child item
 */
void create_component_lvgl(lv_obj_t *obj, esl_base_model_child_t *item);

/**
 * @brief Set the background for a specific display index
 * @param bg Pointer to the background LVGL object
 * @param index Display index identifier
 */
void set_bg(lv_obj_t * bg, int index);

/**
 * @brief Get the background for a specific display index
 * @param index Display index identifier
 * @return Pointer to the background LVGL object, or NULL if not found
 */
lv_obj_t * get_bg(int index);

#endif // __ESL_UI_SHOW_H__