#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include "page_esl_html.h"
#include "../conf/conf.h"
#include "../utils/pic_utils.h"
#include "page_esl.h"

static _handle_price_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,char **other_3_arr,int show_count){
    struct esl_product_item_key_t *user;
    int index = 0;
    char *price = child->extension.html.text;
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
        if (strcmp(child->extension.bind, "$sale_price") == 0) {
            price = product_info->sale_price;
        } else if (strcmp(child->extension.bind, "$vip_price") == 0) {
            price = product_info->vip_price;
        } else if (strcmp(child->extension.bind, "$line_price") == 0) {
            price = child->extension.html.text;
        } else if (strcmp(child->extension.bind, "$cu_price") == 0) {
            price = product_info->sale_price;
        } else if (strcmp(child->extension.bind, "$price") == 0) {
            price = product_info->price;
        }
    }
    if (is_string_empty(price)) {
        price = "0.00";
    }
    char *dot_pos = strchr(price, '.');
    if (dot_pos) {
        lv_obj_t *price_container = lv_obj_create(bg);
        lv_obj_remove_style_all(price_container);
        lv_obj_set_size(price_container, child->w, child->h);
        lv_obj_set_pos(price_container, child->x, child->y);
        lv_obj_set_style_pad_all(price_container, 0, 0);
        //draw_border_for_debug(price_container);

        // Create a new label for the integer part
        lv_obj_t *int_label = lv_label_create(price_container);
        char int_part[50];
        strncpy(int_part, price, dot_pos - price + 1); // Include the dot
        int_part[dot_pos - price + 1] = '\0';

        //字体
        char *ttf_path = k_path_ttf;
        if(child->extension.html.fontFamily == NULL){
            ttf_path = k_path_ttf;
        }else if(strcmp(child->extension.html.fontFamily, "Impact") == 0){
            ttf_path = k_path_impact_ttf;
        }
        LV_LOG_USER("ttf_path: %s ,price : %s\n", ttf_path, price);
        lv_font_t *int_font = NULL;
        qua_xos_ft_init_with_size_path(&int_font, child->extension.html.intFontSize, ttf_path, 0);
        lv_font_t *dec_font = NULL;
        qua_xos_ft_init_with_size_path(&dec_font, child->extension.html.floatFontSize, ttf_path, 0);

        lv_label_set_text(int_label, int_part);
        lv_obj_set_style_text_font(int_label, int_font, 0); // Set font for integer part

        // Create a new label for the decimal part
        lv_obj_t *dec_label = lv_label_create(price_container);
        lv_label_set_text(dec_label, dot_pos + 1); // Exclude the dot
        lv_obj_set_style_text_font(dec_label, dec_font, 0); // Set smaller font for decimal part

        if (!is_string_empty(child->extension.html.color)) {
            lv_color_t color = lv_color_hex(hex_to_uint32(child->extension.html.color));
            lv_obj_set_style_text_color(int_label, color, 0);
            lv_obj_set_style_text_color(dec_label, color, 0);
        }
        if (!is_string_empty(child->extension.html.textAlign)) {
            int container_width = child->w;
            int int_label_width = lv_txt_get_width(int_part, strlen(int_part), int_font, 0);
            int dec_label_width = lv_txt_get_width(dot_pos + 1, strlen(dot_pos + 1), dec_font, 0);
            int x_offset = 0;
            if (strcmp(child->extension.html.textAlign, "left") == 0) {
                lv_obj_set_style_text_align(int_label, LV_TEXT_ALIGN_LEFT, 0);
                lv_obj_set_style_text_align(dec_label, LV_TEXT_ALIGN_LEFT, 0);
            } else if (strcmp(child->extension.html.textAlign, "center") == 0) {
                lv_obj_set_style_text_align(int_label, LV_TEXT_ALIGN_CENTER, 0);
                lv_obj_set_style_text_align(dec_label, LV_TEXT_ALIGN_CENTER, 0);
                x_offset = (container_width - int_label_width - dec_label_width) / 2;

            }else if (strcmp(child->extension.html.textAlign, "right") == 0) {
                lv_obj_set_style_text_align(int_label, LV_TEXT_ALIGN_RIGHT, 0);
                lv_obj_set_style_text_align(dec_label, LV_TEXT_ALIGN_RIGHT, 0);
                x_offset = container_width - int_label_width - dec_label_width;
            }
            lv_obj_align_to(int_label, price_container, LV_ALIGN_LEFT_MID, x_offset, 0);
            if (strcmp(child->extension.html.numFloatStyle, "ct") == 0) {
                lv_obj_align_to(dec_label, int_label, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
            } else if (strcmp(child->extension.html.numFloatStyle, "cc") == 0) {
                lv_obj_align_to(dec_label, int_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
            } else {
                lv_obj_align_to(dec_label, int_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
            }
        }
        if (!is_string_empty(child->extension.html.bgColor)) {
            lv_color_t bgColor = lv_color_hex(hex_to_uint32(child->extension.html.bgColor));
            lv_obj_set_style_bg_color(int_label, bgColor, 0);
            lv_obj_set_style_bg_color(dec_label, bgColor, 0);
        }

        
        if (!is_string_empty(child->extension.html.textDecoration)) {
            lv_text_decor_t decor;
            if (strcmp(child->extension.html.textDecoration, "underline") == 0) {
                decor = LV_TEXT_DECOR_UNDERLINE;
            } else if (strcmp(child->extension.html.textDecoration, "line-through") == 0) {
                decor = LV_TEXT_DECOR_STRIKETHROUGH;
            } else {
                decor = LV_TEXT_DECOR_NONE;
            }
            lv_obj_set_style_text_decor(int_label, decor, 0);
            lv_obj_set_style_text_decor(dec_label, decor, 0);
        }
    }
}


void handle_html_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,char **other_3_arr,int show_count){
    if(strcmp(child->extension.ex_type, "price") == 0){
        _handle_price_extension(bg, child,device_setting, product_info_list, map,count,other_3_arr,show_count);
    }
}
#endif