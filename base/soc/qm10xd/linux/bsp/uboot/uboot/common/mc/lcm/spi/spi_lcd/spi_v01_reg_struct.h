#ifndef _SPI_V01_REG_STRUCT_H_
#define _SPI_V01_REG_STRUCT_H_

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
union spi_crtlr0_tag {
    struct spi_crtlr0_map {
        volatile unsigned int dfs      :4;
        volatile unsigned int frf      :2;
        volatile unsigned int scph     :1;
        volatile unsigned int scpol    :1;
        volatile unsigned int tmod     :2;
        volatile unsigned int slv_oe   :1;
        volatile unsigned int srl      :1;
        volatile unsigned int cfs      :4;
        volatile unsigned int dfs_32   :5;
        volatile unsigned int spi_frf  :2;
        volatile unsigned int reserved :9;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_crtlr11_tag {
    struct spi_crtlr1_map {
        volatile unsigned int ndf      :16;
        volatile unsigned int reserved :16;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_ssienr_tag {
    struct spi_ssienr_map {
        volatile unsigned int ssi_en   :1;
        volatile unsigned int reserved :31;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_mwcr_tag {
    struct spi_mwcr_map {
        volatile unsigned int mwmod      :1;
        volatile unsigned int mdd        :1;
        volatile unsigned int mhs        :1;
        volatile unsigned int reserved   :29;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_ser_tag {
    struct spi_ser_map {
        volatile unsigned int ser      :2;
        volatile unsigned int reserved :30;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_baudr_tag {
    struct spi_baudr_map {
        volatile unsigned int sckdv    :16;
        volatile unsigned int reserved :16;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_imr_tag {
    struct spi_imr_map {
        volatile unsigned int txeim     :1;
        volatile unsigned int txoim     :1;
        volatile unsigned int rxuim     :1;
        volatile unsigned int rxoim     :1;
        volatile unsigned int rxfim     :1;
        volatile unsigned int mstim     :1;
        volatile unsigned int reserved  :26;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_dmacr_tag {
    struct spi_dmacr_map {
        volatile unsigned int tdmae     :1;
        volatile unsigned int rdmae     :1;
        volatile unsigned int reserved  :32;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_dmatdlr_tag {
    struct spi_dmatdlr_map {
        volatile unsigned int tx_abw      :2;
        volatile unsigned int reserved    :30;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_dmardlr_tag {
    struct spi_dmardlr_map {
        volatile unsigned int rx_abw      :2;
        volatile unsigned int reserved    :30;
    }mBits;
    volatile unsigned int dwValue;
};

union spi_rx_sample_dly_tag {
    struct spi_rx_sample_dly_map {
        volatile unsigned int rsd        :8;
        volatile unsigned int reserved   :24;
    }mBits;
    volatile unsigned int dwValue;
};

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
