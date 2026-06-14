#include <common.h>
#include <mach/hardware.h>
#include <mach/uart_drv.h>
#include <mach/download.h>
#include <mach/crc16.h>
#include <mach/spl.h>
#include <mach/board.h>
#include <mach/systimer.h>

uchar recv_buf[RECV_BUF_SIZE] = {0};
uchar txfr_buf[XFR_BUF_SIZE]  = {0};

struct down_info d_info;

DownLoadMap MapTbl[] = {
	{IMG_INVALID, IMG_INVALID,      IMG_INVALID,              IMG_INVALID,   IMG_INVALID,       IMG_INVALID},
	{SPL_IMG_ID, SPL_DOWN_ADDR, SPL_NAND_LOCATION_OFFS, SPL_NOR_LOCATION_OFFS,   SPL_EMMC_LOCATION_OFFS, SPL_SDIO1_LOCATION_OFFS,  SPL_MAX_SIZE},
	{Uboot_IMG_ID, UBOOT_DOWN_ADDR, UBOOT_NAND_LOCATION_OFFS, UBOOT_NOR_LOCATION_OFFS, UBOOT_EMMC_LOCATION_OFFS, UBOOT_SDIO1_LOCATION_OFFS,UBOOT_MAX_SIZE},
	{Kernel_IMG_ID, KERNEL_DOWN_ADDR, KERNEL_NAND_LOCATION_OFFS,KERNEL_NOR_LOCATION_OFFS, KERNEL_EMMC_LOCATION_OFFS, KERNEL_SDIO1_LOCATION_OFFS, KERNEL_MAX_SIZE},
	//{PRE_ROLLING_IMG_ID, PRE_ROLLING_DOWN_ADDR, PRE_ROLLING_NAND_LOCATION_OFFS,PRE_ROLLING_NOR_LOCATION_OFFS, PRE_ROLLING_EMMC_LOCATION_OFFS, PRE_ROLLING_SDIO1_LOCATION_OFFS, PRE_ROLLING_MAX_SIZE},
	//{RTT_IMG_ID, RTT_DOWN_ADDR, RTT_NAND_LOCATION_OFFS,RTT_NOR_LOCATION_OFFS, RTT_EMMC_LOCATION_OFFS, RTT_SDIO1_LOCATION_OFFS, RTT_MAX_SIZE},
	//{NNMODEL_ID, NNMODEL_DOWN_ADDR, NNMODEL_NAND_LOCATION_OFFS,NNMODEL_NOR_LOCATION_OFFS, NNMODEL_EMMC_LOCATION_OFFS, NNMODEL_SDIO1_LOCATION_OFFS, NNMODEL_MAX_SIZE},
	//{RTTAP_ID, RTTAP_DOWN_ADDR, RTTAP_NAND_LOCATION_OFFS,RTTAP_NOR_LOCATION_OFFS, RTTAP_EMMC_LOCATION_OFFS, RTTAP_SDIO1_LOCATION_OFFS, RTTAP_MAX_SIZE},
	//{VDSP_ID, NNMODEL_DOWN_ADDR, VDSP_NAND_LOCATION_OFFS,VDSP_NOR_LOCATION_OFFS,VDSP_EMMC_LOCATION_OFFS,VDSP_SDIO1_LOCATION_OFFS, VDSP_MAX_SIZE},
	{SYSTEM_IMG_ID, SYSTEM_DOWN_ADDR, SYSTEM_NAND_LOCATION_OFFS,SYSTEM_NOR_LOCATION_OFFS, SYSTEM_EMMC_LOCATION_OFFS, SYSTEM_SDIO1_LOCATION_OFFS, SYSTEM_MAX_SIZE},
	{ROOTFS_IMG_ID, ROOTFS_DOWN_ADDR, ROOTFS_NAND_LOCATION_OFFS,ROOTFS_NOR_LOCATION_OFFS, ROOTFS_EMMC_LOCATION_OFFS, ROOTFS_SDIO1_LOCATION_OFFS, ROOTFS_MAX_SIZE},
	{DATA_IMG_ID, DATA_DOWN_ADDR, DATA_NAND_LOCATION_OFFS,DATA_NOR_LOCATION_OFFS, DATA_EMMC_LOCATION_OFFS, DATA_SDIO1_LOCATION_OFFS, DATA_MAX_SIZE},
};

