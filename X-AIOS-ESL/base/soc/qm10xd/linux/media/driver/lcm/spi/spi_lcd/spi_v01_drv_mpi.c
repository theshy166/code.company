#if defined(__KERNEL__)
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/firmware.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <linux/poll.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timer.h>
#include <linux/timekeeping.h>
#include <asm/delay.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/sizes.h>
#include <linux/io.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <asm/mmu.h>
#include <linux/iommu.h>
#include <linux/regmap.h>
#include <linux/mmu_context.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/mfd/syscon.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_reserved_mem.h>
#endif

#include "spi_v01_drv_param.h"
#include "spi_v01_drv.h"
#include "spi_v01_reg_def.h"
#include "spi_v01_drv_mpi_param.h"
#include "spi_data_dir.h"
#include "spi_cs_gpio_drv.h"
#include "spi_dma.h"
#include "reg.h"
#include "dma/spi_lcd_dma.h"
#include "debug.h"
//#include "../dma/dma.h"
//#include "fy_comm_lcdmgr.h"
#include "spi_v01_drv_mpi_param.h"

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
typedef int (*spi_ioctrl_fun)(void *inParam, void *outParam);

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef enum {
    spi_unInited = 0,
    spi_inited,
}spi_status_e;

typedef struct {
    spi_status_e status;
    uint64_t spi_addr;
    uint64_t gpio_addr;
    uint64_t sys_addr;
    uint64_t dma_addr;
    uint32_t txMode;
    uint32_t rxMode;

    uint32_t dfs;
    uint32_t frf;
    uint32_t scph;
    uint32_t scpol;
    uint32_t tmod;
    uint32_t slv_oe;
    uint32_t srl;
    uint32_t cfs;
    uint32_t dfs_32;
    uint32_t spi_frf;
    uint32_t ndf;
    uint32_t ssi_en;
    uint32_t mwmod;
    uint32_t mdd;
    uint32_t mhs;
    uint32_t ser;
    uint32_t sckdv;
    uint32_t txeim;
    uint32_t txoim;
    uint32_t rxuim;
    uint32_t rxoim;
    uint32_t rxfim;
    uint32_t mstim;
    uint32_t tdmae;
    uint32_t rdmae;
    uint32_t tx_abw;
    uint32_t rx_abw;
    uint32_t rsd;

    uint32_t funDebug;
    uint32_t paramDebug;
    uint32_t regDebug;
}spi_mpi_context_t;

typedef struct {
    spi_ioctrl_cmd_e cmd;
    spi_ioctrl_fun fun;
}spi_mpi_ioctrl_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
spi_mpi_context_t s_spi_mpi_context[2] = {0x00};


/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
spi_mpi_context_t *spi_mpi_context_get(uint32_t id)
{
    return &s_spi_mpi_context[id];
}

static int32_t _spi_v0_init_param_checker(spi_init_in_t *inParam, spi_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(inParam, NULL, "spi_init_in_t ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_init_out_t ~_~\n");

    EXIT:
    return rtn;
}

LOCAL int32_t _spi_v0_init_param_set(spi_init_in_t *inParam, spi_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(inParam, NULL, "spi_init_in_t ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_init_out_t ~_~\n");
    spiContext = spi_mpi_context_get(inParam->id);
    CHECK_PTR(spiContext, NULL, "spiContext ~_~\n");

    spiContext->spi_addr = inParam->spi_addr;
    spiContext->gpio_addr = inParam->gpio_addr;
    spiContext->sys_addr = inParam->sys_addr;
    spiContext->dma_addr = inParam->dma_addr;

    //spiContext->dfs = inParam->spiMode;
    spiContext->frf = 0;
    spiContext->scph = 0;
    spiContext->scpol = 0;
    spiContext->tmod = 0;
    spiContext->slv_oe = 0;
    spiContext->srl = 0;
    spiContext->cfs = 0;
    //spiContext->dfs_32 = inParam->spiMode;
    spiContext->spi_frf = 0;
    spiContext->ndf = 1;
    spiContext->ssi_en = 1;
    spiContext->mwmod = 0;
    spiContext->mdd = 1;
    spiContext->mhs = 0;
    spiContext->ser = 1;
    spiContext->sckdv = 2;
    spiContext->txeim = 1;
    spiContext->txoim = 1;
    spiContext->rxuim = 1;
    spiContext->rxoim = 1;
    spiContext->rxfim = 1;
    spiContext->mstim = 1;

    spiContext->tdmae = 0;
    spiContext->rdmae = 0;
    spiContext->tx_abw = 0xf;
    spiContext->rx_abw = 0;

    spiContext->rsd = 0;

    spiContext->funDebug = inParam->funDebug;
    spiContext->paramDebug = inParam->paramDebug;

    EXIT:
    return rtn;
}

