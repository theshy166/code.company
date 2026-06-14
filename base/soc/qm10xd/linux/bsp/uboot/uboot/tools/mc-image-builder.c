#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "../lib/mc_bch.h"
#define DIV_ROUND_UP(x,y)  (((x) + ((y)-1)))/(y)

static int check_image_info(struct image_info *info)
{
	static int valid_ecc_strengths[] = {8,24};
	int eccbytes, eccsteps;
	unsigned i;

	if (!info->page_size) {
		fprintf(stderr, "--page is missing\n");
		return -EINVAL;
	}

	if (!info->oob_size) {
		fprintf(stderr, "--oob is missing\n");
		return -EINVAL;
	}

	if (!info->eraseblock_size) {
		fprintf(stderr, "--eraseblock is missing\n");
		return -EINVAL;
	}

	if (info->ecc_step_size != 512 && info->ecc_step_size != 1024) {
		fprintf(stderr, "Invalid ECC step argument: %d\n",
			info->ecc_step_size);
		return -EINVAL;
	}

	for (i = 0; i < 2; i++) {
		if (valid_ecc_strengths[i] == info->ecc_strength)
			break;
	}

	if (i == 2) {
		fprintf(stderr, "Invalid ECC strength argument: %d\n",
			info->ecc_strength);
		return -EINVAL;
	}
	eccbytes = DIV_ROUND_UP(info->ecc_strength * 14, 8);

	eccsteps = info->page_size / info->ecc_step_size;

	if (info->page_size + info->oob_size <
	    info->page_size + (eccsteps * eccbytes)) {
		fprintf(stderr,
			"ECC bytes do not fit in the NAND page, choose a weaker ECC\n");
		return -EINVAL;
	}
	return 0;
}

void spl_header_init(struct image_info *info,struct spl_nand_header *sheader)
{
	int steps = info->page_size / info->ecc_step_size;
	int eccbytes = DIV_ROUND_UP(info->ecc_strength * 14, 8);
	int ctrlbyte = info->oob_size/steps -eccbytes;
	sheader->main_size = info->page_size;
	sheader->oob_size = info->oob_size;
	if(info->ecc_strength == 8){
		sheader->ecc_mode = 0;
		sheader->info_size = (info->oob_size/(info->page_size>>10))-14;
	}
	else if(info->ecc_strength == 24){
		sheader->ecc_mode = 1;
		sheader->info_size = (info->oob_size/(info->page_size>>10))-42;
	}
	sheader->ecc_pos = ctrlbyte;
	//printf("ec_pos = %d\r\n",ctrlbyte);

	sheader->sector_num = info->page_size/info->ecc_step_size;
	//sheader->spare_size = sheader->ecc_pos;
	sheader->block_page = info->block_page;
}
static int spl_image_create(struct image_info *info,FILE *src,FILE *dst)
{
	uint32_t cnt;
	uint32_t page_num = 0;
	uint8_t *buffer;
	uint32_t fill_page_num;
	uint32_t i;
	struct spl_nand_header *sheader;
	long pos = ftell(dst);
	buffer = malloc(512);
	sheader = (struct spl_nand_header *)buffer;
	memset(buffer, 0xff, 512);
	cnt = fread(buffer, 1, 512, src);
	page_num++;
	if (!cnt) {
		if (!feof(src)) {
			fprintf(stderr,
				"Failed to read data from the source\n");
			return -1;
		} else {
			return 0;
		}
	}
	info->ctrl_num = 0;
	info->ecc_pos = (char *)sheader->ecc;
	encode_bch_24(info,buffer,512,info->ecc_pos,info->no_flag);
	/*init spl header*/
	spl_header_init(info,sheader);

	fwrite(buffer, 512 ,1, dst);
	fseek(dst, pos + 512 , SEEK_SET);
	while (!feof(src)) {
		memset(buffer, 0xff, 512);
		cnt = fread(buffer, 1, 512, src);
		if (cnt == 0) {
			if (feof(src)) {
				break;
			} else {
				fprintf(stderr, "Error reading from source file\n");
				return -1;
			}
		}
		pos = ftell(dst);
		fwrite(buffer, 512 ,1, dst);
		page_num++;
		/* Make dst pointer point to the next page. */
		fseek(dst, pos + 512 , SEEK_SET);
	}
	fill_page_num = 64*1024/512 -page_num;
	memset(buffer, 0xff, 512);
	for(i = 0;i<fill_page_num;i++){
		pos = ftell(dst);
		fwrite(buffer, 512 ,1, dst);
		page_num++;
		fseek(dst, pos + 512 , SEEK_SET);
	}
	return 0;
}

