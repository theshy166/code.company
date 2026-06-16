#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/molchip_fy01.h>
#include <mach/spic.h>
#include <mach/dma.h>

struct wire_mode{
	u8 opcode;
	u8 addr;
	u8 data;
};


#define SPI_CS_OUTPUT()	(__raw_writel(0x10,SPIC_CS_DIR))
#define SPI_CS_UNMASK()	(__raw_writel(0x10,SPIC_CS_MASK))

#define REG_SPIC_NOR_FLASH_BASE			0x10000000

#define spi_get_tx_fifo_level()	spi_readl(REG_SPI_TXFLR)
#define spi_get_rx_fifo_level() spi_readl(REG_SPI_RXFLR)
#define	SPI_FIFO_DEPTH			(128)
#define SPI_RX_ONE_TIME			(128)


void molchip_spic_clk_enable(void)
{
#if defined(CONFIG_TARGET_TS01) || defined(CONFIG_TARGET_FY01)
	
    writel(2, CLK_SPIC_SSI_CFG);

	setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_SPIC_EN);

	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AHB_SPIC_EN | CKG_SPIC_SSI_EN);
#elif defined(CONFIG_TARGET_FY00)
	setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_SFC_EN);
	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AHB_SFC_EN);
#elif defined(CONFIG_TARGET_FY02) || defined(CONFIG_TARGET_FY02B) || defined(CONFIG_TARGET_FY10P) || defined(CONFIG_TARGET_FY02P)
	writel(2, CLK_SPIC_SSI_CFG);

	setbits_le32(REG_AP_PERI_APB_CLK_CTRL,CKG_APB_SFC_EN);

	setbits_le32(REG_AHB_PERI_CLK_CTRL,CKG_AHB_SFC_EN);
#elif defined(CONFIG_TARGET_XC01)

#else
# error "Unsupported Molchip processor"
#endif


}

void reset_spic(void)
{
	u32 val,i;

	val = __raw_readl(REG_AP_AHB_PERI_SOFT_RST);
	val |= SPIC_SOFT_RST;
	__raw_writel(val,REG_AP_AHB_PERI_SOFT_RST);

	for(i=0;i<0x1000;i++);

	val = __raw_readl(REG_AP_AHB_PERI_SOFT_RST);
	val &= ~SPIC_SOFT_RST;
	__raw_writel(val,REG_AP_AHB_PERI_SOFT_RST);
}

void spi_enable(u32 enable)
{
	u32 val;

	if(enable)
		val = 1;
	else
		val = 0;

	spi_writel(REG_SPI_SSIENR,SPI_BF(SER,val));
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
static void spi_timing(u32 value)
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
	spi_writel(REG_NFC_CMD_MODE,val);
}
void sfc_ecc_enable(bool en)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	if(en == true)
		val |= ECC_CAL_EB;
	else
		val &= ~ECC_CAL_EB;
	spi_writel(REG_NFC_CMD_MODE,val);
}
void sfc_data_type(u32 type)
{
	u32 val;
	u32 mask;
	mask = (1 << DATA_TYPE_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(mask << DATA_TYPE_OFFSET);
	val |= (type & mask) << DATA_TYPE_OFFSET;
	spi_writel(REG_NFC_CMD_MODE, val);  // Dummy Byte: 0 byte
}
void sfc_addr_length(u32 len)
{
	u32 val;
	u32 mask;
	mask = (1 << NFC_ADDR_LENGTH_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_ADDR_LENGTH_OFFSET);
	val |= len << NFC_ADDR_LENGTH_OFFSET;
	spi_writel(REG_NFC_CMD_SET0,val);
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
	val |= len << NFC_SPARE_LENGTH_OFFSET;
	spi_writel(REG_NFC_CMD_SET0,val);
}

void sfc_data_length(u32 len)
{
	u32 val;
	u32 mask;
	if(!len)
		return;
	len -= 1;
	mask = (1 << NFC_DATA_LENGTH_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET0);
	val &= ~(mask << NFC_DATA_LENGTH_OFFSET);
	val |= len << NFC_DATA_LENGTH_OFFSET;
	spi_writel(REG_NFC_CMD_SET0,val);
}

void sfc_all_clr(void)
{
	spi_writel(REG_NFC_CMD_MODE,0);
	spi_writel(REG_NFC_CMD_SET0,0);
	spi_writel(REG_NFC_CMD_SET1,0);
	spi_writel(REG_NFC_CMD_SET2,0);
}
void sfc_cmd(u32 cmd)
{
	u32 val;
	u32 mask;
	mask = (1 << NFC_CMD_VALUE_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET1);
	val &= ~(mask << NFC_CMD_VALUE_OFFSET);
	val |= (cmd << NFC_CMD_VALUE_OFFSET);
	spi_writel(REG_NFC_CMD_SET1,val);
}

void ecc_encode(u32 encode)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	if(encode) {
		val |= ECC_ENC_MODE;
		val &= ~ECC_COR_MODE;
	}
	else {
		val &= ~ECC_ENC_MODE;
		val |= ECC_COR_MODE;
	}
	spi_writel(REG_NFC_CMD_MODE,val);
}
bool get_ecc_sts(u32 num)
{
	u32 val,status;
	u32 i;
	if(num >= 4)
		return false;
	val = spi_readl(REG_NFC_ECC_INFO);
	for(i = 0; i < num; i++) {
		status = (val >> (i * 2)) & 0x3;
		if(status == CORRECT_FAIL)
			return false;
	}
	return true;

}
void sfc_addr(u8 *addr,u32 len)
{
	u32 i;
	u32 val;
	u32 address = 0;
	if(!len)
		return;
	sfc_addr_length(len);
	for(i = 0; i < 3; i++)
	{
		address += addr[i] << (i * 8);
	}
	val = spi_readl(REG_NFC_CMD_SET1);
	val &= 0xff;
	val |= address << 8;
	spi_writel(REG_NFC_CMD_SET1,val);
	if(len > 3)
	{
		address = addr[3] + (addr[4] << 8);
		val = spi_readl(REG_NFC_CMD_SET2);
		val &= 0xffff0000;
		val |= address;
		spi_writel(REG_NFC_CMD_SET2,val);
	}
}
void sfc_xfc_mode(u32 mode)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ((1 << SPI_XF_MODE_OFFSET) - 1);
	val |=	(mode << SPI_XF_MODE_OFFSET) &
		~((1 << SPI_XF_MODE_OFFSET) - 1);
	spi_writel(REG_NFC_CMD_MODE,val);
}
void sfc_ecc_pos(u32 pos)
{
	u32 mask;
	u32 val;
	mask = (1 << NFC_ECC_POS_SIZE) - 1;
	val = spi_readl(REG_NFC_CMD_SET2);
	val &= ~(mask << NFC_ECC_POS_OFFSET);
	val |= pos << NFC_ECC_POS_OFFSET;
	spi_writel(REG_NFC_CMD_SET2,val);
}
void sfc_dma_mode(void)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	val |= DMA_MODE;
	spi_writel(REG_NFC_CMD_MODE,val);
}

