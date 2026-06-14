#ifndef __rpc_slave_h__
#define __rpc_slave_h__
#include "rpc_hdr.h"

typedef int (*rpc_function_cb)(struct rpc_hdr *pkt, unsigned int pkt_len, void *priv);

extern int rpc_register_table(unsigned int version, unsigned int table_id, rpc_function_cb *cb_list, unsigned int cb_num);

extern int rpc_send_back(struct rpc_hdr *pkt, unsigned int pkt_len, void *priv);

/*
 * just put it here
 * Notice: this function can only be called in interrupt context.
 */
extern void xbus_trigger_host_intr(unsigned short type, void *args, unsigned short len);

#endif //__rpc_slave_h__
