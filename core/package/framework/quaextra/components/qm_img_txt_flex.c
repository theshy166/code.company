#include "qm_img_txt_flex.h"
#include "qm_lv_obj.h"
#include "qua_font.h"
//
lv_obj_t *qm_img_txt_flex_init(lv_obj_t *p, int w, int h, 
    const void *img_normal, const void *img_selected, 
    int padding_top, int img_txt_space, int padding_bottom,
    const char *font_path,
    const char *label_name, int font_size, bool is_selected)
{
    lv_obj_t *cont = qm_lv_obj_create(p);
    if( cont == NULL){
        LV_LOG_USER("qm_setting_img_txt_init  failed");   
        return NULL;
    }
    lv_obj_set_size(cont, w, h);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_top(cont, padding_top, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_gap(cont, img_txt_space, LV_STATE_DEFAULT);

    lv_obj_set_style_border_width(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    //image
    lv_obj_t * imgbtn = lv_imagebutton_create(cont);
    // lv_obj_align(imgbtn, LV_ALIGN_TOP_MID, 0, padding_top);
    // lv_obj_set_style_pad_all(imgbtn, padding_top, LV_STATE_DEFAULT);
    // lv_obj_set_style_margin_top(imgbtn, padding_top, LV_STATE_DEFAULT);
    // lv_img_set_src(imgbtn, is_selected ? img_selected : img_normal);
    lv_imagebutton_set_src(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, img_selected, NULL);
    lv_imagebutton_set_src(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, img_normal, NULL);
    lv_imagebutton_set_state(imgbtn, is_selected ? LV_IMAGEBUTTON_STATE_PRESSED : LV_IMAGEBUTTON_STATE_RELEASED);
    //debug
    // draw_border_for_debug(imgbtn);

    //label
    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, label_name);
    // draw_border_for_debug(label);
    // lv_obj_set_size(label, w, 20);
    // lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, padding_bottom);
    // lv_obj_set_style_margin_bottom(label, padding_bottom, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(label, padding_bottom, LV_STATE_DEFAULT);
    //get font
    lv_font_t *font = NULL;
    qua_xos_ft_init_with_size_path(&font, font_size, font_path, 0);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // 设置文本居中对齐
    // lv_obj_set_style_text_font(label, g_aichat_objs.font, 0);  // Set font size //&lv_font_montserrat_20, 0);//
    // lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 30);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(imgbtn, LV_OBJ_FLAG_CLICKABLE);
    cont->user_data = imgbtn;
    imgbtn->user_data = label;
    return cont;
}

void qm_img_txt_flex_update(lv_obj_t *cont, bool is_selected)
{
    lv_obj_t *imgbtn = cont->user_data;
    if (imgbtn != NULL) {
        // lv_img_set_src(imgbtn, is_selected ? img_selected : img_normal);
        lv_imagebutton_set_state(imgbtn, is_selected ? LV_IMAGEBUTTON_STATE_PRESSED : LV_IMAGEBUTTON_STATE_RELEASED);
    }
}
