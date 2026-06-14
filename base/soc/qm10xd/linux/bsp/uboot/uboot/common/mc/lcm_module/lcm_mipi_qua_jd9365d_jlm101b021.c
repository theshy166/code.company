#include "include/mol_lcm_local.h"
#include "include/dsi_com_param.h"
#include "include/mol_lcm.h"

#define LCM_JD9365D_WIDTH 800
#define LCM_JD9365D_HEIGHT 1280
#define LCM_JD9365D_DEBUG 0

const lcm_cfg_t qua_lcm_mipi_jd9365d_jlm101b021_info;

static dsi_setting_tab_t qua_lcm_mipi_jd9365d_jlm101b021_init_setting[] = {

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x01, 0x00}},
	{DSI_DI_DELAY,             30, {0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE1, 0x93}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE2, 0x65}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE3, 0xF8}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x80, 0x01}},	//{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x80, 0x01}},	

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x00, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x01, 0x48}}, // 0x3B,0X41~0X48

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0C, 0x74}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x17, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x18, 0xAF}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x19, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1A, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1B, 0xAF}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1C, 0x00}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x35, 0x26}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x37, 0x09}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x38, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x39, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3A, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3C, 0x78}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3D, 0xFF}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3E, 0xFF}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3F, 0x7F}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x40, 0x06}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x41, 0xA0}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x42, 0x81}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x43, 0x14}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x44, 0x23}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x45, 0x28}},

	//{DSI_DI_GENERIC_WRITE_1_PARAM, 2,{0x4A,0x35}},  //bist mode
	//{DSI_DI_GENERIC_WRITE_1_PARAM, 2,{0x4B,0x04}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x55, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x57, 0x69}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x59, 0x0A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5A, 0x2A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5B, 0x17}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5D, 0x7F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5E, 0x69}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5F, 0x59}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x60, 0x4C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x61, 0x47}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x62, 0x39}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x63, 0x3B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x64, 0x24}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x65, 0x3D}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x66, 0x3C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x67, 0x3C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x68, 0x5A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x69, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6A, 0x4A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6B, 0x43}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6C, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6D, 0x20}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6E, 0x0F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6F, 0x00}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x70, 0x7F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x71, 0x69}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x72, 0x59}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x73, 0x4C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x74, 0x47}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x75, 0x39}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x76, 0x3B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x77, 0x24}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x78, 0x3D}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x79, 0x3C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7A, 0x3C}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7B, 0x5A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7C, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7D, 0x4A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7E, 0x43}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7F, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x80, 0x20}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x81, 0x0F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x82, 0x00}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x00, 0x42}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x01, 0x42}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x02, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x03, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x04, 0x5E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x05, 0x5E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x06, 0x5F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x07, 0x5F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x08, 0x5F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x09, 0x57}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0A, 0x57}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0B, 0x77}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0C, 0x77}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0D, 0x47}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0E, 0x47}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0F, 0x45}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x10, 0x45}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x11, 0x4B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x12, 0x4B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x13, 0x49}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x14, 0x49}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x15, 0x5F}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x16, 0x41}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x17, 0x41}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x18, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x19, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1A, 0x5E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1B, 0x5E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1C, 0x5F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1D, 0x5F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1E, 0x5F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x1F, 0x57}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x20, 0x57}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x21, 0x77}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x22, 0x77}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x23, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x24, 0x46}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x25, 0x44}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x26, 0x44}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x27, 0x4A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x28, 0x4A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x29, 0x48}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x2A, 0x48}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x2B, 0x5F}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x2C, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x2D, 0x01}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x2E, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x2F, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x30, 0x1F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x31, 0x1F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x32, 0x1E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x33, 0x1E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x34, 0x1F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x35, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x36, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x37, 0x37}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x38, 0x37}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x39, 0x08}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3A, 0x08}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3B, 0x0A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3C, 0x0A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3D, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3E, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x3F, 0x06}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x40, 0x06}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x41, 0x1F}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x42, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x43, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x44, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x45, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x46, 0x1F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x47, 0x1F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x48, 0x1E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x49, 0x1E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4A, 0x1F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4B, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4C, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4D, 0x37}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4E, 0x37}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4F, 0x09}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x50, 0x09}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x51, 0x0B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x52, 0x0B}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x53, 0x05}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x54, 0x05}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x55, 0x07}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x56, 0x07}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x57, 0x1F}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x58, 0x40}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5B, 0x30}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5C, 0x16}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5D, 0x34}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5E, 0x05}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x5F, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x63, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x64, 0x6A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x67, 0x73}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x68, 0x1D}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x69, 0x08}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6A, 0x6A}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6B, 0x08}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6C, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6D, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6E, 0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x6F, 0x88}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x75, 0xFF}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x77, 0xDD}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x78, 0x3F}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x79, 0x15}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7A, 0x17}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7D, 0x14}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x7E, 0x82}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x04}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x00, 0x0E}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x02, 0xB3}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x09, 0x61}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x0E, 0x48}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x36, 0x49}},	//mark
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x37, 0x58}},	//mark

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x00}},

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE6, 0x02}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE7, 0x0C}},

