#ifndef _VOU_OSAL_
#define _VOU_OSAL_

//#define REGBUF
#define VIDEO_FAST_LOAD_BUFFER (1)
#define GUI_FAST_LOAD_BUFFER (1)

#if defined(__KERNEL__)

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/iommu.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/firmware.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sizes.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_reserved_mem.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/random.h>
#include <linux/semaphore.h>
#include <linux/poll.h>
#include <linux/list.h>

uint64_t get_timestamp(void);

#define printk_none(...)

#define printk_debug(fmt, ...)                                          \
    do {                                                                \
        fhosal_printk(KERN_DEBUG "vou:"fmt, ##__VA_ARGS__);                    \
    } while(0)

#define printk_notice(fmt, ...)                                         \
    do {                                                                \
        fhosal_printk(KERN_NOTICE "vou:"fmt, ##__VA_ARGS__);                   \
    } while(0)

#define printk_err(fmt, ...)                                            \
    do {                                                                \
        fhosal_printk(KERN_ERR "vou:"fmt, ##__VA_ARGS__);                      \
    } while(0)

#define printf(...)
#define off_pos_t				loff_t
#define fhosal_div				do_div

#define REG_VOU_BASE                            0x26000000
#define REG_DSI_BASE                            0x25c00000
#define REG_DPHY_BASE                           0x25d00000
#define REG_DMC_SYS_APB_REG_BASE                0x24000000

#elif defined(X86_PC)

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define inline

#define module_param(a,b,c)

#define get_timestamp() 0
#define jiffies 0
#define fhosal_msecs_to_jiffies(a) 0
#define time_after(a,b) 0

#define __iomem
#define BIT(__n)       (1<<(__n))

//#define udelay usleep
//#define fhosal_msleep(a) usleep(a*1000)
#define get_random_int random

#define INT_VOU_DHD_NUM 109
#define INT_VOU_DSD_NUM 110

#define REG_TOP_APB_REG_BASE                    0X28000000
#define REG_TOP_CLK_CFG_BASE                    0X28100200
#define REG_ANALOG_BASE                         0X28500000
#define REG_VIO_APB_REG_BASE                    0X34000000
#define REG_VOU_BASE                            0X34100000
#define REG_HDMI_BASE                           0X34200000
#define REG_CVBS_BASE                           0X35400000
#define REG_DSI_BASE                            0x34300000
#define REG_DPHY_BASE                           0x34500000

#include <pthread.h>

#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct work_struct;

typedef void (*work_func_t)(struct work_struct *work);

struct work_struct {
	pthread_t tid;
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	work_func_t func;
	bool run;
};

void _init_work(struct work_struct *work, work_func_t func, char *name);
#define INIT_WORK(work, func) _init_work(work, func, #func)
bool schedule_work(struct work_struct *work);

struct mutex {
	pthread_mutex_t native;
};

struct semaphore {
	pthread_mutex_t native;
};

typedef struct {
	pthread_mutex_t native;
} spinlock_t;

int fhosal_mutex_init(struct mutex *lock);
int fhosal_mutex_destroy(struct mutex *lock);
int fhosal_mutex_lock(struct mutex *lock);
int fhosal_mutex_unlock(struct mutex *lock);
#define mutex_is_locked(m) true

void fhosal_sema_init(struct semaphore *sem, int val);
#define down_timeout(sem, timeout) fhosal_down(sem)
int fhosal_down(struct semaphore *sem);
void fhosal_up(struct semaphore *sem);

int fhosal_spin_lock_init(spinlock_t *lock);
int fhosal_spin_lock_bh(spinlock_t *lock);
int fhosal_spin_unlock_bh(spinlock_t *lock);

#define fhosal_printk printf
#define printk_debug printf
#define printk_notice printf
#define printk_err printf
#define printk_none

#elif defined(MINICODE)

#define REG_TOP_APB_REG_BASE REG_GLB_APB_BASE
#define REG_TOP_CLK_CFG_BASE REG_TOP_PRE_DIV_RF_BASE
#define REG_VIO_APB_REG_BASE REG_VIO_SYS_APB_REG_BASE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define module_param(a,b,c)

#define get_timestamp() 0
#define jiffies 0
#define fhosal_msecs_to_jiffies(a) 0
#define time_after(a,b) 0

#define __iomem
#define BIT(__n)       (1<<(__n))

#define ARRAY_SIZE(v) (sizeof(v)/sizeof((v)[0]))

#define fhosal_msleep
uint32_t get_random_int();

#include "mmap.h"

#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct work_struct {};
typedef void (*work_func_t)(struct work_struct *work);

#define _init_work
#define INIT_WORK
#define schedule_work

struct mutex {};
struct semaphore {};
typedef struct {} spinlock_t;

#define fhosal_mutex_init
#define fhosal_mutex_destroy
#define fhosal_mutex_lock
#define fhosal_mutex_unlock
#define mutex_is_locked(m) true

#define fhosal_sema_init
#define down_timeout
#define fhosal_down
#define fhosal_up

#define fhosal_spin_lock_init
#define fhosal_spin_lock_bh
#define fhosal_spin_unlock_bh

#define fhosal_printk printf
#define printk_debug printf
#define printk_notice printf
#define printk_err printf
#define printk_none(...)
#define fhosal_sprintf(...)
#else
#include <string.h>
#include <stdint.h>
#include <rtthread.h>
#include <stdio.h>
#include "list.h"

#define get_timestamp()				0
#define jiffies					0
#define time_after(a,b)				0
#define off_pos_t				rt_off_t
#define fhosal_div				lldiv

typedef unsigned char bool;

#define __iomem
/*
#define REG_TOP_APB_REG_BASE                    0X28000000
#define REG_TOP_CLK_CFG_BASE                    0X28100200
#define REG_ANALOG_BASE                         0X28500000
#define REG_VIO_APB_REG_BASE                    0X34000000
#define REG_VOU_BASE                            0X34100000
#define REG_HDMI_BASE                           0X34200000
#define REG_CVBS_BASE                           0X35400000
#define REG_VOU_QOS_BASE                        0X2C100B00
#define REG_DSI_BASE                            0x34300000
#define REG_DPHY_BASE                           0x34500000
#define REG_AVE_BASE                            0x35500000
*/
#define REG_DMC_SYS_APB_REG_BASE                0x24000000
#define REG_VOU_BASE                            0x26000000
#define REG_DSI_BASE                            0x25c00000
#define REG_DPHY_BASE                           0x25d00000

#define VOU_DHD_IRQ				84//55
#define VOU_DHD1_IRQ			83//55
#define VOU_DSD_IRQ				88//56
#endif

#define dump_layer_level(level, context, devId, layerId, fmt, ...)                                                   \
	do {                                                                                            \
		level("[%d]{%d}"fmt",4b=0x%x/0x%x/0x%x/%s,5p=%s/%s/%s/%s/%s!\n",                       \
				devId, layerId, ##__VA_ARGS__,                                          \
				context->layer[devId][layerId].updated,                                 \
				context->layer[devId][layerId].loaded,                                  \
				context->layer[devId][layerId].locked,                                  \
				context->layer[devId][layerId].eb ? "eb" : "-",                         \
				context->regsets[devId][layerId].initialized ? "on" : "off",            \
				context->regsets[devId][layerId].pending_wait_shadow ? "wait" : "-",    \
				!context->regsets[devId][layerId].pending_wait_shadow ? "-" :           \
				(context->regsets[devId][layerId].wait_shadow_layer_eb ? "on" : "off"), \
				context->regsets[devId][layerId].pending_shadow_update ? "update" : "-",\
				context->regsets[devId][layerId].pending_switch_off ? "close" : "-");   \
	} while(0)

#define dump_layer_e(context, devId, layerId, fmt, ...) do { dump_layer_level(fhosal_pr_err, context, devId, layerId, fmt, ##__VA_ARGS__); } while (0)
#define dump_layer_m(context, devId, layerId, fmt, ...) do { dump_layer_level(fhosal_pr_info, context, devId, layerId, fmt, ##__VA_ARGS__); } while (0)
#define dump_layer_d(context, devId, layerId, fmt, ...) //do { dump_layer_level(fhosal_pr_info, context, devId, layerId, fmt, ##__VA_ARGS__); } while (0)

#define ASSERT_DUMP_LAYER(qualif, context, devId, layerId)                                \
	if ( !(qualif) ) {                                                                \
		dump_layer_e(context, devId, layerId, "%s(), %s:%d, assertion failed: '%s'",\
				__FUNCTION__, __FILE__, __LINE__, #qualif );              \
		ASSERT(0);                                                                \
	}

#define printx fhosal_pr_err
#define printe fhosal_pr_err
#define printm fhosal_pr_info
#define printd if(0)fhosal_pr_info
#define IN_D do { if(0)fhosal_pr_info("%s.s\n", __FUNCTION__); } while(0)
#define OUT_D do { if(0)fhosal_pr_info("%s.e\n", __FUNCTION__); } while(0)

#define ALIGN_TO(x, align) ( (((unsigned int)(x)) + (align) - 1) & (~((align) - 1)) )
#define ALIGN_DOWN_TO(x, align) ( ((unsigned int)(x)) & (~((align) - 1)) )

#ifndef ZERO_FILL_STRUCT
#define ZERO_FILL_STRUCT(v)                                    \
    do {                                                        \
        (void)fhosal_memset(&(v), 0, sizeof(v));                         \
    } while (0)
#endif /* ZERO_FILL_STRUCT */

#ifndef MEMCPY_STRUCT
#define MEMCPY_STRUCT(to,from)                                    \
    do {                                                        \
        (void)fhosal_memcpy(&(to), &(from), sizeof(from));                 \
    } while (0)
#endif /* MEMCPY_STRUCT */

#ifndef FREE_POINTER
#define FREE_POINTER(pointer)                                   \
    do {                                                        \
        if (pointer) {                                          \
            free(pointer);                                      \
            pointer = NULL;                                     \
        }                                                       \
    } while (0)
#endif /* FREE_POINTER */


#ifndef ZERO_FILL_POINTER
#define ZERO_FILL_POINTER(v)                                    \
    do {                                                        \
        (void)fhosal_memset(v, 0, sizeof(*v));                         \
    } while (0)
#endif /* ZERO_FILL_POINTER */


#ifndef MALLOC_POINTER
#define MALLOC_POINTER(v)                                       \
    do {                                                        \
        v = (typeof(v))malloc(sizeof(*v));                      \
    } while (0)
#endif /* MALLOC_POINTER */

#ifndef ASSERT
#define ASSERT(qualif)                                                                          \
    if ( !(qualif) ) {                                                                          \
        fhosal_pr_err("%s(), %s:%d, assertion failed: '%s', abort\n", __FUNCTION__, __FILE__, __LINE__, #qualif ); \
        *(volatile int*)0 = 0;                                                                  \
    }
#endif /* ASSERT */

#define CLIP(_x, _min, _max) ((_x) > (_max) ? (_max) : ((_x) < (_min) ? (_min) : (_x)))

#define ABORT(failMsg, ...)                                                                 \
    do {                                                                                        \
        fhosal_pr_err("%s(), %s:%d, \"" failMsg "\", abort!\n", __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
        *(volatile int*)0 = 0;                                                                  \
    } while(0)

#define ALIGN_BYTE(x,n)		(x >> n)<<n

typedef enum {
	VOU_REGBASE_IDX_TOPAPB = 0,
	VOU_REGBASE_IDX_TOPCLK,
	VOU_REGBASE_IDX_ANALOG,
	VOU_REGBASE_IDX_BASE,
	VOU_REGBASE_IDX_VIOAPB,
	VOU_REGBASE_IDX_CVBS,
	VOU_REGBASE_IDX_HDMI,
	VOU_REGBASE_IDX_QOS,
	VOU_REGBASE_IDX_DSI,
	VOU_REGBASE_IDX_DPHY,
	VOU_REGBASE_IDX_COEF,
	VOU_REGBASE_IDX_AVE,
	VOU_REGBASE_IDX_DMCSYSAPB,
	VOU_REGBASE_IDX_NUM,
} vou_reg_base_idx;

typedef struct {
	void __iomem *base[VOU_REGBASE_IDX_NUM];
	uint32_t phyAddr[VOU_REGBASE_IDX_NUM];
} vou_reg_base_t;

typedef struct {
	uint8_t addr;
	uint8_t value;
} i2c_reg_t;

void vou_i2c_write(uint8_t dev_addr, i2c_reg_t *i2c_regs_p, uint32_t cnt);

uint32_t vou_read_register(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset);
uint32_t vou_read_register_range(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint32_t start, uint32_t bit);
void vou_write_register(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint32_t value);
void vou_write_register_setbit(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint32_t value);
void vou_write_register_clrbit(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint32_t value);
void vou_write_register_range(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit);

uint8_t vou_read_register_byte(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset);
void vou_write_register_byte(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint8_t value);
void vou_write_register_byte_setbit(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint8_t value);
void vou_write_register_byte_clrbit(vou_reg_base_t *regbase, vou_reg_base_idx idx, uint32_t base_addr, uint32_t offset, uint8_t value);
int vou_set_pxp_cen_clk(void);
int vou_set_clock_div(void);

#ifndef __KERNEL__
void set_bit(int nr, volatile unsigned long *addr);
int test_bit(int nr, const volatile unsigned long *addr);
int test_and_set_bit(int nr, volatile unsigned long *addr);
int test_and_clear_bit(int nr, volatile unsigned long *addr);
void clear_bit(int nr, volatile unsigned long *addr);
#endif

#endif
