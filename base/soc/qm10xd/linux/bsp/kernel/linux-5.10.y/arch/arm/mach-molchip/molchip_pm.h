#ifndef __MOLCHIP__PM_H__
#define __MOLCHIP__PM_H__

struct reg_info_t {
	unsigned int reg_addr;
	unsigned int regv;
	unsigned int mask;
};

static inline bool mol_ioremap_reuse(unsigned int f_addr, unsigned int r_addr)
{
	return (f_addr ^ r_addr) >> 12 ? false : true;
}

static inline unsigned int mol_ioremap_base_get(unsigned int addr)
{
	return (addr & ~0xfff);
}

static inline unsigned int mol_ioremap_off_get(unsigned int addr)
{
	return (addr & 0xfff);
}
/**IO cfg*/
int mol_sleep_io_cfg_init(void);
int mol_sleep_io_func(void);
int mol_wakeup_io_restore_state(void);

/*ip cfg*/
int mol_sleep_gate_cfg(void);
int mol_sleep_gate_cfg_to_iram(void *iram_base);

#endif