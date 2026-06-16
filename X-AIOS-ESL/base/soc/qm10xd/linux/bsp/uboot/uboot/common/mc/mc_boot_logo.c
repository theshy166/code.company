#include <common.h>
#include "mc_vou_osal.h"
//#include "mc_lcm_drv.h"
#include "mc_vou_drv.h"
#include "mc_jpegd_drv.h"
#include "lcm/lcm_hal.h"
#include "lcm/mc_lcm.h"


//#define YUV_LOGO
//#define MIPI_LCD
#define BOOT_DEBUG_LOG 0

extern void logo_display(void);
extern void logo_color(void);
extern void lcm_clk_on(void);
// extern unsigned char _binary_logo_jpg_start[];
// extern unsigned char _binary_logo_jpg_end[];
#define JPG_W		1024 //dhd0 only support 1024x600
#define JPG_H		600
#define Y_ADDR		(LOGO_YADDR0) //(0x43D00000) //MMZ END ADDR 0x44000000
#define UV_ADDR		(LOGO_CADDR0) //(0x43D00000 + (ALIGN_UP(JPG_W, 16) * ALIGN_UP(JPG_H, 16))) //JPEG DEC MUST ALIGN 16

vou_cfg_t context;

static int sync_info_gen(vou_sync_info_t *vou_sync_info, dp_sync_info_t *dp_sync)
{
    int Hactive = dp_sync->hor_active_pixel;
    int Vactive = dp_sync->ver_active_line;
    int FrameRate = vou_sync_info->FrameRate;
    int Hfront = dp_sync->hor_front_porch;
    int Hsync = dp_sync->hor_sync_active;
    int Hback = dp_sync->hor_back_porch;
    int Vfront = dp_sync->ver_front_porch;
    int Vsync = dp_sync->ver_sync_active;
    int Vback = dp_sync->ver_back_porch;

    vou_sync_info->Hactvie = Hactive;
    vou_sync_info->Vactive = Vactive;

    vou_sync_info->Hfront = Hfront;
    vou_sync_info->Hsync = Hsync;
    vou_sync_info->Hback = Hback;

    vou_sync_info->Vfront = Vfront;
    vou_sync_info->Vsync = Vsync;
    vou_sync_info->Vback = Vback;

    vou_sync_info->Hblank = Hfront + Hsync + Hback;
    vou_sync_info->Vblank = Vfront + Vsync + Vback;
    vou_sync_info->Htotal = Hactive + vou_sync_info->Hblank;
    vou_sync_info->Vtotal = Vactive + vou_sync_info->Vblank;
    vou_sync_info->PixelRate = FrameRate * vou_sync_info->Htotal * vou_sync_info->Vtotal;
    vou_sync_info->Alen = Hactive;
    vou_sync_info->Aline = Vactive;
    vou_sync_info->Hlen = vou_sync_info->Htotal - 1;
    vou_sync_info->StartBlen = Hsync + Hback - 1;
    vou_sync_info->EndBlen = Hfront;
    vou_sync_info->TVLine = vou_sync_info->Vtotal - 1;
    vou_sync_info->TBLineOfs = Vsync + Vback - 1;
    vou_sync_info->TEndBLine = Vfront;
    vou_sync_info->BVLine = 0;
    vou_sync_info->BBLineOfs = 0;
    vou_sync_info->BEndBLine = 0;
    vou_sync_info->HSyncLength = Hsync -1;
    vou_sync_info->VSyncLength0 = Vsync - 1;

    return 0;
}

/*
 * int x         : logo display position x
 * int y         : logo display position x
 * int w         : logo display width
 * int h         : logo display height
 * uint32_t addr : logo source address
 * int pw        : logo source picture width
 * int pp        : logo source picture pitch
 * int ph        : logo source picture height
 *
 * for example:
 * 1. full screen osd:
 * fast_osd(0, 0, 1024, 600, 0x40000000, 1024, 1024, 600);
 * 2. full screen osd with smaller picture(yuv only):
 * fast_osd(0, 0, 256, 150, 0x40000000, 1024, 1024, 600);
 * 3. topleft 1/16 display area osd with same-sized picture
 * fast_osd(0, 0, 256, 150, 0x40000000, 256, 150, 256);
 */

