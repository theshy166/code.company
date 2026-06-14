#ifndef _MC_APP_DMA64_H
#define _MC_APP_DMA64_H
#include "molchip.h"
#define DMA_INVALID_CH_MAGIC_NUMBER  0xffffffff
int mc_dma64_probe(struct mc_dma64_chip *chip);
int mc_dma_remove(struct mc_dma64_chip *chip);
void mcc_dma_llist_process_tasklet(long unsigned int data);

#endif
