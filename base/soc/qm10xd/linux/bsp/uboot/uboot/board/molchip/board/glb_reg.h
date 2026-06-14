#ifndef __GLB_REG_H__
#define __GLB_REG_H__

#define REG_CPU_SYS_CLK_RF_BASE		(0x09200200)
#define REG_SYS_TIMER_BASE		(0x09700000)
#define REG_TIMER_BASE			(0x09800000)
#define REG_EFUSE_BASE			(0x0A700000)
#define REG_CPU_SYS_AHB_REG_BASE	(0x0C000000)
#define REG_AES_BASE			(0x0C600000)
#define REG_DMA0_BASE			(0x0C700000)
#define REG_CEN_GLB_APB_REG_BASE	(0x10000000)

/*64KB IRAM*/
#define VPPU_G2D360_BASE		(0x27000000)

#define HEAP_BASE			(0x41200000)

#define STRAP_PIN_STATUS		(REG_CEN_GLB_APB_REG_BASE + 0x104)
#define get_strap_status()		__raw_readl(STRAP_PIN_STATUS)
#define NOR_FLASH_BOOT			0
#define NAND_FLASH_BOOT			1
#define USB_DOWNLOAD			2
#define SDIO1_BOOT			3

#define REG_CPLL_CTRL0			(REG_CPU_SYS_AHB_REG_BASE + 0x00)
#define CKG_SYST_EN			(BIT(1))

#define EFUSE_HASH0			(REG_CPU_SYS_AHB_REG_BASE + 0x54)
#define EFUSE_HASH(x)			(EFUSE_HASH0 + ((x) << 2))
#define EFUSE_HASH8			(REG_CPU_SYS_AHB_REG_BASE + 0x74)
#define EFUSE_HASH_TOP(x)		(EFUSE_HASH8 + ((x) << 2))

#define EFUSE_AES_CFG			(REG_CPU_SYS_AHB_REG_BASE + 0x94)
#define COMPARE_START_BLOCK(x)		(((x) & 0xff) << 0)
#define EFUSE_LOCK_BLOCK(x)		(((x) & 0x1fff) << 12)
#define COMPARE_LENGTH(x)		(((x) & 0xf) << 8)

#define EFUSE_HASH_COMPARE		(REG_CPU_SYS_AHB_REG_BASE + 0x98)
#define COMPARE_OK			(BIT(18))
#define COMPARE_DONE			(BIT(17))

#define CEN_PERI_APB_CLK_CTRL		(REG_CPU_SYS_AHB_REG_BASE + 0xB0)
#define CKG_APB_SYST_EN			(BIT(10))
#define CKG_APB_TIMER_EN		(BIT(2))
#define CKG_APB_EFUSE_EN		(BIT(0))

#define AP_PERI_SOFT_RST0		(REG_CPU_SYS_AHB_REG_BASE + 0xB4)
#define EFUSE_SOFT_RST			(BIT(8))

#define AP_MTX_CLK_CTRL0		(REG_CPU_SYS_AHB_REG_BASE + 0xDC)
#define CKG_AHB_AES_EN			(BIT(28))

#define AP_MTX_CLK_CTRL1		(REG_CPU_SYS_AHB_REG_BASE + 0xE0)
#define CKG_EFUSE_EN			(BIT(0))

#define CPU_SYS_RST1			(REG_CPU_SYS_AHB_REG_BASE + 0x1B4)
#define AES_SOFT_RST			(BIT(20))

#define SYS_TIMER_CNT			(REG_SYS_TIMER_BASE + 0xc)
#define get_sys_timer_val()		__raw_readl(SYS_TIMER_CNT)

#define CKG_SYST_CFG			(REG_CPU_SYS_CLK_RF_BASE + 0x38)
#define CKG_SYST_DIV_MASK		GENMASK(15,8)
#define CKG_SYST_SEL_MASK		GENMASK(1,0)
#define CKG_SYST_DIV(x)			(((x) << 8) & CKG_SYST_DIV_MASK)
#define	CKG_SYST_SEL(x)			(((x) << 0 ) & CKG_SYST_SEL_MASK)

#define CKG_RTC_TMR_CFG			(REG_CPU_SYS_CLK_RF_BASE + 0x40)
#define	CKG_RTC_TMR_SEL			(BIT(0))

#endif/*__GLB_REG_H__*/
