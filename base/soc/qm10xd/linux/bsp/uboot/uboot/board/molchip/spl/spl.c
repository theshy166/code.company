/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/clk.h>
#include <mach/clock.h>
#include <mach/spl.h>
#include <mach/uart_drv.h>
#include <mach/spic.h>
#include <mach/board.h>
#include <mach/norflash.h>
#include <mach/nandflash.h>
#include <mach/dma.h>
#include <mach/ddr.h>
#include <mach/sd_card.h>
#include "../board/efuse_drv.h"
#include "../board/system.h"
#include "../board/efuse.h"
#include "../board/system.h"
#include <version.h>

#ifdef CONFIG_SPL_ARGS_TRSF
#include "../board/bootargs.h"
#endif



#ifdef CONFIG_SPL_MMC_SUPPORT
#include <mach/sd_mmc.h>
#endif

#ifndef CONFIG_SPL_DM
/* Pointer to as well as the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;

/*
 * WARNING: This is going away very soon. Don't use it and don't submit
 * pafches that rely on it. The global_data area is set up in crt0.S.
 */
gd_t gdata __attribute__ ((section(".data")));
#endif


/**
 * spl_relocate_stack_gd() - Relocate stack ready for board_init_r() execution
 *
 * Sometimes board_init_f() runs with a stack in SRAM but we want to use SDRAM
 * for the main board_init_r() execution. This is typically because we need
 * more stack space for things like the MMC sub-system.
 *
 * This function calculates the stack position, copies the global_data into
 * place, sets the new gd (except for ARM, for which setting GD within a C
 * function may not always work) and returns the new stack position. The
 * caller is responsible for setting up the sp register and, in the case
 * of ARM, setting up gd.
 *
 * All of this is done using the same layout and alignments as done in
 * board_init_f_init_reserve() / board_init_f_alloc_reserve().
 *
 * @return new stack location, or 0 to use the same stack
 */
ulong spl_relocate_stack_gd(void)
{
	return 0;

}

void mc_bringup_arc0(unsigned int addr)
{
	unsigned int val;
	/*arc to rt-thread*/
	*(volatile unsigned int *)0x10000000 = 0x0f80200a;
	*(volatile unsigned int *)0x10000004 = addr >> 16;
	*(volatile unsigned int *)0x10000008 = 0x00002020;
	*(volatile unsigned int *)0x1000000c = 0x00000000;

	/*release arc600*/
	val = *(volatile unsigned int *)ARC600_0_CTRL;
	val |= ARC_START;
	*(volatile unsigned int *)ARC600_0_CTRL = val;

	val = *(volatile unsigned int *)ARC600_0_CTRL;
	val |= ARC_SOFT_RST;
	*(volatile unsigned int *)ARC600_0_CTRL = val;

	val = *(volatile unsigned int *)ARC600_0_CTRL;
	val &= ~ARC_SOFT_RST;
	*(volatile unsigned int *)ARC600_0_CTRL = val;

}

void mc_arc0_release(void)
{
	unsigned int val;
	/*arc to rt-thread*/
	*(volatile unsigned int *)0x10000000 = 0x0f80200a;
	*(volatile unsigned int *)0x10000008 = 0x00002020;
	*(volatile unsigned int *)0x1000000c = 0x00000000;

	/*release arc600*/
	val = *(volatile unsigned int *)ARC600_0_CTRL;
	val |= ARC_START;
	*(volatile unsigned int *)ARC600_0_CTRL = val;

	val = *(volatile unsigned int *)ARC600_0_CTRL;
	val |= ARC_SOFT_RST;
	*(volatile unsigned int *)ARC600_0_CTRL = val;

	val = *(volatile unsigned int *)ARC600_0_CTRL;
	val &= ~ARC_SOFT_RST;
	*(volatile unsigned int *)ARC600_0_CTRL = val;

}


