#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/uart.h>

void uart_reset(void)
{
	u32 i;
	setbits_le32(REG_AP_PERI_SOFT_RST,UART0_SOFT_RST | UART1_SOFT_RST);

	for(i=0;i<0x100;i++);

	clrbits_le32(REG_AP_PERI_SOFT_RST,UART0_SOFT_RST | UART1_SOFT_RST);

}
void uart_set_baudrate(u32 baudrate, u32 port)
{
	u32 divisor;
	u32 val;

	divisor = UART_CTL_CLK_24M >> 4;
	divisor = divisor / baudrate;

	val = uart_read(UART_LCR_OFFSET,port);
	val |= LCR_DLAB;
	uart_write(val,UART_LCR_OFFSET,port);

	uart_write(divisor,UART_DLL_OFFSET,port);
	uart_write(0,UART_DLH_OFFSET,port);
	val &= ~LCR_DLAB;
	uart_write(val,UART_LCR_OFFSET,port);
}
void uart_select_data_len(u32 len, u32 port)
{
	u32 val;
	val = uart_read(UART_LCR_OFFSET,port);
	val &= ~(LCR_DLS_MASK << LCR_DLS_OFFSET);
	val |= (len << LCR_DLS_OFFSET);
	uart_write(val,UART_LCR_OFFSET,port);
}
void uart_pority_enable(bool en, u32 port)
{
	u32 val;
	val = uart_read(UART_LCR_OFFSET,port);
	if(en)
		val |= LCR_PEN;
	else
		val &= ~LCR_PEN;
	uart_write(val, UART_LCR_OFFSET, port);
}
void uart_select_pority(u32 pority,u32 port)
{
	u32 val;
	val = uart_read(UART_LCR_OFFSET,port);
	if(pority == PORITY_EVEN)
		val |= LCR_EPS;
	else
		val &= ~LCR_EPS;
	uart_write(val,UART_LCR_OFFSET,port);
}
void uart_config_stop(u32 stop, u32 port)
{
	u32 val;
	val = uart_read(UART_LCR_OFFSET,port);
	if(stop == STOP_2b)
		val |= LCR_STOP;
	else
		val &= ~LCR_STOP;
	uart_write(val,UART_LCR_OFFSET,port);
}
void uart_fifo_reset(u32 port, u8 fifo)
{
	u32 val;
	val = uart_read(UART_FCR_OFFSET,port);
	switch(fifo)
	{
		case FIFO_R:
			val |= FCR_RCVR_FIFO_RESET;
			break;
		case FIFO_X:
			val |= FCR_XMIT_FIFO_RESET;
			break;
		case FIFO_XR:
			val |= FCR_RCVR_FIFO_RESET | FCR_XMIT_FIFO_RESET;
			break;
		default: break;
	}
	uart_write(val,UART_FCR_OFFSET,port);
}
void uart_fifo_enable(bool en, u32 port)
{
	u32 val;
	val = uart_read(UART_FCR_OFFSET,port);
	if(en)
		val |= FCR_FIFO_ENABLE;
	else
		val &= ~FCR_FIFO_ENABLE;
	uart_write(val,UART_FCR_OFFSET,port);
}
void uart_init(u32 port)
{
//	uart_set_baudrate(BAUDRATE_115200, port);
	uart_select_data_len(DLS_8b,port);
	uart_pority_enable(false,port);
	uart_config_stop(STOP_1b,port);
#ifdef CONFIG_TARGET_FY12
	uart_fifo_enable(true,port);
#else
	uart_fifo_enable(false,port);
#endif
}

void uart_hw_init(void)
{

	uart_init(REG_UART0_BASE);

	uart_init(REG_UART1_BASE);

	//uart_init(REG_UART2_BASE);
}

bool uart_read_rbr(u32 port,u8 *rcv)
{
	u32 lsr;
	lsr = get_uart_lsr(port);
	if(lsr & LSR_DR)
	{
		*rcv = get_uart_rbr(port);
		return true;
	}
	return false;
}

u8 uart_getc(u32 port)
{
	u8 rbr;
	u32 lsr = 0;
	do
	{
		lsr = get_uart_lsr(port);
		lsr &= LSR_DR;
	}while(!lsr);
	rbr = get_uart_rbr(port);
	return rbr;
}

void uart_putc(u32 port, u8 xfer)
{
	u32 lsr = 0;
	do
	{
		lsr = get_uart_lsr(port);
		lsr &= LSR_TEMT;
	}while(!lsr);
	uart_write(xfer,UART_THR_OFFSET,port);
}

void uart_puts(char *s)
{
	while(*s != '\0')
	{
		uart_putc(REG_UART0_BASE,*s);
		s++;
	}
	uart_putc(REG_UART0_BASE,'\r');
}

#ifdef CONFIG_SDL_BUILD
void puts(const char *s)
{
	uart_puts((char *)s);
}

void putc(const char c)
{
	uart_putc(REG_UART0_BASE, (u8)c);
}

#endif
static const char digits[16] = "0123456789ABCDEF";

static void uart_put_int(int num, unsigned int radix, unsigned int pad_num, unsigned char pad_pat)
{
	u8 pbuf[12];
	unsigned int unum, index, i, sign = 0;
	int need_pad = 0;

	if(radix == 10 && num < 0) {
		unum = (unsigned int)-num;
		sign = 1;
	} else {
		unum = (unsigned int)num;
	}

	if (pad_num > 9)
		pad_num = 9;

	index = 0;
	do {
		pbuf[index++] = digits[unum % radix];
		unum /= radix;
	} while (unum);

	need_pad = pad_num - index;

	if(need_pad > 0) {
		for(i=0;i<need_pad;i++)
			pbuf[index++] = pad_pat;
	}

	if(sign == 1)
		pbuf[index++] = '-';

	pbuf[index] = '\0';

	for(i=0;i < index;i++) {
		uart_putc(REG_UART0_BASE, pbuf[index-1-i]);
	}
}

