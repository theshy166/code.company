#ifndef _HASH_H__
#define _HASH_H__

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

#define HASH_WAIT_COMPLETE_TIME_OUT	5000

#define hash_writel(v, x)		\
	__raw_writel(v, (unsigned long)(REG_HASH_BASE + x))
#define hash_readl(x)			\
	__raw_readl((unsigned long)(REG_HASH_BASE + x))

int hash_cfg(unsigned int chn, unsigned int src, unsigned int len);

//void read_result(unsigned int *result, unsigned int mode);
extern void (*read_result)(unsigned int *result, unsigned int mode);
void hash_enable(unsigned int clk_sel);
void hash_start(void);
bool hash_done(unsigned int time,unsigned int chn);
void hash_sched_init(u32 chn, u32 len);
void hash_handle_start(u32 src, u32 dst, u32 len);
bool hash_handle_done(u32 chn);


#endif
