#include "vou_proc.h"
#include "vou_register_define.h"
#include "vou_register_list.h"
#include "vou_register_write.h"
#include "vou_osal.h"

uint32_t chn_base[VOU_DEV_IDX_NUM] = {
	DHD0_CHN_BASE,
	DHD1_CHN_BASE,
	DSD_CHN_BASE
};

uint32_t chn_coef_base[VOU_DEV_IDX_NUM] = {
	DHD0_CHN_COEF_BASE,
	DHD1_CHN_COEF_BASE
};

void _vou_dev_write_register(vou_cfg_t *context, vou_dev_idx_e devId, uint32_t offset, uint32_t value)
{
	vou_write_register(&context->regbase, chn_base[devId], offset, value);
}

void _vou_dev_write_coef_register(vou_cfg_t *context, vou_dev_idx_e devId, uint32_t offset, uint32_t value)
{
	vou_write_register(&context->regbase, chn_coef_base[devId], offset, value);
}

uint32_t _vou_dev_read_register(vou_cfg_t *context, vou_dev_idx_e devId, uint32_t offset)
{
	return vou_read_register(&context->regbase, chn_base[devId], offset);
}

void set_hdmi_cfg(vou_cfg_t *context, vou_dev_idx_e devId, uint8_t eb)
{
	vou_intf_info_t *intf_info = &context->devcfg[devId].intf_info;
	vou_sync_info_t *sync_info = context->device[devId].sync_info;
	
	if ( eb ) {
		//reset hdmi controller
		hdmi_write_register_byte_setbit(&context->regbase, 0x0, 0x4002, 0xb);
		hdmi_write_register_byte_clrbit(&context->regbase, 0x0, 0x4002, 0xb);
		topapb_write_register_bit(&context->regbase, 0x0, 0x48, sync_info->Clock+ 0x40000, 0, 20);
		udelay(10);
		topapb_write_register_bit(&context->regbase, 0x0, 0x48, sync_info->Clock, 0, 20);

		hdmi_write_register_byte(&context->regbase, 0x0, 0x1000, 0x18 | (sync_info->Vpol<<6) | (sync_info->Hpol<<5) );
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1001, sync_info->Hactvie & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1002, (sync_info->Hactvie & 0xF00)>>8);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1003, sync_info->Hblank & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1004, (sync_info->Hblank & 0xFF00)>>8);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1005, sync_info->Vactive & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1006, (sync_info->Vactive & 0xFF00)>>8);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1007, sync_info->Vblank & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1008, sync_info->Hfront & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1009, (sync_info->Hfront & 0xFF00)>>8);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x100a, sync_info->Hsync & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x100b, (sync_info->Hsync & 0xFF00)>>8);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x100c, sync_info->Vfront & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x100d, sync_info->Vsync & 0xFF);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1011, 0xC);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x1012, 0x20);

		if ( sync_info->Vactive > 1000 ) {
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1013, 0x51);
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1014, 0x0);
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1015, 0x0);
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1016, 0x3f);
		}
		else {
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1013, 0x21);
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1014, 0xff);
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1015, 0x15);
			hdmi_write_register_byte(&context->regbase, 0x0, 0x1016, 0x0);
		}

		// reset HDMI PHY
		hdmi_write_register_byte(&context->regbase, 0x0, 0x4005, 0x1);
		hdmi_write_register_byte(&context->regbase, 0x0, 0x4005, 0x0);

		analog_write_register_bit(&context->regbase, 0x0, 0x80, 0xCCC, 16, 12);

		if (intf_info->RealPixelRate > 148500000 ) { //when clk freq > 148.5MHz
			analog_write_register(&context->regbase, 0x0, 0x78, 0x001f000f);
			analog_write_register(&context->regbase, 0x0, 0x7c, 0x001f001f);
		}
		else { //when clk freq <= 148.5MHz
			analog_write_register(&context->regbase, 0x0, 0x78, 0x000c000c);
			analog_write_register(&context->regbase, 0x0, 0x7c, 0x000c000c);
		}
	}
	else {
		// reset HDMI PHY
		hdmi_write_register_byte(&context->regbase, 0x0, 0x4005, 0x1);
	}
}

void set_vga_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_intf_info_t *intf_info = &context->devcfg[devId].intf_info;
	vou_sync_info_t *sync_info = context->device[devId].sync_info;
	
	topapb_write_register_bit(&context->regbase, 0x0, 0x50, sync_info->Clock + 0x40000, 0, 20);
	udelay(10);
	topapb_write_register_bit(&context->regbase, 0x0, 0x50, sync_info->Clock, 0, 20);

	// VDAC for VGA
	analog_write_register(&context->regbase, 0x0, 0x4c, 0x03800380);
	analog_write_register(&context->regbase, 0x0, 0x50, 0xa3900380);
	_vou_dev_write_register(context, devId, HTV_ENC_BASE + TVE_SYNC_CFG_OFFSET, (intf_info->hdmi_info.vsync_pol<<2) | (intf_info->hdmi_info.hsync_pol<<1));
}

