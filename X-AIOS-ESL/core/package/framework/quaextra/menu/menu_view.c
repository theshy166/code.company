/**
 * @file menu_view.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "stdio.h"
#include "menu_struct.h"
#include "menu_view.h"
#include "lang/xos_strid.h"
#include "xos_ability.h"
#include "appmanager.h"
#include "back_button.h"
#include "qua_los_msgpop.h"
#include "sys/time.h"
#include "qua_font.h"
//#include "menu_res_map.h"
#include "lang/QuaResManager.h"

#define MENU_LIST_BORDER_WIDTH 2
#define MENU_LIST_BORDER_RADIUS 0
#define MENU_ITEM_BORDER_WIDTH 0
#define MENU_ITEM_BORDER_RADIUS 0
#define MENU_ITEM_BORDER_COLOR         0xE6E6E6  // 灰色
#define MENU_ITEM_BORDER_CHECKED_COLOR 0xBCDEFF  // 蓝色
#define MENU_ITEM_BG_COLOR             0xFFFFFF  // 白色
#define MENU_ITEM_FONT_COLOR           0x000000  // 黑色
#define MENU_CHECKBOX_DEFAULT_COLOR    0x919191  // 灰色
#define MENU_CHECKBOX_KNOB_COLOR       0xFFFFFF  // 蓝色

static image_src_mapping *image_src_mapping_table_ptr = NULL;
static text_id_mapping *text_id_mapping_table_ptr = NULL;

static void menu_optionmenu_change_state(menu_item *config, lv_obj_t *obj, lv_event_t *e);
static void menu_checkbox_change_state(menu_item *config, lv_obj_t *obj, lv_event_t *e);
static lv_obj_t* menu_view_create_list(lv_obj_t *parent, const char* title_txt);
static const char* get_text_by_id(const char* textid);
static void menu_float_wnd_callback(lv_event_t *event);

menu_view_t g_menu_view = {0};

/**********************
 *  STATIC VARIABLES
 **********************/
LV_IMG_DECLARE(icon_back);
LV_IMG_DECLARE(icon_back_left);

/**********************
 *  GLOBAL VARIABLES
 **********************/
extern lv_group_t * g_indev_group;

static const char* get_text_by_id(const char* textid) {
    int i = 0;
    if (strncmp(textid, "ID_", 3) == 0) {
        while (text_id_mapping_table_ptr[i].textid != 0 && text_id_mapping_table_ptr[i].text != NULL) {
            if (strcmp(text_id_mapping_table_ptr[i].text, textid) == 0) {
                return GetTextRes(text_id_mapping_table_ptr[i].textid);
            }
            i++;
        }
    }

    return textid;
}

static const void* get_img_by_id(const char* imageid) {
    int i = 0;
    while (image_src_mapping_table_ptr[i].imgsrc != 0 && image_src_mapping_table_ptr[i].name != NULL) {
        if (strcmp(image_src_mapping_table_ptr[i].name, imageid) == 0) {
            return image_src_mapping_table_ptr[i].imgsrc;
        }
        i++;
    }

    return imageid;
}


