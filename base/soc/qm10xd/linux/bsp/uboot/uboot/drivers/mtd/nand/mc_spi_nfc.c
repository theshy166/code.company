
/*
 * The Flash Memory Controller Device Driver for Molchip
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
/*#include <match_table.h>*/
#include <mach/hardware.h>
#include <linux/sizes.h>
#include "spi_nand.h"
#include "mc_spi_nfc.h"

static inline unsigned int mc_snfc_read(struct mc_snfc_host *host, unsigned int reg)
{
	return readl(host->regbase + reg);
}

static inline void mc_snfc_write(struct mc_snfc_host *host, unsigned int value,
				   unsigned int reg)
{
	writel(value, host->regbase + reg);
}
void mc_ecc_sts_clr(struct mc_snfc_host *host)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_ACCESS_START);
	val &= ~NFC_ECC_STS_CLR;
	mc_snfc_write(host, val, REG_NFC_ACCESS_START);
}

void mc_sfc_ecc_clr(struct mc_snfc_host *host)
{
	mc_ecc_sts_clr(host);
	mc_snfc_write(host, 0, REG_NFC_ECC_CFG0);
	mc_snfc_write(host, 0, REG_NFC_ECC_CFG1);
	mc_snfc_write(host, 0, REG_NFC_ECC_CFG2);
}

void mc_sfc_default_ecc_cfg(struct mc_snfc_host *host)
{
	mc_snfc_write(host, 0, REG_NFC_ECC_CFG0);
	mc_snfc_write(host, ECC_DEFAULT_ADDR | ECC_NO_IO_FLAG, REG_NFC_ECC_CFG1);
	mc_snfc_write(host, 0, REG_NFC_ECC_CFG2);
}


static void mc_spi_nfc_cmd_init(struct mc_snfc_host *host)
{
	u32 reg;
	
	mc_snfc_write(host, 0, REG_NFC_CMD_MODE);
	mc_snfc_write(host, 0, REG_NFC_CMD_SET0);
	mc_snfc_write(host, 0, REG_NFC_CMD_SET1);
	mc_snfc_write(host, 0, REG_NFC_CMD_SET2);
	reg = mc_snfc_read(host, REG_NFC_CMD_SET3);
	reg &= ~SPI_RD_DLY_NUM_MASK;
	mc_snfc_write(host, reg, REG_NFC_CMD_SET3);
	mc_snfc_write(host, 0, REG_NFC_CMD_SET4);
	mc_snfc_write(host, 0, REG_NFC_DEV_MODE);
	mc_snfc_write(host, 0, REG_NFC_ECC_INFO0);

	reg = mc_snfc_read(host, REG_NFC_ACCESS_START);
	reg |= NFC_FLOWBUF_CLR;
	mc_snfc_write(host, reg, REG_NFC_ACCESS_START);
	mc_sfc_default_ecc_cfg(host);

	//mc_sfc_ecc_clr(host);

}

/*void sfc_cmd(struct mc_snfc_host *host, u32 cmd)
{
	u32 val;
	u32 mask;

	mask = (1 << NFC_CMD_VALUE_SIZE) - 1;
	val =  mc_snfc_read(host, REG_NFC_CMD_SET1);
	val &= ~(mask << NFC_CMD_VALUE_OFFSET);
	val |= (cmd & mask) << NFC_CMD_VALUE_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET1);
}*/

static void mc_spi_nfc_cmd(struct mc_snfc_host *host, u8 cmd)
{
	u32 val;
	u32 mask;

	mask = (1 << NFC_CMD_VALUE_SIZE) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_SET1);
	val &= ~(mask << NFC_CMD_VALUE_OFFSET);
	val |= (cmd & mask) << NFC_CMD_VALUE_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET1);
}

void mc_sfc_addr_length(struct mc_snfc_host *host, u32 len)
{
	u32 val;
	u32 mask;
	if (!len)
		return;
	len -= 1;
	mask = (1 << NFC_ADDR_LENGTH_SIZE) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_ADDR_LENGTH_OFFSET);
	val |= (len & mask) << NFC_ADDR_LENGTH_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET0);
}

static void mc_spi_nfc_addr(struct mc_snfc_host *host, u8 *addr, u32 naddr)
{

	u32 i;
	u32 val;
	u32 address = 0;

	if (!naddr)
		return;

	mc_sfc_addr_length(host, naddr);

	for (i = 0; i < 3; i++)
		address += addr[i] << (i * 8);
	val = mc_snfc_read(host, REG_NFC_CMD_SET1);
	val &= 0xff;
	val |= address << 8;
	mc_snfc_write(host, val, REG_NFC_CMD_SET1);

	if (naddr > 3) {
		address = addr[3] + (addr[4] << 8);
		val = mc_snfc_read(host, REG_NFC_CMD_SET2);
		val &= 0xffff0000;
		val |= address;
		mc_snfc_write(host, val, REG_NFC_CMD_SET2);
	}
}

void mc_nfc_data_length(struct mc_snfc_host *host, unsigned int len)
{
	unsigned int val;

	if (!len)
		return;
	len -= 1;

	val = mc_snfc_read(host, REG_NFC_CMD_SET3);
	val = SPI_BFINS(DATA_LENGTH_XC, len, val);
	mc_snfc_write(host, val, REG_NFC_CMD_SET3);
}

static void mc_spi_nfc_data_len(struct mc_snfc_host *host, u32 size)
{
	u32 val;
	u32 mask;

	if(!size)
		return;

	mc_nfc_data_length(host, size);
	
	size -= 1;
	mask = (1 << NFC_DATA_LENGTH_SIZE) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_DATA_LENGTH_OFFSET);
	val |= (size & mask)<< NFC_DATA_LENGTH_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET0);
}

void mc_sfc_spare_length(struct mc_snfc_host *host, u32 len)
{
	u32 val;
	u32 mask;

	if(!len)
		return;
	len -= 1;
	mask = (1 << NFC_SPARE_LENGTH_SIZE) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_SPARE_LENGTH_OFFSET);
	val |= (len & mask) << NFC_SPARE_LENGTH_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET0);
}

void mc_sfc_spare_len_1k(struct mc_snfc_host *host, u32 len)
{
	u32 val;
	if (!len)
		return;
	len = len - 1;
	val = (len & 0x3f) << SPARE_LEN_1K_OFFSET;
	val |= (63 - len) << SPARE_SKIP_1K_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET4);
}

static void mc_spi_nfc_spare_len(struct mc_snfc_host *host, u32 size)
{
	unsigned int data_size;
	
	data_size = size / (host->main_size >> 10);
	mc_sfc_spare_length(host, size);
	mc_sfc_spare_len_1k(host, data_size);
}

