/* Quick fb0 test - write red frame and exit */
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

int main() {
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0) { perror("open"); return 1; }
    struct fb_fix_screeninfo fi;
    struct fb_var_screeninfo vi;
    ioctl(fd, FBIOGET_FSCREENINFO, &fi);
    ioctl(fd, FBIOGET_VSCREENINFO, &vi);
    printf("fb0: %dx%d bpp=%d smem=%d\n", vi.xres, vi.yres, vi.bits_per_pixel, fi.smem_len);
    
    uint8_t *fb = mmap(NULL, fi.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb == MAP_FAILED) { perror("mmap"); return 1; }
    
    /* Full red frame */
    memset(fb, 0x00, fi.smem_len);
    for (int i = 0; i < fi.smem_len; i += 4) {
        fb[i+0] = 0xFF; fb[i+1] = 0x00; fb[i+2] = 0x00; fb[i+3] = 0xFF;
    }
    printf("red frame written\n");
    
    /* Check first pixels */
    printf("fb[0..7]: %02x %02x %02x %02x %02x %02x %02x %02x\n",
           fb[0],fb[1],fb[2],fb[3],fb[4],fb[5],fb[6],fb[7]);
    
    munmap(fb, fi.smem_len);
    close(fd);
    return 0;
}
