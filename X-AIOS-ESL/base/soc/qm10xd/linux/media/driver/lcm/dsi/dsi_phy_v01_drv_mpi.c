#include "os.h"
#include "dsi_phy_v01_drv_mpi_param.h"
#include "dsi_phy_v01_drv_param.h"
#include "dsi_phy_v01_drv.h"
#include "vou_clk_v01_drv.h"
#include "dsi_phy_v01_reg_def.h"
#include "dsi_timing_calc.h"
#include "dsi_phy_v0_pll_calc.h"
#include "dsi_com.h"
#include "reg.h"

#include "reg_pool.h"
#include "dsi_phy_v01_reg_pool.h"
#include "vou_clk_v01_drv.h"
#include "debug.h"
#include "fy_osal.h"

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
#define DSI_PHY_PARAM_DEBUG 0
#define DSI_PHY_FUN_DEBUG 0

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef enum {
    PHY_1C2D = 0,
    PHY_1C4D,
    PHY_MODE_MAX
}phy_mode_e;


typedef enum {
    RESET_STATUS0 = 0,
    RESET_STATUS1,
    RESET_STATUS2,
    PHY_RESET_STATUS_MAX
}phy_reset_status_t;

typedef enum {
    PHY_LINK_1C2D = 0,
    PHY_LINK_1C4D,
    PHY_LINK_MODE_MAX
}phy_link_mode_t;

typedef enum {
    clane0 = 0,
    clane1,
    PHY_CLANE_MAX
}phy_c_lane_t;

typedef enum {
    dlane0 = 0,
    dlane1,
    dlane2,
    dlane3,
    PHY_DLANE_MAX
}phy_d_lane_t;

typedef enum {
    lane0 = 0,
    lane1,
    lane2,
    lane3,
    lane4,
    lane5,
    PHY_LANE_MAX
}phy_lane_t;

typedef enum {
    PLL_UNLOCKED = 0,
    PLL_LOCKED,
    PLL_LOCKED_STATUS_MAX
}pll_lock_status_t;

typedef struct {
    uint32_t lpx;
    uint32_t txreadyesc_mode;
    uint32_t auto_ckgate_en;
    uint32_t phy_link_mode;
    uint32_t txreadyesc_extend_vld;
    uint32_t rxvalidesc_extend_vld;
    uint32_t afifo_info; // RO
    uint32_t afifo_cfg;
    uint32_t rcal_pluse_period;
    uint32_t t_ipcd_det_time_d0;
    uint32_t en_deskew;
    uint32_t en_period_deskew;
    uint32_t init_deskew_time;
    uint32_t period_deskew_time;
    uint32_t cl_en_deskew;
    uint32_t cl_init_deskew_done; // RO
    uint32_t cl_deskew_time;
    uint32_t su_iddq_en_soft;
    uint32_t atest_en;
    uint32_t atest_sel;
    uint32_t fset_en;
    uint32_t bg_en;
    uint32_t bg_rbias;
    uint32_t bg_trim;
    uint32_t vbg_rdy; //RO
    uint32_t bist_en;
    uint32_t bist_sel;
    uint32_t bist_bit_error;
    uint32_t bist_out; //RO
    uint32_t rcal_trim_init_time;
    uint32_t rcal_sel;
    uint32_t rcal_pn_swap_i;
    uint32_t rcal_en_i;
    uint32_t rcal_soft_rst;
    uint32_t rcal_trim_soft;
    uint32_t rcal_comp_out; //RO
    uint32_t rcal_done; //RO
    uint32_t rcal_ctrl;
    uint32_t ft_sel;
    uint32_t lpcd_vref_trim;
    uint32_t lprx_vref_trim;
    uint32_t rg_ibias_sel;
    uint32_t rcal_done_bypass;
}phy_com_t;

typedef struct {
    uint32_t rst2enlptx[PHY_CLANE_MAX];
    uint32_t inittime[PHY_CLANE_MAX];
    uint32_t clkprepare[PHY_CLANE_MAX];
    uint32_t clkzero[PHY_CLANE_MAX];
    uint32_t clkpre[PHY_CLANE_MAX];
    uint32_t clkpost[PHY_CLANE_MAX];
    uint32_t clktrail[PHY_CLANE_MAX];
    uint32_t hsexit[PHY_CLANE_MAX];
    uint32_t pulldwn_mode[PHY_CLANE_MAX];
    uint32_t wakeup[PHY_CLANE_MAX];
}c_lane_t;

typedef struct {
    uint32_t rst2enlptx[PHY_DLANE_MAX];
    uint32_t inittime[PHY_DLANE_MAX];
    uint32_t hsprepare[PHY_DLANE_MAX];
    uint32_t hszero[PHY_DLANE_MAX];
    uint32_t hstrail[PHY_DLANE_MAX];
    uint32_t hsexit[PHY_DLANE_MAX];
    uint32_t wakeup[PHY_DLANE_MAX];
    uint32_t tago[PHY_DLANE_MAX];
    uint32_t tasure[PHY_DLANE_MAX];
    uint32_t taget[PHY_DLANE_MAX];
    uint32_t pulldwn_mode[PHY_DLANE_MAX];
    uint32_t init_deskew_done[PHY_DLANE_MAX]; //RO
    uint32_t period_deskew_done[PHY_DLANE_MAX]; //RO
}d_lane_t;

typedef struct {
    uint32_t ck8t_reverse[PHY_LANE_MAX];
    uint32_t predrv_by_dly[PHY_LANE_MAX];
    uint32_t predrv_dly_sel[PHY_LANE_MAX];
    uint32_t hstx_phase[PHY_LANE_MAX];
    uint32_t hs_8t_clk_sel[PHY_LANE_MAX];
    uint32_t lptx_vb_sel[PHY_LANE_MAX];
    uint32_t bist_done[PHY_LANE_MAX]; //RO
    uint32_t bist_ok[PHY_LANE_MAX]; //RO
    uint32_t bist_pattern_mode[PHY_LANE_MAX];
    uint32_t prbs_check_num[PHY_LANE_MAX];
    uint32_t prbs_seed[PHY_LANE_MAX];
    uint32_t prbs_err_th[PHY_LANE_MAX];
    uint32_t prbs_err_num[PHY_LANE_MAX]; //RO
}lane_t;

typedef struct {
    uint32_t dll_sel[PHY_CORE_MAX];
    uint32_t lane_num[PHY_CORE_MAX];
    uint32_t clksel[PHY_CORE_MAX];
    uint32_t phyerr_mask[PHY_CORE_MAX];
    uint32_t ulprx_sel[PHY_CORE_MAX];
    uint32_t dphy_soft_rst[PHY_CORE_MAX];
    uint32_t pwon_sel[PHY_CORE_MAX];
    uint32_t pwon_dsi_soft[PHY_RESET_STATUS_MAX][PHY_CORE_MAX];
    uint32_t pwon_pll_soft[PHY_RESET_STATUS_MAX][PHY_CORE_MAX];
    uint32_t da_clk_en[PHY_CORE_MAX];
    uint32_t pixelclk_div[PHY_CORE_MAX];
    uint32_t pll_soft_rst[PHY_RESET_STATUS_MAX][PHY_CORE_MAX];
    uint32_t pll_bist_en[PHY_CORE_MAX];
    uint32_t pll_bist_ctrl[PHY_CORE_MAX];
    uint32_t pll_bist_cnt[PHY_CORE_MAX];
    uint32_t pll_div_s[PHY_CORE_MAX];
    uint32_t pll_freq_doubler_en[PHY_CORE_MAX];
    uint32_t pll_ref24m_sel[PHY_CORE_MAX];
    uint32_t pll_pdiv[PHY_CORE_MAX];
    uint32_t pll_ssc_en[PHY_CORE_MAX];
    uint32_t pll_ssc_ctrl[PHY_CORE_MAX];
    uint32_t pll_lpf_r2[PHY_CORE_MAX];
    uint32_t pll_lpf_r3[PHY_CORE_MAX];
    uint32_t pll_lpf_c1[PHY_CORE_MAX];
    uint32_t pll_lpf_c2[PHY_CORE_MAX];
    uint32_t pll_ldo_test_en[PHY_CORE_MAX];
    uint32_t pll_ldo_vref[PHY_CORE_MAX];
    uint32_t pll_ldo_vout[PHY_CORE_MAX];
    uint32_t pll_kint[PHY_CORE_MAX];
    uint32_t pll_nint[PHY_CORE_MAX];
    uint32_t pll_n[PHY_CORE_MAX];
    uint32_t pll_icp[PHY_CORE_MAX];
    uint32_t pll_icp_offset[PHY_CORE_MAX];
    uint32_t pll_lock[PHY_CORE_MAX]; //RO
    uint32_t rg_dslpll_ssc_step_wicith_ctrl[PHY_CORE_MAX];
    uint32_t rg_dslpll_ssc_step_number_ctrl[PHY_CORE_MAX];
    uint32_t pll_lock_bypass[PHY_CORE_MAX];
    uint32_t pll_lock_time[PHY_CORE_MAX];
    uint32_t pll_lock_cnt[PHY_CORE_MAX];
    uint32_t hstx_sr[PHY_CORE_MAX];
    uint32_t hstx_amp_trim[PHY_CORE_MAX];
    uint32_t lptx_sr_trim[PHY_CORE_MAX];
    uint32_t lptx_amp_trim[PHY_CORE_MAX];
    uint32_t en_lpcd[PHY_CORE_MAX];
    uint32_t en_lprx[PHY_CORE_MAX];
    uint32_t en_ulprx[PHY_CORE_MAX];
    uint32_t lpcd_fl0[PHY_CORE_MAX];
    uint32_t lpcd_fl1[PHY_CORE_MAX];
    uint32_t lprx_vinit[PHY_CORE_MAX]; //RO
    uint32_t lpcd_vinit[PHY_CORE_MAX]; //RO
}core_t;

typedef struct {
    uint32_t ext_loop_mode_cfg[PHY_MODE_MAX];
    uint32_t en_ext_loop_cfg[PHY_MODE_MAX];
    uint32_t txclkreq_loop_cfg[PHY_MODE_MAX];
    uint32_t txdatareq_loop_cfg[PHY_MODE_MAX];
    uint32_t txdatasel_loop_cfg[PHY_MODE_MAX];
    uint32_t txdata_loop_cfg[PHY_MODE_MAX];
    uint32_t txdatanum_loop_cfg[PHY_MODE_MAX];
    uint32_t ext_loop_bist_bit_error_cfg[PHY_MODE_MAX];
    uint32_t ext_loop_prbs_pattern_mode_cfg[PHY_MODE_MAX];
    uint32_t ext_loop_prbs_seed_cfg[PHY_MODE_MAX];
}phy_mode_t;

typedef struct {
    uint32_t dphy_debug[22];
    uint32_t dphy_debug_cfg_reg[33];
    uint32_t dphy_status_info_reg[4];
}debug_reg_t;

typedef struct {
    frameSize_t total;
    frameSize_t frame;
    uint32_t fromat;
    uint32_t fps;
    uint32_t lane_num;
}frame_info_t;

typedef struct {
    int32_t power_on_cnt[PHY_CORE_MAX];
    int32_t power_off_cnt[PHY_CORE_MAX];
    int32_t power_cur_cnt[PHY_CORE_MAX];
    int32_t power_cnt;
}dsi_phy_power_t;

typedef struct {
    ADDR clk_base_addr;
    ADDR rst_base_addr;
    ADDR phy_base_addr;
    ADDR vou_base_addr;
    dsi_status_e status[PHY_CORE_MAX];
    uint32_t clkStatus;
    frame_info_t frameInfo[PHY_CORE_MAX];
    dsi_phy_work_mode_e phyWorkMode[PHY_CORE_MAX];
    uint32_t dhd_div[PHY_CORE_MAX];
    uint32_t lane_clk[PHY_CORE_MAX];

    dsi_phy_power_t power;
    phy_com_t com;
    c_lane_t clane;
    d_lane_t dlane;
    lane_t lane;
    core_t core;
    phy_mode_t mode;
    debug_reg_t debugReg;

    //os_mutex_t phy_clk_mutex;
}dsi_phy_mpi_context_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
dsi_phy_mpi_context_t s_dsi_phy_mpi_context = {0x00};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
dsi_phy_mpi_context_t *dsi_phy_mipi_context_get(void)
{
    return &s_dsi_phy_mpi_context;
}

static uint32_t _get_dlane_base_id(uint32_t phyId)
{
    return (PHY0 == phyId) ? 0 : 2;
}

static uint32_t _get_lane_base_id(uint32_t phyId)
{
    return (PHY0 == phyId) ? 0 : 3;
}

