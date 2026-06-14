#ifndef __GLB_REG_
#define __GLB_REG_
#define 	IRAM_ADDR_BASE					(0x00100000)
#define 	REG_DMA_BASE					(0x10200000)
#define 	REG_AES_BASE					(0x11700000)
#define 	REG_EFUSE_BASE					(0x1BA00000)

#define 	REG_SPIC_CFG_BASE					(0x1F000000)

#define 	REG_AP_APB_BASE					(0x18000000)
#define 	REG_AP_PERI_CLK_CTRL            (REG_AP_APB_BASE+0x0000)
#define 	CKG_GMAC_CSR_EN                 (BIT(17))
#define 	CKG_EFUSE_EN                    (BIT(16))
#define 	CKG_UART1_EN                    (BIT(11))
#define 	CKG_UART0_EN                    (BIT(10))
//#define 	CKG_WDT_POR_EN                  (BIT(8))
#define 	CKG_WDT_EN                      (BIT(7))
#define 	CKG_XTL_TMR0_EN                 (BIT(2))
#define 	CKG_RTC_TMR0_EN                 (BIT(1))
#define 	CKG_SYST_EN                     (BIT(0))

#define 	REG_AP_PERI_APB_CLK_CTRL        (REG_AP_APB_BASE+0x0004)
//#define 	CKG_AP_APB_ARCH_EN              (BIT(20))
#define 	CKG_APB_SFC_EN                 (BIT(19))
#define 	CKG_APB_GPIO_EN                 (BIT(18))
#define 	CKG_APB_RTCTL_EN                (BIT(14))
#define 	CKG_APB_EFUSE_EN                (BIT(13))
#define 	CKG_APB_UART1_EN                (BIT(7))
#define 	CKG_APB_UART0_EN                (BIT(6))
#define 	CKG_APB_WDT_EN                  (BIT(4))
#define 	CKG_APB_TIMER2_EN               (BIT(3))
#define 	CKG_APB_TIMER1_EN               (BIT(2))
#define 	CKG_APB_TIMER0_EN               (BIT(1))
#define 	CKG_APB_SYST_EN                 (BIT(0))

#define 	CKG_APB_SPIC_EN		CKG_APB_SFC_EN

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

#define 	REG_AP_AHB_BASE                  0X10000000
#if 0
#define 	REG_AHB_PERI_CLK_CTRL            (REG_AP_AHB_BASE+0x0000)
#define 	CKG_AHB_SPIC_EN                  (BIT(15))
#define 	CKG_AHB_HSLOCK_EN                (BIT(14))
#define 	CKG_AHB_AES_EN                   (BIT(12))
#define 	CKG_AHB_USB2_0_EN                (BIT(10))
#define 	CKG_BGM_EN                       (BIT(9))
#define 	CKG_SPIC_SSI_EN                  (BIT(8))
#define 	CKG_GMAC_EB				         (BIT(7))
#define 	CKG_AXI_DMA_AUTO_GATE_SEL        (BIT(5))
#define 	CKG_AXI_DMA_FORCE_EN             (BIT(3))
#define 	CKG_AXI_DMA_EN                   (BIT(1))
#define 	CKG_AP_SYS_ARCH_EN               (BIT(0))
#endif

#define 	REG_AHB_PERI_CLK_CTRL            (REG_AP_AHB_BASE+0x0000)
#define 	CKG_SDIO_DLLREG_EN               (BIT(21))
#define 	CKG_SDIO_EN               		 (BIT(20))
#define 	CKG_SYS_SDIO_EN               	 (BIT(19))
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
#define 	REG_AP_AHB_PERI_SOFT_RST         (REG_AP_AHB_BASE+0x004)
#define 	HSLOCK_SOFT_RST                  (BIT(11))
#define 	SPIC_SOFT_RST                    (BIT(14))
#define 	AES_SOFT_RST                     (BIT(8))
#define 	USB2_0_AHB_SOFT_RST              (BIT(6))
#define 	USB2_0_UTMI_SOFT_RST             (BIT(4))
#define 	USB2_0_SOFT_RST                  (BIT(2))
#define 	DMA_SOFT_RST                     (BIT(0))
#define 	REG_CPU_CLK_CTRL2                (REG_AP_AHB_BASE+0x0048)

#define 	REG_AP_AHB_MISC                  (REG_AP_AHB_BASE+0x0064)

#define 	REG_UART0_BASE					 (0x18400000)
#define		REG_UART1_BASE					 (0x18300000)
#define         REG_UART2_BASE                                   (0x18200000)
#define 	REG_TIMER0_BASE				 	 (0x1A000000)

#define 	REG_SYS_TIMER_BASE				 (0x1A700000)
#define 	REG_SYS_TIMER_VALUE				 (REG_SYS_TIMER_BASE + 0x4)
#define 	get_sys_timer_val()				 __raw_readl(REG_SYS_TIMER_VALUE)

#define 	REG_AP_SYS_CLK_CFG				(0x19F00200)
#define		CLK_AP_APB_CFG					(REG_AP_SYS_CLK_CFG + 0x20)//0: 24m	1: 64m	2: 100m default: 2'h0
#define		CLG_USB0_UTMI0_CFG				(REG_AP_SYS_CLK_CFG + 0x24)
#define		CLK_USB0_UTMI1_CFG				(REG_AP_SYS_CLK_CFG + 0x28)
#define 	CLK_SPIC_SSI_CFG				(REG_AP_SYS_CLK_CFG + 0x38)//0: 24m	1: 64m	2: 91_4m	default: 2'h0
#define     CLK_UART0_CFG                   (REG_AP_SYS_CLK_CFG + 0x60)//0: 24m     1: 64m  2: 100m         3: 128m         default: 2'h0
#define     CLK_UART1_CFG                   (REG_AP_SYS_CLK_CFG + 0x64)


#define     REG_VDSP_SYS_APB_REG            (0x20000000)
#define     VDSP_CLK_RST_CTRL               (REG_VDSP_SYS_APB_REG + 0x20)
#define     CKG_VDSP_SYS_SEL(x)             (((x) & 3) << 0)

#define		REG_TOP_CLK_GEN					(0x28100200)
#define		CLK_GLB_APB_CFG					(REG_TOP_CLK_GEN + 0x20)//0: 24m	1: 64m	2: 100m 	3: 200m 	default: 2'h0
#define		CLK_AP_SYS_CFG					(REG_TOP_CLK_GEN + 0x30)//0: 24m	1: 100m 2: 200m 	default: 2'h0

#define 	REG_GLB_APB_BASE				(0x28000000)
#define 	STRAP_PIN_STATUS				(REG_GLB_APB_BASE + 0x6C)
#define 	USB_DL_MODE						(BIT(0))
#define 	SFC_BOOT_MODE					(BIT(1))
#define 	CPU_BOOT_FREQ					(BIT(2))


//DMA CID defination
#define SPIC_RX_RID					20
#define SPIC_TX_RID					21
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

#define 	VOU_SRAM						(0x38a00000)
#define 	HEAP_BASE						0x38A53000
#define		HEAP_LIMIT						0x38A55000

#define 	get_strap_status()				__raw_readl(STRAP_PIN_STATUS)
#define 	NOR_FLASH_BOOT					0
#define 	NAND_FLASH_BOOT					1


#define SFC_RX_SAMPLE_DELAY				1

#endif

