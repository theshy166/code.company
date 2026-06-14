#ifndef QUA_SYS_LCMD_ADAP_H
#define QUA_SYS_LCMD_ADAP_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>  // 添加这个头文件提供struct stat和mode_t定义
#include <sys/types.h>  // 某些系统可能需要这个头文件来获取mode_t定义

// 线程类型定义
typedef void *qua_sys_thread_t;

// 系统适配层初始化
void qua_sys_adapter_init(void);

// 线程管理接口
qua_sys_thread_t qua_sys_thread_create(const char *name,
                                     void (*thread_func)(void *parameter),
                                     void *parameter,
                                     uint32_t stack_size,
                                     uint32_t priority,
                                     uint32_t timeslice);

int qua_sys_thread_startup(qua_sys_thread_t thread);
int qua_sys_thread_join(qua_sys_thread_t thread);
int qua_sys_thread_delete(qua_sys_thread_t thread);

// 延时函数
void qua_sys_msleep(uint32_t ms);

// 文件系统接口
int qua_sys_stat(const char *path, struct stat *buf);
int qua_sys_mkdir(const char *pathname, mode_t mode);
int qua_sys_mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data);
int qua_sys_unmount(const char *target);

// 打印函数
int qua_sys_printf(const char *format, ...);

#endif // QUA_SYS_LCMD_ADAP_H
    