/*
 * sensor_ops.c
 *
 *  Created on: Jun 2, 2015
 *      Author: duobao
 */

#include "sensor_ops.h"
#include <unistd.h>
#include "fh_std_osal_usr.h"
#ifdef OS_LINUX
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#else  // RTT
#include <stdio.h>
#include "i2c.h"
#define FH_I2C_DEVICE "/dev/i2c-0"
#define FH_SPI_DEVICE "/dev/spi1"
#endif
#include "sensor_ver.h"

#define I2C_M_SPERATE_MSG	0x8000
#define SET_SENSOR_VER      _IOW('i', 26,FH_UINT32)

#ifdef OS_LINUX
#define ISP_DEV_NAME        "/dev/isp"
#else
#define ISP_DEV_NAME        "ISP"
#endif
static SensorDevice sdev = {0};
static DEV_FD sensor_isp_device;

char I2C_DEVICE[2][16] = {
	"/dev/i2c-0",
	"/dev/i2c-1",
};

#if OS_NONE

#define SENSOR_OPS_FUN_DEBUG 1
#define SENSOR_OPS_DEBUG 1

typedef enum {
	UINITED = 0,
	INITED,
	STATUS_MAX = 0xffffffff,
}sensor_ops_status_e;

typedef enum {
	ADDR8_DATA8 = 0,
	ADDR8_DATA16,
	ADDR16_DATA8,
	ADDR16_DATA16,
	ADDR_DATA_MAX = 0xffffffff,
}sensor_i2c_mode_e;

typedef struct {
	uint32_t inited;
	sensor_i2c_mode_e i2cMode;
	struct i2c_cfg i2cCfg;
	struct i2c_req i2cReadReq;
	struct i2c_req i2cWriteReq;
	struct i2c_dev *i2cDev;
	uint8_t writeBuf[4];
	uint8_t readBuf[4];
}sensor_ops_context_t;

sensor_ops_context_t s_sensor_ops_context = {0x00};

sensor_ops_context_t *_sensor_ops_context_get(void)
{
	return &s_sensor_ops_context;
}

#endif

char *get_sensor_version(void)
{
	static char sensor_version[32]=SENSOR_GIT_VERSION;

	sensor_isp_device = dev_open(ISP_DEV_NAME,0,0);

	dev_ioctl(sensor_isp_device,SET_SENSOR_VER,sensor_version);

	return (char *)sensor_version;
}


void SPISensor_Write(FH_UINT16 addr, FH_UINT32 data)
{

}

FH_UINT16 SPISensor_Read(FH_UINT16 addr)
{
	return 0;
}

#ifdef OS_RTT
rt_err_t I2CSensor_WriteEx(struct rt_i2c_bus_device *fh81_i2c,rt_uint16_t reg, rt_uint16_t data, int no_ack)
{
	struct rt_i2c_msg msg;
	rt_uint8_t send_buf[4];

	msg.addr = sdev.i2c_devid;
	msg.flags = RT_I2C_WR;
	msg.buf = send_buf;
	switch(sdev.mode)
	{
	case 0:
		send_buf[0] = (reg & 0xff);
		send_buf[1] = (data & 0xff);
		msg.len = 2;
		break;
	case 1:
		send_buf[0] = (reg & 0xff);
		send_buf[1] = (data>>8) & 0xff;
		send_buf[2]= data & 0xff;
		msg.len = 3;
		break;
	case 2:
		send_buf[0] = (reg >>8)& 0xff;
		send_buf[1] = reg & 0xff;
		send_buf[2]= data & 0xff;
		msg.len = 3;
		break;
	case 3:
		send_buf[0] = (reg >>8)& 0xff;
		send_buf[1] = reg & 0xff;
		send_buf[2]= (data>>8) & 0xff;
		send_buf[3]=	data & 0xff;
		msg.len = 4;
	}

	rt_i2c_transfer(sdev.fh81_i2c, &msg, 1);
//	if(ret < 0 && !no_ack)
//		{
//			perror("ERROR: Unable to write sensor register!");
//			fh_printf("addr: 0x%x, data: 0x%x\n", reg, data);
//		}
	return RT_EOK;
}

