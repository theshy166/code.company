/* Stable fb0 overlay - runs in background, writes content in loop */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

static int g_running = 1;
static volatile int g_signal = 0;

int main(int argc, char *argv[]) {
    /* Redirect output */
    FILE *logf = fopen("/data/ol.log", "w");
    if (!logf) logf = stderr;
    
    fprintf(logf, "ol: starting\n"); fflush(logf);
    
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0) { fprintf(logf, "ol: open fb0 failed\n"); fclose(logf); return 1; }
    
    struct fb_fix_screeninfo fi;
    struct fb_var_screeninfo vi;
    ioctl(fd, FBIOGET_FSCREENINFO, &fi);
    ioctl(fd, FBIOGET_VSCREENINFO, &vi);
    int w = vi.xres, h = vi.yres;
    
    fprintf(logf, "ol: %dx%d size=%d\n", w, h, fi.smem_len); fflush(logf);
    
    uint8_t *fb = mmap(NULL, fi.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb == MAP_FAILED) { 
        fprintf(logf, "ol: mmap failed\n"); fclose(logf); return 1; 
    }
    
    fprintf(logf, "ol: mapped at %p\n", (void*)fb); fflush(logf);
    
    int frame = 0;
    int bar_w = w / 5;
    
    while (g_running) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int off = (y * w + x) * 4;
                int bar = x / bar_w;
                switch (bar % 5) {
                case 0: fb[off]=0xFF;fb[off+1]=0x00;fb[off+2]=0x00;fb[off+3]=0xFF; break;
                case 1: fb[off]=0x00;fb[off+1]=0xFF;fb[off+2]=0x00;fb[off+3]=0xFF; break;
                case 2: fb[off]=0x00;fb[off+1]=0x00;fb[off+2]=0xFF;fb[off+3]=0xFF; break;
                case 3: fb[off]=0xFF;fb[off+1]=0xFF;fb[off+2]=0xFF;fb[off+3]=0xFF; break;
                case 4: fb[off]=0x00;fb[off+1]=0x00;fb[off+2]=0x00;fb[off+3]=0xFF; break;
                }
            }
        }
        frame++;
        if (frame % 100 == 0) {
            fprintf(logf, "ol: frame %d\n", frame); fflush(logf);
        }
        usleep(8000);
    }
    
    fprintf(logf, "ol: done\n"); fclose(logf);
    munmap(fb, fi.smem_len);
    close(fd);
    return 0;
}
