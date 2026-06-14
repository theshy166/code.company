// SPDX-License-Identifier: GPL-2.0+
/*
 * FB driver for the ST7735S LCD Controller
 *
 * Copyright (C) 2013 Noralf Tronnes
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <video/mipi_display.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>


#include "fbtft.h"

#define DRVNAME "fb_st7735s"
#define DEFAULT_GAMMA   "0F 1A 0F 18 2F 28 20 22 1F 1B 23 37 00 07 02 10\n" \
			"0F 1B 0F 17 33 2C 29 2E 30 30 39 3F 00 07 03 10"

// ST7735S initialization code
static const s16 st7735s_init_sequence[] = {

	// Sleep out
	-1, 0x11,                               /* Sleep out */
	-2, 120,                                /* delay 120ms */

	// Frame rate settings for ST7735S
	-1, 0xB1, 0x05, 0x3A, 0x3A,            /* Frame rate 80Hz */
	-1, 0xB2, 0x05, 0x3A, 0x3A,            /* Frame rate 80Hz */
	-1, 0xB3, 0x05, 0x3A, 0x3A, 0x05, 0x3A, 0x3A, /* Frame rate 80Hz */

	// Dot inversion
	-1, 0xB4, 0x03,

	// Power sequence settings for ST7735S
	-1, 0xC0, 0x62, 0x02, 0x04,            /* Power control */
	-1, 0xC1, 0xC0,                        /* Power control */
	-1, 0xC2, 0x0D, 0x00,                  /* Power control */
	-1, 0xC3, 0x8D, 0x6A,                  /* Power control */
	-1, 0xC4, 0x8D, 0xEE,                  /* Power control */

	// VCOM setting
	-1, 0xC5, 0x12,

	// MX, MY, RGB mode
	-1, 0x36, 0xC8,

	// Gamma correction sequence for ST7735S
	-1, 0xE0, 0x03, 0x1B, 0x12, 0x11, 0x3F, 0x3A, 0x32, 0x34, 0x2F, 0x2B, 0x30, 0x3A, 0x00, 0x01, 0x02, 0x05,
	-1, 0xE1, 0x03, 0x1B, 0x12, 0x11, 0x32, 0x2F, 0x2A, 0x2F, 0x2E, 0x2C, 0x35, 0x3F, 0x00, 0x00, 0x01, 0x05,

	// Gate power save mode
	-1, 0xFC, 0x8C,

	// 65k color mode
	-1, 0x3A, 0x05,

	// Display on
	-1, 0x29,
	-2, 100,                               /* delay 100ms */

	// End of sequence marker
	-3
};

static void set_addr_win(struct fbtft_par *par, int xs, int ys, int xe, int ye)
{
	//unsigned short xEnd = 127;
	//unsigned short yEnd = 127;
	unsigned short xoffset = 2;
	unsigned short yoffset = 1;
	xs = xs + xoffset;
	xe = xe + xoffset;
	ys = ys + yoffset;
	ye = ye + yoffset;
	write_reg(par, 0x36, 0x00);
	write_reg(par, 0x2A, ((xs >> 8) & 0xFF), (xs & 0xFF) ,((xe >> 8) & 0xFF), (xe & 0xFF));
	write_reg(par, 0x2B, ((ys >> 8) & 0xFF), (ys & 0xFF), ((ye >> 8) & 0xFF), (ye & 0xFF));
	write_reg(par, 0x2C);
}

void __fb_st7735s_test(struct fbtft_par *par, unsigned short color) {
	int __w = 128, __h = 128;
	int i, m;
	write_reg(par, 0x29);
	write_reg(par, 0x2c);
	for(i=0; i<__w; i++) {
		for(m=0; m<__h; m++) {
			write_dat(par, ((color>>8) & 0xff));
			write_dat(par, ((color>>0) & 0xff));
		}
	}
}
EXPORT_SYMBOL(__fb_st7735s_test);

static struct fbtft_display display =  {
	.regwidth = 8,
	.width = 128,
	.height = 128,
	.init_sequence = st7735s_init_sequence,
	.gamma_num = 2,
	.gamma_len = 16,
	.gamma = DEFAULT_GAMMA,
	.fbtftops = {
		.set_addr_win = set_addr_win,
	},
};

FBTFT_REGISTER_DRIVER(DRVNAME, "sitronix,st7735s", &display);

MODULE_ALIAS("spi:" DRVNAME);
MODULE_ALIAS("platform:" DRVNAME);
MODULE_ALIAS("spi:st7735s");
MODULE_ALIAS("platform:st7735s");

MODULE_DESCRIPTION("FB driver for the ST7735S LCD Controller");
MODULE_AUTHOR("Noralf Tronnes");
MODULE_LICENSE("GPL");
