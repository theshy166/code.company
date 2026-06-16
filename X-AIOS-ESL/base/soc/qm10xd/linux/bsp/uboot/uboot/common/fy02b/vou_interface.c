#include "vou_osal.h"
#include "vou_proc.h"
#include "vou_register_define.h"
#include "vou_register_write.h"
#include "vou_register_list.h"
#include "vou_interface.h"

#include "vou_cfg_defaults.c"

void _vou_clk_on(vou_cfg_t *context)
{
	vioapb_write_register_setbit(&context->regbase, APB_BASE, OFFSET_CKG_CFG, 0x3fff);  // vou clock: 24M/100M/330M/[400M]
	vioapb_write_register_clrbit(&context->regbase, APB_BASE, OFFSET_MISC_REG0, 0x400); // cpu access
	topclk_write_register_setbit(&context->regbase, 0, OFFSET_CKG_GLB_VIO_CFG, 0x3); // vio sys clock: ddr read rate ( share with viu ) 24M/100M/200M/[333.3M]
}

void _vou_soft_reset(vou_cfg_t *context, vou_dev_idx_e devId, bool device, bool interface)
{
	union _soft_rst_tag rst = {};

	if ( devId == VOU_DEV_IDX_DHD ) {
		if ( device ) {
			rst.mBits.vou_dhd0_soft_rst = 1;
		}
		if ( interface ) {
			rst.mBits.hdmi_soft_rst = 1;
			rst.mBits.vou_mif0_soft_rst = 1;
			if (context->glbcfg.vga_if_eb == 1 && context->glbcfg.vga_src_sel == 0){
				rst.mBits.vou_vga_soft_rst = 1;
			}
		}
	}
	else if ( devId == VOU_DEV_IDX_DHD1 ) {
		if ( device ) {
			rst.mBits.vou_dhd1_soft_rst = 1;
		}
		if ( interface ) {
			rst.mBits.hdmi_soft_rst = 1;
			rst.mBits.vou_mif1_soft_rst = 1;
			if (context->glbcfg.vga_if_eb == 1 && context->glbcfg.vga_src_sel == 1){
				rst.mBits.vou_vga_soft_rst = 1;
			}
		}
 	}
 	else if ( devId == VOU_DEV_IDX_DSD ) {
		if ( device ) {
			rst.mBits.vou_dsd_soft_rst = 1;
		}
		if ( interface ) {
			rst.mBits.vou_cvbs_soft_rst = 1;
			rst.mBits.vou_sif_soft_rst = 1;
		}
	}
	else {
			rst.mBits.vou_dhd0_soft_rst = 1;
			rst.mBits.vou_dhd1_soft_rst = 1;
			rst.mBits.hdmi_soft_rst = 1;
			rst.mBits.vou_v1_soft_rst = 1;
			rst.mBits.vou_mif0_soft_rst = 1;
			rst.mBits.vou_mif1_soft_rst = 1;
			rst.mBits.vou_dsd_soft_rst = 1;
			rst.mBits.vou_cvbs_soft_rst = 1;
			rst.mBits.vou_vga_soft_rst = 1;
			rst.mBits.ave_soft_rst = 1;
			rst.mBits.vou_sif_soft_rst = 1;
			rst.mBits.vou_soft_rst = 1;
	}

	vioapb_write_register_setbit(&context->regbase, 0x0, OFFSET_SOFT_RST_CFG, rst.dwValue);
	vioapb_write_register_clrbit(&context->regbase, 0x0, OFFSET_SOFT_RST_CFG, rst.dwValue);

}

vou_error_type_e vou_init_context(vou_cfg_t *context)
{
	uint32_t devId = 0;
	MEMCPY_STRUCT(context->glbcfg, _glb);
	MEMCPY_STRUCT(context->cm_default_cfg, _cm_default);

	ZERO_FILL_STRUCT(context->devcfg);
	ZERO_FILL_STRUCT(context->device);

	for(devId=0;devId<2;devId++) {
		context->device[devId].sync_info = &_vou_vga_hdmi_sync;
		MEMCPY_STRUCT(context->devcfg[devId].dev_info, _a_dev_info);
		MEMCPY_STRUCT(context->devcfg[devId].intf_info.clamp_info, _clamp_info);
		MEMCPY_STRUCT(context->devcfg[devId].intf_info.clip_info, _clip_info);
		context->devcfg[devId].intf_info.tve_fir_bypass = 1;
		MEMCPY_STRUCT(context->devcfg[devId].intf_info.hdmi_info, _hdmi_info);
		MEMCPY_STRUCT(context->devcfg[devId].intf_info.y2r_matrix, _y2r_matrix);
		context->devcfg[devId].irq_mask = 0;//clear irq mask
	}

	MEMCPY_STRUCT(context->devcfg[VOU_DEV_IDX_DSD].dev_info, _a_dev_info);
	MEMCPY_STRUCT(context->devcfg[VOU_DEV_IDX_DSD].intf_info.clip_info, _dsd_clip_info);
	MEMCPY_STRUCT(context->devcfg[VOU_DEV_IDX_DSD].intf_info.y2r_matrix, _y2r_matrix);
	context->devcfg[VOU_DEV_IDX_DSD].irq_mask = 0;//clear irq mask

	return VOU_ERR_NONE;
}

void set_fill_cvbs_pal_sync(vou_cfg_t *context)
{
	context->devcfg[VOU_DEV_IDX_DSD].intf_info.cvbs_ntsc_mode = 0;
	context->device[VOU_DEV_IDX_DSD].sync_info = &_vou_cvbs_pal_sync;
}

void set_fill_cvbs_ntsc_sync(vou_cfg_t *context)
{
	context->devcfg[VOU_DEV_IDX_DSD].intf_info.cvbs_ntsc_mode = 2;
	context->device[VOU_DEV_IDX_DSD].sync_info = &_vou_cvbs_ntsc_sync;
}

