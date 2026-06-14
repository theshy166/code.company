/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * RTT用户层函数统一封装
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-25     chenjn       the first version
 *
 */
#ifndef __RTT_USR_OSAL_H__
#define __RTT_USR_OSAL_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef RPC_RTT
#define siginfo_t void
#endif
#include <rtthread.h>
#include <rtdevice.h>
#include <pthread.h>

#define fh_printf    rt_kprintf
#define fh_sprintf   rt_sprintf
#define fh_memcpy    rt_memcpy
#define fh_memset    rt_memset
#define fh_strcmp    rt_strcmp
#define fh_malloc    rt_malloc
#define fh_free      rt_free

typedef rt_device_t DEV_FD;
#define dev_exist(name)         (rt_device_find(name) != 0)
#define dev_ioctl(dev,cmd,data) \
({ \
	int _ret;\
	_ret = rt_device_control(dev,cmd,data);\
	_ret;\
})
#define dev_write(dev,buf,cnt) \
({ \
	int _ret;\
	_ret = rt_device_write(dev,0,buf,cnt);\
	_ret;\
})
#define dev_open(name,p1,p2)\
({\
	int _ret;\
	DEV_FD _dev;\
	_dev = rt_device_find(name);\
	if(_dev != RT_NULL)\
	{\
		_ret = rt_device_open(_dev,RT_DEVICE_OFLAG_RDWR);\
		if(_ret != 0)\
			_dev = RT_NULL;\
	}\
	_dev;\
})
#define dev_close(dev)          rt_device_close(dev)
#define dev_islegal(dev)        (dev != RT_NULL)

#define ERRNO  ({int _errno=rt_get_errno(); _errno?_errno:errs;})

#define FH_PRTVER                (0)

typedef struct rt_semaphore fh_mutex_t;
#define fh_mutex_init(mutex,name)   rt_sem_init(mutex,name,1,RT_IPC_FLAG_PRIO)
#define fh_mutex_destroy(mutex)     rt_sem_detach (mutex)
#define fh_mutex_lock(mutex)        rt_sem_take(mutex,RT_WAITING_FOREVER)
#define fh_mutex_trylock(mutex)     ((rt_sem_take(mutex,RT_WAITING_NO) == RT_EOK) ? 1 : 0)
#define fh_mutex_unlock(mutex)      rt_sem_release(mutex)

#ifdef RPC_RTT
#define __iomem
#define __force
static inline void __raw_writel(unsigned int w, volatile void __iomem *addr) {
    __asm__ __volatile__("st.di%U1	%0, %1\n" : : "r"(w), "m"(*(volatile unsigned int __force *) addr) : "memory");
}

static inline unsigned int __raw_readl(const volatile void __iomem *addr) {
    unsigned int w;

    __asm__ __volatile__("ld.di%U1	%0, %1\n" : "=r"(w) : "m"(*(volatile unsigned int __force *) addr) : "memory");
    return w;
}
#define readl __raw_readl
#define writel __raw_writel
#define fh_hw_writereg(addr,data)   writel(data, (volatile void *)addr)
#define fh_hw_readreg(addr)         readl((volatile void *)addr)
#define read_reg(addr)          fh_hw_readreg((unsigned int)addr)
#define write_reg(addr, reg)    fh_hw_writereg((unsigned int)addr,(unsigned int)reg)
#define read_char(addr)         fh_hw_readbyte((unsigned int)addr)
#define write_char(addr, val)   fh_hw_writebyte((unsigned int)addr,(unsigned char)val)
#else
#define read_reg(addr)           (*((volatile unsigned int *)(addr)))
#define write_reg(addr, reg)     (*((volatile unsigned int *)(addr))) = (unsigned int)(reg)
#define read_char(addr)          (*((volatile unsigned char *)(addr)))
#define write_char(addr, val)    (*((volatile unsigned char *)(addr))) = (unsigned char)(val)
#endif
#define usleep(us) rt_thread_mdelay(us / 1000)
#define fh_drvclose_ref_cnt_handle(refcnt)
#endif