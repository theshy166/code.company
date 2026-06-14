#ifndef __FY_VGS_DRV_H__
#define __FY_VGS_DRV_H__


/******************************************************************************
    Include other header files
 *****************************************************************************/
#include "fh_vgs_mpipara.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/******************************************************************************
    Macros, Enums, Structures definition list
 *****************************************************************************/
#define VPPU_CHN_MODE_AUTO      VGS_PTH_MODE_AUTO
#define VPPU_CHN_MODE_USER      VGS_PTH_MODE_USER
#define VPPU_CROP_ABS_COOR      VGS_CROP_ABS_COOR
#define VPPU_CROP_RATIO_COOR    VGS_CROP_RATIO_COOR
#define VPPU_MAX_CHN_NUM        VGS_MAX_CHN_NUM
#define VPPU_MAX_PTH_NUM        VGS_MAX_PTH_NUM


typedef VGS_CHN                 VPPU_CHN;
typedef VGS_PTH                 VPPU_PTH;
typedef VGS_PTH_MODE_E          VPPU_PTH_MODE_E;
typedef VGS_CROP_PARA_S         VPPU_CROP_INFO_S;
typedef VGS_CROP_COORDINATE_E   VPPU_CROP_COORDINATE_E;


typedef struct
{
    FY_S32  handle;
    FY_S32  reserved;
    VGS_TASK_ATTR_S stTaskAttr;
}VGS_TASK_ATTR_S_T;

/*
typedef struct
{
    FY_S32  handle;
    FY_BOOL bFrmToSur;
    VIDEO_FRAME_S stFrame;
    TDE2_SURFACE_S stSurface;
}VGS_FMT_CONVERT_S;
*/

typedef struct
{
    FY_S32  handle;
    VIDEO_FRAME_S stFrmIn;
    VIDEO_FRAME_S stFrmOut;
    FY_S32 s3ChangeType;
}VGS_FMT_CHANGE_S;

typedef struct
{
    FY_S32 handle;
    FY_S32 reserved1;
    VGS_TASK_ATTR_S stTaskAttr;
    VGS_LINE_S astVgsDrawLine[10];
    FY_U32 count;
    FY_U32 reserved2;
}ADD_DRAW_LINE_S;

typedef struct
{
    FY_S32 handle;
    FY_S32 reserved1;
    VGS_TASK_ATTR_S stTaskAttr;
    VGS_COVER_S astVgsAddCover[10];
    FY_U32 count;
    FY_U32 reserved2;
}ADD_COVER_S;

typedef struct
{
    FY_S32 handle;
    FY_S32 reserved1;
    VGS_TASK_ATTR_S stTaskAttr;
    VGS_OSD_S astVgsAddOsd[10];
    FY_U32 count;
    FY_U32 reserved2;
}ADD_OSD_S;

typedef struct
{
    FY_S32 handle;
    VIDEO_FRAME_S stImgA;
    VIDEO_FRAME_S stImgB;
    VIDEO_FRAME_S stImgOut;
    VGS_OPT_S stOpt;
}VGS_TASK_BLEND_S_T;

typedef struct
{
    VPPU_CHN                vppuChn;
    FY_BOOL                 bPlayMode;
    VIDEO_FRAME_INFO_S      stImgSd;        	/* send picture */
    FY_U32                  u32TimeOut;
}VGS_SEND_FRAME_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    VGS_CHN_STS_S           stChnSts;
}VGS_CHN_STAT_S;

typedef struct
{
	VGS_CHN                 vppuChn;
	FH_UINT32               size;
	FH_PHYADDR              addr;
	FH_UINT32               timeout;
	FH_UINT32               isidle;
}VGS_ADDR_STAT_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    VPPU_PTH                vppuPth;
    VGS_SCALE_MODE_E        enScaleMode;
}VGS_PTH_SC_MODE_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    VPPU_PTH                vppuPth;
    VGS_PTH_PARA_S          vppuMode;
}VGS_PTH_MODE_S;

typedef struct
{
    FY_BOOL                 bAsync;
    VPPU_CHN                vppuChn;
    FY_U32                  bufDepth;
}VGS_SYNC_MODE_S;

typedef struct
{
    FY_S32                  u32TimeOut;
    VPPU_CHN                vppuChn;
    VPPU_PTH                vppuPth;
    VIDEO_FRAME_INFO_S      stPthFrm;
}VGS_PTH_FRM_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    VPPU_CROP_INFO_S        stVppuCrop;
}VGS_CROP_INFO_S;

