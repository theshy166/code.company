#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/uart.h>
#include <mach/dma.h>
#include <serial.h>
#include "serial_molchip.h"

#ifdef UART_RX_DMA
struct dma_rx_private_data 	dma_rx_private;

static void dma_rx_private_init(){
	struct	dma_rx_private_data *rxpdata;
	rxpdata = &dma_rx_private;
        rxpdata->clen = MOL_NON_FIFO_DMA_LENGTH_MAX;
        rxpdata->rlen = MOL_NON_FIFO_DMA_LENGTH_MAX;
	rxpdata->crx_dma_buf =(unsigned char*)UART_RX_ADDR;

}

void dma_uart_rx_init(u32 chn,u32 uart_addr){
	dma_set_src(chn,uart_addr);
	dma_set_dest(chn,UART_RX_ADDR);
	dma_set_trans_len(chn,MOL_NON_FIFO_DMA_LENGTH_MAX);
	dma_frag_len_reg_set(chn,FRAG_LEN_REG_VAL);
	dma_set_trsf_step(chn,1);
	dma_blk_len_reg_set(chn,1);
	uart_int_enable(chn);
	uart_request_cid(chn,uart_addr);
	enable_channel(chn);
}

#endif
/* Information about a serial port */
struct ts01_serial_platdata {
	struct ts01_uart *reg;  /* address of registers in physical memory */
	u8 port_id;     /* uart port number */
};

static void __maybe_unused ts01_serial_init(struct ts01_uart *uart)
{
	u32 val;
	//select data len
	val = readl(&uart->lcr);
	val &= ~(LCR_DLS_MASK << LCR_DLS_OFFSET);
	val |= (DLS_8b << LCR_DLS_OFFSET);
	writel(val,&uart->lcr);

	//pority enable
	val = readl(&uart->lcr);
	val &= ~LCR_PEN;
	writel(val, &uart->lcr);

	//config stop bit
	val = readl(&uart->lcr);
	val &= ~LCR_STOP;
	writel(val,&uart->lcr);
	//fifo enable
	val = readl(&uart->icr.FCR);
	val |= FCR_FIFO_ENABLE;
	writel(val,&uart->icr.FCR);
}

#ifdef UART_RX_DMA
static void  uart_dma_init(struct udevice *dev){
	
	u32 val;
	struct ts01_serial_platdata *plat = dev->platdata;
	struct ts01_uart *const uart = plat->reg;
	//select data len
	val = readl(&uart->lcr);
	val &= ~(LCR_DLS_MASK << LCR_DLS_OFFSET);
	val |= (DLS_8b << LCR_DLS_OFFSET);
	writel(val,&uart->lcr);

	//pority enable
	val = readl(&uart->lcr);
	val &= ~LCR_PEN;
	writel(val, &uart->lcr);

	//config stop bit
	val = readl(&uart->lcr);
	val &= ~LCR_STOP;
	writel(val,&uart->lcr);
	//fifo disable
	val = readl(&uart->icr.FCR);
	val &= ~FCR_FIFO_ENABLE;
	writel(val,&uart->icr.FCR);

	dma_getc_init();
	dma_rx_private_init();
	dma_uart_rx_init(UART_RX_CHN,(u32)uart);

}
#endif
int ts01_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct ts01_serial_platdata *plat = dev->platdata;
	struct ts01_uart *const uart = plat->reg;

	u32 divisor;
	u32 val;

//	if(plat->port_id == 0)
		writel(0, CLK_UART0_CFG);
//	else
//		writel(1,CLK_UART1_CFG);

	if(baudrate == BAUDRATE_115200)
		divisor = UART_CTL_CLK_24M >> 4;
	else
		divisor = UART_CTL_CLK_64M >> 4;

	divisor = divisor / baudrate;

	val = readl(&uart->lcr);
	val |= LCR_DLAB;
	writel(val,&uart->lcr); //enable access divisor latch bit

	writel(divisor,&uart->rtd.DLL);
	writel(0,&uart->idh.DLH);
	val &= ~LCR_DLAB;
	writel(val,&uart->lcr); //disable access divisor latch bit

	return 0;
}

