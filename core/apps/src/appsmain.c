/*
 * Copyright (c) Quaming Intelligent Technology Co., Ltd.
 * (C) Copyright All Rights Reserved.
 *
 */

#include "appsmain.h"
#include "lvgl/lvgl.h"
#include "xos_app_conf.h"
#include "appsentry_macro.h"

#if defined(CONFIG_XOS_FWK_APPMANAGER) && XOS_USE_APP_MANAGER
#include <unistd.h>
#include "appmanager.h"

#ifdef CONFIG_XOS_FWK_PARAM
#include "param.h"
#endif

extern void AppsRegister(void);

void AppsRegister(void)
{
#ifdef CONFIG_XOS_USE_APP_AUDIO_PLAYER
    extern void audioplayer_init(void);
    audioplayer_init();
#endif

#ifdef CONFIG_XOS_USE_APP_AIBOX
    extern void aibox_init(void);
    aibox_init();
#endif

#ifdef CONFIG_XOS_USE_APP_CASTSCREEN
    extern void castscreen_init(void);
    castscreen_init();
#endif

#ifdef CONFIG_XOS_USE_APP_SETUPWIZARD
    extern void setupwizard_init(void);
    setupwizard_init();
#endif

//app register
#if defined(CONFIG_XOS_USE_APP_MOTOX)
    extern void motox_init(void);
    motox_init();
#elif defined(CONFIG_XOS_USE_APP_MOTOX7IN)
    extern void motox7in_init(void);
    motox7in_init();
#elif defined(CONFIG_XOS_USE_APP_MOTOX102D)
    extern void motox102d_init(void);
    motox102d_init();
#elif defined(CONFIG_XOS_USE_APP_MOTOX_RD)
    extern void motox_init(void);
    motox_init();
#endif

#ifdef CONFIG_XOS_USE_APP_MOTOT
    extern void motot_init(void);
    motot_init();
#endif

#if defined(CONFIG_XOS_USE_APP_LAUNCHER)
    extern void launcher_init(void);
    launcher_init();
#endif

#if defined(CONFIG_XOS_APP_XIAOMING)
    extern void xiaoming_init(void);
    xiaoming_init();
#endif

#if defined(CONFIG_XOS_USE_APP_SCREEN_PROTECT)
    extern void screen_protect_init(void);
    screen_protect_init();
#endif

#if defined(CONFIG_XOS_USE_APP_MOTO) && XOS_USE_APP_MOTO
    extern void moto_init(void);
    moto_init();
#endif

#if defined(CONFIG_XOS_USE_APP_SCANNER)
    extern void scanner_init(void);
    scanner_init();
#endif

#if defined(CONFIG_XOS_USE_APP_MORESETTING)
    extern void more_scanner_init(void);
    more_scanner_init();
#endif

#if defined(CONFIG_XOS_USE_APP_PETROBOT)
    extern void petrobot_init(void);
    petrobot_init();
#endif

#if defined(CONFIG_XOS_USE_APP_SCANPEN) && XOS_USE_APP_SCANPEN
    extern void scanpen_init(void);
    scanpen_init();
#endif

#if defined(CONFIG_XOS_USE_APP_VIDEO_CALL) && XOS_USE_APP_VIDEO_CALL
    extern void videocall_init(void);
    videocall_init();
#endif

#if defined(CONFIG_XOS_USE_APP_MOMO_TALK) 
    extern void momotalk_init(void);
    momotalk_init();
#endif

#if defined(CONFIG_XOS_USE_APP_NATIVE_ESL)
    extern void h5esl_init(void);
    h5esl_init();
    extern void esl_init(void);
    esl_init();  
#endif

#if defined(CONFIG_XOS_APP_ESL2)
    extern void esl2_init(void);
    esl2_init();
#endif

#if defined(CONFIG_XOS_USE_APP_QETV) && XOS_USE_APP_QETV
    extern void qetv_init(void);
    qetv_init();
#endif

#if defined(CONFIG_XOS_USE_APP_GALLERY) && CONFIG_XOS_USE_APP_GALLERY
    extern void gallery_init(void);
    gallery_init();
#endif

#if defined(CONFIG_XOS_USE_APP_AICAMERA) && CONFIG_XOS_USE_APP_AICAMERA
    extern void aicamera_init(void);
    aicamera_init();
#endif

#ifdef CONFIG_XOS_USE_APP_AICHAT
    extern void aichat_init(void);
    aichat_init();

    extern void aitalk_init(void);
    aitalk_init();
    
#ifdef CONFIG_PRODUCT_STUDYPILOT
    extern void aicorrect_init(void);
    aicorrect_init();
    
    extern void aipose_init(void);
    aipose_init();
#endif //CONFIG_PRODUCT_STUDYPILOT
#endif

#if defined(CONFIG_XOS_USE_APP_DEMO) && CONFIG_XOS_USE_APP_DEMO
    extern void demo_init(void);
    demo_init();
#endif

#if defined(CONFIG_XOS_USE_APP_DEMOLIST) && XOS_USE_APP_DEMOLIST
    extern void demolist_init(void);
    demolist_init();

    extern void app_a_init(void);
    app_a_init();

    extern void app_b_init(void);
    app_b_init();

    extern void app_c_init(void);
    app_c_init();
#endif

#if defined(CONFIG_XOS_USE_APP_SETTING) && XOS_USE_APP_SETTING
    extern void setting_init(void);
    setting_init();
#endif

#if defined(CONFIG_XOS_USE_APP_SPHOTO) && CONFIG_XOS_USE_APP_SPHOTO
    extern void sphoto_init(void);
    sphoto_init();

    extern void xmphoto_init(void);
    xmphoto_init();

    // extern void xkphoto_init(void);
    // xkphoto_init();
#endif

#ifdef CONFIG_XOS_USE_APP_CALENDAR
    extern void calendar_init(void);
    calendar_init();
#endif

#ifdef CONFIG_XOS_USE_APP_ALARM
    extern void alarm_init(void);
    alarm_init();
#endif

#ifdef CONFIG_XOS_USE_APP_CALCULATOR
    extern void calculator_init(void);
    calculator_init();
#endif

//app register
#ifdef CONFIG_XOS_USE_APP_SMARTHOME
    extern void smarthome_init(void);
    smarthome_init();
#endif

#ifdef CONFIG_XOS_USE_APP_XHOME
    extern void xhome_init(void);
    xhome_init();
#endif

#ifdef CONFIG_XOS_USE_APP_ANALOG_CLOCK
    extern void analogclock_init(void);
    analogclock_init();
#endif

#ifdef CONFIG_XOS_APP_SETTING_PAD
    extern void settingpad_init(void);
    settingpad_init();
#endif

#ifdef CONFIG_XOS_USE_APP_WIFIX
    extern void wifix_init(void);
    wifix_init();
#endif

#if defined(CONFIG_XOS_USE_APP_DEMOAPP)
    extern void demoapp_init(void);
    demoapp_init();
#endif
}
#endif

