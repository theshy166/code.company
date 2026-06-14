/**
 * @file menu_view.h
 *
 */

#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../menu/menu_res_map.h"
#include "menu_struct.h"

#if defined(CONFIG_RESOLUTION_320x240)
#define XOS_USE_MENU_VIEW_WIDTH   300
#define XOS_USE_MENU_VIEW_HEIGHT  230
#define XOS_USE_MENU_LIST_ITEM_HEIGHT  40
#define XOS_USE_MENU_LIST_ITEM_FONT ft_font_ttf_14
#define XOS_USE_MENU_LIST_ITEM_IMAGE_WIDTH  20
#define XOS_USE_MENU_LIST_ITEM_IMAGE_HEIGHT  20
#define XOS_USE_MENU_TITLE_LABEL_HEIGHT  26
#elif defined(CONFIG_RESOLUTION_240x320)
#define XOS_USE_MENU_VIEW_WIDTH   230
#define XOS_USE_MENU_VIEW_HEIGHT  300
#define XOS_USE_MENU_LIST_ITEM_HEIGHT  40
#define XOS_USE_MENU_LIST_ITEM_FONT ft_font_ttf_14
#define XOS_USE_MENU_LIST_ITEM_IMAGE_WIDTH  20
#define XOS_USE_MENU_LIST_ITEM_IMAGE_HEIGHT  20
#define XOS_USE_MENU_TITLE_LABEL_HEIGHT  26
#else
#define XOS_USE_MENU_VIEW_WIDTH   400
#define XOS_USE_MENU_VIEW_HEIGHT  460
#define XOS_USE_MENU_LIST_ITEM_HEIGHT  60
#define XOS_USE_MENU_LIST_ITEM_FONT ft_font_ttf_20
#define XOS_USE_MENU_LIST_ITEM_IMAGE_WIDTH   30
#define XOS_USE_MENU_LIST_ITEM_IMAGE_HEIGHT  30
#define XOS_USE_MENU_TITLE_LABEL_HEIGHT  26
#endif

typedef struct {
    lv_obj_t *parent;
    lv_obj_t *panel;
    lv_obj_t *list;
    lv_obj_t *back_button;
    bool      update;
    const char *exit_app_name;
} menu_view_t;

void menu_view_create(menu_info *menuconfig, const void *bg_img, const char *title, const char *exit_app_name);
menu_view_t* menu_get_view(void);

void menu_res_set_img_mapping_table(image_src_mapping *ptr);
void menu_res_set_text_mapping_table(text_id_mapping *ptr);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
