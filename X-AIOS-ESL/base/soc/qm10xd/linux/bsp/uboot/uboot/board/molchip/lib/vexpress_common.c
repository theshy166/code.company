/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2003
 * Texas Instruments, <www.ti.com>
 * Kshitij Gupta <Kshitij@ti.com>
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/systimer.h>
#include <asm/arch/sysctrl.h>
#include <asm/arch/wdt.h>
#include <asm/arch/hardware.h>
#include <mach/spic.h>
#include <mach/dma.h>

#define FY01_CHIP_ID 0x00000194

static struct systimer *systimer_base = (struct systimer *)V2M_TIMER01;
#if 0
static struct sysctrl *sysctrl_base = (struct sysctrl *)SCTL_BASE;
#endif

static void vexpress_timer_init(void);
DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
	printf("Boot reached stage %d\n", progress);
}
#endif

static inline void delay(ulong loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b" : "=r" (loops) : "0" (loops));
}
extern int fh_serial_init(void);
extern int mc_get_cpu_clk(void);
int board_init(void)
{
	gd->bd->bi_boot_params = LINUX_BOOT_PARAM_ADDR;
	gd->bd->bi_arch_number = MACH_TYPE_VEXPRESS;

	icache_enable();
	vexpress_timer_init();

	//board_spic_init();

	//dma_init(DMA_CHN31);

	gd->cpu_clk = mc_get_cpu_clk();

	//fh_serial_init();

	return 0;
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_SMC911X_BASE);
#endif
	return rc;
}

static phys_size_t ram_size_detect(phys_size_t p_start)
{
#if defined(CONFIG_ADAPTIVE_MEMORY_SUPPORT)
	unsigned long i;
	writel(p_start, PHYS_SDRAM_1);
#ifdef CONFIG_MOLCHIP_FASTBOOT
	flush_dcache_range(p_start, p_start + ((sizeof(PHYS_SDRAM_1)/64)+1)*64);
#endif
	for(i = (PHYS_SDRAM_1 + PHYS_64MB_SIZE) ; i < (~0UL) ; i += PHYS_64MB_SIZE) {
		writel(i, i);
#ifdef CONFIG_MOLCHIP_FASTBOOT
		flush_dcache_range(i, i + ((sizeof(i)/64)+1)*64);
#endif
		if (readl(p_start) == i)
			break;
	}
	return i-PHYS_SDRAM_1;
#else
	return PHYS_SDRAM_1_SIZE;
#endif
}

int dram_init(void)
{
#if 0 //be careful,should change later
	gd->ram_size =
		get_ram_size((long *)CONFIG_SYS_SDRAM_BASE, PHYS_SDRAM_1_SIZE);
#else
	//gd->ram_size = ram_size_detect(PHYS_SDRAM_1);
	gd->ram_size = CONFIG_BOOT_RAM_SIZE;
#endif
	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size =
			get_ram_size((long *)PHYS_SDRAM_1, ram_size_detect(PHYS_SDRAM_1));
	#if 0
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size =
			get_ram_size((long *)PHYS_SDRAM_2, PHYS_SDRAM_2_SIZE);
	#endif
	return 0;
}

/*
 * Start timer:
 *    Setup a 32 bit timer, running at 1KHz
 *    Versatile Express Motherboard provides 1 MHz timer
 */
static void vexpress_timer_init(void)
{
	/*
	 * Enable Timer0
	 */
	writel(readl(REG_CPLL_CTRL0) | CKG_XTL_TMR_EN | CKG_RTC_TMR_EN,
		REG_CPLL_CTRL0);
	writel(readl(REG_CEN_PERI_APB_CLK_CTRL) | CKG_APB_TIMER_EN,
		REG_CEN_PERI_APB_CLK_CTRL);

	/*
	 * Set System time to be:
	 *   Enabled, free running, no interrupt, 32-bit, wrapping
	 */
	writel(SYSTIMER_RELOAD, &systimer_base->timer0load);
	writel(SYSTIMER_RELOAD, &systimer_base->timer0value);
	writel(SYSTIMER_EN | SYSTIMER_32BIT |
	       readl(&systimer_base->timer0control),
	       &systimer_base->timer0control);
}

