#ifndef _SPI_V01_DRV_MPI_PARAM_H_
#define _SPI_V01_DRV_MPI_PARAM_H_

#include "data_type.h"
#include "mol_lcm_spi.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef enum {
    spi_cmd_bit_width,
    spi_cmd_tx_mode,
    spi_cmd_rx_mode,
    spi_cmd_debug_log,
    spi_cmd_max,
}spi_ioctrl_cmd_e;

typedef enum {
    spi_dsf_4bits = 3,
    spi_dsf_5bits = 4,
    spi_dsf_6bits = 5,
    spi_dsf_7bits = 6,
    spi_dsf_8bits = 7,
    spi_dsf_9bits = 8,
    spi_dsf_10bits = 9,
    spi_dsf_11bits = 10,
    spi_dsf_12bits = 11,
    spi_dsf_13bits = 12,
    spi_dsf_14bits = 13,
    spi_dsf_15bits = 14,
    spi_dsf_16bits = 15,
    spi_dsf_17bits = 16,
    spi_dsf_18bits = 17,
    spi_dsf_19bits = 18,
    spi_dsf_20bits = 19,
    spi_dsf_21bits = 20,
    spi_dsf_22bits = 21,
    spi_dsf_23bits = 22,
    spi_dsf_24bits = 23,
    spi_dsf_25bits = 24,
    spi_dsf_26bits = 25,
    spi_dsf_27bits = 26,
    spi_dsf_28bits = 27,
    spi_dsf_29bits = 28,
    spi_dsf_30bits = 29,
    spi_dsf_31bits = 30,
    spi_dsf_max,
}spi_dfs_mode_e;

typedef enum {
    spi_trx_cpu = 0,
    spi_trx_dma,
    spi_trx_mode_max,
}spi_trx_mode_e;

typedef enum {
    spi_fun_log = 0,
    spi_param_log,
    spi_register_log,
    spi_debug_log_max,
}spi_debug_log_e;

typedef enum {
    spi_lcd_rgb565 = 0,
    spi_lcd_rgb888 = 0,
    spi_lcd_format_max,
}spi_lcd_format_e;

typedef struct {
    uint32_t id;
    uint32_t pin_ctrl; // 0:auto mode 1:manual mode
    ADDR spi_addr;
    ADDR gpio_addr;
    ADDR sys_addr;
    ADDR clk_addr;
    ADDR dma_addr;

    lcm_spi_info info;

    uint32_t funDebug;
    uint32_t paramDebug;
    uint32_t regDebug;
}spi_init_in_t;

typedef struct {
    void *handle;
    uint32_t reserved;
}spi_init_out_t;

typedef struct {
    void *handle;
    uint32_t reserved;
}spi_deinit_in_t;

typedef struct {
    uint32_t reserved;
}spi_deinit_out_t;

typedef struct {
    void *handle;
    spi_ioctrl_cmd_e cmd;
    void *param;
}spi_io_ctrl_in_t;

typedef struct {
    void *param;
}spi_io_ctrl_out_t;

typedef struct {
    void *handle;
    void *param;
    uint32_t num;
}spi_push_table_in_t;

typedef struct {
    void *param;
}spi_push_table_out_t;

typedef struct {
    void *handle;
    spi_dfs_mode_e bitWidth;
}spi_bit_width_in_t;

typedef struct {
    uint32_t reserved;
}spi_bit_width_out_t;

typedef struct {
    void *handle;
    spi_trx_mode_e mode;
}spi_tx_mode_in_t;

typedef struct {
    uint32_t reserved;
}spi_tx_mode_out_t;

typedef struct {
    void *handle;
    spi_trx_mode_e mode;
}spi_rx_mode_in_t;

typedef struct {
    uint32_t reserved;
}spi_rx_mode_out_t;

typedef struct {
    void *handle;
    spi_debug_log_e mode;
    uint32_t eb;
}spi_debug_log_in_t;

typedef struct {
    uint32_t reserved;
}spi_debug_log_out_t;


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

