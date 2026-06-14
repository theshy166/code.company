#ifndef __AUTOTEST_USER_OSAL_H__
#define __AUTOTEST_USER_OSAL_H__
#include "fh_std_error.h"
#include "fh_std_osal_usr.h"

#ifndef _FHK_OBJ_ZONE
#define _FHK_OBJ_ZONE FHK_STD_MODID2ZONE(FHK_STD_MOD_NONE)
#warning "Need to define _FHK_OBJ_ZONE when include fh_std_api.h"
#endif

#ifdef FPGA_ENV
#define FHK_DEF_TIMEOUT_MS (1000 * 5)
#else
#define FHK_DEF_TIMEOUT_MS (1000)
#endif

#endif
