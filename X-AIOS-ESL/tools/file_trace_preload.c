#define _GNU_SOURCE

#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TRACE_LOG_PATH "/data/filetrace.log"

static int (*real_open_fn)(const char *pathname, int flags, ...);
static FILE *(*real_fopen_fn)(const char *pathname, const char *mode);
static int (*real_access_fn)(const char *pathname, int mode);
static int s_logging;

static bool should_log_path(const char *path)
{
    if (path == NULL) {
        return false;
    }
    if (strcmp(path, TRACE_LOG_PATH) == 0) {
        return false;
    }
    return strstr(path, "/data/") != NULL ||
           strstr(path, ".json") != NULL ||
           strstr(path, ".jpg") != NULL ||
           strstr(path, ".png") != NULL ||
           strstr(path, "screen") != NULL ||
           strstr(path, "product_base_model") != NULL;
}

static void trace_line(const char *op, const char *path, const char *extra)
{
    if (!should_log_path(path)) {
        return;
    }
    if (s_logging) {
        return;
    }
    s_logging = 1;

    if (real_fopen_fn == NULL) {
        real_fopen_fn = dlsym(RTLD_NEXT, "fopen");
    }

    FILE *fp = real_fopen_fn(TRACE_LOG_PATH, "a");
    if (fp == NULL) {
        s_logging = 0;
        return;
    }
    fprintf(fp, "%s %s %s\n", op, path ? path : "(null)", extra ? extra : "");
    fclose(fp);
    s_logging = 0;
}

__attribute__((constructor))
static void trace_init(void)
{
    real_open_fn = dlsym(RTLD_NEXT, "open");
    real_fopen_fn = dlsym(RTLD_NEXT, "fopen");
    real_access_fn = dlsym(RTLD_NEXT, "access");
    trace_line("LOAD", "/data/file_trace_preload.c", "");
}

int open(const char *pathname, int flags, ...)
{
    mode_t mode = 0;
    bool has_mode = (flags & O_CREAT) != 0;
    if (has_mode) {
        va_list ap;
        va_start(ap, flags);
        mode = (mode_t)va_arg(ap, int);
        va_end(ap);
    }

    if (real_open_fn == NULL) {
        real_open_fn = dlsym(RTLD_NEXT, "open");
    }

    int ret = has_mode ? real_open_fn(pathname, flags, mode) : real_open_fn(pathname, flags);
    char extra[32];
    snprintf(extra, sizeof(extra), "ret=%d", ret);
    trace_line("open", pathname, extra);
    return ret;
}

FILE *fopen(const char *pathname, const char *mode)
{
    if (real_fopen_fn == NULL) {
        real_fopen_fn = dlsym(RTLD_NEXT, "fopen");
    }

    FILE *ret = real_fopen_fn(pathname, mode);
    char extra[48];
    snprintf(extra, sizeof(extra), "mode=%s ret=%p", mode ? mode : "", (void *)ret);
    trace_line("fopen", pathname, extra);
    return ret;
}

int access(const char *pathname, int mode)
{
    if (real_access_fn == NULL) {
        real_access_fn = dlsym(RTLD_NEXT, "access");
    }

    int ret = real_access_fn(pathname, mode);
    char extra[32];
    snprintf(extra, sizeof(extra), "mode=%d ret=%d", mode, ret);
    trace_line("access", pathname, extra);
    return ret;
}