static void menu_view_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    //XOS_LOGI("%s %d %d\n", __func__, __LINE__, code);

    if(LV_EVENT_KEY == code) {
        char c = *((char *)lv_event_get_param(e));
        uint16_t child_cnt = lv_obj_get_child_cnt(obj);
        uint16_t i;

        LV_LOG_USER("%s %d %d %d\n", __func__, __LINE__, code, c);
        if(c == LV_KEY_RIGHT || c == LV_KEY_DOWN) {
            for(i = 0; i < child_cnt; i++) {
                lv_obj_t * child_obj = lv_obj_get_child(obj, i);
                if (child_obj == g_menu_view.list){
                    lv_obj_t * list = child_obj;
                    uint16_t item_cnt = lv_obj_get_child_cnt(list);
                    //XOS_LOGI("%s %d %d\n", __func__, __LINE__, item_cnt);

                    for (i = 0; i < item_cnt; i++) {
                        lv_obj_t * item = lv_obj_get_child(list, i);
                        if (lv_obj_has_state(item, LV_STATE_CHECKED)) {
                            //XOS_LOGI("%s %d \n", __func__, __LINE__);
                            lv_obj_clear_state(item, LV_STATE_CHECKED);
                            uint16_t row_id = (i == item_cnt - 1) ?  0: (i + 1);
                            item = lv_obj_get_child(list, row_id);
                            lv_obj_add_state(item, LV_STATE_CHECKED);
                            lv_obj_scroll_to_view(item, LV_ANIM_OFF);
                            return;
                        }
                    }
                    lv_obj_add_state(lv_obj_get_child(list, 0), LV_STATE_CHECKED);
                    lv_obj_scroll_to_view(lv_obj_get_child(list, 0), LV_ANIM_OFF);
                    return;
                }
            }
        }
        else if(c == LV_KEY_LEFT || c == LV_KEY_UP) {
            for(i = 0; i < child_cnt; i++) {
                lv_obj_t * child_obj = lv_obj_get_child(obj, i);
                if(child_obj == g_menu_view.list){
                    lv_obj_t * list = child_obj;
                    uint16_t item_cnt = lv_obj_get_child_cnt(list);
                    //XOS_LOGI("%s %d %d\n", __func__, __LINE__, item_cnt);
                    for(i = 0; i < item_cnt; i++) {
                        lv_obj_t * item = lv_obj_get_child(list, i);
                        uint16_t row_id = (i == 0) ? item_cnt - 1 : (i - 1);
                        if (lv_obj_has_state(item, LV_STATE_CHECKED)) {
                            //XOS_LOGI("%s %d \n", __func__, __LINE__);
                            lv_obj_clear_state(item, LV_STATE_CHECKED);
                            item = lv_obj_get_child(list, row_id);
                            lv_obj_add_state(item, LV_STATE_CHECKED);
                            lv_obj_scroll_to_view(item, LV_ANIM_OFF);
                            return;
                        }
                    }
                    lv_obj_add_state(lv_obj_get_child(list, 0), LV_STATE_CHECKED);
                    lv_obj_scroll_to_view(lv_obj_get_child(list, 0), LV_ANIM_OFF);
                }
            }
        } else if(LV_KEY_ENTER == c) {
            lv_obj_t * list = g_menu_view.list;
            uint16_t item_cnt = lv_obj_get_child_cnt(list);

            for (i = 0; i < item_cnt; i++) {
                lv_obj_t * item = lv_obj_get_child(list, i);
                if (lv_obj_has_state(item, LV_STATE_CHECKED)) {
                    menu_item *config = (menu_item*)lv_obj_get_user_data(item);
                    if (config) {
                        if (0 == strcmp(config->type, "checkbox")) {
                            lv_obj_t *switch_ctl = lv_obj_get_child(item, 1);
                            menu_checkbox_change_state(config, switch_ctl, e);
                        } else if (0 == strcmp(config->type, "button")) {
                            menu_info *menu = lv_obj_get_user_data(g_menu_view.parent);
                            menu_param *cur_param = &menu->param;
                            if (cur_param && cur_param->callback)
                                cur_param->callback(config->item_id, NULL, e);
                        } else if (0 == strcmp(config->type, "optionmenu") || 0 == strcmp(config->type, "optionentry")) {
                            menu_optionmenu_change_state(config, item, e);
                        }
                    }
                }
            }
        } else if (LV_KEY_ESC == c) {
            menu_float_wnd_callback(e);
        }
    }
}