void mc_bringup_arc1(unsigned int addr)
{
	unsigned int val;
	/*arc to rt-thread*/

	*(volatile unsigned int *)0x10000040 = 0x0f80200a;
	*(volatile unsigned int *)0x10000044 = addr >> 16;
	*(volatile unsigned int *)0x10000048 = 0x00002020;
	*(volatile unsigned int *)0x1000004c = 0x00000000;

	/*release arc600*/
	//ARC1
	val = *(volatile unsigned int *)ARC600_1_CTRL;
	val |= ARC_START;
	*(volatile unsigned int *)ARC600_1_CTRL = val;

	val = *(volatile unsigned int *)ARC600_1_CTRL;
	val |= ARC_SOFT_RST;
	*(volatile unsigned int *)ARC600_1_CTRL = val;

	val = *(volatile unsigned int *)ARC600_1_CTRL;
	val &= ~ARC_SOFT_RST;
	*(volatile unsigned int *)ARC600_1_CTRL = val;
}
int spl_parse_image_header(struct spl_image_info *spl_image,
                           const struct image_header *header)
{
        return 0;
}

void arch_timer(void)
{
	unsigned int val;
   //arm_arch_timer
   val = readl(0x0C000008);
   val |= 0x8;
   writel(val,0x0C000008);
}

#ifdef CONFIG_PRG_BOOT_STAGE_PLUSIN
static int system_is_wakeup(void)
{
	int ret = 0;
	if (BOOT_FLAG_WAKE_UP == __raw_readl(BOOT_FLAG_CP2AP_BASE)) {
		ret = 1;
	}
	return ret;
}
#endif

static void system_os_ready(void)
{
	__raw_writel(FLAG_OS_READY, BOOT_FLAG_AP2CP_BASE);
}

static void system_mem_ready(void)
{
	__raw_writel(FLAG_MAIN_MEM_READY, BOOT_FLAG_AP2CP_BASE);
}

int set_rtt0_boot_addr(unsigned int addr)
{
       if (addr) {
               *(unsigned int *)0x1000052c = addr;
               return true;
       }
       uart_puts("invalid address!\r\n");
       return false;
}


void s_init(void)
{
        //timer clk init
        molchip_systimer_clk_enable();

        //uart0 clk init
        molchip_uart_clk_enable(REG_UART0_BASE);

        //uart1 clk init
        molchip_uart_clk_enable(REG_UART1_BASE);

        //spic clk init
        molchip_spic_clk_enable();

		//dma_init(DMA_CHN31);
}
/*    This function prepares the hardware for
 *    execution from system RAM (DRAM, DDR...) As system RAM may not
 *    be available yet, , board_init_f() must use the current GD to
 *    store any data which must be passed on to later stages. These
 *    data include the relocation destination, the future stack, and
 *    the future GD location.
 *
 *    bss data not init
 */

#if defined(MC_DDR_TEST) && defined(CONFIG_SPL_BUILD)

#define UMCTL2_REG_SET(x,v) *(volatile unsigned int*)(unsigned long)(x) = (v)
#define UMCTL2_REG_GET(x)  *(volatile unsigned int*)(unsigned long)(x)

#define	perfmon_0 0x24800000
#define perfmon_1 0x24900000
#define perfmon_4 0x24c00000
#define BASE_ADDR_DMC_APB_REG 0x24000000


void xc_perfmon_cfg(unsigned       long  long perf_base,unsigned is_dynami_read)
{
		unsigned int reg_value;

		reg_value = UMCTL2_REG_GET(perf_base + 0x0);
		reg_value |= ((0x1 << 31) | (0x7 << 20) | (0x1FF) | (0x1 << 10)|(0x1 << 11));
		UMCTL2_REG_SET(perf_base + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x4);
		reg_value |=((0x7 << 5) | (0xF));
		UMCTL2_REG_SET(perf_base + 0x4,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x8);
		reg_value |= (0xF);
		UMCTL2_REG_SET(perf_base + 0x8,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0xc);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0xc,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x10);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0x10,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x14);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0x14,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x18);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0x18,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x1c);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0x1c,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x20);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0x20,reg_value);

		reg_value = UMCTL2_REG_GET(perf_base + 0x24);
		reg_value |= (0x1);
		UMCTL2_REG_SET(perf_base + 0x24,reg_value);
}

struct reg_info_t {
        unsigned int reg_addr;
        unsigned int regv;
        unsigned int mask;
};
unsigned int ddr_bist_cfg[] =
{
	#include "soc_init_reg_list_softeye.h"
};
extern u32 get_sys_ticks(void);
void delay_ddr(unsigned int ms)
{
	unsigned int time_s = get_sys_ticks();

	while( (get_sys_ticks() - time_s) < ms );

	return;
}
static volatile unsigned int clr_timer_cnt;


