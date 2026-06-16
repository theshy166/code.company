#ifndef __XMODEM_H__
#define __XMODEM_H__

#define XMODEM_RESTART			0xee
#define XMODEM_CAN_ERROR		0xcc
#define XMODEM_TIME_OUT			0xdd

#define PARSE_ALIGN_SIZE	0x1000  //4K
#define COPY_CODE2FLASH		0xaa5555aa


#define XMODEM_ERROR_PACK_NO				(1 << 0)
#define XMODEM_ERROR_ROOT_HEAD				(1 << 1)
#define XMODEM_ERROR_DATA_CRC				(1 << 2)
#define XMODEM_ERROR_CODE_CRC				(1 << 3)
#define XMODEM_ERROR_DATA_IMAGE_TIME_OUT		(1 << 4)
#define XMODEM_ERROR_TIME_OUT				(1 << 5)
#define XMODEM_ERROR_RETRY_TOO_MUCH			(1 << 6)
#define XMODEM_ERROR_RAMBOOT_CODE_SIZE			(1 << 7)

#define XMODEM_WAIT_TIMEOUT				(1000)

#define CRC_SEED					0xEDB88320



typedef struct _Xmodem
{
	unsigned int pack_no;		//record the pack number
	unsigned int retry_cnt;	//record the retry times
	unsigned int syn_retry_times;
	unsigned int can_flag;		//record the can error
	//UINT8 nak_flag;		//record the nak error
	unsigned int restart_flag;		//post message to the parser to remind the xmodem restart,this flag set by the xmodem,clear by the parse
	//UINT8 trans_status;
	unsigned int frame_state_mac;
	unsigned int time_out_flag;
	unsigned int error;
}Xmodem;

struct device_ops {
	int (*init)(void *data);
	int (*dev_info_update)(void *data);
	int (*read)(unsigned int offset, unsigned char *buf, unsigned int size);
	int (*write)(unsigned int offset, unsigned char *buf, unsigned int size);
	int (*erase)(unsigned int offset, unsigned int unit_size, unsigned int size);
};

void Xmodem_Start(unsigned int *exit_add);
void Xmodem_Init(void);
void xmodem_boot(void);
void Xmodem_ParseInit(void);


#endif
