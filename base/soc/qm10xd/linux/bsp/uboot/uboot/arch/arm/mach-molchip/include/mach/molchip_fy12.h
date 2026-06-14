#ifndef __GLB_REG_
#define __GLB_REG_
#define 	IRAM_ADDR_BASE					(0x00100000)
#define 	IRAM_LENGTH						(0x4000)

#define 	REG_CPU_SYS_APB					(0x02E00000)
#define 	REG_DMA_BASE					(0x10300000)
#define 	REG_AES_BASE					(0x11600000)
#define 	REG_EFUSE_BASE					(0x1BA00000)
#define 	REG_EFUSE1_BASE					(0x1BB00000)


#define 	REG_SPIC_CFG_BASE					(0x1B000000)

#define 	REG_AP_APB_BASE					(0x18000000)
#define 	REG_AP_PERI_CLK_CTRL            (REG_AP_APB_BASE+0x0000)

//#define 	CKG_GMAC_CSR_EN                 (BIT(17))
#define 	CKG_SFC_EN                      (BIT(25))
#define 	CKG_EFUSE_EN                    (BIT(23))
#define 	CKG_UART1_EN                    (BIT(1))
#define 	CKG_UART0_EN                    (BIT(0))
#define 	CKG_WDT_EN                      (BIT(20))
#define 	CKG_XTL_TMR0_EN                 (BIT(15))
#define 	CKG_RTC_TMR0_EN                 (BIT(12))
#define 	CKG_SYST_EN                     (BIT(18))

#define 	REG_AP_PERI_APB_CLK_CTRL        (REG_AP_APB_BASE+0x0004)

#define 	CKG_APB_SFC_EN                  (BIT(28))
#define 	CKG_APB_GPIO_EN                 (BIT(20))
#define 	CKG_APB_RTCTL_EN                (BIT(16))
#define 	CKG_APB_EFUSE_EN                (BIT(22))
#define 	CKG_APB_UART1_EN                (BIT(1))
#define 	CKG_APB_UART0_EN                (BIT(0))
#define 	CKG_APB_WDT_EN                  (BIT(18))
#define 	CKG_APB_TIMER2_EN               (BIT(14))
#define 	CKG_APB_TIMER1_EN               (BIT(13))
#define 	CKG_APB_TIMER0_EN               (BIT(12))
#define 	CKG_APB_SYST_EN                 (BIT(15))

#define 	REG_AP_PERI_APB_CLK_CTRL1        (REG_AP_APB_BASE+0x0008)
#define 	CKG_GMAC0_CSR_EN                 (BIT(10))
#define 	CKG_AXI_GMAC1_EN                 (BIT(9))
#define 	CKG_AXI_GMAC0_EN                 (BIT(8))


#define 	REG_AP_PERI_SOFT_RST            (REG_AP_APB_BASE+0x000C)
#define 	EFUSE_SOFT_RST                  (BIT(25))
//#define 	EFUSE1_SOFT_RST                 (BIT(15))
#define 	GPIO_SOFT_RST                   (BIT(21))
#define 	UART1_SOFT_RST                  (BIT(1))
#define 	UART0_SOFT_RST                  (BIT(0))
#define 	TIMER1_SOFT_RST                 (BIT(13))
#define 	TIMER0_SOFT_RST                 (BIT(12))
#define 	WDT_SOFT_RST                    (BIT(19))
#define 	SYST_SOFT_RST                   (BIT(15))

#define 	REG_AP_PERI_SEL        (REG_AP_APB_BASE+0x0010)


#define 	REG_AP_GMAC            			(REG_AP_APB_BASE+0x000c)

#define 	REG_AP_AHB_BASE                  0X10000000
#define 	REG_AHB_PERI_CLK_CTRL            (REG_AP_AHB_BASE+0x0000)
#define 	CKG_SDIO_DLLREG_EN               (BIT(21))
#define 	CKG_SDIO_EN               		 (BIT(20))
#define 	CKG_SYS_SDIO_EN               	 (BIT(19))
#define 	CKG_AHB_HSLOCK_EN                (BIT(10))
#define 	CKG_AHB_AES_EN                   (BIT(8))
#define 	CKG_AHB_USB2_1_EN                (BIT(7))
#define 	CKG_AHB_USB2_0_EN                (BIT(6))
#define 	CKG_AHB_SFC_EN                   (BIT(5))
#define 	CKG_DMA_EN        				 (BIT(2))
#define 	CKG_AP_SYS_ARCH_EN               (BIT(0))

#define		REG_AP_AHB_PERI_SOFT_RST		(REG_AP_AHB_BASE+0x0004)
#define		EPHY0_SOFT_RST					 (BIT(18))
#define		SDIO_SOFT_RST					 (BIT(17))
#define		SPIC_SOFT_RST					 (BIT(14))
#define		AES_BUF_SOFT_RST				 (BIT(10))
#define		AES_SOFT_RST				 	 (BIT(8))
#define		USB2_0_AHB_SOFT_RST				 (BIT(6))
#define		USB2_0_UTMI_SOFT_RST			 (BIT(4))
#define		USB2_0_SOFT_RST				 	 (BIT(2))
#define		DMA_SOFT_RST				 	 (BIT(0))

#define		REG_CPU_SYS_APB					 (0x02E00000)
#define		REG_CPU_CLK_SEL					 (REG_CPU_SYS_APB + 0x8)
#define 	CKG_CPU_SYS_MTX_DIV(x)           ((x & 0x7) << 7)
#define 	CKG_CPU_DBG_DIV(x)           	 ((x & 0x7) << 4)
#define 	CKG_CS_DBG_SEL(x)           	 ((x & 0x3) << 2)
#define 	CKG_CPU_CORE_SEL(x)           	 ((x & 0x3) << 0)


#define		REG_CPU_SOFT_RST				 (REG_CPU_SYS_APB + 0xC)
#define 	CPU_GLB_SOFT_RST              	 (BIT(9))
#define 	CS_DBG_SOFT_RST                  (BIT(8))
#define 	CPU_L2_SOFT_RST                  (BIT(7))
#define 	CPU_SOCDBG_SOFT_RST              (BIT(6))
#define 	CPU_CORE1_ETM_SOFT_RST           (BIT(5))
#define 	CPU_CORE0_ETM_SOFT_RST           (BIT(4))
#define		CPU_CORE1_DBG_SOFT_RST           (BIT(3))
#define 	CPU_CORE0_DBG_SOFT_RST           (BIT(2))
#define 	CPU_CORE1_SOFT_RST               (BIT(1))
#define 	CPU_CORE0_SOFT_RST               (BIT(0))

#define 	REG_CPU_CORE_CTL                 (REG_CPU_SYS_APB+0x0010)

#define 	REG_UART0_BASE					 (0x18200000)
#define		REG_UART1_BASE					 (0x18300000)
#define		REG_UART2_BASE					 (0x18400000)
#define 	REG_TIMER0_BASE				 	 (0x19400000)


#define 	REG_SYS_TIMER_BASE				 (0x19900000)
#define 	REG_SYS_TIMER_VALUE				 (REG_SYS_TIMER_BASE + 0xC)
#define 	get_sys_timer_val()				 __raw_readl(REG_SYS_TIMER_VALUE)

#define 	REG_AP_SYS_CLK_CFG				(0x19300200)
#define		CLK_AP_APB_CFG					(REG_AP_SYS_CLK_CFG + 0x20)//0: 24m	1: 64m	2: 100m default: 2'h0
#define		CLG_USB2_UTMI0_CFG				(REG_AP_SYS_CLK_CFG + 0x24)
#define		CLK_USB2_UTMI1_CFG				(REG_AP_SYS_CLK_CFG + 0x28)
#define 	CLK_SFC_CFG						(REG_AP_SYS_CLK_CFG + 0x2C)//0: 24m	1: 64m	2: 91_4m	default: 2'h0
#define 	CLK_EFUSE_CFG					(REG_AP_SYS_CLK_CFG + 0x34)//0: 24m	1: 64m	2: 91_4m	default: 2'h0
#define     CLK_UART0_CFG                   (REG_AP_SYS_CLK_CFG + 0x54)//0: 24m     1: 64m  2: 100m         3: 128m         default: 2'h0
#define     CLK_UART1_CFG                   (REG_AP_SYS_CLK_CFG + 0x58)
#define     CLK_AP_SYS_CFG                  (REG_AP_SYS_CLK_CFG + 0x88)
#define     CLK_SDIO0_CFG                   (REG_AP_SYS_CLK_CFG + 0x8C) //0:24M 1:50M 2:100M 3:200M
#define     CLK_SDIO0_DLLREF_CFG            (REG_AP_SYS_CLK_CFG + 0x94) //0:100M


#define		REG_TOP_CLK_GEN					(0x28100200)
#define		CLK_GLB_APB_CFG					(REG_TOP_CLK_GEN + 0x30)//0: 24m	1: 64m	2: 100m 	3: 200m 	default: 2'h0
#define		CLK_GBL_VIO_CFG					(REG_TOP_CLK_GEN + 0x34)//0: 24m	1: 100m 2: 200m 	default: 2'h0

#define 	REG_GLB_APB_BASE				(0x28000000)
#define 	REG_EPLL_400M_CTRL				(REG_GLB_APB_BASE + 0x0C)
#define 	EPLL_400M_CNT_DONE				(BIT(31))	
#define 	REG_UPLL_640M_CTRL				(REG_GLB_APB_BASE + 0x10)
#define 	EPLL_640M_CNT_DONE				(BIT(31))	
#define 	STRAP_PIN_STATUS				(REG_GLB_APB_BASE + 0x7C)
#define 	SFC_BOOT_MODE					(BIT(1))
#define 	BOOT_MODE_MASK 					(0x3)
#define 	REG_ANA_CTRL					(REG_GLB_APB_BASE + 0x64)
#define 	ANA_CTRL_EPHY_RESET				(BIT(5))
#define		CKG_EPHY_REF_EN					(REG_GLB_APB_BASE + 0x84)

#define 	REG_ANALOG_BASE					(0x28500000)

#define 	REG_VIO_SYS_APB					(0x34000000)
#define 	VOU_CLK_CFG						(REG_VIO_SYS_APB + 0x0)
#define 	VOU_SRAM						(0x34100000)
#define 	HEAP_BASE						0x3410D000
#define		HEAP_LIMIT						0x3410F000

#define 	get_strap_status()				__raw_readl(STRAP_PIN_STATUS)
#define 	NOR_FLASH_BOOT					0
#define 	NAND_FLASH_BOOT					1

#define REG_HSIF_SYS_AHB_REG_BASE		0x23000000
#define REG_EMMC_CLK_RST_CTRL			(REG_HSIF_SYS_AHB_REG_BASE+0x00000)
#define BIT_EMMC_HOST_SOFT_RST			(0x1 << 25)
#define BIT_EMMC_PHY_SOFT_RST			(0x1 << 24)
#define BIT_CKG_EMMC_PHYCFG_APB_EN		(0x1 << 23)
#define BIT_CKG_AXI_EMMC_EN				(0x1 << 22)
#define BIT_CKG_EMMC_TM_EN				(0x1 << 21)
#define BIT_CKG_EMMC_CQETM_EN			(0x1 << 20)
#define BIT_CKG_EMMC_BASE_EN			(0x1 << 19)
#define BIT_CKG_EMMC_EN					(0x1 << 18)

#define REG_USB2_CLK_RST_CTRL			(REG_HSIF_SYS_AHB_REG_BASE+0x04)
#define BITS_CKG_EMMC_DIV_MSK			(0x7F << 16)
#define BITS_CKG_EMMC_DIV(x)			(((x) & 0x7F) << 16)

#define REG_EMMC_PHY_CFG_BASE			0x23C00000
#define REG_EMMC_CTRL_CFG_BASE			0x23300000

//DMA CID defination
#define SPIC_RX_RID					20
#define SPIC_TX_RID					21
#define UART0_RX_RID					0
#define UART1_RX_RID					2
#define UART2_RX_RID					4
#define UART0_TX_RID					1
#define UART1_TX_RID					3
#define UART2_TX_RID					5



#define SFC_RX_SAMPLE_DELAY				1

#endif


