/*
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/bitops.h>
#include <linux/dma-mapping.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/spi-nor.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include "mc-sfc.h"
#include "core.h"
#ifdef SFC_CLK_FB
#include <linux/mini_clock/clock.h>
#else
#include <linux/clk.h>
#endif


int mc_clk_get(struct mc_sfc_host *host)
{
#ifdef SFC_CLK_FB
	int ret;
	ret = clk_mini_enable(CKG_SFC_EN);
	if(ret)
		return -1;
	ret = clk_mini_enable(CKG_APB_SFC_EN);
	if(ret)
		return -1;
#else
	host->apb_spic = devm_clk_get(host->dev, "apb_sfc");
	if (IS_ERR(host->apb_spic)) {
		return PTR_ERR(host->apb_spic);
    }
	host->ahb_spic = devm_clk_get(host->dev, "clk_sfc");
	if (IS_ERR(host->ahb_spic)) {
		return PTR_ERR(host->ahb_spic);
    }
#endif
	return 0;
}

inline int wait_op_finish(struct mc_sfc_host *host)
{
	u32 reg;
	int ret;
	ret = readl_poll_timeout(host->regbase + REG_NFC_INTR_RAW, reg,
			(reg & SPI_FINISH), 0, SFC_WAIT_TIMEOUT);
	writel(SPI_FINISH, host->regbase + REG_NFC_INTR_RAW);
#ifdef CONFIG_PM_SLEEP
	host->ctrl_sus = CTRL_END;
#endif
	return ret;
}

int get_if_type(enum spi_nor_protocol proto)
{
	enum mc_sfc_iftype if_type;

	switch (proto) {
	case SNOR_PROTO_1_1_2:
		if_type = IF_TYPE_DUAL;
		break;
	case SNOR_PROTO_1_1_4:
		if_type = IF_TYPE_QUAD;
		break;
	case SNOR_PROTO_1_1_1:
	default:
		if_type = IF_TYPE_STD;
		break;
	}

	return if_type;
}

void mc_spi_nor_init(struct mc_sfc_host *host)
{

	u32 reg;

	reg = readl(host->regbase + SFC_SSIENR);
	reg = (u32)~SSIENR_SSIEN;
	writel(reg, host->regbase + SFC_SSIENR);
	reg = readl(host->regbase + SFC_CTRLR0);
	reg |= CTRLR0_SCPH | CTRLR0_SCPOL;
	writel(reg, host->regbase + SFC_CTRLR0);
	writel(0, host->regbase + SFC_IMR);
	writel(SFC_BAUDR_4, host->regbase + SFC_BAUDR);
	writel(SFC_SAMPLE_DELAY, host->regbase + SFC_RX_SAMPLE_DLY);
	reg = readl(host->regbase + SFC_CCFGR);
	reg &= (u32)~(SFC_CCFGR_DUMBYTE_MASK);
	writel(reg, host->regbase + SFC_CCFGR);
	reg = (u32)(host->regbase + REG_NFC_DMA_COMMON);
	reg &= ~(BIT(28)|BIT(29)|BIT(30)|BIT(31));
	writel(reg, host->regbase + REG_NFC_DMA_COMMON);
	
	reg = SSIENR_SSIEN;
	writel(reg, host->regbase + SFC_SSIENR);

}

void mc_spi_nor_cs_select(struct spi_nor *nor)
{
	u32 reg;
	struct mc_sfc_priv *priv = nor->priv;
	struct mc_sfc_host *host = priv->host;

	reg = 0;
	writel(reg, host->regbase + SFC_SSIENR);
	reg = SER_SELECT(priv->chipselect);
	writel(reg, host->regbase + SFC_SER);
	writel(SSIENR_SSIEN, host->regbase + SFC_SSIENR);
}

int mc_nor_enable_clk(struct mc_sfc_host *host)
{
	int ret;
#ifdef SFC_CLK_FB
	ret = clk_mini_enable(CKG_SFC_EN);
	if(ret){
		clk_mini_disable(CKG_SFC_EN);
		return -1;
	}
	ret = clk_mini_enable(CKG_APB_SFC_EN);
	if(ret)
		if(ret){
		clk_mini_disable(CKG_APB_SFC_EN);
		return -1;
	}
#else
	if(host->apb_spic) {
		ret = clk_prepare_enable(host->apb_spic);
		if (ret)
			return ret;
	}

	if(host->ahb_spic) {
		ret = clk_prepare_enable(host->ahb_spic);
		if (ret) {
			clk_disable_unprepare(host->apb_spic);
			return ret;
		}
	}
#endif
	return 0;

}

void mc_spi_nor_disable_clk(struct mc_sfc_host *host)
{
#ifdef SFC_CLK_FB
	clk_mini_disable(CKG_SFC_EN);
	clk_mini_disable(CKG_APB_SFC_EN);
#else
	if(host->ahb_spic)
		clk_disable_unprepare(host->ahb_spic);

	if(host->apb_spic)
		clk_disable_unprepare(host->apb_spic);
#endif
}

void nfc_fb_incdec_mode(struct mc_sfc_host *host, unsigned char mode)
{
	u32 reg;

	reg = readl(host->regbase + REG_NFC_DMA_COMMON);
	if (mode)
		reg |= FB_INCDEC_MODE;
	else
		reg &= ~(FB_INCDEC_MODE);
	writel(reg, host->regbase + REG_NFC_DMA_COMMON);

}

void nfc_flowbuf_clr(struct mc_sfc_host *host)
{
	writel(NFC_FLOWBUF_CLR, host->regbase + REG_NFC_ACCESS_START);
}

void nfc_all_clr(struct mc_sfc_host *host)
{
	u32 reg;

	writel(0, host->regbase + REG_NFC_CMD_MODE);
	writel(0, host->regbase + REG_NFC_CMD_SET0);
	writel(0, host->regbase + REG_NFC_CMD_SET1);
	writel(0, host->regbase + REG_NFC_CMD_SET2);
	reg = readl(host->regbase + REG_NFC_CMD_SET3);
	reg &= ~SPI_RD_DLY_NUM_MASK;
	writel(reg, host->regbase + REG_NFC_CMD_SET3);
	writel(0, host->regbase + REG_NFC_CMD_SET4);
	writel(0, host->regbase + REG_NFC_DEV_MODE);

}

void nfc_data_type(struct mc_sfc_host *host, unsigned int type)
{
	unsigned int val;

	val = readl(host->regbase + REG_NFC_CMD_MODE);
	val = SPI_NFC_BFINS(DATA_TYPE, type, val);
	writel(val, host->regbase + REG_NFC_CMD_MODE);
}

/*static void nfc_set_page_size(struct mc_sfc_host *host, unsigned int page_size)
{
	unsigned int val = 0;

	page_size = page_size - 1;
	val = readl(host->regbase + REG_NFC_CMD_SET0);
	val = SPI_NFC_BFINS(SET0_PAGE_SIZE, page_size, val);
	writel(val, host->regbase + REG_NFC_CMD_SET0);

}*/

