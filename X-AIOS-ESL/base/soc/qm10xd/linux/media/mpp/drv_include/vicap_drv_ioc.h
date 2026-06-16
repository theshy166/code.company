#ifndef __FY_VICAP_DRV_IOC_H__
#define __FY_VICAP_DRV_IOC_H__


/******************************************************************************
    Include other header files
 *****************************************************************************/
#include"../include/vicap/fh_vicap_mpi.h"
#include"../include/isp_ext/mipi_api.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/******************************************************************************
    Macros, Enums, Structures definition list
 *****************************************************************************/
typedef struct
{
    VI_DEV devId;
    FH_VICAP_DEV_ATTR_S* pstDevAttr;
}VICAP_IOC_DEV_ATTR_S;

typedef struct
{
    VI_DEV devId;
    FH_VICAP_VI_ATTR_S* pstViAttr;
}VICAP_IOC_VI_ATTR_S;

typedef struct
{
    VI_DEV devId;
    FH_VICAP_WDR_CHAN_E enWdrChan;
    FH_VICAP_CROP_CFG_S* pstCropCfg;
}VICAP_IOC_PIPE_CROP_S;

typedef struct
{
    FH_VICAP_SNS_IDX_E enSnsIdx;
    FH_UINT32 u32Clock;
}VICAP_IOC_SNS_CLK_S;

typedef struct
{
    FH_VICAP_SNS_IDX_E enSnsIdx;
    FH_BOOL bEn;
}VICAP_IOC_SNS_CLK_EN_S;

typedef struct
{
    FH_VICAP_SNS_IDX_E enSnsIdx;
    FH_VICAP_FRAME_SEQ_CFG_S* pstFrameSeqCfg;
}VICAP_IOC_FRAME_SEQ_GEN_CFG_S;

typedef struct
{
    FH_VICAP_SNS_IDX_E enSnsIdx;
    FH_VICAP_STATUS_S *stStatus;
}VICAP_IOC_GET_STATUS_S;

typedef struct
{
    FY_S32 (*dummy)(void);
} VICAP_EXPORT_FUNC_S;


/******************************************************************************
    VICAP driver ioctl definition list
 *****************************************************************************/
