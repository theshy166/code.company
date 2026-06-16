#ifndef _DDR_H_
#define _DDR_H_

#ifdef CONFIG_SDL_BUILD
void DRAM_param_store(u32 nvm_mod);
#else
void DRAM_Init(void);
#endif


#endif
