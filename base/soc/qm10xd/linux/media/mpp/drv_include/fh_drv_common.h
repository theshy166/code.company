#ifndef __FH_DRV_COMMON_H_
#define __FH_DRV_COMMON_H_

typedef struct
{
	unsigned int base;
	void * vbase;
	unsigned int size;
}MEM_INFO;

typedef struct
{
	unsigned int width;
	unsigned int height;
}FH_PIC_SIZE;
#endif
