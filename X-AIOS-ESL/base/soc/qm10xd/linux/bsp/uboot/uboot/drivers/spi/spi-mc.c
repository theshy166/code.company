#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/spic.h>
#include <mach/dma.h>
#include <mach/uart_drv.h>

struct spi_cfg {
	u8 *tx_buf;
	u8 *rx_buf;
	u32 rx_xfer_len;
	u32 tx_xfer_len;
};

#define	SPI_FIFO_DEPTH			(128)
#define SPI_RX_ONE_TIME			(128)

#define OFFSET_SPI_CTRL0			(0x00)
#define OFFSET_SPI_CTRL1			(0x04)
#define OFFSET_SPI_SSIENR			(0x08)
#define OFFSET_SPI_MWCR				(0x0c)
#define OFFSET_SPI_SER				(0x10)
#define OFFSET_SPI_BAUD				(0x14)
#define OFFSET_SPI_TXFTLR			(0x18)
#define OFFSET_SPI_RXFTLR			(0x1c)
#define OFFSET_SPI_TXFLR			(0x20)
#define OFFSET_SPI_RXFLR			(0x24)
#define OFFSET_SPI_SR				(0x28)
#define OFFSET_SPI_IMR				(0x2c)
#define OFFSET_SPI_ISR				(0x30)
#define OFFSET_SPI_RISR				(0x34)
#define OFFSET_SPI_TXOICR			(0x38)
#define OFFSET_SPI_RXOICR			(0x3c)
#define OFFSET_SPI_RXUICR			(0x40)
#define OFFSET_SPI_MSTICR			(0x44)
#define OFFSET_SPI_ICR				(0x48)
#define OFFSET_SPI_DMACR			(0x4c)
#define OFFSET_SPI_DMATDLR			(0x50)
#define OFFSET_SPI_DMARDLR			(0x54)
#define OFFSET_SPI_IDR				(0x58)
#define OFFSET_SPI_SSI_COMPVER			(0x5c)
#define OFFSET_SPI_DR				(0x60)
#define OFFSET_SPI_RX_SAMPLE_DELAY		(0xf0)

struct spi_cfg spi;

void molchip_spi_clk_enable(void)
{
	/* gpio en */
	setbits_le32(REG_CEN_PERI_APB_CLK_CTRL, BIT(11));
	/* SPI0_CS --> GPIO8_3*/
	setbits_le32(0xB070004, BIT(3));
	setbits_le32(0xB070008, BIT(3));
	setbits_le32(0xB070000, BIT(3));

	/* 13*13 SPI1_CS --> GPIO10_0*/
	setbits_le32(0xB090004, BIT(0));
	setbits_le32(0xB090008, BIT(0));
	setbits_le32(0xB090000, BIT(0));

	/* 11*11 SPI1_CS --> GPIO4_7*/
	setbits_le32(0xB030004, BIT(7));
	setbits_le32(0xB030008, BIT(7));
	setbits_le32(0xB030000, BIT(7));

	setbits_le32(REG_CEN_PERI_CLK_CTRL, CKG_SPI0_EN | CKG_SPI1_EN);
	setbits_le32(REG_CEN_PERI_APB_CLK_CTRL, CKG_APB_SPI0_EN | CKG_APB_SPI1_EN);
}

void reset_spi(void)
{
	u32 val,i;

	/* SPI RESET */
	val = __raw_readl(REG_AP_PERI_SOFT_RST0);
	val |= (SPI0_SOFT_RST | SPI1_SOFT_RST);
	__raw_writel(val,REG_AP_PERI_SOFT_RST0);

	for(i=0;i<0x1000;i++);

	val = __raw_readl(REG_AP_PERI_SOFT_RST0);
	val &= ~SPI0_SOFT_RST;
	val &= ~SPI1_SOFT_RST;
	__raw_writel(val,REG_AP_PERI_SOFT_RST0);
}

