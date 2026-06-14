/**
 * @file fbdev.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "fbdev.h"

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "filter/qua_mm_filter.h"
#include "quagl/qua_gl.h"
#include "qme_api.h"
//
extern int g_screen_rotation;
extern unsigned long g_lvgl_phy;
extern void * g_lvgl_virt;

static qua_gl_surface_t lvgl_surface, fb_surface;
static qua_tde2_rect_t hole_rect, statusbar_rect;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static char *fbp = 0;

/**********************
 *      MACROS
 **********************/

extern char* fbdev_get_fbp(void);
extern void fbdev_get_info(struct fb_var_screeninfo* pOutScreenInfo, struct fb_fix_screeninfo *pOutFixInfo);
extern int  fbdev_pan_disp(void);
//
static void fbdev_dump_frame(const char *file_name, uint8_t * ptr, size_t size) {
    int fd;

    fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd < 0) {
        ALOGE("%s, open %s failed", __func__, file_name);
        return;
    }

    if (size != write(fd, ptr, size))
        ALOGE("%s, write %s failed size=%zu", __func__, file_name, size);

    close(fd);
}

void fbdev_init(void)
{
    extern void fbdev_init_xd(void);
    fbdev_init_xd();
    fbp = fbdev_get_fbp();
    fbdev_get_info(&vinfo, &finfo);
    fb_surface.data = fbp;
    fb_surface.phy_addr = finfo.smem_start;
    fb_surface.width = vinfo.xres;
    fb_surface.height = vinfo.yres;
    fb_surface.stride = vinfo.xres;
    fb_surface.format = vinfo.bits_per_pixel == 16 ? QUA_PIXEL_FMT_RGB_1555 : QUA_PIXEL_FMT_RGB_8888;
    fb_surface.crop_x = 0;
    fb_surface.crop_y = 0;
    fb_surface.crop_w = vinfo.xres;
    fb_surface.crop_h = vinfo.yres;
#ifdef CONFIG_XOS_FB_ROTATION
    lvgl_surface.phy_addr = finfo.smem_start + finfo.smem_len / 2;
    lvgl_surface.data = fbp + finfo.smem_len / 2;
    lvgl_surface.format = vinfo.bits_per_pixel == 16 ? QUA_PIXEL_FMT_RGB_1555 : QUA_PIXEL_FMT_RGB_8888;
    lvgl_surface.crop_x = 0;
    lvgl_surface.crop_y = 0;
    if (g_screen_rotation == 1 || g_screen_rotation == 3) {
        lvgl_surface.width = vinfo.yres;
        lvgl_surface.height = vinfo.xres;
        lvgl_surface.stride = vinfo.yres;
        lvgl_surface.crop_w = vinfo.yres;
        lvgl_surface.crop_h = vinfo.xres;
    } else {
        lvgl_surface.width = vinfo.xres;
        lvgl_surface.height = vinfo.yres;
        lvgl_surface.stride = vinfo.xres;
        lvgl_surface.crop_w = vinfo.xres;
        lvgl_surface.crop_h = vinfo.yres;
    }
#else
#ifdef CONFIG_XOS_FB_USE_ARGB1555
    lvgl_surface.phy_addr = g_lvgl_phy;
    lvgl_surface.data = g_lvgl_virt;
    lvgl_surface.format = QUA_PIXEL_FMT_RGB_8888;
    lvgl_surface.crop_x = 0;
    lvgl_surface.crop_y = 0;
    lvgl_surface.width = LV_LOGIC_HOR_SIZE;
    lvgl_surface.height = LV_LOGIC_VER_SIZE;
    lvgl_surface.stride = LV_LOGIC_HOR_SIZE;
    lvgl_surface.crop_w = LV_LOGIC_HOR_SIZE;
    lvgl_surface.crop_h = LV_LOGIC_VER_SIZE;
#else
    lvgl_surface = fb_surface;
#endif
#endif
}

void fbdev_exit(void)
{
    printf("%s e\n", __func__);
    extern void fbdev_exit_xd(void);
    fbdev_exit_xd();
    printf("%s x\n", __func__);
}

