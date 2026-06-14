#include <common.h>
#include <adc.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/unaligned.h>
#include <dm/ofnode.h>
#include <linux/list.h>
#include <linux/ctype.h>
#include <nand.h>
#include <blk.h>
#include "resource_img.h"

DECLARE_GLOBAL_DATA_PTR;

#define PART_LOGO			    "logo"
#define RESOURCE_MAGIC			"RES!"
#define RESOURCE_MAGIC_SIZE		4
#define RESOURCE_VERSION		0
#define CONTENT_VERSION			0
#define ENTRY_TAG			    "ENTY"
#define ENTRY_TAG_SIZE			4
#define MAX_FILE_NAME_LEN		220
#define MAX_HASH_LEN			32

#if defined(CONFIG_UBOOT_XKPHOTO) || defined(CONFIG_UBOOT_CB1LD)
#define READ_CMD "sf read"
#define WRITE_CMD "sf write"
#define ERASE_CMD "sf erase"
#elif defined(CONFIG_UBOOT_ESL)
#define READ_CMD "nand read"
#define WRITE_CMD "nand write"
#define ERASE_CMD "nand erase"
#endif

#define LOGO_PAGE_SIZE 2048
/*
 *         resource image structure
 * ----------------------------------------------
 * |                                            |
 * |    header  (1 block)                       |
 * |                                            |
 * ---------------------------------------------|
 * |                      |                     |
 * |    entry0  (1 block) |                     |
 * |                      |                     |
 * ------------------------                     |
 * |                      |                     |
 * |    entry1  (1 block) | contents (n blocks) |
 * |                      |                     |
 * ------------------------                     |
 * |    ......            |                     |
 * ------------------------                     |
 * |                      |                     |
 * |    entryn  (1 block) |                     |
 * |                      |                     |
 * ----------------------------------------------
 * |                                            |
 * |    file0  (x blocks)                       |
 * |                                            |
 * ----------------------------------------------
 * |                                            |
 * |    file1  (y blocks)                       |
 * |                                            |
 * ----------------------------------------------
 * |                   ......                   |
 * |---------------------------------------------
 * |                                            |
 * |    filen  (z blocks)                       |
 * |                                            |
 * ----------------------------------------------
 */

/**
 * struct resource_image_header
 *
 * @magic: should be "RES!"
 * @version: resource image version, current is 0
 * @c_version: content version, current is 0
 * @blks: the size of the header
 * @c_offset: contents offset(by block) in the image
 * @e_blks: the size(by block) of the entry in the contents
 * @e_num: numbers of the entrys.
 */

struct resource_img_hdr {
	char		magic[4];
	uint16_t	version;
	uint16_t	c_version;
	uint8_t		blks;
	uint8_t		c_offset;
	uint8_t		e_blks;
	uint32_t	e_nums;
};

struct resource_entry {
	char		tag[4];
	char		name[MAX_FILE_NAME_LEN];
	char		hash[MAX_HASH_LEN];
	uint32_t	hash_size;
	uint32_t	f_offset;	/* Sector offset */
	uint32_t	f_size;		/* Bytes */
};

struct resource_file {
	char		name[MAX_FILE_NAME_LEN];
	char		hash[MAX_HASH_LEN];
	uint32_t	hash_size;
	uint32_t	f_offset;	/* Sector offset */
	uint32_t	f_size;		/* Bytes */
	struct list_head link;
	/* Sector base of resource when ram=false, byte base when ram=true */
	uint32_t	rsce_base;
	bool		ram;
};

static LIST_HEAD(entrys_head);

static int rsce_base;

int resource_image_check_header(void *rsce_hdr)
{
	struct resource_img_hdr *hdr = rsce_hdr;

	if (memcmp(RESOURCE_MAGIC, hdr->magic, RESOURCE_MAGIC_SIZE) != 0) {
		printf("bad resource image magics\n");
		return -EINVAL;
	}

	printf("resource image header:\n");
	printf("magic:%s\n", hdr->magic);
	printf("version:%d\n", hdr->version);
	printf("c_version:%d\n", hdr->c_version);
	printf("blks:%d\n", hdr->blks);
	printf("c_offset:%d\n", hdr->c_offset);
	printf("e_blks:%d\n", hdr->e_blks);
	printf("e_num:%d\n", hdr->e_nums);

	return 0;
}

static int add_file_to_list(struct resource_entry *entry,
			    int rsce_base, bool ram)
{
	struct resource_file *file;

	if (memcmp(entry->tag, ENTRY_TAG, ENTRY_TAG_SIZE)) {
		printf("invalid entry tag\n");
		return -ENOENT;
	}

	file = malloc(sizeof(*file));
	if (!file) {
		printf("out of memory\n");
		return -ENOMEM;
	}

	strcpy(file->name, entry->name);
	file->rsce_base = rsce_base;
	file->f_offset = entry->f_offset;
	file->f_size = entry->f_size;
	file->hash_size = entry->hash_size;
	file->ram = ram;
	memcpy(file->hash, entry->hash, entry->hash_size);
	list_add_tail(&file->link, &entrys_head);

	printf("entry: %p, %18s, base: 0x%08x, offset: 0x%08x, size: 0x%08x\n",
	      entry, file->name, file->rsce_base, file->f_offset, file->f_size);

	return 0;
}

