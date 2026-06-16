#ifndef _DSI_HOST_V01_DRV_H_
#define _DSI_HOST_V01_DRV_H_

#include "data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/

int32_t spi_set_dfs(uint32_t base_addr, uint32_t dfs);
int32_t spi_set_frf(uint32_t base_addr, uint32_t frf);
int32_t spi_set_scph(uint32_t base_addr, uint32_t scph);
int32_t spi_set_scpol(uint32_t base_addr, uint32_t scpol);
int32_t spi_set_tmod(uint32_t base_addr, uint32_t tmod);
int32_t spi_set_slv_oe(uint32_t base_addr, uint32_t slv_oe);
int32_t spi_set_srl(uint32_t base_addr, uint32_t srl);
int32_t spi_set_cfs(uint32_t base_addr, uint32_t cfs);
int32_t spi_set_dfs_32(uint32_t base_addr, uint32_t dfs_32);
int32_t spi_set_spi_frf(uint32_t base_addr, uint32_t spi_frf);
int32_t spi_set_ndf(uint32_t base_addr, uint32_t ndf);
int32_t spi_set_ssi_en(uint32_t base_addr, uint32_t ssi_en);
int32_t spi_set_mwmod(uint32_t base_addr, uint32_t mwmod);
int32_t spi_set_mdd(uint32_t base_addr, uint32_t mdd);
int32_t spi_set_mhs(uint32_t base_addr, uint32_t mhs);
int32_t spi_set_ser(uint32_t base_addr, uint32_t ser);
int32_t spi_set_sckdv(uint32_t base_addr, uint32_t sckdv);
int32_t spi_set_txeim(uint32_t base_addr, uint32_t txeim);
int32_t spi_set_txoim(uint32_t base_addr, uint32_t txoim);
int32_t spi_set_rxuim(uint32_t base_addr, uint32_t rxuim);
int32_t spi_set_rxoim(uint32_t base_addr, uint32_t rxoim);
int32_t spi_set_rxfim(uint32_t base_addr, uint32_t rxfim);
int32_t spi_set_mstim(uint32_t base_addr, uint32_t mstim);
int32_t spi_set_dma_tx_eb(uint32_t base_addr, uint32_t tdmae);
int32_t spi_set_dma_rx_eb(uint32_t base_addr, uint32_t rdmae);
int32_t spi_set_dma_tx_abw(uint32_t base_addr, uint32_t tx_abw);
int32_t spi_set_dma_rx_abw(uint32_t base_addr, uint32_t rx_abw);
int32_t spi_set_dr(uint32_t base_addr, uint32_t dr);
int32_t spi_set_rx_sample_dly(uint32_t base_addr, uint32_t rsd);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

