/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
#include "qua_param.h"

int qua_sys_get_icon_style(void) {
#if defined(CONFIG_XOS_FWK_PARAM) && !defined(CONFIG_XOS_USE_APP_MOTOX)
    return param_get_int("sys.setting:icon_style", 0);
#else
    return 0;
#endif
}

int qua_sys_set_icon_style(int style) {
#if defined(CONFIG_XOS_FWK_PARAM) && !defined(CONFIG_XOS_USE_APP_MOTOX)
    return param_set_int("sys.setting:icon_style", style);
#else
    return 0;
#endif
}


int qua_sys_get_icon_grid_hor_num(void) {
#if defined(CONFIG_XOS_FWK_PARAM) && !defined(CONFIG_XOS_USE_APP_MOTOX)
    return param_get_int("sys.setting:icon_grid_hor_num", 0);
#else
    return 0;
#endif
}

int qua_sys_set_icon_grid_hor_num(int num) {
#if defined(CONFIG_XOS_FWK_PARAM) && !defined(CONFIG_XOS_USE_APP_MOTOX)
    return param_set_int("sys.setting:icon_grid_hor_num", num);
#else
    return 0;
#endif
}

int qua_sys_get_icon_grid_ver_num(void) {
#if defined(CONFIG_XOS_FWK_PARAM) && !defined(CONFIG_XOS_USE_APP_MOTOX)
    return param_get_int("sys.setting:icon_grid_ver_num", 0);
#else
    return 0;
#endif
}

int qua_sys_set_icon_grid_ver_num(int num) {
#if defined(CONFIG_XOS_FWK_PARAM) && !defined(CONFIG_XOS_USE_APP_MOTOX)
    return param_set_int("sys.setting:icon_grid_ver_num", num);
#else
    return 0;
#endif
}
