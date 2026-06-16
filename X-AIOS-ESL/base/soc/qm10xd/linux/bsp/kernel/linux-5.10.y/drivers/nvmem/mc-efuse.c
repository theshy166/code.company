/*
 * Copyright (c) 2019 Molchip.
 *
 * This program is free software; you can redistribute it and/or modif y
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/nvmem-provider.h>
#include <linux/nvmem-consumer.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <crypto/cryptodev.h>
#include <linux/reset.h>
#include <linux/mini_clock/clock.h>
#include <linux/syscore_ops.h>

#define DEVICE_PM_OPS	0

#define EFUSE_CMD				0x0
#define OP_CMD_MASK				GENMASK(3,0)
#define OP_CMD_OFFSET			0
#define ADDR_MASK				GENMASK(11,4)
#define ADDR_OFFSET				4
#define WRITE_VALUE_MASK		GENMASK(19,12)
#define WRITE_VALUE_OFFSET		12
#define AES_KEY_ID_MASK			GENMASK(22,20)
#define AES_KEY_OFFSET			20
#define CMD_WFLAG				0
#define CMD_LOAD_USER_CMD		1
#define CMD_WRITE				2
#define CMD_READ				3
#define CMD_TRANS_KEY			4
#define CMD_WFLAG_AUTO			8
#define CMD_RELEASE_AESKEY		9
#define EFUSE_CONFIG			0x04
#define AVDD_SW_PRIORITY		BIT(0)
#define USER 					BIT(18)
#define MAP_ENTRY_MASK			GENMASK(31,24)
#define MAP_ENTRY_OFFSET		24
#define AES_BYTE_SEL			19
#define AES_USR0_PRIORITY  		GENMASK(18,0)
#define EFUSE_MATCH_KEY			0x08
#define MATCH_KEY 				0x92fc0025
#define EFUSE_TIMING0			0x0c
#define EFUSE_TIMING1			0x10
#define EFUSE_TIMING2			0x14
#define EFUSE_TIMING3			0x18
#define EFUSE_TIMING4			0x1c
#define EFUSE_TIMING5			0x20
#define EFUSE_TIMING6			0x24
#define EFUSE_DOUT				0x28
#define RVALUE_MASK				GENMASK(7,0)
#define WFLAG 					BIT(8)
#define EFUSE_STATUS0			0x2c
#define WFLAG_DETECT_COMPLT 	BIT(0)
#define LOAD_USER_CMD_COMPLT	BIT(1)
#define WRITE_COMPLT			BIT(2)
#define READ_COMPLT				BIT(3)
#define AES_COMPLT				BIT(4)
#define WFLAG_AUTO_COMPLT		BIT(8)
#define OP_PROCESSING			BIT(14)
#define OP_COMPLT				BIT(15)
#define CUR_CMD_MASK			GENMASK(19,16)
#define CUR_CMD_OFFSET			16
#define APB_LOCK_READ(x)		BIT((x) / 16)
#define APB_LOCK_WRITE(x)		BIT((x) / 4)
#define AES_LOCK_READ(x)		BIT(24 + (x) / 2)
#define USER_CMD_CHECK			BIT(31)
#define EFUSE_STATUS1			0x30
#define WFLAG_L(x)				(1 << (x))
#define EFUSE_STATUS2			0x34
#define WFLAG_H(x)				(1 << ((x) - 32))
#define EFUSE_STATUS3			0x38
#define W_ERR					BIT(0)
#define R_ERR					BIT(1)
#define AES_ERR					BIT(2)
#define CMD_ERR					BIT(3)
#define OP_ERR					BIT(4)
#define MATCH_ERR				BIT(5)
#define READ_LOCK				BIT(6)
#define P_MAX_ERR				BIT(7)
#define R_MAX_ERR				BIT(8)
#define DOUBLE_NQ_WARN			BIT(9)
#define EFUSE_STATUS4			0x3c
#define MEM_INFO(x)				(0x100 + (x) * 4)

#define EFUSE_MIN_ENTRY			0
#define EFUSE_MAX_ENTRY			64
#define EFUSE_LOCK_LIMIT_ENTRY  48
#define HALF_ENTRY_NUM			128
#define EFUSE_MAX_KEY_ID		8
#define EFUSE_WRITE				0x40
#define EFUSE_STATUS6			0x44
#define EFUSE_STATUS7			0x48
#define EFUSE_STATUS8			0x4c

#define WFLAG_COMPLT			BIT(0)

/* default timeout set to 1 sec */
#define TIMEOUT_MS		1000
#define DELAY_INTERVAL_US	100

