#ifndef _PAGE_FOOTER_H_
#define _PAGE_FOOTER_H_

#include "lvgl/lvgl.h"
typedef struct {
    union {
        char *text;
        char *icon;
    } ctn ;
    bool is_icon;
    bool enable_datetime;
} page_footer_t;

typedef enum {
    FOOTER_STATUS_UNBIND_DEVICE,   //未绑定设备
    FOOTER_STATUS_DISCONNECT_NET,  //网络未连接
    FOOTER_STATUS_CONNECTED_NET    //网络已连接
} page_footer_connet_status_t;

void init_show_page_footer_ui(void * active_screen,int bg_index);

void update_page_footer_icon(int bg_index, page_footer_connet_status_t IsConnect_Status);
void async_update_footer_icon(int bg_index);

void deinit_page_footer(int bg_index);

int set_pagefooter_time_from_internet(char* server_ip, int port, int time_zone);



#endif // _PAGE_FOOTER_H_