#ifndef _BOOTARGS_H_
#define _BOOTARGS_H_

#define ARGS_SZ		0x1000
#define ARGS_REAL_SZ		(0x1000-0xc)

#define ARGS_MAGIC_DATA		0x5a5a5a5a
#define ARGS_MAGIC_DATA_SZ		0x4
#define ARGS_MAGIC_DATA_END		0x3ff


#define MMC_ARGS_OFF		0x200000
#define NOR_ARGS_OFF		0x200000

#define ARGS_IDLE_ADDR		0x30600000
#define ARGS_VSI_ADDR		0x30600004
#define ARGS_REAL_ADDR		0x30600008
#define ARGS_END_ADDR		0x30600ffc


//#define NAND_ARGS_OFF



#endif

