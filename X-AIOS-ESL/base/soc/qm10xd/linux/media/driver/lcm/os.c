#include "data_type.h"
#include "os_include.h"
#include <linux/delay.h>

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
#if defined(OS_LINUX)
    udelay(us);
    //msleep(us);
    //_delayUsOSNone(us);

#elif defined(OS_RTT)

#elif defined(OS_NONE)
    _delayUsOSNone(us);
#endif
}

void delayMs(uint32_t ms)
{
#if defined(OS_LINUX)
    mdelay(ms);
#elif defined(OS_RTT)
    rt_thread_mdelay(ms);
#elif defined(OS_NONE)
    delayUs(1000 * ms);
#endif
}

void sleepMs(uint32_t ms)
{
#if defined(OS_LINUX)
    msleep(ms);
#elif defined(OS_RTT)

#elif defined(OS_NONE)
    delayUs(1000 * ms);
#endif
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
#if defined(OS_LINUX)
    return kmalloc(size, GFP_KERNEL);
#elif defined(OS_RTT)
    return rt_malloc(size);
#elif defined(OS_NONE)
    return NULL;
#endif
}

int32_t os_strcmp(const char *str1, const char *str2)
{
#if defined(OS_LINUX)
    return strcmp(str1, str2);
#elif defined(OS_RTT)
    return 0;
#elif defined(OS_NONE)
    return NULL;
#endif
}

char * os_strcpy(char *dest, const char *src)
{
#if defined(OS_LINUX)
    return strcpy(dest, src);
#elif defined(OS_RTT)
    return 0;
#elif defined(OS_NONE)
    return NULL;
#endif
}

void *os_memcpy(void *str1, const void *str2, uint32_t n)
{
#if defined(OS_LINUX)
    return memcpy(str1, str2, n);
#elif defined(OS_RTT)
    return 0;
#elif defined(OS_NONE)
    return NULL;
#endif
}

void *os_memset(void *str, int32_t c, uint32_t n)
{
#if defined(OS_LINUX)
    return memset(str, c, n);
#elif defined(OS_RTT)
    return 0;
#elif defined(OS_NONE)
    return NULL;
#endif
}

uint64_t do_div64_u64(uint64_t dividend, uint64_t divisor)
{
#if defined(OS_LINUX)
    return div_u64(dividend, divisor);
#elif defined(OS_RTT)
    return 1;
#elif defined(OS_NONE)
    return NULL;
#endif
}





/**----------------------------------------------------------------------------*
*           Compiler Flag                       *
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
 }
#endif
/**---------------------------------------------------------------------------*/