void fast_osd(int type, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph)
{
	uint32_t rtn = 0;
	vou_dev_idx_e devId = VOU_DEV_IDX_DHD;
	lcm_probe_in_t lcm_probe_in;
	lcm_probe_out_t lcm_probe_out;
	lcm_probe_info_t* probe; 

	mc_lcm_probe();

	rtn = lcm_func_probe(&lcm_probe_in, &lcm_probe_out);
	if (!rtn) {

		for (devId = VOU_DEV_IDX_DHD; devId < VOU_DEV_IDX_DSD; devId++) {
			probe = lcm_probe_out.prob_info[devId];

			if (NULL == probe) {
				continue;
			}

			context.device[devId].lcm_type = probe->type;
			context.device[devId].width = probe->output.width;
			context.device[devId].height = probe->output.height;
			context.device[devId].frame_rate = probe->output.frame_rate;

			#if BOOT_DEBUG_LOG
			printf("devId                       : %d\n", devId);
			printf("probe->output.width         : %d\n", probe->output.width);
			printf("probe->output.height        : %d\n", probe->output.height);
			printf("probe->output.frame_rate    : %d\n", probe->output.frame_rate);
			printf("probe->lcd_type             : %d\n", probe->type);
			printf("probe->lcm_config.bit_depth : %d\n", probe->lcm_config.bit_depth);
			printf("probe->lcm_config.if_type   : %d\n", probe->lcm_config.if_type);
			printf("probe->lcm_config.rgb_order : %d\n", probe->lcm_config.rgb_order);
			printf("probe->lcm_config.bitswap_en: %d\n", probe->lcm_config.bitswap_en);
			#endif

			if (_LCM_TYPE_RGB == probe->type) {
				context.device[devId].lcm_config.info.rgb.h_sync_pol = probe->lcm_config.info.rgb.h_sync_pol;
				context.device[devId].lcm_config.info.rgb.v_sync_pol = probe->lcm_config.info.rgb.v_sync_pol;
				context.device[devId].lcm_config.info.rgb.de_pol = probe->lcm_config.info.rgb.de_pol;
			} else if (_LCM_TYPE_MCU == probe->type) {
				context.device[devId].lcm_config.info.mcu.trans_mode = probe->lcm_config.info.mcu.trans_mode;
				context.device[devId].lcm_config.info.mcu.te_eb = probe->lcm_config.info.mcu.te_eb;
				context.device[devId].lcm_config.info.mcu.te_mode = probe->lcm_config.info.mcu.te_mode;
				context.device[devId].lcm_config.info.mcu.te_edge = probe->lcm_config.info.mcu.te_edge;
			}

			context.device[devId].lcm_config.bit_depth = probe->lcm_config.bit_depth;
			context.device[devId].lcm_config.if_type = probe->lcm_config.if_type;
			context.device[devId].lcm_config.rgb_order = probe->lcm_config.rgb_order;
			context.device[devId].lcm_config.dithering_en = probe->lcm_config.dithering_en;
			context.device[devId].lcm_config.bitswap_en = probe->lcm_config.bitswap_en;

			sync_info_gen(&context.device[devId].sync_info, &probe->dp_sync);
			vou_dev_kickoff(&context, devId);

			if (type == LOGO_YUV) {
				set_image_block(&context, devId, x, y, w, h, y_addr, uv_addr, pw, pp, ph);/* only dhd support 1024x600 */
				//set_image_block(&context, VOU_DEV_IDX_DHD1, x, y, rgb_sync_info_t.Hactvie, rgb_sync_info_t.Vactive, y_addr, uv_addr, pw, pp, ph);
			} else {
				//set_gui_layer(&context, VOU_DEV_IDX_DHD, LOGO_RGB_1555, x, y, w, h, addr, pw, pp, ph);
			}
		}
	}
}

static bool DHD_LCM_PROBED = false;
static bool DHD1_LCM_PROBED = false;

