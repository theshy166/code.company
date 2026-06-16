#ifndef __FY_OSAL__
#define __FY_OSAL__

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/uio.h>
#else
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <rtconfig.h>
#include <rthw.h>
#include <rtdef.h>
#include <mmu.h>
#endif

#define FY_OSAL_VERSION "1.1"

#include "osal_list.h"

#ifndef __KERNEL__

#ifndef KERN_EMERG
#define KERN_EMERG ""
#endif
#ifndef KERN_ALERT
#define KERN_ALERT ""
#endif
#ifndef KERN_CRIT
#define KERN_CRIT ""
#endif
#ifndef KERN_ERR
#define KERN_ERR ""
#endif
#ifndef KERN_WARNING
#define KERN_WARNING ""
#endif
#ifndef KERN_NOTICE
#define KERN_NOTICE ""
#endif
#ifndef KERN_INFO
#define KERN_INFO ""
#endif
#ifndef KERN_DEBUG
#define KERN_DEBUG ""
#endif

#endif

#ifndef __KERNEL__
// rtos
#define __user
#define loff_t unsigned long long
#define fmode_t unsigned
//#define bool int
#define u64 unsigned long long
#define phys_addr_t unsigned long
#define __must_check
#define __force

#define IS_ERR(ptr)    ((unsigned long)(ptr) > (unsigned long)(-1000))

//#define ARRAY_SIZE(v) (sizeof(v)/sizeof((v)[0]))
#endif

#define fhosal_readb(x) (*((volatile unsigned char *)(x)))
#define fhosal_writeb(v, x) (*((volatile unsigned char *)(x)) = (v))
#define fhosal_readl(x) (*((volatile int *)(x)))
#define fhosal_writel(v, x) (*((volatile int *)(x)) = (v))

#define fhosal_gfp_kernel 0
#define fhosal_gfp_atomic 1

//atomic api
typedef struct {
    void *atomic;
}fhosal_atomic_t;

#define OSAL_ATOMIC_INIT(i)  { (i) }

//semaphore api
#define EINTR 4
typedef struct fhosal_semaphore{
    void * sem;
}fhosal_semaphore_t;

//mutex api
typedef struct fhosal_mutex{
    void * mutex;
}fhosal_mutex_t;

//spin lock api
typedef struct fhosal_spinlock{
    void * lock;
}fhosal_spinlock_t;

//wait api
typedef int (*fhosal_wait_cond_func_t)(void *param);

typedef struct fhosal_wait{
   void *wait;
}fhosal_wait_t;

#define ERESTARTSYS 512

//workqueue api
typedef struct fhosal_work_struct {
    void *work;
    void (*func)(struct fhosal_work_struct *work);
} fhosal_work_struct_t;
typedef void (*fhosal_work_func_t)(struct fhosal_work_struct *work);

struct wq_node {
	struct fhosal_work_struct *fhosal_work;
	struct work_struct *work;
	struct fhosal_list_head node;
};

//interrupt api
typedef enum fhosal_irqreturn {
    OSAL_IRQ_NONE = (0 << 0),
    OSAL_IRQ_HANDLED = (1 << 0),
    OSAL_IRQ_WAKE_THREAD = (1 << 1),
}fhosal_irqreturn_t;

typedef int (*fhosal_irq_handler_t)(int, void *);

#define OSAL_DIS_IRQ_CNT 2
typedef void (*fhosal_gic_handle_t)(unsigned int, unsigned int, void*);

//task api
typedef struct fhosal_task{
    void *task_struct;
}fhosal_task_t;
typedef int (*threadfn_t)(void *data);
struct fhosal_sched_param {
	int sched_priority;
};

//tasklet api
typedef struct fhosal_tasklet{
    void *tasklet_struct;
}fhosal_tasklet_t;

//
typedef struct fhosal_completion {
    void *completion;
}fhosal_completion_t;

#define OSAL_VERIFY_READ  0
#define OSAL_VERIFY_WRITE 1

typedef int (*fhosal_dma_filter_fn)(void *chan, void *filter_param);

#ifdef __KERNEL__
#define fhosal_warn_on(cond) WARN_ON(cond)
#else
#define fhosal_warn_on(cond)
#endif

#define OSAL_BUG() do { } while (1)

//proc
typedef struct fhosal_proc_dir_entry{
     char name[50];                                       /* proc name eg.vi vo vpss */
     void *proc_dir_entry;                                /* proc directory eg./proc/umap/ /dev/logmpp/ */
     int (*open)(struct fhosal_proc_dir_entry *entry);    /* open */
     int (*read)(struct fhosal_proc_dir_entry *entry);
     int (*write)(struct fhosal_proc_dir_entry *entry, const char *buf, int count, long long *); /* write, NULL */
     void *private;                                       /* private */
     void *seqfile;
     struct fhosal_list_head node;
}fhosal_proc_entry_t;

//device api
#ifndef _IOC_TYPECHECK
#ifndef LIBC_SYS_IOCTL_H
#include "osal_ioctl.h"
#endif
#endif

typedef struct fhosal_vm{
    void *vm;
}fhosal_vm_t;

typedef struct fhosal_vm_area{
    void *vm_area_struct;
}fhosal_vm_area_t;

