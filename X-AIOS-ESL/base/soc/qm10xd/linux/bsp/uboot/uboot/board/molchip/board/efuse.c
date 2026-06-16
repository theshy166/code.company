#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "efuse.h"

void efuse_enable(void)
{
	u32 val, i;

	val = __raw_readl(AP_MTX_CLK_CTRL1);
	val |= CKG_EFUSE_EN;
	__raw_writel(val, AP_MTX_CLK_CTRL1);

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val |= CKG_APB_EFUSE_EN;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);

	/* reset efuse controller */

	val = __raw_readl(AP_PERI_SOFT_RST0);
	val |= EFUSE_SOFT_RST;
	__raw_writel(val, AP_PERI_SOFT_RST0);

	for (i = 0; i < 0x1000; i++)
		;

	val &= ~EFUSE_SOFT_RST;
	__raw_writel(val, AP_PERI_SOFT_RST0);
}

void efuse_ece_enable(u32 base, bool en)
{
	u32 val = 0;

	val = efuse_read(base, EFUSE_CONFIG);
	if (en)
		val |= ECE;
	else
		val &= ~ECE;
	efuse_write(val, base, EFUSE_CONFIG);
}

bool efuse_wait_cmd_complete(u32 base, u32 cmd)
{
	u32 val = 0;
	u32 t1, t2;

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		val = efuse_read(base, EFUSE_STATUS0);
		if (val & cmd)
			break;
	} while ((t2 - t1) < EFUSE_CMD_WAIT_TIME);

	if (val & cmd)
		return true;

	return false;
}

bool efuse_load_user_cmd(u32 base)
{
	bool ret;

	efuse_write(CMD_LOAD_USERCMD, base, EFUSE_CMD);
	ret = efuse_wait_cmd_complete(base, LOAD_USER_CMD_COMPLT);

	return ret;
}

void efuse_set_match_key(u32 base, u32 key)
{
	efuse_write(key, base, EFUSE_MATCH_KEY);
}

bool efuse_read_cmd(u32 base, u32 block)
{
	bool ret;
	u32 val;

	ret = efuse_load_user_cmd(base);
	if (ret == false)
		return false;

	val = CMD_READ;
	block &= ADDR_MASK;
	val |= block << ADDR_SHIFT;
	efuse_write(val, base, EFUSE_CMD);
	ret = efuse_wait_cmd_complete(base, R_COMPLT);

	return ret;
}

bool efuse_wcmd_auto(u32 base)
{
	bool ret;
	u32 val;

	val = CMD_WFLAG_AUTO;
	efuse_write(val, base, EFUSE_CMD);
	ret = efuse_wait_cmd_complete(base, WFLAG_AUTO_COMPLT);

	return ret;
}

bool efuse_write_cmd(u32 base, u32 block, u32 value)
{
	bool ret;
	u32 val;

	ret = efuse_load_user_cmd(base);
	if (ret == false)
		return false;

	if (block < 32) {
		val = efuse_read(base, EFUSE_STATUS7);
		if ((val >> block) & 0x1)
			return false;
	} else if ((block < 57) && (block >= 32)) {
		val = efuse_read(base, EFUSE_STATUS8);
		if ((val >> (block - 32)) & 0x1)
			return false;
	}

	ret = efuse_wcmd_auto(base);
	if (ret == false)
		return false;

	if (block < 32) {
		val = efuse_read(base, EFUSE_STATUS1);
		if (!((val >> block) & 0x1))
			return false;
	} else {
		val = efuse_read(base, EFUSE_STATUS2);
		if (!((val >> (block - 32)) & 0x1))
			return false;
	}

	efuse_write(value, base, EFUSE_WRITE_VALUE);
	val = CMD_WRITE;
	block &= ADDR_MASK;
	val |= block << ADDR_SHIFT;
	efuse_write(val, base, EFUSE_CMD);

	ret = efuse_wait_cmd_complete(base, W_COMPLT);

	return ret;
}

