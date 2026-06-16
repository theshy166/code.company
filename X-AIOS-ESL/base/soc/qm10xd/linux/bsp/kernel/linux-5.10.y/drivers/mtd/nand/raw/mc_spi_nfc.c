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

#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/module.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include "spi_nand.h"
#include "mc_spi_nfc.h"
#include <linux/mini_clock/clock.h>

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

static inline void mc_spi_nfc_start_cmd(struct mc_snfc_host *host)
{
#ifdef CONFIG_PM_SLEEP
   host->ctrl_sus = CTRL_START;
#endif
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
	udelay(1);
}

int mc_sfc_wait_fbuf2_sta(struct mc_snfc_host *host)
{
    unsigned int val;
	
	val = mc_snfc_read(host, REG_NFC_BUF_SOFT);
	if (val & NFC_SOFT_FB_P2_RDY_BIT) {
		return 0;
	}
		
	return 1;
}

static void mc_spi_nfc_cs_select(struct mtd_info *mtd, u32 en)
{
	u32 reg;
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;

	mc_snfc_write(host, ~(unsigned int)SSIENR_SSIEN, SPI_NFC_SSIENR);
	reg = mc_snfc_read(host,  SPI_NFC_SER);
	if (en)
		reg = SER_SELECT(priv->chipselect);
	else
		reg &= ~(SER_SELECT(priv->chipselect));
	mc_snfc_write(host, reg, SPI_NFC_SER);
	mc_snfc_write(host, SSIENR_SSIEN, SPI_NFC_SSIENR);

}

static int wait_op_finish(struct mc_snfc_host *host)
{
	unsigned regval, timeout = MC_WAIT_TIMEOUT;

	do {
		udelay(1);
		regval = mc_snfc_read(host, REG_NFC_INTR_RAW);
		--timeout;
	} while ((!(regval & SPI_FINISH) && timeout));
	if (!timeout) {
		dev_err(host->dev, " Wait SPI NFC finish timeout!\n");
#ifdef CONFIG_PM_SLEEP
		host->ctrl_sus = CTRL_END;
#endif
		return -ETIMEDOUT;
	} else {
		mc_snfc_write(host, regval, REG_NFC_INTR_RAW);
#ifdef CONFIG_PM_SLEEP
		host->ctrl_sus = CTRL_END;
#endif
		return 0;
	}
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
		return;
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

static void spi_nand_read_page_to_cache(struct mc_snfc_host *host)
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

static void spi_nand_read_from_cache(struct mc_snfc_host *host, struct nand_chip *chip)
{
	unsigned int data_size;
	unsigned char addr[4];
	unsigned int cmd, xf_mode;

	memset(addr, 0, sizeof(addr));

	addr[0] = (u8)(host->column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
		addr[0] |= (u8)(((host->page_addr >>
				      (chip->phys_erase_shift - chip->page_shift)) & 0x1) << 4);
	addr[1] = (u8)host->column;
	addr[2] = 0;

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

static void spi_nand_program_data_to_cache(struct mc_snfc_host *host, struct nand_chip *chip, 
							struct mtd_info *mtd,  const uint8_t *buf, int oob_required)
{
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
		chip->legacy.write_buf(chip, buf, mtd->writesize);
	if (oob_required)
		chip->legacy.write_buf(chip, chip->oob_poi, mtd->oobsize);

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

void mc_send_cmd_readid(struct mc_snfc_host *host)
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

static int spi_nand_wait(struct mc_snfc_host *host, unsigned char *s)
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

static void mc_send_cmd_erase(struct mc_snfc_host *host)
{
	u8 addr[4];
	unsigned char status;
	int ret;

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
    ret = spi_nand_wait(host, &status);
	if(ret) {
		dev_err(host->dev, "Error2: erase is error! status:%x\n",
				status);
	}

}

static unsigned char mc_read_byte(struct nand_chip *chip)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
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
/*
static unsigned short mc_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
	unsigned int value, offset, shift;
	unsigned short ret_val = 0;

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
*/
static void mc_write_buf(struct nand_chip *chip,
	const u_char *buf, int len)
{
	unsigned int i, total, ofs;
	unsigned int size;
	void __iomem *dest;
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;

	mc_sfc_wait_fbuf2_ready(host);

	if(len & 0x3) {
		dev_warn(host->dev, "data len is not multiple 4\n");
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
			ofs = size * i;
			memcpy(dest, buf + ofs, size);
			dest += 0x40;
		}
	} else {
		host->mbuf_writed = 1;
		size = len >> 2 << 2;
		dest = host->mbuf;
		memcpy(dest, buf, size);
	}

}

static void mc_read_buf(struct nand_chip *chip, u_char *buf, int len)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
	unsigned int i, total, ofs;
	unsigned int size;
	void __iomem * src;

	if(len & 0x3) {
		dev_warn(host->dev, "data len is not multiple 4\n");
	}

	mc_sfc_wait_fbuf2_ready(host);

	if (buf == chip->oob_poi) {
		total = 1 << (chip->page_shift - 10);
		len = len / total;
		size = len >> 2 << 2;
		src = host->sbuf;

		for(i = 0; i < total; i++) {
			ofs = size * i;
			memcpy(buf + ofs, src, size);
			src += 0x40;
		}
	} else {
		src = host->mbuf;
		size = len >> 2 << 2;
		memcpy(buf, src, size);
	}
}

static void mc_select_chip(struct nand_chip *chip, int chipselect)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
    struct mtd_info *mtd = &chip->base.mtd;

	if (chipselect < 0) {
		mc_spi_nfc_cs_select(mtd, false);
		spin_unlock(&spinlock_nor_nand);
		return;
	}

	if (chipselect > MC_MAX_NAND_CHIP_NUM)
		dev_err(host->dev, "Invalid chipselect: %d\n", chipselect);

	spin_lock(&spinlock_nor_nand);
	mc_spi_nfc_cs_select(mtd, true);
	if (host->mtd != mtd) {
		host->mtd = mtd;
	}
}

static void mc_cmdfunc(struct nand_chip *chip, unsigned int command,
				int column, int page_addr)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;

	switch (command) {
	case NAND_CMD_RESET:
		host->send_cmd_reset(host);
		chip->legacy.dev_ready(chip);
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
		host->send_cmd_readstart(host, chip);
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
		dev_warn(host->dev, "%s not support command 0x%08x:\n", chip->base.mtd.name, command);
		break;
	}

}