#if 0
static int32_t _spi_16_v0_init_param_set(spi_init_in_t *inParam, spi_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(inParam, NULL, "spi_init_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_init_out_t is null ~_~\n");

    spiContext->spi_addr = inParam->spi_addr;
    spiContext->gpio_addr = inParam->gpio_addr;

    spiContext->dfs = 7;
    spiContext->frf = 0;
    spiContext->scph = 0;
    spiContext->scpol = 0;
    spiContext->tmod = 0;
    spiContext->slv_oe = 0;
    spiContext->srl = 0;
    spiContext->cfs = 0;
    spiContext->dfs_32 = 15;
    spiContext->spi_frf = 0;
    spiContext->ndf = 1;
    spiContext->ssi_en = 1;
    spiContext->mwmod = 0;
    spiContext->mdd = 1;
    spiContext->mhs = 0;
    spiContext->ser = 1;
    spiContext->sckdv = 6;
    spiContext->txeim = 1;
    spiContext->txoim = 1;
    spiContext->rxuim = 1;
    spiContext->rxoim = 1;
    spiContext->rxfim = 1;
    spiContext->mstim = 1;

    spiContext->tx_abw = 0;

    spiContext->tdmae = 0;
    spiContext->rdmae = 0;
    spiContext->tx_abw = 0;
    spiContext->rx_abw = 0;

    spiContext->rsd = 0;

    spiContext->funDebug = inParam->spi_addr;
    spiContext->paramDebug = inParam->gpio_addr;

    EXIT:

    return rtn;
}
#endif

