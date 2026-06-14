#ifndef __SYSLINK_H_
#define __SYSLINK_H_

#define MAX_PORTS	16
#define MAX_NODES	960
#define MAX_MSG_LEN	(4 * 1024)

#define MAX_EVENT_PORTS		16
#define MAX_PORT_EVENT_CNT	4

enum ops_mode {
	OPS_OPEN = 0,
	OPS_CLOSE,
	OPS_IOCTL,
	OPS_SELECT,
	OPS_READ,
	OPS_WRITE,
	OPS_TEST,
	OPS_CAT,
	OPS_HIK_OPEN,
    OPS_HIK_CLOSE,
	OPS_GET,
	OPS_MAX,
};

enum ops_result {
	RESULT_TRUE = 0,
	RESULT_FALSE = -1,
	RESULT_TIMEOUT = -2,
};

struct request_info {
	char port;
	char slot;
	short node;
	int ops;
	int ioctl_cmd;
	int param_num;
	int resp_size;
	unsigned int data[0];
};

struct response_info {
	char port;
	char slot;
	short node;
	int ops;
	int ioctl_cmd;
	int result;
	unsigned int data[0];
};

struct syslink_handle_attr {
	int port;
	int node;
	int priority;
};

struct syslink_timer {
	long timer;
	void *priv;
};
struct syslink_wait_event {
	long wait;
	long state;
};

int recv_task_init(void);
void recv_task_clean(void);

#define	IOC_SYSLINK_BASE  'M'

	/* Create a new ipcm handle. A file descriptor is only used*
	 * once for one ipcm handle. */
#define SYSLINK_IOC_CONNECT  \
		_IOW(IOC_SYSLINK_BASE, 1, struct syslink_handle_attr)
#define SYSLINK_IOC_TRY_CONNECT  \
		_IOW(IOC_SYSLINK_BASE, 2, struct syslink_handle_attr)

#define SYSLINK_IOC_DISCONNECT  \
		_IOW(IOC_SYSLINK_BASE, 4, unsigned long)

#define SYSLINK_IOC_ATTR_INIT \
		_IOW(IOC_SYSLINK_BASE, 8, struct syslink_handle_attr)

#endif