typedef struct
{
    FY_S32 s32Xpos;
    FY_S32 s32Ypos;
    FY_U32 u32Width;
    FY_U32 u32Height;
    FY_U32 u32FillData;
} VIDEO_FILL_RECT_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    FY_U32                  u32Index;
    VIDEO_FRAME_INFO_S      stChnFrm;
}VGS_CHN_FRM_S;

typedef struct
{
    FY_S32 handle;
    VIDEO_FRAME_INFO_S stFrameInfo;
    VIDEO_FILL_RECT_S stFillRect;
    FY_U32 reserved2;
}ADD_COLOR_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    VPPU_PTH                vppuPth;
    VGS_ROTATE_E            enRotate;
}VGS_ROTATE_INFO_S;

typedef struct
{
    FY_S32                  handle;
    VIDEO_FRAME_INFO_S      stImgIn;        /* input picture */
    VIDEO_FRAME_INFO_S      stImgOut0;      /* output path0 picture */
    VIDEO_FRAME_INFO_S      stImgOut1;      /* output path1 picture */
    VIDEO_FRAME_INFO_S      stImgOut2;      /* output path2 picture */
}VGS_MULTI_PATH_S;

typedef struct
{
    FY_U32                  u32TimeOut;
    FY_S32                  NULL_Flag;      /* path1 NULL:0x02, path2 NULL:0x04 */
    VPPU_CHN                vppuChn;
    VIDEO_FRAME_INFO_S      stFrmPth1;
    VIDEO_FRAME_INFO_S      stFrmPth2;
}VGS_IMG_GET_S;

typedef struct
{
    VPPU_CHN                vppuChn;
    VPPU_PTH                vppuPth;
    FY_U32                  pthDepth;
}VGS_PTH_DEPTH_S;

typedef struct
{
    FY_S32 (*pfnVgsBeginJob)(FY_S32 *pJobId);
    FY_S32 (*pfnVgsScale)(VGS_HANDLE jobId, VGS_TASK_ATTR_S *pstTask);
    FY_S32 (*pfnVgsCopy)(VGS_HANDLE jobId, VGS_TASK_ATTR_S *pstTask);
    FY_S32 (*pfnVgsFillColor)(VGS_HANDLE jobId, VIDEO_FRAME_INFO_S *pstVFrm, VIDEO_FILL_RECT_S *pstRect);
    FY_S32 (*pfnVgsCombine)(VGS_HANDLE jobId, VIDEO_FRAME_S *pstVFrmIn, RECT_S *pstInRect, VIDEO_FRAME_S *pstVFrmOut, RECT_S *pstOutRect);
    FY_S32 (*pfnVgsEndJob)(FY_S32 certain_job);
    FY_S32 (*pfnVgsCombineRe)(VGS_HANDLE jobId, VIDEO_FRAME_S *pstVFrmIn, RECT_S *pstInRect, VIDEO_FRAME_S *pstVFrmOut, RECT_S *pstOutRect);
    FY_S32 (*pfnVgsCancelJob)(FY_S32 certain_job);
    FY_S32 (*pfnVgsRelsBuf)(VPPU_CHN vppuChn);
    FY_S32 (*pfnVgsWakeupNotify)(MOD_ID_E dst_enModId, FY_S32 dst_s32DevId, FY_S32 dst_s32ChnId);
} VGS_EXPORT_FUNC_S;


/******************************************************************************
    VGS driver ioctl definition list
 *****************************************************************************/
