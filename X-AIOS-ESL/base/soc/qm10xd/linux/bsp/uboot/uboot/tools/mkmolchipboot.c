/*
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * a simple tool to generate bootable image for sunxi platform.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

struct image_info {
	unsigned int magic_num;
	unsigned int addr;
	unsigned int length;
	unsigned int iv[4];
	unsigned int res;
	unsigned int rsvd[248];
};

int main(int argc, char *argv[])
{
	int fd_in, fd_out;

	unsigned file_size;
	int count = 0, ret;
	char *tool_name = argv[0];
	struct image_info info;
	unsigned char buf[4];

	if (argc < 3) {
		printf("This program converts an input binary file to a molchip bootable image.\n");
		printf("\nUsage: %s options input_file output_file\n",
		       tool_name);
		printf("Where options is the binary execute address\n");
		return EXIT_FAILURE;
	}

	fd_in = open(argv[2], O_RDONLY);
	if (fd_in < 0) {
		perror("Open input file");
		return EXIT_FAILURE;
	}

	/* get input file size */
	file_size = lseek(fd_in, 0, SEEK_END);

	memset(&info, 0, sizeof(info));
	/* fill the header */
	info.magic_num = 0x46524550;
//	sscanf(&info.addr, "0x%08x", &argv[1][3]);
	info.addr = strtol(argv[1], NULL, 16);
	info.length = file_size;
	fd_out = open(argv[3], O_WRONLY | O_CREAT, 0666);
	if (fd_out < 0) {
		perror("Open output file");
		return EXIT_FAILURE;
	}

	ret = write(fd_out, &info, le32_to_cpu(sizeof(info)));
	if (ret != le32_to_cpu(sizeof(info))) {
		perror("Writing output");
		return EXIT_FAILURE;
	}

	lseek(fd_in, 0, SEEK_SET);
	while(count < file_size) {
		ret = read(fd_in, buf, 1);
		if (ret != 1) {
			printf("Reading input image fail ret=%d\n", ret);
			return EXIT_FAILURE;
		}

		ret = write(fd_out, buf, 1);
		if (ret != 1) {
			printf("Writing output fail\n");
			return EXIT_FAILURE;
		}
		count += 1;
    }

	close(fd_in);
	close(fd_out);
	return EXIT_SUCCESS;
}