static void mc_spi_nfc_rtx_dir(struct mc_snfc_host *host, int todev)
{
	u32 reg;

	if (todev == CMD_WRITE){
			reg = mc_snfc_read(host, REG_NFC_DMA_COMMON);
			reg &= ~(FB_INCDEC_MODE);
			mc_snfc_write(host, reg, REG_NFC_DMA_COMMON);
			mc_snfc_write(host, NFC_FLOWBUF_CLR, REG_NFC_ACCESS_START);
			reg = mc_snfc_read(host, REG_NFC_CMD_MODE);
			reg &= ~(NFC_READ_CMD | NFC_WRITE_CMD);
			reg |= NFC_WRITE_CMD;
			mc_snfc_write(host, reg, REG_NFC_CMD_MODE);
	}else{
			reg = mc_snfc_read(host, REG_NFC_DMA_COMMON);
			reg |= FB_INCDEC_MODE;
			mc_snfc_write(host, reg, REG_NFC_DMA_COMMON);
			mc_snfc_write(host, NFC_FLOWBUF_CLR, REG_NFC_ACCESS_START);
			reg = mc_snfc_read(host, REG_NFC_CMD_MODE);
			reg &= ~(NFC_READ_CMD | NFC_WRITE_CMD);
			reg |= NFC_READ_CMD;
			mc_snfc_write(host, reg, REG_NFC_CMD_MODE);
	}
}

static void mc_spi_nfc_data_type(struct mc_snfc_host *host, u32 type)
{
	u32 val;
	u32 mask;

	mask = (1 << NFC_DATA_TYPE_OFFSET) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_MODE);
	val &= ~(mask << NFC_DATA_TYPE_OFFSET);
	val |= (type & mask) << NFC_DATA_TYPE_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_MODE);// Dummy Byte: 0 byte
}

static void mc_spi_nfc_cs_select(struct mtd_info *mtd, u32 en)
{
	u32 reg;
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;

	mc_snfc_write(host, (unsigned int)~SSIENR_SSIEN, SPI_NFC_SSIENR);
	reg = mc_snfc_read(host,  SPI_NFC_SER);
	if (en)
		reg = SER_SELECT(host->chipselect);
	else
		reg &= ~(SER_SELECT(host->chipselect));
	mc_snfc_write(host, reg, SPI_NFC_SER);
	mc_snfc_write(host, SSIENR_SSIEN, SPI_NFC_SSIENR);

}

static void mc_spi_nfc_enable_clk(void)
{
	unsigned int reg;
	reg = readl(REG_SFC_CTRL0);
	reg |= CKG_SFC_EN | CKG_APB_SFC_EN;
	writel(reg, REG_SFC_CTRL0);
}

static void mc_spi_nfc_sel_clk(void)
{
	unsigned int reg;
	reg = readl(REG_SFC_CTRL0);
	reg &= ~(SFC_CLOCK_SEL(0x7));
	reg |= SFC_CLOCK_SEL(0x3);
	writel(reg, REG_SFC_CTRL0);
}


static int wait_op_finish(struct mc_snfc_host *host)
{
	unsigned regval, timeout = MC_WAIT_TIMEOUT;

	do {
		udelay(10);\
		regval = mc_snfc_read(host, REG_NFC_INTR_RAW);
		--timeout; \
	} while ((!(regval & SPI_FINISH) && timeout));
	if (!timeout) {
		printf("Error: Wait SPI NFC finish timeout!\n");
		return 1;
	} else {
		mc_snfc_write(host, regval, REG_NFC_INTR_RAW);
		return 0;
	}
}

static inline void mc_spi_nfc_start_cmd(struct mc_snfc_host *host)
{
	mc_snfc_write(host, NFC_ACC_START, REG_NFC_ACCESS_START);
}

static inline void mc_spi_nfc_clear_all(struct mc_snfc_host *host)
{
	mc_snfc_read(host, SPI_NFC_ICR);
}

void mc_nfc_dev_mode_clr(struct mc_snfc_host *host)
{
	mc_snfc_write(host, 0, REG_NFC_DEV_MODE);
}

void mc_nfc_set_tmod(struct mc_snfc_host *host, unsigned char mode)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(TMOD, mode, val);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_nfc_set_fldform(struct mc_snfc_host *host, unsigned char mode)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(FLDFORM, mode, val);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_nfc_set_frmform(struct mc_snfc_host *host, unsigned char mode)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(FRMFORM, mode, val);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_nfc_set_phase_vld(struct mc_snfc_host *host, unsigned int bits, unsigned char vld)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	if (vld)
		val |= bits;
	else
		val &= ~(bits);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_nfc_set_cmd_wmode(struct mc_snfc_host *host, unsigned char wire)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(CMD_WMODE, wire, val);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_nfc_set_data_wmode(struct mc_snfc_host *host, unsigned char wire)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(DATA_WMODE, wire, val);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_nfc_set_addr_wmode(struct mc_snfc_host *host, u8 wire)
{
	unsigned int val = 0;

	val = mc_snfc_read(host, REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(ADDR_WMODE, wire, val);
	mc_snfc_write(host, val, REG_NFC_DEV_MODE);
}

void mc_sfc_dev_mode_reg_set(struct mc_snfc_host *host, u32 frmform, u32 vld, u32 nbits)
{
	u32 val;

	mc_nfc_dev_mode_clr(host);
	mc_nfc_set_tmod(host, TMOD_TRANSMIT_ONLY);//Transfer Mode,Transmit Only
	if(nbits == WIRE_MODE_SPI){
		mc_nfc_set_fldform(host, FLDFORM_ALL_SERIAL);//Quad/Dual Data, other Serial
	}else{
		mc_nfc_set_fldform(host, FLDFORM_DQ_DATA_OTHRE_SERIAL);
	}
	mc_nfc_set_frmform(host, frmform);//opcode + 3byte addr
	if (vld == VLD_CMD)
		val = NFC_CMD_PHASE_VLD;
	else if (vld == VLD_CMD_DATA)
		val = NFC_CMD_PHASE_VLD | NFC_DATA_PHASE_VLD;
	else if (vld == VLD_CMD_ADDR)
		val = NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD;
	else if (vld == VLD_CMD_ADDR_DATA)
		val = NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD |
		NFC_ADDR_PHASE_VLD;
	else if (vld == VLD_CMD_ADDR_REG)
		val = NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD |
		NFC_ADDR_PHASE_VLD | NFC_REG_PHASE_VLD;
	else
		val = 0;
	mc_nfc_set_phase_vld(host, val, NFC_ENABLE);
	mc_nfc_set_cmd_wmode(host, WMODE_SINGLE);
	mc_nfc_set_data_wmode(host, nbits);
	mc_nfc_set_addr_wmode(host, WMODE_SINGLE);
}

int mc_sfc_wait_fbuf2_ready(struct mc_snfc_host *host)
{
	unsigned long timeo = 50000;
    unsigned int val;

	do{
		val = mc_snfc_read(host, REG_NFC_BUF_SOFT);
		if (val & NFC_SOFT_FB_P2_RDY_BIT) {
			return 0;
		}
		udelay(1);
	}while(timeo --);
		
	return 1;
}

void mc_sfc_fbuf2_ack(struct mc_snfc_host *host)
{
	mc_snfc_write(host, NFC_SOFT_FB_P2_ACK, REG_NFC_BUF_SOFT);
	udelay(100);
}

int mc_sfc_wait_fbuf2_sta(struct mc_snfc_host *host)
{
    unsigned int val;
	
	udelay(50);
	val = mc_snfc_read(host, REG_NFC_BUF_SOFT);
	if (val & NFC_SOFT_FB_P2_RDY_BIT) {
		return 0;
	}
		
	return 1;
}

void mc_spi_nfc_start_ops(struct mc_snfc_host *host)
{
	int ret;

	ret = mc_sfc_wait_fbuf2_sta(host);
	if(ret == 0){
		mc_sfc_fbuf2_ack(host);
	}

	mc_spi_nfc_start_cmd(host);
	ret = wait_op_finish(host);
	if (ret){
		while(1);
	} else {
		mc_snfc_read(host, SPI_NFC_ICR);
	}
}

void mc_spi_nfc_fb_incdec_mode(struct mc_snfc_host *host, unsigned char mode)
{
	unsigned int val;

	val = mc_snfc_read(host, REG_NFC_DMA_COMMON);
	if (mode)
		val |= FB_INCDEC_MODE;
	else
		val &= ~(FB_INCDEC_MODE);
	mc_snfc_write(host, val, REG_NFC_DMA_COMMON);

}

void spi_nand_read_page_to_cache(struct mc_snfc_host *host)
{
	unsigned char addr[4];

	memset(addr, 0, sizeof(addr));

	addr[0] = (u8)(host->page_addr >> 16);
	addr[1] = (u8)(host->page_addr >> 8);
	addr[2] = (u8)host->page_addr;

	mc_spi_nfc_fb_incdec_mode(host, FB_INCDEC_WRITE);
	mc_spi_nfc_cmd_init(host);

	mc_spi_nfc_cmd(host, SPINAND_CMD_READ);
	mc_spi_nfc_addr(host, addr, 3);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR, XF_MODE_SPI);

	mc_spi_nfc_start_ops(host);
}

