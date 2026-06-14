#ifndef __PRE_ROLLING_DRV_IOC_H__
#define __PRE_ROLLING_DRV_IOC_H__

#include "pre_rolling.h"

#define PRG_IOC_MAGIC_CODE   'P'
#define IOC_PRG_SET_PRG_INFO		_IOW(PRG_IOC_MAGIC_CODE, 0, struct prg_system_info)
#define IOC_PRG_GET_PRG_INFO		_IOR(PRG_IOC_MAGIC_CODE, 1, struct prg_system_info)
#define IOC_PRG_GET_PICTURE		_IOR(PRG_IOC_MAGIC_CODE, 2, struct prg_picture_info)
#define IOC_PRG_ACTIVE_IOMMU		_IOW(PRG_IOC_MAGIC_CODE, 10, unsigned int)
#define IOC_PRG_INVALID_IOMMU		_IOW(PRG_IOC_MAGIC_CODE, 11, unsigned int)
#define IOC_PRG_IMAGE_ACTIVE		_IOW(PRG_IOC_MAGIC_CODE, 20, unsigned int)
#define IOC_PRG_MEM_RELEASE		_IOR(PRG_IOC_MAGIC_CODE, 30, struct prg_buffer)
#define IOC_PRG_PARAM_SYNC		_IOW(PRG_IOC_MAGIC_CODE, 40, unsigned int)
#define IOC_PRG_IMAGE_INFO		_IOR(PRG_IOC_MAGIC_CODE, 50, struct prg_image_info)
#define IOC_PRG_STARTUP			_IOR(PRG_IOC_MAGIC_CODE, 51, unsigned int)
#define IOC_PRG_SET_SENSOR_INFO		_IOW(PRG_IOC_MAGIC_CODE, 52, struct prg_sensor_info)

#define IOC_PRG_AMP_STOP		_IOW(PRG_IOC_MAGIC_CODE, 60, unsigned int)
#define IOC_PRG_SHMEM_RELEASED		_IOR(PRG_IOC_MAGIC_CODE, 61, unsigned int)
#define IOC_PRG_RELEASED		_IOR(PRG_IOC_MAGIC_CODE, 62, unsigned int)
#define IOC_PRG_GET_WAKE_UP_FLAG	_IOR(PRG_IOC_MAGIC_CODE, 63, unsigned int)
#define IOC_PRG_GET_PRG_STATE	_IOR(PRG_IOC_MAGIC_CODE, 64, unsigned int)
#define IOC_PRG_SET_H264_PPBUF	_IOW(PRG_IOC_MAGIC_CODE, 65, unsigned int)

/* 0, default, normal mode; =1, tuning mode. */
//#define IOC_PRG_SET_WORK_MODE		_IO(PRG_IOC_MAGIC_CODE, 7, unsigned int)

#endif
