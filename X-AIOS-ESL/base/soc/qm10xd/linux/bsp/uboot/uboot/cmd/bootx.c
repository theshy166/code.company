/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <bootm.h>
#include <command.h>
#include <linux/compiler.h>
#include <libfdt.h>
#include <../board/molchip/board/system.h>

int __weak bootx_setup(ulong image, ulong *start, ulong *end)
{
	/* Please define bootx_setup() for your platform */

	puts("Your platform's Image format isn't supported yet!\n");
	return -1;
}


int do_bootx(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	char *argv2;
	struct boot_header *header_info;
	ulong Image_addr = 0, fdt_addr = 0;
	ulong machid = 0xffffffff;
	void(*kernel_entry)(int zero, int arch, unsigned int params);
	unsigned long r2;

	/* Consume 'bootx' */
	argc--; argv++;
	argv2 = argv[2];

	Image_addr = simple_strtoul(argv[0], NULL, 16);
	fdt_addr = simple_strtoul(argv[2], NULL, 16);

	if(Image_addr == 0 || fdt_addr == 0){
		printf("invalid argument...\n");
		return ret;
	}

	sprintf(argv2,"0x%08lx",fdt_addr);
	header_info = (struct boot_header *)(Image_addr - HEADER_LEN);

	if(header_info->magic_num != 0x46524550){
		printf("Bad linux ARM Image magic!\n");
		return ret;
	}


	/*relocate the device tree*/
	memcpy((unsigned int *)header_info->image_dst, (unsigned int *)header_info->image_src, header_info->image_len);

	if (fdt_check_header((void *)fdt_addr) != 0){
		printf("Bad fdt image...\n");
		return ret;
	}

#ifdef CONFIG_BOOTATAGS_FH
	extern void setup_atags(char **fdt);
	setup_atags((char **)&fdt_addr);
#endif

	kernel_entry = (void (*)(int, int, unsigned int))Image_addr;

	printf("\nStarting kernel ...\n\n");
	cleanup_before_linux();

	r2 = (unsigned long)fdt_addr;
	kernel_entry(0, machid, r2);

	return ret;
}

#ifdef CONFIG_SYS_LONGHELP
static char bootx_help_text[] =
	"[addr [initrd[:size]] [fdt]]\n"
	"    - boot Linux Image stored in memory\n"
	"\tThe argument 'initrd' is optional and specifies the address\n"
	"\tof the initrd in memory. The optional argument ':size' allows\n"
	"\tspecifying the size of RAW initrd.\n"
	"";
#endif

U_BOOT_CMD(
	bootx,	CONFIG_SYS_MAXARGS,	1,	do_bootx,
	"boot Linux Image image from memory", bootx_help_text
);
