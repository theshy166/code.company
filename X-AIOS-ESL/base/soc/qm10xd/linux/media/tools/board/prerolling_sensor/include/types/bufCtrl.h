#ifndef _BUFCTRL_H_
#define _BUFCTRL_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef struct mem_desc {
	unsigned int base;
	void        *vbase;
	unsigned int size;
	unsigned int align;
} MEM_DESC;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