struct down_info* get_cur_downinfo(void)
{
	return &d_info;
}
#ifdef CONFIG_SDL_BUILD
u32 get_downmap_info(struct down_info * info,u8 cmd)
{
	u32 ret=0;
	switch(cmd)
	{
		case IMGID_CMD:
			ret = MapTbl[info->imageid].id;
			break;
		case IMGRAMADDR_CMD:
			ret = MapTbl[info->imageid].downaddr;
			break;
		case IMG_NANDADDR_CMD:
			ret = MapTbl[info->imageid].nandaddr;
			break;
		case IMG_NORADDR_CMD:
			ret = MapTbl[info->imageid].noraddr;
			break;
		case IMG_EMMCADDR_CMD:
			ret = MapTbl[info->imageid].emmcaddr;
			break;
		case IMG_SDIO1ADDR_CMD:
			ret = MapTbl[info->imageid].sdio1addr;
			break;
		case IMG_SIZE_CMD:
			ret = MapTbl[info->imageid].max_size;
			break;
	}
	return ret;
}
#endif

void uart_xfr_data(uchar *xfr, uint size, uint port)
{
	uint i;
	uart_putc(port,PTC_HEADER);
	for(i=0; i < size; i++)
	{
		if(*xfr == PTC_HEADER)
		{
			uart_putc(port,PTC_ESC_CHARACTOR);
			uart_putc(port,PTC_ESC_HEADER);
		}
		else if(*xfr == PTC_ESC_CHARACTOR)
		{
			uart_putc(port,PTC_ESC_CHARACTOR);
			uart_putc(port,PTC_ESC_APPEND);
		}
		else
			uart_putc(port,*xfr);
		xfr++;
	}
	uart_putc(port,PTC_TAIL);
}

void uart_rcv_data(uchar *rcv, uint port)
{
	uchar val;
	uint limit = 0;
	uint process = PCS_NONE;

	while(1)
	{
		val = uart_getc(port);
		if(limit > RECV_BUF_SIZE)
			return;
		if(process == PCS_NONE)
		{
			if(val == PTC_HEADER)
				process = PCS_START;
		}
		else if(process == PCS_START)
		{
			if(val == PTC_HEADER)
				continue;
			process = PCS_CONTINUE;
			*rcv++ = val;
			limit++;
		}
		else if(process == PCS_CONTINUE)
		{
			switch(val)
			{
				case PTC_ESC_CHARACTOR:
					val = uart_getc(port);
					if(val == PTC_ESC_HEADER)
					{
						*rcv++ = PTC_HEADER;
						limit++;
					}
					else if(val == PTC_ESC_APPEND)
					{
						*rcv++ = PTC_ESC_CHARACTOR;
						limit++;
					}
					else
					{
						*rcv++ = PTC_ESC_CHARACTOR;
						*rcv++ = val;
						limit += 2;
					}
					break;
				case PTC_TAIL:
					process = PCS_END;
					return;
				default:
					*rcv++ = val;
					limit++;
					break;
			}
		}
	}
}
void send_ack(ushort ack, uint port)
{
	uint len = 0;
	ushort crc = 0;
	uchar *pb = txfr_buf;
	memset(txfr_buf,0,sizeof(txfr_buf));
	*pb++ = (ack >> 8) & 0xff;
	*pb++ = (ack) & 0xff;
	*pb++ = 0;
	*pb++ = 0;
	len = 4;
	crc = crc16(crc,txfr_buf,len);
	*pb++ = (crc >> 8) & 0xff;
	*pb = crc & 0xff;
	len += 2;
	uart_xfr_data(txfr_buf,len,port);
}

