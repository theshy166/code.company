#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <unistd.h>

#define FRAME_WIDTH 800
#define FRAME_HEIGHT 1280
#define BYTES_PER_PIXEL 4
#define FRAME_SIZE ((size_t)FRAME_WIDTH * FRAME_HEIGHT * BYTES_PER_PIXEL)

static volatile sig_atomic_t g_running = 1;

typedef struct {
    int fd;
    const char *path;
    struct fb_var_screeninfo vinfo;
    int can_pan;
} fb_target_t;

static void on_signal(int sig)
{
    (void)sig;
    g_running = 0;
}

static int has_suffix(const char *text, const char *suffix)
{
    size_t text_len = strlen(text);
    size_t suffix_len = strlen(suffix);
    return text_len >= suffix_len && strcmp(text + text_len - suffix_len, suffix) == 0;
}

static int compare_names(const void *a, const void *b)
{
    const char * const *sa = (const char * const *)a;
    const char * const *sb = (const char * const *)b;
    return strcmp(*sa, *sb);
}

static char **load_frame_paths(const char *dir_path, int *out_count)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir frame dir");
        return NULL;
    }

    int capacity = 16;
    int count = 0;
    char **paths = calloc((size_t)capacity, sizeof(char *));
    if (paths == NULL) {
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "frame_", 6) != 0 || !has_suffix(entry->d_name, ".bgra")) {
            continue;
        }

        if (count == capacity) {
            capacity *= 2;
            char **new_paths = realloc(paths, (size_t)capacity * sizeof(char *));
            if (new_paths == NULL) {
                break;
            }
            paths = new_paths;
        }

        size_t len = strlen(dir_path) + 1 + strlen(entry->d_name) + 1;
        paths[count] = malloc(len);
        if (paths[count] == NULL) {
            break;
        }
        snprintf(paths[count], len, "%s/%s", dir_path, entry->d_name);
        count++;
    }
    closedir(dir);

    qsort(paths, (size_t)count, sizeof(char *), compare_names);
    *out_count = count;
    return paths;
}

static int read_full_file(const char *path, unsigned char *buf, size_t size)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open frame");
        return -1;
    }

    size_t done = 0;
    while (done < size) {
        ssize_t n = read(fd, buf + done, size - done);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read frame");
            close(fd);
            return -1;
        }
        if (n == 0) {
            close(fd);
            return -1;
        }
        done += (size_t)n;
    }

    close(fd);
    return 0;
}

static int write_full_fd(int fd, const unsigned char *buf, size_t size)
{
    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek fb");
        return -1;
    }

    size_t done = 0;
    while (done < size) {
        ssize_t n = write(fd, buf + done, size - done);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("write fb");
            return -1;
        }
        if (n == 0) {
            return -1;
        }
        done += (size_t)n;
    }
    return 0;
}

static int open_fb(fb_target_t *target, const char *path)
{
    memset(target, 0, sizeof(*target));
    target->fd = -1;
    target->path = path;

    int fd = open(path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "skip missing fb: %s\n", path);
        return -1;
    }

    target->fd = fd;
    if (ioctl(fd, FBIOGET_VSCREENINFO, &target->vinfo) == 0) {
        target->can_pan = 1;
        fprintf(stderr, "%s vinfo: %ux%u bpp=%u offset=(%u,%u)\n",
                path, target->vinfo.xres, target->vinfo.yres,
                target->vinfo.bits_per_pixel,
                target->vinfo.xoffset, target->vinfo.yoffset);
    } else {
        target->can_pan = 0;
        fprintf(stderr, "FBIOGET_VSCREENINFO failed for %s: %s\n", path, strerror(errno));
    }
    fprintf(stderr, "write mode for %s\n", path);

    return 0;
}

static void close_fb(fb_target_t *target)
{
    if (target->fd >= 0) {
        close(target->fd);
        target->fd = -1;
    }
}

static void blit_fb(fb_target_t *target, const unsigned char *frame)
{
    if (target->fd < 0) {
        return;
    }

    write_full_fd(target->fd, frame, FRAME_SIZE);
    if (target->can_pan) {
        if (ioctl(target->fd, FBIOPAN_DISPLAY, &target->vinfo) < 0) {
            fprintf(stderr, "FBIOPAN_DISPLAY failed for %s: %s\n", target->path, strerror(errno));
            target->can_pan = 0;
        }
    }
}

int main(int argc, char **argv)
{
    const char *frame_dir = argc > 1 ? argv[1] : "/data/res/img/video_frames_bgra";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_video_overlay";
    int delay_us = argc > 3 ? atoi(argv[3]) : 50000;
    if (delay_us < 0) {
        delay_us = 0;
    }

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);

    int frame_count = 0;
    char **frames = load_frame_paths(frame_dir, &frame_count);
    if (frames == NULL || frame_count <= 0) {
        fprintf(stderr, "no bgra frames in %s\n", frame_dir);
        return 1;
    }

    unsigned char *frame_buf = malloc(FRAME_SIZE);
    if (frame_buf == NULL) {
        fprintf(stderr, "malloc frame buffer failed: %zu bytes\n", FRAME_SIZE);
        return 1;
    }

    fb_target_t fb0;
    fb_target_t fb4;
    open_fb(&fb0, "/dev/fb0");
    open_fb(&fb4, "/dev/fb4");
    if (fb0.fd < 0 && fb4.fd < 0) {
        fprintf(stderr, "no framebuffer opened\n");
        free(frame_buf);
        return 1;
    }

    fprintf(stderr, "fb video player start: frames=%d delay_us=%d marker=%s\n",
            frame_count, delay_us, marker);

    while (g_running && access(marker, F_OK) == 0) {
        for (int i = 0; i < frame_count && g_running && access(marker, F_OK) == 0; i++) {
            if (read_full_file(frames[i], frame_buf, FRAME_SIZE) != 0) {
                fprintf(stderr, "skip bad frame: %s\n", frames[i]);
                continue;
            }
            blit_fb(&fb0, frame_buf);
            blit_fb(&fb4, frame_buf);
            if (delay_us > 0) {
                usleep((useconds_t)delay_us);
            }
        }
    }

    fprintf(stderr, "fb video player stop\n");

    close_fb(&fb0);
    close_fb(&fb4);
    for (int i = 0; i < frame_count; i++) {
        free(frames[i]);
    }
    free(frames);
    free(frame_buf);
    return 0;
}