void nfc_cmd_type(struct mc_sfc_host *host, unsigned int type)
{
	unsigned int val;

	val = readl(host->regbase + REG_NFC_CMD_MODE);
	val = SPI_NFC_BFINS(CMD_TYPE, type, val);
	writel(val, host->regbase + REG_NFC_CMD_MODE);
}

void nfc_data_length(struct mc_sfc_host *host, unsigned int len)
{
	unsigned int val;

	if (!len)
		return;
	len -= 1;

	val = readl(host->regbase + REG_NFC_CMD_SET3);
	val = SPI_BFINS(DATA_LENGTH_XC, len, val);
	writel(val, host->regbase + REG_NFC_CMD_SET3);
}

void nfc_cmd(struct mc_sfc_host *host, unsigned char opcode)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_CMD_SET1);
	val = SPI_NFC_BFINS(CMD_VALUE, opcode, val);
	writel(val, host->regbase + REG_NFC_CMD_SET1);
}

void nfc_set_tmod(struct mc_sfc_host *host, unsigned char mode)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(TMOD, mode, val);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void nfc_set_fldform(struct mc_sfc_host *host, unsigned char mode)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(FLDFORM, mode, val);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void nfc_set_frmform(struct mc_sfc_host *host, unsigned char mode)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(FRMFORM, mode, val);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void nfc_set_phase_vld(struct mc_sfc_host *host, unsigned int bits, unsigned char vld)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	if (vld)
		val |= bits;
	else
		val &= ~(bits);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void sfc_write_reg(struct mc_sfc_host *host, u32 cfg)
{
	cfg = cfg & 0xff;
	writel(cfg, host->regbase + REG_NFC_FLASH_REG);
}

void nfc_read_en(struct mc_sfc_host *host)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_CMD_MODE);
	val &= ~(NFC_WRITE_CMD | NFC_READ_CMD);
	val |= NFC_READ_CMD;
	writel(val, host->regbase + REG_NFC_CMD_MODE);
}

void sfc_access_start(struct mc_sfc_host *host)
{
#ifdef CONFIG_PM_SLEEP
	host->ctrl_sus = CTRL_START;
#endif
	writel(NFC_ACC_START, host->regbase + REG_NFC_ACCESS_START);
}

int nfc_check_buf_p2_rdy(struct mc_sfc_host *host)
{
	unsigned int val, t;

	t = 10;
	do {
		val = readl(host->regbase + REG_NFC_BUF_SOFT);
		if (val & NFC_SOFT_FB_P2_RDY_BIT)
			break;
		mdelay(1);
	} while (--t);

	if(!t)
		return -1;
	
	return 0;
}

