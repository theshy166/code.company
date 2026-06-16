#include <common.h>
#include <mach/spl.h>

#define IMAGE_ADDR 0x40008000
#define	IMAGE_DTB  0x41000000
#ifdef CONFIG_ARM64
static void do_nonsec_virt_switch(void)
{
	smp_kick_all_cpus();
	dcache_disable();	/* flush cache before swtiching to EL2 */
}

#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
static void switch_to_el1(void)
{
	armv8_switch_to_el1((u64)IMAGE_DTB, 0, 0, 0,
				    IMAGE_ADDR,
				    ES_TO_AARCH64);
}
#endif
#endif

/*******************************************************************/
/* spl-bootm - boot application image from image in memory */
/*******************************************************************/

void spl_bootm(void)
{
#ifdef CONFIG_ARM64
	image_header_t *img_addr = (image_header_t *)IMAGE_ADDR;

	void (*kernel_entry)(void *fdt_addr, void *res0, void *res1,
			void *res2);

	kernel_entry = (void (*)(void *fdt_addr, void *res0, void *res1,
				void *res2))IMAGE_ADDR;

	debug("## Transferring control to Linux (at address %lx)...\n",
		(ulong) kernel_entry);

	printf("\nStarting kernel ...\n\n");
	cleanup_before_linux();

#ifdef CONFIG_ARMV8_PSCI
		armv8_setup_psci();
#endif
		do_nonsec_virt_switch();

#ifdef CONFIG_BOOT_FIRMWARE
		//mc_bringup_arc();
#endif

#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
		armv8_switch_to_el2((u64)IMAGE_DTB, 0, 0, 0,
				    (u64)switch_to_el1, ES_TO_AARCH64);
#else
		armv8_switch_to_el2((u64)IMAGE_DTB, 0, 0, 0,
					    IMAGE_ADDR,
					    ES_TO_AARCH64);
#endif
#else
	unsigned long machid = 0xffffffff;
	void (*kernel_entry)(int zero, int arch, uint params);
	unsigned long r2;

	kernel_entry = (void (*)(int, int, uint))IMAGE_ADDR;
#ifdef CONFIG_CPU_V7M
	ulong addr = (ulong)kernel_entry | 1;
	kernel_entry = (void *)addr;
#endif

	debug("## Transferring control to Linux (at address %08lx)" \
		"...\n", (ulong) kernel_entry);

	printf("\nStarting kernel ...\n\n");
	cleanup_before_linux();

	r2 = (unsigned long)IMAGE_DTB;

	kernel_entry(0, machid, r2);
#endif
}


