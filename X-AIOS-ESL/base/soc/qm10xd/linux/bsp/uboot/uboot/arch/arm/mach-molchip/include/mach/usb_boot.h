#ifndef __USB_BOOT_
#define __USB_BOOT_

#define USB_HEART_BEAT		0x4d
#define USB_TOKEN		0x7e
#define USB_TOKEN_NUM		3

#define USB_PTC_HEADER		0x7e
#define USB_PTC_ESC_CHARACTER	0x7c	//7e :7c 7b ;7c:7c 00
#define USB_PTC_ESC_HEADER		0x7b
#define USB_PTC_ESC_APPEND		0x00
#define USB_PTC_TAIL		0x7e

#define USB_PTC_ACK			0x0061	//a
#define USB_PTC_NACK		0x6e61	//na

#define USB_FUNC_CONNECT		0x46F0
#define USB_FUNC_START		0x46F1
#define USB_FUNC_CONTINUE		0x46F2
#define USB_FUNC_FINISH		0x46F3
#define USB_FUNC_EXEC		0x46F4
#define USB_FUNC_XCH		0x46F5


#define USB_FUNC_COMP_INFO	0x46F6
#define USB_INFO_OK			0x0
#define USB_INFO_UNSUPPORT  0x1
#define USB_INFO_TIMEOUT	0x2
#define USB_INFO_OTHER		0x3


#define USB_PCS_NONE		0
#define USB_PCS_START		1
#define USB_PCS_CONTINUE		2
#define USB_PCS_END			3

#define USB_DOWNLOAD_STRING		"~enter_usb_download~"
#define USB_STRING_SIZE		strlen(USB_DOWNLOAD_STRING)

#define USB_RECV_BUF_SIZE		1024
//#define USB_XFR_BUF_SIZE		64

#define USB_STATUS_NONE		0
#define USB_STATUS_CONNECTED	1
#define USB_STATUS_CONTINUE		2
#define USB_STATUS_FINISH		3
#define USB_STATUS_EXECUTE		4

typedef void (*usb_pfunc)(struct down_info *);

void usb_down_connect(struct down_info *);
void usb_down_start(struct down_info *);
void usb_down_continue(struct down_info *);
void usb_down_finish(struct down_info *);
void usb_down_execute(struct down_info *);
void usb_xch_baudrate(struct down_info *);
void usb_boot(void);
void usb_send_ack(u16 ack);

void usb_identify(void);
void usb_preboot(void);

void usb_complete_info(u16 info);


#endif