void write_nfc_soft_fb_p2_ack(struct mc_sfc_host *host)
{
	u32 val;

	val = readl(host->regbase + REG_NFC_BUF_SOFT);
	val |= NFC_SOFT_FB_P2_ACK;
	writel(val, host->regbase + REG_NFC_BUF_SOFT);
}

void sfc_addr_length(struct mc_sfc_host *host, u32 len)
{
	u32 val;
	u32 mask;
	if (!len)
		return;
	len -= 1;
	mask = (1 << NFC_ADDR_LENGTH_SIZE) - 1;
	val = readl(host->regbase + REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_ADDR_LENGTH_OFFSET);
	val |= (len & mask) << NFC_ADDR_LENGTH_OFFSET;
	writel(val, host->regbase + REG_NFC_CMD_SET0);
}

void spi_enable(struct mc_sfc_host *host, u32 enable)
{
	unsigned int val;

	if (enable)
		val = 1;
	else
		val = 0;
	writel(SPI_BF(SER, val), host->regbase + SFC_SSIENR);
}

void spi_set_dummy_byte(struct mc_sfc_host *host, u32 dummy_byte)
{
	u32 val;
	
	spi_enable(host, NFC_DISABLE);
	val = readl(host->regbase + SFC_CCFGR);
	val = SPI_BFINS(DUMMYBYTE, dummy_byte, val);
	writel(val, host->regbase + SFC_CCFGR);
}

void sfc_addr(struct mc_sfc_host *host, u8 *addr, u32 len)
{
	u32 i;
	u32 val;
	u32 address = 0;

	if (!len)
		return;

	sfc_addr_length(host, len);

	for (i = 0; i < 3; i++)
		address += addr[i] << (i * 8);
	val = readl(host->regbase + REG_NFC_CMD_SET1);
	val &= 0xff;
	val |= address << 8;
	writel(val, host->regbase + REG_NFC_CMD_SET1);

	if (len > 3) {
		address = addr[3] + (addr[4] << 8);
		val = readl(host->regbase + REG_NFC_CMD_SET2);
		val &= 0xffff0000;
		val |= address;
		writel(val, host->regbase + REG_NFC_CMD_SET2);
	}
}

void sfc_data_length(struct mc_sfc_host *host, u32 len)
{
	u32 val;
	u32 mask;

	if(!len)
		return;

	nfc_data_length(host, len);
	
	len -= 1;
	mask = (1 << NFC_DATA_LENGTH_SIZE) - 1;
	val = readl(host->regbase + REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_DATA_LENGTH_OFFSET);
	val |= (len & mask)<< NFC_DATA_LENGTH_OFFSET;
	writel(val, host->regbase + REG_NFC_CMD_SET0);
}

void sfc_cmd(struct mc_sfc_host *host, u32 cmd)
{
	u32 val;
	u32 mask;

	mask = (1 << NFC_CMD_VALUE_SIZE) - 1;
	val = readl(host->regbase + REG_NFC_CMD_SET1);
	val &= ~(mask << NFC_CMD_VALUE_OFFSET);
	val |= (cmd & mask) << NFC_CMD_VALUE_OFFSET;
	writel(val, host->regbase + REG_NFC_CMD_SET1);
}

void nfc_dev_mode_clr(struct mc_sfc_host *host)
{
	writel(0, host->regbase + REG_NFC_DEV_MODE);
}

void nfc_set_cmd_wmode(struct mc_sfc_host *host, unsigned char wire)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(CMD_WMODE, wire, val);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void nfc_set_data_wmode(struct mc_sfc_host *host, unsigned char wire)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(DATA_WMODE, wire, val);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void nfc_set_addr_wmode(struct mc_sfc_host *host, u8 wire)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(ADDR_WMODE, wire, val);
	writel(val, host->regbase + REG_NFC_DEV_MODE);
}

void sfc_dev_mode_reg_set(struct mc_sfc_host *host, u32 frmform, u32 vld, u32 nbits)
{
	u32 val;

	nfc_dev_mode_clr(host);
	nfc_set_tmod(host, TMOD_TRANSMIT_ONLY);//Transfer Mode,Transmit Only
	if(nbits == IF_TYPE_STD){
		nfc_set_fldform(host, FLDFORM_ALL_SERIAL);//Quad/Dual Data, other Serial
	}else{
		nfc_set_fldform(host, FLDFORM_DQ_DATA_OTHRE_SERIAL);
	}
	nfc_set_frmform(host, frmform);//opcode + 3byte addr
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
	nfc_set_phase_vld(host, val, NFC_ENABLE);
	nfc_set_cmd_wmode(host, WMODE_SINGLE);
	nfc_set_data_wmode(host, nbits);
	nfc_set_addr_wmode(host, WMODE_SINGLE);
}

void sfc_cmd_write(struct mc_sfc_host *host)
{
	u32 val;

	val = readl(host->regbase + REG_NFC_CMD_MODE);
	val &= ~(NFC_READ_CMD | NFC_WRITE_CMD);
	val |= NFC_WRITE_CMD;
	writel(val, host->regbase + REG_NFC_CMD_MODE);
}