rt_err_t I2CSensor_WriteEx_Multi(struct rt_i2c_bus_device *fh81_i2c,rt_uint16_t *reg, rt_uint16_t *data, int no_ack,int size)
{
	struct rt_i2c_msg *msg;
	msg = (struct rt_i2c_msg *)malloc(size * sizeof(struct rt_i2c_msg));
	if (!msg)
		return -RT_ERROR;
	int i;
	rt_uint8_t *send_buf;
	send_buf = (rt_uint8_t*)malloc(size*4);
	if (!send_buf)
	{
		free(msg);
		return -RT_ERROR;
	}

	for (i=0;i<size;i++)
	{
		msg[i].addr = sdev.i2c_devid;
		msg[i].flags = RT_I2C_WR | I2C_M_SPERATE_MSG;
		msg[i].buf = &send_buf[4*i];

		switch(sdev.mode)
		{
		case 0:
			send_buf[0+i*4] = (reg[i] & 0xff);
			send_buf[1+i*4] = (data[i] & 0xff);
			msg[i].len = 2;
			break;
		case 1:
			send_buf[0+i*4] = (reg[i] & 0xff);
			send_buf[1+i*4] = (data[i]>>8) & 0xff;
			send_buf[2+i*4] = data[i] & 0xff;
			msg[i].len = 3;
			break;
		case 2:
			send_buf[0+i*4] = (reg[i] >>8)& 0xff;
			send_buf[1+i*4] = reg[i] & 0xff;
			send_buf[2+i*4] = data[i] & 0xff;
			msg[i].len = 3;
			break;
		case 3:
			send_buf[0+i*4] = (reg[i] >>8)& 0xff;
			send_buf[1+i*4] = reg[i] & 0xff;
			send_buf[2+i*4] = (data[i]>>8) & 0xff;
			send_buf[3+i*4] = data[i] & 0xff;
			msg[i].len = 4;
		}

	}

	rt_i2c_transfer(sdev.fh81_i2c, msg, size);

	free(msg);
	free(send_buf);
//	if(ret < 0 && !no_ack)
//		{
//			perror("ERROR: Unable to write sensor register!");
//			fh_printf("addr: 0x%x, data: 0x%x\n", reg, data);
//		}
	return RT_EOK;
}

#elif OS_LINUX
void I2CSensor_WriteEx(FH_UINT16 addr, FH_UINT16 data, int no_ack)
{
	struct i2c_rdwr_ioctl_data i2c_data;
	struct i2c_msg msgs[1] = {{0}};
	FH_UINT8 tmp[4];
	int ret;

	i2c_data.nmsgs = 1;
	//i2c_data.msgs = (struct i2c_msg *)malloc(i2c_data.nmsgs * sizeof(struct i2c_msg));
	i2c_data.msgs = (struct i2c_msg *)&msgs;

	switch(sdev.mode)
	{
	case 0:
		tmp[0] = addr & 0xff;
		tmp[1] = data & 0xff;
		i2c_data.msgs[0].len = 2;
		break;
	case 1:
		tmp[0] = addr & 0xff;
		tmp[1] = (data>>8) & 0xff;
		tmp[2] = data & 0xff;
		i2c_data.msgs[0].len = 3;
		break;
	case 2:
		tmp[0] = (addr>>8) & 0xff;
		tmp[1] = addr & 0xff;
		tmp[2] = data & 0xff;
		i2c_data.msgs[0].len = 3;
		break;
	default:
	case 3:
		tmp[0] = (addr>>8) & 0xff;
		tmp[1] = addr & 0xff;
		tmp[2] = (data>>8) & 0xff;
		tmp[3] = data & 0xff;
		i2c_data.msgs[0].len = 4;
		break;
	}

	i2c_data.msgs[0].addr = sdev.i2c_devid;				/* */
	i2c_data.msgs[0].flags = 0; 			/* write */
	i2c_data.msgs[0].buf = tmp;

	ret = ioctl(sdev.fd, I2C_RDWR, &i2c_data);
	//free(i2c_data.msgs);
	if(ret < 0 && !no_ack)
	{
		perror("ERROR: Unable to write sensor register!");
		fh_printf("ret:%d  addr: 0x%x, data: 0x%x\n", ret, addr, data);
	}
}

