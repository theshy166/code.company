/**
 * @file menu_struct.h
 *
 */

#ifndef MENU_STRUCT_H
#define MENU_STRUCT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#define ENABLE_MENU_DEBUG 1

#if ENABLE_MENU_DEBUG
#define LOG_MENU_DEBUG(fmt, ...) do { \
    LV_LOG_USER(fmt, ##__VA_ARGS__); \
} while (0)
#else
#define LOG_MENU_DEBUG(fmt, ...)
#endif

typedef struct {
    char* item_id;
    char* display_icon;
    char* display_name;
    char* type;
    int option_count;
    char** options;
    int option_index;
    int orig_index;
} menu_item;

typedef struct {
    const char *json_path;
    int (*callback)(char *item_id, char *value, lv_event_t *e);
} menu_param;

typedef struct {
    menu_item *items;
    int itemCount;
    char *ver;
    menu_param param;
} menu_info;

typedef struct {
    lv_ll_t menu_list;
    int inited;
} menu_lists;

menu_info* menu_init(menu_param *param);
void menu_deinit(menu_info *menu);

char* menu_get_item_type(menu_info *menu, const char *item_id);
int menu_get_item_value(menu_info *menu, const char *item_id, char *item_val);
void menu_update_item(menu_info *menu, const char *item_id, int new_option_index);
void menu_restore_all_items(menu_info *menu);

//menu_param* menu_getparam(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