void spi_dtr_enable(struct mc_sfc_host *host)
{
	u32 val;

	val = readl(host->regbase + SFC_CTRLR0);
	val |= (CTRLR0_ADDR_DTR_EN | CTRLR0_DATA_DTR_EN);
	val &= (~CTRLR0_CMD_DTR_EN);

	writel(val,host->regbase + SFC_CTRLR0);
}

void spi_dtr_disable(struct mc_sfc_host *host)
{
	u32 val;

	val = readl(host->regbase + SFC_CTRLR0);
	val &= ~(CTRLR0_ADDR_DTR_EN | CTRLR0_DATA_DTR_EN);

	writel(val,host->regbase + SFC_CTRLR0);
}


void nfc_set_dtr_enable(struct mc_sfc_host *host)
{
	unsigned int val = 0;

	val = readl(host->regbase + REG_NFC_DEV_MODE);
	val |= (NFC_DATA_DTR_EN | NFC_ADDR_DTR_EN);
	val &= ~(NFC_DTR_CONTINUE_EN);
	val &= ~(NFC_CMD_DTR_EN);
	writel(val,host->regbase + REG_NFC_DEV_MODE);

	val = readl(host->regbase + REG_NFC_CMD_MODE);
	val |= NFC_HIGHCLK_MODE;
	writel(val,host->regbase + REG_NFC_CMD_MODE);

}

int mc_spi_nor_op_reg(struct spi_nor *nor,
				u8 opcode, u8 *buf, int len, u8 optype)
{
	int ret;
	u32 val = 0;
	struct mc_sfc_priv *priv = nor->priv;
	struct mc_sfc_host *host = priv->host;
	spin_lock(&spinlock_nor_nand);

	//mc_spi_nor_init(host);

	if(optype == CMD_READ){
		nfc_fb_incdec_mode(host, BUFFER_FLOW_READ);
	}else{
		nfc_fb_incdec_mode(host, BUFFER_FLOW_WRITE);
	}

	nfc_flowbuf_clr(host);
	nfc_all_clr(host);

	if(optype == CMD_READ){
		nfc_data_type(host, DATA_ONLY);
		nfc_cmd_type(host, 0);
		sfc_data_length(host, len);
	}

	nfc_cmd(host, opcode);
	nfc_set_tmod(host, TMOD_TRANSMIT_ONLY);
	nfc_set_fldform(host, FLDFORM_ALL_SERIAL);
	nfc_set_frmform(host, FRMFORM_OPCODE_ONLY);

	if(optype == CMD_WRITE){
		sfc_cmd_write(host);
		if(len == 1 || len == 2){
			/*nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD | NFC_DATA_PHASE_VLD | NFC_REG_PHASE_VLD, NFC_ENABLE);
			memcpy(&val, buf, len);
			sfc_write_reg(host, val);*/
			nfc_data_type(host, DATA_ONLY);
			nfc_cmd_type(host, 0);
			sfc_data_length(host, len);
			//nfc_set_phase_vld(host, NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, IF_TYPE_STD);
			ret = nfc_check_buf_p2_rdy(host);
			if (ret){
				spin_unlock(&spinlock_nor_nand);
				return ret;
			}
			memcpy(&val, buf, len);
			writel(val, host->regbase + REG_DATA_REGION);
			write_nfc_soft_fb_p2_ack(host);
		}else if(len == 3){
			nfc_set_frmform(host, FRMFORM_OPCODE_3ADDR);
			//nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR, IF_TYPE_STD);
			sfc_addr(host, buf, len);
		}else if(len == 4){
			nfc_set_frmform(host, FRMFORM_OPCODE_4ADDR);
			//nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_4ADDR, VLD_CMD_ADDR, IF_TYPE_STD);
			sfc_addr(host, buf, len);
		}else{
			//nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD, IF_TYPE_STD);
		}
		//sfc_cmd_write(host);
	}else{
		//nfc_set_phase_vld(host, NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD, NFC_ENABLE);
		sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, IF_TYPE_STD);
		nfc_read_en(host);
	}

	mc_spi_nor_cs_select(nor);
	sfc_access_start(host);
	ret = wait_op_finish(host);
	if (ret) {
		spin_unlock(&spinlock_nor_nand);
		return ret;
	}
	if(optype == CMD_READ){
		ret = nfc_check_buf_p2_rdy(host);
		if (ret) {
			spin_unlock(&spinlock_nor_nand);
			return ret;
		}
		memcpy(buf, host->regbase + REG_DATA_REGION, len);
		write_nfc_soft_fb_p2_ack(host);
	}
	spin_unlock(&spinlock_nor_nand);
	return 0;
}

int mc_spi_nor_read_reg(struct spi_nor *nor, u8 opcode, u8 *buf,
		size_t len)
{
	return mc_spi_nor_op_reg(nor, opcode, buf, len, CMD_READ);
}