void fast_osd_by_id(int id, int type, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph)
{
	if ((id == VOU_DEV_IDX_DHD && DHD_LCM_PROBED) || (id == VOU_DEV_IDX_DHD1 && DHD1_LCM_PROBED)) {
		printf("DHD_IDX=%d already probed!\n", id);
		return;
	}

	uint32_t rtn = 0;
	vou_dev_idx_e devId = id;
	lcm_probe_in_t lcm_probe_in;
	lcm_probe_out_t lcm_probe_out;
	lcm_probe_info_t* probe; 

	mc_lcm_probe();

	rtn = lcm_func_probe(&lcm_probe_in, &lcm_probe_out);
	if (!rtn) {

		probe = lcm_probe_out.prob_info[devId];

		if (NULL == probe) {
			return;	
		}

		context.device[devId].lcm_type = probe->type;
		context.device[devId].width = probe->output.width;
		context.device[devId].height = probe->output.height;
		context.device[devId].frame_rate = probe->output.frame_rate;

		#if BOOT_DEBUG_LOG
		printf("devId                       : %d\n", devId);
		printf("probe->output.width         : %d\n", probe->output.width);
		printf("probe->output.height        : %d\n", probe->output.height);
		printf("probe->output.frame_rate    : %d\n", probe->output.frame_rate);
		printf("probe->lcd_type             : %d\n", probe->type);
		printf("probe->lcm_config.bit_depth : %d\n", probe->lcm_config.bit_depth);
		printf("probe->lcm_config.if_type   : %d\n", probe->lcm_config.if_type);
		printf("probe->lcm_config.rgb_order : %d\n", probe->lcm_config.rgb_order);
		printf("probe->lcm_config.bitswap_en: %d\n", probe->lcm_config.bitswap_en);
		#endif

		if (_LCM_TYPE_RGB == probe->type) {
			context.device[devId].lcm_config.info.rgb.h_sync_pol = probe->lcm_config.info.rgb.h_sync_pol;
			context.device[devId].lcm_config.info.rgb.v_sync_pol = probe->lcm_config.info.rgb.v_sync_pol;
			context.device[devId].lcm_config.info.rgb.de_pol = probe->lcm_config.info.rgb.de_pol;
		} else if (_LCM_TYPE_MCU == probe->type) {
			context.device[devId].lcm_config.info.mcu.trans_mode = probe->lcm_config.info.mcu.trans_mode;
			context.device[devId].lcm_config.info.mcu.te_eb = probe->lcm_config.info.mcu.te_eb;
			context.device[devId].lcm_config.info.mcu.te_mode = probe->lcm_config.info.mcu.te_mode;
			context.device[devId].lcm_config.info.mcu.te_edge = probe->lcm_config.info.mcu.te_edge;
		}

		context.device[devId].lcm_config.bit_depth = probe->lcm_config.bit_depth;
		context.device[devId].lcm_config.if_type = probe->lcm_config.if_type;
		context.device[devId].lcm_config.rgb_order = probe->lcm_config.rgb_order;
		context.device[devId].lcm_config.dithering_en = probe->lcm_config.dithering_en;
		context.device[devId].lcm_config.bitswap_en = probe->lcm_config.bitswap_en;

		sync_info_gen(&context.device[devId].sync_info, &probe->dp_sync);
		vou_dev_kickoff(&context, devId);

		if (type == LOGO_YUV) {
			set_image_block(&context, devId, x, y, w, h, y_addr, uv_addr, pw, pp, ph);/* only dhd support 1024x600 */
			//set_image_block(&context, VOU_DEV_IDX_DHD1, x, y, rgb_sync_info_t.Hactvie, rgb_sync_info_t.Vactive, y_addr, uv_addr, pw, pp, ph);
		} else {
			//set_gui_layer(&context, VOU_DEV_IDX_DHD, LOGO_RGB_1555, x, y, w, h, addr, pw, pp, ph);
		}

		if (id == VOU_DEV_IDX_DHD) {
			DHD_LCM_PROBED = true;
		} else if (id == VOU_DEV_IDX_DHD1) {
			DHD1_LCM_PROBED = true;
		}
	}
}

void boot_logo(void)
{
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("start boot_logo!\n");
	#endif

#ifdef CONFIG_QUA_LOGO
	extern void qua_boot_logo(void);
	qua_boot_logo();
	return;
#endif

	// unsigned int jpg_size = _binary_logo_jpg_end - _binary_logo_jpg_start;
	// hw_jpeg_dec((unsigned int)_binary_logo_jpg_start, jpg_size, Y_ADDR, UV_ADDR);

	// fast_osd(1, 0, 0, JPG_W, JPG_H, Y_ADDR, UV_ADDR, JPG_W, JPG_W, JPG_H);

	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("end boot_logo!\n");
	#endif
}

int show_boot_logo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	boot_logo();

	return 0;
}

U_BOOT_CMD(
        logo, 1, 1, show_boot_logo,
        "show boot logo",
        "show boot logo"
);