static int mc_dev_ready(struct nand_chip *chip)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
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

	dev_err(host->dev, "device is busy\n");

	return 0;
}

static void mc_spi_nfc_cmd_ctrl(struct nand_chip *chip, int dat, unsigned int ctrl)
{

	/*this is null*/

}

void mc_spi_nand_init(struct nand_chip *chip)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;

	if (!chip->legacy.IO_ADDR_R)
		chip->legacy.IO_ADDR_R = (void __iomem *)host->mbuf;
	chip->legacy.IO_ADDR_W = chip->legacy.IO_ADDR_R;
	memset((char *)chip->legacy.IO_ADDR_R, 0xff, SPI_NAND_MAX_PAGESIZE);

	chip->legacy.read_byte = mc_read_byte;
	chip->legacy.write_buf = mc_write_buf;
	chip->legacy.read_buf = mc_read_buf;

	chip->legacy.select_chip = mc_select_chip;

	chip->legacy.cmdfunc = mc_cmdfunc;
	chip->legacy.cmd_ctrl = mc_spi_nfc_cmd_ctrl;
	chip->legacy.dev_ready = mc_dev_ready;

	chip->legacy.chip_delay = MC_CHIP_DELAY;

	chip->options = NAND_SKIP_BBTSCAN | NAND_NO_SUBPAGE_WRITE | NAND_BROKEN_XD;

	//chip->ecc.layout = NULL;
	chip->ecc.mode = NAND_ECC_HW;
}

static void mc_send_cmd_pageprog(struct mc_snfc_host *host, struct nand_chip *chip,
					struct mtd_info *mtd,  const uint8_t *buf, int oob_required)
{

