#include "../../lcm_module/include/mol_lcm_local.h"
#include "../../lcm_module/include/dsi_com_param.h"
#include "dsi_mpi_param.h"
#include "dsi_phy_v01_drv_mpi.h"
#include "dsi_host_v01_drv_mpi.h"
#include "../clk/vou_clk_v01_drv.h"
#include "../pin_mux/pin_lcd_v01_drv.h"
#include "../lcm_com.h"
#include "../reg/reg.h"
#include "dsi_com.h"
#include "dsi_phy_v0_pll_calc_param.h"
#include "dsi_phy_v0_pll_calc.h"
#include "../os.h"
#include "../debug.h"

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
#define DSI_MPI_DEBUG 0
#define DSI_MPI_FUN_DEBUG 0
#define DSI_MIPI_PARAM_DEBUG 0
/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct {
    dsi_status_e status;

    uint32_t host_sync_div;
    uint32_t phy_txreadyesc;
    dsi_phy_work_mode_e phyMode;
    lcm_pin_type_e lcm_pin_type;
    ADDR rst_base_addr;
    ADDR clk_base_addr;
    ADDR vou_base_addr;
    ADDR host_base_addr;
    ADDR phy_base_addr;
    ADDR pin_base_addr;

    vou_clk_calc_in_t vouClkIn;
    vou_clk_calc_out_t vouClkOut;

    dsi_phy_pll_calc_in_t mifClkIn;
    dsi_phy_pll_calc_out_t mifClkOut;

    dsi_init_in_t dsiInitIn;
    dsi_init_out_t dsiInitOut;

    dsi_deinit_in_t dsiDeInitIn;
    dsi_deinit_out_t dsiDeInitOut;

    dsi_phy_init_in_t phyInitIn;
    dsi_phy_init_out_t phyInitOut;
    dsi_phy_deinit_in_t phyDeinitIn;
    dsi_phy_deinit_out_t phyDeinitOut;

    dsi_phy_power_on_in_t phyPowerOnIn;
    dsi_phy_power_on_out_t phyPowerOnOut;
    dsi_phy_power_off_in_t phyPowerOffIn;
    dsi_phy_power_off_out_t phyPowerOffOut;

    lcm_clk_on_in_t lcmClkOnIn;
    lcm_clk_on_out_t lcmClkOnOut;
    lcm_clk_off_in_t lcmClkOffIn;
    lcm_clk_off_out_t lcmClkOffOut;

    dsi_host_init_in_t hostInitIn;
    dsi_host_init_out_t hostInitOut;
    dsi_host_deinit_in_t hostDeinitIn;
    dsi_host_deinit_out_t hostDeinitOut;

    dsi_host_power_on_in_t hostPowerOnIn;
    dsi_host_power_on_out_t hostPowerOnOut;
    dsi_host_power_off_in_t hostPowerOffIn;
    dsi_host_power_off_out_t hostPowerOffOut;
}dsi_mpi_context_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
dsi_mpi_context_t s_dsi_mpi_context[2] = {0x00};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
dsi_mpi_context_t *dsi_mipi_context_get(    host_core_t hostId)
{
    return &s_dsi_mpi_context[hostId];
}

static int32_t s_dsi_mipi_err_code = 0;
void dsi_mipi_set_err_code(uint32_t hostId)
{
    s_dsi_mipi_err_code |= 1<<hostId;
    printf("s_dsi_mipi_err_code: 0x%x\n", s_dsi_mipi_err_code);
}

uint32_t dsi_mipi_get_err_code(void)
{
    return s_dsi_mipi_err_code;
}

