
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#define BUFFER_COUNT 4
struct buffer {
    void *start;
    size_t length;
};

static int fd = -1;
static struct buffer *buffers = NULL;
static struct v4l2_buffer r_buf;
// 初始化设备并设置格式
static int init_device(char *device) {
    struct v4l2_capability cap;
    struct v4l2_format fmt;

    // 打开设备
    if ((fd = open(device, O_RDWR)) == -1) {
        perror("打开设备失败");
        return -1;
    }

    // 查询设备能力
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("查询设备能力失败");
        close(fd);
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "设备不支持视频采集\n");
        close(fd);
        return -1;
    }
#if 1
    // 设置视频格式
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;  // H264格式
    fmt.fmt.pix.field = V4L2_FIELD_ANY;
    fmt.fmt.pix.width = 640;   // 根据设备支持调整
    fmt.fmt.pix.height = 480;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("设置格式失败");
        close(fd);
        return -1;
    }

    // 验证是否设置成功
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_MJPEG) {
        fprintf(stderr, "设备不支持mjpeg格式\n");
         close(fd);
         return -1;
    }
#endif
    return 0;
}

// 初始化内存映射缓冲区
static int init_mmap() {
    struct v4l2_requestbuffers req;

    memset(&req, 0, sizeof(req));
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        perror("申请缓冲区失败");
        return -1;
    }

    buffers = calloc(req.count, sizeof(*buffers));
    if (!buffers) {
        fprintf(stderr, "内存分配失败\n");
        return -1;
    }

    // 映射缓冲区
    for (int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
            perror("查询缓冲区失败");
            return -1;
        }

        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               fd, buf.m.offset);

        if (buffers[i].start == MAP_FAILED) {
            perror("内存映射失败");
            return -1;
        }
    }
    return 0;
}

// 开始采集
static int start_capturing() {
    for (int i = 0; i < BUFFER_COUNT; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            perror("放入缓冲区队列失败");
            return -1;
        }
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        perror("启动采集失败");
        return -1;
    }
    return 0;
}

// 清理资源
void cleanup() {
    // 停止采集
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    // 释放内存映射
    if (buffers) {
        for (int i = 0; i < BUFFER_COUNT; i++) {
            if (buffers[i].start != MAP_FAILED) {
                munmap(buffers[i].start, buffers[i].length);
            }
        }
        free(buffers);
        buffers = NULL;
    }

    if (fd != -1) close(fd);
}

// 主采集循环
int uvcRead(int chn, unsigned char** pBuf){


    struct pollfd tFds[1];
    int iRet;
    tFds[0].fd     = fd;
    tFds[0].events = POLLIN;
    iRet = poll(tFds, 1, 10000);
    if (iRet <= 0)
    {
        printf("poll error!\n");
        return -1;
     }

    // 取出缓冲区
    memset(&r_buf, 0, sizeof(r_buf));
    r_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    r_buf.memory = V4L2_MEMORY_MMAP;
    
    if (ioctl(fd, VIDIOC_DQBUF, &r_buf) == -1) {
        perror("取出缓冲区失败");
        return 0;
    }

   // QMLOG_I("index=%d\n", r_buf.index);

    *pBuf = buffers[r_buf.index].start;
    return r_buf.bytesused;

}

int uvcOpen(int chn)
{
    int i = 0;
    char device[32] ={0};
    for (i = 0; i < 4; i++){
        snprintf(device, 32, "/dev/video%d", i);
        if (!init_device(device)){
           break;
        }
    }

    if (4 == i)
        return -1;
    
    if (init_mmap() || start_capturing()){
        cleanup();
        return -1;
    }
    return 0;
}
int uvcClose(int chn)
{
    cleanup();
    return 0;
}

int uvcRelease(int chn)
{
    if (ioctl(fd, VIDIOC_QBUF, &r_buf) == -1) {
        perror("重新入队失败");
    }
    return 0;
}

