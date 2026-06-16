/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * LINUX用户层函数统一封装
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-25     chenjn       the first version
 *
 */
#ifndef __LINUX_USR_OSAL_H__
#define __LINUX_USR_OSAL_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define fh_printf    printf
#define fh_sprintf   sprintf
#define fh_memcpy    memcpy
#define fh_memset    memset
#define fh_strcmp    strcmp
#define fh_malloc    malloc
#define fh_free      free

typedef int DEV_FD;
#define dev_exist(name)         (access(name,F_OK) == 0)
#define dev_ioctl(dev,cmd,data) ioctl(dev,cmd,data)
#define dev_open(name,p1,p2)    open(name,p1,p2)
#define dev_write(dev,buf,cnt)  write(dev,buf,cnt)
#define dev_close(dev)          close(dev)
#define dev_islegal(dev)        (dev >= 0)

#define ERRNO  ({int _errno=errno; _errno?_errno:errs;})

#ifndef AUTOTEST
#define FH_PRTVER                (1)
#else
#define FH_PRTVER                (0)
#endif

typedef pthread_mutex_t fh_mutex_t;
#define fh_mutex_init(mutex,name)   pthread_mutex_init(mutex,NULL)
#define fh_mutex_destroy(mutex)     pthread_mutex_destroy(mutex)
#define fh_mutex_lock(mutex)        pthread_mutex_lock(mutex)
#define fh_mutex_trylock(mutex)     pthread_mutex_trylock(mutex)
#define fh_mutex_unlock(mutex)      pthread_mutex_unlock(mutex)

#define read_reg(addr)           (*((volatile unsigned int *)(addr)))
#define write_reg(addr, reg)     (*((volatile unsigned int *)(addr))) = (unsigned int)(reg)
#define read_char(addr)          (*((volatile unsigned char *)(addr)))
#define write_char(addr, val)    (*((volatile unsigned char *)(addr))) = (unsigned char)(val)

#define fh_drvclose_ref_cnt_handle(refcnt)
#endif