void set_clk_select(vou_cfg_t *context)
{
	vou_global_t *glbcfg = &context->glbcfg;
	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 0, 6, 1);
	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 0, 10, 1);

	if(glbcfg->hdmi_if_eb == 1 && glbcfg->vga_if_eb == 1) {
		if(glbcfg->hdmi_src_sel == 0 && glbcfg->vga_src_sel == 0){		
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x0, 0, 3);	 //dhd0->hdmi & vga
		}
		
		if(glbcfg->hdmi_src_sel == 1 && glbcfg->vga_src_sel == 1){		
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x0, 0, 3);	 //dhd1->hdmi & vga
		}
		
		if(glbcfg->hdmi_src_sel == 0 && glbcfg->vga_src_sel == 1) { 	
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x3, 0, 3);	 //dhd0->hdmi  dhd1->vga
		}
		
		if(glbcfg->hdmi_src_sel == 1 && glbcfg->vga_src_sel == 0) { 	
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x5, 0, 3);	 //dhd0->vga   dhd1->hdmi
		}
	}else if(glbcfg->hdmi_if_eb == 1 && glbcfg->vga_if_eb == 0) {
		if(glbcfg->hdmi_src_sel == 0 && glbcfg->vga_src_sel == 0) {
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x0, 0, 3);	 //dhd0->hdmi
		}

		if(glbcfg->hdmi_src_sel == 1 && glbcfg->vga_src_sel == 0) {
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x0, 0, 3);	 //dhd1->hdmi
		}
	}else if(glbcfg->hdmi_if_eb == 0 && glbcfg->vga_if_eb == 1) {
		if(glbcfg->hdmi_src_sel == 0 && glbcfg->vga_src_sel == 0) {
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x5, 0, 3);	 //dhd0->vga
		}

		if(glbcfg->hdmi_src_sel == 0 && glbcfg->vga_src_sel == 1) {
			vioapb_write_register_bit(&context->regbase, 0x0, 0x78, 0x5, 0, 3);	 //dhd1->vga
		}
	}else
		;

	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 1, 6, 1);
	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 1, 10, 1);
}

void set_clk_eb(vou_cfg_t *context)
{
	union _VOU_DSD_CK_EB_tag dsd_clk = {};
	union _VOU_DHD0_CK_EB_tag dhd0_clk = {};
	union _VOU_DHD1_CK_EB_tag dhd1_clk = {};
	union _VOU_HDMI0_SRC_tag	hdmi_src_sel = {};
	union _VOU_VGA_SRC_tag		 vga_src_sel = {};

	dsd_clk.mBits.dsd_chn_ck_eb = context->glbcfg.dsd_chn_ck_eb;
	dhd0_clk.mBits.dhd0_chn_ck_eb = context->glbcfg.dhd_chn_ck_eb;
	dhd1_clk.mBits.dhd1_chn_ck_eb = context->glbcfg.dhd1_chn_ck_eb;	

	vou_write_register(&context->regbase, GLOBAL_BASE, VOU_DSD_CK_EB_OFFSET, dsd_clk.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, VOU_DHD0_CK_EB_OFFSET, dhd0_clk.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, VOU_DHD1_CK_EB_OFFSET, dhd1_clk.dwValue);

	hdmi_src_sel.mBits.hdmi0_eb = context->glbcfg.hdmi_if_eb;
	hdmi_src_sel.mBits.hdmi0_src_sel = context->glbcfg.hdmi_src_sel;
	vou_write_register(&context->regbase, GLOBAL_BASE, VOU_HDMI_SRC_OFFSET, hdmi_src_sel.dwValue);

	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 0, 6, 1);
	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 0, 10, 1);
	vga_src_sel.mBits.vga_eb = context->glbcfg.vga_if_eb;
	vga_src_sel.mBits.vga_src_sel = context->glbcfg.vga_src_sel;
	vou_write_register(&context->regbase, GLOBAL_BASE, VOU_VGA_SRC_OFFSET, vga_src_sel.dwValue);

	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 1, 6, 1);
	topapb_write_register_bit(&context->regbase, 0x0, 0x4c, 1, 10, 1);
}

void set_channel_vga_hdmi_eb(vou_cfg_t *context)
{
	union _CHN_HDMI_EB_tag hdmi_eb = {};
	union _CHN_VGA_EB_tag vga_eb = {};

	if(context->glbcfg.vga_if_eb == 1) {
		vga_eb.mBits.chn_vga_eb = context->glbcfg.vga_if_eb;
		if(context->glbcfg.vga_src_sel == 0){
			vou_write_register(&context->regbase, DHD0_CHN_BASE, CHN_VGA_EB_OFFSET, vga_eb.dwValue);
		}else{
			vou_write_register(&context->regbase, DHD1_CHN_BASE, CHN_VGA_EB_OFFSET, vga_eb.dwValue);
		}
	}
	
	if(context->glbcfg.hdmi_if_eb == 1) {
		hdmi_eb.mBits.chn_hdmi_eb = context->glbcfg.hdmi_if_eb;
		if(context->glbcfg.hdmi_src_sel == 0){
			vou_write_register(&context->regbase, DHD0_CHN_BASE, CHN_HDMI_EB_OFFSET, hdmi_eb.dwValue);
		}else{
			vou_write_register(&context->regbase, DHD1_CHN_BASE, CHN_HDMI_EB_OFFSET, hdmi_eb.dwValue);
		}
	}	
}

