#ifndef __WATCHDOG_H_
#define __WATCHDOG_H_

#define WDT_LOAD_OFFSET			0x0
#define WDT_CTRL_OFFSET			0x4
#define WDT_CLEAR_OFFSET		0x8
#define WDT_INTRAW_OFFSET		0xC
#define WDT_INT_OFFSET			0x10
#define WDT_VAL_OFFSET			0x14
#define WDT_LOCK_OFFSET			0x18
#define WDT_READ_OFFSET			0x1C
#define WDT_IRQVAL_OFFSET		0x20

#define BIT_WDT_IRQ_EN			(0x1 << 0)
#define BIT_WDT_RUN				(0x1 << 1)
#define BIT_WDT_NEW				(0x1 << 2)
#define BIT_WDT_RST_EN			(0x1 << 3)

#define BIT_WDT_UNLOCK			0x1ACCE551

#define wdt_read(b, x)		\
	__raw_readl((unsigned long)((b) + (x)))
#define wdt_write(v, b, x)		\
	__raw_writel((v),  (unsigned long)((b) + (x)))

void watchdog_init(void);


#endif