#ifdef CONFIG_QUA_LOGO_MIPI_JD9365D_FLIP_H_V
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x36, 0x03}},
#endif
	{DSI_DI_GENERIC_WRITE_0_PARAM, 1, {0x11}},
	{DSI_DI_DELAY,                240, {0x00}},

	/* bist mode */
	//{DSI_DI_DELAY,                254, {0x00}},
	//{DSI_DI_DELAY,                254, {0x00}},
	//{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x01}},
	//{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x4A, 0xB0}},	// bist

	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x00}},
	{DSI_DI_GENERIC_WRITE_0_PARAM, 1, {0x29}},
	{DSI_DI_DELAY,                5, {0x00}},
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0x35, 0x00}},
	{DSI_DI_END_OF_TABLE,         0x00, {0x00}}
};

#if 0
static dsi_setting_tab_t lcm_mipi_jd9365d_sleep_out_setting[] = {
    // sleep out
    {DSI_DI_GENERIC_WRITE_0_PARAM,    0x01, {0x11}},
	{DSI_DI_DELAY,                800, {0x00}},
	{DSI_DI_GENERIC_WRITE_0_PARAM,    0x01, {0x29}},
	{DSI_DI_DELAY,                200, {0x00}},
    {DSI_DI_END_OF_TABLE, 0x00, {0}}
};

static dsi_setting_tab_t lcm_mipi_jd9365d_swrst_setting[] = {
	{DSI_DI_GENERIC_WRITE_1_PARAM, 2, {0xE0, 0x00}},
	{DSI_DI_DCS_WRITE_0_PARAM,     1, {0x01}},
	{DSI_DI_DELAY,                50, {0x00}},
	{DSI_DI_END_OF_TABLE,         0x00, {0x00}}
};
#endif

#ifdef CONFIG_QUA_LOGO_MIPI_JD9365D_PWR_CTRL
static int32_t _pwron_1st_exited_flag = 0;
static int32_t _pwron_2nd_exited_flag = 0;
static int32_t qua_lcm_mipi_jd9365d_PowerOn(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;
	lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

#if LCM_JD9365D_DEBUG
	printf("%s e\n",__FUNCTION__);
    printf("%s:%d\n",__FUNCTION__,__LINE__);
#endif
	lcmHandle->lcm_id = lcmHandle->lcm_probe.lcm_id;

	if((lcmHandle->lcm_probe.lcm_id == 0 && _pwron_1st_exited_flag == 0) ||
	   (lcmHandle->lcm_probe.lcm_id == 1 && _pwron_2nd_exited_flag == 0)) {
		printf("%s:%d lcmHandle->lcm_id=%d\n",__func__,__LINE__, lcmHandle->lcm_id);
		extern int qua_lcm_mipi_jd9365d_hrst(int lcm_id);
		qua_lcm_mipi_jd9365d_hrst(lcmHandle->lcm_probe.lcm_id);
	}

	if(lcmHandle->lcm_probe.lcm_id == 0) _pwron_1st_exited_flag = 1;
	if(lcmHandle->lcm_probe.lcm_id == 1) _pwron_2nd_exited_flag = 1;

	ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
#if LCM_JD9365D_DEBUG
	printf("%s x\n",__FUNCTION__);
#endif
	return rtn;
}

