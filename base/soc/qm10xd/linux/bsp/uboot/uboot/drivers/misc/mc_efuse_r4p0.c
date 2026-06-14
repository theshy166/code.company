/*
 * eFuse driver for Molchip devices
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <misc.h>
#include <mach/hardware.h>
#include "mc_aes.h"

struct udevice *mc_efuse_dev[4];
#define EFUSE_VERSION_R4P0
#define MATCH_KEY	0x92fc0025
#define APB_LOCK_READ(x)		BIT((x) / 4)
#define APB_LOCK_WRITE(x)       BIT(x)
#define EFUSE_MAX_BLOCK         16
#define EFUSE_LOCK_LIMIT_BLOCK  12
#define HALF_BLOCK_NUM          32
#define BLOCK55 		55
#define BLOCK62 		62
#define BLOCK63 		63

#define HASH_LOCK		0xFF00
#define AES_LOCK		0xC00

typedef enum{
	CMD_WFLAG = 0,
	CMD_LOAD_USERCMD = 1,
	CMD_WRITE = 2,
	CMD_READ = 3,
	CMD_TRANS_AESKEY = 4,
	CMD_WFLAG_AUTO = 8,
	CMD_RELEASE_AESKEY = 9,
}EFUSE_OPCMD;

//EFUSE CMD Bit Defination
#define OPCMD(x)  (x << 0)
#define OPADDR(x) (x << 4)
#define OPVAL(x)  (x << 12)
#define OPAESKEYID(x) (x << 20)

//EFUSE CONFIG Bit Defination
#ifdef EFUSE_VERSION_R4P0
	#define EFUSE_POWERON	BIT(18)
#else
	#define EFUSE_POWERON	BIT(27)
#endif

#define EFUSE_READ_MOD	(BIT(0))

#ifdef EFUSE_VERSION_R4P0
	#define AESKEY_ENTRY_OFFSET		24
#else
	#define AESKEY_ENTRY_OFFSET		28
#endif

typedef enum EREADMOD{
	Normal_read = 0,
	Margin_read = 1,
}ReadMod;

enum{
	TRANS_AESKEY_32 = 3,
	TRANS_AESKEY_64 = 7,
	TRANS_AESKEY_128 = 15,
	TRANS_AESKEY_192 = 23,
	TRANS_AESKEY_256 = 31,
};

//EFUSE STATUS0 Bit Defination
#define CMD_WFLAG_OFFSET			0
#define CMD_LOAD_USERCMD_OFFSET		1
#define CMD_WRITE_OFFSET			2
#define CMD_READ_OFFSET				3
#define CMD_TRANS_AESKEY_OFFSET		4

#define	CMD_WFLAG_AUTO_OFFSET		8
#define OP_COMPLT_OFFSET                   15

#define polling_sts(reg, name) (((*(volatile u32 *)(&reg->status0)) >> name##_OFFSET) & 0x1	)


struct mc_efuse_regs {
	u32 cmd;      /* 0x00   */
	u32 config;      /* 0x04   */
	u32 match_key;        /* 0x08   */
	u32 timing0;
	u32 timing1; /* 0x10   */
	u32 timing2;
	u32 timing3;
	u32 timing4;
	u32 timing5;/* 0x20   */
	u32 timing6;
	u32 dout;	/*0x28	*/
	u32 status0;
	u32 status1;/* 0x30   */
	u32 status2;
	u32 status3;
	u32 status4;/* 0x3C   */
	u32 write_val;
	u32 status6;
	u32 status7;
	u32 status8;
};

struct mc_efuse_platdata {
	void __iomem *base;
	struct clk *clk;
};

static int dump_efuses(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	/*
	 * N.B.: This function is tailored towards the RK3399 and assumes that
	 *       there's always 32 fuses x 32 bits (i.e. 128 bytes of data) to
	 *       be read.
	 */

	struct udevice *dev;
	u32 fuses[16];
	int ret;
	int e_num;

	memset(fuses, 0, 64);
	if(argc > 2){
		printf("args too many!!!\n");
		return 0;
	}
	e_num = strtoul(argv[1], NULL, 0);

	if(e_num >3){
		printf("invalid efuse number");
	}

	dev = mc_efuse_dev[e_num];

	ret = misc_read(dev, 0, &fuses, sizeof(fuses)/sizeof(fuses[0]));
	if (ret) {
		return 0;
	}

	printf("efuse-contents:\n");
	print_buffer(0, fuses, 4, sizeof(fuses)/sizeof(fuses[0]), 8);

	return 0;
}

