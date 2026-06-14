#ifndef _EFUSE_DRV_H_
#define _EFUSE_DRV_H_

bool secure_enalbe(void);

bool tran_aes_key(void);

unsigned int secure_mode_get(void);

bool header_secure_enable(void);

bool cbc_key_writed(void);

u32 emmc_boot_sel(void);

bool get_aes_iv(u32 *aes_iv);

bool aes_sel(void);

bool rsa_sel(void);

u32 sfc_peri_speed_get(void);


bool sw_bonding_option_init(void);
u32 soc_freq_select(void);
u32 board_voltage_level(void);
u32 emmc_cmd_clk_polarity(void);
u32 emmc_rx_clk_polarity(void);
u32 emmc_tx_clk_polarity(void);
u32 emmc_base_clk_select(void);
u32 emmc_image_select(void);
u32 emmc_baud_rate(void);
u32 emmc_mode_select(void);
bool sdio_sampling_select(void);
bool watchdog_sel(void);
u32 get_wdt_loadvalue(void);

#endif
