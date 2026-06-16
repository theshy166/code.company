#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "efuse.h"
#include "efuse_drv.h"
#include "bond_option.h"

static int secure_block_flag;
static unsigned int secure_block;

bool sw_bonding_option_init(void)
{
	bool ret;
	if (0 == secure_block_flag) {
		ret = read_efuse_entry(&secure_block, 1, SW_BONDING_OPT);
		if ((ret == false)) {
			return false;
		}
		secure_block_flag = 1;
	}
	return true;
}

bool secure_enalbe(void)
{
	bool ret;
	u32 val;

	/*
	 * just keep for V0.1.0, will be removed in next version.
	 * if (!(secure_block & OPT_SECURE))
	 * */
	if (!SW_BOND_SEC_MODE(secure_block))
		return false;

	ret = read_efuse_entry(&val, 1, WRITE_LOCK_H);
	if ((ret == false) || !((val & HASH_WLOCK_BIT_MASK) ==
		HASH_WLOCK_BIT_MASK))
		return false;

	ret = read_efuse_entry(&val, 1, AES_RD_LOCK);
	if ((ret == false) || !((val & HASH_RLOCK_BIT_MASK) ==
		HASH_RLOCK_BIT_MASK))
		return false;

	return true;
}

unsigned int secure_mode_get(void)
{
	u32 secure_mode = 0;

	/*00: no aes no rsa  01: aes only 10:rsa only 11: aes and rsa*/
	/*
	 * just keep for V0.1.0. will be removed in next version.
	 * secure_mode = ((secure_block & (BIT(30) | BIT(31))) >> 30);
	 * */
	secure_mode = SW_BOND_SEC_MODE(secure_block);

	return secure_mode;
}

bool aes_sel(void)
{
	/*
	 * just keep for V0.1.0. will be removed in next version.
	 * if ((secure_block & (BIT(30))) >> 30)
	 * */
	if (secure_block & SW_BOND_AES_BIT)
		return true;

	return false;
}

bool rsa_sel(void)
{
	/*
	 * just keep for V0.1.0. will be removed in next version.
	 * if ((secure_block & (BIT(31))) >> 31)
	 * */
	if (secure_block & SW_BOND_RSA_BIT)
		return true;

	return false;
}

bool header_secure_enable(void)
{
	/*bit29  1: header encript 0: no encript*/

	/*
	 * just keep for V0.1.0. will be removed in next version.
	 * if ((secure_block & BIT(29)) >> 29)
	 * */
	if (secure_block & SW_BOND_HEADER_SEC_BIT)
		return true;

	return false;
}


bool get_aes_iv(u32 *aes_iv)
{
	bool ret;

	ret = read_efuse_entry(aes_iv, 4, AES_IV_DATA);

	return ret;
}

u32 sfc_peri_speed_get(void)
{
	u32 speed_mode = 0;

	/*
	 * just keep for V0.1.0. will be removed in next version.
	 * speed_mode = ((secure_block & (BIT(28) | BIT(27))) >> 28);
	 * */
	speed_mode = SW_BOND_SFC_BAUD(secure_block);

	return speed_mode;
}

u32 soc_freq_select(void)
{
	u32 speed_mode = 0;

	speed_mode = SW_BOND_SOC_FREQ(secure_block);

	return speed_mode;
}

u32 board_voltage_level(void)
{
	u32 level = 0;

	if (secure_block & SW_BOND_EXT_VOLT_BIT) {
		level = 1;
	}

	return level;
}

u32 emmc_cmd_clk_polarity(void)
{
	u32 pol = 0;

	if (secure_block & SW_BOND_EMMC_CMDCLK_POL_BIT) {
		pol = 1;
	}

	return pol;
}

u32 emmc_rx_clk_polarity(void)
{
	u32 pol = 0;

	if (secure_block & SW_BOND_EMMC_RXCLK_POL_BIT) {
		pol = 1;
	}

	return pol;
}

u32 emmc_tx_clk_polarity(void)
{
	u32 pol = 0;

	if (secure_block & SW_BOND_EMMC_TXCLK_POL_BIT) {
		pol = 1;
	}

	return pol;
}

u32 emmc_base_clk_select(void)
{
	u32 base_clk = 0;

	if (secure_block & SW_BOND_EMMC_BASECLK_SEL_BIT) {
		base_clk = 1;
	}

	return base_clk;
}

u32 emmc_image_select(void)
{
	u32 mode = 0;

	mode = SW_BOND_EMMC_IMAGE_SIZE(secure_block);

	return mode;
}

u32 emmc_baud_rate(void)
{
	u32 baud = 0;

	baud = SW_BOND_EMMC_BUAD(secure_block);

	return baud;
}

u32 emmc_mode_select(void)
{
	u32 timeout = 0;

	timeout = SW_BOND_EMMC_TIMEOUT(secure_block);

	return timeout;
}

bool sdio_sampling_select(void)
{
	bool sampling = 0;

	sampling = SW_BOND_SDIO_SAMPLING(secure_block);
	if(sampling){
		return true;
	}else{
		return false;
	}
}

unsigned int if_ip_clock_select(void)
{
	return SW_BOND_IF_CLK_SEL(secure_block);
}

unsigned int emmc_clock_select(void)
{
	return SW_BOND_EMMC_CLK_SEL(secure_block);
}

bool watchdog_sel(void)
{
	if (secure_block & SW_BOND_WATCHDOG_SEL_BIT)
		return true;

	return false;
}

u32 get_wdt_loadvalue(void)
{
	return SW_BOND_WATCHDOG_LOAD_VALUE(secure_block);
}