static phy_link_mode_t _get_phy_link_mode(dsi_phy_work_mode_e phyWorkMode)
{
    phy_link_mode_t phyLinkMode = DSI_PHY_WORK_NONE;

    if (DSI_PHY0_WORK_1C4D == phyWorkMode) {
        phyLinkMode = PHY_LINK_1C4D;
    } else {
        phyLinkMode = PHY_LINK_1C2D;
    }

    return phyLinkMode;
}

#if DSI_PHY_PARAM_DEBUG
static int32_t _dsi_phy_v0_param_show(    uint32_t phyId)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    frame_info_t *frameInfo = NULL;
    dsi_phy_power_t *power = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    lane_t *lane = NULL;
    core_t *core = NULL;
    phy_mode_t *mode = NULL;
    uint32_t laneId = 0x00;
    uint32_t laneBaseId = 0x00;
    uint32_t laneNum = 0x00;

    frameInfo = &dsiPhyContext->frameInfo[phyId];
    power = &dsiPhyContext->power;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    lane = &dsiPhyContext->lane;
    core = &dsiPhyContext->core;
    mode = &dsiPhyContext->mode;

    printf("\n----------dsi txPhy%d param---\n", phyId);
    printf("dsiPhyContext->phyWorkMode[%d]  : %d\n", phyId, dsiPhyContext->phyWorkMode[phyId]);
    printf("\n----------pll-----------------\n");
    printf("dsiPhyContext->phy_base_addr  : 0x%08x\n", dsiPhyContext->phy_base_addr        );
    printf("frameInfo->fromat         : %d\n", frameInfo->fromat               );
    printf("frameInfo->fps            : %d\n", frameInfo->fps                  );
    printf("frameInfo->lane_num       : %d\n", frameInfo->lane_num             );
    printf("dsiPhyContext->laneclk[%d]: %d\n", phyId, dsiPhyContext->lane_clk[phyId]);
    printf("core->pll_freq_doubler_en : %d\n", core->pll_freq_doubler_en[phyId]);
    printf("core->pll_div_s           : %d\n", core->pll_div_s[phyId]          );
    printf("core->pll_n               : %d\n", core->pll_n[phyId]              );
    printf("core->pll_kint            : %d\n", core->pll_kint[phyId]           );
    printf("core->pll_nint            : %d\n", core->pll_nint[phyId]           );
    printf("core->pixelclk_div        : %d\n", core->pixelclk_div[phyId]      );
    printf("core->pll_pdiv            : %d\n", core->pll_pdiv[phyId]           );
    printf("dsiPhyContext->dhd_div    : %d\n", dsiPhyContext->dhd_div[phyId]   );
    printf("\n--------clane-----------------\n");
    printf("clane->rst2enlptx         : %d\n", clane->rst2enlptx[phyId]        );
    printf("clane->inittime           : %d\n", clane->inittime[phyId]          );
    printf("clane->clkprepare         : %d\n", clane->clkprepare[phyId]        );
    printf("clane->clkzero            : %d\n", clane->clkzero[phyId]           );
    printf("clane->clkpre             : %d\n", clane->clkpre[phyId]            );
    printf("clane->clkpost            : %d\n", clane->clkpost[phyId]           );
    printf("clane->clktrail           : %d\n", clane->clktrail[phyId]          );
    printf("clane->hsexit             : %d\n", clane->hsexit[phyId]            );
    printf("clane->pulldwn_mode       : %d\n", clane->pulldwn_mode[phyId]      );
    printf("clane->wakeup             : %d\n", clane->wakeup[phyId]            );

    laneBaseId = _get_dlane_base_id(phyId);
    laneNum = laneBaseId + frameInfo->lane_num + 1;
    printf("\n--------dlane-----------------\n");
    for (laneId = laneBaseId; laneId < laneNum; laneId++) {
        printf("dlane->rst2enlptx[%d]         : %d\n", laneId, dlane->rst2enlptx[laneId]        );
        printf("dlane->inittime[%d]           : %d\n", laneId, dlane->inittime[laneId]          );
        printf("dlane->hsprepare[%d]          : %d\n", laneId, dlane->hsprepare[laneId]         );
        printf("dlane->hszero[%d]             : %d\n", laneId, dlane->hszero[laneId]            );
        printf("dlane->hstrail[%d]            : %d\n", laneId, dlane->hstrail[laneId]           );
        printf("dlane->hsexit[%d]             : %d\n", laneId, dlane->hsexit[laneId]            );
        printf("dlane->wakeup[%d]             : %d\n", laneId, dlane->wakeup[laneId]            );
        printf("dlane->tago[%d]               : %d\n", laneId, dlane->tago[laneId]              );
        printf("dlane->tasure[%d]             : %d\n", laneId, dlane->tasure[laneId]            );
        printf("dlane->taget[%d]              : %d\n", laneId, dlane->taget[laneId]             );
        printf("dlane->pulldwn_mode[%d]       : %d\n", laneId, dlane->pulldwn_mode[laneId]      );
        printf("dlane->init_deskew_done[%d]   : %d\n", laneId, dlane->init_deskew_done[laneId]    );
        printf("dlane->period_deskew_done[%d] : %d\n", laneId, dlane->period_deskew_done[laneId]);
    }

    laneBaseId = _get_lane_base_id(phyId);
    laneNum = laneBaseId + frameInfo->lane_num + 1;
    printf("\n---------lane-----------------\n");
    for (laneId = laneBaseId; laneId < laneNum; laneId++) {
        printf("lane->ck8t_reverse[%d]     : %d\n", laneId, lane->ck8t_reverse[laneId]     );
        printf("lane->predrv_by_dly[%d]    : %d\n", laneId, lane->predrv_by_dly[laneId]    );
        printf("lane->predrv_dly_sel[%d]   : %d\n", laneId, lane->predrv_dly_sel[laneId]   );
        printf("lane->hstx_phase[%d]       : %d\n", laneId, lane->hstx_phase[laneId]       );
        printf("lane->hs_8t_clk_sel[%d]    : %d\n", laneId, lane->hs_8t_clk_sel[laneId]    );
        printf("lane->lptx_vb_sel[%d]      : %d\n", laneId, lane->lptx_vb_sel[laneId]      );
        printf("lane->bist_done[%d]        : %d\n", laneId, lane->bist_done[laneId]        );
        printf("lane->bist_ok[%d]          : %d\n", laneId, lane->bist_ok[laneId]          );
        printf("lane->bist_pattern_mode[%d]: %d\n", laneId, lane->bist_pattern_mode[laneId]);
        printf("lane->prbs_check_num[%d]   : %d\n", laneId, lane->prbs_check_num[laneId]   );
        printf("lane->prbs_seed[%d]        : %d\n", laneId, lane->prbs_seed[laneId]        );
        printf("lane->prbs_err_th[%d]      : %d\n", laneId, lane->prbs_err_th[laneId]      );
        printf("lane->prbs_err_num[%d]     : %d\n", laneId, lane->prbs_err_num[laneId]     );
    }

    printf("\n---------core-----------------\n");
    printf("core[%d]->dll_sel                        : %d\n", phyId, core->dll_sel[phyId]                       );
    printf("core[%d]->lane_num                       : %d\n", phyId, core->lane_num[phyId]                      );
    printf("core[%d]->clksel                         : %d\n", phyId, core->clksel[phyId]                        );
    printf("core[%d]->phyerr_mask                    : %d\n", phyId, core->phyerr_mask[phyId]                   );
    printf("core[%d]->ulprx_sel                      : %d\n", phyId, core->ulprx_sel[phyId]                     );
    printf("core[%d]->dphy_soft_rst                  : %d\n", phyId, core->dphy_soft_rst[phyId]                 );
    printf("core[%d]->pwon_sel                       : %d\n", phyId, core->pwon_sel[phyId]                      );

    printf("core[0][0]->pwon_dsi_soft                  : %d\n", core->pwon_dsi_soft[RESET_STATUS0][PHY0]        );
    printf("core[0][0]->pwon_pll_soft                  : %d\n", core->pwon_pll_soft[RESET_STATUS0][PHY0]        );
    printf("core[0][0]->pll_soft_rst                   : %d\n", core->pll_soft_rst[RESET_STATUS0][PHY0]         );

    printf("core[0][1]->pwon_dsi_soft                  : %d\n", core->pwon_dsi_soft[RESET_STATUS0][PHY1]        );
    printf("core[0][1]->pwon_pll_soft                  : %d\n", core->pwon_pll_soft[RESET_STATUS0][PHY1]        );
    printf("core[0][1]->pll_soft_rst                   : %d\n", core->pll_soft_rst[RESET_STATUS0][PHY1]         );

    printf("core[1][0]->pwon_dsi_soft                  : %d\n", core->pwon_dsi_soft[RESET_STATUS1][PHY0]        );
    printf("core[1][0]->pwon_pll_soft                  : %d\n", core->pwon_pll_soft[RESET_STATUS1][PHY0]        );
    printf("core[1][0]->pll_soft_rst                   : %d\n", core->pll_soft_rst[RESET_STATUS1][PHY0]         );

    printf("core[1][1]->pwon_dsi_soft                  : %d\n", core->pwon_dsi_soft[RESET_STATUS1][PHY1]        );
    printf("core[1][1]->pwon_pll_soft                  : %d\n", core->pwon_pll_soft[RESET_STATUS1][PHY1]        );
    printf("core[1][1]->pll_soft_rst                   : %d\n", core->pll_soft_rst[RESET_STATUS1][PHY1]         );

    printf("core[%d]->da_clk_en                      : %d\n", PHY0, core->da_clk_en[PHY0]                     );
    printf("core[%d]->da_clk_en                      : %d\n", PHY1, core->da_clk_en[PHY1]                     );
    printf("core[%d]->pixelclk_div                   : %d\n", phyId, core->pixelclk_div[phyId]                  );
    printf("core[%d]->pll_bist_en                    : %d\n", phyId, core->pll_bist_en[phyId]                   );
    printf("core[%d]->pll_bist_ctrl                  : %d\n", phyId, core->pll_bist_ctrl[phyId]                 );
    printf("core[%d]->pll_bist_cnt                   : %d\n", phyId, core->pll_bist_cnt[phyId]                  );
    printf("core[%d]->pll_div_s                      : %d\n", phyId, core->pll_div_s[phyId]                     );
    printf("core[%d]->pll_freq_doubler_en            : %d\n", phyId, core->pll_freq_doubler_en[phyId]           );
    printf("core[%d]->pll_ref24m_sel                 : %d\n", phyId, core->pll_ref24m_sel[phyId]                );
    printf("core[%d]->pll_pdiv                       : %d\n", phyId, core->pll_pdiv[phyId]                      );
    printf("core[%d]->pll_ssc_en                     : %d\n", phyId, core->pll_ssc_en[phyId]                    );
    printf("core[%d]->pll_ssc_ctrl                   : %d\n", phyId, core->pll_ssc_ctrl[phyId]                  );
    printf("core[%d]->pll_lpf_r2                     : %d\n", phyId, core->pll_lpf_r2[phyId]                    );
    printf("core[%d]->pll_lpf_r3                     : %d\n", phyId, core->pll_lpf_r3[phyId]                    );
    printf("core[%d]->pll_lpf_c1                     : %d\n", phyId, core->pll_lpf_c1[phyId]                    );
    printf("core[%d]->pll_lpf_c2                     : %d\n", phyId, core->pll_lpf_c2[phyId]                    );
    printf("core[%d]->pll_ldo_test_en                : %d\n", phyId, core->pll_ldo_test_en[phyId]               );
    printf("core[%d]->pll_ldo_vref                   : %d\n", phyId, core->pll_ldo_vref[phyId]                  );
    printf("core[%d]->pll_ldo_vout                   : %d\n", phyId, core->pll_ldo_vout[phyId]                  );
    printf("core[%d]->pll_kint                       : %d\n", phyId, core->pll_kint[phyId]                      );
    printf("core[%d]->pll_nint                       : %d\n", phyId, core->pll_nint[phyId]                      );
    printf("core[%d]->pll_n                          : %d\n", phyId, core->pll_n[phyId]                         );
    printf("core[%d]->pll_icp                        : %d\n", phyId, core->pll_icp[phyId]                       );
    printf("core[%d]->pll_icp_offset                 : %d\n", phyId, core->pll_icp_offset[phyId]                );
    printf("core[%d]->pll_lock                       : %d\n", phyId, core->pll_lock[phyId]                      );
    printf("core[%d]->rg_dslpll_ssc_step_wicith_ctrl : %d\n", phyId, core->rg_dslpll_ssc_step_wicith_ctrl[phyId]);
    printf("core[%d]->rg_dslpll_ssc_step_number_ctrl : %d\n", phyId, core->rg_dslpll_ssc_step_number_ctrl[phyId]);
    printf("core[%d]->pll_lock_bypass                : %d\n", phyId, core->pll_lock_bypass[phyId]               );
    printf("core[%d]->pll_lock_time                  : %d\n", phyId, core->pll_lock_time[phyId]                 );
    printf("core[%d]->pll_lock_ent                   : %d\n", phyId, core->pll_lock_cnt[phyId]                  );
    printf("core[%d]->hstx_sr                        : %d\n", phyId, core->hstx_sr[phyId]                       );
    printf("core[%d]->hstx_amp_trim                  : %d\n", phyId, core->hstx_amp_trim[phyId]                 );
    printf("core[%d]->lptx_sr_trim                   : %d\n", phyId, core->lptx_sr_trim[phyId]                  );
    printf("core[%d]->lptx_amp_trim                  : %d\n", phyId, core->lptx_amp_trim[phyId]                 );
    printf("core[%d]->en_lpcd                        : %d\n", phyId, core->en_lpcd[phyId]                       );
    printf("core[%d]->en_lprx                        : %d\n", phyId, core->en_lprx[phyId]                       );
    printf("core[%d]->en_ulprx                       : %d\n", phyId, core->en_ulprx[phyId]                      );
    printf("core[%d]->lpcd_fl0                       : %d\n", phyId, core->lpcd_fl0[phyId]                      );
    printf("core[%d]->lpcd_fl0                       : %d\n", phyId, core->lpcd_fl0[phyId]                      );
    printf("core[%d]->lpcd_fl1                       : %d\n", phyId, core->lpcd_fl1[phyId]                      );
    printf("core[%d]->lprx_vinit                     : %d\n", phyId, core->lprx_vinit[phyId]                    );
    printf("core[%d]->lpcd_vinit                     : %d\n", phyId, core->lpcd_vinit[phyId]                    );
    printf("\n---------mode-----------------\n");
    printf("mode[%d]->ext_loop_mode_cfg              : %d\n", phyId, mode->ext_loop_mode_cfg[phyId]             );
    printf("mode[%d]->en_ext_loop_cfg                : %d\n", phyId, mode->en_ext_loop_cfg[phyId]               );
    printf("mode[%d]->txclkreq_loop_cfg              : %d\n", phyId, mode->txclkreq_loop_cfg[phyId]             );
    printf("mode[%d]->txdatareq_loop_cfg             : %d\n", phyId, mode->txdatareq_loop_cfg[phyId]            );
    printf("mode[%d]->txdatasel_loop_cfg             : %d\n", phyId, mode->txdatasel_loop_cfg[phyId]            );
    printf("mode[%d]->txdata_loop_cfg                : %d\n", phyId, mode->txdata_loop_cfg[phyId]               );
    printf("mode[%d]->txdatanum_loop_cfg             : %d\n", phyId, mode->txdatanum_loop_cfg[phyId]            );
    printf("mode[%d]->ext_loop_bist_bit_error_cfg    : %d\n", phyId, mode->ext_loop_bist_bit_error_cfg[phyId]   );
    printf("mode[%d]->ext_loop_prbs_pattern_mode_cfg : %d\n", phyId, mode->ext_loop_prbs_pattern_mode_cfg[phyId]);
    printf("mode[%d]->ext_loop_prbs_seed_cfg         : %d\n", phyId, mode->ext_loop_prbs_seed_cfg[phyId]        );
    printf("power->power_on_cnt                      : %d\n", power->power_on_cnt[phyId]                               );
    printf("power->power_off_cnt                     : %d\n", power->power_off_cnt[phyId]                              );
    printf("power->power_cnt                         : %d\n", power->power_cnt                                  );

    return rtn;
}

