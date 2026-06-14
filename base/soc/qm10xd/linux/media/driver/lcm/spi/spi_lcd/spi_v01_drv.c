#include "reg.h"
#include "reg_pool.h"
#include "spi_v01_reg_def.h"
#include "spi_v01_reg_pool.h"
#include "spi_v01_reg_struct.h"
#include "spi_v01_drv_param.h"

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

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
int32_t spi_set_dfs(uint64_t base_addr, uint32_t dfs)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.dfs = dfs;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_frf(uint64_t base_addr, uint32_t frf)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.frf = frf;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_scph(uint64_t base_addr, uint32_t scph)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.frf = scph;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_scpol(uint64_t base_addr, uint32_t scpol)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.scpol = scpol;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_tmod(uint64_t base_addr, uint32_t tmod)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.tmod = tmod;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_slv_oe(uint64_t base_addr, uint32_t slv_oe)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.slv_oe = slv_oe;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_srl(uint64_t base_addr, uint32_t srl)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.srl = srl;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_cfs(uint64_t base_addr, uint32_t cfs)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.cfs = cfs;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_dfs_32(uint64_t base_addr, uint32_t dfs_32)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.dfs_32 = dfs_32;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_spi_frf(uint64_t base_addr, uint32_t spi_frf)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr0_tag *reg = getRegValuePtr(SPI_CTRLR0, spi_get_reg_default_tab());

    reg->mBits.spi_frf = spi_frf;
    regWrite32(base_addr + SPI_CTRLR0, reg->dwValue);

    return rtn;
}

int32_t spi_set_ndf(uint64_t base_addr, uint32_t ndf)
{
    int32_t rtn = SUCCESS;
    union spi_crtlr11_tag *reg = getRegValuePtr(SPI_CTRLR1, spi_get_reg_default_tab());

    reg->mBits.ndf = ndf;
    regWrite32(base_addr + SPI_CTRLR1, reg->dwValue);

    return rtn;
}

int32_t spi_set_ssi_en(uint64_t base_addr, uint32_t ssi_en)
{
    int32_t rtn = SUCCESS;
    union spi_ssienr_tag *reg = getRegValuePtr(SPI_SSIENR, spi_get_reg_default_tab());

    reg->mBits.ssi_en = ssi_en;
    regWrite32(base_addr + SPI_SSIENR, reg->dwValue);

    return rtn;
}

int32_t spi_set_mwmod(uint64_t base_addr, uint32_t mwmod)
{
    int32_t rtn = SUCCESS;
    union spi_mwcr_tag *reg = getRegValuePtr(SPI_MWCR, spi_get_reg_default_tab());

    reg->mBits.mwmod = mwmod;
    regWrite32(base_addr + SPI_MWCR, reg->dwValue);

    return rtn;
}

int32_t spi_set_mdd(uint64_t base_addr, uint32_t mdd)
{
    int32_t rtn = SUCCESS;
    union spi_mwcr_tag *reg = getRegValuePtr(SPI_MWCR, spi_get_reg_default_tab());

    reg->mBits.mdd = mdd;
    regWrite32(base_addr + SPI_MWCR, reg->dwValue);

    return rtn;
}

int32_t spi_set_mhs(uint64_t base_addr, uint32_t mhs)
{
    int32_t rtn = SUCCESS;
    union spi_mwcr_tag *reg = getRegValuePtr(SPI_MWCR, spi_get_reg_default_tab());

    reg->mBits.mhs = mhs;
    regWrite32(base_addr + SPI_MWCR, reg->dwValue);

    return rtn;
}

int32_t spi_set_ser(uint64_t base_addr, uint32_t ser)
{
    int32_t rtn = SUCCESS;
    union spi_ser_tag *reg = getRegValuePtr(SPI_SER, spi_get_reg_default_tab());

    reg->mBits.ser = ser;
    regWrite32(base_addr + SPI_SER, reg->dwValue);

    return rtn;
}

int32_t spi_set_sckdv(uint64_t base_addr, uint32_t sckdv)
{
    int32_t rtn = SUCCESS;
    union spi_baudr_tag *reg = getRegValuePtr(SPI_BAUDR, spi_get_reg_default_tab());

    reg->mBits.sckdv = sckdv;
    regWrite32(base_addr + SPI_BAUDR, reg->dwValue);

    return rtn;
}

