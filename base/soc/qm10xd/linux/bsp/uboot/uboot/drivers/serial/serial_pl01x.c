/*
 * (C) Copyright 2000
 * Rob Taylor, Flying Pig Systems. robt@flyingpig.com.
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* Simple U-Boot driver for the PrimeCell PL010/PL011 UARTs */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <watchdog.h>
#include <asm/io.h>
#include <serial.h>
#include <dm/platform_data/serial_pl01x.h>
#include <linux/compiler.h>
#include "serial_pl01x_internal.h"
#include <fdtdec.h>

DECLARE_GLOBAL_DATA_PTR;

#ifndef CONFIG_DM_SERIAL

static volatile unsigned char *const port[] = CONFIG_PL01x_PORTS;
static enum pl01x_type pl01x_type __attribute__ ((section(".data")));
static struct pl01x_regs *base_regs __attribute__ ((section(".data")));
#define NUM_PORTS (sizeof(port)/sizeof(port[0]))

#endif

static int pl01x_putc(struct pl01x_regs *regs, char c)
{
	/* Wait until there is space in the FIFO */
	if (readl(&regs->fr) & UART_PL01x_FR_TXFF)
		return -EAGAIN;

	/* Send the character */
	writel(c, &regs->dr);

	return 0;
}

static int pl01x_getc(struct pl01x_regs *regs)
{
	unsigned int data;

	/* Wait until there is data in the FIFO */
	if (readl(&regs->fr) & UART_PL01x_FR_RXFE)
		return -EAGAIN;

	data = readl(&regs->dr);

	/* Check for an error flag */
	if (data & 0xFFFFFF00) {
		/* Clear the error */
		writel(0xFFFFFFFF, &regs->ecr);
		return -1;
	}

	return (int) data;
}

static int pl01x_tstc(struct pl01x_regs *regs)
{
	WATCHDOG_RESET();
	return !(readl(&regs->fr) & UART_PL01x_FR_RXFE);
}

