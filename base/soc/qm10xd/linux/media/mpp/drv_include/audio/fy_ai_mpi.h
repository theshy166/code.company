#ifndef __FY_AI_MPI_H__
#define __FY_AI_MPI_H__

#include "fy_comm_aio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * @brief         配置音频输入设备的属性
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     cmd                音频输入命令号，默认传 IOC_CMD_BUTT
 * @param[in]     pstAttr            音频输入设备属性结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 必须在执行 FY_AI_Enable 前配置。
 *
 */
FY_S32 FY_AI_SetPubAttr(AUDIO_DEV AiDevId,FY_U32 cmd, FY_VOID *pstAttr);

/**
 * @brief         获取音频输入设备的属性
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入通道号，参考 AUDIO_DEVID
 * @param[in]     cmd                音频输入命令号，默认传 IOC_CMD_BUTT
 * @param[out]    pstAttr            音频输入设备属性结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 */
FY_S32 FY_AI_GetPubAttr(AUDIO_DEV AiDevId,FY_S32 cmd, FY_VOID *pstAttr);

/**
 * @brief         使能音频输入设备
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 */
FY_S32 FY_AI_Enable(AUDIO_DEV AiDevId);

/**
 * @brief         禁用音频输入设备
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入通道号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 */
FY_S32 FY_AI_Disable(AUDIO_DEV AiDevId);

/**
 * @brief         获取音频输入设备文件句柄
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 */
FY_S32 FY_AI_GetDevFd(AUDIO_DEV AiDevId);

/**
 * @brief         配置音频输入设备声道模式
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     enTrackMode        声道属性参数，参考 AUDIO_TRACK_MODE_E
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 仅支持AUDIO_TRACK_LEFT/AUDIO_TRACK_RIGHT/AUDIO_TRACK_MIX。
 *
 */
FY_S32 FY_AI_SetTrackMode(AUDIO_DEV AiDevId, AUDIO_TRACK_MODE_E enTrackMode);

/**
 * @brief         获取音频输入设备声道模式
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[out]    penTrackMode       声道属性参数指针，参考 AUDIO_TRACK_MODE_E
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 仅支持 AUDIO_TRACK_LEFT / AUDIO_TRACK_RIGHT / AUDIO_TRACK_MIX。
 *
 */
FY_S32 FY_AI_GetTrackMode(AUDIO_DEV AiDevId, AUDIO_TRACK_MODE_E *penTrackMode);

/**
 * @brief         使能音频输入设备通道
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效。
 *
 */

FY_S32 FY_AI_Chnnum_Array(AUDIO_DEV AiDevId, FY_S16* Array);

/**
 * @brief         配置音频使能通道
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效。
 *
 */

FY_S32 FY_AI_EnableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);

/**
 * @brief         禁用音频输入设备通道
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效。
 *
 */
FY_S32 FY_AI_DisableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);

/**
 * @brief         暂停音频输入设备通道
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效。
 *
 */
FY_S32 FY_AI_PauseChn(FY_U32 AiDevId, AI_CHN AiChn);

/**
 * @brief         恢复音频输入设备通道
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AI_ResumeChn(FY_U32 AiDevId, AI_CHN AiChn);

/**
 * @brief         获取音频输入设备数据信息
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 * @param[out]    pstFrm             VQE处理前音频数据指针结构体
 * @param[out]    pstAecFrm          VQE处理后音频，仅支持 AUDIO_ALGO_MPP_MIC1
 * @param[in]     s32MilliSec        超时时间，-1 阻塞/ 0 返回
 *
 * @retval  >=0             成功
 * @retval  "<0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AI_GetChnFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm, void *pstAecFrm, FY_S32 s32MilliSec);

/**
 * @brief         获取音频输入设备数据信息
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 * @param[out]    pstFrm             VQE处理前音频数据指针结构体
 * @param[out]    pstAecFrm          VQE处理后音频，仅支持 AUDIO_ALGO_MPP_MIC1
 * @param[in]     s32MilliSec        超时时间，-1 阻塞/ 0 返回
 *
 * @retval  >=0             成功
 * @retval  "<0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AI_GetChnFrameCallback(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm, FY_S32 s32MilliSec,
											frame_callback callback, callback_param* param);

/**
 * @brief         释放音频输入设备数据信息
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 * @param[in]     pstFrm             VQE处理前音频数据
 * @param[in]     pstAecFrm          VQE处理后音频，仅支持 AUDIO_ALGO_MPP_MIC1
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_Enable 配置后生效。
 *
 */
FY_S32 FY_AI_ReleaseChnFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm, void *pstAecFrm);

/**
 * @brief         设置音频输入通道参数信息
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 * @param[in]     pFrmInfo           通道参数结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_EnableChn 配置前调用。
 *
 */
FY_S32 FY_AI_SetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_CHN_PARAM_S* pFrmInfo);

/**
 * @brief         获取音频输入通道参数信息
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     AiChn              音频输入设备通道号，范围 0~chn_num
 * @param[out]    pFrmInfo           通道参数结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 * @note
 *    - 在 FY_AI_EnableChn 配置后生效。
 *
 */
FY_S32 FY_AI_GetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_CHN_PARAM_S* pFrmInfo);

/**
 * @brief         设置音频输入设备音量
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[in]     VOLUME_PARAM_S     音量参数结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 *
 */
FY_S32 FY_AI_SetVolume(AUDIO_DEV AiDevId, VOLUME_PARAM_S* ptr);

/**
 * @brief         获取音频输入设备音量
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 * @param[out]     VOLUME_PARAM_S     音量参数结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 *
 */
FY_S32 FY_AI_GetVolume(AUDIO_DEV AiDevId, VOLUME_PARAM_S* ptr);

/**
 * @brief         AI进入低功耗状态
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 *
 */
FY_S32 FY_AI_Suspend(AUDIO_DEV AiDevId);

/**
 * @brief         AI从低功耗状态恢复
 *
 * @param[in]     AiDevId            音频输入设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 *
 */
FY_S32 FY_AI_Resume(AUDIO_DEV AiDevId);


FY_VOID FY_AI_SYS_Init(FY_VOID);
FY_VOID FY_AI_SYS_DeInit(FY_VOID);


FY_S32 FY_AI_Mix2Ch(AUDIO_DEV AiDevId,void* src_ptr, void* dst_ptr);
FY_S32 FY_AI_Ch2Mix(AUDIO_DEV AiDevId, AI_CHN AiChn, void* src_ptr, void* dst_ptr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_AI_H__ */
