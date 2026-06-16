#include "../lcm_module/include/data_type.h"
#include "os_include.h"
#include <common.h>
#include <stdlib.h>

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef     __cplusplus
extern     "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
#if defined(OS_NONE)
static volatile int _delayUsOSNone(uint32_t us)
{
    volatile uint32_t i;
    volatile int a=0;

    for(i = 0;i < us/10;i++) {
        a = a/99999999+i+1;
    }

    return a;
}
#endif

void delayUs(uint32_t us)
{
    udelay(us);
}

void delayMs(uint32_t ms)
{
    mdelay(ms);
}

void sleepMs(uint32_t ms)
{
    mdelay(ms);
}

int os_mutex_init(os_mutex_t *mutx)
{
return 0;
}

void os_mutex_deinit(os_mutex_t *mutx)
{
return;
}

void os_mutex_lock(os_mutex_t *mutx)
{
return;
}

void os_mutex_unlock(os_mutex_t *mutx)
{
return;
}

void *os_k_malloc(uint32_t size, int flags)
{
    return (void *)malloc(size);
}

int32_t os_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

char * os_strcpy(char *dest, const char *src)
{
    return strcpy(dest, src);
}

void *os_memcpy(void *str1, const void *str2, uint32_t n)
{
    return memcpy(str1, str2, n);
}

void *os_memset(void *str, int32_t c, uint32_t n)
{
    return memset(str, c, n);
}

uint64_t do_div64_u64(uint64_t dividend, uint64_t divisor)
{
    return 0;
}


/**----------------------------------------------------------------------------*
*           Compiler Flag                       *
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
 }
#endif
/**---------------------------------------------------------------------------*/


