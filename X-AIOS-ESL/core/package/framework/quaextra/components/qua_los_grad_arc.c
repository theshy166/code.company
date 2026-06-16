
#include "qua_los_grad_arc.h"
#include <stdio.h>

#if !defined(CONFIG_LVGL_V9) || CONFIG_LVGL_V9 == 0
typedef struct {
    lv_coord_t width;
    lv_coord_t height;
    int16_t arc_width;
    int16_t arc_percent;
    lv_align_t arc_pos;
    lv_coord_t offset_x;
    lv_coord_t offset_y;
    int16_t arc_base_percent;
    //
    lv_draw_mask_radius_param_t mask_in_param;
    lv_draw_mask_radius_param_t mask_out_param;
    lv_draw_mask_polygon_param_t mask_polygon_param;
    int16_t mask_in_id;
    int16_t mask_out_id;
    int16_t mask_polygon_id;
}my_mask_param;


//
lv_obj_t * qua_los_grad_arc_common(lv_obj_t *parent, int16_t size, int32_t inner_range,
    lv_color_t bg_color, lv_align_t align_pos, bool hasGap,
    lv_color_t begin_color, lv_color_t finish_color)
{
    lv_meter_indicator_t *indic = NULL;
    lv_meter_scale_t *scale_speed = NULL;
    //
    const int init_percent = 90;
    int start_angle_value = 0;
    int end_angle_value = init_percent * inner_range/100 ;//0 ~ 100;

    int16_t angle_offset_of_start = (90  - inner_range % 90) / 8;
    int16_t angle_start = 0;

    //
    int temp_arc_p = (inner_range/10) * 3.14;
    int arc_len = temp_arc_p * (size) / 36 ;
    int count = 0;
    if (hasGap) {
        count = arc_len / (3/*Gap*/ + 10);
    } else {
        count = arc_len / (10 - 4);
    }
    lv_color_t start_color = begin_color;
    lv_color_t end_color = finish_color;

    lv_obj_t *bg_wnd = parent;
    lv_obj_t *meter = lv_meter_create(bg_wnd);

    lv_obj_set_size(meter, size, size);
    lv_obj_align(meter, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(meter, LV_OPA_0, 0);
    lv_obj_set_style_border_width(meter, 0, 0);
    // add scale
    scale_speed = lv_meter_add_scale(meter);

    if (align_pos == LV_ALIGN_BOTTOM_LEFT) {
        angle_start = 90;
        angle_offset_of_start = (90 - angle_offset_of_start - inner_range);
    } else if (align_pos == LV_ALIGN_BOTTOM_RIGHT) {
        angle_offset_of_start = angle_offset_of_start;
        angle_start = 0;

        // //
        start_color = end_color;
        end_color = start_color;
        // //
        // //value:
        int the_percent_value = init_percent * inner_range/100 ;//0 ~ 100;
        end_angle_value = inner_range;//fixed, always = inner range.
        start_angle_value = inner_range - the_percent_value;
    } else if (align_pos == LV_ALIGN_TOP_LEFT) {
        angle_start = 180;
    } else if (align_pos == LV_ALIGN_TOP_RIGHT) {
        angle_start = 270;
        start_color = end_color;
        end_color = start_color;
        // //
        angle_offset_of_start = 90 - angle_offset_of_start - inner_range;
        //
        int the_percent_value = init_percent * inner_range/100 ;//0 ~ 100;
        end_angle_value = inner_range;//fixed, always = inner range.
        start_angle_value = inner_range - the_percent_value;
    } else {
        //
    }

    lv_meter_set_scale_ticks(meter, scale_speed, count /*cnt=10*5*/, 10 /*width*/, 30 /*len*/, lv_color_hex(0xffffff));
    lv_meter_set_scale_range(meter, scale_speed, 0 /*min*/, inner_range /*max*/, inner_range /*angle_range*/, angle_start + angle_offset_of_start);

    indic = lv_meter_add_scale_lines(meter, scale_speed,
                    start_color, end_color, false, 0);

    lv_meter_set_indicator_start_value(meter, indic, start_angle_value);
    lv_meter_set_indicator_end_value(meter, indic, end_angle_value);//40-0

    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);

    meter->user_data = indic;

    return meter;
}

//percent is 0~100
void qua_los_grad_arc_update(lv_obj_t *meter, int16_t inner_range, int16_t percent)
{
    lv_meter_indicator_t *indict = meter->user_data;
    if (percent >= 0) {
        int end_value = (percent%100) * inner_range/100 ;//0 ~ 100;
        lv_meter_set_indicator_end_value(meter, indict, end_value);
    }
    else {
        int init_percent = -percent;
        int the_percent_value = (init_percent % 100) * inner_range/100 ;//0 ~ 100;
        int end_angle_value = inner_range;//fixed, always = inner range.
        int start_angle_value = inner_range - the_percent_value;
        lv_meter_set_indicator_start_value(meter, indict, start_angle_value);
        lv_meter_set_indicator_end_value(meter, indict, end_angle_value);
    }
}

#else

lv_obj_t * qua_los_grad_arc_common(lv_obj_t *parent, int16_t size, int32_t inner_range,
    lv_color_t bg_color, lv_align_t align_pos, bool hasGap,
    lv_color_t begin_color, lv_color_t finish_color)
{
    return NULL;
}

void qua_los_grad_arc_update(lv_obj_t *meter, int16_t inner_range, int16_t percent)
{

}

#endif
