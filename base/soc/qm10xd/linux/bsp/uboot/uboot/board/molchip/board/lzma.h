#ifndef _LZMA_H_
#define _LZMA_H_
#define LZMA_BASE		0x1C300300

#define DECODE_CTRL0		(LZMA_BASE + 0xC)
#define LZMA_DECODE_CNT		(LZMA_BASE + 0x28)
#define SRC_START_ADDR		(LZMA_BASE + 0x30)
#define SRC_END_ADDR		(LZMA_BASE + 0x34)
#define SRC_FILE_LENGTH		(LZMA_BASE + 0x38)
#define DES_BASE_ADDR		(LZMA_BASE + 0x40)
#define INT_DONE_MASK		(LZMA_BASE + 0x74)
#define LZMA_DMA_CTRL		(LZMA_BASE + 0x2C)
#define INT_DONE_STATUS		(LZMA_BASE + 0x70)
#define INT_DONE_CLEAR		(LZMA_BASE + 0x78)

#define INT_DONE		BIT(0)
#define LZMA_DONE_CLEAR		BIT(0)

extern void lzma_cfg(unsigned int lzma_src_addr, unsigned int lzma_dst_addr,
		struct boot_header *header);
bool lzma_done(unsigned int timer);
void lzma_handle_start(u32 src, u32 dst, u32 len);
bool lzma_handle_done(unsigned int *size);
void lzma_enable(struct boot_header *header);
void lzma_sch_init(struct boot_header *header);
extern void lzma_start(void);

#define reg_readl(addr)			(*(volatile unsigned int *) (addr))

#define reg_writel(b, addr)		\
	((*(volatile unsigned int *) (addr)) = (b))

#endif