void set_global(vou_cfg_t *context)
{
	union _ARQOS_LOW_tag      arqos_low      = {};
	union _ARQOS_HIGH_tag     arqos_high     = {};
	union _ARQOS_THR_LOW_tag  arqos_thr_low  = {};
	union _ARQOS_THR_HIGH_tag arqos_thr_high = {};
	union _AWQOS_LOW_tag      awqos_low      = {};
	union _AWQOS_HIGH_tag     awqos_high     = {};
	union _AWQOS_THR_LOW_tag  awqos_thr_low  = {};
	union _AWQOS_THR_HIGH_tag awqos_thr_high = {};
	union _DHD_DISPLAY_FORMAT_tag sel = {};

//	uint32_t arqos = vou_read_register(&context->regbase, VOU_REGBASE_IDX_QOS, 0x0, 0x0);
//	uint32_t awqos = vou_read_register(&context->regbase, VOU_REGBASE_IDX_QOS, 0x0, 0x4);

	arqos_low.mBits.qos            = context->glbcfg.arqos_low;
	arqos_high.mBits.qos           = context->glbcfg.arqos_high;
	arqos_thr_low.mBits.threshold  = context->glbcfg.arqos_thr_low;
	arqos_thr_high.mBits.threshold = context->glbcfg.arqos_thr_high;
	awqos_low.mBits.qos            = context->glbcfg.awqos_low;
	awqos_high.mBits.qos           = context->glbcfg.awqos_high;
	awqos_thr_low.mBits.threshold  = context->glbcfg.awqos_thr_low;
	awqos_thr_high.mBits.threshold = context->glbcfg.awqos_thr_high;

//	if ( arqos && arqos^0xFFFFFFFF && awqos && awqos^0xFFFFFFFF ) {
//		arqos_low.mBits.qos            = ( arqos >> 26 ) & 0xF;
//		arqos_high.mBits.qos           = ( arqos >> 22 ) & 0xF;
//		arqos_thr_low.mBits.threshold  = ( arqos >> 11 ) & 0x7FF;
//		arqos_thr_high.mBits.threshold = arqos & 0x7FF;
//		awqos_low.mBits.qos            = ( awqos >> 24 ) & 0xF;
//		awqos_high.mBits.qos           = ( awqos >> 20 ) & 0xF;
//		awqos_thr_low.mBits.threshold  = ( awqos >> 10 ) & 0x3FF;
//		awqos_thr_high.mBits.threshold = awqos & 0x3FF;
//	}

// 0x8 << 26 + 0xa << 22 + 0x300 << 11 + 0x600
// 0x7 << 24 + 0xa << 20 + 0x80 << 10 + 0x100
//	printk("qos:%x/%x\n", arqos, awqos);
//	printk("split.qos:%x/%x/%x/%x/%x/%x/%x/%x\n", arqos_low.mBits.qos            , arqos_high.mBits.qos           , arqos_thr_low.mBits.threshold  , arqos_thr_high.mBits.threshold , awqos_low.mBits.qos            , awqos_high.mBits.qos           , awqos_thr_low.mBits.threshold  , awqos_thr_high.mBits.threshold );

	vou_write_register(&context->regbase, GLOBAL_BASE, ARQOS_LOW_OFFSET, arqos_low.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, ARQOS_HIGH_OFFSET, arqos_high.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, ARQOS_THR_LOW_OFFSET, arqos_thr_low.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, ARQOS_THR_HIGH_OFFSET, arqos_thr_high.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, AWQOS_LOW_OFFSET, awqos_low.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, AWQOS_HIGH_OFFSET, awqos_high.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, AWQOS_THR_LOW_OFFSET, awqos_thr_low.dwValue);
	vou_write_register(&context->regbase, GLOBAL_BASE, AWQOS_THR_HIGH_OFFSET, awqos_thr_high.dwValue);

	switch ( context->glbcfg.dhd_output_dst ) {
		case DHD_OUTPUT_DST_HDMI:
			sel.mBits.dhd_display_format = 0;
			break;
		case DHD_OUTPUT_DST_DSI:
			sel.mBits.dhd_display_format = 1;
			break;
		case DHD_OUTPUT_DST_AVE:
			sel.mBits.dhd_display_format = 2;
			break;
		case DHD_OUTPUT_DST_BT:
			sel.mBits.dhd_display_format = 3;
			break;
		case DHD_OUTPUT_DST_LCM:
			sel.mBits.dhd_display_format = 4;
			break;
		default:
			break;
	}
	vou_write_register(&context->regbase, GLOBAL_BASE, DHD_DISPLAY_FORMAT_OFFSET, sel.dwValue);
}

void set_work_ctrl(vou_cfg_t *context, vou_dev_idx_e devId, uint8_t force, uint8_t eb, uint8_t all)
{
	union _vou_chn_work_ctrl_tag work_ctrl = {};
	union _vou_chn_force_update_tag force_update = {};
	union _vou_update_mode_tag update_mode = {};

	force_update.mBits.force_update = force;
	_vou_dev_write_register(context, devId, CHN_FORCE_UPDATE_OFFSET, force_update.dwValue);
	
	work_ctrl.mBits.chn_work_eb = eb;
	work_ctrl.mBits.chn_shutdown = !(force||eb);
	_vou_dev_write_register(context, devId, CHN_WORK_CTRL_OFFSET, work_ctrl.dwValue);

	update_mode.mBits.all_update_mode = all;
	_vou_dev_write_register(context, devId, CHN_UPDATE_MODE_OFFSET, update_mode.dwValue);
	
	
}

