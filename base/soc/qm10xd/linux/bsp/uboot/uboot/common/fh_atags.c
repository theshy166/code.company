#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/setup.h>

#ifdef CONFIG_BOOTATAGS_FH
#define IMAGE_ADDR 		0x40008000
#define	IMAGE_DTB  		0x41000000
#define FDT_SIZE_OFFSET	0x6
#define ATAGES_SIZE		0x8000


static struct tag *atags_params;

static void setup_start_tag (void *boot_params)
{
	atags_params = (struct tag *)boot_params;

	atags_params->hdr.tag = ATAG_CORE;
	atags_params->hdr.size = tag_size (tag_core);

	atags_params->u.core.flags = 0;
	atags_params->u.core.pagesize = 0;
	atags_params->u.core.rootdev = 0;

	atags_params = tag_next (atags_params);
}

static void setup_commandline_tag(void *boot_params)
{
	char *p;
	char *cmdline = env_get("bootargs");
	//char cmdline[256] = "atags=1 mem=48M boot=nor earlycon console=ttyS0,115200 root=/dev/mtdblock3 rootfstype=squashfs console=ttyS0,115200 mtdparts=spi-nor:448K(U),64K(E),6656K(K),4096K(R),4096K(T)";

	if (!cmdline)
		return;

	/* eat leading white space */
	for (p = cmdline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	atags_params->hdr.tag = ATAG_CMDLINE;
	atags_params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (atags_params->u.cmdline.cmdline, p);

	atags_params = tag_next (atags_params);

	debug("[atags] %s\n", cmdline);
}

static void setup_fdt_addr_tag(void *boot_params, char **fdt)
{
	atags_params->hdr.tag = ATAG_FDT;
	atags_params->hdr.size = tag_size(tag_fdt);
	atags_params->u.devtree.start = (unsigned int)*fdt;

	atags_params = tag_next (atags_params);
}

static void setup_end_tag(void *boot_params)
{
	atags_params->hdr.tag = ATAG_NONE;
	atags_params->hdr.size = 0;
}

static void fix_fdt_command_line(void * atags_addr, char **fdt)
{
	setup_start_tag(atags_addr);

	setup_commandline_tag(atags_addr);

	setup_fdt_addr_tag(atags_addr, fdt);

	setup_end_tag(atags_addr);
}

 static void combine_fdt_atags(void * atags_addr, char **fdt)
{
	size_t atags_length = 0;
	struct tag * tag = (struct tag *)atags_addr;
	char *dtb_addr = NULL;
	ulong of_len = 0;

	if (NULL == fdt) {
		dtb_addr = (char* )IMAGE_DTB;
	} else {
		dtb_addr = *fdt;
	}

	while(tag->hdr.tag != ATAG_NONE) {
		atags_length += (tag->hdr.size << 2);

		tag = tag_next (tag);
	}

	of_len = swab16(readw((ulong)dtb_addr + 6)) + (ulong)dtb_addr;
	of_len = ALIGN(of_len, 4);
	memcpy((void *)of_len, atags_addr, atags_length + 8);

	printf("[atags] fdt=0x%p sz=0x%x\n",
		dtb_addr,
		swab16(readw((ulong)dtb_addr + 6)));

	if (of_len)
		return ;

	printf("error: invalid FDT or ATAGS argument... -hanging\n");
	hang();
}

void setup_atags(char **fdt)
{
	void *ptr = NULL;

	ptr = malloc(ATAGES_SIZE);
	if (ptr == NULL) {
		printf("error: malloc failed!\n");
		return ;
	}

	debug("[atags] fdt=0x%p\n", *fdt);

	fix_fdt_command_line(ptr, fdt);

	combine_fdt_atags(ptr, fdt);

	free(ptr);
}
#endif

