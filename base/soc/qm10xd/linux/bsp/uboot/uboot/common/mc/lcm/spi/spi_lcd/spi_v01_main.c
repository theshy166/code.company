#include "spi_v01_drv_mpi.h"
#include "spi_cs_gpio_drv.h"
#include "../lcdmgr_comm_inter.h"
#include "dma/spi_lcd_dma.h"
#include "../lcdmgr_comm_inter.h"
#include "fy_comm_lcdmgr.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#define GPIO_BASE_ADDR (0x1AE60000)
#define SPI_BASE_ADDR  (0x1A000000)
#define SYS_BASE_ADDR (0x18000000)
#define DMA_BASE_ADDR (0x10300000)

#define SPI_DR                  (0x0060) //R/W

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
spi_lcd_show_done_callback spSpiDoneCallback;

typedef struct {
    char name[64];
    spi_dfs_mode_e spiMode;
    int32_t width;
    int32_t height;
    spi_lcd_format_e format;
    int32_t fps;
    struct completion dma_completion;
}spi_lcd_info_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
spi_lcd_info_t s_spi_lcd = {
"spi_lcd",
    spi_dsf_8bits,
    320,
    240,
    spi_lcd_rgb565,
    30
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
spi_lcd_info_t *get_spi_info(void)
{
    return &s_spi_lcd;
}

int32_t spi_lcd_reset_gpio(void)
{
    int32_t rtn = SUCCESS;

    spi_rest_gpio_init(GPIO_BASE_ADDR);

    spi_lcd_reset(0, 1);
    mdelay(1);
    spi_lcd_reset(0, 0);
    mdelay(1);//30
    spi_lcd_reset(0, 1);
    //mdelay(120);

    return rtn;
}

int32_t spi_lcd_init(void)
{
    int32_t rtn = SUCCESS;

    mdelay(120);
    spi_single_write_cmd(0x11);
    //mdelay(120);

    spi_single_write_cmd(0x36);
    spi_single_write_data(0x00);   //RGB

    spi_single_write_cmd(0x3A);
    spi_single_write_data(0x55);   //16BIT  ;66 18bit

    spi_single_write_cmd(0xB2);
    spi_single_write_data(0x05);
    spi_single_write_data(0x05);
    spi_single_write_data(0x00);
    spi_single_write_data(0x33);
    spi_single_write_data(0x33);

    spi_single_write_cmd(0xB7);
    spi_single_write_data(0x35);

    spi_single_write_cmd(0xBB);
    spi_single_write_data(0x21);

    spi_single_write_cmd(0xC0);
    spi_single_write_data(0x2C);

    spi_single_write_cmd(0xC2);
    spi_single_write_data(0x01);

    spi_single_write_cmd(0xC3);
    spi_single_write_data(0x0B);

    spi_single_write_cmd(0xC4);
    spi_single_write_data(0x20);

    spi_single_write_cmd(0xC6);
    spi_single_write_data(0x0F);   //60HZ dot inversion

    spi_single_write_cmd(0xD0);
    spi_single_write_data(0xA4);
    spi_single_write_data(0xA1);

    spi_single_write_cmd(0xD6);
    spi_single_write_data(0xA1);

    spi_single_write_cmd(0xE0);
    spi_single_write_data(0xD0);
    spi_single_write_data(0x04);
    spi_single_write_data(0x08);
    spi_single_write_data(0x0A);
    spi_single_write_data(0x09);
    spi_single_write_data(0x05);
    spi_single_write_data(0x2D);
    spi_single_write_data(0x43);
    spi_single_write_data(0x49);
    spi_single_write_data(0x09);
    spi_single_write_data(0x16);
    spi_single_write_data(0x15);
    spi_single_write_data(0x26);
    spi_single_write_data(0x2B);

    spi_single_write_cmd(0xE1);
    spi_single_write_data(0xD0);
    spi_single_write_data(0x03);
    spi_single_write_data(0x09);
    spi_single_write_data(0x0A);
    spi_single_write_data(0x0A);
    spi_single_write_data(0x06);
    spi_single_write_data(0x2E);
    spi_single_write_data(0x44);
    spi_single_write_data(0x40);
    spi_single_write_data(0x3A);
    spi_single_write_data(0x15);
    spi_single_write_data(0x15);
    spi_single_write_data(0x26);
    spi_single_write_data(0x2A);

    spi_single_write_cmd(0x21);

    spi_single_write_cmd(0x29);

    mdelay(100);

    spi_single_write_cmd(0x2c);

    return rtn;
}

int32_t spi_lcd_show_cpu(unsigned short *data)
{
    int32_t rtn = SUCCESS;

    //DRV_LCDMGR_INFO("%s %d:enter\n", __func__, __LINE__);
    //mdelay(100);
    spi_burst_data_write_cpu(data, 320*240);

    return rtn;
}



int32_t spi_lcd_show_dma(unsigned long long phyaddr)
{
    spi_lcd_info_t *info = get_spi_info();
    int32_t rtn = SUCCESS;

    DRV_LCDMGR_INFO("enter");
    spi_burst_data_write_dma(phyaddr, 320*240*2);
    rtn = wait_for_completion_timeout(&info->dma_completion, usecs_to_jiffies(5000 * 1000));
    if(rtn == 0){
        DRV_LCDMGR_LOG_ERR("timeout");
        rtn = FY_ERR_LCDMGR_SPI_DMA_TIMEOUT;
    }else{
        //DRV_LCDMGR_INFO("success");
        rtn = SUCCESS;
    }

    return rtn;
}

int32_t spi_bit_width_set(spi_dfs_mode_e bitWidth)
{
    spi_io_ctrl_in_t ioCtrlIn = {0x00};
    spi_io_ctrl_out_t ioCtrlOut = {0x00};
    spi_bit_width_in_t bitWidthIn = {0x00};
    spi_bit_width_out_t bitWidthOut = {0x00};
    int32_t rtn = SUCCESS;

    ioCtrlIn.cmd = spi_cmd_bit_width;
    ioCtrlIn.param = &bitWidthIn;

    ioCtrlOut.param = &bitWidthOut;

    bitWidthIn.bitWidth = bitWidth;

    rtn = spi_ioCtrl(&ioCtrlIn, &ioCtrlOut);
    return rtn;
}

int32_t spi_tx_mode_set(spi_tx_mode_e tx_mode)
{
    spi_io_ctrl_in_t ioCtrlIn = {0x00};
    spi_io_ctrl_out_t ioCtrlOut = {0x00};
    spi_tx_mode_in_t txModeIn = {0x00};
    spi_tx_mode_out_t txModeOut = {0x00};
    int32_t rtn = 0;

    ioCtrlIn.cmd = spi_cmd_tx_mode;
    ioCtrlIn.param = &txModeIn;
    ioCtrlOut.param = &txModeOut;

    txModeIn.mode = tx_mode;

    rtn = spi_ioCtrl(&ioCtrlIn, &ioCtrlOut);
    return rtn;
}

void spi_log_set(spi_debug_log_e mode, uint32_t eb)
{
    spi_io_ctrl_in_t ioCtrlIn = {0x00};
    spi_io_ctrl_out_t ioCtrlOut = {0x00};
    spi_debug_log_in_t debugLogIn = {0x00};
    spi_debug_log_out_t debugLogOut = {0x00};

    ioCtrlIn.cmd = spi_cmd_debug_log;
    ioCtrlIn.param = &debugLogIn;
    ioCtrlOut.param = &debugLogOut;

    debugLogIn.mode = mode;
    debugLogIn.eb = eb;

    spi_ioCtrl(&ioCtrlIn, &ioCtrlOut);
}

void spi_dma_callback(void){
    spi_lcd_info_t *info = get_spi_info();

    DRV_LCDMGR_INFO("enter")
    complete(&info->dma_completion);

    return;
}

FY_S32 spi_lcd_start(LCDMGR_SHOW_MODE eShowMode){
    spi_lcd_info_t *info = get_spi_info();
    spi_init_in_t spi_init_in = {0x00};
    spi_init_out_t spi_init_out = {0x00};
    int32_t rtn = SUCCESS;

    DRV_LCDMGR_INFO("spi lcd test entery!\n");

    spi_clk_on(1);
    spi_lcd_pinmux();
    spi_lcd_gpio_request(1);

    spi_init_in.spi_addr = SPI_BASE_ADDR;
    spi_init_in.gpio_addr = GPIO_BASE_ADDR;
    spi_init_in.sys_addr = SYS_BASE_ADDR;
    spi_init_in.dma_addr = DMA_BASE_ADDR;
    spi_init_in.spiMode = info->spiMode;
    spi_init_in.funDebug = 1;
    spi_init_in.paramDebug = 1;

    // cs rs gpio init output mode
    spi_lcd_reset_gpio();

    rtn = spi_v0_init(&spi_init_in, &spi_init_out);
    if(rtn) goto EXIT;

    spi_freq_init();
    spi_lcd_init();

    rtn = spi_bit_width_set(spi_dsf_16bits);
    if(rtn) goto EXIT;

    init_completion(&info->dma_completion);
    if(eShowMode == LCDMGR_SHOW_CPU){
        rtn = spi_tx_mode_set(spi_tx_cpu);
    }else{
        rtn += spi_lcd_dma_setup(spi_init_in.spi_addr + SPI_DR, spi_dma_callback);
        rtn += spi_tx_mode_set(spi_tx_dma);
    }


EXIT:
    return rtn;
}
FY_S32 spi_lcd_stop(LCDMGR_SHOW_MODE eShowMode){
	if(eShowMode == LCDMGR_SHOW_DMA){
		spi_lcd_dma_release();
	}
	spi_lcd_gpio_request(0);
	spi_clk_on(0);
	return 0;
}
FY_S32 spi_lcd_show(LCDMGR_SHOW_MODE stShowMode, uint8_t *pu8ImgBuf, unsigned long long phyAddr,uint32_t u32Len){
    if(stShowMode == LCDMGR_SHOW_CPU){
        spi_lcd_show_cpu((unsigned short *)pu8ImgBuf);
    }else{
        spi_lcd_show_dma(phyAddr);
    }
    return 0;
}
LCDMGR_LLD_FUNC_S spi_mod_init(void){
	LCDMGR_LLD_FUNC_S stFunc;
	stFunc.start = spi_lcd_start;
	stFunc.stop = spi_lcd_stop;
	stFunc.show = spi_lcd_show;
	return stFunc;
}



/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
