#include <asm/mach/arch.h>

#include "core.h"
extern void __init molchip_suspend_init(void);
static const char * const v2m_dt_match[] __initconst = {
	"arm,molchip",
	NULL,
};

DT_MACHINE_START(VEXPRESS_DT, "ARM-QUAMING")
	.dt_compat	= v2m_dt_match,
	.l2c_aux_val	= 0x00400000,
	.l2c_aux_mask	= 0xfe0fffff,
	.smp		= smp_ops(molchip_smp_dt_ops),
	.smp_init	= smp_init_ops(molchip_smp_init_ops),
	.nr_irqs	= 128,
#ifdef CONFIG_ARCH_MOLCHIP_SUSPEND
	.init_machine	= molchip_suspend_init,
#endif
MACHINE_END