int v2m_cfg_write(u32 devfn, u32 data)
{
#if 0
	/* Configuration interface broken? */
	u32 val;

	devfn |= SYS_CFG_START | SYS_CFG_WRITE;

	val = readl(V2M_SYS_CFGSTAT);
	writel(val & ~SYS_CFG_COMPLETE, V2M_SYS_CFGSTAT);

	writel(data, V2M_SYS_CFGDATA);
	writel(devfn, V2M_SYS_CFGCTRL);

	do {
		val = readl(V2M_SYS_CFGSTAT);
	} while (val == 0);

	return !!(val & SYS_CFG_ERR);
#else
    return 0;
#endif
}

/* Use the ARM Watchdog System to cause reset */
void reset_cpu(ulong addr)
{
#if 0
	if (v2m_cfg_write(SYS_CFG_REBOOT | SYS_CFG_SITE_MB, 0))
		printf("Unable to reboot\n");
#endif
}

void lowlevel_init(void)
{
	;
}

ulong get_board_rev(void){
#if 0
	return readl((u32 *)SYS_ID);
#else
	return 0;
#endif
}

#ifdef CONFIG_ARMV7_NONSEC
/* Setting the address at which secondary cores start from.
 * Versatile Express uses one address for all cores, so ignore corenr
 */
void smp_set_core_boot_addr(unsigned long addr, int corenr)
{
	/* The SYSFLAGS register on VExpress needs to be cleared first
	 * by writing to the next address, since any writes to the address
	 * at offset 0 will only be ORed in
	 */
	writel(~0, CONFIG_SYSFLAGS_ADDR + 4);
	writel(addr, CONFIG_SYSFLAGS_ADDR);
}
#endif

static void arch_timer_init(void)
{
#if defined(CONFIG_TARGET_FY00)
	/* start arch timer*/
	writel(readl(REG_CPU_SYS_APB + 0x10) | (0x1 << 2),
		REG_CPU_SYS_APB + 0x10);
#elif defined(CONFIG_TARGET_FY10D)
	/* start arch timer*/
	writel(readl(REG_CPU_SYS_APB + 0x10) | (0x1 << 2),
		REG_CPU_SYS_APB + 0x10);
#elif defined(CONFIG_TARGET_FY10DS)
	/* start arch timer*/
	writel(readl(REG_CPU_SYS_APB + 0x10) | (0x1 << 2),
		REG_CPU_SYS_APB + 0x10);
#elif defined(CONFIG_TARGET_FY01)
	/* start arch timer*/
	writel(readl(REG_AP_AHB_BASE + 0x40) | (0x1 << 2),
		REG_AP_AHB_BASE + 0x40);
#endif
}

static void sys_timer_init(void)
{
	writel(readl(REG_CEN_PERI_APB_CLK_CTRL) | CKG_APB_SYST_EN,
		REG_CEN_PERI_APB_CLK_CTRL);
	writel(readl(REG_CPLL_CTRL0) | (0x1 << 13),
                REG_CPLL_CTRL0);
}

static void uart_init(void)
{
	//writel(0x1,0x18000014);
	writel(readl(REG_CEN_PERI_CLK_CTRL) | (3 << 9),
		REG_CEN_PERI_CLK_CTRL);
	writel(readl(REG_CEN_PERI_APB_CLK_CTRL) | (3 << 12),
		REG_CEN_PERI_APB_CLK_CTRL);
}

static void setup_chip_id(void)
{
	//writel(readl(FY01_CHIP_ID), 0x18000010);
}

int board_early_init_f(void)
{
	setup_chip_id();
	arch_timer_init();
	sys_timer_init();
	uart_init();

	return 0;
}

