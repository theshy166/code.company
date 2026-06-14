#ifndef _QM_EFFECTS_H
#define _QM_EFFECTS_H

#include <stdbool.h>

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} qm_area_t;

typedef struct {
    void *buf;
    unsigned long buf_phy;
    char *path;
    short width;
    short height;
    short offset_x;
    short offset_y;
} ArcPartObj;

typedef struct {
    //target
    short canvas_width;//speed_canvas_width
    short canvas_height;//
    void *speed_canvas;//lvgl obj.
    void *speed_buf;
    unsigned long speed_buf_phy;//for Video show
    char *layer1_2_path;
    //bg size
    void *bg_buf;
    unsigned long bg_buf_phy;//
    short bg_width;
    short bg_height;
    //pre size
    void *predraw_buf;//size after blend.
    unsigned long predraw_buf_phy;
    short predraw_width;//aligned 4.
    short predraw_height;
    short arc_image_width;
    char *fore_path;
    //
    short arc_offset_x;
    short arc_offset_y;//目前默认按0偏移
    short arc_border_width;
    short max_angle;
    short arc_full_angle;//主界面：=max_angle, 子界面：=max_angle/2
    //bar
    void *bar_buf;
    unsigned long bar_buf_phy;
    short bar_width;
    short bar_height;
    char *line_path;
    float bar_width_per_value;// bar_width / bar_max_value
    short bar_max_value;//max_angle/2
    short bar_offset_x;
    short bar_offset_y;
    //real width = (speed_value - arc_full_speed) * bar_width_per_value
    ArcPartObj batteryArc;
    //illustraction
    ArcPartObj *illustration;//主子界面共享怎么做，指向同一块
    // //font
    short font_offset_x;
    short font_offset_y;
    //biz param
    short max_speed;
    bool is_subpage;
    bool use_qm_hw;
    bool has_init;
} ArcSpeedObjs;

int qm_effect_init(int screen_w, int screen_h);
int qm_effect_inits(int screen_w, int screen_h, unsigned long phy);
int qm_effect_init_scr_info(int screen_w, int screen_h, void *virtAddr, unsigned long phy);

#endif //_QM_EFFECTS_H