uint8_t get_work_ctrl(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _vou_chn_work_ctrl_tag work_ctrl;
	work_ctrl.dwValue = _vou_dev_read_register(context, devId, CHN_WORK_CTRL_OFFSET);
	return work_ctrl.mBits.chn_work_eb;
}

bool set_update_ctrl(vou_cfg_t *context, vou_dev_idx_e devId, update_ctrl_t *ctrl)
{
	int i;
	bool ret = false;
//	printe("set update_ctrl0 = 0x%x,ctrl1=0x%x,ctrl2=0x%x\n",ctrl->update_value[devId][0],
//		ctrl->update_value[devId][1],ctrl->update_value[devId][2]);
	for(i=0;i<UPDATE_CTRL_IDX_NUM;i++) {
		if(ctrl->update_value[devId][i]) {
			switch(i) {
				case UPDATE_CTRL_IDX0:
					_vou_dev_write_register(context, devId, CHN_UPDATE_CTRL0_OFFSET, ctrl->update_value[devId][0]);
					ret = true;
					break;
				case UPDATE_CTRL_IDX1:
					_vou_dev_write_register(context, devId, CHN_UPDATE_CTRL1_OFFSET, ctrl->update_value[devId][1]);
					ret = true;
					break;
				case UPDATE_CTRL_IDX2:
					_vou_dev_write_register(context, devId, CHN_UPDATE_CTRL2_OFFSET, ctrl->update_value[devId][2]);
					ret = true;
					break;
				default:
					break;
			}
		}
	}
	return ret;
}

void merge_update_ctrl(vou_dev_idx_e devId, update_ctrl_t *dst, update_ctrl_t * src) {
	int i;

	for(i=0;i<UPDATE_CTRL_IDX_NUM;i++) {
		dst->update_value[devId][i] |= src->update_value[devId][i];
	}
}
update_ctrl_t get_update_ctrl(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _vou_update_ctrl0_tag update_ctrl0;
	union _vou_update_ctrl1_tag update_ctrl1;
	union _vou_update_ctrl2_tag update_ctrl2;
	update_ctrl_t ctrl;
	
	update_ctrl0.dwValue = _vou_dev_read_register(context, devId, CHN_UPDATE_CTRL1_OFFSET);
	update_ctrl1.dwValue = _vou_dev_read_register(context, devId, CHN_UPDATE_CTRL2_OFFSET);
	update_ctrl2.dwValue = _vou_dev_read_register(context, devId, CHN_UPDATE_CTRL0_OFFSET);
	ctrl.update_value[devId][UPDATE_CTRL_IDX0] = update_ctrl0.dwValue;
	ctrl.update_value[devId][UPDATE_CTRL_IDX1] = update_ctrl1.dwValue;
	ctrl.update_value[devId][UPDATE_CTRL_IDX2] = update_ctrl2.dwValue;
	return ctrl;
}

void set_irq_mask(vou_cfg_t *context, vou_dev_idx_e devId, uint32_t mask)
{
	_vou_dev_write_register(context, devId, CHN_INT_MASK_OFFSET, mask);
}

void set_tve_clamp_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	tve_clamp_info_t *clamp_info = &context->devcfg[devId].intf_info.clamp_info;
	union _vou_clamp_range_tag range = {};
	union _vou_clamp_offset_tag offset = {};
	uint32_t base_addr = HTV_ENC_BASE;

	range.mBits.level_low = clamp_info->yrgb_black_level;
	range.mBits.level_high = clamp_info->yrgb_white_level;
	range.mBits.blank_level = clamp_info->yrgb_blank_level;
	_vou_dev_write_register(context, devId, base_addr + TVE_CLAMP_INFO0_OFFSET, range.dwValue);

	range.mBits.level_low = clamp_info->cbcr_peak_level_low;
	range.mBits.level_high = clamp_info->cbcr_peak_level_high;
	range.mBits.blank_level = clamp_info->cbcr_blank_level;
	_vou_dev_write_register(context, devId, base_addr + TVE_CLAMP_INFO1_OFFSET, range.dwValue);

	offset.mBits.ofs0 = clamp_info->yrgb_ofs0;
	offset.mBits.ofs1 = clamp_info->yrgb_ofs1;
	_vou_dev_write_register(context, devId, base_addr + TVE_CLAMP_INFO2_OFFSET, offset.dwValue);

	offset.mBits.ofs0 = clamp_info->cbcr_ofs0;
	offset.mBits.ofs1 = clamp_info->cbcr_ofs1;
	_vou_dev_write_register(context, devId, base_addr + TVE_CLAMP_INFO3_OFFSET, offset.dwValue);
}

void set_tve_sync_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	tve_sync_t *info = &context->devcfg[devId].intf_info.tve_sync_info;
	uint32_t base_addr = HTV_ENC_BASE;
	
	_vou_dev_write_register(context, devId, base_addr + TVE_HSYNC_INFO_OFFSET, info->hsync);
	_vou_dev_write_register(context, devId, base_addr + TVE_VSYNC_INFO0_OFFSET, info->vsync0);
	_vou_dev_write_register(context, devId, base_addr + TVE_VSYNC_INFO1_OFFSET, info->vsync1);
}

