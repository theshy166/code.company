/*
 * clock_ops.c
 *
 *  Created on: 2016年12月26日
 *      Author: gaoyb
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "clock_ops.h"
#ifdef OS_RTT
#ifdef RPC_RTT
#define siginfo_t void
#endif
#include <rtthread.h>
#ifdef RPC_RTT
#include "libc/libc_ioctl.h"
#endif
#elif OS_LINUX
static int fd = -1;
static const char dev[]="/dev/fh_clk_miscdev";
#endif
#ifdef OS_RTT
#define fh_printf    rt_kprintf
#elif  OS_NONE
extern void fh_printf(const char *format, ...);
#define perror fh_printf
#else //OS_LINUX
#define fh_printf    printf
#endif //OS_RTT

int set_clk_rate(int clock_type,unsigned long rate)
{

	return 0;
}

int set_clk_enable(int clock_type, int enable)
{
	int ret = 0;
#ifndef OS_NONE
	struct clk_usr myclk;

	switch(clock_type)
	{
	case 0:
		myclk.name = "isp_aclk";
		break;
	case 1:
		myclk.name = "cis_clk_out";
		break;
	default:
		perror("no clock selected");
		return -1;
	}
#endif // !OS_NONE

#ifdef OS_RTT
	rt_device_t clk_device=RT_NULL;
	clk_device=rt_device_find("fh_clock");
	if(clk_device == RT_NULL)
	{
		fh_printf("fh_clk open error");
		return -1;
	}
	if(enable == 1)
		ret = rt_device_control(clk_device,ENABLE_CLK,&myclk);
	else if(enable == 0)
		ret = rt_device_control(clk_device,DISABLE_CLK,&myclk);

#elif OS_LINUX

	fd = open(dev, O_RDWR | O_SYNC);

	if(fd < 0)
	{
		perror("fh_clk open error");
		return -1;
	}
	if(enable == 1)
		ret = ioctl(fd, ENABLE_CLK, &myclk);
	else if(enable == 0)
		ret = ioctl(fd, DISABLE_CLK, &myclk);

	if (fd > 0)
	{
		close(fd);
		fd = -1;
	}
#else

#endif

	if(ret < 0)
	{
		fh_printf("SET_CLK_ENABLE error, ret=%d\n", ret);
		return -1;
	}

	return 0;
}