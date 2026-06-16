//#include <stdlib.h>
#include <config.h>
//#include <mach/hardware.h>
#include "type_use.h"
#include "mci.h"
#include "glb_reg.h"
#include "system.h"
#include "efuse.h"
#include "efuse_drv.h"
#include "uart_drv.h"

u32 core1_path = 0;

#define HEAP_TOTAL_SIZE		(0x200000)
extern void memset(void *, u32 , u32);
typedef struct malloc_header {
	u32 base;
	u16 size;
	u16 free;
} malloc_h_t;

static u8 *heap_region = NULL;
static u32 heap_top = 0;
static u32 heap_resize = HEAP_TOTAL_SIZE;


/* byte. */

void *smalloc(u32 size)
{
	void *mp = NULL;
	malloc_h_t *header = NULL;

	u32 real_size, align_size;
	if (size % 4) {
		align_size = size + (4 - size % 4);
	} else {
		align_size = size;
	}

	if (align_size <= heap_resize) {
		header = (malloc_h_t *)heap_top;
		header->base = heap_top + sizeof(malloc_h_t);
		header->size = size;
		header->free = 1;

		mp = (void *)(heap_top + sizeof(malloc_h_t));

		real_size = align_size + sizeof(malloc_h_t);
		heap_top += real_size;
		heap_resize -= real_size;
	}


	return mp;
}

void sfree(void *addr)
{
	u32 align_size, size = 0;
	malloc_h_t *header = NULL;

	if (addr != NULL) {
		u32 hbase, mbase = (u32)addr;

		hbase = mbase -  sizeof(malloc_h_t);

		if (hbase >= (u32)heap_region) {
			header = (malloc_h_t *)hbase;
			size = header->size;
			if (size % 4) {
				align_size = size + (4 - size % 4);
			} else {
				align_size = size;
			}
			if (mbase == header->base) {
				header->free = 0;
				if (mbase + align_size == heap_top) {
					heap_top = hbase;
					heap_resize += align_size;
					heap_resize += sizeof(malloc_h_t);
				}
			}
		}
	}
}

void heap_init(void)
{

	heap_region = (u8 *)(HEAP_BASE);
	heap_top = (u32)heap_region;
	heap_resize = HEAP_TOTAL_SIZE;
}

#if 0
void heap_reinit(void)
{
	__raw_writel(HEAP_INIT_BASE + 0x14, HEAP_INIT_BASE + 4);
	__raw_writel(HEAP_INIT_BASE, HEAP_INIT_BASE + 8);
	__raw_writel(HEAP_INIT_LIMIT - HEAP_INIT_BASE - 0X14, HEAP_INIT_BASE + 0x14);
}
#endif

void sys_timer_enable(void)
{
	u32 val;

	val = __raw_readl(REG_CPLL_CTRL0);
	val |= CKG_SYST_EN;
	__raw_writel(val, REG_CPLL_CTRL0);

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val |= CKG_APB_SYST_EN;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);
}

void timer_enable(void)
{
	u32 val;
	volatile unsigned i = 0;

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val |= CKG_APB_TIMER_EN ;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);
	for(i =0;i<0x10000;i++);

	__raw_writel(0xFFFFFFFF, REG_TIMER_BASE + 0x40);
	__raw_writel(0x3, REG_TIMER_BASE + 0x50);
}

void mc_udelay(u32 us)
{
	u32 delta, cur, past = __raw_readl(REG_TIMER_BASE + 0x58);
	do {
		cur = __raw_readl(REG_TIMER_BASE + 0x58);
		if (cur <= past) {
			delta = (past - cur) / TIMER_US_CNT;
		} else {
			delta = (0xFFFFFFFFU - cur) / TIMER_US_CNT;
			delta += past / TIMER_US_CNT;
		}
	} while (delta < us);
}
//unsigned int *timestamp_base = (unsigned int *)(0x40008000);

#if 0
void timestamp_record(unsigned int seq)
{
	*timestamp_base++ = seq;
	*timestamp_base++ = __raw_readl(REG_APB_TIMR0_BASE + 0x58);
}
#endif
u32 system_boot_mode(void)
{
	u32 val = 0;

	val = get_strap_status();
	val >>= 1;

	return val;
}

#if 0
int sfc_nor_online(void)
{
	int ret = 0;
	unsigned int boot_mode = system_boot_mode();
	switch (boot_mode) {
		case NOR_FLASH_BOOT:
		default:
			ret = 1;
			break;
		case NAND_FLASH_BOOT:
		case EMMC_BOOT:
		case SDIO0_BOOT:
		case SDIO1_BOOT:
		case UART0_DOWNLOAD:
		case USB_DOWNLOAD:
			break;
	}
	return ret;
}

void smp_boot_secondary(unsigned int path)
{
	unsigned int val,i;
	//volatile unsigned int i;
	u32 *sa = (u32 *)CORE1_STACK_TOP;
	core1_path = path;

	__raw_writel(0x1, CPU_CORE1_CTRL);
	for (i = 0; i < CORE1_STACK_SIZE; i += 4)
		*sa-- = 0;
	__raw_writel((u32)secondary_startup_arm, CPU_CORE1_START_ADDR);
	val = __raw_readl(REG_CPU_SOFT_RST);
	val &= ~CPU_CORE1_SOFT_RST;
	__raw_writel(val, REG_CPU_SOFT_RST);
	mc_udelay(1000);
	//for(i=0;i<0x1000;i++);
	//for(i=0;i<0x1000;i++);
	__asm("sev");
	//for(i=0;i<0x1000;i++);
	//for(i=0;i<0x1000;i++);
	mc_udelay(1000);
}

void smp_reset_core1(void)
{
	unsigned int val;

	val = __raw_readl(REG_CPU_SOFT_RST);
	val |= CPU_CORE1_SOFT_RST;
	__raw_writel(val, REG_CPU_SOFT_RST);
}

void secondary_startup(void)
{
	if (core1_path) {
		spi_sw_fastboot();
	} else {
		//emmc_read_image();
	}
	while(1);
}
#endif
