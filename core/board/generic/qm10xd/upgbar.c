
#include "lvgl/lvgl.h"

#include <stdio.h>



#define XOS_LOGU(fmt, ...)    LV_LOG_USER(fmt, ##__VA_ARGS__)
#define XOS_LOGI(fmt, ...)    LV_LOG_USER(fmt, ##__VA_ARGS__)
#define XOS_LOGE(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)

#define __UPG_VIEW_W  (LV_USE_HOR_SIZE *2 / 3)
#define __UPG_VIEW_H  (LV_USE_VER_SIZE / 3)

#define MIN_PROGRESS 0
#define MAX_PROGRESS 100

typedef struct {
    lv_obj_t *parent;
    lv_obj_t *cont_bar;
    lv_obj_t *bar;
    lv_obj_t *percentage_label;
    lv_obj_t *percentage_label_unit;

    lv_timer_t * upg_timer;
    int percent;

    lv_obj_t * ui_page;
    lv_obj_t * ui_exit_btn;

    bool update;
} xos_upg_view_t;

static xos_upg_view_t s_upg_view = {0};

static int set_upgrade_progress(void)
{
    printf("Setting upgrade progress to %d\n",  s_upg_view.percent++);
    return s_upg_view.percent++;
}

static void __timer_callback(lv_timer_t * timer)
{
//    XOS_LOGI("e\n");
    int _progress = set_upgrade_progress();

    if (_progress > 100) {
        LV_LOG_WARN("_progress=%d", _progress);
        _progress = 100;
    }

    if (_progress != s_upg_view.percent) {
        char buffer[4];
        s_upg_view.percent = _progress;

        if (s_upg_view.bar) {
            lv_bar_set_value(s_upg_view.bar, _progress, LV_ANIM_OFF);
        }

        if (s_upg_view.percentage_label) {
            snprintf(buffer, sizeof(buffer), "%d", _progress);
            lv_label_set_text(s_upg_view.percentage_label, buffer);
            lv_obj_align_to(s_upg_view.percentage_label_unit, s_upg_view.percentage_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        }

        if(timer && _progress == MAX_PROGRESS) {
            lv_timer_pause(timer);
            lv_timer_delete(timer);
            s_upg_view.upg_timer = NULL;
        }
    }
 //   XOS_LOGI("x\n");
}

lv_obj_t * upgrade_progress_view_create(lv_obj_t * parent)
{
    XOS_LOGI("e\n");
    lv_obj_t *ui_parent = parent?parent:lv_screen_active();
    lv_obj_t *ui_page = lv_obj_create(ui_parent);
    lv_obj_remove_style_all(ui_page);
    lv_obj_align(ui_page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui_page, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(ui_page, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_page, 255, LV_PART_MAIN);

    //create inner container
    lv_obj_t * inner_panel = lv_obj_create(ui_page);
    lv_obj_remove_style_all(inner_panel);
    lv_obj_set_size(inner_panel, __UPG_VIEW_W, __UPG_VIEW_H);
    //lv_obj_set_style_bg_img_src(inner_panel, &xniu_bg_battery_grey, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(inner_panel, 0, 0);
    lv_obj_set_style_bg_color(inner_panel, lv_color_make(0x80,0x80,0x80), 0);
    //lv_obj_set_style_radius(inner_panel, 15, 0);
    lv_obj_align(inner_panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scroll_dir(inner_panel, LV_DIR_NONE);
    lv_obj_remove_flag(inner_panel, LV_OBJ_FLAG_CLICKABLE);
    //draw_border_for_debug(inner_panel);
    //lv_obj_update_layout(inner_panel);
    lv_obj_set_style_radius(inner_panel, 15, 0);
    lv_obj_set_style_border_width(inner_panel, 0, 0);

    // Create and configure the bar
    lv_obj_t * bar = lv_bar_create(inner_panel);
    lv_obj_set_size(bar, __UPG_VIEW_W * 9/10, 28);
    lv_bar_set_range(bar, MIN_PROGRESS, MAX_PROGRESS);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);

    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_clear_flag(bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(bar, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_bg_opa(bar, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar, 255, LV_PART_INDICATOR);
   // draw_border_for_debug(bar);

    // Create and configure the percentage label
    lv_obj_t * downloading_label = lv_label_create(inner_panel);
    lv_label_set_text(downloading_label, "Downloading:");
    lv_obj_set_style_text_color(downloading_label, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(downloading_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(downloading_label, bar, LV_ALIGN_OUT_TOP_MID, 0, -30);
    //draw_border_for_debug(downloading_label);
    // Create and configure the percentage label
    lv_obj_t * percentage_label = lv_label_create(inner_panel);

    char buffer[4];
    snprintf(buffer, sizeof(buffer), "%d", s_upg_view.percent);
    lv_label_set_text(percentage_label, buffer);
    lv_obj_set_style_text_color(percentage_label, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(percentage_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(percentage_label, downloading_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    //draw_border_for_debug(percentage_label);

    lv_obj_t * percentage_label_unit = lv_label_create(inner_panel);
    lv_label_set_text(percentage_label_unit, "%");
    lv_obj_set_style_text_color(percentage_label_unit, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(percentage_label_unit, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(percentage_label_unit, percentage_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    //lv_obj_set_style_text_decor(km_label_unit, LV_TEXT_DECOR_ITALIC, 0);
    //draw_border_for_debug(percentage_label_unit);

    // Create and configure the percentage label
    lv_obj_t * attention_label = lv_label_create(inner_panel);
    lv_label_set_text(attention_label, "Waiting for fully update to complete");
    lv_obj_set_style_text_color(attention_label, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(attention_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(attention_label, bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    //draw_border_for_debug(attention_label);

    lv_timer_t *_timer = lv_timer_create((lv_timer_cb_t)__timer_callback, 1000, NULL);

    s_upg_view.parent = ui_parent;
    s_upg_view.cont_bar = inner_panel;
    s_upg_view.bar = bar;
    s_upg_view.percentage_label = percentage_label;
    s_upg_view.percentage_label_unit = percentage_label_unit;
    s_upg_view.ui_page = ui_page;
    s_upg_view.upg_timer = _timer;

    XOS_LOGI("x\n");
    return ui_page;
}

static void __ellipsis_timer_callback(lv_timer_t * timer)
{
    char buffer[20] = {0};
    const char *text = lv_label_get_text(s_upg_view.percentage_label);

    if (strlen(text) >= 12) {
        strcpy(buffer, ". ");
    } else {
        snprintf(buffer, sizeof(buffer), "%s. ", text);
    }
    lv_label_set_text(s_upg_view.percentage_label, buffer);
}

lv_obj_t * upgrade_ellipsis_view_create(lv_obj_t * parent)
{
    XOS_LOGI("e\n");
    lv_obj_t *ui_parent = parent?parent:lv_screen_active();
    lv_obj_t *ui_page = lv_obj_create(ui_parent);
    lv_obj_remove_style_all(ui_page);
    lv_obj_align(ui_page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui_page, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(ui_page, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_page, 255, LV_PART_MAIN);

    //create inner container
    lv_obj_t * inner_panel = lv_obj_create(ui_page);
    lv_obj_remove_style_all(inner_panel);
    lv_obj_set_size(inner_panel, __UPG_VIEW_W, __UPG_VIEW_H);
    //lv_obj_set_style_bg_img_src(inner_panel, &xniu_bg_battery_grey, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(inner_panel, 0, 0);
    lv_obj_set_style_bg_color(inner_panel, lv_color_make(0x80,0x80,0x80), 0);
    //lv_obj_set_style_radius(inner_panel, 15, 0);
    lv_obj_align(inner_panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scroll_dir(inner_panel, LV_DIR_NONE);
    lv_obj_remove_flag(inner_panel, LV_OBJ_FLAG_CLICKABLE);
    //draw_border_for_debug(inner_panel);
    //lv_obj_update_layout(inner_panel);
    lv_obj_set_style_radius(inner_panel, 15, 0);
    lv_obj_set_style_border_width(inner_panel, 0, 0);

    // Create and configure the bar
    lv_obj_t * bar = lv_bar_create(inner_panel);
    lv_obj_set_size(bar, __UPG_VIEW_W * 9/10, 28);
    lv_bar_set_range(bar, MIN_PROGRESS, MAX_PROGRESS);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);

    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_clear_flag(bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(bar, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_bg_opa(bar, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar, 255, LV_PART_INDICATOR);
    lv_obj_add_flag(bar, LV_OBJ_FLAG_HIDDEN);
   // draw_border_for_debug(bar);

    // Create and configure the percentage label
    lv_obj_t * downloading_label = lv_label_create(inner_panel);
    lv_label_set_text(downloading_label, "Downloading:");
    lv_obj_set_style_text_color(downloading_label, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(downloading_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(downloading_label, bar, LV_ALIGN_OUT_TOP_MID, 0, -30);
    //draw_border_for_debug(downloading_label);
    // Create and configure the percentage label
    lv_obj_t * percentage_label = lv_label_create(inner_panel);

    char buffer[4];
    snprintf(buffer, sizeof(buffer), "%s", ". ");
    lv_label_set_text(percentage_label, buffer);
    lv_obj_set_style_text_color(percentage_label, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(percentage_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(percentage_label, downloading_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    //draw_border_for_debug(percentage_label);

    lv_obj_t * percentage_label_unit = lv_label_create(inner_panel);
    lv_label_set_text(percentage_label_unit, "%");
    lv_obj_set_style_text_color(percentage_label_unit, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(percentage_label_unit, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(percentage_label_unit, percentage_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    //lv_obj_set_style_text_decor(km_label_unit, LV_TEXT_DECOR_ITALIC, 0);
    lv_obj_add_flag(percentage_label_unit, LV_OBJ_FLAG_HIDDEN);
    //draw_border_for_debug(percentage_label_unit);

    // Create and configure the percentage label
    lv_obj_t * attention_label = lv_label_create(inner_panel);
    lv_label_set_text(attention_label, "Waiting for Dashboard Update to complete.");
    lv_obj_set_style_text_color(attention_label, lv_color_make(0xFF,0xFF,0xFF), 0);
    lv_obj_set_style_text_font(attention_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align_to(attention_label, bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    //draw_border_for_debug(attention_label);

    lv_timer_t *_timer = lv_timer_create((lv_timer_cb_t)__ellipsis_timer_callback, 5000, NULL);

    s_upg_view.parent = ui_parent;
    s_upg_view.cont_bar = inner_panel;
    s_upg_view.bar = bar;
    s_upg_view.percentage_label = percentage_label;
    s_upg_view.percentage_label_unit = percentage_label_unit;
    s_upg_view.ui_page = ui_page;
    s_upg_view.upg_timer = _timer;

    XOS_LOGI("x\n");
    return ui_page;
}
