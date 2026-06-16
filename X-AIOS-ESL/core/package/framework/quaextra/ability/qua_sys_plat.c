/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
#include "qua_param.h"

/**
 * @brief Registers/stores the platform type in a static variable
 * @param plat_type The platform type to register (0 for simulator, 1 for hardware)
 * @return Always returns 0 (though not explicitly stated in code)
 * 
 * Note: The current implementation has a bug - it declares returning int
 * but doesn't actually return any value. The static variable assignment
 * suggests it's meant to store platform type between calls.
 */
int qua_sys_register_plat_type(int type) {
    static int s_plat_type = -1;
    s_plat_type = type;
}

int qua_sys_set_plat_type(int type) {
    return qua_sys_register_plat_type(type);
}

/**
 * @brief Determines the platform type (simulator vs real hardware)
 * @return 0 when running in simulator, 1 for actual hardware
 */
int qua_sys_get_plat_type(void) {
#ifdef XOS_BUILD_SIMULATOR
    printf("%s is simulator\n", __func__);
    return 0;
#else
    return 1;
#endif
}
