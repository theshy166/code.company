/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>

#include "fbdev.h"
#include "display/qua_mm_display.h"
#include "system/qua_mm_system.h"
#include "common/qua_sys_platform.h"
#include "filter/qua_mm_filter.h"
#include "utils/qua_display_parser.h"
#include "bootanimation/qua_bootanimation.h"

#define DISPLAY_ID          "id:display1"

static QUA_CHAR *pShowScreen;
static QUA_U32 u32PhyAddrScreen;
static qua_fb_fix_screeninfo_t finfo;
static qua_fb_var_screeninfo_t vinfo;
static qua_mm_module_t *g_display_module = NULL;
static qua_mm_vo_device_t *g_vo_device = NULL;
static qua_mm_fb_device_t *g_fb_device = NULL;
static QUA_S32 vo_num, fb_num;
static QUA_CONST_CHAR *vo_id = NULL;
static QUA_CONST_CHAR *fb_id = NULL;

static QUA_S32 enable_vo_device(QUA_VO_DEV dev)
{
    QUA_S32 ret = QUA_SUCCESS;

    qua_mm_module_t *display_module = NULL;
    ret = qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module);
    if (ret != QUA_SUCCESS || display_module == NULL) {
        printf("Error: load display module failed\n");
        return ret;
    }

    g_display_module = display_module;
    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, vo_id, dev, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("Error: open vo dev failed\n");
        return ret;
    }

    qua_mm_vo_device_t *vo_device = (qua_mm_vo_device_t *)mm_device;
    g_vo_device = vo_device;
    return ret;
}

static QUA_S32 disable_vo_device(QUA_VO_DEV dev)
{
    qua_mm_vo_device_t *vo_device = g_vo_device;
    vo_device->disable(dev);
    return vo_device->parent.close((qua_mm_device_t *)vo_device);
}

static QUA_S32 init_fyfb()
{
    QUA_S32 ret = QUA_SUCCESS;
    qua_mm_device_t *mm_device = NULL;
    ret = g_display_module->open_device(g_display_module, fb_id, fb_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open fb dev failed\n");
        return ret;
    }

    qua_mm_fb_device_t *fb_device = (qua_mm_fb_device_t *)mm_device;
    g_fb_device = fb_device;
    fb_device->show(fb_device, QUA_FALSE);
    if (fb_device->get_var_info(fb_device, &vinfo) != QUA_SUCCESS) {
        printf("get var info failed\n");
        return QUA_FAILURE;
    }

#ifdef CONFIG_XOS_FB_USE_ARGB1555
    qua_fb_bitfield_t stA = {15,1,0};
    qua_fb_bitfield_t stR = {10,5,0};
    qua_fb_bitfield_t stG = {5,5,0};
    qua_fb_bitfield_t stB = {0,5,0};
    vinfo.bits_per_pixel = 16;
#else
    qua_fb_bitfield_t stA = {24,8,0};
    qua_fb_bitfield_t stR = {16,8,0};
    qua_fb_bitfield_t stG = {8,8,0};
    qua_fb_bitfield_t stB = {0,8,0};
    vinfo.bits_per_pixel = 32;
#endif

    vinfo.xres_virtual = vinfo.xres = LV_USE_HOR_SIZE;
    vinfo.yres_virtual = vinfo.yres = LV_USE_VER_SIZE;
    vinfo.activate = QUA_FB_ACTIVATE_NOW;
    vinfo.xoffset = 0;
    vinfo.yoffset = 0;
    vinfo.transp = stA;
    vinfo.red   = stR;
    vinfo.green = stG;
    vinfo.blue  = stB;
    if (fb_device->put_var_info(fb_device, &vinfo) != QUA_SUCCESS) {
        printf("put var info failed\n");
        return QUA_FAILURE;
    }

    if (fb_device->get_fix_info(fb_device, &finfo) != QUA_SUCCESS) {
        printf("get fix info failed\n");
        return QUA_FAILURE;
    }

    qua_fb_alpha_t alpha;
    alpha.alpha_enable = QUA_TRUE;
    alpha.alpha_channel = QUA_FALSE;
    alpha.alpha0 = 0;
    alpha.alpha1 = 128;
    alpha.global_alpha = 128;
    alpha.reserved = 0;
    if (fb_device->put_alpha(fb_device, &alpha) != QUA_SUCCESS) {
        printf("put alpha failed\n");
        return QUA_FAILURE;
    }

    pShowScreen = fb_device->map(fb_device, finfo.smem_len, 0);
    if (pShowScreen == QUA_NULL) {
        printf("map failed\n");
        goto map_failed;
    }

    memset(pShowScreen, 0x00, finfo.smem_len);
    fb_device->compress(fb_device, QUA_TRUE);
    return QUA_SUCCESS;
map_failed:
    fb_device->unmap(fb_device, pShowScreen, finfo.smem_len);
    return QUA_FAILURE;
}