#define OSAL_POLLIN      0x0001
#define OSAL_POLLPRI     0x0002
#define OSAL_POLLOUT     0x0004
#define OSAL_POLLERR     0x0008
#define OSAL_POLLHUP     0x0010
#define OSAL_POLLNVAL    0x0020
#define OSAL_POLLRDNORM  0x0040
#define OSAL_POLLRDBAND  0x0080
#define OSAL_POLLWRNORM  0x0100

typedef struct fhosal_poll{
    void *poll_table;
    void *data;
}fhosal_poll_t;

#ifdef __KERNEL__
typedef struct fhosal_fileops{
    int (*open)(void *private_data);
    int (*read)(char *buf, int size, long *offset, void *private_data);
    int (*write)(const char *buf, int size, long *offset, void *private_data);
    int (*write_iter)(void* _kiocb, void* _iov_iter, void *private_data);
    long (*llseek)(long offset, int whence, void *private_data);
    int (*release)(void *private_data);
    long (*unlocked_ioctl) (unsigned int cmd, unsigned long arg, void *private_data);
    unsigned int (*poll) (fhosal_poll_t *fhosal_poll, void *private_data);
    int (*mmap) (fhosal_vm_t *vm, unsigned long start, unsigned long end, unsigned long vm_pgoff, void *private_data);
}fhosal_fileops_t;
#else
typedef struct fhosal_fileops {
	int (*open) (void *inode, void *file);
	int (*read) (void *file, char *, unsigned int, unsigned long long *);
	int (*write) (void *file, const char *, unsigned int, unsigned long long *);
	long (*llseek) (void *file, unsigned long long, int);
	int (*release) (void *inode, void *file);
	long (*unlocked_ioctl) (void *file, unsigned int, unsigned long);
	int (*mmap) (void *file, void *vm_area_struct);
	unsigned int (*poll) (void *file, void *poll_table_struct);
}fhosal_fileops_t;
#endif

typedef struct fhosal_dev{
    char name[48];
    void *dev;
    int minor;
#ifdef __KERNEL__
    struct fhosal_fileops *fops;
    struct fhosal_pmops *fhosal_pmops;
#else
	fhosal_fileops_t *fops;
	void *sys_fops;
	void *misc_dev;
#endif
}fhosal_dev_t;

typedef struct fhosal_pmops{
    int (*pm_prepare)(fhosal_dev_t *dev);
    void (*pm_complete)(fhosal_dev_t *dev);
    int (*pm_suspend)(fhosal_dev_t *dev);
    int (*pm_resume)(fhosal_dev_t *dev);
    int (*pm_freeze)(fhosal_dev_t *dev);
    int (*pm_thaw)(fhosal_dev_t *dev);
    int (*pm_poweroff)(fhosal_dev_t *dev);
    int (*pm_restore)(fhosal_dev_t *dev);
    int (*pm_suspend_late)(fhosal_dev_t *dev);
    int (*pm_resume_early)(fhosal_dev_t *dev);
    int (*pm_freeze_late)(fhosal_dev_t *dev);
    int (*pm_thaw_early)(fhosal_dev_t *dev);
    int (*pm_poweroff_late)(fhosal_dev_t *dev);
    int (*pm_restore_early)(fhosal_dev_t *dev);
    int (*pm_suspend_noirq)(fhosal_dev_t *dev);
    int (*pm_resume_noirq)(fhosal_dev_t *dev);
    int (*pm_freeze_noirq)(fhosal_dev_t *dev);
    int (*pm_thaw_noirq)(fhosal_dev_t *dev);
    int (*pm_poweroff_noirq)(fhosal_dev_t *dev);
    int (*pm_restore_noirq)(fhosal_dev_t *dev);
}fhosal_pmops_t;

#define OSAL_SEEK_SET 0
#define OSAL_SEEK_CUR 1
#define OSAL_SEEK_END 2

#define PAGE_SHIFT 12

//timer
typedef struct fhosal_timer{
    void *timer;
    void (*function)(unsigned long);
    unsigned long data;
}fhosal_timer_t;

typedef struct fhosal_timeval{
      long tv_sec;
      long tv_usec;
}fhosal_timeval_t;

typedef struct fhosal_rtc_time {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
} fhosal_rtc_time_t;

#define OSAL_O_ACCMODE       00000003
#define OSAL_O_RDONLY        00000000
#define OSAL_O_WRONLY        00000001
#define OSAL_O_RDWR          00000002
#define OSAL_O_CREAT         00000100

// reboot
struct fhosal_notifier_block {
    int (*notifier_call)(struct fhosal_notifier_block *nb, unsigned long action, void *data);
    void *notifier_block;
};
typedef int (*fhosal_notifier_fn_t)(struct fhosal_notifier_block *nb, unsigned long action, void *data);

#include <stdarg.h>

#ifndef _OSAL_VA_LIST
#define _OSAL_VA_LIST

#define fhosal_va_list         va_list
#define fhosal_va_arg(ap, T)   va_arg(ap,T)
#define fhosal_va_end(ap)      va_end(ap)
#define fhosal_va_start(ap, A) va_start(ap,A)

#endif  /* va_arg */

#define NULL_STRING "NULL"

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
#ifndef OSAL_UMH_WAIT_PROC
#define OSAL_UMH_WAIT_PROC   2       /* wait for the process to complete */
#endif
#endif

//fifo
typedef struct fhosal_fifo{
	void*  fifo;
}fhosal_fifo_t;