void mc_spi_enable(uint base, u32 enable)
{
	u32 val;

	if(enable)
		val = 1;
	else
		val = 0;

	spi_writel(base + OFFSET_SPI_SSIENR,SPI_BF(SER,val));
}
void mc_spi_set_polarity(uint base, u32 polarity)
{
	u32 val;

	val = spi_readl(base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(SCPOL,polarity,val);
	spi_writel(base + OFFSET_SPI_CTRL0,val);
}
void mc_spi_set_phase(uint base, u32 phase)
{
	u32 val;

	val = spi_readl(base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(SCPH,phase,val);

	spi_writel(base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_set_frame_format(uint base, u32 format)
{
	u32 val = 0;

	val = spi_readl(base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(FRF,format,val);

	spi_writel(base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_set_data_size(uint base, u32 size)
{
	u32 val = 0;

	val = spi_readl(base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(DFS,size,val);
	spi_writel(base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_set_transfer_mode(uint base, u32 mode)
{
	u32 val = 0;

	val = spi_readl(base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(TMOD,mode,val);

	spi_writel(base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_set_read_data_num(uint base, u32 num)
{
	u32 val = 0;
	val = SPI_BF(NDF, num -1);
	spi_writel(base + OFFSET_SPI_CTRL1, val);
}
void mc_spi_set_baudrate(uint base, u32 baudrate)
{
	u32 val = 0;
	val = SPI_BF(SCKDV,	baudrate);
	spi_writel(base + OFFSET_SPI_BAUD, val);
}
void mc_spi_disable_irq(uint base, u32 irq)
{
	u32 val = 0;

	val = spi_readl(base + OFFSET_SPI_IMR);
	val &= ~irq;
	spi_writel(base + OFFSET_SPI_IMR, val);
}
void mc_spi_enable_irq(uint base, u32 irq)
{
	u32 val = 0;

	val = spi_readl(base + OFFSET_SPI_IMR);
	val |= irq;
	spi_writel(base + OFFSET_SPI_IMR, val);

}
u32 mc_spi_get_status(uint base)
{
	u32 val = 0;
	val = spi_readl(base + OFFSET_SPI_SR);
	return val;
}
void mc_spi_enable_port(uint base, u32 port)
{
	u32 val = 0;
	val = spi_readl(base + OFFSET_SPI_SER);
	port = 1 << port;
	val = SPI_BFINS(SSIEN,port,val);
	spi_writel(base + OFFSET_SPI_SER, val);
}
void mc_spi_disable_port(uint base, u32 port)
{
	u32 val = 0;
	val = spi_readl(base + OFFSET_SPI_SER);
	port = 0 << port;
	val = SPI_BFINS(SSIEN,port,val);
	spi_writel(base + OFFSET_SPI_SER,val);
}
void mc_spi_rx_sample_delay(uint base, u32 delay)
{
	delay = delay & 0xff;
	spi_writel(base + OFFSET_SPI_RX_SAMPLE_DELAY,delay);
}

static inline u32 tx_max(u32 base)
{
	u32 tx_room, rxtx_gap;
	tx_room = SPI_FIFO_DEPTH - __raw_readl(base + OFFSET_SPI_TXFLR);

	rxtx_gap = SPI_FIFO_DEPTH - (spi.rx_xfer_len - spi.tx_xfer_len);

	return min3(spi.tx_xfer_len, tx_room, rxtx_gap);
}

static inline u32 rx_max(u32 base)
{
	return min(spi.rx_xfer_len, __raw_readl(base + OFFSET_SPI_RXFLR));
}

static void dw_writer(u32 base)
{
	u32 max = tx_max(base);
	u32 data_reg = base + OFFSET_SPI_DR;
	while (max--) {
		if (spi.tx_buf)
			__raw_writel(*spi.tx_buf++, data_reg);
		else
			__raw_writel(0xff, data_reg);
		--spi.tx_xfer_len;
	}
}

static void dw_reader(u32 base)
{
	u32 max = rx_max(base);
	u32 data_reg = base + OFFSET_SPI_DR;
	while (max--) {
		if (spi.rx_buf)
			*spi.rx_buf++ = __raw_readl(data_reg);
		--spi.rx_xfer_len;
	}
}

void mc_spi_xfer_set_cs(u32 base, u8 enable)
{
	switch (enable)
	{
		case false:
			if (base == REG_SPI0_BASE){
				clrbits_le32(0xB070000, BIT(3));
			} else if (base == REG_SPI1_BASE) {
				clrbits_le32(0xB090000, BIT(0));//13*13 board
				clrbits_le32(0xB030000, BIT(7));//11*11 board
			}
			break;
		case true:
			if (base == REG_SPI0_BASE){
				setbits_le32(0xB070000, BIT(3));
			} else if (base == REG_SPI1_BASE) {
				setbits_le32(0xB090000, BIT(0));//13*13 board
				setbits_le32(0xB030000, BIT(7));//11*11 board
			}
			break;
		default:
			break;
	}
}

void mc_spi_set_slave_mode(u32 base, u8 is_slave)
{
	switch (is_slave)
	{
		case true:
			if (base == REG_SPI0_BASE)
				setbits_le32(0xC0000C8, BIT(0));
			else if (base == REG_SPI1_BASE)
				setbits_le32(0xC0000C8, BIT(1));
			break;
		case false:
			clrbits_le32(0xC0000C8, BIT(0) | BIT(1));
			break;
		default:
			break;
	}
}

int mc_spi_xfer(u32 base, u8 *tx_buf, u8 *rx_buf, u32 len, u8 is_slave)
{
	u32 i;
	spi.tx_buf = tx_buf;
	spi.rx_buf = rx_buf;
	spi.rx_xfer_len = spi.tx_xfer_len = len;
	mc_spi_set_slave_mode(base,is_slave);
	mc_spi_xfer_set_cs(base,false);
	mc_spi_enable_port(base,SPI_PORT0);
	mc_spi_enable(base,SPI_ENABLE);
	do {
		dw_writer(base);
		for(i=0;i<100000;i++);
		dw_reader(base);
	} while (spi.rx_xfer_len);
	mc_spi_xfer_set_cs(base,true);
	if (is_slave) {
		is_slave = !is_slave;
		mc_spi_set_slave_mode(base,is_slave);
	}
	return 0;
}

void mc_spi_init(u32 base)
{
	/* disable spi */
	mc_spi_enable(base,SPI_DISABLE);

	/* frame format */
	mc_spi_set_frame_format(base,SPI_FORMAT_MOTO);

	/* spi polarty */
	mc_spi_set_polarity(base,SPI_POLARITY_LOW);

	/* spi phase */
	mc_spi_set_phase(base,SPI_PHASE_SCLK_TOGGLE_MIDDLE);

	/* spi transfer mode */
	mc_spi_set_transfer_mode(base,SPI_TX_RX_MODE);

	/* transfer data size */
	mc_spi_set_data_size(base,SPI_DATA_SIZE_8BIT);

	/* baudrate */
	mc_spi_set_baudrate(base,0x30);//0x30-->500k   0xf0-->100k

	/* mask intr */
	mc_spi_disable_irq(base,0x3f);

	/* enable cs */
	mc_spi_enable_port(base,SPI_PORT0);
	mc_spi_enable(base,SPI_ENABLE);
}

void mc_spi_pinmux_cfg(void)
{
	__raw_writel(0x2, 0x10200160);//SPI0_CLK
	__raw_writel(0x2, 0x10200164);//SPI0_DO
	__raw_writel(0x2, 0x10200168);//SPI0_DI
	__raw_writel(0x1, 0x1020016C);//SPI0_CSN0 GPIO8_3

	/* 13*13 */
	__raw_writel(0x7, 0x102001a4);
	__raw_writel(0x1, 0x102001a8);//SPI1_CSN0 GPIO10_0
	__raw_writel(0x7, 0x102001ac);
	__raw_writel(0x7, 0x102001b0);

	/* 11*11 */
	__raw_writel(0x6, 0x102000a8);
	__raw_writel(0x1, 0x102000ac);//SPI1_CSN0 GPIO4_7
	__raw_writel(0x6, 0x102000b0);
	__raw_writel(0x6, 0x102000b4);
}

void board_spi_init(void)
{

	molchip_spi_clk_enable();

	reset_spi();

	mc_spi_pinmux_cfg();

	mc_spi_init(REG_SPI0_BASE);
	mc_spi_init(REG_SPI1_BASE);
}
