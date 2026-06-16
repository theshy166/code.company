#ifndef __SYSLINK_H__
#define __SYSLINK_H__

#define SYSLINK_INT_SET          (0x1000)
#define SYSLINK_INT_CLR          (0x1004)
#define SYSLINK_INT_LCLR         (0x1008)
#define SYSLINK_INT_EN		     (0x100c)

typedef int (*recv_notifier)(void *handle, void *buf, unsigned int len);

struct syslink_mem_list {
	struct list_head head;
	void *data;
	unsigned int len;
};

struct syslink_dev_handle {
	unsigned long syslink_handle;
	struct list_head mem_list;
	wait_queue_head_t wait;
	spinlock_t lock;
	struct mutex mutex;
	char *kbuf;
	unsigned int buf_size;
};

struct syslink_transfer_head {
	volatile unsigned int target:6;
	volatile unsigned int source:6;
	volatile unsigned int node:10;
	volatile unsigned int type:10;
	volatile unsigned int len:20;
	volatile unsigned int reserve:12;
};

struct syslink_transfer_handle {
	unsigned int port;
	unsigned int node;
	recv_notifier recv;
	unsigned long data;
	int state;
	unsigned int priority;

	atomic_t send_count;
	atomic_t recv_count;
	atomic_t max_send_len;
	atomic_t max_recv_len;
	spinlock_t  lock;
};

struct mem_region {
	unsigned long phy_base;
	unsigned long base;
	unsigned int size;
};

struct syslink_shared_area {
	struct mem_region region;
	volatile char *data;
	unsigned int len;
	unsigned int lines;
	volatile unsigned int *rp;
	volatile unsigned int *wp;
	volatile unsigned int *np;
	volatile unsigned int *hp;
	spinlock_t		lock;
};

struct syslink_shared_zone {
	struct mem_region region;
	struct syslink_shared_area prio_area;
	struct syslink_shared_area normal_area;
};

struct syslink_node_data{
	char *buf;
	size_t count;
	struct completion done;
	wait_queue_head_t wq;
	int ready;
	int busy;
};


struct syslink_port {
	unsigned int id;
	unsigned int irq;
	void __iomem *regbase;
	struct syslink_shared_zone *sendbuf;
	struct syslink_shared_zone *recvbuf;
	struct syslink_shared_zone *eventbuf;
	int state;
	struct syslink_transfer_handle **handlers;
	struct syslink_node_data **data;
};

typedef struct syslink_opt {
	recv_notifier recv;
	unsigned long data;
} syslink_opt_t;

enum handle_priority {
	HANDLE_MSG_NORMAL,
	HANDLE_MSG_PRIORITY
};


#define syslink_info(s...)	do { \
	printk("<syslink>\r\n"); \
	printk(s); \
} while (0)

#define syslink_err(s...) do { \
	printk("<err>[%s:%d]\r\n", __func__, __LINE__); \
	printk(s); \
} while (0)

#define SYSLINK_PROC_NAME	"syslink"
#define PORT_MEM_SIZE		(1 << 19)
#define PORT_EVENT_MEM_SIZE	(1 << 10)

enum message_type {
	MESSAGE_NORMAL,
	MESSAGE_URGENT,
};
enum syslink_node_state {
	NODE_ALIVE,
	NODE_READY,
	NODE_HALT,
};

#define __DATA_ALIGN(end, align) ((end + align - 1) & (~(align - 1)))
#define __MSG_ALIGNED(end)       __DATA_ALIGN(end, 0x10)
#define __MSG_HEAD_SIZE          sizeof(struct syslink_transfer_head)
#define __MSG_JUMP_MARK__        0x166

#define MAX_TRANSFER_LEN 1048575
#ifndef PAGE_SHIFT
#define PAGE_SHIFT	12
#endif
#ifndef PAGE_SIZE
#define PAGE_SIZE	0x1000
#endif

#define ZONE_MAGIC	0x89765000

struct syslink_port *syslink_get_port(int id);
void __interrupt_trigger__(int port, void *regbase);

int syslink_shared_zone_init(int nid, unsigned long base,
			unsigned long pbase, unsigned long size, int sendbuf);
void *syslink_open(int port, int node, int priority);
void syslink_close(void *data);
int syslink_dev_recv(void *syslink_handle, void *buf, unsigned int len);
int syslink_setopt(void *data, struct syslink_opt *ops);
int syslink_getopt(void *data, struct syslink_opt *ops);
struct syslink_node_data *syslink_get_nodedata(int port, int node);
int syslink_set_nodedata(int port, int node, struct syslink_node_data *pdata);
int register_syslink_nodedata(int port, int node, void *pdata);

struct syslink_transfer_handle *get_handle(int port, int node);
int syslink_get_node(int port);
int syslink_sendmsg(int port, int node,
		const void *buf,
		unsigned int len);

int syslink_handle_init(int port);
void syslink_handle_release(int port, int node);

int syslink_event_register(void *udata, int port, int event_id,
	void (*callback)(void *udata, void *buf, unsigned int len));
int syslink_event_unregister(int port, int event_id);
int syslink_event_send(int port, int event_id, unsigned int data);
int syslink_kmsg_send(int port, int event_id, unsigned int *data, unsigned int length);

#endif