LOCAL int32_t _spi_v0_cfg(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = (spi_mpi_context_t *)inParam; //handle

    CHECK_PTR(inParam, NULL, "inParam is nulll ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is nulll ~_~\n");

    rtn = spi_cs_rs_init(spiContext->gpio_addr);
    EXIT_IF_FAIL(rtn, rtn, "spi_cs_rs_init ~_~\n");

    //0x08
    rtn = spi_set_ssi_en(spiContext->spi_addr, 0);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_ssi_en ~_~\n");

    //0x00
    rtn = spi_set_dfs(spiContext->spi_addr, spiContext->dfs);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_dfs ~_~\n");

    rtn = spi_set_frf(spiContext->spi_addr, spiContext->frf);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_frf ~_~\n");

    rtn = spi_set_scph(spiContext->spi_addr, spiContext->scph);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_scph ~_~\n");

    rtn = spi_set_scpol(spiContext->spi_addr, spiContext->scpol);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_scpol ~_~\n");

    rtn = spi_set_tmod(spiContext->spi_addr, spiContext->tmod);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_tmod ~_~\n");

    rtn = spi_set_slv_oe(spiContext->spi_addr, spiContext->slv_oe);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_slv_oe ~_~\n");

    rtn = spi_set_srl(spiContext->spi_addr, spiContext->srl);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_srl ~_~\n");

    rtn = spi_set_cfs(spiContext->spi_addr, spiContext->cfs);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_cfs ~_~\n");

    rtn = spi_set_dfs_32(spiContext->spi_addr, spiContext->dfs_32);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_dfs_32 ~_~\n");

    rtn = spi_set_spi_frf(spiContext->spi_addr, spiContext->spi_frf);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_spi_frf ~_~\n");

    //0x04
    rtn = spi_set_ndf(spiContext->spi_addr, spiContext->ndf);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_ndf ~_~\n");
    //0x0C
    rtn = spi_set_mwmod(spiContext->spi_addr, spiContext->mwmod);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mwmod ~_~\n");

    rtn = spi_set_mdd(spiContext->spi_addr, spiContext->mdd);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mdd ~_~\n");

    rtn = spi_set_mhs(spiContext->spi_addr, spiContext->mhs);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mhs ~_~\n");
    //0x10
    rtn = spi_set_ser(spiContext->spi_addr, spiContext->ser);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_ser ~_~\n");
    //0x14
    rtn = spi_set_sckdv(spiContext->spi_addr, spiContext->sckdv);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_sckdv ~_~\n");
    //0x2C
    rtn = spi_set_txeim(spiContext->spi_addr, spiContext->txeim);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_txeim ~_~\n");

    rtn = spi_set_txoim(spiContext->spi_addr, spiContext->txoim);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_txoim ~_~\n");

    rtn = spi_set_rxuim(spiContext->spi_addr, spiContext->rxuim);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_rxuim ~_~\n");

    rtn = spi_set_rxoim(spiContext->spi_addr, spiContext->rxoim);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_rxoim ~_~\n");

    rtn = spi_set_rxfim(spiContext->spi_addr, spiContext->rxfim);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_rxfim ~_~\n");

    rtn = spi_set_mstim(spiContext->spi_addr, spiContext->mstim);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mstim ~_~\n");

    //0xf0
    rtn = spi_set_rx_sample_dly(spiContext->spi_addr, spiContext->rsd);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_tx_abw ~_~\n");

    //0x4C
    rtn = spi_set_dma_tx_eb(spiContext->spi_addr, spiContext->tdmae);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mstim ~_~\n");

    rtn = spi_set_dma_rx_eb(spiContext->spi_addr, spiContext->rdmae);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mstim ~_~\n");
    //0x50
    rtn = spi_set_dma_tx_abw(spiContext->spi_addr, spiContext->tx_abw);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_tx_abw ~_~\n");

    rtn = spi_set_dma_rx_abw(spiContext->spi_addr, spiContext->rx_abw);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_dma_rx_abw ~_~\n");

    //0x08
    rtn = spi_set_ssi_en(spiContext->spi_addr, spiContext->ssi_en);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_ssi_en ~_~\n");

    //tx
    rtn = spi_data_dir(spiContext->sys_addr, spi_data_tx);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_ssi_en ~_~\n");

    EXIT:

    return rtn;
}

LOCAL int32_t _spi_v0_dma_start(void *handle)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = (spi_mpi_context_t *)handle;//spi_mpi_context_get();

    spiContext->tdmae = 1;
    spiContext->rdmae = 1;

    //0x4C
    rtn = spi_set_dma_tx_eb(spiContext->spi_addr, spiContext->tdmae);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mstim ~_~\n");

    rtn = spi_set_dma_rx_eb(spiContext->spi_addr, spiContext->rdmae);
    EXIT_IF_FAIL(rtn, rtn, "spi_set_mstim ~_~\n");

    EXIT:

    return rtn;
}

int32_t spi_freq_init(void){
	//0x1930002c[1:0]: 0:24M 1:64M 2:91.4M 3:128M
	regWrite32(0x1930202c, 0x2);
	return 0;
}