void ddr_mbist(void)
{
	unsigned int i,reg_val;
	unsigned int len = ddr_bist_cfg[0];
	struct reg_info_t *reginfo = (struct reg_info_t*)(&ddr_bist_cfg[1]);

	for(i=0;i<len;i++) {
		if ((reginfo[i].reg_addr & 0x3) == 0x0) {
			reg_val = *(volatile unsigned int*)((unsigned long)reginfo[i].reg_addr);
			reg_val &= ~reginfo[i].mask;
			reg_val |= reginfo[i].regv;
			*(volatile unsigned int*)((unsigned long)reginfo[i].reg_addr) = reg_val;
		} else if ((reginfo[i].reg_addr & 0x3) == 0x1) {
			while( (*(volatile unsigned int*)((unsigned long)(reginfo[i].reg_addr & ~0x3)) & reginfo[i].mask) != reginfo[i].regv ) {}
			mc_printf("%08x;polling finish\r\n", (reginfo[i].reg_addr & ~0x3));
		} else if ((reginfo[i].reg_addr & 0x3) == 0x2) {
			if((*(volatile unsigned int*)((unsigned long)(reginfo[i].reg_addr & ~0x3)) & reginfo[i].mask) != reginfo[i].regv) {
				mc_printf("ddr all address bist fail\r\n");
				while(1);
			} else {
				mc_printf("ddr bist pass\r\n");
			}
		}
	}
	mc_printf("ddr_mbist start\r\n");
	/*
	xc_perfmon_cfg(perfmon_0,0);
	xc_perfmon_cfg(perfmon_1,0);
	xc_perfmon_cfg(perfmon_4,0);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
	reg_value |= (0x3 << 0);
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
	reg_value &= ~(0x3 <<0);
	reg_value |= (0x1 << 0);
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x4);
	reg_value |= 0xFFfF;
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x4,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x8);
	reg_value |= 0x1;
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x8,reg_value);

	while(1) {
		if(clr_timer_cnt == 0) {
			reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
			reg_value |= (0x3 << 0);
			UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

			reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
			reg_value |= (0x1 << 0);
			UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);
			clr_timer_cnt = 1;
		} else {
			clr_timer_cnt = 0;
		}

		reg_value = UMCTL2_REG_GET(perfmon_0 + 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_0 + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perfmon_1+ 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_1 + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perfmon_4 + 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_4 + 0x0,reg_value);
		unsigned int time_s;
		time_s = get_sys_ticks();
		while((get_sys_ticks() - time_s) < 1000);

		mc_printf("CHN0 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24800044,*(volatile unsigned int*)0x2480002C,*(volatile unsigned int*)0x24800048,*(volatile unsigned int*)0x24800038);
		mc_printf("CHN1 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24900044,*(volatile unsigned int*)0x2490002C,*(volatile unsigned int*)0x24900048,*(volatile unsigned int*)0x24900038);
		mc_printf("CHN4 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24c00044,*(volatile unsigned int*)0x24c0002C,*(volatile unsigned int*)0x24c00048,*(volatile unsigned int*)0x24c00038);
	*/
	while(1) {
		delay_ddr(10000);
		if(((*(volatile unsigned int*)(0x250000D0) & 0x2) == 0x0) && ((*(volatile unsigned int*)(0x251000D0) & 0x2) == 0x0)) {
				mc_printf("ddr mbist bwtest pass\r\n");
		} else {
				mc_printf("ddr mbist bwtest fail\r\n");
		}
	}
}
unsigned int ddr_r_mbist_cfg[] =
{
	#include "soc_init_reg_list_aread.h"
};

