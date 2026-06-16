#ifndef __MC_JPEGD_DRV_H__
#define __MC_JPEGD_DRV_H__

#define JPEGD_BASE						0x0B310000

#define JPEG_CKG_EN						0x2C//0x30 // BIT23
#define JPEG_SOFT_RESET					0x30//x34 // BIT4
//#define JPEG_CLEAR					0x34 // BIT6

#define JPEG_VER_INFO					0x0000
#define JPEG_STM_INFO					0x0030

#define JPEG_INT_STATUS					0x0004
#define JPEG_INT_EN						0x0008

#define JPEG_PROC_STATUS				0x000C
#define JPEG_PROC_STATUS1				0x0048
#define JPEG_STM_ADDR0					0x0034
#define JPEG_STM_ADDR1					0x0038
#define JPEG_BUFF0_SIZE					0x0050
#define JPEG_BUFF1_SIZE					0x0054
#define JPEG_MB_RATE_CTRL				0x004C
#define JPEG_QMEM						0x0080
#define JPEG_HUFFENC					0x0100
#define JPEG_DHTMEM						0x0400

#define JPEG_REG0						0x0010
#define JPEG_REG1						0x0014
#define JPEG_REG2						0x0018
#define JPEG_REG3						0x001C
#define JPEG_REG4						0x0020
#define JPEG_REG5						0x0024
#define JPEG_REG6						0x0028
#define JPEG_REG7						0x002C
#define JPEG_REG8						0x003C
#define JPEG_REG9						0x0040
#define JPEG_REG10						0x0044
#define JPEG_REG11						0x0058
#define JPEG_REG12						0x005C
#define JPEG_REG13						0x0060
#define JPEG_REG14						0x0064

#define JPEGD_DONE_STATE				0x1

void hw_jpeg_dec(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr);
void sw_jpeg_dec(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr);
unsigned int JPEG_WaitIRQ(void);

#endif