static int write_efuse(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	/*
	 * N.B.: This function is tailored towards the RK3399 and assumes that
	 *       there's always 32 fuses x 32 bits (i.e. 128 bytes of data) to
	 *       be read.
	 */

	struct udevice *dev;
	int val;
	int block;
	int e_num;
	int ret;

	if(argc > 4){
		printf("args too many!!!\n");
		return 0;
	}
	e_num = strtoul(argv[1], NULL, 0);
	block = strtoul(argv[2], NULL, 0);
	val = strtoul(argv[3], NULL, 0);
	printf("e_num = %d, block = %d, val = 0x%x \n", e_num, block, val);
	e_num = strtoul(argv[1], NULL, 0);
	dev = mc_efuse_dev[e_num];

	ret = misc_write(dev, block, &val, 1);
	if (ret) {
		return 0;
	}

	return 0;
}
U_BOOT_CMD(
	dump_efuse, 2, 1, dump_efuses,
	"Dump the content of the efuses",
	"dump_efuse 0 : printf efuse0 content ;	dump_efuse 1: printf efuse1 content"
);

U_BOOT_CMD(
	efuse_write, 4, 1, write_efuse,
	"write the content of the efuses",
	"efuse_write [efuse_num] [entry] [value]"
);

static void efuse_mem_poweron(struct mc_efuse_regs *reg)
{
	*((volatile u32 *)(&reg->config)) |= EFUSE_POWERON;
}


//this timing set is for 36M efuse clk to adjust the memory model
static void efuse_timing(struct mc_efuse_regs *reg)
{
	/*adjust the value of this timing if efuse clk changes*/
	/*nothing to do*/
}

/*
 *  0 - no error
 *  others - error occur
 */
static u32 efuse_check_error(struct mc_efuse_regs *reg)
{
	return readl(&reg->status3);
}

static void efuse_load_user_cmd(struct mc_efuse_regs *reg)
{
	*((volatile u32 *)(&reg->cmd)) = CMD_LOAD_USERCMD;
	while(!polling_sts(reg, CMD_LOAD_USERCMD));
}

static void efuse_aeskey_trans_cmd(struct mc_efuse_regs *reg)
{
	*((volatile u32 *)(&reg->cmd)) = CMD_TRANS_AESKEY;
	while(!polling_sts(reg, CMD_TRANS_AESKEY));
}

static void efuse_aeskey_release_cmd(struct mc_efuse_regs *reg)
{
	*((volatile u32 *)(&reg->cmd)) = CMD_RELEASE_AESKEY;
	while(!polling_sts(reg, OP_COMPLT));
}


/*******************************************************
*
* Function name efuse_send_cmd
* Description   : send efuse command
* Parameter
*        @opcmd        cmd type
*        @entry        entry addr
*        @val          operation value
* Return        0 success  ,  1 fail
**********************************************************/
static int efuse_send_cmd(struct mc_efuse_regs *reg, EFUSE_OPCMD opcmd, u32 block, u32 val, u32 aeskeyid)
{
	u32 mValue=0;

	switch(opcmd)
	{

		case CMD_WFLAG:
			mValue = OPCMD(opcmd) | OPADDR(block);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_WFLAG));
			if(efuse_check_error(reg))
			{
				printk("block %d STS4:%x Error occur, opcmd= %d!\n",block,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_LOAD_USERCMD:
			mValue = OPCMD(opcmd) | OPADDR(block);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_LOAD_USERCMD));
			if(efuse_check_error(reg))
			{
				printk("block %d STS4:%x Error occur, opcmd= %d!\n",block,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_WRITE:
			*((volatile u32 *)&reg->write_val) = val;
			mValue = OPCMD(opcmd) | OPADDR(block);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_WRITE));
			if(efuse_check_error(reg))
			{
				printk("block %d STS4:%x Error occur, opcmd= %d!\n",block,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_READ:
			mValue = OPCMD(opcmd) | OPADDR(block);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_READ));
			if(efuse_check_error(reg))
			{
				printk("block %d STS4:%x Error occur, opcmd= %d!\n",block,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_TRANS_AESKEY:
			mValue = OPCMD(opcmd) | OPADDR(block) | OPAESKEYID(aeskeyid);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_TRANS_AESKEY));
			if(efuse_check_error(reg))
			{
				printk("block %d STS4:%x Error occur, opcmd= %d!\n",block,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
			break;
		case CMD_WFLAG_AUTO:
			mValue = OPCMD(opcmd);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_WFLAG_AUTO));
			if(efuse_check_error(reg))
			{
				printk("block %d STS4:%x Error occur, opcmd= %d!\n",block,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_RELEASE_AESKEY:
			break;
		default:
			break;
	}
	return -EPERM;
}

/* 1 - not programed
*  0 -programed
*/
static int efuse_checkpgm(struct mc_efuse_regs *reg, u32 block)
{
	efuse_send_cmd(reg, CMD_WFLAG, block, 0, 0);

	if(block < 32)
		return ((readl(&reg->status1) >> block) & 0x1);
	else
		return ((readl(&reg->status2) >> (block - 32)) & 0x1);


}

