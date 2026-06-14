#ifndef __CPU_SCHED_H__
#define __CPU_SCHED_H__


/* structure cpu schedule list. */
int fast_boot_init(u32 sfc, u32 aes, u32 lzma, u32 hash);
int sched_fast_boot(struct boot_header *header);

#endif