typedef struct fhosal_event{
	void *event;
}fhosal_event_t;

#define OSAL_KERN_SOH	"\001"		/* ASCII Start Of Header */
#define OSAL_KERN_SOH_ASCII	'\001'

#define OSAL_KERN_EMERG	OSAL_KERN_SOH "0"	/* system is unusable */
#define OSAL_KERN_ALERT	OSAL_KERN_SOH "1"	/* action must be taken immediately */
#define OSAL_KERN_CRIT	OSAL_KERN_SOH "2"	/* critical conditions */
#define OSAL_KERN_ERR	OSAL_KERN_SOH "3"	/* error conditions */
#define OSAL_KERN_WARNING	OSAL_KERN_SOH "4"	/* warning conditions */
#define OSAL_KERN_NOTICE	OSAL_KERN_SOH "5"	/* normal but significant condition */
#define OSAL_KERN_INFO	OSAL_KERN_SOH "6"	/* informational */
#define OSAL_KERN_DEBUG	OSAL_KERN_SOH "7"	/* debug-level messages */

#define OSAL_KERN_DEFAULT	OSAL_KERN_SOH "d"	/* the default kernel loglevel */

/*
 * Annotation for a "continued" line of log printout (only done after a
 * line that had no enclosing \n). Only to be used by core/arch code
 * during early bootup (a continued line is not SMP-safe otherwise).
 */
#define OSAL_KERN_CONT	OSAL_KERN_SOH "c"

typedef struct fhosal_seqops {
	void * (*start) (void *m, loff_t *pos);
	void (*stop) (void *m, void *v);
	void * (*next) (void *m, void *v, loff_t *pos);
	int (*show) (void *m, void *v);
}fhosal_seqops_t;

typedef struct fhosal_seqcontext{
	void * seq_context;
}fhosal_seqcontext_t;

extern void *fhosal_vmalloc(unsigned long size);
extern void  fhosal_vfree(const void *addr);
extern void *fhosal_kmalloc(unsigned long size, unsigned int fhosal_gfp_flag);
extern void  fhosal_kfree(const void * addr);
extern void *fhosal_kcalloc(unsigned long n, unsigned long size, unsigned int fhosal_gfp_flag);
extern void *fhosal_kzalloc(unsigned long size, unsigned int fhosal_gfp_flag);
extern void *fhosal_devm_kmalloc(void *dev, unsigned long size, unsigned int gfp);
extern void  fhosal_devm_kfree(void *dev, void *p);
extern void *fhosal_devm_kmalloc(void *dev, unsigned long size, unsigned int gfp);
extern void *fhosal_devm_ioremap(void *dev, unsigned long offset, unsigned long size);
extern void *fhosal_devm_ioremap_nocache(void *dev, unsigned long offset, unsigned long size);
extern void *fhosal_devm_kzalloc(void *dev, unsigned int size);
extern void *fhosal_devm_ioremap_resource(void *dev,  const void *res);

extern int  fhosal_atomic_init(fhosal_atomic_t *atomic);
extern void fhosal_atomic_destroy(fhosal_atomic_t *atomic);
extern int  fhosal_atomic_read(fhosal_atomic_t *v);
extern void fhosal_atomic_set(fhosal_atomic_t *v, int i);
extern int  fhosal_atomic_inc_return(fhosal_atomic_t *v);
extern int  fhosal_atomic_dec_return(fhosal_atomic_t *v);
extern int fhosal_atomic_dec_and_test(fhosal_atomic_t *atomic);

extern int fhosal_sema_init(fhosal_semaphore_t *sem, int val);
extern int fhosal_down(fhosal_semaphore_t *sem);
extern int fhosal_down_interruptible(fhosal_semaphore_t *sem);
extern int fhosal_down_trylock(fhosal_semaphore_t *sem);
extern int fhosal_down_timeout(fhosal_semaphore_t *sem, long ms);
extern void fhosal_up(fhosal_semaphore_t *sem);
extern void fhosal_sema_destroy(fhosal_semaphore_t *sem); //notice:must be called when kmod exit, other wise will lead to memory leak;

extern int fhosal_mutex_init(fhosal_mutex_t *mutex);
extern int fhosal_mutex_lock(fhosal_mutex_t *mutex);
extern int fhosal_mutex_lock_interruptible(fhosal_mutex_t *mutex);
extern int fhosal_mutex_trylock(fhosal_mutex_t *mutex);
extern int fhosal_mutex_is_locked(fhosal_mutex_t *mutex);
extern void fhosal_mutex_unlock(fhosal_mutex_t *mutex);
extern void fhosal_mutex_destroy(fhosal_mutex_t *mutex); //notice:must be called when kmod exit, other wise will lead to memory leak;

extern int fhosal_spin_lock_init(fhosal_spinlock_t *lock);
extern void fhosal_spin_lock(fhosal_spinlock_t *lock);
extern int fhosal_spin_trylock(fhosal_spinlock_t *lock);
extern void fhosal_spin_unlock(fhosal_spinlock_t *lock);
extern void fhosal_spin_lock_irqsave(fhosal_spinlock_t *lock, unsigned long *flags);
extern int fhosal_spin_trylock_irqsave(fhosal_spinlock_t *lock, unsigned long *flags);
extern void fhosal_spin_unlock_irqrestore(fhosal_spinlock_t *lock, unsigned long *flags);
extern void fhosal_spin_lock_destroy(fhosal_spinlock_t *lock); //notice:must be called when kmod exit, other wise will lead to memory leak;
extern void fhosal_spin_lock_bh(fhosal_spinlock_t *lock);
extern int fhosal_spin_trylock_bh(fhosal_spinlock_t *lock);
extern void fhosal_spin_unlock_bh(fhosal_spinlock_t *lock);

