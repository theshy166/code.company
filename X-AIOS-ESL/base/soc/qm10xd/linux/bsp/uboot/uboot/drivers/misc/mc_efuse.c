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

struct udevice *mc_efuse_dev[2];

#define MATCH_KEY	0x92fc0025

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

#define EFUSE_POWERON	BIT(27)

#define EFUSE_READ_MOD	(BIT(0))

#define AESKEY_ENTRY_OFFSET		28

typedef enum EREADMOD{
	Normal_read = 0,
	Margin_read = 1,
}ReadMod;

//EFUSE STATUS0 Bit Defination
#define CMD_WFLAG_OFFSET			0
#define CMD_LOAD_USERCMD_OFFSET		1
#define CMD_WRITE_OFFSET			2
#define CMD_READ_OFFSET				3
#define CMD_TRANS_AESKEY_OFFSET		4

#define	CMD_WFLAG_AUTO_OFFSET		8

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
	u32 dout;
	u32 status0;
	u32 status1;/* 0x30   */
	u32 status2;
	u32 status3;
	u32 status4;/* 0x3C   */
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
	u8 fuses[64];
	int ret;
	int e_num;

	if(argc > 2){
		printf("args too many!!!\n");
		return 0;
	}
	e_num = strtoul(argv[1], NULL, 0);

	dev = mc_efuse_dev[e_num];

	ret = misc_read(dev, 0, &fuses, sizeof(fuses));
	if (ret) {
		return 0;
	}

	printf("efuse-contents:\n");
	print_buffer(0, fuses, 1, sizeof(fuses), 16);

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
	int entry;
	int e_num;
	int ret;

	if(argc > 4){
		printf("args too many!!!\n");
		return 0;
	}
	e_num = strtoul(argv[1], NULL, 0);
	entry = strtoul(argv[2], NULL, 0);
	val = strtoul(argv[3], NULL, 0);
	printf("e_num = %d, entry = %d, val = 0x%x \n", e_num, entry, val);
	e_num = strtoul(argv[1], NULL, 0);
	dev = mc_efuse_dev[e_num];

	ret = misc_write(dev, entry, &val, 4);
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
	*((volatile u32 *)(&reg->timing0)) = (1 << 24) | (2 << 16) | (4 << 8) | (40 << 0);
	*((volatile u32 *)(&reg->timing1)) = (3 << 24) | (2 << 16) | (1 << 12) | (432 << 0);
	*((volatile u32 *)(&reg->timing2)) = (1 << 24) | (2 << 16) | (40 << 8) | (1 << 0);
	*((volatile u32 *)(&reg->timing3)) = (2 << 24) | (1 << 16) | (5 << 8) | (8 << 0);
	*((volatile u32 *)(&reg->timing4)) = (1 << 0);
	*((volatile u32 *)(&reg->timing5)) = 0x441E9DB;
	*((volatile u32 *)(&reg->timing6)) = 0x6CFDC9;
}

static void efuse_init(struct mc_efuse_regs *reg)
{

//	efuse_reset();

	efuse_mem_poweron(reg);

	efuse_timing(reg);

}

/*
 *  0 - no error
 *  others - error occur
 */
