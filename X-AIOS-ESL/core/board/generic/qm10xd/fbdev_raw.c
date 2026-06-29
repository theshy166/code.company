/**
 * @file fbdev_raw.c
 * Minimal Linux framebuffer backend for LVGL - no MM/QuaMM dependency.
 * Opens /dev/fb0 directly, mmaps it, and provides a buffer for LVGL rendering.
 */

#include "fbdev.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define RAW_BPP 4  /* 32-bit RGBA */

static int g_fb_fd = -1;
static uint8_t *g_fb_mem = NULL;
static uint32_t g_fb_size = 0;
static uint32_t g_fb_width = 0;
static uint32_t g_fb_height = 0;

/* Secondary display (fb4) */
static int g_fb2_fd = -1;
static uint8_t *g_fb2_mem = NULL;
static uint32_t g_fb2_size = 0;

/* Dynamic LVGL render buffers */
static uint8_t *g_lvgl_buf1 = NULL;
static uint8_t *g_lvgl_buf2 = NULL;

/* Stub: g_fb_device is referenced by libqm_effects.so (MM-backed fbdev_10xd.c),
 * but we don't need it in raw fbdev mode. Provide a null symbol to satisfy the linker. */
void *g_fb_device = NULL;

static int _raw_fb_open(const char *dev, int *out_fd, uint8_t **out_mem,
                         uint32_t *out_size, uint32_t *out_w, uint32_t *out_h)
{
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        printf("fbdev_raw: cannot open %s\n", dev);
        return -1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        printf("fbdev_raw: FBIOGET_VSCREENINFO %s failed\n", dev);
        close(fd);
        return -1;
    }
    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        printf("fbdev_raw: FBIOGET_FSCREENINFO %s failed\n", dev);
        close(fd);
        return -1;
    }

    printf("fbdev_raw: %s %ux%u bpp=%d smem_len=%u\n",
           dev, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.smem_len);

    uint8_t *mem = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        printf("fbdev_raw: mmap %s failed\n", dev);
        close(fd);
        return -1;
    }

    *out_fd = fd;
    *out_mem = mem;
    *out_size = finfo.smem_len;
    if (out_w) *out_w = vinfo.xres;
    if (out_h) *out_h = vinfo.yres;
    return 0;
}

void fbdev_init_xd(void)
{
    _raw_fb_open("/dev/fb0", &g_fb_fd, &g_fb_mem, &g_fb_size, &g_fb_width, &g_fb_height);
    /* test: write a red stripe to verify fb is writable */
    if (g_fb_mem && g_fb_size > 400) {
        for (int y = 0; y < 50; y++) {
            for (int x = 0; x < g_fb_width && x < 800; x++) {
                int off = (y * g_fb_width + x) * RAW_BPP;
                g_fb_mem[off+0] = 0xFF; /* R */
                g_fb_mem[off+1] = 0x00; /* G */
                g_fb_mem[off+2] = 0x00; /* B */
                g_fb_mem[off+3] = 0xFF; /* A */
            }
        }
        printf("fbdev_raw: wrote red test stripe to fb0\n");
    }
}

void fbdev2_init_xd(void)
{
    _raw_fb_open("/dev/fb4", &g_fb2_fd, &g_fb2_mem, &g_fb2_size, NULL, NULL);
}

void fbdev_init(void) { fbdev_init_xd(); }
void fbdev2_init(void) { fbdev2_init_xd(); }

void fbdev_exit(void)
{
    free(g_lvgl_buf1); g_lvgl_buf1 = NULL;
    free(g_lvgl_buf2); g_lvgl_buf2 = NULL;
    if (g_fb_mem) { munmap(g_fb_mem, g_fb_size); g_fb_mem = NULL; }
    if (g_fb_fd >= 0) { close(g_fb_fd); g_fb_fd = -1; }
}

void fbdev2_exit(void)
{
    if (g_fb2_mem) { munmap(g_fb2_mem, g_fb2_size); g_fb2_mem = NULL; }
    if (g_fb2_fd >= 0) { close(g_fb2_fd); g_fb2_fd = -1; }
}

void *fbdev_req_mem(int size)
{
    if (!g_lvgl_buf1) {
        g_lvgl_buf1 = calloc(1, size);
        printf("fbdev_raw: lvgl buf1 allocated %d bytes at %p\n", size, g_lvgl_buf1);
    }
    return g_lvgl_buf1;
}

void *fbdev2_req_mem(int size)
{
    if (!g_lvgl_buf2) {
        g_lvgl_buf2 = calloc(1, size);
        printf("fbdev_raw: lvgl buf2 allocated %d bytes at %p\n", size, g_lvgl_buf2);
    }
    return g_lvgl_buf2;
}