bool efuse_get_lock(u32 base, u32 *lock)
{
	bool ret;

	ret = efuse_load_user_cmd(base);
	if (ret == false)
		return false;

	*lock = (efuse_read(base, EFUSE_STATUS6) & 0x7fff);
//	*lock >>= EFUSE_LOCK_SHIFT;
//	*lock &= EFUSE_LOCK_MASK;

	return true;
}

void efuse_init(void)
{
	efuse_enable();

	efuse_ece_enable(REG_EFUSE_BASE, true);
}

bool efuse_apb_read(u32 base, u32 block, u32 *val)
{
	bool ret;
	u32 lock, mask;
	u32 shift;

	if (block <= USER_BLOCK_MAX_NUM) {
		ret = efuse_get_lock(base, &lock);
		if (ret == false)
			return false;

		shift = block / 4;
		mask = 1 << shift;
		if (lock & mask)
			return false;
	}
	efuse_set_match_key(base, MATCH_KEY);
	ret = efuse_read_cmd(base, block);
	if (ret == false)
		return false;

	*val = efuse_read(base, EFUSE_STATUS4);
	efuse_set_match_key(base, 0);

	return true;
}

bool efuse_xfer_aes_key(u32 base, u32 block, u32 len)
{
	bool ret;
	u32 lock, reg;

	ret = efuse_load_user_cmd(base);
		if (ret == false)
			return false;

	lock = ((efuse_read(base, EFUSE_STATUS0)>>24) & 0xf);
	if (lock != 0xf)
		return false;

	efuse_set_match_key(base, MATCH_KEY);

	switch (len) {
	case 32:
		len = TRANS_AESKEY_32;
		block = (block >> 0);
		break;
	case 64:
		len = TRANS_AESKEY_64;
		block = (block >> 1);
		break;
	case 128:
		len = TRANS_AESKEY_128;
		block = (block >> 2);
		break;
	case 192:
		len = TRANS_AESKEY_192;
		block = (block >> 3);
		break;
	case 256:
		len = TRANS_AESKEY_256;
		block = (block >> 3);
		break;
	default:
		len = TRANS_AESKEY_128;
		block = (block >> 2);
		break;
	}
	reg = efuse_read(base, EFUSE_CONFIG);
	reg &= 0x0007ffff;
	reg |= (len<<19);
	reg |= (block<<24);
	efuse_write(reg, base, EFUSE_CONFIG);

	reg = CMD_TRANS_AESKEY;
	efuse_write(reg, base, EFUSE_CMD);
	ret = efuse_wait_cmd_complete(base, AES_COMPLT);
	if (ret == false)
		return false;

	reg = efuse_read(base, EFUSE_STATUS3);
	if (reg & ((1 << STATUS3_AES_ERR_SHIFT) | STATUS3_CMD_ERR |
		STATUS3_OP_ERR | STATUS3_RMAX_ERR))
		return false;

	return true;
}

bool efuse_compare(u32 *hash)
{
	u32 i, val;
	u32 t1, t2;

	for (i = 0; i < 8; i++)
		__raw_writel(hash[i], (unsigned long)EFUSE_HASH(i));

	val = __raw_readl(EFUSE_AES_CFG);
	val &= 0xfffff000;
	val |= COMPARE_LENGTH(2);
	val |= COMPARE_START_BLOCK(HASH_START_BLOCK);
	__raw_writel(val, EFUSE_AES_CFG);

	efuse_write(CMD_COMPARE, REG_EFUSE_BASE, EFUSE_CMD);

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		val = __raw_readl(EFUSE_HASH_COMPARE);
		if (val & COMPARE_DONE)
			break;
	} while ((t2 - t1) < EFUSE_CMD_WAIT_TIME);
	if ((val & COMPARE_OK) == COMPARE_OK)
		return true;

	return false;
}