static int32_t _dsi_phy_pll_v1_param_show(    uint32_t phyId)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    phy_com_t *com = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    core_t *core = NULL;

    com = &dsiPhyContext->com;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    core = &dsiPhyContext->core;

    printf("\n-----------------DHD%d--------------------\n", phyId);
    printf("com->bg_en                        :%d\n", com->bg_en);
    printf("com->bg_trim                      :%d\n", com->bg_trim);
    printf("com->su_iddq_en_soft              :%d\n", com->su_iddq_en_soft);
    printf("core->da_clk_en                   :%d\n", core->da_clk_en[phyId]);
    printf("core->pll_soft_rst[RESET_STATUS0] :%d\n", core->pll_soft_rst[RESET_STATUS0][phyId]);
    printf("clane->pulldwn_mode               :%d\n", clane->pulldwn_mode[phyId]);
    printf("dlane->pulldwn_mode               :%d\n", dlane->pulldwn_mode[phyId]);
    printf("core->en_lpcd                     :%d\n", core->en_lpcd[phyId]);
    printf("core->en_lprx                     :%d\n", core->en_lprx[phyId]);
    printf("core->en_ulprx                    :%d\n", core->en_ulprx[phyId]);
    printf("core->pwon_sel                    :%d\n", core->pwon_sel[phyId]);
    printf("core->pwon_dsi_soft[RESET_STATUS0]:%d\n", core->pwon_dsi_soft[RESET_STATUS0][phyId]);
    printf("core->pwon_pll_soft[RESET_STATUS0]:%d\n", core->pwon_pll_soft[RESET_STATUS0][phyId]);
    printf("core->pixelclk_div                :%d\n", core->pixelclk_div[phyId]);
    printf("core->pll_div_s                   :%d\n", core->pll_div_s[phyId]);
    printf("core->pll_freq_doubler_en         :%d\n", core->pll_freq_doubler_en[phyId]);
    printf("core->pll_n                       :%d\n", core->pll_n[phyId]);
    printf("core->pll_pdiv                    :%d\n", core->pll_pdiv[phyId]);
    printf("core->pll_kint                    :%d\n", core->pll_kint[phyId]);
    printf("core->pll_nint                    :%d\n", core->pll_nint[phyId]);
    printf("core->pll_soft_rst[RESET_STATUS1] :%d\n", core->pll_soft_rst[RESET_STATUS1][phyId]);
    printf("core->pwon_dsi_soft[RESET_STATUS1]:%d\n", core->pwon_dsi_soft[RESET_STATUS1][phyId]);
    printf("core->pwon_pll_soft[RESET_STATUS1]:%d\n", core->pwon_pll_soft[RESET_STATUS1][phyId]);

    return rtn;
}

#endif

