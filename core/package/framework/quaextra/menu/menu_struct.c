/**
 * @file menu_struct.c
 *
 */
#include "menu_struct.h"
#include "xos_misc.h"
#include "cJSON.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#ifdef CONFIG_XOS_FWK_PARAM
#include "param.h"
#endif

static menu_info* menu_parseJson(const char *jsonFilePath);
static void menu_change_item(menu_info *menu, bool update_all, const char *item_id, int newOptionIndex);
static menu_item *menu_get_item(menu_info *menu, const char *item_id);

static menu_lists json_lists = {0};

#ifdef CONFIG_XOS_FWK_PARAM
static void menu_change_param(menu_info *menu, const char *item_id, int new_option_index);
static void menu_load_param(menu_info *menu);
#endif

menu_info* menu_init(menu_param *param)
{
    int i, loaded = 0, cur_index = 0, free_index = 0;
    bool has_free_index = false;
    menu_info *menu = NULL;
    menu_info** menu_ptr;

    if (!json_lists.inited) {
        _lv_ll_init(&json_lists.menu_list, sizeof(menu_info*));
        menu_res_init_mapping_table();
        json_lists.inited = 1;
    }
#if 0
    _LV_LL_READ(&json_lists.menu_list, menu_ptr) {
        if (0 == strcmp(param->json_path, (*menu_ptr)->param.json_path)) {
            LOG_MENU_DEBUG("%s already loaded", param->json_path);
            loaded = 1;
            break;
        }
    }
#endif

    if (!loaded) {
        menu  = menu_parseJson(param->json_path);
        if (NULL == menu) {
            LOG_MENU_DEBUG("loading %s fail", param->json_path);
            return NULL;
        }
        #ifdef CONFIG_XOS_FWK_PARAM
        menu_load_param(menu);
        #endif
        memcpy(&menu->param, param, sizeof(menu_param));
        menu_info **new_menu = _lv_ll_ins_tail(&json_lists.menu_list);
        *new_menu = menu;
    } else {
        menu = *menu_ptr;
        if (param->callback)
            menu->param.callback = param->callback;
    }
    return menu;
}

static void menu_clear(menu_info *menu) {
    menu_info** menu_ptr;

    _LV_LL_READ(&json_lists.menu_list, menu_ptr) {
        if (*menu_ptr == menu) {
            _lv_ll_remove(&json_lists.menu_list, menu_ptr);
            break;
        }
    }
}

void menu_deinit(menu_info *menu)
{
    if (NULL == menu) {
        LOG_MENU_DEBUG("menu is empty");
        return;
    }
    menu_clear(menu);

    for (int i = 0; i < menu->itemCount; i++) {
        free(menu->items[i].item_id);
        free(menu->items[i].display_name);
        free(menu->items[i].type);
        if (menu->items[i].options) {
            for (int j = 0; j < menu->items[i].option_count; j++) {
                free(menu->items[i].options[j]);
            }
            free(menu->items[i].options);
        }
    }
    free(menu->items);
    free(menu);
}

#ifdef CONFIG_XOS_FWK_PARAM
static void menu_load_param(menu_info *menu)
{
    char buf[50] = {0};

    for (int i = 0; i < menu->itemCount; i++) {
        sprintf(buf, "home_setting:%s", menu->items[i].item_id);
        menu->items[i].option_index = param_get_int(buf, 0);
        LOG_MENU_DEBUG("LOAD [%s]=[%d]", buf, menu->items[i].option_index);
    }
}
#endif

