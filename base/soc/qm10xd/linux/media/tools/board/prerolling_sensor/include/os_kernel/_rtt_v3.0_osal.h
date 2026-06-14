/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * RTT系统函数
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */
#ifndef __RTT_3_0_OSAL_H__
#define __RTT_3_0_OSAL_H__

#ifdef RPC_RTT
#define siginfo_t void
#endif
#include <rtthread.h>
#include <cconfig.h>
#include <rtdef.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <rthw.h>

extern void fh_get_chipid(rt_uint32_t *plat_id, rt_uint32_t *chip_id);
char *fh_get_chipname(void);
void mmu_clean_dcache(rt_uint32_t buffer, rt_uint32_t size);
extern void do_gettimeofday(struct timeval *tv);
extern unsigned int get_pts(void);
extern unsigned long long get_pts64(void);
extern unsigned int get_pts_rate(void);
char *rt_strpbrk(const char *s1, const char *s2);
char *rt_strsep(char **s1, const char *s2);
int rt_atoi(const char *nptr);
typedef FH_SINT32 fh_rtt_cmd;
#define IGNORE_UNUSE(val)       (void)(val);

//missing define in this os
typedef unsigned int bool;
#define EXPORT_SYMBOL(func)
#define MSEC_PER_SEC            (1000)
#define USEC_PER_MSEC           (1000)
#define USEC_PER_SEC            (1000000)
#define ARRAY_SIZE(a)           (sizeof(a) / sizeof((a)[0]))

#define STR_LEN(s)              rt_strlen(s)
#define STR_SEP(s,c)            rt_strsep(s,c)
#define STR_NCMP(s1,s2)         rt_strncmp(s1,s2,MAX(STR_LEN(s1),STR_LEN(s2)))
#define STR_CMP(s1,s2)          rt_strcmp(s1,s2)
#define STR2INT(s,pptr,offset)  rt_atoi(s)
#define STR2INT2(s)             rt_atoi(s)

/*special define*/
#define IRQ_RET
#define IRQ_RET_TYPE            void

/*print level*/
#define FH_PRT_ERR              0
#define FH_PRT_NOTICE           0
#define FH_PRT_DEBUG            0

/*printf*/
#define fh_printk(...)          rt_kprintf(__VA_ARGS__)
#define fh_prt_level(level,name,...) \
	do{ \
		fh_printk("[%s]%s %d: ",name , __func__, __LINE__); \
		fh_printk(__VA_ARGS__); \
	}while(0)
#define fh_prt_err(name,...)         fh_prt_level(FH_PRT_ERR,name,__VA_ARGS__)
#define fh_prt_notice(name,...)      fh_prt_level(FH_PRT_NOTICE,name,__VA_ARGS__)
#define fh_prt_debug(name,...)       //fh_prt_level(FH_PRT_DEBUG,name,__VA_ARGS__)
#define fh_prt_trace(name,tag,pts64) \
	fh_printk("[%s] %s=%llu\n",name,tag,pts64)

#ifdef RPC_RTT
#define seq_printf(fd,...)           fh_printk(__VA_ARGS__)
#endif

#define fh_trace_msg(msg,...)          fh_printk(msg,##__VA_ARGS__)

typedef va_list fh_va_list;
#define fh_va_start             va_start
#define fh_va_end               va_end
#define fh_vprintk              rt_kputs
#define fh_vsnprintf            rt_vsnprintf

typedef int fh_proc;

/*memory operate*/
#ifdef OS_USE_VBUS
#include "arc6xx.h"
#include "fh_vbus_dev.h"
typedef struct _fh_vbus_dev_ fh_rtdevice;
// memset for memory use by hw, arc need invalidate dcache
#define fh_memset(addr,val,size) \
do{ \
	rt_memset(addr,val,size); \
}while(0)
#define fh_memcpy(dst,src,size) \
do{ \
	rt_memcpy(dst,src,size); \
}while(0)
#define read_reg(addr)          fh_hw_readreg((unsigned int)addr)
#define write_reg(addr, reg)    fh_hw_writereg((unsigned int)addr,(unsigned int)reg)
#define read_char(addr)         fh_hw_readbyte((unsigned int)addr)
#define write_char(addr, val)   fh_hw_writebyte((unsigned int)addr,(unsigned char)val)
#else
typedef struct rt_device fh_rtdevice;
#define fh_sprintf(data,...)     rt_sprintf(data,__VA_ARGS__)
#define fh_snprintf(data,size,...) rt_snprintf(data,size,__VA_ARGS__)
#define fh_memset(addr,val,size) rt_memset(addr,val,size)
#define fh_memcpy(dst,src,size)  rt_memcpy(dst,src,size)
#ifdef RPC_RTT
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
#endif
#define write_reg_mask(addr, value, mask) \
	write_reg( addr, (read_reg(addr) & (~(mask))) | ((value) & (mask)) )

