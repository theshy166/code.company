#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/spic.h>
#include <mach/dma.h>
#include <mach/clk.h>
#include <mach/nandflash.h>

#define SPI_CS_OUTPUT()	(__raw_writel(0x10,SPIC_CS_DIR))
#define SPI_CS_UNMASK()	(__raw_writel(0x10,SPIC_CS_MASK))

#define REG_SPIC_NOR_FLASH_BASE			0x10000000

#define spi_get_tx_fifo_level()	spi_readl(REG_SPI_TXFLR)
#define spi_get_rx_fifo_level() spi_readl(REG_SPI_RXFLR)
#define	SPI_FIFO_DEPTH			(128)
#define SPI_RX_ONE_TIME			(128)
#define SFC_RX_SAMPLE_DLY_2 2
#define SFC_RX_SAMPLE_DLY_4 4

u32 flash_size = 0;

void sfc_driver_strength(void)
{
	__raw_writel(0x40, G1_PAD10_REG);
}

void enalbe_spic(u32 enable)
{
	u32 val;

	val = __raw_readl(REG_SFC_CTRL0);

	if (enable) {
		val |= CKG_SFC_EN | CKG_APB_SFC_EN;
	} else {
		val &= ~(CKG_SFC_EN | CKG_APB_SFC_EN);
	}

	__raw_writel(val, REG_SFC_CTRL0);
}

void reset_spic(void)
{
	u32 val;
	u32 t1, t2;
	
	val = __raw_readl(REG_SFC_CTRL0);
	val |= SFC_SOFT_RST;
	__raw_writel(val, REG_SFC_CTRL0);

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
	} while (t2 - t1 < 5);

	val = __raw_readl(REG_SFC_CTRL0);
	val &= ~SFC_SOFT_RST;
	__raw_writel(val, REG_SFC_CTRL0);
}

void clk_select_spic(int clk)
{
	unsigned int reg;
	reg = readl(REG_SFC_CTRL0);
	reg &= ~(SFC_CLOCK_SEL(0x7));
	reg |= SFC_CLOCK_SEL(clk);
	writel(reg, REG_SFC_CTRL0);
}


/* function body */
void spi_enable(u32 enable)
{
	unsigned int val;

	if (enable)
		val = 1;
	else
		val = 0;
	spi_writel(REG_SPI_SSIENR, SPI_BF(SER, val));
}

void spi_set_imr(unsigned int data)
{
	spi_writel(REG_SPI_IMR, data);
}

void spi_disalble_slave(u32 date)
{
	spi_writel(REG_SPI_SER, date);
}

void spi_set_polarity(u32 polarity)
{
	u32 val;

	val = spi_readl(REG_SPI_CTRLR0);
	val = SPI_BFINS(SCPOL,polarity,val);
	spi_writel(REG_SPI_CTRLR0,val);
}

void spi_set_phase(u32 phase)
{
	u32 val;

	val = spi_readl(REG_SPI_CTRLR0);
	val = SPI_BFINS(SCPH,phase,val);

	spi_writel(REG_SPI_CTRLR0, val);
}

void spi_set_frame_format(u32 format)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_CTRLR0);
	val = SPI_BFINS(FRF,format,val);

	spi_writel(REG_SPI_CTRLR0, val);
}

void spi_set_data_size(u32 size)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_CTRLR0);
	val = SPI_BFINS(DFS,size,val);
	spi_writel(REG_SPI_CTRLR0, val);
}

void spi_set_transfer_mode(u32 mode)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_CTRLR0);
	val = SPI_BFINS(TMOD,mode,val);

	spi_writel(REG_SPI_CTRLR0, val);
}

void spi_set_read_data_num(u32 num)
{
	u32 val = 0;

	val = SPI_BF(NDF, num -1);
	spi_writel(REG_SPI_CTRLR1, val);
}

void spi_set_baudrate(u32 baudrate)
{
	u32 val = 0;

	val = SPI_BF(SCKDV,	baudrate);
	spi_writel(REG_SPI_BAUDR, val);
}

void spi_set_dummy_byte(u8 dummy_byte)
{
	u32 val;

	val = spi_readl(REG_SPI_CCFGR);
	val = SPI_BFINS(DUMMYBYTE, dummy_byte, val);
	spi_writel(REG_SPI_CCFGR, val);  // Dummy Byte: 0 byte
}

void spi_disable_irq(u32 irq)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_IMR);
	val &= ~irq;
	spi_writel(REG_SPI_IMR, val);
}

void spi_enable_irq(u32 irq)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_IMR);
	val |= irq;
	spi_writel(REG_SPI_IMR, val);

}

u32 spi_get_status(void)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_SR);
	return val;
}

void spi_enable_port(u32 port)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_SER);
	port = 1 << port;
	val = SPI_BFINS(SSIEN,port,val);
	spi_writel(REG_SPI_SER, val);
}

void spi_disable_port(u32 port)
{
	u32 val = 0;

	val = spi_readl(REG_SPI_SER);
	port = 0 << port;
	val = SPI_BFINS(SSIEN,port,val);
	spi_writel(REG_SPI_SER,val);
}

void spi_rx_sample_delay(u32 delay)
{
	delay = delay & 0xff;
	spi_writel(REG_SPI_RX_SAMPLE_DLY,delay);
}

void spi_timing(u32 value)
{
	spi_writel(REG_SPI_TIMCR,value);
}