	spi_nand_write_enable(host);

	spi_nand_program_data_to_cache(host, chip, mtd, buf, oob_required);

	spi_nand_program_execute(host);

}

void spi_nand_write_feature(struct mc_snfc_host *host, u8 reg, u8 val)
{
	unsigned int value;

	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_WRITE_REG);
	mc_spi_nfc_addr(host, &reg, 1);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	mc_sfc_wait_fbuf2_ready(host);
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

static void mc_send_cmd_readstart(struct mc_snfc_host *host, struct nand_chip *chip)
{
	unsigned char status;
	int ret;

	spi_nand_read_page_to_cache(host);

	ret = spi_nand_wait(host, &status);
	if(ret) {
		dev_err(host->dev, "spi nand wait error! status:%x\n",
				status);
	}

	spi_nand_read_from_cache(host, chip);

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

static void mc_spi_nfc_hw_ecc_enable(struct mtd_info *mtd, int todev)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
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

static void mc_spi_nfc_hw_ecc_disable(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
	
	mc_sfc_ecc_enable(host, NFC_DISABLE);
}

static int mc_snfc_hw_common_ecc_read_oob(struct nand_chip *chip,
					    int page)
{
	chip->legacy.cmdfunc(chip, NAND_CMD_READ0, 0, page);

	chip->pagecache.page = -1;

	return chip->ecc.read_page(chip, chip->data_buf, 1, page);
}

static int mc_snfc_hw_common_ecc_write_oob(struct nand_chip *chip,
					     int page)
{
	int ret, status;

	chip->legacy.cmdfunc(chip, NAND_CMD_SEQIN, 0, page);

	chip->pagecache.page = -1;

	memset(chip->data_buf, 0xff, chip->base.mtd.writesize);
	ret = chip->ecc.write_page(chip, chip->data_buf, 1, page);
	if (ret)
		return ret;

	/* Send command to program the OOB data */
	chip->legacy.cmdfunc(chip, NAND_CMD_PAGEPROG, -1, -1);

	status = chip->legacy.waitfunc(chip);

	return status & NAND_STATUS_FAIL ? -EIO : 0;
}

static int mc_spi_nand_ooblayout_ecc(struct mtd_info *mtd, int section,
				    struct mtd_oob_region *oobregion)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct nand_ecc_ctrl *ecc = &nand->ecc;
	struct mc_snfc_hw_ecc *data = ecc->priv;

	if (section >= ecc->steps)
		return -ERANGE;

	oobregion->offset = section * (ecc->bytes + data->start_pos) + data->start_pos;
	oobregion->length = ecc->bytes;

	return 0;
}


static int mc_spi_nand_ooblayout_free(struct mtd_info *mtd, int section,
				     struct mtd_oob_region *oobregion)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct nand_ecc_ctrl *ecc = &nand->ecc;
	int nsectors;
	int noobsize;

	if (section >= ecc->steps)
		return -ERANGE;

	nsectors = mtd->writesize / ecc->size;
	noobsize = mtd->oobsize / nsectors;

	/*
	 * The first 2 bytes are used for BB markers, hence we
	 * only have 2 bytes available in the first user data
	 * section.
	 */
	if (!section && ecc->mode == NAND_ECC_HW) {
		oobregion->offset = 2;
		oobregion->length = noobsize - ecc->bytes - 2;

		return 0;
	}

	oobregion->offset =  mtd->oobsize - section * noobsize;
	oobregion->length = noobsize - ecc->bytes;

	return 0;
}

static const struct mtd_ooblayout_ops mc_spi_nand_ooblayout_ops = {
	.ecc = mc_spi_nand_ooblayout_ecc,
	.free = mc_spi_nand_ooblayout_free,
};

