#if defined(BUILD_SIMULATOR) && (!defined(BUILD_RTT))

#include "qua_sys_lcmd_adap.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>  // 某些系统可能需要这个头文件来获取mode_t定义
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// 线程参数结构体
typedef struct {
    void (*thread_func)(void *parameter);
    void *parameter;
} qua_sys_thread_param_t;

// 线程包装函数
static void *qua_sys_thread_wrapper(void *param) {
    qua_sys_thread_param_t *thread_param = (qua_sys_thread_param_t *)param;
    thread_param->thread_func(thread_param->parameter);
    free(thread_param);
    return NULL;
}

// 线程创建函数适配
qua_sys_thread_t qua_sys_thread_create(const char *name,
                                     void (*thread_func)(void *parameter),
                                     void *parameter,
                                     uint32_t stack_size,
                                     uint32_t priority,
                                     uint32_t timeslice) {
    pthread_t *thread = malloc(sizeof(pthread_t));
    pthread_attr_t attr;
    struct sched_param sched_param;
    
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stack_size);
    
    // 设置线程优先级
    sched_param.sched_priority = priority;
    pthread_attr_setschedparam(&attr, &sched_param);
    
    // 包装线程参数
    qua_sys_thread_param_t *thread_param = malloc(sizeof(qua_sys_thread_param_t));
    thread_param->thread_func = thread_func;
    thread_param->parameter = parameter;
    
    if (pthread_create(thread, &attr, qua_sys_thread_wrapper, thread_param) != 0) {
        free(thread);
        free(thread_param);
        return NULL;
    }
    
    pthread_attr_destroy(&attr);
    return thread;
}

// 线程启动函数适配（Linux线程创建后立即运行）
int qua_sys_thread_startup(qua_sys_thread_t thread) {
    (void)thread;
    return 0; // Linux下线程创建后自动启动
}

// 线程等待函数适配
int qua_sys_thread_join(qua_sys_thread_t thread) {
    return pthread_join(*(pthread_t *)thread, NULL);
}

// 线程删除函数适配
int qua_sys_thread_delete(qua_sys_thread_t thread) {
    free(thread);
    return 0;
}

// 毫秒延时函数适配
void qua_sys_msleep(uint32_t ms) {
    usleep(ms * 1000);
}

// 文件状态查询适配
int qua_sys_stat(const char *path, struct stat *buf) {
    return stat(path, buf);
}

// 目录创建适配
int qua_sys_mkdir(const char *pathname, mode_t mode) {
    return mkdir(pathname, mode);
}

// 挂载函数适配
int qua_sys_mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data) {
    return mount(source, target, filesystemtype, mountflags, data);
}

// 卸载函数适配
int qua_sys_unmount(const char *target) {
    return umount(target);
}

// 打印函数适配
int qua_sys_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);
    return ret;
}

// 系统适配层初始化
void qua_sys_adapter_init(void) {
    // Linux特定初始化代码（如果需要）
}


// 为Linux系统提供命令行接口
// extern int qua_sys_stop_bmp_display_process(void);
// extern int qua_sys_start_bmp_display_process(void);
// #include <stdio.h>
// int main(int argc, char *argv[]) {
//     if (argc > 1) {
//         if (strcmp(argv[1], "start") == 0) {
//             return qua_sys_start_bmp_display_process();
//         } else if (strcmp(argv[1], "stop") == 0) {
//             return qua_sys_stop_bmp_display_process();
//         }
//     }
    
//     printf("Usage: %s [start|stop]\n", argv[0]);
//     return 0;
// }

#endif // XOS_BUILD_SIMULATOR