void spi_nand_read_from_cache(struct mc_snfc_host *host)
{
	struct nand_chip *chip = host->chip;
	unsigned int data_size;
	unsigned char addr[4];
	unsigned int cmd, xf_mode;

	memset(addr, 0, sizeof(addr));

	if (host->dev_id[0]==0xC8) {/*FIXME: early GD chips, test 1G*/
		addr[0] = 0;
		addr[1] = (u8)(host->column >> 8);
		if (chip->options & SPINAND_NEED_PLANE_SELECT)
			addr[0] |= (u8)(((host->page_addr >>
					      (chip->phys_erase_shift - chip->page_shift)) & 0x1) << 4);
		addr[2] = (u8)host->column;
	} else {
		addr[0] = (u8)(host->column >> 8);
		if (chip->options & SPINAND_NEED_PLANE_SELECT)
			addr[0] |= (u8)(((host->page_addr >>
					      (chip->phys_erase_shift - chip->page_shift)) & 0x1) << 4);
		addr[1] = (u8)host->column;
		addr[2] = 0;
	}

	mc_spi_nfc_cmd_init(host);
	if (host->option & SNFC_HWCAPS_READ_1_1_4) {
		cmd = SPINAND_CMD_READ_CACHE_X4;
		xf_mode = XF_MODE_QPI;
	} else if (host->option & SNFC_HWCAPS_READ_1_1_2) {
		cmd = SPINAND_CMD_READ_CACHE_X2;
		xf_mode = XF_MODE_DPI;
	} else {
		cmd = SPINAND_CMD_READ_RDM;
		xf_mode = XF_MODE_SPI;
	}
	mc_spi_nfc_cmd(host, cmd);
	mc_spi_nfc_addr(host, addr, 3);
	mc_spi_nfc_rtx_dir(host, CMD_READ);

	data_size = host->main_size;
	mc_spi_nfc_data_len(host, data_size);
	//data_size = host->spare_size / (host->main_size >> 10);
	mc_spi_nfc_spare_len(host, host->spare_size);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR_DATA, xf_mode);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_SPARE);

	mc_spi_nfc_start_ops(host);
}

static void spi_nand_write_enable(struct mc_snfc_host *host)
{

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_WR_ENABLE);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD, WIRE_MODE_SPI);

	mc_spi_nfc_start_ops(host);
}

static void spi_nand_program_data_to_cache(struct mc_snfc_host *host,
						struct mtd_info *mtd, const uint8_t *buf, int oob_required)
{
	struct nand_chip *chip = host->chip;
	unsigned char addr[4];
	unsigned int data_size;

	memset(addr, 0, sizeof(addr));

	addr[0] = (u8)(host->column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
		addr[0] |= (u8)(((host->page_addr >>
				      (chip->phys_erase_shift - chip->page_shift)) & 0x1) << 4);
	addr[1] = (u8)host->column;

	spi_nand_write_enable(host);
	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_PROG_LOAD);
	mc_spi_nfc_addr(host, addr, 2);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	if(buf)
		chip->write_buf(mtd, buf, mtd->writesize);
	if (oob_required)
		chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

	data_size = host->main_size;
	mc_spi_nfc_data_len(host, data_size);
	//data_size = host->spare_size / (host->main_size >> 10);
	mc_spi_nfc_spare_len(host, host->spare_size);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_2ADDR, VLD_CMD_ADDR_DATA, XF_MODE_SPI);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_SPARE);

	mc_spi_nfc_start_ops(host);
}

static void spi_nand_program_execute(struct mc_snfc_host *host)
{
	unsigned char addr[4];

	memset(addr, 0, sizeof(addr));

	addr[0] = (u8)(host->page_addr >> 16);
	addr[1] = (u8)(host->page_addr >> 8);
	addr[2] = (u8)host->page_addr;

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_PROG);
	mc_spi_nfc_addr(host, addr, 3);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR, XF_MODE_SPI);

	mc_spi_nfc_start_ops(host);
}

static void mc_send_cmd_readid(struct mc_snfc_host *host)
{
	unsigned char addr[4];

	memset(host->mbuf, 0, 8);
	memset(addr, 0, sizeof(addr));

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_READ_ID);
	mc_spi_nfc_addr(host, addr, 1);
	mc_spi_nfc_rtx_dir(host, CMD_READ);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_ONLY);
	mc_spi_nfc_data_len(host, SPINAND_MAX_ID_LEN);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_1ADDR, VLD_CMD_ADDR_DATA, XF_MODE_SPI);

	mc_spi_nfc_start_ops(host);

}

static void mc_send_cmd_reset(struct mc_snfc_host *host)
{

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_RESET);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD, XF_MODE_SPI);

	mc_spi_nfc_start_ops(host);

	/* elapse 1ms before issuing any other command */
	udelay(1000);
}

