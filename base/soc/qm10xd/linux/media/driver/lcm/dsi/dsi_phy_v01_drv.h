#ifndef _DSI_PHY_V01_DRV_H_
#define _DSI_PHY_V01_DRV_H_

#include "dsi_phy_v01_drv_param.h"
#include "dsi_com_param.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/

int32_t dsi_phy_set_lpx(uint32_t base_addr, uint32_t lpx);
int32_t dsi_phy_set_txreadyesc_mode(uint32_t base_addr, uint32_t txreadyesc_mode);
int32_t dsi_phy_set_auto_ckgate_en(uint32_t base_addr, uint32_t auto_ckgate_en);
int32_t dsi_phy_set_link_mode(uint32_t base_addr, uint32_t phy_link_mode);
int32_t dsi_phy_set_txreadyesc_extend_vld(uint32_t base_addr, uint32_t txreadyesc_extend_vld);
int32_t dsi_phy_set_rxvalidesc_extend_vld(uint32_t base_addr, uint32_t rxvalidesc_extend_vld);
int32_t dsi_phy_get_afifo_info(uint32_t base_addr, uint32_t *afifo_info);
int32_t dsi_phy_set_afifo_cfg(uint32_t base_addr, uint32_t afifo_cfg);
int32_t dsi_phy_set_rcal_pluse_period(uint32_t base_addr, uint32_t rcal_pluse_period);
int32_t dsi_phy_set_t_ipcd_det_time_d0(uint32_t base_addr, uint32_t t_ipcd_det_time_d0);
int32_t dsi_phy_set_en_deskew(uint32_t base_addr, uint32_t en_deskew);
int32_t dsi_phy_set_en_period_deskew(uint32_t base_addr, uint32_t en_period_deskew);
int32_t dsi_phy_set_init_deskew_time(uint32_t base_addr, uint32_t init_deskew_time);
int32_t dsi_phy_set_period_deskew_time(uint32_t base_addr, uint32_t period_deskew_time);
int32_t dsi_phy_set_cl_en_deskew(uint32_t base_addr, uint32_t cl_en_deskew);
int32_t dsi_phy_get_cl_init_deskew_done(uint32_t base_addr, uint32_t *cl_init_deskew_done);
int32_t dsi_phy_set_cl_deskew_time(uint32_t base_addr, uint32_t cl_deskew_time);
int32_t dsi_phy_set_su_iddq_en_soft(uint32_t base_addr, uint32_t su_iddq_en_soft);
int32_t dsi_phy_set_atest_en(uint32_t base_addr, uint32_t atest_en);
int32_t dsi_phy_set_atest_sel(uint32_t base_addr, uint32_t atest_sel);
int32_t dsi_phy_set_fset_en(uint32_t base_addr, uint32_t fset_en);
int32_t dsi_phy_set_bg_en(uint32_t base_addr, uint32_t bg_en);
int32_t dsi_phy_set_bg_rbias(uint32_t base_addr, uint32_t bg_rbias);
int32_t dsi_phy_set_bg_trim(uint32_t base_addr, uint32_t bg_trim);
int32_t dsi_phy_get_vbg_rdy(uint32_t base_addr, uint32_t *vbg_rdy);
int32_t dsi_phy_set_bist_en(uint32_t base_addr, uint32_t bist_en);
int32_t dsi_phy_set_bist_sel(uint32_t base_addr, uint32_t bist_sel);
int32_t dsi_phy_set_bist_bit_error(uint32_t base_addr, uint32_t bist_bit_error);
int32_t dsi_phy_get_bist_out(uint32_t base_addr, uint32_t *bist_out);
int32_t dsi_phy_set_rcal_trim_init_time(uint32_t base_addr, uint32_t rcal_trim_init_time);
int32_t dsi_phy_set_rcal_sel(uint32_t base_addr, uint32_t rcal_sel);
int32_t dsi_phy_set_rcal_pn_swap_i(uint32_t base_addr, uint32_t rcal_pn_swap_i);
int32_t dsi_phy_set_rcal_en_i(uint32_t base_addr, uint32_t rcal_en_i);
int32_t dsi_phy_set_rcal_soft_rst(uint32_t base_addr, uint32_t rcal_soft_rst);
int32_t dsi_phy_set_rcal_trim_soft(uint32_t base_addr, uint32_t rcal_trim_soft);
int32_t dsi_phy_get_rcal_comp_out(uint32_t base_addr, uint32_t *rcal_comp_out);
int32_t dsi_phy_get_rcal_done(uint32_t base_addr, uint32_t *rcal_done);
int32_t dsi_phy_set_rcal_ctrl(uint32_t base_addr, uint32_t rcal_ctrl);
int32_t dsi_phy_set_ft_sel(uint32_t base_addr, uint32_t ft_sel);
int32_t dsi_phy_set_lpcd_vref_trim(uint32_t base_addr, uint32_t lpcd_vref_trim);
int32_t dsi_phy_set_lprx_vref_trim(uint32_t base_addr, uint32_t lprx_vref_trim);
int32_t dsi_phy_set_rg_ibias_sel(uint32_t base_addr, uint32_t rg_ibias_sel);
int32_t dsi_phy_set_rcal_done_bypass(uint32_t base_addr, uint32_t rcal_done_bypass);
int32_t dsi_phy_set_clane_rst2enlptx(uint32_t base_addr, uint32_t *rst2enlptx);
int32_t dsi_phy_set_clane_inittime(uint32_t base_addr, uint32_t *inittime);
int32_t dsi_phy_set_clane_clkprepare(uint32_t base_addr, uint32_t *clkprepare);
int32_t dsi_phy_set_clane_clkzero(uint32_t base_addr, uint32_t *clkzero);
int32_t dsi_phy_set_clane_clkpre(uint32_t base_addr, uint32_t *clkpre);
int32_t dsi_phy_set_clane_clkpost(uint32_t base_addr, uint32_t *clkpost);
int32_t dsi_phy_set_clane_clktrail(uint32_t base_addr, uint32_t *clktrail);
int32_t dsi_phy_set_clane_hsexit(uint32_t base_addr, uint32_t *hsexit);
int32_t dsi_phy_set_clane_pulldwn_mode(uint32_t base_addr, uint32_t *pulldwn_mode);
int32_t dsi_phy_set_clane_wakeup(uint32_t base_addr, uint32_t *wakeup);
int32_t dsi_phy_set_dlane_rst2enlptx(uint32_t base_addr, uint32_t *rst2enlptx);
int32_t dsi_phy_set_dlane_inittime(uint32_t base_addr, uint32_t *inittime);
int32_t dsi_phy_set_dlane_hsprepare(uint32_t base_addr, uint32_t *hsprepare);
int32_t dsi_phy_set_dlane_hszero(uint32_t base_addr, uint32_t *hszero);
int32_t dsi_phy_set_dlane_hstrail(uint32_t base_addr, uint32_t *hstrail);
int32_t dsi_phy_set_dlane_hsexit(uint32_t base_addr, uint32_t *hsexit);
int32_t dsi_phy_set_dlane_wakeup(uint32_t base_addr, uint32_t *wakeup);
int32_t dsi_phy_set_dlane_tago(uint32_t base_addr, uint32_t *tago);
int32_t dsi_phy_set_dlane_tasure(uint32_t base_addr, uint32_t *tasure);
int32_t dsi_phy_set_dlane_taget(uint32_t base_addr, uint32_t *taget);
int32_t dsi_phy_set_dlane_pulldwn_mode(uint32_t base_addr, uint32_t *pulldwn_mode);
int32_t dsi_phy_get_dlane_init_deskew_done(uint32_t base_addr, uint32_t *inideskew_done);
int32_t dsi_phy_get_dlane_period_deskew_done(uint32_t base_addr, uint32_t *period_deskew_done);
int32_t dsi_phy_set_lane_ck8t_reverse(uint32_t base_addr, uint32_t *ck8t_reverse);
int32_t dsi_phy_set_lane_predrv_by_dly(uint32_t base_addr, uint32_t *predrv_by_dly);
int32_t dsi_phy_set_lane_predrv_dly_sel(uint32_t base_addr, uint32_t *predrv_dly_sel);
int32_t dsi_phy_set_lane_hstx_phase(uint32_t base_addr, uint32_t *hstx_phase);
int32_t dsi_phy_set_lane_hs_8t_clk_sel(uint32_t base_addr, uint32_t *hs_8t_clk_sel);
int32_t dsi_phy_set_lane_lptx_vb_sel(uint32_t base_addr, uint32_t *lptx_vb_sel);
int32_t dsi_phy_get_lane_bist_done(uint32_t base_addr, uint32_t *bist_done);
int32_t dsi_phy_get_lane_bist_ok(uint32_t base_addr, uint32_t *bist_ok);
int32_t dsi_phy_set_lane_bist_pattern_mode(uint32_t base_addr, uint32_t *bist_pattern_mode);
int32_t dsi_phy_set_lane_prbs_check_num(uint32_t base_addr, uint32_t *prbs_check_num);
int32_t dsi_phy_set_lane_prbs_seed(uint32_t base_addr, uint32_t *prbs_seed);
int32_t dsi_phy_set_lane_prbs_err_th(uint32_t base_addr, uint32_t *prbs_err_th);
int32_t dsi_phy_get_lane_prbs_err_num(uint32_t base_addr, uint32_t *prbs_err_num);
int32_t dsi_phy_set_core_dll_sel(uint32_t base_addr, uint32_t *dll_sel);
int32_t dsi_phy_set_core_lane_num(uint32_t base_addr, uint32_t *lane_num);
int32_t dsi_phy_set_core_clksel(uint32_t base_addr, uint32_t *clksel);
int32_t dsi_phy_set_core_phyerr_mask(uint32_t base_addr, uint32_t *phyerr_mask);
int32_t dsi_phy_set_core_ulprx_sel(uint32_t base_addr, uint32_t *ulprx_sel);
int32_t dsi_phy_set_core_dphy_soft_rst(uint32_t base_addr, uint32_t id, uint32_t *dphy_soft_rst);
int32_t dsi_phy_set_core_pwon_sel(uint32_t base_addr, uint32_t *pwon_sel);
int32_t dsi_phy_set_core_pwon_dsi_soft(uint32_t base_addr, uint32_t id, uint32_t *pwon_dsi_soft);
int32_t dsi_phy_set_core_pwon_pll_soft(uint32_t base_addr, uint32_t id, uint32_t *pwon_pll_soft);
int32_t dsi_phy_set_core_da_clk1_en(uint32_t base_addr, uint32_t *da_clk_en);
int32_t dsi_phy_set_core_pixelclkl_div(uint32_t base_addr, uint32_t *pixelclk_div);
int32_t dsi_phy_set_core_pll_soft_rst(uint32_t base_addr, uint32_t id, uint32_t *pll_soft_rst);
int32_t dsi_phy_set_core_pll_bist_en(uint32_t base_addr, uint32_t *pll_bist_en);
int32_t dsi_phy_set_core_pll_bist_ctrl(uint32_t base_addr, uint32_t *pll_bist_ctrl);
int32_t dsi_phy_set_core_pll_bist_cnt(uint32_t base_addr, uint32_t *pll_bist_cnt);
int32_t dsi_phy_set_core_pll_div_s(uint32_t base_addr, uint32_t *pll_div_s);
int32_t dsi_phy_set_core_pll_freq_doubler_en(uint32_t base_addr, uint32_t *pll_freq_doubler_en);
int32_t dsi_phy_set_core_pll_ref24m_sel(uint32_t base_addr, uint32_t *pll_ref24m_sel);
int32_t dsi_phy_set_core_pll_pdiv(uint32_t base_addr, uint32_t *pll_pdiv);
int32_t dsi_phy_set_core_pll_ssc_en(uint32_t base_addr, uint32_t *pll_ssc_en);
int32_t dsi_phy_set_core_pll_ssc_ctrl(uint32_t base_addr, uint32_t *pll_ssc_ctrl);
int32_t dsi_phy_set_core_pll_lpf_r2(uint32_t base_addr, uint32_t *pll_lpf_r2);
int32_t dsi_phy_set_core_pll_lpf_r3(uint32_t base_addr, uint32_t *pll_lpf_r3);
int32_t dsi_phy_set_core_pll_lpf_cl(uint32_t base_addr, uint32_t *pll_lpf_c1);
int32_t dsi_phy_set_core_pll_lpf_c2(uint32_t base_addr, uint32_t *pll_lpf_c2);
int32_t dsi_phy_set_core_pll_ldo_test_en(uint32_t base_addr, uint32_t *pll_ldo_test_en);
int32_t dsi_phy_set_core_pll_ldo_vref(uint32_t base_addr, uint32_t *pll_ldo_vref);
int32_t dsi_phy_set_core_pll_ldo_vout(uint32_t base_addr, uint32_t *pll_ldo_vout);
int32_t dsi_phy_set_core_pll_kint(uint32_t base_addr, uint32_t *pll_kint);
int32_t dsi_phy_set_core_pll_nint(uint32_t base_addr, uint32_t *pll_nint);
int32_t dsi_phy_set_core_pll_n(uint32_t base_addr, uint32_t *pll_n);
int32_t dsi_phy_set_core_pll_icp(uint32_t base_addr, uint32_t *pll_icp);
int32_t dsi_phy_set_core_pll_icp_of_fset(uint32_t base_addr, uint32_t *pll_icp_offset);
int32_t dsi_phy_get_core_pll_lock(uint32_t base_addr, uint32_t *pll_lock);
int32_t dsi_phy_set_core_rg_dslpll_ssc_step_wicith_ctrl(uint32_t base_addr, uint32_t *rg_dslpll_ssc_step_wicith_ctrl);
int32_t dsi_phy_set_core_rg_dslpll_ssc_step_number_ctrl(uint32_t base_addr, uint32_t *rg_dslpll_ssc_step_number_ctrl);
int32_t dsi_phy_set_core_pll_lock_bypass(uint32_t base_addr, uint32_t *pll_lock_bypass);
int32_t dsi_phy_set_core_pll_lock_time(uint32_t base_addr, uint32_t *pll_lock_time);
int32_t dsi_phy_set_core_pll_lock_cnt(uint32_t base_addr, uint32_t *pll_lock_ent);
int32_t dsi_phy_set_core_hstx_sr(uint32_t base_addr, uint32_t *hstx_sr);
int32_t dsi_phy_set_core_hstx_amp_trim(uint32_t base_addr, uint32_t *hstx_amp_trim);
int32_t dsi_phy_set_core_lptx_sr_trim(uint32_t base_addr, uint32_t *lptx_sr_trim);
int32_t dsi_phy_set_core_lptx_amp_trim(uint32_t base_addr, uint32_t *lptx_amp_trim);
int32_t dsi_phy_set_core_en_lpcd(uint32_t base_addr, uint32_t *en_lpcd);
int32_t dsi_phy_set_core_en_lprx(uint32_t base_addr, uint32_t *en_lprx);
int32_t dsi_phy_set_core_en_ulprx(uint32_t base_addr, uint32_t *en_ulprx);
int32_t dsi_phy_set_core_lpcd_fl0(uint32_t base_addr, uint32_t *lpcd_fl0);
int32_t dsi_phy_set_core_lpcd_fl1(uint32_t base_addr, uint32_t *lpcd_fl1);
int32_t dsi_phy_get_core_lprx_vinit(uint32_t base_addr, uint32_t *lprx_vinit);
int32_t dsi_phy_get_core_lpcd_vinit(uint32_t base_addr, uint32_t *lpcd_vinit);
int32_t dsi_phy_set_mode_ext_loop_mode_cfg(uint32_t base_addr, uint32_t *ext_loop_mode_cfg);
int32_t dsi_phy_set_mode_en_ext_loop_cfg(uint32_t base_addr, uint32_t *en_ext_loop_cfg);
int32_t dsi_phy_set_mode_txclkreq_loop_cfg(uint32_t base_addr, uint32_t *txclkreq_loop_cfg);
int32_t dsi_phy_set_mode_txdatareq_loop_cfg(uint32_t base_addr, uint32_t *txdatareq_loop_cfg);
int32_t dsi_phy_set_mode_txdatasel_loop_cfg(uint32_t base_addr, uint32_t *txdatasel_loop_cfg);
int32_t dsi_phy_set_mode_txdata_loop_cfg(uint32_t base_addr, uint32_t *txdata_loop_cfg);
int32_t dsi_phy_set_mode_txdatanum_loop_cfg(uint32_t base_addr, uint32_t *txdatanum_loop_cfg);
int32_t dsi_phy_set_mode_ext_loop_bist_bit_error_cfg(uint32_t base_addr, uint32_t *ext_loop_bist_bit_error_cfg);
int32_t dsi_phy_set_mode_ext_loop_prbs_pattern_mode_cfg(uint32_t base_addr, uint32_t *ext_loop_prbs_pattern_mode_cfg);
int32_t dsi_phy_set_mode_ext_loop_prbs_seed_cfg(uint32_t base_addr, uint32_t *ext_loop_prbs_seed_cfg);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