static int mc_spi_nfc_hw_common_ecc_ctrl_init(struct mtd_info *mtd,
					      struct nand_ecc_ctrl *ecc)
{
	static const u8 strengths[] = { 8, 24};
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
	struct mc_snfc_hw_ecc *data;
	int nsectors, noobsize;
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
		dev_err(host->dev, "unsupported strength\n");
		ret = -ENOTSUPP;
		goto err;
	}

	data->mode = i;

	/* HW ECC always request ECC bytes for 1024 bytes blocks */
	ecc->bytes = DIV_ROUND_UP(ecc->strength * fls(8 * 1024), 8);

	/* HW ECC always work with even numbers of ECC bytes */
	ecc->bytes = ALIGN(ecc->bytes, 2);

	nsectors = mtd->writesize / ecc->size;
	noobsize = mtd->oobsize / nsectors;

	if (mtd->oobsize < ((ecc->bytes + 4) * nsectors)) {
		ret = -EINVAL;
		goto err;
	}

	ecc->read_oob = mc_snfc_hw_common_ecc_read_oob;
	ecc->write_oob = mc_snfc_hw_common_ecc_write_oob;
	mtd_set_ooblayout(mtd, &mc_spi_nand_ooblayout_ops);

	ecc->priv = data;

	data->info_size = noobsize - ecc->bytes;
	data->start_pos = data->info_size;

	return 0;

err:
	kfree(data);

	return ret;
}


static void mc_spi_nand_hw_common_ecc_ctrl_cleanup(struct nand_ecc_ctrl *ecc)
{
	kfree(ecc->priv);
}

#define BITS_PER_BYTE 8

static int nand_check_erased_buf(void *buf, int len, int bitflips_threshold)
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