static int32_t _identify_1st_exited_flag = 0;
static int32_t _identify_2nd_exited_flag = 0;
static int32_t _identify_lcm_idenfy_flag = 0;
static int32_t _identify_lcm_idenfy_virt = 0;
extern int qua_lcm_mipi_jd9365d_rst(int lcm_id);
extern void qua_set_lcm_bootargs(int value);
extern uint32_t dsi_mipi_get_err_code(void);

static int qua_lcm_mipi_jd9365d_identify_entity(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;
	uint8_t read_buf[4] = {0};
	int count = 3;
	lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

	lcmHandle->lcm_id = lcmHandle->lcm_probe.lcm_id;

	while(count){
		rtn = ops->ctrl_fun.mipi.dcs_read(lcmHandle->lcm_probe.lcm_id, DSI_DI_DCS_READ_0_PARAM, 0x04, 1, read_buf);
		printf("%s %d: 0x%x 0x%x 0x%x\n",__func__, lcmHandle->lcm_probe.lcm_id, read_buf[0], read_buf[1], read_buf[2]);
		if (rtn) {
			printf("%s dcs_read id failed.\n", __func__);
			return -1;
		}
		if(read_buf[0] == 0x93) {
			printf("%s identify sucess.\n", __func__);
			_identify_lcm_idenfy_flag |= (1 << lcmHandle->lcm_probe.lcm_id);
			qua_set_lcm_bootargs(_identify_lcm_idenfy_flag);
			return 0;
		}

		ops->delayMs(2);

		count--;
		if (count == 0) {
			printf("%s identify timeout failed.\n", __func__);
			return -2;
		}
	}

	return 0;
}

static int32_t qua_lcm_mipi_jd9365d_identify(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;
	lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

	#if LCM_JD9365D_DEBUG
	printf("%s:%d\n", __func__, __LINE__);
	#endif
	lcmHandle->lcm_id = lcmHandle->lcm_probe.lcm_id;

	if(lcmHandle->lcm_probe.lcm_id == 0 && _identify_1st_exited_flag) return 0;
	if(lcmHandle->lcm_probe.lcm_id == 1 && _identify_2nd_exited_flag) return 0;

	if(lcmHandle->lcm_probe.lcm_id == 0) _identify_1st_exited_flag = 1;
	if(lcmHandle->lcm_probe.lcm_id == 1) _identify_2nd_exited_flag = 1;

	/* first identify */
	rtn = qua_lcm_mipi_jd9365d_identify_entity(param, ops);
	_identify_lcm_idenfy_virt |= (1 << lcmHandle->lcm_probe.lcm_id);
	qua_set_lcm_bootargs(_identify_lcm_idenfy_virt);
	if(rtn) {
		printf("%s identify_entity failed.\n", __func__);
		goto __err_exit;
	}
	return 0;

// 	/* second identify */
// __err_srst:
// 	rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, lcm_mipi_jd9365d_swrst_setting, ARRAY_SIZE(lcm_mipi_jd9365d_swrst_setting));
// 	if(rtn) {
// 		printf("%s push_table failed.\n", __func__);
// 		goto __err_hrst;
// 	}

// 	rtn = qua_lcm_mipi_jd9365d_identify_entity(param, ops);
// 	if(rtn) {
// 		printf("%s identify_entity failed.\n", __func__);
// 		goto __err_hrst;
// 	}
// 	return 0;

// 	/* third identify */
// __err_hrst:
// 	qua_lcm_mipi_jd9365d_rst(lcmHandle->lcm_probe.lcm_id);
// 	ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
// 	ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
// 	rtn = qua_lcm_mipi_jd9365d_identify_entity(param, ops);
// 	if(rtn) {
// 		printf("%s identify_entity failed.\n", __func__);
// 		goto __err_frst;
// 	}
// 	return 0;

// 	/* fourth identify */
// __err_frst:
// 	qua_lcm_mipi_jd9365d_rst(lcmHandle->lcm_probe.lcm_id);
// 	ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
// 	ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
// 	rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, lcm_mipi_jd9365d_swrst_setting, ARRAY_SIZE(lcm_mipi_jd9365d_swrst_setting));
// 	rtn = qua_lcm_mipi_jd9365d_identify_entity(param, ops);
// 	if(rtn) {
// 		printf("%s identify_entity failed.\n", __func__);
// 		goto __err_exit;
// 	}
// 	return 0;

__err_exit:
	ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
	ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
	return 0;
}
#else /*CONFIG_QUA_LOGO_MIPI_JD9365D_PWR_CTRL*/
static int32_t qua_lcm_mipi_jd9365d_PowerOn(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;

#if LCM_JD9365D_DEBUG
	printf("%s e\n",__func__);
#endif
#ifdef CONFIG_UBOOT_XKPHOTO
	extern int qua_lcm_mipi_jd9365d_bkl(void);
	qua_lcm_mipi_jd9365d_bkl();
#endif
	ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
#if LCM_JD9365D_DEBUG
	printf("%s x\n",__func__);
#endif
	return rtn;
}

