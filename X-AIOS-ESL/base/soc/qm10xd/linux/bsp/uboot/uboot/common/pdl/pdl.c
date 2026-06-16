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
#include <mach/spic.h>
#include <mach/uart_drv.h>
#include <mach/download.h>
#include <mach/ddr.h>
#include <mach/board.h>

void s_init(void)
{
        //timer clk init
        molchip_systimer_clk_enable();

        //uart0 clk init
        molchip_uart_clk_enable(REG_UART0_BASE);

        //uart1 clk init
        molchip_uart_clk_enable(REG_UART1_BASE);

#ifdef CONFIG_TARGET_TS01
		//spic clk enable
		molchip_spic_clk_enable();
#endif

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
void board_init_f(ulong dummy)
{
        /* CPU/BUS CLK config if needed*/

	s_init();
        //uart_hw_init();

#ifdef CONFIG_TARGET_TS01
		board_spic_init();
#endif

	/* Bias config reset */
//	ana_bias_reset();
	/* DDR config if needed */
	DRAM_Init();
    //clock_init();


}
void board_init_r(gd_t *dummy1, ulong dummy2)
{
	uint uart_port;

	uart_port = uart_identify();

	uart_preboot(uart_port);

	uart_boot(uart_port);
	while(1);
}
#ifdef CONFIG_ARM64
void hang(void)
{
	while(1);
}
#endif