int mc_spi_nor_write_reg(struct spi_nor *nor, u8 opcode,
				const u8 *buf, size_t len)
{
	return mc_spi_nor_op_reg(nor, opcode, (u8 *)buf, len, CMD_WRITE);
}

void spi_nor_read_setfreq(struct mc_sfc_host *host, int req)
{
	spi_enable(host, NFC_DISABLE);
	writel(SFC_BAUDR_4, host->regbase + SFC_BAUDR);
	clk_mini_set_rate(CKG_MUX_SFC, req);
	spi_enable(host, NFC_ENABLE);
}

int mc_spi_nor_dma_transfer(struct spi_nor *nor, loff_t start_off,
		u_char *buf, dma_addr_t dma_buf, size_t len, u8 op_type)
{
	struct mc_sfc_priv *priv = nor->priv;
	struct mc_sfc_host *host = priv->host;
	u32 val = 0;
	u8 if_type = 0;
	u8 reg[5] = {0, 0, 0, 0, 0};
	u32 frmform;
	int ret;
	int size = 0;

	//mc_spi_nor_init(host);

	if(op_type == CMD_READ){
		nfc_fb_incdec_mode(host, BUFFER_FLOW_READ);
	}else{
		nfc_fb_incdec_mode(host, BUFFER_FLOW_WRITE);
	}

	nfc_flowbuf_clr(host);
	nfc_all_clr(host);
	nfc_data_type(host, DATA_ONLY);

	if(op_type == CMD_READ) {
		val = nor->addr_width + (nor->read_dummy >> 3);
		//val = (nor->read_dummy >> 3);
	} else {
		val = nor->addr_width;
		//val = 0;
	}

	if(op_type == CMD_READ) {
		if_type = get_if_type(nor->read_proto);
		if(val == 4){
			frmform = FRMFORM_OPCODE_4ADDR;
		}else{
			frmform = FRMFORM_OPCODE_3ADDR;
		}
		sfc_dev_mode_reg_set(host, frmform, VLD_CMD_ADDR_DATA, if_type);
	}else{
		sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR_DATA, IF_TYPE_STD);
	}

	sfc_data_length(host, len);

	if(nor->addr_width == 4) {
		reg[0] = (u8)(start_off >> 24);
		reg[1] = (u8)(start_off >> 16);
		reg[2] = (u8)(start_off >> 8);
		reg[3] = (u8)(start_off);
	} else {
		reg[0] = (u8)(start_off >> 16);
		reg[1] = (u8)(start_off >> 8);
		reg[2] = (u8)(start_off);
	}
	sfc_addr(host, reg, val);
	if(nor->info->rx_sample_delay){
		spi_enable(host, NFC_DISABLE);
		writel(nor->info->rx_sample_delay, host->regbase + SFC_RX_SAMPLE_DLY);
	}
	if(op_type == CMD_READ){
		nfc_read_en(host);
		if(nor->info->flags & SPI_DTR_EN){
			//spi_nor_read_setfreq(host, SFC_CLK_300M_REQ);
			if(nor->read_opcode == OP_READ_QUAD ||nor->read_opcode == OP_READ_DTR){
				nor->read_opcode = OP_READ_DTR;
			}else if(nor->read_opcode == OP_READ_QUAD_4B ||nor->read_opcode == OP_READ_DTR_4B){
				nor->read_opcode = OP_READ_DTR_4B;
			}
			writel(0, host->regbase + REG_NFC_DEV_MODE);
			spi_enable(host, NFC_DISABLE);
			spi_dtr_enable(host);
			nfc_set_dtr_enable(host);
			writel(SFC_BAUDR_2, host->regbase + SFC_BAUDR);
			writel(SFC_SAMPLE_DELAY/2, host->regbase + SFC_RX_SAMPLE_DLY);
			spi_set_dummy_byte(host, nor->info->dtr_dummy_byte);
			nfc_set_data_wmode(host,IF_TYPE_QUAD);
			nfc_set_addr_wmode(host,WMODE_QUAD);
			val = NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD |NFC_ADDR_PHASE_VLD;
			nfc_set_phase_vld(host, val, NFC_ENABLE);
			nfc_set_frmform(host,FRMFORM_OPCODE_ONLY);
			nfc_set_fldform(host, FLDFORM_SERIAL_OP_OTHER_DQ);
			nfc_set_tmod(host, FLDFORM_DQ_DATA_OTHRE_SERIAL);
		}
		sfc_cmd(host, nor->read_opcode);
	}else{
		sfc_cmd_write(host);
		sfc_cmd(host, nor->program_opcode);
		ret = nfc_check_buf_p2_rdy(host);
		if (ret)
			return ret;
		if(len >> 2) {
			size = (len >> 2) << 2;
			if(IS_ALIGNED((uintptr_t)buf,4)) {
				memcpy(host->regbase + REG_DATA_REGION, buf, size);
			} else {
				memcpy(host->buffer, buf, size);
				memcpy(host->regbase + REG_DATA_REGION, host->buffer, size);
			}
		}
		if(len & 0x3) {
			memcpy(&val, buf + size, len & 0x3);
			writel(val,host->regbase + REG_DATA_REGION + size);
		}
		write_nfc_soft_fb_p2_ack(host);
	}

	//readl(host->regbase + SFC_ICR);
	mc_spi_nor_cs_select(nor);
	sfc_access_start(host);

	ret = wait_op_finish(host);
	if (ret)
		return ret;
	if(op_type == CMD_READ){
		ret = nfc_check_buf_p2_rdy(host);
		if (ret)
			return ret;
		memcpy(buf, host->regbase + REG_DATA_REGION, len);
		write_nfc_soft_fb_p2_ack(host);
	}
	spi_set_dummy_byte(host, SPI_DUMMYBYTE_CLEAR);
	spi_dtr_disable(host);
	writel(SFC_BAUDR_4, host->regbase + SFC_BAUDR);
	writel(SFC_SAMPLE_DELAY, host->regbase + SFC_RX_SAMPLE_DLY);