static int ts01_serial_probe(struct udevice *dev)
{
	struct ts01_serial_platdata *plat = dev->platdata;
	struct ts01_uart *const uart = plat->reg;

	ts01_serial_init(uart);

	return 0;
}

#ifdef UART_RX_DMA
static int ts01_serial_getc_dma(struct udevice *dev)
{
	u32 rbr;
	while((dma_rx_private.rlen - dma_get_trans_len(UART_RX_CHN)) == 0){
		if(dma_get_trans_len(UART_RX_CHN) <= TRANS_RESET_LEN){
			dma_set_trans_len(UART_RX_CHN,MOL_NON_FIFO_DMA_LENGTH_MAX);
        		dma_rx_private.clen = MOL_NON_FIFO_DMA_LENGTH_MAX;
       		        dma_rx_private.rlen = MOL_NON_FIFO_DMA_LENGTH_MAX;
		}
	}

	dma_rx_private.clen = dma_get_trans_len(UART_RX_CHN);
	rbr = *dma_rx_private.crx_dma_buf;
	dma_rx_private.crx_dma_buf++;
	if(dma_rx_private.crx_dma_buf ==(u8*)(UART_RX_ADDR + UART_RX_BUFFER_SIZE)){
		dma_set_dest(UART_RX_CHN,UART_RX_ADDR);
       		dma_rx_private.crx_dma_buf =(unsigned char*)UART_RX_ADDR;
	}

	dma_rx_private.rlen--;
	return rbr;
}
#endif

static int ts01_serial_getc(struct udevice *dev)
{
	struct ts01_serial_platdata *plat = dev->platdata;
	struct ts01_uart *const uart = plat->reg;

	u32 lsr = 0;
	u32 rbr;
	do {
		lsr = readl(&uart->lsr);
		lsr &= LSR_DR;
	} while (!lsr);
	rbr = readl(&uart->rtd.RBR);

	return rbr;
}
static int ts01_serial_putc(struct udevice *dev, const char ch)
{
	struct ts01_serial_platdata *plat = dev->platdata;
	struct ts01_uart *const uart = plat->reg;

	u32 lsr = 0;
	do
	{
		lsr = readl(&uart->lsr);
		lsr &= LSR_THRE;
	}while(!lsr);
	writel(ch,&uart->rtd.THR);
	return 0;
}
static int ts01_serial_ofdata_to_platdata(struct udevice *dev)
{
	struct ts01_serial_platdata *plat = dev->platdata;
	fdt_addr_t addr;

	addr = devfdt_get_addr(dev);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	plat->reg = (struct ts01_uart *)addr;
	plat->port_id = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev),
					"id", dev->seq);
	return 0;
}

static int ts01_serial_pending(struct udevice *dev, bool input)
{
	struct ts01_serial_platdata *plat = dev->platdata;
	struct ts01_uart *const uart = plat->reg;
	uint32_t status = readl(&uart->lsr);

	if (input)
		return (status & LSR_DR);
	else
		return !(status & LSR_TEMT);
}

static  struct dm_serial_ops ts01_serial_ops = {
	.putc = ts01_serial_putc,
	.getc = ts01_serial_getc,
	.setbrg = ts01_serial_setbrg,
	.pending = ts01_serial_pending,
#ifdef UART_RX_DMA
	.uartinit = uart_dma_init,	
#endif
};

#ifdef UART_RX_DMA
void dma_getc_init(){
	ts01_serial_ops.getc = ts01_serial_getc_dma;
}
#endif
static const struct udevice_id ts01_serial_ids[] = {
	{ .compatible = "molchip,serial" },
	{ }
};

U_BOOT_DRIVER(serial_ts01) = {
	.name	= "serial_ts01",
	.id	= UCLASS_SERIAL,
	.of_match = ts01_serial_ids,
	.ofdata_to_platdata = ts01_serial_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct ts01_serial_platdata),
	.probe = ts01_serial_probe,
	.ops	= &ts01_serial_ops,
	.flags = DM_FLAG_PRE_RELOC,
};

