/* Standalone fb0 test - writes color bars */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main() {
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0) { perror("open fb0"); return 1; }
    
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    
    printf("fb0: %dx%d bpp=%d smem=%d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.smem_len);
    
    uint8_t *fb = mmap(NULL, finfo.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb == MAP_FAILED) { perror("mmap"); return 1; }
    
    int w = vinfo.xres, h = vinfo.yres;
    
    /* Color bars: R, G, B, W across the screen */
    int bar_w = w / 4;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int off = (y * w + x) * 4;
            if (x < bar_w) {
                fb[off]=0xFF; fb[off+1]=0x00; fb[off+2]=0x00; fb[off+3]=0xFF; /* Red */
            } else if (x < bar_w*2) {
                fb[off]=0x00; fb[off+1]=0xFF; fb[off+2]=0x00; fb[off+3]=0xFF; /* Green */
            } else if (x < bar_w*3) {
                fb[off]=0x00; fb[off+1]=0x00; fb[off+2]=0xFF; fb[off+3]=0xFF; /* Blue */
            } else {
                fb[off]=0xFF; fb[off+1]=0xFF; fb[off+2]=0xFF; fb[off+3]=0xFF; /* White */
            }
        }
    }
    
    printf("Color bars written. Screen should show R/G/B/W bars.\n");
    printf("Press Ctrl+C to exit (keep display)...\n");
    
    /* Keep fb mapped so display stays */
    pause();
    
    return 0;
}