void ddr_rtest(void)
{
	unsigned int i,reg_val;
	unsigned int len = ddr_r_mbist_cfg[0];
	struct reg_info_t *reginfo = (struct reg_info_t*)(&ddr_r_mbist_cfg[1]);

	for(i=0;i<len;i++) {
		if ((reginfo[i].reg_addr & 0x3) == 0x0) {
			reg_val = *(volatile unsigned int*)((unsigned long)reginfo[i].reg_addr);
			reg_val &= ~reginfo[i].mask;
			reg_val |= reginfo[i].regv;
			*(volatile unsigned int*)((unsigned long)reginfo[i].reg_addr) = reg_val;
		} else if ((reginfo[i].reg_addr & 0x3) == 0x1) {
			while( (*(volatile unsigned int*)((unsigned long)(reginfo[i].reg_addr & ~0x3)) & reginfo[i].mask) != reginfo[i].regv ) {}
			mc_printf("%08x;polling finish\r\n", (reginfo[i].reg_addr & ~0x3));
		} else if ((reginfo[i].reg_addr & 0x3) == 0x2) {
			if((*(volatile unsigned int*)((unsigned long)(reginfo[i].reg_addr & ~0x3)) & reginfo[i].mask) != reginfo[i].regv) {
				mc_printf("ddr bist fail\r\n");
			} else {
				mc_printf("ddr bist pass\r\n");
			}
		}
	}
	mc_printf("ddr_rtest start\r\n");
	/*
	unsigned int reg_value;
	xc_perfmon_cfg(perfmon_0,0);
	xc_perfmon_cfg(perfmon_1,0);
	xc_perfmon_cfg(perfmon_4,0);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
	reg_value |= (0x3 << 0);
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
	reg_value &= ~(0x3 <<0);
	reg_value |= (0x1 << 0);
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x4);
	reg_value |= 0xFFfF;
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x4,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x8);
	reg_value |= 0x1;
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x8,reg_value);

	while(1) {
		if(clr_timer_cnt == 0) {
			reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
			reg_value |= (0x3 << 0);
			UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

			reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
			reg_value |= (0x1 << 0);
			UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);
			clr_timer_cnt = 1;
		} else {
			clr_timer_cnt = 0;
		}

		reg_value = UMCTL2_REG_GET(perfmon_0 + 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_0 + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perfmon_1+ 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_1 + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perfmon_4 + 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_4 + 0x0,reg_value);
		unsigned int time_s;
		time_s = get_sys_ticks();
		while((get_sys_ticks() - time_s) < 1000);

		mc_printf("CHN0 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24800044,*(volatile unsigned int*)0x2480002C,*(volatile unsigned int*)0x24800048,*(volatile unsigned int*)0x24800038);
		mc_printf("CHN1 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24900044,*(volatile unsigned int*)0x2490002C,*(volatile unsigned int*)0x24900048,*(volatile unsigned int*)0x24900038);
		mc_printf("CHN4 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24c00044,*(volatile unsigned int*)0x24c0002C,*(volatile unsigned int*)0x24c00048,*(volatile unsigned int*)0x24c00038);
	}
	*/
}


unsigned int ddr_w_mbist_cfg[] =
{
	#include "soc_init_reg_list_awrite.h"
};

void ddr_wtest(void)
{
	unsigned int i,reg_val;
	unsigned int len = ddr_w_mbist_cfg[0];
	struct reg_info_t *reginfo = (struct reg_info_t*)(&ddr_w_mbist_cfg[1]);

	for(i=0;i<len;i++) {
		if ((reginfo[i].reg_addr & 0x3) == 0x0) {
			reg_val = *(volatile unsigned int*)((unsigned long)reginfo[i].reg_addr);
			reg_val &= ~reginfo[i].mask;
			reg_val |= reginfo[i].regv;
			*(volatile unsigned int*)((unsigned long)reginfo[i].reg_addr) = reg_val;
		} else if ((reginfo[i].reg_addr & 0x3) == 0x1) {
			while( (*(volatile unsigned int*)((unsigned long)(reginfo[i].reg_addr & ~0x3)) & reginfo[i].mask) != reginfo[i].regv ) {}
			mc_printf("%08x;polling finish\r\n", (reginfo[i].reg_addr & ~0x3));
		} else if ((reginfo[i].reg_addr & 0x3) == 0x2) {
			if((*(volatile unsigned int*)((unsigned long)(reginfo[i].reg_addr & ~0x3)) & reginfo[i].mask) != reginfo[i].regv) {
				mc_printf("ddr bist fail\r\n");
			} else {
				mc_printf("ddr bist pass\r\n");
			}
		}
	}

	mc_printf("ddr_wtest start\r\n");
	/*
	unsigned int reg_value;
	xc_perfmon_cfg(perfmon_0,0);
	xc_perfmon_cfg(perfmon_1,0);
	xc_perfmon_cfg(perfmon_4,0);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
	reg_value |= (0x3 << 0);
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
	reg_value &= ~(0x3 <<0);
	reg_value |= (0x1 << 0);
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x4);
	reg_value |= 0xFFfF;
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x4,reg_value);

	reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x8);
	reg_value |= 0x1;
	UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x8,reg_value);

	while(1) {
		if(clr_timer_cnt == 0) {
			reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
			reg_value |= (0x3 << 0);
			UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);

			reg_value = UMCTL2_REG_GET(BASE_ADDR_DMC_APB_REG + 0x0);
			reg_value |= (0x1 << 0);
			UMCTL2_REG_SET(BASE_ADDR_DMC_APB_REG + 0x0,reg_value);
			clr_timer_cnt = 1;
		} else {
			clr_timer_cnt = 0;
		}

		reg_value = UMCTL2_REG_GET(perfmon_0 + 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_0 + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perfmon_1+ 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_1 + 0x0,reg_value);

		reg_value = UMCTL2_REG_GET(perfmon_4 + 0x0);
		reg_value |=(0x1 << 11);
		UMCTL2_REG_SET(perfmon_4 + 0x0,reg_value);
		unsigned int time_s;
		time_s = get_sys_ticks();
		while((get_sys_ticks() - time_s) < 1000);

		mc_printf("CHN0 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24800044,*(volatile unsigned int*)0x2480002C,*(volatile unsigned int*)0x24800048,*(volatile unsigned int*)0x24800038);
		mc_printf("CHN1 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24900044,*(volatile unsigned int*)0x2490002C,*(volatile unsigned int*)0x24900048,*(volatile unsigned int*)0x24900038);
		mc_printf("CHN4 [0x%08x] : [0x%08x] : [0x%08x] : [0x%08x]\r\n",*(volatile unsigned int*)0x24c00044,*(volatile unsigned int*)0x24c0002C,*(volatile unsigned int*)0x24c00048,*(volatile unsigned int*)0x24c00038);
	}
	*/
}


struct test {
    char *name;
    void (*fp)(void);
};

struct test tests[] = {
    { "mbist", ddr_mbist },
    { "rtest", ddr_rtest },
    { "wtest", ddr_wtest },
    { NULL, NULL }
};
#endif
void board_init_f(ulong dummy)
{
//	volatile u32 i=100;
	/* CPU/BUS CLK config if needed*/

	/* Bias config reset */
//	ana_bias_reset();
	/* DDR config if needed */
//	while(i != 0);
	if(BOOT_FLAG_WAKE_UP != __raw_readl(BOOT_FLAG_CP2AP_BASE)) {
		DRAM_Init();
	}

	mc_printf("%s\r\n" , U_BOOT_VERSION_STRING);
	system_mem_ready();

#if defined(MC_DDR_TEST) && defined(CONFIG_SPL_BUILD)
	mc_printf("test item1:mbist\r\n");
	mc_printf("test item2:rtest\r\n");
	mc_printf("test item3:wtest\r\n");

	static u8 autoslt_str[16];
	int cnt = 0;
	unsigned int len = strlen(tests[0].name);

	memset(autoslt_str, 0x0, 16);

	while(1) {

		if(cnt == 15) {
			cnt = 0;
			memset(autoslt_str, 0x0, 16);
		}

		autoslt_str[cnt] = uart_getc(0x08F00000);
		if(cnt == (len - 1)) {
			autoslt_str[cnt+1] = 0;
			for (unsigned int i=0;;i++) {
				if(0 == strcmp((const char*)autoslt_str, tests[i].name)) {
					tests[i].fp();
					continue;
				}
			}
			cnt = 0;
			memset(autoslt_str, 0x0, 16);
			continue;
		}

		cnt++;
	}
#endif
    uart_hw_init();
	board_spic_init();
	board_spi_init();
}



