#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/spic.h>
#include <mach/dma.h>
#include <mach/uart_drv.h>

struct spi_cfg {
	u32	base;
	u8	*tx_buf;
	u8	*rx_buf;
	u32	rx_xfer_len;
	u32	tx_xfer_len;
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

struct spi_cfg spi_slave[2];

void mc_spi_slave_clk_enable(void)
{
	setbits_le32(REG_CEN_PERI_CLK_CTRL, CKG_SPI0_EN | CKG_SPI1_EN);
	setbits_le32(REG_CEN_PERI_APB_CLK_CTRL, CKG_APB_SPI0_EN | CKG_APB_SPI1_EN);
}

void reset_spi_slave(u32 id)
{
	u32 i;

	/* SPI RESET */
	if (id == 0) {
		setbits_le32(REG_AP_PERI_SOFT_RST0, SPI0_SOFT_RST);
		for(i=0;i<0x1000;i++);
		clrbits_le32(REG_AP_PERI_SOFT_RST0, SPI0_SOFT_RST);
	} else if (id == 1) {
		setbits_le32(REG_AP_PERI_SOFT_RST0, SPI1_SOFT_RST);
		for(i=0;i<0x1000;i++);
		clrbits_le32(REG_AP_PERI_SOFT_RST0, SPI1_SOFT_RST);
	}
}

void mc_spi_slave_enable(u32 id, u32 enable)
{
	u32 val;

	if(enable)
		val = 1;
	else
		val = 0;

	spi_writel(spi_slave[id].base + OFFSET_SPI_SSIENR,SPI_BF(SER,val));
}
void mc_spi_slave_set_polarity(u32 id, u32 polarity)
{
	u32 val;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(SCPOL,polarity,val);
	spi_writel(spi_slave[id].base + OFFSET_SPI_CTRL0,val);
}
void mc_spi_slave_set_phase(u32 id, u32 phase)
{
	u32 val;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(SCPH,phase,val);

	spi_writel(spi_slave[id].base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_slave_set_frame_format(u32 id, u32 format)
{
	u32 val = 0;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(FRF,format,val);

	spi_writel(spi_slave[id].base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_slave_set_data_size(u32 id, u32 size)
{
	u32 val = 0;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(DFS,size,val);
	spi_writel(spi_slave[id].base + OFFSET_SPI_CTRL0, val);
}
void mc_spi_slave_set_transfer_mode(u32 id, u32 mode)
{
	u32 val = 0;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_CTRL0);
	val = SPI_BFINS(TMOD,mode,val);

	spi_writel(spi_slave[id].base + OFFSET_SPI_CTRL0, val);
}

void mc_spi_slave_disable_irq(u32 id, u32 irq)
{
	u32 val = 0;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_IMR);
	val &= ~irq;
	spi_writel(spi_slave[id].base + OFFSET_SPI_IMR, val);
}
void mc_spi_slave_enable_irq(u32 id, u32 irq)
{
	u32 val = 0;

	val = spi_readl(spi_slave[id].base + OFFSET_SPI_IMR);
	val |= irq;
	spi_writel(spi_slave[id].base + OFFSET_SPI_IMR, val);

}
u32 mc_spi_slave_get_status(u32 id)
{
	u32 val = 0;
	val = spi_readl(spi_slave[id].base + OFFSET_SPI_SR);
	return val;
}

void mc_spi_slave_rx_sample_delay(u32 id, u32 delay)
{
	delay = delay & 0xff;
	spi_writel(spi_slave[id].base + OFFSET_SPI_RX_SAMPLE_DELAY,delay);
}

static inline u32 tx_max(u32 id)
{
	u32 hw_tx_level;

	hw_tx_level = __raw_readl(spi_slave[id].base + OFFSET_SPI_TXFLR);
	hw_tx_level = spi_slave[id].tx_xfer_len - hw_tx_level;
	return min(hw_tx_level, spi_slave[id].tx_xfer_len);
}

static inline u32 rx_max(u32 id)
{
	return min(spi_slave[id].rx_xfer_len, __raw_readl(spi_slave[id].base + OFFSET_SPI_RXFLR));
}


void mc_spi_slave_set_slave_mode(u32 id)
{
	if (id == 0)
		setbits_le32(0xC0000C8, BIT(0));
	else if (id == 1)
		setbits_le32(0xC0000C8, BIT(1));
}

int poll_tx_only_data(u32 id)
{
	u32 tx_fifo_capability;
	u32 data_reg = spi_slave[id].base + OFFSET_SPI_DR;
	while (spi_slave[id].tx_xfer_len)
	{
		tx_fifo_capability = tx_max(id);
		spi_slave[id].tx_xfer_len -= tx_fifo_capability;
		while (tx_fifo_capability)
		{
			__raw_writel(*spi_slave[id].tx_buf++,data_reg);
			tx_fifo_capability--;
		}
	}
	return 0;
}

int poll_rx_only_data(u32 id)
{
	u32 rx_fifo_capability;
	u32 data_reg = spi_slave[id].base + OFFSET_SPI_DR;
	do
	{
		rx_fifo_capability = rx_max(id);
		spi_slave[id].rx_xfer_len -= rx_fifo_capability;
		while (rx_fifo_capability)
		{
			*spi_slave[id].rx_buf++ = __raw_readl(data_reg);
			rx_fifo_capability--;
		}


	}while (spi_slave[id].rx_xfer_len);
	return 0;
}

int mc_spi_slave_xfer(u32 id, u8 *tx_buf, u8 *rx_buf, u32 len)
{
	spi_slave[id].tx_buf = tx_buf;
	spi_slave[id].rx_buf = rx_buf;
	spi_slave[id].rx_xfer_len = spi_slave[id].tx_xfer_len = len;

	mc_spi_slave_enable(id,SPI_DISABLE);
	mc_spi_slave_enable(id,SPI_ENABLE);

	if ((rx_buf == NULL) && (tx_buf != NULL))
		poll_tx_only_data(id);

	else if ((tx_buf == NULL) && (rx_buf != NULL))
		poll_rx_only_data(id);

	return len;
}

void mc_spi_slave_pinmux_cfg(u32 id)
{
	if (id == 0) {
		__raw_writel(0x2, 0x10200160);//SPI0_CLK
		__raw_writel(0x2, 0x10200164);//SPI0_DO
		__raw_writel(0x2, 0x10200168);//SPI0_DI
		__raw_writel(0x2, 0x1020016C);//SPI0_CSN0
	}
	else if (id == 1) {
		/* 13*13 */
		__raw_writel(0x7, 0x102001a4);
		__raw_writel(0x7, 0x102001ac);
		__raw_writel(0x7, 0x102001b0);
		__raw_writel(0x7, 0x102001a8);//13*13 SPI1_CSN0

		/* 11*11 */
		__raw_writel(0x6, 0x102000a8);
		__raw_writel(0x6, 0x102000b0);
		__raw_writel(0x6, 0x102000b4);
		__raw_writel(0x6, 0x102000ac);//11*11 SPI1_CSN0
	}
}

void mc_spi_slave_init(u32 id)
{
	mc_spi_slave_set_slave_mode(id);

	mc_spi_slave_pinmux_cfg(id);
	/* disable spi */
	mc_spi_slave_enable(id,SPI_DISABLE);

	/* frame format */
	mc_spi_slave_set_frame_format(id,SPI_FORMAT_MOTO);

	/* spi polarty */
	mc_spi_slave_set_polarity(id,SPI_POLARITY_LOW);

	/* spi phase */
	mc_spi_slave_set_phase(id,SPI_PHASE_SCLK_TOGGLE_MIDDLE);

	/* spi transfer mode */
	mc_spi_slave_set_transfer_mode(id,SPI_TX_RX_MODE);

	/* transfer data size */
	mc_spi_slave_set_data_size(id,SPI_DATA_SIZE_8BIT);

	/* mask intr */
	mc_spi_slave_disable_irq(id,0x3f);

	mc_spi_slave_enable(id,SPI_ENABLE);
}

void board_spi_slave_init(u32 id)
{
	spi_slave[0].base = REG_SPI0_BASE;
	spi_slave[1].base = REG_SPI1_BASE;
	if (id != 0 && id != 1) {
		mc_printf("Unknown spi device id!\n");
		return ;
	}
	mc_spi_slave_clk_enable();

	reset_spi_slave(id);

	mc_spi_slave_init(id);
}