static int32_t qua_lcm_mipi_jd9365d_identify(void * param, lcm_ops *ops)
{
#if LCM_JD9365D_DEBUG
	printf("%s e\n",__func__);
#endif

#if LCM_JD9365D_DEBUG
	printf("%s x\n",__func__);
#endif
	return 0;
}
#endif /*CONFIG_QUA_LOGO_MIPI_JD9365D_PWR_CTRL*/

static int32_t qua_lcm_mipi_jd9365d_PowerOff(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;

#if LCM_JD9365D_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
#endif
    ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);
#if LCM_JD9365D_DEBUG
	printf("%s\n",__func__);
#endif
	return rtn;
}

static int32_t qua_lcm_mipi_jd9365d_deinit(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;

#if LCM_JD9365D_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
#endif
    rtn = qua_lcm_mipi_jd9365d_PowerOff(param, ops);
	return rtn;
}

static int32_t qua_lcm_mipi_jd9365d_init(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;

	lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

#if LCM_JD9365D_DEBUG
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	printf("%s lcm_id:%d\n",__FUNCTION__,lcmHandle->lcm_id);
#endif

	rtn = ops->ctrl_fun.mipi.push_tab(lcmHandle->lcm_id, qua_lcm_mipi_jd9365d_jlm101b021_init_setting, ARRAY_SIZE(qua_lcm_mipi_jd9365d_jlm101b021_init_setting));
#if LCM_JD9365D_DEBUG
	printf("%s rtn %d\n",__func__,rtn);
#endif

#ifdef CONFIG_QUA_LOGO_MIPI_JD9365D_PWR_CTRL
	/* Check the 2nd lcm initial error code and identify flag 
	** And just assume that the first screen must be existed.
	*/
	if ((lcmHandle->lcm_id == 1) && ((_identify_lcm_idenfy_flag & 2) == 0)) {
		int lcm = 3, err = 0;

		err = dsi_mipi_get_err_code();
		if (err & 2) lcm = 1;
		printf("%s err=%d lcm=%d\n",__func__,err,lcm);
		qua_set_lcm_bootargs(lcm);
	}
#endif

	return rtn;
}