int nor_load_image(struct boot_image_info *info)
{
	int ret = 0;

	if (info) {
		if (HW_CASCADE_FAST_BOOT == info->boot_scheme) {
		} else if (CPU_SCHED_FAST_BOOT == info->boot_scheme) {
			if(info->name == pre_rolling){
#ifdef CONFIG_PRG_BOOT_STAGE_PLUSIN
				if (0 == system_is_wakeup()) {
					ret = sched_nor_load_image(info);
					if (ret == false) {
						return false;
					}
					mc_bringup_arc0(info->rtt0_boot_addr);
				} else {
					//mc_arc0_release();
					/* nothing to do. */
				}
#else
				uart_puts("error:image_info_csv.\n");
#endif
			}

			if(info->name == rtt){
				uart_puts("in rtt test!\n");
				ret = sched_nor_load_image(info);
				if (ret == false) {
					return ret;
				}
				ret = set_rtt0_boot_addr(info->rtt0_boot_addr);
				if (ret == false) {
						return false;
				}
				system_os_ready();
				mc_bringup_arc1(info->rtt1_boot_addr);
				uart_puts("rtt pass!\n");
			}
			if(info->name == uboot){
				ret = sched_nor_load_image(info);
				if (ret == false) {
					return false;
				}
				nosec_jump_to(UBOOT_LOAD_ADDR);
			}
			if(info->name == kernel){
				ret = sched_nor_load_image(info);
				if (ret == false) {
					return false;
				}
				uart_puts("kernel image pass!\n");
				arch_timer();
				spl_bootm();
				hang();
			}
		} else {

		}
	}

	return ret;
}

int nand_load_image(struct boot_image_info *info)
{
	int ret = 0;

	if (info) {
		if (HW_CASCADE_FAST_BOOT == info->boot_scheme) {
		} else if (CPU_SCHED_FAST_BOOT == info->boot_scheme) {

		} else {
			if(info->name == pre_rolling){
#ifdef CONFIG_PRG_BOOT_STAGE_PLUSIN
				if (0 == system_is_wakeup()){
					efuse_init();
					spl_nand_load_image(info);
					mc_bringup_arc0(info->rtt0_boot_addr);
				}else {
					//mc_arc0_release();
					/* nothing to do */
				}
#else
				uart_puts("error:image_info_csv.\n");
#endif
			}

			if(info->name == rtt){
				efuse_init();
				spl_nand_load_image(info);
				ret = set_rtt0_boot_addr(info->rtt0_boot_addr);
				if (ret == false) {
						return ret;
				}
				system_os_ready();
				mc_bringup_arc1(info->rtt1_boot_addr);
			}
			if(info->name == uboot){
				efuse_init();
				spl_nand_load_image(info);
				nosec_jump_to(UBOOT_LOAD_ADDR);
			}
			if(info->name == kernel){
				efuse_init();
				spl_nand_load_image(info);
				arch_timer();
				spl_bootm();
				hang();

			}
		}
	}

	return ret;

}

int nand_read_data(u32 addr , u32 len, u8 *buf )
{
	return 0;
}
#ifdef CONFIG_SPL_MMC_SUPPORT
int emmc_read_data(u32 addr , u32 len, u8 *buf )
{
	int ret;
	u32 dst;
	dst = (u32)buf;
	addr = ALIGN(addr, 512) / 512;
	len = ALIGN(len, 512);
	ret = emmc_blk_mq_issue_rw_rq(READ, dst, addr, len);
	if(ret == false) {
		uart_puts("emmc_blk_mq rq error!\n");
		return false;
	}
	ret = block_cmd_data(dst, len);
	if(ret == false) {
		uart_puts("mmc read error!!!\n");
		return false;
	}
	return ret;
}
int emmc_write_data(u32 addr , u32 len, u8 *buf )
{
	int ret;
	u32 dst;
	dst = (u32)buf;
	addr = ALIGN(addr, 512) / 512;
	len = ALIGN(len, 512);
	ret = emmc_blk_mq_issue_rw_rq(WRITE, dst, addr, len);
	if(ret == false) {
		uart_puts("emmc_blk_mq rq error!\n");
		return false;
	}
	ret = block_cmd_data(dst, len);
	if(ret == false) {
		uart_puts("mmc write error!!!\n");
		return false;
	}
	return ret;
}