static void load_usercmd(struct mc_efuse_regs *reg)
{
	efuse_send_cmd(reg, CMD_LOAD_USERCMD,63,0,0);
}

static void efuse_set_match_key(struct mc_efuse_regs *reg, bool en)
{
	if(en)
	{
	        *((volatile u32 *)(&reg->match_key)) = MATCH_KEY;
	}
	else
	{
	        *((volatile u32 *)(&reg->match_key)) = 0xFFFFFFFF;
	}
}

static void efuse_init(struct mc_efuse_regs *reg)
{

//	efuse_reset();

	efuse_mem_poweron(reg);

	efuse_timing(reg);

	efuse_set_match_key(reg, true);

	load_usercmd(reg);

}

static u32 efuse_read(struct mc_efuse_regs *reg, u32 block)
{
	u32 mvalue = 0;

	efuse_send_cmd(reg , CMD_READ , block , 0 , 0);
	mvalue = *((volatile u32 *)&reg->status4);

	return mvalue;
}

static void efuse_read_mod(struct mc_efuse_regs *reg, ReadMod readmod)
{
	u32 mValue;

	mValue = readl(&reg->config);
	if(Normal_read == readmod)
	{
		mValue &= ~EFUSE_READ_MOD;
	}
	else
	{
		mValue |= EFUSE_READ_MOD;
	}
	writel(mValue, &reg->config);
}

static int mc_efuse_apb_lock(struct mc_efuse_regs *reg, u32 block)
{
	u32 val;

	if (block >= EFUSE_MAX_BLOCK)
		return -EINVAL;

	val = readl(&reg->status6);
	val &= APB_LOCK_READ(block);

	if (val)
		return -EFAULT;
	return 0;
}

static int mc_efuse_checkwrlock(struct mc_efuse_regs *reg, u32 block)
{
	u32 val;

	if (block >= EFUSE_MAX_BLOCK)
		return -EFAULT;

	if (block >= EFUSE_LOCK_LIMIT_BLOCK)
		return 0;

	if (block >= HALF_BLOCK_NUM) {
		val = readl(&reg->status8);
		block -= HALF_BLOCK_NUM;
	} else {
		val = readl(&reg->status7);
	}

	val &= APB_LOCK_WRITE(block);
	if (val)
		return -EFAULT;
	return 0;
}

bool mc_efuse_read_op( int bolck_num,
				      void *data, int block_cnt)
{
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)REG_EFUSE_BASE;
	u32 *val=(u32 *)data;
	int ret = 0;
	efuse_read_mod(reg, Margin_read);
	efuse_set_match_key(reg, true);
	if (block_cnt > 64){
		printk("size parameter error\n");
		return -EFAULT;
	}
	while(block_cnt--){
		ret = mc_efuse_apb_lock(reg, bolck_num);
		if (ret) {
			printk("block:%d  is locked\n", bolck_num++);
			val++;
			continue;
		}
		*val++ =efuse_read(reg, bolck_num++);
	}
	return 0;
}

bool secure_enalbe_judge(void)
{
	int ret,val;
	unsigned int secure_block;

	ret = mc_efuse_read_op(BLOCK55, &secure_block, 1);
	if (ret) {
			printf(" READ BLOCK55 FAIL\r\n");
			return false;
	}

	if (!((secure_block >> 30) & 0x3)){
			printf("the secure_block is 0x%08x \r\n ",secure_block);
			return false;
	}

	ret = mc_efuse_read_op(BLOCK62, &val, 1);
	if ((ret != false) || !((val & HASH_LOCK) == HASH_LOCK)){
			printf(" BLOCK62 val is 0x%08x	ret is 0x%08x \r\n",val,ret);
			return false;
	}

	ret = mc_efuse_read_op(BLOCK63, &val, 1);
	if ((ret != false) || !((val & AES_LOCK) ==AES_LOCK))
	{
			printf("BLOCK63 val is 0x%08x  ret is 0x%08x \r\n",val,ret);
			return false;
	}
		return true;
}

static int mc_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;

	u32 *val=(u32 *)buf;
	int ret = 0;
	efuse_read_mod(reg, Margin_read);
	efuse_set_match_key(reg, true);

	if (size > EFUSE_MAX_BLOCK){
		printk("size parameter error\n");
		return -EFAULT;
	}
	while(size--){
		ret = mc_efuse_apb_lock(reg, offset);
		if (ret) {
			if (ret == -EINVAL) {
				printk("Invalid block num :%d\n", offset);
				return -EFAULT;
			}
			printk("block:%d  is locked\n", offset++);
			val++;
			continue;
		}
		*val++ =efuse_read(reg, offset++);
	}
	return 0;
}

