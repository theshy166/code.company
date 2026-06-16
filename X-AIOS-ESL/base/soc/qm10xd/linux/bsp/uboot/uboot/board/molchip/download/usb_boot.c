#include <common.h>
#include <mach/hardware.h>
#include <mach/download.h>
#include <mach/crc16.h>
#include <mach/spl.h>
#include <mach/board.h>
#include <mach/systimer.h>
#include <mach/usb_hw.h>
#include <mach/usb_drv.h>
#include <mach/usb_boot.h>
#include <mach/uart_drv.h>

unsigned char rx_buf[1024]  __attribute__((aligned(4))) = {0};
unsigned char tx_buf[512]  __attribute__((aligned(4))) = {0};


extern struct down_info d_info;
extern DownLoadMap MapTbl[];

void usb_complete_info(u16 info)
{
	u16 len = 0;
	u16 crc = 0;
	u8 *pb = d_info.xfr;

	*pb++ = (FUNC_COMP_INFO >> 8) & 0xff;
	*pb++ = (FUNC_COMP_INFO) & 0xff;
	*pb++ = 0;
	*pb++ = 0x4;
	*pb++ = (info >> 24) & 0xff;
	*pb++ = (info >> 16) & 0xff;
	*pb++ = (info >> 8) & 0xff;
	*pb++ = info & 0xff;
	len = 8;
	crc = crc16(crc,d_info.xfr,len);
	*pb++ = (crc >> 8) & 0xff;
	*pb = crc & 0xff;
	len += 2;

	if(!usb_xfr_data(d_info.xfr,len))
		uart_puts("USF\r\n");
}

const usb_pfunc usb_pftbl[] = 
{
	usb_down_connect,
	usb_down_start,
	usb_down_continue,
	usb_down_finish,
	usb_down_execute,
	usb_xch_baudrate,
};
const u16 usb_func_list[] =
{
	USB_FUNC_CONNECT,
	USB_FUNC_START,
	USB_FUNC_CONTINUE,
	USB_FUNC_FINISH,
	USB_FUNC_EXEC,
	USB_FUNC_XCH,
};

void usb_down_connect(struct down_info *info)
{
	if(info->status != USB_STATUS_NONE)
		return;
	info->status = USB_STATUS_CONNECTED;
	usb_send_ack(USB_PTC_ACK);
}

void usb_down_start(struct down_info *info)
{

	if(info->status != USB_STATUS_CONNECTED)
		return;

#ifdef CONFIG_SDL_BUILD
	info->imageid = swab32(*((uint *)&info->rcv[4]));
	info->address = MapTbl[info->imageid].downaddr;
#else
	info->address = swab32(*((uint *)&info->rcv[4]));
#endif	
	info->total_len = swab32(*((u32 *)&info->rcv[8]));
	info->cur_len = 0;
	
	if(info->total_len > DownLoad_Max_Size)
		usb_send_ack(USB_PTC_NACK);
	else
	{
		info->status = USB_STATUS_CONTINUE;
		usb_send_ack(USB_PTC_ACK);
	}
}

void usb_down_continue(struct down_info *info)
{
	u16 len;
	
	if(info->status != USB_STATUS_CONTINUE)
		return;
	len = swab16(*((u16 *)&info->rcv[2]));	
	info->cur_len += len;

	if(info->cur_len <= DownLoad_Max_Size)
	{
		memcpy((void *)info->address,&(info->rcv[4]),len);
		info->address += len;
		usb_send_ack(USB_PTC_ACK);
	}
	else
		usb_send_ack(USB_PTC_NACK);
}

void usb_down_finish(struct down_info *info)
{

	if(info->status != USB_STATUS_CONTINUE)
		return;
	if(info->total_len == info->cur_len)
	{
		info->status = USB_STATUS_FINISH;
		usb_send_ack(USB_PTC_ACK);
	}	
	else
		usb_send_ack(USB_PTC_NACK);
}

void usb_down_execute(struct down_info *info)
{
	u32 addr = 0;
	u16 len = 0;
	
	if(info->status != USB_STATUS_FINISH)
		return;
	len = swab16(*((u16 *)&info->rcv[2]));	
	addr = swab32(*((u32 *)&info->rcv[4]));
	if((len == 4) && (addr <= info->address))
	{
		info->status = USB_STATUS_EXECUTE;	
		usb_send_ack(USB_PTC_ACK);
#ifdef CONFIG_PDL_BUILD
		nosec_boot(addr);
#endif
	}
	else
		usb_send_ack(USB_PTC_NACK);
}