static int resource_create_list(int rsce_base)
{
	struct resource_img_hdr *hdr;
	struct resource_entry *entry;
	int blknum, e_num;
	void *data = NULL;
	int ret = 0;
	int size;
	char cmd[50] = {0};

	hdr = memalign(ARCH_DMA_MINALIGN, LOGO_PAGE_SIZE);

	if (!hdr)
		return -ENOMEM;

	sprintf(cmd, "%s %p %x %x", READ_CMD, hdr, rsce_base, LOGO_PAGE_SIZE);
	printf("cmd: %s\n", cmd);
	run_command(cmd, 0);

	if (resource_image_check_header(hdr)) {
		ret = -EINVAL;
		goto err;
	}

	blknum = hdr->e_blks * hdr->e_nums;
	data = memalign(ARCH_DMA_MINALIGN, blknum * LOGO_PAGE_SIZE);
	if (!data) {
		ret = -ENOMEM;
		goto err;
	}

	sprintf(cmd, "%s %p %x %x", READ_CMD, data, rsce_base + LOGO_PAGE_SIZE, blknum * LOGO_PAGE_SIZE);
	printf("cmd: %s\n", cmd);
	run_command(cmd, 0);

	/*
	 * Add all file into resource file list, and load what we want from
	 * storage when we really need it.
	 */
	for (e_num = 0; e_num < hdr->e_nums; e_num++) {
		size = e_num * hdr->e_blks * LOGO_PAGE_SIZE;
		entry = (struct resource_entry *)(data + size);
		add_file_to_list(entry, rsce_base, false);
	}

err:
	if (data)
		free(data);
	if (hdr)
		free(hdr);

	return ret;
}

/*
 * Get the logo image base sector.
 *
 * The logo image base sector is stored in bootargs, like:
 * 1m@1m(logo)
 */
static int get_resource_base_sector(void)
{
    const char *bootargs = env_get("bootargs");

	if (bootargs) {
		const char *pos = strstr(bootargs, PART_LOGO);
		if (pos == NULL) {
			goto error;
		}

		const char *start = pos;
		while (start > bootargs && *start != '@') {
			start--;
		}

		if (*start == '@') {
			start++;
		}

		int num = 0;
		while (isdigit(*start)) {
			num = num * 10 + (*start - '0');
			start++;
		}

		char unit = *start;
		if (unit != 'k' && unit != 'm') {
			goto error;
		}

		if (unit == 'k') {
			num = num * 1024;
		} else if (unit == 'm') {
			num = num * 1024 * 1024;
		}

		printf("logo part offset: %d\n", num);
		return num;
	}

error:
	printf("failed to get logo part offset\n");
	return 0;
}

/*
 * There are: logo pictures in the logo image by default.
 *
 * This function does:
 *
 * 1. Get logo image base sector
 * 2. Create resource files list(addition: add logo pictures)
 * 3. Add dtb from android v2 dtb pos, override the old one from resource file
 */
static int init_resource_list(void)
{
	rsce_base = get_resource_base_sector();
	if (rsce_base > 0) {
		if (resource_create_list(rsce_base)) {
			printf("Failed to create resource list\n");
			return  -EINVAL;
		}
	}

    return 0;
}

static struct resource_file *get_file_info(const char *name)
{
	struct resource_file *file;
	struct list_head *node;

	if (list_empty(&entrys_head)) {
		if (init_resource_list())
			return NULL;
	}

	list_for_each(node, &entrys_head) {
		file = list_entry(node, struct resource_file, link);
		if (!strcmp(file->name, name))
			return file;
	}

	return NULL;
}

/*
 * read file from resource partition
 * @buf: destination buf to store file data;
 * @name: file name
 * @offset: blocks offset in the file
 * @len: the size(by bytes) of file to read.
 */
int mc_read_resource_file(void *buf, const char *name,
				int offset, int len)
{
	struct resource_file *file;
	char cmd[50] = {0};
	file = get_file_info(name);
	if (!file) {
		printf("No file: %s\n", name);
		return -ENOENT;
	}

	if (len <= 0 || len > file->f_size)
		len = file->f_size;

	sprintf(cmd, "%s %p %x %x", READ_CMD, buf, rsce_base + file->f_offset * LOGO_PAGE_SIZE + offset, file->f_size);
	printf("cmd: %s\n", cmd);
	run_command(cmd, 0);
	printf("read file: %s, len: %d\n", name, len);

	return len;
}