void I2CSensor_WriteEx_Multi(FH_UINT16 *addr, FH_UINT16 *data, int no_ack,int size)
{
	struct i2c_rdwr_ioctl_data i2c_data;
	FH_UINT8 *tmp;
	int ret;

	i2c_data.nmsgs = size;
	i2c_data.msgs = (struct i2c_msg *)malloc(i2c_data.nmsgs * sizeof(struct i2c_msg));
	if (!i2c_data.msgs)
		return;
	tmp = (FH_UINT8*)malloc(size*4);

	if (!tmp)
	{
		free(i2c_data.msgs);
		return;
	}


	int i;
	for (i=0;i<size;i++)
	{
		switch(sdev.mode)
		{
		case 0:
			tmp[0+i*4] = addr[i] & 0xff;
			tmp[1+i*4] = data[i] & 0xff;
			i2c_data.msgs[i].len = 2;
			break;
		case 1:
			tmp[0+i*4] = addr[i] & 0xff;
			tmp[1+i*4] = (data[i]>>8) & 0xff;
			tmp[2+i*4] = data[i] & 0xff;
			i2c_data.msgs[i].len = 3;
			break;
		case 2:
			tmp[0+i*4] = (addr[i]>>8) & 0xff;
			tmp[1+i*4] = addr[i] & 0xff;
			tmp[2+i*4] = data[i] & 0xff;
			i2c_data.msgs[i].len = 3;
			break;
		default:
		case 3:
			tmp[0+i*4] = (addr[i]>>8) & 0xff;
			tmp[1+i*4] = addr[i] & 0xff;
			tmp[2+i*4] = (data[i]>>8) & 0xff;
			tmp[3+i*4] = data[i] & 0xff;
			i2c_data.msgs[i].len = 4;
			break;
		}

		i2c_data.msgs[i].addr = sdev.i2c_devid;				/* */
		i2c_data.msgs[i].flags = 0 | I2C_M_SPERATE_MSG; 			/* write */
		i2c_data.msgs[i].buf = &tmp[4*i];
	}

	ret = ioctl(sdev.fd, I2C_RDWR, &i2c_data);
	//FH_UINT8* sndb  = tmp;
	//int k  =0;
	//for (k = 0; k<size;k++) {
	//	write(sdev.fd,sndb,i2c_data.msgs[k].len);
	//	sndb +=4;
	//	ret =0;
	//}
	free(i2c_data.msgs);
	free(tmp);
	if(ret < 0 && !no_ack)
	{
		perror("ERROR: Unable to write sensor register!");
		fh_printf("addr: 0x%x, data: 0x%x\n", addr[0], data[0]);
	}
}

#elif OS_NONE
void I2CSensor_WriteEx(FH_UINT16 addr, FH_UINT16 data, int no_ack)
{
	int ret = 0;
	sensor_ops_context_t *sensorContext = _sensor_ops_context_get();
	int len = 0;
	int i = 0;

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------start--------\n", __func__);
	#endif

	if(INITED != sensorContext->inited) {
		printk("i2c_config uninited\n");
		ret = 1;
		goto EXIT;
	}

	switch(sensorContext->i2cMode)
	{
		case 0:
			sensorContext->writeBuf[len++] = addr & 0xff;
			sensorContext->writeBuf[len++] = data & 0xff;
			break;
		case 1:
			sensorContext->writeBuf[len++] = addr & 0xff;
			sensorContext->writeBuf[len++] = (data>>8) & 0xff;
			sensorContext->writeBuf[len++] = data & 0xff;
			break;
		case 2:
			sensorContext->writeBuf[len++] = (addr>>8) & 0xff;
			sensorContext->writeBuf[len++] = addr & 0xff;
			sensorContext->writeBuf[len++] = data & 0xff;
			break;
		case 3:
			sensorContext->writeBuf[len++] = (addr>>8) & 0xff;
			sensorContext->writeBuf[len++] = addr & 0xff;
			sensorContext->writeBuf[len++] = (data>>8) & 0xff;
			sensorContext->writeBuf[len++] = data & 0xff;
			break;
		default:
			break;
	}

	sensorContext->i2cWriteReq.buf = (void *)&sensorContext->writeBuf;
	sensorContext->i2cWriteReq.len = len;

	#if SENSOR_OPS_DEBUG
	printk("u8SlaveAddr:%d\n", sensorContext->i2cCfg.dev_addr);
	printk("id:%d\n", sensorContext->i2cWriteReq.op);
	printk("read_addr:%d\n", sensorContext->i2cWriteReq.read_addr);
	printk("buf:%x\n", sensorContext->i2cWriteReq.buf);
	printk("len:%d\n", sensorContext->i2cWriteReq.len);
	printk("func:%x\n", sensorContext->i2cWriteReq.func);
	printk("data:%x\n", sensorContext->i2cWriteReq.data);
	printk("status:%d\n", sensorContext->i2cWriteReq.status);
	printk("link_addr:%x\n", sensorContext->i2cWriteReq.link_addr);
	printk("link_len:%x\n", sensorContext->i2cWriteReq.link_len);
	printk("Rx_link_addr:%x\n", sensorContext->i2cWriteReq.Rx_link_addr);

	printk("controller_base:%x\n", sensorContext->i2cDev->controller_base);
	printk("clk:%d\n", sensorContext->i2cDev->source_clk);
	printk("speed:%d\n", sensorContext->i2cDev->scl_clk);
	printk("dev_addr:%d\n", sensorContext->i2cDev->dev_addr);
	#endif

	ret = i2c_xfer_request(sensorContext->i2cDev, &sensorContext->i2cWriteReq);
	if (ret < 0) {
		printk("i2c_tx_req %d fail\n", ret);
		return -1;
	}

	for(i=0;i<50;i++);

	EXIT:

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------end--------\n", __func__);
	#endif

	return ret;
}


