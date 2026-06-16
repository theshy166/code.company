#ifndef __xbus_api_h__
#define __xbus_api_h__

/*
xbusslv_send is used send cmd to host(e.g ARC0->ARM, ARC1->ARC0), it only valid on rtt

pkt_type:
             XBUS_PKT_TYPE(XBUS_PKT_TYPE_CMD, XBUS_PKT_TYPE_NEED_ACK) -- need ARC from host
             XBUS_PKT_TYPE(XBUS_PKT_TYPE_CMD, 0)                      -- no need ARC from host

port:       1 ~ RPC_PRIO_LVL_MAX
cmd_status: XBUS_CMD_TYPE_RPC
seqno     : cmd seq no to idendify the cmd and it's ACK (0 - will set int the xbus lib)
cmd:      : cmd (must hs a struct rpc_hdr at the header)
			     eg.
				struct RPCS_SYS_RegisterReceiver
				{
					struct rpc_hdr hdr; // this is the must header
					MPP_BIND_RECEIVER receiver;
					unsigned char remote;
				}
				you must set the header as the folloing example
				FH_ID_SYS is mod, RPC_SYS_RegisterReceiver is a id to the cmd
				sys_cmd.hdr.ID = RPC_ID(0,0,FH_ID_SYS,RPC_SYS_RegisterReceiver);

				RPCS_SYS_RegisterReceiver cmd;
				buf = &cmd;
size      : cmd size
*/

#ifdef OS_RTT
extern int xbusslv_send(
        unsigned char  pkt_type,
        unsigned char  port,
        unsigned int   cmd_status,
        unsigned int   seqno,
        unsigned char* cmd,
        unsigned int   size);

#endif

/*
xbus_send is used send cmd to slave(e.g ARM->ARC0, ARC0->ARC1,for linux,it's the only xbus API
pkt_type:
			 XBUS_PKT_TYPE(XBUS_PKT_TYPE_CMD, XBUS_PKT_TYPE_NEED_ACK) -- need ARC from slave
			 XBUS_PKT_TYPE(XBUS_PKT_TYPE_CMD, 0)                      -- no need ARC from slave
port:		1 ~ RPC_PRIO_LVL_MAX(15)

cmd_status£º XBUS_CMD_TYPE_RPC
seqno:       same as xbusslv_send
arg:         same as xbusslv_send
size:        same as xbusslv_send
resp_size_min: min respond size(sizeof(struct rpc_hdr))
resp_size_max: max respond size (>=size), arg should has memroy space as 'resp_size_max'

*/


#ifndef ARC1
extern int xbus_send(
        unsigned char  pkt_type,
        unsigned char  port,
        unsigned int   cmd_status,
        unsigned int   seqno,
        unsigned char* cmd,
        unsigned int   size,
        unsigned int   resp_size_min,
        unsigned int   resp_size_max);
#endif



#endif /*__xbus_api_h__*/