static int spl_with_uboot_image_create(struct image_info *info,FILE *src,FILE *dst){

	uint32_t cnt;
	uint8_t *buffer;
	uint32_t fill_page_num;
	uint32_t page_num = 0;
	uint32_t i;
	uint32_t copy;
//	struct spl_nand_header *sheader;
	long pos = ftell(dst);
	buffer = malloc(info->page_size);
//	sheader = (struct spl_nand_header *)buffer;


	for(copy = 0;copy<info->spl_copys;copy++){
		fseek(src, 0 , SEEK_SET);
		pos = ftell(dst);
		page_num = 0;
		memset(buffer, 0xff, info->page_size);
		cnt = fread(buffer, 1, info->page_size, src);
		if (!cnt) {
			if (!feof(src)) {
				fprintf(stderr,
					"Failed to read data from the source\n");
				return -1;
			} else {
				return 0;
			}
		}
		page_num++;
//		info->ctrl_num = 0;
		//info->ecc_pos = buffer +  0x30;
//		info->ecc_pos = (char *)sheader->ecc;
//		encode_bch_24(info,buffer,512,info->ecc_pos);
		/*init spl header*/
//		spl_header_init(info,sheader);
		fwrite(buffer, info->page_size ,1, dst);
		fseek(dst, pos + info->page_size , SEEK_SET);
		while (!feof(src)) {
			memset(buffer, 0xff, info->page_size);
			cnt = fread(buffer, 1, info->page_size, src);
			pos = ftell(dst);
			fwrite(buffer, info->page_size ,1, dst);
			page_num++;
			/* Make dst pointer point to the next page. */
			fseek(dst, pos + info->page_size , SEEK_SET);
			if(page_num*info->page_size == 32*1024)
				break;
		}
		fill_page_num = info->eraseblock_size/info->page_size -page_num;
		memset(buffer, 0xff, info->page_size);
		for(i = 0;i<fill_page_num;i++){
			pos = ftell(dst);
			fwrite(buffer, info->page_size ,1, dst);
			fseek(dst, pos + info->page_size, SEEK_SET);
		}
	}
	while (!feof(src)) {
		memset(buffer, 0xff, info->page_size);
		cnt = fread(buffer, 1, info->page_size, src);
		pos = ftell(dst);
		fwrite(buffer, info->page_size ,1, dst);
		/* Make dst pointer point to the next page. */
		fseek(dst, pos + info->page_size , SEEK_SET);
	}
	return 0;

}
static int write_page(struct image_info *info, uint8_t *buffer,
		      FILE *src, FILE *dst,
		       int page)
{

	int steps = info->page_size / info->ecc_step_size;
	int eccbytes = DIV_ROUND_UP(info->ecc_strength * 14, 8);
	int ctrlbyte = info->oob_size/steps -eccbytes;
	long pos = ftell(dst);
	size_t cnt;
	int i;

	memset(buffer, 0xff, info->page_size + info->oob_size);
	cnt = fread(buffer, 1, info->page_size, src);
	if (!cnt) {
		if (!feof(src)) {
			fprintf(stderr,
				"Failed to read data from the source\n");
			return -1;
		} else {
			return 0;
		}
	}
	info->ctrl_num = ctrlbyte;
	for(i=0;i<steps;i++)
	{
		// calu ecc_pos
		info->ecc_pos = (char *)(buffer + info->page_size + (info->oob_size/steps)*(i+1) - eccbytes);
		//calu ecc
		if(info->ecc_strength == 24)
			encode_bch_24(info,buffer + i*info->ecc_step_size,info->ecc_step_size ,info->ecc_pos,info->no_flag);
		else if(info->ecc_strength == 8)
			encode_bch_8(info,buffer + i*info->ecc_step_size,info->ecc_step_size ,info->ecc_pos,info->no_flag);
	}
	fwrite(buffer, info->page_size + info->oob_size, 1, dst);
	/* Make dst pointer point to the next page. */
	fseek(dst, pos + info->page_size + info->oob_size, SEEK_SET);
	return 0;
}
static int create_image(struct image_info *info)
{
	long page = 0;
	FILE  *src,*dst;
	uint8_t *buffer;

	buffer = malloc(info->page_size + info->oob_size);
	if (!buffer) {
		fprintf(stderr, "Failed to allocate the NAND page buffer\n");
		return -1;
	}
	memset(buffer, 0xff, info->page_size + info->oob_size);

	src = fopen(info->source, "r");
	if (!src) {
		fprintf(stderr, "Failed to open source file (%s)\n",
			info->source);
		return -1;
	}

	dst = fopen(info->dest, "w");
	if (!dst) {
		fprintf(stderr, "Failed to open dest file (%s)\n", info->dest);
		return -1;
	}

	if(info->spl_image_flag)
	{
		spl_image_create(info,src,dst);

	}else if(info->spl_with_uboot){
		spl_with_uboot_image_create(info,src,dst);
	}else{

		while (!feof(src)) {
			int ret;

			ret = write_page(info, buffer, src, dst, page++);
			if (ret)
				return ret;
		}

	}
	return 0;

}