void set_tve_fir_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	uint8_t bypass = context->devcfg[devId].intf_info.tve_fir_bypass;
	uint32_t base_addr = HTV_ENC_BASE;

	_vou_dev_write_register(context, devId, base_addr + TVE_FIR_CFG_OFFSET, bypass);
}

void set_cvbs_video_info(vou_cfg_t *context)
{
	bool ntsc = context->devcfg[VOU_DEV_IDX_DSD].intf_info.cvbs_ntsc_mode;
	union _cvbs_video_format_tag format = {};
	union _cvbs_video_input_tag input = {};
	union _cvbs_video_output_tag output = {};
	uint32_t base_addr = CVBS_BASE;

	format.mBits.black0ire = 1;
	if ( ntsc ) {
		format.mBits.tvf = 0;
	}
	else {
		format.mBits.tvf = 2;
	}

	input.mBits.field_mode = 0;
	input.mBits.vinf = 1; // BT656

	_vou_dev_write_register(context, VOU_DEV_IDX_DSD, base_addr + CVBS_VIDEO_FORMAT_CTRL_OFFSET, format.dwValue);
	_vou_dev_write_register(context, VOU_DEV_IDX_DSD, base_addr + CVBS_VIDEO_INPUT_CTRL_OFFSET, input.dwValue);
	_vou_dev_write_register(context, VOU_DEV_IDX_DSD, base_addr + CVBS_VIDEO_OUTPUT_CTRL_OFFSET, output.dwValue);
}

void set_chn_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_dev_cfg_t *info = &context->devcfg[devId];
	union _vou_chn_cfg_tag param = {};

	param.mBits.Chn_out_type = info->dev_info.progressive;
	param.mBits.Clip_eb = info->dev_info.clip_eb;
	param.mBits.Clip_mode = info->dev_info.clip_mode;

	_vou_dev_write_register(context, devId, CHN_CFG_OFFSET, param.dwValue);
}

void set_layer_order_cfg(vou_cfg_t *context, vou_dev_idx_e devId) 
{
	vou_dev_cfg_t *info = &context->devcfg[devId];
	union _vou_layer_order_tag param = {};

	param.mBits.Layer_order = info->dev_info.img0_on_top;	

	_vou_dev_write_register(context, devId, CHN_LAYER_ORDER_OFFSET, param.dwValue);
}

void set_chn_minor_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _vou_chn_minor_ctrl_tag param = {};

	param.mBits.path_clr_eb = 0;
	param.mBits.sccoef_sw_cfg = 0;
	param.mBits.sccoef_rdy = 0;
	param.mBits.reset_gen_eb = 0;
	param.mBits.reset_each_frame = 0;
	param.mBits.fetch_delay_eb = 0;
	param.mBits.vblank_flush_eb = 0;
	param.mBits.lines_aheadof_frame = 0;
	
	_vou_dev_write_register(context, devId, CHN_MINOR_CTRL_OFFSET, param.dwValue);
}

void set_chn_testmode_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_dev_cfg_t *info = &context->devcfg[devId];
	union _vou_testmode_cfg_tag testmode_cfg = {};

	if ( info->dev_info.test_mode_eb ) {
		set_test_pattern_info(context, devId);
	}
	testmode_cfg.mBits.Test_mode_eb = info->dev_info.test_mode_eb;
	testmode_cfg.mBits.Test_mode_gray = info->dev_info.gray_mode;
	testmode_cfg.mBits.Test_mode = info->dev_info.test_mode;
	
	_vou_dev_write_register(context, devId, TESTMODE_CFG_OFFSET, testmode_cfg.dwValue);
	
}

void set_img0_layer_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _vou_IMG0_CFG_tag param = {};
	if (VOU_DEV_IDX_DSD == devId)
		return;
	param.mBits.Imag0_layer_eb = context->devcfg[devId].dev_info.img0_eb;
	param.mBits.Imag0_Graymode = context->devcfg[devId].dev_info.img0_gray_mode;
	param.mBits.Imag0_Galpha = context->devcfg[devId].dev_info.img0_g_alpha;
	
	_vou_dev_write_register(context, devId, IMG0_CFG_OFFSET, param.dwValue);
}

void set_img1_layer_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _vou_IMG1_CFG_tag param = {};
	
	param.mBits.Imag1_layer_eb = context->devcfg[devId].dev_info.img1_eb;
	param.mBits.Imag1_Graymode = context->devcfg[devId].dev_info.img1_gray_mode;
	param.mBits.Imag1_Galpha = context->devcfg[devId].dev_info.img1_g_alpha;
	
	_vou_dev_write_register(context, devId, IMG1_CFG_OFFSET, param.dwValue);
}


void set_bg_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_bg_color_t *info = &context->devcfg[devId].misc_info.bg_info;
	union _vou_bg_info_yuv_tag yuv = {};
	union _vou_bg_info_rgb_tag rgb = {};

	yuv.mBits.BackgroundY = info->video_yuv.y;
	yuv.mBits.BackgroundCb = info->video_yuv.u;
	yuv.mBits.BackgroundCr = info->video_yuv.v;
	_vou_dev_write_register(context, devId, BG_INFO_YUV_OFFSET, yuv.dwValue);

	if ( devId == VOU_DEV_IDX_DHD || devId == VOU_DEV_IDX_DHD1) {
		rgb.mBits.BackgroundR = info->blend_rgb.r;
		rgb.mBits.BackgroundG = info->blend_rgb.g;
		rgb.mBits.BackgroundB = info->blend_rgb.b;
		_vou_dev_write_register(context, devId, BG_INFO_RGB_OFFSET, rgb.dwValue);
	}
}