void complete_info(uint info, uint port)
{
	uint len = 0;
	ushort crc = 0;
	uchar *pb = txfr_buf;
	memset(txfr_buf,0,sizeof(txfr_buf));
	*pb++ = (FUNC_COMP_INFO >> 8) & 0xff;
	*pb++ = (FUNC_COMP_INFO) & 0xff;
	*pb++ = 0;
	*pb++ = 0x4;
	*pb++ = (info >> 24) & 0xff;
	*pb++ = (info >> 16) & 0xff;
	*pb++ = (info >> 8) & 0xff;
	*pb++ = info & 0xff;
	len = 8;
	crc = crc16(crc,txfr_buf,len);
	*pb++ = (crc >> 8) & 0xff;
	*pb = crc & 0xff;
	len += 2;
	uart_xfr_data(txfr_buf,len,port);
}

typedef void (*pfunc)(struct down_info *);

const ushort func_list[] = {
	FUNC_START,
	FUNC_CONNECT,
	FUNC_CONTINUE,
	FUNC_FINISH,
	FUNC_EXEC,
	FUNC_XCH,
	FUNC_COMP_INFO
};


void down_connect(struct down_info *info)
{
	if(info->status != STATUS_NONE)
		return;
	info->status = STATUS_CONNECTED;
	send_ack(PTC_ACK, info->port);
}
void down_start(struct down_info *info)
{
	if(info->status != STATUS_CONNECTED)
		return;
#ifdef CONFIG_SDL_BUILD
	info->imageid = swab32(*((uint *)&info->rcv[4]));
	info->address = MapTbl[info->imageid].downaddr;
#else
	info->address = swab32(*((uint *)&info->rcv[4]));
#endif
	info->total_len = swab32(*((uint *)&info->rcv[8]));
	info->cur_len = 0;
	if(info->total_len > DownLoad_Max_Size)
		send_ack(PTC_NACK,info->port);
	else
	{
		info->status = STATUS_CONTINUE;
		send_ack(PTC_ACK,info->port);
	}
}
void down_continue(struct down_info *info)
{
	ushort len;
	if(info->status != STATUS_CONTINUE)
		return;
	len = swab16(*((ushort *)&info->rcv[2]));
	info->cur_len += len;
	if(info->cur_len <= DownLoad_Max_Size)
	{
		memcpy((void *)info->address, &(info->rcv[4]),len);
		info->address += len;
		send_ack(PTC_ACK, info->port);
	}
	else
	{
		send_ack(PTC_NACK, info->port);
	}
}
void down_finish(struct down_info *info)
{
	if(info->status != STATUS_CONTINUE)
		return;
	if(info->total_len == info->cur_len)
	{
		info->status = STATUS_FINISH;
		send_ack(PTC_ACK,info->port);
	}
	else
	{
		send_ack(PTC_NACK,info->port);
	}
}
void down_execute(struct down_info *info)
{
	uint addr = 0;
	ushort len = 0;
	if(info->status != STATUS_FINISH)
		return;
	len = swab16(*((ushort *)&info->rcv[2]));
	addr = swab32(*((uint *)&info->rcv[4]));
	if((len == 4) && (addr <= info->address))
	{
		info->status = STATUS_EXECUTE;
		send_ack(PTC_ACK,info->port);
#ifdef CONFIG_PDL_BUILD
		nosec_boot(addr);
#endif
	}
	else
		send_ack(PTC_NACK,info->port);
}
void xch_baudrate(struct down_info *info)
{
	uint baudrate = 0;
	if(info->status != STATUS_CONNECTED)
		return;
	baudrate = swab32(*((uint *)&info->rcv[4]));
	if(baudrate > BAUDRATE_1500000)
		baudrate = BAUDRATE_1500000;
	send_ack(PTC_ACK,info->port);
	delayms(1);
	uart_set_baudrate(baudrate, info->port);
}

