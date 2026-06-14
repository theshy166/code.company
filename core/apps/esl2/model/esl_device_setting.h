#ifndef __ESL_DEVICE_SETTING_H__
#define __ESL_DEVICE_SETTING_H__

typedef struct {
    char *SN;
    char *pos_ip;
    char *pos_id;
    char *server_ip;
    char *speech_play;
    char *store_name;
    char *store_id;
    char *show_msg;
    char *pos_version;
    char *video_muted;
    char *show_led_id;
    char *device_tpl_id;
    char *swiper_time;
    char *other_1;
    char *other_2;
    char *other_3;
    char *other_4;
    char *other_5;
    char *other_6;
    int merchant_id;
    char *local_up_tag;
    char *branch_no;
    char *branch_name;
    char *wifi_SSID;
    char *wifi_pwd;
    char *screen_direction;
    char *video_url;
    char *wxwork_role;
} esl_device_setting_t;

#endif // __ESL_DEVICE_SETTING_H__