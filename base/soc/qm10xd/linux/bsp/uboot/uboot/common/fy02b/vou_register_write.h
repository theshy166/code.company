#ifndef _VOU_REGISTER_WRITE_H_
#define _VOU_REGISTER_WRITE_H_

#include "vou_proc.h"

#ifdef __cplusplus
extern "C" {
#endif
void vou_dev_init_regbufidx(vou_cfg_t *context);
void vou_dev_init_regbufdata(vou_cfg_t *context, vou_dev_idx_e devId);
void set_clk_eb(vou_cfg_t *context);
void set_channel_vga_hdmi_eb(vou_cfg_t *context);
void set_global(vou_cfg_t *context);
void set_work_ctrl(vou_cfg_t *context, vou_dev_idx_e devId, uint8_t force, uint8_t eb, uint8_t all);
uint8_t get_work_ctrl(vou_cfg_t *context, vou_dev_idx_e devId);
bool set_update_ctrl(vou_cfg_t *context, vou_dev_idx_e devId, update_ctrl_t *ctrl);
void merge_update_ctrl(vou_dev_idx_e devId, update_ctrl_t *dst, update_ctrl_t * src);
bool get_chn_idle_sts(vou_cfg_t *context, vou_dev_idx_e devId);
void set_irq_clr(vou_cfg_t *context, vou_dev_idx_e devId);
uint32_t get_irq_raw(vou_cfg_t *context, vou_dev_idx_e devId);
void set_irq_mask(vou_cfg_t *context, vou_dev_idx_e devId, uint32_t mask);
void set_tve_clamp_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_tve_sync_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_tve_fir_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_cvbs_video_info(vou_cfg_t *context);
void set_chn_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_chn_minor_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_intf_common_para(vou_cfg_t *context, vou_dev_idx_e devId);
void set_img0_layer_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_img1_layer_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_chn_testmode_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_layer_order_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_bg_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_active_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_blank_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_clip_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_y2r_matrix(vou_cfg_t *context, vou_dev_idx_e devId);
void set_rim_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_test_pattern_info(vou_cfg_t *context, vou_dev_idx_e devId);
void set_image_block(vou_cfg_t *context, vou_dev_idx_e devId, vou_yuv_layer_t *info);
void set_image_block_csc(vou_cfg_t *context, vou_dev_idx_e devId);
void set_gui_layer(vou_cfg_t *context, vou_dev_idx_e devId, vou_argb_layer_t *osd_cfg);
void set_vou_hdmi_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_hdmi_tx_info(vou_cfg_t *cfg, vou_dev_idx_e devId);
void set_vou_intf_clk(vou_cfg_t *context, vou_dev_idx_e devId);
void set_hdmi_cfg(vou_cfg_t *context, vou_dev_idx_e devId, uint8_t eb);
void set_vga_cfg(vou_cfg_t *context, vou_dev_idx_e devId);
void set_clk_select(vou_cfg_t *context);
void set_vga_hdmi_glbcfg_reset(vou_cfg_t *context, vou_dev_idx_e devId);

uint32_t set_wbc(vou_cfg_t *context, vou_dev_idx_e devId);
void set_scalar_coef(vou_cfg_t *context, vou_dev_idx_e devId, vou_sc_idx_e sidx, uint8_t scCoeIndex);
void set_coef_addr_info(vou_cfg_t *context, vou_dev_idx_e devId);
vou_yuv_layer_t *_get_yuv_layer(vou_cfg_t *context, vou_dev_idx_e devId, vou_layer_idx_e layerId);
vou_argb_layer_t *_get_argb_layer(vou_cfg_t *context, vou_dev_idx_e devId, vou_layer_idx_e layerId);
void set_display_info(vou_cfg_t *context);
#ifdef __cplusplus
}
#endif

#endif