extern unsigned long fhosal_msecs_to_jiffies(const unsigned int m);
extern int fhosal_wait_init(fhosal_wait_t * wait);
extern int fhosal_wait_interruptible(fhosal_wait_t * wait, fhosal_wait_cond_func_t func, void* param);
extern int fhosal_wait_uninterruptible(fhosal_wait_t * wait, fhosal_wait_cond_func_t func, void* param);
extern int fhosal_wait_timeout_interruptible(fhosal_wait_t * wait, fhosal_wait_cond_func_t func, void* param, unsigned long jiff);
extern int fhosal_wait_timeout_uninterruptible(fhosal_wait_t * wait, fhosal_wait_cond_func_t func, void* param, unsigned long jiff);

extern signed long fhosal_schedule_timeout(signed long timeout);
void fhosal_schedule(void);
void fhosal_prepare_to_wait(fhosal_wait_t *wq, void *wait, int state);
void fhosal_finish_wait(fhosal_wait_t *wq, void *wait);
int fhosal_signal_pending_current(void);

extern void fhosal_wakeup(fhosal_wait_t * wait);
extern void fhosal_wake_up_all(fhosal_wait_t * wait);
extern void fhosal_wake_up_interruptible(fhosal_wait_t * wait);
extern void fhosal_wait_destroy(fhosal_wait_t * wait);

extern int fhosal_init_work(struct fhosal_work_struct *work, fhosal_work_func_t func);

extern int fhosal_schedule_work(struct fhosal_work_struct *work);
extern void fhosal_destroy_work(struct fhosal_work_struct *work);

//shedule
extern void fhosal_yield(void);

//extern int fhosal_request_irq(unsigned int irq, fhosal_irq_handler_t handler, const char *name, void *dev);
extern int fhosal_request_threaded_irq(unsigned int irq, fhosal_irq_handler_t handler, fhosal_irq_handler_t thread_fn, const char *name, void *dev);
extern int fhosal_devm_request_irq(void *dev, unsigned int irq, fhosal_irq_handler_t handler, unsigned long irqflags, const char *devname, void
*dev_id);

extern void fhosal_free_irq(unsigned int irq, void *dev);
extern void fhosal_disable_irq(unsigned int irq);
extern void fhosal_enable_irq(unsigned int irq);
extern int fhosal_in_interrupt(void);

extern int fhosal_register_gic_handle(unsigned int index, unsigned int irq, fhosal_gic_handle_t handle,const char *name, void *dev);
extern int fhosal_unregister_gic_handle(unsigned int index,unsigned int irq, void* dev);

extern fhosal_task_t *fhosal_kthread_create(threadfn_t thread,void *data, char *name);
extern void fhosal_kthread_destroy(fhosal_task_t *task, unsigned int stop_flag);
extern void fhosal_kthread_bind(fhosal_task_t *task, unsigned int cpu);
extern int fhosal_kthread_should_stop(void);
extern int fhosal_wake_up_process(fhosal_task_t *task);
extern int fhosal_kthread_stop(fhosal_task_t *task);
extern int fhosal_sched_setscheduler(void *task, int policy, void *param);

extern int fhosal_tasklet_init(fhosal_tasklet_t *tasklet, void (*func)(unsigned long), unsigned long data);
extern void fhosal_tasklet_kill(fhosal_tasklet_t *tasklet);
extern void fhosal_tasklet_schedule(fhosal_tasklet_t *tasklet);
extern void fhosal_tasklet_hi_schedule(fhosal_tasklet_t *tasklet);

extern int fhosal_get_current_tgid(void);
extern int fhosal_get_current_pid(void);

extern int fhosal_completion_init(fhosal_completion_t *completion);
extern void fhosal_completion_destroy(fhosal_completion_t *ocompletion);
extern void fhosal_wait_for_completion(fhosal_completion_t *ocompletion);
extern unsigned long fhosal_wait_for_completion_timeout(fhosal_completion_t *ocompletion, unsigned long jiff);
extern long fhosal_wait_for_completion_interruptible(fhosal_completion_t *ocompletion);
extern long fhosal_wait_for_completion_interruptible_timeout(fhosal_completion_t *ocompletion, unsigned long jiff);
extern void fhosal_complete(fhosal_completion_t *completion);
extern void fhosal_reinit_completion(fhosal_completion_t *ocompletion);

