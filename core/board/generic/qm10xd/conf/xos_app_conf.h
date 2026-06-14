#ifndef XOS_APP_CONF_H
#define XOS_APP_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_XOS_USE_APP_LAUNCHER) && CONFIG_XOS_USE_APP_LAUNCHER
#define XOS_USE_APP_LAUNCHER        1
#endif

#ifdef CONFIG_XOS_FWK_APPMANAGER
#define XOS_USE_APP_MANAGER      1
#endif
#define XOS_USE_APP_STATUS_BAR      0
#define XOS_USE_APP_BOOT_ANIMATE    0
#define XOS_USE_APP_DEMOLIST        0
#define XOS_USE_APP_COFFEE_MILL     0
#define XOS_USE_APP_CAMERA          0
#define XOS_USE_APP_HEALTH          0

#if defined(CONFIG_XOS_USE_APP_VIDEO_CALL) && CONFIG_XOS_USE_APP_VIDEO_CALL
#define XOS_USE_APP_VIDEO_CALL      1
#else
#define XOS_USE_APP_VIDEO_CALL      0
#endif

//1. all sub module in gallery
//2. every module support enable or disable
#if defined(CONFIG_XOS_USE_APP_GALLERY) && CONFIG_XOS_USE_APP_GALLERY
#define XOS_USE_APP_VIDEOPLAYER     1
#define XOS_USE_APP_IMAGEVIEWER     1
#define XOS_USE_APP_FILEEXPLORER    1
#define XOS_USE_APP_GRIDMEDIA       0
#define XOS_USE_APP_AUDIOPLAYER     1
#endif

#define XOS_USE_APP_FACE_RECOGNIZE  0

#if defined(CONFIG_XOS_USE_APP_SETTING) && CONFIG_XOS_USE_APP_SETTING
#define XOS_USE_APP_SETTING         1
#endif

#define XOS_USE_APP_SMART_DOOR      0

#if defined(CONFIG_XOS_USE_APP_COOLFLORID) && CONFIG_XOS_USE_APP_COOLFLORID
#define XOS_USE_APP_COOLFLORID      1
#endif

#if defined(CONFIG_XOS_USE_APP_QETV) && CONFIG_XOS_USE_APP_QETV
#define XOS_USE_APP_QETV            1
#endif


//电子价签
#if (defined(CONFIG_XOS_USE_APP_H5_ESL) && CONFIG_XOS_USE_APP_H5_ESL)
#define XOS_USE_APP_H5_ESL          1
#else
#define XOS_USE_APP_H5_ESL          0
#endif

#if (defined(CONFIG_XOS_USE_APP_NATIVE_ESL) && CONFIG_XOS_USE_APP_NATIVE_ESL)
#define XOS_USE_APP_NATIVE_ESL          1
#else
#define XOS_USE_APP_NATIVE_ESL          0
#endif

#if defined(CONFIG_XOS_USE_APP_MOTO) && CONFIG_XOS_USE_APP_MOTO
#define XOS_USE_APP_MOTO            1
#endif

#if defined(CONFIG_XOS_USE_APP_SCANPEN) && CONFIG_XOS_USE_APP_SCANPEN
#define XOS_USE_APP_SCANPEN         1
#endif

#if defined(CONFIG_XOS_USE_APP_ANALOG_CLOCK)
#define XOS_USE_APP_ANALOG_CLOCK    1
#endif

#ifdef CONFIG_XOS_USE_APP_CALENDAR
#define XOS_USE_APP_CALENDAR        1
#endif

#ifdef CONFIG_XOS_USE_APP_ALARM
#define XOS_USE_APP_ALARM           1
#endif

#ifdef CONFIG_XOS_USE_APP_CALCULATOR
#define XOS_USE_APP_CALCULATOR      1
#endif

#ifdef CONFIG_XOS_USE_APP_SETUPWIZARD
#define XOS_USE_APP_SETUPWIZARD      1
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  /*XOS_APP_CONF_H*/