#define VICAP_IOC_MAGIC_CODE   'V'
#define IOC_VICAP_GET_BUF_SIZE            _IOWR(VICAP_IOC_MAGIC_CODE,0,FH_VICAP_BUF_SIZE_ATTR_S)
#define IOC_VICAP_INIT_VI_DEV             _IOWR(VICAP_IOC_MAGIC_CODE,1,VICAP_IOC_DEV_ATTR_S)
#define IOC_VICAP_SET_VI_ATTR             _IOWR(VICAP_IOC_MAGIC_CODE,2,VICAP_IOC_VI_ATTR_S)
#define IOC_VICAP_GET_VI_ATTR             _IOWR(VICAP_IOC_MAGIC_CODE,3,VICAP_IOC_VI_ATTR_S)
#define IOC_VICAP_SET_PIPE_CROP           _IOWR(VICAP_IOC_MAGIC_CODE,4,VICAP_IOC_PIPE_CROP_S)
#define IOC_VICAP_GET_PIPE_CROP           _IOWR(VICAP_IOC_MAGIC_CODE,5,VICAP_IOC_PIPE_CROP_S)
#define IOC_VICAP_REG_STITCH_DEV          _IOWR(VICAP_IOC_MAGIC_CODE,6,FH_VICAP_STITCH_ATTR_S)
#define IOC_VICAP_UNREG_STITCH_DEV        _IOWR(VICAP_IOC_MAGIC_CODE,7,VI_DEV)
#define IOC_VICAP_EXIT                    _IOWR(VICAP_IOC_MAGIC_CODE,8,VI_DEV)
#define IOC_VICAP_SET_SNS_CLK             _IOWR(VICAP_IOC_MAGIC_CODE,9,VICAP_IOC_SNS_CLK_S)
#define IOC_VICAP_SET_SNS_CLK_EN          _IOWR(VICAP_IOC_MAGIC_CODE,10,VICAP_IOC_SNS_CLK_EN_S)
#define IOC_VICAP_SET_FRAMESEQ_GENCFG     _IOWR(VICAP_IOC_MAGIC_CODE,11,FH_VICAP_FRAME_SEQ_CFG_S)
#define IOC_VICAP_GET_FRAMESEQ_GENCFG     _IOWR(VICAP_IOC_MAGIC_CODE,12,FH_VICAP_FRAME_SEQ_CFG_S)
#define IOC_VICAP_SET_FRAMESEQ_GENEN      _IOWR(VICAP_IOC_MAGIC_CODE,13,FH_UINT16)
#define IOC_VICAP_GET_FRAMESEQ_GENEN      _IOWR(VICAP_IOC_MAGIC_CODE,14,FH_UINT16)
#define IOC_VICAP_GET_STREAM              _IOWR(VICAP_IOC_MAGIC_CODE,15,FH_VICAP_STREAM_S)
#define IOC_VICAP_RELEASE_STREAM          _IOWR(VICAP_IOC_MAGIC_CODE,16,FH_VICAP_STREAM_S)
#define IOC_VICAP_SET_DVP_CFG             _IOWR(VICAP_IOC_MAGIC_CODE,17,FH_VICAP_DVP_CFG_S)
#define IOC_VICAP_GET_DVP_CFG             _IOWR(VICAP_IOC_MAGIC_CODE,18,FH_VICAP_DVP_CFG_S)
#define IOC_VICAP_SET_BT656_CFG           _IOWR(VICAP_IOC_MAGIC_CODE,19,FH_VICAP_BT656_CFG_S)
#define IOC_VICAP_GET_BT656_CFG           _IOWR(VICAP_IOC_MAGIC_CODE,20,FH_VICAP_BT656_CFG_S)
#define IOC_VICAP_GET_STATUS              _IOWR(VICAP_IOC_MAGIC_CODE,21,VICAP_IOC_GET_STATUS_S)
#define IOC_VICAP_SET_IR_REGS              _IOWR(VICAP_IOC_MAGIC_CODE,22,FH_VICAP_IR_REGS_S)
#define IOC_VICAP_GET_IR_REGS              _IOWR(VICAP_IOC_MAGIC_CODE,23,FH_VICAP_IR_REGS_S)
#define IOC_VICAP_SET_IR_OOC_ADDR          _IOWR(VICAP_IOC_MAGIC_CODE,24,FH_PHYADDR)
#define IOC_VICAP_GET_IR_OOC_ADDR          _IOWR(VICAP_IOC_MAGIC_CODE,25,FH_PHYADDR)
#define IOC_VICAP_SET_IR_CFG           _IOWR(VICAP_IOC_MAGIC_CODE,26,FH_VICAP_IR_CFG_S)
#define IOC_VICAP_GET_IR_CFG           _IOWR(VICAP_IOC_MAGIC_CODE,27,FH_VICAP_IR_CFG_S)
#define IOC_VICAP_MIPI_INIT           _IOWR(VICAP_IOC_MAGIC_CODE,28,struct mipi_conf)
#define IOC_VICAP_MIPI_DEINIT           _IOWR(VICAP_IOC_MAGIC_CODE,29,int)
#define IOC_VICAP_MIPI_VERSION           _IOWR(VICAP_IOC_MAGIC_CODE,30,int)
#define IOC_VICAP_SET_LOWPOWER_INFO    _IOWR(VICAP_IOC_MAGIC_CODE,31,FH_VICAP_LOWPOWER_INFO_S)
#define IOC_VICAP_SET_PEAK_LOAD           _IOWR(VICAP_IOC_MAGIC_CODE,32,FH_VICAP_PEAKLOAD_INFO_S)
#define IOC_VICAP_SET_BUF_CNT           _IOWR(VICAP_IOC_MAGIC_CODE,33,FH_VICAP_Buf_Cnt_S)
#define IOC_VICAP_SET_SUBSCRIBE_INFO    _IOWR(VICAP_IOC_MAGIC_CODE,34,FH_VICAP_Subscribe_Info_S)
#define IOC_VICAP_SET_PAUSE       _IOWR(VICAP_IOC_MAGIC_CODE,35,FH_VICAP_Pause_S)
#define IOC_VICAP_GET_FREEBUF       _IOWR(VICAP_IOC_MAGIC_CODE,36,FH_VICAP_FreeBuf_S)
#define IOC_VICAP_SET_OFFLINE       _IOWR(VICAP_IOC_MAGIC_CODE,37,FH_VICAP_OFFLINE_S)
#define IOC_VICAP_SET_PHY_LANE       _IOWR(VICAP_IOC_MAGIC_CODE,38,int)
#define IOC_VICAP_GET_PHY_LANE       _IOWR(VICAP_IOC_MAGIC_CODE,39,int)
#define IOC_VICAP_SET_BUF_SIZE       _IOWR(VICAP_IOC_MAGIC_CODE,40,FH_VICAP_Buf_Size_S)
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__FY_VICAP_DRV_H__
