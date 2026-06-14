#ifndef __ISP_DEF_H__
#define __ISP_DEF_H__

// #define FH_UINT32 unsigned int
// #define FH_SINT32 int
// #define FH_UINT8  unsigned char
// #define FH_SINT8  char
// #define FH_UINT16 unsigned short
// #define FH_SINT16 short
// #define FH_VOID void
// #define u32 FH_UINT32
// #define FH_BOOL int
// #define __iomem
// #define __force

// static inline void __raw_writel(u32 w, volatile void __iomem *addr) {
//     __asm__ __volatile__("st.di%U1	%0, %1\n" : : "r"(w), "m"(*(volatile u32 __force *) addr) : "memory");
// }

// static inline u32 __raw_readl(const volatile void __iomem *addr) {
//     u32 w;

//     __asm__ __volatile__("ld.di%U1	%0, %1\n" : "=r"(w) : "m"(*(volatile u32 __force *) addr) : "memory");
//     return w;
// }

// #define readl __raw_readl
// #define writel __raw_writel
// #define ST32(addr,data)		            writel(data, (volatile void *)addr)
// #define LD32(addr)                      readl((volatile void *)addr)
// #define SET_ISP_REG(addr,data)          ST32(addr,data)
// #define SET_ISP_REG_M(addr, data, mask) ST32((addr),((LD32((addr))&(~(mask))) | ((data)&(mask))))
// #define GET_ISP_REG(addr)               LD32(addr)

// // 控制ISP打印相关的东西
// #define ISP_LOG printf
// #define ISP_DEBUG
#ifdef ISP_DEBUG
// #define ISP_PRT printf
#define ISP_TRACE printf("%s %d\n", __func__, __LINE__);
#else
#define ISP_TRACE
#define ISP_PRT(...)
#endif

// ISP错误码
enum ISP_ERRNO
{
    ISP_RET_OK       = 0,
    ISP_NULL_PTR     = 0x1000,
    ISP_BAD_ADDR     = 0x1001,
    ISP_PARA_INVALID = 0x1002,
};


#define ISP_NPTR_CHK(x) \
do\
{\
    if (x == 0)\
        return ISP_NULL_PTR;\
} while (0);

#define ISP_CHK_RANGE(name, val, min, max) \
do\
{\
    if (vale < main || val > max)\
    {\
        ISP_PRT("%s:%s is out of range, need [%d~%d]", __func__, name, min, max);\
        return ISP_PARA_INVALID;\
    }\
} while (0);
#endif  // __ISP_DEF_H__
