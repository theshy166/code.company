/*
 * Copyright (c) 2022  quaming
 *
 */

#ifndef __QM_AAAA_H__
#define __QM_AAAA_H__
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include <stdint.h>

/* function  in qm_drv_ext  */
uint32_t _test_x(uint32_t random);
void _test_y(void);
uint32_t _test_z(void);
inline static bool qm_drv_shakehands(void)
{
    uint32_t random = 0xFFFFFFFF;
    static uint32_t salt = 0x5a;
    srand(time(NULL) + salt);
    random = rand();
    salt = salt * 2;
    random = _test_x(random);
    if(random != _test_z())
    {
        _test_y();
    }
    return true;
}

/* function in libsysutils.so */
/* uint64_t qm_drv_ext_random(uint32_t random); */

#endif //__QM_AAAA_H__
