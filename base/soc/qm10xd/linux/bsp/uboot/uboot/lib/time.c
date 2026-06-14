/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <timer.h>
#include <watchdog.h>
#include <div64.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>

/*APB TIMER REG DEFINE*/
#define TIMER0_CLK_FREQ			(32768)
#define TIMER0_MS_CNT			TIMER0_CLK_FREQ / 1000
#define TIMER1_CLK_FREQ			(32768)
#define TIMER1_MS_CNT			TIMER1_CLK_FREQ / 1000
#define TIMER2_CLK_FREQ			(24000000)
#define TIMER2_US_CNT			(TIMER2_CLK_FREQ / 1000000)

#define TMR0_LOAD_LO			(0x0000)
#define TMR0_LOAD_HI			(0x0004)
#define TMR0_VALUE_LO			(0x0008)
#define TMR0_VALUE_HI			(0x000c)
#define TMR0_CTL			(0x0010)
#define TMR0_INT			(0x0014)
#define TMR0_VALUE_SHDW_LO		(0x0018)
#define TMR0_VALUE_SHDW_HI		(0x001c)
#define TMR1_LOAD_LO			(0x0020)
#define TMR1_LOAD_HI			(0x0024)
#define TMR1_VALUE_LO			(0x0028)
#define TMR1_VALUE_HI			(0x002c)
#define TMR1_CTL			(0x0030)
#define TMR1_INT			(0x0034)
#define TMR1_VALUE_SHDW_LO		(0x0038)
#define TMR1_VALUE_SHDW_HI		(0x003c)
#define TMR2_LOAD_LO			(0x0040)
#define TMR2_LOAD_HI			(0x0044)
#define TMR2_VALUE_LO			(0x0048)
#define TMR2_VALUE_HI			(0x004c)
#define TMR2_CTL			(0x0050)
#define TMR2_INT			(0x0054)
#define TMR2_VALUE_SHDW_LO		(0x0058)
#define TMR2_VALUE_SHDW_HI		(0x005c)

/*1: 64 bit,  0:32 bit*/
#define TMRn_CTRL_TMRn_WIDTH_SEL		(BIT(16))
/*1: Enabled,  0: Disabled*/
#define TMRn_CTRL_TMRn_RUN			(BIT(1))
/*1:Period mode  0: one-time mode*/
#define TMRn_CTRL_TMRn_MODE			(BIT(0))

/*irq interrupt clear, 1: clear, write only*/
#define TMRn_INT_TMRn_IRQ_CLR			(BIT(3))
/*irq interrupt masked status, read only*/
#define TMRn_INT_TMRn_IRQ_MASK			(BIT(2))
/*irq interrupt raw, read only*/
#define TMRn_INT_TMRn_IRQ_RAW			(BIT(1))
/*irq interrupt enabled, r/w*/
#define TMRn_INT_TMRn_IRQ_EN			(BIT(0))

#ifndef CONFIG_WD_PERIOD
# define CONFIG_WD_PERIOD	(10 * 1000 * 1000)	/* 10 seconds default */
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SYS_TIMER_RATE
/* Returns tick rate in ticks per second */
ulong notrace get_tbclk(void)
{
	return CONFIG_SYS_TIMER_RATE;
}
#endif

#ifdef CONFIG_SYS_TIMER_COUNTER
unsigned long notrace timer_read_counter(void)
{
#ifdef CONFIG_SYS_TIMER_COUNTS_DOWN
	return ~readl(CONFIG_SYS_TIMER_COUNTER);
#else
	return readl(CONFIG_SYS_TIMER_COUNTER);
#endif
}

ulong timer_get_boot_us(void)
{
	ulong count = timer_read_counter();

#if CONFIG_SYS_TIMER_RATE == 1000000
	return count;
#elif CONFIG_SYS_TIMER_RATE > 1000000
	return lldiv(count, CONFIG_SYS_TIMER_RATE / 1000000);
#elif defined(CONFIG_SYS_TIMER_RATE)
	return (unsigned long long)count * 1000000 / CONFIG_SYS_TIMER_RATE;
#else
	/* Assume the counter is in microseconds */
	return count;
#endif
}

#else
extern unsigned long __weak timer_read_counter(void);
#endif

#ifdef CONFIG_TIMER
ulong notrace get_tbclk(void)
{
	if (!gd->timer) {
#ifdef CONFIG_TIMER_EARLY
		return timer_early_get_rate();
#else
		int ret;

		ret = dm_timer_init();
		if (ret)
			return ret;
#endif
	}

	return timer_get_rate(gd->timer);
}

