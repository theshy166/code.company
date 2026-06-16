#ifndef __SYS_DRV_IOC_H__
#define __SYS_DRV_IOC_H__


#include "fh_type.h"
#include "fh_video.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */



typedef struct _MPP_SYS_MEM_CONF_S
{
	MPP_CHN_S chan;
	char ddr_name[FYL_MMZ_NAME_LEN+1];
}MPP_SYS_MEM_CONF_S;


typedef struct MPP_CHN_S_T_S
{
	MPP_CHN_S source;
	MPP_CHN_S dest;
}MPP_CHN_S_T;



#define IOC_SYS_INIT	_IO('Y',0)
#define IOC_SYS_EXIT	_IO('Y',1)
//#define IOC_SYS_SET_CONF	_IOW('Y',2,MPP_SYS_CONF_S)
//#define IOC_SYS_GET_CONF	_IOR('Y',3,MPP_SYS_CONF_S)

#define IOC_SYS_GET_CUR_PTS	_IOR('Y',6,FY_U64)
#define IOC_SYS_INIT_PTS_BASE	_IOW('Y',4,FY_U64)
#define IOC_SYS_SYNC_PTS	_IOW('Y',5,FY_U64)

#define IOC_SYS_BIND	_IOW('Y',7,MPP_CHN_S_T)
#define IOC_SYS_UNBIND	_IOW('Y',8,MPP_CHN_S_T)
#define IOC_SYS_GET_BIND_BY_DEST	_IOWR('Y',9,MPP_CHN_S_T)


#define IOC_SYS_SET_MEM_CONF	_IOW('Y',10,MPP_SYS_MEM_CONF_S)
#define IOC_SYS_GET_MEM_CONF	_IOWR('Y',11,MPP_SYS_MEM_CONF_S)
#define IOC_SYS_GET_CUSTORM_CODE	_IOR('Y',12,FY_U32)
#define IOC_SYS_GET_KENEL_CONFIG	_IOR('Y',14,unsigned long[2])
#define IOC_SYS_GET_CHIP_ID	_IOR('Y',15,FY_U32)
#define IOC_SYS_DETECT_HEART_BEAT	_IO('Y',16)
#define IOC_SYS_MFLUSH_CACHE	_IOW('Y',17,unsigned long[3])

#define IOC_SYS_UNBIND_SRC	_IOW('Y',21,MPP_CHN_S)
#define IOC_SYS_UNBIND_DST	_IOW('Y',22,MPP_CHN_S)

#define IOC_SYS_SEND_DATA	_IOW('Y',30,unsigned long)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__SYS_DRV_IOC_H__

