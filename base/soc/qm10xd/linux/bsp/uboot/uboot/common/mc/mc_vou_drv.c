#include "mc_vou_osal.h"
#include "mc_vou_drv.h"
//#include "mc_lcm_drv.h"
#include "lcm/lcm_hal.h"

extern vou_sync_info_t sync_info_t[];
extern vou_sync_info_t rgb_sync_info_t;

uint32_t chn_base[2] = {
	DHD0_CHN_BASE,
	DHD1_CHN_BASE
};

void set_global(void)
{
	writel(0x00000008, REG_VOU_BASE + ARQOS_LOW_OFFSET);
	writel(0x0000000a, REG_VOU_BASE + ARQOS_HIGH_OFFSET);
	writel(0x00000300, REG_VOU_BASE + ARQOS_THR_LOW_OFFSET);
	writel(0x00000600, REG_VOU_BASE + ARQOS_THR_HIGH_OFFSET);
}

void set_display_format(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _dhd_display_format_tag sel = {};

	sel.dwValue = readl(REG_VOU_BASE + DHD_DISPLAY_FORMAT);

	switch (context->device[devId].lcm_type) {
		case _LCM_TYPE_MIPI:
			if (devId == VOU_DEV_IDX_DHD) {
				sel.mBits.dhd0_display_format = 1;
			} else {
				sel.mBits.dhd1_display_format = 1;
			}
			break;
		case _LCM_TYPE_MCU:
		case _LCM_TYPE_RGB:
			if (devId == VOU_DEV_IDX_DHD) {
				sel.mBits.dhd0_display_format = 4;
			} else {
				sel.mBits.dhd1_display_format = 4;
			}
			break;
		default:
			break;
	}

	writel(sel.dwValue, REG_VOU_BASE + DHD_DISPLAY_FORMAT); 
}

void set_tve_sync_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_sync_info_t *sync_info = &context->device[devId].sync_info;

	writel(sync_info->HSyncLength, REG_VOU_BASE + chn_base[devId] + 0xF14);
	writel(sync_info->VSyncLength0, REG_VOU_BASE + chn_base[devId] + 0xF18);
	writel(0x00000000, REG_VOU_BASE + chn_base[devId] + 0xF1C);
}

void set_intf_common_para(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_sync_info_t *sync_info = &context->device[devId].sync_info;

	writelr(sync_info->Hfront, REG_VOU_BASE + chn_base[devId] + CHN_FRONT_PORCH, 0, 12);
	writelr(sync_info->Vfront, REG_VOU_BASE + chn_base[devId] + CHN_FRONT_PORCH, 16, 12);
	
	writelr(sync_info->Hsync, REG_VOU_BASE + chn_base[devId] + CHN_SYNC_PULSE, 0, 12);
	writelr(sync_info->Vsync, REG_VOU_BASE + chn_base[devId] + CHN_SYNC_PULSE, 16, 12);
	
	writelr(sync_info->Hback, REG_VOU_BASE + chn_base[devId] + CHN_BACK_PORCH, 0, 12);
	writelr(sync_info->Vback, REG_VOU_BASE + chn_base[devId] + CHN_BACK_PORCH, 16, 12);
}

void set_display_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union _dhd_display_cfg_tag cfg = {};

	vou_device_t * device = &context->device[devId];
	lcm_display_cfg_t * lcm_cfg = &context->device[devId].lcm_config;

	cfg.mBits.display_bit_depth 		 = lcm_cfg->bit_depth;
//	cfg.mBits.display_uv_order			 = hdmi->hdmi_uv_order;
	cfg.mBits.display_rgb_order 		 = lcm_cfg->rgb_order;
//	cfg.mBits.dsi_cmd_mode				 = ;
//	cfg.mBits.lcm_cmd_mode				 = ;
//	cfg.mBits.bt_1120_mode				 = ;
	if (LCM_TYPE_RGB == device->lcm_type) {
		cfg.mBits.display_dataen_active_low  = lcm_cfg->info.rgb.de_pol;
		cfg.mBits.display_vsync_active_low	 = lcm_cfg->info.rgb.v_sync_pol;
		cfg.mBits.display_hsync_active_low	 = lcm_cfg->info.rgb.h_sync_pol;
	}
