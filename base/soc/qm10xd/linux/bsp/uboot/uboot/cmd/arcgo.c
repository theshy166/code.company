/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <bootm.h>
#include <command.h>
#include <lmb.h>
#include <linux/compiler.h>

/*
 * zImage booting support
 */
static int arcgo_start(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[], unsigned int addr)
{
	int index=0;
	unsigned int base = 0x10000000;
	unsigned int ctrl = 0x10000030;
	unsigned int val;

	/* Setup Linux kernel zImage entry point */
	if (!argc) {
	    index = 0;
	} else {
	    index = simple_strtoul(argv[1], NULL, 16);
	}

    if (index > 0)
    {
        base += 0x40;
        ctrl += 0x20;
    }

    *(volatile unsigned int *)base = 0x0f80200a;
    *(volatile unsigned int *)(base + 0x4) = (addr >> 16) | ((addr & 0xffff) << 16);
    *(volatile unsigned int *)(base + 0x8) = 0x00002020;
    *(volatile unsigned int *)(base + 0xc) = 0x00000000;

    val = *(volatile unsigned int *)ctrl;
    val |= 4;
    *(volatile unsigned int *)ctrl = val;

    val |= 1;
    *(volatile unsigned int *)ctrl = val;

    val &= 0xfffffffe;
    *(volatile unsigned int *)ctrl = val;

	return 0;
}

int do_arcgo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong image_addr = 0;

	argc--; argv++;

	image_addr = simple_strtoul(argv[0], NULL, 16);

	if (arcgo_start(cmdtp, flag, argc, argv, image_addr))
		return 1;

	return 0;
}

#ifdef CONFIG_SYS_LONGHELP
static char arcgo_help_text[] =
	"[addr [index]]\n"
	"    - boot ARC image stored in memory\n"
	"\tThe argument 'index' is optional and specifies the index\n"
	"\tof the ARC in memory. \n"
	"";
#endif

U_BOOT_CMD(
	arcgo,	CONFIG_SYS_MAXARGS,	1,	do_arcgo,
	"boot ARC image from memory", arcgo_help_text
);
