#ifndef _SERIAL_MOLCHIP__
#define _SERIAL_MOLCHIP__

#define MOL_NON_FIFO_DMA_LENGTH_MAX 	0xfffffff
#define UART_RX_ADDR			0x100000
#define UART_RX_BUFFER_SIZE		0x1ff8
#define UART_RX_BUFFER_GUARDSIZE	0x8
#define UART_GUARD_PATTERN		0xff
#define TRANS_RESET_LEN			0x10000
#define UART_RX_CHN			31
#define FRAG_LEN_REG_VAL		0x00180001

struct ts01_uart {

	union URTD{
		u32 RBR;
		u32 THR;
		u32 DLL;
	} rtd;
	union UIDH{
		u32 IER;
		u32 DLH;
	}idh;
	union UICR{
		u32 IIR;
		u32 FCR;
	}icr;
	u32 lcr;
	u32 mcr;
	u32 lsr;
	u32 msr;
	u32 scr;
	u32 lpdll;
	u32 lpdlh;
	u32 rsvd0[2];
	union USRL{
		u32 SRBRL;
		u32 STHRL;
	}srl;
	u32 rdvd1[14];
	union USRH{
		u32 SRBRH;
		u32 STHRH;
	}srh;
	u32 far;
	u32 tfr;
	u32 rfw;
	u32 usr;
	u32 tfl;
	u32 rfl;
	u32 srr;
	u32 srts;
	u32 sbcr;
	u32 sdmam;
	u32 sfe;
	u32 srt;
	u32 stet;
	u32 htx;
	u32 dmasa;
	u32 cpr;
	u32 ucr;
	u32 ctr;
};

struct dma_rx_private_data{
	u32 			clen;
	u32			rlen;
	unsigned char 		*crx_dma_buf;
};
#endif
