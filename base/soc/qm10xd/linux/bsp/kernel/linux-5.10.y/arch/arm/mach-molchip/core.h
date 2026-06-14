bool molchip_smp_init_ops(void);

extern const struct smp_operations molchip_smp_dt_ops;

extern void molchip_cpu_die(unsigned int cpu);


u32 molchip_sdram_self_refresh(u32 sdr_base);
extern unsigned int molchip_sdram_self_refresh_sz;


