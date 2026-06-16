#include "qm_realtime_label.h"
#include <time.h>

static void _update_date_time(lv_timer_t *t);

extern lv_font_t * ft_font_ttf_14;
extern lv_font_t * ft_font_ttf_30;

lv_obj_t* qm_realtime_label_create(lv_obj_t *superview, int time_style)
{
    lv_obj_t *datetime_label = lv_label_create(superview);
    lv_label_set_text(datetime_label, "12:00");
    #ifdef CONFIG_XOS_DARK_MODE
    lv_obj_set_style_text_color(datetime_label, lv_color_white(), 0);
    #else
    lv_obj_set_style_text_color(datetime_label, lv_color_black(), 0);
    #endif

    //
    #if defined(CONFIG_RESOLUTION_240x320) || defined(CONFIG_RESOLUTION_320x240)
    if (ft_font_ttf_14 != NULL) {
        lv_obj_set_style_text_font(datetime_label, ft_font_ttf_14, 0);
    }
    #else
    if (ft_font_ttf_30 != NULL) {
        lv_obj_set_style_text_font(datetime_label, ft_font_ttf_30, 0);
    }
    #endif

    //
    lv_timer_t *datetime_timer = lv_timer_create(_update_date_time, 3000, datetime_label);
    datetime_label->user_data = (void *)(time_style);
    _update_date_time(datetime_timer);
    return datetime_label;
}

static void _update_date_time(lv_timer_t *t)
{
    time_t timer;
    struct tm* ptTm = NULL;

    lv_obj_t *datetime_label = (lv_obj_t *)t->user_data;
    int time_style = (int)datetime_label->user_data;
    time(&timer);
    ptTm = localtime(&timer);
    if (ptTm) {
        char pTimeBuf[32];
        int model_24 = 1;
// #if CONFIG_XOS_INN_QUANTP != 0
// 		model_24 = get_xos_24hour_mode();
// #endif
        if (model_24) {
            #if !defined(BUILD_RTT) || BUILD_RTT == 0
            if (time_style == 2) {
                snprintf(pTimeBuf, 32, "%02d-%02d %02d:%02d", ptTm->tm_mon + 1, ptTm->tm_mday, ptTm->tm_hour, ptTm->tm_min);
            }
            else if (time_style == 1) {
                snprintf(pTimeBuf, 32, "%02d-%02d", /*ptTm->tm_year + 1900, */ptTm->tm_mon + 1, ptTm->tm_mday);
            }
            else {
                snprintf(pTimeBuf, 32, "%02d:%02d", ptTm->tm_hour, ptTm->tm_min/*, ptTm->tm_sec*/);
            }
            #endif
        }

        #if 0
        else if (ptTm->tm_hour < 12)
            snprintf(pTimeBuf, 32, "%s %02d-%02d %02d:%02d", /*ptTm->tm_year + 1900, */
                    GetTextRes(STRID_CALENDAR_CLK_IDLE_AM),
                    ptTm->tm_mon + 1, ptTm->tm_mday,
                    ptTm->tm_hour, ptTm->tm_min/*, ptTm->tm_sec*/);
        else
            snprintf(pTimeBuf, 32, "%s %02d-%02d %02d:%02d", /*ptTm->tm_year + 1900, */
                    GetTextRes(STRID_CALENDAR_CLK_IDLE_PM),
                    ptTm->tm_mon + 1, ptTm->tm_mday,
                    ptTm->tm_hour - 12, ptTm->tm_min/*, ptTm->tm_sec*/);
        #endif
        //extern lv_font_t * ft_font_ttf_30;
        //qua_xos_ft_init_with_path("H:./out/simulator/xos/res/ttf/2312_v9.ttf");
        //lv_obj_set_style_text_font(s_stabarobjs.datetime_label, (const lv_font_t *)ft_font_ttf_30, LV_PART_MAIN);
        if (datetime_label != NULL) {
            lv_label_set_text(datetime_label, pTimeBuf);
        }
        else {
            LV_LOG_ERROR("datetime_label is NULL");
        }
    }
}