static void efuse_write(struct mc_efuse_regs *reg, u32 block, u32 val)
{
	efuse_send_cmd(reg, CMD_WRITE,block,val,0);
}

static int mc_efuse_write(struct udevice *dev, int offset,
				      const void *buf, int size)
{
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;
	u32 block;
	u32 *val = (u32 *)buf;

	if((offset + size) > EFUSE_MAX_BLOCK){
		printk("entry access the limit!\n");
		return 0;
	}
	for(block = offset; block < (offset +size); block++){
		if (!efuse_checkpgm(reg, block)) {
			printk("block:%d had already programed!\n",block);
			return 0;
		} else if (mc_efuse_checkwrlock(reg, block)) {
			printk("block:%d is wrlocked!\n",block);
			return 0;
		} else {
			efuse_write(reg, block, *val++);
		}
	}
	return 0;
}

#if 0
void aes_access_efuse_switch(bool num)
{
	int val;
	val = readl(REG_AP_AHB_MISC);
	val &= (~(1UL<<16));
	val |= (num <<16);

	writel(val, REG_AP_AHB_MISC);
}
#endif

int mc_aes_call(u32 mode, u32 *key, u32 *iv,u32 *src, u32 *dst, u32 len);
int mc_aes_use_efuse_key(u32 mode, u32 *iv,u32 *src, u32 *dst, u32 len, u32 map_entry, u32  aeskey_len)
{
	struct udevice *dev = mc_efuse_dev[0];
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;
	int lock,config_val;
	int key[4];
	int i;

	//aes_access_efuse_switch(0);
	efuse_init(reg);
	efuse_set_match_key(reg, true);
	efuse_load_user_cmd(reg);
	printf("in efuse\r\n");
	lock =((readl(&reg->status0) >> 24 ) & 0xf); 

	if(lock != 0xf){
		printk("AES lock not set \n");
	}

	efuse_set_match_key(reg, true);

	switch(aeskey_len)
		{
			case 32:
				aeskey_len = TRANS_AESKEY_32;
				map_entry = (map_entry >> 0);
				break;
			case 64:
				aeskey_len = TRANS_AESKEY_64;
				map_entry = (map_entry >> 1);
				break;
			case 128:
				aeskey_len = TRANS_AESKEY_128;
				map_entry = (map_entry >> 2);
				break;
			case 192:
				aeskey_len = TRANS_AESKEY_192;
				map_entry = (map_entry >> 3);
				break;
			case 256:
				aeskey_len = TRANS_AESKEY_256;
				map_entry = (map_entry >> 3);
				break;
			default:
				aeskey_len = TRANS_AESKEY_128;
				map_entry = (map_entry >> 2);
				break;
		}

	config_val = readl(&reg->config) ;
	config_val &= 0x0007ffff;
	config_val |= (aeskey_len<<19);
	config_val |= (map_entry<<24);
	writel(config_val, &reg->config);

	efuse_aeskey_trans_cmd(reg);

	for (i = 0; i < 4; i++)
		key[i] = readl(AES_ENCRYPT_BASE + AES_KEY(i));

	printf("aes_key[0] = 0x%x\naes_key[1] = 0x%x\naes_key[2]= 0x%x\naes_key[3]= 0x%x\n", key[0], key[1], key[2], key[3]);

	mc_aes_call(mode, NULL, iv, src, dst, len);

	return 0;
}

int mc_aeskey_release(void)
{
	int ret = 0;
	struct udevice *dev = mc_efuse_dev[0];
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;

	efuse_aeskey_release_cmd(reg);
	return ret;
}

static const struct misc_ops mc_efuse_ops = {
	.read = mc_efuse_read,
	.write = mc_efuse_write,
};

static int mc_efuse_ofdata_to_platdata(struct udevice *dev)
{
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	plat->base = (void *)dev_read_addr(dev);
	return 0;
}

static const struct udevice_id mc_efuse_ids[] = {
	{ .compatible = "mc,efuse" },
	{}
};


static int mc_efuse_probe(struct udevice *dev)
{
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;
	int val;
	val = readl(REG_CEN_PERI_APB_CLK_CTRL);
	val |= CKG_APB_EFUSE_EN;
	writel(val, REG_CEN_PERI_APB_CLK_CTRL);
	val = readl(REG_CKG_EFUSE_CTL);
	val |= CKG_EFUSE_EN;
	writel(val, REG_CKG_EFUSE_CTL);
	efuse_init(reg);
	
	return 0;
}

U_BOOT_DRIVER(mc_efuse) = {
	.name = "mc,efuse",
	.id = UCLASS_MISC,
	.of_match = mc_efuse_ids,
	.probe = mc_efuse_probe,
	.ofdata_to_platdata = mc_efuse_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct mc_efuse_platdata),
	.ops = &mc_efuse_ops,
};