void I2CSensor_WriteEx_Multi(FH_UINT16 *addr, FH_UINT16 *data, int no_ack,int size)
{
	int ret = 0;
	int i;

	for (i=0;i<size;i++)
	{
		I2CSensor_WriteEx(addr[i], data[i], no_ack);
	}

	return ret;
}
#endif


void I2CSensor_Write_Multi(FH_UINT16 *addr, FH_UINT16 *data,FH_UINT16 size)
{
#ifdef OS_RTT
	I2CSensor_WriteEx_Multi(sdev.fh81_i2c,addr, data, 0,size);
#elif OS_LINUX
	I2CSensor_WriteEx_Multi(addr, data, 0,size);
#elif OS_NONE
	I2CSensor_WriteEx_Multi(addr, data, 0,size);
#endif
}

void I2CSensor_Write(FH_UINT16 addr, FH_UINT16 data)
{
#ifdef OS_RTT
	I2CSensor_WriteEx(sdev.fh81_i2c,addr, data, 0);
#elif OS_LINUX
	I2CSensor_WriteEx(addr, data, 0);
#elif OS_NONE
	I2CSensor_WriteEx(addr, data, 0);
#endif

}

#ifdef OS_RTT
rt_err_t I2CSensor_Read(struct rt_i2c_bus_device *fh81_i2c,rt_uint16_t reg, rt_uint16_t *data)
{
	struct rt_i2c_msg msg[2];
	rt_uint8_t send_buf[2];
	rt_uint8_t recv_buf[2];
	msg[0].addr = sdev.i2c_devid;
	msg[0].flags = RT_I2C_WR;
	msg[1].addr = sdev.i2c_devid;
	msg[1].flags = RT_I2C_RD;
	switch(sdev.mode)
	{
	case 0:
		send_buf[0] = (reg & 0xff);
		msg[0].len = 1;
		msg[0].buf = send_buf;
		msg[1].len = 1;
		msg[1].buf = recv_buf;
		break;
	case 1:
		send_buf[0] = (reg & 0xff);
		msg[0].len = 1;
		msg[0].buf = send_buf;
		msg[1].len = 2;
		msg[1].buf = recv_buf;
		break;
	case 2:
		send_buf[0] =(reg>>8) & 0xff;
		send_buf[1] =(reg & 0xff);
		msg[0].len = 2;
		msg[0].buf = send_buf;
		msg[1].len = 1;
		msg[1].buf = recv_buf;
		break;
	case 3:
		send_buf[0] =(reg>>8) & 0xff;
		send_buf[1] =(reg & 0xff);
		msg[0].len = 2;
		msg[0].buf = send_buf;
		msg[1].len = 2;
		msg[1].buf = recv_buf;
	}
	rt_i2c_transfer(sdev.fh81_i2c, msg, 2);
	if(sdev.mode & 1)
		*data = (recv_buf[0]<<8)| recv_buf[1];
	else
		*data = recv_buf[0];

	return RT_EOK;
}
#elif OS_LINUX
FH_UINT16 I2CSensor_Read(FH_UINT16 addr)
{
	struct i2c_rdwr_ioctl_data i2c_data;
	struct i2c_msg msgs[2] = {{0}};
	FH_UINT8 reg[4] = {0};
	FH_UINT8 val[4] = {0};
	int ret;

	i2c_data.nmsgs = 2;
	//i2c_data.msgs = (struct i2c_msg *)malloc(i2c_data.nmsgs * sizeof(struct i2c_msg));
	i2c_data.msgs = msgs;

	switch(sdev.mode)
	{
	case 0:
		reg[0] = addr & 0xff;
		i2c_data.msgs[0].len = 1;
		i2c_data.msgs[1].len = 1;
		break;
	case 1:
		reg[0] = addr & 0xff;
		i2c_data.msgs[0].len = 1;
		i2c_data.msgs[1].len = 2;
		break;
	case 2:
		reg[0] = (addr>>8) & 0xff;
		reg[1] = addr & 0xff;
		i2c_data.msgs[0].len = 2;
		i2c_data.msgs[1].len = 1;
		break;
	default:
	case 3:
		reg[0] = (addr>>8) & 0xff;
		reg[1] = addr & 0xff;
		i2c_data.msgs[0].len = 2;
		i2c_data.msgs[1].len = 2;
		break;
	}

	i2c_data.msgs[0].addr = sdev.i2c_devid;				/* */
	i2c_data.msgs[0].flags = 0; 						/* write */
	i2c_data.msgs[0].buf = reg;

	i2c_data.msgs[1].addr = sdev.i2c_devid;				/* */
	i2c_data.msgs[1].flags = I2C_M_RD; 					/* read */
	i2c_data.msgs[1].buf = val;

	ret = ioctl(sdev.fd, I2C_RDWR, &i2c_data);

	//free(i2c_data.msgs);
	if(ret < 0)
	{
		perror("ERROR: Unable to read sensor register!");
		return -1;
	}
	if(sdev.mode & 1)
		return (val[0] << 8) | (val[1]);
	else
		return val[0];
}
#elif OS_NONE
FH_UINT16 I2CSensor_Read(FH_UINT16 addr)
{
	int ret = 0;
	sensor_ops_context_t *sensorContext = _sensor_ops_context_get();
	int len = 0;
	int i = 0;
	unsigned short readValue = 0x00;

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------start--------\n", __func__);
	#endif

	if(INITED != sensorContext->inited) {
		printk("i2c_config uninited\n");
		goto EXIT;
	}

	sensorContext->i2cReadReq.read_addr = (uint8_t)addr;
	sensorContext->i2cReadReq.buf = (void *)&sensorContext->readBuf;

	if ((ADDR8_DATA16 == sensorContext->i2cMode)
		|| (ADDR16_DATA16 == sensorContext->i2cMode)) {
	   sensorContext->i2cReadReq.len = 2;
	} else {
		sensorContext->i2cReadReq.len = 1;
	}

	ret = i2c_xfer_request(sensorContext->i2cDev, &sensorContext->i2cReadReq);

	if (0 > ret) {
		printk("i2c_tx_req %d fail\n", ret);
		goto EXIT;
	}

	if ((ADDR8_DATA16 == sensorContext->i2cMode)
		|| (ADDR16_DATA16 == sensorContext->i2cMode)) {
		readValue = (sensorContext->readBuf[1] << 1) | sensorContext->readBuf[0];
	} else {
		readValue = sensorContext->readBuf[0];
	}

	EXIT:

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------end--------\n", __func__);
	#endif

	return readValue;
}

