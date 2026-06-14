#if (!defined(BUILD_SIMULATOR)) && defined(BUILD_RTT)

#include "qua_sys_lcmd_adap.h"
#include <rtthread.h>
// #include <dfs_posix.h>
#include <stdarg.h>

// 线程创建函数适配
qua_sys_thread_t qua_sys_thread_create(const char *name,
                                     void (*thread_func)(void *parameter),
                                     void *parameter,
                                     uint32_t stack_size,
                                     uint32_t priority,
                                     uint32_t timeslice) {
    return rt_thread_create(name, thread_func, parameter,
                           stack_size, priority, timeslice);
}

// 线程启动函数适配
int qua_sys_thread_startup(qua_sys_thread_t thread) {
    return rt_thread_startup((rt_thread_t)thread);
}

// 线程等待函数适配
int qua_sys_thread_join(qua_sys_thread_t thread) {
    // return rt_thread_join((rt_thread_t)thread);
    return 0;
}

// 线程删除函数适配
int qua_sys_thread_delete(qua_sys_thread_t thread) {
    return rt_thread_delete((rt_thread_t)thread);
}

// 毫秒延时函数适配
void qua_sys_msleep(uint32_t ms) {
    rt_thread_mdelay(ms);
}

// // 文件状态查询适配
// int qua_sys_stat(const char *path, struct stat *buf) {
//     return dfs_stat(path, buf);
// }

// // 目录创建适配
// int qua_sys_mkdir(const char *pathname, mode_t mode) {
//     return dfs_mkdir(pathname, mode);
// }

// // 挂载函数适配
// int qua_sys_mount(const char *source, const char *target,
//                  const char *filesystemtype, unsigned long mountflags,
//                  const void *data) {
//     return dfs_mount(source, target, filesystemtype, mountflags, data);
// }

// // 卸载函数适配
// int qua_sys_unmount(const char *target) {
//     return dfs_unmount(target);
// }

static char print_buf[1024];
// 打印函数适配（RT-Thread，使用rt_snprintf实现）
int qua_sys_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    // 定义一个足够大的缓冲区存储格式化后的字符串
    // char print_buf[1024];
    int ret;
    
    // 使用rt_snprintf将格式化字符串写入缓冲区
    // 最后一个参数设为0表示不限制写入长度（实际受缓冲区大小限制）
    ret = rt_vsnprintf(print_buf, sizeof(print_buf) - 1, format, args);
    va_end(args);
    
    // 确保字符串以'\0'结尾
    print_buf[sizeof(print_buf) - 1] = '\0';
    
    // 将缓冲区内容输出到控制台
    if (ret > 0) {
        rt_kprintf("%s", print_buf);
    }
    
    return ret;
}


// 系统适配层初始化
void qua_sys_adapter_init(void) {
    // RT-Thread特定初始化代码（如果需要）
}

#endif // XOS_BUILD_SIMULATOR