#if DSI_MIPI_PARAM_DEBUG
static int32_t _dsi_mipi_param_show(    uint32_t hostId)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(hostId);
    dsi_init_in_t *dsiInitIn = NULL;

    dsiInitIn = &dsiContext->dsiInitIn;

    printf("\n-------------dsi mipi start-------------------\n");

    printf("hostId                     : 0x%x\n", hostId);
    printf("dsiInitIn->rst_base_addr   : 0x%x\n", dsiInitIn->rst_base_addr);
    printf("dsiInitIn->clk_base_addr   : 0x%x\n", dsiInitIn->clk_base_addr);
    printf("dsiInitIn->vou_base_addr   : 0x%x\n", dsiInitIn->vou_base_addr);
    printf("dsiInitIn->host_base_addr  : 0x%x\n", dsiInitIn->host_base_addr);
    printf("dsiInitIn->phy_base_addr   : 0x%x\n", dsiInitIn->phy_base_addr );
    printf("dsiInitIn->pin_base_addr   : 0x%x\n", dsiInitIn->pin_base_addr );
    printf("dsiInitIn->fps             : %d\n",   dsiInitIn->fps             );
    printf("dsiInitIn->format          : %d\n",   dsiInitIn->format          );
    printf("dsiInitIn->laneNum         : %d\n",   dsiInitIn->laneNum         );

    printf("dsiInitIn->timing.clane.mode   : %d\n", dsiInitIn->timing.clane.mode   );
    printf("dsiInitIn->timing.clane.lp11   : %d\n", dsiInitIn->timing.clane.lp11   );
    printf("dsiInitIn->timing.clane.lp01   : %d\n", dsiInitIn->timing.clane.lp01   );
    printf("dsiInitIn->timing.clane.zero   : %d\n", dsiInitIn->timing.clane.zero   );
    printf("dsiInitIn->timing.clane.prepare: %d\n", dsiInitIn->timing.clane.prepare);
    printf("dsiInitIn->timing.clane.pre    : %d\n", dsiInitIn->timing.clane.pre    );
    printf("dsiInitIn->timing.clane.trail  : %d\n", dsiInitIn->timing.clane.trail  );
    printf("dsiInitIn->timing.clane.exit   : %d\n", dsiInitIn->timing.clane.exit   );
    printf("dsiInitIn->timing.clane.post   : %d\n", dsiInitIn->timing.clane.post   );
    printf("dsiInitIn->timing.dlane.mode   : %d\n", dsiInitIn->timing.dlane.mode   );
    printf("dsiInitIn->timing.dlane.lp11   : %d\n", dsiInitIn->timing.dlane.lp11   );
    printf("dsiInitIn->timing.dlane.lp01   : %d\n", dsiInitIn->timing.dlane.lp01   );
    printf("dsiInitIn->timing.dlane.zero   : %d\n", dsiInitIn->timing.dlane.zero   );
    printf("dsiInitIn->timing.dlane.prepare: %d\n", dsiInitIn->timing.dlane.prepare);
    printf("dsiInitIn->timing.dlane.trail  : %d\n", dsiInitIn->timing.dlane.trail  );
    printf("dsiInitIn->timing.dlane.exit   : %d\n", dsiInitIn->timing.dlane.exit   );
    printf("dsiInitIn->timing.read.mode    : %d\n", dsiInitIn->timing.read.mode    );
    printf("dsiInitIn->timing.read.taget   : %d\n", dsiInitIn->timing.read.taget   );
    printf("dsiInitIn->timing.read.tasure  : %d\n", dsiInitIn->timing.read.tasure  );
    printf("dsiInitIn->timing.read.tago    : %d\n", dsiInitIn->timing.read.tago    );


    printf("dsiInitIn->dp_sync_info.hor_sync_active : %d\n", dsiInitIn->dp_sync_info.hor_sync_active );
    printf("dsiInitIn->dp_sync_info.hor_back_porch  : %d\n", dsiInitIn->dp_sync_info.hor_back_porch  );
    printf("dsiInitIn->dp_sync_info.hor_active_pixel: %d\n", dsiInitIn->dp_sync_info.hor_active_pixel);
    printf("dsiInitIn->dp_sync_info.hor_front_porch : %d\n", dsiInitIn->dp_sync_info.hor_front_porch );
    printf("dsiInitIn->dp_sync_info.ver_sync_active : %d\n", dsiInitIn->dp_sync_info.ver_sync_active );
    printf("dsiInitIn->dp_sync_info.ver_back_porch  : %d\n", dsiInitIn->dp_sync_info.ver_back_porch  );
    printf("dsiInitIn->dp_sync_info.ver_active_line : %d\n", dsiInitIn->dp_sync_info.ver_active_line );
    printf("dsiInitIn->dp_sync_info.ver_front_porch : %d\n", dsiInitIn->dp_sync_info.ver_front_porch );

    printf("dsiContext->lcm_pin_type   : %d\n",   dsiContext->lcm_pin_type   );
    printf("dsiContext->mifClkOut.dhd_div           : %d\n", dsiContext->mifClkOut.dhd_div);
    printf("vou->pll_sel                            : %d\n", dsiContext->vouClkOut.cfg.sel);
    printf("vou->pll_div                            : %d\n", dsiContext->vouClkOut.cfg.div);

    printf("-------------dsi mipi end---------------------\n");

    return rtn;
}

