
#ifndef __H5_ESL_CONF_H__
#define __H5_ESL_CONF_H__
#include "lvgl/lvgl.h"

#define SERVER_URL "http://172.16.1.149:3000/"

#define QM_ESL2_ELOG(fmt, ...) LV_LOG_ERROR("[UI:ESL2:][%s] " fmt, __func__, ##__VA_ARGS__)
#define QM_ESL2_LOG(fmt, ...)  LV_LOG_USER("[UI:ESL2:][%s] " fmt, __func__, ##__VA_ARGS__)

#define APP_SECRET "66e8716e1df0aaca7c727081f773a2a7"
#define APP_ID "1735110730"
#define OPEN_HOST 12362 //12349
#define OPEN_BROADCAST_HOST 13358 //13345
#define MAX_MESSAGE_CONSUMER_COUNT 3

#define ESL_MAX_CFG_SSID_STR_LEN 300

#define H5ESL_URL_BASE          SERVER_URL "landscape"
#define H5ESL_URL_BASE_WHITE    SERVER_URL "landscape_white"
#define H5ESL_URL_PIC_TEXT      SERVER_URL "landscape_pic_text"
#define H5ESL_URL_TB_BOILER     SERVER_URL "landscape_tb"
#define H5ESL_URL_BASE_2        SERVER_URL "landscape2"
#define H5ESL_URL_BASE_3        SERVER_URL "landscape3"
#define H5ESL_URL_NO_USE        "点击上面的模板链接"

///
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
#define k_path_prefix      "H:./out/simulator/xos/res"
// #define k_path_ttf "H:./out/simulator/xos/res/h5esl/ttf/AlibabaPuHuiTi-3-55-Regular.ttf"
#define k_path_ttf "H:./out/simulator/xos/res/ttf/2312_v9.ttf"
#define k_path_impact_ttf "H:./out/simulator/xos/res/ttf/impact-2.ttf"
#define k_path_img_prefix "./out/simulator/xos/res/img/"
#define k_master_ip_post 255
#define k_path_json_prefix "./out/simulator/xos/res/json/"
#define k_path_logo_prefix "./out/simulator/xos/res/logo/"
///
#else

#define k_path_prefix      "H:/res"
// #define k_path_ttf "H:/udisk/quatest/xos/res/h5esl/ttf/AlibabaPuHuiTi-3-55-Regular.ttf"
#define k_path_ttf "H:/res/ttf/2312_v9.ttf"
#define k_path_impact_ttf "H:/res/esl2/ttf/impact-2.ttf"
#define k_path_img_prefix "/data/res/img/"
#define k_master_ip_post 255
#define k_path_json_prefix "/data/res/json/"
#define k_path_logo_prefix "/data/res/logo/"

#endif

//image resource

#define k_path_img_bar_code          k_path_prefix"/skin/bar_code.png"
#define k_path_img_carambola         k_path_prefix"/skin/carambola_180x180.png"
#define k_path_img_plum              k_path_prefix"/skin/plum_180x180.png"
#define k_path_img_lizi              k_path_prefix"/skin/lizi_180x180.png"
#define k_path_video_potato          k_path_prefix"/video/patato_600_512.mp4"

#define k_path_img_bg4               k_path_prefix"/skin/bg4.png"
#define k_path_img_fanqie            k_path_prefix"/skin/fanqie.png"

#define k_path_json_product_base_model    k_path_json_prefix"product_base_model_%s.json"

#define k_path_default_bg_img        k_path_prefix"/esl2/skin/bg.jpg"

#define k_path_pagefooter_untie                   k_path_prefix"/esl2/skin/untie_24x24.png"
#define k_path_pagefooter_disconnet               k_path_prefix"/esl2/skin/wifi_broke_24x24.png"
#define k_path_pagefooter_wifi_connet_master           k_path_prefix"/esl2/skin/wifi_connect_1_24x24.png"
#define k_path_pagefooter_wifi_connet_slave           k_path_prefix"/esl2/skin/wifi_connect_2_24x24.png"

#define k_path_template_midautumn_0                 k_path_prefix"/esl2/skin/216c0779ea7118825ca42478ff3a018b_0.png"
#define k_path_template_midautumn_1                 k_path_prefix"/esl2/skin/216c0779ea7118825ca42478ff3a018b_1.png"

#define k_path_boot_bg_img_1        k_path_logo_prefix"boot_bg_0.jpg"
#define k_path_boot_bg_img_2        k_path_logo_prefix"boot_bg_1.jpg"

#endif
