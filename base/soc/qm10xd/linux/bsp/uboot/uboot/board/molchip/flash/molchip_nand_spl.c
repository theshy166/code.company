#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/spic.h>
#include <mach/uart_drv.h>
#include <mach/spl.h>
#include <spi_flash.h>
#include <mach/nandflash.h>
#include <asm/bitops.h>
#include "../board/system.h"
#include "../board/cpu_sched.h"
#include "../board/sched_internal.h"
#include "../board/secure.h"

struct spi_nand_flash *chip;
struct spi_nand_flash nand_chip;
u8 page_buf[4096+256];

struct spi_nand_flash spi_nand_table[] = {
	{
		.name = "XT26G01",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len= 2,
			},
		.dev_id = {0x0B, 0xe1},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_QUAD,
		.ecc_mask = 7,
		.ecc_uncorr = 7,
	},

	{
		.name = "w25n01gv",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len= 3,
			},
		.dev_id = {0xEF, 0xAA, 0x21},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD,
		.ecc_mask =3,
		.ecc_uncorr = 2,
	},

	{
		.name = "MX35F1G",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC2, 0x12},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_DUAL,
		.ecc_mask = 7,
		.ecc_uncorr = 7,
	},

	{
		.name = "W25N02KVxxIRU",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 3,
		},
	.dev_id = {0xef, 0xaa, 0x22},
	.page_size = 2048,
	.page_spare_size = 128,
	.pages_per_blk = 64,
	.blks_per_chip = 2048,
	.options = SPINAND_ECC_MODE_24_bit | SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD,
	.ecc_mask = 3,
	.ecc_uncorr = 2,
	},

	{
		.name = "DS35X1GAXXX",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
	.dev_id = {0xe5, 0x71},
	.page_size = 2048,
	.page_spare_size = 64,
	.pages_per_blk = 64,
	.blks_per_chip = 1024,
	.options = SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD,
	.ecc_mask = 3,
	.ecc_uncorr = 2,
	},

	{
		.name = "MX35LF2GE4AB",
		.id_info = {
			.id_addr = 0,
			.id_len = 2,
		},
		.dev_id = {0xC2, 0x22},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 2048,
		.options = SPINAND_NEED_PLANE_SELECT,
		.ecc_mask = 3,
		.ecc_uncorr = 2,
	},

	{
		.name = "MX35LF1G24AD",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC2, 0x14},
		.page_size = 2048,
		.page_spare_size = 128,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_ECC_MODE_24_bit | SPINAND_SUPPORT_DUAL,
		.ecc_mask = 7,
		.ecc_uncorr = 7,
	},

	{
		.name = "MX35LF4G24AD",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC2, 0x35},
		.page_size = 4096,
		.page_spare_size = 128,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options =  SPINAND_SUPPORT_QUAD,
		.ecc_mask = 7,
		.ecc_uncorr = 7,
	},

	{
		.name = "MT29F1G01ABAFD",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0x2C, 0x14},
		.page_size = 2048,
		.page_spare_size = 128,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_DUAL,
		.ecc_mask = 3,
		.ecc_uncorr = 2,
	},
	{
		.name = "TC58CVG0S3",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0x98, 0xE2},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_DUAL,
		.ecc_mask = 3,
		.ecc_uncorr = 2,
	},

#if defined(CONFIG_TARGET_FY02) || defined(CONFIG_TARGET_FY02B) || defined(CONFIG_TARGET_FY10P) || defined(CONFIG_TARGET_FY02P)
	{
		.name = "GD5F1GQ4U",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC8, 0xD1},
		.page_size = 2048,
		.page_spare_size = 128,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_ECC_MODE_24_bit | SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD ,
	 },
#else
	{
		.name = "GD5F1GQ4U",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC8, 0xD1},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options =  SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD ,
	 },