void spi_set_wire_mode(u8 wmod)
{
	u32 val;

	spi_enable(false);
	val = spi_readl(REG_SPI_CCFGR);
	val = SPI_BFINS(WMOD, wmod,val);
	if(wmod == WIRE_MODE_SPI)
		val = SPI_BFINS(FLDFORM,FILED_FORMAT_STD,val);
	else
		val = SPI_BFINS(FLDFORM,FILED_FORMAT_DPI_QPI,val);
	spi_writel(REG_SPI_CCFGR, val);
	spi_enable(true);
}

void spi_set_field_format(u8 fld_form)
{
	u32 val;

	val = spi_readl(REG_SPI_CCFGR);
	val = SPI_BFINS(FLDFORM, fld_form,val);
	spi_writel(REG_SPI_CCFGR, val);
}

void spi_set_frame_form(u8 frm_form)
{
	u32 val;

	val = spi_readl(REG_SPI_CCFGR);
	val = SPI_BFINS(FRMFORM, frm_form, val);
	spi_writel(REG_SPI_CCFGR,val); // Dummy Byte: 0 byte
}

void spi_set_addr_mode(u8 addr_mode)
{
	u32 val;

	val = spi_readl(REG_SPI_CCFGR);
	val = SPI_BFINS(ADDRMODE,addr_mode,val);
	spi_writel(REG_SPI_CCFGR, val);  //Dummy Byte: 0 byte
}

void sfc_cmd_write(void)
{
	u32 val;

	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(NFC_READ_CMD | NFC_WRITE_CMD);
	val |= NFC_WRITE_CMD;
	spi_writel(REG_NFC_CMD_MODE,val);
}

void sfc_cmd_read(void)
{
	u32 val;

	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(NFC_READ_CMD | NFC_WRITE_CMD);
	val |= NFC_READ_CMD;
	spi_writel(REG_NFC_CMD_MODE, val);
}

void sfc_ecc_enable(bool en)
{
	u32 val;

	val = spi_readl(REG_NFC_CMD_MODE);

	if (en == true)
		val |= ECC_CAL_EB;
	else
		val &= ~ECC_CAL_EB;

	spi_writel(REG_NFC_CMD_MODE, val);

	val = spi_readl(REG_NFC_ECC_CFG1);

	if (en == true)
		val |= ECC_ECC_CAL_EB;
	else
		val &= ~ECC_ECC_CAL_EB;

	spi_writel(REG_NFC_ECC_CFG1, val);
}

void sfc_data_type(u32 type)
{
	u32 val;
	u32 mask;

	mask = (1 << NFC_DATA_TYPE_OFFSET) - 1;
	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(mask << NFC_DATA_TYPE_OFFSET);
	val |= (type & mask) << NFC_DATA_TYPE_OFFSET;
	spi_writel(REG_NFC_CMD_MODE, val);// Dummy Byte: 0 byte
}

void sfc_addr_length(u32 len)
{
	u32 val;
	u32 mask;
	if (!len)
		return;
	len -= 1;
	mask = (1 << NFC_ADDR_LENGTH_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_ADDR_LENGTH_OFFSET);
	val |= (len & mask) << NFC_ADDR_LENGTH_OFFSET;
	spi_writel(REG_NFC_CMD_SET0, val);
}

void sfc_spare_length(u32 len)
{
	u32 val;
	u32 mask;

	if(!len)
		return;
	len -= 1;
	mask = (1 << NFC_SPARE_LENGTH_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_SPARE_LENGTH_OFFSET);
	val |= (len & mask) << NFC_SPARE_LENGTH_OFFSET;
	spi_writel(REG_NFC_CMD_SET0,val);
}

void sfc_spare_len_1k(u32 len)
{
	u32 val;
	if (!len)
		return;
	len = len - 1;
	val = (len & 0x3f) << SPARE_LEN_1K_OFFSET;
	val |= (63 - len) << SPARE_SKIP_1K_OFFSET;
	spi_writel(REG_NFC_CMD_SET4, val);
}

void nfc_data_length(unsigned int len)
{
	unsigned int val;

	if (!len)
		return;
	len -= 1;

	val = spi_readl(REG_NFC_CMD_SET3);
	val = SPI_BFINS(DATA_LENGTH_XC, len, val);
	spi_writel(REG_NFC_CMD_SET3, val);
}

void sfc_data_length(u32 len)
{
	u32 val;
	u32 mask;

	if(!len)
		return;

	nfc_data_length(len);
	
	len -= 1;
	mask = (1 << NFC_DATA_LENGTH_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_DATA_LENGTH_OFFSET);
	val |= (len & mask)<< NFC_DATA_LENGTH_OFFSET;
	spi_writel(REG_NFC_CMD_SET0,val);
}

void ecc_sts_clr(void)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_ACCESS_START);
	val &= ~NFC_ECC_STS_CLR;
	spi_writel(REG_NFC_ACCESS_START, val);
}

void sfc_ecc_clr(void)
{
	ecc_sts_clr();
	spi_writel(REG_NFC_ECC_CFG0, 0);
	spi_writel(REG_NFC_ECC_CFG1, 0);
	spi_writel(REG_NFC_ECC_CFG2, 0);
}

int sfc_start_exec(u8 dma)
{
	unsigned int status, done;
	unsigned int t1, t2;
	int ret = false;

	done = SPI_FINISH;
	if (dma == SFC_DMA_RX)
		done |= DMA_RX0_FINISH | DMA_RX1_FINISH;
	else if (dma == SFC_DMA_TX)
		done |= DMA_TX0_FINISH;
	else if (dma == SFC_DMA_RX_ID)
		done |= DMA_RX0_FINISH;

	spi_writel(REG_NFC_ACCESS_START, NFC_ACC_START);

	t1 = get_sys_timer_val();
	do {
		status = spi_readl(REG_NFC_INTR_RAW);
		if ((status & done) == done) {
			spi_writel(REG_NFC_INTR_RAW, status);//clear interrupt
			ret = true;
			break;
		}
		t2 = get_sys_timer_val();
	} while (t2 - t1 < SPIC_WAIT_READY);

	return ret;
}