static int32_t qua_lcm_mipi_jd9365d_suspend(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;

#if LCM_JD9365D_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
#endif

    ops->ctrl_fun.mipi.bus_deinit((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);

	return rtn;
}

static int32_t qua_lcm_mipi_jd9365d_resume(void * param, lcm_ops *ops)
{
	int32_t rtn = 0;

#if LCM_JD9365D_DEBUG
    printf("%s:%d\n",__FUNCTION__,__LINE__);
#endif

    ops->ctrl_fun.mipi.bus_init((void *)param, (void *)&qua_lcm_mipi_jd9365d_jlm101b021_info);

	//rtn = dsi_push_table(lcm_mipi_JD9365D_sleep_out_setting, ARRAY_SIZE(lcm_mipi_JD9365D_sleep_out_setting));
	return rtn;
}

lcm_module_fun qua_lcm_mipi_jlm101b021_ctrl =
{
	.poweron = qua_lcm_mipi_jd9365d_PowerOn,
    .poweroff = qua_lcm_mipi_jd9365d_PowerOff,
	.identify = qua_lcm_mipi_jd9365d_identify,
	.init = qua_lcm_mipi_jd9365d_init,
	.suspend = qua_lcm_mipi_jd9365d_suspend,
	.resume = qua_lcm_mipi_jd9365d_resume,
	.deinit = qua_lcm_mipi_jd9365d_deinit,
};

static lcm_display_sync_info_t qua_lcm_mipi_jd9365d_dp_info =
{
    .hor_sync_active = 20,
    .hor_back_porch = 40,
    .hor_active_pixel = LCM_JD9365D_WIDTH,
    .hor_front_porch = 40,

    .ver_sync_active = 4,
    .ver_back_porch = 18,
    .ver_active_line = LCM_JD9365D_HEIGHT,
    .ver_front_porch = 20,
};

static dsi_timing_t qua_lcm_mipi_jd9365d_timing =
{
    /*clk lane timing cfg*/
    .clane.mode = 1,
    .clane.lp11 = 0,
    .clane.lp01 = 0,
    .clane.zero = 0x22,
    .clane.prepare = 0x7,
    .clane.pre = 0x4,
    .clane.post = 0x10,
    .clane.trail = 0x9,
    .clane.inittime = 0x10,
    .clane.exit = 0x10,
    /*data lane timing cfg*/
    .dlane.mode = 1,
    .dlane.lp11 = 0x10,
    .dlane.lp01 = 0x10,
    .dlane.zero = 13,
    .dlane.prepare = 6,
    .dlane.trail = 10,
    .dlane.inittime = 16,
    .dlane.exit = 16,
    /*data lane0 read timing cfg*/
    .read.mode = 1,
    .read.tago = 67,
    .read.tasure = 25,
    .read.taget = 84,
    /*clk cfg*/
    .clk = {
        .vou.mode = 1,
        .vou.sel = 0x3,
        .vou.div = 0,

        .mif.mode = 1,
        .mif.pll_doubler = 0,
        .mif.pll_div_s = 1,
        .mif.pll_n = 0,
        .mif.pll_kint = 3254779,
        .mif.pll_nint = 71,
        .mif.pixelclk_div = 12,
        .mif.pll_pdiv = 1,
        .mif.dhd_div = 1,
    }
};

static lcm_mipi_info qua_lcm_mipi_jd9365d_mipi_info =
{
	.work_mode = DSI_VIDEO_MODE,
	.lane_num = DSI_TWO_LANE,
    .format = DSI_RGB888,
    .packet_size = 256,
    .timing = (void *)&qua_lcm_mipi_jd9365d_timing,
};

const lcm_cfg_t qua_lcm_mipi_jd9365d_jlm101b021_info =
{
	.verder_name = VENDER_NAME,
	.name = "qua_jd9365d_dsi_800_1280_60_jlm101b021",
	.width = LCM_JD9365D_WIDTH,
	.height = LCM_JD9365D_HEIGHT,
	.type = LCM_TYPE_MIPI, /*mcu, rgb, mipi*/
	.fps = 60,
	.direction = LCM_DIRECT_NORMAL,
    .dp_sync = &qua_lcm_mipi_jd9365d_dp_info,
	.infor = {
		.mipi = &qua_lcm_mipi_jd9365d_mipi_info,
	},
	.fun = &qua_lcm_mipi_jlm101b021_ctrl,
};