#endif
	{
		.name = "GD5F1GQ5X",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC8, 0x51},
		.page_size = 2048,
		.page_spare_size = 128,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options =  SPINAND_ECC_MODE_24_bit | SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD ,
	 },

	 {
		.name = "GD5F2GQ4U",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xC8, 0xB2},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 2048,
		.options = SPINAND_SUPPORT_DUAL,
	},
	{
		.name = "HYF1GQ4UDACAE",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
				.id_len = 2,
		},
		.dev_id = {0xC9, 0x21},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 2048,
		.options = SPINAND_SUPPORT_DUAL,
	},

	{
		.name = "DS35X1GD",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xe5, 0x51},
		.page_size = 2048,
		.page_spare_size = 128,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_DUAL | SPINAND_ECC_MODE_24_bit,
	},
	{
                .name = "MX35LF2GE4AC",
                .id_info = {
                        .id_addr = SPI_NAND_ID_NO_DUMMY,
                        .id_len = 3,
                },
                .dev_id = {0xC2, 0x26, 0x01},
                .page_size = 2048,
                .page_spare_size = 64,
                .pages_per_blk = 64,
                .blks_per_chip = 2048,
                .options = SPINAND_SUPPORT_QUAD,
        },

	{
                .name = "F35SQA001G",
                .id_info = {
                        .id_addr = SPI_NAND_ID_NO_DUMMY,
                        .id_len = 3,
                },
                .dev_id = {0xCD, 0x71, 0x71},
                .page_size = 2048,
                .page_spare_size = 64,
                .pages_per_blk = 64,
                .blks_per_chip = 1024,
                .options = SPINAND_SUPPORT_QUAD,
        },
	{
                .name = "FM25S01A",
                .id_info = {
                        .id_addr = SPI_NAND_ID_NO_DUMMY,
                        .id_len = 2,
                },
                .dev_id = {0xA1, 0xD4},
                .page_size = 2048,
                .page_spare_size = 64,
                .pages_per_blk = 64,
                .blks_per_chip = 1024,
                .options = SPINAND_SUPPORT_QUAD,
        },
	{
		.name = "UM19A0HISW 3.3V SPI",
		.id_info = {
			.id_addr = SPI_NAND_ID_NO_DUMMY,
			.id_len = 2,
		},
		.dev_id = {0xb0, 0x14},
		.page_size = 2048,
		.page_spare_size = 64,
		.pages_per_blk = 64,
		.blks_per_chip = 1024,
		.options = SPINAND_SUPPORT_DUAL | SPINAND_SUPPORT_QUAD,
	},
	{
                .name = "XT26G01CWSIG",
                .id_info = {
                        .id_addr = SPI_NAND_ID_NO_DUMMY,
                        .id_len= 2,
                        },
                .dev_id = {0x0B, 0x11},
                .page_size = 2048,
                .page_spare_size = 64,
                .pages_per_blk = 64,
                .blks_per_chip = 1024,
                .options = SPINAND_SUPPORT_QUAD,
                .ecc_mask = 7,
                .ecc_uncorr = 7,
        },
};

int ilog2(unsigned long v)
{
	int i = 0;

	while((1UL << i) < v)
		i++;
	return i;
}

static int spi_nand_check_erased_buf(void *buf, int len, int bitflips_threshold)
{
	const u8 *bitmap = buf;
	int bitflips = 0;
	int weight;

	for (; len && ((uintptr_t)bitmap) % sizeof(long);
	     len--, bitmap++) {
		weight = hweight8(*bitmap);
		bitflips += 8 - weight;
		if (unlikely(bitflips > bitflips_threshold))
			return -EBADMSG;
	}

	for (; len >= 4; len -= 4, bitmap += 4) {
		weight = hweight32(*((u32 *)bitmap));
		bitflips += 32 - weight;
		if (unlikely(bitflips > bitflips_threshold))
			return -EBADMSG;
	}

	for (; len > 0; len--, bitmap++) {
		weight = hweight8(*bitmap);
		bitflips += 8 - weight;
		if (unlikely(bitflips > bitflips_threshold))
			return -EBADMSG;
	}
	return bitflips;
}

static int nand_check_ecc_chunk(void *data, int datalen, void *oob,
					int ooblen, int bitflips_threshold)
{
	int data_bitflips = 0, oob_bitflips = 0;
	int bit_corrected = 0;

	data_bitflips = spi_nand_check_erased_buf(data, datalen, bitflips_threshold);
	if (data_bitflips < 0) {
		printf("data bitflips exceed max thresholds %d\n ", bitflips_threshold);
		return data_bitflips;
	}

	bitflips_threshold -= data_bitflips;
	bit_corrected += data_bitflips;

	oob_bitflips = spi_nand_check_erased_buf(oob, ooblen, bitflips_threshold);
	if (oob_bitflips < 0) {
		printf("oob bitflips exceed max thresholds %d\n ", bitflips_threshold);
		return oob_bitflips;
	}
	bitflips_threshold -= oob_bitflips;
	bit_corrected += oob_bitflips;

	if (data_bitflips)
		memset(data, 0xff, datalen);

	if (oob_bitflips)
		memset(oob, 0xff, ooblen);

	return bit_corrected;
}