static void mc_send_cmd_status(struct mc_snfc_host *host)
{

	unsigned char addr[4];

	memset(host->mbuf, 0, 8);
	memset(addr, 0, sizeof(addr));
	addr[0] = REG_STATUS;

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_READ_REG);
	mc_spi_nfc_addr(host, addr, 1);
	mc_spi_nfc_rtx_dir(host, CMD_READ);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_ONLY);
	mc_spi_nfc_data_len(host, 1);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_1ADDR, VLD_CMD_ADDR_DATA, XF_MODE_SPI);

	mc_spi_nfc_start_ops(host);

}

static void mc_send_cmd_erase(struct mc_snfc_host *host)
{
	u8 addr[4];

	memset(addr, 0, 4);

	spi_nand_write_enable(host);
	addr[0] = (u8)(host->page_addr >> 16);
	addr[1] = (u8)(host->page_addr >> 8);
	addr[2] = (u8)host->page_addr;

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_ERASE_BLK);
	mc_spi_nfc_addr(host, addr, 3);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR, XF_MODE_SPI);

	mc_spi_nfc_start_ops(host);

}

static unsigned char mc_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;
	unsigned int value, offset, shift;
	unsigned int buf;
	unsigned char ret_val = 0;

	offset = host->offset >> 2 << 2;
	shift = (host->offset & 0x03) << 3;
	if (host->cmd == NAND_CMD_READOOB) {
		buf = SPI_NFC_SPARE_BUFFER;
	} else {
		buf = SPI_NFC_DATA_BUFFER;
	}
	
	mc_sfc_wait_fbuf2_ready(host);

	value = mc_snfc_read(host, buf + offset);

	if (host->cmd == NAND_CMD_STATUS) {

		if (!(value & STATUS_OIP_MASK))
			ret_val |= NAND_STATUS_READY;

		if ((chip->state == FL_ERASING)
			&& (value & STATUS_E_FAIL_MASK)) {
			ret_val |= NAND_STATUS_FAIL;
		}

		if ((chip->state == FL_WRITING)
			&& (value & STATUS_P_FAIL_MASK)) {
			ret_val |= NAND_STATUS_FAIL;
		}

		return ret_val;
	}

	ret_val = (value >> shift) & 0xff;
	host->offset ++;
	return ret_val;
}

static unsigned short mc_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;
	unsigned int value, offset, shift;
	unsigned short ret_val = 0;
	
	mc_sfc_wait_fbuf2_ready(host);

	offset = host->offset >> 2 << 2;
	shift = (host->offset & 0x03) << 3;
	value = mc_snfc_read(host, SPI_NFC_DATA_BUFFER + offset);
	if(shift == 24) {
		ret_val = (value >> shift ) & 0xff;
		value = mc_snfc_read(host, SPI_NFC_DATA_BUFFER + offset + 4);
		ret_val |= (value & 0xff) << 8;
	} else {
		ret_val = (value >> shift ) & 0xffff;
	}
	host->offset += 2;

	return ret_val;
}

static void mc_write_buf(struct mtd_info *mtd,
	const u_char *buf, int len)
{
	unsigned int i, total, ofs;
	unsigned int size;
	void __iomem *dest;
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;

	mc_sfc_wait_fbuf2_ready(host);

	if(len & 0x3) {
		printf("Warning, data len is not multiple 4\n");
	}
	memset(host->sbuf, 0xff, SPI_NAND_MAX_OOBSIZE);
	if(host->mbuf_writed) {
		host->mbuf_writed = 0;
	} else {
		memset(host->mbuf, 0xff, SPI_NAND_MAX_PAGESIZE);
	}

	if (buf == chip->oob_poi) {
		total = 1 << (chip->page_shift - 10);
		len = len / total;
		size = len >> 2 << 2;
		dest = host->sbuf;

		for(i = 0; i < total; i++) {
			dest += 0x40 * i;
			ofs = size * i;
			memcpy(dest, buf + ofs, size);
		}
	} else {
		host->mbuf_writed = 1;
		size = len >> 2 << 2;
		dest = host->mbuf;
		memcpy(dest, buf, size);
	}

}

static void mc_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;
	unsigned int i, total, ofs;
	unsigned int size;
	void __iomem * src;

	if(len & 0x3) {
		printf("Warning, data len is not multiple 4\n");
	}

	mc_sfc_wait_fbuf2_ready(host);

	if (buf == chip->oob_poi) {
		total = 1 << (chip->page_shift - 10);
		len = len / total;
		size = len >> 2 << 2;
		src = host->sbuf;

		for(i = 0; i < total; i++) {
			src += 0x40 * i;
			ofs = size * i;
			memcpy(buf + ofs, src, size);
		}
	} else {
		src = host->mbuf;
		size = len >> 2 << 2;
		memcpy(buf, src, size);
	}
}

static void mc_select_chip(struct mtd_info *mtd, int chipselect)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;

	if (chipselect < 0) {
//		mc_spi_nfc_cs_select(mtd, false);
		return;
	}

	if (chipselect > CONFIG_SPI_NAND_MAX_CHIP_NUM)
		printf("Invalid chipselect: %d\n", chipselect);

	mc_spi_nfc_cs_select(mtd, true);
	if (host->mtd != mtd) {
		host->mtd = mtd;
	}
}

static void mc_cmdfunc(struct mtd_info *mtd, unsigned int command,
				int column, int page_addr)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;

	switch (command) {
	case NAND_CMD_RESET:
		host->send_cmd_reset(host);
		chip->dev_ready(mtd);
		break;
	case NAND_CMD_READID:
		host->offset = 0;
		host->cmd = command & 0xff;
		host->send_cmd_readid(host);
		break;
	case NAND_CMD_GET_FEATURES:
	case NAND_CMD_STATUS:
		host->offset = 0;
		host->cmd = command & 0xff;
		host->send_cmd_status(host);
		break;
	case NAND_CMD_READOOB:
		host->offset = 0;
		host->cmd = command & 0xff;
		host->page_addr = page_addr;
		host->send_cmd_readstart(host);
		break;
	case NAND_CMD_READ0:
		host->offset = 0;
		host->cmd = command & 0xff;
		host->page_addr = page_addr;
		host->column = 0;
		break;
	case NAND_CMD_SEQIN:
		host->column = 0;
		host->page_addr = page_addr;
		break;
	case NAND_CMD_PAGEPROG:
		host->offset = 0;
		break;
	case NAND_CMD_ERASE1:
		host->cmd = command & 0xff;
		host->page_addr = page_addr;
		host->send_cmd_erase(host);
		break;
	case NAND_CMD_ERASE2:
	case NAND_CMD_READSTART:
		break;
	default:
		printf("%s not support command 0x%08x:\n", mtd->name, command);
		break;
	}
}

static int mc_dev_ready(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;
	unsigned long timeo = MC_WAIT_TIMEOUT;
	unsigned int val;
	do {
		mc_send_cmd_status(host);
		mc_sfc_wait_fbuf2_ready(host);
		val = readl(host->mbuf);
		if (!(val & STATUS_OIP_MASK)) {
			return  1;
		}
		udelay(1);
	} while (timeo --);

	return 0;
}