static lv_obj_t* menu_view_create_list(lv_obj_t *parent, const char* title_txt)
{
    lv_obj_t *tab = (parent ? parent : lv_scr_act());
    LV_ASSERT_NULL(tab);
    lv_obj_clear_flag(tab, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *panel = lv_obj_create(tab);
    LV_ASSERT_NULL(panel);
    lv_obj_t *list = NULL;

    lv_obj_set_style_radius(tab, 0, LV_PART_MAIN);

    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, XOS_USE_MENU_VIEW_WIDTH, XOS_USE_MENU_VIEW_HEIGHT);
    lv_obj_set_style_bg_opa(panel, LV_OPA_TRANSP, 0);
    lv_obj_center(panel);
    lv_group_add_obj(lv_group_get_default(), panel);
    lv_group_focus_obj(panel);
    lv_obj_add_event_cb(panel, menu_view_event_cb, LV_EVENT_ALL, panel);

    // 添加标题 label
    lv_obj_t *label_cont = lv_obj_create(panel);
    lv_obj_remove_style_all(label_cont);
    lv_obj_set_size(label_cont, XOS_USE_MENU_VIEW_WIDTH, XOS_USE_MENU_TITLE_LABEL_HEIGHT);
    lv_obj_clear_flag(label_cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align_to(label_cont, panel, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *label = lv_label_create(label_cont);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_obj_set_style_text_font(label, XOS_USE_MENU_LIST_ITEM_FONT, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(MENU_ITEM_FONT_COLOR), LV_PART_MAIN);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    if (title_txt)
        lv_label_set_text(label, title_txt);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);

    list = lv_list_create(panel);
    lv_obj_remove_style_all(list);
    lv_obj_set_size(list, XOS_USE_MENU_VIEW_WIDTH, XOS_USE_MENU_VIEW_HEIGHT - XOS_USE_MENU_TITLE_LABEL_HEIGHT);
    lv_obj_set_style_border_width(list, MENU_LIST_BORDER_WIDTH, 0);
    lv_obj_set_style_radius(list, MENU_LIST_BORDER_RADIUS, 0);
    lv_obj_set_style_border_color(list, lv_color_hex(MENU_ITEM_BORDER_COLOR), 0);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(list, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(list, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(list, 5, 0);
    lv_obj_set_style_bg_color(list, lv_color_hex(MENU_ITEM_BG_COLOR), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(list, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(list, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_align_to(list, panel, LV_ALIGN_BOTTOM_MID, 0, 0);

    g_menu_view.panel = panel;
    g_menu_view.list = list;
    return list;
}

static void menu_click_clear_checked(void) {
    uint16_t i;
    lv_obj_t *list = g_menu_view.list;
    uint16_t item_cnt = lv_obj_get_child_cnt(list);

    for(i = 0; i < item_cnt; i++) {
        lv_obj_t * item = lv_obj_get_child(list, i);
        if (lv_obj_has_state(item, LV_STATE_CHECKED)) {
            lv_obj_clear_state(item, LV_STATE_CHECKED);
            return;
        }
    }
}

static void menu_optionmenu_change_state(menu_item *config, lv_obj_t *obj, lv_event_t *e) {
    if (config && config->option_count > 0) {
        menu_info *menu = lv_obj_get_user_data(g_menu_view.parent);
        if (0 == strcmp (config->type, "optionmenu")) {
            lv_obj_t * opt_obj = lv_obj_get_child(obj, config->display_icon? 2 : 1);
            config->option_index++;
            if (config->option_index >= config->option_count) {
                config->option_index = 0;
            }
            lv_label_set_text(opt_obj, get_text_by_id(config->options[config ->option_index]));
            menu_update_item(menu, config->item_id, config->option_index);
        }
        menu_param *cur_param = &menu->param;
        if (cur_param && cur_param->callback)
           cur_param->callback(config->item_id, config->options[config->option_index], e);
    }
}

static void menu_view_optionmenu_event_callback(lv_event_t *e)
{
    lv_obj_t *image_label_container =  (lv_obj_t *)lv_event_get_target(e);
    menu_item *config = (menu_item  *)lv_event_get_user_data(e);
    char key = *((char *)lv_event_get_param(e));
    if (e->code == LV_EVENT_CLICKED) {
        menu_click_clear_checked();
        menu_optionmenu_change_state(config, image_label_container, e);
    } else if (e->code == LV_EVENT_KEY && (key == LV_KEY_UP ||
        key == LV_KEY_DOWN ||
        key == LV_KEY_LEFT ||
        key == LV_KEY_RIGHT)) {
        lv_group_focus_obj(g_menu_view.panel);
        lv_event_send(g_menu_view.panel, e->code, e->param);
    }
}

static void menu_view_optionmenu_create(lv_obj_t *parent, menu_item *item)
{
    lv_obj_t *image_label_container;
    lv_obj_t *image_obj, *icon_obj;
    lv_obj_t *label_obj, *opt_obj;
    if (NULL == item) {
        return;
    }
    image_label_container = lv_btn_create(parent);
    lv_obj_set_size(image_label_container, XOS_USE_MENU_VIEW_WIDTH, XOS_USE_MENU_LIST_ITEM_HEIGHT);
    lv_obj_align(image_label_container, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(image_label_container, MENU_ITEM_BORDER_WIDTH, 0);
    lv_obj_set_style_border_color(image_label_container, lv_color_hex(MENU_ITEM_BORDER_COLOR), 0);
    lv_obj_set_style_radius(image_label_container, MENU_ITEM_BORDER_RADIUS, 0);
    lv_obj_set_style_bg_color(image_label_container, lv_color_hex(MENU_ITEM_BG_COLOR), 0);
    lv_obj_set_style_bg_color(image_label_container, lv_color_hex(MENU_ITEM_BORDER_CHECKED_COLOR), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_user_data(image_label_container, item);

    lv_obj_add_event_cb(image_label_container, menu_view_optionmenu_event_callback, LV_EVENT_CLICKED, item);
    lv_obj_add_event_cb(image_label_container, menu_view_optionmenu_event_callback, LV_EVENT_KEY, item);

    // 创建label
    label_obj = lv_label_create(image_label_container);
    lv_obj_set_style_text_font(label_obj, XOS_USE_MENU_LIST_ITEM_FONT, LV_STATE_DEFAULT);
    if (item->display_name != NULL) {
        lv_label_set_text(label_obj, get_text_by_id(item->display_name));
    }
    lv_obj_set_style_text_color(label_obj, lv_color_hex(MENU_ITEM_FONT_COLOR), 0);

    // 创建图标对象
    if (item->display_icon) {
        icon_obj = lv_img_create(image_label_container);
        lv_img_set_src(icon_obj, get_img_by_id(item->display_icon));
        lv_obj_align(icon_obj, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_size(icon_obj, XOS_USE_MENU_LIST_ITEM_IMAGE_WIDTH, XOS_USE_MENU_LIST_ITEM_IMAGE_HEIGHT);
        // 将图标对象添加到 label 对象前面
    }

    lv_obj_align(label_obj, LV_ALIGN_LEFT_MID, item->display_icon ? 40 : 0, 0);

    // 创建标签
    if (item->options && item->options[item->option_index]) {
        if (item->option_index >= item->option_count) {
            LV_LOG_ERROR("wrong data option_index %d, option_count %d", item->option_index, item->option_count);
            return;
        }
        opt_obj = lv_label_create(image_label_container);
        lv_label_set_text(opt_obj, get_text_by_id(item->options[item->option_index]));
        lv_obj_set_style_text_color(opt_obj, lv_color_hex(MENU_ITEM_FONT_COLOR), 0);
        lv_obj_set_style_text_font(opt_obj, XOS_USE_MENU_LIST_ITEM_FONT, LV_STATE_DEFAULT);
        lv_obj_align(opt_obj, LV_ALIGN_RIGHT_MID, -20, 0);
    }

    // 创建箭头
    image_obj = lv_label_create(image_label_container);
    lv_obj_set_style_text_color(image_obj, lv_color_hex(MENU_ITEM_FONT_COLOR), 0);
    lv_label_set_text(image_obj, LV_SYMBOL_RIGHT);
    lv_obj_align(image_obj, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_clear_flag(image_label_container, LV_OBJ_FLAG_SCROLLABLE);
}

static void menu_view_button_entry_event_callback(lv_event_t *e)
{
    char key = *((char *)lv_event_get_param(e));
    menu_info *menu = lv_obj_get_user_data(g_menu_view.parent);
    if (e->code == LV_EVENT_CLICKED) {
        menu_item *config = (menu_item *)lv_event_get_user_data(e);
        menu_param *cur_param = &menu->param;
        if (cur_param->callback)
            cur_param->callback(config->item_id, NULL, e);
    } else if (e->code == LV_EVENT_KEY && (key == LV_KEY_UP ||
        key == LV_KEY_DOWN ||
        key == LV_KEY_LEFT ||
        key == LV_KEY_RIGHT)) {
        lv_group_focus_obj(g_menu_view.panel);
        lv_event_send(g_menu_view.panel, e->code, e->param);
    }
}

static void menu_view_button_entry_create(lv_obj_t *parent, menu_item *item)
{
    lv_obj_t * image_label_container;
    lv_obj_t * image_obj;
    lv_obj_t * label_obj, *icon_obj;
    if (NULL == item) {
        return;
    }

    image_label_container = lv_btn_create(parent);
    // 创建一个容器,用于包含图像和标签
    lv_obj_set_size(image_label_container, XOS_USE_MENU_VIEW_WIDTH, XOS_USE_MENU_LIST_ITEM_HEIGHT);
    lv_obj_align(image_label_container, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(image_label_container, MENU_ITEM_BORDER_WIDTH, 0);
    lv_obj_set_style_border_color(image_label_container, lv_color_hex(MENU_ITEM_BORDER_COLOR), 0);
    lv_obj_set_style_radius(image_label_container, MENU_ITEM_BORDER_RADIUS, 0);
    lv_obj_set_style_bg_color(image_label_container, lv_color_hex(MENU_ITEM_BG_COLOR), 0);
    lv_obj_set_style_bg_color(image_label_container, lv_color_hex(MENU_ITEM_BORDER_CHECKED_COLOR), LV_STATE_CHECKED);
    lv_obj_set_user_data(image_label_container, item);

    lv_obj_add_event_cb(image_label_container, menu_view_button_entry_event_callback, LV_EVENT_KEY, item);
    lv_obj_add_event_cb(image_label_container, menu_view_button_entry_event_callback, LV_EVENT_CLICKED, item);

    // 创建标签
    label_obj = lv_label_create(image_label_container);
    lv_label_set_text(label_obj, get_text_by_id(item->display_name));

    lv_obj_set_style_text_color(label_obj, lv_color_hex(MENU_ITEM_FONT_COLOR), 0);
    lv_obj_set_style_text_font(label_obj, XOS_USE_MENU_LIST_ITEM_FONT, LV_STATE_DEFAULT);
    lv_obj_align(label_obj, LV_ALIGN_LEFT_MID, 0, 0);

    // 创建图标对象
    if (item->display_icon) {
        icon_obj = lv_img_create(image_label_container);
        lv_img_set_src(icon_obj, get_img_by_id(item->display_icon));
        lv_obj_align(icon_obj, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_size(icon_obj, XOS_USE_MENU_LIST_ITEM_IMAGE_WIDTH, XOS_USE_MENU_LIST_ITEM_IMAGE_HEIGHT);
        // 将图标对象添加到 label 对象前面
    }

    lv_obj_align(label_obj, LV_ALIGN_LEFT_MID, item->display_icon ? 40 : 0, 0);

    // 创建箭头
    image_obj = lv_label_create(image_label_container);
    lv_obj_set_style_text_color(image_obj, lv_color_hex(MENU_ITEM_FONT_COLOR), 0);
    lv_label_set_text(image_obj, LV_SYMBOL_RIGHT);
    lv_obj_align(image_obj, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_clear_flag(image_label_container, LV_OBJ_FLAG_SCROLLABLE);
}

static void menu_checkbox_cont_event_cb(lv_event_t *e)
{
    menu_item *config = (menu_item *)lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    char key = *((char *)lv_event_get_param(e));

    if (e->code == LV_EVENT_CLICKED) {
        obj = lv_obj_get_child(obj, config->display_icon ? 2 : 1);
        menu_checkbox_change_state(config, obj, e);
        menu_click_clear_checked();
    } else if (e->code == LV_EVENT_KEY && (key == LV_KEY_UP ||
        key == LV_KEY_DOWN ||
        key == LV_KEY_LEFT ||
        key == LV_KEY_RIGHT)) {
        lv_group_focus_obj(g_menu_view.panel);
        lv_event_send(g_menu_view.panel, e->code, e->param);
    }
}

static void menu_checkbox_event_cb(lv_event_t *e)
{
    menu_item *config = (menu_item *)lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    char key = *((char *)lv_event_get_param(e));

    if (e->code == LV_EVENT_CLICKED) {
        menu_checkbox_change_state(config, obj, e);
        menu_click_clear_checked();
    } else if (e->code == LV_EVENT_KEY && (key == LV_KEY_UP ||
        key == LV_KEY_DOWN ||
        key == LV_KEY_LEFT ||
        key == LV_KEY_RIGHT)) {
        lv_group_focus_obj(g_menu_view.panel);
        lv_event_send(g_menu_view.panel, e->code, e->param);
    }
}

static void menu_checkbox_change_state(menu_item *config, lv_obj_t *obj, lv_event_t *e)
{
    menu_info *menu = lv_obj_get_user_data(g_menu_view.parent);
    config->option_index = !config->option_index;
    menu_update_item(menu, config->item_id, config->option_index);
    menu_param *cur_param = &menu->param;
    if (cur_param->callback)
        cur_param->callback(config->item_id, config->option_index ? "ON":"OFF", e);
    if (config->option_index)
        lv_obj_add_state(obj, LV_STATE_CHECKED);
    else {
        lv_obj_clear_state(obj, LV_STATE_CHECKED);
    }
}

static void menu_view_checkbox_create(lv_obj_t *parent, menu_item *item) {
    lv_obj_t *icon_obj;

    lv_obj_t *cont = lv_btn_create(parent);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, XOS_USE_MENU_VIEW_WIDTH, XOS_USE_MENU_LIST_ITEM_HEIGHT);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(cont, MENU_ITEM_BORDER_RADIUS, 0);
    lv_obj_set_style_border_width(cont, MENU_ITEM_BORDER_WIDTH, 0);
    lv_obj_set_style_border_color(cont, lv_color_hex(MENU_ITEM_BORDER_COLOR), 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(MENU_ITEM_BG_COLOR), 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(MENU_ITEM_BORDER_CHECKED_COLOR), LV_STATE_CHECKED);
    lv_obj_set_user_data(cont, item);
    lv_obj_add_event_cb(cont, menu_checkbox_cont_event_cb, LV_EVENT_CLICKED, item);
    lv_obj_add_event_cb(cont, menu_checkbox_cont_event_cb, LV_EVENT_KEY, item);

    lv_obj_t *label = lv_label_create(cont);
    lv_label_set_text(label, get_text_by_id(item->display_name));

    lv_obj_set_style_text_color(label, lv_color_hex(MENU_ITEM_FONT_COLOR), 0);
    lv_obj_set_style_text_font(label, XOS_USE_MENU_LIST_ITEM_FONT, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

    // 创建图标对象
    if (item->display_icon) {
        icon_obj = lv_img_create(cont);
        lv_img_set_src(icon_obj, get_img_by_id(item->display_icon));
        lv_obj_align(icon_obj, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_size(icon_obj, XOS_USE_MENU_LIST_ITEM_IMAGE_WIDTH, XOS_USE_MENU_LIST_ITEM_IMAGE_HEIGHT);
        // 将图标对象添加到 label 对象前面
    }
    lv_obj_align(label, LV_ALIGN_LEFT_MID, item->display_icon ? 40 : 0, 0);

    lv_obj_t *switch_ctl = lv_switch_create(cont);
    lv_obj_align(switch_ctl, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(switch_ctl, menu_checkbox_event_cb, LV_EVENT_KEY, item);
    lv_obj_add_event_cb(switch_ctl, menu_checkbox_event_cb, LV_EVENT_CLICKED, item);

    lv_obj_clear_flag(switch_ctl, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_set_style_bg_opa(switch_ctl, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(switch_ctl, lv_color_hex(MENU_CHECKBOX_DEFAULT_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(switch_ctl, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(switch_ctl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(switch_ctl, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(switch_ctl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_set_style_bg_opa(switch_ctl, LV_OPA_COVER, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(switch_ctl, lv_color_hex(MENU_CHECKBOX_KNOB_COLOR),  LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir (switch_ctl, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(switch_ctl, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(switch_ctl, 100, LV_PART_KNOB | LV_STATE_DEFAULT);
    if (item->option_index)
        lv_obj_add_state(switch_ctl, LV_STATE_CHECKED);
    else {
        lv_obj_clear_state(switch_ctl, LV_STATE_CHECKED);
    }
}

static void menu_float_wnd_callback(lv_event_t *event)
{
    printf(">>>>>>>>>>>>>>>>>%s %d\n", __func__, __LINE__);
    if (LV_EVENT_CLICKED == lv_event_get_code(event) || LV_EVENT_KEY == lv_event_get_code(event)) {
        lv_obj_t *btn = lv_event_get_target(event);
        lv_obj_del(btn);
        lv_group_remove_obj(g_menu_view.panel);
        lv_obj_del(g_menu_view.parent);
        g_menu_view.parent = NULL;
        if (g_menu_view.exit_app_name)
            app_manager_exit(g_menu_view.exit_app_name);
        qua_los_msgpop_close();
    }
}

void menu_view_destroy(const char *exit_app_name)
{
    if (strcmp(exit_app_name, g_menu_view.exit_app_name) == 0) {
        if (lv_obj_is_valid(g_menu_view.back_button)) {
            lv_obj_del(g_menu_view.back_button);
        }
        lv_group_remove_obj(g_menu_view.panel);
        lv_obj_del(g_menu_view.parent);
        g_menu_view.parent = NULL;
        app_manager_exit(g_menu_view.exit_app_name);
    }
}

void menu_view_create(menu_info *menuconfig, const void *bg_img, const char *title, const char *exit_app_name)
{
    lv_obj_t *list, *parent, *back_button;
    int i;
    if (NULL == menuconfig) {
        LV_LOG_USER("invalid param\n");
        return;
    }
    qua_los_ft_font_init();
    lv_ability_theme_init();

    parent = lv_obj_create(lv_scr_act());
    lv_obj_set_size(parent, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_obj_set_user_data(parent, menuconfig);

    if (bg_img)
        lv_obj_set_style_bg_img_src(parent, bg_img, 0);

    list = menu_view_create_list(parent, title);
    if (NULL == list) {
        LV_LOG_USER("menu list view create fail\n");
        return;
    }

    for (i = 0; i < menuconfig->itemCount; i++) {
        if (0 == strcmp (menuconfig->items[i].type, "optionmenu") || 0 == strcmp (menuconfig->items[i].type, "optionentry")) {
            menu_view_optionmenu_create(list, &menuconfig->items[i]);
        } else if (0 == strcmp (menuconfig->items[i].type, "button")) {
            menu_view_button_entry_create(list, &menuconfig->items[i]);
        } else if (0 == strcmp (menuconfig->items[i].type, "checkbox")) {
            menu_view_checkbox_create(list, &menuconfig->items[i]);
        }
    }

#ifdef CONFIG_XOS_USE_BUTTOM_BACK
#ifdef CONFIG_PRODUCT_STUDYPILOT
    back_button = create_back_button(&icon_back_left, menu_float_wnd_callback);
    // lv_obj_align(back_button, LV_ALIGN_TOP_LEFT, 0, -5);
#else
    back_button = create_back_button(&icon_back, menu_float_wnd_callback);
#endif
    g_menu_view.back_button = back_button;
#endif

    g_menu_view.exit_app_name = exit_app_name;
    g_menu_view.parent = parent;
}

menu_view_t* menu_get_view(void) {
    return &g_menu_view;
}

void menu_res_init_mapping_table(void) {
    if(image_src_mapping_table_ptr == NULL)
        image_src_mapping_table_ptr = image_src_default_mapping_table;
    if(text_id_mapping_table_ptr == NULL)
        text_id_mapping_table_ptr = text_id_default_mapping_table;
}

void menu_res_set_img_mapping_table(image_src_mapping *ptr) {
    image_src_mapping_table_ptr = ptr;
}

void menu_res_set_text_mapping_table(text_id_mapping *ptr) {
    text_id_mapping_table_ptr = ptr;
}