static int spi_nand_send_cmd(struct spi_nand_cmd *cmd)
{
	u32 data_size = 0;
	u32 main_size = 0;
	u32 spare_size = 0;
	u32 count = 0;
	
	sfc_all_clr();
	sfc_cmd(cmd->cmd);

	switch(cmd->cmd){
		case SPINAND_CMD_READ:
		case SPINAND_CMD_PROG:
		case SPINAND_CMD_ERASE_BLK:
			nfc_fb_incdec_mode(FB_INCDEC_WRITE);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR, cmd->tx_nbits);
			break;
		case SPINAND_CMD_READ_RDM:
		case SPINAND_CMD_READ_CACHE_X2:
		case SPINAND_CMD_READ_CACHE_X4:
			nfc_fb_incdec_mode(FB_INCDEC_READ);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR_DATA, cmd->rx_nbits);
			break;
		case SPINAND_CMD_PROG_LOAD:
			nfc_fb_incdec_mode(FB_INCDEC_WRITE);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_2ADDR, VLD_CMD_ADDR_DATA, cmd->tx_nbits);
			break;
		case SPINAND_CMD_WR_ENABLE:
		case SPINAND_CMD_RESET:
			nfc_fb_incdec_mode(FB_INCDEC_WRITE);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD, cmd->tx_nbits);
			break;
		case SPINAND_CMD_READ_ID:
			nfc_fb_incdec_mode(FB_INCDEC_READ);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_1ADDR, VLD_CMD_ADDR_DATA, cmd->rx_nbits);
			break;
		case SPINAND_CMD_READ_REG:
			nfc_fb_incdec_mode(FB_INCDEC_READ);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_1ADDR, VLD_CMD_ADDR_REG, cmd->rx_nbits);
			break;
		case SPINAND_CMD_WRITE_REG:
			nfc_fb_incdec_mode(FB_INCDEC_WRITE);
			sfc_dev_mode_reg_set(FRMFORM_OPCODE_1ADDR, VLD_CMD_ADDR_REG, cmd->tx_nbits);
			break;
		default:
			break;
	}
	
	nfc_flowbuf_clr();
	
	if(cmd->n_addr){
		sfc_addr(cmd->addr, cmd->n_addr);
	}
	if(cmd->n_rx){
		sfc_cmd_read();
		data_size = cmd->n_rx;
	}else{
		sfc_cmd_write();
		data_size = cmd->n_tx;
	}

	if(data_size){
		main_size = data_size & 0xfffffe00;
		spare_size = data_size & 0x1ff;
		if(!main_size){
			main_size = spare_size;
			spare_size = 0;
		}
	}

	if(cmd->ecc_enable == true){
		sfc_ecc_clr();
		if(cmd->n_rx){
			sfc_ecc_enc_mode(ECC_DECODE);
		}else{
			sfc_ecc_enc_mode(ECC_ENCODE);
		}
		sfc_ecc_mode(cmd->ecc_mode);
		if(cmd->ecc_mode == ECC_8BIT){
			sfc_ecc_pos(0x12);
			sfc_spare_ecc_byte(0x12);
		}else{
			sfc_ecc_pos(0x16);
			sfc_spare_ecc_byte(0x16);
		}
		sfc_ecc_page_size(main_size);
		sfc_ecc_enable(true);
	}
	
	if(main_size){
		sfc_data_length(main_size);
		if(cmd->n_tx){
			if(main_size == 1){
				sfc_write_reg(*(cmd->tx_buf));
			}else{
				sfc_write_data(cmd->tx_buf, main_size);
			}
		}
		if(spare_size){
			count = main_size >> 10;
			sfc_spare_length(spare_size);
			sfc_spare_len_1k(spare_size/count);
			if(cmd->n_tx) {
				sfc_wait_fbuf2_ready();
				sfc_write_spare(cmd->tx_buf + main_size, spare_size, count);
				sfc_fbuf2_ack();
				//sfc_dma_write_spare(cmd->tx_buf + main_size, spare_size);
			}
			sfc_data_type(DATA_WITH_SPARE);
		}else if(main_size == 1){
			sfc_data_type(DATA_NONE);
		}else{
			sfc_data_type(DATA_ONLY);
		}
		
		/*
		if(cmd->n_tx){
			if(main_size == 1){
				sfc_write_reg(*(cmd->tx_buf));
			}else{
				sfc_write_data(cmd->tx_buf, main_size);
				//sfc_dma_write_data(cmd->tx_buf, main_size);
			}
		}

		if(cmd->n_rx){
			if(main_size > 1){
				sfc_dma_read_data(cmd->rx_buf, main_size);
				if(spare_size){
					sfc_dma_read_spare(&page_buf[4096], 256);
				}
			}
		}*/
	}
	
	sfc_start_exec(SFC_DMA_NONE);
	
	if(cmd->n_rx){
		if(main_size == 1){
			*(cmd->rx_buf) = sfc_read_reg();
		}else{
			sfc_wait_fbuf2_ready();
			sfc_read_data(cmd->rx_buf, main_size);
			if(spare_size){
				sfc_read_spare(&page_buf[chip->page_size], spare_size, count);
			}
			sfc_fbuf2_ack();
			//sfc_deal_spare(&page_buf[4096], &page_buf[4096], spare_size, count);
		}
	}

	/*if(sfc_tran_mode == SFC_DMA_RX){
		sfc_dma_rx0_stop();
		sfc_dma_rx1_stop();
	}else if(sfc_tran_mode == SFC_DMA_TX){
		sfc_dma_tx0_stop();
		//sfc_dma_tx1_stop();
	}else if(sfc_tran_mode == SFC_DMA_RX_ID){
		sfc_dma_rx0_stop();
	}*/
	
	return 0;
}