static int pl01x_generic_serial_init(struct pl01x_regs *regs,
				     enum pl01x_type type)
{
	switch (type) {
	case TYPE_PL010:
		/* disable everything */
		writel(0, &regs->pl010_cr);
		break;
	case TYPE_PL011:
		/* disable everything */
		writel(0, &regs->pl011_cr);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int pl011_set_line_control(struct pl01x_regs *regs)
{
	unsigned int lcr;
	/*
	 * Internal update of baud rate register require line
	 * control register write
	 */
	lcr = UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN;
	writel(lcr, &regs->pl011_lcrh);
	return 0;
}

static int pl01x_generic_setbrg(struct pl01x_regs *regs, enum pl01x_type type,
				int clock, int baudrate)
{
	switch (type) {
	case TYPE_PL010: {
		unsigned int divisor;

		/* disable everything */
		writel(0, &regs->pl010_cr);

		switch (baudrate) {
		case 9600:
			divisor = UART_PL010_BAUD_9600;
			break;
		case 19200:
			divisor = UART_PL010_BAUD_19200;
			break;
		case 38400:
			divisor = UART_PL010_BAUD_38400;
			break;
		case 57600:
			divisor = UART_PL010_BAUD_57600;
			break;
		case 115200:
			divisor = UART_PL010_BAUD_115200;
			break;
		default:
			divisor = UART_PL010_BAUD_38400;
		}

		writel((divisor & 0xf00) >> 8, &regs->pl010_lcrm);
		writel(divisor & 0xff, &regs->pl010_lcrl);

		/*
		 * Set line control for the PL010 to be 8 bits, 1 stop bit,
		 * no parity, fifo enabled
		 */
		writel(UART_PL010_LCRH_WLEN_8 | UART_PL010_LCRH_FEN,
		       &regs->pl010_lcrh);
		/* Finally, enable the UART */
		writel(UART_PL010_CR_UARTEN, &regs->pl010_cr);
		break;
	}
	case TYPE_PL011: {
		unsigned int temp;
		unsigned int divider;
		unsigned int remainder;
		unsigned int fraction;

		/*
		* Set baud rate
		*
		* IBRD = UART_CLK / (16 * BAUD_RATE)
		* FBRD = RND((64 * MOD(UART_CLK,(16 * BAUD_RATE)))
		*		/ (16 * BAUD_RATE))
		*/
		temp = 16 * baudrate;
		divider = clock / temp;
		remainder = clock % temp;
		temp = (8 * remainder) / baudrate;
		fraction = (temp >> 1) + (temp & 1);

		writel(divider, &regs->pl011_ibrd);
		writel(fraction, &regs->pl011_fbrd);

		pl011_set_line_control(regs);
		/* Finally, enable the UART */
		writel(UART_PL011_CR_UARTEN | UART_PL011_CR_TXE |
		       UART_PL011_CR_RXE | UART_PL011_CR_RTS, &regs->pl011_cr);
		break;
	}
	default:
		return -EINVAL;
	}

	return 0;
}

#ifndef CONFIG_DM_SERIAL
static void pl01x_serial_init_baud(int baudrate)
{
	int clock = 0;

#if defined(CONFIG_PL010_SERIAL)
	pl01x_type = TYPE_PL010;
#elif defined(CONFIG_PL011_SERIAL)
	pl01x_type = TYPE_PL011;
	clock = CONFIG_PL011_CLOCK;
#endif
	base_regs = (struct pl01x_regs *)port[CONFIG_CONS_INDEX];

	pl01x_generic_serial_init(base_regs, pl01x_type);
	pl01x_generic_setbrg(base_regs, pl01x_type, clock, baudrate);
}


/*
 * Integrator AP has two UARTs, we use the first one, at 38400-8-N-1
 * Integrator CP has two UARTs, use the first one, at 38400-8-N-1
 * Versatile PB has four UARTs.
 */
#if 1
int pl01x_serial_init(void)
{
	pl01x_serial_init_baud(CONFIG_BAUDRATE);
	return 0;
}
#else
static void uart_init(unsigned int port_id);
int pl01x_serial_init(void)
{
	//pl01x_serial_init_baud(CONFIG_BAUDRATE);
	
	uart_init(0);
	return 0;
}
#endif

#if 1
static void pl01x_serial_putc(const char c)
{
	if (c == '\n')
		while (pl01x_putc(base_regs, '\r') == -EAGAIN);

	while (pl01x_putc(base_regs, c) == -EAGAIN);
}

static int pl01x_serial_getc(void)
{
	while (1) {
		int ch = pl01x_getc(base_regs);

		if (ch == -EAGAIN) {
			WATCHDOG_RESET();
			continue;
		}

		return ch;
	}
}
#endif

static int pl01x_serial_tstc(void)
{
	return pl01x_tstc(base_regs);
}

#if 0	
#if 1
#define PRIMARY 0


/**
 * Macro used to define the secondary UART channel
 *
 */
#define AUXILIARY 1

#ifdef WITH_LINUX_SMP
    #define CONSOLE AUXILIARY
#else
    #define CONSOLE PRIMARY
#endif /* WITH_LINUX_SMP */

///#define NUM_PORTS 2
#define ADD_CR_TO_LF FALSE


#ifdef VERSATILE_EB
    #define UART_CLK 24000000
#else
    #define UART_CLK 14745600
#endif


/*  Definitions common to both PL010 & PL011 */
#define UART_PL01x_DR                   0x00     /*  Data read or written from the interface. */
#define UART_PL01x_RSR                  0x04     /*  Receive status register (Read). */
#define UART_PL01x_ECR                  0x04     /*  Error clear register (Write). */
#define UART_PL01x_FR                   0x18     /*  Flag register (Read only). */

#define UART_PL01x_RSR_OE               0x08
#define UART_PL01x_RSR_BE               0x04
#define UART_PL01x_RSR_PE               0x02
#define UART_PL01x_RSR_FE               0x01

#define UART_PL01x_FR_TXFE              0x80
#define UART_PL01x_FR_RXFF              0x40
#define UART_PL01x_FR_TXFF              0x20
#define UART_PL01x_FR_RXFE              0x10
#define UART_PL01x_FR_BUSY              0x08
#define UART_PL01x_FR_TMSK              (UART_PL01x_FR_TXFF + UART_PL01x_FR_BUSY)


/* PL011 Registers Offsets from UART Base adress */
#define UART_PL011_DR                   0x0
#define UART_PL011_RSR                  0x4
#define UART_PL011_ECR                  0x4
#define UART_PL011_FR                   0x18
#define UART_PL011_ILPR                 0x20
#define UART_PL011_IBRD                 0x24
#define UART_PL011_FBRD                 0x28
#define UART_PL011_LCRH                 0x2C
#define UART_PL011_CR                   0x30
#define UART_PL011_IFLS                 0x34
#define UART_PL011_IMSC                 0x38
#define UART_PL011_RIS                  0x3C
#define UART_PL011_MIS                  0x40
#define UART_PL011_ICR                  0x44
#define UART_PL011_DMACR                0x48

#define UART_PL011_PERIPH_ID0           0xFE0

/* Line control register */
#define UART_PL011_LCRH_SPS             (1 << 7)
#define UART_PL011_LCRH_WLEN_8          (3 << 5)
#define UART_PL011_LCRH_WLEN_7          (2 << 5)
#define UART_PL011_LCRH_WLEN_6          (1 << 5)
#define UART_PL011_LCRH_WLEN_5          (0 << 5)
#define UART_PL011_LCRH_FEN             (1 << 4)
#define UART_PL011_LCRH_STP2            (1 << 3)
#define UART_PL011_LCRH_EPS             (1 << 2)
#define UART_PL011_LCRH_PEN             (1 << 1)
#define UART_PL011_LCRH_BRK             (1 << 0)


/* Control Register */
#define UART_PL011_CR_CTSEN             (1 << 15)
#define UART_PL011_CR_RTSEN             (1 << 14)
#define UART_PL011_CR_OUT2              (1 << 13)
#define UART_PL011_CR_OUT1              (1 << 12)
#define UART_PL011_CR_RTS               (1 << 11)
#define UART_PL011_CR_DTR               (1 << 10)
#define UART_PL011_CR_RXE               (1 << 9)
#define UART_PL011_CR_TXE               (1 << 8)
#define UART_PL011_CR_LPE               (1 << 7)
#define UART_PL011_CR_IIRLP             (1 << 2)
#define UART_PL011_CR_SIREN             (1 << 1)
#define UART_PL011_CR_UARTEN            (1 << 0)


/* Interrupt Mask Set/Clear Register */
#define UART_PL011_IMSC_OEIM            (1 << 10)
#define UART_PL011_IMSC_BEIM            (1 << 9)
#define UART_PL011_IMSC_PEIM            (1 << 8)
#define UART_PL011_IMSC_FEIM            (1 << 7)
#define UART_PL011_IMSC_RTIM            (1 << 6)
#define UART_PL011_IMSC_TXIM            (1 << 5)
#define UART_PL011_IMSC_RXIM            (1 << 4)
#define UART_PL011_IMSC_DSRMIM          (1 << 3)
#define UART_PL011_IMSC_DCDMIM          (1 << 2)
#define UART_PL011_IMSC_CTSMIM          (1 << 1)
#define UART_PL011_IMSC_RIMIM           (1 << 0)

#endif
#define write32(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define read32(addr) (*(volatile unsigned int *)(addr))


#define DEFAULT_BAUDRATE 115200
#define DEFAULT_CONFIG  (UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN) /* 8N1, with FIFO */
#define DEFAULT_IRQMASK 0   /* same as reset value */



static volatile char * const port[NUM_PORTS] =
{
    (void *) 0xf8090000,
    (void *) 0x1c0a0000
};


/***** See corresponding header file for description *****/
static void serial_enable(int portnum)
{
    write32(port[portnum] + UART_PL011_CR,
        (UART_PL011_CR_UARTEN | UART_PL011_CR_TXE | UART_PL011_CR_RXE));
}


/***** See corresponding header file for description *****/
static void serial_disable(int portnum)
{
    write32(port[portnum] + UART_PL011_CR, 0x0);
}


/***** See corresponding header file for description *****/
static void serial_set_baud_rate(int portnum, int baudrate)
{
    int temp;
    int divider;
    int remainder;
    int fraction;

    /*
     * Set baud rate
     *
     * IBRD = UART_CLK / (16 * BAUD_RATE)
     * FBRD = ROUND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
     */
    temp      = 16 * baudrate;
    divider   = UART_CLK / temp;
    remainder = UART_CLK % temp;
    temp      = (8 * remainder) / baudrate;
    fraction  = (temp >> 1) + (temp & 1);

    write32(port[portnum] + UART_PL011_IBRD, divider);
    write32(port[portnum] + UART_PL011_FBRD, fraction);
}


/***** See corresponding header file for description *****/
static void serial_configure(int portnum, int config)
{
    write32(port[portnum] + UART_PL011_LCRH, config);
}


/***** See corresponding header file for description *****/
static void serial_interrupt_mask_set(int portnum, int irq_mask)
{
    write32(port[portnum] + UART_PL011_IMSC, irq_mask);
}


/***** See corresponding header file for description *****/
static void pl01x_serial_init1(int portnum, int baudrate, int config, int irq_mask)
{
    /* Defaults */
    if (baudrate == 0)
    {
        baudrate = DEFAULT_BAUDRATE;
    }
    if (config == 0)
    {
        config = DEFAULT_CONFIG;
    }
    if (irq_mask == 0)
    {
        irq_mask = DEFAULT_IRQMASK;
    }

    /* Disable UART */
    serial_disable(portnum);

    /* Set baud rate */
    serial_set_baud_rate(portnum, baudrate);

    /* Configure UART */
    serial_configure(portnum, config);

    /* Configure mask for the interrupts generated by the UART */
    serial_interrupt_mask_set(portnum, irq_mask);

    /* Enable UART */
    serial_enable(portnum);
}


/***** See corresponding header file for description *****/
static void pl01x_serial_putc(int portnum, char c)
{
#if ADD_CR_TO_LF
    if (c == '\n')
    {
        /* Wait until there is space in the FIFO */
        while (read32(port[portnum] + UART_PL01x_FR) & UART_PL01x_FR_TXFF);
        /* Send CR */
        write32(port[portnum] + UART_PL01x_DR, '\r');
}
#endif

    /* Wait until there is space in the FIFO */
    while (read32(port[portnum] + UART_PL01x_FR) & UART_PL01x_FR_TXFF);

    /* Send the character */
    write32(port[portnum] + UART_PL01x_DR, c);
}


/***** See corresponding header file for description *****/
static int pl01x_serial_getc(int portnum)
{
    int data;

    /* Wait until there is data in the FIFO */
    while (1) {
		if (read32(port[portnum] + UART_PL01x_FR) & UART_PL01x_FR_RXFE)
			;
		else
			break;
    }

    data = read32(port[portnum] + UART_PL01x_DR);

    /* Check for an error flag */
    if (data & 0xFFFFFF00)
    {
        /* Clear the error */
        write32(port[portnum] + UART_PL01x_ECR, 0xFFFFFFFF);
        return -1;
    }

    return (int)data;
}


/***** See corresponding header file for description *****/
static void pl01x_serial_drain(int portnum)
{
    /* Spin until the Transmit FIFO is empty */
    while (!(read32(port[portnum] + UART_PL01x_FR) & UART_PL01x_FR_TXFE));
}



static void uart_init(unsigned int port_id)
{
	pl01x_serial_init1(CONSOLE, 115200, UART_PL011_LCRH_WLEN_8, UART_PL011_IMSC_RXIM);
}



#endif

static void pl01x_serial_setbrg(void)
{
	/*
	 * Flush FIFO and wait for non-busy before changing baudrate to avoid
	 * crap in console
	 */
	while (!(readl(&base_regs->fr) & UART_PL01x_FR_TXFE))
		WATCHDOG_RESET();
	while (readl(&base_regs->fr) & UART_PL01x_FR_BUSY)
		WATCHDOG_RESET();
	pl01x_serial_init_baud(gd->baudrate);
}

static struct serial_device pl01x_serial_drv = {
	.name	= "pl01x_serial",
	.start	= pl01x_serial_init,
	.stop	= NULL,
	.setbrg	= pl01x_serial_setbrg,
	.putc	= pl01x_serial_putc,
	.puts	= default_serial_puts,
	.getc	= pl01x_serial_getc,
	.tstc	= pl01x_serial_tstc,
};

void pl01x_serial_initialize(void)
{
	serial_register(&pl01x_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &pl01x_serial_drv;
}

#endif /* nCONFIG_DM_SERIAL */

#ifdef CONFIG_DM_SERIAL

struct pl01x_priv {
	struct pl01x_regs *regs;
	enum pl01x_type type;
};

static int pl01x_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct pl01x_serial_platdata *plat = dev_get_platdata(dev);
	struct pl01x_priv *priv = dev_get_priv(dev);

	if (!plat->skip_init) {
		pl01x_generic_setbrg(priv->regs, priv->type, plat->clock,
				     baudrate);
	}

	return 0;
}

static int pl01x_serial_probe(struct udevice *dev)
{
	struct pl01x_serial_platdata *plat = dev_get_platdata(dev);
	struct pl01x_priv *priv = dev_get_priv(dev);

	priv->regs = (struct pl01x_regs *)plat->base;
	priv->type = plat->type;
	if (!plat->skip_init)
		return pl01x_generic_serial_init(priv->regs, priv->type);
	else
		return 0;
}

static int pl01x_serial_getc(struct udevice *dev)
{
	struct pl01x_priv *priv = dev_get_priv(dev);

	return pl01x_getc(priv->regs);
}

static int pl01x_serial_putc(struct udevice *dev, const char ch)
{
	struct pl01x_priv *priv = dev_get_priv(dev);

	return pl01x_putc(priv->regs, ch);
}

static int pl01x_serial_pending(struct udevice *dev, bool input)
{
	struct pl01x_priv *priv = dev_get_priv(dev);
	unsigned int fr = readl(&priv->regs->fr);

	if (input)
		return pl01x_tstc(priv->regs);
	else
		return fr & UART_PL01x_FR_TXFF ? 0 : 1;
}

static const struct dm_serial_ops pl01x_serial_ops = {
	.putc = pl01x_serial_putc,
	.pending = pl01x_serial_pending,
	.getc = pl01x_serial_getc,
	.setbrg = pl01x_serial_setbrg,
};

#if CONFIG_IS_ENABLED(OF_CONTROL)
static const struct udevice_id pl01x_serial_id[] ={
	{.compatible = "arm,pl011", .data = TYPE_PL011},
	{.compatible = "arm,pl010", .data = TYPE_PL010},
	{}
};

static int pl01x_serial_ofdata_to_platdata(struct udevice *dev)
{
	struct pl01x_serial_platdata *plat = dev_get_platdata(dev);
	fdt_addr_t addr;

	addr = devfdt_get_addr(dev);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	plat->base = addr;
	plat->clock = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev), "clock",
				     1);
	plat->type = dev_get_driver_data(dev);
	plat->skip_init = fdtdec_get_bool(gd->fdt_blob, dev_of_offset(dev),
	                                  "skip-init");
	return 0;
}
#endif