#endif

FH_UINT16 Sensor_Read(FH_UINT16 addr)
{
	if(sdev.mode > 3 )
		return SPISensor_Read(addr);
	else
	{
#ifdef OS_RTT
		rt_uint16_t data;
		I2CSensor_Read(sdev.fh81_i2c,addr,&data);
		return data;
#elif OS_LINUX
		return I2CSensor_Read(addr);
#elif OS_NONE
		return I2CSensor_Read(addr);
#endif
	}
}

void Sensor_Write(FH_UINT16 addr, FH_UINT16 data)
{
	if(sdev.mode > 3 )
		SPISensor_Write(addr, data);
	else
		I2CSensor_Write(addr, data);
}

void Sensor_Write_Multi(FH_UINT16* addr, FH_UINT16* data,FH_UINT16 size)
{
	if(sdev.mode > 3 )
		SPISensor_Write(addr[0], data[0]);
	else
		I2CSensor_Write_Multi(addr, data,size);
}

void Sensor_WriteEx(FH_UINT16 addr, FH_UINT16 data, int no_ack)
{
	if(sdev.mode > 3 )
		SPISensor_Write(addr, data);
	else
#ifdef OS_RTT
		I2CSensor_WriteEx(sdev.fh81_i2c,addr, data, no_ack);
#elif OS_LINUX
		I2CSensor_WriteEx(addr, data, no_ack);
#elif OS_NONE
		I2CSensor_WriteEx(addr, data, no_ack);
#endif
}

