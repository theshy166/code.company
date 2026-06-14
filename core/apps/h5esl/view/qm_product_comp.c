#include "qm_product_comp.h"
#include "qm_image_text.h"
#include "../conf/conf.h"
#include "qm_2_label.h"
#include "qm_3_label.h"
#include <stdio.h>


#if XOS_USE_APP_NATIVE_ESL != 0
extern lv_font_t *ft_font_ttf_40;
extern lv_font_t *ft_font_ttf_30;
extern lv_font_t *ft_font_ttf_20;


product_comp_t *product_comp_create(lv_obj_t *parent, int w, int h)
{
    int kPriceLabelWidth = w * 2/3;

    product_comp_t *shelf_comp = lv_mem_alloc(sizeof(product_comp_t));
    if (NULL == shelf_comp) {
        LV_LOG_ERROR("OOM.");
        return NULL;
    }

    // Create the battery card container
    lv_obj_t *shelf_panel = lv_obj_create(parent);
    lv_obj_set_size(shelf_panel, w, h);
    // lv_obj_align(battery_card_container, align, x_offset, y_offset);
    lv_obj_set_style_bg_opa(shelf_panel, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(shelf_panel, lv_color_white(), 0);
    lv_obj_set_style_radius(shelf_panel, 20, 0);
    lv_obj_set_style_border_width(shelf_panel, 0, 0);
    lv_obj_set_scroll_dir(shelf_panel, LV_DIR_NONE);
    lv_obj_set_style_pad_left(shelf_panel, 0 , 0);
    lv_obj_set_style_pad_right(shelf_panel, 0 , 0);
    lv_obj_set_style_pad_top(shelf_panel, 0 , 0);
    lv_obj_set_style_pad_bottom(shelf_panel, 0 , 0);

    shelf_comp->parent = shelf_panel;
    // extern void draw_border_for_debug(lv_obj_t *obj);
    // draw_border_for_debug(battery_card_container);

    // 1. bar code
    static int code_number = 1007;
    code_number++;
    char code_number_str[10] = {0};
    snprintf(code_number_str, 10, "%d", code_number);
    lv_obj_t *img_bar_code = qm_image_text_init(shelf_panel, 64, 62, k_path_img_bar_code, k_path_img_bar_code, code_number_str, true);
    lv_obj_align(img_bar_code, LV_ALIGN_TOP_LEFT, 20, 10);
    // draw_border_for_debug(img_bar_code);

    //2. big name
    lv_obj_t *fruit_name = lv_label_create(shelf_panel);
    shelf_comp->title = fruit_name;
    lv_label_set_text(fruit_name, "苹果");
    lv_obj_set_style_text_font(fruit_name, ft_font_ttf_40, 0);
    lv_obj_align_to(fruit_name, img_bar_code, LV_ALIGN_OUT_BOTTOM_RIGHT, 50, 10);

    //3. price_label
    qm_3_label_create(shelf_panel, kPriceLabelWidth, 40, &(shelf_comp->three_label1), "售价：", "25.00", "元/500g", lv_color_black());
    lv_obj_align(shelf_comp->three_label1.parent, LV_ALIGN_CENTER, -10, 20);
    //3.2 price 2 label
    qm_3_label_create(shelf_panel, kPriceLabelWidth, 40, &(shelf_comp->three_label2), "特价：", "20.00", "元/500g", lv_color_hex(0xffff0000));
    lv_obj_align_to(shelf_comp->three_label2.parent, shelf_comp->three_label1.parent, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    //4. product from
    lv_obj_t *product_address = lv_label_create(shelf_panel);
    lv_obj_set_size(product_address, 100, 36);
    lv_label_set_text(product_address, "产地: 中国");
    lv_obj_align(product_address, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_text_font(product_address, ft_font_ttf_20, 0);
    shelf_comp->product_address = product_address;

    //
    lv_obj_t *product_level = lv_label_create(shelf_panel);
    lv_obj_set_size(product_level, 100, 36);
    lv_label_set_text(product_level, "等级: 一");
    lv_obj_set_style_text_font(product_level, ft_font_ttf_20, 0);
    lv_obj_align_to(product_level, product_address, LV_ALIGN_OUT_TOP_LEFT, 0, 10);
    shelf_comp->product_level = product_level;

    //5. product_image
    lv_obj_t *img = lv_img_create(shelf_panel);
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -10, -20);
    lv_obj_set_size(img, 180, 180);
    shelf_comp->image = img;

    return shelf_comp;
}

//
void product_comp_destroy(product_comp_t *panel)
{
    if (NULL != panel) {
        lv_mem_free(panel);
    }
}

void update_product_comp_image(product_comp_t *shelf_panel, const char *image_path)
{
    LV_LOG_USER("img_path=%s", image_path);
    lv_img_set_src(shelf_panel->image, image_path);
    // LV_LOG_USER("image=%p", shelf_panel->image);
}

void update_product_comp_title(product_comp_t *shelf_panel, const char *title)
{
    lv_label_set_text(shelf_panel->title, title);
}

void update_product_comp_price(three_label_comp_t *three_label, float price)
{
    LV_LOG_USER("price=%.2f", (double)price);
    char value[10] = {0};
    snprintf(value, 10, "%.2f", (double)price);
    lv_label_set_text(three_label->value, value);
}

#endif //XOS_USE_APP_NATIVE_ESL
