#ifndef _MOLCHIP_TS01_H_
#define _MOLCHIP_TS01_H_

#define REG_IROM_BASE		0x04000000
#define REG_IRAM_BASE		0x04010000


#define REG_AP_AHB_BASE		0x12060000
#define REG_UART0_BASE      0x13020000
#define REG_UART1_BASE      0x13030000

#define REG_SPIC_CFG_BASE	0x130E0000
#define REG_AP_APB_BASE		0x131E0000
#define REG_AP_CLK_CFG_BASE	0x13250000

#define REG_TOP_CLK_GEN_BASE	0x15C00000
#define REG_GLB_APB_BASE	0x15C40000

#define REG_SYS_TIMER_BASE				 (0X131b0000)
#define REG_SYS_TIMER_VALUE				 (REG_SYS_TIMER_BASE + 0x4)
#define get_sys_timer_val()				 __raw_readl(REG_SYS_TIMER_VALUE)


#define REG_AP_PERI_CLK_CTRL		(REG_AP_APB_BASE+0x0000)
#define REG_AP_PERI_APB_CLK_CTRL	(REG_AP_APB_BASE+0x0004)
#define REG_AP_PERI_SOFT_RST		(REG_AP_APB_BASE+0x0008)
#define REG_AP_PERI_GMAC_REG		(REG_AP_APB_BASE+0x000C)

//APB Bits defination
//REG_AP_PERI_CLK_CTRL bits defination
#define	CKG_APB_SPIC_EN				(BIT(19))
#define CKG_UART1_EN				(BIT(11))
#define CKG_UART0_EN				(BIT(10))
#define CKG_SYS_EN					(BIT(0))

//REG_AP_PERI_APB_CLK_CTRL bits defination
#define CKG_APB_UART1_EN			(BIT(7))
#define CKG_APB_UART0_EN			(BIT(6))
#define CKG_APB_SYST_EN				(BIT(0))

//REG_AP_PERI_SOFT_RST bits defination
#define UART1_SOFT_RST				(BIT(7))
#define UART0_SOFT_RST				(BIT(6))


#define REG_AP_SYS_CLK_CFG		(0x131B0000)

#define CLK_UART0_CFG			(REG_AP_SYS_CLK_CFG + 0x60)
#define CLK_UART1_CFG                   (REG_AP_SYS_CLK_CFG + 0x64)


#define REG_AHB_PERI_CLK_CTRL		(REG_AP_AHB_BASE+0x0000)

#define REG_AP_AHB_PERI_SOFT_RST	(REG_AP_AHB_BASE+0x0044)
//AHB Bits defination
#define CKG_AHB_SPIC_EN			(BIT(15))
#define SPIC_SOFT_RST			(BIT(10))
#define CKG_SPIC_SSI_EN			(BIT(8))



#define STRAP_PIN_STATUS		(REG_GLB_APB_BASE + 0x74)

#define SFC_BOOT_MODE			(BIT(1))

#define get_strap_status()              __raw_readl(STRAP_PIN_STATUS)


#endif