static int32_t _dsi_phy_v0_init_param_checker(dsi_phy_init_in_t *inParam, dsi_phy_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");

    CHECK_PTR(inParam->timing, NULL, "dsi_phy_init_inparam_t timing is null ~_~\n");
    CHECK_PTR(inParam->dp_sync_info, NULL, "dsi_phy_init_inparam_t dp_sync_info is null ~_~\n");

    if (DSI_PHY0_WORK_1C4D == dsiPhyContext->phyWorkMode[PHY0]) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "dsi phy0 1c4d , phy%d laneNum:%d ~_~\n", inParam->phyId, inParam->laneNum);
    } else if((DSI_PHY0_WORK_1C2D == dsiPhyContext->phyWorkMode[PHY0])
            && (DSI_PHY1_WORK_1C2D == dsiPhyContext->phyWorkMode[PHY1])){
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "dsi phy0/1 1c2d , phy%d laneNum:%d ~_~\n", inParam->phyId, inParam->laneNum);
    }

    if(PHY0 == inParam->phyId) {
        if ((dsi_phy_lane_1 > inParam->laneNum)
            && (dsi_phy_lane_4 < inParam->laneNum)) {
            rtn = FAIL_INVALID;
            EXIT_IF_FAIL(rtn, NULL, "dsi phy%d laneNum:%d ~_~\n", inParam->phyId, inParam->laneNum);
        }
    } else {
        if ((dsi_phy_lane_1 > inParam->laneNum)
            && (dsi_phy_lane_2 < inParam->laneNum)) {
            rtn = FAIL_INVALID;
            EXIT_IF_FAIL(rtn, NULL, "dsi phy%d laneNum:%d ~_~\n", inParam->phyId, inParam->laneNum);
        }
    }

    if ((DSI_FORMAT_MAX <= inParam->format)) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi phy data format:%d ~_~\n", inParam->format);
    }

    if ((0 == inParam->fps)) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi phy fps:%d ~_~\n", inParam->fps);
    }

    if ((0 == inParam->laneNum)) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi phy laneNum:%d ~_~\n", inParam->laneNum);
    }

    if((0x00 == inParam->dp_sync_info->hor_sync_active)
        |(0x00 == inParam->dp_sync_info->hor_back_porch)
        |(0x00 == inParam->dp_sync_info->hor_active_pixel)
        |(0x00 == inParam->dp_sync_info->hor_front_porch)
        |(0x00 == inParam->dp_sync_info->ver_sync_active)
        |(0x00 == inParam->dp_sync_info->ver_back_porch)
        |(0x00 == inParam->dp_sync_info->ver_active_line)
        |(0x00 == inParam->dp_sync_info->ver_front_porch)) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi phy sync info ~_~\n");
    }

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_init_param_set(dsi_phy_init_in_t *inParam, dsi_phy_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    frame_info_t *frameInfo = NULL;
    phy_com_t *com = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    lane_t *lane = NULL;
    core_t *core = NULL;
    phy_mode_t *mode = NULL;
    uint32_t phyId = 0x00;
    uint32_t laneId = 0x00;
    uint32_t laneBaseId = 0x00;
    uint32_t laneNum = 0x00;
    uint32_t i = 0x00;
    uint32_t phyStartId = 0x00;
    uint32_t phyEndId = 0x00;
    dsi_phy_work_mode_e phyWorkMode = 0x00;

    dsi_phy_pll_calc_in_t pllCalcIn = {0x00};
    dsi_phy_pll_calc_out_t pllCalcOut = {0x00};
    dsi_phy_timing_in_t timingIn = {0x00};
    dsi_phy_timing_out_t timingOut = {0x00};

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");

    //os_mutex_init(&dsiPhyContext->phy_clk_mutex);

    phyId = inParam->phyId;
    frameInfo = &dsiPhyContext->frameInfo[phyId];
    com = &dsiPhyContext->com;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    lane = &dsiPhyContext->lane;
    core = &dsiPhyContext->core;
    mode = &dsiPhyContext->mode;

    phyWorkMode = get_phy_work_mode(phyId, inParam->laneNum);
    dsiPhyContext->phyWorkMode[inParam->phyId] = phyWorkMode;
    *outParam->phyMode = phyWorkMode;

    dsiPhyContext->phy_base_addr = inParam->phy_addr;
    dsiPhyContext->vou_base_addr = inParam->vou_addr;
    frameInfo->fromat = inParam->format;
    frameInfo->fps = inParam->fps;
    frameInfo->lane_num = inParam->laneNum;

    pllCalcIn.dp_sync = inParam->dp_sync_info;
    pllCalcIn.fps = inParam->fps;
    pllCalcIn.laneNum = inParam->laneNum;
    pllCalcIn.format = inParam->format;
    pllCalcIn.clk = &inParam->timing->clk.mif;
    pllCalcOut.host_sync_div = outParam->host_sync_div;

    rtn = dsi_phy_pll_cfg_get(&pllCalcIn, &pllCalcOut);

    if (DSI_PHY0_WORK_1C4D == phyWorkMode) {
        for(i = 0; i < 2; i++) {
            core->pll_freq_doubler_en[i] = pllCalcOut.pll_doubler;
            core->pll_div_s[i] = pllCalcOut.pll_div_s;
            core->pll_n[i] = pllCalcOut.pll_n;
            core->pll_kint[i] = pllCalcOut.pll_kint;
            core->pll_nint[i] = pllCalcOut.pll_nint;
            core->pixelclk_div[i] = pllCalcOut.pixelclk_div;
            core->pll_pdiv[i] = pllCalcOut.pll_pdiv;
        }
    } else {
        core->pll_freq_doubler_en[phyId] = pllCalcOut.pll_doubler;
        core->pll_div_s[phyId] = pllCalcOut.pll_div_s;
        core->pll_n[phyId] = pllCalcOut.pll_n;
        core->pll_kint[phyId] = pllCalcOut.pll_kint;
        core->pll_nint[phyId] = pllCalcOut.pll_nint;
        core->pixelclk_div[phyId] = pllCalcOut.pixelclk_div;
        core->pll_pdiv[phyId] = pllCalcOut.pll_pdiv;
    }

    dsiPhyContext->dhd_div[phyId] = pllCalcOut.dhd_div;
    dsiPhyContext->lane_clk[phyId] = pllCalcOut.lane_clk;

    timingIn.lane_clk = pllCalcOut.lane_clk;
    timingIn.dp_sync = inParam->dp_sync_info;
    timingIn.fps = inParam->fps;
    timingIn.format = inParam->format;
    timingIn.laneNum = inParam->laneNum;
    timingIn.timing = inParam->timing;
    rtn = dsi_phy_timing_calc(&timingIn, &timingOut);

    clane->rst2enlptx[phyId] = 0x01f4;
    clane->clkprepare[phyId] = inParam->timing->clane.prepare;
    clane->clkzero[phyId] = inParam->timing->clane.zero;
    clane->clkpre[phyId] = inParam->timing->clane.pre;
    clane->clkpost[phyId] = inParam->timing->clane.post;
    clane->clktrail[phyId] = inParam->timing->clane.trail;
    clane->inittime[phyId] = inParam->timing->clane.inittime; //0x1100; //1c2d
    clane->hsexit[phyId] = inParam->timing->clane.exit;
    clane->pulldwn_mode[phyId] = 0x00; //???
    clane->wakeup[PHY0] = 256; //???
    clane->wakeup[PHY1] = 256; //???

    laneBaseId = _get_dlane_base_id(phyId);
    laneNum = laneBaseId + inParam->laneNum + 1;
    for (laneId = laneBaseId; laneId < laneNum; laneId++) {
        dlane->rst2enlptx[laneId] = 0x6c;
        dlane->hsprepare[laneId] = inParam->timing->dlane.prepare;
        dlane->hszero[laneId] = inParam->timing->dlane.zero;
        dlane->hstrail[laneId] = inParam->timing->dlane.trail;
        dlane->inittime[laneId] = inParam->timing->dlane.inittime; //0x1100;
        dlane->hsexit[laneId] = inParam->timing->dlane.exit;
        dlane->wakeup[laneId] = 0xd50c; //???
        dlane->tago[laneId] = inParam->timing->read.tago;
        dlane->tasure[laneId] = inParam->timing->read.tasure;
        dlane->taget[laneId] = inParam->timing->read.taget;
        dlane->pulldwn_mode[laneId] = 0; //???
    }

    laneBaseId = _get_lane_base_id(phyId);
    for (laneId = laneBaseId; laneId < (inParam->laneNum + laneBaseId); laneId++) {
        lane->ck8t_reverse[laneId] = 0x00; //???
        lane->predrv_by_dly[laneId] = 0x00; //???
        lane->predrv_dly_sel[laneId] = 0x00; //???
        lane->hstx_phase[laneId] = 0x00; //???
        lane->hs_8t_clk_sel[laneId] = 0x00; //???
        lane->lptx_vb_sel[laneId] = 0x01; //???
        lane->bist_pattern_mode[laneId] = 0x00; //???
        lane->prbs_check_num[laneId] = 1000; //???
        lane->prbs_seed[laneId] = 0x0f; //???
        lane->prbs_err_th[laneId] = 0x01; //???
    }

    com->lpx = inParam->timing->dlane.lp01; // com use
    com->txreadyesc_mode = 0x01;
    com->auto_ckgate_en = 0x00;
    com->phy_link_mode = _get_phy_link_mode(phyWorkMode);

//    com->txreadyesc_extend_vld = 0x08;
    com->txreadyesc_extend_vld = com->lpx;
    com->rxvalidesc_extend_vld = 0x02;
    *outParam->phy_txreadyesc = com->lpx;

    com->afifo_cfg = 0x00;
    com->rcal_pluse_period = 10;
    com->t_ipcd_det_time_d0 = 20;
    com->en_deskew = 0x00;
    com->en_period_deskew = 0x00;
    com->init_deskew_time = 20000;
    com->period_deskew_time = 0x00;
    com->cl_en_deskew = 0x00;
    com->cl_init_deskew_done = 0x00;
    com->cl_deskew_time = 20000;
    com->su_iddq_en_soft = 0x00;
    com->atest_en = 0x00;
    com->atest_sel = 0x00;
    com->fset_en = 0x01;
    com->bg_en = 0x01;
    com->bg_rbias = 0x00;
    com->bg_trim = 0x07;
    com->bist_en = 0x00;
    com->bist_sel = 0x00;
    com->bist_bit_error = 0x00;

    com->rcal_trim_init_time = 20000;
    com->rcal_sel = 0x00;
    com->rcal_pn_swap_i = 0x01;
    com->rcal_en_i = 0x01;
    com->rcal_soft_rst = 0x00;
    com->rcal_trim_soft = 0x08;
    com->rcal_ctrl = 0x00;
    com->ft_sel = 0x00;
    com->lpcd_vref_trim = 0x02;
    com->lprx_vref_trim = 0x02;
    com->rg_ibias_sel = 0x01;
    com->rcal_done_bypass = 0x00;

    core->dll_sel[PHY0] = 0x00;
    core->dll_sel[PHY1] = 0x00;
    core->lane_num[phyId] = inParam->laneNum;

    if (PHY_LINK_1C4D == com->phy_link_mode) {
        core->clksel[0] = 0x00;
        core->clksel[1] = 0x01;
    } else {
        core->clksel[0] = 0x00;
        core->clksel[1] = 0x00;
    }

    core->phyerr_mask[PHY0] = 0x1f;
    core->phyerr_mask[PHY1] = 0x1f;
    core->ulprx_sel[phyId] = 0x00;
    core->dphy_soft_rst[phyId] = 0x00;
    core->pwon_sel[PHY0] = 0x00;
    core->pwon_sel[PHY1] = 0x00;


    if (DSI_PHY0_WORK_1C4D == dsiPhyContext->phyWorkMode[inParam->phyId]) {
        /*--RESET_STATUS0--*/
        core->pwon_dsi_soft[RESET_STATUS0][PHY0] = 0x00;
        core->pwon_pll_soft[RESET_STATUS0][PHY0] = 0x00;
        core->pll_soft_rst[RESET_STATUS0][PHY0] = 0x01;

        core->pwon_dsi_soft[RESET_STATUS0][PHY1] = 0x00;
        core->pwon_pll_soft[RESET_STATUS0][PHY1] = 0x00;
        core->pll_soft_rst[RESET_STATUS0][PHY1] = 0x01;

        /*--RESET_STATUS1--*/
        core->pwon_pll_soft[RESET_STATUS1][PHY0] = 0x01;
        core->pwon_dsi_soft[RESET_STATUS1][PHY0] = 0x01;
        core->pll_soft_rst[RESET_STATUS1][PHY0] = 0x00;

        core->pwon_pll_soft[RESET_STATUS1][PHY1] = 0x01;
        core->pwon_dsi_soft[RESET_STATUS1][PHY1] = 0x01;
        core->pll_soft_rst[RESET_STATUS1][PHY1] = 0x00;

        core->da_clk_en[PHY0] = 0x01;
        core->da_clk_en[PHY1] = 0x01;
    } else if (DSI_PHY0_WORK_1C2D == dsiPhyContext->phyWorkMode[inParam->phyId]) {
        /*--RESET_STATUS0--*/
        core->pwon_dsi_soft[RESET_STATUS0][PHY0] = 0x00;
        core->pwon_pll_soft[RESET_STATUS0][PHY0] = 0x00;
        core->pll_soft_rst[RESET_STATUS0][PHY0] = 0x01;

        /*--RESET_STATUS1--*/
        core->pwon_pll_soft[RESET_STATUS1][PHY0] = 0x01;
        core->pwon_dsi_soft[RESET_STATUS1][PHY0] = 0x01;
        core->pll_soft_rst[RESET_STATUS1][PHY0] = 0x00;

        core->da_clk_en[PHY0] = 0x01;
    } else if (DSI_PHY1_WORK_1C2D == dsiPhyContext->phyWorkMode[inParam->phyId]) {
        /*--RESET_STATUS0--*/
        core->pwon_dsi_soft[RESET_STATUS0][PHY1] = 0x00;
        core->pwon_pll_soft[RESET_STATUS0][PHY1] = 0x00;
        core->pll_soft_rst[RESET_STATUS0][PHY1] = 0x01;

        /*--RESET_STATUS1--*/
        core->pwon_pll_soft[RESET_STATUS1][PHY1] = 0x01;
        core->pwon_dsi_soft[RESET_STATUS1][PHY1] = 0x01;
        core->pll_soft_rst[RESET_STATUS1][PHY1] = 0x00;

        core->da_clk_en[PHY1] = 0x01;
    }

    core->pll_bist_en[phyId] = 0x00;
    core->pll_bist_ctrl[phyId] = 0x00;
    core->pll_bist_cnt[phyId] = 0x00;
    core->pll_ref24m_sel[phyId] = 0x00;
    core->pll_ssc_en[phyId] = 0x00;
    core->pll_ssc_ctrl[phyId] = 0x00;

    core->pll_lpf_r2[PHY0] = 0x02;
    core->pll_lpf_r3[PHY0] = 0x01;
    core->pll_lpf_c1[PHY0] = 0x02;
    core->pll_lpf_c2[PHY0] = 0x02;

    core->pll_lpf_r2[PHY1] = 0x02;
    core->pll_lpf_r3[PHY1] = 0x01;
    core->pll_lpf_c1[PHY1] = 0x02;
    core->pll_lpf_c2[PHY1] = 0x02;

    core->pll_ldo_test_en[phyId] = 0x00;
    core->pll_ldo_vref[PHY0] = 0x01;
    core->pll_ldo_vref[PHY1] = 0x01;

    core->pll_ldo_vout[PHY0] = 0x02;
    core->pll_ldo_vout[PHY1] = 0x02;

    core->pll_icp[PHY0] = 0x01;
    core->pll_icp[PHY1] = 0x01;
    core->pll_icp_offset[phyId] = 0x00;
    core->pll_lock[PHY0] = 0x00;
    core->pll_lock[PHY1] = 0x00;
    core->rg_dslpll_ssc_step_wicith_ctrl[phyId] = 0x00;
    core->rg_dslpll_ssc_step_number_ctrl[phyId] = 0x00;
    core->pll_lock_bypass[phyId] = 0x00;
    core->pll_lock_time[PHY0] = 100000;
    core->pll_lock_time[PHY1] = 100000;
    core->pll_lock_cnt[phyId] = 0x00;
    core->hstx_sr[phyId] = 0x00;

    if (DSI_PHY0_WORK_1C4D == phyWorkMode) {
        phyStartId = PHY0;
        phyEndId = PHY_CORE_MAX;
    } else if (DSI_PHY0_WORK_1C2D == phyWorkMode) {
        phyStartId = PHY0;
        phyEndId = PHY1;
    } else if (DSI_PHY1_WORK_1C2D == phyWorkMode){
        phyStartId = PHY0; //???
        phyEndId = PHY_CORE_MAX;
    }

    for (i = phyStartId; i < phyEndId; i++) {
        core->hstx_amp_trim[i] = 0x06;
        core->lptx_sr_trim[i] = 0x03;
        core->lptx_amp_trim[i] = 0x03;

        core->en_lpcd[i] = 0x01;
        core->en_lprx[i] = 0x01;
        core->en_ulprx[i] = 0x01;
    }

    core->lpcd_fl0[phyId] = 0x00;
    core->lpcd_fl1[phyId] = 0x00;

    mode->ext_loop_mode_cfg[phyId] = 0x00;
    mode->en_ext_loop_cfg[phyId] = 0x00;
    mode->txclkreq_loop_cfg[phyId] = 0x00;
    mode->txdatareq_loop_cfg[phyId] = 0x00;
    mode->txdatasel_loop_cfg[phyId] = 0x00;
    mode->txdata_loop_cfg[PHY0] = 0x55;
    mode->txdata_loop_cfg[PHY1] = 0x55;
    mode->txdatanum_loop_cfg[PHY0] = 0x03;
    mode->txdatanum_loop_cfg[PHY1] = 0x03;
    mode->ext_loop_bist_bit_error_cfg[phyId] = 0x00;
    mode->ext_loop_prbs_pattern_mode_cfg[phyId] = 0x00;
    mode->ext_loop_prbs_seed_cfg[PHY0] = 15;
    mode->ext_loop_prbs_seed_cfg[PHY1] = 15;

    #if DSI_PHY_PARAM_DEBUG
    _dsi_phy_v0_param_show(phyId);
    #endif

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_deinit_param_set(dsi_phy_deinit_in_t *inParam, dsi_phy_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    core_t *core = NULL;
    phy_com_t *com = NULL;

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");

    core = &dsiPhyContext->core;
    com = &dsiPhyContext->com;

    core->pwon_dsi_soft[RESET_STATUS0][PHY0] = 0x00;
    core->pwon_pll_soft[RESET_STATUS0][PHY0] = 0x00;
    core->pwon_dsi_soft[RESET_STATUS0][PHY1] = 0x00;
    core->pwon_pll_soft[RESET_STATUS0][PHY1] = 0x00;

    com->su_iddq_en_soft = 0x01;

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_power_on_param_set(dsi_phy_power_on_in_t *inParam, dsi_phy_power_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    dsi_phy_power_t *power = NULL;

    CHECK_PTR(inParam, NULL, "dsi_phy_power_on_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_on_out_t is null ~_~\n");

    power = &dsiPhyContext->power;

    dsiPhyContext->clk_base_addr = inParam->clk_base_addr;
    dsiPhyContext->rst_base_addr = inParam->rst_base_addr;
    dsiPhyContext->phy_base_addr = inParam->phy_base_addr;
    power->power_on_cnt[inParam->phyId]++;
    power->power_cur_cnt[inParam->phyId]++;
    power->power_cnt++;

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_power_off_param_set(dsi_phy_power_off_in_t *inParam, dsi_phy_power_off_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    dsi_phy_power_t *power = NULL;

    CHECK_PTR(inParam, NULL, "dsi_phy_power_off_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_off_out_t is null ~_~\n");

    power = &dsiPhyContext->power;

    dsiPhyContext->phy_base_addr = inParam->phy_base_addr;
    power->power_off_cnt[inParam->phyId]++;
    power->power_cur_cnt[inParam->phyId]--;
    power->power_cnt--;

    if (0 > power->power_cur_cnt[inParam->phyId]) {
        printf("phy power[%d]->power_on_cnt                      : %d\n", inParam->phyId, power->power_on_cnt[inParam->phyId]     );
        printf("phy power[%d]->power_off_cnt                     : %d\n", inParam->phyId, power->power_off_cnt[inParam->phyId]    );
        printf("phy power[%d]->power_cur_cnt                     : %d\n", inParam->phyId, power->power_cur_cnt[inParam->phyId]    );
        printf("phy power[%d]->power_cnt                         : %d\n", inParam->phyId, power->power_cnt        );
    }

    if (0 == power->power_cur_cnt[inParam->phyId]) {
        power->power_off_cnt[inParam->phyId] = 0x00;
        power->power_off_cnt[inParam->phyId] = 0x00;
        power->power_cur_cnt[inParam->phyId] = 0x00;
    }

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_pll_lock_checker(phy_core_e phyId)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    core_t *core = NULL;
    pll_lock_status_t pll_lock_status = PLL_UNLOCKED;

    #if DSI_PHY_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(dsiPhyContext, NULL, "dsiPhyContext is null ~_~\n");

    core = &dsiPhyContext->core;

    do {
        rtn = dsi_phy_get_core_pll_lock(dsiPhyContext->phy_base_addr, core->pll_lock);
        switch (dsiPhyContext->phyWorkMode[phyId])
        {
            case DSI_PHY0_WORK_1C2D:
            {
                if (1 == core->pll_lock[PHY0]){
                    pll_lock_status = PLL_LOCKED;
                }
            }
            break;
            case DSI_PHY0_WORK_1C4D:
            {
                if ((1 == core->pll_lock[PHY0])
                    &&(1 == core->pll_lock[PHY1])){
                    pll_lock_status = PLL_LOCKED;
                }
            }
            break;
            case DSI_PHY1_WORK_1C2D:
            {
                if (1 == core->pll_lock[PHY1]){
                    pll_lock_status = PLL_LOCKED;
                }
            }
            break;
            default:

            break;
        }
        if (PLL_LOCKED == pll_lock_status) {
            break;
        }
    }while(1);


    EXIT:

    #if DSI_PHY_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}


static int32_t _dsi_phy_v0_cfg(dsi_phy_init_in_t *inParam, dsi_phy_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    phy_com_t *com = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    lane_t *lane = NULL;
    core_t *core = NULL;
    phy_mode_t *mode = NULL;
    uint32_t phyId = 0x00;
    dsi_phy_work_mode_e phyWorkMode = DSI_PHY_WORK_NONE;

    #if DSI_PHY_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");
    CHECK_PTR(dsiPhyContext, NULL, "dsiPhyContext is null ~_~\n");

    phyId = inParam->phyId;
    com = &dsiPhyContext->com;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    lane = &dsiPhyContext->lane;
    core = &dsiPhyContext->core;
    mode = &dsiPhyContext->mode;
    phyWorkMode = dsiPhyContext->phyWorkMode[phyId];

    #if 0
    rtn = regDefaultGet(dsiPhyContext->phy_base_addr, dsi_phy_get_reg_default_tab());
    rtn = showRegValuePtr(dsi_phy_get_reg_default_tab(), "tx_phy");
    #endif

    rtn = dsi_phy_set_bg_en(dsiPhyContext->phy_base_addr, com->bg_en); //0x70 bit6
    rtn = dsi_phy_set_core_pwon_sel(dsiPhyContext->phy_base_addr, core->pwon_sel); //0x70 bit12-17
    rtn = dsi_phy_set_core_pwon_dsi_soft(dsiPhyContext->phy_base_addr, phyId, core->pwon_dsi_soft[RESET_STATUS0]);
    rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, phyId, core->pwon_pll_soft[RESET_STATUS0]);
    rtn = dsi_phy_set_su_iddq_en_soft(dsiPhyContext->phy_base_addr, com->su_iddq_en_soft); //0x70 bit11
    rtn = dsi_phy_set_rcal_trim_soft(dsiPhyContext->phy_base_addr, com->rcal_trim_soft);//0x88 bit5-2
    rtn = dsi_phy_set_rcal_pn_swap_i(dsiPhyContext->phy_base_addr, com->rcal_pn_swap_i);//0x88 bit8
    rtn = dsi_phy_set_rcal_en_i(dsiPhyContext->phy_base_addr, com->rcal_en_i);//0x88 bit7
    if (phyWorkMode == DSI_PHY0_WORK_1C4D) {
        rtn = dsi_phy_set_core_pll_soft_rst(dsiPhyContext->phy_base_addr, PHY0, core->pll_soft_rst[RESET_STATUS0]); //0x74/374 bit25
        rtn = dsi_phy_set_core_pll_soft_rst(dsiPhyContext->phy_base_addr, PHY1, core->pll_soft_rst[RESET_STATUS0]); //0x74/374 bit25
    } else {
        rtn = dsi_phy_set_core_pll_soft_rst(dsiPhyContext->phy_base_addr, phyId, core->pll_soft_rst[RESET_STATUS0]); //0x74/374 bit25
    }
    rtn = dsi_phy_set_core_hstx_amp_trim(dsiPhyContext->phy_base_addr, core->hstx_amp_trim); //0x8c bit16-14
    rtn = dsi_phy_set_core_lptx_sr_trim(dsiPhyContext->phy_base_addr, core->lptx_sr_trim); //0x8c bit11-13
    rtn = dsi_phy_set_core_lptx_amp_trim(dsiPhyContext->phy_base_addr, core->lptx_amp_trim); //0x8c bit10-8
    rtn = dsi_phy_set_link_mode(dsiPhyContext->phy_base_addr, com->phy_link_mode); //0x360 bit0
    rtn = dsi_phy_set_core_pixelclkl_div(dsiPhyContext->phy_base_addr, core->pixelclk_div);//0x74 bit29-26
    rtn = dsi_phy_set_core_pll_div_s(dsiPhyContext->phy_base_addr, core->pll_div_s); //0x78 bit28
    rtn = dsi_phy_set_core_pll_freq_doubler_en(dsiPhyContext->phy_base_addr, core->pll_freq_doubler_en); //0x78 bit27
    rtn = dsi_phy_set_core_pll_n(dsiPhyContext->phy_base_addr, core->pll_n); //0x80 bit24-14
    rtn = dsi_phy_set_core_pll_pdiv(dsiPhyContext->phy_base_addr, core->pll_pdiv); //0x78 bit25-23
    rtn = dsi_phy_set_core_pll_kint(dsiPhyContext->phy_base_addr, core->pll_kint); //0x7c bit31-0
    rtn = dsi_phy_set_core_pll_nint(dsiPhyContext->phy_base_addr, core->pll_nint);
    rtn = vou_dhd_clk_on(dsiPhyContext->vou_base_addr, phyId, dsiPhyContext->dhd_div[phyId]);

    rtn = dsi_phy_set_core_en_lpcd(dsiPhyContext->phy_base_addr, core->en_lpcd); //0x98 bit12-10
    rtn = dsi_phy_set_core_en_lprx(dsiPhyContext->phy_base_addr, core->en_lprx);
    rtn = dsi_phy_set_core_en_ulprx(dsiPhyContext->phy_base_addr, core->en_ulprx);

    //clk lane
    rtn = dsi_phy_set_clane_rst2enlptx(dsiPhyContext->phy_base_addr, clane->rst2enlptx);
    rtn = dsi_phy_set_clane_inittime(dsiPhyContext->phy_base_addr, clane->inittime);
    rtn = dsi_phy_set_clane_clkprepare(dsiPhyContext->phy_base_addr, clane->clkprepare);
    rtn = dsi_phy_set_clane_clkzero(dsiPhyContext->phy_base_addr, clane->clkzero);
    rtn = dsi_phy_set_clane_clkpre(dsiPhyContext->phy_base_addr, clane->clkpre);
    rtn = dsi_phy_set_clane_clkpost(dsiPhyContext->phy_base_addr, clane->clkpost);
    rtn = dsi_phy_set_clane_clktrail(dsiPhyContext->phy_base_addr, clane->clktrail);
    rtn = dsi_phy_set_clane_hsexit(dsiPhyContext->phy_base_addr, clane->hsexit);
    rtn = dsi_phy_set_clane_wakeup(dsiPhyContext->phy_base_addr, clane->wakeup);

    //data lane
    rtn = dsi_phy_set_dlane_rst2enlptx(dsiPhyContext->phy_base_addr, dlane->rst2enlptx);
    rtn = dsi_phy_set_dlane_inittime(dsiPhyContext->phy_base_addr, dlane->inittime);
    rtn = dsi_phy_set_dlane_hsprepare(dsiPhyContext->phy_base_addr, dlane->hsprepare);
    rtn = dsi_phy_set_dlane_hszero(dsiPhyContext->phy_base_addr, dlane->hszero);
    rtn = dsi_phy_set_dlane_hstrail(dsiPhyContext->phy_base_addr, dlane->hstrail);
    rtn = dsi_phy_set_dlane_hsexit(dsiPhyContext->phy_base_addr, dlane->hsexit);
    rtn = dsi_phy_set_dlane_wakeup(dsiPhyContext->phy_base_addr, dlane->wakeup);
    rtn = dsi_phy_set_dlane_tago(dsiPhyContext->phy_base_addr, dlane->tago);
    rtn = dsi_phy_set_dlane_tasure(dsiPhyContext->phy_base_addr, dlane->tasure);
    rtn = dsi_phy_set_dlane_taget(dsiPhyContext->phy_base_addr, dlane->taget);

    rtn = dsi_phy_set_lpx(dsiPhyContext->phy_base_addr, com->lpx); //0x54
    rtn = dsi_phy_set_txreadyesc_mode(dsiPhyContext->phy_base_addr, com->txreadyesc_mode); //0x58 bit24
    rtn = dsi_phy_set_core_lane_num(dsiPhyContext->phy_base_addr, core->lane_num); //0x58 bit27 bit6-5
    rtn = dsi_phy_set_core_clksel(dsiPhyContext->phy_base_addr, core->clksel);  //0x58 bit25/26
    rtn = dsi_phy_set_core_phyerr_mask(dsiPhyContext->phy_base_addr, core->phyerr_mask);  //0x58 bit4-0
    rtn = dsi_phy_set_txreadyesc_extend_vld(dsiPhyContext->phy_base_addr, com->txreadyesc_extend_vld); // 0x60 bit15-8
    rtn = dsi_phy_set_rxvalidesc_extend_vld(dsiPhyContext->phy_base_addr, com->rxvalidesc_extend_vld); // 0x60 bit7-0

    if (phyWorkMode == DSI_PHY0_WORK_1C4D) {
        rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, PHY0, core->pwon_pll_soft[RESET_STATUS1]);
        rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, PHY1, core->pwon_pll_soft[RESET_STATUS1]);
    } else {
        rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, phyId, core->pwon_pll_soft[RESET_STATUS1]);
    }
    delayUs(1);
    if (phyWorkMode == DSI_PHY0_WORK_1C4D) {
        rtn = dsi_phy_set_core_pwon_dsi_soft(dsiPhyContext->phy_base_addr, PHY0, core->pwon_dsi_soft[RESET_STATUS1]);
        rtn = dsi_phy_set_core_pwon_dsi_soft(dsiPhyContext->phy_base_addr, PHY1, core->pwon_dsi_soft[RESET_STATUS1]);
    } else {
        rtn = dsi_phy_set_core_pwon_dsi_soft(dsiPhyContext->phy_base_addr, phyId, core->pwon_dsi_soft[RESET_STATUS1]);
    }
    delayUs(100);
    if (phyWorkMode == DSI_PHY0_WORK_1C4D) {
        rtn = dsi_phy_set_core_pll_soft_rst(dsiPhyContext->phy_base_addr, PHY0, core->pll_soft_rst[RESET_STATUS1]); //0x74 bit25
        rtn = dsi_phy_set_core_pll_soft_rst(dsiPhyContext->phy_base_addr, PHY1, core->pll_soft_rst[RESET_STATUS1]); //0x74 bit25
    } else {
        rtn = dsi_phy_set_core_pll_soft_rst(dsiPhyContext->phy_base_addr, phyId, core->pll_soft_rst[RESET_STATUS1]); //0x74 bit25
    }
    rtn = _dsi_phy_v0_pll_lock_checker(phyId);
    WARNING_IF_FAIL(rtn, NULL, "_dsi_phy_v0_pll_lock_checker ~_~!\n");

    if (phyWorkMode == DSI_PHY0_WORK_1C4D) {
        rtn = dsi_phy_set_core_dphy_soft_rst(dsiPhyContext->phy_base_addr, PHY0, core->dphy_soft_rst);
        rtn = dsi_phy_set_core_dphy_soft_rst(dsiPhyContext->phy_base_addr, PHY1, core->dphy_soft_rst);
    } else {
        rtn = dsi_phy_set_core_dphy_soft_rst(dsiPhyContext->phy_base_addr, phyId, core->dphy_soft_rst);
    }

    rtn = dsi_phy_set_auto_ckgate_en(dsiPhyContext->phy_base_addr, com->auto_ckgate_en);
    rtn = dsi_phy_set_afifo_cfg(dsiPhyContext->phy_base_addr, com->afifo_cfg);
    rtn = dsi_phy_set_rcal_pluse_period(dsiPhyContext->phy_base_addr, com->rcal_pluse_period);
    rtn = dsi_phy_set_t_ipcd_det_time_d0(dsiPhyContext->phy_base_addr, com->t_ipcd_det_time_d0);
    rtn = dsi_phy_set_en_deskew(dsiPhyContext->phy_base_addr, com->en_deskew);
    rtn = dsi_phy_set_en_period_deskew(dsiPhyContext->phy_base_addr, com->en_period_deskew);
    rtn = dsi_phy_set_init_deskew_time(dsiPhyContext->phy_base_addr, com->init_deskew_time);
    rtn = dsi_phy_set_period_deskew_time(dsiPhyContext->phy_base_addr, com->period_deskew_time);
    rtn = dsi_phy_set_cl_en_deskew(dsiPhyContext->phy_base_addr, com->cl_en_deskew);
    //rtn = dsi_phy_get_cl_init_deskew_done(dsiPhyContext->phy_base_addr, &com->cl_init_deskew_done);
    rtn = dsi_phy_set_cl_deskew_time(dsiPhyContext->phy_base_addr, com->cl_deskew_time);
    rtn = dsi_phy_set_atest_en(dsiPhyContext->phy_base_addr, com->atest_en);
    rtn = dsi_phy_set_atest_sel(dsiPhyContext->phy_base_addr, com->atest_sel);
    rtn = dsi_phy_set_fset_en(dsiPhyContext->phy_base_addr, com->fset_en);
    rtn = dsi_phy_set_bg_rbias(dsiPhyContext->phy_base_addr, com->bg_rbias);
    rtn = dsi_phy_set_bg_trim(dsiPhyContext->phy_base_addr, com->bg_trim);
    //rtn = dsi_phy_get_vbg_rdy(dsiPhyContext->phy_base_addr, com->vbg_rdy);
    rtn = dsi_phy_set_bist_en(dsiPhyContext->phy_base_addr, com->bist_en);
    rtn = dsi_phy_set_bist_sel(dsiPhyContext->phy_base_addr, com->bist_sel);
    rtn = dsi_phy_set_bist_bit_error(dsiPhyContext->phy_base_addr, com->bist_bit_error);
    rtn = dsi_phy_set_rcal_trim_init_time(dsiPhyContext->phy_base_addr, com->rcal_trim_init_time);
    rtn = dsi_phy_set_rcal_sel(dsiPhyContext->phy_base_addr, com->rcal_sel);
    rtn = dsi_phy_set_rcal_soft_rst(dsiPhyContext->phy_base_addr, com->rcal_soft_rst);
    rtn = dsi_phy_set_rcal_ctrl(dsiPhyContext->phy_base_addr, com->rcal_ctrl);
    rtn = dsi_phy_set_ft_sel(dsiPhyContext->phy_base_addr, com->ft_sel);
    rtn = dsi_phy_set_lpcd_vref_trim(dsiPhyContext->phy_base_addr, com->lpcd_vref_trim);
    rtn = dsi_phy_set_lprx_vref_trim(dsiPhyContext->phy_base_addr, com->lprx_vref_trim);
    rtn = dsi_phy_set_rg_ibias_sel(dsiPhyContext->phy_base_addr, com->rg_ibias_sel);
    rtn = dsi_phy_set_rcal_done_bypass(dsiPhyContext->phy_base_addr, com->rcal_done_bypass);

    rtn = dsi_phy_set_clane_pulldwn_mode(dsiPhyContext->phy_base_addr, clane->pulldwn_mode);
    rtn = dsi_phy_set_dlane_pulldwn_mode(dsiPhyContext->phy_base_addr, dlane->pulldwn_mode);

    rtn = dsi_phy_set_lane_ck8t_reverse(dsiPhyContext->phy_base_addr, lane->ck8t_reverse);
    rtn = dsi_phy_set_lane_predrv_by_dly(dsiPhyContext->phy_base_addr, lane->predrv_by_dly);
    rtn = dsi_phy_set_lane_predrv_dly_sel(dsiPhyContext->phy_base_addr, lane->predrv_dly_sel);
    rtn = dsi_phy_set_lane_hstx_phase(dsiPhyContext->phy_base_addr, lane->hstx_phase);
    rtn = dsi_phy_set_lane_hs_8t_clk_sel(dsiPhyContext->phy_base_addr, lane->hs_8t_clk_sel);
    rtn = dsi_phy_set_lane_lptx_vb_sel(dsiPhyContext->phy_base_addr, lane->lptx_vb_sel);
    rtn = dsi_phy_set_lane_bist_pattern_mode(dsiPhyContext->phy_base_addr, lane->bist_pattern_mode);
    rtn = dsi_phy_set_lane_prbs_check_num(dsiPhyContext->phy_base_addr, lane->prbs_check_num);
    rtn = dsi_phy_set_lane_prbs_seed(dsiPhyContext->phy_base_addr, lane->prbs_seed);
    rtn = dsi_phy_set_lane_prbs_err_th(dsiPhyContext->phy_base_addr, lane->prbs_err_th);

    rtn = dsi_phy_set_core_dll_sel(dsiPhyContext->phy_base_addr, core->dll_sel);
    rtn = dsi_phy_set_core_ulprx_sel(dsiPhyContext->phy_base_addr, core->ulprx_sel);
    rtn = dsi_phy_set_core_da_clk1_en(dsiPhyContext->phy_base_addr, core->da_clk_en);
    rtn = dsi_phy_set_core_pll_bist_en(dsiPhyContext->phy_base_addr, core->pll_bist_en);
    rtn = dsi_phy_set_core_pll_bist_ctrl(dsiPhyContext->phy_base_addr, core->pll_bist_ctrl);
    rtn = dsi_phy_set_core_pll_bist_cnt(dsiPhyContext->phy_base_addr, core->pll_bist_cnt);

    rtn = dsi_phy_set_core_pll_ref24m_sel(dsiPhyContext->phy_base_addr, core->pll_ref24m_sel);
    rtn = dsi_phy_set_core_pll_ssc_en(dsiPhyContext->phy_base_addr, core->pll_ssc_en);
    rtn = dsi_phy_set_core_pll_ssc_ctrl(dsiPhyContext->phy_base_addr, core->pll_ssc_ctrl);
    rtn = dsi_phy_set_core_pll_lpf_r2(dsiPhyContext->phy_base_addr, core->pll_lpf_r2);
    rtn = dsi_phy_set_core_pll_lpf_r3(dsiPhyContext->phy_base_addr, core->pll_lpf_r3);
    rtn = dsi_phy_set_core_pll_lpf_cl(dsiPhyContext->phy_base_addr, core->pll_lpf_c1);
    rtn = dsi_phy_set_core_pll_lpf_c2(dsiPhyContext->phy_base_addr, core->pll_lpf_c2);
    rtn = dsi_phy_set_core_pll_ldo_test_en(dsiPhyContext->phy_base_addr, core->pll_ldo_test_en);
    rtn = dsi_phy_set_core_pll_ldo_vref(dsiPhyContext->phy_base_addr, core->pll_ldo_vref);
    rtn = dsi_phy_set_core_pll_ldo_vout(dsiPhyContext->phy_base_addr, core->pll_ldo_vout);

    rtn = dsi_phy_set_core_pll_icp(dsiPhyContext->phy_base_addr, core->pll_icp);
    rtn = dsi_phy_set_core_pll_icp_of_fset(dsiPhyContext->phy_base_addr, core->pll_icp_offset);
    rtn = dsi_phy_set_core_rg_dslpll_ssc_step_wicith_ctrl(dsiPhyContext->phy_base_addr, core->rg_dslpll_ssc_step_wicith_ctrl);
    rtn = dsi_phy_set_core_rg_dslpll_ssc_step_number_ctrl(dsiPhyContext->phy_base_addr, core->rg_dslpll_ssc_step_number_ctrl);
    rtn = dsi_phy_set_core_pll_lock_bypass(dsiPhyContext->phy_base_addr, core->pll_lock_bypass);
    rtn = dsi_phy_set_core_pll_lock_time(dsiPhyContext->phy_base_addr, core->pll_lock_time);
    rtn = dsi_phy_set_core_pll_lock_cnt(dsiPhyContext->phy_base_addr, core->pll_lock_cnt);
    rtn = dsi_phy_set_core_hstx_sr(dsiPhyContext->phy_base_addr, core->hstx_sr);

    rtn = dsi_phy_set_core_lpcd_fl0(dsiPhyContext->phy_base_addr, core->lpcd_fl0);
    rtn = dsi_phy_set_core_lpcd_fl1(dsiPhyContext->phy_base_addr, core->lpcd_fl1);

    rtn = dsi_phy_set_mode_ext_loop_mode_cfg(dsiPhyContext->phy_base_addr, mode->ext_loop_mode_cfg);
    rtn = dsi_phy_set_mode_en_ext_loop_cfg(dsiPhyContext->phy_base_addr, mode->en_ext_loop_cfg);
    rtn = dsi_phy_set_mode_txclkreq_loop_cfg(dsiPhyContext->phy_base_addr, mode->txclkreq_loop_cfg);
    rtn = dsi_phy_set_mode_txdatareq_loop_cfg(dsiPhyContext->phy_base_addr, mode->txdatareq_loop_cfg);
    rtn = dsi_phy_set_mode_txdatasel_loop_cfg(dsiPhyContext->phy_base_addr, mode->txdatasel_loop_cfg);
    rtn = dsi_phy_set_mode_txdata_loop_cfg(dsiPhyContext->phy_base_addr, mode->txdata_loop_cfg);
    rtn = dsi_phy_set_mode_txdatanum_loop_cfg(dsiPhyContext->phy_base_addr, mode->txdatanum_loop_cfg);
    rtn = dsi_phy_set_mode_ext_loop_bist_bit_error_cfg(dsiPhyContext->phy_base_addr, mode->ext_loop_bist_bit_error_cfg);
    rtn = dsi_phy_set_mode_ext_loop_prbs_pattern_mode_cfg(dsiPhyContext->phy_base_addr, mode->ext_loop_prbs_pattern_mode_cfg);
    rtn = dsi_phy_set_mode_ext_loop_prbs_seed_cfg(dsiPhyContext->phy_base_addr, mode->ext_loop_prbs_seed_cfg);

    rtn = _dsi_phy_v0_pll_lock_checker(phyId);
    WARNING_IF_FAIL(rtn, NULL, "_dsi_phy_v0_pll_lock_checker ~_~!\n");

    //delayUs(1);
    //core->pwon_pll_soft[RESET_STATUS2][PHY1] = 0x00;
    //rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, phyId, core->pwon_pll_soft[RESET_STATUS2]);
    //rtn = dsi_phy_set_core_dphy_soft_rst(dsiPhyContext->phy_base_addr, core->dphy_soft_rst);

    #if 0
    rtn = regDefaultGet(dsiPhyContext->phy_base_addr, dsi_phy_get_reg_default_tab());
    rtn = showRegValuePtr(dsi_phy_get_reg_default_tab(), "tx_phy");
    #endif

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "end");
    #endif

    return rtn;
}

