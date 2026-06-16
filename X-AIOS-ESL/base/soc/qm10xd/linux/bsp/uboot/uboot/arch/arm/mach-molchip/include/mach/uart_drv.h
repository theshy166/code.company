/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


#ifndef _UART_DRV_H_
#define _UART_DRV_H_

void uart_config_clk(unsigned int uport);
void uart_set_baudrate(u32 baudrate, u32 port);
bool uart_read_rbr(u32 port,u8 *rcv);
void uart_hw_init(void);
u8 uart_getc(u32 port);
void uart_putc(u32 port, u8 xfer);
void uart_puts(char *s);
void mc_printf(const char *format, ...);
#if 0
void mc_printf1(const char *format, ...);
#endif
#endif
