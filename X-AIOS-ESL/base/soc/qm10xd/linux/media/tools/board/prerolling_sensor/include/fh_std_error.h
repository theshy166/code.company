/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * 错误号定义,此头文件依据<sdk修订维护文档Ver001.docx>定义相关错误号。
 * 但为了避免和fh_pub_err.h的定义冲突，修改了相关前缀。
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */
#ifndef __FH_STD_ERR_H__
#define __FH_STD_ERR_H__
#include "fh_std_typedef.h"

#define FHK_STD_DRV_ID    (0x80000000) /* 系统\驱动层 */
#define FHK_STD_SUBLIB_ID (0x90000000) /* lib内部逻辑层 */
#define FHK_STD_API_ID    (0xA0000000) /* api 层 */
#define FHK_STD_SOL_ID    (0xB0000000) /* 解决方案层 */
#define FHK_STD_USR_ID    (0xC0000000) /* 用户产品层 */

#define FHK_STD_DRV_ERR(mod, lev, id)    ((FH_SINT32)((FHK_STD_DRV_ID) | ((mod) << 16) | (lev) << 12) | (id))
#define FHK_STD_API_ERR(mod, lev, id)    ((FH_SINT32)((FHK_STD_API_ID) | ((mod) << 16) | (lev) << 12) | (id))
#define FHK_STD_ERR2MOD(error)           ((((FH_UINT32)(error)) >> 16) & 0x1ff)
#define FHK_STD_ERR2ID(error)            (((FH_UINT32)(error)) & 0xfff)

/*SDK标准化定义,SDK预留部分为0-255,媒体私有模块定义从256-512*/
enum FHK_STD_MODULE_ID
{
	FHK_STD_MOD_NONE       = 0,
	FHK_STD_MOD_DDRC       = 1,
	FHK_STD_MOD_WDT        = 2,
	FHK_STD_MOD_TMR0       = 3,
	FHK_STD_MOD_ISP_B      = 7,
	FHK_STD_MOD_ISP_F      = 8,
	FHK_STD_MOD_VPU        = 9,
	FHK_STD_MOD_PAE        = 10,
	FHK_STD_MOD_I2C0       = 11,
	FHK_STD_MOD_I2C1       = 12,
	FHK_STD_MOD_JPEG       = 13,
	FHK_STD_MOD_BGM        = 14,
	FHK_STD_MOD_EMAC       = 15,
	FHK_STD_MOD_AES        = 16,
	FHK_STD_MOD_SDC0       = 17,
	FHK_STD_MOD_SDC1       = 18,
	FHK_STD_MOD_ACW        = 19,
	FHK_STD_MOD_SADC       = 20,
	FHK_STD_MOD_SPI1       = 21,
	FHK_STD_MOD_SPI2       = 22,
	FHK_STD_MOD_DMAC0      = 23,
	FHK_STD_MOD_DMAC1      = 24,
	FHK_STD_MOD_I2S0       = 25,
	FHK_STD_MOD_GPIO0      = 26,
	FHK_STD_MOD_USB        = 27,
	FHK_STD_MOD_SPI0       = 28,
	FHK_STD_MOD_PMU_ARC    = 29,
	FHK_STD_MOD_UART0      = 30,
	FHK_STD_MOD_UART1      = 31,
	FHK_STD_MOD_PMU_ARM    = 32,
	FHK_STD_MOD_RTC        = 33,
	FHK_STD_MOD_AHBC0      = 34,
	FHK_STD_MOD_AHBC1      = 35,
	FHK_STD_MOD_PWM        = 36,
	FHK_STD_MOD_MIPIC      = 37,
	FHK_STD_MOD_MIPI_WRAP  = 38,
	FHK_STD_MOD_HEVC       = 39,
	FHK_STD_MOD_GPIO1      = 40,
	FHK_STD_MOD_PMU        = 255,
	FHK_STD_MOD_SDK_MODULE = 255,

