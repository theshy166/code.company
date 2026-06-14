#ifndef LZMA_IOCTL_H
#define LZMA_IOCTL_H

#include <linux/ioctl.h>

#define CUSTOM_IOC_MAGIC 'k'
#define LZMA_DECODE_CMD _IOW(CUSTOM_IOC_MAGIC, 1, struct custom_data)

struct custom_data {
    unsigned int src;
    unsigned int dst;
    size_t len;
};

#endif
