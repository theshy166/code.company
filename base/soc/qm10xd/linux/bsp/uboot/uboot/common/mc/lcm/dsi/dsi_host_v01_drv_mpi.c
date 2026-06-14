#include "dsi_host_v01_drv_param.h"
#include "dsi_host_v01_drv.h"
#include "dsi_phy_v01_drv.h"
#include "../clk/vou_clk_v01_drv.h"
#include "dsi_host_v01_reg_def.h"
#include "dsi_host_v01_drv_mpi_param.h"
#include "../reg/reg.h"

#include "../reg/reg_pool.h"
#include "dsi_host_v01_reg_pool.h"
#include "../os.h"

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
#define DSI_HOST_MPI_DEBUG 0
#define DSI_HOST_MPI_FUN_DEBUG 0

#define SPRDFB_MIPI_READ_COUNT_MAX 100
#define DSIH_FIFO_ACTIVE_WAIT         (100)    /* no of tries to access the fifo 10000000*/

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct {
    int32_t power_on_cnt[PHY_CORE_MAX];
    int32_t power_off_cnt[PHY_CORE_MAX];
    int32_t power_cur_cnt[PHY_CORE_MAX];
    int32_t power_cnt;
}dsi_host_power_t;

typedef struct {
    ADDR host_base_addr;
    ADDR clk_base_addr;
    ADDR rst_base_addr;

    uint32_t power_up;
    uint32_t tx_esc_clk_division;
    uint32_t to_clk_division;
    uint32_t dpi_vcid;
    dsi_host_dpi_color_format_e dpi_color_format;
    uint32_t loosely18_en;
    uint32_t dataen_pol;
    uint32_t vsync_pol;
    uint32_t hsync_pol;
    uint32_t shutd_pol;
    uint32_t colorm_pol;
    uint32_t invact_lpcmd_time;
    uint32_t outvact_lpcmd_time;
    uint32_t dbi_vcid;
    dsi_host_dbi_data_e in_dbi_conf;
    dsi_host_dbi_data_e out_dbi_conf;
    dsi_host_write_lut_size_e lut_size_conf;
    uint32_t partitioning_en;
    uint32_t wr_cmd_size;
    uint32_t allowed_cmd_size;
    uint32_t eotp_tx_en;
    uint32_t eotp_rx_en;
    uint32_t bta_en;
    uint32_t ecc_rx_en;
    uint32_t crc_rx_en;
    uint32_t gen_vcid_rx;
    uint32_t cmd_video_mode;
    dsi_host_vid_mode_type_e vid_mode_type;
    uint32_t lp_vsa_en;
    uint32_t lp_vbp_en;
    uint32_t lp_vfp_en;
    uint32_t lp_vact_en;
    uint32_t lp_hbp_en;
    uint32_t lp_hfp_en;
    uint32_t frame_bta_ack_en;
    uint32_t lp_cmd_en;
    uint32_t vpg_en;
    uint32_t vpg_orientation;
    uint32_t vid_pkt_size;
    uint32_t vid_num_chunks;
    uint32_t vid_null_size;
    uint32_t vid_hsa_time;
    uint32_t vid_hbp_time;
    uint32_t vid_hline_time;
    uint32_t vsa_lines;
    uint32_t vbp_lines;
    uint32_t vfp_lines;
    uint32_t v_active_lines;
    uint32_t edpi_allowed_cmd_size;
    uint32_t tear_fx_en;
    uint32_t ack_rqst_en;
    uint32_t gen_sw_0p_tx;
    uint32_t gen_sw_1p_tx;
    uint32_t gen_sw_2p_tx;
    uint32_t gen_sr_0p_tx;
    uint32_t gen_sr_1p_tx;
    uint32_t gen_sr_2p_tx;
    uint32_t gen_lw_tx;
    uint32_t dcs_sw_0p_tx;
    uint32_t dcs_sw_1p_tx;
    uint32_t dcs_sr_0p_tx;
    uint32_t dcs_lw_tx;
    uint32_t max_rd_pkt_size;
    uint32_t gen_dt;
    uint32_t gen_vc;
    uint32_t gen_wc_lsbyte;
    uint32_t gen_wc_msbyte;
    uint32_t gen_pld_b1;
    uint32_t gen_pld_b2;
    uint32_t gen_pld_b3;
    uint32_t gen_pld_b4;
    uint32_t gen_pld_data;
    uint32_t gen_cmd_empty;
    uint32_t gen_cmd_full;
    uint32_t gen_pld_w_empty;
    uint32_t gen_pld_w_full;
    uint32_t gen_pld_r_empty;
    uint32_t gen_pld_r_full;
    uint32_t gen_rd_cmd_busy;
    uint32_t dbi_cmd_empy;
    uint32_t dbi_cmd_full;
    uint32_t dbi_pld_w_empty;
    uint32_t dbi_pld_w_full;
    uint32_t dbi_pld_r_empty;
    uint32_t dbi_pld_r_full;
    uint32_t dbi_rd_cmd_busy;
    uint32_t lprx_to_cnt;
    uint32_t hstx_to_cnt;
    uint32_t hs_rd_to_cnt;
    uint32_t lp_rd_to_cnt;
    uint32_t hs_wr_to_cnt;
    uint32_t presp_to_mode;
    uint32_t lp_wr_to_cnt;
    uint32_t bta_to_cnt;
    dsi_host_mode_3d_e mode_3d;
    dsi_host_format_3d_e format_3d;
    uint32_t second_vsync;
    uint32_t right_first;
    uint32_t send_3d_cfg;
    uint32_t phy_txrequestclkhs;
    uint32_t auto_clklane_ctrl;
    uint32_t phy_clklp2hs_time;
    uint32_t phy_clkhs2lp_time;
    uint32_t phy_lp2hs_time;
    uint32_t phy_hs2lp_time;
    uint32_t phy_shutdownz;
    uint32_t phy_rstz;
    uint32_t phy_enableclk;
    uint32_t phy_forcepll;
    dsi_host_n_lanes_e n_lanes;
    uint32_t phy_stop_wait_time;
    uint32_t phy_txrequlpsclk;
    uint32_t phy_txexitulpsclk;
    uint32_t phy_txrequlpslan;
    uint32_t phy_txexitulpslan;
    uint32_t phy_tx_triggers;
    uint32_t phy_lock;
    uint32_t phy_direction;
    uint32_t phy_stopstateclklane;
    uint32_t phy_ulpsactivenotclk;
    uint32_t phy_stopstate0lane;
    uint32_t phy_ulpsactivenot0lane;
    uint32_t phy_rxulpsesc0lane;
    uint32_t phy_stopstate1lane;
    uint32_t phy_ulpsactivenot1lane;
    uint32_t phy_stopstate2lane;
    uint32_t phy_ulpsactivenot2lane;
    uint32_t phy_stopstate3lane;
    uint32_t phy_ulpsactivenot3lane;
    uint32_t phy_testclr;
    uint32_t phy_testclk;
    uint32_t phy_testdin;
    uint32_t phy_testdout;
    uint32_t phy_testen;
    /*int0*/
    uint32_t ack_with_err_0;
    uint32_t ack_with_err_1;
    uint32_t ack_with_err_2;
    uint32_t ack_with_err_3;
    uint32_t ack_with_err_4;
    uint32_t ack_with_err_5;
    uint32_t ack_with_err_6;
    uint32_t ack_with_err_7;
    uint32_t ack_with_err_8;
    uint32_t ack_with_err_9;
    uint32_t ack_with_err_10;
    uint32_t ack_with_err_11;
    uint32_t ack_with_err_12;
    uint32_t ack_with_err_13;
    uint32_t ack_with_err_14;
    uint32_t ack_with_err_15;
    uint32_t dphy_errors_0;
    uint32_t dphy_errors_1;
    uint32_t dphy_errors_2;
    uint32_t dphy_errors_3;
    uint32_t dphy_errors_4;
    /*int1*/
    uint32_t to_hs_tx;
    uint32_t to_lp_rx;
    uint32_t ecc_single_err;
    uint32_t ecc_multi_err;
    uint32_t crc_err;
    uint32_t pkt_size_err;
    uint32_t eopt_err;
    uint32_t dpi_pld_wr_err;
    uint32_t gen_cmd_wr_err;
    uint32_t gen_pld_wr_err;
    uint32_t gen_pld_send_err;
    uint32_t gen_pld_rd_err;
    uint32_t gen_pld_recev_err;
    uint32_t dbi_cmd_wr_err;
    uint32_t dbi_pld_wr_err;
    uint32_t dbi_pld_rd_err;
    uint32_t dbi_pld_recv_err;
    uint32_t dbi_ilegal_comm_err;
    uint32_t int0_status;
    uint32_t int1_status;
    uint32_t int0_mask;
    uint32_t int1_mask;
    uint32_t txskewcalhs;
    uint32_t compression_mode;
    uint32_t compress_algo;
    dsi_host_pps_sel_e pps_sel;
    uint32_t max_rd_time;
    uint32_t vid_shadow_en;
    uint32_t vid_shadow_req;
    uint32_t vid_shadow_pin_req;
//    uint32_t dpi_vcid;
    dsi_host_dpi_color_format_e dpi_color_coding;
//    uint32_t loosely18_en;
//    uint32_t invact_lpcmd_time;
//    uint32_t outvact_lpcmd_time;
//    dsi_host_vid_mode_type_e vid_mode_type;
//    uint32_t lp_vsa_en;
//    uint32_t lp_vbp_en;
//    uint32_t lp_vfp_en;
//    uint32_t lp_vact_en;
//    uint32_t lp_hbp_en;
//    uint32_t lp_hfp_en;
//    uint32_t frame_bta_ack_en;
//    uint32_t lp_cmd_en;
//    uint32_t vid_pkt_size;
//    uint32_t vid_num_chunks;
//    uint32_t vid_null_size;
    uint32_t vid_has_time;
//    uint32_t vid_hbp_time;
//    uint32_t vid_hline_time;
//    uint32_t vsa_lines;
//    uint32_t vbp_lines;
//    uint32_t vfp_lines;
//    uint32_t v_active_lines;
//    dsi_host_mode_3d_e mode_3d;
//    dsi_host_format_3d_e format_3d;
//    uint32_t second_vsync;
//    uint32_t right_first;
//    uint32_t send_3d_cfg;
}dsi_host_core_t;