#define BYTE_SEL_32BIT		0
#define BYTE_SEL_64BIT		1
#define BYTE_SEL_128BIT		2
#define BYTE_SEL_192BIT		3
#define BYTE_SEL_256BIT		3

enum{
	TRANS_AESKEY_32 = 3,
	TRANS_AESKEY_64 = 7,
	TRANS_AESKEY_128 = 15,
	TRANS_AESKEY_192 = 23,
	TRANS_AESKEY_256 = 31,
};

struct mc_efuse_chip {
	struct device *dev;
	struct nvmem_device *nvmem;
	void __iomem *base;
	struct mutex mutex;
};

struct mc_efuse_chip  *efuse_data[CONFIG_EFUSE_CTRL_NUM];

#ifdef CONFIG_CRYPTO_DEV_MC_AES
extern struct blocking_notifier_head aes_chain;
#endif
static int read_poll_timeout(void __iomem *addr, u32 mask)
{
	unsigned long timeout = jiffies + msecs_to_jiffies(TIMEOUT_MS);

	do {
		if (readl(addr) & mask)
			return 0;

		 usleep_range(DELAY_INTERVAL_US, DELAY_INTERVAL_US + 50);
	} while (!time_after(jiffies, timeout));

	return (readl(addr) & mask) ? 0 : -ETIMEDOUT;
}

static int write_user_cmd(struct mc_efuse_chip *efuse)
{
	int val, mask;
	int ret = 0;

	val = CMD_LOAD_USER_CMD << OP_CMD_OFFSET;
	writel(val, efuse->base + EFUSE_CMD);

	mask = OP_COMPLT | LOAD_USER_CMD_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0 ,mask);

	return ret;
}

static int cmd_ops_read(struct mc_efuse_chip *efuse, u32 block)
{
	int val, mask;
	int ret = 0;

	val = (block << ADDR_OFFSET) & ADDR_MASK;
	val |= CMD_READ;
	writel(val, efuse->base + EFUSE_CMD);

	mask = OP_COMPLT | READ_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0, mask);

	return ret;
}

static int cmd_ops_wflag(struct mc_efuse_chip *efuse,u32 entry)
{
	int val, mask;
	int ret = 0;
	u32 block;
	block = entry >> 2;
	val = (block << ADDR_OFFSET) & ADDR_MASK;
	val |= CMD_WFLAG;
	writel(val, efuse->base + EFUSE_CMD);

	mask = OP_COMPLT | WFLAG_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0, mask);

	return ret;
}

static int cmd_ops_write(struct mc_efuse_chip *efuse, u32 entry, u32 din)
{
	int val, mask;
	int ret = 0;
	u32 block,pos;

	block = entry >> 2;
	pos = entry & 0x3;

	writel(din, efuse->base + EFUSE_WRITE);

	val = (block << ADDR_OFFSET) & ADDR_MASK;
	val |= CMD_WRITE;

	writel(val, efuse->base + EFUSE_CMD);

	mask = OP_COMPLT | WRITE_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0, mask);

	return ret;
}

static int cmd_bit_write(struct mc_efuse_chip *efuse, u32 entry, u32 bit_pos)
{
	u32 block;
	int ret = 0;
	u32 val,mask;

	block = entry >> 2;
	writel(bit_pos, efuse->base + EFUSE_WRITE);

	val = ((block << ADDR_OFFSET) & ADDR_MASK) | CMD_WRITE;
	val |= WRITE_VALUE_MASK;

	writel(val, efuse->base + EFUSE_CMD);

	mask = WRITE_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0, mask);

	return ret;
}

