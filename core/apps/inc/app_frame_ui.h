#ifndef _app_frame_ui_h_
#define _app_frame_ui_h_

typedef struct _frame_ui_utils_t{
    char * frame_title_text;
    char * frame_content_text;

    bool is_switch;//右边是箭头还是开关
    bool is_show_content_text;//是否显示内容文字
    void (*click_callback)(lv_event_t *e);
} frame_ui_utils_t;


#endif //_app_frame_ui_h_
