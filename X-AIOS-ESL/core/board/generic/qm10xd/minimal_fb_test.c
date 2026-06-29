/**
 * Minimal LVGL + raw fbdev test
 * Compile manually to verify display output
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <signal.h>
#include "lvgl.h"

/* ========== Raw fbdev ========== */
static int g_fb_fd = -1;
static uint8_t *g_fb_mem = NULL;
static int g_fb_width = 0, g_fb_height = 0;
static uint8_t *g_lvgl_buf = NULL;

static void raw_fb_open(const char *dev)
{
    int fd = open(dev, O_RDWR);
    if (fd < 0) { perror("open fb"); return; }

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    
    printf("fb: %s %dx%d bpp=%d smem=%d\n", dev, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.smem_len);
    
    g_fb_mem = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (g_fb_mem == MAP_FAILED) { perror("mmap"); close(fd); return; }
    
    g_fb_fd = fd;
    g_fb_width = vinfo.xres;
    g_fb_height = vinfo.yres;
    
    printf("fb: mapped at %p\n", g_fb_mem);
}

/* ========== LVGL flush callback ========== */
static int g_flush_count = 0;

static void my_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
    g_flush_count++;
    if (g_flush_count <= 5) {
        printf("FLUSH #%d: area(%d,%d)-(%d,%d) first_px=%02x%02x%02x%02x\n",
               g_flush_count, area->x1, area->y1, area->x2, area->y2,
               color_p[0], color_p[1], color_p[2], color_p[3]);
    }
    
    int w = area->x2 - area->x1 + 1;
    for (int y = area->y1; y <= area->y2; y++) {
        int src_off = (y - area->y1) * w * 4;
        int dst_off = (y * g_fb_width + area->x1) * 4;
        memcpy(g_fb_mem + dst_off, color_p + src_off, w * 4);
    }
    lv_display_flush_ready(disp);
}

/* ========== LVGL tick ========== */
static volatile int g_tick = 0;
static void tick_inc(int sig) { g_tick++; }
static uint32_t my_tick_get(void) { return g_tick; }

int main(void)
{
    printf("=== minimal FB test start ===\n");
    fflush(stdout);
    
    /* Open framebuffer */
    raw_fb_open("/dev/fb0");
    if (!g_fb_mem) { printf("ERROR: cannot open fb0\n"); return 1; }
    
    /* Red stripe test */
    printf("Writing red stripe...\n");
    for (int y = 0; y < 80; y++) {
        for (int x = 0; x < g_fb_width; x++) {
            int off = (y * g_fb_width + x) * 4;
            g_fb_mem[off+0] = 0xFF; /* R */
            g_fb_mem[off+1] = 0x00; /* G */
            g_fb_mem[off+2] = 0x00; /* B */
            g_fb_mem[off+3] = 0xFF; /* A */
        }
    }
    printf("Red stripe written. Check screen!\n");
    
    /* Set up timer for LVGL tick */
    signal(SIGALRM, tick_inc);
    ualarm(1000, 1000); /* 1ms tick */
    
    /* Init LVGL */
    printf("lv_init...\n"); fflush(stdout);
    lv_init();
    lv_tick_set_cb(my_tick_get);
    printf("lv_init done\n"); fflush(stdout);
    
    /* Create display */
    lv_display_t *disp = lv_display_create(g_fb_width, g_fb_height);
    if (!disp) { printf("ERROR: display create failed\n"); return 1; }
    printf("Display created\n"); fflush(stdout);
    
    /* Alloc buffer and set */
    int buf_size = g_fb_width * g_fb_height * 4;
    g_lvgl_buf = calloc(buf_size, 1);
    printf("Buffer: %d bytes at %p\n", buf_size, g_lvgl_buf); fflush(stdout);
    
    lv_display_set_buffers(disp, g_lvgl_buf, NULL, buf_size, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(disp, my_flush_cb);
    printf("Buffers set\n"); fflush(stdout);
    
    /* Create a simple screen with a colored rectangle */
    //lv_obj_t *scr = lv_display_get_screen_active(disp);
    lv_obj_t *scr = lv_screen_active();
    printf("Screen active: %p\n", scr); fflush(stdout);
    
    /* Set background color */
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x0000FF), 0); /* Blue */
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    printf("Background set\n"); fflush(stdout);
    
    /* Add a label */
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "HELLO LVGL!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(label);
    printf("Label created\n"); fflush(stdout);
    
    /* Main loop */
    printf("Entering main loop...\n"); fflush(stdout);
    int loop = 0;
    while (loop < 500) { /* Run for ~5 seconds */
        lv_timer_handler();
        usleep(5000); /* 5ms */
        loop++;
    }
    
    printf("Done: %d flushes\n", g_flush_count);
    close(g_fb_fd);
    free(g_lvgl_buf);
    return 0;
}
