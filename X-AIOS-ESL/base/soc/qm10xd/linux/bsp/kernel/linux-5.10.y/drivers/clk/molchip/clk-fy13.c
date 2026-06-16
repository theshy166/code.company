/*
 * Copyright (c) 2019 Molchip  Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <dt-bindings/clock/fy13-clock.h>
#include <linux/clk-provider.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include "clk.h"

static const struct mc_fixed_rate_clock  fixed_rate_clks[] = {
	{ CLK_UPLL, "clk_upll", NULL, 0, 640000000, },
	{ CLK_MPLL, "clk_mpll", NULL, 0, 600000000, },
	{ CLK_MPLL1, "clk_mpll1", NULL, 0, 546000000, },
	{ CLK_MPLL2, "clk_mpll2", NULL, 0, 500000000, },
	{ CLK_EPLL, "clk_epll", NULL, 0, 458000000, },
	{ CLK_EPLL1, "clk_epll1", NULL, 0, 400000000, },
	{ CLK_VEUPLL, "clk_veupll", NULL, 0, 768000000, },
	{ CLK_AIPLL, "clk_aipll", NULL, 0, 1000000000, },

};

/* fixed factor */
static const struct mc_fixed_factor_clock fixed_factor_clks[] = {
	{ CLK_MPLL_120, "clk_mpll_120", "clk_mpll", 1, 5, 0, },
	{ CLK_MPLL_250, "clk_mepll_250", "clk_mpll2", 1, 2, 0, },
	{ CLK_UPLL_320, "clk_upll_320", "clk_upll", 1, 2, 0, },
	{ CLK_UPLL_128, "clk_upll_128", "clk_upll", 1, 5, 0, },
	{ CLK_UPLL_91_4, "clk_upll_91_4", "clk_upll", 1, 7, 0, },
	{ CLK_UPLL_64, "clk_upll_64", "clk_upll", 1, 10, 0, },
	{ CLK_EPLL_200, "clk_epll_200", "clk_epll1", 1, 2, 0, },
	{ CLK_EPLL_100, "clk_epll_100", "clk_epll1", 1, 4, 0, },
	{ CLK_EPLL_50, "clk_epll_50", "clk_epll1", 1, 8, 0, },
	{ CLK_EPLL_25, "clk_epll_25", "clk_epll1", 1, 16, 0, },
	{ CLK_EPLL_20, "clk_epll_20", "clk_epll1", 1, 20, 0, },
	{ CLK_MPLL_273, "clk_mpll_273", "clk_mpll1", 1, 2, 0, },
	{ CLK_MPLL_300, "clk_mpll_300", "clk_mpll", 1, 2, 0, },
	{ CLK_AIPLL_500, "clk_aipll_500", "clk_aipll", 1, 2, 0, },
	{ CLK_UPLL_160, "clk_upll_160", "clk_upll", 1, 4, 0, },
};

static const struct mc_gate_clock cen_glb_clks[] = {
	{ CKG_EPHY_REF_EN, "ckg_ephy_ref_en", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x84, 0, 0, },
	{ CKG_GLB_APB_HSIF, "ckg_glb_apb_hsif", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 0, 0, },
	{ CKG_GLB_APB_VOU, "ckg_glb_apb_vou", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 2, 0, },
	{ CKG_GLB_APB_VEU, "ckg_glb_apb_veu", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 3, 0, },
	{ CKG_GLB_APB_AP, "ckg_glb_ap", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 4, 0, },
	{ CKG_GLB_APB_DMC, "ckg_glb_dmc", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 5, 0, },
	{ CKG_GLB_APB_CPU, "ckg_glb_cpu", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 6, 0, },
	{ CKG_GLB_APB_VDU0, "ckg_glb_vdu0", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 7, 0, },
	{ CKG_GLB_APB_VDU1, "ckg_glb_vdu1", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 8, 0, },
	{ CKG_GLB_APB_VDU2, "ckg_glb_vdu2", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x88, 9, 0, },
	{ CKG_APB_JTAG, "ckg_apb_jtag", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x90, 0, 0, },
	{ CKG_APB_ANLG, "ckg_apb_anlg", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x90, 1, 0, },
	{ CKG_APB_PIN, "ckg_apb_pin", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x90, 2, 0, },
	{ CKG_APB_INT_CTL, "ckg_apb_int_ctl", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0x90, 3, 0, },
	{ RG_BIAS_EN, "rg_bias_en", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0xc4, 0, 0, },
	{ RG_BG_EN, "rg_bg_en", "clk_mux_glb_apb", CLK_IGNORE_UNUSED, 0xc4, 1, 0, },
	
};

