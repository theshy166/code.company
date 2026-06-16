#ifndef __TDE_DRV_IOC_H__
#define __TDE_DRV_IOC_H__

#include "fh_tde_mpipara.h"
#include "vgs_drv_ioc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*Define attributes of tde bitblit*/
typedef struct fyTDS_BIT_BLIT_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stBackGround;
	TDE2_RECT_S stBackGroundRect;
	TDE2_SURFACE_S stForeGround;
	TDE2_RECT_S stForeGroundRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	TDE2_OPT_S stOpt;
	int NULL_Flag;
}TDS_BIT_BLIT_S;

/*Define attributes of tde solid*/
typedef struct fyTDS_SOLID_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stForeGround;
	TDE2_RECT_S stForeGroundRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	TDE2_FILLCOLOR_S stFillColor;
	TDE2_OPT_S stOpt;
	int NULL_Flag;
}TDS_SOLID_S;


/*Define attributes of tde quick copy*/
typedef struct fyTDS_QUICK_COPY_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stSrc;
	TDE2_RECT_S stSrcRect;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
}TDS_QUICK_COPY_S;

/*Define attributes of tde quick fill*/
typedef struct fyTDS_QUICK_FILL_S
{
	TDE_HANDLE s32Handle;
	TDE2_SURFACE_S stDst;
	TDE2_RECT_S stDstRect;
	FY_U32 u32FillData;
}TDS_QUICK_FILL_S;

/*Define attributes of tde end job*/
typedef struct fyTDS_END_JOB_S
{
	TDE_HANDLE s32Handle;
	FY_BOOL bSync;
	FY_BOOL bBlock;
	FY_U32 u32TimeOut;
}TDS_END_JOB_S;

typedef struct fyTDE_EXPORT_FUNC_S
{
    FY_S32 (*pfnTdeBeginJob)(FY_S32 *pJobId);
    FY_S32 (*pfnTdeCompress)(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstSrc, TDE2_SURFACE_S *pstDst);
    FY_S32 (*pfnTdeEndJob)(TDS_END_JOB_S *pstEndJob);
    FY_S32 (*pfnTdeCancelJob)(FY_S32 certain_job);
    FY_S32 (*pfnTdeReverse)(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstSrc, TDE2_SURFACE_S *pstDst);
    FY_S32 (*pfnTdeYCCopy)(TDE_HANDLE jobId, VIDEO_FRAME_S *pVideoFrmIn, VIDEO_FRAME_S *pVideoFrmOut, FY_BOOL bYData);
    FY_S32 (*pfnTdeYCFill)(TDE_HANDLE jobId, VIDEO_FRAME_INFO_S *pstVFrm, VIDEO_FILL_RECT_S *pstRect, FY_BOOL bYData);
} TDE_EXPORT_FUNC_S;


/******************************************************************************
    TDE driver ioctl definition list
 *****************************************************************************/

#define IOC_TYPE_TDE   't'
#define IOC_TDE_BEGIN_JOB                   _IOR(IOC_TYPE_TDE,1,int)
#define IOC_TDE_BIT_BLIT                    _IOW(IOC_TYPE_TDE,2,TDS_BIT_BLIT_S)
#define IOC_TDE_SOLID_DRAW                  _IOW(IOC_TYPE_TDE,3,TDS_SOLID_S)
#define IOC_TDE_QUICK_COPY                  _IOW(IOC_TYPE_TDE,4,TDS_QUICK_COPY_S)
#define IOC_TDE_QUICK_RESIZE                _IOW(IOC_TYPE_TDE,5,TDS_QUICK_COPY_S)
#define IOC_TDE_QUICK_FILL                  _IOW(IOC_TYPE_TDE,6,TDS_QUICK_FILL_S)
#define IOC_TDE_QUICK_DEFLICKER             _IOW(IOC_TYPE_TDE,7,TDS_QUICK_COPY_S)
#define IOC_TDE_END_JOB                     _IOW(IOC_TYPE_TDE,9,TDS_END_JOB_S)
#define IOC_TDE_WAIT_FOR_DONE               _IOW(IOC_TYPE_TDE,10,FY_S32)
#define IOC_TDE_CANCEL_JOB                  _IOW(IOC_TYPE_TDE,11,FY_S32)
#define IOC_TDE_WAIT_ALL_DONE               _IO(IOC_TYPE_TDE,14)
#define IOC_TDE_RESET                       _IO(IOC_TYPE_TDE,15)
#define IOC_TDE_SET_DEFLICKER_LEVEL         _IOW(IOC_TYPE_TDE,17,TDE_DEFLICKER_LEVEL_E)
#define IOC_TDE_GET_DEFLICKER_LEVEL         _IOR(IOC_TYPE_TDE,18,TDE_DEFLICKER_LEVEL_E)
#define IOC_TDE_SET_ALPHA_THRESHOLD_VALUE   _IOW(IOC_TYPE_TDE,19,FY_U8)
#define IOC_TDE_GET_ALPHA_THRESHOLD_VALUE   _IOR(IOC_TYPE_TDE,20,FY_U8)
#define IOC_TDE_SET_ALPHA_THRESHOLD_STATE   _IOW(IOC_TYPE_TDE,21,FY_BOOL)
#define IOC_TDE_GET_ALPHA_THRESHOLD_STATE   _IOW(IOC_TYPE_TDE,22,FY_BOOL)
#define IOC_TDE_DO_COMPRESS                 _IOW(IOC_TYPE_TDE,25,TDS_QUICK_COPY_S)
#define IOC_TDE_DE_COMPRESS                 _IOW(IOC_TYPE_TDE,26,TDS_QUICK_COPY_S)

#define IOC_TDE_SET_SC_MODE                 _IOW(IOC_TYPE_TDE,27,TDE_RESIZE_MODE_E)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TDE_DRV_IOC_H__ */