typedef struct {
    dsi_status_e status[HOST_CORE_MAX];
    dsi_host_power_t power;
    dsi_host_core_t core[HOST_CORE_MAX];
}dsi_host_mpi_context_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
dsi_host_mpi_context_t s_dsi_host_mpi_context = {0x00};


/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
dsi_host_mpi_context_t *dsi_host_mipi_context_get(void)
{
    return &s_dsi_host_mpi_context;
}

dsi_host_core_t *dsi_host_core_context_get(host_core_t hostId)
{
    return &s_dsi_host_mpi_context.core[hostId];
}

static int32_t _dsi_host_v0_init_param_checker(dsi_host_init_in_t *inParam, dsi_host_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(inParam, NULL, "dsi_host_init_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_init_in_t is null ~_~\n");

    EXIT:

    return rtn;
}

static int32_t _dsi_host_v0_init_param_set(dsi_host_init_in_t *inParam, dsi_host_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    uint32_t val = 0x00;
    uint32_t host_sync_div = 0x00;
    lcm_display_sync_info_t *dp_sync_info = (lcm_display_sync_info_t *)inParam->dp_sync_info;
    dsi_host_core_t *coreContext = NULL;

    CHECK_PTR(inParam, NULL, "dsi_host_init_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_init_in_t is null ~_~\n");

    coreContext = dsi_host_core_context_get(inParam->hostId);

    coreContext->host_base_addr = inParam->base_addr;

    coreContext->dpi_vcid = 0x0;
    coreContext->dpi_color_format = inParam->format;
    coreContext->dpi_color_coding = COLOR_CODE_24BIT;
    coreContext->hsync_pol = 1;
    coreContext->vsync_pol = 1;
//    coreContext->tx_esc_clk_division = 0xc;
    coreContext->tx_esc_clk_division = *inParam->phy_txreadyesc + 1;
    coreContext->to_clk_division = 0x1;

    coreContext->tear_fx_en = 1;
    coreContext->ack_rqst_en = 0;
    coreContext->gen_sw_0p_tx = 1;
    coreContext->gen_sw_1p_tx = 1;
    coreContext->gen_sw_2p_tx = 1;
    coreContext->gen_sr_0p_tx = 1;
    coreContext->gen_sr_1p_tx = 1;
    coreContext->gen_sr_2p_tx = 1;
    coreContext->gen_lw_tx = 1;
    coreContext->dcs_sw_0p_tx = 1;
    coreContext->dcs_sw_1p_tx = 1;
    coreContext->dcs_sr_0p_tx = 1;
    coreContext->dcs_lw_tx = 1;
    coreContext->max_rd_pkt_size = 1;

    coreContext->dbi_vcid = 0x1;

    coreContext->in_dbi_conf = BITS_WIDTH8_BPP_24;
    coreContext->lut_size_conf = BIT_COLOR_DISPLAY_18;
    coreContext->partitioning_en = 1;
    coreContext->allowed_cmd_size = 0x64;

    coreContext->wr_cmd_size = 0x8;
    coreContext->bta_en = 1;
    coreContext->ecc_rx_en = 1;
    coreContext->crc_rx_en = 1;
    coreContext->gen_vcid_rx = 0x0;
    coreContext->cmd_video_mode = 1;
    coreContext->vid_mode_type = VIDEO_BURST_WITH_SYNC_PULSES;
    coreContext->lp_vsa_en = 0;
    coreContext->lp_vbp_en = 1;
    coreContext->lp_vfp_en = 0;
    coreContext->lp_vact_en = 0;
    coreContext->lp_hbp_en = 0;
    coreContext->lp_hfp_en = 0;
    coreContext->vid_pkt_size = dp_sync_info->hor_active_pixel;
    coreContext->vid_num_chunks = 1;
    coreContext->vid_null_size = 0x0a;

    host_sync_div = *inParam->host_sync_div;

    #if DSI_HOST_MPI_DEBUG
    printf("pixel_clk_div: %d\n", host_sync_div);
    #endif

    val = (dp_sync_info->hor_sync_active * host_sync_div / 8);
    coreContext->vid_hsa_time = val;
    val = (dp_sync_info->hor_back_porch * host_sync_div / 8);
    coreContext->vid_hbp_time = val;
    val = ((dp_sync_info->hor_sync_active + dp_sync_info->hor_back_porch + dp_sync_info->hor_active_pixel + dp_sync_info->hor_front_porch) * host_sync_div / 8);
    coreContext->vid_hline_time = val;
    /*data lane sync, uint byte(8bits), rgb888 24bits/lane_num*/
    coreContext->vsa_lines = dp_sync_info->ver_sync_active;
    coreContext->vbp_lines = dp_sync_info->ver_back_porch;
    coreContext->v_active_lines = dp_sync_info->ver_active_line;
    coreContext->vfp_lines = dp_sync_info->ver_front_porch;

    coreContext->outvact_lpcmd_time = 0;
    coreContext->invact_lpcmd_time = 0;
    coreContext->phy_lp2hs_time = 0x6e;
    coreContext->phy_hs2lp_time = 0x25;
    coreContext->edpi_allowed_cmd_size = 0x0;
    coreContext->phy_txrequestclkhs = 1;
    coreContext->lprx_to_cnt = 0;
    coreContext->hstx_to_cnt = 0;
    coreContext->hs_rd_to_cnt = 0;
    coreContext->lp_rd_to_cnt = 0;
    coreContext->hs_wr_to_cnt = 0;
    coreContext->lp_wr_to_cnt = 0;
    coreContext->bta_to_cnt = 0x8;
    coreContext->phy_clklp2hs_time = 0x87;
    coreContext->phy_clkhs2lp_time = 0x25;
    coreContext->max_rd_time = 0;
    coreContext->phy_stop_wait_time = 0x28;
    coreContext->n_lanes = inParam->laneNum;
    coreContext->phy_txrequlpsclk = 0;
    coreContext->phy_txexitulpsclk = 0;
    coreContext->phy_txrequlpslan = 0;
    coreContext->phy_txexitulpslan = 0;
    coreContext->phy_tx_triggers = 0;
    coreContext->vid_shadow_en = 0;
    coreContext->vid_shadow_req = 0;
    coreContext->vid_shadow_pin_req = 0;

    //ENABLE ALL INTERRUPTS
    coreContext->int0_mask = 0xffffffff;
    coreContext->int1_mask = 0xffffffff;

    //WAKING UP CORE
    coreContext->power_up = 0;

    #if DSI_HOST_MPI_DEBUG
    printf("coreContext->host_sync_div   : %d\n", host_sync_div              );
    printf("coreContext->vid_pkt_size    : %d\n", coreContext->vid_pkt_size  );
    printf("coreContext->vid_hsa_time    : %d\n", coreContext->vid_hsa_time  );
    printf("coreContext->vid_hbp_time    : %d\n", coreContext->vid_hbp_time  );
    printf("coreContext->vid_hline_time  : %d\n", coreContext->vid_hline_time);
    printf("coreContext->vsa_lines       : %d\n", coreContext->vsa_lines     );
    printf("coreContext->vbp_lines       : %d\n", coreContext->vbp_lines     );
    printf("coreContext->vfp_lines       : %d\n", coreContext->vfp_lines     );
    printf("coreContext->v_active_lines  : %d\n", coreContext->v_active_lines);

    printf("dp_sync_info->hor_sync_active          : %d\n", dp_sync_info->hor_sync_active );
    printf("dp_sync_info->hor_back_porch           : %d\n", dp_sync_info->hor_back_porch  );
    printf("dp_sync_info->hor_active_pixel         : %d\n", dp_sync_info->hor_active_pixel);
    printf("dp_sync_info->hor_front_porch          : %d\n", dp_sync_info->hor_front_porch );
    printf("dp_sync_info->ver_sync_active          : %d\n", dp_sync_info->ver_sync_active );
    printf("dp_sync_info->ver_back_porch           : %d\n", dp_sync_info->ver_back_porch  );
    printf("dp_sync_info->ver_active_line          : %d\n", dp_sync_info->ver_active_line );
    printf("dp_sync_info->ver_front_porch          : %d\n", dp_sync_info->ver_front_porch );
    #endif

    EXIT:

    return rtn;
}