static int mc_efuse_entry_apb_lock(struct mc_efuse_chip *efuse, u32 entry)
{
	unsigned int val;

	if (entry >= EFUSE_MAX_ENTRY)
		return -EFAULT;

	val = readl(efuse->base + EFUSE_STATUS6);
	val &= APB_LOCK_READ(entry);

	if (val)
		return -EFAULT;
	return 0;
}

static int mc_efuse_checkwrlock(struct mc_efuse_chip *efuse, u32 entry)
{
	u32 val;

	if (entry >= EFUSE_MAX_ENTRY)
		return -EFAULT;

	if (entry >= EFUSE_LOCK_LIMIT_ENTRY)
		return 0;

	if (entry >= HALF_ENTRY_NUM) {
		val = readl(efuse->base + EFUSE_STATUS8);
	} else {
		val = readl(efuse->base + EFUSE_STATUS7);
	}

	val &= APB_LOCK_WRITE(entry);
	if (val)
		return -EFAULT;
	return 0;
}

static int efuse_hw_init(struct mc_efuse_chip *efuse)
{

	writel(USER, efuse->base + EFUSE_CONFIG);
	return 0;
}

static int mc_reg_read(void *context,
			unsigned int reg, void *_val, size_t bytes)
{
	struct mc_efuse_chip *efuse = context;
	u8 *val = _val;
	u32 dout;
	u8 *pdout=(u8*)&dout;
	int i = 0;
	int ret = 0;
	unsigned int entry = 0,block = 0, pos = 0;

	if ((reg >= EFUSE_MAX_ENTRY) || (reg + bytes > EFUSE_MAX_ENTRY)) {
		dev_err(efuse->dev, "read bytes exceed max entry\n");
		return -EINVAL;
	}

	mutex_lock(&efuse->mutex);
	ret = write_user_cmd(efuse);
	if (ret) {
		dev_err(efuse->dev, "load user cmd failed\n");
		goto out;
	}

	memset(val, 0, bytes);

	for (i = 0; i < bytes; ) {
		entry = reg + i;
		val = _val + i;
		i ++;

		ret = mc_efuse_entry_apb_lock(efuse, entry);
		if (ret) {
			dev_err(efuse->dev, "entry:%d  is locked\n", entry);
			continue;
		}

		writel(MATCH_KEY, efuse->base + EFUSE_MATCH_KEY);

		block = (entry >> 2);
		pos = (entry & 0x3);
		ret = cmd_ops_read(efuse, block);

		if (ret) {
			dev_err(efuse->dev, "ops read %d cmd failed \n", entry);
			goto out;
		}

		dout = readl(efuse->base + EFUSE_STATUS4);
		*val = (u8)pdout[pos];
	}

out:
	mutex_unlock(&efuse->mutex);
	return 0;
}

static int ops_read(struct mc_efuse_chip *efuse , u32 entry , u32 *dout)
{
	int ret = 0;
	u32 block = 0,pos = 0;

	ret = write_user_cmd(efuse);
	if (ret) {
		dev_err(efuse->dev, "load user cmd failed\n");
		goto out;
	}

	ret = mc_efuse_entry_apb_lock(efuse, entry);
	if (ret) {
		dev_err(efuse->dev, "block:%d  is locked\n", entry >> 2);
		goto out;
	}

	writel(MATCH_KEY, efuse->base + EFUSE_MATCH_KEY);

	block = (entry >> 2);
	pos = (entry & 0x3);
	ret = cmd_ops_read(efuse, block);

	if (ret) {
		dev_err(efuse->dev, "ops read %d cmd failed \n", block);
		goto out;
	}

	*dout = readl(efuse->base + EFUSE_STATUS4);


out:
	return 0;
}

/*
1 - not programed
0 - programed
*/
static int efuse_checkprogram(struct mc_efuse_chip *efuse, u32 entry)
{
	u32 block = 0;
	block = entry >> 2;
	if (block < 32)
		return ((readl(efuse->base + EFUSE_STATUS1) >> block) & 0x1);
	else
		return ((readl(efuse->base + EFUSE_STATUS2) >> (block - 32)) & 0x1);
}