LOCAL int32_t _spi_mpi_bit_width_set(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    //spi_init_in_t spi_init_in = {0x00};
    spi_init_out_t spi_init_out = {0x00};
    spi_bit_width_in_t *bitWidthIn = (spi_bit_width_in_t *)inParam;
    //spi_bit_width_out_t *bitWidthOut = (spi_bit_width_out_t *)outParam;

    CHECK_PTR(inParam, NULL, "inParam is null ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is null ~_~\n");
    CHECK_PTR(bitWidthIn->handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)bitWidthIn->handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    spiContext->dfs = bitWidthIn->bitWidth;
    spiContext->dfs_32 = bitWidthIn->bitWidth;

    rtn = _spi_v0_cfg((void *)spiContext, &spi_init_out);
    EXIT_IF_FAIL(rtn, rtn, "spi_v0_cfg ~_~\n");

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

LOCAL int32_t _spi_mpi_tx_mode_set(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    spi_init_in_t spi_init_in = {0x00};
    spi_init_out_t spi_init_out = {0x00};
    spi_tx_mode_in_t *txModeIn = (spi_tx_mode_in_t *)inParam;
    //spi_tx_mode_out_t *txModeOut = (spi_tx_mode_out_t *)outParam;

    CHECK_PTR(inParam, NULL, "inParam is null ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is null ~_~\n");
    CHECK_PTR(txModeIn->handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)txModeIn->handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    if (spi_trx_cpu == txModeIn->mode) {
        spiContext->txMode = spi_trx_cpu;
    } else if (spi_trx_dma == txModeIn->mode) {
        spiContext->txMode = spi_trx_dma;
    } else {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "tx mode % is invalid ~_~\n", txModeIn->mode);
    }

    spiContext->tdmae = 0;
    spiContext->rdmae = 0;

    rtn = _spi_v0_cfg(&spi_init_in, &spi_init_out);
    EXIT_IF_FAIL(rtn, rtn, "spi_v0_cfg ~_~\n");

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

LOCAL int32_t _spi_mpi_rx_mode_set(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    spi_init_in_t spi_init_in = {0x00};
    spi_init_out_t spi_init_out = {0x00};
    spi_tx_mode_in_t *rxModeIn = (spi_tx_mode_in_t *)inParam;
    //spi_tx_mode_out_t *txModeOut = (spi_tx_mode_out_t *)outParam;

    CHECK_PTR(inParam, NULL, "inParam is null ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is null ~_~\n");
    CHECK_PTR(rxModeIn->handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)rxModeIn->handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    if (spi_trx_cpu == rxModeIn->mode) {
        spiContext->rxMode = spi_trx_cpu;
    } else if (spi_trx_dma == rxModeIn->mode) {
        spiContext->rxMode = spi_trx_dma;
    } else {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "rx mode % is invalid ~_~\n", rxModeIn->mode);
    }

    spiContext->tdmae = 0;
    spiContext->rdmae = 0;

    rtn = _spi_v0_cfg(&spi_init_in, &spi_init_out);
    EXIT_IF_FAIL(rtn, rtn, "spi_v0_cfg ~_~\n");

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

LOCAL int32_t _spi_mpi_debug_log_set(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    spi_debug_log_in_t *debugLogIn = (spi_debug_log_in_t *)inParam;
    //spi_debug_log_out_t *debugLogOut = (spi_debug_log_out_t *)outParam;

    CHECK_PTR(inParam, NULL, "inParam is null ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is null ~_~\n");
    CHECK_PTR(debugLogIn->handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)debugLogIn->handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    if (spi_fun_log == debugLogIn->mode) {
        spiContext->funDebug = debugLogIn->eb;
    } else if (spi_param_log == debugLogIn->mode) {
        spiContext->paramDebug = debugLogIn->eb;
    } else if (spi_register_log == debugLogIn->mode) {
        spiContext->regDebug = debugLogIn->eb;
        reg_log_enable(spiContext->regDebug);
    }

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    return rtn;
}

spi_mpi_ioctrl_t spi_mpi_ioctrl_fun[] = {
    {spi_cmd_bit_width, (spi_ioctrl_fun)_spi_mpi_bit_width_set},
    {spi_cmd_tx_mode,   (spi_ioctrl_fun)_spi_mpi_tx_mode_set},
    {spi_cmd_rx_mode,   (spi_ioctrl_fun)_spi_mpi_rx_mode_set},
    {spi_cmd_debug_log, (spi_ioctrl_fun)_spi_mpi_debug_log_set},
    {spi_cmd_max, NULL}
};

LOCAL spi_ioctrl_fun _spi_mpi_get_ioCtrl_fun(spi_ioctrl_cmd_e cmd)
{
    int32_t rtn = SUCCESS;
    spi_ioctrl_fun fun = NULL;
    int32_t i = 0x00;

    do {
        if (spi_cmd_max == spi_mpi_ioctrl_fun[i].cmd) {
            rtn = FAIL_FUN_PNULL;
            EXIT_IF_FAIL(rtn, NULL, "cmd %d fun is null ~_~!\n", cmd);
        }
        if (cmd == spi_mpi_ioctrl_fun[i].cmd) {
            fun = spi_mpi_ioctrl_fun[i].fun;
            break;
        }
        i++;
    }while(1);

    EXIT:

    return fun;
}

int32_t spi_ioCtrl(spi_io_ctrl_in_t *inParam, spi_io_ctrl_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    spi_ioctrl_fun fun = NULL;

    CHECK_PTR(inParam, NULL, "inParam is null ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is null ~_~\n");
    CHECK_PTR(inParam->handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)inParam->handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    fun = _spi_mpi_get_ioCtrl_fun(inParam->cmd);

    if (NULL != fun) {
        rtn = fun(inParam->param, outParam->param);
        EXIT_IF_FAIL(rtn, rtn, "spi_ioCtrl %d ~_~\n", inParam->cmd);
    } else {
        rtn = FAIL_FUN_PNULL;
        EXIT_IF_FAIL(rtn, NULL, "spi_ioCtrl %d fun null ~_~\n", inParam->cmd);
    }

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}


int32_t spi_v0_init(spi_init_in_t *inParam, spi_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(inParam, NULL, "spi_init_in_t ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_init_out_t ~_~\n");
    spiContext = spi_mpi_context_get(inParam->id);
    CHECK_PTR(spiContext, NULL, "spiContext ~_~\n");

    debug_fun(inParam->funDebug, __FUNCTION__, __LINE__, "start");

    if(spi_inited == spiContext->status) {
        rtn = FAIL_RE_INIT;
        EXIT_IF_FAIL(rtn, NULL, "spi%d inited ~_~!\n", inParam->id);
    }

    rtn = _spi_v0_init_param_checker(inParam, outParam);
    if(rtn) goto EXIT;

    rtn = _spi_v0_init_param_set(inParam, outParam);
    if(rtn) goto EXIT;

    rtn = _spi_v0_cfg((void *)spiContext, outParam);
    if(rtn) goto EXIT;

    outParam->handle = (void *)spiContext;

    spiContext->status = spi_inited;

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

int32_t spi_v0_deinit(spi_deinit_in_t *inParam, spi_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(inParam, NULL, "inParam is null ~_~\n");
    CHECK_PTR(outParam, NULL, "outParam is null ~_~\n");
    CHECK_PTR(inParam->handle, NULL, "deinit inParam->handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)inParam->handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    if (spi_inited == spiContext->status) {
        goto EXIT;
    }

    // close clk

    // back pin

    memset(spiContext, 0x00, sizeof(spi_mpi_context_t));

    spiContext->status = spi_unInited;

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

int32_t spi_single_write_cmd(void *handle, uint8_t cmd)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    rtn = spi_cs_gpio(spiContext->gpio_addr, 0);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 0);
    rtn = spi_set_dr(spiContext->spi_addr, cmd);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);
    rtn = spi_cs_gpio(spiContext->gpio_addr, 1);

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

int32_t spi_single_write_data(void *handle, uint8_t data)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    rtn = spi_cs_gpio(spiContext->gpio_addr, 0);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);
    rtn = spi_set_dr(spiContext->spi_addr, data);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);
    rtn = spi_cs_gpio(spiContext->gpio_addr, 1);

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

