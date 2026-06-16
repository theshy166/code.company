#include "reg.h"
#include "reg_pool.h"
#include "dsi_phy_v01_reg_def.h"
#include "dsi_phy_v01_reg_pool.h"
#include "dsi_phy_v01_reg_struct.h"
#include "dsi_phy_v01_drv_param.h"
#include "dsi_com_param.h"

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

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

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
int32_t dsi_phy_set_lpx(uint32_t base_addr, uint32_t lpx)
{
    int32_t rtn = SUCCESS;
    union dsi_common_para0_tag *reg = getRegValuePtr(COMMON_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.t_lpx = lpx;
    regWrite32(base_addr + COMMON_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_txreadyesc_mode(uint32_t base_addr, uint32_t txreadyesc_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.txreadyesc_mode = txreadyesc_mode;
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_auto_ckgate_en(uint32_t base_addr, uint32_t auto_ckgate_en)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.auto_ckgate_en = auto_ckgate_en;
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_link_mode(uint32_t base_addr, uint32_t phy_link_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_mode_cfg_tag *reg = getRegValuePtr(MODE_CFG, dsi_phy_get_reg_default_tab());

    reg->mBits.phy_link_mode = phy_link_mode;
    regWrite32(base_addr + MODE_CFG, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_txreadyesc_extend_vld(uint32_t base_addr, uint32_t txreadyesc_extend_vld)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para5_tag *reg = getRegValuePtr(CLANE_PARA5, dsi_phy_get_reg_default_tab());

    reg->mBits.txreadyesc_extend_vld = txreadyesc_extend_vld;
    regWrite32(base_addr + CLANE_PARA5, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rxvalidesc_extend_vld(uint32_t base_addr, uint32_t rxvalidesc_extend_vld)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para5_tag *reg = getRegValuePtr(CLANE_PARA5, dsi_phy_get_reg_default_tab());

    reg->mBits.rxvalidesc_extend_vld = rxvalidesc_extend_vld;
    regWrite32(base_addr + CLANE_PARA5, reg->dwValue);


    return rtn;
}

int32_t dsi_phy_get_afifo_info(uint32_t base_addr, uint32_t *afifo_info)
{
    int32_t rtn = SUCCESS;
    union dsi_pcs_phy_sync_tag *reg = getRegValuePtr(PCS_PHY_SYNC, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PCS_PHY_SYNC);
    *afifo_info = reg->mBits.afifo_info;

    return rtn;
}

int32_t dsi_phy_set_afifo_cfg(uint32_t base_addr, uint32_t afifo_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_pcs_phy_sync_tag *reg = getRegValuePtr(PCS_PHY_SYNC, dsi_phy_get_reg_default_tab());

    reg->mBits.afifo_cfg = afifo_cfg;
    regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);


    return rtn;
}

int32_t dsi_phy_set_rcal_pluse_period(uint32_t base_addr, uint32_t rcal_pluse_period)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc0_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC0, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_pluse_period = rcal_pluse_period;
    regWrite32(base_addr + DESKEW_RCAL_MISC0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_t_ipcd_det_time_d0(uint32_t base_addr, uint32_t t_ipcd_det_time_d0)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc0_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC0, dsi_phy_get_reg_default_tab());

    reg->mBits.t_ipcd_det_time_d0 = t_ipcd_det_time_d0;
    regWrite32(base_addr + DESKEW_RCAL_MISC0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_en_deskew(uint32_t base_addr, uint32_t en_deskew)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc0_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC0, dsi_phy_get_reg_default_tab());

    reg->mBits.en_deskew = en_deskew;
    regWrite32(base_addr + DESKEW_RCAL_MISC0, reg->dwValue);


    return rtn;
}

int32_t dsi_phy_set_en_period_deskew(uint32_t base_addr, uint32_t en_period_deskew)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc0_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC0, dsi_phy_get_reg_default_tab());

    reg->mBits.en_period_deskew = en_period_deskew;
    regWrite32(base_addr + DESKEW_RCAL_MISC0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_init_deskew_time(uint32_t base_addr, uint32_t init_deskew_time)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc1_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC1, dsi_phy_get_reg_default_tab());

    reg->mBits.init_deskew_time = init_deskew_time;
    regWrite32(base_addr + DESKEW_RCAL_MISC1, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_period_deskew_time(uint32_t base_addr, uint32_t period_deskew_time)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc2_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC2, dsi_phy_get_reg_default_tab());

    reg->mBits.period_deskew_time = period_deskew_time;
    regWrite32(base_addr + DESKEW_RCAL_MISC2, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_cl_en_deskew(uint32_t base_addr, uint32_t cl_en_deskew)
{
    int32_t rtn = SUCCESS;
    union dsi_cl_deskew_tag *reg = getRegValuePtr(CL_DESKEW, dsi_phy_get_reg_default_tab());

    reg->mBits.cl_en_deskew = cl_en_deskew;
    regWrite32(base_addr + CL_DESKEW, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_get_cl_init_deskew_done(uint32_t base_addr, uint32_t *cl_init_deskew_done)
{
    int32_t rtn = SUCCESS;
    union dsi_cl_deskew_tag *reg = getRegValuePtr(CL_DESKEW, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + CL_DESKEW);
    *cl_init_deskew_done = reg->mBits.cl_init_deskew_done;

    return rtn;
}

int32_t dsi_phy_set_cl_deskew_time(uint32_t base_addr, uint32_t cl_deskew_time)
{
    int32_t rtn = SUCCESS;
    union dsi_cl_deskew_tag *reg = getRegValuePtr(CL_DESKEW, dsi_phy_get_reg_default_tab());

    reg->mBits.cl_deskew_time = cl_deskew_time;
    regWrite32(base_addr + CL_DESKEW, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_su_iddq_en_soft(uint32_t base_addr, uint32_t su_iddq_en_soft)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.su_iddq_en_soft = su_iddq_en_soft;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_atest_en(uint32_t base_addr, uint32_t atest_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.atest_en = atest_en;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_atest_sel(uint32_t base_addr, uint32_t atest_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.atest_sel = atest_sel;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_fset_en(uint32_t base_addr, uint32_t fset_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.fset_en = fset_en;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_bg_en(uint32_t base_addr, uint32_t bg_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.bg_en = bg_en;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_bg_rbias(uint32_t base_addr, uint32_t bg_rbias)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.bg_rbias = bg_rbias;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_bg_trim(uint32_t base_addr, uint32_t bg_trim)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.bg_trim = bg_trim;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_get_vbg_rdy(uint32_t base_addr, uint32_t *vbg_rdy)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PHY_PARA0);

    *vbg_rdy = reg->mBits.vbg_rdy;

    return rtn;
}

int32_t dsi_phy_set_bist_en(uint32_t base_addr, uint32_t bist_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para9_tag *reg = getRegValuePtr(PHY_PARA9, dsi_phy_get_reg_default_tab());

    reg->mBits.bist_en = bist_en;
    regWrite32(base_addr + PHY_PARA9, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_bist_sel(uint32_t base_addr, uint32_t bist_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para9_tag *reg = getRegValuePtr(PHY_PARA9, dsi_phy_get_reg_default_tab());

    reg->mBits.bist_sel = bist_sel;
    regWrite32(base_addr + PHY_PARA9, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_bist_bit_error(uint32_t base_addr, uint32_t bist_bit_error)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para9_tag *reg = getRegValuePtr(PHY_PARA9, dsi_phy_get_reg_default_tab());

    reg->mBits.bist_bit_error = bist_bit_error;
    regWrite32(base_addr + PHY_PARA9, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_get_bist_out(uint32_t base_addr, uint32_t *bist_out)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para9_tag *reg = getRegValuePtr(PHY_PARA9, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PHY_PARA9);
    *bist_out = reg->mBits.bist_out;

    return rtn;
}

int32_t dsi_phy_set_rcal_trim_init_time(uint32_t base_addr, uint32_t rcal_trim_init_time)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_trim_init_time = rcal_trim_init_time;
    regWrite32(base_addr + PHY_PARA10, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rcal_sel(uint32_t base_addr, uint32_t rcal_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_sel = rcal_sel;
    regWrite32(base_addr + PHY_PARA10, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rcal_pn_swap_i(uint32_t base_addr, uint32_t rcal_pn_swap_i)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_pn_swap_i = rcal_pn_swap_i;
    regWrite32(base_addr + PHY_PARA10, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rcal_en_i(uint32_t base_addr, uint32_t rcal_en_i)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_en_i = rcal_en_i;
    regWrite32(base_addr + PHY_PARA10, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rcal_soft_rst(uint32_t base_addr, uint32_t rcal_soft_rst)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_soft_rst = rcal_soft_rst;
    regWrite32(base_addr + PHY_PARA10, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rcal_trim_soft(uint32_t base_addr, uint32_t rcal_trim_soft)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_trim_soft = rcal_trim_soft;
    regWrite32(base_addr + PHY_PARA10, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_get_rcal_comp_out(uint32_t base_addr, uint32_t *rcal_comp_out)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PHY_PARA10);
    *rcal_comp_out = reg->mBits.rcal_comp_out;

    return rtn;
}

int32_t dsi_phy_get_rcal_done(uint32_t base_addr, uint32_t *rcal_done)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para10_tag *reg = getRegValuePtr(PHY_PARA10, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PHY_PARA10);
    *rcal_done = reg->mBits.rcal_done;

    return rtn;
}

int32_t dsi_phy_set_rcal_ctrl(uint32_t base_addr, uint32_t rcal_ctrl)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_ctrl = rcal_ctrl;
    regWrite32(base_addr + PHY_PARA11, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_ft_sel(uint32_t base_addr, uint32_t ft_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());

    reg->mBits.ft_sel = ft_sel;
    regWrite32(base_addr + PHY_PARA11, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lpcd_vref_trim(uint32_t base_addr, uint32_t lpcd_vref_trim)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());

    reg->mBits.lpcd_vref_trim = lpcd_vref_trim;
    regWrite32(base_addr + PHY_PARA11, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lprx_vref_trim(uint32_t base_addr, uint32_t lprx_vref_trim)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());

    reg->mBits.lprx_vref_trim = lprx_vref_trim;
    regWrite32(base_addr + PHY_PARA11, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_rg_ibias_sel(uint32_t base_addr, uint32_t rg_ibias_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info0_tag *reg = getRegValuePtr(PRBS_INFO0, dsi_phy_get_reg_default_tab());

    reg->mBits.rg_ibias_sel = rg_ibias_sel;
    regWrite32(base_addr + PRBS_INFO0, reg->dwValue);


    return rtn;
}

int32_t dsi_phy_set_rcal_done_bypass(uint32_t base_addr, uint32_t rcal_done_bypass)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_rdy_ctrl0_tag *reg = getRegValuePtr(PHY_RDY_CTRL0, dsi_phy_get_reg_default_tab());

    reg->mBits.rcal_done_bypass = rcal_done_bypass;
    regWrite32(base_addr + PHY_RDY_CTRL0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_rst2enlptx(uint32_t base_addr, uint32_t *rst2enlptx)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para0_tag *reg0 = getRegValuePtr(CLANE0_PARA0, dsi_phy_get_reg_default_tab());
    union dsi_clane_para0_tag *reg1 = getRegValuePtr(CLANE1_PARA0, dsi_phy_get_reg_default_tab());

    reg0->mBits.rst2enlptx = rst2enlptx[0];
    reg1->mBits.rst2enlptx = rst2enlptx[1];
    regWrite32(base_addr + CLANE0_PARA0, reg0->dwValue);
    regWrite32(base_addr + CLANE1_PARA0, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_inittime(uint32_t base_addr, uint32_t *inittime)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para1_tag *reg0 = getRegValuePtr(CLANE0_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_clane_para1_tag *reg1 = getRegValuePtr(CLANE1_PARA1, dsi_phy_get_reg_default_tab());

    reg0->mBits.inittime = inittime[0];
    reg1->mBits.inittime = inittime[1];
    regWrite32(base_addr + CLANE0_PARA1, reg0->dwValue);
    regWrite32(base_addr + CLANE1_PARA1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_clkprepare(uint32_t base_addr, uint32_t *clkprepare)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para2_tag *reg0 = getRegValuePtr(CLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_clane_para2_tag *reg1 = getRegValuePtr(CLANE1_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.clkprepare = clkprepare[0];
    reg1->mBits.clkprepare = clkprepare[1];
    regWrite32(base_addr + CLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + CLANE1_PARA2, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_clkzero(uint32_t base_addr, uint32_t *clkzero)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para2_tag *reg0 = getRegValuePtr(CLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_clane_para2_tag *reg1 = getRegValuePtr(CLANE1_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.clkzero = clkzero[0];
    reg1->mBits.clkzero = clkzero[1];
    regWrite32(base_addr + CLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + CLANE1_PARA2, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_clkpre(uint32_t base_addr, uint32_t *clkpre)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para2_tag *reg0 = getRegValuePtr(CLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_clane_para2_tag *reg1 = getRegValuePtr(CLANE1_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.clkpre = clkpre[0];
    reg1->mBits.clkpre = clkpre[1];
    regWrite32(base_addr + CLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + CLANE1_PARA2, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_clkpost(uint32_t base_addr, uint32_t *clkpost)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para3_tag *reg0 = getRegValuePtr(CLANE0_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_clane_para3_tag *reg1 = getRegValuePtr(CLANE1_PARA3, dsi_phy_get_reg_default_tab());

    reg0->mBits.clkpost = clkpost[0];
    reg1->mBits.clkpost = clkpost[1];
    regWrite32(base_addr + CLANE0_PARA3, reg0->dwValue);
    regWrite32(base_addr + CLANE0_PARA3, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_clktrail(uint32_t base_addr, uint32_t *clktrail)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para3_tag *reg0 = getRegValuePtr(CLANE0_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_clane_para3_tag *reg1 = getRegValuePtr(CLANE1_PARA3, dsi_phy_get_reg_default_tab());

    reg0->mBits.clktrail = clktrail[0];
    reg1->mBits.clktrail = clktrail[1];
    regWrite32(base_addr + CLANE0_PARA3, reg0->dwValue);
    regWrite32(base_addr + CLANE0_PARA3, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_hsexit(uint32_t base_addr, uint32_t *hsexit)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para3_tag *reg0 = getRegValuePtr(CLANE0_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_clane_para3_tag *reg1 = getRegValuePtr(CLANE1_PARA3, dsi_phy_get_reg_default_tab());

    reg0->mBits.hsexit = hsexit[0];
    reg1->mBits.hsexit = hsexit[1];
    regWrite32(base_addr + CLANE0_PARA3, reg0->dwValue);
    regWrite32(base_addr + CLANE1_PARA3, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_pulldwn_mode(uint32_t base_addr, uint32_t *pulldwn_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.pulldwn_mode_c0 = pulldwn_mode[0];
    reg->mBits.pulldwn_mode_c1 = pulldwn_mode[1];
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_clane_wakeup(uint32_t base_addr, uint32_t *wakeup)
{
    int32_t rtn = SUCCESS;
    union dsi_clane_para4tag *reg0 = getRegValuePtr(CLANE0_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_clane_para4tag *reg1 = getRegValuePtr(CLANE_PARA4, dsi_phy_get_reg_default_tab());

    reg0->mBits.t_wakeup_c0 = wakeup[0];
    reg0->mBits.t_wakeup_c0 = wakeup[1];
    regWrite32(base_addr + CLANE0_PARA4, reg0->dwValue);
    regWrite32(base_addr + CLANE_PARA4, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_rst2enlptx(uint32_t base_addr, uint32_t *rst2enlptx)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para0_tag *reg0 = getRegValuePtr(DLANE0_PARA0, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para0_tag *reg1 = getRegValuePtr(DLANE1_PARA0, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para0_tag *reg2 = getRegValuePtr(DLANE2_PARA0, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para0_tag *reg3 = getRegValuePtr(DLANE3_PARA0, dsi_phy_get_reg_default_tab());

    reg0->mBits.rst2enlptx = rst2enlptx[0];
    reg1->mBits.rst2enlptx = rst2enlptx[1];
    reg2->mBits.rst2enlptx = rst2enlptx[2];
    reg3->mBits.rst2enlptx = rst2enlptx[3];
    regWrite32(base_addr + DLANE0_PARA0, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA0, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA0, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA0, reg3->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_inittime(uint32_t base_addr, uint32_t *inittime)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para1_tag *reg0 = getRegValuePtr(DLANE0_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para1_tag *reg1 = getRegValuePtr(DLANE1_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para1_tag *reg2 = getRegValuePtr(DLANE2_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para1_tag *reg3 = getRegValuePtr(DLANE3_PARA1, dsi_phy_get_reg_default_tab());

    reg0->mBits.inittime = inittime[0];
    reg1->mBits.inittime = inittime[1];
    reg2->mBits.inittime = inittime[2];
    reg3->mBits.inittime = inittime[3];
    regWrite32(base_addr + DLANE0_PARA1, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA1, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA1, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA1, reg3->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_hsprepare(uint32_t base_addr, uint32_t *hsprepare)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para2_tag *reg0 = getRegValuePtr(DLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg1 = getRegValuePtr(DLANE1_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg2 = getRegValuePtr(DLANE2_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg3 = getRegValuePtr(DLANE3_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.hsprepare = hsprepare[0];
    reg1->mBits.hsprepare = hsprepare[1];
    reg2->mBits.hsprepare = hsprepare[2];
    reg3->mBits.hsprepare = hsprepare[3];
    regWrite32(base_addr + DLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA2, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA2, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA2, reg3->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_hszero(uint32_t base_addr, uint32_t *hszero)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para2_tag *reg0 = getRegValuePtr(DLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg1 = getRegValuePtr(DLANE1_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg2 = getRegValuePtr(DLANE2_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg3 = getRegValuePtr(DLANE3_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.hszero = hszero[0];
    reg1->mBits.hszero = hszero[1];
    reg2->mBits.hszero = hszero[2];
    reg3->mBits.hszero = hszero[3];
    regWrite32(base_addr + DLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA2, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA2, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA2, reg3->dwValue);

    return rtn;
}


int32_t dsi_phy_set_dlane_hstrail(uint32_t base_addr, uint32_t *hstrail)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para2_tag *reg0 = getRegValuePtr(DLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg1 = getRegValuePtr(DLANE1_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg2 = getRegValuePtr(DLANE2_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg3 = getRegValuePtr(DLANE3_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.hstrail = hstrail[0];
    reg1->mBits.hstrail = hstrail[1];
    reg2->mBits.hstrail = hstrail[2];
    reg3->mBits.hstrail = hstrail[3];
    regWrite32(base_addr + DLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA2, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA2, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA2, reg3->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_hsexit(uint32_t base_addr, uint32_t *hsexit)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para2_tag *reg0 = getRegValuePtr(DLANE0_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg1 = getRegValuePtr(DLANE1_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg2 = getRegValuePtr(DLANE2_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para2_tag *reg3 = getRegValuePtr(DLANE3_PARA2, dsi_phy_get_reg_default_tab());

    reg0->mBits.hsexit = hsexit[0];
    reg1->mBits.hsexit = hsexit[1];
    reg2->mBits.hsexit = hsexit[2];
    reg3->mBits.hsexit = hsexit[3];
    regWrite32(base_addr + DLANE0_PARA2, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA2, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA2, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA2, reg3->dwValue);

    return rtn;
}


int32_t dsi_phy_set_dlane_wakeup(uint32_t base_addr, uint32_t *wakeup)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para3_tag *reg0 = getRegValuePtr(DLANE0_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para3_tag *reg1 = getRegValuePtr(DLANE1_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para3_tag *reg2 = getRegValuePtr(DLANE2_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para3_tag *reg3 = getRegValuePtr(DLANE3_PARA3, dsi_phy_get_reg_default_tab());

    reg0->mBits.wakeup = wakeup[0];
    reg1->mBits.wakeup = wakeup[1];
    reg2->mBits.wakeup = wakeup[2];
    reg3->mBits.wakeup = wakeup[3];
    regWrite32(base_addr + DLANE0_PARA3, reg0->dwValue);
    regWrite32(base_addr + DLANE1_PARA3, reg1->dwValue);
    regWrite32(base_addr + DLANE2_PARA3, reg2->dwValue);
    regWrite32(base_addr + DLANE3_PARA3, reg3->dwValue);

    return rtn;
}


int32_t dsi_phy_set_dlane_tago(uint32_t base_addr, uint32_t *tago)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para4_tag *reg0 = getRegValuePtr(DLANE0_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para4_tag *reg2 = getRegValuePtr(DLANE2_PARA4, dsi_phy_get_reg_default_tab());

    reg0->mBits.tago = tago[0];
    reg2->mBits.tago = tago[2];
    regWrite32(base_addr + DLANE0_PARA4, reg0->dwValue);
    regWrite32(base_addr + DLANE2_PARA4, reg2->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_tasure(uint32_t base_addr, uint32_t *tasure)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para4_tag *reg0 = getRegValuePtr(DLANE0_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para4_tag *reg2 = getRegValuePtr(DLANE2_PARA4, dsi_phy_get_reg_default_tab());

    reg0->mBits.tasure = tasure[0];
    reg2->mBits.tasure = tasure[2];
    regWrite32(base_addr + DLANE0_PARA4, reg0->dwValue);
    regWrite32(base_addr + DLANE2_PARA4, reg2->dwValue);

    return rtn;
}

int32_t dsi_phy_set_dlane_taget(uint32_t base_addr, uint32_t *taget)
{
    int32_t rtn = SUCCESS;
    union dsi_dlane_para4_tag *reg0 = getRegValuePtr(DLANE0_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_dlane_para4_tag *reg2 = getRegValuePtr(DLANE2_PARA4, dsi_phy_get_reg_default_tab());

    reg0->mBits.taget = taget[0];
    reg2->mBits.taget = taget[2];
    regWrite32(base_addr + DLANE0_PARA4, reg0->dwValue);
    regWrite32(base_addr + DLANE2_PARA4, reg2->dwValue);

    return rtn;
}


int32_t dsi_phy_set_dlane_pulldwn_mode(uint32_t base_addr, uint32_t *pulldwn_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.pulldwn_mode_d0 = pulldwn_mode[0];
    reg->mBits.pulldwn_mode_d1 = pulldwn_mode[1];
    reg->mBits.pulldwn_mode_d2 = pulldwn_mode[2];
    reg->mBits.pulldwn_mode_d3 = pulldwn_mode[3];
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_get_dlane_init_deskew_done(uint32_t base_addr, uint32_t *init_deskew_done)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc0_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC0, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PCS_PHY_SYNC);

    init_deskew_done[0] = reg->mBits.d0_init_deskew_done;
    init_deskew_done[0] = reg->mBits.d1_init_deskew_done;
    init_deskew_done[0] = reg->mBits.d2_init_deskew_done;
    init_deskew_done[0] = reg->mBits.d3_init_deskew_done;

    return rtn;
}

int32_t dsi_phy_get_dlane_period_deskew_done(uint32_t base_addr, uint32_t *period_deskew_done)
{
    int32_t rtn = SUCCESS;
    union dsi_deskew_rcal_misc0_tag *reg = getRegValuePtr(DESKEW_RCAL_MISC0, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DESKEW_RCAL_MISC0);
    period_deskew_done[0] = reg->mBits.d0_period_deskew_done;
    period_deskew_done[0] = reg->mBits.d1_period_deskew_done;
    period_deskew_done[0] = reg->mBits.d2_period_deskew_done;
    period_deskew_done[0] = reg->mBits.d3_period_deskew_done;


    return rtn;
}

int32_t dsi_phy_set_lane_ck8t_reverse(uint32_t base_addr, uint32_t *ck8t_reverse)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());

    reg->mBits.ck8t_reverse_d0 = ck8t_reverse[0];
    regWrite32(base_addr + PHY_PARA14, reg->dwValue);


    return rtn;
}

int32_t dsi_phy_set_lane_predrv_by_dly(uint32_t base_addr, uint32_t *predrv_by_dly)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());
    union dsi_phy_para16_tag *reg2 = getRegValuePtr(PHY_PARA16, dsi_phy_get_reg_default_tab());

    reg0->mBits.predrv_by_dly_d0 = predrv_by_dly[0];
    reg2->mBits.predrv_by_dly_d1 = predrv_by_dly[1];
    reg2->mBits.predrv_by_dly_d2 = predrv_by_dly[2];
    reg1->mBits.predrv_by_dly_d0 = predrv_by_dly[3];
    reg2->mBits.predrv_by_dly_d4 = predrv_by_dly[4];
    reg2->mBits.predrv_by_dly_d5 = predrv_by_dly[5];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);
    regWrite32(base_addr + PHY_PARA16, reg2->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_predrv_dly_sel(uint32_t base_addr, uint32_t *predrv_dly_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());
    union dsi_phy_para16_tag *reg2 = getRegValuePtr(PHY_PARA16, dsi_phy_get_reg_default_tab());

    reg0->mBits.predrv_dly_sel_d0 = predrv_dly_sel[0];
    reg2->mBits.predrv_dly_sel_d1 = predrv_dly_sel[1];
    reg2->mBits.predrv_dly_sel_d2 = predrv_dly_sel[2];
    reg1->mBits.predrv_dly_sel_d0 = predrv_dly_sel[3];
    reg2->mBits.predrv_dly_sel_d4 = predrv_dly_sel[4];
    reg2->mBits.predrv_dly_sel_d5 = predrv_dly_sel[5];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);
    regWrite32(base_addr + PHY_PARA16, reg2->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_hstx_phase(uint32_t base_addr, uint32_t *hstx_phase)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());
    union dsi_phy_para16_tag *reg2 = getRegValuePtr(PHY_PARA16, dsi_phy_get_reg_default_tab());

    reg0->mBits.hstx_phase_d0 = hstx_phase[0];
    reg2->mBits.hstx_phase_d1 = hstx_phase[1];
    reg2->mBits.hstx_phase_d2 = hstx_phase[2];
    reg1->mBits.hstx_phase_d0 = hstx_phase[3];
    reg2->mBits.hstx_phase_d4 = hstx_phase[4];
    reg2->mBits.hstx_phase_d5 = hstx_phase[5];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);
    regWrite32(base_addr + PHY_PARA16, reg2->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_hs_8t_clk_sel(uint32_t base_addr, uint32_t *hs_8t_clk_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.dsi_0_hs_8t_clk_sel = hs_8t_clk_sel[0];
    reg->mBits.dsi_1_hs_8t_clk_sel = hs_8t_clk_sel[1];
    reg->mBits.dsi_2_hs_8t_clk_sel = hs_8t_clk_sel[2];
    reg->mBits.dsi_3_hs_8t_clk_sel = hs_8t_clk_sel[3];
    reg->mBits.dsi_4_hs_8t_clk_sel = hs_8t_clk_sel[4];
    reg->mBits.dsi_5_hs_8t_clk_sel = hs_8t_clk_sel[5];
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_lptx_vb_sel(uint32_t base_addr, uint32_t *lptx_vb_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info0_tag *reg = getRegValuePtr(PRBS_INFO0, dsi_phy_get_reg_default_tab());

    reg->mBits.dsi_0_lptx_vb_sel = lptx_vb_sel[0];
    reg->mBits.dsi_1_lptx_vb_sel = lptx_vb_sel[1];
    reg->mBits.dsi_2_lptx_vb_sel = lptx_vb_sel[2];
    reg->mBits.dsi_3_lptx_vb_sel = lptx_vb_sel[3];
    reg->mBits.dsi_4_lptx_vb_sel = lptx_vb_sel[4];
    reg->mBits.dsi_5_lptx_vb_sel = lptx_vb_sel[5];
    regWrite32(base_addr + PRBS_INFO0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_get_lane_bist_done(uint32_t base_addr, uint32_t *bist_done)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info0_tag *reg = getRegValuePtr(PRBS_INFO0, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PRBS_INFO0);
    bist_done[0] = reg->mBits.dsi_0_bist_done;
    bist_done[1] = reg->mBits.dsi_1_bist_done;
    bist_done[2] = reg->mBits.dsi_2_bist_done;
    bist_done[3] = reg->mBits.dsi_3_bist_done;
    bist_done[4] = reg->mBits.dsi_4_bist_done;
    bist_done[4] = reg->mBits.dsi_5_bist_done;

    return rtn;
}

int32_t dsi_phy_get_lane_bist_ok(uint32_t base_addr, uint32_t *bist_ok)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info0_tag *reg = getRegValuePtr(PRBS_INFO0, dsi_phy_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + PRBS_INFO0);
    bist_ok[0] = reg->mBits.dsi_0_bist_ok;
    bist_ok[1] = reg->mBits.dsi_1_bist_ok;
    bist_ok[2] = reg->mBits.dsi_2_bist_ok;
    bist_ok[3] = reg->mBits.dsi_3_bist_ok;
    bist_ok[4] = reg->mBits.dsi_4_bist_ok;
    bist_ok[5] = reg->mBits.dsi_5_bist_ok;

    return rtn;
}

int32_t dsi_phy_set_lane_bist_pattern_mode(uint32_t base_addr, uint32_t *bist_pattern_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info1_tag *reg = getRegValuePtr(PRBS_INFO1, dsi_phy_get_reg_default_tab());

    reg->mBits.dsi_0_bist_pattern_mode = bist_pattern_mode[0];
    reg->mBits.dsi_1_bist_pattern_mode = bist_pattern_mode[1];
    reg->mBits.dsi_2_bist_pattern_mode = bist_pattern_mode[2];
    reg->mBits.dsi_3_bist_pattern_mode = bist_pattern_mode[3];
    reg->mBits.dsi_4_bist_pattern_mode = bist_pattern_mode[4];
    reg->mBits.dsi_5_bist_pattern_mode = bist_pattern_mode[5];
    regWrite32(base_addr + PRBS_INFO1, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_prbs_check_num(uint32_t base_addr, uint32_t *prbs_check_num)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info1_tag *reg0 = getRegValuePtr(PRBS_INFO1, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info2_tag *reg1 = getRegValuePtr(PRBS_INFO2, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info3_tag *reg2 = getRegValuePtr(PRBS_INFO3, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info4_tag *reg3 = getRegValuePtr(PRBS_INFO4, dsi_phy_get_reg_default_tab());

    reg0->mBits.dsi_0_prbs_check_num = prbs_check_num[0];
    reg1->mBits.dsi_1_prbs_check_num = prbs_check_num[1];
    reg1->mBits.dsi_2_prbs_check_num = prbs_check_num[2];
    reg2->mBits.dsi_3_prbs_check_num = prbs_check_num[3];
    reg2->mBits.dsi_4_prbs_check_num = prbs_check_num[4];
    reg3->mBits.dsi_5_prbs_check_num = prbs_check_num[5];
    regWrite32(base_addr + PRBS_INFO1, reg0->dwValue);
    regWrite32(base_addr + PRBS_INFO2, reg1->dwValue);
    regWrite32(base_addr + PRBS_INFO3, reg2->dwValue);
    regWrite32(base_addr + PRBS_INFO4, reg3->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_prbs_seed(uint32_t base_addr, uint32_t *prbs_seed)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info5_tag *reg0 = getRegValuePtr(PRBS_INFO5, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info6_tag *reg1 = getRegValuePtr(PRBS_INFO6, dsi_phy_get_reg_default_tab());

    reg0->mBits.dsi_0_prbs_seed = prbs_seed[0];
    reg0->mBits.dsi_1_prbs_seed = prbs_seed[1];
    reg0->mBits.dsi_2_prbs_seed = prbs_seed[2];
    reg0->mBits.dsi_3_prbs_seed = prbs_seed[3];
    reg1->mBits.dsi_4_prbs_seed = prbs_seed[4];
    reg1->mBits.dsi_5_prbs_seed = prbs_seed[5];
    regWrite32(base_addr + PRBS_INFO5, reg0->dwValue);
    regWrite32(base_addr + PRBS_INFO6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_lane_prbs_err_th(uint32_t base_addr, uint32_t *prbs_err_th)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info6_tag *reg0 = getRegValuePtr(PRBS_INFO6, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info7_tag *reg1 = getRegValuePtr(PRBS_INFO7, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info4_tag *reg2 = getRegValuePtr(PRBS_INFO4, dsi_phy_get_reg_default_tab());

    reg0->mBits.dsi_0_prbs_err_th = prbs_err_th[0];
    reg0->mBits.dsi_1_prbs_err_th = prbs_err_th[1];
    reg1->mBits.dsi_2_prbs_err_th = prbs_err_th[2];
    reg1->mBits.dsi_3_prbs_err_th = prbs_err_th[3];
    reg1->mBits.dsi_4_prbs_err_th = prbs_err_th[4];
    reg2->mBits.dsi_5_prbs_err_th = prbs_err_th[5];
    regWrite32(base_addr + PRBS_INFO5, reg0->dwValue);
    regWrite32(base_addr + PRBS_INFO6, reg1->dwValue);
    regWrite32(base_addr + PRBS_INFO4, reg2->dwValue);

    return rtn;
}

int32_t dsi_phy_get_lane_prbs_err_num(uint32_t base_addr, uint32_t *prbs_err_num)
{
    int32_t rtn = SUCCESS;
    union dsi_prbs_info7_tag *reg0 = getRegValuePtr(PRBS_INFO7, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info8_tag *reg1 = getRegValuePtr(PRBS_INFO8, dsi_phy_get_reg_default_tab());
    union dsi_prbs_info4_tag *reg2 = getRegValuePtr(PRBS_INFO4, dsi_phy_get_reg_default_tab());

    reg0->dwValue = regRead32(base_addr + PRBS_INFO7);
    reg1->dwValue = regRead32(base_addr + PRBS_INFO8);
    reg2->dwValue = regRead32(base_addr + PRBS_INFO4);

    prbs_err_num[0] = reg0->mBits.dsi_0_prbs_err_num;
    prbs_err_num[1] = reg1->mBits.dsi_1_prbs_err_num;
    prbs_err_num[2] = reg1->mBits.dsi_2_prbs_err_num;
    prbs_err_num[3] = reg1->mBits.dsi_3_prbs_err_num;
    prbs_err_num[4] = reg1->mBits.dsi_4_prbs_err_num;
    prbs_err_num[5] = reg2->mBits.dsi_5_prbs_err_num;

    return rtn;
}

int32_t dsi_phy_set_core_dll_sel(uint32_t base_addr, uint32_t *dll_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.dll_sel_core0 = dll_sel[0];
    reg->mBits.dll_sel_core1 = dll_sel[1];
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);


    return rtn;
}

int32_t dsi_phy_set_core_lane_num(uint32_t base_addr, uint32_t *lane_num)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.lane_num_core0 = lane_num[0];
    reg->mBits.lane_num_core1 = lane_num[1];
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_clksel(uint32_t base_addr, uint32_t *clksel)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.core0_clksel = clksel[0];
    reg->mBits.core1_clksel = clksel[1];
    regWrite32(base_addr + MISC_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_phyerr_mask(uint32_t base_addr, uint32_t *phyerr_mask)
{
    int32_t rtn = SUCCESS;
    union dsi_misc_para0_tag *reg0 = getRegValuePtr(MISC_PARA0, dsi_phy_get_reg_default_tab());
    union dsi_misc_para1_tag *reg1 = getRegValuePtr(MISC_PARA1, dsi_phy_get_reg_default_tab());

    reg0->mBits.phyerr_mask_core0 = phyerr_mask[0];
    reg1->mBits.phyerr_mask_core1 = phyerr_mask[1];
    regWrite32(base_addr + MISC_PARA0, reg0->dwValue);
    regWrite32(base_addr + MISC_PARA1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_ulprx_sel(uint32_t base_addr, uint32_t *ulprx_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_pcs_phy_sync_tag *reg = getRegValuePtr(PCS_PHY_SYNC, dsi_phy_get_reg_default_tab());

    reg->mBits.ulprx_sel_core0 = ulprx_sel[0];
    reg->mBits.ulprx_sel_core1 = ulprx_sel[1];
    regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_dphy_soft_rst(uint32_t base_addr, uint32_t id, uint32_t *dphy_soft_rst)
{
    int32_t rtn = SUCCESS;
    union dsi_pcs_phy_sync_tag *reg = getRegValuePtr(PCS_PHY_SYNC, dsi_phy_get_reg_default_tab());

    if (0 == id) {
        reg->mBits.dphy_soft_rst_core0 = 1;//dphy_soft_rst[0];
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);

        reg->mBits.dphy_soft_rst_core0 = 0;//dphy_soft_rst[0];
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);
    } else if (1 == id) {
        reg->mBits.dphy_soft_rst_core1 = 1;//dphy_soft_rst[1];
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);

        reg->mBits.dphy_soft_rst_core1 = 0;//dphy_soft_rst[1];
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);
        regWrite32(base_addr + PCS_PHY_SYNC, reg->dwValue);
    }

    return rtn;
}

int32_t dsi_phy_set_core_pwon_sel(uint32_t base_addr, uint32_t *pwon_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    reg->mBits.pwon_sel_core0 = pwon_sel[0];
    reg->mBits.pwon_sel_core1 = pwon_sel[1];
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    reg->mBits.pwon_sel_core0 = 0;
    reg->mBits.pwon_sel_core1 = 0;
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pwon_dsi_soft(uint32_t base_addr, uint32_t id, uint32_t *pwon_dsi_soft)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    if (0 == id) {
        reg->mBits.pwon_dsi_core0_soft = pwon_dsi_soft[0];
    } else if (1 == id) {
        reg->mBits.pwon_dsi_core1_soft = pwon_dsi_soft[1];
    }
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    //reg->mBits.pwon_dsi_core0_soft = 0;
    //reg->mBits.pwon_dsi_core1_soft = 0;
    //regWrite32(base_addr + PHY_PARA0, reg->dwValue);
    //regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pwon_pll_soft(uint32_t base_addr, uint32_t id, uint32_t *pwon_pll_soft)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para0_tag *reg = getRegValuePtr(PHY_PARA0, dsi_phy_get_reg_default_tab());

    if (0 == id) {
        reg->mBits.pwon_pll0_soft = pwon_pll_soft[0];
    } else if (1 == id) {
        reg->mBits.pwon_pll1_soft = pwon_pll_soft[1];
    }

    regWrite32(base_addr + PHY_PARA0, reg->dwValue);
    regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    //reg->mBits.pwon_pll0_soft = 0;
    //reg->mBits.pwon_pll1_soft = 0;
    //regWrite32(base_addr + PHY_PARA0, reg->dwValue);
    //regWrite32(base_addr + PHY_PARA0, reg->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_da_clk1_en(uint32_t base_addr, uint32_t *da_clk_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para1_tag *reg0 = getRegValuePtr(PHY_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_phy_para1_tag *reg1 = getRegValuePtr(PHY_PARA5, dsi_phy_get_reg_default_tab());

    reg0->mBits.da_clk0_en = da_clk_en[0];
    reg1->mBits.da_clk0_en = da_clk_en[1];
    regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA5, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pixelclkl_div(uint32_t base_addr, uint32_t *pixelclk_div)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para1_tag *reg0 = getRegValuePtr(PHY_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_phy_para1_tag *reg1 = getRegValuePtr(PHY_PARA5, dsi_phy_get_reg_default_tab());

    reg0->mBits.pixelclk0_div = pixelclk_div[0];
    reg1->mBits.pixelclk0_div = pixelclk_div[1];
    regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA5, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_soft_rst(uint32_t base_addr, uint32_t id, uint32_t *pll_soft_rst)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para1_tag *reg0 = getRegValuePtr(PHY_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_phy_para1_tag *reg1 = getRegValuePtr(PHY_PARA5, dsi_phy_get_reg_default_tab());

    if (0 == id) {
        reg0->mBits.pll_soft_rst = pll_soft_rst[0];
        regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
        regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    } else if (1 == id) {
        reg1->mBits.pll_soft_rst = pll_soft_rst[1];
        regWrite32(base_addr + PHY_PARA5, reg1->dwValue);
        regWrite32(base_addr + PHY_PARA5, reg1->dwValue);
    }

    //reg0->mBits.pll_soft_rst = 0;
    //reg1->mBits.pll_soft_rst = 0;
    //regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    //regWrite32(base_addr + PHY_PARA5, reg0->dwValue);
    //regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    //regWrite32(base_addr + PHY_PARA5, reg0->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_bist_en(uint32_t base_addr, uint32_t *pll_bist_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para1_tag *reg0 = getRegValuePtr(PHY_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_phy_para1_tag *reg1 = getRegValuePtr(PHY_PARA5, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_bist_en = pll_bist_en[0];
    reg1->mBits.pll_bist_en = pll_bist_en[1];
    regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA5, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_bist_ctrl(uint32_t base_addr, uint32_t *pll_bist_ctrl)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para1_tag *reg0 = getRegValuePtr(PHY_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_phy_para1_tag *reg1 = getRegValuePtr(PHY_PARA5, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_bist_ctrl = pll_bist_ctrl[0];
    reg1->mBits.pll_bist_ctrl = pll_bist_ctrl[1];
    regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA5, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_bist_cnt(uint32_t base_addr, uint32_t *pll_bist_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para1_tag *reg0 = getRegValuePtr(PHY_PARA1, dsi_phy_get_reg_default_tab());
    union dsi_phy_para1_tag *reg1 = getRegValuePtr(PHY_PARA5, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_bist_cnt = pll_bist_cnt[0];
    reg1->mBits.pll_bist_cnt = pll_bist_cnt[1];
    regWrite32(base_addr + PHY_PARA1, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA5, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_div_s(uint32_t base_addr, uint32_t *pll_div_s)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_div_s = pll_div_s[0];
    reg1->mBits.pll_div_s = pll_div_s[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_freq_doubler_en(uint32_t base_addr, uint32_t *pll_freq_doubler_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_freq_doubler_en = pll_freq_doubler_en[0];
    reg1->mBits.pll_freq_doubler_en = pll_freq_doubler_en[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_ref24m_sel(uint32_t base_addr, uint32_t *pll_ref24m_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_ref24m_sel = pll_ref24m_sel[0];
    reg1->mBits.pll_ref24m_sel = pll_ref24m_sel[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_pdiv(uint32_t base_addr, uint32_t *pll_pdiv)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_pdiv = pll_pdiv[0];
    reg1->mBits.pll_pdiv = pll_pdiv[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_ssc_en(uint32_t base_addr, uint32_t *pll_ssc_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_ssc_en = pll_ssc_en[0];
    reg1->mBits.pll_ssc_en = pll_ssc_en[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_ssc_ctrl(uint32_t base_addr, uint32_t *pll_ssc_ctrl)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_ssc_ctrl = pll_ssc_ctrl[0];
    reg1->mBits.pll_ssc_ctrl = pll_ssc_ctrl[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_lpf_r2(uint32_t base_addr, uint32_t *pll_lpf_r2)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_lpf_r2 = pll_lpf_r2[0];
    reg1->mBits.pll_lpf_r2 = pll_lpf_r2[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_lpf_r3(uint32_t base_addr, uint32_t *pll_lpf_r3)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_lpf_r3 = pll_lpf_r3[0];
    reg1->mBits.pll_lpf_r3 = pll_lpf_r3[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_lpf_cl(uint32_t base_addr, uint32_t *pll_lpf_c1)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_lpf_c1 = pll_lpf_c1[0];
    reg1->mBits.pll_lpf_c1 = pll_lpf_c1[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_lpf_c2(uint32_t base_addr, uint32_t *pll_lpf_c2)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_lpf_c2 = pll_lpf_c2[0];
    reg1->mBits.pll_lpf_c2 = pll_lpf_c2[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_ldo_test_en(uint32_t base_addr, uint32_t *pll_ldo_test_en)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_ldo_test_en = pll_ldo_test_en[0];
    reg1->mBits.pll_ldo_test_en = pll_ldo_test_en[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_ldo_vref(uint32_t base_addr, uint32_t *pll_ldo_vref)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_ldo_vref = pll_ldo_vref[0];
    reg1->mBits.pll_ldo_vref = pll_ldo_vref[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_ldo_vout(uint32_t base_addr, uint32_t *pll_ldo_vout)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para2_tag *reg0 = getRegValuePtr(PHY_PARA2, dsi_phy_get_reg_default_tab());
    union dsi_phy_para2_tag *reg1 = getRegValuePtr(PHY_PARA6, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_ldo_vout = pll_ldo_vout[0];
    reg1->mBits.pll_ldo_vout = pll_ldo_vout[1];
    regWrite32(base_addr + PHY_PARA2, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA6, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_kint(uint32_t base_addr, uint32_t *pll_kint)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para3_tag *reg0 = getRegValuePtr(PHY_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_phy_para3_tag *reg1 = getRegValuePtr(PHY_PARA7, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_kint = pll_kint[0];
    reg1->mBits.pll_kint = pll_kint[1];

    regWrite32(base_addr + PHY_PARA3, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA7, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_nint(uint32_t base_addr, uint32_t *pll_nint)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para3_tag *reg0 = getRegValuePtr(PHY_PARA3, dsi_phy_get_reg_default_tab());
    union dsi_phy_para3_tag *reg1 = getRegValuePtr(PHY_PARA7, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_nint = pll_nint[0];
    reg1->mBits.pll_nint = pll_nint[1];
    regWrite32(base_addr + PHY_PARA3, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA7, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_n(uint32_t base_addr, uint32_t *pll_n)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para4_tag *reg0 = getRegValuePtr(PHY_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_phy_para4_tag *reg1 = getRegValuePtr(PHY_PARA8, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_n = pll_n[0];
    reg1->mBits.pll_n = pll_n[1];
    regWrite32(base_addr + PHY_PARA4, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA8, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_pll_icp(uint32_t base_addr, uint32_t *pll_icp)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para4_tag *reg0 = getRegValuePtr(PHY_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_phy_para4_tag *reg1 = getRegValuePtr(PHY_PARA8, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_icp = pll_icp[0];
    reg1->mBits.pll_icp = pll_icp[1];
    regWrite32(base_addr + PHY_PARA4, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA8, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_icp_of_fset(uint32_t base_addr, uint32_t *pll_icp_offset)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para4_tag *reg0 = getRegValuePtr(PHY_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_phy_para4_tag *reg1 = getRegValuePtr(PHY_PARA8, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_icp_offset = pll_icp_offset[0];
    reg1->mBits.pll_icp_offset = pll_icp_offset[1];
    regWrite32(base_addr + PHY_PARA4, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA8, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_get_core_pll_lock(uint32_t base_addr, uint32_t *pll_lock)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para4_tag *reg0 = getRegValuePtr(PHY_PARA4, dsi_phy_get_reg_default_tab());
    union dsi_phy_para4_tag *reg1 = getRegValuePtr(PHY_PARA8, dsi_phy_get_reg_default_tab());

    reg0->dwValue = regRead32(base_addr + PHY_PARA4);
    reg1->dwValue = regRead32(base_addr + PHY_PARA8);

    pll_lock[0] = reg0->mBits.pll_lock;
    pll_lock[1] = reg1->mBits.pll_lock;

    return rtn;
}

int32_t dsi_phy_set_core_rg_dslpll_ssc_step_wicith_ctrl(uint32_t base_addr, uint32_t *rg_dslpll_ssc_step_wicith_ctrl)
{
    int32_t rtn = SUCCESS;
    union dsi_ssc_ctrl_tag *reg0 = getRegValuePtr(SSC_CTRL0, dsi_phy_get_reg_default_tab());
    union dsi_ssc_ctrl_tag *reg1 = getRegValuePtr(SSC_CTRL1, dsi_phy_get_reg_default_tab());

    reg0->mBits.rg_dslpll_ssc_step_wicith_ctrl = rg_dslpll_ssc_step_wicith_ctrl[0];
    reg1->mBits.rg_dslpll_ssc_step_wicith_ctrl = rg_dslpll_ssc_step_wicith_ctrl[1];
    regWrite32(base_addr + SSC_CTRL0, reg0->dwValue);
    regWrite32(base_addr + SSC_CTRL1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_rg_dslpll_ssc_step_number_ctrl(uint32_t base_addr, uint32_t *rg_dslpll_ssc_step_number_ctrl)
{
    int32_t rtn = SUCCESS;
    union dsi_ssc_ctrl_tag *reg0 = getRegValuePtr(SSC_CTRL0, dsi_phy_get_reg_default_tab());
    union dsi_ssc_ctrl_tag *reg1 = getRegValuePtr(SSC_CTRL1, dsi_phy_get_reg_default_tab());

    reg0->mBits.rg_dslpll_ssc_step_number_ctrl = rg_dslpll_ssc_step_number_ctrl[0];
    reg1->mBits.rg_dslpll_ssc_step_number_ctrl = rg_dslpll_ssc_step_number_ctrl[1];
    regWrite32(base_addr + SSC_CTRL0, reg0->dwValue);
    regWrite32(base_addr + SSC_CTRL1, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_lock_bypass(uint32_t base_addr, uint32_t *pll_lock_bypass)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_rdy_ctrl0_tag *reg0 = getRegValuePtr(PHY_RDY_CTRL0, dsi_phy_get_reg_default_tab());
    union dsi_phy_rdy_ctrl1_tag *reg1 = getRegValuePtr(PHY_RDY_CTRL1, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll0_lock_bypass = pll_lock_bypass[0];
    reg1->mBits.pll1_lock_bypass = pll_lock_bypass[1];
    regWrite32(base_addr + PHY_RDY_CTRL0, reg0->dwValue);
    regWrite32(base_addr + PHY_RDY_CTRL1, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_lock_time(uint32_t base_addr, uint32_t *pll_lock_time)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_rdy_ctrl0_tag *reg0 = getRegValuePtr(PHY_RDY_CTRL0, dsi_phy_get_reg_default_tab());
    union dsi_phy_rdy_ctrl1_tag *reg1 = getRegValuePtr(PHY_RDY_CTRL1, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll0_lock_time = pll_lock_time[0];
    reg1->mBits.pll1_lock_time = pll_lock_time[1];
    regWrite32(base_addr + PHY_RDY_CTRL0, reg0->dwValue);
    regWrite32(base_addr + PHY_RDY_CTRL1, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_pll_lock_cnt(uint32_t base_addr, uint32_t *pll_lock_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_rdy_info_tag *reg0 = getRegValuePtr(PHY_RDY_INFO0, dsi_phy_get_reg_default_tab());
    union dsi_phy_rdy_info_tag *reg1 = getRegValuePtr(PHY_RDY_INFO1, dsi_phy_get_reg_default_tab());

    reg0->mBits.pll_lock_cnt = pll_lock_cnt[0];
    reg1->mBits.pll_lock_cnt = pll_lock_cnt[1];
    regWrite32(base_addr + PHY_RDY_INFO0, reg0->dwValue);
    regWrite32(base_addr + PHY_RDY_INFO1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_hstx_sr(uint32_t base_addr, uint32_t *hstx_sr)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg0 = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());
    union dsi_phy_para12_tag *reg1 = getRegValuePtr(PHY_PARA12, dsi_phy_get_reg_default_tab());

    reg0->mBits.core0_hstx_sr = hstx_sr[0];
    reg1->mBits.core1_hstx_sr = hstx_sr[1];
    regWrite32(base_addr + PHY_PARA11, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA12, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_hstx_amp_trim(uint32_t base_addr, uint32_t *hstx_amp_trim)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg0 = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());
    union dsi_phy_para12_tag *reg1 = getRegValuePtr(PHY_PARA12, dsi_phy_get_reg_default_tab());

    reg0->mBits.core0_hstx_amp_trim = hstx_amp_trim[0];
    reg1->mBits.core1_hstx_amp_trim = hstx_amp_trim[1];
    regWrite32(base_addr + PHY_PARA11, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA12, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_lptx_sr_trim(uint32_t base_addr, uint32_t *lptx_sr_trim)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg0 = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());
    union dsi_phy_para12_tag *reg1 = getRegValuePtr(PHY_PARA12, dsi_phy_get_reg_default_tab());

    reg0->mBits.core0_lptx_sr_trim = lptx_sr_trim[0];
    reg1->mBits.core1_lptx_sr_trim = lptx_sr_trim[1];
    regWrite32(base_addr + PHY_PARA11, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA12, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_lptx_amp_trim(uint32_t base_addr, uint32_t *lptx_amp_trim)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para11_tag *reg0 = getRegValuePtr(PHY_PARA11, dsi_phy_get_reg_default_tab());
    union dsi_phy_para12_tag *reg1 = getRegValuePtr(PHY_PARA12, dsi_phy_get_reg_default_tab());

    reg0->mBits.core0_lptx_amp_trim = lptx_amp_trim[0];
    reg1->mBits.core1_lptx_amp_trim = lptx_amp_trim[1];
    regWrite32(base_addr + PHY_PARA11, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA12, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_en_lpcd(uint32_t base_addr, uint32_t *en_lpcd)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->mBits.en_lpcd_d0 = en_lpcd[0];
    reg1->mBits.en_lpcd_d0 = en_lpcd[1];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_en_lprx(uint32_t base_addr, uint32_t *en_lprx)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->mBits.en_lprx_d0 = en_lprx[0];
    reg1->mBits.en_lprx_d0 = en_lprx[1];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_core_en_ulprx(uint32_t base_addr, uint32_t *en_ulprx)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->mBits.en_ulprx_d0 = en_ulprx[0];
    reg1->mBits.en_ulprx_d0 = en_ulprx[1];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_lpcd_fl0(uint32_t base_addr, uint32_t *lpcd_fl0)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->mBits.lpcd_fl0_d0 = lpcd_fl0[0];
    reg1->mBits.lpcd_fl0_d0 = lpcd_fl0[1];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_core_lpcd_fl1(uint32_t base_addr, uint32_t *lpcd_fl1)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->mBits.lpcd_fl1_d0 = lpcd_fl1[0];
    reg1->mBits.lpcd_fl1_d0 = lpcd_fl1[1];
    regWrite32(base_addr + PHY_PARA14, reg0->dwValue);
    regWrite32(base_addr + PHY_PARA15, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_get_core_lprx_vinit(uint32_t base_addr, uint32_t *lprx_vinit)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->dwValue = regRead32(base_addr + PHY_PARA14);
    reg1->dwValue = regRead32(base_addr + PHY_PARA15);

    lprx_vinit[0] = reg0->mBits.lprx_vinit_d0;
    lprx_vinit[1] = reg1->mBits.lprx_vinit_d0;

    return rtn;
}

int32_t dsi_phy_get_core_lpcd_vinit(uint32_t base_addr, uint32_t *lpcd_vinit)
{
    int32_t rtn = SUCCESS;
    union dsi_phy_para14_tag *reg0 = getRegValuePtr(PHY_PARA14, dsi_phy_get_reg_default_tab());
    union dsi_phy_para14_tag *reg1 = getRegValuePtr(PHY_PARA15, dsi_phy_get_reg_default_tab());

    reg0->dwValue = regRead32(base_addr + PHY_PARA14);
    reg1->dwValue = regRead32(base_addr + PHY_PARA15);

    lpcd_vinit[0] = reg0->mBits.lpcd_vinit_d0;
    lpcd_vinit[1] = reg1->mBits.lpcd_vinit_d0;

    return rtn;
}


int32_t dsi_phy_set_mode_ext_loop_mode_cfg(uint32_t base_addr, uint32_t *ext_loop_mode_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.ext_loop_mode_cfg = ext_loop_mode_cfg[0];
    reg1->mBits.ext_loop_mode_cfg = ext_loop_mode_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_en_ext_loop_cfg(uint32_t base_addr, uint32_t *en_ext_loop_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.en_ext_loop_cfg = en_ext_loop_cfg[0];
    reg1->mBits.en_ext_loop_cfg = en_ext_loop_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_mode_txclkreq_loop_cfg(uint32_t base_addr, uint32_t *txclkreq_loop_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.txclkreq_loop_cfg = txclkreq_loop_cfg[0];
    reg1->mBits.txclkreq_loop_cfg = txclkreq_loop_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_txdatareq_loop_cfg(uint32_t base_addr, uint32_t *txdatareq_loop_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.txdatareq_loop_cfg = txdatareq_loop_cfg[0];
    reg1->mBits.txdatareq_loop_cfg = txdatareq_loop_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_txdatasel_loop_cfg(uint32_t base_addr, uint32_t *txdatasel_loop_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.txdatasel_loop_cfg = txdatasel_loop_cfg[0];
    reg1->mBits.txdatasel_loop_cfg = txdatasel_loop_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_txdata_loop_cfg(uint32_t base_addr, uint32_t *txdata_loop_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.txdata_loop_cfg = txdata_loop_cfg[0];
    reg1->mBits.txdata_loop_cfg = txdata_loop_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}


int32_t dsi_phy_set_mode_txdatanum_loop_cfg(uint32_t base_addr, uint32_t *txdatanum_loop_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.txdatanum_loop_cfg = txdatanum_loop_cfg[0];
    reg1->mBits.txdatanum_loop_cfg = txdatanum_loop_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_ext_loop_bist_bit_error_cfg(uint32_t base_addr, uint32_t *ext_loop_bist_bit_error_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.ext_loop_bist_bit_error_cfg = ext_loop_bist_bit_error_cfg[0];
    reg1->mBits.ext_loop_bist_bit_error_cfg = ext_loop_bist_bit_error_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_ext_loop_prbs_pattern_mode_cfg(uint32_t base_addr, uint32_t *ext_loop_prbs_pattern_mode_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.ext_loop_prbs_pattern_mode_cfg = ext_loop_prbs_pattern_mode_cfg[0];
    reg1->mBits.ext_loop_prbs_pattern_mode_cfg = ext_loop_prbs_pattern_mode_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

int32_t dsi_phy_set_mode_ext_loop_prbs_seed_cfg(uint32_t base_addr, uint32_t *ext_loop_prbs_seed_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_ext_loop_cfg_tag *reg0 = getRegValuePtr(EXT_LOOP_CFG0, dsi_phy_get_reg_default_tab());
    union dsi_ext_loop_cfg_tag *reg1 = getRegValuePtr(EXT_LOOP_CFG1, dsi_phy_get_reg_default_tab());

    reg0->mBits.ext_loop_prbs_seed_cfg = ext_loop_prbs_seed_cfg[0];
    reg1->mBits.ext_loop_prbs_seed_cfg = ext_loop_prbs_seed_cfg[1];
    regWrite32(base_addr + EXT_LOOP_CFG0, reg0->dwValue);
    regWrite32(base_addr + EXT_LOOP_CFG1, reg1->dwValue);

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