#endif

static lcm_pin_type_e _get_mipi_lcm_pin_type(dsi_phy_work_mode_e phy_work_mode)
{
    lcm_pin_type_e lcm_pin_type = 0;
    switch (phy_work_mode)
    {
        case DSI_PHY0_WORK_1C2D:
            lcm_pin_type = lcm_pin_mipi0_1c2d;
        break;
        case DSI_PHY1_WORK_1C2D:
            lcm_pin_type = lcm_pin_mipi1_1c2d;
        break;
        case DSI_PHY0_WORK_1C4D:
            lcm_pin_type = lcm_pin_mipi_1c4d;
        break;
        default:
            lcm_pin_type = 0;
        break;
    }
    return lcm_pin_type;
}

static int32_t _dsi_init_param_cfg(dsi_init_in_t *inParam)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(inParam->hostId);
    dsi_init_in_t *dsiInitIn = NULL;
    uint32_t frame_total_w = 0x00;
    uint32_t frame_total_h = 0x00;

    memcpy((void *)&dsiContext->dsiInitIn, (void *)inParam, sizeof(dsi_init_in_t));

    dsiInitIn = &dsiContext->dsiInitIn;

    dsiContext->rst_base_addr = dsiInitIn->rst_base_addr;
    dsiContext->clk_base_addr = dsiInitIn->clk_base_addr;
    dsiContext->vou_base_addr = dsiInitIn->vou_base_addr;
    dsiContext->host_base_addr = dsiInitIn->host_base_addr;
    dsiContext->phy_base_addr = dsiInitIn->phy_base_addr;
    dsiContext->pin_base_addr = dsiInitIn->pin_base_addr;

    dsiContext->lcm_pin_type =_get_mipi_lcm_pin_type(get_phy_work_mode(dsiInitIn->hostId, dsiInitIn->laneNum));

    dsiContext->vouClkIn.clkInfo.mode = dsiInitIn->timing.clk.vou.mode;
    dsiContext->vouClkIn.clkInfo.cfg.sel = dsiInitIn->timing.clk.vou.sel;
    dsiContext->vouClkIn.clkInfo.cfg.div = dsiInitIn->timing.clk.vou.div;

    frame_total_w = dsiInitIn->dp_sync_info.hor_sync_active
                    + dsiInitIn->dp_sync_info.hor_back_porch
                    + dsiInitIn->dp_sync_info.hor_active_pixel
                    + dsiInitIn->dp_sync_info.hor_front_porch;

    frame_total_h = dsiInitIn->dp_sync_info.ver_sync_active
                    + dsiInitIn->dp_sync_info.ver_back_porch
                    + dsiInitIn->dp_sync_info.ver_active_line
                    + dsiInitIn->dp_sync_info.ver_front_porch;

    dsiContext->vouClkIn.total_pixel = frame_total_w * frame_total_h * dsiInitIn->fps;
    dsiContext->vouClkIn.tx_frq_pixel = 1;

    rtn = vou_clk_calc(&dsiContext->vouClkIn, &dsiContext->vouClkOut);
    EXIT_IF_FAIL(rtn, NULL, "vou_clk_calc ~_~!\n");

    dsiContext->lcmClkOnIn.clk_base_addr = dsiInitIn->clk_base_addr;
    dsiContext->lcmClkOnIn.rst_base_addr = dsiInitIn->rst_base_addr;
    dsiContext->lcmClkOnIn.clk_sel = dsiContext->vouClkOut.cfg.sel;
    dsiContext->lcmClkOnIn.clk_div = dsiContext->vouClkOut.cfg.div;

    dsiContext->lcmClkOffIn.clk_base_addr = dsiInitIn->clk_base_addr;
    dsiContext->lcmClkOffIn.rst_base_addr = dsiInitIn->rst_base_addr;

    dsiContext->hostPowerOnIn.hostId = dsiInitIn->hostId;
    /*host power on param cfg*/
    dsiContext->hostPowerOnIn.clk_addr = dsiInitIn->clk_base_addr;
    dsiContext->hostPowerOnIn.rst_addr = dsiInitIn->rst_base_addr;
    dsiContext->hostPowerOnIn.host_addr = dsiInitIn->host_base_addr;
    dsiContext->hostPowerOnIn.phy_addr = dsiInitIn->phy_base_addr;

    /*phy power on param cfg*/
    dsiContext->phyPowerOnIn.phyId = dsiInitIn->hostId;
    dsiContext->phyPowerOnIn.clk_base_addr = dsiInitIn->clk_base_addr;
    dsiContext->phyPowerOnIn.rst_base_addr = dsiInitIn->rst_base_addr;
    dsiContext->phyPowerOnIn.phy_base_addr = dsiInitIn->phy_base_addr;

    /*phy in param cfg*/
    dsiContext->phyInitIn.phyId = dsiInitIn->hostId;
    dsiContext->phyInitIn.phy_addr = dsiInitIn->phy_base_addr;
    dsiContext->phyInitIn.vou_addr = dsiInitIn->vou_base_addr;
    dsiContext->phyInitIn.fps = dsiInitIn->fps;
    dsiContext->phyInitIn.format = dsiInitIn->format;
    dsiContext->phyInitIn.laneNum = dsiInitIn->laneNum;
    dsiContext->phyInitIn.dp_sync_info = &dsiInitIn->dp_sync_info;
    dsiContext->phyInitIn.timing = &dsiInitIn->timing;
    dsiContext->phyInitOut.host_sync_div = &dsiContext->host_sync_div;
    dsiContext->phyInitOut.phy_txreadyesc = &dsiContext->phy_txreadyesc;
    dsiContext->phyInitOut.phyMode = &dsiContext->phyMode;

    /*host in param cfg*/
    dsiContext->hostInitIn.hostId = dsiInitIn->hostId;
    dsiContext->hostInitIn.base_addr = dsiInitIn->host_base_addr;
    dsiContext->hostInitIn.format = dsiInitIn->format;
    dsiContext->hostInitIn.laneNum = dsiInitIn->laneNum;
    dsiContext->hostInitIn.host_sync_div = &dsiContext->host_sync_div;
    dsiContext->hostInitIn.phy_txreadyesc = &dsiContext->phy_txreadyesc;
    dsiContext->hostInitIn.dp_sync_info = &dsiInitIn->dp_sync_info;

    #if DSI_MIPI_PARAM_DEBUG
    _dsi_mipi_param_show(inParam->hostId);
    #endif

    EXIT:

    return rtn;
}