//string api
extern int fhosal_printk(const char *fmt, ...);
extern char * fhosal_strcpy(char *s1,const char *s2);
extern char * fhosal_strncpy(char *s1,const char *s2, int size);
extern int fhosal_strlcpy(char *s1, const char *s2, int size);
extern char * fhosal_strcat(char *s1, const char *s2);
extern char * fhosal_strncat(char *s1, const char *s2, int size);
extern int fhosal_strlcat(char *s1, const char *s2, int size);
extern int fhosal_strcmp(const char *s1,const char *s2);
extern int fhosal_strncmp(const char *s1,const char *s2,int size);
extern int fhosal_strnicmp(const char *s1, const char *s2, int size);
extern int fhosal_strcasecmp(const char *s1, const char *s2);
extern int fhosal_strncasecmp(const char *s1, const char *s2, int n);
extern char * fhosal_strchr(const char *s,int n);
extern char * fhosal_strnchr(const char *s , int count, int c);
extern char * fhosal_strrchr(const char *s,int c);
extern char * fhosal_skip_spaces(const char *s);
extern char *fhosal_strim(char *s);
extern char *fhosal_strstrip(char *str);
extern char * fhosal_strstr(const char *s1, const char *s2);
extern char * fhosal_strnstr(const char *s1, const char *s2, int n);
extern int fhosal_strlen(const char *s);
extern int fhosal_strnlen(const char *s,int size);
extern char * fhosal_strpbrk(const char *s1,const char *s2);
extern char * fhosal_strsep(char **s,const char *ct);
extern int fhosal_strspn(const char *s,const char *accept);
extern int fhosal_strcspn(const char *s,const char *reject);
extern void * fhosal_memset(void *str,int c,int count);
extern void * fhosal_memcpy(void *s1,const void *s2,int count);
extern void * fhosal_memmove(void *s1,const void *s2,int count);
extern void * fhosal_memscan(void *addr, int c, int size);
extern int fhosal_memcmp(const void *cs,const void *ct,int count);
extern void * fhosal_memchr(const void *s,int c,int n);
extern void * fhosal_memchr_inv(const void *s, int c, int n);

//extern char *fhosal_itoa(int n, char * buf);

extern int fhosal_kstrtol(const char *s, unsigned int base, long *res);
extern unsigned long long fhosal_strtoull(const char *cp, char **endp, unsigned int base);
extern unsigned long fhosal_strtoul(const char *cp, char **endp, unsigned int base);
extern long fhosal_strtol(const char *cp, char **endp, unsigned int base);
extern long long fhosal_strtoll(const char *cp, char **endp, unsigned int base);
extern int fhosal_snprintf(char *buf, int size, const char *fmt, ...);
extern int fhosal_scnprintf(char *buf, int size, const char *fmt, ...);
extern int fhosal_sprintf(char *buf, const char *fmt, ...);
extern int fhosal_sscanf(const char *buf, const char *fmt, ...);

//addr translate
extern void *fhosal_ioremap(unsigned long phys_addr, int size);
extern void *fhosal_ioremap_nocache(unsigned long phys_addr, int size);
extern void *fhosal_ioremap_cached(unsigned long phys_addr, int size);
#ifdef __KERNEL__
extern void fhosal_iounmap(void *addr);
#else
extern void fhosal_iounmap(void *addr, int size);
#endif
extern void *fhosal_arch_ioaddress(void *addr);
extern void *fhosal_ioaddress(int addr);
extern unsigned long fhosal_vm_mmap(void *filp, unsigned long addr,
    unsigned long len, unsigned long prot,
    unsigned long flag, unsigned long offset);
extern int fhosal_vm_munmap(unsigned long start, unsigned long len);
extern unsigned long fhosal_pa_high_memory(void);
extern unsigned long fhosal_usr_virt_to_phys(unsigned long virt);
extern unsigned long fhosal_phys_offset(void);
extern void* fhosal_find_vma(unsigned long addr);
extern int fhosal_is_vmalloc_addr(const void *x);

extern unsigned long fhosal_copy_from_user(void *to, const void *from, unsigned long n);
extern unsigned long fhosal_copy_to_user(void *to, const void *from, unsigned long n);
extern unsigned long fhosal_copy_from_iter(void *addr, unsigned long bytes, void* i);

extern int fhosal_access_ok(int type, const void *addr, unsigned long size);

//cache api
extern void fhosal_flush_cache_all(void);
extern void fhosal_cpuc_flush_dcache_area(void *addr, int size);
extern void fhosal_dma_map_area(void *addr, int size, int dir);

extern void fhosal_flush_dcache_area(void *kvirt, unsigned long phys_addr, unsigned long length);
int fhosal_flush_dcache_all(void);

void fhosal_dma_release_channel(void *chan);

void *fhosal_dma_request_channel(const void *mask,  fhosal_dma_filter_fn fn, void *fn_param);
int fhosal_dmaengine_terminate_sync(void *chan);
void fhosal_dma_cyclic_free(void *chan);
void *fhosal_dma_alloc_coherent(void *dev, int size, void *dma_handle, int flag);
void fhosal_dma_free_coherent(void *dev, int size, void *cpu_addr, int dma_handle);

//math
extern unsigned int fhosal_do_div(unsigned int n, unsigned int base);
extern unsigned long long fhosal_do_div64(unsigned long long n, unsigned int base);
extern unsigned long long fhosal_do_div_rem64(unsigned long long n, unsigned int base);

extern long long fhosal_do_div64_s64(long long dividend, long long divisor);
extern unsigned long long fhosal_do_div64_u64(unsigned long long dividend, unsigned long long divisor);
extern unsigned int fhosal_iter_div_u64_rem(unsigned long long  dividend, unsigned int divisor, unsigned long long  *remainder);

//barrier
extern void fhosal_mb(void);
extern void fhosal_rmb(void);
extern void fhosal_wmb(void);
extern void fhosal_smp_mb(void);
extern void fhosal_smp_rmb(void);
extern void fhosal_smp_wmb(void);
extern void fhosal_isb(void);
extern void fhosal_dsb(void);
extern void fhosal_dmb(void);