void mc_spi_nand_init(struct mc_snfc_host *host)
{
	struct nand_chip *chip = host->chip;

	chip->priv = host;

	if (!chip->IO_ADDR_R)
		chip->IO_ADDR_R = (void __iomem *)host->mbuf;
	chip->IO_ADDR_W = chip->IO_ADDR_R;
	memset((char *)chip->IO_ADDR_R, 0xff, SPI_NAND_MAX_PAGESIZE);

	chip->read_byte = mc_read_byte;
	chip->read_word = mc_read_word;
	chip->write_buf = mc_write_buf;
	chip->read_buf = mc_read_buf;

	chip->select_chip = mc_select_chip;

	chip->cmdfunc = mc_cmdfunc;
	chip->dev_ready = mc_dev_ready;

	chip->chip_delay = MC_CHIP_DELAY;

	chip->options = NAND_BBT_SCANNED | NAND_BROKEN_XD |
			 NAND_NO_SUBPAGE_WRITE;

	//chip->ecc.layout = NULL;
	chip->ecc.mode = NAND_ECC_HW;

}

static struct mc_snfc_host mc_snfc_host = {
	.chip = NULL,
};

static void mc_send_cmd_pageprog(struct mc_snfc_host *host, 
	struct mtd_info *mtd, const uint8_t *buf, int oob_required)
{

	spi_nand_write_enable(host);

	spi_nand_program_data_to_cache(host, mtd, buf, oob_required);

	spi_nand_program_execute(host);

}

static void spi_nand_write_feature(struct mc_snfc_host *host, u8 reg, u8 val)
{
	unsigned int value;

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_WRITE_REG);
	mc_spi_nfc_addr(host, &reg, 1);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	value = val;
	writel(value, host->mbuf);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_ONLY);
	mc_spi_nfc_data_len(host, 1);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_1ADDR, VLD_CMD_ADDR_DATA, XF_MODE_SPI);
	mc_spi_nfc_start_ops(host);

}

static void spi_nand_lock_block(struct mc_snfc_host *host, u8 lock)
{

	spi_nand_write_feature(host, REG_BLOCK_LOCK, lock);

}

static void spi_nand_ecc_disable(struct mc_snfc_host *host)
{

	spi_nand_write_feature(host, REG_OTP, 0x9);

}

int spi_nand_wait(struct mc_snfc_host *host, unsigned char *s)
{
	unsigned long timeo = MC_WAIT_TIMEOUT;
	unsigned char status = 0;
    unsigned int val;

	do {
		mc_send_cmd_status(host);
		mc_sfc_wait_fbuf2_ready(host);
		val = readl(host->mbuf);
		status = val & 0xff;
		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			return  0;
		}
		udelay(1);
	} while (timeo --);

	if (s)
		*s = status;

	return 1;
}

void mc_send_cmd_readstart(struct mc_snfc_host *host)
{
	unsigned char status;
	int ret;

	spi_nand_read_page_to_cache(host);

	ret = spi_nand_wait(host, &status);
	if(ret) {
		printf("Error: spi nand wait error! status:%x\n",
				status);
	}

	spi_nand_read_from_cache(host);

}

void mc_sfc_ecc_enc_mode(struct mc_snfc_host *host, u32 encode)
{
	u32 val;

	val = mc_snfc_read(host, REG_NFC_ECC_CFG1);
	if (encode)
		val |= ECC_ENCODE_MODE;
	else
		val &= ~(ECC_ENCODE_MODE);
	mc_snfc_write(host, val, REG_NFC_ECC_CFG1);
}

void mc_sfc_ecc_mode(struct mc_snfc_host *host, u32 mode)
{
	u32 val;

	val = mc_snfc_read(host, REG_NFC_ECC_CFG1);
	if (mode == ECC_24BIT)
		val |= ECC_MODE;
	else
		val &= ~ECC_MODE;
	mc_snfc_write(host, val, REG_NFC_ECC_CFG1);
}

void mc_sfc_ecc_pos(struct mc_snfc_host *host, u32 pos)
{
	u32 mask;
	u32 val;

	mask = (1 << NFC_ECC_POS_SIZE) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_SET2);
	val &= ~(mask << NFC_ECC_POS_OFFSET);
	val |= (pos & mask) << NFC_ECC_POS_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET2);

	mask = ECC_SPARE_BASE_BYTE_ADDR_MSK;
	val = mc_snfc_read(host, REG_NFC_ECC_CFG1);
	val &= ~(mask << ECC_SPARE_BASE_BYTE_ADDR_OFS);
	val |= (pos & mask) << ECC_SPARE_BASE_BYTE_ADDR_OFS;
	mc_snfc_write(host, val, REG_NFC_ECC_CFG1);
}

void  mc_sfc_spare_ecc_byte(struct mc_snfc_host *host, u32 ecc)
{
	u32 mask;
	u32 val;

	mask = (1 << NFC_ECC_SPARE_SIZE) - 1;
	val = mc_snfc_read(host, REG_NFC_CMD_SET2);
	val &= ~(mask << NFC_ECC_SPARE_OFFSET);
	val |= (ecc & mask) << NFC_ECC_SPARE_OFFSET;
	mc_snfc_write(host, val, REG_NFC_CMD_SET2);

	mask = ECC_SPARE_BYTE_NUM_MSK;
	val = mc_snfc_read(host, REG_NFC_ECC_CFG1);
	val &= ~(mask << ECC_SPARE_BYTE_NUM_OFS);
	val |= (ecc & mask) << ECC_SPARE_BYTE_NUM_OFS;
	mc_snfc_write(host, val, REG_NFC_ECC_CFG1);
}

void mc_sfc_ecc_page_size(struct mc_snfc_host *host, u32 size)
{
	u32 val;
	if (!size)
		return;
	size = size - 1;
	val = size & ECC_DATA_LEN_MSK;
	mc_snfc_write(host, val, REG_NFC_ECC_CFG0);
	val = size & ECC_PAGE_SIZE_MSK;
	mc_snfc_write(host, val, REG_NFC_ECC_CFG2);
}

void mc_sfc_ecc_enable(struct mc_snfc_host *host, bool en)
{
	u32 val;

	val = mc_snfc_read(host, REG_NFC_CMD_MODE);

	if (en == NFC_ENABLE)
		val |= ECC_CAL_EB;
	else
		val &= ~ECC_CAL_EB;

	mc_snfc_write(host, val, REG_NFC_CMD_MODE);

	val = mc_snfc_read(host, REG_NFC_ECC_CFG1);

	if (en == NFC_ENABLE)
		val |= ECC_ECC_CAL_EB;
	else
		val &= ~ECC_ECC_CAL_EB;

	mc_snfc_write(host, val, REG_NFC_ECC_CFG1);
}


