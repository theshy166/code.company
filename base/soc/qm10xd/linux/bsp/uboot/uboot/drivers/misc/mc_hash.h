#ifndef _HASH_H__
#define _HASH_H__


#if 0

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define ROUNDUP(x, y)		((x + ((y)-1)) & ~((y)-1))
//#define GENMASK(h, l)	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
//#define __ALIGN_MASK(x,mask)        (((x)+(mask))&~(mask))
//#define ALIGN(x, a)                 __ALIGN_MASK((x), (a)-1)

#define BIT(nr)			(1UL << (nr))
#define BIT_ULL(nr)		(1ULL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)

//#define ARRAY_SIZE(arr)		(sizeof(arr) / sizeof((arr)[0]))

#define min(x, y)		((x) < (y) ? (x) : (y))

#define max(x, y)		((x) > (y) ? (x) : (y))
#define NULL			0/* see <stddef.h> */
#define swab16(x)		\
	((((x) & 0x00ff) << 8) | \
	(((x) & 0xff00) >> 8))

#define swab32(x)		\
	((((x) & 0x000000ff) << 24) | \
	(((x) & 0x0000ff00) <<  8) | \
	(((x) & 0x00ff0000) >>  8) | \
	(((x) & 0xff000000) >> 24))







#define BITS_PER_LONG		32
#define FALSE			(0)
#define TRUE			(1)
#define PNULL			((void *)0)

#define false			(0)
#define true			(1)

typedef unsigned long long	u64;
typedef unsigned int		u32;
typedef int			s32;
typedef unsigned short		u16;
typedef short			s16;
typedef unsigned char		u8;
//typedef char			s8;
typedef unsigned int		bool;

#endif






#define HASH_CFG			0
#define RX_FIFO_CFG0			4
#define RX_FIFO_CFG1			8
#define RX_FIFO_CFG2			0xc
#define REQ_CFG1			0x10
#define MEM_INFO_CFG0			0x14
#define MEM_INFO_CFG1			0x18
#define MEM_INFO_CFG2			0x1c
#define HMAC_KEY_INFO_CFG0		0x20
#define HMAC_KEY_INFO_CFG1		0x24
#define HMAC_KEY_INFO_CFG2		0x28
#define DEBUG_BUS			0x2C
#define HASH_H0				0x30
#define HASH(x)				(HASH_H0 + ((x) << 2))
#define HASH_MSG_LEN_CFG0		0x70
#define HASH_MSG_LEN_CFG1		0x74
#define HASH_MSG_LEN_CFG2		0x78
#define HASH_MSG_LEN_CFG3		0x7c
#define HKEY_LEN_CFG			0x80
#define HKEY_CFG0			0x84
#define HEKY_CFG(x)			(HKEY_CFG0 + ((x) << 2))
#define HASH_FIFO			0x100
#define HASH_STATUS			0xC8
#define HASH_INT			0xC4
#define HASH_INT_CLR			BIT(2)

#define HASH_MODE_OFS			0
#define HASH_MODE_MASK			0x7
#define HASH_SHA1			0
#define HASH_SHA256			1
#define HASH_SHA384			2
#define HASH_SHA512			3
#define HASH_START			BIT(3)
#define HMAC_START			BIT(4)
#define DBG_BUS_SEL_OFS			5
#define DBG_BUS_SEL_MASK		0x1f70
#define INT_STATUS			BIT(4)

#define REG_SYS_TIME_BASE				 (0x18A00000)
#define REG_SYS_TIME_VALUE				 (REG_SYS_TIME_BASE + 0xc)
#define HASH_WAIT_COMPLETE_TIME_OUT	5000

#define HASH_DMA_CTRL			0x1c
#define HASH_DMA_REG			0x20
#define HASH_DMA_EN			BIT(1)
#define HASH_DMA_RST			BIT(23)
#define HASH_DMA_REQ_BYTE_NUM   (0x80*4)

#define hash_swab32(x) \
	((((x) & 0x000000ff) << 24) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0xff000000) >> 24))

int hash_cfg(unsigned int chn, unsigned int src, unsigned int len);

void read_result(unsigned int *result, unsigned int mode);
void hash_enable(void);
void hash_start(void);
bool hash_done(unsigned int time,unsigned int chn);
void hash_sched_init(u32 chn, u32 len);
void hash_handle_start(u32 src, u32 dst, u32 len);
bool hash_handle_done(u32 chn);

/*in, out, in_len*/
typedef int (*HW_sha)(const unsigned char*, unsigned char*, int);

int mc_hw_sha(const unsigned char *indata, unsigned char *outdata, int datalen);

#endif