#if 0
	if(nor->info->flags & SPI_DTR_EN)
		spi_nor_read_setfreq(host, SFC_CLK_400M_REQ);
#endif
	return ret;
}

ssize_t mc_spi_nor_read(struct spi_nor *nor, loff_t from, size_t len,
		u_char *read_buf)
{
	struct mc_sfc_priv *priv = nor->priv;
	struct mc_sfc_host *host = priv->host;
	size_t offset;
	int ret;

	spin_lock(&spinlock_nor_nand);
	for (offset = 0; offset < len; offset += SFC_DATA_BUFFER) {
		size_t trans = min_t(size_t, SFC_DATA_BUFFER, len - offset);

		ret = mc_spi_nor_dma_transfer(nor,
			from + offset, read_buf + offset, host->dma_buffer, trans, CMD_READ);
		if (ret) {
			spin_unlock(&spinlock_nor_nand);
			dev_warn(nor->dev, "DMA read timeout\n");
			return ret;
		}
	}
	spin_unlock(&spinlock_nor_nand);

	return len;
}

ssize_t mc_spi_nor_write(struct spi_nor *nor, loff_t to,
			size_t len, const u_char *write_buf)
{
	struct mc_sfc_priv *priv = nor->priv;
	struct mc_sfc_host *host = priv->host;
	size_t offset;
	int ret;

	spin_lock(&spinlock_nor_nand);
	for (offset = 0; offset < len; offset += SFC_DMA_MAX_LEN) {
		size_t trans = min_t(size_t, SFC_DMA_MAX_LEN, len - offset);
		ret = mc_spi_nor_dma_transfer(nor,
			to + offset, (u_char *)write_buf + offset, host->dma_buffer, trans, CMD_WRITE);
		if (ret) {
			spin_unlock(&spinlock_nor_nand);
			dev_warn(nor->dev, "DMA write timeout\n");
			return ret;
		}
	}
	spin_unlock(&spinlock_nor_nand);

	return len;
}

static const struct spi_nor_controller_ops mc_spi_controller_ops = {
    .read_reg = mc_spi_nor_read_reg,
    .write_reg = mc_spi_nor_write_reg,
    .read = mc_spi_nor_read,
    .write = mc_spi_nor_write,
    .erase = NULL,

};

/**
 * Get spi flash device information and register it as a mtd device.
 */
static int mc_spi_nor_register(struct device_node *np,
				struct mc_sfc_host *host)
{

	struct device *dev = host->dev;
	struct spi_nor *nor;
	struct mc_sfc_priv *priv;
	struct mtd_info *mtd;
	int ret;
    const struct spi_nor_hwcaps hwcaps = {
            .mask = SNOR_HWCAPS_READ |
                    SNOR_HWCAPS_READ_FAST |
                    SNOR_HWCAPS_READ_1_1_2 |
                    SNOR_HWCAPS_READ_1_1_4 |
                    SNOR_HWCAPS_PP, };

	nor = devm_kzalloc(dev, sizeof(*nor), GFP_KERNEL);
	if (!nor)
		return -ENOMEM;

	nor->dev = dev;
	spi_nor_set_flash_node(nor, np);

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	ret = of_property_read_u32(np, "reg", &priv->chipselect);
	if (ret) {
		dev_err(dev, "There's no reg property for %s\n",
			np->full_name);
		return ret;
	}

	priv->host = host;
	nor->priv = priv;
    nor->controller_ops = &mc_spi_controller_ops;

	ret = spi_nor_scan(nor, NULL, &hwcaps);
	if (ret)
		return ret;

	mtd = &nor->mtd;
	mtd->name = np->name;
	ret = mtd_device_register(mtd, NULL, 0);
	if (ret)
		return ret;
	host->nor[host->num_chip] = nor;
	host->num_chip++;