void sfc_all_clr(void)
{
	u32 reg;
	spi_writel(REG_NFC_CMD_MODE, 0);
	spi_writel(REG_NFC_CMD_SET0, 0);
	spi_writel(REG_NFC_CMD_SET1, 0);
	spi_writel(REG_NFC_CMD_SET2, 0);
	reg = spi_readl(REG_NFC_CMD_SET3);
	reg &= ~SPI_RD_DLY_NUM_MASK;
	spi_writel(REG_NFC_CMD_SET3, reg);
	spi_writel(REG_NFC_CMD_SET4, 0);
	spi_writel(REG_NFC_DEV_MODE, 0);
}

void sfc_cmd(u32 cmd)
{
	u32 val;
	u32 mask;

	mask = (1 << NFC_CMD_VALUE_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET1);
	val &= ~(mask << NFC_CMD_VALUE_OFFSET);
	val |= (cmd & mask) << NFC_CMD_VALUE_OFFSET;
	spi_writel(REG_NFC_CMD_SET1, val);
}

void sfc_addr(u8 *addr,u32 len)
{
	u32 i;
	u32 val;
	u32 address = 0;

	if (!len)
		return;

	sfc_addr_length(len);

	for (i = 0; i < 3; i++)
		address += addr[i] << (i * 8);
	val = spi_readl(REG_NFC_CMD_SET1);
	val &= 0xff;
	val |= address << 8;
	spi_writel(REG_NFC_CMD_SET1, val);

	if (len > 3) {
		address = addr[3] + (addr[4] << 8);
		val = spi_readl(REG_NFC_CMD_SET2);
		val &= 0xffff0000;
		val |= address;
		spi_writel(REG_NFC_CMD_SET2, val);
	}
}

void nfc_fb_incdec_mode(unsigned char mode)
{
	unsigned int val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	if (mode)
		val |= FB_INCDEC_MODE;
	else
		val &= ~(FB_INCDEC_MODE);
	spi_writel(REG_NFC_DMA_COMMON, val);

}

void nfc_set_tmod(unsigned char mode)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(TMOD, mode, val);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void nfc_set_fldform(unsigned char mode)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(FLDFORM, mode, val);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void nfc_set_frmform(unsigned char mode)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(FRMFORM, mode, val);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void nfc_set_cmd_wmode(unsigned char wire)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(CMD_WMODE, wire, val);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void nfc_set_phase_vld(unsigned int bits, unsigned char vld)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	if (vld)
		val |= bits;
	else
		val &= ~(bits);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void nfc_set_data_wmode(unsigned char wire)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(DATA_WMODE, wire, val);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void nfc_set_addr_wmode(u8 wire)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val = SPI_NFC_BFINS(ADDR_WMODE, wire, val);
	spi_writel(REG_NFC_DEV_MODE, val);
}

void sfc_dev_mode_reg_set(u32 frmform, u32 vld, u32 nbits)
{
	u32 val;

	nfc_dev_mode_clr();
	nfc_set_tmod(TMOD_TRANSMIT_ONLY);//Transfer Mode,Transmit Only
	if(nbits == WIRE_MODE_SPI){
		nfc_set_fldform(FLDFORM_ALL_SERIAL);//Quad/Dual Data, other Serial
	}else{
		nfc_set_fldform(FLDFORM_DQ_DATA_OTHRE_SERIAL);
	}
	nfc_set_frmform(frmform);//opcode + 3byte addr
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
	nfc_set_phase_vld(val, NFC_ENABLE);
	nfc_set_cmd_wmode(WMODE_SINGLE);//+
	nfc_set_data_wmode(nbits);
	nfc_set_addr_wmode(WMODE_SINGLE);//+
}

void sfc_write_reg(u32 cfg)
{
	cfg = cfg & 0xff;
	spi_writel(REG_NFC_FLASH_REG, cfg);
}
void sfc_xfc_mode(u32 mode)
{
	u32 val, mask;

	mask = (1 << SPI_XF_MODE_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_MODE);
	val &=	~(mask << SPI_XF_MODE_OFFSET);
	val |= (mode & mask) << SPI_XF_MODE_OFFSET;
	spi_writel(REG_NFC_CMD_MODE,val);
}

void sfc_ecc_pos(u32 pos)
{
	u32 mask;
	u32 val;

	mask = (1 << NFC_ECC_POS_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET2);
	val &= ~(mask << NFC_ECC_POS_OFFSET);
	val |= (pos & mask) << NFC_ECC_POS_OFFSET;
	spi_writel(REG_NFC_CMD_SET2, val);

	mask = ECC_SPARE_BASE_BYTE_ADDR_MSK;
	val = spi_readl(REG_NFC_ECC_CFG1);
	val &= ~(mask << ECC_SPARE_BASE_BYTE_ADDR_OFS);
	val |= (pos & mask) << ECC_SPARE_BASE_BYTE_ADDR_OFS;
	spi_writel(REG_NFC_ECC_CFG1, val);
}

void sfc_dma_mode(void)
{
	u32 val;

	val = spi_readl(REG_NFC_CMD_MODE);
	val |= DMA_MODE;
	spi_writel(REG_NFC_CMD_MODE,val);
}

