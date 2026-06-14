#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define BAUDRATE_1500000            1500000
#define BAUDRATE_2000000            2000000
#define DownLoad_Max_Size           0x4000000

#define UART_IDENTIFY_TIMEOUT       60
#define UART_HEART_BEAT             0x4d
#define UART_TOKEN                  0x7e
#define TOKEN_NUM                   3

#define PTC_HEADER                  0x7E
#define PTC_ESC_CHARACTOR           0x7C //7e:7c 7b ; 7c:7c 00
#define PTC_ESC_HEADER              0x7B
#define PTC_ESC_APPEND              0x00
#define PTC_TAIL                    0x7E

#define PTC_ACK                     0x0061 //a
#define PTC_NACK                    0x6e61 //na

#define FUNC_START                  0x46F0
#define FUNC_CONNECT                0x46F1
#define FUNC_CONTINUE               0x46F2
#define FUNC_FINISH                 0x46F3
#define FUNC_EXEC                   0x46F4
#define FUNC_XCH                    0x46F5
#define FUNC_COMP_INFO              0x46F6

#define INFO_OK                     0x0
#define INFO_UNSUPPORT              0x1
#define INFO_TIMEOUT                0x2
#define INFO_OTHER                  0x3

#define STATUS_NONE                 0
#define STATUS_CONNECTED            1
#define STATUS_CONTINUE             2
#define STATUS_FINISH               3
#define STATUS_EXECUTE              4

#define PCS_NONE                    0
#define PCS_START                   1
#define PCS_CONTINUE                2
#define PCS_END                     3

#define DOWNLOAD_STRING             "~enter_download~"
#define STRING_SIZE                 strlen(DOWNLOAD_STRING)
#define RECV_BUF_SIZE               1024
#define XFR_BUF_SIZE                64

struct down_info{
    uchar       *rcv;
    uchar       *xfr;
    uint        port;
#ifdef CONFIG_SDL_BUILD
    ulong       imageid;
#endif
    ulong       address;
    uint        cur_len;
    uint        total_len;
    uint        status;
    int         comp_status;
};


typedef struct _TDownMap{
	u32 id;
	u32 downaddr;
	u32 nandaddr;
	u32 noraddr;
	u32 emmcaddr;
	u32 sdio1addr;
	u32 max_size;
}DownLoadMap;


enum{
	IMGID_CMD,
	IMGRAMADDR_CMD,
	IMG_NANDADDR_CMD,
	IMG_NORADDR_CMD,
	IMG_EMMCADDR_CMD,
	IMG_SDIO1ADDR_CMD,
	IMG_SIZE_CMD,
};


struct down_info* get_cur_downinfo(void);

#ifdef CONFIG_SDL_BUILD
u32 get_downmap_info(struct down_info * info,u8 cmd);
#endif

uint uart_identify(void);
void uart_preboot(uint port);
void uart_boot(uint port);
void complete_info(uint info, uint port);

#endif