#define fh_memcmp(dst,src,size)  memcmp(dst,src,size)

#define _fh_mem_remap(addr,size) \
({ \
	rt_uint32_t _map = (rt_uint32_t)rt_ioremap((void*)addr, size);\
	(unsigned char*)_map; \
})
#define _fh_mem_unmap(addr)
#define _fh_malloc(size)         rt_malloc(size)
#define _fh_realloc(addr,size)   rt_realloc(addr,size)
#define _fh_free(addr)           rt_free(addr)
#define _fh_vmalloc(size)        rt_malloc(size)
#define _fh_vfree(addr)          rt_free(addr)

extern void mmu_clean_dcache(rt_uint32_t buffer, rt_uint32_t size);
extern void mmu_invalidate_dcache(rt_uint32_t buffer, rt_uint32_t size);
#define fh_flush_dcache(addr,size)      mmu_clean_dcache((unsigned int)(addr),size)
#define fh_invalidate_dcache(addr,size) mmu_invalidate_dcache((unsigned int)(addr),size)

/*mutex*/
typedef struct rt_semaphore fh_mutex_t;
#define _fh_mutex_init(mutex,name)  rt_sem_init(mutex,name,1,RT_IPC_FLAG_PRIO)
#define _fh_mutex_destroy(mutex)    rt_sem_detach (mutex)
#define fh_mutex_lock(mutex)        rt_sem_take(mutex,RT_WAITING_FOREVER)
#define fh_mutex_trylock(mutex)     ((rt_sem_take(mutex,RT_WAITING_NO) == RT_EOK) ? 1 : 0)
#define fh_mutex_unlock(mutex)      rt_sem_release(mutex)
#define fh_mutex_is_locked(mutex)   (((fh_mutex_t *)mutex)->value == 0)

typedef struct
{
	fh_mutex_t   mutex;
	unsigned int level;
}fh_spinlock_t;
#define _fh_spin_init(lock,name)   _fh_mutex_init(&((fh_spinlock_t *)lock)->mutex,name)
#define fh_spin_lock(lock) \
	do{ \
		((fh_spinlock_t *)lock)->level = rt_hw_interrupt_disable(); \
		fh_mutex_lock(&((fh_spinlock_t *)lock)->mutex); \
	}while(0)
#define fh_spin_unlock(lock) \
	do{ \
		fh_mutex_unlock(&((fh_spinlock_t *)lock)->mutex); \
		rt_hw_interrupt_enable(((fh_spinlock_t *)lock)->level); \
	}while(0)
#define _fh_spin_destroy(lock)    _fh_mutex_destroy(&((fh_spinlock_t *)lock)->mutex)

/*time*/
typedef unsigned int fh_timeval;
#define fh_gettime(time_value)      *time_value = rt_tick_get()
#define fh_timediff_us(curr_time,last_time) \
	(unsigned long)((unsigned long long)OVERFLOW_U32_INTERVAL(*(fh_timeval *)curr_time,*(fh_timeval *)last_time) \
	* USEC_PER_SEC / RT_TICK_PER_SECOND)

#define FH_SYS_HZ                   rt_tick_from_millisecond(MSEC_PER_SEC)
#define FH_SYS_TICK_NOW             rt_tick_get()

#define TIME_ACCURACY_MS            ((MSEC_PER_SEC/FH_SYS_HZ))
/*时间精度为tick 而非ms*/
#define MS2TICK(ms)                 MAX(1,ms / TIME_ACCURACY_MS)
#define msleep(delay)               rt_thread_delay(MAX(1,delay / TIME_ACCURACY_MS))

/*注意:此函数在RTT3中变更为会加锁,不能在中断等中调用*/
typedef struct timeval fh_systime;
#define fh_getsystime(time_value)    do_gettimeofday(time_value)
#define fh_getsec(time_value)        (unsigned int)((fh_systime *)time_value)->tv_sec
#define fh_getusec(time_value)       (unsigned int)((fh_systime *)time_value)->tv_usec

/*time2*/
typedef unsigned long long fh_logtime;
#define fh_getlogtime(time_value)     *time_value = rt_tick_get()
#define fh_getlogsec(time_value)      (unsigned int)((*time_value) / RT_TICK_PER_SECOND)
#define fh_getlogusec(time_value)     (unsigned int)(((*time_value) % RT_TICK_PER_SECOND) * 1000000 / RT_TICK_PER_SECOND)

/*timer*/
typedef struct rt_timer fh_timer_t;
#define fh_timer_init(timer,name,func,data) \
	rt_timer_init(timer,name,func,data,FH_SYS_HZ,RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_ONE_SHOT)
#define fh_timer_control(timer,func,dt,delaytime) \
	rt_timer_control(timer,RT_TIMER_CTRL_SET_TIME,delaytime)
