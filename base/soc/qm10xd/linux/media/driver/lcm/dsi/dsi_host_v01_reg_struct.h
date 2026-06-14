#ifndef _DIS_HOST_V01_REG_STRUCT_H_
#define _DIS_HOST_V01_REG_STRUCT_H_

#include "data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
union dsi_host_version_tag {
    struct dsi_host_versuion_map {
        volatile unsigned int version    :32;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_power_up_tag {
    struct dsi_host_power_up_map {
        volatile unsigned int shutdownz    :1;
        volatile unsigned int reserved     :31;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_clkmgr_cfg_tag {
    struct dsi_host_clkmgr_cfg_map {
        volatile unsigned int tx_esc_clk_division   :8;
        volatile unsigned int to_clk_division       :8;
        volatile unsigned int reserved              :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_vcid_tag {
    struct dsi_host_dpi_vcid_map {
        volatile unsigned int dpi_vcid        :2;
        volatile unsigned int reserved        :30;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_color_coding_tag {
    struct dsi_host_dpi_color_coding_map {
        volatile unsigned int dpi_color_coding   :4;
        volatile unsigned int reserved0          :4;
        volatile unsigned int loosely18_en       :1;
        volatile unsigned int reserved1          :15;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_cfg_pol_tag {
    struct dsi_host_dpi_cfg_pol_map {
        volatile unsigned int dataen_active_low   :1;
        volatile unsigned int vsync_active_low    :1;
        volatile unsigned int hsync_active_low    :1;
        volatile unsigned int shutd_active_low    :1;
        volatile unsigned int colorm_active_low   :1;
        volatile unsigned int reserved            :27;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_lp_cmd_tim_tag {
    struct dsi_host_dpi_lp_cmd_tim_map {
        volatile unsigned int invact_lpcmd_time     :8;
        volatile unsigned int reserved0             :8;
        volatile unsigned int outvact_lpcmd_time    :8;
        volatile unsigned int reserved1             :8;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dbi_vcid_tag {
    struct dsi_host_dbi_vcid_map {
        volatile unsigned int dbi_vcid      :2;
        volatile unsigned int reserved      :30;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dbi_cfg_tag {
    struct dsi_host_dbi_cfg_map {
        volatile unsigned int in_dbi_conf        :4;
        volatile unsigned int reserved0          :4;
        volatile unsigned int out_dbi_conf       :4;
        volatile unsigned int reserved1          :4;
        volatile unsigned int lut_size_conf      :2;
        volatile unsigned int reserved2          :8;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dbi_partitioning_en_tag {
    struct dsi_host_dbi_partitioning_en_map {
        volatile unsigned int partitioning_en   :1;
        volatile unsigned int reserved          :31;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dbi_cmdsize_tag {
    struct dsi_host_dbi_cmdsize_map {
        volatile unsigned int wr_cmd_size           :16;
        volatile unsigned int  allowed_cmd_size     :1;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_pckhdl_cfg_tag {
    struct dsi_host_pckhdl_cfg_map {
        volatile unsigned int eotp_tx_en    :1;
        volatile unsigned int eotp_rx_en    :1;
        volatile unsigned int bta_en        :1;
        volatile unsigned int ecc_rx_en     :1;
        volatile unsigned int crc_rx_en     :1;
        volatile unsigned int reserved      :27;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_gen_vcid_tag {
    struct dsi_host_gen_vcid_map {
        volatile unsigned int gen_vcid_rx   :2;
        volatile unsigned int reserved      :30;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_mode_cfg_tag {
    struct dsi_host_mode_cfg_map {
        volatile unsigned int cmd_video_mode    :1;
        volatile unsigned int reserved          :31;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_VID_MODE_CFG_tag {
    struct dsi_host_VID_MODE_CFG_map {
        volatile unsigned int vid_mode_type       :2;
        volatile unsigned int reserved0           :6;
        volatile unsigned int lp_vsa_en           :1;
        volatile unsigned int lp_vbp_en           :1;
        volatile unsigned int lp_vfp_en           :1;
        volatile unsigned int lp_vact_en          :1;
        volatile unsigned int lp_hbp_en           :1;
        volatile unsigned int lp_hfp_en           :1;
        volatile unsigned int frame_bta_ack_en    :1;
        volatile unsigned int lp_cmd_en           :1;
        volatile unsigned int vpg_en              :1;
        volatile unsigned int reserved1           :3;
        volatile unsigned int vpg_mode            :1;
        volatile unsigned int reserved2           :3;
        volatile unsigned int vpg_orientation     :1;
        volatile unsigned int reserved3           :7;

    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_pkt_size_tag {
    struct dsi_host_vid_pkt_size_map {
        volatile unsigned int vid_pkt_size      :14;
        volatile unsigned int reserved          :18;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_num_chunks_tag {
    struct dsi_host_vid_num_chunks_map {
        volatile unsigned int vid_num_chunks    :13;
        volatile unsigned int reserved          :19;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_null_size_tag {
    struct dsi_host_vid_null_size_map {
        volatile unsigned int vid_null_size    :13;
        volatile unsigned int reserved         :19;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_hsa_time_tag {
    struct dsi_host_vid_hsa_time_map {
        volatile unsigned int vid_hsa_time    :12;
        volatile unsigned int reserved        :20;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_hbp_time_tag {
    struct dsi_host_vid_hbp_time_map {
        volatile unsigned int vid_hbp_time    :12;
        volatile unsigned int reserved        :20;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_hline_time_tag {
    struct dsi_host_vid_line_time_map {
        volatile unsigned int vid_hline_time  :15;
        volatile unsigned int reserved        :17;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vsa_line_tag {
    struct dsi_host_vid_vsa_line_map {
        volatile unsigned int vsa_lines       :10;
        volatile unsigned int reserved        :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vbp_line_tag {
    struct dsi_host_vid_vbp_line_map {
        volatile unsigned int vbp_lines       :10;
        volatile unsigned int reserved        :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vfp_line_tag {
    struct dsi_host_vid_vfp_line_map {
        volatile unsigned int vfp_lines       :10;
        volatile unsigned int reserved        :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vactive_line_tag {
    struct dsi_host_vid_vactive_line_map {
        volatile unsigned int v_active_lines  :14;
        volatile unsigned int reserved        :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_edpi_cmd_size_tag {
    struct dsi_host_edpi_cmd_size_map {
        volatile unsigned int edpi_allowed_cmd_size    :16;
        volatile unsigned int reserved                 :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_cmd_mode_cfg_tag {
    struct dsi_host_cmd_mode_cfg_map {
        volatile unsigned int tear_fx_en         :1;
        volatile unsigned int ack_rqst_en        :1;
        volatile unsigned int reserved           :6;
        volatile unsigned int gen_sw_0p_tx       :1;
        volatile unsigned int gen_sw_1p_tx       :1;
        volatile unsigned int gen_sw_2p_tx       :1;
        volatile unsigned int gen_sr_0p_tx       :1;
        volatile unsigned int gen_sr_1p_tx       :1;
        volatile unsigned int gen_sr_2p_tx       :1;
        volatile unsigned int gen_lw_tx          :1;
        volatile unsigned int reserved0          :1;
        volatile unsigned int dcs_sw_0p_tx       :1;
        volatile unsigned int dcs_sw_1p_tx       :1;
        volatile unsigned int dcs_sr_0p_tx       :1;
        volatile unsigned int dcs_lw_tx          :1;
        volatile unsigned int reserved1          :4;
        volatile unsigned int max_rd_pkt_size    :1;
        volatile unsigned int reserved2          :7;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_gen_hdr_tag {
    struct dsi_host_gen_hdr_map {
        volatile unsigned int gen_dt           :6;
        volatile unsigned int gen_vc           :2;
        volatile unsigned int gen_wc_lsbyte    :8;
        volatile unsigned int gen_wc_msbyte    :8;
        volatile unsigned int reserved         :8;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_gen_pld_data_tag {
    struct dsi_host_gen_pld_data_map {
        volatile unsigned int gen_pld_b1    :8;
        volatile unsigned int gen_pld_b2    :8;
        volatile unsigned int gen_pld_b3    :8;
        volatile unsigned int gen_pld_b4    :8;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_cmd_pkt_status_tag {
    struct dsi_host_cmd_pkt_status_map {
        volatile unsigned int gen_cmd_empty      :1;
        volatile unsigned int gen_cmd_full       :1;
        volatile unsigned int gen_pld_w_empty    :1;
        volatile unsigned int gen_pld_w_full     :1;
        volatile unsigned int gen_pld_r_empty    :1;
        volatile unsigned int gen_pld_r_full     :1;
        volatile unsigned int gen_rd_cmd_busy    :1;
        volatile unsigned int reserved0          :1;
        volatile unsigned int dbi_cmd_empy       :1;
        volatile unsigned int dbi_cmd_full       :1;
        volatile unsigned int dbi_pld_w_empty    :1;
        volatile unsigned int dbi_pld_w_full     :1;
        volatile unsigned int dbi_pld_r_empty    :1;
        volatile unsigned int dbi_pld_r_full     :1;
        volatile unsigned int dbi_rd_cmd_busy    :1;
        volatile unsigned int reserved1          :17;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_to_cnt_cfg_tag {
    struct dsi_host_to_cnt_cfg_map {
        volatile unsigned int lprx_to_cnt       :16;
        volatile unsigned int hstx_to_cnt       :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_hs_rd_to_cnt_tag {
    struct dsi_host_hs_rd_to_cnt_map {
        volatile unsigned int hs_rd_to_cnt    :16;
        volatile unsigned int reserved        :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_lp_rd_to_cnt_tag {
    struct dsi_host_lp_rd_to_cnt_map {
        volatile unsigned int lp_rd_to_cnt    :16;
        volatile unsigned int reserved        :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_hs_wr_to_cnt_tag {
    struct dsi_host_hs_wr_to_cnt_map {
        volatile unsigned int hs_wr_to_cnt     :16;
        volatile unsigned int reserved0        :8;
        volatile unsigned int presp_to_mode    :1;
        volatile unsigned int reserved1        :7;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_lp_wr_to_cnt_tag {
    struct dsi_host_lp_wr_to_cnt_map {
        volatile unsigned int lp_wr_to_cnt    :16;
        volatile unsigned int reserved        :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_bta_to_cnt_tag {
    struct dsi_host_bta_to_cnt_map {
        volatile unsigned int bta_to_cnt    :16;
        volatile unsigned int reserved      :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_SDF_3D_tag {
    struct dsi_host_SDF_3D_map {
        volatile unsigned int mode_3d         :2;
        volatile unsigned int format_3d       :2;
        volatile unsigned int second_vsync    :1;
        volatile unsigned int right_first     :1;
        volatile unsigned int reserved0       :10;
        volatile unsigned int send_3d_cfg     :1;
        volatile unsigned int reserved1       :15;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_lpclk_ctrl_tag {
    struct dsi_host_lpclk_ctrl_map {
        volatile unsigned int phy_txrequestclkhs    :1;
        volatile unsigned int auto_clklane_ctrl     :1;
        volatile unsigned int reserved              :30;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_tmr_lpclk_cfg_tag {
    struct dsi_host_phy_tmr_lpclk_cfg_map {
        volatile unsigned int phy_clklp2hs_time    :10;
        volatile unsigned int reserved0            :6;
        volatile unsigned int phy_clkhs2lp_time    :10;
        volatile unsigned int reserved1            :6;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_tmr_cfg_tag {
    struct dsi_host_phy_tmr_cfg_map {
        volatile unsigned int phy_lp2hs_time    :10;
        volatile unsigned int reserved0         :6;
        volatile unsigned int phy_hs2lp_time    :10;
        volatile unsigned int reserved1         :6;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_rstz_tag {
    struct dsi_host_phy_rstz_map {
        volatile unsigned int phy_shutdownz   :1;
        volatile unsigned int phy_rstz        :1;
        volatile unsigned int phy_enableclk   :1;
        volatile unsigned int phy_forcepll    :1;
        volatile unsigned int reserved        :28;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_if_cfg_tag {
    struct dsi_host_phy_if_cfg_map {
        volatile unsigned int n_lanes              :2;
        volatile unsigned int reserved0            :6;
        volatile unsigned int phy_stop_wait_time   :8;
        volatile unsigned int reserved1            :16;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_ulps_ctrl_tag {
    struct dsi_host_phy_ulps_ctrl_map {
        volatile unsigned int phy_txrequlpsclk     :1;
        volatile unsigned int phy_txexitulpsclk    :1;
        volatile unsigned int phy_txrequlpslan     :1;
        volatile unsigned int phy_txexitulpslan    :1;
        volatile unsigned int reserved             :28;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_tx_triggers_tag {
    struct dsi_host_phy_tx_triggers_map {
        volatile unsigned int phy_tx_triggers    :4;
        volatile unsigned int reserved           :28;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_status_tag {
    struct dsi_host_phy_status_map {
        volatile unsigned int phy_lock                  :1;
        volatile unsigned int phy_direction             :1;
        volatile unsigned int phy_stopstateclklane      :1;
        volatile unsigned int phy_ulpsactivenotclk      :1;
        volatile unsigned int phy_stopstate0lane        :1;
        volatile unsigned int phy_ulpsactivenot0lane    :1;
        volatile unsigned int phy_rxulpsesc0lane        :1;
        volatile unsigned int phy_stopstate1lane        :1;
        volatile unsigned int phy_ulpsactivenot1lane    :1;
        volatile unsigned int phy_stopstate2lane        :1;
        volatile unsigned int phy_ulpsactivenot2lane    :1;
        volatile unsigned int phy_stopstate3lane        :1;
        volatile unsigned int phy_ulpsactivenot3lane    :1;
        volatile unsigned int reserved                  :19;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_tst_ctrl0_tag {
    struct dsi_host_phy_tst_ctrl0_map {
        volatile unsigned int phy_testclr    :1;
        volatile unsigned int phy_testclk    :1;
        volatile unsigned int reserved       :30;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_tst_ctrl1_tag {
    struct dsi_host_phy_tst_ctrl1_map {
        volatile unsigned int phy_testdin     :8;
        volatile unsigned int phy_testdout    :8;
        volatile unsigned int phy_testen      :1;
        volatile unsigned int reserved        :15;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_int0_tag {
    struct dsi_host_int0_map {
        volatile unsigned int ack_with_err_0     :1;
        volatile unsigned int ack_with_err_1     :1;
        volatile unsigned int ack_with_err_2     :1;
        volatile unsigned int ack_with_err_3     :1;
        volatile unsigned int ack_with_err_4     :1;
        volatile unsigned int ack_with_err_5     :1;
        volatile unsigned int ack_with_err_6     :1;
        volatile unsigned int ack_with_err_7     :1;
        volatile unsigned int ack_with_err_8     :1;
        volatile unsigned int ack_with_err_9     :1;
        volatile unsigned int ack_with_err_10    :1;
        volatile unsigned int ack_with_err_11    :1;
        volatile unsigned int ack_with_err_12    :1;
        volatile unsigned int ack_with_err_13    :1;
        volatile unsigned int ack_with_err_14    :1;
        volatile unsigned int ack_with_err_15    :1;
        volatile unsigned int dphy_errors_0      :1;
        volatile unsigned int dphy_errors_1      :1;
        volatile unsigned int dphy_errors_2      :1;
        volatile unsigned int dphy_errors_3      :1;
        volatile unsigned int dphy_errors_4      :1;
        volatile unsigned int reserved           :11;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_int1_tag {
    struct dsi_host_int1_map {
        volatile unsigned int to_hs_tx               :1;
        volatile unsigned int to_lp_rx               :1;
        volatile unsigned int ecc_single_err         :1;
        volatile unsigned int ecc_multi_err          :1;
        volatile unsigned int crc_err                :1;
        volatile unsigned int pkt_size_err           :1;
        volatile unsigned int eopt_err               :1;
        volatile unsigned int dpi_pld_wr_err         :1;
        volatile unsigned int gen_cmd_wr_err         :1;
        volatile unsigned int gen_pld_wr_err         :1;
        volatile unsigned int gen_pld_send_err       :1;
        volatile unsigned int gen_pld_rd_err         :1;
        volatile unsigned int gen_pld_recev_err      :1;
        volatile unsigned int dbi_cmd_wr_err         :1;
        volatile unsigned int dbi_pld_wr_err         :1;
        volatile unsigned int dbi_pld_rd_err         :1;
        volatile unsigned int dbi_pld_recv_err       :1;
        volatile unsigned int dbi_ilegal_comm_err    :1;
        volatile unsigned int reserved               :14;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_cal_tag {
    struct dsi_host_phy_cal_map {
        volatile unsigned int txskewcalhs           :1;
        volatile unsigned int reserved2             :31;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_int_force0_tag {
    struct dsi_host_int_force0_map {
        volatile unsigned int force_to_sh_tx        :1;
        volatile unsigned int force_ack_with_err_1  :1;
        volatile unsigned int force_ack_with_err_2  :1;
        volatile unsigned int force_ack_with_err_3  :1;
        volatile unsigned int force_ack_with_err_4  :1;
        volatile unsigned int force_ack_with_err_5  :1;
        volatile unsigned int force_ack_with_err_6  :1;
        volatile unsigned int force_ack_with_err_7  :1;
        volatile unsigned int force_ack_with_err_8  :1;
        volatile unsigned int force_ack_with_err_9  :1;
        volatile unsigned int force_ack_with_err_10 :1;
        volatile unsigned int force_ack_with_err_11 :1;
        volatile unsigned int force_ack_with_err_12 :1;
        volatile unsigned int force_ack_with_err_13 :1;
        volatile unsigned int force_ack_with_err_14 :1;
        volatile unsigned int force_ack_with_err_15 :1;
        volatile unsigned int force_dphy_errors_0   :1;
        volatile unsigned int force_dphy_errors_1   :1;
        volatile unsigned int force_dphy_errors_2   :1;
        volatile unsigned int force_dphy_errors_3   :1;
        volatile unsigned int force_dphy_errors_4   :1;
        volatile unsigned int reserved2             :11;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_int_force1_tag {
    struct dsi_host_int_force1_map {
        volatile unsigned int force_to_hs_tx         :1;
        volatile unsigned int force_to_lp_rx         :1;
        volatile unsigned int force_ecc_single_err   :1;
        volatile unsigned int force_ecc_multi_err    :1;
        volatile unsigned int force_crc_err          :1;
        volatile unsigned int force_pkt_size_err     :1;
        volatile unsigned int force_eopt_err         :1;
        volatile unsigned int force_dpi_pld_wr_err   :1;
        volatile unsigned int force_gen_cmd_wr_err   :1;
        volatile unsigned int force_gen_pld_wr_err   :1;
        volatile unsigned int force_gen_pld_send_err :1;
        volatile unsigned int force_gen_pld_rd_err   :1;
        volatile unsigned int force_gen_pld_recev_err:1;
        volatile unsigned int force_dbi_cmd_wr_err   :1;
        volatile unsigned int force_dbi_pld_wr_err   :1;
        volatile unsigned int force_dbi_pld_rd_err   :1;
        volatile unsigned int force_dbi_pld_recv_err :1;
        volatile unsigned int force_dbi_ilegal_comm_err :1;
        volatile unsigned int reserved               :14;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dsc_parameter_tag {
    struct dsi_host_dsc_parameter_map {
        volatile unsigned int compression_mode      :1;
        volatile unsigned int reserved0             :7;
        volatile unsigned int compress_algo         :2;
        volatile unsigned int reserved1             :6;
        volatile unsigned int pps_sel               :2;
        volatile unsigned int reserved2             :14;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_phy_tmp_rd_cfg_tag {
    struct dsi_host_phy_tmp_rd_cfg_map {
        volatile unsigned int max_rd_time           :15;
        volatile unsigned int reserved2             :17;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_shadow_ctrl_tag {
    struct dsi_host_vid_shadow_ctrl_map {
        volatile unsigned int vid_shadow_en         :1;
        volatile unsigned int reserved0             :7;
        volatile unsigned int vid_shadow_req        :1;
        volatile unsigned int reserved1             :7;
        volatile unsigned int vid_shadow_pin_req    :1;
        volatile unsigned int reserved2             :15;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_vcid_act_tag {
    struct dsi_host_dpi_vcid_act_map {
        volatile unsigned int dpi_vcid    :2;
        volatile unsigned int reserved    :30;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_color_coding_act_tag {
    struct dsi_host_dpi_color_coding_act_map {
        volatile unsigned int dpi_color_coding     :4;
        volatile unsigned int reserved0            :4;
        volatile unsigned int loosely18_en         :1;
        volatile unsigned int reserved1            :23;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_dpi_lp_cmd_tim_act_tag {
    struct dsi_host_dpi_lp_cmd_tim_act_map {
        volatile unsigned int invact_lpcmd_time     :8;
        volatile unsigned int reserved0             :8;
        volatile unsigned int outvact_lpcmd_time    :8;
        volatile unsigned int reserved1             :8;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_mode_cfg_act_tag {
    struct dsi_host_vid_mode_cfg_act_map {
        volatile unsigned int vid_mode_type    :2;
        volatile unsigned int lp_vsa_en        :1;
        volatile unsigned int lp_vbp_en        :1;
        volatile unsigned int lp_vfp_en        :1;
        volatile unsigned int lp_vact_en       :1;
        volatile unsigned int lp_hbp_en        :1;
        volatile unsigned int lp_hfp_en        :1;
        volatile unsigned int frame_bta_ack_en :1;
        volatile unsigned int lp_cmd_en        :1;
        volatile unsigned int reserved1        :12;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_pkt_size_act_tag {
    struct dsi_host_vid_pkt_size_act_map {
        volatile unsigned int vid_pkt_size    :14;
        volatile unsigned int reserved        :18;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_num_chunks_act_tag {
    struct dsi_host_vid_num_chunks_act_map {
        volatile unsigned int vid_num_chunks  :13;
        volatile unsigned int reserved        :19;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_null_size_act_tag {
    struct dsi_host_vid_null_size_act_map {
        volatile unsigned int vid_null_size    :13;
        volatile unsigned int reserved        :19;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_hsa_time_act_tag {
    struct dsi_host_vid_hsa_time_act_map {
        volatile unsigned int vid_has_time     :12;
        volatile unsigned int reserved         :20;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_hbp_time_act_tag {
    struct dsi_host_vid_hbp_time_act_map {
        volatile unsigned int vid_hbp_time    :12;
        volatile unsigned int reserved        :20;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_hline_time_act_tag {
    struct dsi_host_vid_hline_time_act_map {
        volatile unsigned int vid_hline_time    :15;
        volatile unsigned int reserved          :17;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vsa_lines_act_tag {
    struct dsi_host_vid_vsa_lines_act_map {
        volatile unsigned int vsa_lines    :10;
        volatile unsigned int reserved     :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vbp_lines_act_tag {
    struct dsi_host_vbp_vsa_lines_act_map {
        volatile unsigned int vbp_lines    :10;
        volatile unsigned int reserved     :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vfp_lines_act_tag {
    struct dsi_host_vfp_vsa_lines_act_map {
        volatile unsigned int vfp_lines    :10;
        volatile unsigned int reserved     :22;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_vid_vactive_lines_act_tag {
    struct dsi_host_vid_vactive_lines_act_map {
        volatile unsigned int v_active_lines    :14;
        volatile unsigned int reserved          :18;
    }mBits;
    volatile unsigned int dwValue;
};

union dsi_host_sdf_3d_act_tag {
    struct dsi_host_sdf_3d_act_map {
        volatile unsigned int mode_3d         :2;
        volatile unsigned int format_3d       :2;
        volatile unsigned int second_vsync    :1;
        volatile unsigned int right_first     :1;
        volatile unsigned int reserved0       :10;
        volatile unsigned int send_3d_cfg     :1;
        volatile unsigned int reserved1       :15;
    }mBits;
    volatile unsigned int dwValue;
};

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