void down_comp_info(struct down_info *info)
{
    if(!info->comp_status)
        complete_info(INFO_OK, info->port);
    else
        complete_info(INFO_OTHER, info->port);
}

const pfunc pftbl[] = {
    down_connect,
    down_start,
    down_continue,
    down_finish,
    down_execute,
    xch_baudrate,
    down_comp_info,
};

void uart_send_heart_beat(void)
{
	static uint counter = 0;
	counter++;
	if(counter & 0x80)
	{
		counter = 0;
		uart_putc(REG_UART0_BASE,UART_HEART_BEAT);
//		uart_putc(REG_UART1_BASE,UART_HEART_BEAT);
	}
}
void uart_handshake(uint port, uint *u)
{
	bool ret;
	uchar val = 0;
	ret = uart_read_rbr(port,&val);
	if(ret == true)
	{
		if(val == UART_TOKEN)
			*u += 1;
	}
	return;
}
uint uart_identify(void)
{
	uint u1=0;
//	uint u2=0;

	while(1)
	{
		uart_send_heart_beat();
		uart_handshake(REG_UART0_BASE,&u1);
//		uart_handshake(REG_UART1_BASE,&u2);

		if(u1 >= UART_TOKEN)
			return REG_UART0_BASE;

//		if(u2 >= UART_TOKEN)
//			return REG_UART1_BASE;
	}
}

void send_string(uchar *buf,uint port)
{
	ushort len = 0;
	ushort crc = 0;
	uchar *pb = buf;
	*pb++ = (PTC_ACK >> 8) & 0xff;
	*pb++ = (PTC_ACK) & 0xff;
	len = STRING_SIZE;
	*pb++ = (len >> 8) & 0xff;
	*pb++ = (len) & 0xff;
	strcpy((char*)pb,DOWNLOAD_STRING);
	pb += STRING_SIZE;
	len += 4;
	crc = crc16(crc,buf,len);
	*pb++ = (crc >> 8) & 0xff;
	*pb = crc & 0xff;
	len += 2;
	uart_xfr_data(buf,len,port);
}

bool crc_check(uchar *buf)
{
	uint crc_pos;
	uint len;
	ushort crc_transfer = 0;
	ushort crc_calculate = 0;
	len = (buf[2] << 8) + buf[3];
	len += 4;
	crc_pos = len;
	if(len >= RECV_BUF_SIZE)
		return false;
	crc_transfer = (buf[crc_pos] << 8) + buf[crc_pos + 1];
	crc_calculate = crc16(crc_calculate,buf,len);
	if(crc_transfer == crc_calculate)
		return true;
	else
		return false;
}
bool lookfor_func(ushort f,uchar *idx)
{
	uint i = 0;
	uint s = ARRAY_SIZE(func_list);
	for(i=0; i<s; i++)
	{
		if(f == func_list[i])
		{
			*idx = i;
			return true;
		}
	}
	return false;
}
void uart_preboot(uint port)
{
    d_info.xfr = txfr_buf;
    d_info.rcv = recv_buf;
    d_info.port = port;
    d_info.status = STATUS_NONE;
    d_info.comp_status = INFO_OTHER;
    send_string(d_info.xfr,port);
}
void uart_boot(uint port)
{
	bool ret;
	uint func;
	uchar idx = 0;
#ifdef CONFIG_SDL_BUILD
	while(d_info.status != STATUS_FINISH)
#else
	while(d_info.status != STATUS_EXECUTE)
#endif
	{
		memset(recv_buf,0,sizeof(recv_buf));
		uart_rcv_data(d_info.rcv, port);
		ret = crc_check(d_info.xfr);
		func = (d_info.rcv[0] << 8) + d_info.rcv[1];
		ret &= lookfor_func(func, &idx);
		if(ret == false)
		{
			send_ack(PTC_NACK,port);
			continue;
		}

		(*pftbl[idx])(&d_info);
	}
	d_info.status = STATUS_CONNECTED;
}