void set_active_info(vou_cfg_t *context, vou_dev_idx_e devId)
{

	vou_dev_info_t *info = &context->devcfg[devId].dev_info;
	union _vou_active_info_tag param = {};

	param.mBits.Alen = info->width;
	param.mBits.Aline = info->height;
	_vou_dev_write_register(context, devId, ACTIVE_INFO_OFFSET, param.dwValue);
}

void set_blank_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_blanking_info_t *blank_info = &context->devcfg[devId].intf_info.blank_info;
	union _vou_hblank_info_tag hblank = {};
	union _vou_tfblank_info_tag tfblank = {};
	union _vou_bfblank_info_tag bfblank = {};

	hblank.mBits.Hlen = blank_info->h_len;
	hblank.mBits.StartBLen= blank_info->start_b_len;
	_vou_dev_write_register(context, devId, HBLANK_INFO_OFFSET, hblank.dwValue);

	tfblank.mBits.TVLine = blank_info->tv_line;
	tfblank.mBits.TBLineOfs= blank_info->tb_line_ofs;
	_vou_dev_write_register(context, devId, TF_BLANK_INFO_OFFSET, tfblank.dwValue);

	bfblank.mBits.BVLine = blank_info->bv_line;
	bfblank.mBits.BBLineOfs= blank_info->bb_line_ofs;
	_vou_dev_write_register(context, devId, BF_BLANK_INFO_OFFSET, bfblank.dwValue);
}

void set_clip_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_clip_info_t *info = &context->devcfg[devId].intf_info.clip_info;
	union _vou_clip_info_tag param = {};

	param.mBits.YRGBClipLow = info->yrgb_clip_low;
	param.mBits.YRGBClipHigh= info->yrgb_clip_high;
	param.mBits.CbCrClipLow = info->uv_clip_low;
	param.mBits.CbCrClipHigh= info->uv_clip_high;
	_vou_dev_write_register(context, devId, CHN_CLIP_INFO_OFFSET, param.dwValue);
}

void set_y2r_matrix(vou_cfg_t *context, vou_dev_idx_e devId)
{
	y2r_matrix_t *matrix = &context->devcfg[devId].intf_info.y2r_matrix;
	union _vou_CSC_MATRIX_COEFF_tag coeff0 = {};
	union _vou_CSC_MATRIX_COEFF_tag coeff1 = {};
	union _vou_CSC_MATRIX_COEFF_tag coeff2 = {};
	union _vou_CSC_OFFSET_tag offset = {};

	coeff0.mBits.GY = matrix->coef[0][0] & 0x3ff;
	coeff0.mBits.BCb = matrix->coef[0][1] & 0x3ff;
	coeff0.mBits.RCr = matrix->coef[0][2] & 0x3ff;

	coeff1.mBits.GY = matrix->coef[1][0] & 0x3ff;
	coeff1.mBits.BCb = matrix->coef[1][1] & 0x3ff;
	coeff1.mBits.RCr = matrix->coef[1][2] & 0x3ff;

	coeff2.mBits.GY = matrix->coef[2][0] & 0x3ff;
	coeff2.mBits.BCb = matrix->coef[2][1] & 0x3ff;
	coeff2.mBits.RCr = matrix->coef[2][2] & 0x3ff;

	offset.mBits.GYOfs = matrix->offset[0] & 0x3ff;
	offset.mBits.BcbOfs = matrix->offset[1] & 0x3ff;
	offset.mBits.RcrOfs = matrix->offset[2] & 0x3ff;

	_vou_dev_write_register(context, devId, CSC_MATRIX_COEFF0_OFFSET, coeff0.dwValue);
	_vou_dev_write_register(context, devId, CSC_MATRIX_COEFF1_OFFSET, coeff1.dwValue);
	_vou_dev_write_register(context, devId, CSC_MATRIX_COEFF2_OFFSET, coeff2.dwValue);
	_vou_dev_write_register(context, devId, CSC_MATRIX_OFFSET_OFFSET, offset.dwValue);
}

void set_rim_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_rim_info_t *info = &context->devcfg[devId].misc_info.rim_info;
	union _vou_IMG_RIM_COLOR_tag rim_color = {};
	union _vou_IMG_RIM_WIDTH_tag rim_width = {};

	//only CHN_DHD support rim
	if (VOU_DEV_IDX_DSD == devId)
		return;

	rim_width.mBits.IMG_RIM_WIDTH = info->width;
	_vou_dev_write_register(context, devId, IMG_RIM_WIDTH_OFFSET, rim_width.dwValue);

	rim_color.mBits.IMG_RIM_C_Y = info->color[0].y;
	rim_color.mBits.IMG_RIM_C_Cr = info->color[0].u;
	rim_color.mBits.IMG_RIM_C_Cb = info->color[0].v;
	_vou_dev_write_register(context, devId, IMG_RIM_COLOR0_OFFSET, rim_color.dwValue);

	rim_color.mBits.IMG_RIM_C_Y = info->color[1].y;
	rim_color.mBits.IMG_RIM_C_Cr = info->color[1].u;
	rim_color.mBits.IMG_RIM_C_Cb = info->color[1].v;
	_vou_dev_write_register(context, devId, IMG_RIM_COLOR1_OFFSET, rim_color.dwValue);
}