#ifdef CONFIG_NAND_SPL_ECC_8BIT
void mc_spi_nfc_hw_8ecc_enable(struct mtd_info *mtd, int todev)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;

	mc_sfc_ecc_clr(host);
	if(todev == CMD_READ){
		mc_sfc_ecc_enc_mode(host, ECC_DECODE);
	}else{
		mc_sfc_ecc_enc_mode(host, ECC_ENCODE);
	}
	mc_sfc_ecc_mode(host, ECC_8BIT);
	mc_sfc_ecc_pos(host, 0x12);
	mc_sfc_spare_ecc_byte(host, 0x12);
	mc_sfc_ecc_page_size(host, host->main_size);
	mc_sfc_ecc_enable(host, NFC_ENABLE);
}
#endif

void mc_spi_nfc_hw_ecc_enable(struct mtd_info *mtd, int todev)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;
	struct mc_snfc_hw_ecc *data = chip->ecc.priv;


	mc_sfc_ecc_clr(host);
	if(todev == CMD_READ){
		mc_sfc_ecc_enc_mode(host, ECC_DECODE);
	}else{
		mc_sfc_ecc_enc_mode(host, ECC_ENCODE);
	}
	mc_sfc_ecc_mode(host, data->mode);
	mc_sfc_ecc_pos(host, data->start_pos);
	mc_sfc_spare_ecc_byte(host, data->info_size);
	mc_sfc_ecc_page_size(host, host->main_size);
	mc_sfc_ecc_enable(host, NFC_ENABLE);
	
}

void mc_spi_nfc_hw_ecc_disable(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_host *host = chip->priv;

	mc_sfc_ecc_enable(host, NFC_DISABLE);
}

int mc_spi_nfc_hw_common_ecc_ctrl_init(struct mtd_info *mtd,
					      struct nand_ecc_ctrl *ecc)
{
	static const u8 strengths[] = { 8, 24};
	struct mc_snfc_hw_ecc *data;
	struct nand_ecclayout *layout;
	int nsectors;
	int ret;
	int i;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	if (ecc->size != 1024)
		return -EINVAL;

	/* Add ECC info retrieval from DT */
	for (i = 0; i < ARRAY_SIZE(strengths); i++) {
		if (ecc->strength <= strengths[i])
			break;
	}

	if (i >= ARRAY_SIZE(strengths)) {
		dev_err(nfc->dev, "unsupported strength\n");
		ret = -ENOTSUPP;
		goto err;
	}

	data->mode = i;

	/* HW ECC always request ECC bytes for 1024 bytes blocks */
	ecc->bytes = DIV_ROUND_UP(ecc->strength * fls(8 * 1024), 8);

	/* HW ECC always work with even numbers of ECC bytes */
	ecc->bytes = ALIGN(ecc->bytes, 2);

	layout = &data->layout;
	nsectors = mtd->writesize / ecc->size;

	if (mtd->oobsize < ((ecc->bytes + 4) * nsectors)) {
		ret = -EINVAL;
		goto err;
	}

	layout->eccbytes = (ecc->bytes * nsectors);

	ecc->layout = layout;
	ecc->priv = data;

	data->info_size = mtd->oobsize / nsectors - ecc->bytes;
	data->start_pos = data->info_size;

	return 0;

err:
	kfree(data);

	return ret;
}

#define BITS_PER_BYTE 8

int nand_check_erased_buf(void *buf, int len, int bitflips_threshold)
{
	const unsigned char *bitmap = buf;
	int bitflips = 0;
	int weight;

	for (; len && ((uintptr_t)bitmap) % sizeof(long);
	     len--, bitmap++) {
		weight = hweight8(*bitmap);
		bitflips += BITS_PER_BYTE - weight;
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
		bitflips += BITS_PER_BYTE - weight;
		if (unlikely(bitflips > bitflips_threshold))
			return -EBADMSG;
	}

	return bitflips;
}

int nand_check_ecc_chunk(void *data, int datalen, void *oob,
					int ooblen, int bitflips_threshold)
{
	int data_bitflips = 0, oob_bitflips = 0;
	int bit_corrected = 0;

	data_bitflips = nand_check_erased_buf(data, datalen, bitflips_threshold);
	if (data_bitflips < 0) {
		printf("data bitflips exceed max thresholds %d\n ", bitflips_threshold);
		return data_bitflips;
	}

	bitflips_threshold -= data_bitflips;
	bit_corrected += data_bitflips;

	oob_bitflips = nand_check_erased_buf(oob, ooblen, bitflips_threshold);
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

int mc_spi_nfc_hw_ecc_read(struct mtd_info *mtd,
					struct nand_chip *chip, uint8_t *buf,
					int oob_required, int page, unsigned int data_size)
{
	struct mc_snfc_host *host = chip->priv;
	struct nand_ecc_ctrl *ecc = &chip->ecc;
	unsigned int  ecc_info, max_bitflips = 0;
	int ret, i;
	unsigned char status;
	unsigned char column[4] = {0};
	unsigned int cmd, xf_mode;

	host->page_addr = page;
	spi_nand_read_page_to_cache(host);

	ret = spi_nand_wait(host, &status);
	if(ret) {
		printf("Error: spi nand wait error! status:%x\n",
				status);
	}
	mc_spi_nfc_fb_incdec_mode(host, FB_INCDEC_READ);
	mc_spi_nfc_cmd_init(host);
	if (host->option & SNFC_HWCAPS_READ_1_1_4) {
		cmd = SPINAND_CMD_READ_CACHE_X4;
		xf_mode = XF_MODE_QPI;
	} else if (host->option & SNFC_HWCAPS_READ_1_1_2) {
		cmd = SPINAND_CMD_READ_CACHE_X2;
		xf_mode = XF_MODE_DPI;
	} else {
		cmd = SPINAND_CMD_READ_RDM;
		xf_mode = XF_MODE_SPI;
	}
	mc_spi_nfc_cmd(host, cmd);
	mc_spi_nfc_addr(host, column, 3);
	mc_spi_nfc_rtx_dir(host, CMD_READ);

	mc_spi_nfc_data_len(host, data_size);
#ifdef CONFIG_NAND_SPL_ECC_8BIT
	if (page <  0x8000 / mtd->writesize) {
		mc_spi_nfc_hw_8ecc_enable(mtd, CMD_READ);
	} else
#endif
		mc_spi_nfc_hw_ecc_enable(mtd, CMD_READ);
	//data_size = host->spare_size / (host->main_size >> 10);
	mc_spi_nfc_spare_len(host, host->spare_size);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR_DATA, xf_mode);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_SPARE);

	mc_spi_nfc_start_ops(host);
	
	mc_sfc_wait_fbuf2_ready(host);

	ecc_info = mc_snfc_read(host, REG_NFC_ECC_INFO0);
	for (i = 0; i < ecc->steps; i++) {
		unsigned int errno;
		void *databuf, *oobbuf;
		int datalen, ooblen;
		status = (ecc_info >> (i << 1)) & 0x3;
		if (status == ECC_UNCORRECTED) {
			databuf = host->mbuf + (i << 10) ;
			datalen = 1024;
			oobbuf = host->sbuf + (i << 6);
			ooblen = host->spare_size / ecc->steps;
			ret = nand_check_ecc_chunk(databuf, datalen, oobbuf,
						ooblen, ecc->strength);
			if (ret < 0) {
				printf("page %x bitflips exceed the ecc strength\n", page);
				mtd->ecc_stats.failed++;
			} else {
				mtd->ecc_stats.corrected += ret;
				max_bitflips = max_t(unsigned int, max_bitflips, ret);
			}
		} else {
			errno = (ecc_info >> (8 + i * 5)) & 0x1f;
			mtd->ecc_stats.corrected += errno;
			max_bitflips = max_t(unsigned int, max_bitflips, errno);

		}
	}

	chip->read_buf(mtd, buf, mtd->writesize);
	if (oob_required)
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	mc_spi_nfc_hw_ecc_disable(mtd);

	return max_bitflips;
}

