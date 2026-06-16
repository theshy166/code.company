#include "qm_image_text.h"
#include "qm_lv_obj.h"
#include "qua_font.h"

extern lv_font_t *ft_font_ttf_20;
//
lv_obj_t *qm_image_text_init(lv_obj_t *p, int w, int h, const void *img_normal, const void *img_selected, const char *label_name, bool is_selected)
{
    lv_obj_t *cont = qm_lv_obj_create(p);
    lv_obj_set_size(cont, w, h);
    lv_obj_center(cont);
    // lv_obj_set_style_bg_color(cont, lv_color_hex(0xff00ff), 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0);

    //imgbtn
    lv_obj_t * imgbtn = lv_img_create(cont);
    // lv_obj_set_size(imgbtn, w, w);
    lv_obj_align(imgbtn, LV_ALIGN_TOP_LEFT, 0, 0);
    // lv_obj_set_style_bg_opa(imgbtn, LV_OPA_TRANSP, 0);
    // lv_obj_set_style_bg_color(cont, lv_color_hex(0xff00ff), 0);

    // Set the source for the different states
    // char path[128] = {0};
    // lv_snprintf(path, 128, k_path_home_blank, img_name, "normal");
    lv_img_set_src(imgbtn, is_selected ? img_selected : img_normal);
    // memset(path, 0, 128);
    // lv_snprintf(path, 128, k_path_home_blank, img_name, "selected");
    // lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, img_selected, NULL); // Selected state icon

    //label
    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, label_name);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_set_style_text_font(label, ft_font_ttf_20, 0);  // Set font size //&lv_font_montserrat_20, 0);//
    lv_obj_align_to(label, imgbtn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    if (is_selected) {
        lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(imgbtn, LV_OBJ_FLAG_CLICKABLE);

    cont->user_data = imgbtn;
    imgbtn->user_data = label;
    return cont;
}

void qm_image_text_update(lv_obj_t *cont, const void *img_normal, const void *img_selected, bool is_selected)
{
    lv_obj_t *imgbtn = cont->user_data;
    if (imgbtn != NULL) {
        lv_img_set_src(imgbtn, is_selected ? img_selected : img_normal);

        lv_obj_t *label = imgbtn->user_data;
        if (label != NULL) {
            if (is_selected) {
                lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
            }
            else {
                lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

lv_obj_t *qm_image_text_noselect(lv_obj_t *p, int w, int h, const void *img, char *label_name,
            int padding_top, int img_txt_space, int padding_bottom,const char *font_path,int font_size)
{
    lv_obj_t *cont = lv_obj_create(p);

    if( cont == NULL){
            LV_LOG_USER("qm_ai_image_text_init  failed");   
        return NULL;
    }
    lv_font_t *ch_font_small = NULL;
    qua_xos_ft_init_with_size_path(&ch_font_small, font_size, font_path, 0);

    lv_obj_set_size(cont, w, h);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_top(cont, padding_top, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_gap(cont, img_txt_space, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(cont, 0, 0);

    lv_obj_t * imgbtn = lv_img_create(cont);
    lv_obj_align(imgbtn, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_size(imgbtn, 24, 24);
    lv_img_set_src(imgbtn, img);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, label_name);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(label, ch_font_small, LV_PART_MAIN);  // Set font size //&lv_font_montserrat_20, 0);//
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // 设置文本居中对齐
    lv_obj_set_style_pad_all(label, 0, 0);
    lv_obj_set_size(label, w-2, LV_SIZE_CONTENT);
    //lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 26);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(imgbtn, LV_OBJ_FLAG_CLICKABLE);
    cont->user_data = imgbtn;
    imgbtn->user_data = label;
    //draw_border_for_debug(cont);
    return cont;
}


lv_obj_t *qm_image_text_noselect_row(lv_obj_t *p, int w, int h, const void *img, char *label_name,
            int padding_top, int img_txt_space, int padding_bottom, const char *font_path, int font_size)
{
    lv_obj_t *cont = lv_obj_create(p);

    if (cont == NULL) {
        LV_LOG_USER("qm_ai_image_text_init failed");
        return NULL;
    }

    lv_font_t *ch_font_small = NULL;
    qua_xos_ft_init_with_size_path(&ch_font_small, font_size, font_path, 0);
    
    lv_obj_set_size(cont, w, h);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);  // 修改为横向布局
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_top(cont, padding_top, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cont, padding_bottom, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, img_txt_space, LV_STATE_DEFAULT);  // 设置行之间的间距
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *imgbtn = lv_img_create(cont);
    lv_obj_set_size(imgbtn, 24, h - padding_top - padding_bottom);  // 设置图片高度为容器高度减去上下内边距
    lv_img_set_src(imgbtn, img);

    lv_obj_t *label = lv_label_create(cont);
    lv_label_set_text(label, label_name);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(label, ch_font_small, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);  // 设置文本左对齐
    lv_obj_set_width(label, w - 2 - 24 - img_txt_space);  // 设置标签宽度为剩余宽度
    lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(imgbtn, LV_OBJ_FLAG_CLICKABLE);

    // 纵向居中
    lv_coord_t cont_h = lv_obj_get_height(cont);
    lv_coord_t img_h = lv_obj_get_height(imgbtn);
    lv_coord_t label_h = lv_obj_get_height(label);
    lv_coord_t y_offset = (cont_h - img_h - label_h) / 2;
    lv_obj_align(imgbtn, LV_ALIGN_TOP_LEFT, 0, y_offset);
    lv_obj_align(label, LV_ALIGN_TOP_RIGHT, -img_txt_space, y_offset);

    // 清除之前设置的自动对齐
    //lv_obj_clear_flag(cont, LV_OBJ_FLAG_AUTO_REALIGN);
    //draw_border_for_debug(cont);
    return cont;
}