static int nand_check_ecc_chunk(void *data, int datalen, void *oob,
					int ooblen, int bitflips_threshold)
{
	int data_bitflips = 0, oob_bitflips = 0;
	int bit_corrected = 0;

	data_bitflips = nand_check_erased_buf(data, datalen, bitflips_threshold);
	if (data_bitflips < 0) {
			printk("data bitflips exceed thresholds %d\n", bitflips_threshold);
		return data_bitflips;
	}

	bitflips_threshold -= data_bitflips;
	bit_corrected += data_bitflips;

	oob_bitflips = nand_check_erased_buf(oob, ooblen, bitflips_threshold);
	if (oob_bitflips < 0) {
			printk("oob bitflips exceed thresholds %d\n", bitflips_threshold);
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

int mc_spi_nfc_hw_ecc_read_page(struct nand_chip *chip, uint8_t *buf,
					int oob_required, int page)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
	struct nand_ecc_ctrl *ecc = &chip->ecc;
    struct mtd_info *mtd = &chip->base.mtd;
	unsigned int data_size, ecc_info, max_bitflips = 0;
	int ret, i;
	unsigned char status;
	unsigned char column[4] = {0};
	unsigned int cmd, xf_mode;

	host->page_addr = page;
	spi_nand_read_page_to_cache(host);

	ret = spi_nand_wait(host, &status);
	if(ret) {
		dev_err(host->dev, "Error: spi nand wait error! status:%x\n",
				status);
	}
	spi_nand_write_feature(host, REG_OTP, 0x9);
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

	if (chip->options & SPINAND_NEED_PLANE_SELECT){
		column[0] |= (u8)(((host->page_addr >>
					  (chip->phys_erase_shift - chip->page_shift)) & 0x1) << 4);
	}

	mc_spi_nfc_cmd(host, cmd);
	mc_spi_nfc_addr(host, column, 3);
	mc_spi_nfc_rtx_dir(host, CMD_READ);

	data_size = host->main_size;
	mc_spi_nfc_data_len(host, data_size);
	mc_spi_nfc_hw_ecc_enable(mtd, CMD_READ);
	//data_size = host->spare_size / (host->main_size >> 10);
	mc_spi_nfc_spare_len(host, host->spare_size);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR_DATA, xf_mode);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_SPARE);

	//mc_spi_nfc_hw_ecc_enable(mtd, CMD_READ);
	mc_spi_nfc_start_ops(host);
	
	mc_sfc_wait_fbuf2_ready(host);

	ecc_info = mc_snfc_read(host, REG_NFC_ECC_INFO0);
	for (i = 0; i < ecc->steps; i++) {
		unsigned int errno;
		void *databuf, *oobbuf;
		int datalen, ooblen;
		status = (ecc_info >> (i << 1)) & 0x3;
		if (status == ECC_UNCORRECTED) {
			databuf = host->mbuf + (i << 10);
			datalen = 1024;
			oobbuf = host->sbuf + (i << 6);
			ooblen = host->spare_size / ecc->steps;
			ret = nand_check_ecc_chunk(databuf, datalen, oobbuf,
						ooblen, ecc->strength);
			if (ret < 0) {
				dev_err( host->dev, "page %x bitflips exceed the ecc strength\n", page);
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

		chip->legacy.read_buf(chip, buf, mtd->writesize);
		if (oob_required)
			chip->legacy.read_buf(chip, chip->oob_poi, mtd->oobsize);

	mc_spi_nfc_hw_ecc_disable(mtd);

	return max_bitflips;
}

static int mc_spi_nfc_hw_ecc_read_subpage(struct nand_chip *chip,
					 uint32_t data_offs, uint32_t readlen,
					 uint8_t *bufpoi, int page)
{

	return 0;
}

int mc_spi_nfc_hw_ecc_write_page(struct nand_chip *chip,
				       const uint8_t *buf, int oob_required,
				       int page)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
    struct mtd_info *mtd = &chip->base.mtd;
	unsigned char addr[4];
	unsigned int data_size;
	unsigned char status;
	int ret;

	spi_nand_write_enable(host);
	mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_rtx_dir(host, CMD_WRITE);
	chip->legacy.write_buf(chip, buf, mtd->writesize);
	if (oob_required)
		chip->legacy.write_buf(chip, chip->oob_poi, mtd->oobsize);
	else
		memset(host->sbuf, 0xff, SPI_NAND_MAX_OOBSIZE);

	memset(addr, 0, sizeof(addr));

	if((page /0x40 % 2)&&(chip->options & SPINAND_NEED_PLANE_SELECT_MXIC)){
		if(mtd->writesize == SZ_4K)
			addr[0] = 0x20;
		else if(mtd->writesize == SZ_2K)
			addr[0] = 0x10;
	}

	//mc_spi_nfc_cmd_init(host);
	mc_spi_nfc_cmd(host, SPINAND_CMD_PROG_LOAD);
	mc_spi_nfc_addr(host, addr, 2);
	//mc_spi_nfc_rtx_dir(host, CMD_WRITE);

	if (chip->options & SPINAND_NEED_PLANE_SELECT)
		addr[0] |= (u8)(((host->page_addr >>
					  (chip->phys_erase_shift - chip->page_shift)) & 0x1) << 4);

	data_size = host->main_size;
	mc_spi_nfc_data_len(host, data_size);
	//data_size = host->spare_size / (host->main_size >> 10);
	mc_spi_nfc_spare_len(host, host->spare_size);
	mc_sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_2ADDR, VLD_CMD_ADDR_DATA, XF_MODE_SPI);
	mc_spi_nfc_data_type(host, DATA_TYPE_DATA_SPARE);

	mc_spi_nfc_hw_ecc_enable(mtd, CMD_WRITE);

	mc_spi_nfc_start_ops(host);
	mc_spi_nfc_hw_ecc_disable(mtd);

	host->page_addr = page;
	spi_nand_program_execute(host);
    ret = spi_nand_wait(host, &status);
	if(ret) {
		dev_err(host->dev, "Error1: spi nand write is error! status:%x\n",
				status);
	}

	return 0;
}

static int mc_spi_nfc_hw_ecc_write_subpage(struct nand_chip *chip,
					  u32 data_offs, u32 data_len,
					  const u8 *buf, int oob_required,
					  int page)
{

	return 0;
}

