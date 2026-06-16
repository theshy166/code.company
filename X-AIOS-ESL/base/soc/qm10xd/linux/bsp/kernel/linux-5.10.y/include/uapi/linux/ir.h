/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_api_ddal.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/01/23
  Description   : Hi_IR.c header file
  History       :
  1.Date        : 2007/01/23
    Author      : j37987
    Modification: Created file

******************************************************************************/

#ifndef _UAPI_LINUX_IR_H
#define _UAPI_LINUX_IR_H

#include <linux/types.h>

//#define OSDRV_MODULE_VERSION_STRING "HISI_IRDA-MF @Hi3518v100R001"
/* define device name */
#define MCIR_DEVICE_NAME "MC_IR"

/* Ioctl definitions */
#define IR_IOC_SET_BUF				0x01
#define IR_IOC_SET_ENABLE_KEYUP			0x02// 1:check keyup
#define IR_IOC_SET_ENABLE_REPKEY		0x03// 1:check repkey, 0:hardware behave
#define IR_IOC_SET_REPKEY_TIMEOUTVAL		0x04
#define IR_IOC_SET_IR_PARM			0x09
#define IR_IOC_GET_CONFIG			0x24
#define IR_IOC_ENDBG				0x31
#define IR_IOC_DISDBG				0x32

/* device parameter */
struct ir_pro_desc {
	u16	lead_s;
	u16	lead_e;
	u16	b_0;
	u16	b_1;
	u16	slead_e;
	u16	burst;
	u16	ir_format;
	u16	ir_bits;
	u16	ir_mode;
	u16	ir_freq;
	u16	fram_lenth;
	u16	release_time_fp;
	u16	release_time_sp;	
};

/* DEFINE KEY STATE */
#define MCIR_KEY_DOWN	0x00
#define MCIR_KEY_UP	0x01

struct irkey_info_s {
	u32 irkey_datah;
	u32 irkey_datal;
	u32 irkey_state_code;
};

/*lead_s,lead_e,b_0,b_1,slead_e,burst,ir_format,ir_bits,ir_mode,ir_freq,fram_lenth,release_time_fp,release_time_sp*/
static struct ir_pro_desc static_dev_param[] = 
{
	/*NEC with simple repeat code : uPD6121G*/
	{900, 450, 56, 169, 225, 55 , 0, 32, 0, 0, 10800, 10000, 15000},

	/*NEC with full repeat code : uPD6121G*/
	{900, 450, 56, 169, 0, 55 , 0, 32, 0, 0, 10800, 10000, 15000},

	/*TC9012 :TC9012F/9243*/
	{450, 450, 56, 169, 0, 56, 0, 32, 0, 0, 10800, 10000, 15000},

	/*SONY: SONY-D7C8*/
	{240, 60, 60, 120, 0, 0, 0, 15, 0, 0, 10800, 10000, 15000},

};

#endif /* End of #ifndef __HI_IR_H__*/

