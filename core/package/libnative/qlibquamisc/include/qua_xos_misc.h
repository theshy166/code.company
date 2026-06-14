/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/
/**
 * @file qua_xos_misc.h
 *
 */

#ifndef QUA_XOS_MISC_H
#define QUA_XOS_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

extern void xos_init_phase_1(void);
extern void xos_res_init_phase_1(void);
extern void xos_res_init_phase_2(void);
extern int  xos_qua_record_image(unsigned char * data, unsigned int size, void **new_ptr, int is_bg);
extern void xos_qua_boot_init(void);
extern void xos_qua_queue_init(int num);
extern int  xos_qua_uncompress_strings(unsigned char * dst, unsigned long * dst_len,
    const unsigned char * src, unsigned long src_len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*QUA_XOS_MISC_H*/