static int32_t _dsi_host_v0_power_on_param_set(dsi_host_power_on_in_t *inParam, dsi_host_power_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *dsiHostContext = dsi_host_mipi_context_get();
    dsi_host_power_t *power = NULL;
    dsi_host_core_t *coreContext = NULL;

    CHECK_PTR(inParam, NULL, "dsi_host_power_on_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_power_on_out_t is null ~_~\n");

    coreContext = dsi_host_core_context_get(inParam->hostId);

    coreContext->clk_base_addr = inParam->clk_addr;
    coreContext->rst_base_addr = inParam->rst_addr;
    coreContext->host_base_addr = inParam->host_addr;

    power = &dsiHostContext->power;

    power->power_on_cnt[inParam->hostId]++;
    power->power_cur_cnt[inParam->hostId]++;
    power->power_cnt++;

    EXIT:

    return rtn;
}

static int32_t _dsi_host_v0_power_off_param_set(dsi_host_power_off_in_t *inParam, dsi_host_power_off_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *dsiHostContext = dsi_host_mipi_context_get();
    dsi_host_power_t *power = NULL;

    CHECK_PTR(inParam, NULL, "dsi_host_power_off_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_power_off_out_t is null ~_~\n");

    power = &dsiHostContext->power;

    power->power_off_cnt[inParam->hostId]++;
    power->power_cur_cnt[inParam->hostId]--;
    power->power_cnt--;

    if (0 > power->power_cur_cnt[inParam->hostId]) {
        printf("host power[%d]->power_on_cnt                      : %d\n", inParam->hostId, power->power_on_cnt[inParam->hostId]     );
        printf("host power[%d]->power_off_cnt                     : %d\n", inParam->hostId, power->power_off_cnt[inParam->hostId]    );
        printf("host power[%d]->power_cur_cnt                     : %d\n", inParam->hostId, power->power_cur_cnt[inParam->hostId]    );
        printf("host power->power_cnt                         : %d\n", power->power_cnt        );
    }

    if (0 == power->power_cur_cnt[inParam->hostId]) {
        power->power_on_cnt[inParam->hostId] = 0x00;
        power->power_off_cnt[inParam->hostId] = 0x00;
        power->power_cur_cnt[inParam->hostId] = 0x00;
    }

    EXIT:

    return rtn;
}


static int32_t _dsi_host_hs_mode_param_cfg(    host_core_t hostId)
{
    int32_t rtn = SUCCESS;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    coreContext->tear_fx_en = 0x00;
    coreContext->ack_rqst_en = 0x00;
    coreContext->gen_sw_0p_tx = 0x00;
    coreContext->gen_sw_1p_tx = 0x00;
    coreContext->gen_sw_2p_tx = 0x00;
    coreContext->gen_sr_0p_tx = 0x00;
    coreContext->gen_sr_1p_tx = 0x00;
    coreContext->gen_sr_2p_tx = 0x00;
    coreContext->gen_lw_tx = 0x00;
    coreContext->dcs_sw_0p_tx = 0x00;
    coreContext->dcs_sw_1p_tx = 0x00;
    coreContext->dcs_sr_0p_tx = 0x00;
    coreContext->dcs_lw_tx = 0x00;
    coreContext->max_rd_pkt_size = 0x00;

    coreContext->eotp_tx_en = 0x00;
    coreContext->eotp_rx_en = 0x00;
    coreContext->bta_en = 0x01;
    coreContext->ecc_rx_en = 0x00;
    coreContext->crc_rx_en = 0x01;

    coreContext->cmd_video_mode = 0x00;

    return rtn;
}

static int32_t _dsi_host_lp_mode_param_cfg(    host_core_t hostId)
{
    int32_t rtn = SUCCESS;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    coreContext->tear_fx_en = 0x01;
    coreContext->ack_rqst_en = 0x00;
    coreContext->gen_sw_0p_tx = 0x00;
    coreContext->gen_sw_1p_tx = 0x00;
    coreContext->gen_sw_2p_tx = 0x01;
    coreContext->gen_sr_0p_tx = 0x01;
    coreContext->gen_sr_1p_tx = 0x01;
    coreContext->gen_sr_2p_tx = 0x01;
    coreContext->gen_lw_tx = 0x01;
    coreContext->dcs_sw_0p_tx = 0x01;
    coreContext->dcs_sw_1p_tx = 0x01;
    coreContext->dcs_sr_0p_tx = 0x01;
    coreContext->dcs_lw_tx = 0x01;
    coreContext->max_rd_pkt_size = 0x01;

    coreContext->eotp_tx_en = 0x00;
    coreContext->eotp_rx_en = 0x00;
    coreContext->bta_en = 0x01;
    coreContext->ecc_rx_en = 0x01;
    coreContext->crc_rx_en = 0x01;

    coreContext->cmd_video_mode = 0x01;

    return rtn;
}

static int32_t _dsi_host_bus_mode_cfg(    host_core_t hostId)
{
    int32_t rtn = SUCCESS;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    /*DSI_HOST_CMD_MODE_CFG*/
    rtn = dsi_host_set_tear_fx_en(coreContext->host_base_addr, coreContext->tear_fx_en);
    rtn = dsi_host_set_ack_rqst_en(coreContext->host_base_addr, coreContext->ack_rqst_en);
    rtn = dsi_host_set_gen_sw_0p_tx(coreContext->host_base_addr, coreContext->gen_sw_0p_tx);
    rtn = dsi_host_set_gen_sw_1p_tx(coreContext->host_base_addr, coreContext->gen_sw_1p_tx);
    rtn = dsi_host_set_gen_sw_2p_tx(coreContext->host_base_addr, coreContext->gen_sw_2p_tx);
    rtn = dsi_host_set_gen_sr_0p_tx(coreContext->host_base_addr, coreContext->gen_sr_0p_tx);
    rtn = dsi_host_set_gen_sr_1p_tx(coreContext->host_base_addr, coreContext->gen_sr_1p_tx);
    rtn = dsi_host_set_gen_sr_2p_tx(coreContext->host_base_addr, coreContext->gen_sr_2p_tx);
    rtn = dsi_host_set_gen_lw_tx(coreContext->host_base_addr, coreContext->gen_lw_tx);
    rtn = dsi_host_set_dcs_sw_0p_tx(coreContext->host_base_addr, coreContext->dcs_sw_0p_tx);
    rtn = dsi_host_set_dcs_sw_1p_tx(coreContext->host_base_addr, coreContext->dcs_sw_1p_tx);
    rtn = dsi_host_set_dcs_sr_0p_tx(coreContext->host_base_addr, coreContext->dcs_sr_0p_tx);
    rtn = dsi_host_set_dcs_lw_tx(coreContext->host_base_addr, coreContext->dcs_lw_tx);
    rtn = dsi_host_set_max_rd_pkt_size(coreContext->host_base_addr, coreContext->max_rd_pkt_size);

    /*DSI_HOST_PCKHDL_CFG*/
    rtn = dsi_host_set_pckhdl_eotp_tx_en(coreContext->host_base_addr, coreContext->eotp_tx_en);
    rtn = dsi_host_set_pckhdl_eotp_rx_en(coreContext->host_base_addr, coreContext->eotp_rx_en);
    rtn = dsi_host_set_pckhdl_bta_en(coreContext->host_base_addr, coreContext->bta_en);
    rtn = dsi_host_set_pckhdl_ecc_rx_en(coreContext->host_base_addr, coreContext->ecc_rx_en);
    rtn = dsi_host_set_pckhdl_crc_rx_en(coreContext->host_base_addr, coreContext->crc_rx_en);

    /*DSI_HOST_MODE_CFG*/
    rtn = dsi_host_set_cmd_video_mode(coreContext->host_base_addr, coreContext->cmd_video_mode);

    #if 0
    {
        char hostName[2][64] = {{"HOST0"},{"HOST1"}};
        rtn = regDefaultGet(coreContext->host_base_addr, dsi_host_get_reg_default_tab());
        rtn = showRegValuePtr(dsi_host_get_reg_default_tab(), hostName[hostId]);
    }
    #endif

    return rtn;
}