static const struct mc_gate_clock ahb_peri_ctrl_clks[] = {
	{ CKG_DMA0, "ckg_dma0_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 0, 0, "clk_dma0_en" },
	{ CKG_DMA1, "ckg_dma1_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 1, 0, "clk_dma1_en" },
	{ CKG_BGM, "ckg_bgm_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 2, 0, "clk_bgm_en" },
	{ CKG_VPPU0, "ckg_vppu0_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 6, 0, "clk_vppu0_en" },
	{ CKG_VPPU1, "ckg_vppu1_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 7, 0, "clk_vppu1_en" },
	{ CKG_AHB_AES0, "ckg_ahb_aes0_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 8, 0, "ckg_ahb_aes0_en" },
	{ CKG_VPPU2_EN, "ckg_vppu2_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 9, 0, "clk_vppu2_en" },
	{ CKG_HSLOCK, "ckg_hslock_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 10, 0, "clk_hsl_en" },
	{ CKG_AHB_BGM, "ckg_ahb_bgm_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 11, 0, "clk_ahb_bgm_en" },
	{ CKG_AHB_VPPU0, "ckg_ahb_vppu0_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 15, 0, "clk_ahb_vppu0_en" },
	{ CKG_AHB_VPPU1, "ckg_ahb_vppu1_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 16, 0, "clk_ahb_vppu1_en" },
	{ CKG_AHB_VPPU2, "ckg_ahb_vppu2_en", "clk_ap_sys",
	  CLK_SET_RATE_PARENT, 0x0, 17, 0, "clk_ahb_vppu2_en" },
	{ CKG_AP_APB_ARCH, "ckg_apapb_arch_en", "clk_ap_sys",
	  CLK_IGNORE_UNUSED, 0x0, 21, 0, "apapb_arch_en" },
};

static const struct mc_gate_clock ap_peri_ctrl_clks[] = {
	{ CKG_UART0, "ckg_uart0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 0, 0, "clk_uart0_en" },
	{ CKG_UART1, "ckg_uart1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 1, 0, "clk_uart1_en" },
	{ CKG_UART2, "ckg_uart2_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 2, 0, "clk_uart2_en" },
	{ CKG_UART3, "ckg_uart3_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 3, 0, "clk_uart3_en" },
	{ CKG_IR, "ckg_ir_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 4, 0, "clk_ir_en" },
	{ CKG_I2C0, "ckg_i2c0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 5, 0, "clk_i2c0_en" },
	{ CKG_I2C1, "ckg_i2c1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 6, 0, "clk_i2c1_en" },
	{ CKG_I2S0, "ckg_i2s0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 7, 0, "clk_i2s0_en" },
	{ CKG_I2S1, "ckg_i2s1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 8, 0, "clk_i2s1_en" },
	{ CKG_I2S2, "ckg_i2s2_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 9, 0, "clk_i2s2_en" },
	{ CKG_I2S3, "ckg_i2s3_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 10, 0, "clk_i2s3_en" },
	{ CKG_I2S4, "ckg_i2s4_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 11, 0, "clk_i2s4_en" },
	{ CKG_RTC_TMR0, "ckg_rtc_tmr0_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 12, 0, "clk_rtmr0_en" },
	{ CKG_RTC_TMR1, "ckg_rtc_tmr1_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 13, 0, "clk_rtmr1_en" },
	{ CKG_RTC_TMR2, "ckg_rtc_tmr2_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 14, 0, "clk_rtmr2_en" },
	{ CKG_XTL_TMR0, "ckg_xtl_tmr0_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 15, 0, "clk_xtmr0_en" },
	{ CKG_XTL_TMR1, "ckg_xtl_tmr1_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 16, 0, "clk_xtmr1_en" },
	{ CKG_XTL_TMR2, "ckg_xtl_tmr2_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 17, 0, "clk_xtmr2_en" },
	{ CKG_AP_SYST, "ckg_syst_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x0, 18, 0, "clk_syst_en" },
	{ CKG_IOE, "ckg_ioe_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x0, 19, 0, "clk_ioe_en" },
	{ CKG_WDG0, "ckg_wdt0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 20, 0, "clk_wdt0_en" },
	{ CKG_WDG1, "ckg_wdt1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 21, 0, "clk_wdt1_en" },
	{ CKG_SPI, "ckg_spi_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 22, 0, "clk_spi_en" },
	{ CKG_EFUSE, "ckg_efuse_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 23, 0, "clk_efuse_en" },
	{ CKG_PWM, "ckg_pwm_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 24, 0, "clk_pwm_en" },
	{ CKG_SRC, "ckg_src_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 26, 0, "clk_src_en" },
	{ CKG_RSA, "ckg_rsa_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 27, 0, "clk_rsa_en" },
	{ CKG_HASH, "ckg_hash_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x0, 28, 0, "clk_hash_en" },
	{ CKG_BSM_TMR, "ckg_bsm_tmr_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x0, 29, 0, "bsm_tmr_en" },
	{ CKG_APB_UART0, "ckg_apb_uart0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 0, 0, "apb_uart0_en" },
	{ CKG_APB_UART1, "ckg_apb_uart1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 1, 0, "apb_uart1_en" },
	{ CKG_APB_UART2, "ckg_apb_uart2_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 2, 0, "apb_uart2_en" },
	{ CKG_APB_UART3, "ckg_apb_uart3_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 3, 0, "apb_uart3_en" },
	{ CKG_APB_IR, "ckg_apb_ir_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 4, 0, "apb_ir_en" },
	{ CKG_APB_I2C0, "ckg_apb_i2c0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 5, 0, "apb_i2c0_en" },
	{ CKG_APB_I2C1, "ckg_apb_i2c1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 6, 0, "apb_i2c1_en" },
	{ CKG_APB_I2S0, "ckg_apb_i2s0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 7, 0, "apb_i2s0_en" },
	{ CKG_APB_I2S1, "ckg_apb_i2s1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 8, 0, "apb_i2s1_en" },
	{ CKG_APB_I2S2, "ckg_apb_i2s2_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 9, 0, "apb_i2s2_en" },
	{ CKG_APB_I2S3, "ckg_apb_i2s3_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 10, 0, "apb_i2s3_en" },
	{ CKG_APB_I2S4, "ckg_apb_i2s4_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 11, 0, "apb_i2s4_en" },
	{ CKG_APB_TMR0, "ckg_apb_tmr0_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 12, 0, "apb_tmr0_en" },
	{ CKG_APB_TMR1, "ckg_apb_tmr1_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 13, 0, "apb_tmr1_en" },
	{ CKG_APB_TMR2, "ckg_apb_tmr2_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 14, 0, "apb_tmr2_en" },
	{ CKG_APB_SYST, "ckg_apb_syst_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 15, 0, "apb_syst_en" },
	{ CKG_APB_RTCT, "ckg_apb_rtct_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 16, 0, "apb_rtct_en" },
	{ CKG_APB_IOE, "ckg_apb_ioe_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x4, 17, 0, "apb_ioe_en" },
	{ CKG_APB_WDG0, "ckg_apb_wdt0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 18, 0, "apb_wdt0_en" },
	{ CKG_APB_WDG1, "ckg_apb_wdt1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 19, 0, "apb_wdt1_en" },
	{ CKG_APB_GPIO, "ckg_apb_gpio_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 20, 0, "apb_gpio_en" },
	{ CKG_APB_SPI, "ckg_apb_spi_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 21, 0, "apb_spi_en" },
	{ CKG_APB_EFUSE0, "ckg_apb_efuse0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 22, 0, "apb_efuse0_en" },
	{ CKG_IOE_PUB, "ckg_ioe_pub_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 23, 0, "ioe_pub_en" },
	{ CKG_IOE_CH0, "ckg_ioe_ch0_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 24, 0, "ioe_ch0_en" },
	{ CKG_IOE_CH1, "ckg_ioe_ch1_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 25, 0, "ioe_ch1_en" },
	{ CKG_APB_PWM, "ckg_apb_pwm_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 26, 0, "apb_pwm_en" },
	{ CKG_APB_SRC, "ckg_apb_src_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x4, 27, 0, "apb_src_en" },
	{ CKG_APB_RSA, "ckg_apb_rsa_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 29, 0, "apb_rsa_en" },
	{ CKG_APB_HASH, "ckg_apb_hash_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x4, 30, 0, "apb_hash_en" },
	{ CKG_I2S0_FAST_EN, "apb_i2s0_fast_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x8, 1, 0, "apb_i2s0_fast_en"},
	{ CKG_I2S1_FAST_EN, "apb_i2s1_fast_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x8, 2, 0, "apb_i2s1_fast_en" },
	{ CKG_I2S2_FAST_EN, "apb_i2s2_fast_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x8, 3, 0, "apb_i2s2_fast_en" },
	{ CKG_I2S3_FAST_EN, "apb_i2s3_fast_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x8, 4, 0, "apb_i2s3_fast_en" },
	{ CKG_I2S4_FAST_EN, "apb_i2s4_fast_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x8, 5, 0, "apb_i2s4_fast_en" },
	{ CKG_I2S0_FRAC_DIV_EN, "i2s0_frac_div_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x8, 6, 0, "i2s0_frac_div_en" },
	{ CKG_I2S1_FRAC_DIV_EN, "i2s1_frac_div_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x8, 7, 0, "i2s1_frac_div_en" },
	{ CKG_I2S2_FRAC_DIV_EN, "i2s2_frac_div_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x8, 8, 0, "i2s2_frac_div_en" },
	{ CKG_I2S3_FRAC_DIV_EN, "i2s3_frac_div_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x8, 9, 0, "i2s3_frac_div_en" },
	{ CKG_I2S4_FRAC_DIV_EN, "i2s4_frac_div_en", "ckg_ap_apb",
	 CLK_SET_RATE_PARENT, 0x8, 10, 0, "i2s4_frac_div_en" },	 
	{ CKG_APB_DEWARP_EN, "ckg_apb_dewap_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x1e0, 0, 0, "ckg_apb_dewap_en" },
	{ CKG_DEWARP_EN, "ckg_dewap_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x1e0, 1, 0, "ckg_dewap_en" },
	{ CKG_DEWARP_MAPC_EN, "ckg_dewap_mapc_en", "ckg_ap_apb",
	  CLK_IGNORE_UNUSED, 0x1e0, 3, 0, "ckg_dewap_mapc_en" },
	{ CKG_APB_KCF_EN, "ckg_apb_kcf_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1e4, 0, 0, "ckg_apb_kcf_en" },
	{ CKG_KCF_EN, "ckg_kcf_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1e4, 1, 0, "ckg_kcf_en" },  
	{ CKG_APB_UART4_EN, "ckg_apb_uart4_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1e8, 0, 0, "ckg_apb_uart4_en" },
	{ CKG_UART4_EN, "ckg_uart4_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1e8, 1, 0, "ckg_uart4_en" },
	{ CKG_APB_I2S5T_EN, "ckg_apb_i2s5_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1ec, 0, 0, "ckg_apb_i2s5_en" }, 
	{ CKG_I2S5_FRAC_DIV_EN, "ckg_i2s5_frac_div_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1ec, 2, 0, "ckg_i2s5_frac_div_en" },
	{ CKG_I2S5_SLOW_EN, "ckg_i2s5_slow_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1ec, 4, 0, "ckg_i2s5_slow_en" }, 
	{ CKG_I2S5_FAST_EN, "ckg_i2s5_fast_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1ec, 5, 0, "ckg_i2s5_fast_en" }, 
	{ CKG_APB_I2S6_EN, "ckg_apb_i2s6_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1f0, 0, 0, "ckg_apb_i2s6_en" }, 
	{ CKG_I2S6_FRAC_DIV_EN, "ckg_i2s6_frac_div_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1f0, 2, 0, "ckg_i2s6_frac_div_en" },
	{ CKG_I2S6_SLOW_EN, "ckg_i2s6_slow_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1f0, 4, 0, "ckg_i2s6_slow_en" }, 
	{ CKG_I2S6_FAST_EN, "ckg_i2s6_fast_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x1f0, 5, 0, "ckg_i2s6_fast_en" }, 
	{ CKG_APB_I2C2_EN, "ckg_apb_i2c2_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x214, 0, 0, "ckg_apb_i2c2_en" },
	{ CKG_I2C2_EN, "ckg_i2c2_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x214, 1, 0, "ckg_i2c2_en" },
	{ CKG_VIRTUALVO_TMR_EN, "ckg_virtualvo_tmr_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x21C, 1, 0, "ckg_virtualvo_tmr_en" },
	{ CKG_APB_I2C3_EN, "ckg_apb_i2c3_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x218, 0, 0, "ckg_apb_i2c3_en" },
	{ CKG_I2C3_EN, "ckg_i2c3_en", "ckg_ap_apb",
	  CLK_SET_RATE_PARENT, 0x218, 1, 0, "ckg_i2c3_en" },
};

static const struct mc_gate_clock hsif_clk_ctrl[] = {
	{ CLK_EMMC_EN, "ckg_emmc_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 18, 0, "clk_emmc_en" },
	{ CLK_EMMC_BASE_EN, "ckg_emmc_base_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 19, 0, "emmc_base_en" },
	{ CLK_EMMC_CQETM_EN, "ckg_emmc_cqetm_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 20, 0, "emmc_cqetm_en" },
	{ CLK_EMMC_TM_EN, "ckg_emmc_tm_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 21, 0, "emmc_tm_en" },
	{ CLK_AXI_EMMC_EN, "ckg_axi_emmc_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 22, 0, "axi_emmc_en" },
	{ CLK_EMMC_PHYCFG_EN, "ckg_emmc_phycfg_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 23, 0, "emmc_phycfg_en" },
	{ CLK_EMMC_2X_EN, "ckg_emmc_2x_en", "hsif_sys_clk",
	  CLK_SET_RATE_PARENT, 0x0, 26, 0, "emmc_2x_en" },  
	{ CLK_AXI_USB20_EN, "ckg_axi_usb20_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x4, 0, 0, "axi_usb20_en" },
	{ CLK_AXI_USB21_EN, "ckg_axi_usb21_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x4, 1, 0, "axi_usb21_en" },
	{ CLK_USB20_REF_EN, "ckg_usb20_ref_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x4, 2, 0, "usb20_ref_en" },
	{ CLK_USB21_REF_EN, "ckg_usb21_ref_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x4, 3, 0, "usb21_ref_en" },
	{ CLK_AHB_PCIE0_CORE_EN, "ckg_ahb_pcie0_core_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x8, 10, 0, "ahb_pcie0_en" },
	{ CLK_AHB_PCIE1_CORE_EN, "ckg_ahb_pcie1_core_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x8, 11, 0, "ahb_pcie0_en" },
	{ CLK_AHB_PCIE_X4_MISC_EN, "ckg_ahb_pcie_x4_misc_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x8, 12, 0, "ahb_pciex4_misc_en" },
	{ CLK_PCIE2_AUX_REF_EN, "ckg_pcie2_aux_ref_en", "hsif_sys_clk",
  	  CLK_IGNORE_UNUSED, 0xc, 3, 0, "pcie2_ref_en" },
	{ CLK_AXI_PCIE2_EN, "ckg_axi_pcie2_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 4, 0, "axi_pcie2_en" },
	{ CLK_AHB_PCIE2_CORE_EN, "ckg_ahb_pcie2_core_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 5, 0, "pcie2_core_en" },
	{ CLK_AHB_PCIEX2_MISC_EN, "ckg_ahb_pciex2_misc_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 6, 0, "pciex2_misc_en" },
	{ CLK_PCIE1_AUX_REF_EN, "ckg_pcie1_aux_ref_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 11, 0, "pcie1_ref_en" },
	{ CLK_AXI_PCIE1_EN, "ckg_axi_pcie1_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 12, 0, "axi_pcie1_en" },
	{ CLK_AHB_PCIE1_X1_CORE_EN, "ahb_pcie1_x1_core_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 13, 0, "ahb_pcie1_x1_core_en" },
	{ CLK_AHB_PCIEX1_MISC_EN, "ckg_ahb_pciex1_misc_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0xc, 14, 0, "pciex1_misc_en" },
	{ CLK_APB_PCIEX2_PHYCFG_EN, "ckg_pciex2_phycfg_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x10, 2, 0, "pciex2_phycfg_en" },
	{ CLK_SATA0_PMALIVE_EN, "ckg_sata0_pmalive_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x14, 6, 0, "sata0_pive_en" },
	{ CLK_SATA0_RXOOB_EN, "ckg_sata0_rx00b_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x14, 7, 0, "sata0_rxoob_en" },
	{ CLK_AXI_SATA0_EN, "ckg_axi_sata0_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x14, 8, 0, "axi_sata0_en" },
	{ CLK_SATA1_PMALIVE_EN, "ckg_sata1_pmalive_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x18, 6, 0, "sata1_plive_en" },
	{ CLK_SATA1_RXOOB_EN, "ckg_sata1_rx00b_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x18, 7, 0, "sata1_rxoob_en" },
	{ CLK_AXI_SATA1_EN, "ckg_axi_sata1_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x18, 8, 0, "axi_sata1_en" },
	{ CLK_SATA2_PMALIVE_EN, "ckg_sata2_pmalive_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x1C, 6, 0, "sata2_plive_en" },
	{ CLK_SATA2_RXOOB_EN, "ckg_sata2_rx00b_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x1C, 7, 0, "sata2_rxoob_en" },
	{ CLK_AXI_SATA2_EN, "ckg_axi_sata2_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x1C, 8, 0, "axi_sata2_en" },
	{ CLK_APB_CMPHY0_EN, "ckg_apb_cmphy0_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x44, 5, 0, "apb_cmphy0_en" },  
	{ CLK_APB_CMPHY1_EN, "ckg_apb_cmphy1_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x44, 6, 0, "apb_cmphy1_en" }, 
	{ CLK_APB_CMPHY2_EN, "ckg_apb_cmphy2_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x44, 7, 0, "apb_cmphy2_en" },
	{ CKG_APB_SFC, "ckg_apb_sfc_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x4C, 0, 0, "apb_sfc_en" },
	{ CKG_SFC, "ckg_sfc_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x4C, 1, 0, "sfc_en" },
    { CLK_AXI_RAIDE_EN, "ckg_axi_raide_en", "hsif_sys_clk",
      CLK_IGNORE_UNUSED, 0x660, 0, 0, "axi_raide_en" },
    {CLK_AFIFOW_EN , "ckg_afifow_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x700, 0, 0, "afifow_en" },
	  
	  
	 #if 0
	{ CLK_SATA3_PMALIVE_EN, "ckg_sata3_pmalive_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x20, 6, 0, "sata3_plive_en" },
	{ CLK_SATA3_RXOOB_EN, "ckg_sata3_rx00b_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x20, 7, 0, "sata3_rxoob_en" },
	{ CLK_AXI_SATA3_EN, "ckg_axi_sata3_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x20, 8, 0, "axi_sata3_en" },
	{ CLK_SATA4_PMALIVE_EN, "ckg_sata4_pmalive_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x24, 6, 0, "sata4_plive_en" },
	{ CLK_SATA4_RXOOB_EN, "ckg_sata4_rx00b_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x24, 7, 0, "sata4_rxoob_en" },
	{ CLK_AXI_SATA4_EN, "ckg_axi_sata4_en", "hsif_sys_clk",
	  CLK_IGNORE_UNUSED, 0x24, 8, 0, "axi_sata4_en" },
	#endif
};

static const char *const
clk_mux_emmc_2x_hsif_p[] = {"clk24m","clk_epll_50","clk_epll_200",""};

static const char *const
clk_mux_emmc_base_hsif_p[] = {"clk24m", "clk_epll_200","clk_mepll_250"};

static const char *const
clk_mux_emmc_cqetm_hsif_p[] = {"rtc32k","clk24m", "clk_epll_50"};

static const char *const
clk_mux_emmc_tm_hsif_p[] = {"rtc32k","clk24m", "clk_epll_50"};

static const char *const
clk_mux_usb20_ref_hsif_p[] = {"rtc32k","clk24m", "clk_epll_20","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_usb21_ref_hsif_p[] = {"rtc32k","clk24m", "clk_epll_20","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_pcie0_ref_hsif_p[] = {"clk_epll_25","clk_epll_50", "clk_epll_100","clk_epll_200","clk_mepll_250"};

static const char *const
clk_mux_pcie1_ref_hsif_p[] = {"clk_epll_25","clk_epll_50", "clk_epll_100","clk_epll_200","clk_mepll_250"};

static const char *const
clk_mux_pcie2_ref_hsif_p[] = {"clk_epll_25","clk_epll_50", "clk_epll_100","clk_epll_200","clk_mepll_250"};

static const char *const
clk_mux_pcie2_gen2_x1_aux_ref_hsif_p[] = {"clk_epll_25","clk_epll_50", "clk_epll_100","clk_epll_200","clk_mepll_250"};

static const char *const
clk_mux_sata0_pmalive_hsif_p[] = {"clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata0_rxoob_hsif_p[] = {"clk24m","clk_epll_25", "clk_epll_50","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata1_pmalive_hsif_p[] = {"clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata1_rxoob_hsif_p[] = {"clk24m","clk_epll_25", "clk_epll_50","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata2_pmalive_hsif_p[] = {"clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata2_rxoob_hsif_p[] = {"clk24m","clk_epll_25", "clk_epll_50","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata3_pmalive_hsif_p[] = {"clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata3_rxoob_hsif_p[] = {"clk24m","clk_epll_25", "clk_epll_50","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata4_pmalive_hsif_p[] = {"clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sata4_rxoob_hsif_p[] = {"clk24m","clk_epll_25", "clk_epll_50","clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_sfc_hsif_p[] = {"clk24m", "clk_upll_91_4","clk_epll_100","clk_epll_200"};


static const struct mc_mux_clock  hisf_mux_clks[] = {
	{ CKG_EMMC_SEL, "clk_emmc_2x_hsif", clk_mux_emmc_2x_hsif_p,
		ARRAY_SIZE(clk_mux_emmc_2x_hsif_p), CLK_SET_RATE_PARENT, 0x0, 0, 2,
		0, 0, "clk_emmc_2x_hsif_sel" },
	{ CKG_EMMC_BASE_SEL, "clk_emmc_base_hsif", clk_mux_emmc_base_hsif_p,
		ARRAY_SIZE(clk_mux_emmc_base_hsif_p), CLK_SET_RATE_PARENT, 0x0, 2, 2,
		0, 0, "clk_emmc_base_hsif_sel" },
	{ CKG_EMMC_CQETM_SEL, "clk_emmc_cqetm_hsif", clk_mux_emmc_cqetm_hsif_p,
		ARRAY_SIZE(clk_mux_emmc_cqetm_hsif_p), CLK_SET_RATE_PARENT, 0x0, 4, 2,
		0, 0, "clk_emmc_cqetm_hsif_sel" },
	{ CKG_EMMC_TM_SEL, "clk_emmc_tm_hsif", clk_mux_emmc_tm_hsif_p,
		ARRAY_SIZE(clk_mux_emmc_tm_hsif_p), CLK_SET_RATE_PARENT, 0x0, 11, 2,
		0, 0, "clk_emmc_tm_hsif_sel" },
	{ CKG_USB20_REF_SEL, "clk_mux_usb20_ref_hsif", clk_mux_usb20_ref_hsif_p,
		ARRAY_SIZE(clk_mux_usb20_ref_hsif_p), CLK_SET_RATE_PARENT, 0x4, 4, 3,
		0, 0, "clk_mux_usb20_ref_hsif_sel" },
	{ CKG_USB21_REF_SEL, "clk_mux_usb21_ref_hsif", clk_mux_usb21_ref_hsif_p,
		ARRAY_SIZE(clk_mux_usb21_ref_hsif_p), CLK_SET_RATE_PARENT, 0x4, 7, 3,
		0, 0, "clk_mux_usb20_ref_hsif_sel" },
	{ CKG_PCIE0_AUX_REF_SEL, "clk_mux_pcie0_aux_hsif", clk_mux_pcie0_ref_hsif_p,
		ARRAY_SIZE(clk_mux_pcie0_ref_hsif_p), CLK_SET_RATE_PARENT, 0x8, 0, 3,
		0, 0, "clk_mux_pcie0_aux_hsif_sel" },
	{ CKG_PCIE1_AUX_REF_SEL, "clk_mux_pcie1_aux_hsif", clk_mux_pcie1_ref_hsif_p,
		ARRAY_SIZE(clk_mux_pcie1_ref_hsif_p), CLK_SET_RATE_PARENT, 0x8, 4, 3,
		0, 0, "clk_mux_pcie1_aux_hsif_sel" },
	{ CKG_PCIE2_AUX_REF_SEL, "clk_mux_pcie2_aux_hsif", clk_mux_pcie2_ref_hsif_p,
		ARRAY_SIZE(clk_mux_pcie2_ref_hsif_p), CLK_SET_RATE_PARENT, 0xC, 0, 3,
		0, 0, "clk_mux_pcie2_aux_hsif_sel" },
	{ CKG_PCIE2_GEN2_X1_AUX_REF_SEL, "clk_mux_pcie2_gen2_x1_aux_hsif", clk_mux_pcie2_gen2_x1_aux_ref_hsif_p,
		ARRAY_SIZE(clk_mux_pcie2_gen2_x1_aux_ref_hsif_p), CLK_SET_RATE_PARENT, 0xC, 8, 3,
		0, 0, "clk_mux_pcie2_aux_hsif_sel" },		
	{ CKG_SATA0_PMALIAVE_SEL, "clk_mux_sata0_pmalive_hsif", clk_mux_sata0_pmalive_hsif_p,
		ARRAY_SIZE(clk_mux_sata0_pmalive_hsif_p), CLK_SET_RATE_PARENT, 0x14, 0, 2,
		0, 0, "clk_mux_sata0_pmalive_hsif_sel" },
	{ CKG_SATA0_RXOOB_SEL, "clk_mux_sata0_rxoob_hsif", clk_mux_sata0_rxoob_hsif_p,
		ARRAY_SIZE(clk_mux_sata0_rxoob_hsif_p), CLK_SET_RATE_PARENT, 0x14, 2, 3,
		0, 0, "clk_mux_sata0_rxoob_hsif_sel" },
	{ CKG_SATA1_PMALIAVE_SEL, "clk_mux_sata1_pmalive_hsif", clk_mux_sata1_pmalive_hsif_p,
		ARRAY_SIZE(clk_mux_sata1_pmalive_hsif_p), CLK_SET_RATE_PARENT, 0x18, 0, 2,
		0, 0, "clk_mux_sata1_pmalive_hsif_sel" },
	{ CKG_SATA1_RXOOB_SEL, "clk_mux_sata1_rxoob_hsif", clk_mux_sata1_rxoob_hsif_p,
		ARRAY_SIZE(clk_mux_sata1_rxoob_hsif_p), CLK_SET_RATE_PARENT, 0x18, 2, 3,
		0, 0, "clk_mux_sata1_rxoob_hsif_sel" },
	{ CKG_SATA2_PMALIAVE_SEL, "clk_mux_sata2_pmalive_hsif", clk_mux_sata2_pmalive_hsif_p,
		ARRAY_SIZE(clk_mux_sata2_pmalive_hsif_p), CLK_SET_RATE_PARENT, 0x1C, 0, 2,
		0, 0, "clk_mux_sata2_pmalive_hsif_sel" },
	{ CKG_SATA2_RXOOB_SEL, "clk_mux_sata2_rxoob_hsif", clk_mux_sata2_rxoob_hsif_p,
		ARRAY_SIZE(clk_mux_sata2_rxoob_hsif_p), CLK_SET_RATE_PARENT, 0x1C, 2, 3,
		0, 0, "clk_mux_sata2_rxoob_hsif_sel" },		
	{ CKG_SATA3_PMALIAVE_SEL, "clk_mux_sata3_pmalive_hsif", clk_mux_sata3_pmalive_hsif_p,
		ARRAY_SIZE(clk_mux_sata3_pmalive_hsif_p), CLK_SET_RATE_PARENT, 0x20, 0, 2,
		0, 0, "clk_mux_sata3_pmalive_hsif_sel" },
	{ CKG_SATA3_RXOOB_SEL, "clk_mux_sata3_rxoob_hsif", clk_mux_sata3_rxoob_hsif_p,
		ARRAY_SIZE(clk_mux_sata3_rxoob_hsif_p), CLK_SET_RATE_PARENT, 0x20, 2, 3,
		0, 0, "clk_mux_sata3_rxoob_hsif_sel" },	
	{ CKG_SATA4_PMALIAVE_SEL, "clk_mux_sata4_pmalive_hsif", clk_mux_sata4_pmalive_hsif_p,
		ARRAY_SIZE(clk_mux_sata4_pmalive_hsif_p), CLK_SET_RATE_PARENT, 0x24, 0, 2,
		0, 0, "clk_mux_sata4_pmalive_hsif_sel" },
	{ CKG_SATA4_RXOOB_SEL, "clk_mux_sata4_rxoob_hsif", clk_mux_sata4_rxoob_hsif_p,
		ARRAY_SIZE(clk_mux_sata4_rxoob_hsif_p), CLK_SET_RATE_PARENT, 0x24, 2, 3,
		0, 0, "clk_mux_sata4_rxoob_hsif_sel" },
	{ CLK_MUX_SFC, "clk_mux_sfc_sel", clk_mux_sfc_hsif_p,
		ARRAY_SIZE(clk_mux_sfc_hsif_p), CLK_SET_RATE_PARENT, 0x4C, 2, 2,
		0, 0, "clk_mux_sfc_sel" },
};



#if 1

static const char *const
clk_mux_vdu0_vdec660_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const char *const
clk_mux_vdu0_vppu860_sys_p[] = {"clk24m","clk_epll1","clk_mpll2","clk_mpll"};

static const char *const
clk_mux_vdu0_vpu_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2","clk_mpll"};

static const char *const
clk_mux_vdu0_g2d860_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const char *const
clk_mux_vdu0_jpg_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_epll"};

static const char *const
clk_mux_vdu1_vdec660_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const char *const
clk_mux_vdu1_vppu860_sys_p[] = {"clk24m","clk_epll1","clk_mpll1","clk_mpll"};

static const char *const
clk_mux_vdu1_vpu_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll1","clk_mpll"};

static const char *const
clk_mux_vdu1_g2d860_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll1"};

static const char *const
clk_mux_vdu1_jpg_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_epll"};

static const char *const
clk_mux_vdu2_vppu860_2_sys_p[] = {"clk24m","clk_epll1","clk_mpll2","clk_mpll"};

static const char *const
clk_mux_vdu2_vppu860_3_sys_p[] = {"clk24m","clk_epll1","clk_mpll2","clk_mpll"};

static const char *const
clk_mux_vdu2_vdec660_2_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const char *const
clk_mux_vdu2_vdec660_3_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const char *const
clk_mux_vdu2_bus_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const char *const
clk_mux_vdu2_bgmcv_sys_p[] = {"clk24m", "clk_epll_100","clk_epll_200","clk_upll_320"};

static const char *const
clk_mux_vdu2_rot_sys_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_mpll2"};

static const struct mc_gate_clock vdu0_clk_ctrl[] = {
	{ CKG_VDU0_VPPU860_EN, "ckg_vdu0_vppu860_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x0, 2, 0, "ckg_vdu0_vppu860_en" },
	{ CKG_VDU0_VDEC660_EN, "ckg_vdu0_vdec0660_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x0, 5, 0, "ckg_vdu0_vdec0660_en" },
	{ CKG_VDU0_APB_PERFMON_EN, "ckg_vdu0_apb_perfmon0_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x0, 7, 0, "ckg_vdu0_apb_perfmon0_en" },
	{ CKG_VDU0_GLB_VDU0_EN, "ckg_glb_vdu0_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x0, 8, 0, "ckg_glb_vdu0_en" }, 
	{ CKG_VDU0_VPU_EN, "ckg_vdu0_vpu_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 2, 0, "ckg_vdu0_vpu_en" },
	{ CKG_VDU0_G2D860_EN, "ckg_vdu0_g2d860_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 8, 0, "ckg_vdu0_g2d860_en" },
	{ CKG_VDU0_JPEG_EN, "ckg_vdu0_jpeg_en", "clk_vdu0_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 11, 0, "ckg_vdu0_jpeg_en" },
	  
};

static const struct mc_mux_clock  vdu0_clk_mux_clks[] = {
	{ CKG_VDU0_VDEC660_SEL, "ckg_vdu0_vdec660_sel", clk_mux_vdu0_vdec660_sys_p,
		ARRAY_SIZE(clk_mux_vdu0_vdec660_sys_p), CLK_SET_RATE_PARENT, 0x74, 4, 2,
		0, 0, "ckg_vdu0_vdec660_sys_sel" },
	{ CKG_VDU0_VPPU860_SEL, "ckg_vdu0_vppu860_sel", clk_mux_vdu0_vppu860_sys_p,
		ARRAY_SIZE(clk_mux_vdu0_vppu860_sys_p), CLK_SET_RATE_PARENT, 0x74, 10, 2,
		0, 0, "ckg_vdu0_vppu860_sys_sel" },	
	{ CKG_VDU0_VPU_SEL, "ckg_vdu0_vpu_sel", clk_mux_vdu0_vpu_sys_p,
		ARRAY_SIZE(clk_mux_vdu0_vpu_sys_p), CLK_SET_RATE_PARENT, 0x74, 16, 3,
		0, 0, "ckg_vdu0_vpu_sys_sel" }, 	
	{ CKG_VDU0_G2D860_SEL, "ckg_vdu0_g2d860_sel", clk_mux_vdu0_g2d860_sys_p,
		ARRAY_SIZE(clk_mux_vdu0_g2d860_sys_p), CLK_SET_RATE_PARENT, 0x74, 23, 2,
		0, 0, "ckg_vdu0_g2d860_sys_sel" }, 			
	{ CKG_VDU0_JPG_SEL, "ckg_vdu0_jpg_sel", clk_mux_vdu0_jpg_sys_p,
		ARRAY_SIZE(clk_mux_vdu0_jpg_sys_p), CLK_SET_RATE_PARENT, 0x74, 29, 2,
		0, 0, "ckg_vdu0_jpg_sys_sel" }, 

};

static const struct mc_gate_clock vdu1_clk_ctrl[] = {
	{ CKG_VDU1_VPPU860_EN, "ckg_vdu1_vppu860_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x0, 2, 0, "ckg_vdu1_vppu860_en" },
	{ CKG_VDU1_VDEC660_EN, "ckg_vdu1_vdec0660_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x0, 5, 0, "ckg_vdu1_vdec0660_en" },
	{ CKG_VDU1_APB_PERFMON_EN, "ckg_vdu1_apb_perfmon0_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x0, 7, 0, "ckg_vdu1_apb_perfmon0_en" },
	{ CKG_VDU1_GLB_VDU1_EN, "ckg_glb_vdu1_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x0, 8, 0, "ckg_glb_vdu1_en" }, 
	{ CKG_VDU1_VPU_EN, "ckg_vdu1_vpu_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 2, 0, "ckg_vdu1_vpu_en" },
	{ CKG_VDU1_G2D860_EN, "ckg_vdu1_g2d860_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 8, 0, "ckg_vdu1_g2d860_en" },
	{ CKG_VDU1_JPEG_EN, "ckg_vdu1_jpeg_en", "clk_vdu1_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 11, 0, "ckg_vdu1_jpeg_en" },	  
};

static const struct mc_mux_clock  vdu1_clk_mux_clks[] = {
	{ CKG_VDU1_VDEC660_SEL, "ckg_vdu1_vdec660_sel", clk_mux_vdu1_vdec660_sys_p,
		ARRAY_SIZE(clk_mux_vdu1_vdec660_sys_p), CLK_SET_RATE_PARENT, 0x74, 4, 2,
		0, 0, "ckg_vdu1_vdec660_sys_sel" },
	{ CKG_VDU1_VPPU860_SEL, "ckg_vdu1_vppu860_sel", clk_mux_vdu1_vppu860_sys_p,
		ARRAY_SIZE(clk_mux_vdu1_vppu860_sys_p), CLK_SET_RATE_PARENT, 0x74, 10, 2,
		0, 0, "ckg_vdu1_vppu860_sys_sel" },	
	{ CKG_VDU1_VPU_SEL, "ckg_vdu1_vpu_sel", clk_mux_vdu1_vpu_sys_p,
		ARRAY_SIZE(clk_mux_vdu1_vpu_sys_p), CLK_SET_RATE_PARENT, 0x74, 16, 3,
		0, 0, "ckg_vdu1_vpu_sys_sel" }, 	
	{ CKG_VDU1_G2D860_SEL, "ckg_vdu1_g2d860_sel", clk_mux_vdu1_g2d860_sys_p,
		ARRAY_SIZE(clk_mux_vdu1_g2d860_sys_p), CLK_SET_RATE_PARENT, 0x74, 23, 2,
		0, 0, "ckg_vdu1_g2d860_sys_sel" }, 			
	{ CKG_VDU1_JPG_SEL, "ckg_vdu1_jpg_sel", clk_mux_vdu1_jpg_sys_p,
		ARRAY_SIZE(clk_mux_vdu1_jpg_sys_p), CLK_SET_RATE_PARENT, 0x74, 29, 2,
		0, 0, "ckg_vdu1_jpg_sys_sel" },  

};

static const struct mc_gate_clock vdu2_clk_ctrl[] = {
	{ CKG_VDU2_VPPU860_2_EN, "ckg_vdu2_vppu860_en", "clk_vdu2_sys",
		 CLK_IGNORE_UNUSED, 0x0, 2, 0, "ckg_vdu2_vppu_en" },
	{ CKG_VDU2_VDEC660_2_EN, "ckg_vdu2_vdec660_2_en", "clk_vdu2_sys",
		 CLK_IGNORE_UNUSED, 0x0, 5, 0, "ckg_vdu2_vdec_2_en" },
	{ CKG_VDU2_VDEC660_3_EN, "ckg_vdu2_vdec660_3_en", "clk_vdu2_sys",
		 CLK_IGNORE_UNUSED, 0x0, 8, 0, "ckg_vdu2_vdec_3_en" },
	{ CKG_VDU2_VPPU860_3_EN, "ckg_vdu2_vppu860_3_en", "clk_vdu2_sys",
		 CLK_IGNORE_UNUSED, 0x0, 11, 0, "ckg_vdu2_vppu_3_en" },
	{ CKG_VDU2_APB_PERFMON_EN, "ckg_apb_perfmon2_en", "clk_vdu2_sys",
		 CLK_IGNORE_UNUSED, 0x0, 13, 0, "ckg_apb_perfmon_en" },	
  	{ CKG_VDU2_ROT_EN, "ckg_vdu2_rot_en", "clk_vdu2_sys",
  	     CLK_IGNORE_UNUSED, 0x34, 2, 0, "ckg_vdu2_rot_en" },
	{ CKG_VDU2_BGMCV_EN, "ckg_vdu2_bgmcv_en", "clk_vdu2_sys",
	     CLK_IGNORE_UNUSED, 0x34, 7, 0, "ckg_vdu2_bgmcv_en" },
	{ CKG_VDU2_BUS_EN, "ckg_vdu2_bus_en", "clk_vdu2_sys",
		 CLK_IGNORE_UNUSED, 0x34, 15, 0, "ckg_vdu2_bus_en" },
};

static const struct mc_mux_clock  vdu2_clk_mux_clks[] = {
	
	{ CKG_VDU2_VPPU860_2_SEL, "ckg_vdu2_vppu860_2_sel", clk_mux_vdu2_vppu860_2_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_vppu860_2_sys_p), CLK_SET_RATE_PARENT, 0x0, 16, 2,
		0, 0, "ckg_vdu2_vppu860_2_sys_sel" },	
	{ CKG_VDU2_VPPU860_3_SEL, "ckg_vdu2_vppu860_3_sys_sel", clk_mux_vdu2_vppu860_3_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_vppu860_3_sys_p), CLK_SET_RATE_PARENT, 0x0, 22, 2,
		0, 0, "ckg_vdu2_vppu860_3_sys_sel" },	
	{ CKG_VDU2_VDEC660_2_SEL, "ckg_vdu2_vdec660_2_sel", clk_mux_vdu2_vdec660_2_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_vdec660_2_sys_p), CLK_SET_RATE_PARENT, 0x8c, 4, 2,
		0, 0, "ckg_vdu2_vdec660_2_sys_sel" },
	{ CKG_VDU2_VDEC660_3_SEL, "ckg_vdu2_vdec660_3_sel", clk_mux_vdu2_vdec660_3_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_vdec660_3_sys_p), CLK_SET_RATE_PARENT, 0x8c, 10, 2,
		0, 0, "ckg_vdu2_vdec660_3_sys_sel" },
	{ CKG_VDU2_BGMCV_SEL, "ckg_vdu2_bgmcv_sel", clk_mux_vdu2_bgmcv_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_bgmcv_sys_p), CLK_SET_RATE_PARENT, 0x90, 11, 2,
		0, 0, "ckg_vdu2_bgmcv_sys_sel" },				
	{ CKG_VDU2_ROT_SEL, "ckg_vdu2_rot_sel", clk_mux_vdu2_rot_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_rot_sys_p), CLK_SET_RATE_PARENT, 0x90, 4, 2,
		0, 0, "ckg_vdu2_rot_sys_sel" },  
	{ CKG_VDU2_BUS_SEL, "ckg_vdu2_bus_sel", clk_mux_vdu2_bus_sys_p,
		ARRAY_SIZE(clk_mux_vdu2_bus_sys_p), CLK_SET_RATE_PARENT, 0x90, 13, 2,
		0, 0, "ckg_vdu2_bus_sys_sel" }, 	

};

//FY13 without vdu3
#if 0
static const struct mc_gate_clock vdu3_clk_ctrl[] = {
	{ CKG_VDU3_VPPU860_EN, "ckg_vdu3_vppu860_en", "clk_vdu3_sys",
		CLK_IGNORE_UNUSED, 0x0, 2, 0, "ckg_vdu3_vppu_en" },
	{ CKG_VDU3_VDEC660_0_EN, "ckg_vdu3_vdec660_0_en", "clk_vdu3_sys",
		CLK_IGNORE_UNUSED, 0x0, 5, 0, "ckg_vdu3_vdec_0_en" },
	{ CKG_VDU3_VDEC660_1_EN, "ckg_vdu3_vdec660_1_en", "clk_vdu3_sys",
		 CLK_IGNORE_UNUSED, 0x0, 8, 0, "ckg_vdu3_vdec_1_en" },
	{ CKG_VDU3_G2D860_EN, "ckg_vdu3_g2d860_en", "clk_vdu3_sys",
		CLK_IGNORE_UNUSED, 0x0, 11, 0, "ckg_vdu3_g2d_en" },

};


static const struct mc_mux_clock  vdu3_clk_mux_clks[] = {

	{ CKG_VDU3_SYS_SEL, "clk_mux_vdu3_sys", clk_mux_vdu3_sys_p,
		ARRAY_SIZE(clk_mux_vdu3_sys_p), CLK_SET_RATE_PARENT, 0x0, 23, 2,
		0, 0, "clk_mux_vdu3_sys_sel" },

};
#endif

static const char *const
//clk_mux_veu_sys_p[] = {"clk24m", "clk_epll_100","clk_epll1","clk_upll","clk_veupll"};
clk_mux_veu_sys_p[] = {"clk24m", "clk_epll_100","clk_epll1","clk_upll"};

static const char *const
clk_mux_veu0_sys_p[] = {"clk24m", "clk_epll","clk_mpll1","clk_upll","clk_veupll"};

static const char *const
clk_mux_veu1_sys_p[] = {"clk24m", "clk_epll","clk_mpll1","clk_upll","clk_veupll"};

static const char *const
clk_mux_ahb_veu_sys_p[] = {"clk24m", "clk_epll_100","clk_upll_320","clk_epll1"};

static const char *const
clk_mux_gmac_sys_p[] = {"clk24m", "clk_epll_200","clk_mpll_273","clk_upll_320"};

static const char *const
clk_mux_gmac2_sys_p[] = {"clk24m", "clk_epll_200","clk_mpll_273","clk_upll_320"};


static const struct mc_mux_clock  veu_clk_mux_clks[] = {

	{ CKG_VEU_SYS_SEL, "clk_mux_veu_sys", clk_mux_veu_sys_p,
		ARRAY_SIZE(clk_mux_veu_sys_p), CLK_SET_RATE_PARENT, 0x0, 5, 2,
		0, 0, "clk_mux_veu_sys_sel" },
	{ CKG_AHB_VEU_SYS_SEL, "clk_mux_ahb_veu_sys", clk_mux_ahb_veu_sys_p,
		ARRAY_SIZE(clk_mux_ahb_veu_sys_p), CLK_SET_RATE_PARENT, 0x0, 12, 2,
		0, 0, "clk_mux_ahb_veu_sys_sel" },		
	{ CKG_VEU1_SEL, "clk_mux_veu1_sys", clk_mux_veu1_sys_p,
		ARRAY_SIZE(clk_mux_veu1_sys_p), CLK_SET_RATE_PARENT, 0x150, 5, 3,
		0, 0, "clk_mux_veu0_sys_sel" },
	{ CKG_VEU0_SEL, "clk_mux_veu0_sys", clk_mux_veu0_sys_p,
		ARRAY_SIZE(clk_mux_veu0_sys_p), CLK_SET_RATE_PARENT, 0x150, 13, 3,
		0, 0, "clk_mux_veu0_sys_sel" },
	{ CKG_AXI_GMAC_SEL, "clk_gmac_mux_sel", clk_mux_gmac_sys_p,
		ARRAY_SIZE(clk_mux_gmac_sys_p), CLK_SET_RATE_PARENT, 0x158, 1, 2,
		0, 0, "clk_axi_gmac_mux_sel" },

};

static const struct mc_gate_clock vio_clk_ctrl[] = {
	{ CKG_CVBS_PIXEL, "ckg_cvbs_pixel_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 4, 0, "clk_cvbs_p_en" },
	{ CKG_CVBS_VDAC, "ckg_cvbs_vdac_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 8, 0, "clk_cvbs_en" },
	{ VOU_CKG_BUSMON_TIMER, "ckg_vou_busmon_timer_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 12, 0, "ckg_busmon_timer_en" },
	{ CKG_AXI_USB3_EN, "ckg_axi_usb3_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 13, 0, "ckg_axi_usb3_r_en" },
	{ CKG_AXI_VOU_EN, "ckg_axi_vou_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 16, 0, "clk_axi_vou_r_en" },
	{ CKG_USB_SUSPEND_EN, "ckg_usb3_suspend_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 19, 0, "ckg_usb3_s_en" },
	{ CKG_USB3_REF_EN, "ckg_usb3_ref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x0, 23, 0, "ckg_usb3_r_en" },
	{ CKG_APB_HDMI0_EN, "ckg_apb_hdmi0_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 0, 0, "ckg_ap_hdmi0_en" },
	{ CKG_APB_HDMI1_EN, "ckg_apb_hdmi1_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 1, 0, "ckg_ap_hdmi1_en" },
	{ CKG_VOU_MTX_EN, "ckg_vou_mtx_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 2, 0, "ckg_vou_mt_en" },
	{ CKG_VOU_EN, "ckg_vou_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 7, 0, "ckg_vou_r_en" },
	{ CLK_AXI_TRACE_EN, "ckg_axi_trace_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 8, 0, "ckg_axi_trace_r_en" },
	{ CLK_USB3_EN, "ckg_usb3_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x4, 10, 0, "ckg_usb3_r_en" },
	{ CLK_USB3_PIPE3_TRACE_EN, "ckg_usb3_pipe3_trace_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 11, 0, "ckg_usb3_pipe3_trace_r_en" },
	{ CKG_HDMI0_I2S_EN, "ckg_hdmi0_i2s_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x4, 15, 0, "ckg_hdmi0_i2s_r_en" },
	{ CKG_HDMI1_I2S_EN, "ckg_hdmi1_i2s_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x4, 18, 0, "ckg_hdmi1_i2s_r_en" },
	{ TIMER2_EN, "ckg_vou_timer2_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 21, 0, "ckg_vou_timer2_r_en" },
	{ TIMER1_EN, "ckg_vou_timer1_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 22, 0, "ckg_vou_timer1_r_en" },
	{ CKG_APB_HDMI0_PHY_EN, "ckg_apb_hdim0_phy_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 24, 0, "ckg_apb_hdim0_phy_r_en" },
	{ CKG_APB_HDMI1_PHY_EN, "ckg_apb_hdim1_phy_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 25, 0, "ckg_apb_hdim1_phy_r_en" },
	{ CKG_APB_USB3_CMPHY_EN, "ckg_apb_usb3_cmphy_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 26, 0, "ckg_apb_usb3_cmphy_r_en" },
	{ CKG_HDMI0_TCLK_TRACE_EN, "ckg_hdmi0_tclk_trace_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 27, 0, "ckg_hdmi0_tclk_trace_r_en" },
	{ CKG_HDMI1_TCLK_TRACE_EN, "ckg_hdmi1_tclk_trace_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x4, 28, 0, "ckg_hdmi1_tclk_trace_r_en" },
	{ CKG_HDMI0_PIXEL_VOU_DIV_EN, "ckg_hdmi0_pixel_vou_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x8, 15, 0, "ckg_hdmi0_pixel_vou_div_r_en" },
	{ CKG_VGA0_VDAC_RAMP_EN, "ckg_vga0_vdac_ramp_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x8, 16, 0, "ckg_vga0_vdac_ramp_r_en" },
	{ CKG_HDMI0_TCLK_DIV_EN, "ckg_hdmi0_tclk_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x8, 19, 0, "ckg_hdmi0_tclk_div_r_en" },
	{ CKG_IIPI_HDMI0_PIXEL_DIV_EN, "ckg_iipi_hdmi0_pixel_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x8, 23, 0, "ckg_iipi_hdmi0_pixel_div_r_en" },
	{ CKG_IVIDP_HDMI0_PIXEL_DIV_EN, "ckg_ividp_hdmi0_pixel_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x8, 27, 0, "ckg_ividp_hdmi0_pixel_r_en" },  
	{ CKG_HDMI0_TCLK_EN, "ckg_hdmi0_tclk_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x8, 31, 0, "ckg_hdmi0_tclk_en" },
	{ CKG_HDMI1_PIXEL_VOU_DIV_EN, "ckg_hdmi1_pixel_vou_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0xC, 15, 0, "ckg_hdmi1_pixel_vou_div_r_en" },
	{ CKG_HDMI1_TCLK_DIV_EN, "ckg_hdmi1_tclk_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0xc, 19, 0, "ckg_hdmi1_tclk_div_r_en" },
	{ CKG_IIPI_HDMI1_PIXEL_DIV_EN, "ckg_iipi_hdmi1_pixel_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0xc, 23, 0, "ckg_iipi_hdmi1_pixel_div_r_en" },
	{ CKG_IVIDP_HDMI1_PIXEL_DIV_EN, "ckg_ividp_hdmi1_pixel_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0xc, 27, 0, "ckg_ividp_hdmi1_pixel_r_en" },
	{ CKG_HDMI1_TCLK_EN, "ckg_hdmi1_tclk_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0xc, 31, 0, "ckg_hdmi1_tclk_en" },  
	{ CKG_CVBS_VDAC_VDAC_RAMP_EN, "ckg_cvbs_vdac_vdac_ramp_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x10, 16, 0, "ckg_cvbs_vdac_vdac_ramp_en" },
	{ CKG_DBG_RG_CVBS_VDAC_RG_RBIAS_EN, "ckg_dbg_rg_cvbs_vdac_rg_rbias_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x28, 23, 0, "ckg_dbg_rg_cvbs_vdac_rg_rbias_en" },
	{ CKG_DBG_DA_CVBS_CABLE_S_EN, "ckg_dbg_da_cvbs_cable_s_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x28, 30, 0, "ckg_dbg_da_cvbs_cable_s_en" },
	{ CKG_DBG_DA_CVBS_CABLE_D_EN, "ckg_dbg_da_cvbs_cable_d_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x28, 31, 0, "ckg_dbg_da_cvbs_cable_d_en" },
	{ CKG_DBG_RG_CVBS_VDAC_OVP_EN, "ckg_dbg_rg_cvbs_vdac_ovp_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 9, 0, "ckg_dbg_rg_cvbs_vdac_ovp_en" },
	{ CKG_VDAC_RAMP_GEN_OUT_EN, "ckg_vdac_ramp_gen_out_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x2C, 11, 0, "ckg_vdac_ramp_gen_out_en" },
	{ CKG_DBG_RG_VGA_VDAC_BG_RBIAS_EN, "ckg_dbg_rg_vga_vdac_bg_rbias_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x34, 23, 0, "ckg_dbg_rg_vga_vdac_bg_rbias_en" },
	{ CKG_VGA_VDAC_RAMP_GEN_OUT_EN, "ckg_vga_vdac_ramp_gen_out_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x3C, 16, 0, "ckg_vga_vdac_ramp_gen_out_en" },
	{ CKG_VPLL_108M_VOU_FORCE_EN, "ckg_vpll_108m_force_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x94, 0, 0, "ckg_vpll_108m_force_en" },
	{ CKG_RW_USB3_PME_EN, "ckg_rw_usb3_pme_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0xa4, 30, 0, "ckg_rw_usb3_pme_en" },
	{ CKG_HDMI0_IAUD_DIV_EN, "ckg_hdmi0_iaud_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x11C, 23, 0, "ckg_hdmi0_iaud_div_r_en" },
	{ CKG_HDMI0_IAUD_EN, "ckg_hdmi0_iaud_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x11C, 24, 0, "ckg_hdmi0_iaud_r_en" },
	{ CKG_HDMI0_IREF_EN, "ckg_hdmi0_iref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x11C, 27, 0, "ckg_hdmi0_iref_r_en" },
	{ CKG_HDMI1_IAUD_DIV_EN, "ckg_hdmi1_iaud_div_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x124, 23, 0, "ckg_hdmi1_iaud_div_r_en" },
	{ CKG_HDMI1_IAUD_EN, "ckg_hdmi1_iaud_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x124, 24, 0, "ckg_hdmi1_iaud_r_en" },
	{ CKG_HDMI1_IREF_EN, "ckg_hdmi1_iref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x124, 27, 0, "ckg_hdmi1_iref_r_en" },
	{ CKG_USB3_1_SUSPEND_EN, "ckg_usb3_1_suspend_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x330, 7, 0, "ckg_usb3_1_suspend_en" }, 
	{ CKG_USB3_1_REF_EN, "ckg_usb3_1_ref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x330, 11, 0, "ckg_usb3_1_ref_en" }, 
	{ CKG_AXI_VIU0_EN, "ckg_axi_viu0__en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x330, 19, 0, "ckg_axi_viu0__en" },
	{ CKG_AXI_VIU1_EN, "ckg_axi_viu1__en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x330, 27, 0, "ckg_axi_viu1__en" },
	{ CKG_AXI_VIU2_EN, "ckg_axi_viu2__en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 7, 0, "ckg_axi_viu2__en" },
	{ CKG_AXI_VIU3_EN, "ckg_axi_viu3__en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 15, 0, "ckg_axi_viu3__en" },
	{ CKG_VIU0_DLL_REF_EN, "ckg_viu0_dll_ref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 16, 0, "ckg_viu0_dll_ref_en" },
	{ CKG_VIU1_DLL_REF_EN, "ckg_viu1_dll_ref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 17, 0, "ckg_viu1_dll_ref_en" },
	{ CKG_VIU2_DLL_REF_EN, "ckg_viu2_dll_ref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 18, 0, "ckg_viu2_dll_ref_en" },
	{ CKG_VIU3_DLL_REF_EN, "ckg_viu3_dll_ref_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 19, 0, "ckg_viu3_dll_ref_en" },
	{ CKG_VI_ADC_REF0_EN, "ckg_vi_adc_ref0_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 21, 0, "ckg_vi_adc_ref0_en" },
	{ CKG_VI_ADC_REF1_EN, "ckg_vi_adc_ref1_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 23, 0, "ckg_vi_adc_ref1_en" },
	{ CKG_VI_ADC_REF2_EN, "ckg_vi_adc_ref2_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 25, 0, "ckg_vi_adc_ref2_en" },
	{ CKG_VI_ADC_REF3_EN, "ckg_vi_adc_ref3_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 27, 0, "ckg_vi_adc_ref3_en" },
	{ CKG_DLLREF_GMAC2_EN, "ckg_dllref_gmac2_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 28, 0, "ckg_dllref_gmac2_en" },  
	{ CKG_DLLREF_GMAC3_EN, "ckg_dllref_gmac3_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 29, 0, "ckg_dllref_gmac3_en" },
	{ CKG_GMAC2_TX_EN, "ckg_gmac2_tx_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 30, 0, "ckg_gmac2_tx_en" },
	{ CKG_GMAC3_TX_EN, "ckg_gmac3_tx_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x334, 31, 0, "ckg_gmac3_tx_en" },
	{ CKG_GMAC2_RX_EN, "ckg_gmac2_rx_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x338, 0, 0, "ckg_gmac2_rx_en" },
	{ CKG_GMAC3_RX_EN, "ckg_gmac3_rx_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x338, 1, 0, "ckg_gmac3_rx_en" },
	{ CKG_GMAC2_RMII_EN, "ckg_gmac2_rii_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x338, 2, 0, "ckg_gmac2_rii_en" },	
	{ CKG_GMAC3_RMII_EN, "ckg_gmac3_rii_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 3, 0, "ckg_gmac3_rii_en" },
	{ CKG_GMAC2_CSR_EN, "ckg_gmac2_csr_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 4, 0, "ckg_gmac2_csr_en" },
	{ CKG_GMAC3_CSR_EN, "ckg_gmac3_csr_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 5, 0, "ckg_gmac3_csr_en" },
	{ CKG_AXI_GMAC2_EN, "ckg_axi_gmac2_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 6, 0, "ckg_axi_gmac2_en" },
	{ CKG_AXI_GMAC3_EN, "ckg_axi_gmac3_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 7, 0, "ckg_axi_gmac3_en" },
	{ CKG_APB_USB3_1_CMPHY_EN, "ckg_apb_usb3_1_cmphy_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 8, 0, "ckg_apb_usb3_1_cmphy_en" },
	{ CKG_APB_VIU0_EN, "ckg_apb_viu0_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 9, 0, "ckg_apb_viu0_en" },
	{ CKG_APB_VIU1_EN, "ckg_apb_viu1_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 10, 0, "ckg_apb_viu1_en" },
	{ CKG_APB_VIU2_EN, "ckg_apb_viu2_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 11, 0, "ckg_apb_viu2_en" },
	{ CKG_APB_VIU3_EN, "ckg_apb_viu3_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 12, 0, "ckg_apb_viu3_en" },
	{ CKG_APB_PERFMON_VOU_MTX_EN, "ckg_apb_perfmon_vou_mtx_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 13, 0, "ckg_apb_perfmon_vou_mtx_en" },
	{ CKG_APB_PERFMON_USB3_MTX_EN, "ckg_apb_perfmon_usb3_mtx_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 14, 0, "ckg_apb_perfmon_usb3_mtx_en" },
	{ CKG_USB3_1_EN, "ckg_usb3_1_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 15, 0, "ckg_usb3_1_en" },
	{ CKG_USB3_1_PIPE3_TRACE_EN, "ckg_usb3_1_pipe3_trace_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 16, 0, "ckg_usb3_1_pipe3_trace_en" },
	{ CKG_VICASC_DLL_REF_EN, "ckg_vicasc_dll_ref_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 17, 0, "ckg_vicasc_dll_ref_en" },
	{ CKG_AXI_GMAC_EN, "ckg_axi_gmac_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 19, 0, "ckg_axi_gmac_en" }, 
	{ CKG_VIU0_EN, "ckg_viu0_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 23, 0, "ckg_viu0_en" },   
	{ CKG_VIU1_EN, "ckg_viu1_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 24, 0, "ckg_viu1_en" },
    { CKG_VIU2_EN, "ckg_viu2_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 25, 0, "ckg_viu2_en" },   
    { CKG_VIU3_EN, "ckg_viu3_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x338, 26, 0, "ckg_viu3_en" }, 
	{ CKG_RGMII2_DATA_SWAP_EN, "ckg_rgmii2_data_swap_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x384, 14, 0, "ckg_rgmii2_data_swap_en" },
	{ CKG_RGMII3_DATA_SWAP_EN, "ckg_rgmii3_data_swap_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x384, 15, 0, "ckg_rgmii3_data_swap_en" },
	{ CKG_GMAC2_RMII_OUT_INV_EN, "ckg_gmac2_rmii_out_inv_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x388, 5, 0, "ckg_gmac2_rmii_out_inv_en" },
	{ CKG_GMAC2_MDCK_OUT_INV_EN, "ckg_gmac2_mdck_out_inv_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x388, 6, 0, "ckg_gmac2_mdck_out_inv_en" },
	{ CKG_GMAC2_TX_IN_INV_EN, "ckg_gmac2_tx_in_inv_en", "clk_vio_sys",
      CLK_IGNORE_UNUSED, 0x388, 7, 0, "ckg_gmac2_tx_in_inv_en" },
	{ CKG_GMAC3_RMII_OUT_INV_EN, "ckg_gmac3_rmii_out_inv_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x408, 5, 0, "ckg_gmac3_rmii_out_inv_en" },
	{ CKG_GMAC3_MDCK_OUT_INV_EN, "ckg_gmac3_mdck_out_inv_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x408, 6, 0, "ckg_gmac3_mdck_out_inv_en" },
	{ CKG_GMAC3_TX_IN_INV_EN, "ckg_gmac3_tx_in_inv_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x408, 7, 0, "ckg_gmac3_tx_in_inv_en" },
	{ CKG_VIU0_LITE_EN, "ckg_viu0_lite_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 0, 0, "ckg_viu0_lite_en" },
	{ CKG_VIU1_LITE_EN, "ckg_viu1_lite_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 1, 0, "ckg_viu1_lite_en" },
	{ CKG_VIU2_LITE_EN, "ckg_viu2_lite_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 2, 0, "ckg_viu2_lite_en" },
	{ CKG_VIU3_LITE_EN, "ckg_viu3_lite_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 3, 0, "ckg_viu3_lite_en" },  
	{ CKG_AXI_VIU0_FORCE_EN, "ckg_axi_viu0_force_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 4, 0, "ckg_axi_viu0_force_en" },
	{ CKG_AXI_VIU1_FORCE_EN, "ckg_axi_viu1_force_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 5, 0, "ckg_axi_viu1_force_en" },
	{ CKG_AXI_VIU2_FORCE_EN, "ckg_axi_viu2_force_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 6, 0, "ckg_axi_viu2_force_en" },
	{ CKG_AXI_VIU3_FORCE_EN, "ckg_axi_viu3_force_en", "clk_vio_sys",
	  CLK_IGNORE_UNUSED, 0x428, 7, 0, "ckg_axi_viu3_force_en" },
};
	

static const struct mc_gate_clock ai_clk_ctrl[] = {	
	{ CKG_AIPLL1000M_WAIT_FORCE_EN, "aipll1000m_wait_force_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x0, 2, 0, "aipll1000m_wait_force_en" },
	{ CKG_DIV1000M_FORCE_EN, "aipll1000m_force_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x0, 7, 0, "aipll1000m_force_en" },
	{ CKG_AIPLL1000M_AI_FORCE_EN, "aipll1000m_ai_force_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x0, 7, 0, "aipll1000m_ai_force_en" },
	{ CKG_AIPLL500M_AI_FORCE_EN, "aipll500m_ai_force_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x0, 13, 0, "aipll500m_ai_force_en" },
	{ CKG_HKDLA_EN, "ckg_hkdla_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x4, 0, 0, "ckg_hkdla_en" },
	{ CKG_CNN2X_EN, "ckg_cnn2x_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x4, 10, 0, "ckg_cnn2x_en" },
	{ CKG_AXI_HKDLA_EN, "ckg_axi_hkdla_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x4, 17, 0, "ckg_axi_hkdla_en" },
	{ CKG_AXI_CNN_EN, "ckg_axi_cnn_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0x4, 23, 0, "ckg_axi_cnn_en" },
	{ CKG_NNP_EN, "ckg_nnp_en", "clk_ai_sys",
	  CLK_IGNORE_UNUSED, 0x8, 0, 0, "ckg_nnp_en" },
	{ CKG_APB_NNP_EN, "ckg_apb_nnp_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0xC, 2, 0, "ckg_apb_nnp_en" },
	{ CKG_APB_HKDLA_EN, "ckg_apb_hkdla_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0xC, 3, 0, "ckg_apb_hkdla_en" },
	{ CKG_APB_CNN_EN, "ckg_apb_cnn_en", "clk_ai_sys",
	  CLK_SET_RATE_PARENT, 0xC, 4, 0, "ckg_apb_cnn_en" },  
};



static const char *const
	clk_mux_usb3_vou_p[] = {"clk24m","clk_epll_100","clk_upll_160","clk_epll_200"};

static const char *const
	clk_mux_vou_sys_p[] = {"clk24m", "clk_upll_320","clk_epll1","clk_upll"};

static const char *const
	clk_mux_vou_usb3_suspend_p[] = {"rtc32k","clk_epll_20","clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
	clk_mux_vou_usb3_ref_p[] = {"rtc32k","clk_epll_20","clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
	clk_mux_hdmi0_ref_p[] = {"clk24m","clk_epll_100","clk_epll1"};

static const char *const
	clk_mux_hdmi0_iaud_p[] = {"clk24m", "clk_upll_64","clk_epll_200","clk_epll1"};

static const char *const
	clk_axi_viu0_sys_p[] = {"clk24m","clk_epll_200", "clk_mpll_273", "clk_upll_320","clk_epll1"};

static const char *const
	clk_axi_viu1_sys_p[] = {"clk24m","clk_epll_200", "clk_mpll_273", "clk_upll_320","clk_epll1"};

static const char *const
	clk_axi_viu2_sys_p[] = {"clk24m","clk_epll_200", "clk_mpll_273", "clk_upll_320","clk_epll1"};

static const char *const
	clk_axi_viu3_sys_p[] = {"clk24m","clk_epll_200", "clk_mpll_273", "clk_upll_320","clk_epll1"};

static const char *const
	ckg_vi_adc_ref0_sys_p[] = {"clk24m","clk27m"};

static const char *const
	ckg_vi_adc_ref1_sys_p[] = {"clk24m","clk27m"};

static const char *const
	ckg_vi_adc_ref2_sys_p[] = {"clk24m","clk27m"};

static const char *const
	ckg_vi_adc_ref3_sys_p[] = {"clk24m","clk27m"};

static const char *const
	clk_usb3_1_suspend_p[] = {"rtc32k","clk_epll_20","clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
	clk_mux_usb3_1_ref_p[] = {"rtc32k","clk_epll_20","clk24m", "clk_upll_64","clk_epll_100"};


static const char *const
	clk_mux_hdmi1_ref_p[] = {"clk24m","clk_epll_100","clk_epll1"};

static const char *const
	clk_mux_hdmi1_iaud_p[] = {"clk24m", "clk_upll_64","clk_epll_200","clk_epll1"};

static const char *const
	clk_mux_cnn2x_p[] = {"clk24m","clk_epll1","clk_mpll","clk_aipll"};

static const char *const
	clk_mux_hkdla_p[] = {"clk24m", "clk_mpll_300","clk_epll1","clk_aipll_500"};

static const char *const
	clk_mux_axi_hkdla_p[] = {"clk24m", "clk_mpll_300","clk_epll1","clk_aipll_500"};

static const char *const
	clk_mux_axi_cnn_p[] = {"clk24m","clk_epll1","clk_mpll","clk_aipll_500"};

static const char *const
	clk_mux_nnp_p[] = {"clk24m","clk_mpll_300","clk_epll1","clk_aipll_500"};


	





static const struct mc_mux_clock  vio_clk_mux_clks[] = {
	{ CKG_AXI_USB3_SEL, "clk_axi_vou_usb3", clk_mux_usb3_vou_p,
		ARRAY_SIZE(clk_mux_usb3_vou_p), CLK_SET_RATE_PARENT, 0x0, 14, 2,
		0, 0, "clk_axi_vou_usb3_sel" },
	{ CKG_AXI_VOU_SEL, "clk_axi_vou_sys", clk_mux_vou_sys_p,
		ARRAY_SIZE(clk_mux_vou_sys_p), CLK_SET_RATE_PARENT, 0x0, 17, 2,
		0, 0, "clk_axi_vou_sys_sel" },
	{ CKG_AXI_USB3_SUSPEND_SEL, "clk_axi_vou_usb3_suspend", clk_mux_vou_usb3_suspend_p,
		ARRAY_SIZE(clk_mux_vou_usb3_suspend_p), CLK_SET_RATE_PARENT, 0x0, 20, 3,
		0, 0, "clk_axi_vou_usb3_suspend_sel" },
	{ CKG_AXI_USB3_REF_SEL, "clk_axi_vou_usb3_ref", clk_mux_vou_usb3_ref_p,
		ARRAY_SIZE(clk_mux_vou_usb3_ref_p), CLK_SET_RATE_PARENT, 0x0, 24, 3,
		0, 0, "clk_axi_vou_usb3_ref_ref" },
	{ CKG_HDMI0_IAUD_SEL, "clk_hdmi0_iaud_sel", clk_mux_hdmi0_iaud_p,
		ARRAY_SIZE(clk_mux_hdmi0_iaud_p), CLK_SET_RATE_PARENT, 0x11C, 25, 2,
		0, 0, "clk_hdmi0_iaud_r_sel" },
	{ CKG_HDMI0_IREF_SEL, "clk_hdmi0_iref_sel", clk_mux_hdmi0_ref_p,
		ARRAY_SIZE(clk_mux_hdmi0_ref_p), CLK_SET_RATE_PARENT, 0x11C, 28, 2,
		0, 0, "clk_hdmi0_iref_sel" },
	{ CKG_HDMI1_IAUD_SEL, "clk_hdmi1_iaud_sel", clk_mux_hdmi1_iaud_p,
		ARRAY_SIZE(clk_mux_hdmi0_iaud_p), CLK_SET_RATE_PARENT, 0x124, 25, 2,
		0, 0, "clk_hdmi1_iaud_r_sel" },
	{ CKG_HDMI1_IREF_SEL, "clk_hdmi1_iref_sel", clk_mux_hdmi1_ref_p,
		ARRAY_SIZE(clk_mux_hdmi0_ref_p), CLK_SET_RATE_PARENT, 0x124, 28, 2,
		0, 0, "clk_hdmi1_iref_sel" },
	{ CKG_USB3_1_SUSPEND_SEL, "clk_usb3_1_suspend_sel", clk_usb3_1_suspend_p,
		ARRAY_SIZE(clk_usb3_1_suspend_p), CLK_SET_RATE_PARENT, 0x330, 4, 3,
		0, 0, "clk_usb3_1_suspend_sel" },
	{ CKG_USB3_1_REF_SEL, "ckg_usb3_1_ref_sel", clk_mux_usb3_1_ref_p,
		ARRAY_SIZE(clk_mux_usb3_1_ref_p), CLK_SET_RATE_PARENT, 0x330, 8, 3,
		0, 0, "ckg_usb3_1_ref_sel" },
	{ CKG_AXI_VIU0_SEL, "ckg_axi_viu0_sel", clk_axi_viu0_sys_p,
		ARRAY_SIZE(clk_axi_viu0_sys_p), CLK_SET_RATE_PARENT, 0x330, 12, 3,
		0, 0, "ckg_axi_viu0_sel" },
	{ CKG_AXI_VIU1_SEL, "ckg_axi_viu1_sel", clk_axi_viu1_sys_p,
		ARRAY_SIZE(clk_axi_viu1_sys_p), CLK_SET_RATE_PARENT, 0x330, 20, 3,
		0, 0, "ckg_axi_viu1_sel" },
	{ CKG_AXI_VIU2_SEL, "ckg_axi_viu2_sel", clk_axi_viu2_sys_p,
		ARRAY_SIZE(clk_axi_viu2_sys_p), CLK_SET_RATE_PARENT, 0x334, 0, 3,
		0, 0, "ckg_axi_viu2_sel" },
	{ CKG_AXI_VIU3_SEL, "ckg_axi_viu3_sel", clk_axi_viu3_sys_p,
        ARRAY_SIZE(clk_axi_viu3_sys_p), CLK_SET_RATE_PARENT, 0x334, 8, 3,
        0, 0, "ckg_axi_viu3_sel" },
	{ CKG_VI_ADC_REF0_SEL, "ckg_vi_adc_ref0_sel", ckg_vi_adc_ref0_sys_p,
		ARRAY_SIZE(ckg_vi_adc_ref0_sys_p), CLK_SET_RATE_PARENT, 0x334, 20, 1,
		0, 0, "ckg_vi_adc_ref0_sel" },
	{ CKG_VI_ADC_REF1_SEL, "ckg_vi_adc_ref1_sel", ckg_vi_adc_ref1_sys_p,
		ARRAY_SIZE(ckg_vi_adc_ref1_sys_p), CLK_SET_RATE_PARENT, 0x334, 22, 1,
		0, 0, "ckg_vi_adc_ref1_sel" },
	{ CKG_VI_ADC_REF2_SEL, "ckg_vi_adc_ref2_sel", ckg_vi_adc_ref2_sys_p,
		ARRAY_SIZE(ckg_vi_adc_ref2_sys_p), CLK_SET_RATE_PARENT, 0x334, 24, 1,
		0, 0, "ckg_vi_adc_ref2_sel" },
	{ CKG_VI_ADC_REF3_SEL, "ckg_vi_adc_ref3_sel", ckg_vi_adc_ref3_sys_p,
		ARRAY_SIZE(ckg_vi_adc_ref3_sys_p), CLK_SET_RATE_PARENT, 0x334, 26, 1,
		0, 0, "ckg_vi_adc_ref3_sel" },
	{ CKG_AXI_GMAC_SEL2, "clk_gmac_mux_sel2", clk_mux_gmac2_sys_p,
		ARRAY_SIZE(clk_mux_gmac2_sys_p), CLK_SET_RATE_PARENT, 0x338, 20, 2,
		0, 0, "clk_axi_gmac_mux_sel2" },
};


static const struct mc_mux_clock  ai_clk_mux_clks[] = {
	{ CKG_HKDLA_SEL, "clk_hkdla_sel", clk_mux_hkdla_p,
		ARRAY_SIZE(clk_mux_hkdla_p), CLK_SET_RATE_PARENT, 0x4, 5, 2,
		0, 0, "clk_hkdla_sel" },
	{ CKG_CNN2X_SEL, "clk_cnn2x_sel", clk_mux_cnn2x_p,
		ARRAY_SIZE(clk_mux_cnn2x_p), CLK_SET_RATE_PARENT, 0x4, 11, 2,
		0, 0, "clk_cnn2x_sel" },
	{ CKG_AXI_HKDLA_SEL, "clk_axi_hkdla_sel", clk_mux_axi_hkdla_p,
		ARRAY_SIZE(clk_mux_axi_hkdla_p), CLK_SET_RATE_PARENT, 0x4, 18, 2,
		0, 0, "clk_axi_hkdla_sel" },
	{ CKG_AXI_CNN_SEL, "clk_axi_cnn_sel", clk_mux_axi_cnn_p,
		ARRAY_SIZE(clk_mux_axi_cnn_p), CLK_SET_RATE_PARENT, 0x4, 24, 2,
		0, 0, "clk_axi_cnn_sel" },
	{ CKG_NNP_SEL, "clk_nnp_sel", clk_mux_nnp_p,
		ARRAY_SIZE(clk_mux_nnp_p), CLK_SET_RATE_PARENT, 0x8, 5, 3,
		0, 0, "clk_nnp_sel" },
};



#if 0
static const struct mc_gate_clock vpu_clk_ctrl[] =
{
	{ CKG_VPU, "ckg_vpu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 0, 0, "clk_vpu_en" },
	{ CKG_AHB_VPU, "ckg_ahb_vpu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 1, 0, "clk_ahb_vpu_en" },
	{ CKG_AXI_VDMA, "ckg_axi_vdma_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x4, 0, 0, "clk_axi_vdma_en" },
	{ CKG_APB_VDMA, "ckg_apb_vdma_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x4, 1, 0, "clk_apb_vdma_en" },
	{ CKG_VPPU, "ckg_vppu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x8, 0, 0, "clk_vppu_en" },
	{ CKG_BM_TIMER, "ckg_bm_timer_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 3, 0, 0 },
	{ CKG_APB_BM_VDMA, "ckg_apb_bm_vdma_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 4, 0, 0 },
	{ CKG_AXI_BM_VDMA, "ckg_axi_bm_vdma_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 5, 0, 0 },
	{ CKG_APB_BM_VPPU, "ckg_apb_bm_vppu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 6, 0, 0 },
	{ CKG_AXI_BM_VPPU, "ckg_axi_bm_vppu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 7, 0, 0 },
	{ CKG_APB_BM_VPU, "ckg_apb_bm_vpu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 8, 0, 0 },
	{ CKG_AXI_BM_VPU, "ckg_axi_bm_vpu_en", "clk_vpu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 9, 0, 0 },
};
#endif
static const struct mc_gate_clock veu_clk_ctrl[] = {
	{ CKG_AHB_VEU, "clk_ahb_veu_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 0, 0, "clk_ahb_veu_en" }, 
	{ CKG_APB_PERFMON_VEU, "ckg_apb_perfmon_veu", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 1, 0, "ckg_apb_perfmon_veu_en" },
	{ CKG_AXI_PERFMON_VEU, "ckg_axi_perfmon_veu", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 2, 0, "ckg_axi_perfmon_veu_en" },
	{ CKG_BUSMON_TIMER, "ckg_busmon_timer", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 3, 0, "ckg_busmon_timer_en" },
	{ CKG_VEU_EN, "ckg_veu_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x0, 11, 0, "ckg_veu_en" },
	{ CKG_TIMER1, "ckg_timer1", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x1c, 1, 0, "clk_timer1_en" },  
  	{ CKG_DLLREF_GMAC0_EN, "ckg_dllref_gmac0_en", "clk_veu_sys",
      CLK_IGNORE_UNUSED, 0x30, 0, 0, "ckg_dllref_gmac0_en" },
  	{ CKG_DLLREF_GMAC1_EN, "ckg_dllref_gmac1_en", "clk_veu_sys",
      CLK_IGNORE_UNUSED, 0x30, 1, 0, "ckg_dllref_gmac1_en" },
	{ CKG_AXI_GMAC1_EN, "ckg_axi_gmac1_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 2, 0, "veu_ckg_axi_gmac1_en" },
	{ CKG_GMAC0_CSR_EN, "ckg_gmac0_csr_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 3, 0, "veu_ckg_gmac0_csr_en" },
  	{ CKG_GMAC0_RMII_EN, "ckg_gmac0_rmii_en", "clk_veu_sys",
 	  CLK_IGNORE_UNUSED, 0x30, 6, 0, "ckg_gmac0_rmii_en" },  	  
  	{ CKG_GMAC0_RX_EN, "ckg_gmac0_rx_en", "clk_veu_sys",
 	  CLK_IGNORE_UNUSED, 0x30, 7, 0, "ckg_gmac0_rx_en" },   
  	{ CKG_GMAC0_TX_EN, "ckg_gmac0_tx_en", "clk_veu_sys",
 	  CLK_IGNORE_UNUSED, 0x30, 8, 0, "ckg_gmac0_tx_en" }, 
	{ CKG_AXI_GMAC0_EN, "ckg_axi_gmac0_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 9, 0, "veu_ckg_axi_gmac0_en" },
	{ CKG_GMAC1_CSR_EN, "ckg_gmac1_csr_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x30, 10, 0, "veu_ckg_gmac1_csr_en" },  
  	{ CKG_GMAC1_RMII_EN, "ckg_gmac1_rmii_en", "clk_veu_sys",
 	  CLK_IGNORE_UNUSED, 0x30, 13, 0, "ckg_gmac1_rmii_en" },  
  	{ CKG_GMAC1_RX_EN, "ckg_gmac1_rx_en", "clk_veu_sys",
 	  CLK_IGNORE_UNUSED, 0x30, 14, 0, "ckg_gmac1_rx_en" },   
  	{ CKG_GMAC1_TX_EN, "ckg_gmac1_tx_en", "clk_veu_sys",
 	  CLK_IGNORE_UNUSED, 0x30, 15, 0, "ckg_gmac1_tx_en" },
	{ CKG_VEU1_ARCH_EN, "ckg_veu1_arch_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x150, 0, 0, "ckg_veu1_arch_en" }, 	  
	{ CKG_VEU0_ARCH_EN, "ckg_veu0_arch_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x150, 8, 0, "ckg_veu0_arch_en" }, 
	{ CKG_VEU_SYS_MTX_EN, "ckg_veu_sys_mtx_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 0, 0, "ckg_veu_sys_mtx_en" },
	{ CKG_VEU1_EN, "ckg_veu1_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 1, 0, "ckg_veu1_en" },
	{ CKG_VEU0_EN, "ckg_veu0_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 2, 0, "ckg_veu0_en" },  
	{ CKG_APB_PERFMON_GMAC1_MTX_EN, "ckg_apb_perfmon_gmac1_mtx_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 3, 0, "ckg_apb_perfmon_gmac1_mtx_en" },  
	{ CKG_APB_PERFMON_VEU_MTX_EN, "ckg_apb_perfmon_veu_mtx_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 4, 0, "ckg_apb_perfmon_veu_mtx_en" },  
	{ CKG_AHB_VEU1, "ckg_ahb_veu1_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 5, 0, "clk_ahb_veu1_en" }, 
	{ CKG_AHB_VEU0, "ckg_ahb_veu0_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x154, 6, 0, "clk_ahb_veu0_en" },   
	{ CKG_VEU_AXI_GMAC_EN, "ckg_veu_axi_gmac_en", "clk_veu_sys",
	  CLK_IGNORE_UNUSED, 0x158, 0, 0, "ckg_veu_axi_gmac_en" },
};

static const char *const
ckg_ap_apb_arch_p[] = {"clk24m", "clk_upll_64","clk_epll_100"};

static const char *const
clk_mux_spic_ssi_p[] = {"clk24m", "clk_upll_64","clk_upll_91_4"};

static const char *const
clk_mux_efuse_p[] = {"clk24m", "clk_upll_64"};

static const char *const
clk_mux_sfc_p[] = {"clk24m", "clk_upll_64", "clk_upll_91_4"};

static const char *const
clk_mux_spi_p[] = {"clk24m", "clk_upll_64", "clk_upll_91_4"};

static const char *const
clk_mux_syst_p[] = {"rtc32k"};

static const char *const
clk_mux_xtl_tmr0_p[] = {"clk24m"};

static const char *const
clk_mux_rtc_tmr0_p[] = {"rtc32k"};

static const char *const
clk_mux_xtl_tmr1_p[] = {"clk24m"};

static const char *const
clk_mux_rtc_tmr1_p[] = {"rtc32k"};

static const char *const
clk_mux_xtl_tmr2_p[] = {"clk24m"};

static const char *const
clk_mux_rtc_tmr2_p[] = {"rtc32k"};

static const char *const
clk_mux_iis0_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128", "clk_epll_200"};

static const char *const
clk_mux_iis1_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128", "clk_epll_200"};

static const char *const
clk_mux_iis2_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128", "clk_epll_200"};

static const char *const
clk_mux_iis3_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128", "clk_epll_200"};

static const char *const
clk_mux_iis4_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128", "clk_epll_200"};


static const char *const
clk_mux_uart_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_i2c_p[] = {"clk24m", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_i2s_p[] = {"clk24m", "clk_upll_64", "clk_epll_100" , "clk_upll_128" , "clk_epll_200"};

static const char *const
clk_mux_pwm_p[] = {"clk24m", "clk_epll_50", "clk_epll_100"};

static const char *const
clk_mux_vppu_p[] = {"clk24m", "clk_epll_100", "clk_epll1","clk_cpll"};

static const char *const
clk_mux_jpg_p[] = {"clk24m", "clk_epll_100", "clk_epll1","clk_epll2"};

static const char *const
clk_mux_hash_p[] = {"clk24m", "clk_upll_64", "clk_epll_100"};

static const char *const
clk_mux_src_p[] = {"clk24m", "clk_upll_64", "clk_epll_100"};

static const char *const
clk_mux_rsa_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_epll"};

static const char *const
clk_mux_ap_p[] = {"clk24m", "clk_epll_50","clk_epll_100", "clk_epll_200"};

static const char *const
clk_mux_rtc_24m_p[] = {"clk24m"};

static const char *const
clk_mux_dewarp_p[] = {"clk24m", "clk_epll_200" , "clk_upll_320", "clk_epll1", "clk_epll"};

static const char *const
clk_mux_dewarp_mapc_p[] = {"clk24m","clk_epll_50", "clk_epll_100", "clk_epll_200" };

static const char *const
clk_mux_kcf_p[] = {"clk24m", "clk_epll_100", "clk_epll_200","clk_upll_320"};

static const char *const
clk_mux_ioe_p[] = {"clk24m", "clk_mpll_273", "clk_upll_320", "clk_epll1", "clk_epll"};

static const char *const
clk_mux_bgm_p[] = {"clk24m", "clk_mpll_273", "clk_upll_320", "clk_epll1", "clk_epll"};

static const char *const
clk_mux_ive_p[] = {"clk24m", "clk_epll_100", "clk_epll_200","clk_upll_320"};

static const char *const
clk_mux_busmon_p[] = {"clk24m", "clk_upll_64", "clk_epll_100"};

static const char *const
clk_mux_uart0_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_uart1_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_uart2_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_uart3_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_uart4_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_ir_p[] = {"clk24m", "clk_upll_64", "clk_epll_100", "clk_upll_128"};

static const char *const
clk_mux_i2c0_p[] = {"clk24m", "clk_epll_100", "clk_epll_128"};

static const char *const
clk_mux_i2c1_p[] = {"clk24m", "clk_epll_100", "clk_epll_128"};

static const char *const
clk_mux_i2c2_p[] = {"clk24m", "clk_epll_100", "clk_epll_128"};

static const char *const
clk_mux_i2c3_p[] = {"clk24m", "clk_epll_100", "clk_epll_128"};


static const struct mc_mux_clock  ap_sys_clk_mux_clks[] = {
	{CLK_MUX_AP_APB	  ,"ckg_ap_apb"	  ,ckg_ap_apb_arch_p   ,ARRAY_SIZE(ckg_ap_apb_arch_p) , CLK_SET_RATE_PARENT ,0x220,0  ,2  ,0  ,0  ,"clk_ap_apb_sel" },
	{CLK_MUX_SPI      ,"ckg_spi"      ,clk_mux_spi_p       ,ARRAY_SIZE(clk_mux_spi_p)     , CLK_SET_RATE_PARENT ,0x22C,0  ,2  ,0  ,0  ,"ckg_spi_sel"   },
	{CLK_MUX_EFUSE    ,"ckg_efuse"    ,clk_mux_efuse_p     ,ARRAY_SIZE(clk_mux_efuse_p)   , CLK_SET_RATE_PARENT ,0x230,0  ,1  ,0  ,0  ,"ckg_efuse_sel"  },
	{CLK_MUX_SYST     ,"ckg_syst"     ,clk_mux_syst_p      ,ARRAY_SIZE(clk_mux_syst_p)    , CLK_SET_RATE_PARENT ,0x234,0  ,1  ,0  ,0  ,"ckg_syst_sel" },
	{CLK_MUX_XTL_TMR0 ,"ckg_xtl_tmr0" ,clk_mux_xtl_tmr0_p  ,ARRAY_SIZE(clk_mux_xtl_tmr0_p), CLK_SET_RATE_PARENT ,0x238,0  ,1  ,0  ,0  ,"ckg_xtl_tmr0_sel"  },
	{CLK_MUX_RTC_TMR0 ,"ckg_rtc_tmr0" ,clk_mux_rtc_tmr0_p  ,ARRAY_SIZE(clk_mux_rtc_tmr0_p), CLK_SET_RATE_PARENT ,0x23C,0  ,1  ,0  ,0  ,"ckg_rtc_tmr0_sel"  },
	{CLK_MUX_XTL_TMR1 ,"ckg_xtl_tmr1" ,clk_mux_xtl_tmr1_p  ,ARRAY_SIZE(clk_mux_xtl_tmr1_p), CLK_SET_RATE_PARENT ,0x240,0  ,1  ,0  ,0  ,"ckg_xtl_tmr1_sel"  },
	{CLK_MUX_RTC_TMR1 ,"ckg_rtc_tmr1" ,clk_mux_rtc_tmr1_p  ,ARRAY_SIZE(clk_mux_rtc_tmr1_p), CLK_SET_RATE_PARENT ,0x244,0  ,1  ,0  ,0  ,"ckg_rtc_tmr1_sel" },
	{CLK_MUX_XTL_TMR2 ,"ckg_xtl_tmr2" ,clk_mux_xtl_tmr2_p  ,ARRAY_SIZE(clk_mux_xtl_tmr2_p), CLK_SET_RATE_PARENT ,0x248,0  ,1  ,0  ,0  ,"ckg_xtl_tmr2_sel"  },
	{CLK_MUX_RTC_TMR2 ,"ckg_rtc_tmr2" ,clk_mux_rtc_tmr2_p  ,ARRAY_SIZE(clk_mux_rtc_tmr2_p), CLK_SET_RATE_PARENT ,0x24C,0  ,1  ,0  ,0  ,"ckg_rtc_tmr2_sel"  },
	{CLK_MUX_UART0 	  ,"ckg_uart0" 	  ,clk_mux_uart0_p	   ,ARRAY_SIZE(clk_mux_uart0_p)   , CLK_SET_RATE_PARENT ,0x250,0  ,2  ,0  ,0  ,"ckg_uart0_sel"	},
	{CLK_MUX_UART1	  ,"ckg_uart1"	  ,clk_mux_uart1_p	   ,ARRAY_SIZE(clk_mux_uart1_p)   , CLK_SET_RATE_PARENT ,0x254,0  ,2  ,0  ,0  ,"ckg_uart1_sel"	},
	{CLK_MUX_UART2	  ,"ckg_uart2"	  ,clk_mux_uart2_p	   ,ARRAY_SIZE(clk_mux_uart2_p)   , CLK_SET_RATE_PARENT ,0x258,0  ,2  ,0  ,0  ,"ckg_uart2_sel"	},
	{CLK_MUX_UART3	  ,"ckg_uart3"	  ,clk_mux_uart3_p	   ,ARRAY_SIZE(clk_mux_uart3_p)   , CLK_SET_RATE_PARENT ,0x25C,0  ,2  ,0  ,0  ,"ckg_uart3_sel"	},
	{CLK_MUX_IR	  	  ,"ckg_ir"	      ,clk_mux_ir_p	       ,ARRAY_SIZE(clk_mux_ir_p)      , CLK_SET_RATE_PARENT ,0x260,0  ,2  ,0  ,0  ,"ckg_ir_sel"	},
	{CLK_MUX_I2C0	  ,"ckg_i2c0"	  ,clk_mux_i2c0_p	   ,ARRAY_SIZE(clk_mux_i2c0_p)    , CLK_SET_RATE_PARENT ,0x264,0  ,2  ,0  ,0  ,"ckg_i2c0_sel"	},
	{CLK_MUX_I2C1	  ,"ckg_i2c1"	  ,clk_mux_i2c1_p	   ,ARRAY_SIZE(clk_mux_i2c1_p)    , CLK_SET_RATE_PARENT ,0x268,0  ,2  ,0  ,0  ,"ckg_i2c1_sel"	},
	{CLK_MUX_PWM 	  ,"ckg_pwm"      ,clk_mux_pwm_p       ,ARRAY_SIZE(clk_mux_pwm_p)     , CLK_SET_RATE_PARENT ,0x26C,0  ,2  ,0  ,0  ,"ckg_pwm_sel"  },
	{CLK_MUX_HASH	  ,"ckg_hash"	  ,clk_mux_hash_p	   ,ARRAY_SIZE(clk_mux_hash_p) 	  , CLK_SET_RATE_PARENT ,0x270,0  ,2  ,0  ,0  ,"ckg_hash_sel"	 },
	{CLK_MUX_SRC	  ,"ckg_src"	  ,clk_mux_src_p 	   ,ARRAY_SIZE(clk_mux_src_p)	  , CLK_SET_RATE_PARENT ,0x274,0  ,2  ,0  ,0  ,"ckg_src_sel"  },
	{CLK_MUX_RSA	  ,"ckg_rsa"	  ,clk_mux_rsa_p	   ,ARRAY_SIZE(clk_mux_rsa_p)     , CLK_SET_RATE_PARENT ,0x278,0  ,2  ,0  ,0  ,"ckg_rsa_sel"	},
	{CLK_MUX_AP_SYS	  ,"ckg_ap"		  ,clk_mux_ap_p	       ,ARRAY_SIZE(clk_mux_ap_p)      , CLK_SET_RATE_PARENT ,0x27C,0  ,2  ,0  ,0  ,"ckg_ap_sel"	},
	{CLK_MUX_RTC_24M  ,"ckg_rtc_24m"  ,clk_mux_rtc_24m_p   ,ARRAY_SIZE(clk_mux_rtc_24m_p) , CLK_SET_RATE_PARENT ,0x284,0  ,1  ,0  ,0  ,"ckg_rtc_24m_sel" },
	{CLK_DEWARP       ,"ckg_dewarp"   ,clk_mux_dewarp_p    ,ARRAY_SIZE(clk_mux_dewarp_p)  , CLK_IGNORE_UNUSED ,0x288,0  ,3  ,0  ,0  ,"ckg_dewarp_sel" },
	{CLK_KCF          ,"ckg_kcf"      ,clk_mux_kcf_p       ,ARRAY_SIZE(clk_mux_kcf_p)     , CLK_SET_RATE_PARENT ,0x28C,0  ,2  ,0  ,0  ,"ckg_kcf_sel" },
	{CLK_IOE		  ,"ckg_ioe"	  ,clk_mux_ioe_p	   ,ARRAY_SIZE(clk_mux_ioe_p)	  , CLK_SET_RATE_PARENT ,0x290,0  ,3  ,0  ,0  ,"ckg_ioe_sel" },
	{CLK_BGM		  ,"ckg_bgm"	  ,clk_mux_bgm_p	   ,ARRAY_SIZE(clk_mux_bgm_p)	  , CLK_SET_RATE_PARENT ,0x294,0  ,3  ,0  ,0  ,"ckg_bgm_sel" },
	{CLK_IVE		  ,"ckg_ive"	  ,clk_mux_ive_p	   ,ARRAY_SIZE(clk_mux_ive_p)	  , CLK_SET_RATE_PARENT ,0x298,0  ,2  ,0  ,0  ,"ckg_ive_sel" },
	{CLK_MUX_UART4	  ,"ckg_uart4"	  ,clk_mux_uart4_p	   ,ARRAY_SIZE(clk_mux_uart4_p)   , CLK_SET_RATE_PARENT ,0x29C,0  ,2  ,0  ,0  ,"ckg_uart4_sel"	},
	{CLK_MUX_I2C2	  ,"ckg_i2c2"	  ,clk_mux_i2c2_p	   ,ARRAY_SIZE(clk_mux_i2c2_p)    , CLK_SET_RATE_PARENT ,0x2a0,0  ,2  ,0  ,0  ,"ckg_i2c2_sel"	},
	{CLK_MUX_I2C3	  ,"ckg_i2c3"	  ,clk_mux_i2c3_p	   ,ARRAY_SIZE(clk_mux_i2c3_p)	  , CLK_SET_RATE_PARENT ,0x2a4,0  ,2  ,0  ,0  ,"ckg_i2c3_sel"	},
	{CLK_DEWARP_MAPC  ,"ckg_dewarp_mapc",clk_mux_dewarp_mapc_p  ,ARRAY_SIZE(clk_mux_dewarp_mapc_p)  , CLK_IGNORE_UNUSED ,0x2a8,0  ,2  ,0  ,0  ,"ckg_dewarp_mapc_sel" },
	
};

static const char *const
	clk_mux_glb_apb_p[] = {"clk24m", "clk_upll_64","clk_epll_100","clk_epll_200"};

static const char *const
	clk_mux_veu_p[] = {"clk24m","clk_epll_200","clk_epll1","clk_epll","clk_mpll1"};

static const char *const
	clk_mux_glb_vou_p[] = {"clk24m","clk_epll_200","clk_epll1","clk_epll","clk_mpll1"};

static const char *const
	clk_mux_glb_vdu0_p[] = {"clk24m","clk_epll_200","clk_epll1","clk_epll","clk_mpll1"};

static const char *const
	clk_mux_glb_vdu1_cfg_p[] = {"clk24m", "clk_epll_200","clk_epll1","clk_epll","clk_mpll1"};

static const char *const
	clk_glb_ai_cfg_p[] = {"clk24m", "clk_epll1","clk_epll","clk_mpll1","clk_upll"};

static const char *const
	clk_glb_dmc_cfg_p[] = {"clk24m", "clk_epll1","clk_epll","clk_mpll1","clk_upll"};

static const char *const
	clk_mux_pts_p[] = {"clk24m","clk_upll_64"};
	
static const char *const
	clk_mux_ephy_p[] = {"clk_epll_50"};

//static const char *const
//	clk_mux_glb_hsif_p[] = {"clk24m", "clk_epll_100","clk_epll_200","clk_epll1"};

//static const char *const
//	clk_mux_glb_gpu_p[] = {"clk24m", "clk_epll1","clk_epll_200","clk_mpll1"};

static const struct mc_mux_clock  top_clk_mux_clks[] = {
	{ CLK_MUX_PTS, "clk_mux_pts", clk_mux_pts_p,
		ARRAY_SIZE(clk_mux_pts_p), CLK_SET_RATE_PARENT, 0x228, 0, 2,
		0, 0, "clk_mux_pts_sel" },
	{ CLK_MUX_EPHY, "clk_mux_ephy", clk_mux_ephy_p,
		ARRAY_SIZE(clk_mux_ephy_p), CLK_SET_RATE_PARENT, 0x22c, 0, 2,
		0, 0, "clk_mux_ephy_sel" },
	{ CLK_MUX_GLB_APB, "clk_glb_apb", clk_mux_glb_apb_p,
		ARRAY_SIZE(clk_mux_glb_apb_p), CLK_SET_RATE_PARENT, 0x230, 0, 2,
		0, 0, "clk_glb_apb_sel" },
	{ CLK_MUX_VEU, "clk_veu_sys", clk_mux_veu_p,
		ARRAY_SIZE(clk_mux_veu_p), CLK_SET_RATE_PARENT, 0x234, 0, 3,
		0, 0, "clk_veu_sys_sel" },
	{ CLK_MUX_VOU, " ckg_glb_vou_sel", clk_mux_glb_vou_p,
		ARRAY_SIZE(clk_mux_glb_vou_p), CLK_SET_RATE_PARENT, 0x238, 0, 3,
		0, 0, "ckg_glb_vou_sel" },
	{ CLK_MUX_VDU0, " ckg_glb_vdu0_sel", clk_mux_glb_vdu0_p,
		ARRAY_SIZE(clk_mux_glb_vdu0_p), CLK_SET_RATE_PARENT, 0x23C, 0, 3,
		0, 0, "ckg_glb_vdu0_sel" },
	{ CLK_GLB_VDU1_CFG, "clk_glb_vdu1_sel", clk_mux_glb_vdu1_cfg_p,
		ARRAY_SIZE(clk_mux_glb_vdu1_cfg_p), CLK_SET_RATE_PARENT, 0x240, 0, 3,
		0, 0, "clk_glb_vdu1_sys_sel" },
	{ CLK_GLB_AI_CFG, "clk_glb_ai_sel", clk_glb_ai_cfg_p,
		ARRAY_SIZE(clk_glb_ai_cfg_p), CLK_SET_RATE_PARENT, 0x244, 0, 3,
		0, 0, "clk_glb_ai_sys_sel" },
	{ CLK_GLB_DMC_CFG, "clk_glb_dmc_sel", clk_glb_dmc_cfg_p,
		ARRAY_SIZE(clk_glb_dmc_cfg_p), CLK_SET_RATE_PARENT, 0x248, 0, 3,
		0, 0, "clk_glb_dmc_sys_sel" },
	
	#if 0
	{ CLK_MUX_GLB_HSIF, "clk_mux_glb_hsif", clk_mux_glb_hsif_p,
		ARRAY_SIZE(clk_mux_glb_hsif_p), CLK_SET_RATE_PARENT, 0x238, 0, 2,
		0, 0, "clk_mux_gil_hsif_sel" },
	{ CLK_MUX_GLB_GPU, "clk_mux_glb_gpu", clk_mux_glb_gpu_p,
		ARRAY_SIZE(clk_mux_glb_gpu_p), CLK_SET_RATE_PARENT, 0x23C, 0, 2,
		0, 0, "clk_mux_glb_gpu_sel" },
	#endif
	
};

#if 0
static const struct mc_composite top_composite_clks[] = {
	MUX_DIV_FLAGS( CLK_MUX_PTS, "clk_mux_pts", clk_mux_pts_p,
		0x228, 0, 2, 8, 6, CLK_SET_RATE_PARENT ),
	MUX_DIV_FLAGS( CLK_MUX_EPHY, "clk_mux_ephy", clk_mux_ephy_p,
		0x24c, 0, 1, 8, 6, CLK_SET_RATE_PARENT ),
};
#endif
static const struct mc_gate_clock top_composite_clks[] = {
	{ CKG_GLB_VEU_AP, "ckg_glb_veu_ap_en", "clk_top_sys",
  	 CLK_SET_RATE_PARENT, 0x88, 22, 0, "ckg_glb_veu_ap_en" },
};

static const char *const
clk_mux_sata_rxoob_p[] = {"clk24m", "clk_epll_25", "clk_epll_50", "clk_upll_64","clk_epll_100"};
static const char *const
clk_mux_sata_pmalive_p[] = {"clk24m","clk_upll_64","clk_epll_100"};

#if 0
static const char *const
clk_mux_vdsp_uart_p[] = {"clk24m","clk_upll_64","clk_upll_91_4"};

static const struct mc_composite vdsp_uart_composite_clks[] = {
	MUX_DIV_FLAGS(CLK_MUX_VDSP_UART_SEL,"clk_vdsp_uart",clk_mux_vdsp_uart_p,
		0x20, 5, 2, 7, 3, CLK_SET_RATE_PARENT ),
};
#endif
#endif
static struct mc_clock_data *clk_apapb_data;

static void clk_ahb_peri_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE( ahb_peri_ctrl_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}

	mc_clk_register_gate(ahb_peri_ctrl_clks,
				  ARRAY_SIZE( ahb_peri_ctrl_clks),
				  clk_data);
}

static void clk_glb_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(cen_glb_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(cen_glb_clks,
					ARRAY_SIZE(cen_glb_clks),clk_data);
}
#if 1
static void clk_topsys_init(struct device *dev)
{
	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(top_composite_clks) 
			+ ARRAY_SIZE(top_clk_mux_clks);
	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(top_composite_clks,
					ARRAY_SIZE(top_composite_clks),clk_data);		
	mc_clk_register_mux(top_clk_mux_clks,
					ARRAY_SIZE(top_clk_mux_clks), clk_data);
}
#endif
static void clk_apb_init(struct device *dev)
{

	int nr = ARRAY_SIZE(fixed_rate_clks) +
			ARRAY_SIZE(fixed_factor_clks) +
			ARRAY_SIZE(ap_peri_ctrl_clks);
	int i;

	clk_apapb_data = mc_clk_init(dev, nr);
	if (!clk_apapb_data) {
		dev_err(dev, "%s: clk_apapb_data is null\n",__func__);
		return;
	}

	for (i = 0; i < nr; i++)
		clk_apapb_data->clk_data.clks[i] = ERR_PTR(-EPROBE_DEFER);

	mc_clk_register_fixed_rate(fixed_rate_clks,
					 ARRAY_SIZE(fixed_rate_clks),
					 clk_apapb_data);

	mc_clk_register_fixed_factor(fixed_factor_clks,
				 ARRAY_SIZE(fixed_factor_clks),
				 clk_apapb_data);
}

static void clk_ap_peri_init(struct device *dev)
{
	if (!clk_apapb_data) {
		 clk_apb_init(dev);
	}
	if (!clk_apapb_data) {
		dev_err(dev, "%s:clk_apapb_data is null",__func__);
		return;
	}
	mc_clk_register_gate(ap_peri_ctrl_clks,
				  ARRAY_SIZE(ap_peri_ctrl_clks), clk_apapb_data);

}
#if 1
static void clk_apsys_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(ap_sys_clk_mux_clks);/* +
            ARRAY_SIZE(ap_composite_clks);*/

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}

	mc_clk_register_mux(ap_sys_clk_mux_clks,
				 ARRAY_SIZE(ap_sys_clk_mux_clks),clk_data);

   /* mc_clk_register_composites(ap_composite_clks,
                    ARRAY_SIZE(ap_composite_clks),
                    clk_data);*/

}
#endif

static void clk_hsifsys_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(hsif_clk_ctrl) +
				ARRAY_SIZE(hisf_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(hsif_clk_ctrl,
					ARRAY_SIZE(hsif_clk_ctrl),clk_data);
	mc_clk_register_mux(hisf_mux_clks,
					ARRAY_SIZE(hisf_mux_clks),clk_data);
	
	/*mc_clk_register_composites(vdsp_uart_composite_clks,
	  ARRAY_SIZE(vdsp_uart_composite_clks),clk_data);*/

}

#if 1

static void clk_vdu0_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE( vdu0_clk_ctrl)
			+ ARRAY_SIZE(vdu0_clk_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(vdu0_clk_ctrl,
					ARRAY_SIZE(vdu0_clk_ctrl),clk_data);
	mc_clk_register_mux(vdu0_clk_mux_clks,
				 	ARRAY_SIZE(vdu0_clk_mux_clks),clk_data);

}


static void clk_vdu1_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE( vdu1_clk_ctrl)
			+ ARRAY_SIZE(vdu1_clk_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(vdu1_clk_ctrl,
					ARRAY_SIZE(vdu1_clk_ctrl),clk_data);

	mc_clk_register_mux(vdu1_clk_mux_clks,
				 ARRAY_SIZE(vdu1_clk_mux_clks),clk_data);

}


static void clk_vdu2_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE( vdu2_clk_ctrl)
			+ ARRAY_SIZE(vdu2_clk_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(vdu2_clk_ctrl,
					ARRAY_SIZE(vdu2_clk_ctrl),clk_data);
	
	mc_clk_register_mux(vdu2_clk_mux_clks,
				 ARRAY_SIZE(vdu2_clk_mux_clks),clk_data);

}


static void clk_veu_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE( veu_clk_ctrl)+ 
				ARRAY_SIZE(veu_clk_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(veu_clk_ctrl,
					ARRAY_SIZE(veu_clk_ctrl),clk_data);
	mc_clk_register_mux(veu_clk_mux_clks,
			 		ARRAY_SIZE(veu_clk_mux_clks),clk_data);
}

static void clk_vou_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(vio_clk_ctrl)
			+ ARRAY_SIZE(vio_clk_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(vio_clk_ctrl,
					ARRAY_SIZE(vio_clk_ctrl),clk_data);
	mc_clk_register_mux(vio_clk_mux_clks,
			 		ARRAY_SIZE(vio_clk_mux_clks),clk_data);
}

static void clk_ai_init(struct device *dev)
{

	static struct mc_clock_data *clk_data;
	int nr = ARRAY_SIZE(ai_clk_ctrl)
			+ ARRAY_SIZE(ai_clk_mux_clks);

	clk_data = mc_clk_init(dev,nr);
	if (!clk_data) {
		dev_err(dev, "%s:clk_data is null",__func__);
		return;
	}
	mc_clk_register_gate(ai_clk_ctrl,
					ARRAY_SIZE(ai_clk_ctrl),clk_data);
	mc_clk_register_mux(ai_clk_mux_clks,
			 		ARRAY_SIZE(ai_clk_mux_clks),clk_data);
}


#endif

static const struct of_device_id clk_match_table[] = {

	{ .compatible = "apapb-periclk",
	  .data =  clk_ap_peri_init },
	{ .compatible = "apahb-periclk",
	  .data =  clk_ahb_peri_init },
	{ .compatible = "apsys-clk",
	  .data =  clk_apsys_init },
	{ .compatible = "glb-clk",
	  .data =  clk_glb_init },
	{ .compatible = "topclk",
	  .data =  clk_topsys_init },
	{ .compatible = "hsif-clk",
	  .data =  clk_hsifsys_init },
	{ .compatible = "vdu0-sys-clk",
	  .data =  clk_vdu0_init },
	{ .compatible = "vdu1-sys-clk",
	  .data =  clk_vdu1_init },
	{ .compatible = "vdu2-sys-clk",
	  .data =  clk_vdu2_init },
	{ .compatible = "veu-sys-clk",
	  .data =  clk_veu_init },
	{ .compatible = "vou-sys-clk",
	  .data =  clk_vou_init },
	{ .compatible = "ai-sys-clk",
	  .data =  clk_ai_init },
	{ }
};

static int  clk_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	void (*init_func)(struct device *dev);

	init_func = of_device_get_match_data(dev);
	if (!init_func)
		return -ENODEV;

	init_func(dev);
	return 0;
}

static struct platform_driver clk_driver = {
	.probe		 = clk_probe,
	.driver		 = {
		.name  = "fy12-clk",
		.of_match_table = clk_match_table,
	},
};

static int __init clk_init(void)
{
	return platform_driver_register(& clk_driver);
}
core_initcall( clk_init);