//	cfg.mBits.dpi_shutdn_active_low 	 = ;
//	cfg.mBits.dpi_colorm_active_low 	 = ;
	if (LCM_TYPE_MCU == device->lcm_type) {
		cfg.mBits.display_te_en 			 = lcm_cfg->info.mcu.te_eb;
	}
//	cfg.mBits.dpi_video_mode_wait_halt	 = ;
	cfg.mBits.dithering_en				 = lcm_cfg->dithering_en;
	cfg.mBits.bitswap_en				 = lcm_cfg->bitswap_en;
//	cfg.mBits.use_clk_negedge			 = ;
//	cfg.mBits.use_te_negedge			 = ;
//	cfg.mBits.display_rgb_map			 = ;

	writel(cfg.dwValue, REG_VOU_BASE + chn_base[devId] + DHD_DISPLAY_CFG);
}

void set_chn_minor_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x00000000, REG_VOU_BASE + chn_base[devId] + CHN_MINOR_CTRL);

	set_intf_common_para(context, devId);
	set_display_info(context, devId);
}

void set_active_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_sync_info_t *sync_info = &context->device[devId].sync_info;

	writel((sync_info->Vactive << 16) + sync_info->Hactvie, REG_VOU_BASE + chn_base[devId] + ACTIVE_INFO_OFFSET);//writel(0x02580400, 0x26001024);
}

void set_blank_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	vou_sync_info_t *sync_info = &context->device[devId].sync_info;

	writel((sync_info->StartBlen << 16) + sync_info->Hlen, REG_VOU_BASE + chn_base[devId] + HBLANK_INFO_OFFSET);
	writel((sync_info->TBLineOfs << 16) + sync_info->TVLine, REG_VOU_BASE + chn_base[devId] + TF_BLANK_INFO_OFFSET);
	writel((sync_info->BBLineOfs << 16) + sync_info->BVLine, REG_VOU_BASE + chn_base[devId] + BF_BLANK_INFO_OFFSET);
}

void set_y2r_matrix(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x0fa80100, REG_VOU_BASE + chn_base[devId] + CSC_MATRIX_COEFF0_OFFSET);
	writel(0x01000f49, REG_VOU_BASE + chn_base[devId] + CSC_MATRIX_COEFF1_OFFSET);
	writel(0x000001c6, REG_VOU_BASE + chn_base[devId] + CSC_MATRIX_COEFF2_OFFSET);
	writel(0x00000100, REG_VOU_BASE + chn_base[devId] + CSC_MATRIX_COEFF3_OFFSET);
	writel(0x00000167, REG_VOU_BASE + chn_base[devId] + CSC_MATRIX_COEFF4_OFFSET);
	writel(0x34dc7487, REG_VOU_BASE + chn_base[devId] + CSC_MATRIX_OFFSET_OFFSET);
}

void set_bg_info(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x008080ff, REG_VOU_BASE + chn_base[devId] + BG_INFO_YUV_OFFSET);
}

void set_chn_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x00000103, REG_VOU_BASE + chn_base[devId] + CHN_CFG_OFFSET);  //test mode
}

void set_img_layer_cfg(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x80800000, REG_VOU_BASE + chn_base[devId] + IMG_LAYER_CFG_OFFSET);
}

void set_irq_mask(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x00000000, REG_VOU_BASE + chn_base[devId] + CHN_INT_MASK_OFFSET);  //irq mask
}

void set_work_ctrl(vou_cfg_t *context, vou_dev_idx_e devId)
{
	writel(0x00000300, REG_VOU_BASE + chn_base[devId] + CHN_WORK_CTRL_OFFSET);
	writel(0x00000001, REG_VOU_BASE + chn_base[devId] + CHN_WORK_CTRL_OFFSET);
}

