/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * LINUX系统函数
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */
#ifndef __LINUX_3_0_8_OSAL_H__
#define __LINUX_3_0_8_OSAL_H__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <mach/pmu.h>
#include <asm/cacheflush.h>
#include <asm/mach/map.h>
#include <../arch/arm/mach-fh/include/mach/system.h>

#define IGNORE_UNUSE(val)       (void)(val);

#define STR_LEN(s)              strlen(s)
#define STR_SEP(s,c)            strsep(s,c)
#define STR_NCMP(s1,s2)         strncasecmp(s1,s2,MAX(STR_LEN(s1),STR_LEN(s2)))
#define STR_CMP(s1,s2)          strcmp(s1,s2)
#define STR2INT(s,pptr,offset)  simple_strtol(s,pptr,offset)
#define STR2INT2(s) \
({ \
	int _val; \
	char *_tmp; \
	_val = simple_strtol(s,&_tmp,0); \
	_val; \
})

/*special define*/
#define IRQ_RET                 IRQ_HANDLED
#define IRQ_RET_TYPE            irqreturn_t

/*print level*/
#define FH_PRT_ERR              KERN_ERR
#define FH_PRT_NOTICE           KERN_NOTICE
#define FH_PRT_WARN             KERN_WARNING
#define FH_PRT_DEBUG            KERN_DEBUG

/*printf*/
#define fh_printk(...)          printk(__VA_ARGS__)
#define fh_prt_level(level,name,...) \
	do{ \
		fh_printk(level "[%s]%s %d: ",name , __func__, __LINE__); \
		fh_printk(__VA_ARGS__); \
	}while(0)
#define fh_prt_err(name,...)         fh_prt_level(FH_PRT_ERR,name,__VA_ARGS__)
#define fh_prt_notice(name,...)      fh_prt_level(FH_PRT_NOTICE,name,__VA_ARGS__)
#define fh_prt_debug(name,...)       fh_prt_level(FH_PRT_DEBUG,name,__VA_ARGS__)
#define fh_prt_trace(name,tag,pts64) \
	fh_printk(FH_PRT_DEBUG "[vcd]%llu,%s,%s\n",pts64,name,tag)
#define fh_prt_trace_idx(name,tag,pts64,idx)  \
	fh_printk(FH_PRT_DEBUG "[vcd]%llu,%s_BLK%d,%s\n",pts64,name,idx,tag)

#define fh_trace_msg(msg,...)          printk(FH_PRT_ERR msg,##__VA_ARGS__)

typedef va_list fh_va_list;
#define fh_va_start             va_start
#define fh_va_end               va_end
#define fh_vprintk(...)         fh_trace_msg("%s",__VA_ARGS__)
#define fh_vsnprintf            vsnprintf

typedef struct seq_file fh_proc;

/*memory operate*/
#define fh_sprintf(data,...)     sprintf(data,__VA_ARGS__)
#define fh_snprintf(data,size,...) snprintf(data,size,__VA_ARGS__)
#define fh_memset(addr,val,size) memset(addr,val,size)
#define fh_memcpy(dst,src,size)  memcpy(dst,src,size)
#define fh_memcmp(dst,src,size)  memcmp(dst,src,size)
#define read_reg(addr)          __raw_readl(((void *)addr))
#define write_reg(addr,val)     __raw_writel((val),((void *)addr))
#define read_char(addr)         __raw_readb(((void *)addr))
#define write_char(addr, val)   __raw_writeb((val),((void *)addr))
#define write_reg_mask(addr, value, mask) \
	write_reg( addr, (read_reg(addr) & (~(mask))) | ((value) & (mask)) )

#define _fh_mem_remap(addr,size) ioremap(addr,size)
#define _fh_mem_unmap(addr)      iounmap(addr)
#define _fh_malloc(size)         kmalloc(size,GFP_KERNEL)
#define _fh_realloc(addr,size)   krealloc(addr,size,GFP_KERNEL)
#define _fh_free(addr)           kfree(addr)
#define _fh_vmalloc(size)        vmalloc(size)
#define _fh_vfree(addr)          vfree(addr)

#define fh_flush_dcache(addr,size)      flush_kernel_vmap_range((void *)(addr),size)
#define fh_invalidate_dcache(addr,size) invalidate_kernel_vmap_range((void *)(addr),size)

/*mutex*/
typedef struct mutex fh_mutex_t;
#define _fh_mutex_init(mutex,name) mutex_init(mutex)
#define _fh_mutex_destroy(mutex)   mutex_destroy(mutex)
#define fh_mutex_lock(mutex)       mutex_lock(mutex)
#define fh_mutex_trylock(mutex)    mutex_trylock(mutex)
#define fh_mutex_unlock(mutex)     mutex_unlock(mutex)
#define fh_mutex_is_locked(mutex)  mutex_is_locked(mutex)

typedef struct
{
	spinlock_t    spinlock;
	unsigned long flags;
}fh_spinlock_t;
#define _fh_spin_init(lock,name) spin_lock_init(&((fh_spinlock_t *)lock)->spinlock)
#define fh_spin_lock(lock)       spin_lock_irqsave(&((fh_spinlock_t *)lock)->spinlock,((fh_spinlock_t *)lock)->flags)
#define fh_spin_unlock(lock)     spin_unlock_irqrestore(&((fh_spinlock_t *)lock)->spinlock,((fh_spinlock_t *)lock)->flags)
#define _fh_spin_destroy(lock)   IGNORE_UNUSE(lock)