int mc_spi_nfc_read_page_raw(struct nand_chip *chip,
	  uint8_t *buf, int oob_required, int page)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
    struct mtd_info *mtd = &chip->base.mtd;

	mc_send_cmd_readstart(host, chip);
	chip->legacy.read_buf(chip, buf, mtd->writesize);
	if (oob_required)
		chip->legacy.read_buf(chip, chip->oob_poi, mtd->oobsize);

	return 0;

}
int mc_spi_nfc_write_page_raw(struct nand_chip *chip,
	  const uint8_t *buf, int oob_required, int page)
{
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
    struct mtd_info *mtd = &chip->base.mtd;

	/*chip->legacy.write_buf(chip, buf, mtd->writesize);
	if (oob_required)
	  chip->legacy.write_buf(chip, chip->oob_poi, mtd->oobsize);*/
	mc_send_cmd_pageprog(host, chip, mtd, buf, oob_required);

	return 0;
}

int mc_spi_nand_write_oob(struct nand_chip *chip,
				int page)
{
	int status = 0;
	//const uint8_t *buf = chip->oob_poi;
	//int length = chip->base.mtd.oobsize;
	struct mc_snfc_priv *priv = chip->priv;
	struct mc_snfc_host *host = priv->host;
	struct mtd_info *mtd = &chip->base.mtd;

	//chip->legacy.write_buf(chip, buf, length);
	/* Send command to program the OOB data */
	mc_send_cmd_pageprog(host, chip, mtd, NULL, 1);

	status = chip->legacy.waitfunc(chip);

	return status & NAND_STATUS_FAIL ? -EIO : 0;
}

static int mc_spi_nfc_hw_ecc_ctrl_init(struct mtd_info *mtd,
				       struct nand_ecc_ctrl *ecc)
{
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
	ecc->write_oob = mc_spi_nand_write_oob;

	return 0;
}


