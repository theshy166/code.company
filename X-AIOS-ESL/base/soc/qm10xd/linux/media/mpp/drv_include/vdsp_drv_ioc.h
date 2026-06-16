#ifndef _MC_VDSP_H
#define _MC_VDSP_H

#include "types/type_def.h"
#include "fh_vdsp_mpipara.h"

/*          ioctls                  */
#define IOC_VDSP_QUERY              _IOWR('v', 101, FH_UINT32)
#define IOC_VDSP_REQUEST            _IOWR('v', 102, SVP_DSP_IMAGE_INFO_S)
#define IOC_VDSP_REQUEST_TIMEOUT    _IOWR('v', 104, SVP_DSP_IMAGE_INFO_S)
#define IOC_VDSP_GET_PHYADDR        _IOWR('v', 103, FH_UINT32)
#define IOC_VDSP_SET_SRAM_INFO      _IOWR('v', 105, DSP_SRAM_INFO_S)
#define IOC_VDSP_SET_CONFID         _IOWR('v', 106, FH_UINT32)
#define VDSP_QUEUE_LENGTH   50

#define IOC_VDSP_SET_INTRINSIC_MATRIX    _IOWR('v', 107, FH_UINT32)
#define IOC_VDSP_GET_INTRINSIC_MATRIX    _IOWR('v', 108, FH_UINT32)
#define IOC_VDSP_SET_EXTRINSIC_MATRIX    _IOWR('v', 109, FH_UINT32)
#define IOC_VDSP_GET_EXTRINSIC_MATRIX    _IOWR('v', 110, FH_UINT32)

#define IOC_VDSP_PERSP_TRANS             _IOWR('v', 111, SVP_DSP_PERSP_TRANS_S)

typedef struct fySVP_DSP_REQUEST_ENTITY_TO_S {
    SVP_DSP_SRC_IMAGE_S stSrcImg;
    SVP_DSP_DST_IMAGE_S stDstImg;
    FH_UINT32 u32ToMs;
}SVP_DSP_IMAGE_INFO_S;

typedef struct fyDSP_ADDR_INFO_S
{
    SVP_DSP_MEM_TYPE_E enMemType;
    FH_UINT32 u32PhyAddr;
}DSP_ADDR_INFO_S;


typedef struct fyDSP_SRAM_INFO_S
{
    FH_UINT16 u16Width;
    FH_UINT16 u16Height;
    FH_UINT32 u32PhyAddr;
    FH_UINT32 u32MemSize;
    FH_UINT32 u32Version;
} DSP_SRAM_INFO_S;


typedef struct fySVP_DSP_PERSP_TRANS_S {
    SVP_DSP_IMAGE_INFO_S stImgInfo;
    FH_FLOAT *pafMatrix;
    FH_UINT32 u32Size;
}SVP_DSP_PERSP_TRANS_S;

#endif /* _LINUX_MC_VDSP_H */


