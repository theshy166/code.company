#ifndef __SYSTEM_H_
#define __SYSTEM_H_


/* add for HASH Glitch issue */
#define HASH_GLITCH_EXIST		(0x47)

#define VPPU_G2D360_BASE		(0x27000000)

#define PLL_WAIT_TIMEOUT		500
#define SPL_BASE_ADDR			(VPPU_G2D360_BASE + 0x0000)
#define HEADER_LEN			0x400
#define START_ADDRESS			0x43000000
#define SPL_LENGTH			0x10000
#define SIGN_INFO_ADDR			(SPL_BASE_ADDR + 0x400)
#define IDLE_BUFFER				0x30060000

#define MAGICE_DATA			0x46524550
#define MAX_FILES_NUM		16

#define TRACE_CNT_MAX			(2)

#define TRACE_INIT			BIT(0)
#define TRACE_UART_DOWNLOAD		BIT(1)
#define TRACE_READ_HASH_FAIL		BIT(2)

#define TRACE_NOR_BOOT			BIT(4)
#define TRACE_NOR_FAIL			BIT(5)
#define TRACE_NOR_SUCCESS		BIT(6)

#define TRACE_NAND_BOOT			BIT(8)
#define TRACE_NAND_IDTY_SUCCESS		BIT(9)
#define TRACE_NAND_READ_SPL		BIT(10)
#define TRACE_NAND_READ_SUCCESS		BIT(11)

#define TRACE_NAND_IDTY_FAIL		BIT(12)
#define TRACE_NAND_READ_CACHE_FAIL	BIT(13)
#define TRACE_NAND_READ_FAIL		BIT(14)
#define TRACE_NAND_BOOT_FAIL		BIT(15)

#define TRACE_HASH_COMP_FAIL		BIT(16)
#define TRACE_RSA_DECRYT_FAIL		BIT(17)
#define TRACE_CBC_DECODE_FAIL		BIT(18)
#define TRACE_BOOT_SUCCESS		BIT(19)

#define TRACE_CBC_INIT_FAIL		BIT(20)
#define TRACE_CBC_PBKDF2_FAIL		BIT(21)

#define TRACE_SD_INIT_FAIL		BIT(24)
#define TRACE_SD_READ_FAIL		BIT(25)
#define TRACE_SD_BOOT_FAIL		BIT(26)
#define TRACE_SD_BOOT_SUCCESS		BIT(27)

#define TRACE_EMMC_INIT_FAIL		BIT(28)
#define TRACE_EMMC_READ_FAIL		BIT(29)
#define TRACE_EMMC_BOOT_FAIL		BIT(30)
#define TRACE_EMMC_BOOT_SECCESS		BIT(31)

#define TRACE1_HEADER_DECRYPT_FAIL	BIT(0)
#define TRACE1_LZMA_HW_CASCADE		BIT(1)
#define TRACE1_LZMA_SW_CASCADE		BIT(2)
#define TRACE1_SFC_RAM_TRANSCATION	BIT(3)

#define TRACE1_XMODEM_ENTRY		BIT(4)
#define TRACE1_XMODEM_PARM_IN		BIT(5)
#define TRACE1_XMODEM_SEC_VERI_IN	BIT(6)
#define TRACE1_XMODEM_UPDT_IN(n)	(1 << (((n) & 0x3) + 7))
#define TRACE1_XMODEM_SEC_VERI_FAILED	BIT(11)
#define TRACE1_XMODEM_SEC_VERI_PASS	BIT(12)
#define TRACE1_XMODEM_PROGRAM_FAILED	BIT(13)
#define TRACE1_XMODEM_PROGRAM_PASS	BIT(14)


#define TRACE_SD_MCI_INIT_FAIL		BIT(15)
#define TRACE_SD_READ_HEADER_FAIL	BIT(16)
#define TRACE_SD_MAGIC_NUM_FAIL		BIT(17)

#define TRACE1_PLL_DONE				BIT(18)
#define TRACE1_CORE1_ENTRY			BIT(19)
#define TRACE1_CORE1_SCHED_FAILED	BIT(20)

#define HW_CASCADE_FAST_BOOT	1
#define CPU_SCHED_FAST_BOOT		2
#define BOOT_INFO_OFFSET		0x170
#define	BIT_HEADER				BIT(0)