static int ops_write(struct mc_efuse_chip *efuse,u32 entry,u32 buf)
{
	int ret = 0;
	u32 dout = 0;
	u32 val,i;

	ret = mc_efuse_checkwrlock(efuse, entry);
	if (ret) {
		dev_err(efuse->dev, "block:%d is wrlocked!\n", entry >> 2);
		return -EINVAL;
	}

	ret = cmd_ops_wflag(efuse,entry);
	if (ret) {
		dev_err(efuse->dev, "wflag cmd failed\n");
		return -EINVAL;
	}
	if (efuse_checkprogram(efuse, entry)) {
		ret = cmd_ops_write(efuse, entry, buf);
		if (ret) {
			dev_err(efuse->dev, "ops write %d fail\n", entry);
			return -EINVAL;
		}
	}else {
		ret = ops_read(efuse, entry ,&dout);
		if (ret) {
			dev_err(efuse->dev, "ops read fail\n");
			return -EIO;
		}
		val = ~dout & buf;
		dev_dbg(efuse->dev, "---DEBUG[EFUSE BIT WRITE VALUE]:%08x---\n",val);

		for(i = 0; ;i++)
		{
			if(i == 0x20)
				return ret;
			if(((val >> i) & 0x1)) {
				ret = cmd_ops_wflag(efuse,entry);
				if (ret) {
					dev_err(efuse->dev, "wflag cmd failed\n");
					return -EINVAL;
				}

				ret = cmd_bit_write(efuse, entry, i);
				if (ret) {
					dev_err(efuse->dev, "bit write error\n");
					return ret;
				}
			}
		}
	}

	return ret;
}
static int mc_reg_write(void *context,
			 unsigned int reg, void *_val, size_t bytes)
{
	struct mc_efuse_chip *efuse = context;
	u8 *val = _val;
	int i = 0,pos;
	int ret = 0;
	size_t count,byte_end;
	unsigned int entry = 0;
	unsigned int buf=0;
	if ((reg >= EFUSE_MAX_ENTRY) || (reg + bytes > EFUSE_MAX_ENTRY)) {
		dev_err(efuse->dev, "write bytes exceed max entry\n");
		return -EINVAL;
	}

	mutex_lock(&efuse->mutex);

	ret = write_user_cmd(efuse);
	if (ret) {
		dev_err(efuse->dev, "load user cmd failed!\n");
		goto out;
	}

	byte_end = bytes & 0x3;
	count = bytes >> 2;
	if(byte_end == 0)
	{
		for(pos = 0;pos < count;pos++)
		{
			for (i = 0; i < 4; ) {
				entry = reg + i;
				val = _val + i;
				buf |= (*val << (i<<3));
				i++;
			}
			ret = ops_write(efuse,entry,buf);
			if (ret) {
				dev_err(efuse->dev, "ops_write failed\n");
				goto out;
			}
			reg = entry +1;
			_val = val + 1;
			buf = 0;
		}
	}else{
		for(pos = 0;pos <(count+1);pos++){
			if(pos == count){
				for (i = 0; i < byte_end; ) {
					entry = reg + i;
					val = _val + i;
					buf |= (*val << (i<<3));
					i++;
				}
				ret = ops_write(efuse,entry,buf);
				if (ret) {
					dev_err(efuse->dev, "ops_write failed\n");
					goto out;
				}
			}else{
				for (i = 0; i < 4; ) {
					entry = reg + i;
					val = _val + i;
					buf |= (*val << (i<<3));
					i++;
				}
			}
			ret = ops_write(efuse,entry,buf);
			if (ret) {
				dev_err(efuse->dev, "ops_write failed\n");
				goto out;
			}
			reg = entry +1;
			_val = val + 1;
			buf = 0;
		}
	}

out:
	mutex_unlock(&efuse->mutex);
	return ret;
}