static menu_info* menu_parseJson(const char *jsonFilePath) {
    FILE *fp = fopen(jsonFilePath, "r");
    cJSON *json_icon;
    if (!fp) {
        LV_LOG_USER("Error: Failed to open file '%s'.\n", jsonFilePath);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    char *jsonString = (char *)malloc(fileSize + 1);
    memset(jsonString, 0, fileSize + 1);
    fseek(fp, 0, SEEK_SET);
    fread(jsonString, 1, fileSize, fp);
    cJSON *root = cJSON_Parse(jsonString);
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    cJSON *menuArray = cJSON_GetObjectItem(root, "menus");

    menu_info *menu = (menu_info *)malloc(sizeof(menu_info));
    memset(menu , 0 , sizeof(menu_info));
    if (ver)
        menu->ver = strdup(ver->valuestring);
    menu->itemCount = cJSON_GetArraySize(menuArray);
    menu->items = (menu_item *)malloc(menu->itemCount * sizeof(menu_item));
    memset(menu->items, 0x0, menu->itemCount * sizeof(menu_item));
    for (int i = 0; i < menu->itemCount; i++) {
        cJSON *item = cJSON_GetArrayItem(menuArray, i);
        cJSON *optionIndex, *origIndex;

        menu->items[i].item_id = strdup(cJSON_GetObjectItem(item, "itemId")->valuestring);

        json_icon = cJSON_GetObjectItem(item, "displayIcon");
        if (json_icon)
            menu->items[i].display_icon = strdup(json_icon->valuestring);
        menu->items[i].display_name = strdup(cJSON_GetObjectItem(item, "displayName")->valuestring);
        menu->items[i].type = strdup(cJSON_GetObjectItem(item, "type")->valuestring);

        cJSON *optionsArray = cJSON_GetObjectItem(item, "options");
        if (optionsArray) {
            menu->items[i].option_count = cJSON_GetArraySize(optionsArray);
            menu->items[i].options = (char **)malloc( menu->items[i].option_count * sizeof(char *));
            for (int j = 0; j < menu->items[i].option_count; j++) {
                menu->items[i].options[j] = strdup(cJSON_GetArrayItem(optionsArray, j)->valuestring);
            }
        } else {
            menu->items[i].option_count = 0;
            menu->items[i].options = NULL;
        }
        optionIndex = cJSON_GetObjectItem(item, "optionIndex");
        if (optionIndex)
            menu->items[i].option_index = optionIndex->valueint;

        origIndex = cJSON_GetObjectItem(item, "origIndex");
        if (origIndex)
            menu->items[i].orig_index = origIndex->valueint;
    }

    cJSON_Delete(root);
    fclose(fp);
    free(jsonString);
    return menu;
}

#ifdef CONFIG_XOS_FWK_PARAM
static void menu_change_param(menu_info *menu, const char *item_id, int new_option_index)
{
    char buf[50] = {0};
    sprintf(buf, "home_setting:%s", item_id);
    param_set_int(buf, new_option_index);
    param_save();
    LOG_MENU_DEBUG("SAVE [%s]=[%d]", buf, new_option_index);
}
#endif

static void menu_change_item(menu_info *menu, bool update_all, const char *item_id, int new_option_index) {
    if (NULL == menu || 0 == menu->itemCount) {
        LOG_MENU_DEBUG("menu is empty, please load the json first");
        return;
    }
    FILE *fp = fopen(menu->param.json_path, "w");
    if (!fp) {
        LOG_MENU_DEBUG("Error: Failed to open file '%s' for writing.\n", menu->param.json_path);
        return;
    }

    // 将更新后的 menu_info 结构重新序列化为 JSON 字符串
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", menu->ver);
    cJSON_AddItemToObject(root, "menus", cJSON_CreateArray());
    for (int i = 0; i < menu->itemCount; i++) {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "itemId", menu->items[i].item_id);
        if (menu->items[i].display_icon)
            cJSON_AddStringToObject(item, "displayIcon", menu->items[i].display_icon);
        cJSON_AddStringToObject(item, "displayName", menu->items[i].display_name);
        cJSON_AddStringToObject(item, "type", menu->items[i].type);
        if (menu->items[i].option_count > 0) {
            cJSON_AddItemToObject(item, "options", cJSON_CreateStringArray((const char **)menu->items[i].options, menu->items[i].option_count));
        }
        if (update_all) {
            menu->items[i].option_index = menu->items[i].orig_index;
        } else if (0 == strcmp(item_id, menu->items[i].item_id)) {
            menu->items[i].option_index = new_option_index;
        }
        if (0 != strcmp(menu->items[i].type, "button")) {
            cJSON_AddNumberToObject(item, "optionIndex", menu->items[i].option_index);
            cJSON_AddNumberToObject(item, "origIndex", menu->items[i].orig_index);
        }

        cJSON_AddItemToArray(cJSON_GetObjectItem(root, "menus"), item);
    }
    char *updatedJsonString = cJSON_Print(root);
    cJSON_Delete(root);

    //将更新后的 JSON 字符串写回到原始的 JSON 文件中
    fwrite(updatedJsonString, strlen(updatedJsonString), 1, fp);
    fclose(fp);
    free(updatedJsonString);
}

static menu_item *menu_get_item(menu_info *menu, const char *item_id) {
    if (NULL == menu) {
        LV_LOG_USER("menu NULL, please load the json(call menu_json_init) first\n");
        return NULL;
    }
    for (int i = 0; i < menu->itemCount; i++) {
        if (0 == strcmp(item_id, menu->items[i].item_id)) {
            return &menu->items[i];
        }
    }
    return NULL;
}

char* menu_get_item_type(menu_info *menu, const char *item_id) {
    menu_item *item = menu_get_item(menu, item_id);
    if (NULL == item) {
          return -1;
    }

    return item->type;
}

int menu_get_item_value(menu_info *menu, const char *item_id, char *item_val) {
    menu_item *item = menu_get_item(menu, item_id);
    if (NULL == item) {
          return -1;
    }

    if (0 == strcmp(item->type, "optionmenu") || 0 == strcmp(item->type, "optionentry")) {
        strcpy(item_val, item->options[item->option_index]);
    } else if(0 == strcmp(item->type, "checkbox")) {
        strcpy(item_val, item->option_index ? "ON" : "OFF");
    } else if(0 == strcmp(item->type, "button")) {
        return -1;
    }

    return item->option_index;
}

void menu_update_item(menu_info *menu, const char *item_id, int new_option_index) {
    #ifdef CONFIG_XOS_FWK_PARAM
    menu_change_param(menu, item_id, new_option_index);
    #else
    menu_change_item(menu, false, item_id, new_option_index);
    #endif
}

void menu_restore_all_items(menu_info *menu) {
    menu_change_item(menu, true, NULL, 0);
}