void *fbdev_req_mem(int size)
{
    qm_effect_init_scr_info(lvgl_surface.width, lvgl_surface.height, lvgl_surface.data, lvgl_surface.phy_addr);
    return lvgl_surface.data;
}

void fbdev_set_hole(int x, int y, int width, int height)
{
    hole_rect.x_pos = x;
    hole_rect.y_pos = y;
    hole_rect.width = width;
    hole_rect.height = height;
}

void fbdev_set_statusbar(int x, int y, int width, int height)
{
    statusbar_rect.x_pos = x;
    statusbar_rect.y_pos = y;
    statusbar_rect.width = width;
    statusbar_rect.height = height;
}

static void fbdev_draw_hole()
{
    if (hole_rect.width == 0 || hole_rect.height == 0)
        return;

    qua_tde2_rect_t tmp_rect = hole_rect;
    QUA_S32 new_y = statusbar_rect.y_pos + statusbar_rect.height;
    if (new_y > tmp_rect.y_pos) {
        tmp_rect.height = tmp_rect.y_pos + tmp_rect.height - new_y;
        tmp_rect.y_pos = new_y;
    }

#ifdef CONFIG_XOS_USE_FB_DIRECT
    lvgl_surface.crop_x = tmp_rect.x_pos;
    lvgl_surface.crop_y = tmp_rect.y_pos;
    lvgl_surface.crop_w = tmp_rect.width;
    lvgl_surface.crop_h = tmp_rect.height;
    qua_gl_fill(&lvgl_surface, 0);
    lvgl_surface.crop_x = 0;
    lvgl_surface.crop_y = 0;
    lvgl_surface.crop_w = lvgl_surface.width;
    lvgl_surface.crop_h = lvgl_surface.height;
#else
    fb_surface.crop_x = tmp_rect.x_pos;
    fb_surface.crop_y = tmp_rect.y_pos;
    fb_surface.crop_w = tmp_rect.width;
    fb_surface.crop_h = tmp_rect.height;
    qua_gl_fill(&fb_surface, 0);
    fb_surface.crop_x = 0;
    fb_surface.crop_y = 0;
    fb_surface.crop_w = fb_surface.width;
    fb_surface.crop_h = fb_surface.height;
#endif
}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */

void fbdev_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p)
{
#ifndef CONFIG_XOS_USE_FB_DIRECT
    /*Truncate the area to the screen*/
    lv_area_t buf_area = drv->layer_head->buf_area;
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > (int32_t)lvgl_surface.width - 1 ? (int32_t)lvgl_surface.width - 1 : area->x2;
    int32_t act_y2 = area->y2 > (int32_t)lvgl_surface.height - 1 ? (int32_t)lvgl_surface.height - 1 : area->y2;

    int32_t w = (act_x2 - act_x1 + 1);
    int32_t h = (act_y2 - act_y1 + 1);
    QUA_BOOL ret;
    qua_gl_surface_t src_surface = {};
    qua_gl_transform_t trans = {};

    src_surface.phy_addr = lvgl_surface.phy_addr;
    src_surface.format = QUA_PIXEL_FMT_RGB_8888;
    src_surface.width = buf_area.x2 - buf_area.x1 + 1;
    src_surface.height = buf_area.y2 - buf_area.y1 + 1;
    src_surface.stride = src_surface.width;
    src_surface.crop_x = act_x1 - buf_area.x1;
    src_surface.crop_y = act_y1 - buf_area.y1;
    src_surface.crop_w = w;
    src_surface.crop_h = h;
    fb_surface.crop_x = act_x1;
    fb_surface.crop_y = act_y1;
    fb_surface.crop_w = w;
    fb_surface.crop_h = h;
    trans.conversion = 1;
    ret = qua_gl_transform(&trans, &src_surface, &fb_surface);
    if (ret == QUA_FALSE)
        printf("%s, transform failed clip_area=[%d,%d,%d,%d] buf_area=[%d,%d,%d,%d]\n", __func__,
            area->x1, area->y1, area->x2, area->y2, buf_area.x1, buf_area.y1, buf_area.x2, buf_area.y2);

    fb_surface.crop_x = 0;
    fb_surface.crop_y = 0;
    fb_surface.crop_w = fb_surface.width;
    fb_surface.crop_h = fb_surface.height;
#endif

    //fbdev_dump_frame("/data/lvgl.rgb", lvgl_surface.data, lvgl_surface.stride * lvgl_surface.height * 4);
    if (lv_display_flush_is_last(drv)) {
        #ifndef CONFIG_XOS_USE_DLMPI
        fbdev_draw_hole();
#ifdef CONFIG_XOS_FB_ROTATION
        qua_gl_transform_t trans = { 0 };
        trans.rotate = 1;
        trans.rotation = g_screen_rotation;
        qua_gl_transform(&trans, &lvgl_surface, &fb_surface);
#else
#if defined(CONFIG_XOS_USE_FB_DIRECT) && defined(CONFIG_XOS_FB_USE_ARGB1555)
        qua_gl_transform_t trans = { 0 };
        trans.scale = 1;
        trans.conversion = 1;
        qua_gl_transform(&trans, &lvgl_surface, &fb_surface);
#endif
#endif
        #endif
        fbdev_pan_disp();
    }

    lv_display_flush_ready(drv);
    //printf("%s x\n", __func__);
}