static int mc_efuse_entry_ahb_lock(struct mc_efuse_chip *efuse, u32 key_len)
{
	int val = 0;
	int i = 0;
	int keyid = 0;

	for(i = 0; i < key_len; ){

		keyid = i >> 2;
		i += 4;

		if (keyid >= EFUSE_MAX_KEY_ID)
			return -EFAULT;

		val = readl(efuse->base + EFUSE_STATUS0);
		val &= AES_LOCK_READ(keyid);
		if (!val)
			return -EFAULT;
	}
	return 0;
}

static int cmd_trans_aeskey(struct mc_efuse_chip *efuse, u32 entry, u32 keyid)
{
	int val = 0;
	int mask = 0;
	int ret = 0;

	val |= CMD_TRANS_KEY;

	writel(val, efuse->base + EFUSE_CMD);
	mask = OP_COMPLT | AES_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0, mask);

	return ret;
}

static int cmd_release_aeskey(struct mc_efuse_chip *efuse)
{
	int val, mask;
	int ret = 0;

	val = CMD_RELEASE_AESKEY;

	writel(val, efuse->base + EFUSE_CMD);

	mask = OP_COMPLT;
	ret = read_poll_timeout(efuse->base + EFUSE_STATUS0, mask);

	return ret;
}


static int mc_efuse_xfer_key(struct mc_efuse_chip *efuse, u32 reg, u32 bytes)
{
	int ret = 0;
	int val = 0;
	int entry_map = 0;
	int aeskey_len = 0;
	unsigned int entry = 0;
	if ((reg >= EFUSE_MAX_ENTRY) || (reg + bytes > EFUSE_MAX_ENTRY)) {
		dev_err(efuse->dev, "xfer bytes exceed max entry\n");
		return -EINVAL;
	}

	if (bytes & 0x3) {
		dev_err(efuse->dev, "xfer bytes do not aligned 4\n");
		return -EINVAL;
	}

	mutex_lock(&efuse->mutex);
	aeskey_len = bytes;
	entry = reg;
	entry_map = entry  >> 2;//entry_map == block

	ret = write_user_cmd(efuse);
	if (ret) {
		dev_err(efuse->dev, "load user cmd failed\n");
		goto out;
	}

	ret = mc_efuse_entry_ahb_lock(efuse, aeskey_len);
	if (ret) {
		dev_err(efuse->dev, "key: %d ahb read not locked\n", entry_map);
		goto out;
	}

	switch(aeskey_len)
	{
		case 4:
			aeskey_len = TRANS_AESKEY_32;
			entry_map = (entry_map >> BYTE_SEL_32BIT);
			break;
		case 8:
			aeskey_len = TRANS_AESKEY_64;
			entry_map = (entry_map >> BYTE_SEL_64BIT);
			break;
		case 16:
			aeskey_len = TRANS_AESKEY_128;
			entry_map = (entry_map >> BYTE_SEL_128BIT);
			break;
		case 24:
			aeskey_len = TRANS_AESKEY_192;
			entry_map = (entry_map >> BYTE_SEL_192BIT);
			break;
		case 32:
			aeskey_len = TRANS_AESKEY_256;
			entry_map = (entry_map >> BYTE_SEL_192BIT);
			break;
		default:
			aeskey_len = TRANS_AESKEY_128;
			entry_map = (entry_map >> BYTE_SEL_128BIT);
			break;
	}

	val = readl(efuse->base + EFUSE_CONFIG);
	val &=AES_USR0_PRIORITY;
	val |=(aeskey_len << AES_BYTE_SEL);
	val |= (entry_map << MAP_ENTRY_OFFSET);
	writel(val, efuse->base + EFUSE_CONFIG);

	ret = cmd_trans_aeskey(efuse, entry, entry_map);

	if (ret) {
		dev_err(efuse->dev, "cmd tansfer aes key failed\n");
		goto out;
	}

out:
	mutex_unlock(&efuse->mutex);
	return ret;

}

