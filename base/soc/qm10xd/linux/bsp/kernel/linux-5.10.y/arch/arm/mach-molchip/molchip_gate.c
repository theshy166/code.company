#include <linux/io.h>
#include "molchip_pm.h"

static unsigned int sleep_gate_cfg[] = {
	#include "soc_init_reg_list_gate.h"
};

int mol_sleep_gate_cfg(void)
{
	struct reg_info_t *dbg = NULL;
	unsigned int cnt, i;
	void __iomem * addr = NULL;
	unsigned int val;
	unsigned int base_addr = 0, off_addr = 0;

	cnt = sleep_gate_cfg[0];
	dbg = (struct reg_info_t *)&sleep_gate_cfg[1];

	for (i = 0; i < cnt; i++, dbg++) {
		if (mol_ioremap_reuse(base_addr, dbg->reg_addr) == false) {
			base_addr = mol_ioremap_base_get(dbg->reg_addr);
			if (addr != NULL) {
				iounmap(addr);
			}
			addr = ioremap(base_addr, PAGE_SIZE);
			if (!addr)
				return -ENOMEM;
		}

		off_addr = mol_ioremap_off_get(dbg->reg_addr);
		val = readl(addr + off_addr);

		val &= ~dbg->mask;
		val |= dbg->regv;
		writel(val, addr + off_addr);

	}
	iounmap(addr);
	return 0;

}

int mol_sleep_gate_cfg_to_iram(void *iram_base)
{
	struct reg_info_t *dbg = NULL;
	struct reg_info_t *iram = NULL;
	unsigned int cnt, i;

	if (iram_base == NULL)
		return -EINVAL;

	cnt = sleep_gate_cfg[0];
	dbg = (struct reg_info_t *)&sleep_gate_cfg[1];
	iram = (struct reg_info_t *)iram_base;

	for (i = 0; i < cnt; i++, dbg++, iram++) {
		iram->reg_addr = dbg->reg_addr;
		iram->mask = dbg->mask;
		iram->regv = dbg->regv;
	}
	iram->reg_addr = 0;	//end flag

	return 0;

}

