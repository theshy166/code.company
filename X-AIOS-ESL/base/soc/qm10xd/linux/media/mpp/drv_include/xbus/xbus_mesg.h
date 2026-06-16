#ifndef __xbus_mesg_h__
#define __xbus_mesg_h__

/*
 * type: must not be zero
 * len:  now the max supported len is 64 bytes
 */
typedef void xbus_intr_cb_t(unsigned short type, void *args, unsigned short len);

/*
 * 1) Only one callback function is supported.
 * 2) the supported callback mesg length is 64 bytes now.
 * 3) you can unregister callback if cb equal to NULL.
 */
void xbus_intr_register_cb(xbus_intr_cb_t *cb);

#endif //__xbus_mesg_h__
