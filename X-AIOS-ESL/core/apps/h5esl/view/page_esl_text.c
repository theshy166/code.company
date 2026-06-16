#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include "page_esl_text.h"
#include "../conf/conf.h"
#include "../utils/pic_utils.h"
#include "page_esl.h"

static _handle_text_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int srceen_index,char **other_3_arr,int show_count){
    struct esl_product_item_key_t *user;
    int index = 0;
    lv_obj_t *label = lv_label_create(bg);
    if (!is_string_empty(child->extension.bind)) {
        user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count);
        set_label_text(label, child->extension.bind, product_info, is_string_empty(child->desc.text) ? "" : child->desc.text,device_setting,srceen_index);
    } else {
        lv_label_set_text(label, is_string_empty(child->desc.text) ? "" : child->desc.text);
    }
    lv_obj_set_size(label, child->w, child->h);
    lv_obj_set_pos(label, child->x, child->y);

    // Set additional styles like color, font size, etc.
    if (!is_string_empty(child->desc.color)) {
        lv_color_t color = lv_color_hex(hex_to_uint32(child->desc.color));
        lv_obj_set_style_text_color(label, color, 0);
    }
    if (!is_string_empty(child->desc.fontSize)) {
        /* lv_font_t* font = NULL;
        int font_size = atoi(font_str);
        qm_xos_font_with_path_size(&font, font_size, "path、to ttf", 0 );*/
        lv_font_t *int_font = NULL;
        int font_size = atoi(child->desc.fontSize);
        //字体
        char *ttf_path = k_path_ttf;
        if(child->desc.fontFamily == NULL){
            ttf_path = k_path_ttf;
        }else if(strcmp(child->desc.fontFamily, "Impact") == 0){
            ttf_path = k_path_impact_ttf;
        }
        qua_xos_ft_init_with_size_path(&int_font, font_size, ttf_path, 0);
        lv_obj_set_style_text_font(label, int_font, 0);
    }
    /* if (!is_string_empty(child->desc.fontWeight) && strcmp(child->desc.fontWeight, "bold") == 0) {
        lv_obj_set_style_text_font(label, ft_font_ttf_30, 0);
    } */
    if (!is_string_empty(child->desc.textAlign)) {
        if (strcmp(child->desc.textAlign, "left") == 0) {
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);
        } else if (strcmp(child->desc.textAlign, "center") == 0) {
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        } else if (strcmp(child->desc.textAlign, "right") == 0) {
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
        }
    }
    if (!is_string_empty(child->desc.bgColor)) {
        lv_color_t bgColor = lv_color_hex(hex_to_uint32(child->desc.bgColor));
        lv_obj_set_style_bg_color(label, bgColor, 0);
    }
    if (child->desc.borderRadius > 0) {
        lv_obj_set_style_radius(label, child->desc.borderRadius, 0);
    }
    if (child->desc.borderWidth > 0) {
        lv_obj_set_style_border_width(label, child->desc.borderWidth, 0);
    }
}

void handle_text_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int index,char **other_3_arr,int show_count){
    if(strcmp(child->extension.ex_type, "text") == 0){
        _handle_text_extension(bg, child, device_setting,product_info_list, map, count, index, other_3_arr, show_count);
    }else if(strcmp(child->extension.ex_type, "button") == 0){
        //_handle_button_extension(bg, child, product_info_list, map);
    }
}
#endif