static int32_t _dsi_deinit_param_cfg(dsi_deinit_in_t *inParam)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(inParam->hostId);
    dsi_deinit_in_t *dsiDeInitIn = NULL;

    memcpy((void *)&dsiContext->dsiDeInitIn, (void *)inParam, sizeof(dsi_deinit_in_t));

    dsiDeInitIn = &dsiContext->dsiDeInitIn;

    /*phy in param cfg*/
    dsiContext->phyDeinitIn.phyId = dsiDeInitIn->hostId;
    dsiContext->phyPowerOffIn.phyId = dsiDeInitIn->hostId;

    /*host in param cfg*/
    dsiContext->hostDeinitIn.hostId = dsiDeInitIn->hostId;
    dsiContext->hostPowerOffIn.hostId = dsiDeInitIn->hostId;

    return rtn;
}

int32_t dsi_generic_write(host_core_t hostId, uint8_t *param, uint16_t param_length)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    rtn = dsi_gen_wr_cmd(hostId, param, param_length);
    if(RTN_DSI_SUCCESS != rtn){
        printf("mipiDsi: [%s] error (%d)\n", __FUNCTION__, rtn);
        return -1;
    }

    return rtn;
}

int32_t dsi_generic_read(host_core_t hostId, uint8_t *param, uint16_t param_length, uint8_t bytes_to_read, uint8_t *read_buffer)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    rtn = dsi_gen_rd_cmd(hostId, param, param_length, bytes_to_read, read_buffer);
    if(RTN_DSI_SUCCESS != rtn){
        printf("mipiDsi: [%s] error (%d)\n", __FUNCTION__, rtn);
        return -1;
    }

    return rtn;
}

