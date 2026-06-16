#include "vou_proc.h"

const cm_param_t _cm_default = {
	.brightness = 0,
	.uvSatvXContrs = 512,
	.hueSin = 0,
	.hueCos = 32,
	.sharpThr = {0, 0},
	.sharpK = {0, 0},
	.yContrastOffset = 0,
	.yContrastSlope = 16
};

const vou_global_t _glb = {
	.dhd_chn_ck_eb = 0,
	.dsd_chn_ck_eb = 0,
	.dhd1_chn_ck_eb = 0,
	.vga_if_eb = 0,
	.vga_src_sel = 0,   //0:dhd0, 1:dhd1
	.hdmi_if_eb = 0,
	.hdmi_src_sel = 0,  //0:dhd0, 1:dhd1
	.arqos_low = 0x8,    //max:0xf
	.arqos_high = 0xf,   //max:0xf
	.arqos_thr_low = 0x200,  //max:7ff
	.arqos_thr_high = 0x400,  //max:7ff
	.awqos_low = 0x8,         //max:0xf
	.awqos_high = 0xf,        //max:0xf
	.awqos_thr_low = 0x80,
	.awqos_thr_high = 0xc0,
	.dhd_output_dst = DHD_OUTPUT_DST_HDMI,
};

const vou_dev_info_t _a_dev_info = {
	.width = 0,
	.height = 0,
	.frame_rate = 0,
	.progressive = 1,
	.gray_mode = 0,
	.img0_eb = 0,
	.img1_eb = 0,
	.osd_eb = 0,
	.hc_eb = 0,
	.img0_g_alpha = 128,
	.img1_g_alpha = 128,
	.osd_g_alpha = 128,
	.hc_g_alpha = 128,
	.img0_on_top = 0,
	.test_mode_eb = 0,
	.test_mode = 0
};

const tve_clamp_info_t _clamp_info = {
	.yrgb_black_level = 0x0,
	.yrgb_white_level = 0x224,
	.yrgb_blank_level = 0x0,
	.cbcr_peak_level_low = 0xee,
	.cbcr_peak_level_high = 0x312,
	.cbcr_blank_level = 0x200,
	.yrgb_ofs0 = 0x0,
	.yrgb_ofs1 = 0x0,
	.cbcr_ofs0 = 0x80,
	.cbcr_ofs1 = 0x200
};

const vou_clip_info_t _clip_info = {
	.yrgb_clip_low = 0,
	.yrgb_clip_high = 255,
	.uv_clip_low = 0,
	.uv_clip_high = 255,
	.mode = 0
};

const vou_hdmi_info_t _hdmi_info = {
	.hdmi_src = 0,
	.hsync_pol = 0,
	.vsync_pol = 0,
	.de_pol = 0,
	.vtotal_even = 0,
	.r2y_matrix_sel = 0,
	.clip_eb = 0,
	.clip_mode = 0,
	.hdmi_uv_order = 0
};

const y2r_matrix_t _y2r_matrix = {
	.coef = {{256, -88, -183}, {256, 454, 0}, {256, 0, 359}},
	.offset = {135, -227, -179}
};

const vou_clip_info_t _dsd_clip_info = {
	.yrgb_clip_low = 16,
	.yrgb_clip_high = 235,
	.uv_clip_low = 16,
	.uv_clip_high = 240,
	.mode = 0
};

vou_sync_info_t _vou_vga_hdmi_sync = {
	.Clock = 0x13C4,// 108MHz
	.Hactvie = 1280,
	.Vactive = 1024,
	.FrameMode = 1,
	.FrameRate = 60,
	.Hfront = 48,
	.Hback = 248,
	.Hsync = 112,
	.Hpol = 1,
	.Vfront = 1,
	.Vback = 38,
	.Vsync = 3,
	.Vpol = 1,
};

vou_sync_info_t _vou_cvbs_pal_sync = {
	.Clock = 0,
	.Hactvie = 720,
	.Vactive = 576,
	.FrameMode = 0,
	.FrameRate = 50,
	.Alen = 720,
	.Aline = 576,
	.Hlen = 863,
	.StartBlen = 144,
	.EndBlen = 0,
	.TVLine = 311,
	.TBLineOfs = 21,
	.TEndBLine = 2,
	.BVLine = 312,
	.BBLineOfs = 22,
	.BEndBLine = 2,
	.HSyncLength = 0,
	.VSyncLength0 = 0,
};

vou_sync_info_t _vou_cvbs_ntsc_sync = {
	.Clock = 0,
	.Hactvie = 720,
	.Vactive = 480,
	.FrameMode = 0,
	.FrameRate = 60,
	.Alen = 720,
	.Aline = 480,
	.Hlen = 857,
	.StartBlen = 138,
	.EndBlen = 0,
	.TVLine = 261,
	.TBLineOfs = 15,
	.TEndBLine = 2,
	.BVLine = 262,
	.BBLineOfs = 15,
	.BEndBLine = 3,
	.HSyncLength = 0,
	.VSyncLength0 = 0,
};

vou_sync_info_t _vou_cvbs_sync_table[] = {
	{0,              720,   576,   0,   50,   27000000,   864,   625,   144,   49,   0,     0,    0,    0,  0,   0,   0,    0,  720,   576,   863,   144,  0,     311,   21,   2,   312,  22,  2,  0,    0},
	{0,              720,   480,   0,   60,   26999973,   858,   525,   138,   45,   0,     0,    0,    0,  0,   0,   0,    0,  720,   480,   857,   138,  0,     261,   19,   2,   262,  19,  3,  0,    0},
};

