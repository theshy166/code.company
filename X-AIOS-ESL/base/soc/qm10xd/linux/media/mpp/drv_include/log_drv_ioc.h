#ifndef __LOG_DRV_IOC_H__
#define __LOG_DRV_IOC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* __cplusplus */


typedef enum{
    IOC_LOG_SET_MOD_LEVEL = 0,
    IOC_LOG_GET_MOD_INFO  = 1,
    IOC_LOG_WAKEUP        = 2,
    IOC_LOG_SET_WAIT      = 3,
    IOC_LOG_GET_BUF_SIZE  = 4,
    IOC_LOG_FLUSH         = 5,
    IOC_LOG_GET_VERSION   = 6,
} IOC_LOG_E;


#define LOG_IOC_MAGIC_CODE   'L'

#define LOG_SET_MOD_LEVEL_CTL   _IOW(LOG_IOC_MAGIC_CODE,  IOC_LOG_SET_MOD_LEVEL, LOG_LEVEL_CONF_S)
#define LOG_GET_MOD_INFO_CTL    _IOWR(LOG_IOC_MAGIC_CODE, IOC_LOG_GET_MOD_INFO,  LOG_LEVEL_CONF_S)
#define LOG_WAKEUP_CTL          _IO(LOG_IOC_MAGIC_CODE,   IOC_LOG_WAKEUP)
#define LOG_SET_WAIT_CTL        _IOW(LOG_IOC_MAGIC_CODE,  IOC_LOG_SET_WAIT, unsigned int)
#define LOG_GET_BUF_SIZE        _IO(LOG_IOC_MAGIC_CODE,   IOC_LOG_GET_BUF_SIZE)
#define LOG_FLUSH_LOG           _IO(LOG_IOC_MAGIC_CODE,   IOC_LOG_FLUSH)
#define LOG_GET_VERSION         _IO(LOG_IOC_MAGIC_CODE,   IOC_LOG_GET_VERSION)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  /* __cplusplus */

#endif /*__LOG_DRV_IOC_H__*/