int mc_spi_nfc_hw_ecc_read_page(struct mtd_info *mtd,
					struct nand_chip *chip, uint8_t *buf,
					int oob_required, int page)
{
	unsigned int max_bitflips = 0;

	if ((mtd->writesize == SZ_4K) &&
						(page < 16)) {
		page = page * 2;
		max_bitflips = mc_spi_nfc_hw_ecc_read(mtd, chip, buf,
						oob_required, page, SZ_2K);
		max_bitflips = mc_spi_nfc_hw_ecc_read(mtd, chip, buf + SZ_2K,
						oob_required, page + 1, SZ_2K);
	} else {
		max_bitflips = mc_spi_nfc_hw_ecc_read(mtd, chip, buf,
						oob_required, page, mtd->writesize);
	}
	flush_cache((ulong)buf, mtd->writesize);
	return max_bitflips;
}

int mc_spi_nfc_hw_ecc_read_subpage(struct mtd_info *mtd,
					 struct nand_chip *chip,
					 uint32_t data_offs, uint32_t readlen,
					 uint8_t *bufpoi, int page)
{

	return 0;
}

int mc_spi_nfc_hw_ecc_write(struct mtd_info *mtd,
				       struct nand_chip *chip,
				       const uint8_t *buf, int oob_required,
				       int page, unsigned int data_size)
{
	struct mc_snfc_host *host = chip->priv;
	unsigned char addr[4];

	spi_nand_write_enable(host);
	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	chip->write_buf(mtd, buf, data_size);
	if (oob_required)
		chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);
	else
		memset(host->sbuf, 0xff, SPI_NAND_MAX_OOBSIZE);

	memset(addr, 0, sizeof(addr));

	//mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_PROG_LOAD);
	if((page /0x40 % 2)&&(chip->options |SPINAND_NEED_PLANE_SELECT_MXIC)){
		if(mtd->writesize == SZ_4K)
			addr[0] = 0x20;
		else if(mtd->writesize == SZ_2K)
			addr[0] = 0x10;
	}

	mc_spi_nfc_addr(host, addr, 2);
	//mc_spi_nfc_rtx_dir(host, CMD_WRITE);

//	data_size = host->main_size;
	mc_spi_nfc_data_len(host, data_size);
#ifdef CONFIG_NAND_SPL_ECC_8BIT
	if (page <  SPL_DATA_AREA / mtd->writesize) {
		mc_spi_nfc_hw_8ecc_enable(mtd, CMD_WRITE);
	} else
#endif
		mc_spi_nfc_hw_ecc_enable(mtd, CMD_WRITE);

	//data_size = host->spare_size / (host->main_size >> 10);
	mc_spi_nfc_spare_len(host, host->spare_size);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_2ADDR, VLD_CMD_ADDR_DATA, XF_MODE_SPI);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_SPARE);

	mc_spi_nfc_start_ops(host);
	mc_spi_nfc_hw_ecc_disable(mtd);

	host->page_addr = page;
	spi_nand_program_execute(host);

	return 0;
}

int mc_spi_nfc_hw_ecc_write_page(struct mtd_info *mtd,
				       struct nand_chip *chip,
				       const uint8_t *buf, int oob_required,
				       int page)
{

	if ((mtd->writesize == SZ_4K) &&
						(page < 16)){
		page = page * 2;
		mc_spi_nfc_hw_ecc_write(mtd, chip, buf,
						oob_required, page , SZ_2K);
		mc_dev_ready(mtd);
		mc_spi_nfc_hw_ecc_write(mtd, chip, buf + SZ_2K,
						oob_required, page + 1, SZ_2K);
		mc_dev_ready(mtd);

	} else {
		mc_spi_nfc_hw_ecc_write(mtd, chip, buf,
					oob_required, page , mtd->writesize);
    }

	return 0;
}

static int mc_spi_nfc_hw_ecc_write_subpage(struct mtd_info *mtd,
					  struct nand_chip *chip,
					  u32 data_offs, u32 data_len,
					  const u8 *buf, int oob_required,
					  int page)
{

	return 0;
}

int mc_spi_nfc_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
	  uint8_t *buf, int oob_required, int page)
{
	struct mc_snfc_host *host = chip->priv;
	mc_send_cmd_readstart(host);
	chip->read_buf(mtd, buf, mtd->writesize);
	if (oob_required)
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	return 0;

}
int mc_spi_nfc_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
	  const uint8_t *buf, int oob_required, int page)
{
	struct mc_snfc_host *host = chip->priv;
	/*chip->write_buf(mtd, buf, mtd->writesize);
	if (oob_required)
	  chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);*/
	mc_send_cmd_pageprog(host, mtd, buf, oob_required);
	return 0;
}

int mc_spi_nand_write_oob(struct mtd_info *mtd, struct nand_chip *chip,
				int page)
{
	int status = 0;
	//const uint8_t *buf = chip->oob_poi;
	//int length = mtd->oobsize;
	struct mc_snfc_host *host = chip->priv;

	//chip->write_buf(mtd, buf, length);
	/* Send command to program the OOB data */
	mc_send_cmd_pageprog(host, mtd, NULL, 1);

	status = chip->waitfunc(mtd, chip);

	return status & NAND_STATUS_FAIL ? -EIO : 0;
}

int mc_spi_nfc_hw_ecc_ctrl_init(struct mtd_info *mtd,
				       struct nand_ecc_ctrl *ecc)
{
	struct nand_ecclayout *layout;
	int nsectors;
	int noobsize;
	int i, j;
	int ret;

	ret = mc_spi_nfc_hw_common_ecc_ctrl_init(mtd, ecc);
	if (ret)
		return ret;

	ecc->read_page_raw = mc_spi_nfc_read_page_raw;
	ecc->write_page_raw = mc_spi_nfc_write_page_raw;
	ecc->read_page = mc_spi_nfc_hw_ecc_read_page;
	ecc->write_page = mc_spi_nfc_hw_ecc_write_page;
	ecc->read_subpage = mc_spi_nfc_hw_ecc_read_subpage;
	ecc->write_subpage = mc_spi_nfc_hw_ecc_write_subpage;
	ecc-> write_oob = mc_spi_nand_write_oob;
	layout = ecc->layout;
	nsectors = mtd->writesize / ecc->size;
	noobsize = mtd->oobsize / nsectors;

