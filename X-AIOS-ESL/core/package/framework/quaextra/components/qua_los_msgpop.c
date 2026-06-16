#include <stdio.h>
#include "qua_font.h"
#include "qua_los_msgpop.h"

extern lv_font_t *ft_font_ttf_20;

#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
#define k_path_ttf "H:./out/simulator/xos/res/motox/res/ttf/SourceHanSansCN-Bold.ttf"
#else
#define k_path_ttf "H:/res/ttf/SourceHanSansCN-Bold.ttf"
#endif

lv_obj_t *g_popup = NULL;

static void close_popup_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *popup = lv_event_get_user_data(e);

    if (LV_EVENT_KEY == code) {
        char c = *((char *)lv_event_get_param(e));
        if (LV_KEY_ESC == c) {
            lv_obj_del(popup);
            g_popup = NULL;
        }
    }
    else if (LV_EVENT_CLICKED == code) {
        printf("close popup window\n");
        lv_obj_del(popup);
        g_popup = NULL;
    }
}

void qua_los_msgpop_close(void)
{
    if (g_popup) {
        lv_obj_del(g_popup);
        g_popup = NULL;
    }
}

void qua_los_msgpop(char *title, char *content, lv_event_cb_t ok_btn_cb, void *ok_btn_data, bool has_close_btn)
{
    int x_offset = 0;

    lv_font_t *font_20 = NULL;

    qua_xos_ft_init_with_size_path(&font_20, 20, k_path_ttf, 0);

    if (NULL == ok_btn_cb && ok_btn_data != NULL) {
        LV_LOG_ERROR("ok_btn_cb should be set if got ok_btn_data\n");
        return;
    }
    if (g_popup != NULL) {
        lv_obj_del(g_popup);
        g_popup = NULL;
    }

    //lv_obj_t *popup = lv_obj_create(lv_scr_act());
    lv_obj_t *popup = lv_obj_create(lv_layer_sys());
    g_popup = popup;

    lv_obj_set_size(popup, XOS_USE_MSGPOP_WIDTH, LV_USE_VER_SIZE);
    lv_obj_center(popup);
    lv_obj_set_style_bg_color(popup, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_bg_opa(popup, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(popup, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(popup, 0, 0);

    lv_obj_t *panel = lv_obj_create(popup);
    lv_obj_set_size(panel, XOS_USE_MSGPOP_PANEL_WIDTH, XOS_USE_MSGPOP_PANEL_HEIGHT);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(panel, close_popup_event_cb, LV_EVENT_CLICKED, popup);

    #ifdef CONFIG_XOS_USE_PHYSICAL_KEY
    lv_group_add_obj(lv_group_get_default(), panel);
    lv_group_focus_obj(panel);
    lv_obj_add_event_cb(panel, close_popup_event_cb, LV_EVENT_KEY, popup);
    #endif

    lv_obj_t *title_label = lv_label_create(panel);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_font(title_label, font_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(title_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *content_label = lv_label_create(panel);
    lv_label_set_text(content_label, content);
    lv_obj_set_style_text_font(content_label, font_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(content_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(content_label, LV_ALIGN_TOP_MID, 0, 40);

    if (ok_btn_cb != NULL && has_close_btn) {
        x_offset = 30;  // if there are two buttons, both should be adjusted with an offset
    }

    if (ok_btn_cb) {
        lv_obj_t *ok_btn = lv_btn_create(panel);
        lv_obj_set_size(ok_btn, XOS_USE_MSGPOP_BUTTON_WIDTH, XOS_USE_MSGPOP_BUTTON_HEIGHT);
        lv_obj_align(ok_btn, LV_ALIGN_BOTTOM_MID, -x_offset, 0);
        lv_obj_add_flag(ok_btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(ok_btn, ok_btn_cb, LV_EVENT_CLICKED, ok_btn_data);
        lv_obj_add_event_cb(ok_btn, close_popup_event_cb, LV_EVENT_CLICKED, popup);
        #ifdef CONFIG_XOS_USE_PHYSICAL_KEY
        lv_obj_add_event_cb(panel, ok_btn_cb, LV_EVENT_KEY, popup);
        #endif
        lv_obj_t *ok_label = lv_label_create(ok_btn);
        lv_label_set_text(ok_label, LV_SYMBOL_OK);
        lv_obj_align(ok_label, LV_ALIGN_CENTER, 0, 0);
    }

    if (has_close_btn) {
        lv_obj_t *close_btn = lv_btn_create(panel);
        lv_obj_set_size(close_btn, XOS_USE_MSGPOP_BUTTON_WIDTH, XOS_USE_MSGPOP_BUTTON_HEIGHT);
        lv_obj_align(close_btn, LV_ALIGN_BOTTOM_MID, x_offset, 0);
        lv_obj_add_flag(close_btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(close_btn, close_popup_event_cb, LV_EVENT_CLICKED, popup);
        lv_obj_t *close_label = lv_label_create(close_btn);
        lv_label_set_text(close_label, LV_SYMBOL_CLOSE);
        lv_obj_align(close_label, LV_ALIGN_CENTER, 0, 0);
    }
}