void set_image_block(vou_cfg_t *context, vou_dev_idx_e devId, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph)
{
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("LOGO_YUV !!!\n");
	#endif
	writel(0x00000040, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_CTRL_OFFSET);
	writel((ph<<16) + pw, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_SRC_SIZE_OFFSET);
	writel(pp, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_SRC_PITCH_OFFSET);
	writel(y_addr, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_Y_BASE_ADDR_OFFSET);
	writel(uv_addr, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_C_BASE_ADDR_OFFSET);
	writel((y<<16) + x, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_DES_START_OFFSET);
	writel((h<<16) + w, REG_VOU_BASE + chn_base[devId] + IMG0_OFFSET + IMG_BX_DES_SIZE_OFFSET);
	writel(0x80800001, REG_VOU_BASE + chn_base[devId] + IMG_LAYER_CFG_OFFSET);
	writel(0x00000009, REG_VOU_BASE + chn_base[devId] + CHN_CFG_OFFSET);

	writel(0xffffffff, REG_VOU_BASE + chn_base[devId] + UPDATE_CTRL_OFFSET);
}

void set_gui_layer(vou_cfg_t *context, vou_dev_idx_e devId, int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
	if (type == LOGO_RGB_8888) {
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		printf("LOGO_RGB_8888 !!!\n");
		#endif
		writel(0x0000001A, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_CTRL_OFFSET);
	} else if (type == LOGO_RGB_1555) {
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		printf("LOGO_RGB_1555 !!!\n");
		#endif
		writel(0x00000010, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_CTRL_OFFSET);  //mc argb1555
	}
	writel(0x00008000, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_ALPHA_PARA_OFFSET);
	writel(0x00000000, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_SRC_START_OFFSET);
	writel((ph<<16) + pw, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_SRC_SIZE_OFFSET);
	writel(pp, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_SRC_PITCH_OFFSET);
	writel(addr, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_BASE_ADDR_OFFSET);
	writel((y<<16) + x, REG_VOU_BASE + chn_base[devId] + OSD_BASE_OFFSET + GUI_DES_START_OFFSET);
	writel(0x00000011, REG_VOU_BASE + chn_base[devId] + CHN_CFG_OFFSET);

	writel(0xffffffff, REG_VOU_BASE + chn_base[devId] + UPDATE_CTRL_OFFSET);
}

void lcm_config(vou_cfg_t *context, vou_dev_idx_e devId)
{
	union 	_vou_LCM_CFG_tag lcm_cfg = {};
	vou_device_t * device = &context->device[devId];
	lcm_display_cfg_t * lcm_dp_cfg = &context->device[devId].lcm_config;

	if (LCM_TYPE_MCU == device->lcm_type) {
		lcm_cfg.mBits.LCM_SYS_IF_TYPE = lcm_dp_cfg->if_type;
		lcm_cfg.mBits.LCM_MCU_HIGHBYTE_FIRST = lcm_dp_cfg->info.mcu.trans_mode;
	} else if (LCM_TYPE_RGB == device->lcm_type) {
		lcm_cfg.mBits.LCM_RGB_IF_TYPE = lcm_dp_cfg->if_type;
	}

	writel(lcm_cfg.dwValue, REG_VOU_BASE + chn_base[devId] + DHD_LCM_CFG);
}

void vou_dev_kickoff(vou_cfg_t *context, vou_dev_idx_e devId)
{
#ifndef CONFIG_UBOOT_WIFIX
	lcm_init_in_t lcm_init_in;
	lcm_init_out_t lcm_init_out;

	lcm_init_in.lcm_id = devId;
#endif
	set_global();
	set_display_format(context, devId);
	set_tve_sync_info(context, devId);

	switch (context->device[devId].lcm_type) {
		case _LCM_TYPE_MCU:
//			lcm_config(context, devId);
//			writel(0x00000101, REG_VOU_BASE + chn_base[devId] + 0xE0);
			break;
		case _LCM_TYPE_RGB:
			set_chn_minor_cfg(context, devId);
			lcm_config(context, devId);
			writel(0x00000201, REG_VOU_BASE + chn_base[devId] + 0xE0);
			break;
		case _LCM_TYPE_MIPI:
			set_chn_minor_cfg(context, devId);
			break;
		default:
			break;
	}

#ifndef CONFIG_UBOOT_WIFIX
	lcm_func_init(&lcm_init_in, &lcm_init_out);
#endif
	set_active_info(context, devId);
	set_blank_info(context, devId);
	set_y2r_matrix(context, devId);
	set_bg_info(context, devId);
	set_chn_cfg(context, devId);
	set_img_layer_cfg(context, devId);
	set_irq_mask(context, devId);

	set_work_ctrl(context, devId);
}

