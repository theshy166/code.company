#ifndef __xbus_ioctl_h__
#define __xbus_ioctl_h__

#define XBUS_DEV_NAME "rtxbus"
#define XBUS_DEV1_NAME "rtxbus1"

#define XBUS_IOC_CMD_RPC_QUERY     (0x01<<28)
#define XBUS_IOC_CMD_RPC_CALL      (0x02<<28)
#define XBUS_IOC_CMD_GET_VMM_RANGE (0x03<<28)
#define XBUS_IOC_CMD_ACW_OPEN_CLK  (0x04<<28)

struct vmm_range
{
    unsigned int vmm_start;
    unsigned int vmm_size;	
};

/*
 *Return value for ioctl:
 *  <0, error number.
 * >=0, the real output length from peer.
 */

#endif /*__xbus_ioctl_h__*/

