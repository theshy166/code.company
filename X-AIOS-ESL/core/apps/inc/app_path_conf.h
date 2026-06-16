
#ifndef _app_PATH_CONF_H_
#define _app_PATH_CONF_H_

#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
//path of skin
#define k_path_prefix      "H:./out/simulator/xos/res/skin/"
#define k_path_sufix       ".png"
//album path
#define DCIM_PATH         "H:./out/simulator/xos/res/DCIM"
//setting path
#define k_path_setting_json      "./out/simulator/xos/res/settings.json"

//-----------linux or rtt
#else

#define k_path_prefix      "H:/res/skin/"
#define k_path_sufix       ".png"
//album path
#ifdef CONFIG_XOS_USE_APP_SPHOTO
#define DCIM_PATH         "H:/mnt"
#endif

#ifdef CONFIG_XOS_USE_APP_AICAMERA
#define DCIM_PATH         "H:/data/DCIM/.dcim"
#endif //BUILD_SIMULATOR

//setting path
#define k_path_setting_json      "/res/settings.json"
#endif

//-------------------------------common----------------------
#define k_path_home_bg                           k_path_prefix"background"k_path_sufix


#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
#define k_path_free_ttf     "./out/simulator/xos/res/ttf/2312.ttf"
#define k_path_tiny_ttf     "H:./out/simulator/xos/res/ttf/2312_v9.ttf"
#else
#define k_path_free_ttf     "/res/ttf/2312.ttf"
#define k_path_tiny_ttf     "H:/res/ttf/2312_v9.ttf"
#endif

#define k_path_font_ttf     k_path_tiny_ttf

#define XOS_COLOR_BLUE    (lv_color_make(0x00,0xb7,0xee))
#define XOS_COLOR_ORANGE  (lv_color_make(0xff,0xa7,0x0d))
#define XOS_COLOR_GRAY    (lv_color_make(0xa0,0xa0,0xa0))
#define XOS_COLOR_YELLOW  (lv_color_make(0xff,0xd8,0x00))
#define XOS_COLOR_BLACK   (lv_color_make(0x00,0x00,0x00))
#define XOS_COLOR_WHITE   (lv_color_make(0xff,0xff,0xff))

#endif //_app_PATH_CONF_H_
