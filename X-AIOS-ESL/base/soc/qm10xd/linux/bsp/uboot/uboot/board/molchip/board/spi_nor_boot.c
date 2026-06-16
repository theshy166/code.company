#include <config.h>
#include "type_use.h"
#include "sstring.h"
#include "mci.h"
#include "glb_reg.h"
#include "system.h"
#include "uart_drv.h"
#include "spic.h"
#include "spi_nor_flash.h"
#include "secure.h"
//#include "dma.h"
#include "pkcs5.h"
#include "rsa.h"
#include "bignum.h"
#include "lzma.h"
#include "efuse_drv.h"
#include "aes.h"
#include "spic_reg.h"
#include "mc_hw_rsa.h"
#include "hash.h"
#include "cpu_sched.h"


#define CPU_SYNC_TIMEOUT	500000000
#define CORE1_SCHED_DONE	(0x4c696e79)
volatile unsigned int  (*core1_done) = (volatile unsigned int *)(0x30048014);

typedef enum {
	NONE_SECURE_MODE = 0,
	AES_ONLY_SECURE_MODE,
	RSA_ONLY_SECURE_MODE,
	FULL_SECURE_MODE
} SECURE_BOOT_MODE_E;

volatile unsigned int secure_mode;

//volatile unsigned int core1_done = 0;
struct boot_header *sw_fsb_header = NULL;

 bool spi_cpu_sched_boot(struct boot_header *header)
{
	int ret = true;
	
	if (rsa_sel()) {
		hash_enable((header->sel >> 29) & 0x3);
		rsa_enable((header->sel >> 1) & 0x7);
	
		ret = public_key_verify(header);
		
		if (ret == false) 
			uart_puts("HCF\r\n");
	}

	/* release core1 to run sched_fast_boot. */
	sw_fsb_header = header;
	*core1_done = 0;
	heap_reinit();
	smp_reset_core1();
	smp_boot_secondary(1);

	if (rsa_sel()) 
		ret = signature_verify(header);
	
	return ret;
}
 
int spi_sw_fastboot(void)
{
	bool ret = 0;
	uart_puts("spi_sw_fastboot\r\n");
	if (sw_fsb_header) {
		ret = sched_fast_boot(sw_fsb_header);
		if (0 == ret) {
			*core1_done = CORE1_SCHED_DONE;
		} else {
		}
	} else {
		uart_puts("CSF\r\n");
	}
	return ret;
}

int spi_cpu_sched_finish(void)
{
	int ret = 0;
	u32 time_start, time_now;

	time_start = get_sys_timer_val();
	while (CORE1_SCHED_DONE != *core1_done) {
		time_now = get_sys_timer_val();
		if (time_now - time_start > CPU_SYNC_TIMEOUT) {
			ret = -1;
			break;
		}
	};
	if (CORE1_SCHED_DONE == *core1_done) {
		smp_reset_core1();
	}

	return ret;
}
