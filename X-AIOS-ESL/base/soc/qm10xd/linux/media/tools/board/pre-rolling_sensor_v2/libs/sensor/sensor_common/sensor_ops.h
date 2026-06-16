/*
 * sensor_ops.h
 *
 *  Created on: Jun 2, 2015
 *      Author: duobao
 */

#ifndef SENSOR_OPS_H_
#define SENSOR_OPS_H_

#include "types/type_def.h"
#ifdef OS_LINUX
#include <stdio.h>
typedef int SEN_DEV_FD;
#define fh_printf printf
#define sensor_dev_ioctl ioctl
#define sensor_dev_open open
#define sensor_dev_close close
#elif  defined(OS_RTT)
#include <rtthread.h>
#define fh_printf rt_kprintf
typedef rt_device_t SEN_DEV_FD;
#define sensor_dev_ioctl(dev,cmd,data) \
({ \
	int _ret;\
	_ret = rt_device_control(dev,cmd,data);\
	if(_ret < 0)\
		rt_set_errno(_ret);\
	_ret;\
})
#define sensor_dev_open(name,p1,p2)\
({\
	int _ret;\
	SEN_DEV_FD _dev;\
	_dev = rt_device_find(name);\
	if(_dev != RT_NULL)\
	{\
		_ret = rt_device_open(_dev,RT_DEVICE_OFLAG_RDWR);\
		if(_ret != 0)\
			_dev = RT_NULL;\
	}\
	_dev;\
})
#define sensor_dev_close(dev)          rt_device_close(dev)
#else
extern FH_SINT32 isp_ioctl(FH_UINT32 cmd, FH_VOID * arg);
#define dev_ioctl(pdev, cmd, value) isp_ioctl(cmd, value)
#define sensor_dev_open(name,p1,p2) p1
#define sensor_dev_close(name) name
#endif

#ifdef OS_NONE
extern void *fh_malloc(int size);
extern void *fh_memcpy(void *dest, const void *src, int count);
extern int fh_free(void *addr);
extern void *fh_memset(void *s, int c, unsigned int count);
extern void fh_printf(const char *format, ...);
extern int fh_strcmp(const char *str1, const char *str2);
#define malloc  fh_malloc
#define free fh_free
#define memset fh_memset
#define sprintf fh_printf
#define strcmp fh_strcmp
#endif // OS_NONE

// #define I2C_DEVICE		"/dev/i2c-0"
#define SPI_DEVICE		"/dev/spi1"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLIP(x,min,max) MAX((min),MIN((x),(max)))
#define ABS(x) ((x) > 0 ? (x) : -(x))

typedef struct SensorDevice
{
	char *dev;
	int fd;
	FH_UINT8 mode;
	FH_UINT16 i2c_devid;
#ifndef OS_LINUX
	struct rt_i2c_bus_device *fh81_i2c;
#endif
	char ver[32];

}SensorDevice;

typedef struct iic_drv
{
    unsigned int slaveId;
    unsigned int addrBytes;
    unsigned int dataBytes;
}iic_drv_t;

int SensorDevice_Init(SensorDevice *sdev,FH_UINT16 i2c_devid, FH_UINT8 mode, FH_UINT32 devIdx);
int SensorDevice_Close(SensorDevice *sdev);
void Sensor_Write(SensorDevice *sdev,FH_UINT16 addr, FH_UINT16 data);
FH_UINT16 Sensor_Read(SensorDevice *sdev,FH_UINT16 addr);
int gpio_reset(void);

#endif /* SENSOR_OPS_H_ */