static int32_t _dsi_phy_v0_uncfg(dsi_phy_deinit_in_t *inParam, dsi_phy_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    //core_t *core = NULL;
    //phy_com_t *com = NULL;

    CHECK_PTR(inParam, NULL, "dsi_phy_deinit_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_deinit_out_t is null ~_~\n");
    CHECK_PTR(dsiPhyContext, NULL, "dsiPhyContext is null ~_~\n");

    //core = &dsiPhyContext->core;
    //com = &dsiPhyContext->com;

  //  rtn = dsi_phy_set_core_pwon_dsi_soft(dsiPhyContext->phy_base_addr, PHY0, core->pwon_dsi_soft[RESET_STATUS0]);
  //  rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, PHY0, core->pwon_pll_soft[RESET_STATUS0]);
  //  rtn = dsi_phy_set_core_pwon_dsi_soft(dsiPhyContext->phy_base_addr, PHY1, core->pwon_dsi_soft[RESET_STATUS0]);
  //  rtn = dsi_phy_set_core_pwon_pll_soft(dsiPhyContext->phy_base_addr, PHY1, core->pwon_pll_soft[RESET_STATUS0]);
  //  rtn = dsi_phy_set_su_iddq_en_soft(dsiPhyContext->phy_base_addr, com->su_iddq_en_soft); //0x70 bit11
  //  rtn = dsi_phy_set_core_dphy_soft_rst(dsiPhyContext->phy_base_addr, PHY0, core->dphy_soft_rst);
  //  rtn = dsi_phy_set_core_dphy_soft_rst(dsiPhyContext->phy_base_addr, PHY1, core->dphy_soft_rst);

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_power_on_cfg(dsi_phy_power_on_in_t *inParam, dsi_phy_power_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    //dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    //dsi_phy_power_t *power = NULL;

    CHECK_PTR(inParam, NULL, "dsi_phy_power_on_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_on_out_t is null ~_~\n");

    //power = &dsiPhyContext->power;

    #if 0
    if (1 == power->power_cnt)
    {
        rtn = dsi_phy_clk_on_reset(dsiPhyContext->clk_base_addr, dsiPhyContext->rst_base_addr);
        EXIT_IF_FAIL(rtn, NULL, "dsi_phy_clk_on_reset ~_~\n");
    }
    #endif

    EXIT:

    return rtn;
}

static int32_t _dsi_phy_v0_power_off_cfg(dsi_phy_power_off_in_t *inParam, dsi_phy_power_off_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    //dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    //dsi_phy_power_t *power = NULL;

    CHECK_PTR(inParam, NULL, "dsi_phy_power_off_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_off_out_t null ~_~\n");

    //power = &dsiPhyContext->power;

    #if 0
    if (0 == power->power_cnt) {
        rtn = dsi_phy_clk_off(dsiPhyContext->phy_base_addr);
        EXIT_IF_FAIL(rtn, NULL, "dsi_phy_set_pwon_dsi_soft ~_~\n");
    }
    #endif

    EXIT:

    return rtn;
}

int32_t dsi_phy_v0_init(dsi_phy_init_in_t *inParam, dsi_phy_init_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");
    CHECK_PTR(dsiPhyContext, NULL, "dsiPhyContext is null ~_~\n");

    if (DSI_INITED == dsiPhyContext->status[inParam->phyId]) {
        rtn = FAIL_RE_INIT;
        EXIT_IF_FAIL(rtn, NULL, "dsi_phy_v0_init status(%d) ~_~!\n", dsiPhyContext->status[inParam->phyId]);
    }

    rtn = _dsi_phy_v0_init_param_checker(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_init_param_checker ~_~\n");

    rtn = _dsi_phy_v0_init_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_init_param_set ~_~\n");

    rtn = _dsi_phy_v0_cfg(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "dsi_phy_v0_cfg ~_~\n");

    dsiPhyContext->status[inParam->phyId] = DSI_INITED;

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_phy_v0_deinit(dsi_phy_deinit_in_t *inParam, dsi_phy_deinit_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    if(DSI_INITED != dsiPhyContext->status[inParam->phyId]) {
        rtn = FAIL_UN_INIT;
        EXIT_IF_FAIL(rtn, NULL, "dsi_phy_v0_deinit status(%d) ~_~!\n", dsiPhyContext->status[inParam->phyId]);
    }

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");

    rtn = _dsi_phy_v0_deinit_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_init_param_set ~_~\n");

    rtn = _dsi_phy_v0_uncfg(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_uncfg ~_~\n");

    dsiPhyContext->status[inParam->phyId] = DSI_INITED;

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_phy_power_on(dsi_phy_power_on_in_t *inParam, dsi_phy_power_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    if(DSI_POWER_OFF != dsiPhyContext->status[inParam->phyId]) {
        rtn = FAIL_INVALID;
        EXIT_IF_FAIL(rtn, NULL, "dsi_phy_power_on status(%d) ~_~!\n", dsiPhyContext->status[inParam->phyId]);
    }

    CHECK_PTR(inParam, NULL, "dsi_phy_power_on_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_on_out_t is null ~_~\n");

    rtn =_dsi_phy_v0_power_on_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_power_on_param_set ~_~\n");

    rtn = _dsi_phy_v0_power_on_cfg(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_power_on_cfg ~_~\n");

    dsiPhyContext->status[inParam->phyId] = DSI_POWER_ON;

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_phy_power_off(dsi_phy_power_off_in_t *inParam, dsi_phy_power_off_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    dsi_phy_power_t *power = NULL;


    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d----------start-------------\n",__FUNCTION__,__LINE__);
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_power_off_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_power_off_out_t is null ~_~\n");
    CHECK_PTR(dsiPhyContext, NULL, "dsiPhyContext is null ~_~\n");

    if(DSI_POWER_OFF == dsiPhyContext->status[inParam->phyId]) {
        EXIT_IF_FAIL(1, NULL, "dsi_phy_power_off status(%d) ~_~!\n", dsiPhyContext->status[inParam->phyId]);
    }

    power = &dsiPhyContext->power;

    rtn =_dsi_phy_v0_power_off_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_power_off_param_set ~_~\n");

    rtn = _dsi_phy_v0_power_off_cfg(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy_v0_power_off_cfg ~_~\n");

    if (0 == power->power_cnt) {
        memset(dsiPhyContext, 0x00, sizeof(dsi_phy_mpi_context_t));
    }

    dsiPhyContext->status[inParam->phyId] = DSI_POWER_OFF;

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

static int32_t _dsi_phy_pll_v1_init_param_set(dsi_phy_pll_on_in_t *inParam, dsi_phy_pll_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    phy_com_t *com = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    core_t *core = NULL;
    uint32_t phyId = 0x00;
    uint32_t laneId = 0x00;
    uint32_t laneBaseId = 0x00;
    uint32_t laneNum = 0x00;
    uint32_t i = 0x00;
    uint32_t phyStartId = 0x00;
    uint32_t phyEndId = 0x00;
    dsi_phy_work_mode_e phyWorkMode = 0x00;

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");

    //os_mutex_init(&dsiPhyContext->phy_clk_mutex);

    phyId = inParam->phyId;
    com = &dsiPhyContext->com;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    core = &dsiPhyContext->core;

    phyWorkMode = (PHY0 == phyId) ? DSI_PHY0_WORK_1C2D : DSI_PHY1_WORK_1C2D;
    dsiPhyContext->phyWorkMode[phyId] = phyWorkMode;

    dsiPhyContext->phy_base_addr = inParam->phy_addr;
    dsiPhyContext->vou_base_addr = inParam->vou_addr;

    core->pll_freq_doubler_en[phyId] = inParam->pll_doubler;
    core->pll_div_s[phyId] = inParam->pll_div_s;
    core->pll_n[phyId] = inParam->pll_n;
    core->pll_kint[phyId] = inParam->pll_kint;
    core->pll_nint[phyId] = inParam->pll_nint;
    core->pixelclk_div[phyId] = inParam->pixelclk_div;
    core->pll_pdiv[phyId] = inParam->pll_pdiv;

    dsiPhyContext->dhd_div[phyId] = inParam->dhd_div;



    clane->pulldwn_mode[phyId] = 0x00;

    laneBaseId = _get_dlane_base_id(phyId);
    laneNum = laneBaseId + 2;
    for (laneId = laneBaseId; laneId < laneNum; laneId++) {
        dlane->pulldwn_mode[laneId] = 0; //???
    }

    com->su_iddq_en_soft = 0x00;
    com->bg_en = 0x01;
    com->bg_trim = 0x07;

    /*--RESET_STATUS0--*/
    core->pwon_dsi_soft[RESET_STATUS0][phyId] = 0x00;
    core->pwon_pll_soft[RESET_STATUS0][phyId] = 0x00;
    core->pll_soft_rst[RESET_STATUS0][phyId] = 0x01;

    /*--RESET_STATUS1--*/
    core->pwon_pll_soft[RESET_STATUS1][phyId] = 0x01;
    core->pwon_dsi_soft[RESET_STATUS1][phyId] = 0x01;
    core->pll_soft_rst[RESET_STATUS1][phyId] = 0x00;

    core->da_clk_en[phyId] = 0x01;


    if (DSI_PHY0_WORK_1C4D == phyWorkMode) {
        phyStartId = PHY0;
        phyEndId = PHY_CORE_MAX;
    } else if (DSI_PHY0_WORK_1C2D == phyWorkMode) {
        phyStartId = PHY0;
        phyEndId = PHY1;
    } else if (DSI_PHY1_WORK_1C2D == phyWorkMode){
        phyStartId = PHY1; //???
        phyEndId = PHY_CORE_MAX;
    }

    for (i = phyStartId; i < phyEndId; i++) {
        core->en_lpcd[i] = 0x00;
        core->en_lprx[i] = 0x01;
        core->en_ulprx[i] = 0x01;
    }

    #if DSI_PHY_PARAM_DEBUG
    _dsi_phy_pll_v1_param_show(phyId);
    #endif

    EXIT:

    return rtn;
}

int32_t _dsi_phy_pll_v1_cfg(dsi_phy_pll_on_in_t *inParam, dsi_phy_pll_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    phy_com_t *com = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    core_t *core = NULL;
    uint32_t phyId = 0x00;
    dsi_phy_work_mode_e phyWorkMode[PHY_CORE_MAX] = {0x00};

    #if DSI_PHY_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_pll_cfg_in_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_pll_cfg_out_t is null ~_~\n");
    CHECK_PTR(dsiPhyContext, NULL, "dsiPhyContext is null ~_~\n");

    phyId = inParam->phyId;
    com = &dsiPhyContext->com;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    core = &dsiPhyContext->core;
    phyWorkMode[PHY0] = dsiPhyContext->phyWorkMode[PHY0];
    phyWorkMode[PHY1] = dsiPhyContext->phyWorkMode[PHY1];

    #if DSI_PHY_FUN_DEBUG
    printf("\n-----------------DHD%d--------------------\n", phyId);
    printf("com->bg_en                        :%d\n", com->bg_en);
    printf("com->bg_trim                      :%d\n", com->bg_trim);
    printf("com->su_iddq_en_soft              :%d\n", com->su_iddq_en_soft);
    printf("core->da_clk_en                   :%d\n", core->da_clk_en[phyId]);
    printf("core->pll_soft_rst[RESET_STATUS0] :%d\n", core->pll_soft_rst[RESET_STATUS0][phyId]);
    printf("clane->pulldwn_mode               :%d\n", clane->pulldwn_mode[phyId]);
    printf("dlane->pulldwn_mode               :%d\n", dlane->pulldwn_mode[phyId]);
    printf("core->en_lpcd                     :%d\n", core->en_lpcd[phyId]);
    printf("core->en_lprx                     :%d\n", core->en_lprx[phyId]);
    printf("core->en_ulprx                    :%d\n", core->en_ulprx[phyId]);
    printf("core->pwon_sel                    :%d\n", core->pwon_sel[phyId]);
    printf("core->pwon_dsi_soft[RESET_STATUS0]:%d\n", core->pwon_dsi_soft[RESET_STATUS0][phyId]);
    printf("core->pwon_pll_soft[RESET_STATUS0]:%d\n", core->pwon_pll_soft[RESET_STATUS0][phyId]);
    printf("core->pixelclk_div                :%d\n", core->pixelclk_div[phyId]);
    printf("core->pll_div_s                   :%d\n", core->pll_div_s[phyId]);
    printf("core->pll_freq_doubler_en         :%d\n", core->pll_freq_doubler_en[phyId]);
    printf("core->pll_n                       :%d\n", core->pll_n[phyId]);
    printf("core->pll_pdiv                    :%d\n", core->pll_pdiv[phyId]);
    printf("core->pll_kint                    :%d\n", core->pll_kint[phyId]);
    printf("core->pll_nint                    :%d\n", core->pll_nint[phyId]);
    printf("core->pll_soft_rst[RESET_STATUS1] :%d\n", core->pll_soft_rst[RESET_STATUS1][phyId]);
    printf("core->pwon_dsi_soft[RESET_STATUS1]:%d\n", core->pwon_dsi_soft[RESET_STATUS1][phyId]);
    printf("core->pwon_pll_soft[RESET_STATUS1]:%d\n", core->pwon_pll_soft[RESET_STATUS1][phyId]);
    #endif

    rtn = dsi_phy_set_bg_en(inParam->phy_addr, com->bg_en); //0x70 bit6
    rtn = dsi_phy_set_bg_trim(inParam->phy_addr, com->bg_trim);
    rtn = dsi_phy_set_su_iddq_en_soft(inParam->phy_addr, com->su_iddq_en_soft); //0x70 bit11

    rtn = dsi_phy_set_core_da_clk1_en(inParam->phy_addr, core->da_clk_en);
    rtn = dsi_phy_set_core_pll_soft_rst(inParam->phy_addr, phyId, core->pll_soft_rst[RESET_STATUS0]); //0x74/374 bit25

    rtn = dsi_phy_set_clane_pulldwn_mode(inParam->phy_addr, clane->pulldwn_mode); // 0x58 bit23-20
    rtn = dsi_phy_set_dlane_pulldwn_mode(inParam->phy_addr, dlane->pulldwn_mode); // 0x58 bit19-12

    if (PHY0 == phyId) {
        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG8, 21, 0x03, 0x01); //0x120
        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG9,  0, 0x03, 0x01); //0x124

        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG9, 22, 0x03, 0x01); //0x124
        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG10, 9, 0x03, 0x01); //0x1a0

        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG10, 25, 0x03, 0x01); //0x1a0
        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG11,  9, 0x03, 0x01); //0x1a4

        //regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG29, 21, 0x01, 0x01); //0x1a4
    } else if (PHY1 == phyId) {
        if (DSI_PHY0_WORK_1C4D != phyWorkMode[PHY0]) {
            regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG11, 25, 0x03, 0x01); //0x1a4
            regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG12,  9, 0x03, 0x01); //0x1a8

            regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG12, 25, 0x03, 0x01); //0x1a8
            regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG13, 9, 0x03, 0x01); //0x1ac
        }

        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG29,  8, 0x03, 0x01); //0x1ec
        regWriteBits32(inParam->phy_addr + DPHY_DEBUG_CFG29, 21, 0x03, 0x01); //0x1ec
    }

    rtn = dsi_phy_set_core_en_lpcd(inParam->phy_addr, core->en_lpcd); //0x98 bit12
    rtn = dsi_phy_set_core_en_lprx(inParam->phy_addr, core->en_lprx); //0x98 bit11
    rtn = dsi_phy_set_core_en_ulprx(inParam->phy_addr, core->en_ulprx);//0x98 bit10

    rtn = dsi_phy_set_core_pwon_sel(inParam->phy_addr, core->pwon_sel); //0x70 bit16-17

    rtn = dsi_phy_set_core_pwon_dsi_soft(inParam->phy_addr, phyId, core->pwon_dsi_soft[RESET_STATUS0]); //0x70 bit13 bit15
    rtn = dsi_phy_set_core_pwon_pll_soft(inParam->phy_addr, phyId, core->pwon_pll_soft[RESET_STATUS0]); //0x70 bit12 bit14

    rtn = dsi_phy_set_core_pixelclkl_div(dsiPhyContext->phy_base_addr, core->pixelclk_div);//0x74 bit29-26
    rtn = dsi_phy_set_core_pll_div_s(dsiPhyContext->phy_base_addr, core->pll_div_s); //0x78 bit28
    rtn = dsi_phy_set_core_pll_freq_doubler_en(dsiPhyContext->phy_base_addr, core->pll_freq_doubler_en); //0x78 bit27
    rtn = dsi_phy_set_core_pll_n(dsiPhyContext->phy_base_addr, core->pll_n); //0x80 bit24-14
    rtn = dsi_phy_set_core_pll_pdiv(dsiPhyContext->phy_base_addr, core->pll_pdiv); //0x78 bit25-23
    rtn = dsi_phy_set_core_pll_kint(dsiPhyContext->phy_base_addr, core->pll_kint); //0x7c bit31-9
    rtn = dsi_phy_set_core_pll_nint(dsiPhyContext->phy_base_addr, core->pll_nint); //0x7c bit8-0
    rtn = vou_dhd_clk_on(dsiPhyContext->vou_base_addr, phyId, dsiPhyContext->dhd_div[phyId]);

    rtn = dsi_phy_set_core_pwon_pll_soft(inParam->phy_addr, phyId, core->pwon_pll_soft[RESET_STATUS1]); //0x70 bit12 bit14
    delayUs(1);
    rtn = dsi_phy_set_core_pwon_dsi_soft(inParam->phy_addr, phyId, core->pwon_dsi_soft[RESET_STATUS1]); //0x74 bit13 bit14
    delayUs(100);
    rtn = dsi_phy_set_core_pll_soft_rst(inParam->phy_addr, phyId, core->pll_soft_rst[RESET_STATUS1]); //0x74 bit25

    rtn = _dsi_phy_v0_pll_lock_checker(phyId);
    WARNING_IF_FAIL(rtn, NULL, "_dsi_phy_v0_pll_lock_checker ~_~!\n");

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_phy_pll_v1_on(dsi_phy_pll_on_in_t *inParam, dsi_phy_pll_on_out_t *outParam)
{
    int32_t rtn = SUCCESS;

    #if DSI_PHY_FUN_DEBUG
    debug_fun(1, __FUNCTION__, __LINE__, "start");
    #endif

    CHECK_PTR(inParam, NULL, "dsi_phy_init_inparam_t is null ~_~\n");
    CHECK_PTR(outParam, NULL, "dsi_phy_init_outparam_t is null ~_~\n");

    rtn = _dsi_phy_pll_v1_init_param_set(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy%d_pll_v1_init_param_set ~_~!\n", inParam->phyId);

    rtn = _dsi_phy_pll_v1_cfg(inParam, outParam);
    EXIT_IF_FAIL(rtn, NULL, "_dsi_phy%d_pll_v1_cfg ~_~!\n", inParam->phyId);

    EXIT:

    #if DSI_PHY_FUN_DEBUG
    printf("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t dsi_phy_param_show(fhosal_proc_entry_t *sqf, uint32_t phyId)
{
    int32_t rtn = SUCCESS;
    dsi_phy_mpi_context_t *dsiPhyContext = dsi_phy_mipi_context_get();
    frame_info_t *frameInfo = NULL;
    //dsi_phy_power_t *power = NULL;
    c_lane_t *clane = NULL;
    d_lane_t *dlane = NULL;
    //lane_t *lane = NULL;
    core_t *core = NULL;
    phy_com_t *com = NULL;
    //phy_mode_t *mode = NULL;
    uint32_t laneId = 0x00;
    uint32_t laneBaseId = 0x00;
    uint32_t laneNum = 0x00;

    frameInfo = &dsiPhyContext->frameInfo[phyId];
    //power = &dsiPhyContext->power;
    clane = &dsiPhyContext->clane;
    dlane = &dsiPhyContext->dlane;
    //lane = &dsiPhyContext->lane;
    core = &dsiPhyContext->core;
    //mode = &dsiPhyContext->mode;
    com = &dsiPhyContext->com;

    fhosal_seq_printf(sqf, "\n----------dsi txPhy%d param---\n", phyId);
    fhosal_seq_printf(sqf, "dsiPhyContext->phyWorkMode[%d]  : %d\n", phyId, dsiPhyContext->phyWorkMode[phyId]);
    fhosal_seq_printf(sqf, "\n----------pll-----------------\n");
    fhosal_seq_printf(sqf, "frameInfo->fromat         : %d\n", frameInfo->fromat               );
    fhosal_seq_printf(sqf, "frameInfo->fps            : %d\n", frameInfo->fps                  );
    fhosal_seq_printf(sqf, "frameInfo->lane_num       : %d\n", frameInfo->lane_num             );
    fhosal_seq_printf(sqf, "dsiPhyContext->laneclk[%d]: %d\n", phyId, dsiPhyContext->lane_clk[phyId]);
    fhosal_seq_printf(sqf, "core->pll_freq_doubler_en : %d\n", core->pll_freq_doubler_en[phyId]);
    fhosal_seq_printf(sqf, "core->pll_div_s           : %d\n", core->pll_div_s[phyId]          );
    fhosal_seq_printf(sqf, "core->pll_n               : %d\n", core->pll_n[phyId]              );
    fhosal_seq_printf(sqf, "core->pll_kint            : %d\n", core->pll_kint[phyId]           );
    fhosal_seq_printf(sqf, "core->pll_nint            : %d\n", core->pll_nint[phyId]           );
    fhosal_seq_printf(sqf, "core->pixelclk_div        : %d\n", core->pixelclk_div[phyId]      );
    fhosal_seq_printf(sqf, "core->pll_pdiv            : %d\n", core->pll_pdiv[phyId]           );
    fhosal_seq_printf(sqf, "dsiPhyContext->dhd_div    : %d\n", dsiPhyContext->dhd_div[phyId]   );
    fhosal_seq_printf(sqf, "\n--------clane-----------------\n");
    fhosal_seq_printf(sqf, "clane->rst2enlptx         : %d\n", clane->rst2enlptx[phyId]        );
    fhosal_seq_printf(sqf, "clane->inittime           : %d\n", clane->inittime[phyId]          );
    fhosal_seq_printf(sqf, "clane->clkprepare         : %d\n", clane->clkprepare[phyId]        );
    fhosal_seq_printf(sqf, "clane->clkzero            : %d\n", clane->clkzero[phyId]           );
    fhosal_seq_printf(sqf, "clane->clkpre             : %d\n", clane->clkpre[phyId]            );
    fhosal_seq_printf(sqf, "clane->clkpost            : %d\n", clane->clkpost[phyId]           );
    fhosal_seq_printf(sqf, "clane->clktrail           : %d\n", clane->clktrail[phyId]          );
    fhosal_seq_printf(sqf, "clane->hsexit             : %d\n", clane->hsexit[phyId]            );
    fhosal_seq_printf(sqf, "clane->pulldwn_mode       : %d\n", clane->pulldwn_mode[phyId]      );
    fhosal_seq_printf(sqf, "clane->wakeup             : %d\n", clane->wakeup[phyId]            );

    laneBaseId = _get_dlane_base_id(phyId);
    laneNum = laneBaseId + frameInfo->lane_num + 1;
    fhosal_seq_printf(sqf, "\n--------dlane-----------------\n");
    for (laneId = laneBaseId; laneId < laneNum; laneId++) {
        fhosal_seq_printf(sqf, "dlane->rst2enlptx[%d]         : %d\n", laneId, dlane->rst2enlptx[laneId]        );
        fhosal_seq_printf(sqf, "dlane->inittime[%d]           : %d\n", laneId, dlane->inittime[laneId]          );
        fhosal_seq_printf(sqf, "dlane->hsprepare[%d]          : %d\n", laneId, dlane->hsprepare[laneId]         );
        fhosal_seq_printf(sqf, "dlane->hszero[%d]             : %d\n", laneId, dlane->hszero[laneId]            );
        fhosal_seq_printf(sqf, "dlane->hstrail[%d]            : %d\n", laneId, dlane->hstrail[laneId]           );
        fhosal_seq_printf(sqf, "dlane->hsexit[%d]             : %d\n", laneId, dlane->hsexit[laneId]            );
        fhosal_seq_printf(sqf, "dlane->wakeup[%d]             : %d\n", laneId, dlane->wakeup[laneId]            );
        fhosal_seq_printf(sqf, "dlane->tago[%d]               : %d\n", laneId, dlane->tago[laneId]              );
        fhosal_seq_printf(sqf, "dlane->tasure[%d]             : %d\n", laneId, dlane->tasure[laneId]            );
        fhosal_seq_printf(sqf, "dlane->taget[%d]              : %d\n", laneId, dlane->taget[laneId]             );
    }

    fhosal_seq_printf(sqf, "com->lpx lp01                  : %d\n", com->lpx                         );

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

