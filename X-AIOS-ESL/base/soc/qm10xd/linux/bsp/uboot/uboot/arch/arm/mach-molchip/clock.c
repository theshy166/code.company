/*
 * (C) Copyright 2019
 *
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/clock.h>
#include <asm/arch/molchip_fy01.h>

void clock_init_core(void)
{
    u32 reg;
    reg = readl(CLK_DIG_CTRL);
    reg &= 0xFFFFFFF;
    reg |= (0x3a << 24);
    writel(reg, CLK_DIG_CTRL);

    reg = readl(REG_CPU_CLK_CTRL1);
    reg &= ~CKG_CPU_CORE_SEL(3);
    reg &= ~CKG_CS_DBG_SEL(3);
    reg &= ~CKG_CPU_DBG_DIV(7);
    reg &= ~CKG_CPU_SYS_MTX_DIV(7);

    reg |= CKG_CPU_DBG_DIV(3);
    reg |= CKG_CPU_SYS_MTX_DIV(1);
    reg |= CKG_CPU_CORE_SEL(2);
    reg |= CKG_CS_DBG_SEL(2);
    writel(reg, REG_CPU_CLK_CTRL1);
}

void clock_init_bus(void)
{
    writel(2, CLK_AP_APB_CFG);  //ap apb clock 100M
    writel(3, CLK_GLB_APB_CFG); //GLB clock 200M

}

void clock_init_sys(void)
{
    u32 reg;
    writel(2, CLK_AP_SYS_CFG);  //ap sys clock 200M

    reg = readl(VDSP_CLK_RST_CTRL);
    reg &= ~CKG_VDSP_SYS_SEL(3);
    reg |= CKG_VDSP_SYS_SEL(3);
    writel(reg , VDSP_CLK_RST_CTRL); //vdsp sys clock 640M
}

int clock_init(void)
{

	clock_init_core();
	clock_init_bus();
    clock_init_sys();

	return 0;
}
