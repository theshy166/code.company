#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "efuse.h"
#include "efuse_drv.h"
#include "emmc.h"

/*
static int secure_block_flag;
static int emmc_flag;
static unsigned int secure_block;
static unsigned int emmc_set;
*/

bool read_efuse_entry(u32 *edata, u32 num, u32 block)
{
	u32 i;
	bool ret;
	u32 base = REG_EFUSE_BASE;

	if (block > EFUSE_MAX_BLOCK)
		return false;

	for (i = 0; i < num; i++) {
		ret = efuse_apb_read(base, block, edata);
		if (ret == false)
			return false;
		block++;
		edata++;
	}

	return true;
}

bool tran_aes_key(void)
{
	bool ret;
	u32 block = CBC_KEY_BLOCK;
	u32 base = REG_EFUSE_BASE;
	u32 val;

	ret = read_efuse_entry(&val, 1, WRITE_LOCK_H);
	if ((ret == false) || !((val & AES_WLOCK_BIT_MASK) ==
		AES_WLOCK_BIT_MASK))
		return false;

	ret = read_efuse_entry(&val, 1, AES_RD_LOCK);
	if ((ret == false) || !((val & AES_RLOCK_BIT_MASK) ==
		AES_RLOCK_BIT_MASK))
		return false;

	ret = efuse_xfer_aes_key(base, block, AES_KEY_LEN);

	return ret;
}


//detect programed
static bool efuse_writed(u32 block, u32 num)
{
	bool ret;
	u32 val1, val2, i, shift;
	u32 base = REG_EFUSE_BASE;

	if (block > EFUSE_MAX_BLOCK)
		return false;

	ret = efuse_wcmd_auto(base);
	if (ret == false)
		return false;

	val1 = efuse_read(base, EFUSE_STATUS1);
	val2 = efuse_read(base, EFUSE_STATUS2);

	for (i = 0 ; i < num; i++) {
		if (block < 32) {
			shift = block & 0x1f;
			block++;
			if ((val1 & (1 << shift)) == 0)
				return true;
		} else {
			shift = (block - 32) & 0x1f;
			block++;
			if ((val2 & (1 << shift)) == 0)
				return true;
		}
	}
	return false;
}

bool cbc_key_writed(void)
{
	bool ret;

	ret = efuse_writed(CBC_KEY_BLOCK, AES_KEY_LEN >> 5);

	return ret;
}