#define ROUNDUP(x,y) ((x + ((y)-1)) & ~((y)-1))

void sfc_write_data(u8 *src,u32 len)
{
	u32 val;
	u32 i,l;
	u32 *dest = (u32 *)REG_DATA_REGION;
	u32  *psrc = (u32 *)src;
	u32 count = 0;

	if(len > 4096)
		len = 4096;
	while(len) {
		if(len > 4){
			len -= 4;
			l = 4;
		} else {
			l = len;
			len = 0;
		}
		if(((unsigned long)src & 0x3) || (l & 0x3)) {
			val = 0;
			for(i = 0; i < l; i++)
				val += src[count + i] << (i << 3);
			*dest++ = val;
			count += l;
		} else {
		   *dest++ = *psrc++;
           count += 4;
		}
	}
}

u32 sfc_read_reg(void)
{
	u32 val;

	val = spi_readl(REG_NFC_FLASH_REG);
	return (val >> 16) & 0xff;
}

int sfc_wait_fbuf2_ready(void)
{
	unsigned int val = 0;
	unsigned int t1, t2;
	int ret;

	t1 = get_sys_timer_val();
	while(1) {
		val = spi_readl(REG_NFC_BUF_SOFT);
		if (val & NFC_SOFT_FB_P2_RDY_BIT) {
			ret = true;
			break;
		}
		t2 = get_sys_timer_val();
		if (t2 - t1 > SPIC_FB_READY) {
			ret = false;
			break;
		}
	}
	return ret;
}

void sfc_fbuf2_ack(void)
{
	spi_writel(REG_NFC_BUF_SOFT, NFC_SOFT_FB_P2_ACK);
}

void sfc_read_data(u8 *dest,u32 len)
{
	u32 val,count,i,l;
	u32 *src = (u32 *)REG_DATA_REGION;
	u32  *pdest = (u32 *)dest;

	count = 0;
	if(len > 4096)
		len = 4096;
	while(len) {
		if(len > 4) {
			len -= 4;
			l = 4;
		} else {
			l = len;
			len = 0;
		}

		if(((unsigned long)dest & 0x3) || (l & 0x3)) {
			val = *src++;
			for(i = 0; i < l; i++)
				dest[count + i] = val >> (i << 3);
			count += l;
		} else {
		  *pdest++ = *src++ ;
		  count += 4;
		}
	}

}