	FHK_STD_MOD_STD        = 256,
	FHK_STD_MOD_MEDIA_BASE = 257,
	FHK_STD_MOD_VENC       = 258,
	FHK_STD_MOD_BGMCV      = 259,
	FHK_STD_MOD_NNIP       = 260,
	FHK_STD_MOD_IVE        = 261,
	FHK_STD_MOD_MAX_MODULE = 512,
};

/*错误级别*/
enum FHK_STD_LEVEL_ID
{
	FHK_STD_LEV_DEBG = 0,      /* debug-level                                  */
	FHK_STD_LEV_INFO = 1,      /* informational                                */
	FHK_STD_LEV_NOTC = 2,      /* normal but significant condition            */
	FHK_STD_LEV_WARN = 3,      /* warning conditions                          */
	FHK_STD_LEV_ERRR = 4,      /* error conditions                            */
	FHK_STD_LEV_CRIT = 5,      /* critical conditions                          */
	FHK_STD_LEV_ALRT = 6,      /* action must be taken immediately            */
	FHK_STD_LEV_FATL = 7,      /* just for compatibility with previous version */
};

/*通用错误号*/
/*FHERR_SDK_BUTT为SDK定义的通用错误号,任何自定义错误号必须大于FHERR_SDK_BUTT*/
/*FHERR_STD_BUTT为标准化模块定义媒体相关的通用错误号,任何调用此接口的相关模块自定义错误号必须大于FHERR_STD_BUTT*/
enum FHK_STD_PUB_ERR_ID
{
	FHER_STD_RET_OK            = 0,
	FHER_STD_INVALID_HANDLE    = 1,
	FHER_STD_INVALID_DEVICE    = 2,
	FHER_STD_ILLEGAL_PARAM     = 3,
	FHER_STD_NOT_EXIST         = 4,
	FHER_STD_EXIST             = 5,
	FHER_STD_NULL_POINTER      = 6,
	FHER_STD_NOT_SUPPORT       = 7,
	FHER_STD_NOT_CONFIG        = 8,
	FHER_STD_NOT_PERMITTED     = 9,
	FHER_STD_TIMEOUT           = 10,
	FHER_STD_NOMEM             = 11,
	FHER_STD_NOVMM_MEM         = 12,
	FHER_STD_BUF_EMPTY         = 13,
	FHER_STD_BUF_FULL          = 14,
	FHER_STD_NOT_READY         = 15,
	FHER_STD_BAD_ADDRESS       = 16,
	FHER_STD_BUSY              = 17,

	FHERR_SDK_BUTT             = 127,
	FHER_STD_HAS_CREATE        = 128,
	FHER_STD_NOT_CREATE        = 129,
	FHER_STD_NOT_START         = 130,
	FHER_STD_NOT_STOP          = 131,
	FHER_STD_DATA_EMPTY        = 132,
	FHER_STD_DATA_FULL         = 133,
	FHER_STD_REMAP_FAIL        = 134,
	FHER_STD_NOT_MATCH         = 135,
	FHER_STD_SDK_ERR           = 136,

	FHERR_STD_BUTT             = 255,
};

/*一些为本模块私有定义*/
/*0-22比特为全局功能，23-31作为各个模块私有*/
#define FHK_STD_ZONE_OS        (1 << 0)
#define FHK_STD_ZONE_MEMORY    (1 << 4) /*模块的细分内存打印*/
#define FHK_STD_ZONE_TIMING    (1 << 5) /*模块的开始完成等时序打印*/
#define FHK_STD_ZONE_CODETRC   (1 << 6) /*函数调用追踪*/

#define FHK_STD_ZONE_NONE      (0)
#define FHK_STD_ZONE_ALL       (0x007fffff)

#define FHK_STD_MODID2ZONE(mod)  (((FH_UINT32)mod) << 23)
#define FHK_STD_ZONE2MODID(zone) (((FH_UINT32)zone) >> 23)
#endif
