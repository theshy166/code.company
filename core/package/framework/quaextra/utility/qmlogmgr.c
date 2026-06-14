//app: moto, gallery, 
//module: battery, statusbar, poweronoff,

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include "qmlogmgr.h"
#include "lvgl/lvgl.h"

typedef struct {
    qm_log_module_id_t module;
    const char tag[32];
    bool is_enabled;
} qm_log_mgr_module_t;

static qm_log_mgr_module_t s_log_app_module[] = {
    {QM_LOG_MODULE_MOTO, "ui:moto", false}, 
    {QM_LOG_MODULE_MOTO_NAV, "ui:moto:nav", false}, 
    {QM_LOG_MODULE_MOTO_MAP, "ui:moto:map", false}, 
    {QM_LOG_MODULE_MOTO_MUSIC, "ui:moto:music", false}, 
    {QM_LOG_MODULE_MOTO_VIDEO, "ui:moto:video", false}, 
    {QM_LOG_MODULE_MOTO_GAME, "ui:moto:game", false}, 
    {QM_LOG_MODULE_MOTO_TRACK, "ui:moto:track", false}, 
    {QM_LOG_MODULE_MOTO_STATISTIC, "ui:moto:statistic", false}, 
    {QM_LOG_MODULE_MOTO_PHONE, "ui:moto:phone", false},
    {QM_LOG_MODULE_MOTO_SPEED, "ui:moto:speed", false},
    {QM_LOG_MODULE_MOTO_TIRE, "ui:moto:tire", false}, 
    {QM_LOG_MODULE_MOTO_BATTERY, "ui:moto:battery", false}, 
    {QM_LOG_MODULE_MOTO_WEATHER, "ui:moto:weather", false},
    {QM_LOG_MODULE_MOTO_GEAR, "ui:moto:gear", false},
    {QM_LOG_MODULE_MOTO_SKIN, "ui:moto:skin", false},
    {QM_LOG_MODULE_MOTO_BOOST, "ui:moto:boost", false},
    {QM_LOG_MODULE_MOTO_NFC, "ui:moto:nfc", false},
    {QM_LOG_MODULE_MOTO_POWER, "ui:moto:power", false},
    {QM_LOG_MODULE_MOTO_PASSWORD, "ui:moto:password", false},
    {QM_LOG_MODULE_MOTO_SHORTCUT, "ui:moto:shortcut", false},//快捷方式
    {QM_LOG_MODULE_GALLERY, "ui:gallery", false},
    {QM_LOG_MODULE_BATTERY, "ui:battery", false},
    {QM_LOG_MODULE_STATUSBAR, "ui:statusbar", false},
    {QM_LOG_MODULE_POWERONOFF, "ui:poweronoff", false},
    {QM_LOG_MODULE_PHONE, "ui:phone", false},
    {QM_LOG_MODULE_MUSIC, "ui:music", false},
};

void qm_log_mgr_enable_log(qm_log_module_id_t module)
{
    for (int i = 0; i < sizeof(s_log_app_module) / sizeof(s_log_app_module[0]); i++) {
        if (s_log_app_module[i].module == module) {
            s_log_app_module[i].is_enabled = true;
            break;
        }
    }
}

void qm_log_mgr_disable_log(qm_log_module_id_t module)
{
    for (int i = 0; i < sizeof(s_log_app_module) / sizeof(s_log_app_module[0]); i++) {
        if (s_log_app_module[i].module == module) {
            s_log_app_module[i].is_enabled = false;
            break;
        }
    }
}

//
void qm_log_mgr_log(qm_log_module_id_t module, const char* fmt, ...)
{
    for (int i = 0; i < sizeof(s_log_app_module) / sizeof(s_log_app_module[0]); i++) {
        if (s_log_app_module[i].module == module) {
            if (s_log_app_module[i].is_enabled) {
                va_list args;
                va_start(args, fmt);
                char buf[256];
                vsnprintf(buf, sizeof(buf), fmt, args);
                LV_LOG_USER("[%s] %s", s_log_app_module[i].tag, buf);
                va_end(args);
            }
            break;
        }
    }
}