#define fh_timer_start(timer)       rt_timer_start(timer)
#define fh_timer_stop(timer)        rt_timer_stop(timer)
#define fh_timer_delete(timer)      rt_timer_detach(timer)

/*waitqueue*/
/*rtt use event*/
typedef struct rt_event fh_wait_queue_t;
#define _fh_waitqueue_init(wait,name) \
	rt_event_init(wait, name, RT_IPC_FLAG_FIFO)
#define fh_waitqueue_wait(wait,flag,timeout) \
({ \
	int _ret = FH_TRUE; \
	int level; \
	level = rt_hw_interrupt_disable(); \
	if(!(flag)) \
	{ \
		(wait).set = 0; \
		rt_hw_interrupt_enable(level); \
		_ret = (RT_EOK == rt_event_recv(&wait,1,RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, \
			timeout,RT_NULL)); \
	} \
	else \
	{ \
		rt_hw_interrupt_enable(level); \
	} \
	_ret; \
})
#define fh_waitqueue_wakeup(wait) \
	rt_event_send(wait,1)
#define _fh_waitqueue_destroy(wait) \
	rt_event_detach(wait)

// driver on vbus, 3K's stack is enough, no value for pure RTT,
// assume 4K is enough.
#define dsp_thread_stack_size        (0x1000)
#define dsp_thread_priority          (9)

typedef rt_thread_t fh_thread;
#define fh_thread_run(func,data,name) do{ \
		rt_thread_t thread_t = rt_thread_create(name,func, data, \
			dsp_thread_stack_size, dsp_thread_priority, 20); \
		if (thread_t != RT_NULL) \
			rt_thread_startup(thread_t); \
	}while(0)
#define fh_thread_prio_highest()
#define fh_thread_prio_high()

//NOTE: RTT中暂时不使用修改优先级相关,如需要使用请此定义
#define FH_SCHED_NORMAL               0
#define FH_SCHED_FIFO                 0
#define FH_SCHED_RR                   0
#define FH_SCHED_DEFAULT              FH_SCHED_NORMAL
#define FH_SCHED_PRI_NORMAL           20
#define FH_SCHED_PRI_HIGH             15
#define FH_SCHED_PRI_HIGHEST          10
#define FH_SCHED_PRI_MAX              32
#define fh_thread_setscheduler(policy,priority)
#define fh_schedule()                 rt_thread_yield()

/*clk*/
#ifndef OS_USE_VBUS
#ifdef RTOSV3
typedef struct clk                 fh_clock;
#define fh_clk_get(name)           clk_get(NULL,name)
#define fh_clk_put(clk)
#define fh_clk_enable(clk)         clk_enable(clk)
#define fh_clk_disable(clk)        clk_disable(clk)
#define fh_clk_is_err(clk)         (clk == NULL)
#define fh_clk_set_rate(clk, rate) clk_set_rate(clk, rate)
#define fh_clk_get_rate(clk)       clk_get_rate(clk)
#else
extern struct fh_clk *clk_get(const char *name);
extern rt_uint32_t clk_set_rate(struct fh_clk *p_clk, rt_uint32_t rate_value);
extern rt_uint32_t clk_get_rate(struct fh_clk *p_clk);
extern int clk_enable(struct fh_clk *p_clk);
extern void clk_disable(struct fh_clk *p_clk);
typedef struct fh_clk              fh_clock;
#define fh_clk_get(name)           clk_get(name)
#define fh_clk_put(clk)
#define fh_clk_enable(clk)         clk_enable(clk)
#define fh_clk_disable(clk)        clk_disable(clk)
#define fh_clk_is_err(clk)         (clk == NULL)
#define fh_clk_set_rate(clk, rate) clk_set_rate(clk, rate)
#define fh_clk_get_rate(clk)       clk_get_rate(clk)
#endif
#endif

#define fh_media_get_chipid(platid,chipid)  fh_get_chipid((rt_uint32_t *)platid,(rt_uint32_t *)chipid)

#define IOCMD_DESC
#define fh_copy_from_usr(pst_drv, pst_usr) \
	fh_memcpy((void *)pst_drv,(const void *)pst_usr,sizeof(*(pst_drv)))
#define fh_copy_to_usr(pst_drv, pst_usr) \
	fh_memcpy((void *)pst_usr,(const void *)pst_drv,sizeof(*(pst_drv)))

#define fh_copy_from_usr_with_size(pst_drv, pst_usr, size) \
	fh_memcpy((void *)pst_drv,(const void *)pst_usr,size)
#define fh_copy_to_usr_with_size(pst_drv, pst_usr, size) \
	fh_memcpy((void *)pst_usr,(const void *)pst_drv,size)

//rtt下rt_device_close自身带refcnt,当被调用时说明refcnt已经被减为0了
#define fh_drvclose_ref_cnt_handle(refcnt) refcnt = (refcnt > 0 ? 1 : 0)
#endif