int emmc_load_image(struct boot_image_info *info)
{
	int ret = true;

	if (info) {
		if (HW_CASCADE_FAST_BOOT == info->boot_scheme) {
		} else if (CPU_SCHED_FAST_BOOT == info->boot_scheme) {

		} else {
			if(info->name == pre_rolling){
#ifdef CONFIG_PRG_BOOT_STAGE_PLUSIN
				if (0 == system_is_wakeup()){
					ret = sched_emmc_load_image(info);
					if (ret == false)
						return ret;
					mc_bringup_arc0(info->rtt0_boot_addr);
				}else {
					//mc_arc0_release();
					/* nothing to do */
				}
#else
				uart_puts("error:image_info_csv.\n");
#endif
			}

			if(info->name == rtt){
				ret = sched_emmc_load_image(info);
				if (ret == false)
					return ret;
				ret = set_rtt0_boot_addr(info->rtt0_boot_addr);
				if (ret == false) {
						return ret;
				}
				system_os_ready();
				mc_bringup_arc1(info->rtt1_boot_addr);
			}
			if(info->name == uboot){
				ret = sched_emmc_load_image(info);
				if (ret == false)
					return ret;
				nosec_jump_to(UBOOT_LOAD_ADDR);
			}
			if(info->name == kernel){
				ret = sched_emmc_load_image(info);
				if (ret == false)
					return ret;
				uart_puts("Kernel LE\n");

				arch_timer();
				spl_bootm();
				hang();
			}
		}
	}

	return ret;

}
#endif

void nvmem_boot(struct boot_image *images, int (*load_data)(u32, u32, u8 *),
				int (*load_image)(struct boot_image_info *))
{
	unsigned int idx, nof_load;
	struct boot_image_info *info;
	bool ret = false;
	nof_load = images->cnt;
	info = (struct boot_image_info *)(&images->data[0]);

	if ((load_data == NULL) || (load_image == NULL)){
		printf("nvmem function is NULL\n");
		return;
					}
	if (nof_load > MAX_FILES_NUM) {
		printf("the nof load is exceed the MAX");
		return;
				}

	if (MAGICE_DATA != images->magic_number) {
		printf("invalid boot argument...\n");
		return;
				}

	for (idx = 0; idx < nof_load; idx++) {
					/*
					ret = nand_load_image(&info[idx]);
					*/
		switch (info[idx].type) {
		case LOAD_DATA:
			ret = load_data(info[idx].nvm_base, info[idx].size, (u8 *)info[idx].mem_base);
			break;
		 case LOAD_IMAGE:
		 case LOAD_nIMAGE:
			ret = load_image(&info[idx]);
			 break;

			}
					/*
					ret = nand_load_image(&info[idx]);
				*/
		if (ret == false) {
			printf("boot[%d] %d failed\n", info[idx].type, info[idx].name);
			return;
				}
			}


		}
#ifdef CONFIG_SPL_MMC_SUPPORT
static void emmc_pin_cfg(void)
{
	u32 reg;
	u32 val;

	for (reg = PAD0_G2_FUNC_SEL;
			reg <= PAD11_G2_FUNC_SEL; reg += 4)
	 	writel(0, reg);
	for (reg = PAD0_G2_CTRL; reg <= PAD11_G2_CTRL; reg += 4) {
		val = readl(reg);
		val &= ~(0xf);
		val |= 0x8;
		writel(val, reg);
	}

}
#endif

static void flash_pin_cfg(void)
{
	 u32 reg;

	 for (reg = PAD10_G1_FUNC_SEL;
	 		reg <= PAD16_G1_FUNC_SEL; reg += 4)
		writel(0, reg);
    }

static void syst_clk_cfg(void)
{
	writel(0, CLK_SYST_CFG); /*0x10300228 syst */
}