static int mc_efuse_map_aes_key(unsigned long msg, struct aes_key_param *data)
{
	struct aes_key_param param;
	struct mc_efuse_chip *efuse;
	int ret = 0 , id = 0;
	if (!try_module_get(THIS_MODULE)) {
	    printk("efuse module get failed\n");
        goto err;
    }

	switch (msg) {
	case AES_MSG_KEY_REQUEST:
		memcpy(&param, data, sizeof(param));
		id = param.entry / EFUSE_MAX_ENTRY;
		efuse = efuse_data[id];
		ret = mc_efuse_xfer_key(efuse, param.entry, param.length);
		break;

	case AES_MSG_KEY_RELEASE:
		for(id = 0; id < CONFIG_EFUSE_CTRL_NUM; id++) {
			efuse = efuse_data[id];
			ret = cmd_release_aeskey(efuse);
        }
		break;
	}

	module_put(THIS_MODULE);

	if (ret)
		return NOTIFY_STOP;
	return NOTIFY_OK;
err:
	return NOTIFY_STOP;
}

static int mc_efuse_notify(struct notifier_block *this, unsigned long msg,
			    void *data)
{
	return mc_efuse_map_aes_key(msg, data);
}

static struct notifier_block mc_efuse_notifier = {
	.notifier_call = mc_efuse_notify,
};

static int mc_efuse_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mc_efuse_chip *efuse;
	struct resource *res;
	struct nvmem_config *econfig;
	int ret;
	bool no_need_init_clock;

	if (pdev->dev.of_node)
		pdev->id = of_alias_get_id(pdev->dev.of_node, "efuse");

	if (pdev->id < 0 || pdev->id >= CONFIG_EFUSE_CTRL_NUM) {
		dev_err(&pdev->dev, "id %d out of range\n", pdev->id);
		return -EINVAL;
	}
	if (efuse_data[pdev->id]) {
		dev_err(&pdev->dev, "port %d already allocated\n", pdev->id);
		return -EBUSY;
	}

	efuse = devm_kzalloc(dev, sizeof(*efuse),
				GFP_KERNEL);
	if (!efuse)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	efuse->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(efuse->base ))
		return PTR_ERR(efuse->base);

	efuse->dev = dev;
	econfig = devm_kzalloc(dev, sizeof(*econfig), GFP_KERNEL);
	if (!econfig) {
		devm_kfree(dev, efuse);
		return -ENOMEM;
	}

	econfig->stride = 1;
	econfig->word_size = 1;
	econfig->reg_read = mc_reg_read;
	econfig->reg_write = mc_reg_write;
	if (of_property_read_u32(dev->of_node, "mc,efuse-size",
				 &econfig->size))
	econfig->size = resource_size(res);
	econfig->id = pdev->id;
	econfig->priv = efuse;
	econfig->dev = efuse->dev;
	econfig->owner = THIS_MODULE;
	efuse->nvmem = nvmem_register(econfig);
	if (IS_ERR(efuse->nvmem))
		return PTR_ERR(efuse->nvmem);

	no_need_init_clock = of_property_read_bool(pdev->dev.of_node,"bypass-init-clock");
	if(no_need_init_clock == 0)
	{
		ret = clk_mini_enable(CKG_EFUSE_EN);
		if (ret) {
			dev_err(dev, "failed to enable gate\n");
			goto err;
		}

		ret = clk_mini_enable(CKG_APB_EFUSE_EN);
		if (ret) {
			clk_mini_disable(CKG_EFUSE_EN);
			dev_err(dev, "failed to enable gate\n");
			goto err;
		}
	}
	mutex_init(&efuse->mutex);

	efuse_hw_init(efuse);

	platform_set_drvdata(pdev, efuse);

	efuse_data[pdev->id] = efuse;

#ifdef CONFIG_CRYPTO_DEV_MC_AES
	ret = blocking_notifier_chain_register(&aes_chain, &mc_efuse_notifier);
	if (ret) {
		dev_err(dev, "failed to register blocking notifier chain\n");
		goto exit;
	}
#endif
	dev_info(dev, "NVMEM EFUSE\n");
	return 0;