extern void fhosal_panic(const char *fmt, const char * file, const char * fun, int line, const char *);

extern fhosal_proc_entry_t *fhosal_create_proc_entry(const char *name, fhosal_proc_entry_t *parent);
extern fhosal_proc_entry_t *fhosal_proc_mkdir(const char *name, fhosal_proc_entry_t *parent);
extern void fhosal_remove_proc_entry(const char *name, fhosal_proc_entry_t *parent);
extern int fhosal_seq_printf(fhosal_proc_entry_t *entry, const char *fmt, ...);
extern fhosal_proc_entry_t *fhosal_create_proc_private(const char *name, fhosal_proc_entry_t *parent);
extern void fhosal_remove_proc_private(const char *name, fhosal_proc_entry_t *parent);

extern fhosal_dev_t *fhosal_createdev(char *name);
extern int fhosal_destroydev(fhosal_dev_t *pdev);
extern int fhosal_registerdevice(fhosal_dev_t *pdev);
extern void fhosal_deregisterdevice(fhosal_dev_t *pdev);
extern void fhosal_poll_wait(fhosal_poll_t *table, fhosal_wait_t * wait);
extern void fhosal_pgprot_noncached(fhosal_vm_t *vm);
extern void fhosal_pgprot_cached(fhosal_vm_t *vm);
extern void fhosal_pgprot_writecombine(fhosal_vm_t *vm);
extern void fhosal_pgprot_stronglyordered(fhosal_vm_t *vm);
extern int fhosal_remap_pfn_range(fhosal_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size);
extern int fhosal_io_remap_pfn_range(fhosal_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size);
extern int fhosal_misc_register(void *dev);
extern void fhosal_misc_deregister(void *dev);
extern void *fhosal_register_sysctl_table(void *table);
extern void fhosal_unregister_sysctl_table(void * table);
extern void *fhosal_bus_find_device_by_name(void *bus, void *start, const char *name);
extern void fhosal_put_device(void *dev);
extern void *fhosal_dev_get_platdata(void *dev);
extern void* fhosal_to_spi_device(void *dev);

extern int fhosal_timer_init(fhosal_timer_t *timer);
extern int fhosal_set_timer(fhosal_timer_t *timer, unsigned long interval);//ms
extern int fhosal_del_timer(fhosal_timer_t *timer);
extern int fhosal_timer_destroy(fhosal_timer_t *timer);

extern unsigned long fhosal_msleep(unsigned int msecs);
extern void fhosal_udelay(unsigned int usecs);
extern void fhosal_mdelay(unsigned int msecs);
extern void fhosal_usleep_range(unsigned long min, unsigned long max);

extern unsigned int fhosal_get_tickcount(void);
extern unsigned long long fhosal_sched_clock(void);
extern void fhosal_gettimeofday(fhosal_timeval_t *tv);
extern void fhosal_rtc_time_to_tm(unsigned long time, fhosal_rtc_time_t *tm);
extern void fhosal_rtc_tm_to_time(fhosal_rtc_time_t *tm ,unsigned long *time);
extern int fhosal_rtc_valid_tm(struct fhosal_rtc_time *tm);
extern void fhosal_getjiffies(unsigned long long *pjiffies);
extern int fhosal_is_enable_hrtimer(void);
extern void fhosal_ktime_get_ts(void *ts);

extern void *fhosal_klib_fopen(const char *filename, int flags, int mode);
extern void fhosal_klib_fclose(void *filp);
extern int fhosal_klib_fwrite(char *buf, int len, void *filp);
extern int fhosal_klib_fread(char *buf, unsigned int len, void *filp);
extern int fhosal_klib_nonseekable_open(void *inode, void *filp);

extern int fhosal_register_reboot_notifier(struct fhosal_notifier_block *nb);
extern int fhosal_unregister_reboot_notifier(struct fhosal_notifier_block *nb);

extern int fhosal_vsnprintf(char *str, int size, const char *fmt, fhosal_va_list args);
extern int fhosal_vscnprintf(char *buf, int size, const char *fmt, fhosal_va_list args);

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
extern int fhosal_call_usermodehelper_force(char *path, char **argv, char **envp, int wait);
#endif

extern int fhosal_pfn_valid(unsigned int pfn);

int fhosal_platform_driver_register(void *drv);
void fhosal_platform_driver_unregister(void *drv);
int fhosal_platform_device_register(void *pdev);
void fhosal_platform_device_unregister(void *pdev);
void* fhosal_platform_get_resource_byname(void *dev, unsigned int type,
    const char *name);
void* fhosal_platform_get_resource(void *dev, unsigned int type,
    unsigned int num);
int fhosal_platform_get_irq(void *dev, unsigned int num);
int fhosal_platform_get_irq_byname(void *dev, const char *name);
void *fhosal_platform_get_drvdata(void *pdev);
void fhosal_platform_set_drvdata(void *pdev, void *data);

int fhosal_register_framebuffer(void *fb_info);
int fhosal_unregister_framebuffer(void *fb_info);
void *fhosal_framebuffer_alloc(int size, void *dev);
void fhosal_framebuffer_release(void *info);
int fhosal_fb_alloc_cmap(void *cmap, int len, int transp);