int32_t dsi_dcs_write(host_core_t hostId, uint8_t *param, uint16_t param_length)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    rtn = dsi_dcs_wr_cmd(hostId, param, param_length);
    if(RTN_DSI_SUCCESS != rtn){
        printf("mipiDsi: [%s] error (%d)\n", __FUNCTION__, rtn);
        return -1;
    }

    return 0;
}

int32_t dsi_dcs_read(host_core_t hostId, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t *read_buffer)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    rtn = dsi_dcs_rd_cmd(hostId, data_type, command, bytes_to_read, read_buffer);

    if(RTN_DSI_SUCCESS != rtn){
        printf("mipiDsi: [%s] error (%d)\n", __FUNCTION__, rtn);
        return -1;
    }

    return rtn;
}

int32_t dsi_push_table(host_core_t hostId, dsi_setting_tab_t *table, uint32_t count)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    uint32_t i = 0;
    int32_t err = 0;

    #if DSI_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------, hostId:%d, table_len:%d\n", __FUNCTION__, __LINE__, hostId, count);
    #endif

    for(i = 0; i < count; i++) {

        uint32_t di = table[i].di;

        switch (di) {
            case DSI_DI_DELAY :
            delayMs(table[i].count);
            break;

            case DSI_DI_END_OF_TABLE :
            break;

            case DSI_DI_GENERIC_WRITE_NONE :
            case DSI_DI_GENERIC_WRITE_0_PARAM :
            case DSI_DI_GENERIC_WRITE_1_PARAM :
            case DSI_DI_GENERIC_WRITE_LONG :
            {
                err = dsi_generic_write(hostId, &table[i].para_list[0], table[i].count);
                if(err) dsi_mipi_set_err_code(hostId);
            }
            break;

            case DSI_DI_DCS_WRITE_0_PARAM :
            case DSI_DI_DCS_WRITE_1_PARAM :
            case DSI_DI_DCS_WRITE_LONG :
            {
                err = dsi_dcs_write(hostId, &table[i].para_list[0], table[i].count);
                if(err) dsi_mipi_set_err_code(hostId);
            }
            break;

            default:
            break;
        }
    }

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _dsi_power_on(host_core_t hostId)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(hostId);

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = lcm_clk_on(&dsiContext->lcmClkOnIn, &dsiContext->lcmClkOnOut);
    EXIT_IF_FAIL(rtn, NULL, "lcm_clk_on ~_~!\n");

    rtn = dsi_host_power_on(&dsiContext->hostPowerOnIn, &dsiContext->hostPowerOnOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_power_on ~_~!\n");

    rtn = dsi_phy_power_on(&dsiContext->phyPowerOnIn, &dsiContext->phyPowerOnOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_power_on ~_~!\n");

    #if 0
    rtn = dsi_phy_power_on(&dsiContext->phyPowerOnIn, &dsiContext->phyPowerOnOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_power_on ~_~!\n");

    rtn = vou_clk_on(dsiContext->clk_base_addr, dsiContext->vouClkOut.cfg.sel, dsiContext->vouClkOut.cfg.div);
    EXIT_IF_FAIL(rtn, NULL, "vou_clk_on ~_~!\n");
    #endif

    EXIT:

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _dsi_power_off(host_core_t hostId)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(hostId);

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = dsi_host_power_off(&dsiContext->hostPowerOffIn, &dsiContext->hostPowerOffOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_power_off ~_~!\n");

    rtn = dsi_phy_power_off(&dsiContext->phyPowerOffIn, &dsiContext->phyPowerOffOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_power_off ~_~!\n");

    rtn = lcm_clk_off(&dsiContext->lcmClkOffIn, &dsiContext->lcmClkOffOut);
    EXIT_IF_FAIL(rtn, NULL, "lcm_clk_off ~_~!\n");

    EXIT:

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_init(dsi_init_in_t *inParam, dsi_init_out_t *outParam)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(inParam->hostId);

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "inParam is null ~_~!\n");
    CHECK_PTR(outParam, NULL, "inParam is null ~_~!\n");
    CHECK_PTR(dsiContext, NULL, "dsiContext is null ~_~!\n");

    if (DSI_INITED == dsiContext->status) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_init status(%d) ~_~!\n", dsiContext->status);
    }

    rtn = _dsi_init_param_cfg(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_init_param_cfg ~_~!\n");

    rtn = lcm_bus_pin_cfg(dsiContext->dsiInitIn.pin_base_addr, dsiContext->lcm_pin_type);
    EXIT_IF_FAIL(rtn, NULL, "lcm_bus_dsi_cfg ~_~!\n");

    rtn = _dsi_power_on(dsiContext->dsiInitIn.hostId);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_power_on ~_~!\n");

    rtn = dsi_phy_v0_init(&dsiContext->phyInitIn, &dsiContext->phyInitOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_v0_init ~_~!\n");

    rtn = dsi_host_v0_init(&dsiContext->hostInitIn, &dsiContext->hostInitOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_v0_init ~_~!\n");

    dsiContext->status = DSI_INITED;

    EXIT:

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_deinit(dsi_deinit_in_t *inParam, dsi_deinit_out_t *outParam)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_mpi_context_t *dsiContext = dsi_mipi_context_get(inParam->hostId);

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    if (DSI_INITED != dsiContext->status) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_deinit status(%d) ~_~!\n", dsiContext->status);
    }

    CHECK_PTR(inParam, NULL, "inParam is null ~_~!\n");
    CHECK_PTR(outParam, NULL, "inParam is null ~_~!\n");

    rtn = _dsi_deinit_param_cfg(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_deinit_param_cfg ~_~!\n");

    rtn = dsi_host_v0_deinit(&dsiContext->hostDeinitIn, &dsiContext->hostDeinitOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_v0_init ~_~!\n");

    rtn = dsi_phy_v0_deinit(&dsiContext->phyDeinitIn, &dsiContext->phyDeinitOut);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_v0_init ~_~!\n");

    rtn = _dsi_power_off(dsiContext->dsiDeInitIn.hostId);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_power_off ~_~!\n");

    rtn = lcm_bus_pin_default_cfg(dsiContext->pin_base_addr, dsiContext->lcm_pin_type);
    EXIT_IF_FAIL(rtn, NULL, "lcm_bus_pin_default_cfg ~_~!\n");

    dsiContext->status = DSI_UN_INITED;
    EXIT:

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_lp_mode(dsi_lp_mode_in_t *inParam, dsi_lp_mode_out_t *outParam)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = dsi_host_lp_mode(inParam->hostId);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_lp_mode ~_~!\n");

    EXIT:

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

int32_t dsi_hs_mode(dsi_hs_mode_in_t *inParam, dsi_hs_mode_out_t *outParam)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    rtn = dsi_host_hs_mode(inParam->hostId);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_hs_mode ~_~!\n");

    EXIT:

    #if DSI_MPI_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

#ifdef __cplusplus
}
#endif