uint64_t notrace get_ticks(void)
{
	u64 count;
	int ret;

	if (!gd->timer) {
#ifdef CONFIG_TIMER_EARLY
		return timer_early_get_count();
#else
		int ret;

		ret = dm_timer_init();
		if (ret)
			return ret;
#endif
	}

	ret = timer_get_count(gd->timer, &count);
	if (ret)
		return ret;

	return count;
}

#else /* !CONFIG_TIMER */

uint64_t __weak notrace get_ticks(void)
{
	unsigned long now = timer_read_counter();

	/* increment tbu if tbl has rolled over */
	if (now < gd->timebase_l)
		gd->timebase_h++;
	gd->timebase_l = now;
	return ((uint64_t)gd->timebase_h << 32) | gd->timebase_l;
}

#endif /* CONFIG_TIMER */

/* Returns time in milliseconds */
static uint64_t notrace tick_to_time(uint64_t tick)
{
	ulong div = get_tbclk();

	tick *= CONFIG_SYS_HZ;
	do_div(tick, div);
	return tick;
}

int __weak timer_init(void)
{
	return 0;
}

/* Returns time in milliseconds */
ulong __weak get_timer(ulong base)
{
	return tick_to_time(get_ticks()) - base;
}

unsigned long __weak notrace timer_get_us(void)
{
	return tick_to_time(get_ticks() * 1000);
}

#if 0
static uint64_t usec_to_tick(unsigned long usec)
{
    uint64_t tick = usec;
	tick *= get_tbclk();
	do_div(tick, 1000000);
	return tick;
};
#endif

void __weak __udelay(unsigned long usec)
{
#if 0
	uint64_t tmp;

	tmp = get_ticks() + usec_to_tick(usec);	/* get current timestamp */

	while (get_ticks() < tmp+1)	/* loop till event */
		 /*NOP*/;
#endif
}

/* ------------------------------------------------------------------------- */
void udelay(unsigned long usec)
{
#if  defined(CONFIG_TARGET_FY01)
		*((u32 volatile *)(REG_TIMER0_BASE + 0x30)) = 0;
		*((u32 volatile *)(REG_TIMER0_BASE + 0x28)) = usec * 24;
		*((u32 volatile *)(REG_TIMER0_BASE + 0x30)) = 3;
		while(*((u32 volatile *)(REG_TIMER0_BASE + 0x38)) == 0);
		*((u32 volatile *)(REG_TIMER0_BASE + 0x30)) = 0;
#else
		if (usec >= (0xFFFFFFFF / TIMER2_US_CNT)) {
			printf("udelay param %ld is too big\r\n", usec);
		} else {
			*((u32 volatile *)(REG_CPLL_CTRL0)) |= CKG_RTC_TMR_EN;
			*((u32 volatile *)(REG_CPLL_CTRL0)) |= CKG_XTL_TMR_EN;
			*((u32 volatile *)(REG_CEN_PERI_APB_CLK_CTRL)) |=
						CKG_APB_TIMER_EN;
			*((u32 volatile *)(REG_AP_PERI_SOFT_RST0)) |=
						TIMER0_RTC_SOFT_RST;
			*((u32 volatile *)(REG_AP_PERI_SOFT_RST0)) &=
						~TIMER0_RTC_SOFT_RST;
			*((u32 volatile *)(REG_AP_PERI_SOFT_RST0)) |=
						TIMER0_XTL_SOFT_RST;
			*((u32 volatile *)(REG_AP_PERI_SOFT_RST0)) &=
						~TIMER0_XTL_SOFT_RST;

			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_INT)) |=
						TMRn_INT_TMRn_IRQ_EN;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_CTL)) = 0;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_LOAD_LO)) =
						usec * TIMER2_US_CNT;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_LOAD_HI)) = 0;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_CTL)) |=
						TMRn_CTRL_TMRn_MODE;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_CTL)) |=
						TMRn_CTRL_TMRn_WIDTH_SEL;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_CTL)) |=
						TMRn_CTRL_TMRn_RUN;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_INT)) &=
						~TMRn_INT_TMRn_IRQ_EN;

			while((*((u32 volatile *)(REG_TIMER0_BASE + TMR2_INT)) &
						TMRn_INT_TMRn_IRQ_RAW) == 0);

			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_CTL)) = 0;
			*((u32 volatile *)(REG_TIMER0_BASE + TMR2_INT)) |=
						TMRn_INT_TMRn_IRQ_CLR;
			*((u32 volatile *)(REG_CPLL_CTRL0)) &= ~CKG_RTC_TMR_EN;
			*((u32 volatile *)(REG_CPLL_CTRL0)) &= ~CKG_XTL_TMR_EN;
			*((u32 volatile *)(REG_CEN_PERI_APB_CLK_CTRL)) &=
						~CKG_APB_TIMER_EN;
		}

#endif

}