//cdev
void fhosal_cdev_init(void *dev, const void *fop);
void *fhosal_cdev_alloc(void);
int fhosal_cdev_add(void *p, int dev, unsigned count);
void fhosal_cdev_del(void *p);

//of APIs
void *fhosal_of_find_node_by_name(void *from, const char *name);
int fhosal_of_get_named_gpio_flags(void *np, const char *list_name, int index, int *flags);
int fhosal_of_get_named_gpio(void *np, const char *propname, int index);
int fhosal_of_property_read_u32(void *np, char *propname, unsigned int *value);
int fhosal_of_alias_get_id(void *np, const char *stem);
void *fhosal_of_device_get_match_data(void *dev);

//gpio
void *fhosal_gpio_to_desc(unsigned gpio);
int fhosal_desc_to_gpio(void *desc);
int fhosal_gpio_direction_output(unsigned gpio, int value);
int fhosal_gpio_direction_input(unsigned gpio);
void fhosal_gpio_set_value(unsigned gpio, int value);
int fhosal_gpio_get_value(unsigned gpio);

//spi
void *fhosal_spi_busnum_to_master(unsigned short busnum);
void *fhosal_get_spi_bus_type(void);
int fhosal_spi_sync(void *spi, void *data, int len, int bpw);
int fhosal_spi_write_then_read(void* spi, void* buff, int len, void*p, int t);

//i2c
void *fhosal_i2c_new_device(void *adap, void const *info);
void *fhosal_i2c_get_adapter(int nr);
void fhosal_i2c_put_adapter(void *adap);
int fhosal_i2c_transfer(void *adap, void *msgs, int num);
void fhosal_i2c_unregister_device(void *dev);
void* fhosal_devm_regmap_init_i2c(void *dev, const void* config);
void fhosal_i2c_set_clientdata(void *dev, void *data);

//clk
void *fhosal_devm_clk_get(void *dev, const char *id);
int fhosal_clk_enable(void *clk);
int fhosal_clk_prepare(void *clk);
void fhosal_clk_unprepare(void *clk);
int fhosal_alloc_chrdev_region(void *dev, unsigned baseminor, unsigned count,
                			const char *name);
void fhosal_unregister_chrdev_region(int from, unsigned count);
void fhosal_unregister_chrdev(unsigned int major, const char *name);

//regmap
int fhosal_regmap_write(void *map, unsigned int reg, unsigned int val);
int fhosal_regmap_read(void *map, unsigned int reg, unsigned int *val);
int fhosal_regmap_update_bits(void *map, unsigned int reg, unsigned int mask, unsigned int val);

void *fhosal_device_create(void *cls, void *parent, int devt, void *drvdata, const char *fmt);
void fhosal_device_destroy(void *cls, int devt);

void* fhosal_class_create(void *owner, const char *name);
void fhosal_class_destroy(void *cls);

int fhosal_reset_control_assert(void *rstc);
int fhosal_reset_control_deassert(void *rstc);
void *fhosal_devm_reset_control_get_optional_exclusive(void *dev, const char *id);

int  fhosal_kfifo_alloc(fhosal_fifo_t* ofifo, int size);
int  fhosal_kfifo_free(fhosal_fifo_t* ofifo);
int  fhosal_kfifo_size(fhosal_fifo_t* ofifo);
int  fhosal_kfifo_len(fhosal_fifo_t* ofifo);
int  fhosal_kfifo_is_empty(fhosal_fifo_t* ofifo);
int  fhosal_kfifo_is_full(fhosal_fifo_t* ofifo);
int  fhosal_kfifo_in(fhosal_fifo_t * ofifo,const void * buf,unsigned int len);
int  fhosal_kfifo_out(fhosal_fifo_t * ofifo,void * buf,unsigned int len);
int  fhosal_kfifo_in_spinlocked(fhosal_fifo_t* ofifo,const void *buf, unsigned int len,fhosal_spinlock_t* spinlock);
int  fhosal_kfifo_out_spinlocked(fhosal_fifo_t* ofifo, void *buf, unsigned int len,fhosal_spinlock_t* spinlock);
void fhosal_kfifo_reset(fhosal_fifo_t* ofifo);

#ifdef __KERNEL__
extern int fhosal_request_irq(unsigned int irq, fhosal_irq_handler_t handler, const char *name, void *dev);
#endif

#define fhosal_max(x, y) ({                            \
        __typeof__(x) _max1 = (x);                  \
        __typeof__(y) _max2 = (y);                  \
        (void) (&_max1 == &_max2);              \
        _max1 > _max2 ? _max1 : _max2; })

#define fhosal_min(x, y) ({                \
    __typeof__(x) _min1 = (x);          \
     __typeof__(y) _min2 = (y);          \
     (void) (&_min1 == &_min2);      \
     _min1 < _min2 ? _min1 : _min2; })

#define fhosal_abs(x) ({                       \
     long ret;                   \
     if (sizeof(x) == sizeof(long)) {        \
         long __x = (x);             \
         ret = (__x < 0) ? -__x : __x;       \
     } else {                    \
         int __x = (x);              \
         ret = (__x < 0) ? -__x : __x;       \
     }                       \
     ret;                        \
})

