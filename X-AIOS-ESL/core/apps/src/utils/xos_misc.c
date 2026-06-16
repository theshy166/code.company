#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>


#if defined(CONFIG_STORAGE_TYPE_SPINOR)

#ifdef CONFIG_PRODUCT_XKPHOTO
#define MISC_DEVICE "/dev/mtdblock3"
#elif defined(CONFIG_PRODUCT_KIDCAMERA_NOR)
#define MISC_DEVICE "/dev/mtdblock6"
#else
#define MISC_DEVICE "/dev/mtdblock5"
#endif

struct android_bootloader_message {
    char command[32];
    char status[32];
    /* Number of times left attempting to boot this slot. */
    char tries_counter[2];
    /* 1 if this slot has booted successfully, 0 otherwise. */
    char successful_boot[2];
    char recovery[764];//reserved

    /* The 'recovery' field used to be 1024 bytes.  It has only ever
     * been used to store the recovery command line, so 768 bytes
     * should be plenty.  We carve off the last 256 bytes to store the
     * stage string (for multistage packages) and possible future
     * expansion. */
    char stage[32];//reserved

    /* The 'reserved' field used to be 224 bytes when it was initially
     * carved off from the 1024-byte recovery field. Bump it up to
     * 1184-byte so that the entire bootloader_message struct rounds up
     * to 2048-byte. */
    char reserved[1184];
};

#define BCB_SIZE    sizeof(struct android_bootloader_message)

int readmisc(struct android_bootloader_message *bcb) {
    char cmd[128];
    FILE *fp;

    // 使用dd命令读取misc分区
    snprintf(cmd, sizeof(cmd), "dd if=%s of=/tmp/misc_bcb bs=%zu count=1 2>/dev/null", 
            MISC_DEVICE, BCB_SIZE);
    if(system(cmd) != 0) return -1;

    // 读取临时文件
    if((fp = fopen("/tmp/misc_bcb", "rb")) == NULL) return -2;
    if(fread(bcb, BCB_SIZE, 1, fp) != 1) {
        fclose(fp);
        return -3;
    }
    fclose(fp);
    remove("/tmp/misc_bcb");
    return 0;
}

int writemisc(struct android_bootloader_message *bcb) {
    char cmd[256];
    FILE *fp;

    // 写入临时文件
    if((fp = fopen("/tmp/misc_bcb", "wb")) == NULL) return -1;
    if(fwrite(bcb, BCB_SIZE, 1, fp) != 1) {
        fclose(fp);
        return -2;
    }
    fclose(fp);

    // 使用dd直接写入块设备
    snprintf(cmd, sizeof(cmd), "dd if=/tmp/misc_bcb of=%s bs=%d count=1 2>/dev/null",
            MISC_DEVICE, BCB_SIZE);
    int ret = system(cmd);
    remove("/tmp/misc_bcb");
    return (ret == 0) ? 0 : -3;
}

int boot2linux() {
    struct android_bootloader_message bcb;
    if(readmisc(&bcb) != 0) {
        fprintf(stderr, "Failed to read misc partition\n");
        return -1;
    }
    printf("boot2linux bcb.command:%s\n",bcb.command);
    printf("boot2linux bcb.tries_counter:%d\n",bcb.tries_counter[0]);
    printf("boot2linux bcb.successful_boot:%d\n",bcb.successful_boot[0]);
    if(strlen(bcb.command) == 0 && bcb.tries_counter[0] == 0 && bcb.successful_boot[0] == 1) {
        return 0;
    }
    memset(bcb.command, 0, sizeof(bcb.command));
    memset(bcb.tries_counter, 0, sizeof(bcb.tries_counter));
    bcb.successful_boot[0] = 1;
    if(writemisc(&bcb) != 0) {
        fprintf(stderr, "Failed to write misc partition\n");
        return -2;
    }
    return 0;
}

int boot2downloader() {
    struct android_bootloader_message bcb;
    if(readmisc(&bcb) != 0) {
        fprintf(stderr, "Failed to read misc partition\n");
        return -1;
    }
    printf("boot2linux bcb.command:%s\n",bcb.command);
    printf("boot2linux bcb.tries_counter:%d\n",bcb.tries_counter[0]);
    printf("boot2linux bcb.successful_boot:%d\n",bcb.successful_boot[0]);

    memset(bcb.command, 0, sizeof(bcb.command));
    strncpy(bcb.command, "boot-downloader", sizeof(bcb.command)-1);
    bcb.command[sizeof(bcb.command)-1] = '\0';  // 强制添加终止符
    if(writemisc(&bcb) != 0) {
        fprintf(stderr, "Failed to write misc partition\n");
        return -2;
    }
    return 0;
}

#endif /*BUILD_RTT*/