/* Stubs */
void fbdev_set_hole(int x, int y, int w, int h) { (void)x;(void)y;(void)w;(void)h; }
void fbdev2_set_hole(int x, int y, int w, int h) { (void)x;(void)y;(void)w;(void)h; }
void fbdev_set_statusbar(int x, int y, int w, int h) { (void)x;(void)y;(void)w;(void)h; }
void fbdev2_set_statusbar(int x, int y, int w, int h) { (void)x;(void)y;(void)w;(void)h; }

char *fbdev_get_fbp(void) { return (char *)g_fb_mem; }
char *fbdev2_get_fbp(void) { return (char *)g_fb2_mem; }

void fbdev_get_info(struct fb_var_screeninfo *v, struct fb_fix_screeninfo *f)
{
    if (v) memset(v, 0, sizeof(*v));
    if (f) memset(f, 0, sizeof(*f));
    if (g_fb_fd >= 0) {
        if (v) ioctl(g_fb_fd, FBIOGET_VSCREENINFO, v);
        if (f) ioctl(g_fb_fd, FBIOGET_FSCREENINFO, f);
    }
}

void fbdev2_get_info(struct fb_var_screeninfo *v, struct fb_fix_screeninfo *f)
{
    if (v) memset(v, 0, sizeof(*v));
    if (f) memset(f, 0, sizeof(*f));
    if (g_fb2_fd >= 0) {
        if (v) ioctl(g_fb2_fd, FBIOGET_VSCREENINFO, v);
        if (f) ioctl(g_fb2_fd, FBIOGET_FSCREENINFO, f);
    }
}

int fbdev_pan_disp(void) { return 0; }
int fbdev2_pan_disp(void) { return 0; }

/* LVGL v9 flush callbacks */
#if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9
void fbdev_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p)
{
    static int flush_count = 0;
    if (flush_count == 0) {
        printf("fbdev_raw: FIRST FLUSH area=(%d,%d)-(%d,%d) w=%d h=%d first_pixel=%02x%02x%02x%02x\n",
               area->x1, area->y1, area->x2, area->y2,
               area->x2 - area->x1 + 1, area->y2 - area->y1 + 1,
               color_p[0], color_p[1], color_p[2], color_p[3]);
    }
    if (flush_count < 3) printf("fbdev_raw: flush #%d\n", flush_count);
    flush_count++;
    if (!g_fb_mem || !g_lvgl_buf1) {
        lv_display_flush_ready(drv);
        return;
    }

    int w = area->x2 - area->x1 + 1;
    int fb_w = g_fb_width > 0 ? g_fb_width : LV_USE_HOR_SIZE;

    for (int y = area->y1; y <= area->y2; y++) {
        int src_off = (y - area->y1) * w * RAW_BPP;
        int dst_off = (y * fb_w + area->x1) * RAW_BPP;
        memcpy(g_fb_mem + dst_off, color_p + src_off, w * RAW_BPP);
    }

    lv_display_flush_ready(drv);
}

void fbdev2_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p)
{
    if (!g_fb2_mem || !g_lvgl_buf2) {
        lv_display_flush_ready(drv);
        return;
    }

    int w = area->x2 - area->x1 + 1;
    int fb_w = g_fb_width > 0 ? g_fb_width : LV_USE_HOR_SIZE;

    for (int y = area->y1; y <= area->y2; y++) {
        int src_off = (y - area->y1) * w * RAW_BPP;
        int dst_off = (y * fb_w + area->x1) * RAW_BPP;
        memcpy(g_fb2_mem + dst_off, color_p + src_off, w * RAW_BPP);
    }

    lv_display_flush_ready(drv);
}
#else
void fbdev_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    /* LVGL v8 fallback */
    if (!g_fb_mem || !g_lvgl_buf1) {
        lv_disp_flush_ready(drv);
        return;
    }

    int w = area->x2 - area->x1 + 1;
    int fb_w = g_fb_width > 0 ? g_fb_width : LV_USE_HOR_SIZE;

    for (int y = area->y1; y <= area->y2; y++) {
        int src_off = (y - area->y1) * w * RAW_BPP;
        int dst_off = (y * fb_w + area->x1) * RAW_BPP;
        memcpy(g_fb_mem + dst_off, color_p + src_off, w * RAW_BPP);
    }

    lv_disp_flush_ready(drv);
}

void fbdev2_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p)
{
    if (!g_fb2_mem || !g_lvgl_buf2) {
        lv_display_flush_ready(drv);
        return;
    }

    int w = area->x2 - area->x1 + 1;
    int fb_w = g_fb_width > 0 ? g_fb_width : LV_USE_HOR_SIZE;

    for (int y = area->y1; y <= area->y2; y++) {
        int src_off = (y - area->y1) * w * RAW_BPP;
        int dst_off = (y * fb_w + area->x1) * RAW_BPP;
        memcpy(g_fb2_mem + dst_off, color_p + src_off, w * RAW_BPP);
    }

    lv_display_flush_ready(drv);
}
#endif /* CONFIG_LVGL_V9 */
