#ifndef __ECDC_H__
#define __ECDC_H__

#include "fh_std_typedef.h"

typedef struct
{
    FH_UINT16 u16Width;     // 图像的宽度 | [0~0xffff]
    FH_UINT16 u16Height;     // 图像的高度 | [0~0xffff]
}ECDC_SIZE_S;

typedef struct
{
    FH_UINT32          u32PipeId;
    ECDC_SIZE_S        stSize;
    FH_UINT32          u32Addr;
    FH_UINT8           u8SourceBpp;
    FH_UINT8           u8TargetBpp;
    FH_UINT8           u8RgbMode;
    FH_UINT8           u8BayerPattern;
    FH_UINT8           u8Bypass;
    FH_UINT8           u8CirEn;
    FH_UINT16          u32CirDepth;
}ECDC_BASE_CFG_S;

typedef struct
{
    FH_UINT32    u32EcParam[16];
    FH_UINT32    u32DcParam[13];
}ECDC_PARAM_S;

typedef struct
{
    FH_UINT32 u32TotalByteSum;
    FH_UINT32 u32EncodeRate;
    FH_UINT32 u8AverageTh;
    FH_UINT32 u8AddCount;
}ECDC_EC_INFO_S;

typedef struct
{
    ECDC_EC_INFO_S st1Frame;
    ECDC_EC_INFO_S st10Frame;
    // RaweSizeT   stSize;
    FH_UINT32   u32OriginSize;
    // FH_UINT8    u8DataWidth;
}ECDC_EC_ADV_INFO_S;

FH_SINT32 Ecdc_GetParam(ECDC_BASE_CFG_S* pstCfg, ECDC_PARAM_S* pstParam);
FH_SINT32 Ecdc_CopyDcFromEcParam(ECDC_PARAM_S* pstParam);
FH_SINT32 Ecdc_InitEcRate(ECDC_EC_ADV_INFO_S* stEcAdvInfo, FH_UINT32 u32EcOriginSize);
FH_SINT32 Ecdc_CalcEcRate(ECDC_EC_ADV_INFO_S* stEcAdvInfo, FH_UINT32 u32CurrTotalByte);
#endif  // __ECDC_H__