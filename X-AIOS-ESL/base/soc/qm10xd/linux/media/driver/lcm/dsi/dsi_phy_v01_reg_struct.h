#ifndef _DIS_PHY_V01_REG_STRUCT_H_
#define _DIS_PHY_V01_REG_STRUCT_H_

#include "data_type.h"
#include "reg_param.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
union dsi_clane_para0_tag {
    struct dsi_clane_para0_map {
        volatile unsigned int rst2enlptx                      :16;  //       [15:0]
        volatile unsigned int reserved                        :16;  //       [31:16]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_clane_para1_tag {
    struct dsi_clane_para1_map {
        volatile unsigned int inittime                        :32;  //       [31:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_clane_para2_tag {
    struct dsi_clane_para2_map {
        volatile unsigned int clkpre                          :8 ;  //       [7:0]
        volatile unsigned int clkzero                         :8 ;  //       [15:8]
        volatile unsigned int clkprepare                      :8 ;  //       [23:16]
        volatile unsigned int reserved                        :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_clane_para3_tag {
    struct dsi_clane_para3_map {
        volatile unsigned int hsexit                          :8 ;  //       [7:0]
        volatile unsigned int clktrail                        :8 ;  //       [15:8]
        volatile unsigned int clkpost                         :8 ;  //       [23:16]
        volatile unsigned int reserved                        :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_dlane_para0_tag {
    struct dsi_dlane_para0_map {
        volatile unsigned int rst2enlptx                      :16;  //       [15:0]
        volatile unsigned int reserved                        :16;  //       [31:16]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_dlane_para1_tag {
    struct dsi_dlane_para1_map {
        volatile unsigned int inittime                        :32;  //       [31:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_dlane_para2_tag {
    struct dsi_dlane_para2_map {
        volatile unsigned int hsexit                          :8 ;  //       [7:0]
        volatile unsigned int hstrail                         :8 ;  //       [15:8]
        volatile unsigned int hszero                          :8 ;  //       [23:16]
        volatile unsigned int hsprepare                       :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_dlane_para3_tag {
    struct dsi_dlane_para3_map {
        volatile unsigned int wakeup                          :32;  //       [31:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_dlane_para4_tag {
    struct dsi_dlane_para4_map {
        volatile unsigned int taget                           :8 ;  //       [7:0]
        volatile unsigned int tasure                          :8 ;  //       [15:8]
        volatile unsigned int tago                            :8 ;  //       [23:16]
        volatile unsigned int reserved                        :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_common_para0_tag {
    struct dsi_common_para0_map {
        volatile unsigned int t_lpx                                :8 ;  //       [7:0]
        volatile unsigned int reserved                             :24;  //       [31:8]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_misc_para0_tag {
    struct dsi_misc_para0_map {
        volatile unsigned int phyerr_mask_core0                    :5 ;  //       [4:0]
        volatile unsigned int lane_num_core0                       :2 ;  //       [6:5]
        volatile unsigned int dll_sel_core0                        :4 ;  //       [10:7]
        volatile unsigned int auto_ckgate_en                       :1 ;  //       [11]
        volatile unsigned int pulldwn_mode_d3                      :2 ;  //       [13:12]
        volatile unsigned int pulldwn_mode_d2                      :2 ;  //       [15:14]
        volatile unsigned int pulldwn_mode_d1                      :2 ;  //       [17:16]
        volatile unsigned int pulldwn_mode_d0                      :2 ;  //       [19:18]
        volatile unsigned int pulldwn_mode_c1                      :2 ;  //       [21:20]
        volatile unsigned int pulldwn_mode_c0                      :2 ;  //       [23:22]
        volatile unsigned int txreadyesc_mode                      :1 ;  //       [24]
        volatile unsigned int core1_clksel                         :1 ;  //       [25]
        volatile unsigned int core0_clksel                         :1 ;  //       [26]
        volatile unsigned int lane_num_core1                       :1;  //       [27]
        volatile unsigned int dll_sel_core1                        :4;   //       [31:28]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_misc_para1_tag {
    struct dsi_misc_para1_map {
        volatile unsigned int phyerr_mask_core1                    :5 ;  //       [4:0]
        volatile unsigned int reserved                             :27;  //       [31:5]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_mode_cfg_tag {
    struct dsi_mode_cfg_map {
        volatile unsigned int phy_link_mode                        :1 ;  //       [0]
        volatile unsigned int reserved                             :31;  //       [31:1]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_clane_para4tag {
    struct dsi_clane_para4_map {
        volatile unsigned int t_wakeup_c0                          :32;  //       [31:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_clane_para5_tag {
    struct dsi_clane_para5_map {
        volatile unsigned int rxvalidesc_extend_vld                :8 ;  //       [7:0]
        volatile unsigned int txreadyesc_extend_vld                :8 ;  //       [15:8]
        volatile unsigned int reserved                             :16;  //       [31:16]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_pcs_phy_sync_tag {
    struct dsi_pcs_phy_sync_map {
        volatile unsigned int afifo_info                           :24;  //       [23:0]
        volatile unsigned int afifo_cfg                            :4 ;  //       [27:24]
        volatile unsigned int dphy_soft_rst_core0                  :1 ;  //       [28]
        volatile unsigned int ulprx_sel_core0                      :1 ;  //       [29]
        volatile unsigned int ulprx_sel_core1                      :1 ;  //       [30]
        volatile unsigned int dphy_soft_rst_core1                  :1 ;  //       [31]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_deskew_rcal_misc0_tag {
    struct dsi_deskew_rcal_misc0_map {
        volatile unsigned int d3_period_deskew_done                :1 ;  //       [0]
        volatile unsigned int d3_init_deskew_done                  :1 ;  //       [1]
        volatile unsigned int d2_period_deskew_done                :1 ;  //       [2]
        volatile unsigned int d2_init_deskew_done                  :1 ;  //       [3]
        volatile unsigned int d1_period_deskew_done                :1 ;  //       [4]
        volatile unsigned int d1_init_deskew_done                  :1 ;  //       [5]
        volatile unsigned int d0_period_deskew_done                :1 ;  //       [6]
        volatile unsigned int d0_init_deskew_done                  :1 ;  //       [7]
        volatile unsigned int en_period_deskew                     :1 ;  //       [8]
        volatile unsigned int en_deskew                            :1 ;  //       [9]
        volatile unsigned int t_ipcd_det_time_d0                   :10;  //       [19:10]
        volatile unsigned int rcal_pluse_period                    :10;  //       [29:20]
        volatile unsigned int reserved                             :2 ;  //       [31:30]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_deskew_rcal_misc1_tag {
    struct dsi_deskew_rcal_misc1_map {
        volatile unsigned int init_deskew_time                     :2 ;  //       [19:0]
        volatile unsigned int reserved                             :1 ;  //       [31:20]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_deskew_rcal_misc2_tag {
    struct dsi_deskew_rcal_misc2_map {
        volatile unsigned int period_deskew_time                   :2 ;  //       [19:0]
        volatile unsigned int reserved                             :1 ;  //       [31:20]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_ext_loop_cfg_tag {
    struct dsi_ext_loop_cfg_map {
        volatile unsigned int ext_loop_prbs_seed_cfg         :8 ;  //       [7:0]
        volatile unsigned int ext_loop_prbs_pattern_mode_cfg :2 ;  //       [9:8]
        volatile unsigned int ext_loop_bist_bit_error_cfg    :1 ;  //       [10]
        volatile unsigned int txdatanum_loop_cfg             :2 ;  //       [12:11]
        volatile unsigned int txdata_loop_cfg                :8 ;  //       [20:13]
        volatile unsigned int txdatasel_loop_cfg             :1 ;  //       [21]
        volatile unsigned int txdatareq_loop_cfg             :1 ;  //       [22]
        volatile unsigned int txclkreq_loop_cfg              :1 ;  //       [23]
        volatile unsigned int en_ext_loop_cfg                :1 ;  //       [24]
        volatile unsigned int ext_loop_mode_cfg              :1 ;  //       [25]
        volatile unsigned int reserved                       :6 ;  //       [31:26]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_cl_deskew_tag {
    struct dsi_cl_deskew_map {
        volatile unsigned int cl_deskew_time                       :2 ;  //       [19:0]
        volatile unsigned int cl_init_deskew_done                  :1 ;  //       [20]
        volatile unsigned int cl_en_deskew                         :1 ;  //       [21]
        volatile unsigned int reserved                             :1 ;  //       [31:22]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para0_tag {
    struct dsi_phy_para0_map {
        volatile unsigned int vbg_rdy                              :1 ;  //       [0]
        volatile unsigned int bg_trim                              :4 ;  //       [4:1]
        volatile unsigned int bg_rbias                             :1 ;  //       [5]
        volatile unsigned int bg_en                                :1 ;  //       [6]
        volatile unsigned int fset_en                              :1 ;  //       [7]
        volatile unsigned int atest_sel                            :2 ;  //       [9:8]
        volatile unsigned int atest_en                             :1 ;  //       [10]
        volatile unsigned int su_iddq_en_soft                      :1 ;  //       [11]
        volatile unsigned int pwon_pll1_soft                       :1 ;  //       [12]
        volatile unsigned int pwon_dsi_core1_soft                  :1 ;  //       [13]
        volatile unsigned int pwon_pll0_soft                       :1 ;  //       [14]
        volatile unsigned int pwon_dsi_core0_soft                  :1 ;  //       [15]
        volatile unsigned int pwon_sel_core1                       :1 ;  //       [16]
        volatile unsigned int pwon_sel_core0                       :1 ;  //       [17]
        volatile unsigned int dsi_0_hs_8t_clk_sel                  :1 ;  //       [18]
        volatile unsigned int dsi_1_hs_8t_clk_sel                  :1 ;  //       [19]
        volatile unsigned int dsi_2_hs_8t_clk_sel                  :1 ;  //       [20]
        volatile unsigned int dsi_3_hs_8t_clk_sel                  :1 ;  //       [21]
        volatile unsigned int dsi_4_hs_8t_clk_sel                  :1 ;  //       [22]
        volatile unsigned int dsi_5_hs_8t_clk_sel                  :1 ;  //       [23]
        volatile unsigned int reserved                             :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para1_tag {
    struct dsi_phy_para1_map {
        volatile unsigned int pll_bist_cnt                        :16;  //       [15:0]
        volatile unsigned int pll_bist_ctrl                       :8 ;  //       [23:16]
        volatile unsigned int pll_bist_en                         :1 ;  //       [24]
        volatile unsigned int pll_soft_rst                        :1 ;  //       [25]
        volatile unsigned int pixelclk0_div                        :4 ;  //       [29:26] 16 1...15
        volatile unsigned int da_clk0_en                           :1 ;  //       [30]
        volatile unsigned int reserved                             :1 ;  //       [31]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para2_tag {
    struct dsi_phy_para2_map {
        volatile unsigned int pll_ldo_vout                        :2 ;  //       [1:0]
        volatile unsigned int pll_ldo_vref                        :2 ;  //       [3:2]
        volatile unsigned int pll_ldo_test_en                     :1 ;  //       [4]
        volatile unsigned int pll_lpf_c2                          :2 ;  //       [6:5]
        volatile unsigned int pll_lpf_c1                          :2 ;  //       [8:7]
        volatile unsigned int pll_lpf_r3                          :1 ;  //       [9]
        volatile unsigned int pll_lpf_r2                          :2 ;  //       [11:10]
        volatile unsigned int pll_ssc_ctrl                        :8 ;  //       [19:12]
        volatile unsigned int pll_ssc_en                          :1 ;  //       [20]
        volatile unsigned int reserved0                            :2 ;  //       [22:21]
        volatile unsigned int pll_pdiv                            :3 ;  //       [25:23] 2^n
        volatile unsigned int pll_ref24m_sel                      :1 ;  //       [26]
        volatile unsigned int pll_freq_doubler_en                 :1 ;  //       [27]
        volatile unsigned int pll_div_s                           :1 ;  //       [28]
        volatile unsigned int reserved1                            :3 ;  //       [31:29]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para3_tag {
    struct dsi_phy_para3_map {
        volatile unsigned int pll_nint                            :9 ;  //       [8:0]
        volatile unsigned int pll_kint                            :23;  //       [31:9]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para4_tag {
    struct dsi_phy_para4_map {
        volatile unsigned int pll_lock                            :1 ;  //       [0]
        volatile unsigned int pll_reserved                        :8 ;  //       [8:1]
        volatile unsigned int pll_icp_offset                      :2 ;  //       [10:9]
        volatile unsigned int pll_icp                             :3 ;  //       [13:11]
        volatile unsigned int pll_n                               :11;  //       [24:14]
        volatile unsigned int reserved                             :7 ;  //       [31:25]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para9_tag {
    struct dsi_phy_para9_map {
        volatile unsigned int bist_out                             :6 ;  //       [5:0]
        volatile unsigned int bist_bit_error                       :6 ;  //       [11:6]
        volatile unsigned int bist_sel                             :6 ;  //       [17:12]
        volatile unsigned int bist_en                              :6 ;  //       [23:18]
        volatile unsigned int reserved                             :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para10_tag {
    struct dsi_phy_para10_map {
        volatile unsigned int rcal_done                            :1 ;  //       [0]
        volatile unsigned int rcal_comp_out                        :1 ;  //       [1]
        volatile unsigned int rcal_trim_soft                       :4 ;  //       [5:2]
        volatile unsigned int rcal_soft_rst                        :1 ;  //       [6]
        volatile unsigned int rcal_en_i                            :1 ;  //       [7]
        volatile unsigned int rcal_pn_swap_i                       :1 ;  //       [8]
        volatile unsigned int rcal_sel                             :1 ;  //       [9]
        volatile unsigned int reserved                             :6 ;  //       [15:10]
        volatile unsigned int rcal_trim_init_time                  :16;  //       [31:16]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para11_tag {
    struct dsi_phy_para11_map {
        volatile unsigned int lprx_vref_trim                       :4 ;  //       [3:0]
        volatile unsigned int lpcd_vref_trim                       :4 ;  //       [7:4]
        volatile unsigned int core0_lptx_amp_trim                  :3 ;  //       [10:8]
        volatile unsigned int core0_lptx_sr_trim                   :3 ;  //       [13:11]
        volatile unsigned int core0_hstx_amp_trim                  :3 ;  //       [16:14]
        volatile unsigned int core0_hstx_sr                        :2 ;  //       [18:17]
        volatile unsigned int ft_sel                               :3 ;  //       [21:19]
        volatile unsigned int reserved                             :2 ;  //       [23:22]
        volatile unsigned int rcal_ctrl                            :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para12_tag {
    struct dsi_phy_para12_map {
        volatile unsigned int reserved_d0                          :8 ;  //       [7:0]
        volatile unsigned int reserved_ad0ut                       :8 ;  //       (15:8]
        volatile unsigned int core1_lptx_amp_trim                  :3 ;  //       [18:16]
        volatile unsigned int core1_lptx_sr_trim                   :3 ;  //       [21:19]
        volatile unsigned int core1_hstx_amp_trim                  :3 ;  //       [24:22]
        volatile unsigned int core1_hstx_sr                        :2 ;  //       [26:25]
        volatile unsigned int reserved                             :5 ;  //       [31:27]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para13_tag {
    struct dsi_phy_para13_map {
        volatile unsigned int reserved_d4                          :8 ;  //       [31:24]
        volatile unsigned int reserved_d3                          :8 ;  //       [23:16]
        volatile unsigned int reserved_d2                          :8 ;  //       (15:8]
        volatile unsigned int reserved_d1                          :8 ;  //       [7:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para14_tag {
    struct dsi_phy_para14_map {
        volatile unsigned int lpcd_vinit_d0                        :4 ;  //       [3:0]
        volatile unsigned int lprx_vinit_d0                        :4 ;  //       [7:4]
        volatile unsigned int lpcd_fl1_d0                          :1 ;  //       [8]
        volatile unsigned int lpcd_fl0_d0                          :1 ;  //       [9]
        volatile unsigned int en_ulprx_d0                          :1 ;  //       [10]
        volatile unsigned int en_lprx_d0                           :1 ;  //       [11]
        volatile unsigned int en_lpcd_d0                           :1 ;  //       [12]
        volatile unsigned int hstx_phase_d0                        :1 ;  //       [13]
        volatile unsigned int predrv_dly_sel_d0                    :3 ;  //       [16:14]
        volatile unsigned int predrv_by_dly_d0                     :1 ;  //       [17]
        volatile unsigned int ck8t_reverse_d0                      :1 ;  //       [18]
        volatile unsigned int reserved                             :5 ;  //       [23:19]
        volatile unsigned int reserved_d5                          :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_para16_tag {
    struct dsi_phy_para16_map {
        volatile unsigned int reserved                             :8 ;  //       [31:24]
        volatile unsigned int ck8t_reverse_d1                      :1 ;  //       [23]
        volatile unsigned int predrv_by_dly_d1                     :1 ;  //       [22]
        volatile unsigned int predrv_dly_sel_d1                    :3 ;  //       [21:19]
        volatile unsigned int hstx_phase_d1                        :1 ;  //       [18]

        volatile unsigned int ck8t_reverse_d2                      :1 ;  //       [17]
        volatile unsigned int predrv_by_dly_d2                     :1 ;  //       [16]
        volatile unsigned int predrv_dly_sel_d2                    :3 ;  //       [15:13]
        volatile unsigned int hstx_phase_d2                        :1 ;  //       [12]
        volatile unsigned int ck8t_reverse_d4                      :1 ;  //       [11]
        volatile unsigned int predrv_by_dly_d4                     :1 ;  //       [10]
        volatile unsigned int predrv_dly_sel_d4                    :3 ;  //       [9:7]
        volatile unsigned int hstx_phase_d4                        :1 ;  //       [6]
        volatile unsigned int ck8t_reverse_d5                      :1 ;  //       [5]
        volatile unsigned int predrv_by_dly_d5                     :1 ;  //       [4]
        volatile unsigned int predrv_dly_sel_d5                    :3 ;  //       [3:1]
        volatile unsigned int hstx_phase_d5                        :1 ;  //       [0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info0_tag {
    struct dsi_prbs_info0_map {
        volatile unsigned int dsi_5_bist_ok                        :1 ;  //       [0]
        volatile unsigned int dsi_4_bist_ok                        :1 ;  //       [1]
        volatile unsigned int dsi_3_bist_ok                        :1 ;  //       [2]
        volatile unsigned int dsi_2_bist_ok                        :1 ;  //       [3]
        volatile unsigned int dsi_1_bist_ok                        :1 ;  //       [4]
        volatile unsigned int dsi_0_bist_ok                        :1 ;  //       [5]
        volatile unsigned int dsi_5_bist_done                      :1 ;  //       [6]
        volatile unsigned int dsi_4_bist_done                      :1 ;  //       [7]
        volatile unsigned int dsi_3_bist_done                      :1 ;  //       [8]
        volatile unsigned int dsi_2_bist_done                      :1 ;  //       [9]
        volatile unsigned int dsi_1_bist_done                      :1 ;  //       [10]
        volatile unsigned int dsi_0_bist_done                      :1 ;  //       [11]
        volatile unsigned int rg_ibias_sel                         :1 ;  //       [12]
        volatile unsigned int dsi_5_lptx_vb_sel                    :1 ;  //       [13]
        volatile unsigned int dsi_4_lptx_vb_sel                    :1 ;  //       [14]
        volatile unsigned int dsi_3_lptx_vb_sel                    :1 ;  //       [15]
        volatile unsigned int dsi_2_lptx_vb_sel                    :1 ;  //       [16]
        volatile unsigned int dsi_1_lptx_vb_sel                    :1 ;  //       [17]
        volatile unsigned int dsi_0_lptx_vb_sel                    :1 ;  //       [18]
        volatile unsigned int reserved                             :13;  //       [31:19]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_ssc_ctrl_tag {
    struct dsi_ssc_ctrl_map {
        volatile unsigned int reserved                             :1 ;  //       [31]
        volatile unsigned int rg_dslpll_ssc_step_wicith_ctrl      :8 ;  //       [30:23]
        volatile unsigned int rg_dslpll_ssc_step_number_ctrl      :23;  //       [22:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_rdy_ctrl0_tag {
    struct dsi_phy_rdy_ctrl0_map {
        volatile unsigned int pll0_lock_time                       :20;  //       [19:0]
        volatile unsigned int rcal_done_bypass                     :1 ;  //       [20]
        volatile unsigned int pll0_lock_bypass                     :1 ;  //       [21]
        volatile unsigned int reserved                             :10;  //       [31:22]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_rdy_ctrl1_tag {
    struct dsi_phy_rdy_ctrl1_map {
        volatile unsigned int pll1_lock_time                       :20;  //       [19:0]
        volatile unsigned int pll1_lock_bypass                     :1 ;  //       [20]
        volatile unsigned int reserved                             :11;  //       [31:21]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_phy_rdy_info_tag {
    struct dsi_phy_rdy_info_map {
        volatile unsigned int pll_lock_cnt                         :20;  //       [19:0]
        volatile unsigned int reserved                             :12;  //       [31:20]
    }mBits;
    volatile unsigned int dwValue;
};


union dsi_prbs_info1_tag {
    struct dsi_prbs_info1_map {
        volatile unsigned int dsi_0_prbs_check_num                 :16;  //       [15:0]
        volatile unsigned int dsi_5_bist_pattern_mode              :2 ;  //       [17:16]
        volatile unsigned int dsi_4_bist_pattern_mode              :2 ;  //       [19:18]
        volatile unsigned int dsi_3_bist_pattern_mode              :2 ;  //       [21:20]
        volatile unsigned int dsi_2_bist_pattern_mode              :2 ;  //       [23:22]
        volatile unsigned int dsi_1_bist_pattern_mode              :2 ;  //       [25:24]
        volatile unsigned int dsi_0_bist_pattern_mode              :2 ;  //       [27:26]
        volatile unsigned int reserved                             :4 ;  //       [31:28]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info2_tag {
    struct dsi_prbs_info2_map {
        volatile unsigned int dsi_1_prbs_check_num                 :16;  //       [31:16]
        volatile unsigned int dsi_2_prbs_check_num                 :16;  //       [15:0]

    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info3_tag {
    struct dsi_prbs_info3_map {
        volatile unsigned int dsi_3_prbs_check_num                 :16;  //       [31:16]
        volatile unsigned int dsi_4_prbs_check_num                 :16;  //       [15:0]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info4_tag {
    struct dsi_prbs_info4_map {
        volatile unsigned int dsi_5_prbs_err_num                   :8 ;  //       [7:0]
        volatile unsigned int dsi_5_prbs_err_th                    :8 ;  //       [15:8]
        volatile unsigned int dsi_5_prbs_check_num                 :16;  //       [31:16]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info5_tag {
    struct dsi_prbs_info5_map {
        volatile unsigned int dsi_3_prbs_seed                      :8 ;  //       [7:0]
        volatile unsigned int dsi_2_prbs_seed                      :8 ;  //       [15:8]
        volatile unsigned int dsi_1_prbs_seed                      :8 ;  //       [23:16]
        volatile unsigned int dsi_0_prbs_seed                      :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info6_tag {
    struct dsi_prbs_info6_map {
        volatile unsigned int dsi_1_prbs_err_th                    :8 ;  //       [7:0]
        volatile unsigned int dsi_0_prbs_err_th                    :8 ;  //       [15:8]
        volatile unsigned int dsi_5_prbs_seed                      :8 ;  //       [23:16]
        volatile unsigned int dsi_4_prbs_seed                      :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info7_tag {
    struct dsi_prbs_info7_map {
        volatile unsigned int dsi_0_prbs_err_num                   :8 ;  //       [7:0]
        volatile unsigned int dsi_4_prbs_err_th                    :8 ;  //       [15:8]
        volatile unsigned int dsi_3_prbs_err_th                    :8 ;  //       [23:16]
        volatile unsigned int dsi_2_prbs_err_th                    :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_prbs_info8_tag {
    struct dsi_prbs_info8_map {
        volatile unsigned int dsi_4_prbs_err_num                   :8 ;  //       [7:0]
        volatile unsigned int dsi_3_prbs_err_num                   :8 ;  //       [15:8]
        volatile unsigned int dsi_2_prbs_err_num                   :8 ;  //       [23:16]
        volatile unsigned int dsi_1_prbs_err_num                   :8 ;  //       [31:24]
    }mBits;
    volatile unsigned int dwValue;
};

#if 0
volatile unsigned int dphy_debug0                          :32;  //       [31:0]
volatile unsigned int dphy_debugl                          :32;  //       [31:0]
volatile unsigned int dphy_debug2                          :32;  //       [31:0]
volatile unsigned int dphy_debug3                          :32;  //       [31:0]
volatile unsigned int dphy_debug4                          :32;  //       [31:0]
volatile unsigned int dphy_debugs                          :32;  //       [31:0]
volatile unsigned int dphy_debug6                          :32;  //       [31:0]
volatile unsigned int dphy_debug7                          :32;  //       [31:0]
volatile unsigned int dphy_debugs                          :32;  //       [31:0]
volatile unsigned int dphy_debug9                          :32;  //       [31:0]
volatile unsigned int dphy_debug10                         :32;  //       [31:0]
volatile unsigned int dphy_debugll                         :32;  //       [31:0]
volatile unsigned int dphy_debugl2                         :32;  //       [31:0]
volatile unsigned int dphy_debugl3                         :32;  //       [31:0]
volatile unsigned int dphy_debugl4                         :32;  //       [31:0]
volatile unsigned int dphy_debugl5                         :32;  //       [31:0]
volatile unsigned int dphy_debugl6                         :32;  //       [31:0]
volatile unsigned int dphy_debugl7                         :32;  //       [31:0]
volatile unsigned int dphy_debugl8                         :32;  //       [31:0]
volatile unsigned int dphy_debugl9                         :32;  //       [31:0]
volatile unsigned int dphy_debug20                         :32;  //       [31:0]
volatile unsigned int dphy_debug21                         :32;  //       [31:0]

volatile unsigned int dphy_debug_cfg_reg0                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg2                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg3                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg4                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg5                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg6                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg7                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg8                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg9                  :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl0                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regll                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl2                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl3                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl4                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl5                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl6                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl7                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl8                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_regl9                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg20                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg21                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg22                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg23                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg24                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg25                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg26                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg27                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg28                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg29                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg30                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg31                 :32;  //       [31:0]
volatile unsigned int dphy_debug_cfg_reg32                 :32;  //       [31:0]
volatile unsigned int dphy_status_info_reg0                :32;  //       [31:0]
volatile unsigned int dphy_status_info_regl                :32;  //       [31:0]
volatile unsigned int dphy_status_info_reg2                :32;  //       [31:0]
volatile unsigned int dphy_status_info_reg3                :32;  //       [31:0]
#endif
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
