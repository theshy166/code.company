#include "mol_lcm.h"
#include "mol_lcm_spi.h"
#include "mol_gpio_spi.h"
#include "mol_lcm_spi_param.h"

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
#define MOL_LCM_SPI_FUN_DEBUG 0

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef enum
{
    SPI_UN_INITED = 0,
    SPI_INITED,
    //ON,
    //OFF
}spi_state_e;

typedef struct {
    int32_t status;
    int32_t type; // 0:hw 1:gpio
}mol_lcm_spi_t;

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
static mol_lcm_spi_t s_mol_lcm_spi = {
    .status = -1,
    .type = -1,
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
static mol_lcm_spi_t *_get_lcm_spi_context(void)
{
    return &s_mol_lcm_spi;
}

int32_t write_cmd(serial_bus_info *info, uint32_t cmd)
{
    int32_t rtn = SUCCESS;
    mol_lcm_spi_t *spi = _get_lcm_spi_context();

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    if(SPI_INITED != spi->status) {
        rtn = FAIL_UN_INIT;
        EXIT_IF_FAIL(rtn, NULL, "lcm spi uninit ~_~!\n");
    }

    if (1 == spi->type) {
        rtn = gpio_spi_write_cmd(info, cmd);
        EXIT_IF_FAIL(rtn, NULL, "gpio_spi_write_cmd ~_~!\n");
    } else if (0 == spi->type) {
        EXIT_IF_FAIL(rtn, NULL, "~_~!\n");
    }

EXIT:

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d------------end---------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t write_data(serial_bus_info *info, uint32_t data)
{
    int32_t rtn = SUCCESS;
    mol_lcm_spi_t *spi = _get_lcm_spi_context();

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    if(SPI_INITED != spi->status) {
        rtn = FAIL_UN_INIT;
        EXIT_IF_FAIL(rtn, NULL, "lcm spi uninit ~_~!\n");
    }

    if (1 == spi->type) {
        rtn = gpio_spi_write_data(info, data);
        EXIT_IF_FAIL(rtn, NULL, "gpio_spi_write_data ~_~!\n");
    } else if (0 == spi->type) {
        EXIT_IF_FAIL(rtn, NULL, "~_~!\n");
    }

EXIT:

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d------------end---------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}


int32_t read_data(serial_bus_info *info, uint32_t cmd, uint8_t *data, uint32_t rd_num)
{
    int32_t rtn = SUCCESS;
    mol_lcm_spi_t *spi = _get_lcm_spi_context();

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    if(SPI_INITED != spi->status) {
        rtn = FAIL_UN_INIT;
        EXIT_IF_FAIL(rtn, NULL, "lcm spi uninit ~_~!\n");
    }

    if (1 == spi->type) {
        rtn = gpio_spi_read(info, cmd, data, rd_num);
        EXIT_IF_FAIL(rtn, NULL, "gpio_spi_write_data ~_~!\n");
    } else if (0 == spi->type) {
        EXIT_IF_FAIL(rtn, NULL, "~_~!\n");
    }

EXIT:

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d------------end---------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t write_burst_cmd(serial_bus_info *info, uint8_t *cmd, uint32_t num)
{
    int32_t rtn = 0;

    return rtn;
}

int32_t write_burst_data(serial_bus_info *info, uint8_t *data, uint32_t num)
{
    int32_t rtn = 0;

    return rtn;
}

int32_t read_burst(serial_bus_info *info, uint32_t cmd, uint8_t* buf, uint32_t rd_num)
{
    int32_t rtn = 0;

    return rtn;
}

/*
void *inParam : serial_bus_info
void *outParam: reserved
*/
int32_t lcm_spi_init(lcm_spi_init_in_t *inParam, lcm_spi_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    mol_lcm_spi_t *spi = _get_lcm_spi_context();

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "lcm_spi_init is null ~_~\n");

    rtn = gpio_spi_init(inParam);

    if (SUCCESS == rtn) {
        spi->type = 1;
    } else {

        EXIT_IF_FAIL(rtn, NULL, "lcm_spi_init ~_~!\n");
        spi->type = 0;
    }

    spi->status = SPI_INITED;

EXIT:

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return 0;
}

/*
void *inParam : serial_bus_info
void *outParam: reserved
*/
int32_t lcm_spi_deInit(lcm_spi_deinit_in_t *inParam, lcm_spi_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    mol_lcm_spi_t *spi = _get_lcm_spi_context();

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    if (1 == spi->type) {
        rtn = gpio_spi_deinit(inParam);
        EXIT_IF_FAIL(rtn, NULL, "gpio_spi_deinit ~_~!\n");
    } else if (0 == spi->type) {

        EXIT_IF_FAIL(rtn, NULL, "lcm_spi_deInit ~_~!\n");
    }

EXIT:

    spi->status = -1;
    spi->type = -1;

    #if MOL_LCM_SPI_FUN_DEBUG
    printk("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return 0;
}
