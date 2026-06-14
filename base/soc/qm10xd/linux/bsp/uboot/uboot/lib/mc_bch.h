#ifndef __BCH_H_
#define __BCH_H_

struct spl_nand_header{
	unsigned int resever[24];
	unsigned int main_size;
 	unsigned int oob_size;
 	unsigned int ecc_mode;
 	unsigned int ecc_pos;
 	unsigned char  ecc[48];
 	unsigned int sector_num;
	unsigned int info_size;
 	unsigned int block_page;
	unsigned char resevered[340];
};

struct image_info {
	int ecc_strength;
	int ecc_step_size;
	int page_size;
	int oob_size;
	int usable_page_size;
	int eraseblock_size;
	int ctrl_num;
	long offset;
	char *ecc_pos;
	const char *source;
	const char *dest;
        int no_flag;
	/*spl header*/
	int spl_image_flag;
	int block_page;
	/*spl with uboot*/
	int spl_with_uboot;
	int spl_copys;

};
void encode_bch_24(struct image_info *info,const uint8_t *data,
                unsigned int len,  char *ecc, int no_flag);
void encode_bch_8(struct image_info *info,const uint8_t *data,
		unsigned int len,  char *ecc, int no_flag);



#endif
