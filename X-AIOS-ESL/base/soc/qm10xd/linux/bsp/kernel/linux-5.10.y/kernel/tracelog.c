/*
 *  tracelog.c
 *  Authored by yuanhong.he@molchip.com
 *  The tracelog function is designed to aid in the debugging process for various kernel components such as the scheduler,
 *  Interrupt requests (IRQs),and soft IRQs. It provides detailed task information and outputs it to the console.
 *  Making it easier to analyze and diagnose system behavior and performance issues.
 *
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/threads.h>
#include <linux/sched/debug.h>
#include <linux/regmap.h>


static char * mc_log_buf;
static volatile unsigned int r_pos = 0;
static volatile unsigned int w_pos = 0;

#define TRACELOG_MAGIC			0xd2409

#define LOGBUF_SIZE			(CONFIG_TRACELOG_SIZE_KB * SZ_1K)

#define PREROLLING_RAM 		CONFIG_PREROLLING_RAM
DEFINE_SPINLOCK(tracelog_lock);

struct record_node
{
	unsigned int magic0;
	unsigned int r_pointer_current;
	unsigned int w_pointer_current;
	unsigned int buf_size;
} __packed;

static char * _trace_pos_start;

#define LOGBUF_LENGTH 	(LOGBUF_SIZE - sizeof(struct record_node))

#define R_ROD(t) \
	do {		 \
		struct record_node *n = (struct record_node *)mc_log_buf; \
		n->r_pointer_current = t;	\
	}while(0)

#define W_ROD(t) \
	do {		 \
		struct record_node *n = (struct record_node *)mc_log_buf; \
		n->w_pointer_current = t;	\
	}while(0)

static int _trace_log_record(struct seq_file *sfile, struct record_node *n)
{
	n->magic0 = TRACELOG_MAGIC;
	n->r_pointer_current = r_pos;
	n->w_pointer_current = w_pos;
	n->buf_size = LOGBUF_SIZE;

	return 0;
}

static bool is_logbuf_empty(void)
{
	if (r_pos == w_pos)
		return true;

	return false;
}

static bool is_logbuf_full(void)
{
	if (((w_pos + 1) % LOGBUF_LENGTH) == r_pos)
		return true;

	return false;
}

static void mc_log_buf_putc(char c)
{
	if(is_logbuf_full()) {
		r_pos = (r_pos + 1) % LOGBUF_LENGTH;
		R_ROD(r_pos);
	}

	_trace_pos_start[w_pos] = c;
	w_pos = (w_pos + 1) % LOGBUF_LENGTH;
}

static int mc_log_buf_getc(char *pc)
{
	if(is_logbuf_empty())
		return 0;

	*pc = _trace_pos_start[r_pos];
	r_pos = (r_pos + 1) % LOGBUF_LENGTH;

	return 1;
}

int mcprintk(const char *fmt, ...)
{
    va_list args;
    int n,i;
	unsigned long flags;
	char tmp_buf[128];

    va_start(args, fmt);
    n = vsnprintf(tmp_buf, sizeof(tmp_buf), fmt, args);
    va_end(args);

	spin_lock_irqsave(&tracelog_lock, flags);

	for(i = 0; i < n; i++) {
		mc_log_buf_putc(tmp_buf[i]);
	}

	W_ROD(w_pos);
	spin_unlock_irqrestore(&tracelog_lock, flags);

    return n;
}
EXPORT_SYMBOL(mcprintk);

/********************** debug proc **********************/
static int mcprintk_proc_show(struct seq_file *m, void *v)
{
	unsigned long flags;
	unsigned int itr=0;
	char c;

	spin_lock_irqsave(&tracelog_lock, flags);
	if(is_logbuf_empty()) {
		spin_unlock_irqrestore(&tracelog_lock, flags);
		return 0;
	}

	itr = r_pos;
	if( is_logbuf_full() )
		seq_putc(m,_trace_pos_start[w_pos]);

	while(mc_log_buf_getc(&c))
		seq_putc(m,c);
	r_pos = itr;

	spin_unlock_irqrestore(&tracelog_lock, flags);
	return 0;
}

int mc_printk_init(void)
{
	phys_addr_t phys_addr;
	unsigned long flags;

	if (!mc_log_buf)
	{
#ifdef CONFIG_TRACELOG_USE_IRAM_MEM
		mc_log_buf = (unsigned char *)ioremap(PREROLLING_RAM, LOGBUF_SIZE);
		phys_addr = PREROLLING_RAM - LOGBUF_SIZE;
#else
		mc_log_buf = kmalloc(LOGBUF_SIZE, GFP_KERNEL);
		phys_addr = virt_to_phys(mc_log_buf);
#endif
		if (!mc_log_buf) {
			printk("init log buf memory error!!\n");
			return -ENOMEM;
		}
		else
		{
			printk(">>>>>>>> init mc log memory success!: va(%08lx) pa(%08lx).\n", (unsigned long)mc_log_buf, (unsigned long)phys_addr);
			memset((void*)mc_log_buf, 0, LOGBUF_SIZE);
		}
	}

	_trace_pos_start = (char *)(mc_log_buf + sizeof(struct record_node));
	spin_lock_irqsave(&tracelog_lock, flags);
	_trace_log_record(NULL,(struct record_node *)mc_log_buf);
	spin_unlock_irqrestore(&tracelog_lock, flags);
	proc_create_single("mckmsg", 0, NULL, mcprintk_proc_show);

	mcprintk("trace log debug:%s-%d \r\n",__func__,__LINE__);

	return 0;
}

core_initcall(mc_printk_init);
