#ifndef _RGB_MPI_PARAM_H_
#define _RGB_MPI_PARAM_H_

#include "data_type.h"
//#include "dsi_com_param.h"
#include "rgb_com_param.h"
#include "lcm_com_param.h"


#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef struct {
    ADDR rst_base_addr;
    ADDR clk_base_addr;
    ADDR vou_sel_addr;
    ADDR mif_base_addr;
    ADDR vou_base_addr;
    ADDR pin_base_addr;

    uint32_t lcd_id;
    uint32_t fps;
    lcm_display_format_e format;
    lcm_rgb_if_type_e if_type;

    rgb_clk_t clk;
    lcm_display_sync_info_t dp_sync_info;
}rgb_init_in_t;

typedef struct {
    uint32_t reserved;
}rgb_init_out_t;

typedef struct {
    uint32_t reserved;
}rgb_deinit_in_t;

typedef struct {
    uint32_t reserved;
}rgb_deinit_out_t;

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