void spi_clk_on(int on){
	uint32_t reg0 = 0, reg1 = 0;

	reg0 = regRead32(0x18000000);
	reg1 = regRead32(0x18000004);
	if(on){
		reg0 |= BIT(22);
		reg1 |= BIT(21);
		regWrite32(0x18000000, reg0);
		regWrite32(0x18000004, reg1);
	}else{
		reg0 = reg0 & (~BIT(22));
		reg1 = reg1 & (~BIT(21));
		regWrite32(0x18000000, reg0);
		regWrite32(0x18000004, reg1);
	}
}

int32_t spi_burst_write_data_cpu(void *handle, ADDR *data, uint32_t len)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    int32_t i = 0;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    rtn = spi_cs_gpio(spiContext->gpio_addr, 0);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);

    for(i = 0; i < len; i++) {
        rtn = spi_set_dr(spiContext->spi_addr,data[i]);
    }

    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);
    rtn = spi_cs_gpio(spiContext->gpio_addr, 1);

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

int32_t spi_burst_write_data_dma(void *handle, ADDR phyaddr, uint32_t len)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    //spi_lcd_dma_transfer(phyaddr, len);

    rtn = spi_cs_gpio(spiContext->gpio_addr, 0);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);

    rtn = _spi_v0_dma_start(handle);

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    return rtn;
}

