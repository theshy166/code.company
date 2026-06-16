/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 */
#ifndef __XOS_TYPES_H__
#define __XOS_TYPES_H__

#undef ABS
#undef MAX
#undef MIN
#undef TRUE
#undef FALSE
#undef NULL
#undef PNULL
#undef SIZEOF
#undef NIL

#define TRUE	(0 == 0)
#define FALSE	(0 == 1)

#define NIL		((void *) 0)
#define NULL	NIL
#define PNULL	NIL
#define UNUSED(x) (void)(x)

typedef int boolean;
typedef int BOOLEAN;
typedef int BOOL;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned char uint8;
typedef unsigned short uint16;

typedef unsigned int u32;
typedef unsigned int uint;
typedef unsigned int uint32;

typedef unsigned long long u64;
typedef unsigned long long uint64;

typedef unsigned int uint_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
// typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned int uint_t;
typedef unsigned char uint_8;
typedef unsigned short uint_16;
typedef unsigned int uint_32;
typedef unsigned long long uint_64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed int int_32;
typedef signed long long int64;

typedef signed char int8_t;
typedef signed short int16_t;
// typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef int INT;
typedef char INT8;
typedef short INT16;
typedef int INT32;
typedef long long INT64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef char S8;
typedef short S16;
typedef int S32;
typedef long long S64;

typedef char s8;
typedef short s16;
typedef int s32;

typedef unsigned int size_t;
typedef int ssize_t;

typedef unsigned char BYTE;
typedef char CHAR;
typedef unsigned short WCHAR;
typedef unsigned short WORD;
typedef signed int WORD32;
typedef unsigned int UWORD32;
typedef unsigned long DWORD;

typedef volatile unsigned char REG8;
typedef volatile unsigned short REG16;
typedef volatile unsigned int REG32;


#define BIT_0               0x00000001
#define BIT_1               0x00000002
#define BIT_2               0x00000004
#define BIT_3               0x00000008
#define BIT_4               0x00000010
#define BIT_5               0x00000020
#define BIT_6               0x00000040
#define BIT_7               0x00000080
#define BIT_8               0x00000100
#define BIT_9               0x00000200
#define BIT_10              0x00000400
#define BIT_11              0x00000800
#define BIT_12              0x00001000
#define BIT_13              0x00002000
#define BIT_14              0x00004000
#define BIT_15              0x00008000
#define BIT_16              0x00010000
#define BIT_17              0x00020000
#define BIT_18              0x00040000
#define BIT_19              0x00080000
#define BIT_20              0x00100000
#define BIT_21              0x00200000
#define BIT_22              0x00400000
#define BIT_23              0x00800000
#define BIT_24              0x01000000
#define BIT_25              0x02000000
#define BIT_26              0x04000000
#define BIT_27              0x08000000
#define BIT_28              0x10000000
#define BIT_29              0x20000000
#define BIT_30              0x40000000
#define BIT_31              0x80000000
#define BIT_(__n)           (1U<<(__n))
#define BIT(__n)            (1U<<(__n))
#define MASK_(__n,__w)      (((1U<<(__w))-1)<<(__n))
#define VALUE_(__n,__v)     ((__v)<<(__n))

/*rounds a number up to the nearest multiple of 8 */
#define RND8( _x )       ((((_x) + 7) / 8 ) * 8 )


#define  UPCASE( _c ) ( ((_c) >= 'a' && (_c) <= 'z') ? ((_c) - 0x20) : (_c) )

#define  DECCHK( _c ) ((_c) >= '0' && (_c) <= '9')

#define  DTMFCHK( _c ) ( ((_c) >= '0' && (_c) <= '9') ||\
                       ((_c) >= 'A' && (_c) <= 'D') ||\
                       ((_c) >= 'a' && (_c) <= 'd') ||\
					   ((_c) == '*') ||\
					   ((_c) == '#'))

#define  HEXCHK( _c ) ( ((_c) >= '0' && (_c) <= '9') ||\
                       ((_c) >= 'A' && (_c) <= 'F') ||\
                       ((_c) >= 'a' && (_c) <= 'f') )

#define  ARR_SIZE( _a )  ( sizeof( (_a) ) / sizeof( (_a[0]) ) )


#define REG8(x)                                     (*((volatile uint8*) (x)))
#define REG16(x)                                    (*((volatile uint16*) (x)))
#define REG32(x)                                    (*((volatile uint32 *)(x)))
#define REG64(x)                                    (*((volatile uint64 *)(x)))

#define write64(addr, val)                          (*(volatile unsigned long long *)(addr) = (val))
#define read64(addr)                                (*(volatile unsigned long long *)(addr))

#define write32(addr, val)                          (*(volatile unsigned int *)(addr) = (val))
#define read32(addr)                                (*(volatile unsigned int *)(addr))

#define write16(addr, val)                          (*(volatile uint16 *)(addr) = (val))
#define read16(addr)                                (*(volatile uint16 *)(addr))

#define REG_OR(reg_addr, value)    (*(volatile uint32 *)(reg_addr) |= (uint32)(value))
#define REG_AND(reg_addr, value)   (*(volatile uint32 *)(reg_addr) &= (uint32)(value))
#define REG_GET(reg_addr)          (*(volatile uint32 *)(reg_addr))
#define REG_SET(reg_addr, value)   (*(volatile uint32 *)(reg_addr)  = (uint32)(value))

#undef ALIGN
#undef ARRAY_SIZE
#define OFFSETOF(s,m)                   ((UINT32)&(((s *)0)->m)) // Get the address offset of the specified member.
#define ALIGN(val,exp)                  (((val) + ((exp)-1)) & ~((exp)-1))
#define ARRAY_SIZE(a)                   (sizeof(a)/sizeof(a[0]))
#define BOUND(x, min, max)              ( (x) < (min) ? (min) : ((x) > (max) ? (max):(x)) )

#define MSB(x)                          (((x) >> 8) & 0xff) // most signif byte of 2-byte integer
#define LSB(x)                          ((x) & 0xff)        // least signif byte of 2-byte integer

#define SWAPT(a,b,type)                	{type v; v = a; a = b; b = v; }

#define ABS(x)                          ((x<0)?(-(x)):(x))
#define MAX(a,b)                        (((a) > (b)) ? (a) : (b))
#define MIN(a,b)                        (((a) < (b)) ? (a) : (b))

#define ALIGN(val,exp)                  (((val) + ((exp)-1)) & ~((exp)-1))
#define ARRAY_SIZE(a)                   (sizeof(a)/sizeof(a[0]))

// C++ needs to know that types and declarations are C, not C++.
#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

#endif /*__XOS_TYPES_H__*/
