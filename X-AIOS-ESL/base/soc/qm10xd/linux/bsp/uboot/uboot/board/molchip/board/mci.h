#ifndef __MCI_H_
#define __MCI_H_

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define ROUNDUP(x, y)		((x + ((y)-1)) & ~((y)-1))
#define GENMASK(h, l)		\
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
#define __ALIGN_MASK(x,mask)        (((x)+(mask))&~(mask))
#define ALIGN(x, a)                 __ALIGN_MASK((x), (a)-1)

#define BIT(nr)			(1UL << (nr))
#define BIT_ULL(nr)		(1ULL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)

#define ARRAY_SIZE(arr)		(sizeof(arr) / sizeof((arr)[0]))

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

#define __raw_readb(addr)	\
	(*(volatile unsigned char *) (addr))

#define __raw_writeb(b, addr)	\
	((*(volatile unsigned char *) (addr)) = (unsigned int)(b))

#define __raw_readl(addr)	\
	(*(volatile unsigned int *) (addr))

#define __raw_writel(b, addr)	\
	((*(volatile unsigned int *) (addr)) = (unsigned int)(b))

#endif