	for (i = 0; i < nsectors; i++) {
		if (i) {
			layout->oobfree[i].offset =
				layout->oobfree[i - 1].offset +
				layout->oobfree[i - 1].length +
				ecc->bytes;
			layout->oobfree[i].length = noobsize - ecc->bytes;;
		} else {
			/*
			 * The first 2 bytes are used for BB markers, hence we
			 * only have 2 bytes available in the first user data
			 * section.
			 */
			layout->oobfree[i].length = noobsize - 2 - ecc->bytes;
			layout->oobfree[i].offset = 2;
		}

		for (j = 0; j < ecc->bytes; j++)
			layout->eccpos[(ecc->bytes * i) + j] =
					layout->oobfree[i].offset +
					layout->oobfree[i].length + j;
	}

	return 0;
}


int mc_snfc_ecc_init(struct mtd_info *mtd, struct nand_ecc_ctrl *ecc)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	int ret;

	if (!ecc->size) {
		ecc->size = nand->ecc_step_ds;
		ecc->strength = nand->ecc_strength_ds;
	}

	if (!ecc->size || !ecc->strength)
		return -EINVAL;

	switch (ecc->mode) {
	case NAND_ECC_SOFT_BCH:
		break;
	case NAND_ECC_HW:
		ret = mc_spi_nfc_hw_ecc_ctrl_init(mtd, ecc);
		if (ret)
			return ret;
		break;
	case NAND_ECC_HW_SYNDROME:
		break;
	case NAND_ECC_NONE:
		ecc->layout = kzalloc(sizeof(*ecc->layout), GFP_KERNEL);
		if (!ecc->layout)
			return -ENOMEM;
		ecc->layout->oobfree[0].length = mtd->oobsize;
	case NAND_ECC_SOFT:
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

int mc_snfc_host_init(struct mc_snfc_host *host)
{
	unsigned int reg;

	host->regbase = (void __iomem *)SPI_NFC_REG_BASE;
	host->mbuf = (void __iomem *)(host->regbase + SPI_NFC_DATA_BUFFER);
	host->sbuf = (void __iomem *)(host->regbase + SPI_NFC_SPARE_BUFFER);

	reg = mc_snfc_read(host, SPI_NFC_SSIENR);
	reg = (unsigned int)~SSIENR_SSIEN;
	mc_snfc_write(host, reg, SPI_NFC_SSIENR);
	reg = mc_snfc_read(host, SPI_NFC_CTRLR0);
	reg |= CTRLR0_SCPH | CTRLR0_SCPOL;
	mc_snfc_write(host, reg, SPI_NFC_CTRLR0);
	mc_snfc_write(host, 0, SPI_NFC_IMR);
	mc_snfc_write(host, 4, SPI_NFC_BAUDR);
	mc_snfc_write(host, SFC_SAMPLE_DELAY, SPI_NFC_RX_SAMPLE_DLY);
	reg = mc_snfc_read(host, SPI_NFC_CCFGR);
	reg &= (u32)~(SFC_CCFGR_DUMBYTE_MASK);
	mc_snfc_write(host, reg, SPI_NFC_CCFGR);
	reg = SSIENR_SSIEN;
	mc_snfc_write(host, reg, SPI_NFC_SSIENR);

	host->send_cmd_pageprog = mc_send_cmd_pageprog;
	host->send_cmd_status = mc_send_cmd_status;
	host->send_cmd_readstart = mc_send_cmd_readstart;
	host->send_cmd_erase = mc_send_cmd_erase;
	host->send_cmd_readid = mc_send_cmd_readid;
	host->send_cmd_reset = mc_send_cmd_reset;

	return 0;
}

void mc_spi_nfc_probe(struct nand_chip *chip, unsigned char cs)
{
	int ret = 0;
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct mc_snfc_host *host = &mc_snfc_host;

	if (!host->chip) {

		memset((char *)host, 0, sizeof(struct mc_snfc_host));
		mc_spi_nfc_enable_clk();
		mc_spi_nfc_sel_clk();
		ret = mc_snfc_host_init(host);
		if (ret) {
			printf("Error: Host init failed, result: %d\n", ret);
			return;
		}
	} else {
		printf("*-SPI Nand host is initialized.\n");
	}
	host->option = SNFC_HWCAPS_READ_1_1_4;
	host->chipselect = cs;
	host->chip = chip;
	mc_spi_nand_init(host);

	/* identify chip */
	ret = nand_scan_ident(mtd, CONFIG_SYS_NAND_MAX_CHIPS, NULL);
	if (ret) {
		printf("nand_scan_ident returned %i", ret);
		return;
	}
	host->main_size = mtd->writesize;
	host->spare_size = mtd->oobsize;

	ret = mc_snfc_ecc_init(mtd, &chip->ecc);
	if (ret) {
		printf("ECC init failed: %d\n", ret);
		return;
	}

	/* finish scanning the chip */
	ret = nand_scan_tail(mtd);
	if (ret) {
		printf("nand_scan_tail returned %i", ret);
		return;
	}

	/* chip is good, register it */
	ret = nand_register(0, mtd);
	if (ret)
		printf("nand_register returned %i", ret);

	printf("*-End SPI Nand flash driver probe.\n");

	return;
}

int mc_spi_nand_pre_probe(struct nand_chip *chip)
{
	uint8_t nand_maf_id;
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct mc_snfc_host *host = chip->priv;

	chip->select_chip(mtd, 0);
	/* Reset the chip first */
	host->send_cmd_reset(host);
	chip->dev_ready(mtd);

	/* Check the ID */
	host->offset = 0;
	memset((unsigned char *)(chip->IO_ADDR_R), 0, 0x10);
	host->send_cmd_readid(host);
	nand_maf_id = readb(chip->IO_ADDR_R);
	spi_nand_lock_block(host, BL_ALL_UNLOCKED);
	spi_nand_ecc_disable(host);
	chip->select_chip(mtd, -1);

	if (nand_maf_id == 0x00 || nand_maf_id == 0xff) {
		 printf("Cannot found a valid SPI Nand Device\n");
		 return 1;
	}

	return 0;
}
static struct nand_chip mc_chip;

void board_nand_init(void)
{

	unsigned char chip_num = CONFIG_SPI_NAND_MAX_CHIP_NUM;

#ifndef CONFIG_FLASH_NOR_NAND_COEXISTENCE
	static unsigned char cs;

	for (cs = 0; chip_num && (cs < 1); cs++) {
		mc_spi_nfc_probe(&mc_chip, cs);
		chip_num--;
	}
#else
	mc_spi_nfc_probe(&mc_chip, 1);
	chip_num--;
#endif
	if (chip_num)
		return;

	if (mc_spi_nand_pre_probe(&mc_chip))
		return;

}