static QUA_S32 deinit_fyfb()
{
    qua_mm_fb_device_t *fb_device = (qua_mm_fb_device_t *)g_fb_device;

    if (pShowScreen != NULL)
        fb_device->unmap(fb_device, pShowScreen, finfo.smem_len);

    fb_device->parent.close((qua_mm_device_t *)fb_device);
    return QUA_SUCCESS;
}

char * fbdev2_get_fbp(void)
{
    return pShowScreen;
}

void fbdev2_get_info(struct fb_var_screeninfo* pOutScreenInfo, struct fb_fix_screeninfo *pOutFixInfo)
{
    memcpy(pOutScreenInfo, &vinfo, sizeof(qua_fb_var_screeninfo_t));
    memcpy(pOutFixInfo, &finfo, sizeof(qua_fb_fix_screeninfo_t));
}

int fbdev2_init_xd(void)
{
    QUA_S32 ret;

    ret = qua_parse_display_id(DISPLAY_ID, &vo_id, &vo_num, &fb_id, &fb_num);
    if (ret != QUA_SUCCESS) {
        printf("%s, qua_parse_display_id error!\n", __func__);
        return ret;
    }

    ret = enable_vo_device(vo_num);
    if (ret != QUA_SUCCESS) {
        printf("enable_vo_device failed\n");
        //return -1;
    }

    ret = init_fyfb();
    if (ret != QUA_SUCCESS) {
        printf("init_fyfb failed\n");
        return -1;
    }

    return 0;
}

int fbdev2_exit_xd(void)
{
    deinit_fyfb();
    disable_vo_device(vo_num);
    return 0;
}

int fbdev2_pan_disp(void)
{
    qua_mm_fb_device_t *fb_dev = (qua_mm_fb_device_t *)g_fb_device;
    static int is_first_frame = 1;

    if (is_first_frame) {
#ifdef CONFIG_XOS_PLAY_BOOTANIMATION
        int status = qua_request_bootanimation(QUA_BOOTANIMAION_CMD_QUERY_STATUS);
        if (status >= QUA_BOOTANIMAION_STATUS_UNKNOWN && status < QUA_BOOTANIMAION_STATUS_BOOT_FINISH) {
            printf("%s: boot doesn't finish, status=%d\n", __func__, status);
            return -1;
        }
        printf("%s: boot animation finished, status=%d\n", __func__, status);
#else
        qua_vo_pub_attr_t dev_attr = {
            .bg_color = 0,
#ifdef CONFIG_XOS_LCM_800x1280
            .intf_sync = QUA_VO_OUTPUT_800x1280_60,
#elif defined(CONFIG_XOS_LCM_800x480)
            .intf_sync = QUA_VO_OUTPUT_800x480_60,
#else
            .intf_sync = QUA_VO_OUTPUT_1024x600_60,
#endif
            .intf_type = QUA_VO_INTF_LCD,
        };

        QUA_S32 ret = g_vo_device->set_public_attr(vo_num, &dev_attr);
        if (ret != QUA_SUCCESS) {
            printf("%s set_public_attr return %d\n", __func__, ret);
            return ret;
        }

        ret = g_vo_device->enable(vo_num);
        if (ret != QUA_SUCCESS) {
            printf("%s enable return %d\n", __func__, ret);
            return ret;
        }
        printf("%s VO enable ok, vo_num=%d\n", __func__, vo_num);
#endif
        if (fb_dev->show(fb_dev, QUA_TRUE) != QUA_SUCCESS) {
            printf("%s show failed\n", __func__);
            return -1;
        }
        printf("%s show ok\n", __func__);

        /* Re-apply alpha after VO enable (may have been reset) */
        qua_fb_alpha_t alpha;
        alpha.alpha_enable = QUA_TRUE;
        alpha.alpha_channel = QUA_FALSE;
        alpha.alpha0 = 0;
        alpha.alpha1 = 255;
        alpha.global_alpha = 255;
        alpha.reserved = 0;
        fb_dev->put_alpha(fb_dev, &alpha);
        printf("%s alpha set to 255\n", __func__);

        is_first_frame = 0;
    }

    return fb_dev->render(fb_dev, &vinfo);
}