static int mc_snfc_ecc_init(struct mtd_info *mtd, struct nand_ecc_ctrl *ecc)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	int ret;

	if (!ecc->size) {
		ecc->size = nand->base.ecc.requirements.step_size;
		ecc->strength = nand->base.ecc.requirements.strength;
	}
	if (!ecc->size || !ecc->strength)
		return -EINVAL;

	switch (ecc->mode) {
	case NAND_ECC_HW:
		ret = mc_spi_nfc_hw_ecc_ctrl_init(mtd, ecc);
		if (ret)
			return ret;
		break;
	case NAND_ECC_HW_SYNDROME:
	case NAND_ECC_NONE:
	case NAND_ECC_SOFT:
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static void mc_spi_nand_ecc_cleanup(struct nand_ecc_ctrl *ecc)
{
	switch (ecc->mode) {
	case NAND_ECC_HW:
	case NAND_ECC_HW_SYNDROME:
		mc_spi_nand_hw_common_ecc_ctrl_cleanup(ecc);
		break;
	case NAND_ECC_NONE:
	default:
		break;
	}
}
static int mc_spi_nand_attach_chip(struct nand_chip *chip)
{
    struct mtd_info *mtd = nand_to_mtd(chip);
    int ret;

	ret = mc_snfc_ecc_init(mtd, &chip->ecc);
	if (ret) {
		printk("ECC init failed: %d\n", ret);
		return ret;
	}

    return 0;
}

static const struct nand_controller_ops mc_spi_nand_controller_ops = {
	.attach_chip = mc_spi_nand_attach_chip,
};

int mc_snfc_host_init(struct mc_snfc_host *host)
{
	unsigned int reg;

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

	host->option = SNFC_HWCAPS_READ_1_1_4;

	return 0;
}

/**
 * Get spi flash device information and register it as a mtd device.
 */
int mc_spi_nfc_register(struct device_node *np,
				struct mc_snfc_host *host)
{
	struct device *dev = host->dev;
	struct nand_chip *chip;
	struct mtd_info *mtd;
	struct mc_snfc_priv *priv;

	int ret;

	chip = devm_kzalloc(dev, sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	chip->priv = priv;
	priv->host = host;
	host->chip[host->num_chip] = chip;
	mtd = nand_to_mtd(chip);

	mtd_set_of_node(mtd, np);

	ret = of_property_read_u32(np, "reg", &priv->chipselect);
	if (ret) {
		dev_err(dev, "There's no reg property for %pOF\n",
			np);
		return ret;
	}

	mtd->name = np->name;
	mtd->owner = THIS_MODULE;
	//mtd_set_ooblayout(mtd, &mc_ooblayout_ops);

	ret = mc_snfc_host_init(host);
	if (ret)
		return ret;

	mc_spi_nand_init(chip);
	mc_select_chip(chip, priv->chipselect);
	spi_nand_lock_block(host, BL_ALL_UNLOCKED);
	spi_nand_ecc_disable(host);
    chip->legacy.dummy_controller.ops = &mc_spi_nand_controller_ops;

    ret = nand_scan_with_ids(chip, 1, NULL);
    if (ret){
        return ret;
    }

	host->main_size = mtd->writesize;
	host->spare_size = mtd->oobsize;

	ret = mtd_device_register(mtd, NULL, 0);
	if (ret) {
		dev_err(dev, "spi nand mtd device register fail\n");
		return ret;
	}

	host->num_chip++;

	return 0;
}

static void mc_spi_nfc_unregister_all(struct mc_snfc_host *host)
{
	int i;

	for (i = 0; i < host->num_chip; i++) {
		mtd_device_unregister(&host->chip[i]->base.mtd);
		mc_spi_nand_ecc_cleanup(&host->chip[i]->ecc);
	}
}

static int mc_spi_nfc_register_all(struct mc_snfc_host *host)
{
	struct device *dev = host->dev;
	struct device_node *np;
	int ret;

	for_each_available_child_of_node(dev->of_node, np) {
		ret = mc_spi_nfc_register(np, host);
		if (ret)
			goto fail;

		if (host->num_chip == MC_MAX_NAND_CHIP_NUM) {
			dev_warn(dev, "Flash device number exceeds the maximum chipselect number\n");
			break;
		}
	}

	return 0;

fail:
	mc_spi_nfc_unregister_all(host);
	return ret;
}
#if 0
static int mc_spi_nfc_get_clk(struct mc_snfc_host *host)
{

	host->apb_spic = devm_clk_get(host->dev, "apb_spci");
	if (IS_ERR(host->apb_spic))
		return PTR_ERR(host->apb_spic);

	host->ahb_spic = devm_clk_get(host->dev, "ahb_spic");
	if (IS_ERR(host->ahb_spic))
		return PTR_ERR(host->ahb_spic);

#ifdef CONFIG_MTD_SPI_NAND_MC_SEP_REFCLK
	host->spic_ssi = devm_clk_get(host->dev, "spic_ssi");
	if (IS_ERR(host->spic_ssi))
		return PTR_ERR(host->spic_ssi);
#endif
	host->clk = devm_clk_get(host->dev, "spic_clk");
	if (IS_ERR(host->clk))
		return PTR_ERR(host->clk);

	return 0;

}

static int mc_spi_nfc_enable_clk(struct mc_snfc_host *host)
{
	int ret;

	ret = clk_mini_set_rate(CKG_MUX_SFC, 400000000);//spic clk source is 200M
	if (ret)
		return ret;

	ret = clk_mini_enable(CKG_APB_SFC_EN);
	if (ret)
		return ret;

	ret = clk_mini_enable(CKG_SFC_EN);
	if (ret) {
		clk_mini_disable(CKG_APB_SFC_EN);
		return ret;
	}

#ifdef CONFIG_MTD_SPI_NAND_MC_SEP_REFCLK
	if (host->spic_ssi) {
		ret = clk_prepare_enable(host->spic_ssi);
		if (ret) {
			clk_disable_unprepare(host->apb_spic);
			clk_disable_unprepare(host->ahb_spic);
			return ret;
		}
	}
#endif
	return 0;
}

static void mc_spi_nfc_disable_clk(struct mc_snfc_host *host)
{
#ifdef CONFIG_MTD_SPI_NAND_MC_SEP_REFCLK
	if (host->spic_ssi)
		clk_disable_unprepare(host->spic_ssi);
#endif

	clk_mini_disable(CKG_SFC_EN);

	clk_mini_disable(CKG_APB_SFC_EN);
}
#endif
int mc_spi_nfc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct resource *res;
	struct mc_snfc_host *host;
	int ret;

	host = devm_kzalloc(dev, sizeof(*host), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

	platform_set_drvdata(pdev, host);
	host->dev = dev;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "control");
	host->regbase = devm_ioremap_resource(dev, res);
	if (IS_ERR(host->regbase))
		return PTR_ERR(host->regbase);
#ifdef CONFIG_PM_SLEEP
	device_property_read_u32(host->dev, "waitting-time-suspend", &host->sus_timeout);
	device_property_read_u32(host->dev, "force-suspend", &host->force_suspend);
#endif
	ret = of_property_read_u32(dev->of_node, "spi-max-frequency",
			&host->clkrate);
	if (ret) {
		dev_err(dev, "There's no spi-max-frequency property for %s\n",
			dev->of_node->full_name);
		return ret;
	}

	clk_mini_enable(CKG_APB_SFC_EN);
	clk_mini_enable(CKG_SFC_EN);
	ret = clk_mini_set_rate(CKG_MUX_SFC,host->clkrate);
	if (ret){
		dev_err(host->dev, "failed to set rate %uHz\n", host->clkrate);
		return ret;
	}
	ret = mc_spi_nfc_register_all(host);
	dev_set_drvdata(dev, host);
	return ret;
}

static int mc_spi_nfc_remove(struct platform_device *pdev)
{
	struct mc_snfc_host *host = platform_get_drvdata(pdev);

	mc_spi_nfc_unregister_all(host);
	clk_mini_disable(CKG_APB_SFC_EN);
	clk_mini_disable(CKG_SFC_EN);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
/*
Reserve power off interface
*/

static int nand_molchip_suspend(struct device *dev)
{
	struct mc_snfc_host *host = dev_get_drvdata(dev);
	u32 timeout = SUS_DEFAULT_TIMEOUT;
	u8 status = 0xff;
	u32 ret = 0;

	if(host->sus_timeout)
		timeout = host->sus_timeout;

	//wait sfc controller idle
	while(timeout){
		if(host->ctrl_sus == CTRL_END){
			break;
		}
		mdelay(1);
		timeout--;
	}
	if((!timeout) && (!(host->force_suspend)) ){
		printk("nand suspend timeout\r\n");
		return -EBUSY;
	}

	//pull up CS
	mc_snfc_write(host, 0, SPI_NFC_SSIENR);
	mc_snfc_write(host, 1, SPI_NFC_SER);

	//wait nand flash idle
	ret = spi_nand_wait(host,&status);
	if( (ret) && (!(host->force_suspend)) ){
		printk("nand flash id busy\r\n");
		return -EBUSY;
	}

	//clk disable
	clk_mini_disable(CKG_APB_SFC_EN);
	clk_mini_disable(CKG_SFC_EN);

	printk("nand_molchip_suspend done\r\n");
	return 0;
}

static int nand_molchip_resume(struct device *dev)
{
	struct mc_snfc_host *host = dev_get_drvdata(dev);
	clk_mini_set_rate(CKG_MUX_SFC, 400000000);
	clk_mini_enable(CKG_APB_SFC_EN);
	clk_mini_enable(CKG_SFC_EN);
	mc_snfc_host_init(host);
	printk("nand_molchip_resume done\r\n");
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(nand_molchip_pm_ops, nand_molchip_suspend,
			 nand_molchip_resume);

static const struct of_device_id mc_spi_nfc_dt_ids[] = {
	{ .compatible = "mc, spi-nand-controller"},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mc_spi_nfc_dt_ids);

static struct platform_driver mc_spi_nfc_driver = {
	.driver = {
		.name	= "mc-spi-nfc",
		.pm	= &nand_molchip_pm_ops,
		.of_match_table = mc_spi_nfc_dt_ids,
	},
	.probe	= mc_spi_nfc_probe,
	.remove	= mc_spi_nfc_remove,
};
module_platform_driver(mc_spi_nfc_driver);

MODULE_DESCRIPTION("MC SPI NAND Controller");
MODULE_LICENSE("GPL v2");