	return 0;
}

static void mc_spi_nor_unregister_all(struct mc_sfc_host *host)
{
	int i;

	for (i = 0; i < host->num_chip; i++)
		mtd_device_unregister(&host->nor[i]->mtd);
}

static int mc_spi_nor_register_all(struct mc_sfc_host *host)
{
	struct device *dev = host->dev;
	struct device_node *np;
	int ret;

	for_each_available_child_of_node(dev->of_node, np) {
		ret = mc_spi_nor_register(np, host);
		if (ret)
			goto fail;

		if (host->num_chip > MC_MAX_CHIP_NUM) {
			dev_warn(dev, "Flash device number exceeds the maximum chipselect number\n");
			break;
		}
	}

	return 0;

fail:
	mc_spi_nor_unregister_all(host);
	return ret;
}

static int mc_spi_nor_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct resource *res;
	struct mc_sfc_host *host;
	int (*get_clk)(struct mc_sfc_host *);
	#ifndef SFC_CLK_FB
	struct clk *clk;
	#endif
	int ret;
	host = devm_kzalloc(dev, sizeof(*host), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

	platform_set_drvdata(pdev, host);
	host->dev = dev;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "control");
	host->regbase = devm_ioremap(dev, res->start, resource_size(res));
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
#ifdef SFC_CLK_FB
	ret = clk_mini_set_rate(CKG_MUX_SFC,24000000);
	if (ret){
		dev_err(host->dev, "failed to set rate %uHz\n", host->clkrate);
		return ret;
	}
	ret = clk_mini_set_rate(CKG_MUX_SFC,host->clkrate);
	if (ret){
		dev_err(host->dev, "failed to set rate %uHz\n", host->clkrate);
		return ret;
	}
#else
	clk = devm_clk_get(&pdev->dev, "clk_mux_sfc");
	if(IS_ERR(clk)){
		dev_err(host->dev, "sfc clk not available");
		return PTR_ERR(clk);
	}
	ret = clk_set_rate(clk,24000000);
	if (ret){
		dev_err(host->dev, "failed to set rate %uHz\n", host->clkrate);
		return ret;
	}
	ret = clk_set_rate(clk,host->clkrate);
	if (ret){
		dev_err(host->dev, "failed to set rate %uHz\n", host->clkrate);
		return ret;
	}
	ret = clk_get_rate(clk);
	if (ret == 0)
		return ret;
#endif

	get_clk = of_device_get_match_data(dev);
	if(!get_clk)
	    return -ENODEV;
#if 1
	ret = (*get_clk)(host);
	if (ret) {
		dev_err(dev, "get clock failed\n");
		return ret;
	}
#endif

/*
	ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32));
	if (ret) {
		dev_warn(dev, "Unable to set dma mask\n");
		return ret;
	}

	host->buffer = dmam_alloc_coherent(dev, SFC_DMA_MAX_LEN,
			&host->dma_buffer, GFP_KERNEL);
	if (!host->buffer)
		return -ENOMEM;
*/
	host->buffer = devm_kzalloc(dev, SFC_DMA_MAX_LEN, GFP_KERNEL);
	if (!host->buffer)
		return -ENOMEM;
/*
	ret = clk_set_rate(host->clk, host->clkrate);
	if (ret)
		return ret;
*/
	mutex_init(&host->lock);

	mc_spi_nor_init(host);
	ret = mc_spi_nor_register_all(host);
	if (ret) {
		mutex_destroy(&host->lock);
		mc_spi_nor_disable_clk(host);
	}

	return ret;
}