//dev_wait_event-->wait_event_interruptible
#define fhosal_wait_event_interruptible(wait, func, param)    \
({                                  \
    int __ret = 0;                          \
                                          \
    for (;;){                          \
        if(func(param)){                       \
            __ret = 0;                  \
            break;                    \
        }\
        __ret = fhosal_wait_interruptible(wait, (func), param);   \
        if(__ret < 0)                 \
            break;           \
    }                                    \
    __ret;                                   \
})

#define fhosal_wait_event_uninterruptible(wait, func, param)    \
({                                  \
    int __ret = 0;                          \
                                          \
    for (;;){                          \
        if(func(param)){                       \
            __ret = 0;                  \
            break;                    \
        }\
        __ret = fhosal_wait_uninterruptible(wait, (func), param);   \
        if(__ret < 0)                 \
            break;           \
    }                                    \
    __ret;                                   \
})

#define fhosal_wait_event_timeout_interruptible(wait, func, param, timeout)             \
({                                  \
    int __ret = timeout;                          \
                                        \
    if ((func(param)) && !timeout) \
    { \
    __ret = 1; \
    } \
                                          \
    for (;;) {                          \
        if (func(param))                       \
        {\
            __ret = fhosal_msecs_to_jiffies(__ret);     \
            break;                    \
        }\
        __ret = fhosal_wait_timeout_interruptible(wait, (func), param, __ret);   \
        if(!__ret || __ret == -ERESTARTSYS)   \
            break;                    \
    }                                   \
    __ret;                                   \
})

#define fhosal_wait_event_timeout_uninterruptible(wait, func, param, timeout)             \
		({									\
			int __ret = timeout;						  \
												\
			if ((func(param)) && !timeout) \
			{ \
			__ret = 1; \
			} \
												  \
			for (;;) {							\
				if (func(param))					   \
				{\
					__ret = fhosal_msecs_to_jiffies(__ret);	  \
					break;					  \
				}\
				__ret = fhosal_wait_timeout_uninterruptible(wait, (func), param, __ret);	 \
				if(!__ret || __ret == -ERESTARTSYS)   \
					break;					  \
			}									\
			__ret;									 \
		})

#define OSAL_INIT_WORK(_work, _func) \
do {  \
    fhosal_init_work((_work), (_func));           \
} while (0)

#define fhosal_kthread_run(threadfn, data, namefmt, ...)			   \
({	\
    char __tmpname[128]; \
	struct fhosal_task *__k = NULL; \
	fhosal_sprintf(__tmpname, namefmt, ##__VA_ARGS__); \
	__k = fhosal_kthread_create(threadfn, data, __tmpname); \
	if (!IS_ERR(__k))						   \
		fhosal_wake_up_process(__k);					   \
	__k;								   \
})

//debug
#ifdef __KERNEL__

#define fhosal_pr_emerg(fmt, ...) \
	fhosal_printk(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_pr_alert(fmt, ...) \
	fhosal_printk(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_pr_crit(fmt, ...) \
	fhosal_printk(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_pr_err(fmt, ...) \
	fhosal_printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_pr_warning(fmt, ...) \
	fhosal_printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_pr_warn fhosal_pr_warning
#define fhosal_pr_notice(fmt, ...) \
	fhosal_printk(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_pr_info(fmt, ...) \
	fhosal_printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)


#define fhosal_dev_err(dev, fmt, ...) \
    fhosal_printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_dev_info(dev, fmt, ...)  \
    fhosal_printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
#define fhosal_dev_dbg(dev, fmt, ...)  \
    fhosal_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)

#else

#define fhosal_pr_emerg(fmt, ...) \
	fhosal_printk(KERN_EMERG "[emerg]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_alert(fmt, ...) \
	fhosal_printk(KERN_ALERT "[alert]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_crit(fmt, ...) \
	fhosal_printk(KERN_CRIT "[crit]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_err(fmt, ...) \
	fhosal_printk(KERN_ERR "[err]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_warning(fmt, ...) \
	fhosal_printk(KERN_WARNING "[warning]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_warn(fmt, ...) \
	fhosal_printk(KERN_WARNING "[warn]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_notice(fmt, ...) \
	fhosal_printk(KERN_NOTICE "[notice]:" fmt, ##__VA_ARGS__)
#define fhosal_pr_info(fmt, ...) \
	fhosal_printk(KERN_INFO "[info]:" fmt, ##__VA_ARGS__)

#endif

#define OSAL_ASSERT(expr)               \
do{                                   \
    if (!(expr)) {                    \
        fhosal_printk("\nASSERT failed at:\n"\
               "  >Condition: %s\n",  \
                #expr);\
        OSAL_BUG();\
    } \
}while(0)

#define OSAL_BUG_ON(expr)  \
do{\
    if(expr){\
        fhosal_printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
        OSAL_BUG();\
    }\
}while(0)

#define fhosal_module_driver(fhosal_driver, fhosal_register, fhosal_unregister, ...) \
static int __init fhosal_driver##_init(void) \
{ \
        return fhosal_register(&(fhosal_driver)); \
} \
module_init(fhosal_driver##_init); \
static void __exit fhosal_driver##_exit(void) \
{ \
        fhosal_unregister(&(fhosal_driver)); \
} \
module_exit(fhosal_driver##_exit);

#define fhosal_module_platform_driver(platform_driver) \
        fhosal_module_driver(platform_driver, fhosal_platform_driver_register, \
                        fhosal_platform_driver_unregister)

#endif