#define IOC_VGS_BEGIN_JOB               _IOWR('J',0,FY_S32)
#define IOC_VGS_CANCEL_JOB              _IOW('J',1,FY_S32)
#define IOC_VGS_END_JOB                 _IOW('J',2,FY_S32)
#define IOC_VGS_ADD_SCALE_TASK          _IOW('J',3,VGS_TASK_ATTR_S_T)
#define IOC_VGS_ADD_FILL_COLOR          _IOW('J',4,ADD_COLOR_S)
#define IOC_VGS_ADD_DRAW_LINE_TASK      _IOW('J',5,ADD_DRAW_LINE_S)
#define IOC_VGS_ADD_COVER_TASK          _IOW('J',6,ADD_COVER_S)
#define IOC_VGS_ADD_OSD_TASK            _IOW('J',7,ADD_OSD_S)
#define IOC_VGS_ADD_DECOMPRESS_TASK     _IOW('J',8,VGS_TASK_ATTR_S_T)
#define IOC_VGS_ADD_COMBINE_TASK        _IOW('J',9,VGS_COMBINE_S)
#define IOC_VGS_ADD_COPY_TASK           _IOW('J',10,VGS_TASK_ATTR_S_T)
#define IOC_VGS_ADD_BLEND_TASK          _IOW('J',11,VGS_TASK_BLEND_S_T)
#define IOC_VGS_ADD_CHANGE_TASK         _IOW('J',12,VGS_FMT_CHANGE_S)
#define IOC_VGS_ADD_MULTIPTH_TASK       _IOW('J',13,VGS_MULTI_PATH_S)
#define IOC_VGS_ADD_ROTATE_TASK         _IOW('J',14,VGS_TASK_ATTR_S_T)

#define IOC_VGS_CREATE_CHN              _IOWR('J',15,VPPU_CHN)
#define IOC_VGS_DESTORY_CHN             _IOWR('J',16,VPPU_CHN)
#define IOC_VGS_SET_CROP_INFO           _IOWR('J',17,VGS_CROP_INFO_S)
#define IOC_VGS_GET_CROP_INFO           _IOWR('J',18,VGS_CROP_INFO_S)
#define IOC_VGS_SEND_FRAME              _IOWR('J',19,VGS_SEND_FRAME_S)
#define IOC_VGS_QUERY_FRAME             _IOWR('J',20,VGS_SEND_FRAME_S)
#define IOC_VGS_SET_PTH_MODE            _IOWR('J',21,VGS_PTH_MODE_S)
#define IOC_VGS_GET_PTH_MODE            _IOWR('J',22,VGS_PTH_MODE_S)
#define IOC_VGS_GET_IMAGE               _IOWR('J',23,VGS_IMG_GET_S)
#define IOC_VGS_GET_PATH_FRAME          _IOWR('J',25,VGS_PTH_FRM_S)
#define IOC_VGS_RELEASE_PATH_FRAME      _IOWR('J',26,VGS_PTH_FRM_S)
#define IOC_VGS_SET_CHN_ASYNC           _IOWR('J',27,VGS_SYNC_MODE_S)
#define IOC_VGS_SET_ROTAT_INFO          _IOWR('J',29,VGS_ROTATE_INFO_S)
#define IOC_VGS_GET_ROTAT_INFO          _IOWR('J',30,VGS_ROTATE_INFO_S)
#define IOC_VGS_ASYNC_RELEASE           _IOWR('J',31,VGS_CHN)
#define IOC_VGS_ENABLE_BACKUP_FRAME     _IOWR('J',33,VGS_CHN)
#define IOC_VGS_DISABLE_BACKUP_FRAME    _IOWR('J',34,VGS_CHN)
#define IOC_VGS_REFRESH_CHN             _IOWR('J',35,VGS_CHN)
#define IOC_VGS_GET_CHN_FRAME           _IOWR('J',36,VGS_CHN_FRM_S)

#define IOC_VGS_CHECK_ADDR              _IOWR('J',42,VGS_ADDR_STAT_S)
#define IOC_VGS_SET_PTH_SC              _IOWR('J',44,VGS_PTH_SC_MODE_S)
#define IOC_VGS_SET_TSK_SC              _IOWR('J',45,VGS_SCALE_MODE_E)
#define IOC_VGS_FREEZE_VIDEO            _IOWR('J',46,VGS_CHN)
#define IOC_VGS_UNFREEZE_VIDEO          _IOWR('J',47,VGS_CHN)
#define IOC_VGS_SET_PTH_DEPTH           _IOWR('J',48,VGS_PTH_DEPTH_S)
#define IOC_VGS_GET_PTH_DEPTH           _IOWR('J',49,VGS_PTH_DEPTH_S)
#define IOC_VGS_GET_PATH_FRAME_EX       _IOWR('J',50,VGS_PTH_FRM_S)

#define IOC_VGS_SET_MODULE_PARAM        _IOWR('J',51,VGS_MODULE_PARAM_S)

#define IOC_G2D_SET_FRAMELOSS_ENABLE    _IOW('J',60,FH_BOOL)
#define IOC_G2D_SUSPEND                 _IO('J',61)
#define IOC_G2D_RESUME                  _IO('J',62)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__FY_VGS_DRV_H__
