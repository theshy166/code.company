/* Overlay: write image to /dev/fb0 in a loop, overriding factory binary's render.
 * Compile: arm-linux-gcc -std=gnu99 -static -O2 -o overlay overlay.c
 * Usage:   ./overlay /data/res/img/demo.png     (PNG with stb_image)
 *          ./overlay                              (built-in color bars)
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <signal.h>

#define FB_DEV  "/dev/fb0"
#define FB_DEV2 "/dev/fb4"

static volatile int g_running = 1;
static void sig_handler(int s) { g_running = 0; }

static int fb_open(const char *dev, uint8_t **mem, int *w, int *h, int *size)
{
    int fd = open(dev, O_RDWR);
    if (fd < 0) { perror(dev); return -1; }
    
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) < 0) { perror("fix"); close(fd); return -1; }
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) { perror("var"); close(fd); return -1; }
    
    *w = vinfo.xres;
    *h = vinfo.yres;
    *size = finfo.smem_len;
    
    *mem = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (*mem == MAP_FAILED) { perror("mmap"); close(fd); return -1; }
    
    printf("%s: %dx%d bpp=%d size=%d\n", dev, *w, *h, vinfo.bits_per_pixel, *size);
    return fd;
}

/* Simple color bars */
static void draw_bars(uint8_t *fb, int w, int h)
{
    int bars = 5;
    int bw = w / bars;
    uint8_t colors[5][4] = {
        {0xFF,0x00,0x00,0xFF}, /* R */
        {0x00,0xFF,0x00,0xFF}, /* G */
        {0x00,0x00,0xFF,0xFF}, /* B */
        {0xFF,0xFF,0xFF,0xFF}, /* W */
        {0x00,0x00,0x00,0xFF}, /* Black */
    };
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int b = (x / bw) % bars;
            if (b >= bars) b = bars - 1;
            int off = (y * w + x) * 4;
            memcpy(fb + off, colors[b], 4);
        }
    }
}

/* Load raw RGBA8888 from file, draw centered */
static int load_rgba(const char *path, uint8_t **data, int *iw, int *ih)
{
    /* First 8 bytes: width(4) height(4) little-endian, then raw RGBA8888 */
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); return -1; }
    fread(iw, 4, 1, f);
    fread(ih, 4, 1, f);
    int size = (*iw) * (*ih) * 4;
    *data = malloc(size);
    if (!*data) { fclose(f); return -1; }
    fread(*data, 1, size, f);
    fclose(f);
    printf("loaded %s: %dx%d\n", path, *iw, *ih);
    return 0;
}

static void draw_image_centered(uint8_t *fb, int fbw, int fbh,
                                 uint8_t *img, int iw, int ih,
                                 int bg_r, int bg_g, int bg_b)
{
    int ox = (fbw - iw) / 2;
    int oy = (fbh - ih) / 2;
    
    for (int y = 0; y < fbh; y++) {
        for (int x = 0; x < fbw; x++) {
            int off = (y * fbw + x) * 4;
            int ix = x - ox, iy = y - oy;
            if (ix >= 0 && ix < iw && iy >= 0 && iy < ih) {
                int src = (iy * iw + ix) * 4;
                memcpy(fb + off, img + src, 4);
            } else {
                fb[off+0] = bg_r;
                fb[off+1] = bg_g;
                fb[off+2] = bg_b;
                fb[off+3] = 0xFF;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    
    printf("=== overlay: opening %s ===\n", FB_DEV);
    
    uint8_t *fb0 = NULL, *fb4 = NULL;
    int w, h, size, w2, h2, size2;
    int fd0 = fb_open(FB_DEV, &fb0, &w, &h, &size);
    int fd4 = fb_open(FB_DEV2, &fb4, &w2, &h2, &size2);
    
    if (fd0 < 0 && fd4 < 0) { printf("no fb device\n"); return 1; }
    
    uint8_t *img = NULL;
    int iw = 0, ih = 0;
    int has_img = 0;
    
    if (argc > 1) {
        has_img = (load_rgba(argv[1], &img, &iw, &ih) == 0);
    }
    
    int frame = 0;
    printf("=== overlay loop START (Ctrl+C to stop) ===\n");
    
    while (g_running) {
        if (has_img && img) {
            /* Center image on black background */
            draw_image_centered(fb0, w, h, img, iw, ih, 0, 0, 0);
            if (fb4) draw_image_centered(fb4, w2, h2, img, iw, ih, 0, 0, 0);
        } else {
            /* Color bars */
            draw_bars(fb0, w, h);
            if (fb4) draw_bars(fb4, w2, h2);
        }
        
        frame++;
        if (frame % 200 == 0) printf("frame %d\n", frame);
        
        usleep(8000); /* ~125fps, faster than LVGL's ~30fps */
    }
    
    printf("=== overlay: %d frames done ===\n", frame);
    if (img) free(img);
    if (fb0 && fb0 != MAP_FAILED) munmap(fb0, size);
    if (fb4 && fb4 != MAP_FAILED) munmap(fb4, size2);
    if (fd0 >= 0) close(fd0);
    if (fd4 >= 0) close(fd4);
    return 0;
}
