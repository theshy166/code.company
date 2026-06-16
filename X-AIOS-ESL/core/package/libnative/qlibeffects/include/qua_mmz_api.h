
#ifndef __QUA_MMZ_API_H__
#define __QUA_MMZ_API_H__

int QUA_SYS_MmzAlloc_Cached(unsigned long *pu32PhyAddr, void **ppVirtAddr,
        const char *strMmb, const char *strZone, unsigned int u32Len);
int QUA_SYS_MmzAlloc(unsigned long *pu32PhyAddr, void **ppVirtAddr,
        const char *strMmb, const char *strZone, unsigned int u32Len);

int QUA_SYS_MmzFree(unsigned long u32PhyAddr, void *pVirtAddr);

int QUA_SYS_MmzFlush(unsigned long pu32PhyAddr, void *ppVirtAddr, unsigned int u32Len);

//Todo: Alloc fail
int QUA_req_mem(unsigned int size, unsigned long *phyAddr, void **pVirAddr);

#endif