static unsigned int is_radix(unsigned char radix)
{
	static unsigned char radix_list[] = {'d','c','s','x','X'};
	static unsigned int sz_radix_list = sizeof(radix_list)/sizeof(radix_list[0]);
	unsigned int is_rx = 0, i;
	for(i=0;i<sz_radix_list;i++) {
		if(radix == radix_list[i]) {
			is_rx = 1;
			break;
		}
	}
	return is_rx;
}

void mc_printf(const char *format, ...)
{
	va_list unnamed_p;
	char *p, sval, *p_sval;
	int value_i;
	unsigned int pad_num = 0;
	unsigned char pad_pat = 0;

	va_start(unnamed_p, format);
	for(p = (char *)format; *p != '\0'; p++) {
		if (*p != '%') {
			uart_putc(REG_UART0_BASE, *p);
			continue;
		} else {
			if(is_radix(*(p+1)))
				p++;
			else {
				if (*(p+1) == '0') {
					pad_pat = '0';
					if ((*(p+2) > '0') && (*(p+2) <= '9')) {
						pad_num = *(p+2) - '0';
						p += 3;
					} else {
						uart_puts("maybe a wrong pad number!!!\n");
						return;
					}
				} else {
					pad_pat = ' ';
					if ((*(p+1) > '0') && (*(p+1) <= '9')) {
						pad_num = *(p+1) - '0';
						p += 2;
					} else {
						uart_puts("maybe a wrong pad number!!!\n");
						return;
					}
				}
			}
		}

		switch (*p) {
			case 'd':
				value_i = va_arg(unnamed_p, int);
				uart_put_int(value_i, 10, pad_num, pad_pat);
				pad_num = 0;
				pad_pat = 0;
				break;
			case 'c':
				sval = va_arg(unnamed_p, int);
				uart_putc(REG_UART0_BASE, sval);
				break;
			case 's':
				for(p_sval = va_arg(unnamed_p, char *); *p_sval; p_sval++)
					uart_putc(REG_UART0_BASE, *p_sval);
				break;
			case 'x':
			case 'X':
				value_i = va_arg(unnamed_p, unsigned int);
				uart_put_int(value_i, 16, pad_num, pad_pat);
				pad_num = 0;
				pad_pat = 0;
				break;
			default:
				uart_putc(REG_UART0_BASE, *p);
				break;
		}
	}
	va_end(unnamed_p);
}

#if 0
void uart1_puts(char *s)
{
	while(*s != '\0')
	{
		uart_putc(REG_UART1_BASE,*s);
		s++;
	}
	uart_putc(REG_UART1_BASE,'\r');
}

static void uart1_put_int(int num, unsigned int radix, unsigned int pad_num, unsigned char pad_pat)
{
	u8 pbuf[12];
	unsigned int unum, index, i, sign = 0;
	int need_pad = 0;

	if(radix == 10 && num < 0) {
		unum = (unsigned int)-num;
		sign = 1;
	} else {
		unum = (unsigned int)num;
	}

	if (pad_num > 9)
		pad_num = 9;

	index = 0;
	do {
		pbuf[index++] = digits[unum % radix];
		unum /= radix;
	} while (unum);

	need_pad = pad_num - index;

	if(need_pad > 0) {
		for(i=0;i<need_pad;i++)
			pbuf[index++] = pad_pat;
	}

	if(sign == 1)
		pbuf[index++] = '-';

	pbuf[index] = '\0';

	for(i=0;i < index;i++) {
		uart_putc(REG_UART1_BASE, pbuf[index-1-i]);
	}
}


void mc_printf1(const char *format, ...)
{
	va_list unnamed_p;
	char *p, sval, *p_sval;
	int value_i;
	unsigned int pad_num = 0;
	unsigned char pad_pat = 0;

	va_start(unnamed_p, format);
	for(p = (char *)format; *p != '\0'; p++) {
		if (*p != '%') {
			uart_putc(REG_UART1_BASE, *p);
			continue;
		} else {
			if(is_radix(*(p+1)))
				p++;
			else {
				if (*(p+1) == '0') {
					pad_pat = '0';
					if ((*(p+2) > '0') && (*(p+2) <= '9')) {
						pad_num = *(p+2) - '0';
						p += 3;
					} else {
						uart1_puts("maybe a wrong pad number!!!\n");
						return;
					}
				} else {
					pad_pat = ' ';
					if ((*(p+1) > '0') && (*(p+1) <= '9')) {
						pad_num = *(p+1) - '0';
						p += 2;
					} else {
						uart1_puts("maybe a wrong pad number!!!\n");
						return;
					}
				}
			}
		}

		switch (*p) {
			case 'd':
				value_i = va_arg(unnamed_p, int);
				uart1_put_int(value_i, 10, pad_num, pad_pat);
				pad_num = 0;
				pad_pat = 0;
				break;
			case 'c':
				sval = va_arg(unnamed_p, int);
				uart_putc(REG_UART1_BASE, sval);
				break;
			case 's':
				for(p_sval = va_arg(unnamed_p, char *); *p_sval; p_sval++)
					uart_putc(REG_UART1_BASE, *p_sval);
				break;
			case 'x':
			case 'X':
				value_i = va_arg(unnamed_p, unsigned int);
				uart1_put_int(value_i, 16, pad_num, pad_pat);
				pad_num = 0;
				pad_pat = 0;
				break;
			default:
				uart_putc(REG_UART1_BASE, *p);
				break;
		}
	}
	va_end(unnamed_p);
}

#endif