U_BOOT_DRIVER(serial_pl01x) = {
	.name	= "serial_pl01x",
	.id	= UCLASS_SERIAL,
	.of_match = of_match_ptr(pl01x_serial_id),
	.ofdata_to_platdata = of_match_ptr(pl01x_serial_ofdata_to_platdata),
	.platdata_auto_alloc_size = sizeof(struct pl01x_serial_platdata),
	.probe = pl01x_serial_probe,
	.ops	= &pl01x_serial_ops,
	.flags = DM_FLAG_PRE_RELOC,
	.priv_auto_alloc_size = sizeof(struct pl01x_priv),
};

#endif

#if defined(CONFIG_DEBUG_UART_PL010) || defined(CONFIG_DEBUG_UART_PL011)

#include <debug_uart.h>

static void _debug_uart_init(void)
{
#ifndef CONFIG_DEBUG_UART_SKIP_INIT
	struct pl01x_regs *regs = (struct pl01x_regs *)CONFIG_DEBUG_UART_BASE;
	enum pl01x_type type = CONFIG_IS_ENABLED(DEBUG_UART_PL011) ?
				TYPE_PL011 : TYPE_PL010;

	pl01x_generic_serial_init(regs, type);
	pl01x_generic_setbrg(regs, type,
			     CONFIG_DEBUG_UART_CLOCK, CONFIG_BAUDRATE);
#endif
}

static inline void _debug_uart_putc(int ch)
{
	struct pl01x_regs *regs = (struct pl01x_regs *)CONFIG_DEBUG_UART_BASE;

	pl01x_putc(regs, ch);
}

DEBUG_UART_FUNCS

#endif
