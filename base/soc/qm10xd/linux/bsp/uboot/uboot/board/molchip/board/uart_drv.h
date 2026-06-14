#ifndef __UART_DRV_H_
#define __UART_DRV_H_

#define UART_RBR_OFFSET		0x00
#define UART_THR_OFFSET		0x00
#define UART_DLL_OFFSET		0x00
#define UART_IER_OFFSET		0x04
#define UART_DLH_OFFSET		0x04
#define UART_IIR_OFFSET		0x08
#define UART_FCR_OFFSET		0x08
#define UART_LCR_OFFSET		0x0C
#define UART_MCR_OFFSET		0x10
#define UART_LSR_OFFSET		0x14
#define UART_MSR_OFFSET		0x18
#define UART_SCR_OFFSET		0x1C
#define UART_LPDLL_OFFSET	0x20
#define UART_LPDLH_OFFSET	0x24
#define UART_SRBRL_OFFSET	0x30
#define UART_SRBRH_OFFSET	0x6C
#define UART_STHRL_OFFSET	0x30
#define UART_STHRH_OFFSET	0x6C
#define UART_FAR_OFFSET		0x70
#define UART_TFR_OFFSET		0x74
#define UART_RFW_OFFSET		0x78
#define UART_USR_OFFSET		0x7C
#define UART_TFL_OFFSET		0x80
#define UART_RFL_OFFSET		0x84
#define UART_SRR_OFFSET		0x88
#define UART_SRTS_OFFSET	0x8C
#define UART_SBCR_OFFSET	0x90
#define UART_SDMAM_OFFSET	0x94
#define UART_SFE_OFFSET		0x98
#define UART_SRT_OFFSET		0x9C
#define UART_STET_OFFSET	0xA0
#define UART_HTX_OFFSET		0xA4
#define UART_DMASA_OFFSET	0xA8
#define UART_CPR_OFFSET		0xF4
#define UART_UCR_OFFSET		0xF8
#define UART_CTR_OFFSET		0xFC

// LCR
#define LCR_DLAB		BIT(7)
#define LCR_BREAK		BIT(6)
#define LCR_STICK_PARITY	BIT(5)
#define LCR_EPS			BIT(4)
#define LCR_PEN			BIT(3)
#define LCR_STOP		BIT(2)
#define LCR_DLS_OFFSET		0
#define LCR_DLS_MASK		3
#define DLS_5b			0
#define DLS_6b			1
#define DLS_7b			2
#define DLS_8b			3
#define PORITY_ODD		0
#define PORTIY_EVEN		1
#define STOP_1b			0
#define STOP_2b			1

#define USR_RFF			BIT(4)
#define USR_RFNE		BIT(3)
#define USR_TFE			BIT(2)
#define USR_TFNF		BIT(1)
#define USR_BUSY		BIT(0)

#define FCR_DMA_MODE		BIT(3)
#define FCR_XMIT_FIFO_RESET	BIT(2)
#define FCR_RCVR_FIFO_RESET	BIT(1)
#define FCR_FIFO_ENABLE		BIT(0)

#define LSR_RFE			BIT(7)
#define LSR_TEMT		BIT(6)
#define LSR_THRE		BIT(5)
#define LSR_BI			BIT(4)
#define LSR_FE			BIT(3)
#define LSR_PE			BIT(2)
#define LSR_OE			BIT(1)
#define LSR_DR			BIT(0)

#define SFE_SFE			BIT(0)

#define get_uart_lsr(p)		uart_read(UART_LSR_OFFSET, p)
#define get_uart_rbr(p)		uart_read(UART_RBR_OFFSET, p)
#define UART_CTL_CLK_64M	64000000
#define UART_CTL_CLK_24M	24000000
#define BAUDRATE_115200		115200
#define BAUDRATE_1000000	1000000
#define BAUDRATE_1500000	1500000

#define uart_write(v, r, p)	__raw_writel(v, (p) + (r))
#define uart_read(r, p)		__raw_readl((p) + (r))

#define shadow_fifo_enable(p)	uart_write(SFE_SFE, UART_SFE_OFFSET, (p))
#define shadow_fifo_disable(p)	uart_write(~SFE_SFE, UART_SFE_OFFSET, (p))

#define FIFO_R			0
#define FIFO_X			1
#define FIFO_XR			2

void uart_enable(bool enable);
void uart_reset(void);
void uart_init(u32 port);
u8 uart_getc(u32 port);
void uart_putc(u32 port, u8 xfer);
void uart_puts(char *s);
void uart_set_baudrate(u32 baudrate, u32 port);
bool uart_read_rbr(u32 port, u8 *rcv);


int Uart_Line_Status_Din(void);
int Uart_Read_Data(unsigned char *buf, unsigned int size);
int Uart_Write_Data(unsigned char *buf, unsigned int size);
int uart_readb(u8 *byte);

#endif