exit:
	clk_mini_disable(CKG_EFUSE_EN);
	clk_mini_disable(CKG_APB_EFUSE_EN);
err:
	devm_kfree(dev, efuse);
	devm_kfree(dev, econfig);

	return ret;
}

static int mc_efuse_remove(struct platform_device *pdev)
{
#ifdef CONFIG_CRYPTO_DEV_MC_AES
	int err;
#endif
	struct mc_efuse_chip *efuse = platform_get_drvdata(pdev);
	struct nvmem_device *nvmem = efuse->nvmem;

	mutex_destroy(&efuse->mutex);
	clk_mini_disable(CKG_EFUSE_EN);
	clk_mini_disable(CKG_APB_EFUSE_EN);

#ifdef CONFIG_CRYPTO_DEV_MC_AES
	err = blocking_notifier_chain_unregister(&aes_chain, &mc_efuse_notifier);
	BUG_ON(err);
#endif
	nvmem_unregister(nvmem);
	return 0;
}

static const struct of_device_id mc_efuse_of_match[] = {
	{ .compatible = "mc,efuse" },
	{/* sentinel */},
};
MODULE_DEVICE_TABLE(of, mc_efuse_of_match);

#if DEVICE_PM_OPS

/**
 * mol_efuse_suspend
 * @dev: device pointer
 * Description: this function is invoked when suspend the driver and it direcly
 * call the main suspend function and then, if required, on some platform, it
 * can call an exit helper.
 */
static int __maybe_unused mol_efuse_suspend(struct device *dev)
{
	clk_mini_disable(CKG_EFUSE_EN);
	clk_mini_disable(CKG_APB_EFUSE_EN);
	dev_info(dev, "%s\n",__func__);
	return 0;
}

/**
 * mol_efuse_resume
 * @dev: device pointer
 * Description: this function is invoked when resume the driver before calling
 * the main resume function, on some platforms, it can call own init helper
 * if required.
 */
static int __maybe_unused mol_efuse_resume(struct device *dev)
{
	struct mc_efuse_chip *efuse = dev_get_drvdata(dev);
	clk_mini_enable(CKG_EFUSE_EN);
	clk_mini_enable(CKG_APB_EFUSE_EN);
	efuse_hw_init(efuse);
	dev_info(dev, "%s\n",__func__);
	return 0;
}

static struct dev_pm_ops mol_efuse_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(mol_efuse_suspend, mol_efuse_resume)
};
#endif


static int mol_efuse_suspend(void)
{
	clk_mini_disable(CKG_EFUSE_EN);
	clk_mini_disable(CKG_APB_EFUSE_EN);
	return 0;
}

static void  mol_efuse_resume(void)
{
	clk_mini_enable(CKG_EFUSE_EN);
	clk_mini_enable(CKG_APB_EFUSE_EN);
	efuse_hw_init(efuse_data[0]);
}

static struct syscore_ops mol_efuse_pm_ops = {
	.resume = mol_efuse_resume,
	.suspend = mol_efuse_suspend,
};


static struct platform_driver mc_efuse_driver = {
	.probe = mc_efuse_probe,
	.remove = mc_efuse_remove,
	.driver = {
		.name = "mc,efuse",
#if DEVICE_PM_OPS
		.pm = &mol_efuse_pm_ops,
#endif
		.of_match_table = mc_efuse_of_match,
	},
};


static int __init mc_efuse_init(void)
{
	int ret;

	register_syscore_ops(&mol_efuse_pm_ops);

	ret = platform_driver_register(&mc_efuse_driver);
	if (ret) {
		pr_err("Failed to register efuse driver\n");
		return ret;
	}

	return 0;
}

static void __exit mc_efuse_exit(void)
{
	unregister_syscore_ops(&mol_efuse_pm_ops);

	return platform_driver_unregister(&mc_efuse_driver);
}

subsys_initcall(mc_efuse_init);
module_exit(mc_efuse_exit);

MODULE_DESCRIPTION("EFUSE driver");
MODULE_LICENSE("GPL v2");