/*time*/
typedef struct timeval fh_timeval;
#define fh_gettime(time_value)    do_gettimeofday(time_value)
#define fh_timediff_us(curr_time,last_time) \
	(((fh_timeval *)curr_time)->tv_sec - ((fh_timeval *)last_time)->tv_sec) * USEC_PER_SEC \
		+ ((fh_timeval *)curr_time)->tv_usec - ((fh_timeval *)last_time)->tv_usec;
#define FH_SYS_HZ                 HZ
#define FH_SYS_TICK_NOW           jiffies

#define TIME_ACCURACY_MS          ((MSEC_PER_SEC/FH_SYS_HZ))
#define MS2TICK(ms)               MAX(1,ms / TIME_ACCURACY_MS)

typedef struct timeval fh_systime;
#define fh_getsystime(time_value)    do_gettimeofday(time_value)
#define fh_getsec(time_value)        (unsigned int)((fh_systime *)time_value)->tv_sec
#define fh_getusec(time_value)       (unsigned int)((fh_systime *)time_value)->tv_usec

/*log time*/
typedef struct timeval fh_logtime;
#define fh_getlogtime(time_value)     do_gettimeofday(time_value)
#define fh_getlogsec(time_value)      (unsigned int)((fh_logtime *)time_value)->tv_sec
#define fh_getlogusec(time_value)     (unsigned int)((fh_logtime *)time_value)->tv_usec

/*timer*/
typedef struct timer_list fh_timer_t;
#define fh_timer_init(timer,name,func,data) init_timer(timer)
#define fh_timer_control(timer,func,dt,delaytime) \
	do{ \
		((fh_timer_t *)timer)->expires = FH_SYS_TICK_NOW + *delaytime; \
		((fh_timer_t *)timer)->function = (void *)func; \
		((fh_timer_t *)timer)->data = dt; \
	}while(0)
#define fh_timer_start(timer) \
	do{ \
		mod_timer(timer,((fh_timer_t *)timer)->expires); \
	}while(0)
#define fh_timer_stop(timer)      del_timer_sync(timer)
#define fh_timer_delete(timer)    del_timer_sync(timer)

/*waitqueue*/
typedef wait_queue_head_t fh_wait_queue_t;
#define _fh_waitqueue_init(wait,name) \
	init_waitqueue_head(wait)
#define fh_waitqueue_wait(wait,flag,timeout) \
	wait_event_interruptible_timeout(wait,flag,timeout)
#define fh_waitqueue_wakeup(wait) \
	wake_up_interruptible(wait)
#define _fh_waitqueue_destroy(wait)   IGNORE_UNUSE(wait)

/*thread*/
typedef struct task_struct fh_thread;
#define fh_thread_run(func,data,name) kthread_run(func,data,name)
#define fh_thread_stop(thread)        kthread_stop(thread)
#define fh_thread_prio_highest()      set_user_nice(current, -10)
#define fh_thread_prio_high()         set_user_nice(current, -5)

#define FH_SCHED_NORMAL               SCHED_NORMAL
#define FH_SCHED_FIFO                 SCHED_FIFO
#define FH_SCHED_RR                   SCHED_RR
#define FH_SCHED_DEFAULT              SCHED_RR
#define FH_SCHED_PRI_NORMAL           50
#define FH_SCHED_PRI_HIGH             55
#define FH_SCHED_PRI_HIGHEST          60
#define FH_SCHED_PRI_MAX              MAX_RT_PRIO
#define fh_thread_setscheduler(policy,priority) \
	fh_setscheduler(policy,priority)
#define fh_schedule()                 yield()

/*clk*/
typedef struct clk                  fh_clock;
#define fh_clk_get(name)            clk_get(NULL,name)
#define fh_clk_put(clk)             clk_put(clk)
#define fh_clk_enable(clk)          clk_enable(clk)
#define fh_clk_disable(clk)         clk_disable(clk)
#define fh_clk_is_err(clk)          IS_ERR(clk)
#define fh_clk_set_rate(clk,rate)   clk_set_rate(clk,rate)
#define fh_clk_get_rate(clk)        clk_get_rate(clk)

#define fh_irq_map(domain,hwirq) (hwirq - 32)
#define fh_irq_map_ex(domain,hwirq,add) (hwirq - add)

#define fh_media_get_chipid(platid,chipid)  fh_get_chipid(platid,chipid)

#define IOCMD_DESC __user
#define fh_copy_from_usr(pst_drv, pst_usr) \
	do{ \
		if(copy_from_user((pst_drv),(const void __user *)(pst_usr),sizeof(*(pst_drv)))) \
			return -EFAULT; \
	}while(0)
#define fh_copy_to_usr(pst_drv, pst_usr) \
	do{ \
		if(copy_to_user((void __user *)(pst_usr),(pst_drv),sizeof(*(pst_drv)))) \
			return -EFAULT; \
	}while(0)


#define fh_copy_from_usr_with_size(pst_drv, pst_usr,size) \
	do{ \
		if(copy_from_user((pst_drv),(const void __user *)(pst_usr),size)) \
			return -EFAULT; \
	}while(0)
#define fh_copy_to_usr_with_size(pst_drv, pst_usr,size) \
	do{ \
		if(copy_to_user((void __user *)(pst_usr),(pst_drv),size)) \
			return -EFAULT; \
	}while(0)

#define fh_drvclose_ref_cnt_handle(refcnt)
#endif