int32_t spi_burst_write_data(void *handle, ADDR *data, uint32_t len)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");
    CHECK_PTR(data, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    switch(spiContext->txMode) {
        case spi_trx_cpu:
            rtn = spi_burst_write_data_cpu(handle, data, len);
        break;
        case spi_trx_dma:
            rtn = spi_burst_write_data_dma(handle, (ADDR)data, len);
        break;
        default :
        break;
    }

    EXIT:

    return rtn;
}

int32_t spi_single_read_data(void *handle, ADDR *data)
{
    int32_t rtn = SUCCESS;
    //spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");
    CHECK_PTR(data, NULL, "handle is null ~_~\n");

    //spiContext = (spi_mpi_context_t *)handle;

    EXIT:

    return rtn;
}

int32_t spi_single_read(void *handle, uint8_t cmd)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;
    uint32_t val;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "start");

    rtn = spi_cs_gpio(spiContext->gpio_addr, 0);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 0);
    // send cmd
    rtn = spi_set_dr(spiContext->spi_addr, cmd);

    rtn = spi_data_dir(spiContext->sys_addr, spi_data_rx);
    rtn = spi_rs_gpio(spiContext->gpio_addr, 1);
    // send read data clk
    rtn = spi_set_dr(spiContext->spi_addr, cmd);
    rtn = spi_cs_gpio(spiContext->gpio_addr, 1);

    rtn = spi_data_dir(spiContext->sys_addr, spi_data_tx);

    val = regRead32(spiContext->spi_addr + SPI_DR);
    val = regRead32(spiContext->spi_addr + SPI_DR+4);
    val = regRead32(spiContext->spi_addr + SPI_DR+8);
    val = regRead32(spiContext->spi_addr + SPI_DR+0x0c);
    if (val) {
    }

    EXIT:

    debug_fun(spiContext->funDebug, __FUNCTION__, __LINE__, "end");

    return rtn;
}

int32_t spi_burst_read_data(void *handle, ADDR *data, uint32_t len)
{
    int32_t rtn = SUCCESS;
    spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");
    CHECK_PTR(data, NULL, "handle is null ~_~\n");

    spiContext = (spi_mpi_context_t *)handle;

    switch(spiContext->txMode) {
        case spi_trx_cpu:
        break;
        case spi_trx_dma:
        break;
        default :
        break;
    }

    EXIT:

    return rtn;
}

int32_t spi_push_table(void *handle, void *setting, uint32_t num)
{
    int32_t rtn = SUCCESS;
    //spi_mpi_context_t *spiContext = NULL;

    CHECK_PTR(handle, NULL, "handle is null ~_~\n");
    CHECK_PTR(setting, NULL, "setting is null ~_~\n");
    EXIT_IF_ZERO(num, NULL, "setting num is zero ~_~\n");
    //spiContext = (spi_mpi_context_t *)handle;

    EXIT:

    return rtn;
}


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