static int spi_nand_read_status(u8 *status)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_STATUS;
	cmd.n_rx = 1;
	cmd.rx_buf = status;

	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

static int spi_nand_wait(u8 *status)
{
	unsigned long timeo = 0x400000;

	while(timeo --) {
		spi_nand_read_status(status);
		if((*status & STATUS_OIP_MASK) == STATUS_READY){
			return 0;
		}
	}
	if(timeo == 0)return -1;
	return 0;
}

/*
 * spi_nand_reset- send RESET command "0xff" to the SPI-NAND.
 * @chip: SPI-NAND device structure
 */
static int spi_nand_reset(void)
{
	struct spi_nand_cmd cmd = {0};
	u8 status = 0;

	cmd.cmd = SPINAND_CMD_RESET;
	spi_nand_send_cmd(&cmd);
	/* elapse 1ms before issuing any other command */
	if(spi_nand_wait(&status))
		return ETIMEDOUT;
	return 0;
}


static int spi_nand_write_enable(void)\
{
	struct spi_nand_cmd cmd = {0};
	int ret = 0;
	cmd.cmd = SPINAND_CMD_WR_ENABLE;
	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

static int spi_nand_read_page_to_cache(u32 page_addr)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = (u8)(page_addr);
	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

int get_ecc_sts_with_check_ecc_chunk(u32 num, void *data, int datalen, int ecc_mode)
{
	u32 val,status;
	u32 i;

	void *data_temp, *oob_temp;
	int datalen_temp, ooblen_temp;
	int ret = 0 ;
	int bitflips_threshold;
	if(num > 4)
		return -EINVAL;
	val = spi_readl(REG_NFC_ECC_INFO0);
	//sfc_read_spare(&page_buf[4096], chip->page_spare_size, chip->page_size>>10);
	for(i = 0; i < num; i++) {
		status = (val >> (i * 2)) & 0x3;
		if(status == CORRECT_FAIL){
			data_temp = data + (i << 10);
			datalen_temp = 1024;
			ooblen_temp = chip->page_spare_size / num;
			oob_temp = &page_buf[4096 + ooblen_temp*i] ;
			if(ecc_mode == ECC_24BIT)
				bitflips_threshold = 24;
			else
				bitflips_threshold = 8;
			ret =  nand_check_ecc_chunk(data_temp, datalen_temp, oob_temp, ooblen_temp, bitflips_threshold);
			if(ret < 0)
				return ret;
		}
	}
	return ret;
}

static int spi_nand_read_from_cache(u32 page_addr, u32 column, u32 len, u8 *rbuf)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ_RDM;
	cmd.n_addr = 3;

	cmd.addr[0] = (u8)(column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
			cmd.addr[0] |= (u8)(((page_addr >>
					      (chip->block_shift - chip->page_shift)) & 0x1) << 4);
	cmd.addr[1] = (u8)(column);
	cmd.addr[2] = 0;

	cmd.n_rx = len;
	cmd.rx_nbits = WIRE_MODE_SPI;
	cmd.rx_buf = rbuf;

	cmd.ecc_enable = true;
	if(chip->options & SPINAND_ECC_MODE_24_bit)
		cmd.ecc_mode = ECC_24BIT;
	else
		cmd.ecc_mode = ECC_8BIT;
	spi_nand_send_cmd(&cmd);
	ret = get_ecc_sts_with_check_ecc_chunk(chip->page_size>>10, rbuf, len, cmd.ecc_mode);
	if(ret < 0 ){
		printf("page %x bitflips exceed the ecc strength\n", page_addr);
	}
	return ret;
}

static int spi_nand_read_from_cache_x2(u32 page_addr, u32 column, u32 len, u8 *rbuf)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ_CACHE_X2;
	cmd.n_addr = 3;

	cmd.addr[0] = (u8)(column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
			cmd.addr[0] |= (u8)(((page_addr >>
					      (chip->block_shift - chip->page_shift)) & 0x1) << 4);
	cmd.addr[1] = (u8)(column);
	cmd.addr[2] = 0;

	cmd.n_rx = len;
	cmd.rx_nbits = WIRE_MODE_DPI;
	cmd.rx_buf = rbuf;

	cmd.ecc_enable = true;
	if(chip->options & SPINAND_ECC_MODE_24_bit)
		cmd.ecc_mode = ECC_24BIT;
	else
		cmd.ecc_mode = ECC_8BIT;
	spi_nand_send_cmd(&cmd);
	ret = get_ecc_sts_with_check_ecc_chunk(chip->page_size>>10, rbuf, len, cmd.ecc_mode);
	if(ret < 0 ){
		printf("page %x bitflips exceed the ecc strength\n", page_addr);
	}
	return ret;
}


static int spi_nand_read_from_cache_x4(u32 page_addr, u32 column, u32 len, u8 *rbuf)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ_CACHE_X4;
	cmd.n_addr = 3;

	cmd.addr[0] = (u8)(column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
			cmd.addr[0] |= (u8)(((page_addr >>
					      (chip->block_shift - chip->page_shift)) & 0x1) << 4);
	cmd.addr[1] = (u8)(column);
	cmd.addr[2] = 0;

	cmd.n_rx = len;
	cmd.rx_nbits = WIRE_MODE_QPI;
	cmd.rx_buf = rbuf;

	cmd.ecc_enable = true;
	if(chip->options & SPINAND_ECC_MODE_24_bit) cmd.ecc_mode = ECC_24BIT;
	else cmd.ecc_mode = ECC_8BIT;
	spi_nand_send_cmd(&cmd);
	ret = get_ecc_sts_with_check_ecc_chunk(chip->page_size>>10, rbuf, len, cmd.ecc_mode);
	if(ret < 0 ){
		printf("page %x bitflips exceed the ecc strength\n", page_addr);
	}
	return ret;
}

static int spi_nand_read_id(u8 *id)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ_ID;
	cmd.n_addr = 1;
	cmd.rx_buf = id;
	cmd.n_rx =4;
	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

static int spi_nand_scan_id_table(u8 *id)
{
	int i,j =0;
	int m=0;
	struct spi_nand_flash *type = spi_nand_table;

	if(id[0] == 0 && id[1] == 0){
		return false;
	}
	for(m=0; m<ARRAY_SIZE(spi_nand_table);m++,type++){
		for(j=0, i = (SPI_NAND_ID_NO_DUMMY == type->id_info.id_addr) ? 0:1;
		j < type->id_info.id_len;j++,i++){
			if (id[i] != type->dev_id[j])
				break;
		}
		if(j == type->id_info.id_len){
			memcpy(chip, type, sizeof(struct spi_nand_flash));
			chip->block_size = type->page_size * type->pages_per_blk;
			chip->block_shift = ilog2(chip->block_size);
			chip->page_shift = ilog2(chip->page_size);
			mc_printf("SPINAND : %s is found \r\n", chip->name);
			return 0;
		}
	}
	mc_printf("SPINAND is not in id table\r\n");
	return -1;
}

int spi_nand_scan(struct spi_flash *nand)
{
	u8 id[SPINAND_MAX_ID_LEN] = {0};
	chip = &nand_chip;
	spi_nand_read_id(id);
	if(!spi_nand_scan_id_table(id)){
		nand->name = chip->name;
		nand->size = chip->blks_per_chip * chip->pages_per_blk * chip->page_size;
		nand->page_size = chip->page_size;
	}
	return 0;
}

static int spi_nand_get_otp(u8 *otp)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_READ_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_OTP;
	cmd.n_rx = 1;
	cmd.rx_buf = otp;
	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

static int spi_nand_set_otp(u8 *otp)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_WRITE_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_OTP;
	cmd.tx_buf = otp;
	cmd.n_tx = 1;
	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

static int spi_nand_enable_ecc(void)
{
	int ret;
	u8 otp = 0;

	ret = spi_nand_get_otp(&otp);
	if (ret < 0)
		return ret;

	if ((otp & OTP_ECC_MASK) == OTP_ECC_ENABLE)
		return 0;

	otp |= OTP_ECC_ENABLE;
	ret = spi_nand_set_otp(&otp);
	if (ret < 0)
		return ret;
	return spi_nand_get_otp(&otp);
}

int spi_nand_disable_ecc(void)
{
	u8 otp = 0;

	spi_nand_get_otp(&otp);

	if ((otp & OTP_ECC_MASK) == OTP_ECC_ENABLE) {
		otp &= ~OTP_ECC_ENABLE;
		otp |= (1<<3);
		spi_nand_set_otp(&otp);
		spi_nand_get_otp(&otp);
		if ((otp & OTP_ECC_MASK) == OTP_ECC_ENABLE)
			return -1;
		return 0;
	} else
		return 0;
}

static int spi_nand_enable_quad(void)
{
	u8 otp;

	spi_nand_get_otp(&otp);
	otp |= OTP_QUAD_ENABLE;
	spi_nand_set_otp(&otp);
	return spi_nand_get_otp(&otp);
}

static int spi_nand_program_data_to_cache(u32 page_addr, u32 column, u32 len, u8 *wbuf)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_PROG_LOAD;
	cmd.n_addr = 2;
	cmd.addr[0] = (u8)(column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
		cmd.addr[0] |= (u8)(((page_addr >>
				      (chip->block_shift - chip->page_shift)) & 0x1) << 4);
	cmd.addr[1] = (u8)column;
	cmd.n_tx = len;
	cmd.tx_buf = wbuf;
	cmd.ecc_enable = true;
	if(chip->options & SPINAND_ECC_MODE_24_bit) cmd.ecc_mode = ECC_24BIT;
	else cmd.ecc_mode = ECC_8BIT;

	ret = spi_nand_send_cmd(&cmd);

	return ret;
}

static int spi_nand_execute(u32 page_addr)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_PROG;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = (u8)page_addr;

	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

static int spi_nand_erase_blk(u32 page_addr)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_ERASE_BLK;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = page_addr;
	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

/**
 * spi_nand_lock_block- send write register 0x1f command to the lock/unlock device
 * @spi: spi device structure
 * @lock: value to set to block lock register
 *
 * Description:
 *    After power up, all the Nand blocks are locked.  This function allows
 *    one to unlock the blocks, and so it can be written or erased.
 */
static int spi_nand_lock_block(u8 lock)
{
	struct spi_nand_cmd cmd = {0};
	int ret;

	cmd.cmd = SPINAND_CMD_WRITE_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_BLOCK_LOCK;
	cmd.n_tx = 1;
	cmd.tx_buf = &lock;

	ret = spi_nand_send_cmd(&cmd);
	return ret;
}

/*void spi_nand_calculate_ecc(void)
{
	sfc_all_clr();
	sfc_data_length(512);
	sfc_spare_length(0);
	sfc_ecc_mode(ECC_24BIT);
	sfc_ecc_pos(0x0);//0x16
	sfc_ecc_enc_mode();
	sfc_access_start();
}*/

/**
 * spi_nand_do_write_page - [INTERN] write data from buffer to flash
 * @page_addr: page address/raw address
 * @column :column address
 * @raw: without ecc or not
 *
 * write data from buffer pointed by chip->buf to flash
 */

static int spi_nand_do_write_page(u8 *buffer, u32 page_addr, u32 column, bool raw)
{
	u8 status;
	bool p_fail = false;
	bool p_timeout = false;
	int ret = 0;

	if (raw) {
		ret = spi_nand_disable_ecc();
		if (ret < 0) {
			return ret;
		}
	}

	ret = spi_nand_write_enable();
	if (ret < 0) {
		return ret;
	}
	/* Store the page to cache */

	ret = spi_nand_program_data_to_cache(page_addr, column,
		chip->page_size + chip->page_spare_size - column,
		buffer);

	if (ret < 0) {
		return ret;
	}
	/* Get page from the device cache into our internal buffer */

	ret = spi_nand_execute(page_addr);
	if (ret < 0) {
		return ret;
	}

	ret = spi_nand_wait(&status);
	if (ret < 0) {
		return ret;
	}

	if ((status & STATUS_P_FAIL_MASK) == STATUS_P_FAIL) {
		p_fail = true;
	}

	if ((status & STATUS_OIP_MASK) == STATUS_BUSY) {
			p_timeout = true;
		}

	if (raw) {
		ret =spi_nand_enable_ecc();
		if (ret < 0) {
			return ret;
		}
	}
	if ((p_fail==true)||(p_timeout==true))
		ret = -EIO;

	return ret;
}

/**
 * spi_nand_do_read_page - [INTERN] read data from flash to buffer
 * @mtd: MTD device structure
 * @page_addr: page address/raw address
 * @column :column address
 * @raw: without ecc or not
 * @corrected: how many bit error corrected
 *
 * read a page to buffer pointed by chip->buf
 */
static int spi_nand_do_read_page(u8 *buffer, u32 page_addr,
				   u32 column, bool raw, u32 *corrected)
{
	int ret;
	u8 status;

	/*read data from chip*/
	memset(buffer, 0, chip->page_size );
	if (raw) {
		ret = spi_nand_disable_ecc();
		if (ret < 0) {
			return ret;
		}
	}

	ret = spi_nand_read_page_to_cache(page_addr);
	if (ret < 0) {
		return ret;
	}

	ret = spi_nand_wait(&status);
	if (ret < 0) {
		return ret;
	}
	/* Get page from the device cache into our internal buffer */
	if ((status & STATUS_ECC_SUCCESS_MASK) != STATUS_ECC_SUCCESS) {
	}

	if(chip->options & SPINAND_SUPPORT_QUAD){
		spi_nand_enable_quad();
		ret = spi_nand_read_from_cache_x4(page_addr, column,
				chip->page_size + chip->page_spare_size - column,
				buffer);
	}
	else if(chip->options & SPINAND_SUPPORT_DUAL){
		ret = spi_nand_read_from_cache_x2(page_addr, column,
				chip->page_size + chip->page_spare_size - column,
				buffer);
	}
	else{
		ret = spi_nand_read_from_cache(page_addr, column,
				chip->page_size + chip->page_spare_size - column,
				buffer);
	}

	if (raw) {
		ret =spi_nand_enable_ecc();
		if (ret < 0) {
			return ret;
		}
	}

	return ret;
}

int spi_nand_read_data(u8 *read_buf,u32 from, u32 len)
{
	u32 corrected;
	u32 page_shift;
	u32 start_page;
	u8 status;
	int ret;

	page_shift = ilog2(chip->page_size);
	start_page = from>>page_shift;
	while(len){
		if(!(start_page % chip->pages_per_blk)){
			spi_nand_read_page_to_cache(start_page);
			spi_nand_wait(&status);
			spi_nand_read_from_cache(start_page, 0, chip->page_size + chip->page_spare_size, page_buf);
			//sfc_read_spare(page_buf, 2, 1);
			if((page_buf[chip->page_size + 0] != 0xff) || (page_buf[chip->page_size + 1] != 0xff))
			{
				start_page += chip->pages_per_blk;
				continue;
			}
		}
		ret = spi_nand_do_read_page(read_buf, start_page++ , 0, 0 , &corrected);
		if(ret <0)
			return -1;
		if(len < chip->page_size)
			len = chip->page_size;
		read_buf += chip->page_size;
		len -= chip->page_size;
	}
	return 0;
}

int spl_nand_ctroller_reset(void)
{
	u8 id[SPINAND_MAX_ID_LEN] = {0};


	spi_nand_reset();

	spi_nand_disable_ecc();

	spi_nand_read_id(id);
	chip = &nand_chip;
	return spi_nand_scan_id_table(id);
}
int spl_nand_load_image(struct boot_image_info * info)
	{

	bool ret;
	struct boot_header *header_info;
	unsigned char *header_addr ;
	unsigned int base = 0;
	u8 *spl_load_addr = NULL;
	uart_puts("SPL Nand Load Start!\r\n");
	header_addr = smalloc(HEADER_LEN);
	if(header_addr == NULL)
		uart_puts("malloc header fail\r\n");

	base = info->nvm_base;

	ret = spl_nand_ctroller_reset();
	if(ret){
		uart_puts("nand device scan fail\r\n");
		return ret;
	}
		
		
	if (info->flag & BIT_HEADER) {
		spi_nand_read_data((u8 *)header_addr,base, HEADER_LEN);

		header_info = (struct boot_header *)(header_addr);

		if (header_info->lzma)
			spl_load_addr = (u8 *)(header_info->lzma_src);
		else
			spl_load_addr = (u8 *)(header_info->addr);

		ret = spi_nand_read_data((u8 *)spl_load_addr,base + HEADER_LEN, header_info->length);
		if(ret){
			uart_puts("nand read data  fail!\n");
			return -1;
		}
		ret = secure_verify(header_info);
		if (true == ret)
		{
			uart_puts("program ok!\n");
		}

		if(info->type == LOAD_nIMAGE){
			memcpy((unsigned int *)header_info->image_dst, (unsigned int *)header_info->image_src, header_info->image_len);
		}
	}else {
		spi_nand_read_data((u8 *)info->mem_base,base, info->size);
	}

	return 0;
}

int spi_nand_read(struct spi_flash *nand,u32 from, size_t len, void *read_buf)
{
	u32 corrected;
	u32 page_shift;
	u32 start_page;
	u32 column;
	u32 rd_len;

	memset(page_buf, 0 , chip->page_size);
	page_shift = ilog2(chip->page_size);
	start_page = rounddown(from, chip->page_size) >> page_shift;

	spi_nand_disable_ecc();

	while(len){
		column = from%chip->page_size;
		if(column + len < chip->page_size)rd_len = len;
		else rd_len = chip->page_size -column;

		spi_nand_do_read_page(page_buf, start_page++ , 0, 0 , &corrected);
		memcpy((u8 *)read_buf, page_buf + column, rd_len);

		len -= rd_len;
		read_buf += rd_len;
		from += rd_len;
	}
	flush_cache((ulong)read_buf, len);

	return 0;
}

int spi_nand_program(struct spi_flash *nand,u32 to, size_t len, const void *write_buf)
{
	u32 start_page;
	u8 idx =0;
	u32 page_shift;
	u32 column;
	u32 wr_len;

	page_shift = ilog2(chip->page_size);

	start_page = rounddown(to, chip->page_size) >> page_shift;

	while(len){
		column = to%chip->page_size;
		if(column + len < chip->page_size)wr_len = len;
		else wr_len = chip->page_size - column;
		memset(page_buf, 0xff, chip->page_size+ chip->page_spare_size);

		memcpy(page_buf + column, (u8 *)write_buf, wr_len);

		spi_nand_do_write_page(page_buf, start_page++, 0, 0);

		write_buf += wr_len;
		to += wr_len;
		len -= wr_len;
	}
	for(idx =0 ;idx<10;idx++){
		mc_printf("$");
	}
	mc_printf("\r\n");
	return 0;
}

int spi_nand_erase(struct spi_flash *nand,u32 to, size_t len)
{
	int ret;
	u8 status;
	int i;
	u8 lock = 0;
	u32 block_num;
	spi_nand_lock_block(lock);
	spi_nand_write_enable();
	spi_nand_disable_ecc();

	block_num = roundup((to +len), chip->page_size*chip->pages_per_blk)/chip->page_size/chip->pages_per_blk;

	for(i=to/chip->page_size/chip->pages_per_blk;
		i<block_num;
		i++){
		spi_nand_write_enable();
		spi_nand_erase_blk(i*chip->pages_per_blk);
		ret = spi_nand_wait(&status);
		if(ret <0){
		}
	}
	return 0;
}