int SensorDevice_Init(FH_UINT16 i2c_devid, FH_UINT8 mode, FH_UINT32 devIdx)
{
	get_sensor_version();

#ifdef OS_RTT

	sdev.fh81_i2c = rt_i2c_bus_device_find("i2c0");

	if(mode > 3 )
		sdev.dev = FH_SPI_DEVICE;
	else
		sdev.dev = FH_I2C_DEVICE;

	sdev.i2c_devid = i2c_devid;
	sdev.mode = mode;
#elif OS_LINUX
	int ret;
	if(sdev.fd > 0)
		{
			perror("Error: Sensor device opened !\n");
			return -1;
		}

		if(mode > 3 )
			sdev.dev = SPI_DEVICE;
		else
			sdev.dev = I2C_DEVICE[devIdx];

		sdev.i2c_devid = i2c_devid;
		sdev.mode = mode;

		sdev.fd = open(sdev.dev, O_RDWR | O_NONBLOCK);

		if(sdev.fd < 0)
		{
			perror("ERROR: Unable to open sensor!");
			return -1;
		}

		ret = ioctl(sdev.fd, I2C_TENBIT, 0);

		if(ret < 0)
		{
			perror("Error: Unable to set address mode!\n");
			return -1;
		}

		ret = ioctl(sdev.fd, I2C_SLAVE_FORCE, i2c_devid);
		if (ret < 0)
		{
			perror("Error: Unable to set slave address!\n");
			return -1;
		}
#elif OS_NONE
	int ret = 0;
	sensor_ops_context_t *sensorContext = _sensor_ops_context_get();

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------start--------\n", __func__);
	#endif

	if(INITED == sensorContext->inited) {
		printk("i2c_config fail %d\n");
		ret = 1;
		goto EXIT;
	}

	sensorContext->i2cCfg.clk = 36000000,
	sensorContext->i2cCfg.speed = I2C_SPEED_STD,
	sensorContext->i2cCfg.addr_10 = false,
	sensorContext->i2cCfg.mode = I2C_FIFO_MODE,

	sensorContext->i2cCfg.id = devIdx;
	sensorContext->i2cCfg.dev_addr = i2c_devid;
	sensorContext->i2cMode = mode;

	#if SENSOR_OPS_DEBUG
	printk("id:%d\n", sensorContext->i2cCfg.id);
	printk("clk:%d\n", sensorContext->i2cCfg.clk);
	printk("speed:%d\n", sensorContext->i2cCfg.speed);
	printk("dev_addr:%d\n", sensorContext->i2cCfg.dev_addr);
	printk("mode:%d\n", sensorContext->i2cCfg.mode);
	#endif

	ret = i2c_config(&sensorContext->i2cCfg, &sensorContext->i2cDev);
	if(0 != ret){
		printk("i2c_config fail %d\n");
		goto EXIT;
	}

	sensorContext->i2cReadReq.op = I2C_OP_READ;
	sensorContext->i2cWriteReq.op = I2C_OP_WRITE;
	sensorContext->inited = INITED;

	EXIT:

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------end--------\n", __func__);
	#endif

	return ret;
#endif

	return 0;
}


int SensorDevice_Close()
{
	int ret = 0;

#ifdef OS_RTT
#elif OS_LINUX
	if(sdev.fd <= 0)
	{
		perror("Error: Sensor device not open !\n");
		ret = -1;
	}
	else
	{
		close(sdev.fd);

		sdev.fd = -1;
	}

	// 设置版本号的时候，打开了ISP的设备，这边需要关掉
	if (sensor_isp_device <= 0)
	{
		perror("Error: ISP device not open !\n");
		ret = -1;
	}
	else
	{
		close(sensor_isp_device);

		sensor_isp_device = -1;
	}
#elif OS_NONE
	sensor_ops_context_t *sensorContext = _sensor_ops_context_get();

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------start--------\n", __func__);
	#endif

	if(INITED != sensorContext->inited) {
		printk("i2c_config fail %d\n");
		ret = 1;
		goto EXIT;
	}

	memset((void *)sensorContext, 0x00, sizeof(sensor_ops_context_t));

	EXIT:

	#if SENSOR_OPS_FUN_DEBUG
	printk("%s, ------end--------\n", __func__);
	#endif

#endif

	return ret;
}