void sfc_access_start(void)
{
	u32 status;
	u32 t1,t2;
	static u32 i = 0;
	spi_writel(REG_NFC_ACCESS_START,NFC_ACC_START);
	t1 = get_sys_timer_val();
	do
	{
		i++;
		status = spi_readl(REG_SPI_ISR);
		if(status == 0x20) {
			status = spi_readl(REG_SPI_ICR);//clear interrupt
			break;
		}
		t2 = get_sys_timer_val();
	}while(t2 - t1 < SPIC_WAIT_READY);

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
		}
		else {
			l = len;
			len = 0;
		}
		if(((u32)src & 0x3) || (l & 0x3)) {
			val = 0;
			for(i = 0; i < l; i++)
				val += src[count + i] << (i << 3);
			*dest++ = val;
			count += l;
		}
		else {
		   *dest++ = *psrc++;
           count += 4;
		}
	}
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
		}
		else {
			l = len;
			len = 0;
		}
		if(((u32)dest & 0x3) || (l & 0x3)) {
			val = *src++;
			for(i = 0; i < l; i++)
				dest[count + i] = val >> (i << 3);
			count += l;
		}
		else {
		  *pdest++ = *src++ ;
		  count += 4;
		}

	}

}
void sfc_dma_write_data(u8 *src,u32 len)
{
	enable_channel(DMA_CHN31);
	dma_chn_src(DMA_CHN31,(u32)src);
	dma_chn_dst(DMA_CHN31,REG_DATA_REGION);
	dma_trsc_len(DMA_CHN31,len);
}

void sfc_dma_read_data(u8 *dest,u32 len)
{
	enable_channel(DMA_CHN31);
	dma_chn_src(DMA_CHN31,REG_DATA_REGION);
	dma_chn_dst(DMA_CHN31,(u32)dest);
	dma_trsc_len(DMA_CHN31,len);

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
		dest =(u32 *)(REG_SPARE_REGION + (i << 6));
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
	if(len > 256)
		len = 256;
	len = ROUNDUP(len, 4);
	size = len / count;
	for(i = 0; i < count; i++) {
		src = (u32 *)(REG_SPARE_REGION + (i << 6));
		for(j = 0; j < size; j += 4) {
			*pdest++ = *src++;
		}
	}
}
void sfc_ecc_enc_mode(void)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(ECC_COR_MODE);
	val |= ECC_ENC_MODE;
	spi_writel(REG_NFC_CMD_MODE,val);
}
void  sfc_spare_ecc_byte(u32 ecc)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_SET2);
	val &= (1 << NFC_ECC_SPARE_OFFSET) - 1;
	val |= (ecc << NFC_ECC_SPARE_OFFSET) &
		~((1 << NFC_ECC_SPARE_OFFSET) - 1);
	spi_writel(REG_NFC_CMD_SET2,val);

}
void sfc_ecc_cor_mode(void)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	val &= ~(ECC_ENC_MODE);
	val |= ECC_COR_MODE;
	spi_writel(REG_NFC_CMD_MODE,val);
}
void sfc_ecc_mode(u32 mode)
{
	u32 val;
	val = spi_readl(REG_NFC_CMD_MODE);
	if(mode == ECC_24BIT)
		val |= ECC_MODE;
	else
		val &= ~ECC_MODE;
	spi_writel(REG_NFC_CMD_MODE,val);
}


void board_spic_init(void)
{

	molchip_spic_clk_enable();

	reset_spic();

	//disable spi
	spi_enable(SPI_DISABLE);

	//frame format
	spi_set_frame_format(SPI_FORMAT_MOTO);

	//spi polarty
	spi_set_polarity(SPI_POLARITY_HIGH);

	//spi phase
	spi_set_phase(SPI_PHASE_SCLK_TOGGLE_START);

	//transfer data size
	spi_set_data_size(SPI_DATA_SIZE_8BIT);

	//baudrate
	spi_set_baudrate(2);

	spi_disable_irq(0x3f);
	spi_enable_irq(0x20);

	spi_rx_sample_delay(1);

	spi_timing(0);

	spi_set_field_format(FILED_FORMAT_STD);
	spi_set_frame_form(FRAME_FORM_RESERVED);

	spi_enable_port(SPI_PORT0);
	spi_enable(SPI_ENABLE);


}