void devfb_flush(void *virtualAddr, uint32_t offsetx, uint32_t offsety, uint32_t width, uint32_t height)
{
    printf("%s e\n", __func__);
    // XOS_LOGI("devfb_flush-->write argb from=%p, vinfo.bits_per_pixel=%d.finfo.line_length=%d.fbp=%p", virtualAddr, vinfo.bits_per_pixel, finfo.line_length, fbp);
    int32_t x1 = offsetx;
    int32_t y1 = offsety;
    int32_t x2 = x1 + width - 1;
    int32_t y2 = y1 + height - 1;

    if (fbp == NULL ||
        x2 < 0 ||
        y2 < 0 ||
        x1 > (int32_t)vinfo.xres - 1 ||
        y1 > (int32_t)vinfo.yres - 1)
    {
        // lv_display_flush_ready(drv);
        printf("[error]devfb_flush error. fbp=%p. x1=%d, y1=%d, x2=%d, y2=%d", fbp, x1, y1, x2, y2);
        return;
    }

    /*Truncate the area to the screen*/
    uint32_t act_x1 = x1 < 0 ? 0 : x1;
    uint32_t act_y1 = y1 < 0 ? 0 : y1;
    uint32_t act_x2 = x2 > (int32_t)vinfo.xres - 1 ? (int32_t)vinfo.xres - 1 : x2;
    uint32_t act_y2 = y2 > (int32_t)vinfo.yres - 1 ? (int32_t)vinfo.yres - 1 : y2;

    int32_t w = (act_x2 - act_x1 + 1);
    long int location = 0;
    // long int byte_location = 0;
    // unsigned char bit_location = 0;

    int32_t color_p = (int32_t)virtualAddr;

    /*32 or 24 bit per pixel*/
    if(vinfo.bits_per_pixel == 32 || vinfo.bits_per_pixel == 24) {
        uint32_t * fbp32 = (uint32_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            location = (act_x1 + vinfo.xoffset) + (y + vinfo.yoffset) * finfo.line_length/4;
            memcpy(&fbp32[location], (uint32_t *)color_p, (w) * 4);
            color_p += (w * 4);
        }
    }
    /*16 bit per pixel*/
    else if(vinfo.bits_per_pixel == 16) {
        uint16_t * fbp16 = (uint16_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            location = (act_x1 + vinfo.xoffset) + (y + vinfo.yoffset) * finfo.line_length / 2;
            memcpy(&fbp16[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1) * 2);
            color_p += w * 2;
        }
    }
    fbdev_pan_disp();
    //printf("%s x\n", __func__);
}