int32_t spi_set_txeim(uint64_t base_addr, uint32_t txeim)
{
    int32_t rtn = SUCCESS;
    union spi_imr_tag *reg = getRegValuePtr(SPI_IMR, spi_get_reg_default_tab());

    reg->mBits.txeim = txeim;
    regWrite32(base_addr + SPI_IMR, reg->dwValue);

    return rtn;
}

int32_t spi_set_txoim(uint64_t base_addr, uint32_t txoim)
{
    int32_t rtn = SUCCESS;
    union spi_imr_tag *reg = getRegValuePtr(SPI_IMR, spi_get_reg_default_tab());

    reg->mBits.txoim = txoim;
    regWrite32(base_addr + SPI_IMR, reg->dwValue);

    return rtn;
}

int32_t spi_set_rxuim(uint64_t base_addr, uint32_t rxuim)
{
    int32_t rtn = SUCCESS;
    union spi_imr_tag *reg = getRegValuePtr(SPI_IMR, spi_get_reg_default_tab());

    reg->mBits.rxuim = rxuim;
    regWrite32(base_addr + SPI_IMR, reg->dwValue);

    return rtn;
}

int32_t spi_set_rxoim(uint64_t base_addr, uint32_t rxoim)
{
    int32_t rtn = SUCCESS;
    union spi_imr_tag *reg = getRegValuePtr(SPI_IMR, spi_get_reg_default_tab());

    reg->mBits.rxoim = rxoim;
    regWrite32(base_addr + SPI_IMR, reg->dwValue);

    return rtn;
}

int32_t spi_set_rxfim(uint64_t base_addr, uint32_t rxfim)
{
    int32_t rtn = SUCCESS;
    union spi_imr_tag *reg = getRegValuePtr(SPI_IMR, spi_get_reg_default_tab());

    reg->mBits.rxfim = rxfim;
    regWrite32(base_addr + SPI_IMR, reg->dwValue);

    return rtn;
}

int32_t spi_set_mstim(uint64_t base_addr, uint32_t mstim)
{
    int32_t rtn = SUCCESS;
    union spi_imr_tag *reg = getRegValuePtr(SPI_IMR, spi_get_reg_default_tab());

    reg->mBits.mstim = mstim;
    regWrite32(base_addr + SPI_IMR, reg->dwValue);

    return rtn;
}

int32_t spi_set_dma_tx_eb(uint64_t base_addr, uint32_t tdmae)
{
    int32_t rtn = SUCCESS;
    union spi_dmacr_tag *reg = getRegValuePtr(SPI_DMACR, spi_get_reg_default_tab());

    reg->mBits.tdmae = tdmae;
    regWrite32(base_addr + SPI_DMACR, reg->dwValue);

    return rtn;
}

int32_t spi_set_dma_rx_eb(uint64_t base_addr, uint32_t rdmae)
{
    int32_t rtn = SUCCESS;
    union spi_dmacr_tag *reg = getRegValuePtr(SPI_DMACR, spi_get_reg_default_tab());

    reg->mBits.rdmae = rdmae;
    regWrite32(base_addr + SPI_DMACR, reg->dwValue);

    return rtn;
}

int32_t spi_set_dma_tx_abw(uint64_t base_addr, uint32_t tx_abw)
{
    int32_t rtn = SUCCESS;
    union spi_dmatdlr_tag *reg = getRegValuePtr(SPI_DMATDLR, spi_get_reg_default_tab());

    reg->mBits.tx_abw = tx_abw;
    regWrite32(base_addr + SPI_DMATDLR, reg->dwValue);

    return rtn;
}

int32_t spi_set_dma_rx_abw(uint64_t base_addr, uint32_t rx_abw)
{
    int32_t rtn = SUCCESS;
    union spi_dmardlr_tag *reg = getRegValuePtr(SPI_DMARDLR, spi_get_reg_default_tab());

    reg->mBits.rx_abw = rx_abw;
    regWrite32(base_addr + SPI_DMARDLR, reg->dwValue);

    return rtn;
}

int32_t spi_set_dr(uint64_t base_addr, uint32_t dr)
{// data to fifo
    int32_t rtn = SUCCESS;
    regWrite32(base_addr + SPI_DR, dr);

    return rtn;
}

int32_t spi_set_rx_sample_dly(uint64_t base_addr, uint32_t rsd)
{
    int32_t rtn = SUCCESS;
    union spi_rx_sample_dly_tag *reg = getRegValuePtr(SPI_RX_SAMPLE_DLY, spi_get_reg_default_tab());

    reg->mBits.rsd = rsd;
    regWrite32(base_addr + SPI_RX_SAMPLE_DLY, reg->dwValue);

    return rtn;
}


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
