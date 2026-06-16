
#if 1//def XBUS_RPC
#include "rpc_hdr.h"
//#include "xbus_cmd.h"
#include "xbus_protocol.h"
#include "xbus_api.h"

#include "fh_common.h"
#include "fy_osal.h"

#include "rpc_slave.h"

#define PROC_PORT 2

struct RPCS_PROC_RDWR
{
	struct rpc_hdr hdr;  /** rpc hdr */
	char name[64];
	int buflen;
	int retlen;
	unsigned long addr;
};

struct RPCS_PROC_FYLOG_LEVEL
{
	struct rpc_hdr hdr;  /** rpc hdr */
	char mname[24];
	int mod;
	int level;
};


struct RPCS_PROC_FYLOG_GET
{
	struct rpc_hdr hdr;  /** rpc hdr */
	unsigned long addr_log; /*ARC0*/
	unsigned long addr2_log; /*ARC1*/
};

struct RPCS_PROC_LOG_INIT
{
	struct rpc_hdr hdr;  /** rpc hdr */
	unsigned long addr_log;
	unsigned int  tv_sec;
	unsigned int  tv_usec;
	unsigned int  tv_usec_plus0;
	unsigned int  tv_usec_plus1;
};

struct RPCS_PROC_LOG_DEINIT
{
	struct rpc_hdr hdr;  /** rpc hdr */
	unsigned long addr_log;
	int size;
};


typedef enum
{
	RPC_PROC_READ = 0,
	RPC_PROC_WRITE,
	RPC_PROC_MOD_SET_LEVEL,
	RPC_PROC_MOD_GET_LEVEL,
	RPC_PROC_LOG_INIT,
	RPC_PROC_LOG_GET
}RPC_CMD_PROC_E;

int rpc_proc_register(void);
int rpc_proc_unregister(void);

extern void fhosal_rpc_proc_read(fhosal_proc_entry_t *entry);
extern int fhosal_rpc_proc_write(fhosal_proc_entry_t *file, const char *buf, int count, long long *ppos);


#endif

