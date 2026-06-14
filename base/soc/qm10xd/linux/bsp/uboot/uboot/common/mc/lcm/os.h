#ifndef __OS_H__
#define __OS_H__

#include "../lcm_module/include/data_type.h"
#include "os_include.h"
#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
void delayUs(uint32_t us);
void delayMs(uint32_t ms);
void sleepMs(uint32_t ms);
void *os_k_malloc(uint32_t size, int flags);
int32_t os_strcmp(const char *str1, const char *str2);
char * os_strcpy(char *dest, const char *src);
void *os_memcpy(void *str1, const void *str2, uint32_t n);
void *os_memset(void *str, int32_t c, uint32_t n);

uint64_t do_div64_u64(uint64_t dividend, uint64_t divisor);

int os_mutex_init(os_mutex_t *mutx);
void os_mutex_deinit(os_mutex_t *mutx);
void os_mutex_lock(os_mutex_t *mutx);
void os_mutex_unlock(os_mutex_t *mutx);
uint64_t os_gettimestamp(void);

#if 0
int os_thread_mutex_init(os_thread_mutex_t *mutx);
void os_thread_mutex_deinit(os_thread_mutex_t *mutx);
void os_thread_mutex_lock(os_thread_mutex_t *mutx);
void os_thread_mutex_unlock(os_thread_mutex_t *mutx);
#endif
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