void set_test_pattern_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	pattern_info_t *info = &context->devcfg[devId].misc_info.pattern_info;
	union _vou_chess_info_tag param = {};

	param.mBits.BlockWidth = info->blk_w;
	param.mBits.BlockHeight= info->blk_h;
	_vou_dev_write_register(context, devId, CHESS_INFO_OFFSET, param.dwValue);
	if (info->color_eb) {
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO0_OFFSET, info->color[0]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO1_OFFSET, info->color[1]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO2_OFFSET, info->color[2]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO3_OFFSET, info->color[3]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO4_OFFSET, info->color[4]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO5_OFFSET, info->color[5]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO6_OFFSET, info->color[6]);
		_vou_dev_write_register(context, devId, COLOR_BAR_INFO7_OFFSET, info->color[7]);
	}
}

void set_image_block(vou_cfg_t *context, vou_dev_idx_e devId, vou_yuv_layer_t *info)
{
	vou_yuv_layer_t *img_info = info;
	union _vou_IMG_BX_CTRL_tag ctrl = {};
	union _vou_IMG_BX_START_tag start = {};
	union _vou_IMG_BX_SIZE_tag size = {};
	union _vou_IMG_BX_PITCH_tag pitch = {};
	union _vou_u32_tag addr = {};

	uint32_t base_addr;

	if(devId == VOU_DEV_IDX_DSD) {
		base_addr = IMG1_OFFSET;
	} else
		base_addr = IMG0_OFFSET;

	ctrl.mBits.format = img_info->format;
	ctrl.mBits.fbc_mode = img_info->compress;
	ctrl.mBits.uv_order = img_info->uv_order;
	if (VOU_DEV_IDX_DHD == devId || VOU_DEV_IDX_DHD1 == devId) {
		ctrl.mBits.rim_en = img_info->rim_eb;
		ctrl.mBits.rim_color_mode = img_info->rim_color_idx;
		ctrl.mBits.scalor_en = img_info->scalar_eb;
	}
	ctrl.mBits.img_block_eb = img_info->blk_eb;

	_vou_dev_write_register(context, devId, base_addr + IMG_BX_CTRL_OFFSET, ctrl.dwValue);

	if (VOU_DEV_IDX_DHD == devId || VOU_DEV_IDX_DHD1 == devId) {
		start.mBits.x = img_info->src_x;
		start.mBits.y = img_info->src_y;
		_vou_dev_write_register(context, devId, base_addr + IMG_BX_SRC_START_OFFSET, start.dwValue);
	}

	size.mBits.width = img_info->src_w;
	size.mBits.height = img_info->src_h;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_SRC_SIZE_OFFSET, size.dwValue);

	pitch.mBits.pitch = img_info->src_pitch;
	if(img_info->compress == 1)
		pitch.mBits.src_orig_height = img_info->src_orig_height;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_SRC_PITCH_OFFSET, pitch.dwValue);

	addr.mBits.u32val = img_info->y_img;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_Y_BASE_ADDR_OFFSET, addr.dwValue);

	addr.mBits.u32val = img_info->uv_img;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_C_BASE_ADDR_OFFSET, addr.dwValue);

	start.mBits.x = img_info->dst_x;
	start.mBits.y = img_info->dst_y;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_DES_START_OFFSET, start.dwValue);

	if (VOU_DEV_IDX_DHD == devId || VOU_DEV_IDX_DHD1 == devId) {
		size.mBits.width = img_info->dst_w;
		size.mBits.height = img_info->dst_h;

		_vou_dev_write_register(context, devId, base_addr + IMG_BX_DES_SIZE_OFFSET, size.dwValue);
	}
}

void set_image_block_csc(vou_cfg_t *context, vou_dev_idx_e devId)
{
	cm_param_t *cm_info = &context->cm_default_cfg;
	union _vou_IMG_BX_CM_PARA0_tag param0 = {};
	union _vou_IMG_BX_CM_PARA1_tag param1 = {};
	union _vou_IMG_BX_CM_PARA2_tag param2 = {};
	union _vou_IMG_BX_CM_PARA3_tag param3 = {};

	uint32_t base_addr;

	if(devId == VOU_DEV_IDX_DSD) {
		base_addr = IMG1_OFFSET;
	} else
		base_addr = IMG0_OFFSET;

	param0.mBits.brightness = cm_info->brightness;
	param0.mBits.satvxcontrs = cm_info->uvSatvXContrs;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_CM_PARA0_OFFSET, param0.dwValue);

	if ( cm_info->hueSin < 0 ) {
		param1.mBits.huesinvalue = -cm_info->hueSin;
		param1.mBits.sighuesin = 1;
	}
	else {
		param1.mBits.huesinvalue = cm_info->hueSin;
		param1.mBits.sighuesin = 0;
	}
	if ( cm_info->hueCos < 0 ) {
		param1.mBits.huecosvalue = -cm_info->hueCos;
		param1.mBits.sighuecos = 1;
	}
	else {
		param1.mBits.huecosvalue = cm_info->hueCos;
		param1.mBits.sighuecos = 0;
	}
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_CM_PARA1_OFFSET, param1.dwValue);

	param2.mBits.shth0 = cm_info->sharpThr[0];
	param2.mBits.shth1 = cm_info->sharpThr[1];
	param2.mBits.k0 = cm_info->sharpK[0];
	param2.mBits.k1 = cm_info->sharpK[1];
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_CM_PARA2_OFFSET, param2.dwValue);

	param3.mBits.controffset = cm_info->yContrastOffset;
	param3.mBits.contrslope = cm_info->yContrastSlope;
	_vou_dev_write_register(context, devId, base_addr + IMG_BX_CM_PARA3_OFFSET, param3.dwValue);
}