static int32_t _dsi_host_v0_cfg(dsi_host_init_in_t *inParam, dsi_host_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_core_t *coreContext = NULL;

    CHECK_PTR(inParam, NULL, "dsi_host_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_init_outparam_t is null ~_~\n");

    coreContext = dsi_host_core_context_get(inParam->hostId);

    rtn = dsi_host_set_dpi_vcid(coreContext->host_base_addr, coreContext->dpi_vcid);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dpi_vcid ~_~\n");

    rtn = dsi_host_set_dpi_color_format(coreContext->host_base_addr, coreContext->dpi_color_format);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dpi_color_format ~_~\n");

    rtn = dsi_host_set_dpi_hsync_pol(coreContext->host_base_addr, coreContext->hsync_pol);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dpi_hsync_pol ~_~\n");

    rtn = dsi_host_set_dpi_vsync_pol(coreContext->host_base_addr, coreContext->vsync_pol);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dpi_vsync_pol ~_~\n");

    rtn = dsi_host_set_tx_esc_clk_division(coreContext->host_base_addr, coreContext->tx_esc_clk_division);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_tx_esc_clk_division ~_~\n");

    rtn = dsi_host_set_to_clk_division(coreContext->host_base_addr, coreContext->to_clk_division);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_to_clk_division ~_~\n");

    rtn = dsi_host_set_tear_fx_en(coreContext->host_base_addr, coreContext->tear_fx_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_tear_fx_en ~_~\n");

    rtn = dsi_host_set_ack_rqst_en(coreContext->host_base_addr, coreContext->ack_rqst_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_ack_rqst_en ~_~\n");

    rtn = dsi_host_set_gen_sw_0p_tx(coreContext->host_base_addr, coreContext->gen_sw_0p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_sw_0p_tx ~_~\n");

    rtn = dsi_host_set_gen_sw_1p_tx(coreContext->host_base_addr, coreContext->gen_sw_1p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_sw_1p_tx ~_~\n");

    rtn = dsi_host_set_gen_sw_2p_tx(coreContext->host_base_addr, coreContext->gen_sw_2p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_sw_2p_tx ~_~\n");

    rtn = dsi_host_set_gen_sr_0p_tx(coreContext->host_base_addr, coreContext->gen_sr_0p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_sr_0p_tx ~_~\n");

    rtn = dsi_host_set_gen_sr_1p_tx(coreContext->host_base_addr, coreContext->gen_sr_1p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_sr_1p_tx ~_~\n");

    rtn = dsi_host_set_gen_sr_2p_tx(coreContext->host_base_addr, coreContext->gen_sr_2p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_sr_2p_tx ~_~\n");

    rtn = dsi_host_set_gen_lw_tx(coreContext->host_base_addr, coreContext->gen_lw_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_lw_tx ~_~\n");

    rtn = dsi_host_set_dcs_sw_0p_tx(coreContext->host_base_addr, coreContext->dcs_sw_0p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dcs_sw_0p_tx ~_~\n");

    rtn = dsi_host_set_dcs_sw_1p_tx(coreContext->host_base_addr, coreContext->dcs_sw_1p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dcs_sw_1p_tx ~_~\n");

    rtn = dsi_host_set_dcs_sr_0p_tx(coreContext->host_base_addr, coreContext->dcs_sr_0p_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dcs_sr_0p_tx ~_~\n");

    rtn = dsi_host_set_dcs_lw_tx(coreContext->host_base_addr, coreContext->dcs_lw_tx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dcs_lw_tx ~_~\n");

    rtn = dsi_host_set_max_rd_pkt_size(coreContext->host_base_addr, coreContext->max_rd_pkt_size);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_max_rd_pkt_size ~_~\n");

    rtn = dsi_host_set_dbi_vcid(coreContext->host_base_addr, coreContext->dbi_vcid);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dbi_vcid ~_~\n");

    rtn = dsi_host_set_in_dbi_conf(coreContext->host_base_addr, coreContext->in_dbi_conf);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_in_dbi_conf ~_~\n");

    rtn = dsi_host_set_lut_size_conf(coreContext->host_base_addr, coreContext->lut_size_conf);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_lut_size_conf ~_~\n");

    rtn = dsi_host_set_partitioning(coreContext->host_base_addr, coreContext->partitioning_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_partitioning ~_~\n");

    rtn = dsi_host_set_dbi_allowed_cmd_size(coreContext->host_base_addr, coreContext->allowed_cmd_size);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dbi_allowed_cmd_size ~_~\n");

    rtn = dsi_host_set_dbi_wr_cmd_size(coreContext->host_base_addr, coreContext->wr_cmd_size);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_dbi_wr_cmd_size ~_~\n");

    rtn = dsi_host_set_pckhdl_bta_en(coreContext->host_base_addr, coreContext->bta_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_pckhdl_bta_en ~_~\n");

    rtn = dsi_host_set_pckhdl_ecc_rx_en(coreContext->host_base_addr, coreContext->ecc_rx_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_pckhdl_ecc_rx_en ~_~\n");

    rtn = dsi_host_set_pckhdl_crc_rx_en(coreContext->host_base_addr, coreContext->crc_rx_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_pckhdl_crc_rx_en ~_~\n");

    rtn = dsi_host_set_gen_vcid_rx(coreContext->host_base_addr, coreContext->gen_vcid_rx);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_gen_vcid_rx ~_~\n");

    rtn = dsi_host_set_cmd_video_mode(coreContext->host_base_addr, coreContext->cmd_video_mode);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_cmd_video_mode ~_~\n");

    rtn = dsi_host_set_vid_mode_type(coreContext->host_base_addr, coreContext->vid_mode_type);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_mode_type ~_~\n");

    rtn = dsi_host_set_vid_lp_vsa_en(coreContext->host_base_addr, coreContext->lp_vsa_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_lp_vsa_en ~_~\n");

    rtn = dsi_host_set_vid_lp_vbp_en(coreContext->host_base_addr, coreContext->lp_vbp_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_lp_vbp_en ~_~\n");

    rtn = dsi_host_set_vid_lp_vfp_en(coreContext->host_base_addr, coreContext->lp_vfp_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_lp_vfp_en ~_~\n");

    rtn = dsi_host_set_vid_lp_vact_en(coreContext->host_base_addr, coreContext->lp_vact_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_lp_vact_en ~_~\n");

    rtn = dsi_host_set_vid_lp_hbp_en(coreContext->host_base_addr, coreContext->lp_hbp_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_lp_hbp_en ~_~\n");

    rtn = dsi_host_set_vid_lp_hfp_en(coreContext->host_base_addr, coreContext->lp_hfp_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_lp_hfp_en ~_~\n");

    rtn = dsi_host_set_vid_pkt_size(coreContext->host_base_addr, coreContext->vid_pkt_size);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_pkt_size ~_~\n");

    rtn = dsi_host_set_vid_num_chunks(coreContext->host_base_addr, coreContext->vid_num_chunks);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_num_chunks ~_~\n");

    rtn = dsi_host_set_vid_null_size(coreContext->host_base_addr, coreContext->vid_null_size);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_null_size ~_~\n");

    rtn = dsi_host_set_vid_hsa_time(coreContext->host_base_addr, coreContext->vid_hsa_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_hsa_time ~_~\n");

    rtn = dsi_host_set_vid_hbp_time(coreContext->host_base_addr, coreContext->vid_hbp_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_hbp_time ~_~\n");

    rtn = dsi_host_set_vid_hline_time(coreContext->host_base_addr, coreContext->vid_hline_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_hline_time ~_~\n");

    rtn = dsi_host_set_vid_vsa_lines(coreContext->host_base_addr, coreContext->vsa_lines);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_vsa_lines ~_~\n");

    rtn = dsi_host_set_vid_vbp_lines(coreContext->host_base_addr, coreContext->vbp_lines);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_vbp_lines ~_~\n");

    rtn = dsi_host_set_vid_vfp_lines(coreContext->host_base_addr, coreContext->vfp_lines);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_vfp_lines ~_~\n");

    rtn = dsi_host_set_vid_vactive_line(coreContext->host_base_addr, coreContext->v_active_lines);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_vactive_line ~_~\n");

    rtn = dsi_host_set_outvact_lpcmd_time(coreContext->host_base_addr, coreContext->outvact_lpcmd_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_outvact_lpcmd_time ~_~\n");

    rtn = dsi_host_set_invact_lpcmd_time(coreContext->host_base_addr, coreContext->invact_lpcmd_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_invact_lpcmd_time ~_~\n");

    rtn = dsi_host_set_phy_lp2hs_time(coreContext->host_base_addr, coreContext->phy_lp2hs_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_lp2hs_time ~_~\n");

    rtn = dsi_host_set_phy_hs2lp_time(coreContext->host_base_addr, coreContext->phy_hs2lp_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_hs2lp_time ~_~\n");

    rtn = dsi_host_set_edpi_allowed_cmd_size(coreContext->host_base_addr, coreContext->edpi_allowed_cmd_size);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_edpi_allowed_cmd_size ~_~\n");

    rtn = dsi_host_set_phy_txrequestclkhs(coreContext->host_base_addr, coreContext->phy_txrequestclkhs);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_txrequestclkhs ~_~\n");

    rtn = dsi_host_set_lprx_to_cnt(coreContext->host_base_addr, coreContext->lprx_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_lprx_to_cnt ~_~\n");

    rtn = dsi_host_set_hstx_to_cnt(coreContext->host_base_addr, coreContext->hstx_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_hstx_to_cnt ~_~\n");

    rtn = dsi_host_set_hs_rd_to_cnt(coreContext->host_base_addr, coreContext->hs_rd_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_hs_rd_to_cnt ~_~\n");

    rtn = dsi_host_set_lp_rd_to_cnt(coreContext->host_base_addr, coreContext->lp_rd_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_lp_rd_to_cnt ~_~\n");

    rtn = dsi_host_set_hs_wr_to_cnt(coreContext->host_base_addr, coreContext->hs_wr_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_hs_wr_to_cnt ~_~\n");

    rtn = dsi_host_set_presp_to_mode(coreContext->host_base_addr, coreContext->presp_to_mode);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_presp_to_mode ~_~\n");

    rtn = dsi_host_set_lp_wr_to_cnt(coreContext->host_base_addr, coreContext->lp_wr_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_lp_wr_to_cnt ~_~\n");

    rtn = dsi_host_set_bta_to_cnt(coreContext->host_base_addr, coreContext->bta_to_cnt);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_bta_to_cnt ~_~\n");

    rtn = dsi_host_set_phy_clklp2hs_time(coreContext->host_base_addr, coreContext->phy_clklp2hs_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_clklp2hs_time ~_~\n");

    rtn = dsi_host_set_phy_clkhs2lp_time(coreContext->host_base_addr, coreContext->phy_clkhs2lp_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_clkhs2lp_time ~_~\n");


    rtn = dsi_host_set_phy_stop_wait_time(coreContext->host_base_addr, coreContext->phy_stop_wait_time);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_stop_wait_time ~_~\n");

    rtn = dsi_host_set_n_lanes(coreContext->host_base_addr, coreContext->n_lanes);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_n_lanes ~_~\n");

    rtn = dsi_host_set_phy_txrequlpsclk(coreContext->host_base_addr, coreContext->phy_txrequlpsclk);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_txrequlpsclk ~_~\n");

    rtn = dsi_host_set_phy_txexitulpsclk(coreContext->host_base_addr, coreContext->phy_txexitulpsclk);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_txexitulpsclk ~_~\n");

    rtn = dsi_host_set_phy_txrequlpslan(coreContext->host_base_addr, coreContext->phy_txrequlpslan);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_txrequlpslan ~_~\n");

    rtn = dsi_host_set_phy_txexitulpslan(coreContext->host_base_addr, coreContext->phy_txexitulpslan);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_txexitulpslan ~_~\n");

    rtn = dsi_host_set_phy_tx_triggers(coreContext->host_base_addr, coreContext->phy_tx_triggers);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_phy_tx_triggers ~_~\n");

    rtn = dsi_host_set_vid_shadow_en(coreContext->host_base_addr, coreContext->vid_shadow_en);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_shadow_en ~_~\n");

    rtn = dsi_host_set_vid_shadow_req(coreContext->host_base_addr, coreContext->vid_shadow_req);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_shadow_req ~_~\n");

    rtn = dsi_host_set_vid_shadow_pin_req(coreContext->host_base_addr, coreContext->vid_shadow_pin_req);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_vid_shadow_pin_req ~_~\n");

    rtn = dsi_host_set_int0(coreContext->host_base_addr, coreContext->int0_mask);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_int0 ~_~\n");

    rtn = dsi_host_set_int1(coreContext->host_base_addr, coreContext->int1_mask);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_int1 ~_~\n");

    rtn = dsi_host_set_power_up(coreContext->host_base_addr, 0);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_reset ~_~\n");

    rtn = dsi_host_set_power_up(coreContext->host_base_addr, 1);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_set_power_up ~_~\n");

    #if 0
    rtn = regDefaultGet(coreContext->host_base_addr, dsi_host_get_reg_default_tab());
    rtn = showRegValuePtr(dsi_host_get_reg_default_tab(), "host");
    #endif

    EXIT:

    return rtn;
}


static void dsi0_udelay(uint32_t us)
{
    return delayUs(us);
}

static void dsi0_mdelay(uint32_t ms)
{
    return delayMs(ms);
}

uint32_t _dsi_hal_gen_rd_cmd_busy(host_core_t hostId)
{
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    dsi_host_get_gen_rd_cmd_busy(coreContext->host_base_addr, &coreContext->gen_rd_cmd_busy);

    return coreContext->gen_rd_cmd_busy;
}

uint32_t _dsi_hal_gen_read_fifo_empty(host_core_t hostId)
{
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    dsi_host_get_gen_pld_r_empty(coreContext->host_base_addr, &coreContext->gen_pld_r_empty);

    return coreContext->gen_pld_r_empty;
}

int32_t _dsi_hal_gen_read_payload(host_core_t hostId, uint32_t* payload)
{
    int32_t rtn = 0x00;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    rtn = dsi_host_get_gen_pld_data(coreContext->host_base_addr, &coreContext->gen_pld_data);

    *payload = coreContext->gen_pld_data;

    if (rtn) {
        //
    }
    return RTN_DSI_SUCCESS;
}

uint32_t _dsi_hal_gen_write_fifo_full(host_core_t hostId)
{
    int32_t rtn = 0x00;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    rtn = dsi_host_get_gen_pld_w_full(coreContext->host_base_addr, &coreContext->gen_pld_w_full);

    if (rtn) {
        //
    }

    return coreContext->gen_pld_w_full;
}

static int32_t _dsi_hal_gen_packet_payload(host_core_t hostId, uint32_t payload)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    if (_dsi_hal_gen_write_fifo_full(hostId)) {
        rtn = RTN_DSI_OVERFLOW;
        goto EXIT;
    }

    coreContext->gen_pld_data = payload;

    rtn = dsi_host_set_gen_pld_data(coreContext->host_base_addr, coreContext->gen_pld_data);

    EXIT:

    return rtn;
}

static uint32_t _dsi_hal_gen_cmd_fifo_full(host_core_t hostId)
{
    int32_t rtn = 0x00;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    rtn = dsi_host_get_gen_cmd_full(coreContext->host_base_addr, &coreContext->gen_cmd_full);

    if (rtn) {
        //
    }

    return coreContext->gen_cmd_full;
}

static int32_t _dsi_hal_gen_packet_header(host_core_t hostId, uint8_t vc, uint8_t packet_type, uint8_t ms_byte, uint8_t ls_byte)
{
    int32_t rtn = RTN_DSI_OVERFLOW;
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    if (vc < 4) {
        rtn = dsi_host_set_gen_dt(coreContext->host_base_addr, packet_type);
        rtn = dsi_host_set_gen_vc(coreContext->host_base_addr, vc);
        rtn = dsi_host_set_gen_wc_lsbyte(coreContext->host_base_addr, ls_byte);
        rtn = dsi_host_set_gen_wc_msbyte(coreContext->host_base_addr, ms_byte);
        rtn = dsi_host_set_gen_packet_header_cfg(coreContext->host_base_addr, (ms_byte <<  16) | (ls_byte << 8 ) | ((vc << 6) | packet_type));
        rtn = RTN_DSI_SUCCESS;
   }

    return  rtn;
}

/**
 * Send READ packet to peripheral using the generic interface
 * This will force command mode and stop video mode (because of BTA)
 * @param instance pointer to structure holding the DSI Host core information
 * @param vc destination virtual channel
 * @param data_type generic command type
 * @param lsb_byte first command parameter
 * @param msb_byte second command parameter
 * @param bytes_to_read no of bytes to read (expected to arrive at buffer)
 * @param read_buffer pointer to 8-bit array to hold the read buffer words
 * return read_buffer_length
 * @note this function has an active delay to wait for the buffer to clear.
 * The delay is limited to 2 x DSIH_FIFO_ACTIVE_WAIT
 * (waiting for command buffer, and waiting for receiving)
 * @note this function will enable BTA
 */
static int32_t _dsi_gen_rd_packet(host_core_t hostId, uint8_t vc, uint8_t data_type, uint8_t msb_byte, uint8_t lsb_byte, uint8_t bytes_to_read, uint8_t* read_buffer)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    int timeout = 0;
    int counter = 0;
    int i = 0;
    int last_count = 0;
    uint32_t temp[1] = {0};

    if (bytes_to_read < 1) {
        return 0;
    }
    if (read_buffer == 0) {
        return 0;
    }

    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {
        /* check if payload Tx fifo is not full */
        if (!_dsi_hal_gen_cmd_fifo_full(hostId)) {
            _dsi_hal_gen_packet_header(hostId, vc, data_type, msb_byte, lsb_byte);
            dsi0_udelay(100);
            break;
        }
    }

    if (!(timeout < DSIH_FIFO_ACTIVE_WAIT)) {
        printf("dsi: tx rd command timed out\n");
        return 0;
    }
    /* loop for the number of words to be read */
    for (timeout = 0; timeout < 2; timeout++) {
        /* check if command transaction is done */
        if (!_dsi_hal_gen_rd_cmd_busy(hostId)) {
            dsi0_mdelay(1);
            if (!_dsi_hal_gen_read_fifo_empty(hostId)) {

                for (counter = 0; (!_dsi_hal_gen_read_fifo_empty(hostId) && (counter < SPRDFB_MIPI_READ_COUNT_MAX)); counter += 4) {

                    rtn = _dsi_hal_gen_read_payload(hostId, temp);

                    if (rtn) {
                        printf("dsi: read payLoad ~_~\n");
                        return 0;
                    }

                    if (counter < bytes_to_read) {

                        for (i = 0; i < 4; i++) {

                            if ((counter + i) < bytes_to_read) {
                                /* put 32 bit temp in 4 bytes of buffer passed by user*/
                                read_buffer[counter + i] = (uint8_t)(temp[0] >> (i * 8));
                                last_count = i + counter;
                            } else {

                                if ((uint8_t)(temp[0] >> (i * 8)) != 0x00) {
                                    last_count = i + counter;
                                }
                            }
                        }
                    } else {
                        last_count = counter;

                        for (i = 0; i < 4; i++) {
                            if ((uint8_t)(temp[0] >> (i * 8)) != 0x00) {
                                last_count = i + counter;
                            }
                        }
                    }
                }

                if(counter >=  SPRDFB_MIPI_READ_COUNT_MAX){
                    printf("dsi: read too many buffers!\n");
                }

                return last_count + 1;
            } else {
                printf("dsi: read fifo empty ~_~\n");
                return 0;
            }
        }
        if(timeout == 0) dsi0_mdelay(1);
    }

    printf("dsi: rx command timed out\n");

    return 0;
}

static int32_t _dsi_gen_rd_packet_ext(host_core_t hostId, uint8_t vc, uint8_t data_type, uint8_t msb_byte, uint8_t lsb_byte, uint8_t bytes_to_read, uint8_t* read_buffer)
{
    int32_t rtn = RTN_DSI_SUCCESS;

    int timeout = 0;
    int counter = 0;
    int i = 0;
    int last_count = 0;
    uint32_t temp[1] = {0};

    if (bytes_to_read < 1) {
        return 0;
    }
    if (read_buffer == 0) {
        return 0;
    }

    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {
        /* check if payload Tx fifo is not full */
        if (!_dsi_hal_gen_cmd_fifo_full(hostId)) {
            _dsi_hal_gen_packet_header(hostId, vc, data_type, msb_byte, lsb_byte);
            dsi0_udelay(100);
            break;
        }
    }

    if (!(timeout < DSIH_FIFO_ACTIVE_WAIT)) {
        printf("dsi: tx rd command timed out\n");
        return 0;
    }
    /* loop for the number of words to be read */
    for (timeout = 0; timeout < 2; timeout++) {
        /* check if command transaction is done */
        if (!_dsi_hal_gen_rd_cmd_busy(hostId)) {
            dsi0_mdelay(1);
            if (!_dsi_hal_gen_read_fifo_empty(hostId)) {

                for (counter = 0; (!_dsi_hal_gen_read_fifo_empty(hostId) && (counter < SPRDFB_MIPI_READ_COUNT_MAX)); counter += 4) {

                    rtn = _dsi_hal_gen_read_payload(hostId, temp);

                    if (rtn) {
                        printf("dsi: read payLoad ~_~\n");
                        return 0;
                    }

                    if (counter < bytes_to_read) {

                        for (i = 0; i < 4; i++) {

                            if ((counter + i) < bytes_to_read) {
                                /* put 32 bit temp in 4 bytes of buffer passed by user*/
                                read_buffer[counter + i] = (uint8_t)(temp[0] >> (i * 8));
                                last_count = i + counter;
                            } else {

                                if ((uint8_t)(temp[0] >> (i * 8)) != 0x00) {
                                    last_count = i + counter;
                                }
                            }
                        }
                    } else {
                        last_count = counter;

                        for (i = 0; i < 4; i++) {
                            if ((uint8_t)(temp[0] >> (i * 8)) != 0x00) {
                                last_count = i + counter;
                            }
                        }
                    }
                }

                if(counter >=  SPRDFB_MIPI_READ_COUNT_MAX){
                    printf("dsi: read too many buffers!\n");
                }

                return last_count + 1;
            } else {
                printf("dsi: read fifo empty ~_~\n");
                return 0;
            }
        }
        if(timeout == 0) dsi0_mdelay(1);
    }

    printf("dsi: rx command timed out\n");

    return -1;
}

static int32_t _dsi_gen_wr_packet(host_core_t hostId, uint8_t vc, uint8_t data_type, uint8_t* params, uint16_t param_length)
{
    dsi_rtn_t rtn = RTN_DSI_SUCCESS;
    /* active delay iterator */
    int timeout = 0;
    /* iterators */
    int i = 0;
    int j = 0;
    /* holds padding bytes needed */
    int compliment_counter = 0;
    uint8_t* payload = 0;
    /* temporary variable to arrange bytes into words */
    uint32_t temp = 0;
    uint16_t word_count = 0;

    if ((params == 0) && (param_length != 0)) /* pointer NULL */
    {
        return RTN_DSI_OUT_OF_BOUND;
    }

    if (param_length > 2)
    {   /* long packet - write word count to header, and the rest to payload */
        payload = params + (2 * sizeof(params[0]));
        word_count = (params[1] << 8) | params[0];
        if ((param_length - 2) < word_count)
        {
            printf("dsiMipi: sent > input payload. complemented with zeroes");
            compliment_counter = (param_length - 2) - word_count;
        }
        else if ((param_length - 2) > word_count)
        {
            printf("dsiMipi: Overflow - input > sent. payload truncated");
        }
        for (i = 0; i < (param_length - 2); i += j)
        {
            temp = 0;
            for (j = 0; (j < 4) && ((j + i) < (param_length - 2)); j++)
            {   /* temp = (payload[i + 3] << 24) | (payload[i + 2] << 16) | (payload[i + 1] << 8) | payload[i]; */
                temp |= payload[i + j] << (j * 8);
            }
            /* check if payload Tx fifo is not full */
            for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
            {
                if (!_dsi_hal_gen_packet_payload(hostId, temp))
                {
                    break;
                }
            }
            if (!(timeout < DSIH_FIFO_ACTIVE_WAIT))
            {
                return RTN_DSI_TIMEOUT;
            }
        }
        /* if word count entered by the user more than actual parameters received
         * fill with zeroes - a fail safe mechanism, otherwise controller will
         * want to send data from an empty buffer */
        for (i = 0; i < compliment_counter; i++)
        {
            /* check if payload Tx fifo is not full */
            for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
            {
                if (!_dsi_hal_gen_packet_payload(hostId, 0x00))
                {
                    break;
                }
            }
            if (!(timeout < DSIH_FIFO_ACTIVE_WAIT))
            {
                return RTN_DSI_TIMEOUT;
            }
        }
    }

    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
    {
        /* check if payload Tx fifo is not full */
        if (!_dsi_hal_gen_cmd_fifo_full(hostId))
        {
            if (param_length == 0)
            {
                rtn |= _dsi_hal_gen_packet_header(hostId, vc, data_type, 0x0, 0x0);
            }
            else if (param_length == 1)
            {
                rtn |= _dsi_hal_gen_packet_header(hostId, vc, data_type, 0x0, params[0]);
            }
            else
            {
                rtn |= _dsi_hal_gen_packet_header(hostId, vc, data_type, params[1], params[0]);
            }
            break;
        }
    }
    if (!(timeout < DSIH_FIFO_ACTIVE_WAIT))
    {
        rtn = RTN_DSI_TIMEOUT;
    }
    return rtn;
}

/**
 * Send Generic READ command to peripheral
 * - function sets the packet data type automatically
 * @param instance pointer to structure holding the DSI Host core information
 * @param vc destination virtual channel
 * @param params byte array of command parameters
 * @param param_length length of the above array
 * @param bytes_to_read no of bytes to read (expected to arrive at buffer)
 * @param read_buffer pointer to 8-bit array to hold the read buffer words
 * return read_buffer_length
 * @note this function has an active delay to wait for the buffer to clear.
 * The delay is limited to 2 x DSIH_FIFO_ACTIVE_WAIT
 * (waiting for command buffer, and waiting for receiving)
 * @note this function will enable BTA
 */
static uint32_t _dsi_gen_rd_cmd(host_core_t hostId, uint8_t vc, uint8_t* params, uint16_t param_length, uint8_t bytes_to_read, uint8_t* read_buffer)
{
    uint8_t data_type = 0;

    switch(param_length)
    {
        case 0:
            data_type = 0x04;
            return _dsi_gen_rd_packet(hostId, vc, data_type, 0x00, 0x00, bytes_to_read, read_buffer);
        case 1:
            data_type = 0x14;
            return _dsi_gen_rd_packet(hostId, vc, data_type, 0x00, params[0], bytes_to_read, read_buffer);
        case 2:
            data_type = 0x24;
            return _dsi_gen_rd_packet(hostId, vc, data_type, params[1], params[0], bytes_to_read, read_buffer);
        default:
            return 0;
    }
}

/**
 * Send a DCS READ command to peripheral
 * function sets the packet data type automatically
 * @param instance pointer to structure holding the DSI Host core information
 * @param vc destination virtual channel
 * @param command DCS code
 * @param bytes_to_read no of bytes to read (expected to arrive at buffer)
 * @param read_buffer pointer to 8-bit array to hold the read buffer words
 * return read_buffer_length
 * @note this function has an active delay to wait for the buffer to clear.
 * The delay is limited to 2 x DSIH_FIFO_ACTIVE_WAIT
 * (waiting for command buffer, and waiting for receiving)
 * @note this function will enable BTA
 */
uint32_t _dsi_dcs_rd_cmd(host_core_t hostId, uint8_t vc, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t* read_buffer)
{
    switch (data_type)
    {
        case 0xA8:
        case 0xA1:
        case 0x45:
        case 0x3E:
        case 0x2E:
        case 0x0F:
        case 0x0E:
        case 0x0D:
        case 0x0C:
        case 0x0B:
        case 0x0A:
        case 0x08:
        case 0x07:
        case 0x06:
            /* COMMAND_TYPE 0x06 - DCS Read no params refer to DSI spec p.47 */
            // return _dsi_gen_rd_packet(hostId, vc, 0x06, 0x0, command, bytes_to_read, read_buffer);
            return _dsi_gen_rd_packet_ext(hostId, vc, 0x06, 0x0, command, bytes_to_read, read_buffer);
        default:
            printf("dsiMipi: invalid DCS command");
            return 0;
    }
    return 0;
}


int32_t dsi_gen_wr_cmd(host_core_t hostId, uint8_t* params, uint16_t param_length)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    uint8_t data_type = 0;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();

    if (DSI_INITED != hostContext->status[hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_gen_wr_cmd status(%d) ~_~!\n", hostContext->status[hostId]);
    }

    switch(param_length)
    {
        case 0:
            data_type = 0x03;
            break;
        case 1:
            data_type = 0x13;
            break;
        case 2:
            data_type = 0x23;
            break;
        default:
            data_type = 0x29;
            break;
    }

    rtn = _dsi_gen_wr_packet(hostId, 0, data_type, params, param_length);

    EXIT:

    return rtn;
}

int32_t dsi_gen_rd_cmd(host_core_t hostId, uint8_t *param, uint16_t param_length, uint8_t bytes_to_read, uint8_t *read_buffer)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    if (DSI_INITED != hostContext->status[hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_gen_rd_cmd status(%d) ~_~!\n", hostContext->status[hostId]);
    }

    rtn = _dsi_gen_rd_cmd(hostId, 0, param, param_length, bytes_to_read, read_buffer);

    rtn = dsi_host_get_phy_direction(coreContext->host_base_addr, &coreContext->phy_direction);
    if(0 != coreContext->phy_direction){
        printf("dsi: [%s] mipi read hang (0x%x)!\n", __FUNCTION__, coreContext->phy_direction);
        rtn = 0;
    }
    rtn = dsi_host_get_int0_status_ack_with_err_0(coreContext->host_base_addr, &coreContext->ack_with_err_0);
    rtn = dsi_host_get_int0_status_ack_with_err_8(coreContext->host_base_addr, &coreContext->ack_with_err_8);
    rtn = dsi_host_get_int0_status_ack_with_err_9(coreContext->host_base_addr, &coreContext->ack_with_err_9);
    rtn = dsi_host_get_int0_status_ack_with_err_10(coreContext->host_base_addr, &coreContext->ack_with_err_10);

    if((0 != coreContext->ack_with_err_0)
        | (0 != coreContext->ack_with_err_8)
        | (0 != coreContext->ack_with_err_9)
        | (0 != coreContext->ack_with_err_10)) {

        rtn = dsi_host_get_phy_direction(coreContext->host_base_addr, &coreContext->phy_direction);
        if(0 != coreContext->phy_direction){
            printf("dsi: [%s] mipi read hang (0x%x)!\n", __FUNCTION__, coreContext->phy_direction);
            rtn = 0;
        }
    }

    if(0 == rtn){
        printf("dsi: [%s] return error (%d)\n", __FUNCTION__, rtn);
        return -1;
    }

    EXIT:

    return rtn;
}

int32_t dsi_dcs_wr_cmd(host_core_t hostId, uint8_t* params, uint16_t param_length)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    uint8_t packet_type = 0;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();

    if (DSI_INITED != hostContext->status[hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_dcs_wr_cmd status(%d) ~_~!\n", hostContext->status[hostId]);
    }

    if (params == 0)
    {
        return RTN_DSI_OUT_OF_BOUND;
    }

    switch(param_length)
    {
        case 0:
            packet_type = 0x05; /* DCS short write no param */
            break;
        case 1:
        case 2:
            packet_type = 0x15; /* DCS short write 1 or 2 param */
            break;
        default:
            packet_type = 0x39; /* DCS long write/write_LUT command packet */
            break;
    }

    rtn = _dsi_gen_wr_packet(hostId, 0, packet_type, params, param_length);

    EXIT:

    return rtn;
}


int32_t dsi_dcs_rd_cmd(host_core_t hostId, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t *read_buffer)
{
    int32_t rtn = RTN_DSI_SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();
    dsi_host_core_t *coreContext = dsi_host_core_context_get(hostId);

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    if (DSI_INITED != hostContext->status[hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_dcs_rd_cmd status(%d) ~_~!\n", hostContext->status[hostId]);
    }

    rtn = _dsi_dcs_rd_cmd(hostId, 0, data_type, command, bytes_to_read, read_buffer);
    EXIT_IF_FAIL(0x00 == rtn, NULL, "_dsi_dcs_rd_cmd ~_~\n");

    rtn = dsi_host_get_phy_direction(coreContext->host_base_addr, &coreContext->phy_direction);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_phy_direction ~_~\n");

    rtn = dsi_host_get_int0_status(coreContext->host_base_addr, &coreContext->ack_with_err_0);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status ~_~\n");
    rtn = dsi_host_get_int0_status_ack_with_err_0(coreContext->host_base_addr, &coreContext->ack_with_err_0);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_0 ~_~\n");
    rtn = dsi_host_get_int0_status_ack_with_err_8(coreContext->host_base_addr, &coreContext->ack_with_err_8);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_8 ~_~\n");
    rtn = dsi_host_get_int0_status_ack_with_err_9(coreContext->host_base_addr, &coreContext->ack_with_err_9);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_9 ~_~\n");
    rtn = dsi_host_get_int0_status_ack_with_err_10(coreContext->host_base_addr, &coreContext->ack_with_err_10);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_10 ~_~\n");

    if(0 != coreContext->phy_direction){
        printf("%s: line %d mipi read hang (0x%x)!\n", __FUNCTION__, __LINE__, coreContext->phy_direction);
        rtn |= RTN_DSI_INVALID_IO;
    }

    if((0 != coreContext->ack_with_err_0)
        | (0 != coreContext->ack_with_err_8)
        | (0 != coreContext->ack_with_err_9)
        | (0 != coreContext->ack_with_err_10)) {
        rtn = dsi_host_get_phy_direction(coreContext->host_base_addr, &coreContext->phy_direction);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_phy_direction ~_~\n");

        dsi0_mdelay(10);

        rtn = dsi_host_get_int0_status(coreContext->host_base_addr, &coreContext->int0_status);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status ~_~\n");
        rtn = dsi_host_get_int0_status_ack_with_err_0(coreContext->host_base_addr, &coreContext->ack_with_err_0);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_0 ~_~\n");
        rtn = dsi_host_get_int0_status_ack_with_err_8(coreContext->host_base_addr, &coreContext->ack_with_err_8);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_8 ~_~\n");
        rtn = dsi_host_get_int0_status_ack_with_err_9(coreContext->host_base_addr, &coreContext->ack_with_err_9);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_9 ~_~\n");
        rtn = dsi_host_get_int0_status_ack_with_err_10(coreContext->host_base_addr, &coreContext->ack_with_err_10);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_get_int0_status_ack_with_err_10 ~_~\n");

        if(0 != coreContext->phy_direction){
            printf("%s: line %d mipi read hang (0x%x)!\n", __FUNCTION__, __LINE__, coreContext->phy_direction);
            rtn |= RTN_DSI_INVALID_IO;
        }

        if((0 != coreContext->ack_with_err_0)
            | (0 != coreContext->ack_with_err_8)
            | (0 != coreContext->ack_with_err_9)
            | (0 != coreContext->ack_with_err_10)) {
            printf("%s: line %d mipi read status error!\n", __FUNCTION__, __LINE__);
            rtn |= RTN_DSI_INVALID_IO;
        }
    }

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_host_v0_init(dsi_host_init_in_t *inParam, dsi_host_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "dsi_host_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_init_outparam_t is null ~_~\n");
    CHECK_PTR(hostContext, NULL, "hostContext is null ~_~\n");

    if (DSI_INITED == hostContext->status[inParam->hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_v0_init status(%d) ~_~!\n", hostContext->status[inParam->hostId]);
    }

    rtn = _dsi_host_v0_init_param_checker(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_v0_init_param_checker ~_~\n");

    rtn = _dsi_host_v0_init_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_v0_init_param_set ~_~\n");

    rtn = _dsi_host_v0_cfg(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "dsi_host_v0_cfg ~_~\n");

    hostContext->status[inParam->hostId] = DSI_INITED;

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_host_v0_deinit(dsi_host_deinit_in_t *inParam, dsi_host_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");
    CHECK_PTR(hostContext, NULL, "hostContext is null ~_~\n");

    if (DSI_UN_INITED == hostContext->status[inParam->hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_host%d_v0_deinit status(%d) ~_~!\n", inParam->hostId, hostContext->status[inParam->hostId]);
    }

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_host_hs_mode(host_core_t hostId)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    if (DSI_INITED != hostContext->status[hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_hs_mode status(%d) ~_~!\n", hostContext->status[hostId]);
    }

    rtn = _dsi_host_hs_mode_param_cfg(hostId);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_hs_mode_param_cfg ~_~\n");

    rtn = _dsi_host_bus_mode_cfg(hostId);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_bus_mode_cfg ~_~\n");

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_host_lp_mode(host_core_t hostId)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    if (DSI_INITED != hostContext->status[hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_lp_mode status(%d) ~_~!\n", hostContext->status[hostId]);
    }

    rtn = _dsi_host_lp_mode_param_cfg(hostId);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_lp_mode_param_cfg ~_~\n");

    rtn = _dsi_host_bus_mode_cfg(hostId);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_bus_mode_cfg ~_~\n");

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_host_power_on(dsi_host_power_on_in_t *inParam, dsi_host_power_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();
    dsi_host_core_t *coreContext = NULL;

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_power_on_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_on_out_t is null ~_~\n");
    CHECK_PTR(hostContext, NULL, "hostContext is null ~_~\n");

    if (DSI_POWER_OFF != hostContext->status[inParam->hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_power_on status(%d) ~_~!\n", hostContext->status[inParam->hostId]);
    }

    coreContext = dsi_host_core_context_get(inParam->hostId);

    rtn = _dsi_host_v0_power_on_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_v0_power_on_param_set ~_~!\n");

    #if 0
    if (0x01 == hostContext->power.power_cnt) {
        rtn = dsi_host_clk_on_reset(coreContext->clk_base_addr, coreContext->rst_base_addr); // for mipi host clk independent cotral
    }
    #endif

    /*Figure 3-3 D-PHY Initialization Sequence*/
    /*set to 1, this bit enables the D-PHY Clock Lane Module.*/
    dsi_host_set_phy_enableclk(coreContext->host_base_addr, 0);
    /*PHY test interface clear :set 1*/
    dsi_host_set_phy_testclr(coreContext->host_base_addr, 1);

    dsi_host_set_phy_forcepll(coreContext->host_base_addr, 1);

    //RSTZCAL down
    //dsi_phy_set_racl_en(inParam->phy_addr, 1);

    /*This bit is used to clock the TESTDIN bus into the D-PHY.*/
    dsi_host_set_phy_testclk(coreContext->host_base_addr, 1);
    /*PHY test interface operation selector:rising 0,failing 1*/
    dsi_host_set_phy_testen(coreContext->host_base_addr, 0);
    /*this bit places the complete D-PHY macro in  power-down state.0:power_down 1:power up*/
    dsi_host_set_phy_shutdownz(coreContext->host_base_addr, 1);
    /*this bit places the digital section of the D-PHY in the  reset state.0: reset*/
    dsi_host_set_phy_rstz(coreContext->host_base_addr, 1);
    /*PHY test interface clear: set 0 */
    dsi_host_set_phy_testclr(coreContext->host_base_addr, 0);

    //RSTZCAL up
    //dsi_phy_set_racl_en(inParam->phy_addr, 1);

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_host_power_off(dsi_host_power_off_in_t *inParam, dsi_host_power_off_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_host_mpi_context_t *hostContext = dsi_host_mipi_context_get();
    dsi_host_core_t *coreContext = NULL;

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "dsi_host_power_off_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_host_power_off_out_t is null ~_~\n");
    CHECK_PTR(hostContext, NULL, "dsi_host_power_off_out_t is null ~_~\n");

    if (DSI_UN_INITED == hostContext->status[inParam->hostId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_power_off status(%d) ~_~!\n", hostContext->status[inParam->hostId]);
    }

    coreContext = dsi_host_core_context_get(inParam->hostId);

    rtn = _dsi_host_v0_power_off_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_host_v0_power_off_param_set ~_~!\n");

    #if 0
    if (0x01 == hostContext->power.power_cnt) {
        rtn = dsi_host_clk_off(coreContext->clk_base_addr);
        EXIT_IF_FAIL(rtn, NULL, "dsi_host_clk_off ~_~!\n");
    }
    #endif

    memset(coreContext, 0x00, sizeof(dsi_host_core_t));
    hostContext->status[inParam->hostId] = DSI_POWER_OFF;

    EXIT:

    #if DSI_HOST_MPI_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

#ifdef __cplusplus
}
#endif