static void display_help(int status)
{

	printf(
		"Usage: mc-image-builder [OPTIONS] source-image output-image\n"
		"\n"
		"Creates a raw NAND image that can be read by the molchip NAND controller.\n"
		"\n"
		"-h               --help               Display this help and exit\n"
		"-c <str>/<step>  --ecc=<str>/<step>   ECC config (strength/step-size)\n"
		"-p <size>        --page=<size>        Page size\n"
		"-o <size>        --oob=<size>         OOB size\n"
		"-e <size>        --erase=<size>       erase size\n"
		"-n <number>      --pages=<number>     pages per block\n"
		"-d <number>      -duplication-=<number>     duplication spl\n"
		"-w      	  --spl with uboot     spl with uboot \n"
		"-s        	  --spl                generate spl header ecc\n"
	        "-f        	  --spl                generate spl header ecc\n");

	exit(status);
}

int main(int argc, char **argv)
{
	struct image_info info;
	memset(&info, 0, sizeof(info));

	for(;;){

		int option_index = 0;
		char *endptr = NULL;
		static const struct option long_options[] = {
			{"ecc", required_argument, 0, 'c'},
			{"page", required_argument, 0, 'p'},
			{"oob", required_argument, 0, 'o'},
			{"eraseblock", required_argument, 0, 'e'},
			{0, 0, 0, 0},
		};

		int c = getopt_long(argc, argv, "c:p:o:e:n:d:shwf",
				long_options, &option_index);
		if (c == EOF)
			break;
		switch (c) {
			case 'c':
				info.ecc_strength = strtol(optarg, &endptr, 0);
				if (*endptr == '/')
					info.ecc_step_size = strtol(endptr + 1, NULL, 0);
				break;
			case 'p':
				info.page_size = strtol(optarg, NULL, 0);
				break;
			case 'o':
				info.oob_size = strtol(optarg, NULL, 0);
				break;
			case 'e':
				info.eraseblock_size = strtol(optarg, NULL, 0);
				break;
		 	case 's':
				info.spl_image_flag = 1;
				break;
		 	case 'w':
				info.spl_with_uboot = 1;
				break;
		 	case 'd':
				info.spl_copys = strtol(optarg, NULL, 0);
				break;
			case 'h':
				display_help(-1);
				break;
			case 'n':
				info.block_page = strtol(optarg,NULL,0);
				break;
			case 'f':
				info.no_flag = 1;
				break;
			default:
			   	break;
		}

	}

	if ((argc - optind) != 2)
		display_help(-1);
	info.source = argv[optind];
	info.dest = argv[optind + 1];

	printf("ecc_strength = %d,ecc_step_size = %d,eraseblokc_size = %x,page_size = %d,oob_size=%d\r\n",info.ecc_strength,info.ecc_step_size,info.eraseblock_size,info.page_size,info.oob_size);
	check_image_info(&info);
	return create_image(&info);

}
