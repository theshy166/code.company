#ifndef _VOU_REGISTER_DEFINE_H_
#define _VOU_REGISTER_DEFINE_H_

union _vou_u32_tag {
	struct _vou_u32_map {
		volatile unsigned int u32val				:32;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_chn_u1_tag {
	struct _vou_chn_u1_map {
		volatile unsigned int u1val					:1;
	}mbits;
	volatile unsigned int dwvalue;
};

union _VOU_VERSION_ID_tag {
	 struct _VOU_VERSION_ID_map {
        volatile unsigned int vou_version_id	:8;
		volatile unsigned int reserved0			:24;
    }mBits;
    volatile unsigned int dwValue;
}; 

/*global registers*/
union _VOU_DSD_CK_EB_tag{
	struct _VOU_DSD_CK_EB_map{
		volatile unsigned int dsd_chn_ck_eb		:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;

};

union _VOU_DHD0_CK_EB_tag{
	struct _VOU_DHD0_CK_EB_map{
		volatile unsigned int dhd0_chn_ck_eb	:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;

};

union _VOU_DHD1_CK_EB_tag{
	struct _VOU_DHD1_CK_EB_map{
		volatile unsigned int dhd1_chn_ck_eb	:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;

};

union _VOU_DHD_V1_CK_EB_tag{
	struct _VOU_DHD_V1_CK_EB_map{
		volatile unsigned int dhd_v1_ck_eb		:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _VOU_VGA_SRC_tag{
	struct _VOU_VGA_SRC_map{
		volatile unsigned int vga_eb			:1;
		volatile unsigned int vga_src_sel		:1;    //0:DHD0 1:DHD1
		volatile unsigned int reserved0			:30;
	}mBits;
	volatile unsigned int dwValue;
};

union _VOU_HDMI0_SRC_tag{
	struct _VOU_HDMI0_SRC_map{
		volatile unsigned int hdmi0_eb			:1;
		volatile unsigned int hdmi0_src_sel		:1;    //0:DHD0 1:DHD1
		volatile unsigned int reserved0			:30;
	}mBits;
	volatile unsigned int dwValue;
};

union _VOU_AXI_CFG_tag {
	 struct _VOU_AXI_CFG_map {
        volatile unsigned int rch_outstand_cfg	:2;
		volatile unsigned int wch_outstand_cfg	:2;
		volatile unsigned int reserved0			:4;
		volatile unsigned int rbus_stop			:1;
		volatile unsigned int wbus_stop			:1;
		volatile unsigned int reserved1			:6;
		volatile unsigned int dsd_rch_stop		:1;
		volatile unsigned int dsd_wch_stop		:1;
		volatile unsigned int dsd_rcnt_clr		:1;
		volatile unsigned int dsd_wcnt_clr		:1;
		volatile unsigned int dhd0_rch_stop		:1;
		volatile unsigned int dhd0_wch_stop		:1;
		volatile unsigned int dhd0_rcnt_clr		:1;
		volatile unsigned int dhd0_wcnt_clr		:1;
		volatile unsigned int dhd1_rch_stop		:1;
		volatile unsigned int dhd1_wch_stop		:1;
		volatile unsigned int dhd1_rcnt_clr		:1;
		volatile unsigned int dhd1_wcnt_clr		:1;
		volatile unsigned int reserved2			:4;
    }mBits;
    volatile unsigned int dwValue;
}; 

union _VOU_AXI_STS_tag {
	 struct _VOU_AXI_STS_map {
        volatile unsigned int axi_busy			:1;
		volatile unsigned int axi_rch_busy		:1;
		volatile unsigned int axi_wch_busy		:1;
		volatile unsigned int axi_rbus_busy		:1;
		volatile unsigned int axi_wbus_busy		:1;
		volatile unsigned int reserved0			:11;
		volatile unsigned int dsd_rch_busy		:1;
		volatile unsigned int dsd_wch_busy		:1;
		volatile unsigned int dhd0_rch_busy		:1;
		volatile unsigned int dhd0_wch_busy		:1;
		volatile unsigned int dhd1_rch_busy		:1;
		volatile unsigned int dhd1_wch_busy		:1;
		volatile unsigned int reserved1			:10;
    }mBits;
    volatile unsigned int dwValue;
}; 

union _VOU_AXI_CH_STS0_tag {
	 struct _VOU_AXI_CH_STS0_map {
		volatile unsigned int dsd_rcnt			:8;
		volatile unsigned int dsd_wcnt			:8;
		volatile unsigned int dhd0_rcnt			:8;
		volatile unsigned int dhd0_wcnt			:8;
    }mBits;
    volatile unsigned int dwValue;
}; 

union _VOU_AXI_CH_STS1_tag {
	 struct _VOU_AXI_CH_STS1_map {
		volatile unsigned int dhd1_rcnt			:8;
		volatile unsigned int dhd1_wcnt			:8;
		volatile unsigned int reserved			:16;
    }mBits;
    volatile unsigned int dwValue;
};

union _DHD_DISPLAY_FORMAT_tag {
	struct _DHD_DISPLAY_FORMAT_map {
		volatile unsigned int dhd_display_format	:3; // DHD输出模式: 000: VGA/HDMI mode, 001: DSI mode, 010: AVE mode, 011: BT656/1120 MODE, 100: LCM mode
		volatile unsigned int reserved				:29;
	}mBits;
	volatile unsigned int dwValue;
};

//union _\1_tag {\r  struct _\1_map {\r volatile unsigned int reserved0			:32;\r    }mBits;\r    volatile unsigned int dwValue;\r};\r

union _ARQOS_LOW_tag {
	 struct _ARQOS_LOW_map {
		volatile unsigned int qos				:4;
		volatile unsigned int reserved0			:28;
}mBits;
volatile unsigned int dwValue;
};

union _ARQOS_HIGH_tag {
	 struct _ARQOS_HIGH_map {
		volatile unsigned int qos				:4;
		volatile unsigned int reserved0			:28;
	}mBits;
	volatile unsigned int dwValue;
};

union _ARQOS_THR_LOW_tag {
	 struct _ARQOS_THR_LOW_map {
		volatile unsigned int threshold			:13;
		volatile unsigned int reserved0			:19;
	}mBits;
	volatile unsigned int dwValue;
};

union _ARQOS_THR_HIGH_tag {
	 struct _ARQOS_THR_HIGH_map {
		volatile unsigned int threshold			:13;
		volatile unsigned int reserved0			:19;
	}mBits;
	volatile unsigned int dwValue;
};

union _AWQOS_LOW_tag {
	 struct _AWQOS_LOW_map {
		volatile unsigned int qos				:4;
		volatile unsigned int reserved0			:28;
	}mBits;
	volatile unsigned int dwValue;
};

union _AWQOS_HIGH_tag {
	 struct _AWQOS_HIGH_map {
		volatile unsigned int qos				:4;
		volatile unsigned int reserved0			:28;
	}mBits;
	volatile unsigned int dwValue;
};

union _AWQOS_THR_LOW_tag {
	 struct _AWQOS_THR_LOW_map {
		volatile unsigned int threshold			:11;
		volatile unsigned int reserved0			:21;
	}mBits;
	volatile unsigned int dwValue;
};

union _AWQOS_THR_HIGH_tag {
	 struct _AWQOS_THR_HIGH_map {
		volatile unsigned int threshold			:11;
		volatile unsigned int reserved0			:21;
	}mBits;
	volatile unsigned int dwValue;
};

union _RCH_URGENCY_CNT_CTRL_tag {
	 struct _RCH_URGENCY_CNT_CTRL_map {
		volatile unsigned int rch_urgency_cnt_clr	:1;
		volatile unsigned int rch_urgency_cnt_en	:1;
		volatile unsigned int reserved				:30;
    }mBits;
    volatile unsigned int dwValue;
};

union _WCH_URGENCY_CNT_CTRL_tag {
	 struct _WCH_URGENCY_CNT_CTRL_map {
		 volatile unsigned int wch_urgency_cnt_clr	 :1;
		 volatile unsigned int wch_urgency_cnt_en	 :1;
		 volatile unsigned int reserved 			 :30;
    }mBits;
    volatile unsigned int dwValue;
};

union _RCH_URGENCY_CNT_tag {
	 struct _RCH_URGENCY_CNT_map {
		volatile unsigned int rch_urgency_cnt		:32;
	}mBits;
	volatile unsigned int dwValue;
};

union _WCH_URGENCY_CNT_tag {
	 struct _WCH_URGENCY_CNT_map {
		volatile unsigned int wch_urgency_cnt		:32;
	}mBits;
	volatile unsigned int dwValue;
};

union _VOU_CHN_INT_STS_tag {
	 struct _VOU_CHN_INT_STS_map {
        volatile unsigned int chn_sof				:1;
		volatile unsigned int chn_eof				:1;
		volatile unsigned int chn_underrun			:1;
		volatile unsigned int reg_update_done		:1;
		volatile unsigned int reserved0				:3;
		volatile unsigned int reg_rdy_update_done	:1;
		volatile unsigned int wbc_done				:1;
		volatile unsigned int wbc_overflow			:1;
		volatile unsigned int osd_fbc_err			:1;
		volatile unsigned int reserved1				:1;
		volatile unsigned int tt_slice0_err			:1;
		volatile unsigned int tt_slice1_err			:1;
		volatile unsigned int tt_slice2_err			:1;
		volatile unsigned int tt_slice3_err			:1;
		volatile unsigned int img_state_err			:1;
		volatile unsigned int reserved2				:1;
		volatile unsigned int tt_slice4_err			:1;
		volatile unsigned int tt_slice5_err			:1;
		volatile unsigned int tt_slice6_err			:1;
		volatile unsigned int tt_slice7_err			:1;
		volatile unsigned int reserved3				:7;
		volatile unsigned int internal_reset_done	:1;
		volatile unsigned int chn_open				:1;
		volatile unsigned int chn_shutdown			:1;
    }mBits;
    volatile unsigned int dwValue;
};

/*for CHN_INT_STS/CHN_INIT_MASK/CHN_INT_CLR*/
union _vou_chn_int_tag {
    struct _vou_chn_int_map {
        volatile unsigned int chn_int			:32;
    }mBits;
    volatile unsigned int dwValue;
};

union _CHN_VGA_EB_tag {
	 struct _CHN_VGA_EB_map {
		volatile unsigned int chn_vga_eb		:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _CHN_HDMI_EB_tag {
	 struct _CHN_HDMI_EB_map {
		volatile unsigned int chn_hdmi_eb		:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _CHN_WBC_CLR_tag {
	 struct _CHN_WBC_CLR_map {
		volatile unsigned int chn_wbc_clr		:1;
		volatile unsigned int reserved0			:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_update_ctrl0_tag{
	struct _vou_update_ctrl0_map{
		volatile unsigned int  all_update_rdy			:1;
		volatile unsigned int  glb_update_rdy			:1;
		volatile unsigned int  v0_glb_update_rdy		:1;
		volatile unsigned int  v1_glb_update_rdy		:1;
		volatile unsigned int  osd_update_rdy			:1;
		volatile unsigned int  hc_update_rdy			:1;
		volatile unsigned int  tve_update_rdy			:1;
		volatile unsigned int  wbc_update_rdy			:1;
		volatile unsigned int  v1_b0_update_rdy 		:1;
		volatile unsigned int  v0_sccoef_update_rdy 	:1;
		volatile unsigned int  v1_sccoef_update_rdy 	:1;
		volatile unsigned int  wbc_sccoef_update_rdy 	:1;
		volatile unsigned int  reserved0			 	:20;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_update_ctrl1_tag{
		struct _vou_update_ctrl1_map{
			volatile unsigned int v0_b00_update_rdy	:1;
			volatile unsigned int v0_b01_update_rdy	:1;
			volatile unsigned int v0_b02_update_rdy	:1;
			volatile unsigned int v0_b03_update_rdy	:1;
			volatile unsigned int v0_b04_update_rdy	:1;
			volatile unsigned int v0_b05_update_rdy	:1;
			volatile unsigned int v0_b06_update_rdy	:1;
			volatile unsigned int v0_b07_update_rdy	:1;
			volatile unsigned int v0_b10_update_rdy	:1;
			volatile unsigned int v0_b11_update_rdy	:1;
			volatile unsigned int v0_b12_update_rdy	:1;
			volatile unsigned int v0_b13_update_rdy	:1;
			volatile unsigned int v0_b14_update_rdy	:1;
			volatile unsigned int v0_b15_update_rdy	:1;
			volatile unsigned int v0_b16_update_rdy	:1;
			volatile unsigned int v0_b17_update_rdy	:1;
			volatile unsigned int v0_b20_update_rdy	:1;
			volatile unsigned int v0_b21_update_rdy	:1;
			volatile unsigned int v0_b22_update_rdy	:1;
			volatile unsigned int v0_b23_update_rdy	:1;
			volatile unsigned int v0_b24_update_rdy	:1;
			volatile unsigned int v0_b25_update_rdy	:1;
			volatile unsigned int v0_b26_update_rdy	:1;
			volatile unsigned int v0_b27_update_rdy	:1;
			volatile unsigned int v0_b30_update_rdy	:1;
			volatile unsigned int v0_b31_update_rdy	:1;
			volatile unsigned int v0_b32_update_rdy	:1;
			volatile unsigned int v0_b33_update_rdy	:1;
			volatile unsigned int v0_b34_update_rdy	:1;
			volatile unsigned int v0_b35_update_rdy	:1;
			volatile unsigned int v0_b36_update_rdy	:1;
			volatile unsigned int v0_b37_update_rdy	:1;			
		}mBits;
		volatile unsigned int dwValue;
};

union _vou_update_ctrl2_tag{
		struct _vou_update_ctrl2_map{
			volatile unsigned int v0_b40_update_rdy	:1;
			volatile unsigned int v0_b41_update_rdy	:1;
			volatile unsigned int v0_b42_update_rdy	:1;
			volatile unsigned int v0_b43_update_rdy	:1;
			volatile unsigned int v0_b44_update_rdy	:1;
			volatile unsigned int v0_b45_update_rdy	:1;
			volatile unsigned int v0_b46_update_rdy	:1;
			volatile unsigned int v0_b47_update_rdy	:1;
			volatile unsigned int v0_b50_update_rdy	:1;
			volatile unsigned int v0_b51_update_rdy	:1;
			volatile unsigned int v0_b52_update_rdy	:1;
			volatile unsigned int v0_b53_update_rdy	:1;
			volatile unsigned int v0_b54_update_rdy	:1;
			volatile unsigned int v0_b55_update_rdy	:1;
			volatile unsigned int v0_b56_update_rdy	:1;
			volatile unsigned int v0_b57_update_rdy	:1;
			volatile unsigned int v0_b60_update_rdy	:1;
			volatile unsigned int v0_b61_update_rdy	:1;
			volatile unsigned int v0_b62_update_rdy	:1;
			volatile unsigned int v0_b63_update_rdy	:1;
			volatile unsigned int v0_b64_update_rdy	:1;
			volatile unsigned int v0_b65_update_rdy	:1;
			volatile unsigned int v0_b66_update_rdy	:1;
			volatile unsigned int v0_b67_update_rdy	:1;
			volatile unsigned int v0_b70_update_rdy	:1;
			volatile unsigned int v0_b71_update_rdy	:1;
			volatile unsigned int v0_b72_update_rdy	:1;
			volatile unsigned int v0_b73_update_rdy	:1;
			volatile unsigned int v0_b74_update_rdy	:1;
			volatile unsigned int v0_b75_update_rdy	:1;
			volatile unsigned int v0_b76_update_rdy	:1;
			volatile unsigned int v0_b77_update_rdy	:1;
		}mBits;
		volatile unsigned int dwValue;
};

union _vou_chn_force_update_tag{
	struct _vou_chn_force_update_map{
		volatile unsigned int force_update			:1;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_update_mode_tag{
	struct _vou_update_mode_map{
		volatile unsigned int all_update_mode		:1;

	}mBits;
	volatile unsigned int dwValue;
};

union _vou_chn_work_ctrl_tag{
	struct _vou_chn_work_ctrl_map{
		volatile unsigned int chn_work_eb			:1;
		volatile unsigned int chn_shutdown			:1;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_chn_minor_ctrl_tag{
	struct _vou_chn_minor_ctrl_map{
		volatile unsigned int path_clr_eb			:1;
		volatile unsigned int sccoef_sw_cfg			:1;
		volatile unsigned int sccoef_rdy			:1;
		volatile unsigned int reserve0				:1;
		volatile unsigned int reset_gen_eb			:1;
		volatile unsigned int reset_each_frame		:1;
		volatile unsigned int fetch_delay_eb		:1;
		volatile unsigned int vblank_flush_eb		:1;
		volatile unsigned int reserve1				:8;
		volatile unsigned int lines_aheadof_frame	:4;
		volatile unsigned int reserve2				:12;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_chn_cfg_tag {
	struct _vou_chn_cfg_map {
		volatile unsigned int Chn_out_type		:1;
		volatile unsigned int Reserved			:7;
		volatile unsigned int Clip_eb	 		:1;
		volatile unsigned int Clip_mode			:1;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_layer_order_tag{
	struct _vou_layer_order_map{
		volatile unsigned int Layer_order		:1;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_clip_info_tag {
    struct _vou_clip_info_map {
        volatile unsigned int YRGBClipLow		:8;
        volatile unsigned int YRGBClipHigh		:8;
        volatile unsigned int CbCrClipLow		:8;
		volatile unsigned int CbCrClipHigh		:8;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_bg_info_yuv_tag {
    struct _vou_bg_info_yuv_map {
        volatile unsigned int BackgroundY		:8;
        volatile unsigned int BackgroundCb		:8;
        volatile unsigned int BackgroundCr		:8;
		volatile unsigned int Reserved0			:8;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_bg_info_rgb_tag {
    struct _vou_bg_info_rgb_map {
        volatile unsigned int BackgroundR		:8;
        volatile unsigned int BackgroundG		:8;
        volatile unsigned int BackgroundB		:8;
        volatile unsigned int BackgroundA		:8;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_active_info_tag {
    struct _vou_active_info_map {
        volatile unsigned int Alen				:13;
        volatile unsigned int Reserved0			:3;
        volatile unsigned int Aline				:12;
		volatile unsigned int Reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_hblank_info_tag {
    struct _vou_hblank_info_map {
        volatile unsigned int Hlen				:13;
        volatile unsigned int Reserved0			:3;
        volatile unsigned int StartBLen			:12;
		volatile unsigned int Reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_tfblank_info_tag {
    struct _vou_tfblank_info_map {
        volatile unsigned int TVLine			:12;
        volatile unsigned int Reserved0			:4;
        volatile unsigned int TBLineOfs			:12;
		volatile unsigned int Reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_bfblank_info_tag {
    struct _vou_bfblank_info_map {
        volatile unsigned int BVLine			:12;
        volatile unsigned int Reserved0			:4;
        volatile unsigned int BBLineOfs			:12;
		volatile unsigned int Reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_testmode_cfg_tag{
	struct _vou_testmode_cfg_map{
		volatile unsigned int Test_mode_eb		:1;
		volatile unsigned int Test_mode_gray	:1;
		volatile unsigned int Test_mode			:2;
	}mBits;
	volatile unsigned int dwValue;

};

union _vou_chess_info_tag {
    struct _vou_chess_info_map {
        volatile unsigned int BlockWidth		:11;
        volatile unsigned int Reserved0			:5;
        volatile unsigned int BlockHeight		:11;
		volatile unsigned int CbCrClipHigh		:5;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_CSC_MATRIX_COEFF_tag {
    struct _vou_CSC_MATRIX_COEFF_map {
        volatile unsigned int GY				:10;
        volatile unsigned int BCb				:10;
        volatile unsigned int RCr				:10;
		volatile unsigned int Reserved			:2;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_CSC_OFFSET_tag {
    struct _vou_CSC_OFFSET_map {
        volatile unsigned int GYOfs				:10;
        volatile unsigned int BcbOfs			:10;
        volatile unsigned int RcrOfs			:10;
		volatile unsigned int Reserved			:2;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_RIM_WIDTH_tag {
    struct _vou_IMG_RIM_WIDTH_map {
        volatile unsigned int IMG_RIM_WIDTH		:2;
		volatile unsigned int Reserved0			:30;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_RIM_COLOR_tag {
	 struct _vou_IMG_RIM_COLOR_map {
        volatile unsigned int IMG_RIM_C_Y		:8;
        volatile unsigned int IMG_RIM_C_Cb		:8;
        volatile unsigned int IMG_RIM_C_Cr		:8;
		volatile unsigned int Reserved0			:8;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG0_CFG_tag{
	struct _vou_IMG0_CFG_map{
		volatile unsigned int Imag0_layer_eb	:1;
		volatile unsigned int Imag0_Graymode	:1;
		volatile unsigned int Reserved			:6;
		volatile unsigned int Imag0_Galpha		:8;
	}mBits;
	 volatile unsigned int dwValue;
};

union _vou_IMG1_CFG_tag{
	struct _vou_IMG1_CFG_map{
		volatile unsigned int Imag1_layer_eb	:1;
		volatile unsigned int Imag1_Graymode	:1;
		volatile unsigned int Reserved			:6;
		volatile unsigned int Imag1_Galpha		:8;
	}mBits;
	 volatile unsigned int dwValue;
};

/*V0/V1/WBC sccoef*/
union _vou_SCCOEF_ADDR_tag{
	struct _vou_SCCOEF_ADDR_map{
		volatile unsigned int sccoef_addr	:32;
	}mBits;
	 volatile unsigned int dwValue;
};

union _vou_HDMI_CFG_tag {
	 struct _vou_HDMI_CFG_map {
        volatile unsigned int HDMI_src			:1;
        volatile unsigned int HDMI_HSYNC_POL	:1;
        volatile unsigned int HDMI_VSYNC_POL	:1; 
		volatile unsigned int HDMI_DE_pol		:1;
		volatile unsigned int HDMI_vtotal_even	:1;
		volatile unsigned int Csc_matrix_sel	:2;
		volatile unsigned int clip_eb			:1;
		volatile unsigned int clip_mode			:1;
		volatile unsigned int uv_order 			:1;
        volatile unsigned int Reserved0			:22;
    }mBits;
    volatile unsigned int dwValue;
};

union _dhd_display_cfg_tag {
	struct _dhd_display_cfg_map {
		volatile unsigned int display_bit_depth         :2; // For DSI/LCM: 00: 24bits 888, 01: 18bits 666, 10: 16bits 565
		volatile unsigned int display_uv_order          :1; // for AVE/BT656/1120: 0: {VU}, 1: {UV}
		volatile unsigned int display_rgb_order         :3; //for DSI/LCM: 0: {BGR}, 1: {GBR}, 2: {RBG}, 3: {BRG}, 4: {GRB}, 5: {RGB}
		volatile unsigned int dsi_cmd_mode              :1; //0: VIDEO_mode, 1: CMD mode
		volatile unsigned int lcm_cmd_mode              :1; //0: RGB_mode, 1: CMD mode
		volatile unsigned int bt_1120_mode              :1; //0: BT656, 1: BT1120
		volatile unsigned int dpi_dataen_active_low     :1; //For DSI/LCM, 数据使能极性: 0：数据有效为高电平, 1：数据有效为低电平
		volatile unsigned int dpi_hsync_active_low      :1; //For DSI/LCM,水平同步极性: 0：消隐为高电平, 1：消隐为低电平
		volatile unsigned int dpi_vsync_active_low      :1; // For DSI/LCM,垂直同步极性: 0：消隐为高电平, 1：消隐为低电平
		volatile unsigned int dpi_shutdn_active_low     :1; // For DSI,关闭信号极性: 0：高有效, 1：低有效
		volatile unsigned int dpi_colorm_active_low     :1; // For DSI,COLORM极性: 0：高有效, 1：低有效
		volatile unsigned int dpi_te_en                 :1; // For DSI, Tearing Effect使能: 0：关闭, 1：使能
		volatile unsigned int dpi_video_mode_wait_halt  :1; // For DSI, DPI Video模式下，每帧内数据发送是否要受HALT信号影响
		volatile unsigned int dithering_en              :1; // 打开Dithering
		volatile unsigned int bitswap_en                :1; // RGB内部bit位置Swap
		volatile unsigned int use_clk_negedge           :1; // 0:在时钟上升沿输出 1:在时钟下降沿输出
		volatile unsigned int reserved0                 :5;
		volatile unsigned int ave_rgb2yuv_matrix_sel    :2; // for AVE/BT656/1120: 00:BT601, 01:BT709, 10:BT2020, 11:reserved
		volatile unsigned int ave_rgb2yuv_clip_eb       :1; // for AVE/BT656/1120: 0: output is full range, 1：clip to Narrow range 
		volatile unsigned int ave_rgb2yuv_clip_mode     :1; // for AVE/BT656/1120: 0: 直接clip, 1：进行拉伸操作
		volatile unsigned int rgb_format_type           :4; // For DSI/LCM 16bits and 18bits 0000: 有效数据向低位集中 0001: 有效数据分别向8bit低位集中 0010: 有效数据分别向8bit高位集中 0011: 在0001基础上bit数较少分量空出低位以补足位宽到最大位宽。 见说明。
	}mBits;
	volatile unsigned int dwValue;
};

union _dhd_display_delay_tag {
	 struct _dhd_display_delay_map {
		volatile unsigned int dhd_display_delay		:32;
	}mBits;
	volatile unsigned int dwValue;
};

union _vou_WBC_CFG_tag {
	 struct _vou_WBC_CFG_map {
        volatile unsigned int Wbc_chn_eb			:1;
        volatile unsigned int Wbc_src_sel			:2; // 00:video0 yuv 01:blended ARGB8888 10:reserved 11:blended YUV
        volatile unsigned int Wbc_format			:1;
        volatile unsigned int Wbc_IMG_UV_order		:1;
        volatile unsigned int Wbc_img_Scaler_en		:1;
		volatile unsigned int Wbc_img_crop_eb		:1;
		volatile unsigned int Wbc_img_format		:1;
		volatile unsigned int reserved1				:24;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_WBC_CROP_SRC_START_tag {
	 struct _vou_WBC_CROP_SRC_START_map {
        volatile unsigned int WBC_Crop_src_st_x	:12;
        volatile unsigned int Reserved0			:4;
        volatile unsigned int WBC_Crop_src_st_y	:12;
        volatile unsigned int Reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_WBC_CROP_DES_SIZE_tag {
	 struct _vou_WBC_CROP_DES_SIZE_map {
        volatile unsigned int WBC_crop_des_width	:13;
        volatile unsigned int Reserved0				:3;
        volatile unsigned int WBC_crop_des_height 	:12;
        volatile unsigned int Reserved1				:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_WBC_SCALER_DES_SIZE_tag {
	 struct _vou_WBC_SCALER_DES_SIZE_map {
        volatile unsigned int WBC_scaler_des_width	:12;
        volatile unsigned int Reserved0				:4;
        volatile unsigned int WBC_scaler_des_height	:11;  //needs to check why 11bits
        volatile unsigned int Reserved1				:5;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_OSD_CTRL_tag {
	 struct _vou_OSD_CTRL_map {
        volatile unsigned int OSD_in_format		:2;
        volatile unsigned int OSD_fbc_eb		:1;
		volatile unsigned int OSD_blk_mode		:1;
        volatile unsigned int OSD_alpha_order	:1;
        volatile unsigned int OSD_byte_order	:1;
        volatile unsigned int OSD_extend_mode	:2;
		volatile unsigned int reserved1			:23;
		volatile unsigned int OSD_layer_eb		:1;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_OSD_ALPHA_PARA_tag {
	 struct _vou_OSD_ALPHA_PARA_map {
        volatile unsigned int OSD_palpha_en		:1;
        volatile unsigned int OSD_Pre_multi_en	:1;
        volatile unsigned int OSD_Palpha_range	:1;
        volatile unsigned int Reserved			:5;	
        volatile unsigned int OSD_Galpha		:8;
        volatile unsigned int OSD_alpha_v0		:8;
        volatile unsigned int OSD_alpha_v1		:8;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_OSD_SRC_START_tag {
	 struct _vou_OSD_SRC_START_map {
        volatile unsigned int OSD_src_st_x		:12;
        volatile unsigned int reserved0			:4;
        volatile unsigned int OSD_src_st_y		:12;
        volatile unsigned int reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_OSD_SRC_SIZE_tag {
	 struct _vou_OSD_SRC_SIZE_map {
        volatile unsigned int OSD_src_width		:13;
        volatile unsigned int reserved0			:3;
        volatile unsigned int OSD_src_height	:12;
        volatile unsigned int reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_OSD_SRC_PITCH_tag {
	 struct _vou_OSD_SRC_PITCH_map {
        volatile unsigned int OSD_src_pitch		:13;
		volatile unsigned int reserved	:3;
		volatile unsigned int src_orig_height	:12;//just for cmp case
		volatile unsigned int reserved2 :4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_OSD_DES_START_tag {
	 struct _vou_OSD_DES_START_map {
        volatile unsigned int OSD_des_st_x		:12;
        volatile unsigned int reserved0			:4;
        volatile unsigned int OSD_des_st_y		:12;
        volatile unsigned int reserved1			:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_CTRL_tag {
	 struct _vou_IMG_BX_CTRL_map {
       volatile unsigned int format			:1;
       volatile unsigned int fbc_mode		:1;
       volatile unsigned int uv_order 		:1;
       volatile unsigned int reserved0		:1;
       volatile unsigned int rim_en			:1;
       volatile unsigned int rim_color_mode	:1;
       volatile unsigned int scalor_en		:1;
       volatile unsigned int reserved1		:24;
	   volatile unsigned int img_block_eb	:1;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_START_tag {
	 struct _vou_IMG_BX_START_map {
       volatile unsigned int x				:12;
       volatile unsigned int reserved0		:4;
       volatile unsigned int y				:12;
       volatile unsigned int reserved1		:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_SIZE_tag {
	 struct _vou_IMG_BX_SIZE_map {
       volatile unsigned int width			:13;
       volatile unsigned int reserved0		:3;
       volatile unsigned int height			:12;
       volatile unsigned int reserved1		:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_PITCH_tag {
	 struct _vou_IMG_BX_PITCH_map {
       volatile unsigned int pitch				:13;
		volatile unsigned int reserved			:3;
		volatile unsigned int src_orig_height	:12;//just for cmp case
		volatile unsigned int reserved2 		:4;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_CM_PARA0_tag {
	 struct _vou_IMG_BX_CM_PARA0_map {
       volatile unsigned int brightness		:8;
       volatile unsigned int satvxcontrs	:11;
       volatile unsigned int reserved0		:13; 
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_CM_PARA1_tag {
	 struct _vou_IMG_BX_CM_PARA1_map {
       volatile unsigned int huesinvalue	:6;
       volatile unsigned int sighuesin		:1;
       volatile unsigned int reserved0		:1;
       volatile unsigned int huecosvalue	:6;
       volatile unsigned int sighuecos		:1;
	   volatile unsigned int reserved1		:17;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_CM_PARA2_tag {
	 struct _vou_IMG_BX_CM_PARA2_map {
       volatile unsigned int shth0			:8;
       volatile unsigned int shth1			:8;
       volatile unsigned int k0				:4;
       volatile unsigned int k1				:4;
	   volatile unsigned int reserved0		:8;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_IMG_BX_CM_PARA3_tag {
	 struct _vou_IMG_BX_CM_PARA3_map {
       volatile unsigned int controffset	:8;
       volatile unsigned int reserved0		:8;
       volatile unsigned int contrslope		:5;
       volatile unsigned int reserved1 		:11;
    }mBits;
    volatile unsigned int dwValue;
};

union _vou_SC_COEF_tag {
	 struct _vou_SC_COEF_map {
       volatile unsigned int coef0			:9;
       volatile unsigned int coef1			:9;
       volatile unsigned int reserved0 		:14;
    }mBits;
    volatile unsigned int dwValue;
};

union _cvbs_video_format_tag {
	struct _cvbs_video_format_map {
		volatile unsigned int tvf			:2;
		volatile unsigned int black0ire		:1;
		volatile unsigned int vin_range		:1;
		volatile unsigned int reserved0		:1;
		volatile unsigned int nonitl		:1;
		volatile unsigned int subc			:1;
		volatile unsigned int nonitl_263	:1;
		volatile unsigned int reserved1		:24;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_video_input_tag {
	struct _cvbs_video_input_map {
		volatile unsigned int field_mode	:1;
		volatile unsigned int vinf			:2;
		volatile unsigned int vs_p			:1;
		volatile unsigned int hs_p			:1;
		volatile unsigned int vin_d			:2;
		volatile unsigned int reserved0		:25;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_video_output_tag {
	struct _cvbs_video_output_map {
		volatile unsigned int y_d			:3;
		volatile unsigned int no_c			:1;
		volatile unsigned int black			:1;
		volatile unsigned int blue			:1;
		volatile unsigned int vof			:1;
		volatile unsigned int reserved0		:25;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_power_down_tag {
	struct _cvbs_power_down_map {
		volatile unsigned int dacc_pd		:1;
		volatile unsigned int dacy_pd		:1;
		volatile unsigned int dacv_pd		:1;
		volatile unsigned int dac_clk_p		:1;
		volatile unsigned int clki_p		:1;
		volatile unsigned int reserved0		:27;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_soft_reset_tag {
	struct _cvbs_soft_reset_map {
		volatile unsigned int sreset		:1;
		volatile unsigned int reserved0		:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_filter_coef01_tag {
	struct _cvbs_filter_coef01_map {
		volatile unsigned int cvbs_filter_coef0		:16;
		volatile unsigned int cvbs_filter_coef1		:16;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_filter_coef23_tag {
	struct _cvbs_filter_coef23_map {
		volatile unsigned int cvbs_filter_coef2		:16;
		volatile unsigned int cvbs_filter_coef3		:16;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_filter_coef45_tag {
	struct _cvbs_filter_coef45_map {
		volatile unsigned int cvbs_filter_coef4		:16;
		volatile unsigned int cvbs_filter_coef5		:16;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_filter_coef67_tag {
	struct _cvbs_filter_coef67_map {
		volatile unsigned int cvbs_filter_coef6		:16;
		volatile unsigned int cvbs_filter_coef7		:16;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_detect_en_tag {
	struct _cvbs_detect_en_map {
		volatile unsigned int cvbs_detect_en		:1;
		volatile unsigned int reserved				:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_high_num_tag {
	struct _cvbs_high_num_map {
		volatile unsigned int cvbs_high_num			:32;
	}mBits;
	volatile unsigned int dwValue;
};

union _cvbs_low_num_tag {
	struct _cvbs_low_num_map {
		volatile unsigned int cvbs_low_num			:32;
	}mBits;
	volatile unsigned int dwValue;
};

//CLAMP_INFO0 and CLAMP_INFO1

union _vou_clamp_range_tag {
	struct _vou_clamp_range_map {
		volatile unsigned int level_low		:10;
		volatile unsigned int level_high	:10;
		volatile unsigned int blank_level	:10;
		volatile unsigned int reserved0		:2;
	}mBits;
	volatile unsigned int dwValue;
};

//CLAMP_INFO2 and CLAMP_INFO3
union _vou_clamp_offset_tag {
	struct _vou_clamp_offset_map {
		volatile unsigned int ofs0			:8;
		volatile unsigned int reserved0		:8;
		volatile unsigned int ofs1			:10;
		volatile unsigned int reserved1		:6;
	}mBits;
	volatile unsigned int dwValue;
};

union _sync_cfg_tag {
	struct _sync_cfg_map {
		volatile unsigned int fpol			:1;
		volatile unsigned int hpol			:1;
		volatile unsigned int vpol			:1;
		volatile unsigned int hsync_en		:1;
		volatile unsigned int vsync_en		:1;
		volatile unsigned int reserved1		:27;
	}mBits;
	volatile unsigned int dwValue;
};

union _sync_info_tag {
	struct _hsync_info_map {
		volatile unsigned int sync_length	:10;
		volatile unsigned int reserved0		:6;
		volatile unsigned int sync_delay	:12;
		volatile unsigned int reserved1		:4;
	}mBits;
	volatile unsigned int dwValue;
};

union _tir_cfg_tag {
	struct _tir_cfg_map {
		volatile unsigned int fir_bypass	:1;
		volatile unsigned int reserved0		:31;
	}mBits;
	volatile unsigned int dwValue;
};

union _dac_cfg_tag {
	struct _dac_cfg_map {
		volatile unsigned int gdac_level	:10;
		volatile unsigned int bdac_Level	:10;
		volatile unsigned int rdac_level	:10;
		volatile unsigned int dacdirectmode	:1;
		volatile unsigned int reserved0		:1;
	}mBits;
	volatile unsigned int dwValue;
};

union _soft_rst_tag {
	struct _soft_rst_map {
		volatile unsigned int hdmi_soft_rst			:1;
		volatile unsigned int vou_v1_soft_rst		:1;
		volatile unsigned int vou_dsd_soft_rst		:1;
		volatile unsigned int vou_dhd0_soft_rst		:1;
		volatile unsigned int vou_dhd1_soft_rst		:1;
		volatile unsigned int vou_vga_soft_rst		:1;
		volatile unsigned int vou_soft_rst			:1;
		volatile unsigned int reserve0				:2;
		volatile unsigned int ave_soft_rst			:1;
		volatile unsigned int vou_ave_clear			:1;
		volatile unsigned int vou_cvbs_soft_rst		:1;
		volatile unsigned int vou_sif_soft_rst		:1;
		volatile unsigned int vou_mif1_soft_rst		:1;
		volatile unsigned int vou_mif0_soft_rst		:1;
	}mBits;
	volatile unsigned int dwValue;
};


#endif
