#ifndef __MC_VOU_OSAL_H__
#define __MC_VOU_OSAL_H__
#include <common.h>
#include "lcm/lcm_hal.h"
#include "lcm_module/include/mol_lcm_local_def.h"

#define REG_DMC_SYS_APB_REG_BASE                0x24000000
#define REG_VOU_BASE                            0x26000000
#define REG_DSI_BASE                            0x25c00000
#define REG_DPHY_BASE                           0x25d00000

#define LOGO_YUV				(1)
#define LOGO_RGB_1555				(2)
#define LOGO_RGB_8888				(3)
#define ALIGN_UP(x, a)              ((x+a-1)&(~(a-1)))

typedef enum {
	VOU_DEV_IDX_DHD = 0,
	VOU_DEV_IDX_DHD1,
	VOU_DEV_IDX_DSD,
	VOU_DEV_IDX_NUM,
} vou_dev_idx_e;
#if 0
typedef struct {
	uint32_t Hactvie;
	uint32_t Vactive;
	uint32_t FrameMode;
	uint32_t FrameRate;
	uint32_t PixelRate;
	uint32_t Htotal;
	uint32_t Vtotal;
	uint32_t Hblank;
	uint32_t Vblank;
	uint32_t Hfront;
	uint32_t Hsync;
	uint32_t Hback;
	uint32_t Hpol;
	uint32_t Vfront;
	uint32_t Vsync;
	uint32_t Vback;
	uint32_t Vpol;
	uint32_t Alen;
	uint32_t Aline;
	uint32_t Hlen;
	uint32_t StartBlen;
	uint32_t EndBlen;
	uint32_t TVLine;
	uint32_t TBLineOfs;
	uint32_t TEndBLine;
	uint32_t BVLine;
	uint32_t BBLineOfs;
	uint32_t BEndBLine;
	uint32_t HSyncLength;
	uint32_t VSyncLength0;
} vou_sync_info_t;
#endif

typedef struct {
	uint16_t width;
	uint16_t height;
	uint32_t frame_rate;
	vou_sync_info_t sync_info;
	lcm_type_e lcm_type;
	lcm_display_cfg_t lcm_config;
} vou_device_t;

typedef struct {
	vou_device_t device[VOU_DEV_IDX_NUM];
} vou_cfg_t;

union _dhd_display_format_tag {
	struct _dhd_display_format_map {
		volatile unsigned int dhd0_display_format		:3; // DHD0输出模式: 000/001: DSI mode, 011: BT656/1120 MODE, 100: LCM mode , 101: EXTSYNC mode, Others: Reserved
		volatile unsigned int reserved0 				:1;
		volatile unsigned int dhd1_display_format		:3; // DHD1输出模式: 000/001: DSI mode, 011: BT656/1120 MODE, 100: LCM mode , 101: EXTSYNC mode, Others: Reserved
		volatile unsigned int reserved1 				:25;
	}mBits ;
	volatile unsigned int dwValue ;
};

union _dhd_display_cfg_tag {
	struct _dhd_display_cfg_map {
		volatile unsigned int display_bit_depth         	:2; // For DSI/LCM: 00: 24bits 888, 01: 18bits 666, 10: 16bits 565
		volatile unsigned int display_uv_order          	:1; // for AVE/BT656/1120: 0: {VU}, 1: {UV}
		volatile unsigned int display_rgb_order         	:3; //for DSI/LCM: 0: {BGR}, 1: {GBR}, 2: {RBG}, 3: {BRG}, 4: {GRB}, 5: {RGB}
		volatile unsigned int dsi_cmd_mode              	:1; //0: VIDEO_mode, 1: CMD mode
		volatile unsigned int lcm_mcu_mode              	:1; //0: RGB_mode, 1: MCU mode
		volatile unsigned int bt_1120_mode              	:1; //0: BT656, 1: BT1120
		volatile unsigned int display_dataen_active_low     	:1; //For DSI/LCM, 数据使能极性: 0：数据有效为高电平, 1：数据有效为低电平
		volatile unsigned int display_vsync_active_low      	:1; //For DSI/LCM,水平同步极性: 0：消隐为高电平, 1：消隐为低电平
		volatile unsigned int display_hsync_active_low      	:1; // For DSI/LCM,垂直同步极性: 0：消隐为高电平, 1：消隐为低电平
		volatile unsigned int dpi_shutdn_active_low     	:1; // For DSI,关闭信号极性: 0：高有效, 1：低有效
		volatile unsigned int dpi_colorm_active_low     	:1; // For DSI,COLORM极性: 0：高有效, 1：低有效
		volatile unsigned int display_te_en                 	:1; // For DSI, Tearing Effect使能: 0：关闭, 1：使能
		volatile unsigned int dpi_video_mode_wait_halt  	:1; // For DSI, DPI Video模式下，每帧内数据发送是否要受HALT信号影响
		volatile unsigned int dithering_en              	:1; // 打开Dithering
		volatile unsigned int bitswap_en                	:1; // RGB内部bit位置Swap
		volatile unsigned int use_clk_negedge           	:1; // 0:在时钟上升沿输出 1:在时钟下降沿输出
		volatile unsigned int use_te_negedge            	:1; // 0:在TE上升沿后开始输出下一帧 1:在TE下降沿后开始输出下一帧
		volatile unsigned int bt_ext_yuv_mode           	:2; //0: yuv422 8bit  1:yuv422 16bit 2:yuv444 24bit
		volatile unsigned int skip_step0                 	:1;	//for mcu interface display
		volatile unsigned int reserved0           			:1;
		volatile unsigned int display_rgb2yuv_matrix_sel    	:2; // for display/BT656/1120: 00:BT601, 01:BT709, 10:BT2020, 11:reserved
		volatile unsigned int display_rgb2yuv_clip_eb       	:1; // for display/BT656/1120: 0: output is full range, 1：clip to Narrow range 
		volatile unsigned int display_rgb2yuv_clip_mode     	:1; // for display/BT656/1120: 0: 直接clip, 1：进行拉伸操作
		volatile unsigned int display_rgb_map           	:4; // For DSI/LCM 16bits and 18bits 0000: 有效数据向低位集中 0001: 有效数据分别向8bit低位集中 0010: 有效数据分别向8bit高位集中 0011: 在0001基础上bit数较少分量空出低位以补足位宽到最大位宽。 见说明。
	}mBits ;
	volatile unsigned int dwValue ;
};

union _vou_LCM_CFG_tag{
	struct _vou_LCM_CFG_map{
		volatile unsigned int Reserved0					:4;
		volatile unsigned int LCM_SYS_IF_TYPE			:2;
		volatile unsigned int LCM_RGB_IF_TYPE			:2;
		volatile unsigned int LCM_MCU_TIMING_TYPE		:1;
		volatile unsigned int LCM_SW_USE_TE				:1;
		volatile unsigned int LCM_MCU_HIGHBYTE_FIRST	:1;
		volatile unsigned int LCM_MCU_3_TRANS			:1;
		volatile unsigned int Reserved1					:2;
		volatile unsigned int LCM_WAIT_FIFO				:1;
		volatile unsigned int Reserved2					:17;
	}mBits;
	volatile unsigned int dwValue;
};

uint32_t readl(uint32_t addr);
void writel(uint32_t value, uint32_t addr);
void writels(uint32_t value, uint32_t addr);
void writelc(uint32_t value, uint32_t addr);
void writelr(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit);
#endif