#define TIMER_CLK_FREQ			(24000000)
#define TIMER_US_CNT			(TIMER_CLK_FREQ/1000000)

#define ENTRY_BAD_ADDRESS		(0xdeadbeed)
#define CPU_SCHED_CORE1_RLS		(0x70)


/* chip frequency select. */
#define CHIP_FREQ_HIGHEST	(0x0)
#define CHIP_FREQ_MIDDLE	(0x1)
#define CHIP_FREQ_LOW		(0x2)

typedef enum {
	SYS_CLOCK_24MHZ = 24,
	SYS_CLOCK_333MHZ = 333,
	SYS_CLOCK_400MHZ = 400
} SYS_CLOCK_MHZ;

/*
 *@delay, [23:0], udelay;[31:24],valid flag. not 0,indicate validation.
 */
struct reg_info {
	unsigned int reg_addr;
	unsigned int value;
	unsigned int delay:24;
	unsigned int valid:8;
};
#define PERI_IF_CNT_MAX			(51)
#define GLB_CLK_CNT_MAX			(20)

#define ACC_EMMC_PARTITION_USER			0
#define ACC_EMMC_PARTITION_BOOT			1
#define ACC_EMMC_PARTITION_BOOT_SW		2
/*
 *@sel,
 *@lzma, indicate whether use LZMA module.
 *@cascade, indicate whether use full-hw-cascade fastboot scheme.
 */
struct boot_header {
	u32 magic_num;
	u32 addr;
	u32 length;
	u8 clk_reconfig;//0
	u8 ctrl_reconfig;//1
	u8 lzma;//1
	u8 cascade;//1
	/*bit0 ras sel bit1:3 rsa clk_sel bit4:16 rsa done bit17:28 hash done bit29:30 hash clk bit 31 casecade */
	u32 sel;
	u32 decode_length;
	u32 nand_data_len;
	u32 data_src;
	u32 data_dst;
	u32 data_len;
	u32 image_src;
	u32 image_dst;
	u32 image_len;
	u32 lzma_zip_len;
	/* bit0:15 lzma done time bit 16 lzma debug use*/
	u32 lzma_ctrl;//bit 0:16 lzma done 16:19 core dir 20:22 core se   lbit23 debug use
	u32 lzma_decode_reg;
	u32 lzma_dma_ctrl_cfg;
	u32 aes_src;
	u32 aes_dst;
	u32 lzma_src;
	u32 lzma_dst;

	/* CPU schedule fastboot. */
	u16 granularity;
	u8 cpu_sched;
	u8 rev0;
	u32 rev1;
	u32 done_delay;

	/* offset: 0x60, size: 76bytes for spi_nand_info. */
	u32 nand_info[19];

	struct reg_info clk_seq[GLB_CLK_CNT_MAX];
	struct reg_info if_seq[PERI_IF_CNT_MAX];
};

void mc_udelay(u32 us);
void timer_enable(void);
void mc_delay_ms(u32 delay);
u32 system_boot_mode(void);
//void timestamp_record(unsigned int seq);

bool usb_download_en(void);
bool cpu_high_freq_en(void);
void jump_to(u32 address);
void secure_boot(u32 address);
void debug_boot(void);
void ctrl_config(struct reg_info *regs, unsigned int cnt);
u32 system_xmodem_mode(void);
int emmc_boot(void);
int emmc_read_image(void);

void share_memory_select(void);
int sfc_nor_online(void);
void smp_boot_secondary(unsigned int path);
void smp_reset_core1(void);
void secondary_startup_arm(void);
void secondary_startup(void);

unsigned int sfc_refclock_select(void);
int spi_sw_fastboot(void);
int spi_cpu_sched_finish(void);
u32 aon_sys_clock_get(void);
unsigned int if_ip_clock_select(void);
unsigned int emmc_clock_select(void);
u32 xmodem_fast_entry(void);
extern bool spi_cpu_sched_boot(struct boot_header *header);
void heap_init(void);
void heap_reinit(void);
void sfree(void *addr);
void *smalloc(u32 size);

int spi_transaction_dma_read(u32 from, u32 len, u8 *read_buf);
int  sdhci_data_tran_done(u32 addr);



#endif