static u32 efuse_check_error(struct mc_efuse_regs *reg)
{
	return readl(&reg->status3);
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
static int efuse_send_cmd(struct mc_efuse_regs *reg, EFUSE_OPCMD opcmd, u32 entry, u32 val, u32 aeskeyid)
{
	u32 mValue=0;

	switch(opcmd)
	{

		case CMD_WFLAG:
			mValue = OPCMD(opcmd) | OPADDR(entry);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_WFLAG));
			if(efuse_check_error(reg))
			{
				printk("entry %d STS4:%x Error occur, opcmd= %d!\n",entry,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_LOAD_USERCMD:
			mValue = OPCMD(opcmd) | OPADDR(entry);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_LOAD_USERCMD));
			if(efuse_check_error(reg))
			{
				printk("entry %d STS4:%x Error occur, opcmd= %d!\n",entry,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_WRITE:
			mValue = OPCMD(opcmd) | OPADDR(entry) |OPVAL(val);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_WRITE));
			if(efuse_check_error(reg))
			{
				printk("entry %d STS4:%x Error occur, opcmd= %d!\n",entry,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_READ:
			mValue = OPCMD(opcmd) | OPADDR(entry);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_READ));
			if(efuse_check_error(reg))
			{
				printk("entry %d STS4:%x Error occur, opcmd= %d!\n",entry,efuse_check_error(reg), opcmd);
				return -EIO;
			}
			else
				return 0;
		case CMD_TRANS_AESKEY:
			mValue = OPCMD(opcmd) | OPADDR(entry) | OPAESKEYID(aeskeyid);
			*((volatile u32 *)&reg->cmd) = mValue;
			while(!polling_sts(reg, CMD_TRANS_AESKEY));
			if(efuse_check_error(reg))
			{
				printk("entry %d STS4:%x Error occur, opcmd= %d!\n",entry,efuse_check_error(reg), opcmd);
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
				printk("entry %d STS4:%x Error occur, opcmd= %d!\n",entry,efuse_check_error(reg), opcmd);
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
static int efuse_checkpgm(struct mc_efuse_regs *reg, u32 entry)
{
	if(entry == 0xff)
	{
		efuse_send_cmd(reg, CMD_WFLAG_AUTO, 0, 0, 0);

		if(readl(&reg->status1) && readl(&reg->status2) == 0xffffffff)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		efuse_send_cmd(reg, CMD_WFLAG, entry, 0, 0);

		if(entry < 32)
			return ((readl(&reg->status1) >> entry) & 0x1);
		else
			return ((readl(&reg->status2) >> (entry - 32)) & 0x1);
	}

}

static u8 efuse_read(struct mc_efuse_regs *reg, u32 entry)
{
	u32 mvalue = 0;

	efuse_send_cmd(reg , CMD_READ , entry , 0 , 0);
	mvalue = *((volatile u32 *)&reg->dout);

	return (u8)mvalue;
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

static void efuse_read_config(struct mc_efuse_regs *reg)
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

static int mc_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;

	u8 *val = (u8 *)buf;

	efuse_init(reg);
	efuse_read_mod(reg, Margin_read);
	efuse_set_match_key(reg, true);
	efuse_read_config(reg);


	while(size--){
	*val++ =efuse_read(reg, offset++);
}

	return 0;
}

static void efuse_write(struct mc_efuse_regs *reg, u32 entry, u8 val)
{
	efuse_send_cmd(reg, CMD_WRITE,entry,val,0);
}

static int mc_efuse_write(struct udevice *dev, int offset,
				      const void *buf, int size)
{
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;
	u32 entry;
	u8 *val = (u8 *)buf;
	if((offset + size)>63){
		printk("entry access the limit!\n");
		return 0;
	}
	efuse_init(reg);
	efuse_set_match_key(reg, true);

	for(entry = offset; entry < (offset +size); entry++){
		if(!efuse_checkpgm(reg, entry)){
			printk("entry:%d had already programed!\n",entry);
			return 0;
		}
		else{
			efuse_write(reg, entry, *val++);
		}
	}
	return 0;
}

static void efuse_aeskey_entrycfg(struct mc_efuse_regs *reg, u32 entry)
{
	u32 config_val;

	config_val = readl(&reg->config) ;

	entry = entry>>2;
	config_val &= (~(0xF << AESKEY_ENTRY_OFFSET));

	config_val |= ((entry & 0xF) << AESKEY_ENTRY_OFFSET);

	writel(config_val, &reg->config);
}

static void efuse_security_read(struct mc_efuse_regs *reg, u32 entry, u32 aeskeyid)
{
	efuse_aeskey_entrycfg(reg, entry);

	efuse_read_config(reg);

	efuse_send_cmd(reg, CMD_TRANS_AESKEY,entry,0,aeskeyid);

}

void aes_access_efuse_switch(bool         num)
{
	int val;
	val = readl(REG_AP_AHB_MISC);
	val &= (~(1UL<<16));
	val |= (num <<16);

	writel(val, REG_AP_AHB_MISC);
}

int mc_aes_call(u32 mode, u32 *key, u32 *iv,u32 *src, u32 *dst, u32 len);
int mc_aes_use_efuse_key(u32 mode, u32 *iv,u32 *src, u32 *dst, u32 len, u8 entry_offset, u8 key_num)
{
	struct udevice *dev = mc_efuse_dev[0];
	struct mc_efuse_platdata *plat = dev_get_platdata(dev);
	struct mc_efuse_regs *reg =
		(struct mc_efuse_regs *)plat->base;
	int i;

	if(entry_offset >64){
		dev = mc_efuse_dev[1];
		aes_access_efuse_switch(1);
		plat = dev_get_platdata(dev);
		reg =(struct mc_efuse_regs *)plat->base;
		entry_offset -= 64;
	}
	else{
		aes_access_efuse_switch(0);
	}

	efuse_init(reg);
	efuse_set_match_key(reg, true);

	for(i=0 ; i< key_num; i++){
		efuse_security_read(reg, (entry_offset + i*4), i);
	}
	int key[4];
	key[0]= readl(0x11600000+ 0x10);
	key[1]= readl(0x11600000+ 0x14);
	key[2]= readl(0x11600000+ 0x18);
	key[3]= readl(0x11600000+ 0x1c);

	printf("aes_key[0] = 0x%x\naes_key[1] = 0x%x\naes_key[2]= 0x%x\naes_key[3]= 0x%x\n", key[0], key[1], key[2], key[3]);
	mc_aes_call(mode, NULL, iv, src, dst, len);

	return 0;
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
