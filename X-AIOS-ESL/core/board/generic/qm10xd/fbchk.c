/**
 * @file fbchk.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "fbdev.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

/*********************
 *      DEFINES
 *********************/

#define DHD0_FB_TEST 1
#define DHD1_FB_TEST 0

#define XOS_LOGU(fmt, ...)
#define XOS_LOGI(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)
#define XOS_LOGE(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)

#define SAMPLE_VO_DEV_DHD0    0
#define SAMPLE_VO_DEV_DHD1    1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      STRUCTURES
 **********************/
typedef struct fyUSER_SYNC_INFO_S{
    unsigned short	width;
    unsigned short	height;
    unsigned short	framerate;
    unsigned char	progressive;
    unsigned short	Hfront;
    unsigned short	Hsync;
    unsigned short	Hback;
    unsigned short	Hpol;
    unsigned short	Vfront;
    unsigned short	Vsync;
    unsigned short	Vback;
    unsigned short	Vpol;
} USER_SYNC_INFO_S;

typedef enum{
    VO_DHD0 = 0,				//!< 高清设备
    VO_DSD0 = 1				//!< 标清设备
}VO_DEV_ID_E;

extern int FH_VO_GetDispSize(VO_DEV_ID_E VoDev, const USER_SYNC_INFO_S *pstSyncInfo);

/**********************
 *      MACROS
 **********************/
// static USER_SYNC_INFO_S _syncInfo1st = {0};
// static USER_SYNC_INFO_S _syncInfo2nd = {0};
static int _lcm_1st_existed = 0;
static int _lcm_2nd_existed = 0;
static int _lcm_scn_numbers = 0;

int xos_lcm_1st_chk_existed(void)
{
    int s32Ret = 0;
    USER_SYNC_INFO_S syncInfo;

    s32Ret = FH_VO_GetDispSize(SAMPLE_VO_DEV_DHD0, &syncInfo);
    if (syncInfo.width != LV_USE_HOR_SIZE || syncInfo.height!= LV_USE_VER_SIZE) {
        printf("%s FH_VO_GetDispSize failed!\n", __func__);
        printf("%s %d %d failed!\n", __func__, syncInfo.width, syncInfo.height);
        return -1;
    }
    // _syncInfo1st.width = syncInfo.width;
    // _syncInfo1st.height = syncInfo.height;
    _lcm_1st_existed = 1;
    return s32Ret;
}

int xos_lcm_2nd_chk_existed(void)
{
#ifdef CONFIG_XOS_LCM_2ND_SCR
    int s32Ret = 0;
    USER_SYNC_INFO_S syncInfo;

    s32Ret = FH_VO_GetDispSize(SAMPLE_VO_DEV_DHD1, &syncInfo);
    if (syncInfo.width != LV_USE_HOR_SIZE || syncInfo.height!= LV_USE_VER_SIZE) {
        printf("%s FH_VO_GetDispSize failed!\n", __func__);
        printf("%s %d %d failed!\n", __func__, syncInfo.width, syncInfo.height);
        return -1;
    }
    // _syncInfo2nd.width = syncInfo.width;
    // _syncInfo2nd.height = syncInfo.height;
    _lcm_2nd_existed = 1;
    return s32Ret;
#else
    return -1;
#endif
}

int xos_lcm_1st_is_existed(void)
{
    return _lcm_1st_existed;
}

int xos_lcm_2nd_is_existed(void)
{
    return _lcm_2nd_existed;
}

int xos_lcm_chk_screens(void) {
    if(xos_lcm_1st_chk_existed() == 0) {
        _lcm_scn_numbers++;
    }
    if(xos_lcm_2nd_chk_existed() == 0) {
        _lcm_scn_numbers++;
    }
    return _lcm_scn_numbers;
}

int xos_lcm_get_screen_num(void) {
    return _lcm_scn_numbers;
}

// int xos_lcm_1st_rtn_width(void) {
//     return _syncInfo1st.width;
// }

// int xos_lcm_1st_rtn_height(void) {
//     return _syncInfo1st.height;
// }

// int xos_lcm_2nd_rtn_width(void) {
//     return _syncInfo2nd.width;
// }

// int xos_lcm_2nd_rtn_height(void) {
//     return _syncInfo2nd.height;
// }

// int xos_existed_screen_number(void) {
//     int screen_number = 0;
//     if(xos_lcm_1st_is_existed() == 0) {
//         screen_number++;
//     }
//     if(xos_lcm_2nd_is_existed() == 0) {
//         screen_number++;
//     }
//     return screen_number;
// }
