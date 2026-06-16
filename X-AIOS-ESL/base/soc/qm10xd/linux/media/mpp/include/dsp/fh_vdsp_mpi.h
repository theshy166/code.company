#ifndef _FH_MPI_DSP_H_
#define _FH_MPI_DSP_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "types/type_def.h"
#include "fh_vdsp_mpipara.h"

/**
 * @brief         DSP上电。
 *
 * @param[in]     enDspId            DSP ID号
 *
 * @retval  0              成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,vdsp_errcode} 。
 *
 * @note
 *    - 在加载DSP镜像前，必须先调用FY_MPI_SVP_DSP_PowerOn接口使DSP上电。
 */
FH_SINT32 FH_MPI_SVP_DSP_PowerOn(SVP_DSP_ID_E enDspId);

FH_SINT32 FH_MPI_SVP_DSP_LoadBin(const FH_CHAR *pszBinFileName,SVP_DSP_MEM_TYPE_E enMemType);

/***** Enable DSP Core *****/
FH_SINT32 FH_MPI_SVP_DSP_EnableCore(SVP_DSP_ID_E enDspId);

/***** Disable DSP Core *****/
FH_SINT32 FH_MPI_SVP_DSP_DisableCore(SVP_DSP_ID_E enDspId);

/***** Dectect request *****/
FH_SINT32 FH_MPI_SVP_DSP_RPC(SVP_DSP_HANDLE *phHandle,SVP_DSP_SRC_IMAGE_S *pstSrcImg, SVP_DSP_DST_IMAGE_S *pstDstImg, SVP_DSP_ID_E enDspId);

FH_SINT32 FH_MPI_SVP_DSP_RPC_Timeout(SVP_DSP_HANDLE *phHandle,SVP_DSP_SRC_IMAGE_S *pstSrcImg, SVP_DSP_DST_IMAGE_S *pstDstImg,
    SVP_DSP_ID_E enDspId, FH_UINT32 u32TimeoutMs);

FH_SINT32 FH_MPI_SVP_DSP_PerspTrans(SVP_DSP_HANDLE *phHandle, SVP_DSP_SRC_IMAGE_S *pstSrcImg, SVP_DSP_DST_IMAGE_S *pstDstImg,
    FH_FLOAT *pMatrix, FH_UINT32 u32Size);

/*****query the status of a called function *****/
FH_SINT32 FH_MPI_SVP_DSP_Query(SVP_DSP_ID_E enDspId,SVP_DSP_HANDLE hHandle,FH_BOOL *pbFinish,FH_BOOL bBlock);


/*****Disable DSP Core*****/
FH_SINT32 FH_MPI_SVP_DSP_PowerOff(SVP_DSP_ID_E enDspId);

/*****Config DSP*****/
FH_SINT32 FH_MPI_SVP_DSP_SetConfig(SVP_DSP_HANDLE *phHandle, SVP_DSP_CFG_S *pstCfg, SVP_DSP_ID_E enDspId);

FH_SINT32 FH_MPI_SVP_DSP_SetIntrinsicMatrix(SVP_DSP_HANDLE *phHandle, FH_FLOAT *pMatrix);
FH_SINT32 FH_MPI_SVP_DSP_GetIntrinsicMatrix(SVP_DSP_HANDLE *phHandle, FH_FLOAT *pMatrix);
FH_SINT32 FH_MPI_SVP_DSP_SetExtrinsicMatrix(SVP_DSP_HANDLE *phHandle, FH_FLOAT *pMatrix);
FH_SINT32 FH_MPI_SVP_DSP_GetExtrinsicMatrix(SVP_DSP_HANDLE *phHandle, FH_FLOAT *pMatrix);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif/*_FH_MPI_DSP_H_*/