void sfc_dma_tx0_enable(void)
{
	u32 val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	val &= ~NFC_RX_CH0_DMA_EN;
	val |= NFC_TX_CH0_DMA_EN;
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void sfc_dma_tx1_enable(void)
{
	u32 val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	val &= ~NFC_RX_CH1_DMA_EN;
	val |= NFC_TX_CH1_DMA_EN;
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void sfc_dma_rx0_enable(void)
{
	u32 val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	val &= ~NFC_TX_CH0_DMA_EN;
	val |= NFC_RX_CH0_DMA_EN ;
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void sfc_dma_rx1_enable(void)
{
	u32 val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	val &= ~NFC_TX_CH1_DMA_EN;
	val |= NFC_RX_CH1_DMA_EN ;
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void sfc_dma_disable(void)
{
	u32 val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	val &= ~(NFC_RX_CH0_DMA_EN | NFC_TX_CH0_DMA_EN | NFC_RX_CH1_DMA_EN | NFC_TX_CH1_DMA_EN);
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void sfc_dma_tx0_start(u32 len)
{
	u32 val, mask;
	if (!len)
		return;
	len = len - 1;
	mask = (1 << NFC_DMA_TX0_LEN_SIZE) - 1;
	len = len & mask;
	val = len << NFC_DMA_TX0_LEN_OFFSET;
	val |= DMA_TX0_RUN;
	spi_writel(REG_NFC_DMA_TX_CH0, val);
}

void sfc_dma_tx1_start(u32 len)
{
	u32 val, mask;
	if (!len)
		return;
	len = len - 1;
	mask = (1 << NFC_DMA_TX1_LEN_SIZE) - 1;
	len = len & mask;
	val = len << NFC_DMA_TX1_LEN_OFFSET;
	val |= DMA_TX1_RUN;
	spi_writel(REG_NFC_DMA_TX_CH1, val);
}

void sfc_dma_tx0_stop(void)
{
	spi_writel(REG_NFC_DMA_TX_CH0, 0);
}

void sfc_dma_tx1_stop(void)
{
	spi_writel(REG_NFC_DMA_TX_CH1, 0);
}

void sfc_dma_rx0_start(u32 len)
{
	u32 val, mask;
	if (!len)
		return;
	len = len - 1;
	mask = (1 << NFC_DMA_RX0_LEN_SIZE) - 1;
	len = len & mask;
	val = len << NFC_DMA_RX0_LEN_OFFSET;
	val |= DMA_RX0_RUN;
	spi_writel(REG_NFC_DMA_RX_CH0, val);
}

void sfc_dma_rx1_start(u32 len)
{
	u32 val, mask;
	if (!len)
		return;
	len = len - 1;
	mask = (1 << NFC_DMA_RX1_LEN_SIZE) - 1;
	len = len & mask;
	val = len << NFC_DMA_RX1_LEN_OFFSET;
	val |= DMA_RX1_RUN;
	spi_writel(REG_NFC_DMA_RX_CH1, val);
}

void sfc_dma_rx0_stop(void)
{
	spi_writel(REG_NFC_DMA_RX_CH0, 0);
}

void sfc_dma_rx1_stop(void)
{
	spi_writel(REG_NFC_DMA_RX_CH1, 0);
}

static void sfc_dma_write(u8 *src, u32 len, int spare)
{
	if(src == NULL || len == 0)
		return;

	if (spare){
		dma_chn_src(DMA_CHN28, (u32)src);
		dma_chn_dst(DMA_CHN28, (u32)REG_SPARE_REGION);
		dma_trsc_len(DMA_CHN28, len);
		enable_channel(DMA_CHN28);
		sfc_dma_tx1_enable();
		sfc_dma_tx1_start(len);
	}else{
		dma_chn_src(DMA_CHN30, (u32)src);
		dma_chn_dst(DMA_CHN30, (u32)REG_DATA_REGION);
		dma_trsc_len(DMA_CHN30, len);
		enable_channel(DMA_CHN30);
		sfc_dma_tx0_enable();
		sfc_dma_tx0_start(len);
	}

}

void sfc_dma_write_data(u8 *src,u32 len)
{
	sfc_dma_write(src, len, 0);
}

void sfc_dma_write_spare(u8 *src,u32 len)
{
	sfc_dma_write(src, len, 1);
}

static void sfc_dma_read(u8 *dest, u32 len, int spare)
{
	if(dest == NULL || len == 0)
		return;

	if (spare){
		dma_chn_src(DMA_CHN29, (u32)REG_SPARE_REGION);
		dma_chn_dst(DMA_CHN29, (u32)dest);
		dma_trsc_len(DMA_CHN29, len);
		enable_channel(DMA_CHN29);
		sfc_dma_rx1_enable();
		sfc_dma_rx1_start(len);
	}
	else{
		dma_chn_src(DMA_CHN31, (u32)REG_DATA_REGION);
		dma_chn_dst(DMA_CHN31, (u32)dest);
		dma_trsc_len(DMA_CHN31, len);
		enable_channel(DMA_CHN31);
		sfc_dma_rx0_enable();
		sfc_dma_rx0_start(len);
	}
}

void sfc_dma_read_data(u8 *dest, u32 len)
{
	sfc_dma_read(dest, len, 0);
}

void sfc_dma_read_spare(u8 *dest, u32 len)
{
	sfc_dma_read(dest, len, 1);
}

void sfc_dma_wait_ready(void)
{
	dma_wait_complete(DMA_CHN31);
}

void sfc_write_spare(u8 *src,u32 len,u32 count)
{
	u32 i,j;
	u32 size;
	u32 *dest;
	u32 *psrc = (u32 *)src;

	if(len > 256)
		len = 256;
	len = ROUNDUP(len, 4);
	size = len / count;
	for(i = 0; i < count; i++) {
		dest =(u32 *)((unsigned long)REG_SPARE_REGION + (i << 6));
		for(j = 0; j < size; j += 4) {
			*dest++ = *psrc++;
		}

	}
}

void sfc_fill_spare(void)
{
	u32 i;
	u32 *dest =(u32 *)(REG_SPARE_REGION);

	for(i = 0; i < 64; i++) {
			*dest++ = 0xFFFFFFFF;
	}
}

void sfc_read_spare(u8 *dest,u32 len,u32 count)
{
	u32 i,j,size;
	u32 *src = (u32 *)REG_SPARE_REGION;
	u32 *pdest = (u32 *)dest;

	if(dest == NULL || len == 0 || count == 0)
		return;
	
	if(len > 256)
		len = 256;
	len = ROUNDUP(len, 4);
	size = len / count;
	for(i = 0; i < count; i++) {
		src = (u32 *)((unsigned long)REG_SPARE_REGION + (i << 6));
		for(j = 0; j < size; j += 4) {
			*pdest++ = *src++;
		}
	}
}

void sfc_deal_spare(u8 *dest, u8 *src_addr, u32 len, u32 count)
{
	u8 spare[256];
	u32 i,j,size;
	u32 *src = (u32 *)spare;
	u32 *pdest = (u32 *)dest;
	
	if(dest == NULL || src_addr == NULL || len == 0 || count == 0)
		return;
	
	if(len > 256)
		len = 256;
	len = ROUNDUP(len, 4);
	memcpy(spare, src_addr, len);
	size = len / count;
	for(i = 0; i < count; i++) {
		src = (u32 *)((unsigned long)spare + (i << 6));
		for(j = 0; j < size; j += 4) {
			*pdest++ = *src++;
		}
	}
}

void sfc_ecc_enc_mode(u32 encode)
{
	u32 val;

	val = spi_readl(REG_NFC_ECC_CFG1);
	if (encode)
		val |= ECC_ENCODE_MODE;
	else
		val &= ~(ECC_ENCODE_MODE);
	spi_writel(REG_NFC_ECC_CFG1, val);
}

void  sfc_spare_ecc_byte(u32 ecc)
{
	u32 mask;
	u32 val;

	mask = (1 << NFC_ECC_SPARE_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET2);
	val &= ~(mask << NFC_ECC_SPARE_OFFSET);
	val |= (ecc & mask) << NFC_ECC_SPARE_OFFSET;
	spi_writel(REG_NFC_CMD_SET2, val);

	mask = ECC_SPARE_BYTE_NUM_MSK;
	val = spi_readl(REG_NFC_ECC_CFG1);
	val &= ~(mask << ECC_SPARE_BYTE_NUM_OFS);
	val |= (ecc & mask) << ECC_SPARE_BYTE_NUM_OFS;
	spi_writel(REG_NFC_ECC_CFG1, val);
}

void sfc_ecc_page_size(u32 size)
{
	u32 val;
	if (!size)
		return;
	size = size - 1;
	val = size & ECC_DATA_LEN_MSK;
	spi_writel(REG_NFC_ECC_CFG0, val);
	val = size & ECC_PAGE_SIZE_MSK;
	spi_writel(REG_NFC_ECC_CFG2, val);
}

void nfc_dma_reg_inter_num(unsigned char cycle)
{
	unsigned int val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	val = SPI_NFC_BFINS(DMA_REG_INTER_NUM, cycle, val);
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void spi_cs_holdtime(unsigned int data)
{
	unsigned int val;

	val = spi_readl(REG_SPI_TIMCR);
	val = SPI_BFINS(TSHSL, data, val);
	spi_writel(REG_SPI_TIMCR, val);
}

void spi_set_rd_delay_cnt(unsigned int rd_delay_cnt)
{
	unsigned int val;

	val = spi_readl(REG_NFC_CMD_SET3);
	val = SPI_BFINS(RD_DLY_NUM_XC, rd_delay_cnt, val);
	spi_writel(REG_NFC_CMD_SET3, val);
}

void sfc_ecc_mode(u32 mode)
{
	u32 val;

	val = spi_readl(REG_NFC_ECC_CFG1);
	if (mode == ECC_24BIT)
		val |= ECC_MODE;
	else
		val &= ~ECC_MODE;
	spi_writel(REG_NFC_ECC_CFG1, val);
}



void spi_init(unsigned int speed_mode)
{
	unsigned int rd_delay_cnt = 0;
	/*unsigned int sel, baud = 2;
	unsigned int aon_clock, rd_delay_cnt = 0;
	if (speed_mode < sizeof(sfc_configs)/sizeof(sfc_ctrl_config)) {
		enalbe_spic(false);
		sel = sfc_refclock_select();
		baud = sfc_ref_clock[sel] / sfc_configs[speed_mode].baud_rate;
		aon_clock = aon_sys_clock_get();
		if (aon_clock > SYS_CLOCK_24MHZ) {
			rd_delay_cnt = 33;
		} else {
			rd_delay_cnt = 0;
		}
	}*/
	/* sfc_clk driver strength over 100Mhz */
	sfc_driver_strength();

	enalbe_spic(true);

	reset_spic();

	//disable spi
	spi_enable(SPI_DISABLE);

	spi_set_imr(0);

	/* chip select. */
	spi_disalble_slave(1);

	/* clock mode: 3. */
	//spi polarty
	spi_set_polarity(SPI_POLARITY_HIGH);
	//spi phase
	spi_set_phase(SPI_PHASE_SCLK_TOGGLE_START);

	//baudrate,dummy,delay
	spi_set_baudrate(BIT_SFC_BAUDR_4);//2

	spi_set_dummy_byte(0);//linklist//4

	spi_rx_sample_delay(SFC_SAMPLE_DELAY);//0

	clk_select_spic(0x4);

	/* DMA request time interval. */
	rd_delay_cnt = 33;//set rd_delay_cnt,value comes from romcode
	nfc_dma_reg_inter_num(10);

	spi_cs_holdtime(0);
	spi_set_rd_delay_cnt(rd_delay_cnt);

	dma_reset();
	//dma_init(DMA_CHN28, SPIC_TX1_RID);
	dma_init(DMA_CHN29, SPIC_RX1_RID);
	dma_init(DMA_CHN30, SPIC_TX_RID);
	dma_init(DMA_CHN31, SPIC_RX_RID);

}
void board_spic_init(void)
{
	spi_init(3);
}

void nfc_all_clr(void)
{
	u32 reg;
	spi_writel(REG_NFC_CMD_MODE, 0);
	spi_writel(REG_NFC_CMD_SET0, 0);
	spi_writel(REG_NFC_CMD_SET1, 0);
	spi_writel(REG_NFC_CMD_SET2, 0);
	reg = spi_readl(REG_NFC_CMD_SET3);
	reg &= ~SPI_RD_DLY_NUM_MASK;
	spi_writel(REG_NFC_CMD_SET3, reg);
	spi_writel(REG_NFC_CMD_SET4, 0);
	spi_writel(REG_NFC_DEV_MODE, 0);
}

void sfc_access_start(void)
{
	spi_writel(REG_NFC_ACCESS_START, NFC_ACC_START);
}

void nfc_flowbuf_clr(void)
{
	spi_writel(REG_NFC_ACCESS_START, NFC_FLOWBUF_CLR);
}

void dma_rx0_cfg1(u32 interrupt)
{
	int val = 0;

	val = spi_writel(REG_NFC_DMA_RX_CH0_CFG1, val);
	val = SPI_NFC_BFINS(DMA_RX0_CNT_INTR_CFG, interrupt, val);
	spi_writel(REG_NFC_DMA_RX_CH0_CFG1, val);
}

void nfc_cmd_mode_clr(void)
{
	spi_writel(REG_NFC_CMD_MODE, 0);
}

void nfc_cmd_set_clr(void)
{
	u32 reg;
	spi_writel(REG_NFC_CMD_SET0, 0);
	spi_writel(REG_NFC_CMD_SET1, 0);
	spi_writel(REG_NFC_CMD_SET2, 0);
	reg = spi_readl(REG_NFC_CMD_SET3);
	reg &= ~SPI_RD_DLY_NUM_MASK;
	spi_writel(REG_NFC_CMD_SET3, reg);
	spi_writel(REG_NFC_CMD_SET4, 0);
	spi_writel(REG_NFC_DEV_MODE, 0);
}

void nfc_dev_mode_clr(void)
{
	spi_writel(REG_NFC_DEV_MODE, 0);
}

void nfc_dma_common_clr(void)
{
	spi_writel(REG_NFC_DMA_COMMON, 0);
}

void nfc_dma_en(unsigned int ch, unsigned char enable)
{
	unsigned int val;

	val = spi_readl(REG_NFC_DMA_COMMON);
	if (enable)
		val |= ch;
	else
		val &= ~(ch);
	spi_writel(REG_NFC_DMA_COMMON, val);
}

void nfc_dma_common_reg_set(u32 able, u32 ch, u8 pclk_cycle, u32 mode)
{
	nfc_dma_common_clr();
	nfc_dma_en(ch, able);
	nfc_fb_incdec_mode(mode);
	nfc_dma_reg_inter_num(pclk_cycle);
}


void nfc_dma_rx_ch0_cfg1_reg_set(u32 interrupt)
{
	dma_rx0_cfg1(interrupt);
}

void nfc_dma_rx_ch0_start(unsigned int num, unsigned int len, unsigned char run)
{
	unsigned int val = 0;

	if (run)
		val |= DMA_RX0_RUN;
	else
		val &= ~(DMA_RX0_RUN);

	val = SPI_NFC_BFINS(DMA_RX0_LEN, len - 1, val);
	val = SPI_NFC_BFINS(DMA_RX0_NUM, num - 1, val);//rpagenum -1
	spi_writel(REG_NFC_DMA_RX_CH0, val);

}

void nfc_dma_rx_ch0_reg_set(u32 num, u32 len, u8 run)
{
	nfc_dma_rx_ch0_start(num, len, run);
}

void nfc_dma_clr(void)
{
	spi_writel(REG_NFC_DMA_RX_CH0, 0);
}

void access_start_clr(void)
{
	spi_writel(REG_NFC_ACCESS_START, 0);
}

void access_start_set(u32 able, u32 flowbuf)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_ACCESS_START);
	if (able)
		val |= NFC_ACC_START;
	else
		val &= ~(NFC_ACC_START);
	if (flowbuf)
		val |= NFC_FLOWBUF_CLR;
	else
		val &= ~(NFC_FLOWBUF_CLR);

	spi_writel(REG_NFC_ACCESS_START, val);
}

void nfc_access_start_reg_set(u32 able, u32 flowbuf)
{
	access_start_clr();
	access_start_set(able, flowbuf);
}

void nfc_cmd_mode_reg_set(u32 type)
{
	nfc_cmd_mode_clr();
	nfc_cmd_type(type);
	nfc_data_type(DATA_ONLY);
	nfc_read_en();
}

void nfc_ccmd_datalength(unsigned int len)
{
	unsigned int val;

	if (!len)
		return;

	len -= 1;
	val = spi_readl(REG_NFC_CMD_SET4);
	val = SPI_NFC_BFINS(CCMD_DATA_LENGTH_XC, len, val);
	spi_writel(REG_NFC_CMD_SET4, val);
}

void nfc_cmd_set_reg_set(u32 from, u32 page_num, u32 page_size,
	u32 opcode, u32 ccmd_pagesize)
{
	nfc_cmd_set_clr();
	if(flash_size > FLASH_SIZE_16M)
		spic_addr(from, BYTE_MODE_4);
	else
		spic_addr(from, BYTE_MODE_3);
	nfc_set_page_size(page_size);
	nfc_cmd(opcode);//opcode
	nfc_data_length(page_num * page_size);
	nfc_ccmd_datalength(ccmd_pagesize);
}

void nfc_dev_mode_reg_set(u32 tmod, u32 fldform, u32 frmform,
	u32 data, u32 addr)
{
	nfc_dev_mode_clr();
	nfc_set_tmod(tmod);//Transfer Mode,Transmit Only
	nfc_set_fldform(fldform);//Quad/Dual Data, other Serial
	nfc_set_frmform(frmform);//opcode + 3byte addr
	nfc_set_phase_vld(NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD |
		NFC_ADDR_PHASE_VLD, NFC_ENABLE);
	nfc_set_data_wmode(data);
	nfc_set_addr_wmode(addr);//+
}

void nfc_cmd(unsigned char opcode)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_CMD_SET1);
	val = SPI_NFC_BFINS(CMD_VALUE, opcode, val);
	spi_writel(REG_NFC_CMD_SET1, val);
}

bool sfc_done(u32 finish)
{
	unsigned int status, t1, t2;

	t1 = get_sys_timer_val();

	do {
		status = spi_readl(REG_NFC_INTR_RAW);
		if (status & finish) {
			/*clear interrupt*/
			status = spi_writel(REG_NFC_INTR_RAW, finish);
			goto sfc_complete;
		}

		t2 = get_sys_timer_val();

	} while (t2 - t1 < SPIC_WAIT_READY);

	return false;

sfc_complete:
	return true;

}

void spi_set_4_bit_address_mode(void)
{
	u32 ret;
	ret = spi_readl(REG_NFC_CMD_MODE);
	ret |= BIT_ADDR_4BYTE_MODE;
	spi_writel(REG_NFC_CMD_MODE,ret);

	ret = spi_readl(REG_NFC_CMD_SET0);
	ret &= ~GENMASK(2,0);
	ret |= BIT_ADDR_LEN;
	spi_writel(REG_NFC_CMD_SET0,ret);

	ret = spi_readl(REG_SPI_CCFGR);
	ret |= BIT_ADDRMOD_4;
	spi_writel(REG_SPI_CCFGR,ret);
}


void nfc_data_type(unsigned int type)
{
	unsigned int val;

	val = spi_readl(REG_NFC_CMD_MODE);
	val = SPI_NFC_BFINS(DATA_TYPE, type, val);
	spi_writel(REG_NFC_CMD_MODE, val);
}

void nfc_set_page_size(unsigned int page_size)
{
	unsigned int val = 0;

	page_size = page_size - 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val = SPI_NFC_BFINS(SET0_PAGE_SIZE, page_size, val);
	spi_writel(REG_NFC_CMD_SET0, val);

}

void nfc_cmd_type(unsigned int type)
{
	unsigned int val;

	val = spi_readl(REG_NFC_CMD_MODE);
	val = SPI_NFC_BFINS(CMD_TYPE, type, val);
	spi_writel(REG_NFC_CMD_MODE, val);
}

void nfc_read_en(void)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(NFC_WRITE_CMD | NFC_READ_CMD);
	val |= NFC_READ_CMD;
	spi_writel(REG_NFC_CMD_MODE, val);
}

void nfc_check_buf_p2_rdy(void)
{
	unsigned int val, t1, t2;

	t1 = get_sys_timer_val();
	do {
		val = spi_readl(REG_NFC_BUF_SOFT);
		if (val & NFC_SOFT_FB_P2_RDY_BIT)
			break;
		t2 = get_sys_timer_val();
	} while (t2 - t1 < SPIC_WAIT_READY);

	if (!(val & NFC_SOFT_FB_P2_RDY_BIT))
		;
		//uart_puts("CBR\r\n");
}

void nfc_addr4(unsigned int data)
{
	unsigned int val;

	val = spi_readl(REG_NFC_CMD_SET2);
	val = SPI_NFC_BFINS(ADDR4, data, val);
	spi_writel(REG_NFC_CMD_SET2, val);
}

void nfc_addr_length(unsigned int len)
{
	unsigned int val;

	len -= 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val = SPI_NFC_BFINS(ADDR_LENGTH, len, val);
	spi_writel(REG_NFC_CMD_SET0, val);
}

void spic_addr(unsigned int addr, unsigned int len)
{
	unsigned int val,ret;

	if (!len)
		return;

	nfc_addr_length(len);
	if (len > 3){
		val = (((addr&0xff000000)>>16)|(addr&0x00ff0000)|((addr&0x0000ff00)<<16));
		spi_writel(REG_NFC_CMD_SET1, val);
		val = addr<<24>>24;
		ret = spi_readl(REG_NFC_CMD_SET2);
		ret &= ~GENMASK(8,0);
		ret |= val;
		spi_writel(REG_NFC_CMD_SET2, ret);
	}else {
		val = __SWP32(addr);
		spi_writel(REG_NFC_CMD_SET1, val);
	}

}


void write_nfc_soft_fb_p2_ack(void)
{
	u32 val;

	val = spi_readl(REG_NFC_BUF_SOFT);
	val |= NFC_SOFT_FB_P2_ACK;
	spi_writel(REG_NFC_BUF_SOFT, val);
}

void sfc_dma_nor_read_data(u8 *dest, u32 len)
{
	dma_chn_src(DMA_CHN31, REG_DATA_REGION);
	dma_chn_dst(DMA_CHN31, (u32)dest);
	dma_trsc_len(DMA_CHN31, len);
	enable_channel(DMA_CHN31);
}

void sfc_start(void)
{
	u32 val;

	val = spi_readl(REG_NFC_DMA_RX_CH0);
	val |= DMA_RX0_RUN;

	spi_writel(REG_NFC_DMA_RX_CH0, val);
	spi_writel(REG_NFC_ACCESS_START, NFC_ACC_START);
}

int sfc_wait_fbuf0_ready(void)
{
	unsigned int val = 0;
	unsigned int t1, t2;
	int ret;

	t1 = get_sys_timer_val();
	while(1) {
		val = spi_readl(REG_NFC_BUF_SOFT);
		if (val & NFC_SOFT_FB_P0_RDY_BIT) {
			ret = true;
			break;
		}
		t2 = get_sys_timer_val();
		if (t2 - t1 > SPIC_FB_READY) {
			ret = false;
			break;
		}
	}
	return ret;
}

int sfc_start_ecc_encode(void)
{
	int ret;

	sfc_fbuf2_ack();
	ret = sfc_wait_fbuf0_ready();
	return ret;
}

void sfc_read_ecc(u8 *dest, u32 len)
{
	u32 *src = (u32 *)REG_SPARE_REGION0;

	if (len > SFC_SPARE_BUF_SIZE)
		len = SFC_SPARE_BUF_SIZE;

	memcpy(dest, src, len);
}

int spi_nand_calculate_ecc(u32 size)
{
	int ret;

	nfc_fb_incdec_mode(FB_INCDEC_WRITE);
	sfc_all_clr();
	sfc_ecc_page_size(size);
	sfc_ecc_mode(ECC_24BIT);
	sfc_ecc_pos(0x0);//0x16
	sfc_ecc_enc_mode(true);
	sfc_ecc_enable(true);
	ret = sfc_start_ecc_encode();
	return ret;
}

#ifdef CONFIG_FLASH_NOR_NAND_COEXISTENCE
void spi_nor_cs_select(void)
{
	spi_enable(SPI_DISABLE);
	writel(BIT(0), REG_SPI_SER);
	spi_enable(SPI_ENABLE);
}
#endif