void set_gui_layer(vou_cfg_t *context, vou_dev_idx_e devId, vou_argb_layer_t *osd_cfg)
{
	union _vou_OSD_CTRL_tag osd_ctrl = {};
	union _vou_OSD_ALPHA_PARA_tag osd_alpha_para = {};
	union _vou_OSD_SRC_START_tag src_start = {};
	union _vou_OSD_SRC_SIZE_tag size = {};
	union _vou_OSD_SRC_PITCH_tag pitch = {};
	union _vou_u32_tag addr = {};
	union _vou_OSD_DES_START_tag des_start = {};
	vou_dev_cfg_t *info = &context->devcfg[devId];
	
	uint32_t base_addr = 0;

	vou_argb_layer_t *cfg = osd_cfg;

	base_addr = OSD_BASE_OFFSET;

	osd_ctrl.mBits.OSD_in_format = cfg->format;
	osd_ctrl.mBits.OSD_fbc_eb = (VOU_DEV_IDX_DSD != devId) ? cfg->compress : 0;
	osd_ctrl.mBits.OSD_extend_mode = cfg->extend_mode;
	osd_ctrl.mBits.OSD_byte_order = cfg->byte_order;
	osd_ctrl.mBits.OSD_alpha_order = (cfg->format == E_ARGB8888) ? cfg->alpha_order : 0;
	osd_ctrl.mBits.OSD_layer_eb = info->dev_info.osd_eb;

//	printe("gui/hc layer eb=%d\n",osd_ctrl.mBits.OSD_layer_eb);
	_vou_dev_write_register(context, devId, base_addr + GUI_CTRL_OFFSET, osd_ctrl.dwValue);

	osd_alpha_para.mBits.OSD_palpha_en = cfg->pixel_alpha_eb;
	osd_alpha_para.mBits.OSD_Pre_multi_en = cfg->pixel_alpha_premultiplied;
	osd_alpha_para.mBits.OSD_Palpha_range = cfg->pixel_alpha_range_255;
	osd_alpha_para.mBits.OSD_Galpha = cfg->glb_alpha;
	osd_alpha_para.mBits.OSD_alpha_v0 = cfg->alpha0;
	osd_alpha_para.mBits.OSD_alpha_v1 = cfg->alpha1;
	_vou_dev_write_register(context, devId, base_addr + GUI_ALPHA_PARA_OFFSET, osd_alpha_para.dwValue);

	if (VOU_DEV_IDX_DSD != devId) {
		src_start.mBits.OSD_src_st_x = cfg->src_x;
		src_start.mBits.OSD_src_st_y = cfg->src_y;
		_vou_dev_write_register(context, devId, base_addr + GUI_SRC_START_OFFSET, src_start.dwValue);
	}

	size.mBits.OSD_src_width = cfg->src_w;
	size.mBits.OSD_src_height = cfg->src_h;
	_vou_dev_write_register(context, devId, base_addr + GUI_SRC_SIZE_OFFSET, size.dwValue); // osd 13bits, hc: 10bits

	pitch.mBits.OSD_src_pitch = cfg->src_pitch;
	if ( osd_ctrl.mBits.OSD_fbc_eb )
		pitch.mBits.src_orig_height = cfg->src_orig_height;
	_vou_dev_write_register(context, devId, base_addr + GUI_SRC_PITCH_OFFSET, pitch.dwValue); // osd 13bits, hc: 10bits;

	addr.mBits.u32val = cfg->argb_img;
	_vou_dev_write_register(context, devId, base_addr + GUI_BASE_ADDR_OFFSET, addr.dwValue);

	des_start.mBits.OSD_des_st_x = cfg->dst_x;
	des_start.mBits.OSD_des_st_y = cfg->dst_y;
	_vou_dev_write_register(context, devId, base_addr + GUI_DES_START_OFFSET, des_start.dwValue);
}

void set_vou_hdmi_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_hdmi_info_t *cfg = &context->devcfg[devId].intf_info.hdmi_info;
	union _vou_HDMI_CFG_tag hdmi = {};

	hdmi.mBits.HDMI_src = cfg->hdmi_src;
	hdmi.mBits.HDMI_HSYNC_POL = cfg->hsync_pol;
	hdmi.mBits.HDMI_VSYNC_POL = cfg->vsync_pol;
	hdmi.mBits.HDMI_DE_pol = cfg->de_pol;
	hdmi.mBits.HDMI_vtotal_even = cfg->vtotal_even;
	hdmi.mBits.clip_eb = cfg->clip_eb;
	hdmi.mBits.Csc_matrix_sel = cfg->r2y_matrix_sel;
	hdmi.mBits.clip_mode = cfg->clip_mode;
	hdmi.mBits.uv_order = cfg->hdmi_uv_order;
	_vou_dev_write_register(context, devId, HDMI_CFG_OFFSET, hdmi.dwValue);
}

