#ifndef _SPI_V01_REG_DEFINE_H_
#define _SPI_V01_REG_DEFINE_H_

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
#define SPI_BASE_ADDR

#define SPI_CTRLR0              (0x0000) //R/W
#define SPI_CTRLR1              (0x0004) //R/W
#define SPI_SSIENR              (0x0008) //R/W
#define SPI_MWCR                (0x000C) //R/W
#define SPI_SER                 (0x0010) //R/W
#define SPI_BAUDR               (0x0014) //R/W
#define SPI_TXFTLR              (0x0018) //R/W
#define SPI_RXFTLR              (0x001C) //R/W
#define SPI_TXFLR               (0x0020) //R/W
#define SPI_RXFLR               (0x0024) //R/W
#define SPI_SR                  (0x0028) //R/W
#define SPI_IMR                 (0x002C) //R/W
#define SPI_ISR                 (0x0030) //R/W
#define SPI_RISR                (0x0034) //R/W
#define SPI_TXOICR              (0x0038) //R/W
#define SPI_RXOICR              (0x003C) //R/W
#define SPI_RXOUICR             (0x0040) //R/W
#define SPI_MSTICR              (0x0044) //R/W
#define SPI_ICR                 (0x0048) //R/W
#define SPI_DMACR               (0x004C) //R/W
#define SPI_DMATDLR             (0x0050) //R/W
#define SPI_DMARDLR             (0x0054) //R/W
#define SPI_IDR                 (0x0058) //R/W
#define SPI_SSI_COMP_VERSION    (0x005C) //R/W
#define SPI_DR                  (0x0060) //R/W
#define SPI_RX_SAMPLE_DLY       (0x00F0) //R/W
#define SPI_SPI_CTRLR0          (0x00F4) //R/W
#define SPI_RSVD1               (0x00F8) //R/W
#define SPI_RSVD2               (0x00FC) //R/W


/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif