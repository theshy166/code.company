#ifndef __GLB_REG_
#define __GLB_REG_
#define 	IRAM_ADDR_BASE					(0x00100000)
#define 	REG_DMA_BASE					(0x0c700000)
#define 	REG_SDIO1_BASE					(0x1D300000)
#define 	REG_SDIO0_BASE					(0x1D400000)
#define 	CEN_PIN_REG						(0x10200000)
#define 	REG_AES_BASE					(0x11700000)
#define 	REG_EFUSE_BASE					(0x1BA00000)

#define 	REG_SPIC_CFG_BASE					(0x1c700000)

#define 	REG_AP_APB_BASE					(0x10000000)
#define 	REG_AP_PERI_CLK_CTRL            (REG_AP_APB_BASE+0x0200)
#define 	CKG_GMAC_CSR_EN                 (BIT(17))
#define 	CKG_EFUSE_EN                    (BIT(0))
#define 	CKG_UART1_EN                    (BIT(11))
#define 	CKG_UART0_EN                    (BIT(10))
//#define 	CKG_WDT_POR_EN                  (BIT(8))
#define 	CKG_WDT_EN                      (BIT(7))
#define 	CKG_XTL_TMR0_EN                 (BIT(12))
#define 	CKG_RTC_TMR0_EN                 (BIT(11))
#define 	CKG_SYST_EN                     (BIT(9))

#define 	REG_AP_PERI_APB_CLK_CTRL        (REG_AP_APB_BASE+0x0204)
//#define 	CKG_AP_APB_ARCH_EN              (BIT(20))
//#define 	CKG_APB_SFC_EN                 (BIT(19))
#define 	CKG_APB_GPIO_EN                 (BIT(18))
#define 	CKG_APB_RTCTL_EN                (BIT(14))
#define 	CKG_APB_EFUSE_EN                (BIT(0))
#define 	CKG_APB_UART1_EN                (BIT(7))
#define 	CKG_APB_UART0_EN                (BIT(6))
#define 	CKG_APB_WDT_EN                  (BIT(4))
#define 	CKG_APB_TIMER2_EN               (BIT(3))
#define 	CKG_APB_TIMER1_EN               (BIT(2))
#define 	CKG_APB_TIMER0_EN               (BIT(4))
#define 	CKG_APB_SYST_EN                 (BIT(13))

//#define 	CKG_APB_SPIC_EN		CKG_APB_SFC_EN

#define 	REG_AP_PERI_SOFT_RST            (REG_AP_APB_BASE+0x0008)
#define 	EFUSE_SOFT_RST                  (BIT(18))
#define 	EFUSE1_SOFT_RST                  (BIT(15))
#define 	GPIO_SOFT_RST                   (BIT(8))
#define 	UART1_SOFT_RST                  (BIT(7))
#define 	UART0_SOFT_RST                  (BIT(6))
#define 	TIMER1_SOFT_RST                 (BIT(3))
#define 	TIMER0_SOFT_RST                 (BIT(2))
#define 	WDT_SOFT_RST                    (BIT(1))
#define 	SYST_SOFT_RST                   (BIT(0))

#define 	REG_AP_GMAC            			(REG_AP_APB_BASE+0x000c)

#define 	REG_AP_AHB_BASE                  0X0c000000
#if 1
#define VEU_SYS_APB_REG			(0x1c000000)
#define IO_GRP2_PIN_REG_BASE		(0x1f000000)
#define REG_SFC_CTRL0			(VEU_SYS_APB_REG + 0x1a4)
#define CKG_SFC_EN				(BIT(0))
#define CKG_APB_SFC_EN			(BIT(2))
#define SFC_SOFT_RST			(BIT(11))
#define SFC_CLOCK_SEL(val)		(((val) & 0x7) << 4)


#define REG_SPIC_BASE			(0x1C700000)

#endif

#define 	REG_AHB_PERI_CLK_CTRL            (REG_AP_AHB_BASE+0x1c)
//#define 	CKG_SDIO_DLLREG_EN               (BIT(21))
//#define 	CKG_SDIO_EN               		 (BIT(20))
//#define 	CKG_SYS_SDIO_EN               	 (BIT(19))
#define 	CKG_AHB_HSLOCK_EN                (BIT(10))
#define 	CKG_AHB_AES_EN                   (BIT(8))
#define 	CKG_AHB_USB2_1_EN                (BIT(7))
#define 	CKG_AHB_USB2_0_EN                (BIT(6))
#define 	CKG_AHB_SFC_EN                   (BIT(5))
#define 	CKG_GMAC_EB				         (BIT(3))
#define 	CKG_DMA_EN        				 (BIT(2))
#define    CKG_DMA0_EN                       (BIT(1))
#define 	CKG_AP_SYS_ARCH_EN               (BIT(0))

#define 	CKG_AHB_SPIC_EN		CKG_AHB_SFC_EN


#define		REG_CPU_CLK_CTRL1				 (REG_AP_AHB_BASE+0x003C)
#define 	CKG_CS_DBG_SEL(x)                ((x & 0x3) << 17)
#define 	CKG_CPU_DBG_DIV(x)               ((x & 0x7) << 14)
#define 	CKG_CPU_SYS_MTX_DIV(x)           ((x & 0x7) << 11)
#define 	CKG_CPU_CORE_SEL(x)              ((x & 0x3)  << 9)
#define 	CS_DBG_SOFT_RST                  (BIT(8))
#define 	CPU_L2_SOFT_RST                  (BIT(7))
#define 	CPU_SOCDBG_SOFT_RST              (BIT(6))
#define 	CPU_CORE1_ETM_SOFT_RST           (BIT(5))
#define 	CPU_CORE0_ETM_SOFT_RST           (BIT(4))
#define		CPU_CORE1_DBG_SOFT_RST           (BIT(3))
#define 	CPU_CORE0_DBG_SOFT_RST           (BIT(2))
#define 	CPU_CORE1_SOFT_RST               (BIT(1))
#define 	CPU_CORE0_SOFT_RST               (BIT(0))

#define 	REG_CPU_CORE                     (REG_AP_AHB_BASE+0x0040)
#define 	REG_AP_AHB_PERI_SOFT_RST         (REG_AP_AHB_BASE+0x20)
#define 	HSLOCK_SOFT_RST                  (BIT(11))
#define 	SPIC_SOFT_RST                    (BIT(14))
#define 	AES_SOFT_RST                     (BIT(20))
#define 	USB2_0_AHB_SOFT_RST              (BIT(6))
#define 	USB2_0_UTMI_SOFT_RST             (BIT(4))
#define 	USB2_0_SOFT_RST                  (BIT(2))
#define 	DMA_SOFT_RST                     (BIT(23))
#define 	REG_CPU_CLK_CTRL2                (REG_AP_AHB_BASE+0x0048)

#define 	REG_AP_AHB_MISC                  (REG_AP_AHB_BASE+0x0064)



#define 	REG_AP_SYS_CLK_CFG				(0x19F00200)
#define		CLK_AP_APB_CFG					(REG_AP_SYS_CLK_CFG + 0x20)//0: 24m	1: 64m	2: 100m default: 2'h0
#define		CLG_USB0_UTMI0_CFG				(REG_AP_SYS_CLK_CFG + 0x24)
#define		CLK_USB0_UTMI1_CFG				(REG_AP_SYS_CLK_CFG + 0x28)
#define 	CLK_SPIC_SSI_CFG				(REG_AP_SYS_CLK_CFG + 0x38)//0: 24m	1: 64m	2: 91_4m	default: 2'h0
#define     CLK_UART0_CFG                   (REG_AP_SYS_CLK_CFG + 0x60)//0: 24m     1: 64m  2: 100m         3: 128m         default: 2'h0
#define     CLK_UART1_CFG                   (REG_AP_SYS_CLK_CFG + 0x64)
/* 0:24M 1:50M 2:100M 3:200M */
#define CLK_SDIO0_CFG						(REG_AP_SYS_CLK_CFG + 0x94)

/* 0:100M */
#define CLK_SDIO0_DLLREF_CFG				(REG_AP_SYS_CLK_CFG + 0x9c)


#define     REG_VDSP_SYS_APB_REG            (0x20000000)
#define     VDSP_CLK_RST_CTRL               (REG_VDSP_SYS_APB_REG + 0x20)
#define     CKG_VDSP_SYS_SEL(x)             (((x) & 3) << 0)

#define		REG_TOP_CLK_GEN					(0x10300200)
#define		CLK_SYST_CFG					(REG_TOP_CLK_GEN + 0x28)

#define 	REG_GLB_APB_BASE				(0x10000000)
#define 	STRAP_PIN_STATUS				(REG_GLB_APB_BASE + 0x104)
#define 	USB_DL_MODE						(BIT(0))
#define 	SFC_BOOT_MODE					(BIT(1))
#define 	CPU_BOOT_FREQ					(BIT(2))


//DMA CID defination
#define SPIC_RX_RID					20
#define SPIC_TX_RID					22
#define SPIC_RX1_RID				21
#define SPIC_TX1_RID				23
#define UART0_RX_RID					2
#define UART1_RX_RID					4
#define UART2_RX_RID					0
#define UART0_TX_RID					3
#define UART1_TX_RID					5
#define UART2_TX_RID					1


#define 	REG_ANA_CTRL					(REG_GLB_APB_BASE + 0x64)
#define 	ANA_CTRL_EPHY_RESET				(BIT(5))
#define 	CLK_DIG_CTRL					(REG_GLB_APB_BASE + 0x6c)

#define 	REG_ANALOG_BASE					(0x28500000)

#define 	AON_SHARE_SRAM					(0x30010000)
#define 	VOU_SRAM						(0x38a00000)
#define 	HEAP_BASE						0x38A53000
//#define		HEAP_LIMIT						0x38A55000

#define 	get_strap_status()				__raw_readl(STRAP_PIN_STATUS)
#define 	NOR_FLASH_BOOT					0
#define 	NAND_FLASH_BOOT					1

#define		ARC600_0_CTRL					(0x10000030)
#define		ARC600_1_CTRL					(0x10000050)
#define		ARC_SOFT_RST					(BIT(0))
#define		ARC_START						(BIT(2))

#define SFC_RX_SAMPLE_DELAY				1
/*for xc01 define*/
#define		REG_CPU_SYS_AHB_REG_BASE	(0xC000000)

#define		REG_CEN_PERI_CLK_CTRL		(REG_CPU_SYS_AHB_REG_BASE + 0x000ac)
#define		REG_CEN_PERI_APB_CLK_CTRL	(REG_CPU_SYS_AHB_REG_BASE + 0x000b0)
#define		REG_AP_PERI_SOFT_RST0		(REG_CPU_SYS_AHB_REG_BASE + 0x000b4)
#define 	REG_CEN_GLB_APB_REG_BASE                (0x10000000)
#define 	REG_CKG_APB_GATE_CTRL		(REG_CEN_GLB_APB_REG_BASE + 0x00200)
#define 	REG_CKG_PERI_GATE_CTRL		(REG_CEN_GLB_APB_REG_BASE + 0x00204)

#define 	REG_UART0_BASE					 (0x08F00000)
#define		REG_UART1_BASE					 (0x09000000)
#define         REG_UART2_BASE                                   (0x18200000)
#define 	REG_TIMER0_BASE				 	 (0x18B00000)
#define 	REG_SPI0_BASE					 (0x08B00000)
#define 	REG_SPI1_BASE					 (0x08C00000)
#define 	CKG_SPI0_EN					 (BIT(6))
#define 	CKG_SPI1_EN					 (BIT(7))
#define 	CKG_APB_SPI0_EN					 (BIT(8))
#define 	CKG_APB_SPI1_EN					 (BIT(9))
#define 	SPI0_SOFT_RST					 (BIT(15))
#define 	SPI1_SOFT_RST					 (BIT(16))

#define 	REG_SYS_TIMER_BASE				 (0x18A00000)
#define 	REG_SYS_TIMER_VALUE				 (REG_SYS_TIMER_BASE + 0xc)
#define 	get_sys_timer_val()				 __raw_readl(REG_SYS_TIMER_VALUE)

#define	REG_VEU_SYS_APB_BASE				(0x1c000000)
#define	REG_SDIO0_CTRL0						(REG_VEU_SYS_APB_BASE + 0x138)
#define	REG_SDIO0_CTRL1						(REG_VEU_SYS_APB_BASE + 0x13c)
#define SDIO0_DLLIN_CTRL_REG_SPL			(REG_VEU_SYS_APB_BASE + 0x140)
#define SDIO0_DLLIN_INFO_REG_SPL			(REG_VEU_SYS_APB_BASE + 0x144)
#define SDIO0_DLLOUT_STATUS_SPL				(REG_VEU_SYS_APB_BASE + 0x148)
#define SDIO0_DLLIN_CTRL_REG_DRV			(REG_VEU_SYS_APB_BASE + 0x14c)
#define SDIO0_DLLIN_INFO_REG_DRV			(REG_VEU_SYS_APB_BASE + 0x150)
#define SDIO0_DLLOUT_STATUS_DRV				(REG_VEU_SYS_APB_BASE + 0x154)
#define	REG_SDIO1_CTRL0						(REG_VEU_SYS_APB_BASE + 0x160)
#define	REG_SDIO1_CTRL1						(REG_VEU_SYS_APB_BASE + 0x164)
#define	AHB_SDIO_SOFT_RST					(BIT(27))
#define SDIO1_DLLIN_CTRL_REG_SPL			(REG_VEU_SYS_APB_BASE + 0x168)
#define SDIO1_DLLIN_INFO_REG_SPL			(REG_VEU_SYS_APB_BASE + 0x16c)
#define SDIO1_DLLOUT_STATUS_SPL				(REG_VEU_SYS_APB_BASE + 0x170)
#define SDIO1_DLLIN_CTRL_REG_DRV			(REG_VEU_SYS_APB_BASE + 0x174)
#define SDIO1_DLLIN_INFO_REG_DRV			(REG_VEU_SYS_APB_BASE + 0x178)
#define SDIO1_DLLOUT_STATUS_DRV				(REG_VEU_SYS_APB_BASE + 0x17c)

#define	REG_SDIO0_CTRL2						(REG_VEU_SYS_APB_BASE + 0x18c)
#define	REG_SDIO1_CTRL2						(REG_VEU_SYS_APB_BASE + 0x190)
#define	CKG_AHB_SDIO_EN						(BIT(0))
#define	CKG_SDIO_DLLREF_EN					(BIT(4))
#define	CKG_SDIO_EN							(BIT(5))

#define IO_MUX_SD0							(0x00)
#define IO_MUX_SD1							(0x02)
#define IO_SD0_CD							(CEN_PIN_REG + 0x15c)
#define IO_SD0_CLK							(CEN_PIN_REG + 0x160)
#define IO_SD0_CMD							(CEN_PIN_REG + 0x164)
#define IO_SD0_D0							(CEN_PIN_REG + 0x168)
#define IO_SD0_D1							(CEN_PIN_REG + 0x16c)
#define IO_SD0_D2							(CEN_PIN_REG + 0x170)
#define IO_SD0_D3							(CEN_PIN_REG + 0x174)
#define IO_SD1_CD							(CEN_PIN_REG + 0x138)
#define IO_SD1_CLK							(CEN_PIN_REG + 0x134)
#define IO_SD1_CMD							(CEN_PIN_REG + 0x130)
#define IO_SD1_D0							(CEN_PIN_REG + 0x110)
#define IO_SD1_D1							(CEN_PIN_REG + 0x114)
#define IO_SD1_D2							(CEN_PIN_REG + 0x118)
#define IO_SD1_D3							(CEN_PIN_REG + 0x11C)

#define PAD0_G2_FUNC_SEL						(CEN_PIN_REG + 0x110)
#define PAD1_G2_FUNC_SEL						(CEN_PIN_REG + 0x114)
#define PAD2_G2_FUNC_SEL						(CEN_PIN_REG + 0x118)
#define PAD3_G2_FUNC_SEL						(CEN_PIN_REG + 0x11c)
#define PAD4_G2_FUNC_SEL						(CEN_PIN_REG + 0x120)
#define PAD5_G2_FUNC_SEL						(CEN_PIN_REG + 0x124)
#define PAD6_G2_FUNC_SEL						(CEN_PIN_REG + 0x128)
#define PAD7_G2_FUNC_SEL						(CEN_PIN_REG + 0x12c)
#define PAD8_G2_FUNC_SEL						(CEN_PIN_REG + 0x130)
#define PAD9_G2_FUNC_SEL						(CEN_PIN_REG + 0x134)
#define PAD10_G2_FUNC_SEL						(CEN_PIN_REG + 0x138)
#define PAD11_G2_FUNC_SEL						(CEN_PIN_REG + 0x13c)
#define PAD12_G2_FUNC_SEL						(CEN_PIN_REG + 0x140)
#define PAD13_G2_FUNC_SEL						(CEN_PIN_REG + 0x144)
#define PAD14_G2_FUNC_SEL						(CEN_PIN_REG + 0x148)
#define PAD15_G2_FUNC_SEL						(CEN_PIN_REG + 0x14c)
#define PAD16_G2_FUNC_SEL						(CEN_PIN_REG + 0x150)
#define PAD17_G2_FUNC_SEL						(CEN_PIN_REG + 0x154)
#define PAD18_G2_FUNC_SEL						(CEN_PIN_REG + 0x158)
#define PULL_UP(reg_val)		((reg_val & (~(0x3 << 5)))|(2 << 5))
#define DRI_STRENGTH(reg_val, strength)	((reg_val & (~(0xf))) | \
					(strength & 0xf))
#define DRI_PULL_UP_SD0_CD					(IO_GRP2_PIN_REG_BASE + 0x50)
#define DRI_PULL_UP_SD0_CLK					(IO_GRP2_PIN_REG_BASE + 0x54)
#define DRI_PULL_UP_SD0_CMD					(IO_GRP2_PIN_REG_BASE + 0x58)
#define DRI_PULL_UP_SD0_D0					(IO_GRP2_PIN_REG_BASE + 0x5c)
#define DRI_PULL_UP_SD0_D1					(IO_GRP2_PIN_REG_BASE + 0x60)
#define DRI_PULL_UP_SD0_D2					(IO_GRP2_PIN_REG_BASE + 0x64)
#define DRI_PULL_UP_SD0_D3					(IO_GRP2_PIN_REG_BASE + 0x68)
#define DRI_PULL_UP_SD1_CD					(IO_GRP2_PIN_REG_BASE + 0x2c)
#define DRI_PULL_UP_SD1_CLK					(IO_GRP2_PIN_REG_BASE + 0x28)
#define DRI_PULL_UP_SD1_CMD					(IO_GRP2_PIN_REG_BASE + 0x24)
#define DRI_PULL_UP_SD1_D0					(IO_GRP2_PIN_REG_BASE + 0x04)
#define DRI_PULL_UP_SD1_D1					(IO_GRP2_PIN_REG_BASE + 0x08)
#define DRI_PULL_UP_SD1_D2					(IO_GRP2_PIN_REG_BASE + 0x0c)
#define DRI_PULL_UP_SD1_D3					(IO_GRP2_PIN_REG_BASE + 0x10)

#define PAD0_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x04)
#define PAD1_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x08)
#define PAD2_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x0C)
#define PAD3_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x10)
#define PAD4_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x14)
#define PAD5_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x18)
#define PAD6_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x1C)
#define PAD7_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x20)
#define PAD8_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x24)
#define PAD9_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x28)
#define PAD10_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x2C)
#define PAD11_G2_CTRL						(IO_GRP2_PIN_REG_BASE + 0x30)

#define REG_EMMC_CLK_RST_CTRL			(REG_VEU_SYS_APB_BASE+0x194)
#define BIT_EMMC_HOST_SOFT_RST			(0x1 << 29)
#define BIT_EMMC_PHY_SOFT_RST			(0x1 << 28)
#define BIT_CKG_EMMC_PHYCFG_APB_EN		(0x1 << 27)
#define BIT_CKG_AHB_EMMC_EN				(0x1 << 26)
#define BIT_CKG_AXI_EMMC_EN				(0x1 << 25)
#define BIT_CKG_EMMC_2X_EN				(0x1 << 24)
#define BIT_CKG_EMMC_EN					(0x1 << 23)
#define BIT_CKG_EMMC_BASE_EN			(0x1 << 22)
#define BIT_CKG_EMMC_CQETM_EN			(0x1 << 21)
#define BIT_CKG_EMMC_TM_EN				(0x1 << 20)

#define BITS_CKG_EMMC_BASE_SEL_OFFSET		18
#define BITS_CKG_EMMC_BASE_SEL(x)		(((x) & 0x3) << 18)
#define BITS_CKG_EMMC_BASE_MSK			(0x3 << 18)
#define BITS_CKG_EMMC_DIV(x)			(((x) & 0x7F) << 11)
#define BITS_CKG_EMMC_DIV_MSK			(0x7F << 11)
#define BITS_CKG_EMMC_2X_DIV(x)			(((x) & 0x7F) << 4)
#define BITS_CKG_EMMC_2X_DIV_MSK			(0x7F << 4)
#define BITS_CKG_EMMC_2X_SEL(x)			(((x) & 0x3) << 2)
#define BITS_CKG_EMMC_2X_SEL_MSK			(0x3 << 2)

#define REG_EMMC_CLK_RST_CTRL1			(REG_VEU_SYS_APB_BASE+0x198)
#define BIT_CKG_EMMC_BASE_AUTO_GATE_SEL		(0x1 << 25)
#define BIT_CKG_EMMC_BASE_FORCE_EN			(0x1 << 24)

#define BITS_CKG_EMMC_BASE_DIV(x)			(((x) & 0xF) << 16)
#define BITS_CKG_EMMC_BASE_DIV_MSK			(0xF << 16)

#define BITS_CKG_EMMC_TM_DIV(x)				(((x) & 0x1F) << 9)
#define BITS_CKG_EMMC_TM_DIV_MSK			(0x1F << 9)

#define BITS_CKG_EMMC_TM_SEL(x)				(((x) & 0x3) << 7)
#define BITS_CKG_EMMC_TM_SEL_MSK			(0x3 << 7)

#define BITS_CKG_EMMC_CQETM_DIV(x)			(((x) & 0x1F) << 2)
#define BITS_CKG_EMMC_CQETM_DIV_MSK			(0x1F << 2)

#define BITS_CKG_EMMC_CQETM_SEL(x)			(((x) & 0x3) << 0)
#define BITS_CKG_EMMC_CQETM_MSK(x)			(0x3 << 0)

#define REG_EMMC_CTRL_CFG_BASE		(0x1d200000)
#define REG_EMMC_PHY_CFG_BASE		(0x1ca00000)

#define REG_PLL_BASE	0x2F300024
#define REG_SPLL_BASE	0x2F300050
#define REG_CORE_BASE	0x09200220
#define REG_LPSEL_BASE	0x10000108

struct clk_src_reg {
	unsigned int pll_addr;
	unsigned int pll_end_bit;
	unsigned int pll_start_bit;
	unsigned int pll_val;
};

struct clk_src_cfg {
	unsigned int base_addr;
	struct clk_src_reg clk_src_regs[3];
};

#endif