void AppsFactories(void)
{
#if defined(CONFIG_XOS_USE_APP_SETUPWIZARD) && XOS_USE_APP_SETUPWIZARD
#if defined(CONFIG_XOS_FWK_PARAM) && CONFIG_XOS_FWK_PARAM
    // APPMGR_PRINT("sys:setupwizard=%d sys:firstboot=%d", param_get_int("sys:setupwizard", 0), param_get_int("sys:firstboot", 0));
    if (param_get_int("sys:setupwizard", 0) &&
        param_get_int("sys:firstboot", 0)) {
        app_manager_start("setupwizard", NULL);
        return;
    }
#endif
#endif
#if defined(CONFIG_XOS_USE_APP_LAUNCHER)
#if defined(CONFIG_XOS_FWK_APPMANAGER) && XOS_USE_APP_MANAGER
    app_manager_start("launcher", NULL);
    return;
#else
    extern void launcher_entry(void);
    launcher_entry();
    return;
#endif
#endif

#if defined(CONFIG_XOS_FWK_APPMANAGER)
    FUNC_DECL2(CONFIG_XOS_APP_MAIN_ENTRY);
    return;
#else
#if defined(CONFIG_PRODUCT_MOTOX_RD)
#if defined(CONFIG_XOS_USE_APP_MOTOX_RD)
    FUNC_DECL(CONFIG_XOS_APP_MAIN_ENTRY);
#endif
    return;
#else
    FUNC_DECL(CONFIG_XOS_APP_MAIN_ENTRY);
    return;
#endif
#endif

}
