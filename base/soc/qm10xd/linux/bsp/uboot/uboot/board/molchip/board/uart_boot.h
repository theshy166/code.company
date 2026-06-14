#ifndef __UART_BOOT_
#define __UART_BOOT_

#define UART_HEART_BEAT		0x4d
#define UART_TOKEN		0x7e
#define TOKEN_NUM		3

#define PTC_HEADER		0x7e
#define PTC_ESC_CHARACTER	0x7c	//7e :7c 7b ;7c:7c 00
#define PTC_ESC_HEADER		0x7b
#define PTC_ESC_APPEND		0x00
#define PTC_TAIL		0x7e

#define PTC_ACK			0x0061	//a
#define PTC_NACK		0x6e61	//na

#define FUNC_CONNECT		0x46F0
#define FUNC_START		0x46F1
#define FUNC_CONTINUE		0x46F2
#define FUNC_FINISH		0x46F3
#define FUNC_EXEC		0x46F4
#define FUNC_XCH		0x46F5


#define PCS_NONE		0
#define PCS_START		1
#define PCS_CONTINUE		2
#define PCS_END			3

#define DOWNLOAD_STRING		"~enter_download~"
#define STRING_SIZE		sstrlen(DOWNLOAD_STRING)

#define UART0_BOOT		0x5530
#define UART1_BOOT		0x5531

#define RECV_BUF_SIZE		1024
#define XFR_BUF_SIZE		64

#define STATUS_NONE		0
#define STATUS_CONNECTED	1
#define STATUS_CONTINUE		2
#define STATUS_FINISH		3
#define STATUS_EXECUTE		4

struct down_info {
	u8 *rcv;
	u8 *xfr;
	u32 port;
	u32 address;
	u32 cur_len;
	u32 total_len;
	u32 status;
};

typedef void (*pfunc)(struct down_info *);

void down_connect(struct down_info *);
void down_start(struct down_info *);
void down_continue(struct down_info *);
void down_finish(struct down_info *);
void down_execute(struct down_info *);
void xch_baudrate(struct down_info *);
void uart_download(void);
void uart_boot(u32 port);
void send_ack(u16 ack, u32 port);

#endif
