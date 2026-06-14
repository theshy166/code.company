
#include "data_type.h"
#include "spi_v01_drv_mpi_param.h"
#include "spi_v01_drv_mpi.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef     __cplusplus
extern     "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
void *lcmSpiApiInit(spi_init_in_t *inParam, spi_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(inParam, NULL, "spi_init_in_t ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_init_out_t ~_~\n");

    rtn = spi_v0_init(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    EXIT:

    return (void *)outParam->handle;
}

int32_t lcmSpiApiDeInit(spi_deinit_in_t *inParam, spi_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(inParam, NULL, "spi_init_in_t ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_init_out_t ~_~\n");

    rtn = spi_v0_deinit(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "lcm mcu api handle is not exit ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiWriteCmd(void *handle, uint32_t cmd)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(handle, NULL, "handle ~_~\n");

    rtn = spi_single_write_cmd(handle, cmd);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_write_cmd ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiWriteData(void *handle, uint32_t data)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(handle, NULL, "handle ~_~\n");

    rtn = spi_single_write_data(handle, data);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_write_data ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiWriteBurstData(void *handle, uint32_t *data, uint32_t num)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(handle, NULL, "handle ~_~\n");
    CHECK_PTR(data, NULL, "data ~_~\n");
    EXIT_IF_ZERO(num, NULL, "num zero ~_~\n");

    rtn = spi_burst_write_data(handle, (ADDR *)data, num);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_write_data ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiReadData(void *handle, uint32_t *data)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(handle, NULL, "handle ~_~\n");
    CHECK_PTR(data, NULL, "data ~_~\n");

    rtn = spi_single_read_data(handle, (ADDR *)data);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_read_data ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiReadBurstData(void *handle, uint32_t *data, uint32_t num)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(handle, NULL, "handle ~_~\n");
    CHECK_PTR(data, NULL, "data ~_~\n");
    EXIT_IF_ZERO(num, NULL, "num zero ~_~\n");

    rtn = spi_burst_read_data(handle, (ADDR *)data, num);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_read_data ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiIoCtrl(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(inParam, NULL, "spi_io_ctrl_in_t ~_~\n");
    CHECK_PTR(inParam, NULL, "spi_io_ctrl_out_t ~_~\n");

    rtn = spi_ioCtrl((spi_io_ctrl_in_t *)inParam, (spi_io_ctrl_out_t *)outParam);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_read_data ~_~!");

    EXIT:

    return rtn;
}

int32_t lcmSpiApiPushTable(void *inParam, void *outParam)
{
    int32_t rtn = SUCCESS;
    spi_push_table_in_t *pushParam = NULL;

    CHECK_PTR(inParam, NULL, "spi_push_table_in_t ~_~\n");
    CHECK_PTR(outParam, NULL, "spi_push_table_out_t ~_~\n");
    pushParam = (spi_push_table_in_t *)inParam;
    CHECK_PTR(pushParam->handle, NULL, "handle ~_~\n");

    rtn = spi_push_table(pushParam->handle, pushParam->param, pushParam->num);
    EXIT_IF_FAIL(rtn, NULL, "lcm spi_single_read_data ~_~!");

    EXIT:

    return rtn;
}

/**----------------------------------------------------------------------------*
*           Compiler Flag                       *
**----------------------------------------------------------------------------*/
#ifdef     __cplusplus
 }
#endif
/**---------------------------------------------------------------------------*/