#ifdef CONFIG_SPL_ARGS_TRSF
int spl_trsf_args(int (*load_data)(u32, u32, u8 *), u32 bootmod)
{
	u32 ret,len;
	char * args = NULL;

	len = strlen(CONFIG_SPL_ARGS_STR)+1;
	if(len > ARGS_REAL_SZ)
	return -1;

	args = (char *)smalloc(ARGS_SZ);
	*(volatile unsigned int *)args = ARGS_MAGIC_DATA;
	*((volatile unsigned int *)args+ARGS_MAGIC_DATA_END) = ARGS_MAGIC_DATA;
	memcpy((void*)(args+ARGS_MAGIC_DATA_SZ),(void*)CONFIG_SPL_ARGS_STR,len);

	switch (bootmod) {
		default:
		case BOOT_DEVICE_NOR:
			ret = load_data(NOR_ARGS_OFF, ARGS_SZ, (u8 *)ARGS_IDLE_ADDR);
			if(ret == false){
				uart_puts("nraf\n");
				return ret;
			}
			if(*(volatile unsigned int *)ARGS_VSI_ADDR == *(volatile unsigned int *)CONFIG_SPL_ARGS_STR)
				break;

			ret = spi_nor_program_image(NOR_ARGS_OFF,ARGS_SZ,(u8*)(unsigned long)args);
			if(ret){
				mc_printf("nwaf\n");
				return ret;
			}

			ret = load_data(NOR_ARGS_OFF, ARGS_SZ, (u8 *)ARGS_IDLE_ADDR);
			if(ret == false){
				mc_printf("nwraf\n");
				return ret;
			}
			break;

		case BOOT_DEVICE_EMMC:
			ret = load_data(MMC_ARGS_OFF, ARGS_SZ, (u8 *)ARGS_IDLE_ADDR);
			if(ret == false){
				mc_printf("mraf\n");
				return ret;
			}
			if(*(volatile unsigned int *)ARGS_VSI_ADDR == *(volatile unsigned int *)CONFIG_SPL_ARGS_STR)
				break;
			ret = emmc_write_data(MMC_ARGS_OFF,ARGS_SZ,(u8*)(unsigned long)args);
			if(ret == false){
				mc_printf("mwaf\n");
				return ret;
			}
			ret = load_data(MMC_ARGS_OFF, ARGS_SZ, (u8 *)ARGS_IDLE_ADDR);
			if(ret == false){
				mc_printf("mwraf\n");
				return ret;
			}
			break;
		/*case BOOT_DEVICE_NAND:
			break;*/
	}
	if(*(volatile unsigned int *)ARGS_IDLE_ADDR != ARGS_MAGIC_DATA || *(volatile unsigned int *)ARGS_END_ADDR != ARGS_MAGIC_DATA){
		uart_puts("acf\n");
		return -1;
	}
	sfree(args);
	mc_printf("spl read sdl bootargs: %s\r\n",(char*)ARGS_IDLE_ADDR+ARGS_MAGIC_DATA_SZ);
	return 0;
}
#endif
void board_init_r(gd_t *dummy1, ulong dummy2)
{
	u8 bootmod;
#ifdef CONFIG_SPL_MMC_SUPPORT
	int  ret;
#endif
	struct boot_image *images;
	int (*load_data_ptr)(u32, u32, u8 *) = NULL;
	int (*load_image_ptr)(struct boot_image_info *) = NULL;
	images = (struct boot_image *)(CONFIG_SPL_TEXT_BASE - BOOT_INFO_OFFSET);

	timer_enable();
	syst_clk_cfg();

	sw_bonding_option_init();
	heap_init();
	bootmod = nvm_boot_device();

	switch (bootmod) {
		default:
		case BOOT_DEVICE_NOR:
			load_data_ptr = spi_transaction_dma_read;
			load_image_ptr = nor_load_image;
			flash_pin_cfg();
			board_spic_init();
			break;
		/*case BOOT_DEVICE_NAND:
			load_data_ptr = nand_read_data;
			load_image_ptr = nand_load_image;
			flash_pin_cfg();
			board_spic_init();
		break;*/
#ifdef CONFIG_SPL_MMC_SUPPORT
		case BOOT_DEVICE_EMMC:
			load_data_ptr = emmc_read_data;
			load_image_ptr = emmc_load_image;
			emmc_pin_cfg();
			ret = mmc_init_card();
			if (ret == false) {
				uart_puts("emmc init failed\n");
				hang();
			}
			break;
#endif
	}

#ifdef CONFIG_SPL_ARGS_TRSF
	if(spl_trsf_args(load_data_ptr, bootmod)) {
		uart_puts("args trsf failed\n");
		hang();
	}
#endif
	nvmem_boot(images, load_data_ptr, load_image_ptr);
	while(1);
}