static int mc_spi_nor_remove(struct platform_device *pdev)
{
	struct mc_sfc_host *host = platform_get_drvdata(pdev);

	mc_spi_nor_unregister_all(host);
	mutex_destroy(&host->lock);
	mc_spi_nor_disable_clk(host);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int wait_nor_finish(struct mc_sfc_host *host)
{
	unsigned regval, timeout = host->sus_timeout;

	do {
		mdelay(1);
		regval = readl(host->regbase + REG_NFC_INTR_RAW);
		--timeout;
	} while ((!(regval & SPI_FINISH) && timeout));
	if (!timeout) {
		dev_err(host->dev, " Wait SPI NOR finish timeout!\n");
		host->ctrl_sus = CTRL_END;
		return -ETIMEDOUT;
	} else {
		//writel(regval,host->regbase + REG_NFC_INTR_RAW);
		host->ctrl_sus = CTRL_END;
		return 0;
	}
}

int mc_spi_nor_wr_reg(struct mc_sfc_host *host,
				u8 opcode, u8 *buf, int len, u8 optype)
{
	int ret;
	u32 val = 0;

	if(optype == CMD_READ){
		nfc_fb_incdec_mode(host, BUFFER_FLOW_READ);
	}else{
		nfc_fb_incdec_mode(host, BUFFER_FLOW_WRITE);
	}

	nfc_flowbuf_clr(host);
	nfc_all_clr(host);

	if(optype == CMD_READ){
		nfc_data_type(host, DATA_ONLY);
		nfc_cmd_type(host, 0);
		sfc_data_length(host, len);
	}

	nfc_cmd(host, opcode);
	nfc_set_tmod(host, TMOD_TRANSMIT_ONLY);
	nfc_set_fldform(host, FLDFORM_ALL_SERIAL);
	nfc_set_frmform(host, FRMFORM_OPCODE_ONLY);

	if(optype == CMD_WRITE){
		sfc_cmd_write(host);
		if(len == 1 || len == 2){
			/*nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD | NFC_DATA_PHASE_VLD | NFC_REG_PHASE_VLD, NFC_ENABLE);
			memcpy(&val, buf, len);
			sfc_write_reg(host, val);*/
			nfc_data_type(host, DATA_ONLY);
			nfc_cmd_type(host, 0);
			sfc_data_length(host, len);
			//nfc_set_phase_vld(host, NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, IF_TYPE_STD);
			ret = nfc_check_buf_p2_rdy(host);
			if (ret)
				return ret;
			memcpy(&val, buf, len);
			writel(val, host->regbase + REG_DATA_REGION);
			write_nfc_soft_fb_p2_ack(host);
		}else if(len == 3){
			nfc_set_frmform(host, FRMFORM_OPCODE_3ADDR);
			//nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_3ADDR, VLD_CMD_ADDR, IF_TYPE_STD);
			sfc_addr(host, buf, len);
		}else if(len == 4){
			nfc_set_frmform(host, FRMFORM_OPCODE_4ADDR);
			//nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_4ADDR, VLD_CMD_ADDR, IF_TYPE_STD);
			sfc_addr(host, buf, len);
		}else{
			//nfc_set_phase_vld(host, NFC_CMD_PHASE_VLD, NFC_ENABLE);
			sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD, IF_TYPE_STD);
		}
		//sfc_cmd_write(host);
	}else{
		//nfc_set_phase_vld(host, NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD, NFC_ENABLE);
		sfc_dev_mode_reg_set(host, FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, IF_TYPE_STD);
		nfc_read_en(host);
	}
	sfc_access_start(host);
	ret = wait_nor_finish(host);
	if (ret)
		return ret;

	if(optype == CMD_READ){
		ret = nfc_check_buf_p2_rdy(host);
		if (ret)
			return ret;
		memcpy(buf, host->regbase + REG_DATA_REGION, len);
		write_nfc_soft_fb_p2_ack(host);
	}

	return 0;
}

static int nor_molchip_suspend(struct device *dev)
{
	struct mc_sfc_host *host = dev_get_drvdata(dev);
	u32 ret;
	u32 timeout = SUS_DEFAULT_TIMEOUT;
	u8 status = 0xff;

	if(host->sus_timeout)
		timeout = host->sus_timeout;

	//wait sfc controller idle
	if(host->ctrl_sus){
		ret = wait_nor_finish(host);
	}
	if(ret && !(host->force_suspend)){
		printk("nor suspend timeout\r\n");
		return -EBUSY;
	}

	//wait nor flash idle
	while(timeout){
		mc_spi_nor_wr_reg(host,OP_READ_SR, &status, 1,CMD_READ);
		if ((status & 1) == 0){
			break;
		}
		mdelay(1);
		timeout--;
	}
	if((!timeout) && (!(host->force_suspend))){
		printk("nor flash status is busy\r\n");
		return -EBUSY;
	}

	//set flash power down
	mc_spi_nor_wr_reg(host,POWRE_DOWN, &status, 1,CMD_WRITE);

	//clk disable

	clk_mini_disable(CKG_APB_SFC_EN);
	clk_mini_disable(CKG_SFC_EN);

	printk("nor suspend done\r\n");
	return 0;
}

static int nor_molchip_resume(struct device *dev)
{
	struct mc_sfc_host *host = dev_get_drvdata(dev);
	clk_mini_set_rate(CKG_MUX_SFC, host->clkrate);
	clk_mini_enable(CKG_APB_SFC_EN);
	clk_mini_enable(CKG_SFC_EN);
	mc_spi_nor_init(host);
	printk("nor resume done\r\n");
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(nor_molchip_pm_ops, nor_molchip_suspend,
			 nor_molchip_resume);


static const struct of_device_id mc_spi_nor_dt_ids[] = {
	{ .compatible = "mc,spi-nor", .data = mc_clk_get },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mc_spi_nor_dt_ids);

static struct platform_driver mc_spi_nor_driver = {
	.driver = {
		.name	= "mc-sfc",
		.pm	= &nor_molchip_pm_ops,
		.of_match_table = mc_spi_nor_dt_ids,
	},
	.probe	= mc_spi_nor_probe,
	.remove	= mc_spi_nor_remove,
};
module_platform_driver(mc_spi_nor_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SPI Nor Flash Controller Driver");

