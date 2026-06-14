#ifndef _DSI_HOST_V01_DRV_H_
#define _DSI_HOST_V01_DRV_H_

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

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/


int32_t dsi_host_get_version(uint32_t base_addr, uint32_t *version);
int32_t dsi_host_set_power_up(uint32_t base_addr, uint32_t power_up);
int32_t dsi_host_set_tx_esc_clk_division(uint32_t base_addr, uint32_t tx_esc_clk_division);
int32_t dsi_host_set_to_clk_division(uint32_t base_addr, uint32_t to_clk_division);
int32_t dsi_host_set_dpi_vcid(uint32_t base_addr, uint32_t dpi_vcid);
int32_t dsi_host_set_dpi_color_format(uint32_t base_addr, dsi_host_dpi_color_format_e dpi_color_fromat);
int32_t dsi_host_set_loosely18(uint32_t base_addr, uint32_t loosely18_en);
int32_t dsi_host_set_dpi_dataen_pol(uint32_t base_addr, uint32_t dataen_pol);
int32_t dsi_host_set_dpi_vsync_pol(uint32_t base_addr, uint32_t vsync_pol);
int32_t dsi_host_set_dpi_hsync_pol(uint32_t base_addr, uint32_t hsync_pol);
int32_t dsi_host_set_dpi_shutd_pol(uint32_t base_addr, uint32_t shutd_pol);
int32_t dsi_host_set_dpi_colorm_pol(uint32_t base_addr, uint32_t colorm_pol);
int32_t dsi_host_set_invact_lpcmd_time(uint32_t base_addr, uint32_t invact_lpcmd_time);
int32_t dsi_host_set_outvact_lpcmd_time(uint32_t base_addr, uint32_t outvact_lpcmd_time);
int32_t dsi_host_set_dbi_vcid(uint32_t base_addr, uint32_t dbi_vcid);
int32_t dsi_host_set_in_dbi_conf(uint32_t base_addr, dsi_host_dbi_data_e in_dbi_conf);
int32_t dsi_host_set_out_dbi_conf(uint32_t base_addr, dsi_host_dbi_data_e out_dbi_conf);
int32_t dsi_host_set_lut_size_conf(uint32_t base_addr, dsi_host_write_lut_size_e lut_size_conf);
int32_t dsi_host_set_partitioning(uint32_t base_addr, uint32_t partitioning_en);
int32_t dsi_host_set_dbi_wr_cmd_size(uint32_t base_addr, uint32_t wr_cmd_size);
int32_t dsi_host_set_dbi_allowed_cmd_size(uint32_t base_addr, uint32_t allowed_cmd_size);
int32_t dsi_host_set_pckhdl_eotp_tx_en(uint32_t base_addr, uint32_t eotp_tx_en);
int32_t dsi_host_set_pckhdl_eotp_rx_en(uint32_t base_addr, uint32_t eotp_rx_en);
int32_t dsi_host_set_pckhdl_bta_en(uint32_t base_addr, uint32_t bta_en);
int32_t dsi_host_set_pckhdl_ecc_rx_en(uint32_t base_addr, uint32_t ecc_rx_en);
int32_t dsi_host_set_pckhdl_crc_rx_en(uint32_t base_addr, uint32_t crc_rx_en);
int32_t dsi_host_set_gen_vcid_rx(uint32_t base_addr, uint32_t gen_vcid_rx);
int32_t dsi_host_set_cmd_video_mode(uint32_t base_addr, uint32_t cmd_video_mode);
int32_t dsi_host_set_vid_mode_type(uint32_t base_addr, dsi_host_vid_mode_type_e vid_mode_type);
int32_t dsi_host_set_vid_lp_vsa_en(uint32_t base_addr, uint32_t lp_vsa_en);
int32_t dsi_host_set_vid_lp_vbp_en(uint32_t base_addr, uint32_t lp_vbp_en);
int32_t dsi_host_set_vid_lp_vfp_en(uint32_t base_addr, uint32_t lp_vfp_en);
int32_t dsi_host_set_vid_lp_vact_en(uint32_t base_addr, uint32_t lp_vact_en);
int32_t dsi_host_set_vid_lp_hbp_en(uint32_t base_addr, uint32_t lp_hbp_en);
int32_t dsi_host_set_vid_lp_hfp_en(uint32_t base_addr, uint32_t lp_hfp_en);
int32_t dsi_host_set_vid_frame_bta_ack_en(uint32_t base_addr, uint32_t frame_bta_ack_en);
int32_t dsi_host_set_vid_lp_cmd_en(uint32_t base_addr, uint32_t lp_cmd_en);
int32_t dsi_host_set_vid_vpg_en(uint32_t base_addr, uint32_t vpg_en);
int32_t dsi_host_set_vid_vpg_mode(uint32_t base_addr, uint32_t vpg_en);
int32_t dsi_host_set_vid_vpg_orientation(uint32_t base_addr, uint32_t vpg_orientation);
int32_t dsi_host_set_vid_pkt_size(uint32_t base_addr, uint32_t vid_pkt_size);
int32_t dsi_host_set_vid_num_chunks(uint32_t base_addr, uint32_t vid_num_chunks);
int32_t dsi_host_set_vid_null_size(uint32_t base_addr, uint32_t vid_null_size);
int32_t dsi_host_set_vid_hsa_time(uint32_t base_addr, uint32_t vid_hsa_time);
int32_t dsi_host_set_vid_hbp_time(uint32_t base_addr, uint32_t vid_hbp_time);
int32_t dsi_host_set_vid_hline_time(uint32_t base_addr, uint32_t vid_hline_time);
int32_t dsi_host_set_vid_vsa_lines(uint32_t base_addr, uint32_t vsa_lines);
int32_t dsi_host_set_vid_vbp_lines(uint32_t base_addr, uint32_t vbp_lines);
int32_t dsi_host_set_vid_vfp_lines(uint32_t base_addr, uint32_t vfp_lines);
int32_t dsi_host_set_vid_vactive_line(uint32_t base_addr, uint32_t v_active_lines);
int32_t dsi_host_set_edpi_allowed_cmd_size(uint32_t base_addr, uint32_t edpi_allowed_cmd_size);
int32_t dsi_host_set_tear_fx_en(uint32_t base_addr, uint32_t tear_fx_en);
int32_t dsi_host_set_ack_rqst_en(uint32_t base_addr, uint32_t ack_rqst_en);
int32_t dsi_host_set_gen_sw_0p_tx(uint32_t base_addr, uint32_t gen_sw_0p_tx);
int32_t dsi_host_set_gen_sw_1p_tx(uint32_t base_addr, uint32_t gen_sw_1p_tx);
int32_t dsi_host_set_gen_sw_2p_tx(uint32_t base_addr, uint32_t gen_sw_2p_tx);
int32_t dsi_host_set_gen_sr_0p_tx(uint32_t base_addr, uint32_t gen_sr_0p_tx);
int32_t dsi_host_set_gen_sr_1p_tx(uint32_t base_addr, uint32_t gen_sr_1p_tx);
int32_t dsi_host_set_gen_sr_2p_tx(uint32_t base_addr, uint32_t gen_sr_2p_tx);
int32_t dsi_host_set_gen_lw_tx(uint32_t base_addr, uint32_t gen_lw_tx);
int32_t dsi_host_set_dcs_sw_0p_tx(uint32_t base_addr, uint32_t dcs_sw_0p_tx);
int32_t dsi_host_set_dcs_sw_1p_tx(uint32_t base_addr, uint32_t dcs_sw_1p_tx);
int32_t dsi_host_set_dcs_sr_0p_tx(uint32_t base_addr, uint32_t dcs_sr_0p_tx);
int32_t dsi_host_set_dcs_lw_tx(uint32_t base_addr, uint32_t dcs_lw_tx);
int32_t dsi_host_set_max_rd_pkt_size(uint32_t base_addr, uint32_t max_rd_pkt_size);
int32_t dsi_host_set_gen_dt(uint32_t base_addr, uint32_t gen_dt);
int32_t dsi_host_set_gen_vc(uint32_t base_addr, uint32_t gen_vc);
int32_t dsi_host_set_gen_wc_lsbyte(uint32_t base_addr, uint32_t gen_wc_lsbyte);
int32_t dsi_host_set_gen_wc_msbyte(uint32_t base_addr, uint32_t gen_wc_msbyte);
int32_t dsi_host_set_gen_packet_header_cfg(uint32_t base_addr, uint32_t gen_packet_header);
int32_t dsi_host_set_gen_pld_b1(uint32_t base_addr, uint32_t gen_pld_b1);
int32_t dsi_host_set_gen_pld_b2(uint32_t base_addr, uint32_t gen_pld_b2);
int32_t dsi_host_set_gen_pld_b3(uint32_t base_addr, uint32_t gen_pld_b3);
int32_t dsi_host_set_gen_pld_b4(uint32_t base_addr, uint32_t gen_pld_b4);
int32_t dsi_host_set_gen_pld_data(uint32_t base_addr, uint32_t gen_pld_data);
int32_t dsi_host_get_gen_pld_data(uint32_t base_addr, uint32_t *gen_pld_data);
int32_t dsi_host_get_gen_cmd_empty(uint32_t base_addr, uint32_t *gen_cmd_empty);
int32_t dsi_host_get_gen_cmd_full(uint32_t base_addr, uint32_t *gen_cmd_full);
int32_t dsi_host_get_gen_pld_w_empty(uint32_t base_addr, uint32_t *gen_pld_w_empty);
int32_t dsi_host_get_gen_pld_w_full(uint32_t base_addr, uint32_t *gen_pld_w_full);
int32_t dsi_host_get_gen_pld_r_empty(uint32_t base_addr, uint32_t *gen_pld_r_empty);
int32_t dsi_host_get_gen_pld_r_full(uint32_t base_addr, uint32_t *gen_pld_r_full);
int32_t dsi_host_get_gen_rd_cmd_busy(uint32_t base_addr, uint32_t *gen_rd_cmd_busy);
int32_t dsi_host_get_dbi_cmd_empy(uint32_t base_addr, uint32_t *dbi_cmd_empy);
int32_t dsi_host_get_dbi_cmd_full(uint32_t base_addr, uint32_t *dbi_cmd_full);
int32_t dsi_host_get_dbi_pld_w_empty(uint32_t base_addr, uint32_t *dbi_pld_w_empty);
int32_t dsi_host_get_dbi_pld_w_full(uint32_t base_addr, uint32_t *dbi_pld_w_full);
int32_t dsi_host_get_dbi_pld_r_empty(uint32_t base_addr, uint32_t *dbi_pld_r_empty);
int32_t dsi_host_get_dbi_pld_r_full(uint32_t base_addr, uint32_t *dbi_pld_r_full);
int32_t dsi_host_get_dbi_rd_cmd_busy(uint32_t base_addr, uint32_t *dbi_rd_cmd_busy);
int32_t dsi_host_set_lprx_to_cnt(uint32_t base_addr, uint32_t lprx_to_cnt);
int32_t dsi_host_set_hstx_to_cnt(uint32_t base_addr, uint32_t hstx_to_cnt);
int32_t dsi_host_set_hs_rd_to_cnt(uint32_t base_addr, uint32_t hs_rd_to_cnt);
int32_t dsi_host_set_lp_rd_to_cnt(uint32_t base_addr, uint32_t lp_rd_to_cnt);
int32_t dsi_host_set_hs_wr_to_cnt(uint32_t base_addr, uint32_t hs_wr_to_cnt);
int32_t dsi_host_set_presp_to_mode(uint32_t base_addr, uint32_t presp_to_mode);
int32_t dsi_host_set_lp_wr_to_cnt(uint32_t base_addr, uint32_t lp_wr_to_cnt);
int32_t dsi_host_set_bta_to_cnt(uint32_t base_addr, uint32_t bta_to_cnt);
int32_t dsi_host_set_mode_3d(uint32_t base_addr, dsi_host_mode_3d_e mode_3d);
int32_t dsi_host_set_format_3d(uint32_t base_addr, dsi_host_format_3d_e format_3d);
int32_t dsi_host_set_second_vsync(uint32_t base_addr, uint32_t second_vsync);
int32_t dsi_host_set_right_first(uint32_t base_addr, uint32_t right_first);
int32_t dsi_host_set_send_3d_cfg(uint32_t base_addr, uint32_t send_3d_cfg);
int32_t dsi_host_set_phy_txrequestclkhs(uint32_t base_addr, uint32_t phy_txrequestclkhs);
int32_t dsi_host_set_auto_clklane_ctrl(uint32_t base_addr, uint32_t auto_clklane_ctrl);
int32_t dsi_host_set_phy_clklp2hs_time(uint32_t base_addr, uint32_t phy_clklp2hs_time);
int32_t dsi_host_set_phy_clkhs2lp_time(uint32_t base_addr, uint32_t phy_clkhs2lp_time);
int32_t dsi_host_set_phy_lp2hs_time(uint32_t base_addr, uint32_t phy_lp2hs_time);
int32_t dsi_host_set_phy_hs2lp_time(uint32_t base_addr, uint32_t phy_hs2lp_time);
int32_t dsi_host_set_phy_shutdownz(uint32_t base_addr, uint32_t phy_shutdownz);
int32_t dsi_host_set_phy_rstz(uint32_t base_addr, uint32_t phy_rstz);
int32_t dsi_host_set_phy_enableclk(uint32_t base_addr, uint32_t phy_enableclk);
int32_t dsi_host_set_phy_forcepll(uint32_t base_addr, uint32_t phy_forcepll);
int32_t dsi_host_set_n_lanes(uint32_t base_addr, dsi_host_n_lanes_e n_lanes);
int32_t dsi_host_set_phy_stop_wait_time(uint32_t base_addr, uint32_t phy_stop_wait_time);
int32_t dsi_host_set_phy_txrequlpsclk(uint32_t base_addr, uint32_t phy_txrequlpsclk);
int32_t dsi_host_set_phy_txexitulpsclk(uint32_t base_addr, uint32_t phy_txexitulpsclk);
int32_t dsi_host_set_phy_txrequlpslan(uint32_t base_addr, uint32_t phy_txrequlpslan);
int32_t dsi_host_set_phy_txexitulpslan(uint32_t base_addr, uint32_t phy_txexitulpslan);
int32_t dsi_host_set_phy_tx_triggers(uint32_t base_addr, uint32_t phy_tx_triggers);
int32_t dsi_host_get_phy_lock(uint32_t base_addr, uint32_t *phy_lock);
int32_t dsi_host_get_phy_direction(uint32_t base_addr, uint32_t *phy_direction);
int32_t dsi_host_get_phy_stopstateclklane(uint32_t base_addr, uint32_t *phy_stopstateclklane);
int32_t dsi_host_get_phy_ulpsactivenotclk(uint32_t base_addr, uint32_t *phy_ulpsactivenotclk);
int32_t dsi_host_get_phy_stopstate0lane(uint32_t base_addr, uint32_t *phy_stopstate0lane);
int32_t dsi_host_get_phy_ulpsactivenot0lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot0lane);
int32_t dsi_host_get_phy_rxulpsesc0lane(uint32_t base_addr, uint32_t *phy_rxulpsesc0lane);
int32_t dsi_host_get_phy_stopstate1lane(uint32_t base_addr, uint32_t *phy_stopstate1lane);
int32_t dsi_host_get_phy_ulpsactivenot1lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot1lane);
int32_t dsi_host_get_phy_stopstate2lane(uint32_t base_addr, uint32_t *phy_stopstate2lane);
int32_t dsi_host_get_phy_ulpsactivenot2lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot2lane);
int32_t dsi_host_get_phy_stopstate3lane(uint32_t base_addr, uint32_t *phy_stopstate3lane);
int32_t dsi_host_get_phy_ulpsactivenot3lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot3lane);
int32_t dsi_host_set_phy_testclr(uint32_t base_addr, uint32_t phy_testclr);
int32_t dsi_host_set_phy_testclk(uint32_t base_addr, uint32_t phy_testclk);
int32_t dsi_host_set_phy_testdin(uint32_t base_addr, uint32_t phy_testdin);
int32_t dsi_host_set_phy_testdout(uint32_t base_addr, uint32_t phy_testdout);
int32_t dsi_host_set_phy_testen(uint32_t base_addr, uint32_t phy_testen);
int32_t dsi_host_get_int0_status_ack_with_err_0(uint32_t base_addr, uint32_t *ack_with_err_0);
int32_t dsi_host_get_int0_status_ack_with_err_1(uint32_t base_addr, uint32_t *ack_with_err_1);
int32_t dsi_host_get_int0_status_ack_with_err_2(uint32_t base_addr, uint32_t *ack_with_err_2);
int32_t dsi_host_get_int0_status_ack_with_err_3(uint32_t base_addr, uint32_t *ack_with_err_3);
int32_t dsi_host_get_int0_status_ack_with_err_4(uint32_t base_addr, uint32_t *ack_with_err_4);
int32_t dsi_host_get_int0_status_ack_with_err_5(uint32_t base_addr, uint32_t *ack_with_err_5);
int32_t dsi_host_get_int0_status_ack_with_err_6(uint32_t base_addr, uint32_t *ack_with_err_6);
int32_t dsi_host_get_int0_status_ack_with_err_7(uint32_t base_addr, uint32_t *ack_with_err_7);
int32_t dsi_host_get_int0_status_ack_with_err_8(uint32_t base_addr, uint32_t *ack_with_err_8);
int32_t dsi_host_get_int0_status_ack_with_err_9(uint32_t base_addr, uint32_t *ack_with_err_9);
int32_t dsi_host_get_int0_status_ack_with_err_10(uint32_t base_addr, uint32_t *ack_with_err_10);
int32_t dsi_host_get_int0_status_ack_with_err_11(uint32_t base_addr, uint32_t *ack_with_err_11);
int32_t dsi_host_get_int0_status_ack_with_err_12(uint32_t base_addr, uint32_t *ack_with_err_12);
int32_t dsi_host_get_int0_status_ack_with_err_13(uint32_t base_addr, uint32_t *ack_with_err_13);
int32_t dsi_host_get_int0_status_ack_with_err_14(uint32_t base_addr, uint32_t *ack_with_err_14);
int32_t dsi_host_get_int0_status_ack_with_err_15(uint32_t base_addr, uint32_t *ack_with_err_15);
int32_t dsi_host_get_int0_status_dphy_errors_0(uint32_t base_addr, uint32_t *dphy_errors_0);
int32_t dsi_host_get_int0_status_dphy_errors_1(uint32_t base_addr, uint32_t *dphy_errors_1);
int32_t dsi_host_get_int0_status_dphy_errors_2(uint32_t base_addr, uint32_t *dphy_errors_2);
int32_t dsi_host_get_int0_status_dphy_errors_3(uint32_t base_addr, uint32_t *dphy_errors_3);
int32_t dsi_host_get_int0_status_dphy_errors_4(uint32_t base_addr, uint32_t *dphy_errors_4);
int32_t dsi_host_get_int0_status(uint32_t base_addr, uint32_t *int0_status);
int32_t dsi_host_get_int1_status_to_hs_tx(uint32_t base_addr, uint32_t *to_hs_tx);
int32_t dsi_host_get_int1_status_to_lp_rx(uint32_t base_addr, uint32_t *to_lp_rx);
int32_t dsi_host_get_int1_status_ecc_single_err(uint32_t base_addr, uint32_t *ecc_single_err);
int32_t dsi_host_get_int1_status_ecc_multi_err(uint32_t base_addr, uint32_t *ecc_multi_err);
int32_t dsi_host_get_int1_status_crc_err(uint32_t base_addr, uint32_t *crc_err);
int32_t dsi_host_get_int1_status_pkt_size_err(uint32_t base_addr, uint32_t *pkt_size_err);
int32_t dsi_host_get_int1_status_eopt_err(uint32_t base_addr, uint32_t *eopt_err);
int32_t dsi_host_get_int1_status_dpi_pld_wr_err(uint32_t base_addr, uint32_t *dpi_pld_wr_err);
int32_t dsi_host_get_int1_status_gen_cmd_wr_err(uint32_t base_addr, uint32_t *gen_cmd_wr_err);
int32_t dsi_host_get_int1_status_gen_pld_wr_err(uint32_t base_addr, uint32_t *gen_pld_wr_err);
int32_t dsi_host_get_int1_status_gen_pld_send_err(uint32_t base_addr, uint32_t *gen_pld_send_err);
int32_t dsi_host_get_int1_status_gen_pld_rd_err(uint32_t base_addr, uint32_t *gen_pld_rd_err);
int32_t dsi_host_get_int1_status_gen_pld_recev_err(uint32_t base_addr, uint32_t *gen_pld_recev_err);
int32_t dsi_host_get_int1_status_dbi_cmd_wr_err(uint32_t base_addr, uint32_t *dbi_cmd_wr_err);
int32_t dsi_host_get_int1_status_dbi_pld_wr_err(uint32_t base_addr, uint32_t *dbi_pld_wr_err);
int32_t dsi_host_get_int1_status_dbi_pld_rd_err(uint32_t base_addr, uint32_t *dbi_pld_rd_err);
int32_t dsi_host_get_int1_status_dbi_pld_recv_err(uint32_t base_addr, uint32_t *dbi_pld_recv_err);
int32_t dsi_host_get_int1_status_dbi_ilegal_comm_err(uint32_t base_addr, uint32_t *dbi_ilegal_comm_err);
int32_t dsi_host_get_int1_status(uint32_t base_addr, uint32_t *int1_status);
int32_t dsi_host_set_int0_ack_with_err_0(uint32_t base_addr, uint32_t ack_with_err_0);
int32_t dsi_host_set_int0_ack_with_err_1(uint32_t base_addr, uint32_t ack_with_err_1);
int32_t dsi_host_set_int0_ack_with_err_2(uint32_t base_addr, uint32_t ack_with_err_2);
int32_t dsi_host_set_int0_ack_with_err_3(uint32_t base_addr, uint32_t ack_with_err_3);
int32_t dsi_host_set_int0_ack_with_err_4(uint32_t base_addr, uint32_t ack_with_err_4);
int32_t dsi_host_set_int0_ack_with_err_5(uint32_t base_addr, uint32_t ack_with_err_5);
int32_t dsi_host_set_int0_ack_with_err_6(uint32_t base_addr, uint32_t ack_with_err_6);
int32_t dsi_host_set_int0_ack_with_err_7(uint32_t base_addr, uint32_t ack_with_err_7);
int32_t dsi_host_set_int0_ack_with_err_8(uint32_t base_addr, uint32_t ack_with_err_8);
int32_t dsi_host_set_int0_ack_with_err_9(uint32_t base_addr, uint32_t ack_with_err_9);
int32_t dsi_host_set_int0_ack_with_err_10(uint32_t base_addr, uint32_t ack_with_err_10);
int32_t dsi_host_set_int0_ack_with_err_11(uint32_t base_addr, uint32_t ack_with_err_11);
int32_t dsi_host_set_int0_ack_with_err_12(uint32_t base_addr, uint32_t ack_with_err_12);
int32_t dsi_host_set_int0_ack_with_err_13(uint32_t base_addr, uint32_t ack_with_err_13);
int32_t dsi_host_set_int0_ack_with_err_14(uint32_t base_addr, uint32_t ack_with_err_14);
int32_t dsi_host_set_int0_ack_with_err_15(uint32_t base_addr, uint32_t ack_with_err_15);
int32_t dsi_host_set_int0_dphy_errors_0(uint32_t base_addr, uint32_t dphy_errors_0);
int32_t dsi_host_set_int0_dphy_errors_1(uint32_t base_addr, uint32_t dphy_errors_1);
int32_t dsi_host_set_int0_dphy_errors_2(uint32_t base_addr, uint32_t dphy_errors_2);
int32_t dsi_host_set_int0_dphy_errors_3(uint32_t base_addr, uint32_t dphy_errors_3);
int32_t dsi_host_set_int0_dphy_errors_4(uint32_t base_addr, uint32_t dphy_errors_4);
int32_t dsi_host_set_int0(uint32_t base_addr, uint32_t int0_mask);
int32_t dsi_host_set_int1_to_hs_tx(uint32_t base_addr, uint32_t to_hs_tx);
int32_t dsi_host_set_int1_to_lp_rx(uint32_t base_addr, uint32_t to_lp_rx);
int32_t dsi_host_set_int1_ecc_single_err(uint32_t base_addr, uint32_t ecc_single_err);
int32_t dsi_host_set_int1_ecc_multi_err(uint32_t base_addr, uint32_t ecc_multi_err);
int32_t dsi_host_set_int1_crc_err(uint32_t base_addr, uint32_t crc_err);
int32_t dsi_host_set_int1_pkt_size_err(uint32_t base_addr, uint32_t pkt_size_err);
int32_t dsi_host_set_int1_eopt_err(uint32_t base_addr, uint32_t eopt_err);
int32_t dsi_host_set_int1_dpi_pld_wr_err(uint32_t base_addr, uint32_t dpi_pld_wr_err);
int32_t dsi_host_set_int1_gen_cmd_wr_err(uint32_t base_addr, uint32_t gen_cmd_wr_err);
int32_t dsi_host_set_int1_gen_pld_wr_err(uint32_t base_addr, uint32_t gen_pld_wr_err);
int32_t dsi_host_set_int1_gen_pld_send_err(uint32_t base_addr, uint32_t gen_pld_send_err);
int32_t dsi_host_set_int1_gen_pld_rd_err(uint32_t base_addr, uint32_t gen_pld_rd_err);
int32_t dsi_host_set_int1_gen_pld_recev_err(uint32_t base_addr, uint32_t gen_pld_recev_err);
int32_t dsi_host_set_int1_dbi_cmd_wr_err(uint32_t base_addr, uint32_t dbi_cmd_wr_err);
int32_t dsi_host_set_int1_dbi_pld_wr_err(uint32_t base_addr, uint32_t dbi_pld_wr_err);
int32_t dsi_host_set_int1_dbi_pld_rd_err(uint32_t base_addr, uint32_t dbi_pld_rd_err);
int32_t dsi_host_set_int1_dbi_pld_recv_err(uint32_t base_addr, uint32_t dbi_pld_recv_err);
int32_t dsi_host_set_int1_dbi_ilegal_comm_err(uint32_t base_addr, uint32_t dbi_ilegal_comm_err);
int32_t dsi_host_set_int1(uint32_t base_addr, uint32_t int1_mask);
int32_t dsi_host_set_txskewcalhs(uint32_t base_addr, uint32_t txskewcalhs);
int32_t dsi_host_set_compression_mode(uint32_t base_addr, uint32_t compression_mode);
int32_t dsi_host_set_compress_algo(uint32_t base_addr, uint32_t compress_algo);
int32_t dsi_host_set_pps_sel(uint32_t base_addr, dsi_host_pps_sel_e pps_sel);
int32_t dsi_host_set_max_rd_time(uint32_t base_addr, uint32_t max_rd_time);
int32_t dsi_host_set_vid_shadow_en(uint32_t base_addr, uint32_t vid_shadow_en);
int32_t dsi_host_set_vid_shadow_req(uint32_t base_addr, uint32_t vid_shadow_req);
int32_t dsi_host_set_vid_shadow_pin_req(uint32_t base_addr, uint32_t vid_shadow_pin_req);
int32_t dsi_host_get_dpi_vcid_act(uint32_t base_addr, uint32_t *dpi_vcid);
int32_t dsi_host_get_dpi_color_coding(uint32_t base_addr, dsi_host_dpi_color_format_e *dpi_color_coding);
int32_t dsi_host_get_loosely18_en(uint32_t base_addr, uint32_t *loosely18_en);
int32_t dsi_host_get_invact_lpcmd_tim_act(uint32_t base_addr, uint32_t *invact_lpcmd_time_act);
int32_t dsi_host_get_outvact_lpcmd_tim_act(uint32_t base_addr, uint32_t *outvact_lpcmd_time_act);
int32_t dsi_host_get_vid_mode_type_act(uint32_t base_addr, dsi_host_vid_mode_type_e *vid_mode_type_act);
int32_t dsi_host_get_lp_vsa_en(uint32_t base_addr, uint32_t *lp_vsa_en);
int32_t dsi_host_get_lp_vbp_en(uint32_t base_addr, uint32_t *lp_vbp_en);
int32_t dsi_host_get_lp_vfp_en(uint32_t base_addr, uint32_t *lp_vfp_en);
int32_t dsi_host_get_lp_vact_en(uint32_t base_addr, uint32_t *lp_vact_en);
int32_t dsi_host_get_lp_hbp_en(uint32_t base_addr, uint32_t *lp_hbp_en);
int32_t dsi_host_get_lp_hfp_en(uint32_t base_addr, uint32_t *lp_hfp_en);
int32_t dsi_host_get_frame_bta_ack_en(uint32_t base_addr, uint32_t *frame_bta_ack_en);
int32_t dsi_host_get_lp_cmd_en(uint32_t base_addr, uint32_t *lp_cmd_en);
int32_t dsi_host_get_vid_pkt_size_act(uint32_t base_addr, uint32_t *vid_pkt_size_act);
int32_t dsi_host_get_vid_num_chunks_act(uint32_t base_addr, uint32_t *vid_num_chunks_act);
int32_t dsi_host_get_vid_null_size_act(uint32_t base_addr, uint32_t *vid_null_size_act);
int32_t dsi_host_get_vid_has_time_act(uint32_t base_addr, uint32_t *vid_has_time_act);
int32_t dsi_host_get_vid_hbp_time_act(uint32_t base_addr, uint32_t *vid_hbp_time_act);
int32_t dsi_host_get_vid_hline_time_act(uint32_t base_addr, uint32_t *vid_hline_time_act);
int32_t dsi_host_get_vsa_lines(uint32_t base_addr, uint32_t *vsa_lines);
int32_t dsi_host_get_vbp_lines(uint32_t base_addr, uint32_t *vbp_lines);
int32_t dsi_host_get_vfp_lines(uint32_t base_addr, uint32_t *vfp_lines);
int32_t dsi_host_get_v_active_lines(uint32_t base_addr, uint32_t *v_active_lines);
int32_t dsi_host_get_mode_3d_act(uint32_t base_addr, dsi_host_mode_3d_e *mode_3d_act);
int32_t dsi_host_get_format_3d_act(uint32_t base_addr, dsi_host_format_3d_e *format_3d_act);
int32_t dsi_host_get_second_vsync_act(uint32_t base_addr, uint32_t *second_vsync_act);
int32_t dsi_host_get_right_first_act(uint32_t base_addr, uint32_t *right_first_act);
int32_t dsi_host_get_send_3d_cfg_act(uint32_t base_addr, uint32_t *send_3d_cfg_act);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

