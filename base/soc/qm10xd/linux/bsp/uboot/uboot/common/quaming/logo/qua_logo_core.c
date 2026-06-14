/*
#
# Copyright (c) Quaming Intelligent Technology Co., Ltd.
# (C) Copyright 2002-2006 All Rights Reserved.
# 
# SPDX-License-Identifier:	GPL-2.0+
#
*/
#include <common.h>

void qua_boot_logo(void) {
#ifdef CONFIG_QUA_LOGO_MIPI_JD9365D
	extern void qua_boot_logo_jd9365d(void);
	qua_boot_logo_jd9365d();
#endif

#ifdef CONFIG_QUA_LOGO_RGB_EK79001H
	extern void qua_gpio_set_lcm_ek79001h(void);
	qua_gpio_set_lcm_ek79001h();
	extern void qua_boot_logo_ek79001h(void);
	qua_boot_logo_ek79001h();
#endif
}
