#ifndef __IOE_DRV_IOC_H__
#define __IOE_DRV_IOC_H__

#include "fh_type.h"
#include "fh_video.h"
#include "fh_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/******************************************************************************
    IOE driver ioctl definition list
 *****************************************************************************/

#define IOC_TYPE_IOE   'S'
#define IOC_IOE_OPERATE                     _IOW(IOC_TYPE_IOE,1,IOE_PARAM_T)
#define IOC_IOE_ADD_CMD                     _IOW(IOC_TYPE_IOE,3,IOE_PARAM_T)
#define IOC_IOE_START                       _IOW(IOC_TYPE_IOE,4,FY_U32)

typedef struct ioe_param_t{
	FY_S32			img_y_only;                /* 0: C-only; 1:Y-only */
	FY_S32			img_yuv420;                /* 0: yuv422,1:yuv420   */
	FY_S32			img_uv_order;              /* 0: UV;  1: VU */
	FY_U32			md_y_threshold;            /*Threshold threshold of motion detection image difference (Y threshold)*/
	FY_U32			md_u_threshold;            /*Threshold threshold of motion detection image difference (U threshold)*/
	FY_U32			md_v_threshold;            /*Threshold threshold of motion detection image difference (V threshold)*/
	FY_U32			img_src_size_w;
	FY_U32			img_src_size_h;
	FY_U32			img_src_pitch_w;
	FY_U32			img0_base_addr_y;
	FY_U32			img0_base_addr_c;
	FY_U32			img1_base_addr_y;
	FY_U32			img1_base_addr_c;
	FY_U32			img_des_size_w;
	FY_U32			img_des_size_h;
	FY_U32			img_des_base_addr_y;
	FY_U32			img_des_base_addr_c;
	FY_U32			pixel_num_threshold;      /*Motion detection sub-block noise filtering threshold*/
	FY_U32			timeout;
	FY_S32			id;
	FY_U32			is_first;
}IOE_PARAM_T;


#define FH_ID_IOE 52

#define FH_ERR_IOE_ILLEGAL_PARAM     FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)	//0xa0348003
#define FH_ERR_IOE_NULL_PTR          FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)			//0xa0348006
#define FH_ERR_IOE_SYS_NOTREADY      FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)		//0xa0348010
#define FH_ERR_IOE_NOT_SUPPORT       FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)		//0xa0348008
#define FH_ERR_IOE_TIMEOUT           FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, EN_ERR_TIMEOUT)			//0xa034800a
#define FH_ERR_IOE_MDERR             FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, 19)						//0xa0348013
#define FH_ERR_IOE_OVERFLOW          FH_DEF_ERR(FH_ID_IOE, EN_ERR_LEVEL_ERROR, 20)						//0xa0348014



#ifdef __cplusplus
#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __IOE_DRV_IOC_H__ */


