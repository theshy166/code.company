#ifndef __MC_VOU_DRV_H__
#define __MC_VOU_DRV_H__

//#include "mc_lcm_drv.h"

#define DHD0_CHN_BASE				0x00001000
#define DHD1_CHN_BASE				0x00002000

#define ARQOS_LOW_OFFSET			0x20
#define ARQOS_HIGH_OFFSET			0x24
#define ARQOS_THR_LOW_OFFSET			0x28
#define ARQOS_THR_HIGH_OFFSET			0x2c
#define DHD_DISPLAY_FORMAT			0x40

#define CHN_CFG_OFFSET       			0x00
#define IMG_LAYER_CFG_OFFSET 			0x04
#define UPDATE_CTRL_OFFSET   			0x08
#define CHN_WORK_CTRL_OFFSET     		0x0C
#define CHN_INT_MASK_OFFSET  			0x14

#define BG_INFO_YUV_OFFSET   			0x20
#define ACTIVE_INFO_OFFSET   			0x24
#define HBLANK_INFO_OFFSET   			0x28
#define TF_BLANK_INFO_OFFSET 			0x2C
#define BF_BLANK_INFO_OFFSET 			0x30
#define CHN_FRONT_PORCH				0x74
#define CHN_SYNC_PULSE				0x78
#define CHN_BACK_PORCH				0x7C
#define CHN_MINOR_CTRL				0x8C

#define DHD_DISPLAY_CFG				0xB4
#define DHD_LCM_CFG					0xBC

#define CSC_MATRIX_COEFF0_OFFSET		0x600
#define CSC_MATRIX_COEFF1_OFFSET		0x604
#define CSC_MATRIX_COEFF2_OFFSET		0x608
#define CSC_MATRIX_COEFF3_OFFSET		0x60c
#define CSC_MATRIX_COEFF4_OFFSET		0x610
#define CSC_MATRIX_OFFSET_OFFSET		0x614

#define IMG0_OFFSET				0x200

/*offset based on IMG0_OFFSET or IMG1_OFFSET*/
#define IMG_BX_CTRL_OFFSET            		0x00
#define IMG_BX_SRC_START_OFFSET       		0x04
#define IMG_BX_SRC_SIZE_OFFSET        		0x08
#define IMG_BX_SRC_PITCH_OFFSET       		0x0C
#define IMG_BX_Y_BASE_ADDR_OFFSET     		0x10
#define IMG_BX_C_BASE_ADDR_OFFSET    		0x14
#define IMG_BX_DES_START_OFFSET       		0x18
#define IMG_BX_DES_SIZE_OFFSET        		0x1C
#define IMG_BX_CM_PARA0_OFFSET        		0x20
#define IMG_BX_CM_PARA1_OFFSET        		0x24
#define IMG_BX_CM_PARA2_OFFSET        		0x28
#define IMG_BX_CM_PARA3_OFFSET        		0x2C
#define IMG_BX_Y_OT_ADDR_OFFSET        		0x30
#define IMG_BX_C_OT_ADDR_OFFSET        		0x34
#define IMG_BX_CTRL_OFFSET_STEP        		0x40

#define OSD_BASE_OFFSET              		0x120
#define GUI_CTRL_OFFSET              		0x000
#define GUI_ALPHA_PARA_OFFSET        		0x004
#define GUI_SRC_START_OFFSET         		0x008
#define GUI_SRC_SIZE_OFFSET          		0x00C
#define GUI_SRC_PITCH_OFFSET        		0x010
#define GUI_BASE_ADDR_OFFSET         		0x014
#define GUI_DES_START_OFFSET         		0x018

void vou_dev_kickoff(vou_cfg_t *context, vou_dev_idx_e devId);
void set_image_block(vou_cfg_t *context, vou_dev_idx_e devId, int x, int y, int w, int h, uint32_t y_addr, uint32_t uv_addr, int pw, int pp, int ph);
void set_gui_layer(vou_cfg_t *context, vou_dev_idx_e devId, int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph);

#endif