void usb_xch_baudrate(struct down_info *info)
{

//	u32 baudrate = 0;
	if(info->status != USB_STATUS_CONNECTED)
		return;
//	baudrate = swab32(*((u32 *)&info->rcv[4]));
//	if(baudrate > BAUDRATE_1500000)
//		baudrate = BAUDRATE_1500000;
	usb_send_ack(USB_PTC_ACK);
//	delay_ms(10);
//	uart_set_baudrate(baudrate, info->port);
}

/*void uart_send_heart_beat(void)
{
	static u32 counter = 0;
	counter++;
	if(counter&0x80)
	{
		counter = 0;
		uart_putc(REG_UART0_BASE,UART_HEART_BEAT);
		//uart_putc(REG_UART1_BASE,UART_HEART_BEAT);
	}
}*/

void usb_identify(void)
{
	u32 u = 0;

	dwc2_sw_init();

	while(1)
	{
		usb_get_data(rx_buf,1);

		if (rx_buf[0] == USB_TOKEN)
			u += 1;

		if(u >= UART_TOKEN)
			return;
	}

	return;
}


void usb_rcv_data(u8 *rcv)
{
	if(!usb_get_data(rcv,sizeof(rx_buf)))
		uart_puts("URF\r\n");
}

void usb_send_string(u8 *buf)
{
	u16 len = 0;
	u16 crc = 0;
	u8 *pb = buf;

	*pb++ = (USB_PTC_ACK >> 8) & 0xff;
	*pb++ = (USB_PTC_ACK) & 0xff;
	len = USB_STRING_SIZE;
	*pb++ = (len >> 8) & 0xff;
	*pb++ = (len) & 0xff;
	strcpy((char *)pb,USB_DOWNLOAD_STRING);
	pb += USB_STRING_SIZE;
	len += 4;
	crc = crc16(crc,buf,len);
	*pb++ = (crc >> 8) & 0xff;
	*pb = crc & 0xff;
	len += 2;

	if(!usb_xfr_data(buf,len))
		uart_puts("USF\r\n");

}

void usb_send_ack(u16 ack)
{
	u32 len = 0;
	u16 crc = 0;
	u8 *pb = tx_buf;

	memset(tx_buf,0,sizeof(tx_buf));
	*pb++ = (ack >> 8) & 0xff;
	*pb++ = (ack) & 0xff;
	*pb++ = 0;
	*pb++ = 0;
	len = 4;
	crc = crc16(crc,tx_buf,len);
	*pb++ = (crc >> 8) & 0xff;
	*pb = crc & 0xff;
	len += 2;

	if(!usb_xfr_data(tx_buf,len))
		uart_puts("UAF\r\n");
}

bool usb_crc_check(u8 *buf)
{
	u32 crc_pos;
	u32 len;
	u16 crc_transfer = 0;
	u16 crc_calculate = 0;
	len = (buf[2] << 8) + buf[3];
	len += 4;
	crc_pos = len;
	if(len >= USB_RECV_BUF_SIZE)
		return false;
	crc_transfer = (buf[crc_pos] << 8) + buf[crc_pos + 1];
	crc_calculate = crc16(crc_calculate,buf,len);
	if(crc_transfer == crc_calculate)
		return true;
	else
		return false;
}

bool usb_lookfor_func(u16 f,u8 *idx)
{
	u32 i = 0;
	u32 s = ARRAY_SIZE(usb_func_list);
	for(i = 0; i < s;i++)
		if(f == usb_func_list[i])
		{
			*idx = i;
			return true;
		}
	return false;
}

void usb_preboot(void)
{
	d_info.xfr = tx_buf;
	d_info.rcv = rx_buf;
	d_info.status = USB_STATUS_NONE;
	
	usb_send_string(d_info.xfr);
}

void usb_boot(void)
{
	bool ret;
	u32 func;
	u8 idx = 0;
#ifdef CONFIG_SDL_BUILD
	while(d_info.status != USB_STATUS_FINISH)
#else
	while(d_info.status != USB_STATUS_EXECUTE)
#endif
	{
		memset(rx_buf,0,sizeof(rx_buf));
		usb_rcv_data(d_info.rcv);
		ret = usb_crc_check(d_info.rcv);
		func = (d_info.rcv[0] << 8) + d_info.rcv[1];
		ret &= usb_lookfor_func(func, &idx);
		if(ret == false)
		{
			usb_send_ack(USB_PTC_NACK);
			continue;
		}
		(*usb_pftbl[idx])(&d_info);

	}
	d_info.status = USB_STATUS_CONNECTED;

}

